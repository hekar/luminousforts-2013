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


#ifndef HUD_INTERMISSION_H
#define HUD_INTERMISSION_H

#ifdef WIN32
#pragma once
#endif // WIN32

#include "Hud_Base.h"
#include <vgui_controls/Panel.h>

namespace vgui
{
	class Label;
};

class CHudIntermission : public CModHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudIntermission, vgui::Panel );
public:
	CHudIntermission( const char *pElementName );

	void Init( void );
	void VidInit( void );
	void Reset( void );
	bool ShouldDraw( void );
	void Paint( void );

	// Add a message for displaying

private:
	vgui::Label *m_BlueScore;
	vgui::Label *m_RedScore;

	int m_iLastBlueScore;
	int m_iLastRedScore;

	CPanelAnimationVarAliasType( float, m_flX, "PosX", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flY, "PosY", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flWidth, "PosWidth", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flHeight, "PosHeight", "10", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flTextureX, "TextureX", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flTextureY, "TextureY", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flTextureWidth, "TextureWidth", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flTextureHeight, "TextureHeight", "10", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flBlueScoreX, "BlueScoreX", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlueScoreY, "BlueScoreY", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlueScoreWidth, "BlueScoreWidth", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flBlueScoreHeight, "BlueScoreHeight", "10", "proportional_float" );

	CPanelAnimationVarAliasType( float, m_flRedScoreX, "RedScoreX", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRedScoreY, "RedScoreY", "0", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRedScoreWidth, "RedScoreWidth", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRedScoreHeight, "RedScoreHeight", "10", "proportional_float" );

	int m_iWinningTeam;
	bool m_bIntermission;

	// Texture ID for the display of the winning team
	unsigned int m_iWinningTextureID[ TRUE_TEAM_COUNT ];
};


#endif // HUD_INTERMISSION_H