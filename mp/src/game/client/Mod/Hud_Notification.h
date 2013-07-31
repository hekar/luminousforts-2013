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

#ifndef HUD_NOTIFICATION_H
#define HUD_NOTIFICATION_H

#ifdef WIN32
#pragma once
#endif // WIN32

#include "Hud_Base.h"
#include <vgui_controls/Panel.h>

#define ICON_COUNT 1
#define ICON_LIST_DISPLAY_AT_ONCE 3

struct IconNotify_t
{
	int x, y;
	CHudTexture *icon;
};

class CHudNotification : public vgui::Panel, public CModHudElement
{
	DECLARE_CLASS_SIMPLE( CHudNotification, vgui::Panel );
public:
	CHudNotification( const char* pElementName );
	~CHudNotification();

	void Init( void );
	void LevelInit( void );
	bool ShouldDraw (void);
	void Paint( void );

	void MsgFunc_NotificationIcon( bf_read& data );

private:
	CHudTexture *m_Textures[ ICON_COUNT ];
	CUtlVector< IconNotify_t * > m_IconList;

	CPanelAnimationVar( Color, m_Background, "Background", "0 0 0 0" );

	CPanelAnimationVarAliasType( float, m_flX, "X", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flY, "Y", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flWidth, "Width", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flHeight, "Height", "10", "proportional_float" );
};

#endif // HUD_NOTIFICATION_H