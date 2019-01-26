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

	Replaces the main menu found in gamedll.

	Last Updated -- Unknown --
===============================================================*/

#include "cbase.h"
#include <vgui/ISurface.h>
#include "vgui/IInput.h"
#include "vgui_bitmapbutton.h"
#include "ienginevgui.h"

#include <vgui_controls/Controls.h>
#include <vgui_controls/ImagePanel.h>

#include "MGUI_BorderLessButton.h"

#include "GUI_MainMenu.h"

ConVar lf_title_mainmenu_justifyleft_offsetx( "lf_title_mainmenu_justifyleft_offsetx", "100", FCVAR_ARCHIVE, "Offset of menu from the left (only when left justified)" );
ConVar lf_title_mainmenu_justifyleft( "lf_title_mainmenu_justifyleft", "0", FCVAR_ARCHIVE, "Justify main menu to the left" );

CREATE_GUI_INTERFACE( MainMenuInterface, mainmenupanel )

//-------------- Main Menu ---------------//
CMainMenu::CMainMenu( vgui::VPANEL parent ) :
	BaseClass( NULL, "MainMenu" )
{
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/SourceScheme.res", "SourceScheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/MainMenu.res" );

	m_pResumeButton = NULL;
	m_pDisconnectButton = NULL;

	InitSettings();
}

CMainMenu::~CMainMenu()
{
	delete m_pResumeButton;
	delete m_pDisconnectButton;
}

Panel *CMainMenu::CreateControlByName( const char *controlName )
{
	if ( !Q_stricmp( "BitmapButton", controlName ) )
	{
		Panel *button = new MButtonBorderless( this );

		//color32 col = { 255, 255, 255, 255 };
		//button->SetImage( CBitmapButton::BUTTON_ENABLED, "materials/vgui/button/button.vmt", col );

		return button;
	}
	else if ( !Q_stricmp( "ImagePanel", controlName )  )
	{
		return new ImagePanel( this, "image_panel" );
	}

	return BaseClass::CreateControlByName( controlName );
}

void CMainMenu::InitSettings()
{
	SetVisible( true );

	int sw, sh;
	surface()->GetScreenSize( sw, sh );

	int x, y, w, h;
	GetBounds( x, y, w, h );
	SetBounds( 8, sh - 100, w, sh );

	if ( lf_title_mainmenu_justifyleft.GetBool() )
	{
		// Left justify this menu for people without ` keys

		SetPos( lf_title_mainmenu_justifyleft_offsetx.GetInt(), sh / 4 );

		// Align all Labels to the left side
		for ( int i = 0; i < GetChildCount(); i++ )
		{
			vgui::Label *label = dynamic_cast< vgui::Label * > ( GetChild( i ) );
			if ( label )
			{
				label->SetContentAlignment( vgui::Label::a_west );
			}
		}
	}
	else
	{
		// Center main menu
		SetPos( ( sw - GetWide() ) / 2, sh / 3.5 );

		// Align all Labels to the left side
		for ( int i = 0; i < GetChildCount(); i++ )
		{
			vgui::Label *label = dynamic_cast< vgui::Label * > ( GetChild( i ) );
			if ( label )
			{
				label->SetContentAlignment( vgui::Label::a_center );
			}
		}
	}

	SetProportional( true );

	MakePopup();
}

void CMainMenu::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetMouseInputEnabled( true );
		RequestFocus();
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CMainMenu::OnScreenSizeChanged( int oldwide, int oldtall )
{
	InitSettings();
}

void CMainMenu::OnMousePressed( MouseCode code )
{
	if ( !IsBuildGroupEnabled() )
	{
		RequestFocus();
	}
	
	EditablePanel::OnMousePressed( code );
}

//------- Protected Functions -------------------//
void CMainMenu::OnCommand( const char *command )
{
	if ( Q_strstr( command, "engine" ) && Q_strstr( command, " " )  )
	{
		const int start = strlen( "engine" ) + 1;
		const char *cmd = (command + start);
		engine->ClientCmd( cmd );
	}
	if ( !Q_strcmp( command, "findservers" ) )
	{
		engine->ClientCmd( "gamemenucommand OpenServerBrowser" );
	}
	else if ( !Q_strcmp( command, "resume" ) )
	{
		engine->ClientCmd( "gamemenucommand ResumeGame" );
	}
	else if ( !Q_strcmp( command, "disconnect" ) )
	{
		engine->ClientCmd( "gamemenucommand Disconnect" );
	}
	else if ( !Q_strcmp( command, "createserver" ) )
	{
		engine->ClientCmd( "gamemenucommand OpenCreateMultiplayerGameDialog" );
		//engine->ClientCmd( "smod_panel_listenserver_show" );
	}
	else if ( !Q_strcmp( command, "options" ) )
	{
		engine->ClientCmd( "gamemenucommand OpenOptionsDialog" );
	}
	else if ( !Q_strcmp( command, "quit" ) )
	{
		engine->ClientCmd( "gamemenucommand Quit" );
	}
	else if ( !Q_strcmp( command, "credits" ) )
	{
		engine->ClientCmd( "smod_panel_credits_show" );
	}
	else if ( !Q_strcmp( command, "community" ) )
	{
		engine->ClientCmd( "smod_panel_community_toggle" );
	}
	else if ( !Q_strcmp( command, "tutorials" ) )
	{
		engine->ClientCmd( "smod_panel_tutorials_show" );
	}

	BaseClass::OnCommand( command );
}

void CMainMenu::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor( "Modfg", GetFgColor() ) );
	SetBgColor( Color(0, 0, 0, 0) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );

	m_pResumeButton = dynamic_cast< MButtonBorderless * >( FindChildByName("Resume") );
	m_pDisconnectButton = dynamic_cast< MButtonBorderless * >( FindChildByName("Disconnect") );
}

void CMainMenu::OnThink()
{
	// Save some CPU cycles by doing a check before
	// invalidating layouts
	bool engineInGame = engine->IsInGame();

	if ( m_InGame == engineInGame || !IsVisible() )
	{
		return;
	}

	if ( engineInGame )
	{
		m_pResumeButton->SetEnabled( true );
		m_pResumeButton->SetVisible( true );

		m_pDisconnectButton->SetEnabled( true );
		m_pDisconnectButton->SetVisible( true );
	}
	else
	{
		m_pResumeButton->SetEnabled( false );
		m_pResumeButton->SetVisible( false );

		m_pDisconnectButton->SetEnabled( false );
		m_pDisconnectButton->SetVisible( false );
	}

	m_InGame = engineInGame;

	BaseClass::OnThink();
}

// This is a helper for firing things on the first game load or a map load, etc.
#if 0
class CMainMenuGameSystem : public CAutoGameSystem
{
public:
	CMainMenuGameSystem()
	{
	}

	// On first loading of the game
	void PostInit()
	{
		// Move the window to the front and place it
		// over the GameTitlePanel (We need to do this
		// after the Panel has loaded).
		mainmenupanel->GetFullPanel()->InitSettings();
		mainmenupanel->GetFullPanel()->ShowPanel( true );
	}

	void Shutdown()
	{
		mainmenupanel->GetFullPanel()->ShowPanel( false );
	}
};

static CMainMenuGameSystem g_MainMenuHelper;
#endif // 0
