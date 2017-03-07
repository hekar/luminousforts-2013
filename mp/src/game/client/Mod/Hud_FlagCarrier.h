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


#ifndef HUD_FLAGCARRIER_H
#define HUD_FLAGCARRIER_H

#ifdef WIN32
#pragma once 
#endif // WIN32

#include "Hud_Base.h"

class CHudFlagCarrier : public CModHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudFlagCarrier, vgui::Panel );

public:
	CHudFlagCarrier( const char *pElementName );
	~CHudFlagCarrier ();
	void Init( void );
	void LevelInit( void );
	void Reset( void );
	void VidInit( void );

	bool ShouldDraw (void);
	bool NeedsUpdate ( void );
	void Update( void );
	void UpdateFlagState( int TeamIndex );

	// CODE DUPLICATION. FIXME!
	void UpdateTakenAvatar( int TeamIndex );

	void OnThink( void );

	void Paint( void );
	void PaintBackground( void );

	void DisplayText (wchar_t *pText, int x, int y, Color col);

	void MsgFunc_UpdateFlagCarrier( bf_read& data );
	
private:
	CPanelAnimationVar( vgui::HFont, m_hTextFont, "TextFont", "SFHUDLarge" );

	CPanelAnimationVarAliasType( float, m_flBlueAvatarX, "BlueAvatarX", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlueAvatarY, "BlueAvatarY", "2", "proportional_float" );

	CPanelAnimationVar( bool, m_DrawBlueBorder, "BlueBorder", "0" );
	CPanelAnimationVar( bool, m_DrawRedBorder, "RedBorder", "0" );

	CPanelAnimationVar( Color, m_BlueForegroundColor, "BlueTextColor", "255 255 255 255" );
	CPanelAnimationVar( Color, m_BlueBackgroundColor, "BlueBackgroundColor", "0 0 0 220" );
	CPanelAnimationVarAliasType( float, m_flBlueTextX, "BlueTextX", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlueTextY, "BlueTextY", "2", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flBlueRectX, "BlueRectX", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlueRectY, "BlueRectY", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlueRectWidth, "BlueRectWidth", "192", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlueRectHeight, "BlueRectHeight", "26", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flRedAvatarX, "RedAvatarX", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRedAvatarY, "RedAvatarY", "2", "proportional_float" );
	
	CPanelAnimationVar( Color, m_RedForegroundColor, "RedTextColor", "255 255 255 255" );
	CPanelAnimationVar( Color, m_RedBackgroundColor, "RedBackgroundColor", "0 0 0 220" );
	CPanelAnimationVarAliasType( float, m_flRedTextX, "RedTextX", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRedTextY, "RedTextY", "2", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flRedRectX, "RedRectX", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRedRectY, "RedRectY", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRedRectWidth, "RedRectWidth", "192", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRedRectHeight, "RedRectHeight", "26", "proportional_float" );

	CAvatarImage *m_pAvatar [TEAM_COUNT];

	float m_DroppedMaxTime;
	CountdownTimer m_DroppedTimer[TEAM_COUNT];

	int m_iTakenByPlayer [TEAM_COUNT];

	wchar_t m_Text [TEAM_COUNT][ 256 ];
	int m_FlagState [TEAM_COUNT];
	int m_LastFlagState [TEAM_COUNT];
};


#endif // HUD_FLAGCARRIER_H