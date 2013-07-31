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
#include "hud_macros.h"
#include "clientmode.h"
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/Panel.h>
#include <vgui_controls/Controls.h>
#include "vgui_avatarimage.h"

#include "c_team.h"
#include "c_playerresource.h"
#include "c_hl2mp_player.h"

#include "Mod/Dom_GameRules.h"
#include "Mod/ClientGlobals.h"
#include "Mod/SharedModCvars.h"
#include "Mod/Hud_DomScore.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_HUDELEMENT( CHudDomScore );

//
//	Name: CHudDomScore
//	Author: Hekar Khani
//	Description: HUD panel for displaying player scores
//	Notes: 
//
CHudDomScore::CHudDomScore( const char *pElementName ) :
	CModHudElement( pElementName, HUDELEM_DOMSCORE ),
	BaseClass( NULL, "HudDomScore" ) 
{
	// Set our parent window
	SetParent( g_pClientMode->GetViewport() );
	vgui::HScheme scheme = vgui::scheme()->GetScheme( "ClientScheme" );
	m_hTextFont = vgui::scheme()->GetIScheme( scheme )->GetFont( "SFHUDLarge" );

	SetPaintBackgroundEnabled( true );

	SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_BUILDPHASE );
}

CHudDomScore::~CHudDomScore ()
{
}

void CHudDomScore::LevelInit( void )
{
	for ( int i = 0; i < TEAM_COUNT; i++ )
	{
		m_iScores[ i ] = 0;
	}

	// Initialize the scores to 0
	for ( int i = TEAM_BLUE; i <= TEAM_RED; i++ )
	{
		_snwprintf( m_Text[ i ], sizeof( m_Text[ i ] ), L"Score: %d", m_iScores[ i ] );
	}
}

void CHudDomScore::VidInit( void )
{
	Reset();
}

bool CHudDomScore::ShouldDraw()
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;

	if ( GameRules()->GetGameMode() == GAMEMODE_CLASSIC )
	{
		return false;
	}
	if ( GameRules()->GetGameMode() == GAMEMODE_DOM &&
		DomGameRules()->GetCurrentPhaseID() != PHASE_BUILD )
	{
		return true;
	}

	return CModHudElement::ShouldDraw();
}

void CHudDomScore::Paint ( void )
{
	SetBgColor( Color( GetBgColor().r(), GetBgColor().g(), GetBgColor().b(), 128 ) );

	DisplayText
	(
		m_Text[ SDK_TEAM_BLUE ], 
		m_flBlueTextX, 
		m_flBlueTextY,
		m_BlueForegroundColor
	);
	
	DisplayText
	(
		m_Text[ SDK_TEAM_RED ], 
		m_flRedTextX,
		m_flRedTextY, 
		m_RedForegroundColor
	);
}

void CHudDomScore::PaintBackground( void )
{
	// Draw The Blue Flag Carrier
	vgui::surface()->DrawSetColor( m_BlueBackgroundColor );

	vgui::surface()->DrawFilledRect
	(
		m_flBlueRectX, 
		m_flBlueRectY, 
		m_flBlueRectX + m_flBlueRectWidth, 
		m_flBlueRectY + m_flBlueRectHeight
	);

	if ( m_DrawBlueBorder )
	{
		const Color BlueBorderColor = Color
		( 
			GetTeamColor( SDK_TEAM_BLUE ).r(), 
			GetTeamColor( SDK_TEAM_BLUE ).g(), 
			GetTeamColor( SDK_TEAM_BLUE ).b(), 
			200
		);

//		DrawBrokenBorder
//		(
//			BlueBorderColor,
//			m_flBlueRectX,
//			m_flBlueRectY,
//			m_flBlueRectWidth,
//			m_flBlueRectHeight,
//			12, 8
//		);
	}

	// Draw The Red Flag Carrier
	vgui::surface()->DrawSetColor( m_RedBackgroundColor );

	vgui::surface()->DrawFilledRect
	( 
		m_flRedRectX,
		m_flRedRectY, 
		m_flRedRectX + m_flRedRectWidth, 
		m_flRedRectY + m_flRedRectHeight
	);

	if ( m_DrawRedBorder )
	{
		const Color RedBorderColor = Color
		( 
			GetTeamColor( SDK_TEAM_RED ).r(), 
			GetTeamColor( SDK_TEAM_RED ).g(), 
			GetTeamColor( SDK_TEAM_RED ).b(), 
			200
		);

//		DrawBrokenBorder
//		(
//			RedBorderColor,
//			m_flRedRectX,
//			m_flRedRectY,
//			m_flRedRectWidth,
//			m_flRedRectHeight,
//			12, 8
//		);
	}
}

void CHudDomScore::DisplayText( wchar_t *pText, int x, int y, Color col )
{
	vgui::surface()->DrawSetTextFont( m_hTextFont ); // set the font	
	vgui::surface()->DrawSetTextColor( col.r(), col.g(), col.b(), col.a() );
	vgui::surface()->DrawSetTextPos( x, y );

	int TextLength = wcslen( pText );
	vgui::surface()->DrawPrintText( pText, TextLength ); // print text
}

void CHudDomScore::OnThink()
{
	bool requiresUpdate = false;
	for ( int i = TEAM_BLUE; i < TEAM_RED; i++ )
	{
		if ( m_iScores[ i ] != m_iLastScores[ i ] )
		{
			requiresUpdate = true;
		}

		m_iLastScores[ i ] = m_iScores[ i ];
	}

	if ( requiresUpdate )
	{
		Update();
	}
}

void CHudDomScore::Update()
{
	for ( int i = TEAM_BLUE; i <= TEAM_RED; i++ )
	{
		m_iScores[ i ] = g_PR->GetTeamScore( i );
		_snwprintf( m_Text[ i ], sizeof( m_Text[ i ] ), L"Score: %d", m_iScores[ i ] );
	}
}
