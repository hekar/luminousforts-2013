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


#ifndef HUD_PHASETIME_H
#define HUD_PHASETIME_H

#ifdef WIN32
#pragma once
#endif

#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/Panel.h>

#include "Hud_Base.h"

class CHudPhaseTime : public CModHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudPhaseTime, vgui::Panel );

public:
	CHudPhaseTime( const char *pElementName );
	~CHudPhaseTime ();
	void Init( void );
	void Reset( void );
	void VidInit( void );

	bool ShouldDraw( void );
	bool NeedsUpdate( void );
	void DisplayBlockCount( int x, int y, Color col );
	void DisplayTimer( int x, int y, Color col );
	void DisplayPhase( int x, int y, Color col );

	void Paint( void );
	void PaintBackground();
	void PaintBorder();

	void DisplayText( wchar_t *pText, int x, int y, Color col );

	void FireGameEvent( IGameEvent* pEvent );
	void MsgFunc_NewTime( bf_read &msg );

private:
	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "SFHUDLarge" );

	CPanelAnimationVar( bool, m_bRightJustify, "RightJustify", "true" )

	CPanelAnimationVarAliasType( float, m_flTimerX, "TimerX", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flTimerY, "TimerY", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlockPointsX, "BlockPointsX", "12", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlockPointsY, "BlockPointsY", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flPhaseIconX, "PhaseIconX", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flPhaseIconY, "PhaseIconY", "10", "proportional_float" );

	CPanelAnimationVar( Color, m_TimeColorBlue, "TimeColorBlue", "0 0 0 200" );
	CPanelAnimationVar( Color, m_TimeColorRed, "TimeColorRed", "0 0 0 200" );

	CPanelAnimationVar( Color, m_PhaseColorBlue, "PhaseColorBlue", "0 0 0 200" );
	CPanelAnimationVar( Color, m_PhaseColorRed, "PhaseColorRed", "0 0 0 200" );

	CPanelAnimationVar( Color, m_BlockColorBlue, "BlockColorBlue", "0 0 0 200" );
	CPanelAnimationVar( Color, m_BlockColorRed, "BlockColorRed", "0 0 0 200" );

	CPanelAnimationVar( Color, m_BorderColorBlue, "BorderColorBlue", "0 0 0 200" );
	CPanelAnimationVar( Color, m_BorderColorRed, "BorderColorRed", "0 0 0 200" );

	CPanelAnimationVar( Color, m_BackgroundColorBlue, "BackgroundColorBlue", "0 0 0 200" );
	CPanelAnimationVar( Color, m_BackgroundColorRed, "BackgroundColorRed", "0 0 0 200" );

	CPanelAnimationVarAliasType( float, m_flBackgroundX, "BackgroundRectX", "r100", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundY, "BackgroundRectY", "10", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundWidth, "BackgroundRectWidth", "230", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBackgroundHeight, "BackgroundRectHeight", "90", "proportional_float" );

	wchar_t m_TextPhase[256];

	CountdownTimer m_PhaseTimer;

	int m_Phasenum;
	int m_PhaseSeconds;
};


#endif // HUD_PHASETIME_H