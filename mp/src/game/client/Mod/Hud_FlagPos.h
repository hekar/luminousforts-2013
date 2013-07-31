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


#ifndef HUD_FLAGPOS_H
#define HUD_FLAGPOS_H

#ifdef _WIN32
#pragma once
#endif

#include "Hud_Base.h"
#include <vgui_controls/Panel.h>

class C_SDKPlayer;

class CHudFlagLocations : public CModHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudFlagLocations, vgui::Panel );

public:
	CHudFlagLocations( const char* pElementName );

	void ApplySchemeSettings( vgui::IScheme* scheme );
	void VidInit( void );
	bool ShouldDraw (void);

	void Paint( void );

	Vector GetFlagLocation( int Team );
	void UpdateFlagPosition( int Team, const Vector& Pos );

private:
	vgui::HFont m_hFont;
	wchar_t m_sIDString[256];

	CHudTexture *m_BlueTexture;
	CHudTexture *m_RedTexture;

	bool m_bNeedsUpdate;

	Vector m_vRedLocation;
	Vector m_vBlueLocation;
};

#endif // HUD_FLAGPOS_H