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

	ToolWindow for the starting of listen servers. The goal
	is to provide a better user interface and replace the 
	titlescreen components that Valve made.

	Last Updated Feb 27, 2010
===============================================================*/

#include "cbase.h"
#if 0
#include <vgui/ISurface.h>
#include <vgui_controls/PropertySheet.h>
#include "Panel_Listen_Main.h"
#include "GUI_ListenServer.h"

CREATE_GUI_INTERFACE( ListenServerInterface, listenserver );

// Just show the singleton panel
CON_COMMAND( smod_panel_listenserver_show, NULL )
{
	listenserver->Show();
}

// Hide the panel
CON_COMMAND( smod_panel_listenserver_hide, NULL )
{
	listenserver->Hide();
}

//
//	Name: CGUIListenServer
//	Author: Hekar Khani
//	Description: GUI Panel to replace Valve's CreateServer
//	Notes: 
//
CGUIListenServer::CGUIListenServer( vgui::VPANEL parent ) :
	BaseClass( NULL, "GUIListenServer" )
{	
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/SourceScheme.res", "SourceScheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/ListenServer.res" );

	SetVisible( false );
	SetSizeable( false );
	SetMoveable( false );
	SetSmallCaption( true );

	SetMaximizeButtonVisible( false );
	SetMinimizeButtonVisible( false );
	SetCloseButtonVisible( false );
	
	SetMenuButtonResponsive( true );
	SetDeleteSelfOnClose( false );

	GetPropertySheet()->SetSmallTabs( false );

	AddPage( new CPanelListenMain( this ), "Main" );
}

CGUIListenServer::~CGUIListenServer ()
{
}

void CGUIListenServer::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		int x, y, w, h;
		vgui::surface()->GetWorkspaceBounds( x, y, w, h );
		SetPos( 0, 0 );
		SetSize( w, h );

		SetMouseInputEnabled( true );
		Activate();
		RequestFocus();
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

ProtectedMethod void CGUIListenServer::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor( "Modfg", GetFgColor() ) );
	SetBgColor( Color( 0, 0, 0, 0 ) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );
}
#endif // 0
