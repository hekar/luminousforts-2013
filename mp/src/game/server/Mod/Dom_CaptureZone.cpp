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

#include "cbase.h"
#include "triggers.h"
#include "team.h"
#include "Mod/ClassicGameRules.h"
#include "Mod/Dom_SharedDefs.h"
#include "Mod/Dom_CaptureZone.h"

#define LF_CAPTURE_THINK_TIME 0.1f

LINK_ENTITY_TO_CLASS( lfd_capture_zone, CDomCaptureZone )
BEGIN_DATADESC( CDomCaptureZone )
	DEFINE_KEYFIELD( m_iSecondsToCapture, FIELD_INTEGER, "SecondsToCapture" ),
	DEFINE_KEYFIELD( m_bDisableInBuild, FIELD_INTEGER, "DisableInBuild" ),
	DEFINE_KEYFIELD( m_ZoneId, FIELD_INTEGER, "ZoneId" ),
	DEFINE_KEYFIELD( m_ParentZone1, FIELD_INTEGER, "ParentZone1" ),
	DEFINE_KEYFIELD( m_ParentZone2, FIELD_INTEGER, "ParentZone2" ),
	DEFINE_KEYFIELD( m_ParentZone3, FIELD_INTEGER, "ParentZone3" ),
	DEFINE_KEYFIELD( m_PointIncrease, FIELD_INTEGER, "PointIncrease" ),
	DEFINE_KEYFIELD( m_PointIncreaseInterval, FIELD_INTEGER, "PointIncreaseInterval" ),
	DEFINE_KEYFIELD( m_Locked, FIELD_BOOLEAN, "Locked" ),
	
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableCaptures", InputDisableCaptures ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableCaptures", InputEnableCaptures ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Lock", InputLock ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Unlock", InputUnlock ),

	DEFINE_OUTPUT( m_Capture, "OnCaptured" ),
	DEFINE_OUTPUT( m_CapturedBy, "OnCapturedBy" ),
	DEFINE_OUTPUT( m_Lost, "OnLost" ),
	
	DEFINE_THINKFUNC( Think ),
END_DATADESC()


IMPLEMENT_SERVERCLASS_ST( CDomCaptureZone, DT_DomCaptureZone )
	SendPropInt(SENDINFO(m_Locked), 8, SPROP_UNSIGNED),
	SendPropInt(SENDINFO(m_ZoneId), 8, SPROP_UNSIGNED),
	SendPropInt(SENDINFO(m_CapState), 8, SPROP_UNSIGNED),
	SendPropInt(SENDINFO(m_iCapturePercentage), 8, SPROP_UNSIGNED),
	SendPropInt(SENDINFO(m_iCapturingCount), 8, SPROP_UNSIGNED),
	SendPropInt(SENDINFO(m_iCapturingTeam), 8, SPROP_UNSIGNED),
END_SEND_TABLE()

CDomCaptureZone::CDomCaptureZone()
{
	m_bDisabled = false;
	m_bDisableInBuild = true;
	m_CapState = CAP_NONE;
	m_iCapturingCount = 0;
	m_iCapturingTeam = TEAM_SPECTATOR;
	m_RedTouching = 0;
	m_BlueTouching = 0;
	m_iSecondsToCapture = 12;
	m_Locked = false;
	m_ZoneId = 0;
	m_ParentZone1 = 0;
	m_ParentZone2 = 0;
	m_ParentZone3 = 0;
	m_iCapturePercentage = 0;
	m_PointIncreaseInterval = 0;
	m_PointIncrease = 0;

	m_TouchingPlayers = new CUtlVector< CBasePlayer * >(0, 15);
	for ( int i = 0; i < m_TouchingPlayers->Size(); i++ )
	{
		m_TouchingPlayers->Element( i ) = NULL;
	}
}

CDomCaptureZone::~CDomCaptureZone()
{
	delete m_TouchingPlayers;
}
	
void CDomCaptureZone::InitTrigger()
{
	SetSolid( GetParent() ? SOLID_VPHYSICS : SOLID_BSP );	
	AddSolidFlags( FSOLID_NOT_SOLID );
	if ( m_bDisabled )
	{
		RemoveSolidFlags( FSOLID_TRIGGER );	
	}
	else
	{
		AddSolidFlags( FSOLID_TRIGGER );	
	}

	SetMoveType( MOVETYPE_NONE );
	SetModel( STRING( GetModelName() ) );
	AddEffects( EF_NODRAW );

	m_TouchingPlayers->Purge();

	if ( HasSpawnFlags( SF_TRIG_TOUCH_DEBRIS ) )
	{
		CollisionProp()->AddSolidFlags( FSOLID_TRIGGER_TOUCH_DEBRIS );
	}
}

void CDomCaptureZone::Spawn()
{
	BaseClass::Spawn();
	InitTrigger();

	m_iCapturePercentage = 100;
	if ( GetTeamNumber() != TEAM_RED || GetTeamNumber() != TEAM_BLUE )
	{
		ChangeTeam( TEAM_SPECTATOR );
	}

	DevMsg( "Bounding Radius of Domination point is: %f\n", BoundingRadius() );

	SetNextThink( gpGlobals->curtime + LF_CAPTURE_THINK_TIME );
}

void CDomCaptureZone::OnPhase( int phase )
{
	if ( phase == PHASE_BUILD )
	{
		OnBuild();
	}
	else if ( phase == PHASE_COMBAT )
	{
		OnCombat();
	}
}

void CDomCaptureZone::StartTouch( CBaseEntity *pOther )
{
	if ( m_bDisabled || m_bDisableCaptures )
		return;

	if ( pOther->IsPlayer() )
	{
		CBasePlayer *pPlayer = ToBasePlayer( pOther );
		if ( !pPlayer )
		{
			return;
		}

		m_TouchingPlayers->AddToTail( pPlayer );
		SetNextThink( LF_CAPTURE_THINK_TIME );
	}
}

void CDomCaptureZone::EndTouch( CBaseEntity *pOther )
{
	if ( pOther->IsPlayer() )
	{
		CBasePlayer *pPlayer = ToBasePlayer( pOther );
		if ( !pPlayer )
		{
			return;
		}

		for ( int i = 0; i < m_TouchingPlayers->Size(); i++ )
		{
			if ( m_TouchingPlayers->Element( i ) == pPlayer )
			{
				m_TouchingPlayers->Remove( i );
			}
		}
	}
}

void CDomCaptureZone::Think()
{
	OnPhase( ClassicGameRules()->GetCurrentPhaseID() );

	UpdatePointsThink();
	CaptureThink();
	SetNextThink( gpGlobals->curtime + LF_CAPTURE_THINK_TIME );
}

void CDomCaptureZone::UpdatePointsThink()
{
	int teamnum = GetTeamNumber();
	if ( teamnum == TEAM_RED || teamnum == TEAM_BLUE )
	{
		if ( gpGlobals->curtime - m_LastPointIncrease > m_PointIncreaseInterval )
		{
			CTeam *pTeam = GetGlobalTeam( teamnum );
			if ( pTeam )
			{
				pTeam->AddCapturePoints( m_PointIncrease );
			}
		}

		m_LastPointIncrease = gpGlobals->curtime;
	}
}

void CDomCaptureZone::CaptureThink()
{
	if ( m_bDisabled || m_Locked )
	{
		m_iCapturingTeam = TEAM_SPECTATOR;
		m_iCapturePercentage = 100;
		return;
	}

	if ( m_TouchingPlayers->Size() > 0 )
	{
		m_RedTouching = 0;
		m_BlueTouching = 0;
		for ( int i = 0; i < m_TouchingPlayers->Size(); i++ )
		{
			CBasePlayer *pPlayer = ToBasePlayer( m_TouchingPlayers->Element( i ) );
			if ( !pPlayer )
				continue;

			int team = pPlayer->GetTeamNumber();
			if ( team == TEAM_RED )
			{
				m_RedTouching++;
			}
			else if ( team == TEAM_BLUE )
			{
				m_BlueTouching++;
			}
		}

		bool bothTeamsTouching = false;
		if ( ( m_RedTouching > 0 ) && ( m_BlueTouching > 0 ) )
		{
			bothTeamsTouching = true;
		}

		if ( !bothTeamsTouching )
		{
			if ( m_RedTouching > 0 )
			{
				RedTouching();
			}
			else if ( m_BlueTouching > 0 )
			{
				BlueTouching();
			}
		}
		else
		{
			BothTeamsTouching();
		}
	}
	else
	{
		NoTeamsTouching();
	}

	HandleCapture();
}

/**
 * Only Red is touching
 */
void CDomCaptureZone::RedTouching()
{
	if ( GetTeamNumber() == TEAM_RED )
	{
		m_CapState = CAP_OWNED_RED;
	}
	else
	{
		m_iCapturePercentage = ( m_CaptureTimer.GetElapsedTime() / (float)m_iSecondsToCapture ) * 100;
		m_iCapturingTeam = TEAM_RED;
		m_iCapturingCount = m_RedTouching;
		
		if ( GetTeamNumber() == TEAM_BLUE )
		{
			m_CapState = CAP_DRAIN_BLUE;
		}
		else if ( GetTeamNumber() == TEAM_SPECTATOR )
		{
			m_CapState = CAP_CAPTURE_RED;
		}

		if ( !m_CaptureTimer.HasStarted() )
		{
			m_CaptureTimer.Start( 0.0f );
		}
	}
}

/**
 * Only Blue is touching
 */
void CDomCaptureZone::BlueTouching()
{
	if ( GetTeamNumber() == TEAM_BLUE )
	{
		m_CapState = CAP_OWNED_BLUE;
	}
	else
	{
		m_iCapturePercentage = ( m_CaptureTimer.GetElapsedTime() / (float)m_iSecondsToCapture ) * 100;
		m_iCapturingTeam = TEAM_BLUE;
		m_iCapturingCount = m_BlueTouching;
		
		if ( GetTeamNumber() == TEAM_RED )
		{
			m_CapState = CAP_DRAIN_RED;
		}
		else if ( GetTeamNumber() == TEAM_SPECTATOR )
		{
			m_CapState = CAP_CAPTURE_BLUE;
		}

		if ( !m_CaptureTimer.HasStarted() )
		{
			m_CaptureTimer.Start( 0.0f );
		}
	}
}

void CDomCaptureZone::BothTeamsTouching()
{
	if ( GetTeamNumber() == TEAM_RED )
	{
		m_iCapturingCount = m_BlueTouching;
		m_CapState = CAP_STALLED_BLUE;
		m_CaptureTimer.Reset();
	}
	else if ( GetTeamNumber() == TEAM_BLUE )
	{
		m_iCapturingCount = m_RedTouching;
		m_CapState = CAP_STALLED_RED;
		m_CaptureTimer.Reset();
	}
}

void CDomCaptureZone::NoTeamsTouching()
{
	if ( m_iCapturingTeam == TEAM_RED && GetTeamNumber() != TEAM_RED )
	{
		m_iCapturePercentage = 100;
	}
	else if ( m_iCapturingTeam == TEAM_BLUE && GetTeamNumber() != TEAM_BLUE )
	{
		m_iCapturePercentage = 100;
	}

	m_CapState = CAP_NONE;
	m_iCapturingTeam = TEAM_SPECTATOR;
	m_CaptureTimer.Reset();
}

void CDomCaptureZone::HandleCapture()
{
	bool timerFinished = m_CaptureTimer.HasStarted() && 
		( m_CaptureTimer.GetElapsedTime() > ( float )m_iSecondsToCapture );
	
	if ( m_iCapturingTeam != GetTeamNumber() && GetTeamNumber() != TEAM_SPECTATOR && timerFinished )
	{
		ChangeTeam( TEAM_SPECTATOR );
		m_CaptureTimer.Reset();
		m_CaptureTimer.Start( 0.0f );
	}
	else if ( m_iCapturingTeam != GetTeamNumber() && GetTeamNumber() == TEAM_SPECTATOR 
		&& timerFinished )
	{
		if ( m_iCapturingTeam == TEAM_RED )
		{
			Msg( "RED TEAM HAS CAPTURED THE ZONE!!\n" );
		}
		else if ( m_iCapturingTeam == TEAM_BLUE )
		{
			Msg( "BLUE TEAM HAS CAPTURED THE ZONE!!\n" );
		}

		ChangeTeam( m_iCapturingTeam );
		m_Capture.FireOutput( this, this );
		
		variant_t cappingTeam;
		cappingTeam.SetInt( m_iCapturingTeam );
		m_CapturedBy.FireOutput( cappingTeam, this, this );
		
		variant_t unCappingTeam;
		unCappingTeam.SetInt( GetOtherTeamNumber( m_iCapturingTeam ) );
		m_Lost.FireOutput( unCappingTeam, this, this );
	}
}

void CDomCaptureZone::InputEnableCaptures( inputdata_t& inputdata )
{
	m_bDisableCaptures = false;
}

void CDomCaptureZone::InputDisableCaptures( inputdata_t& inputdata )
{
	m_bDisableCaptures = true;
}

void CDomCaptureZone::InputLock( inputdata_t& inputdata )
{
	m_Locked = MOD_TEAM_BOTH;
}

void CDomCaptureZone::InputUnlock( inputdata_t& inputdata )
{
	m_Locked = MOD_TEAM_BOTH;
}

void CDomCaptureZone::OnBuild()
{
	if ( m_bDisableInBuild )
	{
		m_bDisabled = true;
	}
}

void CDomCaptureZone::OnCombat()
{
	m_bDisabled = false;
}
