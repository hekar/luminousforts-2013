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
#include "Mod/GUI_Community.h"
#include "Mod/GUI_WebWindow.h"
#include "Mod/FileConfig.h"

static CFileConfig g_Comm( "community", "config/gui/community.txt" );

// Default routes if no configuration file is found
#define LF_URL_NEWS "http://luminousforts.appspot.com/news/index"
#define LF_URL_FORUMS "http://luminousforts.appspot.com/forum/index"
#define LF_URL_FAQ "http://luminousforts.appspot.com/faq/index"
#define LF_URL_IRC "http://mibbit.com/?channel=%23hlcoders&server=irc.gamesurge.net"
#define LF_URL_DEV "http://luminousforts.appspot.com/dev/index"

CREATE_GUI_INTERFACE( CommunityInterface, communitypanel );

// Just show the singleton panel
CON_COMMAND( smod_panel_community_toggle, NULL )
{
	communitypanel->Toggle();
}

// Hide the panel
CON_COMMAND( smod_panel_community_hide, NULL )
{
	communitypanel->Hide();
}

//
//	Name: CGUICommunity
//	Author: Hekar Khani
//	Description: Community portal to news, etc
//	Notes: 
//
CGUICommunity::CGUICommunity( vgui::VPANEL parent ) :
	BaseClass( NULL, "GUICommunity" )
{	
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/sourcescheme.res", "sourcescheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/Community.res" );

	SetVisible( false );
	SetTitle( "#lf_community_title", true );
	SetSizeable( false );

	SetMaximizeButtonVisible( false );
	SetMinimizeButtonVisible( false );
}

CGUICommunity::~CGUICommunity ()
{
}

void CGUICommunity::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
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

ProtectedMethod void CGUICommunity::OnCommand( const char *command )
{
	// TODO: Fix
	if ( !Q_strcmp( command, "news" ) )
	{
		Close();
		const char *url = g_Comm.All()->GetString( command, LF_URL_NEWS );
		OpenWebWindow( "#lf_community_news", url );
	}
	else if ( !Q_strcmp( command, "forums" ) )
	{
		Close();
		const char *url = g_Comm.All()->GetString( command, LF_URL_FORUMS );
		OpenWebWindow( "#lf_community_forums", url );
	}
	else if ( !Q_strcmp( command, "irc" ) )
	{
		Close();
		const char *url = g_Comm.All()->GetString( command, LF_URL_IRC );
		OpenWebWindow( "#lf_community_irc", url );
	}
	else if ( !Q_strcmp( command, "faq" ) )
	{
		Close();
		const char *url = g_Comm.All()->GetString( command, LF_URL_FAQ );
		OpenWebWindow( "#lf_community_faq", url );
	}
	else if ( !Q_strcmp( command, "dev" ) )
	{
		Close();
		const char *url = g_Comm.All()->GetString( command, LF_URL_DEV );
		OpenWebWindow( "#lf_community_dev", url );
	}

	BaseClass::OnCommand( command );
}

ProtectedMethod void CGUICommunity::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor("Modfg", GetFgColor()) );
	SetBgColor( Color(0, 0, 0, 0) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );
}
