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
#include "FileConfig.h"
#include "Dom_GameRules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

REGISTER_GAMERULES_CLASS( CDomGameRules );

CFileConfig g_DGRConfig( "domgamerules", "config/gamemode/dom.txt" );

CDomGameRules::CDomGameRules()
{
	g_DGRConfig.Load();
	m_iBuildFallDamage = g_DGRConfig.All()->GetInt( "build_fall_damage", 0 );
	m_iCombatFallDamage = g_DGRConfig.All()->GetInt( "combat_fall_damage", 0 );
}

CDomGameRules::~CDomGameRules()
{
}

const char *CDomGameRules::GetClassmenuRed() const
{
	return "resource/ui/ClassMenu_Dom_Red.res";
}

const char *CDomGameRules::GetClassmenuBlue() const
{
	return "resource/ui/ClassMenu_Dom_Blue.res";
}

#ifndef CLIENT_DLL
const char *CDomGameRules::GetGameDescription( void )
{
	if ( GetCurrentPhaseID() == PHASE_BUILD )
	{
		return g_DGRConfig.All()->GetString( "dom_build", "Domination Build" );
	}
	else if ( GetCurrentPhaseID() == PHASE_COMBAT )
	{
		return g_DGRConfig.All()->GetString( "dom_combat", "Domination Combat" );
	}

	return g_DGRConfig.All()->GetString( "dom_none", "Domination" );
}

float CDomGameRules::FlPlayerFallDamage( CBasePlayer *pPlayer )
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

#endif // !CLIENT_DLL
