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

	Video Player for Blink videos. Meant to play tutorials.
	I doubt I'll be able to add any type of seeking.

	I'll probably make a program in C# to easily get at
	Youtube versions of the	tutorial videos.

	This is Spagetti code :( - hekar

	Should be called tutorialplayer :/ - Me (Hekar)
	
	talking to myself...

	Last Updated Dec 25, 2009
===============================================================*/

#include "cbase.h"
#ifdef MOD_BINK
#include "FMOD/fmod_manager.h"
#include "engine/IEngineSound.h"

#include "GUI_BinkVideoPlayer.h"

// Create a list of tutorial videos
#define TUTORIAL_COUNT 10

const static char *g_TutorialList[ TUTORIAL_COUNT ] =
{
	"media/valve.bik",
	"media/valve.bik",
	"media/valve.bik",
	"media/valve.bik",
	"media/valve.bik",
	"media/valve.bik",
	"media/valve.bik",
	"media/valve.bik",
	"media/valve.bik",
	"media/valve.bik"
};


CREATE_GUI_INTERFACE(BinkInterface,binkwindowpanel)

// Toggle visibility of the bink panel
CON_COMMAND( smod_panel_binkvideo_toggle, "Video Player Window (Toggles display)" )
{
	binkwindowpanel->Toggle();
}

// Show bink panel
CON_COMMAND( smod_panel_binkvideo_show, "Show Video Player Window" )
{
	binkwindowpanel->Show();
}

CBinkWindow::CBinkWindow( vgui::VPANEL parent ) :
	BaseClass( NULL, "BinkWindow" )
{
	SetScheme(vgui::scheme()->LoadSchemeFromFile( "resource/SourceScheme.res", "SourceScheme" ) );
	LoadControlSettings( "resource/ui/BinkPlayer.res" );

	SetTitle( "#lf_video_tutorial_title", true );
	SetSizeable( false );

	SetMaximizeButtonVisible( false );
	SetMinimizeButtonVisible( false );

	m_bPlaying = false;
}

CBinkWindow::~CBinkWindow ()
{
}

void CBinkWindow::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetMouseInputEnabled( true );
		Activate();
		RequestFocus();

		FMODManager()->Pause();
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );

		FMODManager()->Resume();
	}
}

void CBinkWindow::Play( const char *filename )
{
	if ( m_bPlaying )
	{
		Stop();
	}

	int x, y;
	GetPos( x, y );
	int w, h;
	GetSize( w, h );

	int vpx = 200;
	int vpy = 0;

	int vph = h - 150;
	int vpw = w - vpx;
	m_pVideoPanel = new CBinkVideoPanel( vpx, vpy, vph, vpw, GetVPanel() );
	m_pVideoPanel->Play( filename );

	m_bPlaying = true;
}

void CBinkWindow::Stop()
{
	if ( !m_bPlaying )
	{
		return;
	}

	if ( m_pVideoPanel != NULL && m_pVideoPanel->IsPlaying() )
	{
		// Close the actual video playing portion
		m_pVideoPanel->Stop();

		// Delete the higher level wrapper panel
		delete m_pVideoPanel;
	}

	m_pVideoPanel = NULL;

	m_bPlaying = false;
}

void CBinkWindow::OnClose()
{
	Stop();
	BaseClass::OnClose();
	FMODManager()->Resume();
}

ProtectedMethod void CBinkWindow::OnCommand( const char *command )
{
	for (int i = 0; i < TUTORIAL_COUNT; i++)
	{
		char playnumber [16];

		Q_snprintf( playnumber, sizeof( playnumber ), "play_%d", (i + 1));

		if ( !Q_strcmp( command, playnumber ) )
		{
			Play( g_TutorialList [i] );
		}
	}

	if ( !Q_strcmp( command, "stop" ) )
	{
		Stop();
	}
	else if ( !Q_strcmp( command, "close" ) )
	{
		OnClose();
	}

	BaseClass::OnCommand( command );
}

#endif // MOD_BINK
