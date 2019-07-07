/* ***** BEGIN LICENSE BLOCK *****
Version: MPL 1.1/LGPL 2.1/GPL 2.0

The contents of this file are subject to the Mozilla Public License Version
1.1 (the
	"License"); you may not use this file except in compliance with
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
either of the GNU General Public License Version 2 or later (the
	"GPL"),
...
the terms of any one of the MPL, the GPL or the LGPL.

***** END LICENSE BLOCK ***** */

#include "cbase.h"
#include "SharedModCvars.h"

// Score limit before map change
ConVar lf_score_limit(
	"lf_score_limit",
	"1000",
	FCVAR_REPLICATED,
	"Score limit for teams"
);

// Block spawn limits
ConVar lf_block_limit_type(
	"lf_block_limit_type",
	"1",
	FCVAR_NOTIFY | FCVAR_REPLICATED,
	"0: map scaled | 1: team based | 2: single block limit",
	true,
	0.0,
	true,
	20000.0f
);

ConVar lf_block_limit(
	"lf_block_limit",
	"500",
	FCVAR_NOTIFY | FCVAR_REPLICATED,
	"Block limit for both teams. Requires lf_block_limit_type = 2",
	true,
	0.0,
	true,
	20000.0f
);

ConVar lf_block_blue_limit(
	"lf_block_blue_limit",
	"500",
	FCVAR_NOTIFY | FCVAR_REPLICATED,
	"Blue Team block limit. Please do not abuse. Requires lf_block_limit_type = 1",
	true,
	0.0,
	true,
	20000.0f
);

ConVar lf_block_red_limit(
	"lf_block_red_limit",
	"500",
	FCVAR_NOTIFY | FCVAR_REPLICATED,
	"Red Team block limit. Please do not abuse. Requires lf_block_limit_type = 1",
	true,
	0.0,
	true,
	20000.0f
);

ConVar lf_block_lg_limit_factor(
	"lf_block_lg_limit_factor",
	"1.0",
	FCVAR_NOTIFY | FCVAR_REPLICATED,
	"Scale block limit for large maps 1.5",
	true,
	0.75,
	true,
	30.0f
);

ConVar lf_block_med_limit_factor(
	"lf_block_med_limit_factor",
	"1.0",
	FCVAR_NOTIFY | FCVAR_REPLICATED,
	"Scale block limit for medium maps 1.0",
	true,
	0.5,
	true,
	20.0f
);

ConVar lf_block_sm_limit_factor(
	"lf_block_sm_limit_factor",
	"0.75",
	FCVAR_NOTIFY | FCVAR_REPLICATED,
	"Scale block limit for small maps 0.75",
	true,
	0.25,
	true,
	10.0f
);

// Round limit before map change
ConVar lf_round_limit(
	"lf_round_limit",
	"2",
	FCVAR_REPLICATED,
	"Round limit"
);

ConVar lf_build_length_first(
	"lf_build_length_first",
	"900",
	FCVAR_REPLICATED,
	"Length of first build phase (seconds)"
);

ConVar lf_combat_length_first(
	"lf_combat_length_first",
	"800",
	FCVAR_REPLICATED,
	"Length of first combat phase (seconds)"
);

ConVar lf_build_length(
	"lf_build_length",
	"300",
	FCVAR_REPLICATED,
	"Length of build phases after first (seconds)"
);

ConVar lf_combat_length(
	"lf_combat_length",
	"300",
	FCVAR_REPLICATED,
	"Length of combat phases after first (seconds)"
);

// Health of a block
ConVar lf_block_health(
	"lf_block_health",
	"4000",
	FCVAR_REPLICATED,
	"Health of blocks"
);

ConVar lf_block_unique_health(
	"lf_block_unique_health",
	"1",
	FCVAR_NOTIFY,
	"Follow the mod settings for each block health"
);

ConVar lf_block_worth_system(
	"lf_block_worth_system",
	"1",
	FCVAR_NOTIFY,
	"Enable the new Worth system for blocks"
);

// Time before dropped flag is returned
ConVar lf_flag_droptime(
	"lf_flag_droptime",
	"3.0",
	FCVAR_REPLICATED,
	"seconds until player can pickup flag again",
	true,
	2.0f,
	true,
	5.0f
);

ConVar lf_flag_return(
	"lf_flag_return",
	"30.0",
	FCVAR_REPLICATED,
	"seconds until flag is returned",
	true,
	0.0f,
	true,
	180.0f
);

ConVar lf_block_destructable(
	"lf_block_destructable",
	"1",
	FCVAR_REPLICATED,
	"Enables/Disables block destruction"
);

ConVar lfcus_block_red_limit(
	"lfcus_block_red_limit",
	"500",
	FCVAR_REPLICATED,
	"block limit for red team"
);

ConVar lfcus_block_blue_limit(
	"lfcus_block_blue_limit",
	"500",
	FCVAR_REPLICATED,
	"block limit for red team"
);

ConVar lf_build_player_runspeed(
	"lf_build_player_runspeed",
	"240.0",
	FCVAR_ARCHIVE,
	"Players running speed (without holding shift) in SF", true, 0.0f,
	false, 0.0f);

ConVar lf_build_player_sprintspeed(
	"lf_build_player_sprintspeed",
	"320.0",
	FCVAR_ARCHIVE,
	"Players sprinting speed (holding shift) in SF", true, 0.0f,
	false, 0.0f);

ConVar lf_combat_default_drainrate(
	"lf_combat_default_drainrate",
	"40.0",
	FCVAR_NOTIFY,
	"Default drainrate for classes that do not have them specified (determines how fast stamina runs out)", true, 0.0f,
	false, 0.0f);

ConVar lf_combat_default_restorerate(
	"lf_combat_default_restorerate",
	"15.0",
	FCVAR_NOTIFY,
	"Default restorerate for classes that do not have them specified (determines how fast stamina regenerates)", true, 0.0f,
	false, 0.0f);

ConVar lf_combat_charger_next_reactivate(
	"lf_combat_charger_next_reactivate",
	"15.0",
	FCVAR_NOTIFY,
	"Delay until health charger is refilled", true, 0.0f,
	false, 0.0f);

ConVar lf_reverse_flags(
	"lf_reverse_flags",
	"0",
	FCVAR_NOTIFY,
	"Reverse CTF flags (ie. Red becomes blue, Blue becomes red)"
);

ConVar lf_hand_spawn_enabled(
	"lf_hand_spawn_enabled",
	"0",
	FCVAR_REPLICATED,
	"Disable Hand Spawner: 0 - Disabled, 6 - All, 2 - Blue, 4 - Red", true, 0, true, 6 );

ConVar lf_hand_spawn_distance(
	"lf_hand_spawn_distance",
	"128.0f",
	FCVAR_NOTIFY,
	"Maximum distance to spawn block"
);

