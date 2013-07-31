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
#include "MiscCvars.h"

// Limiting cvars? I should remove limits on classes, but the SF community might uproar if I do so
ConVar mp_limit_blue_classscout( "mp_limit_blue_class_scout", "-1", FCVAR_REPLICATED, "Class limit for Blue Scout" );
ConVar mp_limit_blue_classengineer( "mp_limit_blue_class_engineer", "-1", FCVAR_REPLICATED, "Class limit for Blue Engineer" );
ConVar mp_limit_blue_classsoldier(	"mp_limit_blue_class_soldier", "-1", FCVAR_REPLICATED, "Class limit for Blue Soldier" );
ConVar mp_limit_blue_classsniper( "mp_limit_blue_class_sniper", "-1", FCVAR_REPLICATED, "Class limit for Blue Sniper" );
ConVar mp_limit_blue_classrocketeer( "mp_limit_blue_class_rocketeer", "-1", FCVAR_REPLICATED, "Class limit for Blue Rocketeer" );

ConVar mp_limit_red_classscout( "mp_limit_red_class_scout", "-1", FCVAR_REPLICATED, "Class limit for Red Scout" );
ConVar mp_limit_red_classengineer( "mp_limit_red_class_engineer", "-1", FCVAR_REPLICATED, "Class limit for Red Engineer" );
ConVar mp_limit_red_classsoldier( "mp_limit_red_class_soldier", "-1", FCVAR_REPLICATED, "Class limit for Red Soldier" );
ConVar mp_limit_red_classsniper( "mp_limit_red_class_sniper", "-1", FCVAR_REPLICATED, "Class limit for Red Sniper" );
ConVar mp_limit_red_classrocketeer( "mp_limit_red_class_rocketeer", "-1", FCVAR_REPLICATED, "Class limit for Red Rocketeer" );
