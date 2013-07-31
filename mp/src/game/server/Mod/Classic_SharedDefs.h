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
	Sourceforts
	Shared header file for SF's Classic Gamemode

	Contains global definitions and constants relating to
	the Classic Gamemode
	This is added through "SharedMod.h"
===============================================================*/

#ifndef SHARED_CLASSIC_H
#define SHARED_CLASSIC_H

#ifdef WIN32
#pragma once
#endif // WIN32

enum BlockTypes_enum
{
	BLOCK_START = 0,
	BLOCK_1x2 = BLOCK_START,
	BLOCK_1x3,
	BLOCK_1x4,
	BLOCK_1x5,
	BLOCK_2x2,
	BLOCK_2x3,
	BLOCK_2x4,
	BLOCK_2x5,
	BLOCK_3x3,
	BLOCK_3x4,
	BLOCK_3x5,

	BLOCK_COUNT,

	// SF 1.9.4 blocks
	BLOCK_1_5x4 = BLOCK_COUNT,
	BLOCK_1x1x2,
	
	BLOCK_LAST
};

// Entity Names for each Block
extern const char *g_szBlocks[ BLOCK_LAST ];

extern const char *g_szBlockSpawnerNames[ BLOCK_LAST ];

// Modelnames for each block
extern const char *g_szBlockModels[ BLOCK_LAST ];

// Identifier for each of the old classic phases
// found in SF.
enum ClassicGameMode_PhaseID_enum
{
	PHASE_NONE = 0,

	PHASE_BUILD = 1,
	// Build phase is basically where players only have a
	// a gravity and construct their forts.

	PHASE_COMBAT = 2,
	// Combat phase is when they get their gear and fight
	// using those forts to their advantage (or disadvantage.)

	PHASE_LAST
};


#endif // SHARED_CLASSIC_H