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
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include "vgui/IInput.h"
#include "vgui_bitmapbutton.h"
#include "ienginevgui.h"

#include <vgui_controls/Controls.h>

#include "GUI_MainMenu.h"
#include "GUI_Titlescreen.h"

CREATE_GUI_INTERFACE( TitleScreenInterface, titlescreen );

//-------------------- Background Image ---------------------//
CBackgroundImage::CBackgroundImage( Panel *Parent, VPANEL parent ) :
	ImagePanel( Parent, "BackgroundImage" )
{
	SetParent( parent );
	int w, h;
	surface()->GetScreenSize( w, h );

	SetSize( w, h );

	SetVisible( true );
	SetProportional( true );

	SetShouldScaleImage( true );
}

CBackgroundImage::~CBackgroundImage()
{
}

CTitleScreen::CTitleScreen( vgui::VPANEL parent ) :
	BaseClass( NULL, "TitleScreen" )
{
	VPANEL gamepanel = enginevgui->GetPanel( PANEL_GAMEUIDLL );
	SetParent( gamepanel );

	int sw, sh;
	surface()->GetScreenSize( sw, sh );
	SetBounds( 0, 0, sw, sh );

#ifdef MOD_BACKGROUND_VIDEO
	m_pLoopedBackground = new CBinkVideoPanel( 0, 0, sw, sh, GetVPanel() );
	m_pLoopedBackground->SetRepeat( true );
#endif

#ifdef MOD_BACKGROUND_IMAGE
	m_pBackground = new CBackgroundImage( this, GetVPanel() );
	m_pBackground->SetVisible( true );
#endif

#ifdef MOD_DEV
	m_pTitle = new Label( this, "Title_Label", "#lf_titlescreen_title_dev" );
#else
	m_pTitle = new Label( this, "Title_Label", "#lf_titlescreen_title" );
#endif

	SetProportional( true );
	SetMouseInputEnabled( false );
	SetKeyBoardInputEnabled( false );

	ipanel()->MoveToFront( GetVPanel() );

	InitSettings();
}

CTitleScreen::~CTitleScreen()
{
#ifdef MOD_BACKGROUND_IMAGE
	m_pBackground->SetParent( NULL );
	delete m_pBackground;
#endif

	delete m_pTitle;
}

void CTitleScreen::InitSettings()
{
	m_drawHover = false;
	m_hoverX = 0;
	m_hoverY = 0;
	m_hoverW = 0;
	m_hoverH = 0;
	m_hoverToolTip[0] = '\0';

	int sw, sh;
	surface()->GetScreenSize( sw, sh );
	SetBounds( 0, 0, sw, sh );

#ifdef MOD_BACKGROUND_VIDEO
	m_pLoopedBackground->SetPos( 0, 0 );
	m_pLoopedBackground->SetSize( sw, sh );
	m_pLoopedBackground->Activate();
	m_pLoopedBackground->Play( "media/valve.bik" );
#endif

#ifdef MOD_BACKGROUND_IMAGE
	m_pBackground->SetImage( "background/background_arora.vmt" );
	m_pBackground->SetAlpha( 32 );
	m_pBackground->SetPos( 0, 0 );
	m_pBackground->SetSize( sw, sh );
#endif

	m_pTitle->SetSize( sw, 128 );
	m_pTitle->SetPos( 0, sh / 4.25 );
	
	if ( !lf_title_mainmenu_justifyleft.GetBool() )
	{
		m_pTitle->SetContentAlignment( vgui::Label::a_center );
	}
	else
	{
		m_pTitle->SetPos( lf_title_mainmenu_justifyleft_offsetx.GetInt(), sh / 5 );
		m_pTitle->SetContentAlignment( vgui::Label::a_west );
	}

	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/sourcescheme.res", "sourcescheme" );
	HFont titleFont = vgui::scheme()->GetIScheme( scheme )->GetFont( "LFTitle" );
	m_hoverFont = vgui::scheme()->GetIScheme( scheme )->GetFont( "LFToolTip" );
	m_pTitle->SetFont( titleFont );
}

void CTitleScreen::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetVisible( true );
		RequestFocus();
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CTitleScreen::OnClose()
{
#ifdef MOD_BACKGROUND_VIDEO
	m_pLoopedBackground->OnClose();
#endif

	BaseClass::OnClose();
}

void CTitleScreen::OnScreenSizeChanged( int oldwide, int oldtall )
{
	InitSettings();
}

void CTitleScreen::OnThink( void )
{
	// if ( engine->IsConnected() ) // True in loading screen also
	if ( engine->IsInGame() )
	{
		SetAlpha( 64 );
#ifdef MOD_BACKGROUND_VIDEO
	if ( m_pLoopedBackground->IsPlaying() )
	{
		m_pLoopedBackground->Stop();
	}
#endif
	}
	else
	{
		SetAlpha( 255 );
	}
}

void CTitleScreen::DrawHover( int x0, int y0, int x1, int y1, const char *toolTip )
{
	m_drawHover = true;
	m_hoverX = x0;
	m_hoverY = y0;
	m_hoverW = x1 - x0;
	m_hoverH = y1 - y0;
	
	g_pVGuiLocalize->ConvertANSIToUnicode( toolTip, m_hoverToolTip, sizeof( m_hoverToolTip ) );
}

void CTitleScreen::HideHover()
{
	m_drawHover = false;
}

void CTitleScreen::Paint()
{
	if (m_drawHover)
	{
		int x0 = m_hoverX;
		int y0 = m_hoverY;
		int x1 = m_hoverX + m_hoverW;
		int y1 = m_hoverY + m_hoverH;

		//vgui::surface()->DrawSetColor(32,32,32,220);
		vgui::surface()->DrawSetTextColor(255,255,255,255);
		//vgui::surface()->DrawFilledRect(x0, y0, x1, y1); 
		vgui::surface()->DrawSetTextPos(x0 + ((x1-x0)/2) - 25, y0 + (y1-y0)/5);
		vgui::surface()->DrawSetTextFont(m_hoverFont);
		if ( m_hoverToolTip != NULL )
		{
			vgui::surface()->DrawPrintText( m_hoverToolTip, wcslen(m_hoverToolTip) );
		}
		DrawHollowBox(x0, y0, x1-x0, y1-y0, Color(16,32,128),255);
	}
}
