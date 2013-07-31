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
#ifdef MOD_SF132
#include "SF132_SharedDefs.h"

BuyWeapon_t g_WeaponCommands [SF132_BUY_WEAPON_COUNT] =
{
#if 0
	{"#sf132_item_pulse", 7, "sf132_buyitem weapon_hl2_ar2"},
	{"#sf132_item_slam", 10, "sf132_buyitem weapon_hl2_slam"},
	{"#sf132_item_shotgun", 12, "sf132_buyitem weapon_hl2_shotgun"},
	{"#sf132_item_357", 15, "sf132_buyitem weapon_hl2_357"},
	{"#sf132_item_rpg", 25, "sf132_buyitem weapon_hl2_rpg"},
	{"#sf132_item_crossbow", 30, "sf132_buyitem weapon_hl2_crossbow"},
	{"#sf132_item_pulse_ammo", 3, "sf132_buyitem AR2"},
	{"#sf132_item_357_ammo", 5, "sf132_buyitem 357"},
	{"#sf132_item_rpg_ammo", 8, "sf132_buyitem RPG_Round"},
	{"#sf132_item_crossbow_ammo", 10, "sf132_buyitem XBowBolt"}
#endif // 0

	{"Pulse Rifle", 7, "sf132_buyitem weapon_hl2_ar2"},
	{"Slam", 10, "sf132_buyitem weapon_hl2_slam"},
	{"Shotgun", 12, "sf132_buyitem weapon_hl2_shotgun"},
	{"357", 15, "sf132_buyitem weapon_hl2_357"},
	{"RPG Launcher", 25, "sf132_buyitem weapon_hl2_rpg"},
	{"Crossbow", 30, "sf132_buyitem weapon_hl2_crossbow"},
	{"Pulse Rifle Ammo", 3, "sf132_buyitem AR2"},
	{"357 Ammo", 5, "sf132_buyitem 357"},
	{"RPG Ammo", 8, "sf132_buyitem RPG_Round"},
	{"CrossBow Ammo", 10, "sf132_buyitem XBowBolt"}
};

#endif // MOD_SF132