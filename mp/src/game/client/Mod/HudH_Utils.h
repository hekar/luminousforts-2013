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


#ifndef HUD_UTILS_H
#define HUD_UTILS_H


#ifdef WIN32
#pragma once
#endif

#include "Mod/Hud_Base.h"

extern void InitFontTexture( CHudTexture *texture, const char *font, char font_char, const char *textname );

void DrawBackground( Color bgColor, int x, int y, int wide, int tall );

void DrawBorder( Color borderColor, int wide, int tall, int borderwidth = 2 );
void DrawEmptyBorder( Color borderColor, int x1, int y1, int x2, int y2, int thickness = 1 );
void DrawBrokenBorder( Color borderColor, int x, int y, int wide, int tall, int broken_w= 15, int broken_h = 15, int borderwidth = 2 );

#endif // HUD_UTILS_H
