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
	ClassicGameRules.
	This is a gamerule that derives from SDKGameRules.
	It defines the classic sf194 gameplay, along with setting
	its gamemode player classes.

	If you want to make your own gamemode then derive from 
	SDKGameRules or this.
===============================================================*/

#include "cbase.h"
#include "ammodef.h"
#include "KeyValues.h"
#include "Mod/MiscCvars.h"
#include "Mod/FileConfig.h"

#ifdef CLIENT_DLL
	#include "precache_register.h"
	#include "c_hl2mp_player.h"
	#include "c_team.h"
	#include "Mod/C_ModPlayer.h"
#else
	#include "hl2mp_player.h"
	#include "team.h"
	#include "playerclass_info_parse.h"
	#include "player_resource.h"
	#include "mapentities.h"

	#include "Mod/CBlockBase.h"
	#include "Mod/SharedModCvars.h"
	#include "Mod/CModPlayer.h"
#endif

#include "Mod/ClassicGameRules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Register the gamerules class
// This must be done for each class.
REGISTER_GAMERULES_CLASS( CClassicGameRules );

BALANCE_DEFINE( lfm_class_undefined_maxspeed, 400 );

CFileConfig g_CGRConfig( "classicgamerules", "config/gamemode/classic.txt" );

#define CLASSIC_GAME_DESCRIPTION_BUILD g_CGRConfig.All()->GetString( "ctf_build", "CTF Build" )
#define CLASSIC_GAME_DESCRIPTION_COMBAT g_CGRConfig.All()->GetString( "ctf_combat", "CTF Combat" )

#define CLASSIC_GAME_CLASSCOUNT 5

char *g_ClassicGameRules_Classes_Blue[ CLASSIC_GAME_CLASSCOUNT + 1 ] =
{
	"blue_classscout",
	"blue_classengineer",
	"blue_classsoldier",
	"blue_classsniper",
	"blue_classrocketeer",
	NULL
};

char *g_ClassicGameRules_Classes_Red[ CLASSIC_GAME_CLASSCOUNT + 1 ] =
{
	"red_classscout",
	"red_classengineer",
	"red_classsoldier",
	"red_classsniper",
	"red_classrocketeer",
	NULL
};

//
//	Name: CClassicGameRules
//	Author: Hekar Khani
//	Description: ClassicGameRules to replicate SF 194 gameplay
//	Notes: 
//		This is basically just a remake of the basic SF 194 gameplay
//
CClassicGameRules::CClassicGameRules()
{
	g_CGRConfig.Load();
#ifdef CLIENT_DLL
	m_iCurrentPhaseID = PHASE_BUILD;
	m_flPhaseTimeLeft = 0.0;
#else
	m_BuildPhase = new CBuildPhase();
	m_CombatPhase = new CCombatPhase();
	m_PhaseManager.AddPhase( m_BuildPhase );
	m_PhaseManager.AddPhase( m_CombatPhase );
	
	m_iBuildRounds = 0;
	m_iCombatRounds = 0;
	m_iBuildFallDamage = g_CGRConfig.All()->GetInt( "build_fall_damage", 0 );
	m_iCombatFallDamage = g_CGRConfig.All()->GetInt( "combat_fall_damage", 0 );

	m_bLastRoundOver = false;

	m_flPhaseTimeLeft = 0.0f;
#endif // CLIENT_DLL + else
}

CClassicGameRules::~CClassicGameRules()
{
#ifndef CLIENT_DLL
	delete m_BuildPhase;
	delete m_CombatPhase;

	// Note, don't delete each team since they are in the gEntList and will 
	// automatically be deleted from there, instead.
	g_Teams.Purge();

	ExecConfig( "classic/on_destruct.cfg" );
#endif // !CLIENT_DLL
}

bool CClassicGameRules::Init( void )
{
#ifndef CLIENT_DLL
	ResetFilePlayerClassInfoDatabase();
#endif
	for ( int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++ )
	{
		GetGlobalTeam( i )->ClearPlayerClassInfo();
	}

	for ( int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++ )
	{
		GetGlobalTeam( i )->LoadPlayerClassInfo();
	}

	return BaseClass::Init();
}

#ifndef CLIENT_DLL
void CClassicGameRules::LevelInitPostEntity()
{
	SwitchToPhase( "BuildPhase", false );
	ExecConfig( "classic/on_postinit.cfg" );
}

void CClassicGameRules::LevelShutdownPostEntity()
{
	ExecConfig( "classic/on_postshutdown.cfg" );
}

float CClassicGameRules::FlPlayerFallDamage( CBasePlayer *pPlayer )
{
	if ( IsBuildPhase() )
	{
		return m_iBuildFallDamage;
	}
	else
	{
		return m_iCombatFallDamage;
		//return BaseClass::FlPlayerFallDamage( pPlayer );
	}
}

const char *CClassicGameRules::GetGameDescription( void )
{
	if ( IsBuildPhase() )
	{
		return CLASSIC_GAME_DESCRIPTION_BUILD;
	}
	else
	{
		return CLASSIC_GAME_DESCRIPTION_COMBAT;
	}
}

bool CClassicGameRules::IsAllowedToSpawn( CBaseEntity *pEntity )
{
	bool AllowedToSpawn = true;

	if ( pEntity->IsBlock() )
	{
		CTeam *pTeam = (CTeam *) pEntity->GetTeam();
		Assert( pTeam );

		CBlockBase *pBlock = dynamic_cast< CBlockBase * > ( pEntity );

		Assert( pBlock );

		int block_limit = 0;
		if ( pEntity->GetTeamNumber() == TEAM_RED )
		{
			block_limit = lf_block_red_limit.GetInt();
		}
		else if ( pEntity->GetTeamNumber() == TEAM_BLUE )
		{
			block_limit = lf_block_blue_limit.GetInt();
		}

		if ( pTeam->GetBlockCount() + pBlock->GetBlockWorth() >= block_limit )
		{
			return false;
		}
	}

	return AllowedToSpawn && BaseClass::IsAllowedToSpawn( pEntity );
}

void CClassicGameRules::Think()
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

void CClassicGameRules::CheckWinningTeam()
{
	CTeam *pRedTeam = GetGlobalTeam( SDK_TEAM_RED );
	CTeam *pBlueTeam = GetGlobalTeam( SDK_TEAM_BLUE );

	if ( pRedTeam->GetCapturePoints() >= lf_score_limit.GetInt() )
	{
		TeamWins( SDK_TEAM_RED );
	}
	else if ( pBlueTeam->GetCapturePoints() >= lf_score_limit.GetInt() )
	{
		TeamWins( SDK_TEAM_BLUE );
	}

	if ( m_bLastRoundOver )
	{
		if ( pRedTeam->GetCapturePoints() > pBlueTeam->GetCapturePoints() )
		{
			TeamWins( SDK_TEAM_RED );
		}
		else if ( pRedTeam->GetCapturePoints() == pBlueTeam->GetCapturePoints() )
		{
			TeamsDraw();
		}
		else
		{
			TeamWins( SDK_TEAM_BLUE );
		}
	}
}

void CClassicGameRules::SetPhaseTime( float PhaseTime )
{
	m_flPhaseLength = clamp( PhaseTime, 0.0f, 1000000.0f );
	m_PhaseManager.GetCurrentPhase()->SetPhaseTime( m_flPhaseLength );
	m_PhaseTimer.Reset();
	m_PhaseTimer.Start( 0.0f );

	m_flPhaseTimeLeft = m_flPhaseLength;

	CReliableBroadcastRecipientFilter filter;	 
	UserMessageBegin( filter, "NewTime" );
		WRITE_LONG( m_iCurrentPhaseID );
		WRITE_FLOAT( m_flPhaseTimeLeft );
	MessageEnd();
}

void CClassicGameRules::SwitchToPhase( const char *PhaseName, bool respawn )
{
	CheckWinningTeam();

	if ( m_iCombatRounds >= lf_round_limit.GetInt() )
	{
		m_bLastRoundOver = true;
		return;
	}

	m_PhaseManager.SwitchToPhase( PhaseName );
	if ( !Q_stricmp( PhaseName, "BuildPhase" ) )
	{
		if ( m_iBuildRounds <= 0 )
		{
			SetPhaseTime( lf_build_length_first.GetFloat() );
		}
		else
		{
			SetPhaseTime( lf_build_length.GetFloat() );
		}

		ExecConfig( "classic/on_build.cfg" );
		m_iBuildRounds++;
	}
	else if ( !Q_stricmp (PhaseName, "CombatPhase") )
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

		ExecConfig( "classic/on_combat.cfg" );
		m_iCombatRounds++;
	}

	m_iCurrentPhaseID = m_PhaseManager.GetCurrentPhaseID();

	if ( respawn )
	{
		RespawnPlayers();
		
		// Only execute switch on respawn
		m_PhaseManager.GetCurrentPhase()->SwitchTo();
	}

	CReliableBroadcastRecipientFilter filter;	 
	UserMessageBegin( filter, "NewTime" );
		WRITE_LONG( m_iCurrentPhaseID );
		WRITE_FLOAT( m_flPhaseTimeLeft );
	MessageEnd();
}

void CClassicGameRules::TeamsDraw()
{
	PlaySound( "CTF.Draw" );

	// Fire user messages, etc

	GoToIntermission();
}

void CClassicGameRules::TeamWins( int TeamID )
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

void CClassicGameRules::GoToIntermission()
{
	BaseClass::GoToIntermission();

	// Show intermission panel
	for ( int i = 1; i <= MAX_PLAYERS; i++ )
	{
		CBasePlayer *pPlayer = UTIL_PlayerByIndex( i );

		if ( !pPlayer )
			continue;

		pPlayer->ShowViewPortPanel( "HudIntermission" );
	}

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

bool CClassicGameRules::GivePlayerClassAttributes( CModPlayer *pPlayer )
{
	pPlayer->SetPlayerClass( pPlayer->GetDesiredPlayerClass() );

	int playerclass = pPlayer->GetPlayerClass();

	if( playerclass == PLAYERCLASS_UNDEFINED )
	{
		pPlayer->SetModel( pszPossiblePlayerModels [0] );
		pPlayer->SetMaxSpeed( BALANCE_VALUE( Int, lfm_class_undefined_maxspeed ) );
		return false;
	}

	CTeam *pTeam = GetGlobalTeam( pPlayer->GetTeamNumber() );

	const CPlayerClassInfo &pClassInfo = pTeam->GetPlayerClassInfo( playerclass );

	Assert( playerclass < CLASSIC_GAME_CLASSCOUNT && playerclass >= 0 );
	Assert( pClassInfo.m_iTeam == pPlayer->GetTeamNumber() );

	pPlayer->SetHealth( pClassInfo.m_iHealth );
	pPlayer->SetMaxHealth( pClassInfo.m_iHealth );
	pPlayer->SetModel( pClassInfo.m_szPlayerModel );
	pPlayer->SetHitboxSet( 0 );

	return true;
}

void CClassicGameRules::GivePlayerWeapons( CModPlayer *pPlayer )
{
	CBaseCombatWeapon *pPrimaryWeapon = NULL;

	CTeam *pTeam = GetGlobalTeam( pPlayer->GetTeamNumber() );
	int playerclass = pPlayer->GetPlayerClass();
	const CPlayerClassInfo &pClassInfo = pTeam->GetPlayerClassInfo( playerclass );

	for (int i = 0; i < pClassInfo.m_iWeaponCount; i++)
	{
		if (pClassInfo.m_WeaponVector [i] != WEAPON_NONE)
		{
			char buf[ 64 ];
			Q_snprintf( buf, sizeof( buf ), "weapon_%s", WeaponIDToAlias( pClassInfo.m_WeaponVector[ i ] ) );

			// Primary Ammo
			CBaseCombatWeapon *pWeapon = dynamic_cast< CBaseCombatWeapon * > ( pPlayer->GiveNamedItem( buf ) );

			Msg( "Weapon %s found %d\n", buf, pWeapon );
			Assert( pWeapon );
			if ( !pWeapon )
			{
				continue;
			}

			int iAmmoCount = pClassInfo.m_AmmoVector[ i ];
			if ( iAmmoCount == 0 )
			{
				int iNumClip = pWeapon->GetWpnData().iDefaultClip1 - 1; //account for one clip in the gun
				int iClipSize = pWeapon->GetWpnData().iMaxClip1;
				pPlayer->GiveAmmo( iNumClip * iClipSize, GetAmmoDef()->Index(pWeapon->GetWpnData().szAmmo1), true );
			}
			else
			{
				pPlayer->GiveAmmo( iAmmoCount, GetAmmoDef()->Index(pWeapon->GetWpnData().szAmmo1), true );
			}

			if ( i == 0 )
			{
				pPrimaryWeapon = pWeapon;
			}
		}
	}

	pPlayer->SetSprintSpeed( pClassInfo.m_flSprintSpeed );
	pPlayer->SetRunSpeed( pClassInfo.m_flRunSpeed );

	pPlayer->SetMaxSpeed( pClassInfo.m_flSprintSpeed );

	Assert( pPrimaryWeapon );
	pPlayer->Weapon_Switch( (CBaseCombatWeapon *)pPrimaryWeapon );
}

void CClassicGameRules::GiveBuildPhaseItems( CModPlayer *pPlayer )
{
	pPlayer->SetHitboxSet( 0 );
	pPlayer->GiveNamedItem( "weapon_lf_build_cannon" );
	pPlayer->GiveNamedItem( "weapon_lf_beacon" );
//	pPlayer->SetMaxSpeed( BALANCE_VALUE( Int, lfm_builder_maxspeed ) );
//	pPlayer->SetRunSpeed( BALANCE_VALUE( Float, lfm_builder_runspeed ) );

	pPlayer->SetRunSpeed(lf_build_player_runspeed.GetFloat());
	pPlayer->SetSprintSpeed(lf_build_player_sprintspeed.GetFloat());
	pPlayer->SetMaxSpeed( pPlayer->GetSprintSpeed() );
}

void CClassicGameRules::PlayerSpawn( CBasePlayer *pBasePlayer )
{	
	CModPlayer *player = ToModPlayer( pBasePlayer );

	int team = player->GetTeamNumber();
	if( team != TEAM_SPECTATOR && team != TEAM_UNASSIGNED )
	{
		if ( !GivePlayerClassAttributes( player ) )
		{
			return;
		}

		if ( IsCombatPhase() )
		{
			int playerclass = player->GetPlayerClass();
			switch ( playerclass )
			{
			case CLASSIC_CLASS_SCOUT:
				ExecConfig( "classic/scout.cfg" );
				break;
			case CLASSIC_CLASS_ENGINEER:
				ExecConfig( "classic/engineer.cfg" );
				break;
			case CLASSIC_CLASS_SOLDIER:
				ExecConfig( "classic/soldier.cfg" );
				break;
			case CLASSIC_CLASS_SNIPER:
				ExecConfig( "classic/sniper.cfg" );
				break;
			case CLASSIC_CLASS_ROCKETEER:
				ExecConfig( "classic/rocketeer.cfg" );
				break;
			}

			GivePlayerWeapons( player );
			player->RemoveFromHud( HIDEHUD_BUILDPHASE );
		}
		else // Is buildphase
		{
			GiveBuildPhaseItems( player );
			player->AddToHud( HIDEHUD_BUILDPHASE );
		}
	}
}

const char *CClassicGameRules::GetChatPrefix( bool bTeamOnly, CBasePlayer *pPlayer )
{
	return "";
}

const char *CClassicGameRules::GetChatFormat( bool bTeamOnly, CBasePlayer *pPlayer )
{
	if ( !pPlayer )  // dedicated server output
		return NULL;

	const char *pszFormat = NULL;

	if ( bTeamOnly )
	{
		if ( pPlayer->GetTeamNumber() == TEAM_SPECTATOR )
			pszFormat = "SDK_Chat_Spec";
		else
		{
			if (pPlayer->m_lifeState != LIFE_ALIVE )
				pszFormat = "SDK_Chat_Team";
			else
				pszFormat = "SDK_Chat_Team";
		}
	}
	else
	{
		if ( pPlayer->GetTeamNumber() == TEAM_SPECTATOR)
			pszFormat = "SDK_Chat_All_Spec";
		else
		{
			if (pPlayer->m_lifeState != LIFE_ALIVE )
				pszFormat = "SDK_Chat_All";
			else
				pszFormat = "SDK_Chat_All";
		}
	}

	return pszFormat;
}

#endif

char **CClassicGameRules::GetClassNames( int TeamNum )
{
	Assert( TeamNum == SDK_TEAM_RED || TeamNum == SDK_TEAM_BLUE );
	if (TeamNum == SDK_TEAM_RED)
	{
		return g_ClassicGameRules_Classes_Red;
	}
	else if (TeamNum == SDK_TEAM_BLUE)
	{
		return g_ClassicGameRules_Classes_Blue;
	}

	Msg( "Bad Team Request %d\n", TeamNum );
	return NULL;
}

const char *CClassicGameRules::GetClassmenuRed() const
{
	return "resource/ui/Classmenu_CTF_Red.res";
}

const char *CClassicGameRules::GetClassmenuBlue() const
{
	return "resource/ui/Classmenu_CTF_Blue.res";
}

int CClassicGameRules::GetClassCount( void )
{
	return CLASSIC_GAME_CLASSCOUNT;
}

int CClassicGameRules::GetCurrentPhaseID()
{
	return m_iCurrentPhaseID;
}


#ifndef CLIENT_DLL
bool CClassicGameRules::ClientConnected(edict_t* pEntity, const char* pszName,
		const char* pszAddress, char* reject, int maxrejectlen)
{
	return BaseClass::ClientConnected(pEntity, pszName, pszAddress, reject, maxrejectlen);
}

void CClassicGameRules::ClientDisconnected(edict_t* pClient)
{
	CModPlayer *player = ToModPlayer( (CBasePlayer *)CBaseEntity::Instance( pClient ) );
	if (player)
	{
		player->Event_Disconnected();
	}

	BaseClass::ClientDisconnected(pClient);
}

bool CClassicGameRules::IsPlayerClassOnTeam( int cls, int team )
{
	CTeam *pTeam = GetGlobalTeam( team );
	return ( cls >= 0 && cls < CLASSIC_GAME_CLASSCOUNT );
}

int CClassicGameRules::CountPlayerClass( int team, int cls )
{
	int num = 0;

	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		CModPlayer *pModPlayer = ToModPlayer( UTIL_PlayerByIndex( i ) );

		if (pModPlayer == NULL)
			continue;

		if (FNullEnt( pModPlayer->edict() ))
			continue;

		if( pModPlayer->GetTeamNumber() != team )
			continue;

		if( pModPlayer->GetDesiredPlayerClass() == cls )
			num++;
	}

	return num;
}

const char *CClassicGameRules::GetPlayerClassName( int cls, int team )
{
	if ( team == TEAM_SPECTATOR )
	{
		return "Spectator";
	}

	CTeam *pTeam = GetGlobalTeam( team );

	if( cls == PLAYERCLASS_RANDOM )
	{
		return "#class_random";
	}

	if( cls < 0 || cls >= CLASSIC_GAME_CLASSCOUNT )
	{
		Assert( false );
		return NULL;
	}

	return pTeam->GetPlayerClassInfo( cls ).m_szPrintName;
}

void CClassicGameRules::PlayerKilled(CBasePlayer* pVictim, const CTakeDamageInfo& info)
{
	BaseClass::PlayerKilled(pVictim, info);
}

void CClassicGameRules::RespawnPlayers()
{
	int iPlayersSpawned = 0;

	CBasePlayer *pPlayer;
	for ( int i = 1; i <= gpGlobals->maxClients; i++ )
	{
		pPlayer = ToBasePlayer( UTIL_PlayerByIndex( i ) );

		if ( !pPlayer || pPlayer->GetTeamNumber() == TEAM_SPECTATOR )
			continue;

		// players that haven't chosen a team/class can never spawn
		if ( !pPlayer->IsReadyToPlay() )
		{
			// Let the player spawn immediately when they do pick a class
			if ( pPlayer->ShouldGainInstantSpawn() )
			{
				pPlayer->AllowInstantSpawn();
			}

			continue;
		}

		// If we aren't force respawning, don't respawn players that:
		// - are alive
		// - are still in the death anim stage of dying
		bool bForceRespawn = true;
		if ( !bForceRespawn )
		{
			if ( pPlayer->IsAlive() )
				continue;

			if ( !pPlayer->IsReadyToSpawn() )
			{
				// Let the player spawn immediately when they do pick a class
				if ( pPlayer->ShouldGainInstantSpawn() )
				{
					pPlayer->AllowInstantSpawn();
				}

				continue;
			}

		}

		// Respawn this player
		pPlayer->ForceRespawn();
		iPlayersSpawned++;
	}
}

#endif // !CLIENT_DLL
