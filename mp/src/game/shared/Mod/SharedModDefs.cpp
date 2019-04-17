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
#include "SharedModDefs.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

const char *pszTeamNames[] =
{
	"#SDK_Team_Unassigned",
	"#SDK_Team_Spectator",
	"#SDK_Team_Blue",
	"#SDK_Team_Red",
};

//Tony; We need to precache all possible player models that we're going to use
const char *pszPossiblePlayerModels[] =
{
	"models/humans/group03/male_01.mdl",
	"models/humans/group03/male_02.mdl",
	"models/humans/group03/female_01.mdl",
	"models/humans/group03/male_03.mdl",
	"models/humans/group03/female_02.mdl",
	"models/humans/group03/male_04.mdl",
	"models/humans/group03/female_03.mdl",
	"models/humans/group03/male_05.mdl",
	"models/humans/group03/female_04.mdl",
	"models/humans/group03/male_06.mdl",
	"models/humans/group03/female_06.mdl",
	"models/humans/group03/male_07.mdl",
	"models/humans/group03/female_07.mdl",
	"models/humans/group03/male_08.mdl",
	"models/humans/group03/male_09.mdl",
	"models/combine_soldier.mdl",
	"models/combine_soldier_prisonguard.mdl",
	"models/combine_super_soldier.mdl",
	"models/police.mdl",
	NULL
};

// ----------------------------------------------------------------------------- //
// Global Weapon Definitions
// ----------------------------------------------------------------------------- //

//--------------------------------------------------------------------------------------------------------
static const char * s_WeaponAliasInfo[] =
{
	"none",		// WEAPON_NONE
	"357",
	"ar2",
	"crossbow",
	"crowbar",
	"frag",
	"shotgun",
	"slam",
	"smg1",
	"stunstick",
	"pistol",
	"physcannon",
	"rpg",
	"sdk_mp5",		// SDK_WEAPON_MP5
	"hl2_rpg",
	"sdk_shotgun",	// SDK_WEAPON_SHOTGUN
	"sdk_grenade",	// SDK_WEAPON_GRENADE
	"sdk_pistol",	// SDK_WEAPON_PISTOL
	"sdk_crowbar",	// SDK_WEAPON_CROWBAR
	"hl2_physcannon",
	"sdk_smg1",
	"hl2_ar2",
	"hl2_pistol",
	"hl2_shotgun",
	"hl2_crossbow",
	"hl2_frag",
	"hl2_357",
	"hl2_slam",
	"lf_build_cannon",
	"lf_combat_cannon",
	"lf_medium_shotgun",
	"lf_railgun",
	"lf_airgun",
	"lf_airblaster",
	"lf_grenade_launcher",
	"lf_conc",
	"lf_beacon",
	"lf_orbit_cannon",
	"sf132_freezer",
	"sf132_physcannon",
	"lf_quickpistol",
	"lf_submachinegun",
	"lf_engineerfrag",
	"lf_impact_grenade_launcher",
	"lf_medikit",

	NULL,		// WEAPON_NONE
};



//--------------------------------------------------------------------------------------------------------
//
// Given an alias, return the associated weapon ID
//
int AliasToWeaponID( const char *alias )
{
	if (alias)
	{
		for( int i=0; s_WeaponAliasInfo[i] != NULL; ++i )
			if (!Q_stricmp( s_WeaponAliasInfo[i], alias ))
				return i;
	}

	return WEAPON_NONE;
}

//--------------------------------------------------------------------------------------------------------
//
// Given a weapon ID, return its alias
//
const char *WeaponIDToAlias( int id )
{
	if ( (id >= WEAPON_MAX) || (id < 0) )
		return NULL;

	return s_WeaponAliasInfo[id];
}
