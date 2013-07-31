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

	Reimplementation of Stamina Hud Element

	Last Updated May 25, 2010
=================================================================*/

#include "cbase.h"
#include "hud.h"
#include "hud_macros.h"
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include "c_hl2mp_player.h"
#include "iclientmode.h"
#include <vgui_controls/Label.h>
#include <vgui_controls/CircularProgressBar.h>
#include <vgui_controls/AnimationController.h>

#include "Mod/ClassicGameRules.h"
#include "Mod/Hud_Stamina.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define STAMINA_INIT -1
#define STAMINA_ICON ""

DECLARE_HUDELEMENT( CHudStamina );

CHudStamina::CHudStamina( const char *pElementName ) : 
	CHudElement( pElementName ),
	BaseClass( NULL, "HudSuitPower" )
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	vgui::HScheme scheme = vgui::scheme()->GetScheme( "ClientScheme" );

	// Font for Stamina Percentage
	m_hTextFont = vgui::scheme()->GetIScheme( scheme )->GetFont( "SFHUDLarge" );

	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_BUILDPHASE );
}

void CHudStamina::Init( void )
{
	m_flStamina = STAMINA_INIT;
	m_flStaminaLow = -1;
	
	// Get the stamina bar icon texture
	m_iTextureID = surface()->DrawGetTextureId( STAMINA_ICON );
}

void CHudStamina::Reset( void )
{
	Init();
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudStamina::ShouldDraw()
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;

	if ( g_pGameRules->GetGameMode() == GAMEMODE_CLASSIC && 
		ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD )
	{
		return false;
	}

	return CHudElement::ShouldDraw();
}

void CHudStamina::OnThink( void )
{
	float flCurrentStamina = 0;
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return;

	//flCurrentStamina = pPlayer->m_Shared.GetStamina();

	// Only update if we've changed stamina
	if ( flCurrentStamina == m_flStamina )
	{
		return;
	}

	//TD Do animation stuff

	m_flStamina = flCurrentStamina;
}

void CHudStamina::Paint()
{
	// Get the stamina percentage
	float DisplayStamina = m_flStamina;

	Color barcolor;
	// Draw Stamina Bar
	if ( LocalTeamNumber() == TEAM_BLUE )
	{
		barcolor = m_StaminaColorBlue;
	}
	else if ( LocalTeamNumber() == TEAM_RED )
	{
		barcolor = m_StaminaColorRed;
	}

	int chunkCount = ( m_flBarWidth - 3 ) / ( m_flBarChunkWidth );
	int enabledChunks = ( int )( ( float )chunkCount * ( DisplayStamina * 1.0f / 100.0f ) + 0.5f );

	surface()->DrawSetColor( barcolor );
	int xpos = m_flBarInsetX, ypos = m_flBarInsetY;
	for (int i = 0; i < enabledChunks; i++)
	{
		surface()->DrawFilledRect( xpos, ypos, xpos + m_flBarChunkWidth, ypos + m_flBarHeight );
		xpos += ( m_flBarChunkWidth + m_flBarChunkGap );
	}

	if ( m_ShowText )
	{
		// Draw Stamina Percentage
		surface()->DrawSetTextFont( m_hTextFont );
		surface()->DrawSetTextColor( Color( 255, 255, 255, 255 ) );
		surface()->DrawSetTextPos( m_flTextX, m_flTextY );
		surface()->DrawPrintText( L"Stamina", sizeof( "Stamina" ) - 1 );
	}
}
