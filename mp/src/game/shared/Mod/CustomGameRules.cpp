/* ***** BEGIN LICENSE BLOCK *****
Version: MPL 1.1/LGPL 2.1/GPL 2.0

The contents of this file are subject to the Mozilla Public License Version 
1.1 (the "License"); you may not use this file except in compliance with
...
for the specific language governing rights and limitations under the
License.

The Original Code is for LuminousForts.

The Initial Developer of the Original Code is Hekar Khani.
Portions created by the Hekar Khani are Copyright (C) 2010
Hekar Khani. All Rights Reserved.

Contributor(s):
  Hekar Khani <hekark@gmail.com>

Alternatively, the contents of this file may be used under the terms of
either of the GNU General Public License Version 2 or later (the "GPL"),
...
the terms of any one of the MPL, the GPL or the LGPL.

***** END LICENSE BLOCK ***** */


/*===============================================================
	Shared
	
	CustomGameRules.
	The gamerules that allows the mapper to define the gamerules.
	This has many globals and allows for interaction from much of the
	rest of the game. There is no definition of when a team wins, instead
	the mapper defines when a team wins and how. :D!

	Last Updated March, 29, 2010
===============================================================*/

#include "cbase.h"
#include "Mod/CustomGameRules.h"
#include "ammodef.h"
#include "KeyValues.h"

#ifdef CLIENT_DLL

	#include "precache_register.h"
	#include "c_hl2mp_player.h"
	#include "c_team.h"
#else
	#include "hl2mp_player.h"
	#include "team.h"
	#include "playerclass_info_parse.h"
	#include "player_resource.h"
	#include "mapentities.h"

	#include "Mod/SharedModCvars.h"

#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


// Register the gamerules class
// This must be done for each class.
REGISTER_GAMERULES_CLASS( CCustomGameRules );

char *g_CustomGameRules_Classes_Blue [CUSTOM_GAME_CLASSCOUNT + 1] =
{
	"blue_classscout",
	"blue_classengineer",
	"blue_classsoldier",
	"blue_classsniper",
	"blue_classrocketeer",
	NULL
};

char *g_CustomGameRules_Classes_Red [CUSTOM_GAME_CLASSCOUNT + 1] =
{
	"red_classscout",
	"red_classengineer",
	"red_classsoldier",
	"red_classsniper",
	"red_classrocketeer",
	NULL
};

// <-------------- WARNING!!! -----------------> //
// THE PROXY TABLE FOR CustomGameRules IS BROKEN FOR A REASON UNKNOWN TO ME.
// :/ for now m_iCurrentPhaseID is sent via a HUD message and the
// HUD element hackily sets the current phase in the client
// Hekar (the noob)
// <-------------- WARNING!!! -----------------> //
BEGIN_NETWORK_TABLE_NOBASE( CCustomGameRules, DT_CustomGameRules )
#if defined ( CLIENT_DLL )
//		RecvPropFloat( RECVINFO( m_iCurrentPhaseID ) ),
#else
//		SendPropFloat( SENDINFO( m_iCurrentPhaseID ), 8, SPROP_UNSIGNED ),
#endif
END_NETWORK_TABLE()

LINK_ENTITY_TO_CLASS( CUSTOM_gamerules, CCustomGameRulesProxy );
IMPLEMENT_NETWORKCLASS_ALIASED( CustomGameRulesProxy, DT_CustomGameRulesProxy )

#ifdef CLIENT_DLL
	void RecvProxy_CustomGameRules( const RecvProp *pProp, void **pOut, void *pData, int objectID )
	{
		CCustomGameRules *pRules = CustomGameRules();
		Assert( pRules );
		*pOut = pRules;
	}

	BEGIN_RECV_TABLE( CCustomGameRulesProxy, DT_CustomGameRulesProxy )
		RecvPropDataTable( "CUSTOM_gamerules_data", 0, 0, &REFERENCE_RECV_TABLE( DT_CustomGameRules ), RecvProxy_CustomGameRules )
	END_RECV_TABLE()
#else
	void *SendProxy_CustomGameRules( const SendProp *pProp, const void *pStructBase, const void *pData, CSendProxyRecipients *pRecipients, int objectID )
	{
		CCustomGameRules *pRules = CustomGameRules();
		Assert( pRules );
		pRecipients->SetAllRecipients();
		return pRules;
	}

	BEGIN_SEND_TABLE( CCustomGameRulesProxy, DT_CustomGameRulesProxy )
		SendPropDataTable( "CUSTOM_gamerules_data", 0, &REFERENCE_SEND_TABLE( DT_CustomGameRules ), SendProxy_CustomGameRules )
	END_SEND_TABLE()
#endif

////////////////////////////////////////////////////////
// The gamemode view vectors
////////////////////////////////////////////////////////

static CCustomViewVectors g_CustomViewVectors(

	Vector( 0, 0, 64 ),       //VEC_VIEW (m_vView) 
							  
	Vector(-16, -16, 0 ),	  //VEC_HULL_MIN (m_vHullMin)
	Vector( 16,  16,  72 ),	  //VEC_HULL_MAX (m_vHullMax)
							  					
	Vector(-16, -16, 0 ),	  //VEC_DUCK_HULL_MIN (m_vDuckHullMin)
	Vector( 16,  16,  36 ),	  //VEC_DUCK_HULL_MAX	(m_vDuckHullMax)
	Vector( 0, 0, 28 ),		  //VEC_DUCK_VIEW		(m_vDuckView)
							  					
	Vector(-10, -10, -10 ),	  //VEC_OBS_HULL_MIN	(m_vObsHullMin)
	Vector( 10,  10,  10 ),	  //VEC_OBS_HULL_MAX	(m_vObsHullMax)
							  					
	Vector( 0, 0, 14 )		  //VEC_DEAD_VIEWHEIGHT (m_vDeadViewHeight)
);



const CViewVectors* CCustomGameRules::GetViewVectors() const
{
	return (CViewVectors*)GetCustomViewVectors();
}



const CCustomViewVectors *CCustomGameRules::GetCustomViewVectors() const
{
	return &g_CustomViewVectors;
}

//
//	Name: CCustomGameRules
//	Author: Hekar Khani
//	Description: CustomGameRules to create any gameplay (allowed by the entities)
//	Notes: 
//		Mapper decides when a team wins
//
bool CCustomGameRules::Init( void )
{
#ifndef CLIENT_DLL
//	ResetFilePlayerClassInfoDatabase();
#endif
//	GetGlobalTeam(SDK_TEAM_BLUE)->ClearPlayerClassInfo();
//	GetGlobalTeam(SDK_TEAM_RED)->ClearPlayerClassInfo();
//
//	GetGlobalTeam(SDK_TEAM_BLUE)->LoadPlayerClassInfo();
//	GetGlobalTeam(SDK_TEAM_RED)->LoadPlayerClassInfo();

	return BaseClass::Init();
}

#ifndef CLIENT_DLL

CCustomGameRules::CCustomGameRules()
{
	m_BuildPhase = new CBuildPhase();
	m_CombatPhase = new CCombatPhase();
	m_PhaseManager.AddPhase( m_BuildPhase );
	m_PhaseManager.AddPhase( m_CombatPhase );
	
	m_iBuildRounds = 0;
	m_iCombatRounds = 0;

	m_iWinningTeam = TEAM_INVALID;
	m_bLastRoundOver = false;

	m_flPhaseTimeLeft = 0.0f;
}

CCustomGameRules::~CCustomGameRules()
{
	// Note, don't delete each team since they are in the gEntList and will 
	// automatically be deleted from there, instead.
	delete m_BuildPhase;
	delete m_CombatPhase;
	g_Teams.Purge();
}

void CCustomGameRules::LevelInitPostEntity()
{
	SwitchToPhase( "BuildPhase", false );
}

float CCustomGameRules::FlPlayerFallDamage( CBasePlayer *pPlayer )
{
	if ( IsBuildPhase() )
	{
		return 0;
	}
	else
	{
		return BaseClass::FlPlayerFallDamage( pPlayer );
	}
}

const char *CCustomGameRules::GetGameDescription( void )
{
	if ( IsBuildPhase() )
	{
		return CUSTOM_GAME_DESCRIPTION_BUILD;
	}
	else
	{
		return CUSTOM_GAME_DESCRIPTION_COMBAT;
	}
}

bool CCustomGameRules::IsAllowedToSpawn( CBaseEntity *pEntity )
{
	bool AllowedToSpawn = true;

	if ( pEntity->IsBlock() )
	{
		CTeam *pTeam = (CTeam *) pEntity->GetTeam();

		Assert( pTeam );

		int block_limit = 0;
		if ( pEntity->GetTeamNumber() == TEAM_RED )
		{
			block_limit = lf_block_red_limit.GetInt();
		}
		else if ( pEntity->GetTeamNumber() == TEAM_BLUE )
		{
			block_limit = lf_block_blue_limit.GetInt();
		}

		if ( pTeam->GetBlockCount() >=  block_limit )
		{
			return false;
		}
	}

	return AllowedToSpawn && BaseClass::IsAllowedToSpawn( pEntity );
}

void CCustomGameRules::Think()
{
	CGameRules::Think();
	if ( g_fGameOver )
	{
		if ( m_flIntermissionEndTime && ( m_flIntermissionEndTime < gpGlobals->curtime ) )
		{
			ChangeLevel();
		}

		return;
	}

	if ( IsBuildPhase() && m_PhaseTimer.GetElapsedTime() > GetPhaseTime() )
	{
		SwitchToPhase( "CombatPhase" );
	}
	else if( IsCombatPhase() && m_PhaseTimer.GetElapsedTime() > GetPhaseTime() )
	{
		SwitchToPhase( "BuildPhase" );
	}


	CheckWinningTeam();

	BaseClass::Think();
}

void CCustomGameRules::CheckWinningTeam()
{
	if ( m_iWinningTeam != TEAM_INVALID  )
	{
		if ( m_iWinningTeam == SDK_TEAM_BLUE || m_iWinningTeam == SDK_TEAM_RED)
		{
			TeamWins( m_iWinningTeam );
		}
		else if ( m_iWinningTeam == SDK_TEAM_SPECTATOR )
		{
			TeamsDraw();
		}
	}
}

void CCustomGameRules::SetWinningTeam( int teamnum )
{
	m_iWinningTeam = teamnum;
}

void CCustomGameRules::SwitchToPhase( const char *PhaseName, bool respawn )
{
	CheckWinningTeam();

	if ( m_iCombatRounds >= lf_round_limit.GetInt() )
	{
		m_bLastRoundOver = true;
		return;
	}

	m_PhaseManager.SwitchToPhase( PhaseName );
	if ( !Q_strcmp( PhaseName, "BuildPhase" ) )
	{
		if ( m_iBuildRounds <= 0 )
		{
			SetPhaseTime( lf_build_length_first.GetFloat() );
		}
		else
		{
			SetPhaseTime( lf_build_length.GetFloat() );
		}

		m_iBuildRounds++;
	}
	else if ( !Q_strcmp (PhaseName, "CombatPhase") )
	{
		if ( m_iCombatRounds <= 0 )
		{
			// First combat phase so set longer time
			SetPhaseTime( lf_combat_length_first.GetFloat() );
		}
		else
		{
			SetPhaseTime( lf_combat_length.GetFloat() );
		}

		m_iCombatRounds++;
	}

	m_iCurrentPhaseID = m_PhaseManager.GetCurrentPhaseID();
}

void CCustomGameRules::SetPhaseTime( float PhaseTime )
{
	m_flPhaseLength = clamp (PhaseTime, 0.0f, 1000000.0f);
	m_PhaseManager.GetCurrentPhase ()->SetPhaseTime( m_flPhaseLength );
	m_PhaseTimer.Reset();
	m_PhaseTimer.Start( 0.0f );

	m_flPhaseTimeLeft = m_flPhaseLength;

	CReliableBroadcastRecipientFilter filter;	 
	UserMessageBegin( filter, "NewTime" );
		WRITE_LONG( m_iCurrentPhaseID );
		WRITE_FLOAT( m_flPhaseTimeLeft );
	MessageEnd();
}

bool CCustomGameRules::GivePlayerClassAttributes( CHL2MP_Player *pPlayer )
{
#if 0
		if( pPlayer->m_Shared.DesiredPlayerClass() == PLAYERCLASS_RANDOM )
		{
			ChooseRandomClass( pPlayer );
			ClientPrint( pPlayer, HUD_PRINTTALK, "#game_now_as", GetPlayerClassName( pPlayer->m_Shared.PlayerClass(), pPlayer->GetTeamNumber() ) );
		}
		else
		{
			pPlayer->m_Shared.SetPlayerClass( pPlayer->m_Shared.DesiredPlayerClass() );
		}

		int playerclass = pPlayer->m_Shared.PlayerClass();

		if( playerclass == PLAYERCLASS_UNDEFINED )
		{
			pPlayer->SetModel( pszPossiblePlayerModels [0] );
			pPlayer->SetMaxSpeed( 400 );
			return false;
		}

		CTeam *pTeam = GetGlobalSDKTeam( pPlayer->GetTeamNumber() );

		const CHL2MP_PlayerClassInfo &pClassInfo = pTeam->GetPlayerClassInfo( playerclass );

		Assert( playerclass < CUSTOM_GAME_CLASSCOUNT && playerclass >= 0 );
		Assert( pClassInfo.m_iTeam == pPlayer->GetTeamNumber() );

		pPlayer->SetModel( pClassInfo.m_szPlayerModel );
		pPlayer->SetHitboxSet( 0 );
#endif

		return true;
}

void CCustomGameRules::GivePlayerWeapons( CHL2MP_Player *pPlayer )
{
#if 0
	CWeaponSDKBase *pPrimaryWeapon = NULL;

	CTeam *pTeam = GetGlobalSDKTeam( pPlayer->GetTeamNumber() );
	int playerclass = pPlayer->m_Shared.PlayerClass();
	const CHL2MP_PlayerClassInfo &pClassInfo = pTeam->GetPlayerClassInfo( playerclass );

	for (int i = 0; i < pClassInfo.m_iWeaponCount; i++)
	{
		if (pClassInfo.m_WeaponVector [i] != WEAPON_NONE)
		{
			char buf[64];
			Q_snprintf( buf, sizeof( buf ), "weapon_%s", WeaponIDToAlias( pClassInfo.m_WeaponVector [i] ) );

			// Primary Ammo
			CWeaponSDKBase *pWeapon = dynamic_cast< CWeaponSDKBase * > ( pPlayer->GiveNamedItem( buf ) );

			Assert( pWeapon );

			int iAmmoCount = pClassInfo.m_AmmoVector [i];
			if ( iAmmoCount == 0 )
			{
				int iNumClip = pWeapon->GetSDKWpnData().m_iDefaultAmmoClips - 1; //account for one clip in the gun
				int iClipSize = pWeapon->GetSDKWpnData().iMaxClip1;
				pPlayer->GiveAmmo( iNumClip * iClipSize, pWeapon->GetSDKWpnData().szAmmo1 );
			}
			else
			{
				pPlayer->GiveAmmo( iAmmoCount, pWeapon->GetSDKWpnData().szAmmo1 );
			}

			if (i == 0)
			{
				pPrimaryWeapon = pWeapon;
			}
		}
	}

	Assert (pPrimaryWeapon);
	pPlayer->Weapon_Switch( (CBaseCombatWeapon *)pPrimaryWeapon );
#endif
}

void CCustomGameRules::GiveBuildPhaseItems( CHL2MP_Player *pPlayer )
{
#ifndef CLIENT_DLL
	pPlayer->SetHitboxSet( 0 );
	pPlayer->GiveNamedItem( "weapon_lf_build_cannon" );
	pPlayer->SetMaxSpeed( 300 );
#endif
}

void CCustomGameRules::PlayerSpawn( CBasePlayer *pBasePlayer )
{	
	CHL2MP_Player *pPlayer = ToHL2MPPlayer( pBasePlayer );

	if( pPlayer->GetTeamNumber() != TEAM_SPECTATOR )
	{
		if ( !GivePlayerClassAttributes( pPlayer ) )
		{
			return;
		}

		if (IsCombatPhase())
		{
			GivePlayerWeapons( pPlayer );
			//pPlayer->m_Local.m_iHideHUD &= ~HIDEHUD_BUILDPHASE;
		}
		else // Is buildphase
		{
			GiveBuildPhaseItems( pPlayer );
			//pPlayer->m_Local.m_iHideHUD |= HIDEHUD_BUILDPHASE;
		}
	}
}

void CCustomGameRules::GetTaggedConVarList( KeyValues *pCvarTagList )
{
	BaseClass::GetTaggedConVarList( pCvarTagList );

	KeyValues *pNoBuild = new KeyValues( "lfcus_phase_nobuild" );
	pNoBuild->SetString( "convar", "lfcus_phase_nobuild" );
	pNoBuild->SetString( "tag", "nobuild" );
	pCvarTagList->AddSubKey( pNoBuild );
	
	KeyValues *pNoCombat = new KeyValues( "lfcus_phase_nocombat" );
	pNoCombat->SetString( "convar", "lfcus_phase_nocombat" );
	pNoCombat->SetString( "tag", "nocombat" );
	pCvarTagList->AddSubKey( pNoCombat );

	BaseClass::GetTaggedConVarList( pCvarTagList );
}

char **CCustomGameRules::GetClassNames( int TeamNum )
{
	Assert( TeamNum == SDK_TEAM_RED || TeamNum == SDK_TEAM_BLUE );
	if (TeamNum == SDK_TEAM_RED)
	{
		return g_CustomGameRules_Classes_Red;
	}
	else if (TeamNum == SDK_TEAM_BLUE)
	{
		return g_CustomGameRules_Classes_Blue;
	}

	Msg( "Bad Team Request %d\n", TeamNum );
	return NULL;
}

void CCustomGameRules::GoToIntermission()
{
	BaseClass::GoToIntermission();

	// set all players to FL_FROZEN
	for ( int i = 1; i <= MAX_PLAYERS; i++ )
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex( i );

		if ( pPlayer )
		{
			pPlayer->AddFlag( FL_FROZEN );
		}
	}
}

void CCustomGameRules::TeamsDraw()
{
	PlaySound( "CTF.Draw" );

	// Fire user messages, etc

	GoToIntermission();
}

void CCustomGameRules::TeamWins( int TeamID )
{
	if ( TeamID == SDK_TEAM_BLUE )
	{
		PlaySound( "CTF.BlueWins" );
	}
	else if ( TeamID == SDK_TEAM_RED )
	{
		PlaySound( "CTF.RedWins" );
	}

	// Fire user messages, etc

	GoToIntermission();
}

#endif // !CLIENT_DLL
