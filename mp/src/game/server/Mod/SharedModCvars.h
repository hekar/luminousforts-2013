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


#ifndef SHAREDMODCVARS_H
#define SHAREDMODCVARS_H

#ifdef _WIN32
#pragma once
#endif

///////////////////////////////////////
// Global Sourceforts convars	//
//////////////////////////////////////

// CClassicGameRules
extern ConVar lf_score_limit;

extern ConVar lf_block_red_limit;
extern ConVar lf_block_blue_limit;

extern ConVar lf_round_limit;

extern ConVar lf_build_length_first;
extern ConVar lf_build_length;
extern ConVar lf_combat_length_first;
extern ConVar lf_combat_length;

extern ConVar lf_block_health;
extern ConVar lf_block_unique_health;
extern ConVar lf_block_worth_system;

// CFlagClassic
extern ConVar lf_flag_droptime;
extern ConVar lf_flag_return;

// Block destruction
extern ConVar lf_block_destructable;

extern ConVar lfcus_block_red_limit;
extern ConVar lfcus_block_blue_limit;

extern ConVar lf_build_player_runspeed;
extern ConVar lf_build_player_sprintspeed;

extern ConVar lf_combat_default_drainrate;
extern ConVar lf_combat_default_restorerate;

extern ConVar lf_combat_charger_next_reactivate;

extern ConVar lf_reverse_flags;

extern ConVar lf_hand_spawn_enabled;
extern ConVar lf_hand_spawn_distance;

///////////////////////////////////////////
// End Global Sourceforts convars	//
/////////////////////////////////////////

#endif // SHAREDMODCVARS_H