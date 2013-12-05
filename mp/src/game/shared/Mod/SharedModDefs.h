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
#ifndef SHAREDMODDEFS_H_
#define SHAREDMODDEFS_H_

#ifdef _WIN32
	#pragma once
#endif


#define SDK_GAME_DESCRIPTION	"LuminousForts 13.0"

#define SDK_USE_TEAMS
#define SDK_USE_PLAYERCLASSES
#define SDK_USE_STAMINA
#define SDK_USE_SPRINTING
#define SDK_USE_STAMINA
#define SDK_SHOOT_WHILE_SPRINTING
#define SDK_SHOOT_ON_LADDERS
#define SDK_SHOOT_WHILE_JUMPING

// Can your players go prone?
//#define SDK_USE_PRONE


#define SDK_TEAM_SPECTATOR TEAM_SPECTATOR
enum sdkteams_e
{
	SDK_TEAM_BLUE = 2,
	SDK_TEAM_RED = 3,
};

#define INITIAL_SPRINT_STAMINA_PENALTY 0
#define LOW_STAMINA_THRESHOLD	35

#define SDK_NUM_PLAYERCLASSES GameRules()->GetClassCount()
#define SDK_PLAYERCLASS_IMAGE_LENGTH 64

#define PLAYERCLASS_RANDOM		-2
#define PLAYERCLASS_UNDEFINED	-1

//Tony; using teams with classes, so make sure the team class panel names are defined.
#define PANEL_CLASS_BLUE		"class_blue"
#define PANEL_CLASS_RED			"class_red"

#define pszTeamBlueClasses GameRules()->GetClassNames( SDK_TEAM_BLUE )
#define pszTeamRedClasses GameRules()->GetClassNames( SDK_TEAM_RED )
//extern const char *pszTeamBlueClasses[];
//extern const char *pszTeamRedClasses[];

//Tony; We need to precache all possible player models that we're going to use
extern const char *pszPossiblePlayerModels[];

extern const char *pszTeamNames[];

extern void ModDebugMsg( const char *message );

//Tony; these defines handle the default speeds for all of these - all are listed regardless of which option is enabled.
#define SDK_DEFAULT_PLAYER_RUNSPEED			340
#define SDK_DEFAULT_PLAYER_SPRINTSPEED		440
#define SDK_DEFAULT_PLAYER_PRONESPEED		100

//--------------------------------------------------------------------------------------------------------
//
// Weapon IDs for all SDK Game weapons
//
typedef enum
{
	WEAPON_NONE = 0,

	SDK_WEAPON_NONE = WEAPON_NONE,
	SDK_WEAPON_MP5,
	SDK_WEAPON_RPG,
	SDK_WEAPON_SHOTGUN,
	SDK_WEAPON_GRENADE,
	SDK_WEAPON_PISTOL,
	SDK_WEAPON_CROWBAR,
	SDK_WEAPON_PHYSCANNON,
	SDK_WEAPON_SMG1,
	SDK_WEAPON_AR2,
	SDK_WEAPON_HL2_PISTOL,
	SDK_WEAPON_HL2_SHOTGUN,
	SDK_WEAPON_CROSSBOW,
	SDK_WEAPON_FRAG,
	SDK_WEAPON_357,
	SDK_WEAPON_SLAM,
	LF_WEAPON_BUILDCANNON,
	LF_WEAPON_COMBATCANNON,
	LF_WEAPON_MEDIUMSHOTGUN,
	LF_WEAPON_RAILGUN,
	LF_WEAPON_AIRGUN,
	LF_WEAPON_AIRBLASTER,
	LF_WEAPON_GRENADE_LAUNCHER,
	LF_WEAPON_CONC,
	LF_WEAPON_BEACON,
	LF_WEAPON_ORBIT_CANNON,
	SF132_WEAPON_FREEZER,
	SF132_WEAPON_PHYSCANNON,
	LF_WEAPON_QUICKPISTOL,
	LF_WEAPON_SUBMACHINEGUN,
	LF_WEAPON_ENGINEERFRAG,
	LF_WEAPON_IMPACTGL,
	LF_WEAPON_MEDIKIT,
	LF_WEAPON_BUILDTOOL,

	WEAPON_MAX,		// number of weapons weapon index
} SDKWeaponID;

#define MAX_WEAPON_COUNT WEAPON_MAX

typedef enum
{
	FM_AUTOMATIC = 0,
	FM_SEMIAUTOMATIC,
	FM_BURST,

} SDK_Weapon_Firemodes;

const char *WeaponIDToAlias( int id );
int AliasToWeaponID( const char *alias );

#define SDK_PLAYER_DEATH_TIME	3.0f	//Minimum Time before respawning

// Special Damage types
enum
{
	SDK_DMG_CUSTOM_NONE = 0,
	SDK_DMG_CUSTOM_SUICIDE,
};

// Player avoidance
#define PUSHAWAY_THINK_INTERVAL		(1.0f / 20.0f)


#endif /* SHAREDMODDEFS_H_ */
