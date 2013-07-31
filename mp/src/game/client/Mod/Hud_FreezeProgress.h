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


#ifndef HUD_FREEZEPROGRESS_H
#define HUD_FREEZEPROGRESS_H

#ifdef _WIN32
#pragma once
#endif

#include "Mod/Hud_Base.h"
#include <vgui_controls/Panel.h>

class CHudFreezeProgress : public CModHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudFreezeProgress, vgui::Panel );

public:
	CHudFreezeProgress( const char* pElementName );
	~CHudFreezeProgress();

	void Init( void );
	void Reset( void );
	bool ShouldDraw( void );
	void Paint( void );

	void ApplySchemeSettings( vgui::IScheme* scheme );

	// Add msg handler
	void MsgFunc_UpdateFreezeProgress( bf_read& data );

private:
	bool m_bShow;
	int m_iProgressType; // FreezeProgressType_t
	float m_flProgress; // 0 - 100

	// Based on Team
	Color m_Foreground;
	Color m_Border;

	// Background of panel
	CPanelAnimationVar( Color, m_Background, "Background", "0 0 0 128" );

	CPanelAnimationVar( Color, m_RedProgressForeground, "RedBarForeground", "0 0 0 128" );
	CPanelAnimationVar( Color, m_BlueProgressForeground, "BlueBarForeground", "0 0 0 128" );
	CPanelAnimationVar( Color, m_ProgressBackground, "BarBackground", "0 0 0 128" );

	CPanelAnimationVar( Color, m_RedBorder, "RedBorder", "255 0 0 128" );
	CPanelAnimationVar( Color, m_BlueBorder, "BlueBorder", "0 0 255 128" );

	CPanelAnimationVarAliasType( float, m_flX, "X", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flY, "Y", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flWidth, "Width", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flHeight, "Height", "10", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flHeaderTextX, "HeaderTextX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flHeaderTextY, "HeaderTextY", "8", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flBarX, "BarX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarY, "BarY", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarWidth, "BarWidth", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarHeight, "BarHeight", "10", "proportional_float" );
};

#endif // HUD_FREEZEPROGRESS_H
