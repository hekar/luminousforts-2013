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


#ifndef HUD_CAPPOINTS_H
#define HUD_CAPPOINTS_H

#ifdef WIN32
#pragma once 
#endif // WIN32

#include "Hud_Base.h"
#include <vgui_controls/Panel.h>

class C_DomCaptureZone;

class CHudCapPoints : public CModHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudCapPoints, vgui::Panel );
public:
	CHudCapPoints( const char *pElementName );

	void Init( void );
	void LevelInit( void );
	void Reset( void );
	bool ShouldDraw( void );
	void Paint( void );

	bool AddCaptureZone( C_DomCaptureZone *capzone );

	void PaintBackground( void ) {}

private:
	CPanelAnimationVar( Color, m_BorderColorBlue, "BorderColorBlue", "0 128 220 255" );
	CPanelAnimationVar( Color, m_BorderColorRed, "BorderColorRed", "128 32 16 255" );
	CPanelAnimationVar( Color, m_BorderColorSpec, "BorderColorSpec", "128 128 128 255" );

	CPanelAnimationVar( Color, m_ProgressColorBlue, "ProgressColorBlue", "0 128 220 228" );
	CPanelAnimationVar( Color, m_ProgressColorRed, "ProgressColorRed", "128 32 16 228" );
	CPanelAnimationVar( Color, m_ProgressColorSpec, "ProgressColorSpec", "128 128 128 228" );

	CPanelAnimationVarAliasType( float, m_GapWidth, "GapWidth", "12", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_BarWidth, "BarWidth", "32", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_BarHeight, "BarHeight", "32", "proportional_float" );

	CUtlVector< C_DomCaptureZone * > m_CPs;
};


#endif // HUD_CAPPOINTS_H