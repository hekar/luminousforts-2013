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


/*===============================================================
	Client

	Web panel for main menu. This is a global singleton and
	there can only be one window.

	Add in tabbed browsing and a better HTML widget

	Last Updated Jan 21, 2009
===============================================================*/

#include "cbase.h"
#include <vgui/ISystem.h>
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/Frame.h>
#include <vgui_controls/TextEntry.h>
#include <vgui_controls/HTML.h>
#include <filesystem.h>

#include "GUI_WebWindow.h"

CREATE_GUI_INTERFACE(WebWindowInterface, webwindowpanel);

// Open a webpage
void OpenWebWindow( const char *title, const char *url )
{
	if ( webwindowpanel->GetFullPanel()->IsVisible() )
	{
		webwindowpanel->GetFullPanel()->ShowPanel( false );
	}

	KeyValues *data = new KeyValues( "WebWindowData" );

	data->SetString( "title", title );
	data->SetString( "url", url );

	webwindowpanel->GetFullPanel()->SetData( data );
	
	webwindowpanel->Show();

	data->deleteThis();
}

// Path is relative to game folder
void OpenLocalPage( const char *title, const char *path )
{
	if ( webwindowpanel->GetFullPanel()->IsVisible() )
	{
		webwindowpanel->GetFullPanel()->ShowPanel( false );
	}

	// Need to do this to display a local html file
	KeyValues *data = new KeyValues( "WebWindowData" );
	data->SetString( "title", title );

	// it's a local HTML file
	char localURL[ _MAX_PATH + 7 ];
	Q_strncpy( localURL, "file://", sizeof( localURL ) );

	char pPathData[ _MAX_PATH ];
	g_pFullFileSystem->GetLocalPath( path, pPathData, sizeof(pPathData) );
	Q_strncat( localURL, pPathData, sizeof( localURL ), COPY_ALL_CHARACTERS );

	Msg( "%s", localURL );
	data->SetString( "url", localURL );

	webwindowpanel->GetFullPanel()->SetData( data );
		
	webwindowpanel->Show();

	data->deleteThis();
}

// Open a url with the webwindow
CON_COMMAND( smod_panel_weburl_show, NULL )
{
	// Maybe add size to args too?
	if ( args.ArgC() != 2 )
	{
		Msg( "Usage: webwindowsingle_show url\n" );
		return;
	}

	OpenWebWindow ( args [ 1 ], args [ 1 ] );
}

// Open a url with the webwindow and set the title
CON_COMMAND( smod_panel_webtitleurl_show, NULL )
{
	// Maybe add size to args too?
	if ( args.ArgC() <= 2 )
	{
		Msg( "Usage: webwindow_show <title> url\n" );
		return;
	}

	OpenWebWindow( args [ 1 ], args [ 2 ] );
}

// Hide the webwindow
CON_COMMAND( smod_panel_weburl_hide, NULL )
{
	webwindowpanel->Hide();
}

//
//	Name: CWebWindow
//	Author: Hekar Khani
//	Description: Web panel for main menu
//	Notes:
//
CWebWindow::CWebWindow( vgui::VPANEL parent ) : vgui::Frame( NULL, "WebWindow" )
{	
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/SourceScheme.res", "SourceScheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/WebWindow.res" );
	InvalidateLayout( true, true );

	SetVisible( false );
	SetTitle( "#lf_webwindow_loading", true );
	SetSizeable( true );
	SetMinimizeButtonVisible( true );
	SetMaximizeButtonVisible( true );

	m_LocationBar = NULL;
	m_LocationBar = dynamic_cast< vgui::TextEntry * > ( FindChildByName( "LocationBar", true ) );
	AssertMsg( m_LocationBar, "LocationBar" );

	m_czTitle[0] = '\0';
	m_czURL[0] = '\0';
}

CWebWindow::~CWebWindow()
{
}

vgui::Panel *CWebWindow::CreateControlByName( const char *controlName )
{
	if ( !Q_strcmp( controlName, "NewHTML" ) )
	{
		return new vgui::HTML( this, "HTMLMessage" );
	}
	else if ( !Q_strcmp( controlName, "TextEntry" ) )
	{
		return new vgui::TextEntry( this, "LocationBar" );
	}

	return BaseClass::CreateControlByName( controlName );
}

void CWebWindow::SetData(KeyValues *data)
{
	Q_strncpy( m_czTitle, data->GetString( "title" ), sizeof( m_czTitle ) );
	SetTitle( m_czTitle, true );

	Q_strncpy( m_czURL, data->GetString( "url" ), sizeof( m_czURL ) );
}

void CWebWindow::ShowPanel( bool bShow )
{
	int x, y;
	GetPos( x, y );
	if ( x < 0 || y < 0 )
	{
		SetPos( 0, 0 );
	}

	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetVisible( true );
		Activate ();
		SetMouseInputEnabled( true );
		RequestFocus ();
		m_pHTMLMessage->OpenURL( m_czURL, null, true );
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CWebWindow::OnStartLoading( const char *url, const char *target )
{
	if ( m_LocationBar )
	{
		m_LocationBar->SetText( "Loading..." );
	}
}

void CWebWindow::OnFinishedLoading( const char *url )
{
	if ( m_LocationBar )
	{
		m_LocationBar->SetText( url );
	}
}

ProtectedMethod void CWebWindow::Refresh()
{
	m_pHTMLMessage->Refresh();
}

ProtectedMethod void CWebWindow::Stop()
{
	m_pHTMLMessage->StopLoading();
}

ProtectedMethod void CWebWindow::OnCommand( const char *command )
{
	if ( !Q_strcmp( command, "refresh" ) )
	{
		Refresh();
	}
	else if ( !Q_strcmp( command, "stop" ) )
	{
		Stop();
	}
	else if ( !Q_strcmp( command, "openexternal") ) 
	{
		char location[1024];
		m_LocationBar->GetText( location, sizeof( location ) );
		vgui::system()->ShellExecute( "open", location );
	}

	BaseClass::OnCommand( command );
}

ProtectedMethod void CWebWindow::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );
}

ProtectedMethod void CWebWindow::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor("Modfg", GetFgColor()) );
	SetBgColor( Color(0, 0, 0, 0) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );

	m_pHTMLMessage = dynamic_cast< vgui::HTML * > ( FindChildByName("HTMLMessage") );
	AssertMsg( m_pHTMLMessage, "HTML Widget" );
}

