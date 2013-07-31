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


#ifndef HUD_MESSAGE_H
#define HUD_MESSAGE_H

#ifdef _WIN32
#pragma once
#endif

#include "Hud_Base.h"
#include <vgui_controls/Panel.h>

#define MESSAGE_MAXLEN 128

struct logmessage_t
{
	int message_type;
	wchar_t message[ MESSAGE_MAXLEN ];
};

class CHudMessageLog : public CModHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudMessageLog, vgui::Panel );

public:
	CHudMessageLog( const char* pElementName );
	~CHudMessageLog();

	void Init( void );
	bool ShouldDraw (void);
	void Paint( void );

	void ApplySchemeSettings( vgui::IScheme* scheme );

	bool AppendMessage( const wchar_t *message, int message_type );

	// Complex protocol for messaging on this Hud element
	void MsgFunc_SendMessage( bf_read& data );

private:
	CUtlLinkedList< logmessage_t * > m_MessageList;

	CPanelAnimationVar( Color, m_Background, "Background", "0 0 0 128" );

	CPanelAnimationVarAliasType( float, m_flX, "X", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flY, "Y", "8", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flWidth, "Width", "74", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flHeight, "Height", "10", "proportional_float" );
};

extern void MessageLog_Write( const wchar_t *message, int message_type );

#endif // HUD_MESSAGE_H