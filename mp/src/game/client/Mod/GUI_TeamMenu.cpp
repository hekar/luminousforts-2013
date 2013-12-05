//======== Copyright � 1996-2008, Valve Corporation, All rights reserved. =========//
//
// Purpose: 
//
// $NoKeywords: $
//=================================================================================//

#include "cbase.h"
#include <stdio.h>

#include <cdll_client_int.h>

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <KeyValues.h>
#include <vgui_controls/ImageList.h>
#include <filesystem.h>
#include <game/client/iviewport.h>

#include <vgui_controls/TextEntry.h>
#include <vgui_controls/Button.h>
#include <vgui_controls/Panel.h>

#include "cdll_util.h"

#include <game/client/iviewport.h>

#include "Mod/GUI_ClassMenu.h"
#include "Mod/GUI_TeamMenu.h"
#include "Mod/C_ModPlayer.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

CON_COMMAND_F( lf_hud_teammenu, "Choose a new team", FCVAR_SERVER_CAN_EXECUTE|FCVAR_CLIENTCMD_CAN_EXECUTE )
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();

	if ( pPlayer )
	{
		gViewPortInterface->ShowPanel( PANEL_TEAM, true );
	}
}

CModTeamMenu::CModTeamMenu(IViewPort *pViewPort) : CTeamMenu( pViewPort )
{
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/GameScheme.res", "GameScheme" );
	SetScheme( scheme );
}

CModTeamMenu::~CModTeamMenu()
{
}
void CModTeamMenu::MoveToCenterOfScreen()
{
	int wx, wy, ww, wt;
	surface()->GetWorkspaceBounds(wx, wy, ww, wt);
	SetPos((ww - GetWide()) / 2, (wt - GetTall()) / 2);
}

void CModTeamMenu::OnKeyCodePressed( KeyCode code )
{
	if ( IsVisible() )
	{
		if ( code == KEY_0 )
		{
			OnCommand( "vguicancel" );
		}
		else if ( code == KEY_1 )
		{
			// Join blue
			OnCommand( "jointeam 2" );
		}
		else if ( code == KEY_2 )
		{
			// Join red
			OnCommand( "jointeam 3" );
		}
		else if ( code == KEY_3 )
		{
			// Spectator
			OnCommand( "jointeam 1" );
		}
	}
}
//-----------------------------------------------------------------------------
// Purpose: called to update the menu with new information
//-----------------------------------------------------------------------------
void CModTeamMenu::Update( void )
{
	BaseClass::Update();

	const ConVar *allowspecs =  cvar->FindVar( "mp_allowspectators" );

	C_ModPlayer *pPlayer = C_ModPlayer::GetLocalModPlayer();
	
	if ( !pPlayer || !GameRules() )
		return;

	if ( allowspecs && allowspecs->GetBool() )
	{
		SetVisibleButton("specbutton", true);
	}
	else
	{
		SetVisibleButton("specbutton", false );
	}

	MoveToCenterOfScreen();
}

void CModTeamMenu::SetVisible(bool state)
{
	BaseClass::SetVisible( state );

	if ( state )
	{
		Panel *pAutoButton = FindChildByName( "autobutton" );
		if ( pAutoButton )
		{
			pAutoButton->RequestFocus();
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: When a team button is pressed it triggers this function to 
//			cause the player to join a team
//-----------------------------------------------------------------------------
void CModTeamMenu::OnCommand( const char *command )
{
	if ( Q_stricmp( command, "vguicancel" ) )
	{
		C_ModPlayer *pPlayer = C_ModPlayer::GetLocalModPlayer();
		
		if ( !Q_stricmp( command, "jointeam 2" ) )
		{
			// TODO: Implement local team number
			//pPlayer->SetLocalTeamNumber( Mod_TEAM_BLUE );
		}
		else if ( !Q_stricmp( command, "jointeam 3" ) )
		{
			// TODO: Implement local team number
			//pPlayer->SetLocalTeamNumber( Mod_TEAM_RED );
		}

		engine->ClientCmd( command );
	}

	// This is kinda hacky...
	CModClassMenu *classmenu = dynamic_cast< CModClassMenu * > ( gViewPortInterface->FindPanelByName( PANEL_CLASS_BLUE ) );
	if ( classmenu )
	{
		classmenu->Update();
	}
	else
	{
		classmenu = dynamic_cast< CModClassMenu * > ( gViewPortInterface->FindPanelByName( PANEL_CLASS_RED ) );
		if ( classmenu )
		{
			classmenu->Update();
		}
	}
	
	BaseClass::OnCommand( command );

	gViewPortInterface->ShowBackGround( false );
	OnClose();
}

//-----------------------------------------------------------------------------
// Purpose: Sets the visibility of a button by name
//-----------------------------------------------------------------------------
void CModTeamMenu::SetVisibleButton(const char *textEntryName, bool state)
{
	Button *entry = dynamic_cast<Button *>( FindChildByName( textEntryName ) );
	if ( entry )
	{
		entry->SetVisible( state );
	}
}

void CModTeamMenu::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	m_bgColor = GetSchemeColor("Lum.Game.Background", GetBgColor(), pScheme);
	m_borderColor = pScheme->GetColor( "Lum.GameBorder", Color( 0, 0, 0, 0 ) );

	const int kGameBorderAlpha = 170;
	m_borderColor.SetColor( m_borderColor.r (), m_borderColor.g (), m_borderColor.b (), kGameBorderAlpha );

	SetBgColor( m_bgColor );
	SetBorder( pScheme->GetBorder( "BaseBorder") );

	DisableFadeEffect(); //Tony; shut off the fade effect because we're using sourcesceheme.
}
