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
	HUD Element
	Sourceforts 1.3.2 Gamemode

	Buy menu for the SF132 gamemode

	Last Updated Oct 30, 2009
===============================================================*/


#include "cbase.h"
#ifdef MOD_SF132
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include <game/client/iviewport.h>
#include "c_team.h"
#include "C_ModPlayer.h"
#include "c_playerresource.h"

#include "IGameUIFuncs.h" // for key bindings

#include "Hud_Base.h"
#include "ClientGlobals.h"
#include "Mod/ClassicGameRules.h"
#include "SF132_Hud_Buy.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static bool g_IsVisible = false;

CSF132HudBuy::CSF132HudBuy( vgui::Panel *Parent ) :
	BaseClass( NULL, "buy" )
{
	SetScheme( "sourcescheme" );
	LoadControlSettings( "resource/ui/SF132_BuyMenu.res" );
	BaseClass::SetParent( Parent );

	m_iToggleMenuKey = BUTTON_CODE_INVALID;

	SetTitle( "#sf132_buy_menu_title", true );
	SetMaximizeButtonVisible( false );
	SetMinimizeButtonVisible( false );
	SetSizeable( false );

	SetVisible( g_IsVisible );
	SetEnabled( g_IsVisible );
}

bool CSF132HudBuy::ShouldDraw()
{
	C_ModPlayer *pPlayer = C_ModPlayer::GetLocalModPlayer();
	if ( !pPlayer )
		return false;

	if ( g_pGameRules->GetGameMode() != GAMEMODE_SF132 )
	{
		return false;
	}

	return IsVisible();
}

void CSF132HudBuy::PaintBackground()
{
	if ( !ShouldDraw() )
		return;

	BaseClass::PaintBackground();

	C_ModPlayer *pLocalPlayer = C_ModPlayer::GetLocalModPlayer();

	if ( !pLocalPlayer )
		return;

	int posy = 35;
	for (int i = 0; i < ARRAYSIZE( g_WeaponCommands ); i++)
	{
		char text [128] = {0};
		wchar_t wtext [128] = {0};

		vgui::surface()->DrawSetTextPos( 5, posy );

		vgui::surface()->DrawSetTextColor( LF_CLR_WHITE );
		Q_snprintf( text, sizeof( text ), "%02d. %s Price: %02d", (i + 1), g_WeaponCommands[i].WeaponName, g_WeaponCommands[i].Price );

		g_pVGuiLocalize->ConvertANSIToUnicode( text, wtext, sizeof( wtext ) );
		vgui::surface()->DrawPrintText( wtext, Q_wcslen( wtext ) );

		posy += 20;
	}
}

void CSF132HudBuy::ShowPanel( bool bShow )
{
	if ( IsVisible() == bShow )
		return;

	if ( bShow )
	{
		engine->CheckPoint( "HudBuy" );
		m_iToggleMenuKey = gameuifuncs->GetButtonCodeForBind( "sf132_buymenu_toggle" );

		SetEnabled( true );
		SetKeyBoardInputEnabled( true );
		RequestFocus(); // ?
	}
	else
	{
		SetEnabled( false );
		SetKeyBoardInputEnabled( false );
	}

	SetVisible( bShow );
}

void CSF132HudBuy::OnKeyCodePressed( vgui::KeyCode code )
{
	if ( code == m_iToggleMenuKey )
	{
		HidePanel();
	}
	else
	{
		int WeaponIndex = code - KEY_F1;
		if ( WeaponIndex >= 0 && WeaponIndex < SF132_BUY_WEAPON_COUNT )
		{
			engine->ClientCmd( g_WeaponCommands[ WeaponIndex ].Command );
			HidePanel();
		}
	}
}

void CSF132HudBuy::HidePanel()
{
	g_IsVisible = false;
	ShowPanel( false );
}

// Toggle visibility of the buy menu
CON_COMMAND( sf132_buymenu_toggle, "Toggle the Display of the Buy Menu" )
{
	if ( gViewPortInterface )
	{
		g_IsVisible = !g_IsVisible;
		gViewPortInterface->ShowPanel( "buy", g_IsVisible );
	}
}

#endif // MOD_SF132
