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


#ifndef HUD_FREEZESTATUS_H
#define HUD_FREEZESTATUS_H

#ifdef WIN32
#pragma once
#endif // WIN32

#include "Hud_Base.h"
#include "Hud_BlockHealth.h"

class CHudFreezeStatus : public CHUDBlockHealth
{
	DECLARE_CLASS_SIMPLE( CHudFreezeStatus, CHUDBlockHealth );
public:
	CHudFreezeStatus( const char *pElementName );

	void Init( void );
	void Reset( void );
	bool ShouldDraw( void );
	void Paint( void );

	void DrawBackground();
	void DrawFrozenName( int UserID );
	void DrawUnFrozenName( int UserID );

private:
	CPanelAnimationVar( Color, m_BorderColorBlue, "BorderColorBlue", "0 128 220 128" );
	CPanelAnimationVar( Color, m_BorderColorRed, "BorderColorRed", "128 32 16 144" );
	CPanelAnimationVar( float, m_flBgAlpha, "BackgroundAlpha", "170" );

	CPanelAnimationVarAliasType( float, m_flBackgroundX_Build, "BackgroundRectX_BuildPhase", "r100", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundY_Build, "BackgroundRectY_BuildPhase", "10", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundWidth_Build, "BackgroundRectWidth_BuildPhase", "230", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundHeight_Build, "BackgroundRectHeight_BuildPhase", "90", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flBackgroundX_Combat, "BackgroundRectX_CombatPhase", "r100", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundY_Combat, "BackgroundRectY_CombatPhase", "10", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundWidth_Combat, "BackgroundRectWidth_CombatPhase", "230", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundHeight_Combat, "BackgroundRectHeight_CombatPhase", "90", "proportional_float" );

	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "SFHUDLarge" );
	CPanelAnimationVarAliasType( float, m_flFrozenTextX, "FrozenTextX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flFrozenTextY, "FrozenTextY", "20", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flUnFrozenTextX, "UnFrozenTextX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flUnFrozenTextY, "UnFrozenTextY", "20", "proportional_float" );

	Color m_BorderColor;
	Color m_BackgroundColor;

	float m_flBackgroundX;
	float m_flBackgroundY;
	float m_flBackgroundWidth;
	float m_flBackgroundHeight;
};


#endif // HUD_FREEZESTATUS_H