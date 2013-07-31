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
#include "gamerules.h"
#include "Mod/CSpawnPoint.h"

// Spawnpoints that use the datadesc to determine their team number

const char *g_GenericSpawnPoints[ GENERIC_SPAWN_COUNT ] =
{
	"lf_player_spawnpoint",
	"lfcus_player_spawnpoint"
#ifdef MOD_SF194
	, "info_player_team_spawn"
#endif
};

const char *g_BlueSpawnPoints[ BLUE_SPAWN_COUNT ] =
{
	"info_player_blue"
#ifdef MOD_SF132
	, "info_player_combine"
#endif //MOD_SF132
};

const char *g_RedSpawnPoints[ RED_SPAWN_COUNT ] =
{
	"info_player_red"
#ifdef MOD_SF132
	, "info_player_rebel"
#endif //MOD_SF132
};

BEGIN_DATADESC(CSpawnPoint)

	// Keyfields
	DEFINE_KEYFIELD( m_bDisabled, FIELD_BOOLEAN, "StartDisabled" ),

	// Inputs
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),

END_DATADESC();

LINK_ENTITY_TO_CLASS( info_player_deathmatch, CSpawnPoint );

CSpawnPoint::CSpawnPoint()
{
}

BEGIN_DATADESC(CModSpawnPoint)

	DEFINE_KEYFIELD( m_iDesiredTeamNumber, FIELD_INTEGER, "Team" ),

END_DATADESC();

LINK_ENTITY_TO_CLASS( lf_player_spawnpoint, CModSpawnPoint );

void CModSpawnPoint::Spawn()
{
	BaseClass::Spawn();
	ChangeTeam( m_iDesiredTeamNumber );
}

/*-------------------------------------------------------------------------------------
	IF YOU DEFINE NEW SPAWNPOINT ENTITIES, YOU MUST ADD THEM TO THE SPAWNPOINT
	ARRAY DEFINED FOR THIS MODIFICATION (The array is defined in CSpawnPoint.*)
-------------------------------------------------------------------------------------*/
LINK_ENTITY_TO_CLASS( info_player_red, CRedSpawnPoint ); // backwards support
CRedSpawnPoint::CRedSpawnPoint()
{
	m_iDesiredTeamNumber = SDK_TEAM_RED;
}


LINK_ENTITY_TO_CLASS( info_player_blue, CBlueSpawnPoint ); // backwards support
CBlueSpawnPoint::CBlueSpawnPoint()
{
	m_iDesiredTeamNumber = SDK_TEAM_BLUE;
}
