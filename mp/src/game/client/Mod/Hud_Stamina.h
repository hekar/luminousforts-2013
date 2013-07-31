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


//========= Copyright © 1996-2008, Valve Corporation, All rights reserved. ============//
//
// Purpose: Simple Hud Element, based on hud_suitpower from hl2, to display stamina in SDK Template.
//
// $NoKeywords: $
//=====================================================================================//
#ifndef SDK_HUD_STAMINA_H
#define SDK_HUD_STAMINA_H

#ifdef _WIN32
#pragma once
#endif

#include "hudelement.h"
#include "hud_numericdisplay.h"
#include <vgui_controls/Panel.h>

using namespace vgui;

//-----------------------------------------------------------------------------
// Purpose: Shows the sprint power bar
//-----------------------------------------------------------------------------
class CHudStamina : public CHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudStamina, vgui::Panel );

public:
	CHudStamina( const char *pElementName );
	virtual void	Init( void );
	virtual void	Reset( void );
	virtual void	OnThink( void );
	bool			ShouldDraw( void );

protected:
	virtual void	Paint();

private:
	CPanelAnimationVar( bool, m_ShowText, "ShowText", "true" );

	CPanelAnimationVar( Color, m_StaminaColorBlue, "StaminaColorBlue", "255 160 0 255" );
	CPanelAnimationVar( Color, m_StaminaColorRed, "StaminaColorRed", "255 160 0 255" );

	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "SFHUDLarge" );
	CPanelAnimationVarAliasType( float, m_flTextX, "TextX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flTextY, "TextY", "20", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flStaminaPercentageX, "StaminaPercentageX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flStaminaPercentageY, "StaminaPercentageY", "20", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flBackX, "BackX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackY, "BackY", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackWidth, "BackWidth", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackHeight, "BackHeight", "10", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flIconX, "IconX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flIconY, "IconY", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flIconWidth, "IconWidth", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flIconHeight, "IconHeight", "10", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flBarInsetX, "BarInsetX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarInsetY, "BarInsetY", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarWidth, "BarWidth", "64", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarHeight, "BarHeight", "64", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarChunkWidth, "BarChunkWidth", "12", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBarChunkGap, "BarChunkGap", "0", "proportional_float" );

	unsigned int m_iTextureID;

	float m_flStamina;
	int m_flStaminaLow;
};	

#endif // SDK_HUD_STAMINA_H
