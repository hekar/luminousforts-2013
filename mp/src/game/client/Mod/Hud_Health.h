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


#ifndef HUD_HEALTH_H
#define HUD_HEALTH_H

#ifdef WIN32
#pragma once
#endif // WIN32

#include "Hud_Base.h"

#define HEALTH_TEXT_SIZE 32

using namespace vgui;

class CHudPlayerHealth : public CModHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudPlayerHealth, vgui::Panel );

public:
	CHudPlayerHealth( const char *pElementName );
	void Init( void );
	void VidInit( void );
	void Reset( void );
	bool ShouldDraw( void );
	void OnThink( void );
	void Paint( void );
	void MsgFunc_Damage( bf_read &msg );

private:
	CPanelAnimationVar( Color, m_BarColorBlue, "BarColorBlue", "0 128 220 128" );
	CPanelAnimationVar( Color, m_BarColorRed, "BarColorRed", "128 32 16 144" );
	CPanelAnimationVar( float, m_flBgAlpha, "BackgroundAlpha", "170" );

	CPanelAnimationVarAliasType( float, m_flBarInsetX, "BarInsetX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarInsetY, "BarInsetY", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarWidth, "BarWidth", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarHeight, "BarHeight", "10", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarChunkWidth, "BarChunkWidth", "12", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarChunkGap, "BarChunkGap", "0", "proportional_float" );

	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "SFHUDLarge" );
	CPanelAnimationVarAliasType( float, m_flTextX, "TextX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flTextY, "TextY", "20", "proportional_float" );

	unsigned int m_iTextureID;

	wchar_t m_cswHealthText[ HEALTH_TEXT_SIZE ];
	int m_iHealthTextLength;

	// old variables
	int	m_iMaxHealth;
	int	m_iHealth;
	int	m_iLastHealth;
	int	m_bitsDamage;
};


#endif // HUD_HEALTH_H