//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: Weapon data file parsing, shared by game & client dlls.
//
// $NoKeywords: $
//=============================================================================//
#include "cbase.h"
#include <KeyValues.h>
#include <tier0/mem.h>
#include "filesystem.h"
#include "utldict.h"
#include "ammodef.h"
#include "Mod/SharedModCvars.h"

#include "playerclass_info_parse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static CUtlDict< FilePlayerClassInfo_t*, unsigned short > m_PlayerClassInfoDatabase;

#define MAX_PLAYERCLASSES	32
#define MAX_DIGITS 2

#ifdef _DEBUG

// used to track whether or not two player classes have been mistakenly assigned the same slot
bool g_bUsedPlayerClassSlots[MAX_PLAYERCLASSES] = { 0 };

#endif


#ifdef DEBUG

void CC_ReloadPlayerClasses_f (void)
{
	ResetFilePlayerClassInfoDatabase();
}

static ConCommand dod_reloadplayerclasses("dod_reloadplayerclasses", CC_ReloadPlayerClasses_f, "Reset player class info cache" );

#endif

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *name - 
// Output : FilePlayerClassInfo_t
//-----------------------------------------------------------------------------
static PLAYERCLASS_FILE_INFO_HANDLE FindPlayerClassInfoSlot( const char *name )
{
	// Complain about duplicately defined metaclass names...
	unsigned short lookup = m_PlayerClassInfoDatabase.Find( name );
	if ( lookup != m_PlayerClassInfoDatabase.InvalidIndex() )
	{
		return lookup;
	}

	FilePlayerClassInfo_t *insert = new FilePlayerClassInfo_t();

	lookup = m_PlayerClassInfoDatabase.Insert( name, insert );
	Assert( lookup != m_PlayerClassInfoDatabase.InvalidIndex() );
	return lookup;
}

// Find a class slot, assuming the weapon's data has already been loaded.
PLAYERCLASS_FILE_INFO_HANDLE LookupPlayerClassInfoSlot( const char *name )
{
	return m_PlayerClassInfoDatabase.Find( name );
}



// FIXME, handle differently?
static FilePlayerClassInfo_t gNullPlayerClassInfo;


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : handle - 
// Output : FilePlayerClassInfo_t
//-----------------------------------------------------------------------------
CPlayerClassInfo *GetFilePlayerClassInfoFromHandle( PLAYERCLASS_FILE_INFO_HANDLE handle )
{
	if ( handle == GetInvalidPlayerClassInfoHandle() )
	{
		Assert( !"bad index into playerclass info UtlDict" );
		return &gNullPlayerClassInfo;
	}

	return m_PlayerClassInfoDatabase[ handle ];
}

//-----------------------------------------------------------------------------
// Purpose: 
// Output : PLAYERCLASS_FILE_INFO_HANDLE
//-----------------------------------------------------------------------------
PLAYERCLASS_FILE_INFO_HANDLE GetInvalidPlayerClassInfoHandle( void )
{
	return (PLAYERCLASS_FILE_INFO_HANDLE)m_PlayerClassInfoDatabase.InvalidIndex();
}

void ResetFilePlayerClassInfoDatabase( void )
{
	m_PlayerClassInfoDatabase.PurgeAndDeleteElements();

#ifdef _DEBUG
	memset(g_bUsedPlayerClassSlots, 0, sizeof(g_bUsedPlayerClassSlots));
#endif
}

#ifndef _XBOX
KeyValues* ReadEncryptedKVPlayerClassFile( IFileSystem *filesystem, const char *szFilenameWithoutExtension, const unsigned char *pICEKey )
{
	Assert( strchr( szFilenameWithoutExtension, '.' ) == NULL );
	char szFullName[512];

	// Open the weapon data file, and abort if we can't
	KeyValues *pKV = new KeyValues( "PlayerClassDatafile" );

	Q_snprintf(szFullName,sizeof(szFullName), "%s.txt", szFilenameWithoutExtension);

	if ( !pKV->LoadFromFile( filesystem, szFullName, "GAME" ) ) // try to load the normal .txt file first
	{
		if ( pICEKey )
		{
			Q_snprintf(szFullName,sizeof(szFullName), "%s.ctx", szFilenameWithoutExtension); // fall back to the .ctx file

			FileHandle_t f = filesystem->Open( szFullName, "rb", "GAME");

			if (!f)
			{
				pKV->deleteThis();
				return NULL;
			}
			// load file into a null-terminated buffer
			int fileSize = filesystem->Size(f);
			char *buffer = (char*)MemAllocScratch(fileSize + 1);
		
			Assert(buffer);
		
			filesystem->Read(buffer, fileSize, f); // read into local buffer
			buffer[fileSize] = 0; // null terminate file as EOF
			filesystem->Close( f );	// close file after reading

			UTIL_DecodeICE( (unsigned char*)buffer, fileSize, pICEKey );

			bool retOK = pKV->LoadFromBuffer( szFullName, buffer, filesystem );

			MemFreeScratch();

			if ( !retOK )
			{
				pKV->deleteThis();
				return NULL;
			}
		}
		else
		{
			pKV->deleteThis();
			return NULL;
		}
	}

	return pKV;
}
#endif

//-----------------------------------------------------------------------------
// Purpose: Read data on weapon from script file
// Output:  true  - if data2 successfully read
//			false - if data load fails
//-----------------------------------------------------------------------------
bool ReadPlayerClassDataFromFileForSlot( IFileSystem* filesystem, const char *szPlayerClassName, PLAYERCLASS_FILE_INFO_HANDLE *phandle, const unsigned char *pICEKey )
{
	if ( !phandle )
	{
		Assert( 0 );
		return false;
	}

	*phandle = FindPlayerClassInfoSlot( szPlayerClassName );
	FilePlayerClassInfo_t *pFileInfo = GetFilePlayerClassInfoFromHandle( *phandle );
	Assert( pFileInfo );

	if ( pFileInfo->m_bParsedScript )
		return true;

	char sz[128];
	Q_snprintf( sz, sizeof( sz ), "scripts/pc_%s", szPlayerClassName );
	KeyValues *pKV = ReadEncryptedKVFile( filesystem, sz, pICEKey );
	if ( !pKV )
		return false;

	pFileInfo->Parse( pKV, szPlayerClassName );

	pKV->deleteThis();

	return true;
}


//-----------------------------------------------------------------------------
// FilePlayerClassInfo_t implementation.
//-----------------------------------------------------------------------------

FilePlayerClassInfo_t::FilePlayerClassInfo_t()
{
	m_bParsedScript = false;

	m_szPlayerClassName[0] = 0;
	m_szPrintName[0] = 0;
	m_szPlayerModel[0] = 0;
	m_szSelectCmd[0] = 0;

	m_iTeam				= TEAM_UNASSIGNED;

	m_szLimitCvar[0]	= '\0';
	m_flRunSpeed		= 200;
	m_flSprintSpeed		= 340;
	m_flProneSpeed		= 160;

	m_iHealth			= 0;
	m_iArmor			= 0;
}

void FilePlayerClassInfo_t::Parse( KeyValues *pKeyValuesData, const char *szPlayerClassName )
{
	// Okay, we tried at least once to look this up...
	m_bParsedScript = true;

	// Classname
	Q_strncpy( m_szPlayerClassName, szPlayerClassName, MAX_PLAYERCLASS_NAME_LENGTH );

	// Printable name
	Q_strncpy( m_szPrintName, pKeyValuesData->GetString( "printname", "!! Missing printname on Player Class" ), MAX_PLAYERCLASS_NAME_LENGTH );

	// Player Model
	Q_strncpy( m_szPlayerModel, pKeyValuesData->GetString( "playermodel", "!! Missing playermodel on Player Class" ), MAX_PLAYERCLASS_NAME_LENGTH );

	// Select command
	Q_strncpy( m_szSelectCmd, pKeyValuesData->GetString( "selectcmd", "!! Missing selectcmd on Player Class" ), 32 );

	m_iTeam = TEAM_UNASSIGNED;

	const char *pTeam = pKeyValuesData->GetString( "team", NULL );
	if ( pTeam )
	{
		if ( Q_stricmp( pTeam, "BLUE" ) == 0 )
		{
			m_iTeam = SDK_TEAM_BLUE;
		}
		else if ( Q_stricmp( pTeam, "RED" ) == 0 )
		{
			m_iTeam = SDK_TEAM_RED;
		}
		else
		{
			Assert( false );
		}
	}
	else
	{
		Assert( false );
	}

	const int keyLength = sizeof( "weapon_" ) + MAX_DIGITS;
	char keyName[ keyLength ];
	int ammoKeyLength = sizeof( "weapon__ammo" ) + MAX_DIGITS;
	char ammoKeyName[ ammoKeyLength ];
	for (int i = 1; i <= WEAPON_MAX; i++)
	{
		Q_snprintf( keyName, keyLength, "weapon_%d", i );
		
		const char *pszWeapon = pKeyValuesData->GetString( keyName, NULL );
		if( !pszWeapon )
		{
			Warning( "weapon_%s: %s requested by class %s not found\n", keyName, pszWeapon, m_szPlayerClassName );
			break;
		}

		int weaponId = AliasToWeaponID( pszWeapon );
		m_WeaponVector.AddToTail( weaponId );

		Q_snprintf( ammoKeyName, ammoKeyLength, "weapon_%d_ammo", i );
		int ammoCount = pKeyValuesData->GetInt( ammoKeyName, 0 );
		m_AmmoVector.AddToTail( ammoCount );

		m_iWeaponCount = i;
	}

	Q_strncpy( m_szLimitCvar, pKeyValuesData->GetString( "limitcvar", "!! Missing limit cvar on Player Class" ), sizeof(m_szLimitCvar) );

	Assert( Q_strlen( m_szLimitCvar ) > 0 && "Every class must specify a limitcvar" );

	// HUD player status health images (when the player is hurt)
	Q_strncpy( m_szClassImage, pKeyValuesData->GetString( "classimage", "white" ), sizeof( m_szClassImage ) );
	Q_strncpy( m_szClassImageBG, pKeyValuesData->GetString( "classimagebg", "white" ), sizeof( m_szClassImageBG ) );

	m_iHealth		= pKeyValuesData->GetFloat( "Health", 100 );
	m_flRunSpeed		= pKeyValuesData->GetFloat( "RunSpeed", 200 );
	m_flSprintSpeed		= pKeyValuesData->GetFloat( "SprintSpeed", 340 );
	m_flProneSpeed		= pKeyValuesData->GetFloat( "ProneSpeed", 160 );
	m_flStaminaDrainRate = pKeyValuesData->GetFloat( "StaminaDrainRate", lf_combat_default_drainrate.GetFloat() );
	m_flStaminaRestoreRate = pKeyValuesData->GetFloat( "StaminaRestoreRate", lf_combat_default_restorerate.GetFloat() );

	m_iArmor			= pKeyValuesData->GetInt( "armor", 0 );
}

