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
	Contains global definitions and constants
	This is added through "sdk_shareddefs.h"
===============================================================*/

#ifndef SHARED_MOD_H
#define SHARED_MOD_H

#ifdef _WIN32
#pragma once
#endif

#include "Mod/SharedColors.h"
#include "Mod/SharedGlobals.h"
#include "Mod/SharedModDefs.h"
#include "Mod/SharedModCvars.h"
#include "Mod/BalanceMacros.h"
#ifdef CLIENT_DLL
	#include "Mod/ClientGlobals.h"
#else
	#include "Mod/ServerGlobals.h"
#endif

#define MOD_VERSION 1

//----------------------------------------------------------------------
// Mod compile conditions
//----------------------------------------------------------------------
#define MOD_ALPHA
#define MOD_ALPHA_PLAYER
#define MOD_ALPHA_PLAYER_MOVEMENT

// Use Construct Blocks
#define MOD_CONSTRUCT_BLOCKS

#define MOD_HAND_BLOCK_SPAWN

// Development release of LuminousForts.
// (Adds extra options for development)
#define MOD_DEV

// Beta features that may not make it into the next release.
#define MOD_BETA

// Features that are designated for releases in the distant
// future.
#define MOD_LATE

// Features that are to be removed.
#define MOD_DEPRECATED

// Plays a video in the space of the titlescreen's background image.
// #define MOD_BACKGROUND_VIDEO

// Overlays an image over the titlescreen's background video.
// #define MOD_BACKGROUND_IMAGE

#define NUKLEAR_GUI

#define GWEN_GUI

#define GWEN_NO_ANIMATION

//---------------------------------------------------------------
// Gamemode Definitions just enable the gamemodes.
// They do not effect entities.
//---------------------------------------------------------------
#define MOD_CLASSIC
#define MOD_CUSTOM

//---------------------------------------------------------------
// Backward Map Compatibility
//---------------------------------------------------------------
// Sourceforts 194 backward map compatiblity
// (Can only be enabled with MOD_CLASSIC enabled)
#ifdef MOD_CLASSIC
#define MOD_SF132
#define MOD_SF194
#endif

//----------------------------------------------------------------------
#define ProtectedMethod
#define PrivateMethod

// Identifier for each particular gamemode
enum GameModeID_enum
{
	GAMEMODE_NOGAME		= (1 << 0),
	GAMEMODE_SDKBASE	= (1 << 1),

	// Capture the flag
	GAMEMODE_CLASSIC	= (1 << 2),

	// Domination gamemode
	GAMEMODE_DOM		= (1 << 3),

	// Custom gamemode
	GAMEMODE_CUSTOM		= (1 << 4),

	// Sourceforts 194
	GAMEMODE_SF194		= (1 << 29),

	// Sourceforts 132
	GAMEMODE_SF132		= (1 << 30),

	// All of the gamemodes combined
	GAMEMODE_ALL		= (unsigned int)~0x00
};

// Event types to send to flag carrier hud element
enum FlagStates_enum
{
	HFLG_BASE = 0,
	HFLG_PICKUP,
	HFLG_DROPPED,
	HFLG_DROPPED_TIMER,
	HFLG_RETURNED
};

// Colors for the messagelog hud element
enum MessageColor_enum
{
	MESSCLR_RED = 0,
	MESSCLR_BLUE,
	MESSCLR_GREEN,
	MESSCLR_YELLOW,
	MESSCLR_PURPLE,
	MESSCLR_WHITE,
	MESSCLR_BLACK,

	MESSCLR_COUNT
};

// Redefinitions of Team macros
#define TEAM_START TEAM_SPECTATOR + 1
#define TEAM_BLUE SDK_TEAM_BLUE
#define TEAM_RED SDK_TEAM_RED
#define TEAM_LAST TEAM_RED
#define TEAM_COUNT TEAM_LAST + 1

// Maximum number of team?
#define TEAM_MAXCOUNT TEAM_LAST + 1

// Number of real teams
#define TRUE_TEAM_BLUE 0
#define TRUE_TEAM_RED 1
#define TRUE_TEAM_COUNT 2

// Bitwise masks for each team
enum TeamMask_enum
{
	MOD_TEAM_BOTH = (1 << 0),
	MOD_TEAM_BLUE = (1 << 1),
	MOD_TEAM_RED  = (1 << 2),
	MOD_TEAM_SPEC = (1 << 3)
};


// Freeze Progressbar types for the Hud Element
enum FreezeProgressType_t
{
	SFH_FREEZEPROGRESS_NONE = 0,
	SFH_FREEZEPROGRESS_HEALING,
	SFH_FREEZEPROGRESS_FREEZING,
	SFH_FREEZEPROGRESS_UNFREEZING,
	SFH_FREEZEPROGRESS_UNFREEZING_ENEMY,

	SFH_FREEZEPROGRESS_COUNT
};

enum BlockFreezerID
{
	// Block has been frozen by world
	FROZEN_BY_WORLD	= -1,

	// Block has been frozen by anyone,
	// so don't change the last freezer ID
	FROZEN_BY_NONE = -2,

	// Block has been frozen by a player,
	// so grab the userID from CBasePlayer
	FROZEN_BY_PLAYER = -3
};

#ifdef MOD_CLASSIC
#include "Classic_SharedDefs.h"
#endif // MOD_CLASSIC

// Include Some Extras for the Sourceforts 1.3.2 Gamemode
#ifdef MOD_SF132
#include "SF132_SharedDefs.h"
#endif // MOD_SF132

#endif // SHARED_MOD_H
