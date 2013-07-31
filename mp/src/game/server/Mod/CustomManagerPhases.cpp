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
#include "Mod/CustomCvars.h"
#include "Mod/ClassicGameRules.h"
#include "Mod/CustomManagerPhases.h"

LINK_ENTITY_TO_CLASS( lfcus_manager_phase, CCustomManagerPhases );

BEGIN_DATADESC( CCustomManagerPhases )
	DEFINE_KEYFIELD( m_bHasBuildPhase, FIELD_BOOLEAN, "HasBuildPhase" ),
	DEFINE_KEYFIELD( m_bHasCombatPhase, FIELD_BOOLEAN, "HasCombatPhase" ),
	DEFINE_KEYFIELD( m_flBuildLong, FIELD_FLOAT, "BuildPhaseLong" ),
	DEFINE_KEYFIELD( m_flBuildShort, FIELD_FLOAT, "BuildPhaseShort" ),
	DEFINE_KEYFIELD( m_flCombatLong, FIELD_FLOAT, "CombatPhaseLong" ),
	DEFINE_KEYFIELD( m_flCombatShort, FIELD_FLOAT, "CombatPhaseShort" ),
	DEFINE_KEYFIELD( m_bRespawnOnPhaseChange, FIELD_BOOLEAN, "RespawnOnPhaseChange" ),

	DEFINE_INPUTFUNC( FIELD_VOID, "DisableBuildPhase", InputDisableBuildPhase ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableBuildPhase", InputEnableBuildPhase ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableCombatPhase", InputDisableCombatPhase ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableCombatPhase", InputEnableCombatPhase ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisablePhaseRespawn", InputEnablePhaseRespawn ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisablePhaseRespawn", InputEnablePhaseRespawn ),

	DEFINE_THINKFUNC( Think ),
END_DATADESC()

CCustomManagerPhases::CCustomManagerPhases()
{
	ListenForGameEvent( "phase_switch" );
	ListenForGameEvent( "phase_time" );

	lfcus_phase_nobuild.SetValue( !m_bHasBuildPhase );
	lfcus_phase_nocombat.SetValue( !m_bHasCombatPhase );

	lf_build_length_first.SetValue( m_flBuildLong );
	lf_build_length.SetValue( m_flBuildShort );

	lf_combat_length_first.SetValue( m_flCombatLong );
	lf_combat_length.SetValue( m_flCombatShort );

	lfcus_phase_respawnonswitch.SetValue( m_bRespawnOnPhaseChange );

	m_flLastTimeLeft = 0;
}

CCustomManagerPhases::~CCustomManagerPhases()
{
	StopListeningForAllEvents();
}

void CCustomManagerPhases::Spawn()
{
	BaseClass::Spawn();
	SetThink( &CCustomManagerPhases::Think );
	SetNextThink( 0.0f );
}

void CCustomManagerPhases::InputDisableBuildPhase(  inputdata_t& data )
{
	lfcus_phase_nobuild.SetValue( 1 );
}

void CCustomManagerPhases::InputEnableBuildPhase( inputdata_t& data  )
{
	lfcus_phase_nobuild.SetValue( 0 );
}

void CCustomManagerPhases::InputDisableCombatPhase(  inputdata_t& data  )
{
	lfcus_phase_nocombat.SetValue( 1 );
}

void CCustomManagerPhases::InputEnableCombatPhase(  inputdata_t& data  )
{
	lfcus_phase_nocombat.SetValue( 0 );
}

void CCustomManagerPhases::InputDisablePhaseRespawn(  inputdata_t& data  )
{
	lfcus_phase_respawnonswitch.SetValue( 0 );
}

void CCustomManagerPhases::InputEnablePhaseRespawn(  inputdata_t& data  )
{
	lfcus_phase_respawnonswitch.SetValue( 1);
}

void CCustomManagerPhases::Think()
{
	float phasetime = ClassicGameRules()->GetPhaseTimeLeft();
	
	// Update SecondChange every second
	if ( phasetime - m_flLastTimeLeft >= 1.0f )
	{
		m_flLastTimeLeft = phasetime;
		m_OnSecondChange.Set( phasetime, this, this );
	}

	SetNextThink( 1.0f );
}

void CCustomManagerPhases::FireGameEvent( IGameEvent* pEvent )
{
	if ( !Q_strcmp( "phase_switch", pEvent->GetName() ) )
	{
		if ( pEvent->GetInt( "phaseid" ) == PHASE_BUILD )
		{
			m_OnBuildPhase.FireOutput( this, this, 0 );
		}
		else if ( pEvent->GetInt( "phaseid" ) == PHASE_COMBAT )
		{
			m_OnCombatPhase.FireOutput( this, this, 0 );
		}
	}
	else if ( !Q_strcmp( "phase_time", pEvent->GetName() ) )
	{
		m_OnTimeLeftChange.Set( pEvent->GetInt( "newtime" ), this, this );
	}
}
