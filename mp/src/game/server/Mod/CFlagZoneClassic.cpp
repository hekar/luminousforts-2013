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
	Brush Entity

	Capture zone for "Capture the Flag" for classic gamemode.

	Last Updated Oct 30 2009
===============================================================*/
#include "cbase.h"
#include "hl2mp_player.h"
#include "team.h"
#include "Mod/SharedModCvars.h"
#include "Mod/CFlagZoneBase.h"
#include "Mod/CFlagZoneClassic.h"


LINK_ENTITY_TO_CLASS( lfc_trigger_flagzone, CFlagZoneClassic );

BEGIN_DATADESC( CFlagZoneClassic )
	
	DEFINE_ENTITYFUNC( Touch ),

	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "team" ),

END_DATADESC()

//
//	Name: CFlagZoneClassic
//	Author: Hekar Khani
//	Description: This is this brush entity that adds capture points on touches
//	Notes:
//
CFlagZoneClassic::CFlagZoneClassic()
{
	ListenForGameEvent( "flag_pickup" );
	ListenForGameEvent( "flag_returned" );
}

CFlagZoneClassic::~CFlagZoneClassic()
{
	StopListeningForAllEvents();
}

void CFlagZoneClassic::FireGameEvent( IGameEvent* pEvent )
{
	if ( lf_reverse_flags.GetBool() )
	{
		return;
	}

	// Disable the point if the flag is not at base
	if ( !Q_strcmp( pEvent->GetName(), "flag_pickup" ) )
	{
		if ( pEvent->GetInt( "teamid" ) != GetTeamNumber() )
		{
			m_bDisabled = true;
		}
	}
	else if ( !Q_strcmp( pEvent->GetName(), "flag_returned" ) )
	{
		if ( pEvent->GetInt( "teamid" ) != GetTeamNumber() )
		{
			m_bDisabled = false;
		}
	}
}
