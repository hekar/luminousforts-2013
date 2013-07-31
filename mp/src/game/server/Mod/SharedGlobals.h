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


#ifndef SHAREDGLOBALS_H
#define SHAREDGLOBALS_H

#ifdef WIN32
#pragma once
#endif

#include "KeyValues.h"
#include "utllinkedlist.h"

extern int GetOtherTeamNumber( int iTeamNum );
extern int TeamMaskToNumber( int TeamMask );

enum TeamColor_enum
{
	TEAM_CLR_NORMAL = 0,
	TEAM_CLR_LITE,
	TEAM_CLR_MED,
	TEAM_CLR_DARK
};

extern Color GetTeamColor( int nTeamIndex, int TeamColorType = TEAM_CLR_NORMAL );

extern int Q_StrReplace( const char *str, char *output, int size, const char *original, const char *replace );

#endif // SHAREDGLOBALS_H