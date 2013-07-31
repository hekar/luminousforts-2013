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


#ifndef MOD_COLOR_H
#define MOD_COLOR_H


///////////////////////////////////
// Sourceforts game colors	//
/////////////////////////////////

#define LF_CLR_WHITE 255, 255, 255, 255
#define LF_CLR_BLACK 0, 0, 0, 255

#define LF_CLR_TRANSPARENT 0, 0, 0, 0

#define LF_CLR_RED 128, 32, 16
#define LF_CLR_BLUE 0, 128, 220
#define LF_CLR_GREEN 0, 128, 0

#define LF_CLR_TEAM_RED LF_CLR_RED , 255
#define LF_CLR_TEAM_BLUE LF_CLR_BLUE , 255

#define LF_CLR_TEAM_UNASSIGNED LF_CLR_GREEN , 255

#define LF_CLR_TEAM_RED_LITE  LF_CLR_TEAM_RED
#define LF_CLR_TEAM_BLUE_LITE LF_CLR_TEAM_BLUE

#define LF_CLR_TEAM_RED_MED LF_CLR_TEAM_RED
#define LF_CLR_TEAM_BLUE_MED LF_CLR_TEAM_BLUE

#define LF_CLR_TEAM_RED_DARK LF_CLR_TEAM_RED
#define LF_CLR_TEAM_BLUE_DARK LF_CLR_TEAM_BLUE

///////////////////////////////////////
// End Sourceforts game colors	//
/////////////////////////////////////


#endif // MOD_COLOR_H