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


#ifndef SF132_SHAREDDEFS_H
#define SF132_SHAREDDEFS_H

#ifdef WIN32
#pragma once
#endif

#define SF132_TEAM_COMBINE	1
#define SF132_TEAM_REBELS	2

typedef struct BuyWeapon
{
	const char *WeaponName;
	int Price;
	const char *Command;
} BuyWeapon_t;

#define SF132_BUY_WEAPON_COUNT 10

extern BuyWeapon_t g_WeaponCommands [SF132_BUY_WEAPON_COUNT];


#endif // SF132_SHAREDDEFS_H