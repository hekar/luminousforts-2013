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
	Client/Server
	Physics Model Entity

	This has been a negleted piece of code.
	It's really old and	really poorly done by myself.

	Cleaned up a little.

	TODO:
		-Rocket firing at flag
		-Flag not "touching" disposer

	Last Updated Dec 20 2009 <- keep forgetting to update this
===============================================================*/

#include "cbase.h"
#include "CFlagBase.h"

#include "team.h"
#include "CModPlayer.h"
#include "in_buttons.h"

#include "Mod/ClassicGameRules.h"
#include "Mod/SharedModCvars.h"


BEGIN_DATADESC( CFlagBase )
	
	DEFINE_ENTITYFUNC( Touch ),
	DEFINE_THINKFUNC( Think ),

	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "team" ),
	DEFINE_KEYFIELD( m_iSourcefortsTeamNumber, FIELD_INTEGER, "teamnum" ),

	DEFINE_KEYFIELD( m_bFullyDisabled, FIELD_BOOLEAN, "StartDisabled" ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),

	DEFINE_KEYFIELD( m_vecSpawnOrigin, FIELD_POSITION_VECTOR, "SpawnLocation" ),
	DEFINE_INPUTFUNC( FIELD_POSITION_VECTOR, "ChangeSpawnLocation", InputChangeSpawnLocation ),

	DEFINE_OUTPUT( m_OnScore, "OnScore" ),
	DEFINE_OUTPUT( m_OnPickup, "OnPickup" ),
	DEFINE_OUTPUT( m_OnDropped, "OnDropped" ),
	DEFINE_OUTPUT( m_OnReturned, "OnReturned" ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CFlagBase, DT_FlagBase )
END_SEND_TABLE()


//
//	Name: CFlagBase
//	Author: Hekar Khani
//	Description: Base class for CTF flag
//	Notes: Really bad and confusing code
//		Cleaned up a little
//
CFlagBase::CFlagBase() :
	m_vecSpawnOrigin( 0.0f, 0.0f, 0.0f )
{
	m_pPlayerWithFlag = NULL;
	m_pPlayerDroppedFlag = NULL;
	m_bPlayerHasFlag = false;
	m_bDropped = false;

	// Player can't pickup flag within first few seconds of round start
	m_TimeDropped = 0.0f;
	m_TimeTillReturn = 0.0f;
	m_bFullyDisabled = false;
	m_bDisabled = true;

	ListenForGameEvent( "phase_switch" );
}

CFlagBase::~CFlagBase()
{
	// Stop listening on game events
	StopListeningForAllEvents();
}

// Override this and do your own
void CFlagBase::Precache( void )
{
	BaseClass::Precache();
}

// Call precache and set modelname in own spawn func
void CFlagBase::Spawn( void )
{
	if ( g_pGameRules->IsAllowedToSpawn( this ) == false )
	{
		UTIL_Remove( this );
		return;
	}

	if ( lf_reverse_flags.GetBool() )
	{
		ChangeTeam( GetOtherTeamNumber( GetTeamNumber() ) );
	}

	SetTouch( &CFlagBase::Touch );
	AddSolidFlags( FSOLID_TRIGGER );

	BaseClass::Spawn();
	if ( ClassicGameRules( )->IsSourcefortsMap( ) && Q_stricmp( gpGlobals->mapname.ToCStr( ), "sf_skywalk" ) )
	{
		ChangeTeam( m_iSourcefortsTeamNumber );
	}

	m_vecSpawnOrigin = GetAbsOrigin();
	SetCollisionGroup( COLLISION_GROUP_FLAG );

	SetRenderMode( kRenderWorldGlow );
	switch ( GetTeamNumber() )
	{
		case SDK_TEAM_RED:
			SetRenderColor( LF_CLR_TEAM_RED );
			break;
		case SDK_TEAM_BLUE:
			SetRenderColor( LF_CLR_TEAM_BLUE );
			break;
		default:
			SetRenderColor( LF_CLR_TEAM_UNASSIGNED );
			break;
	}

	// Default flag status
	m_FlagStatus = HFLG_BASE;
	m_StatusTeamId = GetTeamNumber();
	m_StatusFlagTeamId = GetTeamNumber();
	m_StatusUserId = -1;

	// Disallow punting, pushing and picking up
	AddSpawnFlags( SF_PHYSPROP_PREVENT_PICKUP | SF_PHYSPROP_DONT_TAKE_PHYSICS_DAMAGE );
	AddEFlags( EFL_NO_PHYSCANNON_INTERACTION | EFL_NO_DAMAGE_FORCES );

	if ( m_bFullyDisabled )
	{
		DisableFlag();
	}
}

const bool CFlagBase::PlayerHasFlag() const
{
	return m_bPlayerHasFlag;
}

void CFlagBase::SetSpawnPoint( const Vector& Origin )
{
	m_vecSpawnOrigin = Origin;
}

// Do not call directly, use CModPlayer->GiveFlag(CBaseEntity *pFlag) instead
void CFlagBase::Pickup( CModPlayer *pPlayer )
{
	SetNextThink( gpGlobals->curtime + 0.1f );

	m_bDropped = false;
	m_bPlayerHasFlag = true;
	m_pPlayerWithFlag = pPlayer;
	m_pPlayerDroppedFlag = NULL;

	OnPickup();
	OnPickupEvent();
	OnPickupOutput();
}

void CFlagBase::Drop ()
{
	// Cannot drop twice
	if ( m_bDropped )
	{
		return;
	}

	m_bDropped = true;
	m_TimeDropped = gpGlobals->curtime;

	m_TimeTillReturn = gpGlobals->curtime + lf_flag_return.GetFloat();
	
	OnDrop();
	OnDropEvent();
	OnDropOutput();

	m_pPlayerDroppedFlag = m_pPlayerWithFlag;
	m_pPlayerWithFlag = NULL;
	m_bPlayerHasFlag = false;
}

// Do not call directly if the player has the flag (use CModPlayer ReturnFlag())
void CFlagBase::ReturnToSpawn( bool scored )
{
	// Do not fire events or anything if it's already returned
	if ( !m_bDropped && !m_bPlayerHasFlag )
	{
		return;
	}

	m_TimeDropped = 0.0f;
	m_TimeTillReturn = 0.0f;
	m_bDropped = false;

	OnReturn();
	OnReturnEvent();
	OnReturnOutput();

	SetAbsOrigin( m_vecSpawnOrigin );

	// The player no longer has the flag
	m_pPlayerDroppedFlag = NULL;
	m_bPlayerHasFlag = false;
	m_pPlayerWithFlag = NULL;
	m_pPlayerReturnedFlag = NULL;
}

int CFlagBase::OnTakeDamage( const CTakeDamageInfo &info )
{
	// Do not take external physics forces originating from damage
	return 0;
}

// Enable Flag touches
void CFlagBase::InputEnable( inputdata_t& data )
{
	m_bDisabled = false;
	EnableFlag();
}

// Disable Flag touches
void CFlagBase::InputDisable( inputdata_t& data )
{
	m_bDisabled = true;
	DisableFlag();
}

// Change the spawning location of the flag
void CFlagBase::InputChangeSpawnLocation( inputdata_t& data )
{
	data.value.Vector3D( m_vecSpawnOrigin );
}

void CFlagBase::Touch( CBaseEntity *pOther )
{
	if ( FClassnameIs( pOther, "lfc_trigger_disposer") )
	{
		ReturnToSpawn();
	}

	// 2nd dirtiest hack I've ever done
	if ( m_bDisabled || m_bFullyDisabled )
	{
		return;
	}

	// Dirtiest hack I have ever done
	if ( m_TimeDropped <= 0.0f )
	{
		m_TimeDropped = gpGlobals->curtime;
	}
	
	// If a player already has the flag, or if the flag is dropped and leeway time isn't over
	if ( m_bPlayerHasFlag || ( m_bDropped && (gpGlobals->curtime - m_TimeDropped <= lf_flag_droptime.GetFloat()) ) )
	{
		return;
	}
	
	if ( pOther->IsPlayer() && pOther->IsAlive() )
	{
		CModPlayer *pPlayer = ToModPlayer( pOther );
		Assert( pPlayer );

		OnPlayerTouch( pPlayer );

		if ( m_bDropped && GetTeamNumber() == pOther->GetTeamNumber() )
		{
			m_pPlayerReturnedFlag = pPlayer;
			ReturnToSpawn();
		}
		// The Flag team shouldn't be the same as the player's to pickup
		else if ( pOther->GetTeamNumber() != GetTeamNumber() )
		{
			pPlayer->GiveFlag( this );
		}
	}
}

void CFlagBase::FireGameEvent(IGameEvent* pEvent)
{
	// check event type and print message
	if (!Q_strcmp( "phase_switch", pEvent->GetName() ) )
	{
		if (pEvent->GetInt( "phaseid" ) == PHASE_BUILD)
		{
			// Disable the flag, it's build phase
			if ( m_bPlayerHasFlag )
			{
				m_pPlayerWithFlag->ReturnFlag();
			}
			else if ( m_bDropped )
			{
				ReturnToSpawn();
			}

			m_bDisabled = true;
		}
		else if ( pEvent->GetInt( "phaseid" ) == PHASE_COMBAT)
		{
			// Enable the flag, it's combat phase
			if ( m_bPlayerHasFlag )
			{
				m_pPlayerWithFlag->ReturnFlag();
			}
			else if ( m_bDropped )
			{
				ReturnToSpawn();
			}

			m_bDisabled = false;
		}
		else
		{
			AssertMsg( 0, "Switched to invalid Phase" );
		}
	}
}

// Event on Pickup
void CFlagBase::OnPickupEvent()
{
#if 0
	CReliableBroadcastRecipientFilter filter;
	UserMessageBegin( filter, "UpdateFlagCarrier" );
		WRITE_LONG( HFLG_PICKUP );
		WRITE_LONG( m_pPlayerWithFlag->GetTeamNumber() );
		WRITE_LONG( GetTeamNumber() );
		WRITE_LONG( m_pPlayerWithFlag->GetUserID() );
	MessageEnd();
#endif // 0
	SetFlagStatus( HFLG_PICKUP, m_pPlayerWithFlag->GetTeamNumber(), GetTeamNumber(), m_pPlayerWithFlag->GetUserID() );

	IGameEvent* pEvent = gameeventmanager->CreateEvent( "flag_pickup", true );
	
	pEvent->SetInt( "userid", m_pPlayerWithFlag->GetUserID() );
	pEvent->SetInt( "teamid", m_pPlayerWithFlag->GetTeamNumber() );
	pEvent->SetInt( "flagteamid", GetTeamNumber() );
	gameeventmanager->FireEvent( pEvent, false );
}

void CFlagBase::OnScoreEvent()
{
	// No event for scoring yet
}

// Event on Drop
void CFlagBase::OnDropEvent()
{
#if 0
	CReliableBroadcastRecipientFilter filter;
	UserMessageBegin( filter, "UpdateFlagCarrier" );
		WRITE_LONG( HFLG_DROPPED_TIMER );
		WRITE_LONG( m_pPlayerWithFlag->GetTeamNumber() );
		WRITE_LONG( GetTeamNumber() );
		WRITE_LONG( m_pPlayerWithFlag->GetUserID() );
	MessageEnd();
#endif // 0

	SetFlagStatus( HFLG_DROPPED_TIMER, m_pPlayerWithFlag->GetTeamNumber(), GetTeamNumber(), m_pPlayerWithFlag->GetUserID() );

	IGameEvent* pEvent = gameeventmanager->CreateEvent("flag_dropped", true);
	
	pEvent->SetInt( "userid", m_pPlayerWithFlag->GetUserID() );
	pEvent->SetInt( "teamid", m_pPlayerWithFlag->GetTeamNumber() );
	pEvent->SetInt( "flagteamid", GetTeamNumber() );

	gameeventmanager->FireEvent( pEvent, false );
}

// Event handled on return
void CFlagBase::OnReturnEvent()
{
	// Our flag has been returned

	int userid = 0;
	int teamid = ( GetTeamNumber() == SDK_TEAM_RED ) ? SDK_TEAM_BLUE : SDK_TEAM_RED;

	// If it was actually a player that returned the flag
	if ( m_pPlayerReturnedFlag )
	{
		userid = m_pPlayerReturnedFlag->GetUserID();
	}

	// OMG, wtf - Hekar
	if ( m_pPlayerDroppedFlag )
	{
		teamid = m_pPlayerDroppedFlag->GetTeamNumber();
	}

#if 0
	CReliableBroadcastRecipientFilter filter;
	UserMessageBegin( filter, "UpdateFlagCarrier" );
		WRITE_LONG( HFLG_RETURNED );
		WRITE_LONG( teamid );
		WRITE_LONG( GetTeamNumber() );
		WRITE_LONG( userid );
	MessageEnd();
#endif // 0
	SetFlagStatus( HFLG_RETURNED, teamid, GetTeamNumber(), userid );

	IGameEvent* pEvent = gameeventmanager->CreateEvent( "flag_returned", true );
	pEvent->SetInt( "userid", userid );
	pEvent->SetInt( "teamid", teamid  );
	pEvent->SetInt( "flagteamid", GetTeamNumber() );
	gameeventmanager->FireEvent( pEvent, false );
}

// Output for flag pickup
void CFlagBase::OnPickupOutput()
{
	m_OnPickup.FireOutput( this, NULL, 0.0f );
}

// Output for scoring
void CFlagBase::OnScoreOutput()
{
	m_OnScore.FireOutput( this, NULL, 0.0f );
}

// Output for flag drop
void CFlagBase::OnDropOutput()
{
	m_OnDropped.FireOutput( this, NULL, 0.0f );
}

void CFlagBase::OnReturnOutput()
{
	m_OnReturned.FireOutput( this, NULL, 0.0f );
}

void CFlagBase::SetFlagStatus( int status, int teamid, int flagteamid, int userid )
{
	// Update flag status
	m_FlagStatus = status;
	m_StatusTeamId = teamid;
	m_StatusFlagTeamId = flagteamid;
	m_StatusUserId = userid;

	UpdateFlagStatus();
}

void CFlagBase::UpdateFlagStatus()
{
	CReliableBroadcastRecipientFilter filter;
	UserMessageBegin( filter, "UpdateFlagCarrier" );
		WRITE_LONG( m_FlagStatus );
		WRITE_LONG( m_StatusTeamId );
		WRITE_LONG( m_StatusFlagTeamId );
		WRITE_LONG( m_StatusUserId );

		switch ( m_FlagStatus )
		{
			case HFLG_DROPPED_TIMER:
			{
				WRITE_FLOAT( m_TimeTillReturn - gpGlobals->curtime );
				break;
			}
		}

	MessageEnd();
}

/**
 * \brief Update the flag status for a single player
 * \author hekar
 */
void CFlagBase::UpdateFlagStatus( CBasePlayer *pPlayer )
{
	CSingleUserRecipientFilter filter( pPlayer );
	filter.MakeReliable();

	UserMessageBegin( filter, "UpdateFlagCarrier" );
		WRITE_LONG( m_FlagStatus );
		WRITE_LONG( m_StatusTeamId );
		WRITE_LONG( m_StatusFlagTeamId );
		WRITE_LONG( m_StatusUserId );
		
		switch ( m_FlagStatus )
		{
			case HFLG_DROPPED_TIMER:
			{
				WRITE_FLOAT( m_TimeTillReturn - gpGlobals->curtime );
				break;
			}
		}

	MessageEnd();
}

void CFlagBase::EnableFlag()
{
	m_bFullyDisabled = false;
	SetRenderColorA( 255 );
}

void CFlagBase::DisableFlag()
{
	m_bFullyDisabled = true;
	SetRenderColorA( 0 );
}
