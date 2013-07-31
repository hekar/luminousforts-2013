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


#ifndef HUD_BLOCKHEALTH_H
#define HUD_BLOCKHEALTH_H

#include "vgui_entitypanel.h"
#include "Mod/Hud_Base.h"

using namespace vgui;

#define MAX_ID_STRING 256

class CHUDBlockHealth : public CModHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHUDBlockHealth, vgui::Panel );

public:
	CHUDBlockHealth( const char *pElementName, const char *pPanelName = "HudBlockHealth" );
	void Init( void );
	void VidInit( void );
	bool ShouldDraw( void );
	
	C_BaseEntity *ProcessTarget( void );

	void Paint( void );
	void PaintBackground( void );
	void ApplySchemeSettings( vgui::IScheme *scheme );

protected:
	CPanelAnimationVar( vgui::HFont, m_hFont, "TextFont", "SFHUDLarge" );

	CPanelAnimationVar( bool, m_bCenter, "CenterBesideCrosshair", "true" )

	CPanelAnimationVar( Color, m_BackgroundColorBlue, "BackgroundColorBlue", "0 0 0 200" );
	CPanelAnimationVar( Color, m_BackgroundColorRed, "BackgroundColorRed", "0 0 0 200" );
	CPanelAnimationVar( Color, m_BorderColorBlue, "BorderColorBlue", "0 0 0 200" );
	CPanelAnimationVar( Color, m_BorderColorRed, "BorderColorRed", "0 0 0 200" );

	CPanelAnimationVarAliasType( float, m_flBlockTextX, "BlockTextX", "10", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlockTextY, "BlockTextY", "10", "proportional_float" );

	bool m_bBlockInTarget;
	int	m_iLastEntIndex;
	float m_flLastChangeTime;

private:
	CPanelAnimationVarAliasType( float, m_flBackgroundX, "BackgroundRectX", "r100", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundY, "BackgroundRectY", "10", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundWidth, "BackgroundRectWidth", "230", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundHeight, "BackgroundRectHeight", "90", "proportional_float" );
};

#endif // HUD_BLOCKHEALTH_H
