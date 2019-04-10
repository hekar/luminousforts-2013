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

#include <vgui_controls/TextEntry.h>
#include <vgui_controls/Button.h>
#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include "cdll_util.h"
#include "c_team.h"
#include <game/client/iviewport.h>


#include "IGameUIFuncs.h" // for key bindings
#include "Mod/GUI_ClassMenu.h"
#include "Mod/ClientGlobals.h"
#include "Mod/C_ModPlayer.h"
#include "Mod/ClassicGameRules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;


ConVar _cl_classmenuopen( "_cl_classmenuopen", "0", FCVAR_CLIENTCMD_CAN_EXECUTE, "internal cvar used to tell server when class menu is open" );


CON_COMMAND_F( lf_hud_classmenu, "Choose a new class", FCVAR_SERVER_CAN_EXECUTE|FCVAR_CLIENTCMD_CAN_EXECUTE )
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();

	if ( pPlayer)
	{
		switch( pPlayer->GetTeamNumber() )
		{
		case SDK_TEAM_BLUE:
			gViewPortInterface->ShowPanel( PANEL_CLASS_BLUE, true );
			break;
		case SDK_TEAM_RED:
			gViewPortInterface->ShowPanel( PANEL_CLASS_RED, true );
			break;
		case TEAM_UNASSIGNED:
		default:
			break;
		}
	}
}

Panel *CModClassInfoPanel::CreateControlByName( const char *controlName )
{
	return BaseClass::CreateControlByName( controlName );
}

void CModClassInfoPanel::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
}

CModClassMenu::CModClassMenu(IViewPort *pViewPort) :
		CClassMenu( pViewPort )
{
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/GameScheme.res", "GameScheme" );
	SetScheme( scheme );

	m_mouseoverButtons.RemoveAll();
	m_iClassMenuKey = BUTTON_CODE_INVALID;
	m_pInitialButton = NULL;

	m_pClassInfoPanel = new CModClassInfoPanel( this, "ClassInfoPanel" );
	
	vgui::ivgui()->AddTickSignal( GetVPanel() );

	m_iActivePlayerClass = -1;
	m_iLastPlayerClassCount = -1;
	m_iLastClassLimit = -1;

	m_bNeedsUpdate = false;
}

CModClassMenu::CModClassMenu(IViewPort *pViewPort, const char *panelName, const char *schemename) :
		CClassMenu(pViewPort, panelName)
{
	if ( schemename )
	{
		char schemefile[ MAX_PATH + 1 ];
		schemefile[0] = '\0';
		Q_snprintf( schemefile, sizeof( schemefile ), "resource/%s.res", schemename );
		vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( schemefile, schemename );
		SetScheme( scheme );
	}
	else
	{
		vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/GameScheme.res", "GameScheme" );
		SetScheme( scheme );
	}

	m_mouseoverButtons.RemoveAll();
	m_iClassMenuKey = BUTTON_CODE_INVALID;
	m_pInitialButton = NULL;

	m_pClassInfoPanel = new CModClassInfoPanel( this, "ClassInfoPanel" );
	
	vgui::ivgui()->AddTickSignal( GetVPanel() );

	m_bNeedsUpdate = false;

	m_iActivePlayerClass = -1;
	m_iLastPlayerClassCount = -1;
	m_iLastClassLimit = -1;
}

//Destructor
CModClassMenu::~CModClassMenu()
{
}

void CModClassMenu::ShowPanel( bool bShow )
{
	if ( bShow )
	{
		engine->CheckPoint( "ClassMenu" );

		m_iClassMenuKey = gameuifuncs->GetButtonCodeForBind( "lf_hud_classmenu" );
	}

	int parentW, parentH;
	GetSize( parentW, parentH );

	BaseClass::ShowPanel( bShow );
}

void CModClassMenu::OnKeyCodePressed( KeyCode code )
{
	if ( m_iClassMenuKey != BUTTON_CODE_INVALID && m_iClassMenuKey == code )
	{
		ShowPanel( false );
	}
	else
	{
		if ( code == KEY_0 )
		{
			OnCommand( "vguicancel" );
		}
		else if ( code >= KEY_1 && code <= KEY_9 )
		{
			// Shift keys over to zero offset :/
			// HACK HACK HACK HACK
			int KeyIndex = code - KEY_1;

			char changeclass [128] = {0};
			Q_snprintf( changeclass, sizeof( changeclass ), "joinclass %d", KeyIndex );
			
			engine->ClientCmd( changeclass );

			ShowPanel( false );
		}

		BaseClass::OnKeyCodePressed( code );
	}
}

void CModClassMenu::MoveToCenterOfScreen()
{
	int wx, wy, ww, wt;
	surface()->GetWorkspaceBounds(wx, wy, ww, wt);
	SetPos((ww - GetWide()) / 2, (wt - GetTall()) / 2);
}

void CModClassMenu::LevelInit()
{
	m_bNeedsUpdate = true;
}

void CModClassMenu::Update()
{
	C_ModPlayer *pPlayer = C_ModPlayer::GetLocalModPlayer();
	
	if ( !pPlayer )
	{
		return;
	}

	int LocalTeam = GetTeamNumber();

	if ( LocalTeam == SDK_TEAM_BLUE )
	{
		LoadControlSettings( ClassicGameRules()->GetClassmenuBlue() );
	}
	else if ( LocalTeam == SDK_TEAM_RED )
	{
		LoadControlSettings( ClassicGameRules()->GetClassmenuRed() );
	}

	InvalidateLayout( true, true );
	
	if ( pPlayer && pPlayer->m_HL2Local.m_iDesiredPlayerClass == PLAYERCLASS_UNDEFINED )
	{
		SetVisibleButton( "CancelButton", false );
	}
	else
	{
		SetVisibleButton( "CancelButton", true ); 
	}

	MoveToCenterOfScreen();

	m_bNeedsUpdate = false;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
Panel *CModClassMenu::CreateControlByName( const char *controlName )
{
	if ( !Q_stricmp( "SDKButton", controlName ) )
	{
		MouseOverButton<CModClassInfoPanel> *newButton = new MouseOverButton<CModClassInfoPanel>( this, NULL, m_pClassInfoPanel );

		if( !m_pInitialButton )
		{
			m_pInitialButton = newButton;
		}

		return newButton;
	}
	else
	{
		return BaseClass::CreateControlByName( controlName );
	}
}

void CModClassMenu::OnShowPage( const char *pagename )
{
	// change which class we are counting based on class name
	// turn the button name into a classname

	char buf[64];

	Q_snprintf( buf, sizeof(buf), "cls_%s", pagename );

	C_Team *pTeam = GetGlobalTeam( GetTeamNumber() );

	if( !pTeam )
		return;

	// Pull the index of this class via IsClassOnTeam
	if ( !pTeam->IsClassOnTeam( buf, m_iActivePlayerClass ) )
	{
		Assert( !"bad class name on class button" );
	}
}

void CModClassMenu::OnTick( void )
{
	//When a player changes teams, their class and team values don't get here 
	//necessarily before the command to update the class menu. This leads to the cancel button 
	//being visible and people cancelling before they have a class. check for class == PLAYERCLASS_UNASSIGNED and if so
	//hide the cancel button

	if ( !IsVisible() )
		return;

	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();

	if( pPlayer && pPlayer->m_HL2Local.m_iPlayerClass == PLAYERCLASS_UNDEFINED )
	{
		SetVisibleButton("CancelButton", false);
	}

	BaseClass::OnTick();
}

void CModClassMenu::SetVisible( bool state )
{
	BaseClass::SetVisible( state );

	if ( state )
	{
		engine->ClientCmd( "_cl_classmenuopen 1" );
	}
	else
	{
		engine->ClientCmd( "_cl_classmenuopen 0" );
	}
}

void CModClassMenu::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	m_bgColor = GetSchemeColor("Lum.Game.Background", GetBgColor(), pScheme);
	m_borderColor = pScheme->GetColor( "Lum.GameBorder", Color( 0, 0, 0, 0 ) );

	const int kGameBorderAlpha = 170;
	m_borderColor.SetColor( m_borderColor.r (), m_borderColor.g (), m_borderColor.b (), kGameBorderAlpha );

	SetBgColor( m_bgColor );
	SetBorder (pScheme->GetBorder ("FrameBorder"));

	DisableFadeEffect(); //Tony; shut off the fade effect because we're using sourcesceheme.
}

