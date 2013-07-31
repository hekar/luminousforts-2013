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

#include "cbase.h"
#include <vgui/IPanel.h>
#include <vgui/ISurface.h>
#include <vgui_controls/Panel.h>
#include <vgui_controls/Tooltip.h>
#include "GUI_Titlescreen.h"
#include "MGUI_Button.h"

MButton::MButton( vgui::Panel *Parent, const char *Name ) :
	BaseClass( Parent, Name, L"Button" )
{
	m_command[0] = '\0';
	m_hoverText[0] = '\0';
	DrawFocusBox( false );
}

MButton::~MButton()
{
}

void MButton::DoClick()
{
	BaseClass::DoClick();
	if ( m_command )
	{
		engine->ClientCmd( m_command );
	}
}

void MButton::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
}

void MButton::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );

	const char *command = inResourceData->GetString( "concommand", NULL );
	if ( !command )
	{
		Warning( "No Concommand set for Button! %s\n", GetName() );
		SetEnabled( false );
		return;
	}

	Q_strncpy( m_command, command, sizeof( m_command ) );

	const char *hoverText = inResourceData->GetString( "hovertext", NULL );

	if ( hoverText )
	{
		Q_strncpy( m_hoverText, hoverText, sizeof( m_hoverText ) );
	}
}

void MButton::SetArmed(bool state)
{
	BaseClass::SetArmed(state);
	if (IsArmed() && m_hoverText[0] != '\0')
	{
		// Tell the titlescreen to draw the hover
		int x0, y0, x1, y1;
		GetClipRect( x0, y0, x1, y1 );
		titlescreen->GetFullPanel()->DrawHover( x0 + 125, y0 + 6, x1, y1, m_hoverText );
	}
	else
	{
		// Tell the titlescreen to stop the hover
		titlescreen->GetFullPanel()->HideHover();
	}
}
