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

	This is the UI for when a map is over and we need to display
	who won, along with the teams' scores.

	Todo: Add a hud message to show this 
		  after the scoreboard is shown.

	Last Updated Dec 28, 2009
=================================================================*/

#include "cbase.h"
#include "c_hl2mp_player.h"
#include "c_playerresource.h"
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/Label.h>

#include "Mod/ClientGlobals.h"
#include "Mod/ClassicGameRules.h"
#include "Mod/Hud_Intermission.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

// Winning Texture Displays
#define INTERMISSION_BLUE_WINS	""
#define INTERMISSION_RED_WINS	""

CHudIntermission::CHudIntermission( const char *pElementName ) :
	CModHudElement( pElementName ),
	BaseClass( NULL, "HudIntermission" )
{
	SetParent( g_pClientMode->GetViewport() );

	m_BlueScore = new vgui::Label( this, "BlueScore", "Blue Score: " );
	m_BlueScore->SetFgColor( GetTeamColor( MOD_TEAM_BLUE ) );
	m_RedScore = new vgui::Label( this, "RedScore", "Red Score: " );
	m_RedScore->SetFgColor( GetTeamColor( MOD_TEAM_RED ) );

	m_BlueScore->SetBounds( m_flBlueScoreX, m_flBlueScoreY, m_flBlueScoreWidth, m_flBlueScoreHeight );
	m_RedScore->SetBounds( m_flRedScoreX, m_flRedScoreY, m_flRedScoreWidth, m_flRedScoreHeight );

	m_iLastBlueScore = -1;
	m_iLastRedScore = -1;

	// Never Hide if the element is enabled by choice
	SetHiddenBits( 0 );
}

void CHudIntermission::Init( void )
{
	m_iWinningTextureID[ TRUE_TEAM_BLUE ] = surface()->DrawGetTextureId( INTERMISSION_BLUE_WINS );
	m_iWinningTextureID[ TRUE_TEAM_RED ] = surface()->DrawGetTextureId( INTERMISSION_RED_WINS );
}

void CHudIntermission::VidInit( void )
{
	// Resize Everything
	SetBounds( m_flX, m_flY, m_flWidth, m_flHeight );
	m_BlueScore->SetBounds( m_flBlueScoreX, m_flBlueScoreY, m_flBlueScoreWidth, m_flBlueScoreHeight );
	m_RedScore->SetBounds( m_flRedScoreX, m_flRedScoreY, m_flRedScoreWidth, m_flRedScoreHeight );
}

void CHudIntermission::Reset( void )
{	
}

bool CHudIntermission::ShouldDraw( void )
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;
	
	return m_bIntermission;
}

void CHudIntermission::Paint( void )
{
	int x, y, w, h;
	GetBounds( x, y, w, h );

	// Paint the background
	//DrawBackground( GetBgColor(), 0, 0, w, h );

	// Paint the border
	DrawHollowBox( x, y, w, h, Color( LocalTeamColor() ), 220.0f );

	// The Team's Winning Texture
	vgui::surface()->DrawSetTexture( m_iWinningTextureID[ TRUE_TEAM_BLUE ] );
	vgui::surface()->DrawTexturedRect( m_flTextureX, m_flTextureY, m_flTextureWidth, m_flTextureHeight );

	// Show the Scores and Highlight Winning Name

	// Check the Blue score

	int curbluescore = g_PR->GetTeamScore( MOD_TEAM_BLUE );
	if ( curbluescore != m_iLastBlueScore )
	{
		// Update blue score ...

		m_iLastBlueScore = curbluescore;
	}


	// Check the Red score
	int curredscore = g_PR->GetTeamScore( MOD_TEAM_RED );
	if ( curredscore != m_iLastRedScore )
	{
		// Update red score ...

		m_iLastRedScore = curredscore;
	}
}
