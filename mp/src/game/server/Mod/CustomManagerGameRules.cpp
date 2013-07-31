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
#if 0
#include "Mod/CustomGameRules.h"
#include "Mod/CustomManagerGameRules.h"

#ifndef CLIENT_DLL
BEGIN_DATADESC( CCustomManagerGameRules )
	DEFINE_INPUTFUNC( FIELD_VOID, "BlueWins", InputBlueWins ),
	DEFINE_INPUTFUNC( FIELD_VOID, "RedWins", InputRedWins ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Draw", InputDraw ),

	DEFINE_OUTPUT( m_OnBlueWins, "OnBlueWins" ),
	DEFINE_OUTPUT( m_OnRedWins, "OnRedWins" ),
	DEFINE_OUTPUT( m_OnDraw, "OnDraw" ),
END_DATADESC()
#endif // !CLIENT_DLL

LINK_ENTITY_TO_CLASS( lfcus_manager_gamerule, CCustomManagerGameRules )

CCustomManagerGameRules::CCustomManagerGameRules()
{
	if ( GameRules()->GetGameMode() != GAMEMODE_CUSTOM )
	{
		m_bEnabled = false;
	}
}

#ifndef CLIENT_DLL
void CCustomManagerGameRules::InputBlueWins( inputdata_t& data )
{
	if ( m_bEnabled )
	{
		CustomGameRules()->SetWinningTeam( TEAM_BLUE );
		m_OnBlueWins.FireOutput( this, this, 0 );
	}
}

void CCustomManagerGameRules::InputRedWins( inputdata_t& data )
{
	if ( m_bEnabled )
	{
		CustomGameRules()->SetWinningTeam( TEAM_RED );
		m_OnRedWins.FireOutput( this, this, 0 );
	}
}

void CCustomManagerGameRules::InputDraw( inputdata_t& data )
{
	if ( m_bEnabled )
	{
		// Spectator means draw
		CustomGameRules()->SetWinningTeam( TEAM_SPECTATOR );
		m_OnDraw.FireOutput( this, this, 0 );
	}
}

#endif

#endif // 0
