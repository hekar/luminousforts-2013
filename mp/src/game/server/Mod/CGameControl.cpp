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
	Logic Entity

	Similar to Sourcefort's gamecontrol entity. This just basically
	fires off a few outputs on events. Might give it some inputs.

	Last Updated Sept 05, 2009
===============================================================*/

#include "cbase.h"
#include "Mod/ClassicGameRules.h"
#include "Mod/CGameControl.h"

LINK_ENTITY_TO_CLASS( lf_logic_gamecontrol, CGameControl );
LINK_ENTITY_TO_CLASS( lfc_logic_gamecontrol, CGameControl );

BEGIN_DATADESC( CGameControl  )

	DEFINE_INPUTFUNC( FIELD_VOID, "TogglePhase", InputTogglePhase ),
	DEFINE_INPUTFUNC( FIELD_VOID, "SwitchToBuild", InputSwitchToBuild ),
	DEFINE_INPUTFUNC( FIELD_VOID, "SwitchToCombat", InputSwitchToCombat ),

	DEFINE_OUTPUT( m_OnBuild, "OnBuildPhase" ),
	DEFINE_OUTPUT( m_OnCombat, "OnCombatPhase" ),
	DEFINE_OUTPUT( m_OnSwitchPhase, "OnSwitchPhase" ),

END_DATADESC()

//
//	Name: CGameControl
//	Author: Hekar Khani
//	Description: Entity for phase event firings, etc
//
CGameControl::CGameControl()
{
	ListenForGameEvent( "phase_switch" );
}

CGameControl::~CGameControl ()
{
	StopListeningForAllEvents();
}

void CGameControl::InputTogglePhase( inputdata_t &inputdata )
{
	if ( GameRules()->GetGameModeMask() & GAMEMODE_CLASSIC )
	{
		if ( ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD )
		{
			ClassicGameRules()->SwitchToPhase( "CombatPhase", true );
		}
		else if ( ClassicGameRules()->GetCurrentPhaseID() == PHASE_COMBAT )
		{
			ClassicGameRules()->SwitchToPhase( "BuildPhase", true );
		}
	}
}

void CGameControl::InputSwitchToBuild( inputdata_t &inputdata )
{
	if ( GameRules()->GetGameModeMask() & GAMEMODE_CLASSIC )
	{
		ClassicGameRules()->SwitchToPhase( "BuildPhase", true );
	}
}

void CGameControl::InputSwitchToCombat( inputdata_t &inputdata )
{
	if ( GameRules()->GetGameModeMask() & GAMEMODE_CLASSIC )
	{
		ClassicGameRules()->SwitchToPhase( "CombatPhase", true );
	}	
}

void CGameControl::FireGameEvent( IGameEvent* pEvent )
{
	if ( !Q_strcmp( pEvent->GetName(), "phase_switch" ) )
	{
		if ( pEvent->GetInt( "phaseid" ) == PHASE_BUILD )
		{
			m_OnBuild.FireOutput( this, NULL, 0.0f );
		}
		else if ( pEvent->GetInt( "phaseid" ) == PHASE_COMBAT )
		{
			m_OnCombat.FireOutput( this, NULL, 0.0f );
		}

		m_OnSwitchPhase.FireOutput( this, NULL, 0.0f );
	}
}
