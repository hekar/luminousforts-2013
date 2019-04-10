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
	Server
	
	The Player class that introduces modifications.
	Unfortunately, most of the modifications are actually in
	CHL2MP_Player and this piece of code hasn't been looked at 
	in a long time.

	Though, this is still the instantiated class for players.
	If you have time, think about moving some mod specific
	code from CHL2MP_Player to CModPlayer.

	Last Updated June 21, 2010
===============================================================*/

#include "cbase.h"

#include "team.h"
#include "hl2mp_player.h"

#ifdef MOD_SF132
#include "Mod/SF132_GameRules.h"
#endif

#include "props.h"
#include "grenade_satchel.h"
#include "eventqueue.h"
#include "igameevents.h"
#include "GameEventListener.h"

#include "Mod/CBlockBase.h"
#include "Mod/WPN_CombatCannon.h"
#include "Mod/CFlagBase.h"
#include "Mod/Proj_Beacon.h"
#include "Mod/ClassicGameRules.h"
#include "Mod/CSpawnPoint.h"
#include "Mod/CModPlayer.h"

LINK_ENTITY_TO_CLASS( player, CModPlayer );
PRECACHE_REGISTER( player );

BEGIN_DATADESC( CModPlayer )
END_DATADESC()

// main table
IMPLEMENT_SERVERCLASS_ST( CModPlayer, DT_ModPlayer )
	SendPropInt( SENDINFO( m_iMaxHealth ) ),
END_SEND_TABLE()

EHANDLE g_pLastDMSpawn;
EHANDLE g_pLastBlueSpawn;
EHANDLE g_pLastRedSpawn;

CModPlayer::CModPlayer()
{
	m_bHasFlag = false;
	m_bHasTouched = false;
	m_pFlag = NULL;
}

CModPlayer::~CModPlayer()
{
}

CModPlayer *CModPlayer::CreatePlayer( const char *className, edict_t *ed )
{
	CModPlayer::s_PlayerEdict = ed;
	return ( CModPlayer* )CreateEntityByName( className );
}

bool CModPlayer::CanBecomeRagdoll( void )
{
	if ( GameRules()->GetGameMode() == GAMEMODE_CLASSIC &&
		ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD )
	{
		return false;
	}
	else
	{
		return BaseClass::CanBecomeRagdoll();
	}
}

CFlagBase *CModPlayer::GetFlag()
{
	return ( CFlagBase * ) m_pFlag;
}

bool CModPlayer::HasTouched()
{
	return m_bHasTouched;
}

// Has touched flag
void CModPlayer::SetTouched( bool touched )
{
	m_bHasTouched = touched;
}

void CModPlayer::GiveFlag (CBaseEntity *pFlag)
{
	if ( !pFlag )
	{
		return;
	}

	m_pFlag = dynamic_cast< CFlagBase * > (pFlag);
	m_bHasFlag = true;

	m_pFlag->Pickup( this );
}

bool CModPlayer::HasFlag()
{
	return m_bHasFlag;
}

void CModPlayer::ReturnFlag( bool scored )
{
	if (!m_pFlag || !m_bHasFlag)
	{
		return;
	}

	m_pFlag->ReturnToSpawn( scored );

	m_bHasFlag = false;
	m_pFlag = NULL;
}

void CModPlayer::DropFlag()
{
	if ( !m_pFlag || !m_bHasFlag )
	{
		return;
	}

	m_pFlag->Drop();

	m_bHasFlag = false;
	m_pFlag = NULL;
}

void CModPlayer::ChangeTeam( int iTeamNum )
{
	CancelTeamActions();

	BaseClass::ChangeTeam( iTeamNum );
}

void CModPlayer::Event_Disconnected()
{
	CancelTeamActions();
}

void CModPlayer::Event_Killed( const CTakeDamageInfo &info )
{
	CancelTeamActions();
	BaseClass::Event_Killed( info );
}

void CModPlayer::CancelTeamActions() {
	// TODO: Fix
	if ( HasFlag() )
	{
		DropFlag();
	}

	m_bHasTouched = false;

	CBaseCombatWeapon *wpn = GetActiveWeapon();
	if ( wpn && wpn->GetWeaponID() == LF_WEAPON_COMBATCANNON )
	{
		CWeaponCombatCannon *combatcannon = dynamic_cast< CWeaponCombatCannon* >( wpn );
		assert( combatcannon );
		if ( combatcannon )
		{
			DevMsg( "Reset block freezing\n" );
			combatcannon->ResetFreezing( false );

			CBlockBase *block = combatcannon->GetLastBlock();
			if ( block )
			{
				DevMsg( "Block current freezer is set to NULL\n" );
				block->SetCurrentFreezer( NULL );
			}
		}
	}

#if 0
	// Remove all the player's beacons
	RemoveBeacons();

	BaseClass::Event_Disconnected();
#endif // 0
}

void CModPlayer::Precache()
{
	UTIL_PrecacheOther( "lfc_prop_block1x2" );
	UTIL_PrecacheOther( "lfc_prop_block1x3" );
	UTIL_PrecacheOther( "lfc_prop_block1x4" );
	UTIL_PrecacheOther( "lfc_prop_block1x5" );
	UTIL_PrecacheOther( "lfc_prop_block2x2" );
	UTIL_PrecacheOther( "lfc_prop_block2x3" );
	UTIL_PrecacheOther( "lfc_prop_block2x4" );
	UTIL_PrecacheOther( "lfc_prop_block2x5" );
	UTIL_PrecacheOther( "lfc_prop_block3x3" );
	UTIL_PrecacheOther( "lfc_prop_block3x4" );
	UTIL_PrecacheOther( "lfc_prop_block3x5" );
	PrecacheScriptSound( "HL2Player.SprintStart" );
	BaseClass::Precache();
}

void CModPlayer::InitialSpawn()
{
	BaseClass::InitialSpawn();
	State_Enter( STATE_ENTER );
}

void CModPlayer::Spawn()
{
	BaseClass::Spawn();

	if ( GetPlayerClass() == PLAYERCLASS_UNDEFINED )
	{
		SetMoveType( MOVETYPE_NONE );
	}

	if ( GetPlayerState() != STATE_ENTER )
	{
		if ( GetTeamNumber() == TEAM_UNASSIGNED )
		{
			State_Transition( STATE_TEAM_REQUIRED );
		}
		else if ( GetPlayerClass() == PLAYERCLASS_UNDEFINED &&
				( GetTeamNumber() != TEAM_SPECTATOR || GetTeamNumber() != TEAM_UNASSIGNED ) )
		{
			State_Transition( STATE_CLASS_REQUIRED );
		}
	}

	// Send Flag info if in classic mode
	if ( GameRules()->GetGameModeMask() & GAMEMODE_CLASSIC )
	{
#ifndef CLIENT_DLL
		//HODO: fix this, because could cause lag
		// Refresh the phase time for everyone
		ClassicGameRules()->SetPhaseTime( ClassicGameRules()->GetPhaseTimeLeft() );
#endif // !CLIENT_DLL

		CBaseEntity *ent = NULL;
		while ( ( ent = gEntList.FindEntityByClassname( ent, "lfc_prop_flag" ) ) != NULL )
		{
			if ( ent->IsFlag() )
			{
				CFlagBase *flag = dynamic_cast< CFlagBase *> ( ent );
				flag->UpdateFlagStatus( this );
			}
		}
	}

#ifdef MOD_SF132
	if ( GameRules()->GetGameMode() == GAMEMODE_SF132 )
	{
		if ( SF132GameRules()->GetCurrentPhaseID() == PHASE_COMBAT )
		{
			m_ItemRegister.GiveItems( this );
		}
	}
#endif // MOD_SF132
}

void CModPlayer::DetonateTripmines()
{
	CBaseEntity *pEntity = NULL;

	while ((pEntity = gEntList.FindEntityByClassname( pEntity, "npc_satchel" )) != NULL)
	{
		CSatchelCharge *pSatchel = dynamic_cast<CSatchelCharge *>(pEntity);
		if (pSatchel->m_bIsLive && pSatchel->GetThrower() == this )
		{
			g_EventQueue.AddEvent( pSatchel, "Explode", 0.20, this, this );
		}
	}

	// Play sound for pressing the detonator
	EmitSound( "Weapon_SLAM.SatchelDetonate" );
}

void CModPlayer::RemoveBeacons()
{
#ifndef CLIENT_DLL
	CBaseEntity *pEntity = NULL;

	while ((pEntity = gEntList.FindEntityByClassname( pEntity, "npc_beacon" )) != NULL)
	{
		CBeaconGrenade *pBeacon = dynamic_cast<CBeaconGrenade *>(pEntity);
		if ( pBeacon->m_hOwner == this )
		{
			UTIL_Remove( pBeacon );
		}
	}
#endif // CLIENT_DLL
}


CBaseEntity *CModPlayer::EntSelectSpawnPoint()
{
	CBaseEntity *pSpot = NULL;
	const char *pSpawnPointName = "";

	switch( GetTeamNumber() )
	{
	case SDK_TEAM_BLUE:
		{
			pSpot = g_pLastBlueSpawn;
			if ( SelectTeamSpawnSpot( SDK_TEAM_BLUE, pSpot ) )
			{
				g_pLastBlueSpawn = pSpot;
			}
		}
		break;
	case SDK_TEAM_RED:
		{
			pSpot = g_pLastRedSpawn;
			if ( SelectTeamSpawnSpot( SDK_TEAM_RED, pSpot ) )
			{
				g_pLastRedSpawn = pSpot;
			}
		}
		break;
	case TEAM_UNASSIGNED:
		{
			pSpawnPointName = "info_player_deathmatch";
			pSpot = g_pLastDMSpawn;
			if ( SelectTeamSpawnSpot( SDK_TEAM_RED, pSpot ) )
			{
				g_pLastDMSpawn = pSpot;
			}
			else if ( SelectSpawnSpot( pSpawnPointName, pSpot ) )
			{
				g_pLastDMSpawn = pSpot;
			}
		}
		break;
	case TEAM_SPECTATOR:
	default:
		{
			pSpot = CBaseEntity::Instance( INDEXENT(0) );
		}
		break;
	}

	if ( !pSpot )
	{
		Warning( "PutClientInServer: no %s on level\n", pSpawnPointName );
		return CBaseEntity::Instance( INDEXENT(0) );
	}

	return pSpot;
}

bool CModPlayer::SelectTeamSpawnSpot( int iTeamNum, CBaseEntity* &pSpot )
{
	CBaseEntity *pNewSpot = NULL;
	for ( int i = 0; i < ARRAYSIZE( g_GenericSpawnPoints ); i++ )
	{
		const char *GenericSpawnPointName = g_GenericSpawnPoints[ i ];

		pNewSpot = gEntList.FindEntityByClassname( pSpot, GenericSpawnPointName );

		if ( pNewSpot == NULL ) // skip over the null point
			pNewSpot = gEntList.FindEntityByClassname( pNewSpot, GenericSpawnPointName );

		CBaseEntity *pFirstSpot = pNewSpot;
		do
		{
			if ( pNewSpot )
			{
				if ( g_pGameRules->IsSpawnPointValid( pNewSpot, this ) )
				{
					if ( pNewSpot->GetAbsOrigin() == vec3_origin )
					{
						continue;
					}
					else if ( pNewSpot->GetTeamNumber() == iTeamNum )
					{
						pSpot = pNewSpot;
						return true;
					}
				}
			}

			pNewSpot = gEntList.FindEntityByClassname( pNewSpot, GenericSpawnPointName );

		} while ( pNewSpot != pFirstSpot );
	}


	if ( iTeamNum == SDK_TEAM_BLUE )
	{
		for ( int i = 0; i < ARRAYSIZE( g_BlueSpawnPoints ); i++ )
		{
			const char *SpawnPoint = g_BlueSpawnPoints[ i ];
			if ( SelectSpawnSpot( SpawnPoint, pSpot ) )
			{
				Msg( "Choosing a blue spawn point\n" );
				return true;
			}
		}
	}
	else if ( iTeamNum == SDK_TEAM_RED )
	{
		for ( int i = 0; i < ARRAYSIZE( g_RedSpawnPoints ); i++ )
		{
			const char *SpawnPoint = g_RedSpawnPoints[ i ];
			if ( SelectSpawnSpot( SpawnPoint, pSpot ) )
			{
				Msg( "Choosing a red spawn point\n" );
				return true;
			}
		}
	}

	return false;
}

bool CModPlayer::SelectSpawnSpot( const char *pEntClassName, CBaseEntity* &pSpot )
{
	// Find the next spawn spot.
	pSpot = gEntList.FindEntityByClassname( pSpot, pEntClassName );

	if ( pSpot == NULL ) // skip over the null point
		pSpot = gEntList.FindEntityByClassname( pSpot, pEntClassName );

	CBaseEntity *pFirstSpot = pSpot;
	do
	{
		if ( pSpot )
		{
			// check if pSpot is valid
			if ( g_pGameRules->IsSpawnPointValid( pSpot, this ) )
			{
				if ( pSpot->GetAbsOrigin() == Vector( 0, 0, 0 ) )
				{
					pSpot = gEntList.FindEntityByClassname( pSpot, pEntClassName );
					continue;
				}

				// if so, go to pSpot
				return true;
			}
		}
		// increment pSpot
		pSpot = gEntList.FindEntityByClassname( pSpot, pEntClassName );
	} while ( pSpot != pFirstSpot ); // loop if we're not back to the start

	DevMsg("CSDKPlayer::SelectSpawnSpot: couldn't find valid spawn point.\n");

	return false;
}

void CModPlayer::RemoveFromHud( unsigned int hud )
{
	m_Local.m_iHideHUD &= ~hud;
}

void CModPlayer::AddToHud( unsigned int hud )
{
	m_Local.m_iHideHUD |= hud;
}

void CModPlayer::Weapon_DropAll(bool bDisallowWeaponPickup)
{
}

bool CModPlayer::HandleCommand_JoinClass( int iClass )
{
	Assert( GetTeamNumber() != TEAM_SPECTATOR );
	Assert( GetTeamNumber() != TEAM_UNASSIGNED );

	if( GetTeamNumber() == TEAM_SPECTATOR )
		return false;

	if( iClass == PLAYERCLASS_UNDEFINED )
		return false;

	int iOldPlayerClass = m_HL2Local.m_iDesiredPlayerClass;

	if( iClass == iOldPlayerClass )
	{
		return true;
	}

	if( !ClassicGameRules()->IsPlayerClassOnTeam( iClass, GetTeamNumber() ) )
	{
		return false;
	}

	const char *classname = ClassicGameRules()->GetPlayerClassName( iClass, GetTeamNumber() );

	m_HL2Local.m_iDesiredPlayerClass = iClass ;

	if( IsAlive() )
	{
		ClientPrint(this, HUD_PRINTTALK, "#game_respawn_as", classname );
	}
	else
	{
		ClientPrint(this, HUD_PRINTTALK, "#game_spawn_as", classname );
	}

	IGameEvent * event = gameeventmanager->CreateEvent( "player_changeclass" );
	if ( event )
	{
		event->SetInt( "userid", GetUserID() );
		event->SetInt( "class", iClass );

		gameeventmanager->FireEvent( event );
	}

	if ( GetPlayerState() == STATE_CLASS_REQUIRED )
	{
		State_Transition( STATE_ACTIVE );
	}

	return true;
}

bool CModPlayer::ClientCommand( const CCommand &args )
{
	const char *pcmd = args[0];
	if ( !Q_strncmp( pcmd, "cls_", 4 ) )
	{
		CTeam *pTeam = GetGlobalTeam( GetTeamNumber( ) );

		Assert( pTeam );

		int iClassIndex = PLAYERCLASS_UNDEFINED;

		if ( pTeam->IsClassOnTeam( pcmd, iClassIndex ) )
		{
			HandleCommand_JoinClass( iClassIndex );
		}

		return true;
	}
	else if ( FStrEq( pcmd, "mod_motd_closed" ) )
	{
		if ( State_Get() == STATE_ENTER )
		{
			State_Transition( STATE_TEAM_REQUIRED );
		}

		return true;
	}
	else if ( FStrEq( pcmd, "joinclass" ) )
	{
		if ( args.ArgC( ) < 2 )
		{
			Warning( "Player sent bad joinclass syntax\n" );
		}

		int iClass = atoi( args[1] );
		HandleCommand_JoinClass( iClass );
		return true;
	}

	return BaseClass::ClientCommand( args );
}

#ifdef MOD_SF132
CItemRegister *CModPlayer::GetItemRegister()
{
	return &m_ItemRegister;
}
#endif // MOD_SF132

CModPlayer *ToModPlayer( CBaseEntity *pEntity )
{
	return dynamic_cast< CModPlayer * > ( pEntity );
}
