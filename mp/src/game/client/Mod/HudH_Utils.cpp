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
#include <vgui_controls/Controls.h>
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include "Mod/HudH_Utils.h"

void InitFontTexture( CHudTexture *texture, const char *font, char font_char, const char *textname )
{
	vgui::HScheme scheme = vgui::scheme()->GetScheme( "ClientScheme" );
	texture->hFont = vgui::scheme()->GetIScheme(scheme)->GetFont( font, true );
	Q_strncpy( texture->szShortName, textname, sizeof( texture->szShortName ) );
	texture->rc.top = 0;
	texture->rc.left = 0;
	texture->rc.right = vgui::surface()->GetCharacterWidth( texture->hFont, texture->cCharacterInFont );
	texture->rc.bottom = vgui::surface()->GetFontTall( texture->hFont );
	texture->bRenderUsingFont = true;
	texture->cCharacterInFont = font_char;
}


void DrawBackground( Color bgColor, int x, int y, int wide, int tall )
{
	int x1, x2, y1, y2;
	vgui::surface()->DrawSetColor(bgColor);
	vgui::surface()->DrawSetTextColor(bgColor);

	x1 = x;
	x2 = wide;
	y1 = y;
	y2 = tall;

	vgui::surface()->DrawFilledRect( x1, y1, x2, y2 );
}


void DrawBorder( Color borderColor, int wide, int tall, int borderwidth )
{
	int x1, x2, y1, y2;

	vgui::surface()->DrawSetColor(borderColor);
	vgui::surface()->DrawSetTextColor(borderColor);

	// top --------------------------------------------------------------------
	x1 = 0;
	x2 = wide;
	y1 = 0;
	y2 = borderwidth;
	vgui::surface()->DrawFilledRect( x1, y1, x2, y2 );

	// bottom -----------------------------------------------------------------
	x1 = 0;
	x2 = wide;
	y1 = tall - borderwidth;
	y2 = tall;
	vgui::surface()->DrawFilledRect( x1, y1, x2, y2 );

	// left -------------------------------------------------------------------
	x1 = 0;
	x2 = borderwidth;
	y1 = 0;
	y2 = tall;
	vgui::surface()->DrawFilledRect( x1, y1, x2, y2 );

	// right ------------------------------------------------------------------
	x1 = wide - borderwidth;
	x2 = wide;
	y1 = 0;
	y2 = tall;
	vgui::surface()->DrawFilledRect( x1, y1, x2, y2 );
}

void DrawEmptyBorder( Color borderColor, int x1, int y1, int x2, int y2, int thickness )
{
	vgui::surface()->DrawSetColor( borderColor );
	vgui::surface()->DrawSetTextColor( borderColor );

	vgui::surface()->DrawOutlinedRect( x1, y1, x2, y2 );
	vgui::surface()->DrawOutlinedRect( x1 + 1 - thickness, y1 - thickness, x2 - 1 + thickness, y2 - 1 + thickness );
}

void DrawBrokenBorder( Color borderColor, int x, int y, int wide, int tall, int broken_w, int broken_h, int borderwidth )
{
	int x1, x2, y1, y2;

	vgui::surface()->DrawSetColor(borderColor);
	vgui::surface()->DrawSetTextColor(borderColor);

	// top --------------------------------------------------------------------
	x1 = x + broken_w;
	x2 = x + wide - broken_w - 1;
	y1 = y;
	y2 = y + borderwidth;
	vgui::surface()->DrawFilledRect( x1, y1, x2, y2 );

	// bottom -----------------------------------------------------------------
	x1 = x + broken_w;
	x2 = x + wide - broken_w - 1;
	y1 = y + tall - borderwidth;
	y2 = y + tall;
	vgui::surface()->DrawFilledRect( x1, y1, x2, y2 );

	// left -------------------------------------------------------------------
	x1 = x;
	x2 = x + borderwidth + 1;
	y1 = y + broken_h;
	y2 = y + tall + 1 - broken_h;
	vgui::surface()->DrawFilledRect( x1, y1, x2, y2 );

	// right ------------------------------------------------------------------
	x1 = x + wide - borderwidth;
	x2 = x + wide;
	y1 = y + broken_h;
	y2 = y + tall + 1 - broken_h;
	vgui::surface()->DrawFilledRect( x1, y1, x2, y2 );
}
