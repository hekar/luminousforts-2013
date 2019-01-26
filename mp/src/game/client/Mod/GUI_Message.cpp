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


/**
 * \file GUI_Message.cpp
 * \author Hekar Khani
 * Basic message dialog (ie. MessageBox.Show())
 */


#include "cbase.h"
#include <vgui_controls/Label.h>
#include "GUI_Message.h"
	
CREATE_GUI_INTERFACE( MessageInterface, message );

CGUIMessage::CGUIMessage( vgui::VPANEL parent ) :
	BaseClass( NULL, "GUIMessage" )
{	
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/SourceScheme.res", "SourceScheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/Message.res" );

	SetMaximizeButtonVisible( false );
	SetMinimizeButtonVisible( false );
	SetSizeable( false );

	SetMenuButtonResponsive( true );
	SetDeleteSelfOnClose( false );
}

CGUIMessage::~CGUIMessage ()
{
}

void CGUIMessage::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetMouseInputEnabled( true );
		Activate();
		RequestFocus();

		vgui::Label *message = dynamic_cast< vgui::Label * > ( FindChildByName( "MessageLabel" ) );
		message->SetText( m_czMessage );
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CGUIMessage::SetData(KeyValues *data)
{
	Q_strncpy( m_czTitle, data->GetString( "title" ), sizeof( m_czTitle ) );
	SetTitle( m_czTitle, true );

	Q_strncpy( m_czMessage, data->GetString( "message" ), sizeof( m_czMessage ) );
}


void CGUIMessage::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor( "Modfg", GetFgColor() ) );
	SetBgColor( Color( 0, 0, 0, 0 ) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );
}

void CGUIMessage::OnCommand( const char *command )
{
	if ( !Q_stricmp( command, "okay" ) )
	{
		ShowPanel( false );
	}

	BaseClass::OnCommand( command );
}
