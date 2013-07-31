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

	Panel for selecting what Tutorials one wants to access.
	IE. Video, Web Form, Demo Player etc.

	TODO:
		Add "game demo" player

	Last Updated Dec 08, 2009
===============================================================*/

#include "cbase.h"
#include "cdll_client_int.h"

#include <vgui/IVGui.h>
#include <vgui_controls/Controls.h>
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/Button.h>

#include "Mod/FileConfig.h"
#include "Mod/GUI_Tutorial.h"
#include "Mod/GUI_WebWindow.h"
#include "Mod/GUI_BinkVideoPlayer.h"

CFileConfig g_Tut( "tutorial", "config/gui/tutorial.txt" );

#define LF_WIKI_PAGE		"http://wiki.luminousforts.com/index.php?title=Main_Page"
#define LF_USERGUIDE_PAGE	"http://wiki.luminousforts.com/index.php?title=Game_Guide"

CREATE_GUI_INTERFACE(TutorialInterface, tutorialspanel)

// Just show the singleton panel
CON_COMMAND( smod_panel_tutorials_show, NULL )
{
	tutorialspanel->Toggle();
}

// Hide the panel
CON_COMMAND( smod_panel_tutorials_hide, NULL )
{
	tutorialspanel->Hide();
}

CTutorialPanel::CTutorialPanel (vgui::VPANEL parent) :
	vgui::Frame( NULL, "TutorialPanel" )
{
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/sourcescheme.res", "sourcescheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/Tutorial.res" );

	SetVisible( false );
	SetTitle( "#lf_tutorial_title", true );
	SetSizeable( false );

	SetMaximizeButtonVisible( false );
	SetMinimizeButtonVisible( false );
}

CTutorialPanel::~CTutorialPanel ()
{
}

void CTutorialPanel::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetMouseInputEnabled( true );
		Activate();
		MoveToCenterOfScreen();
		RequestFocus();
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CTutorialPanel::OnCommand( const char *command )
{
	if ( !Q_strcmp( command, "videotutorials" ) )
	{
		Close();
		// TODO: Fix
		//binkwindowpanel->GetFullPanel()->ShowPanel( true );
	}
	else if ( !Q_strcmp( command, "wiki" ) )
	{
		Close();
		const char *url = g_Tut.All()->GetString( "wiki", LF_WIKI_PAGE );
		OpenWebWindow( "Wiki", url );
	}
	else if ( !Q_strcmp( command, "userguide" ) )
	{
		Close();
		const char *url = g_Tut.All()->GetString( "userguide", LF_USERGUIDE_PAGE );
		OpenWebWindow( "User Guide", url );
	}

	BaseClass::OnCommand( command );
}

void CTutorialPanel::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor("Modfg", GetFgColor()) );
	SetBgColor( Color(0, 0, 0, 0) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );
}
