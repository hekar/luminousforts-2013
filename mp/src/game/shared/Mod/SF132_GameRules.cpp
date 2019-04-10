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
	Shared
	Sourceforts 1.3.2 Gamemode

	Sourceforts 1.3.2 gamerules derived from the ClassicGameRules

	Last Updated Feb 27, 2010
===============================================================*/

#include "cbase.h"
#ifdef MOD_SF132
#include "SF132_GameRules.h"

// Register the gamerules class
// This must be done for each class.
REGISTER_GAMERULES_CLASS( CSF132GameRules );

#define SF132_GAME_DESCRIPTION_BUILD "Sourceforts 1.3.2 Build!"
#define SF132_GAME_DESCRIPTION_COMBAT "Sourceforts 1.3.2 Fight!"

ConVar	mp_limit_blue_class_sf132( "mp_limit_blue_class_sf132", "-1", FCVAR_REPLICATED, "Class limit for Sourceforts 132 main class BLUE" );
ConVar	mp_limit_red_class_sf132( "mp_limit_red_class_sf132", "-1", FCVAR_REPLICATED, "Class limit for Sourceforts 132 main class RED" );

#define SF132_GAME_CLASSCOUNT 1

char *g_SF132GameRules_Classes_Blue [SF132_GAME_CLASSCOUNT + 1] =
{
	"blue_sf132",
	NULL
};

char *g_SF132GameRules_Classes_Red [SF132_GAME_CLASSCOUNT + 1] =
{
	"red_sf132",
	NULL
};

bool CSF132GameRules::Init( void )
{
	return BaseClass::Init();
}
#ifndef CLIENT_DLL
CSF132GameRules::CSF132GameRules()
{
}

CSF132GameRules::~CSF132GameRules()
{
}

const char *CSF132GameRules::GetGameDescription( void )
{
	if ( IsBuildPhase() )
	{
		return SF132_GAME_DESCRIPTION_BUILD;
	}
	else
	{
		return SF132_GAME_DESCRIPTION_COMBAT;
	}
}

bool CSF132GameRules::IsAllowedToSpawn( CBaseEntity *pEntity )
{
	return BaseClass::IsAllowedToSpawn( pEntity );
}

void CSF132GameRules::GiveBuildPhaseItems( CHL2MP_Player *pPlayer )
{
	pPlayer->SetHitboxSet( 0 );
	pPlayer->GiveNamedItem( "weapon_sf132_physcannon" );
	pPlayer->GiveNamedItem( "weapon_sf132_freezer" );
	pPlayer->SetMaxSpeed( 300 );
}

void CSF132GameRules::PlayerSpawn( CBasePlayer *pBasePlayer )
{
	BaseClass::PlayerSpawn( pBasePlayer );
}

const char *CSF132GameRules::GetChatPrefix( bool bTeamOnly, CBasePlayer *pPlayer )
{
	return BaseClass::GetChatPrefix( bTeamOnly, pPlayer );
}

const char *CSF132GameRules::GetChatFormat( bool bTeamOnly, CBasePlayer *pPlayer )
{
	return BaseClass::GetChatFormat( bTeamOnly, pPlayer );
}
#endif

char **CSF132GameRules::GetClassNames( int TeamNum )
{
	Assert( TeamNum == SDK_TEAM_RED || TeamNum == SDK_TEAM_BLUE );
	if (TeamNum == SDK_TEAM_RED)
	{
		return g_SF132GameRules_Classes_Red;
	}
	else if (TeamNum == SDK_TEAM_BLUE)
	{
		return g_SF132GameRules_Classes_Blue;
	}

	Msg( "Bad Team Request %d\n", TeamNum );
	return NULL;
}

int CSF132GameRules::GetClassCount( void )
{
	return SF132_GAME_CLASSCOUNT;
}

const char *CSF132GameRules::GetClassmenuRed() const
{
	return "resource/ui/SF132_ClassMenu_Red.res";
}

const char *CSF132GameRules::GetClassmenuBlue() const
{
	return "resource/ui/SF132_ClassMenu_Blue.res";
}

#endif // MOD_SF132