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

	A basic factory to initialize the gamerules dependant
	on each map's settings.

	(look at world.cpp for more info)

	Last Updated June 9, 2010
===============================================================*/

#include "cbase.h"
#include "Mod/GameRuleMaker.h"
#include "Mod/ClassicGameRules.h"

//
//	Name: CreateGameRule( GameModeID_enum GameMode )
//	Author: Hekar Khani
//	Description: Initializes the gamerules based on
//			selected argument and enabled gamemodes
//	Notes:
//
void CreateGameRule( GameModeID_enum GameMode )
{
	Msg("=================================================\n");
	Msg("[LF] Gamemode: ");
	switch( GameMode )
	{
//-------------------------------------------------
#ifdef MOD_SF132
	case GAMEMODE_SF132:
		CreateGameRulesObject( "CSF132GameRules" );
		Msg("Sourceforts 132");
		break;
#endif // MOD_SF132
//-------------------------------------------------
#ifdef MOD_CLASSIC
	case GAMEMODE_CLASSIC:
		CreateGameRulesObject( "CClassicGameRules" );
		Msg("CTF\n");
		break;
#endif // MOD_CLASSIC
	case GAMEMODE_DOM:
		CreateGameRulesObject( "CDomGameRules" );
		Msg("Domination\n");
		break;
//-------------------------------------------------
	default:
//-------------------------------------------------
#ifdef MOD_SF132
		// Check if the mapname is prefixed as a SF132 map and use
		// the 132 gamerules if so
		if ( Q_strstr( gpGlobals->mapname.ToCStr(), "sf132_" ) )
		{
			CreateGameRulesObject( "CSF132GameRules" );
			ClassicGameRules()->SetSourcefortsMap( true );
			Msg("Sourceforts 132\n");
		}
		else
#endif // MOD_SF132
//-------------------------------------------------
#ifdef MOD_CLASSIC
		{
			// The Classic gamerules work with 194 
			// and CTF based 195 maps
			CreateGameRulesObject( "CClassicGameRules" );
			ClassicGameRules()->SetSourcefortsMap( true );
			Msg("Sourceforts 194\n");
		}
#endif // MOD_CLASSIC
//-------------------------------------------------
		// This "break" is here in case MOD_SF132
		// is enabled and MOD_CLASSIC is disabled
		break;
	}
	Msg("=================================================\n");

	Assert( g_pGameRules );
	g_pGameRules->Init();
}
