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
#include <vgui/ILocalize.h>
#include "Mod/C_BlockBase.h"
#include "Mod/C_ModPlayer.h"
#include "Mod/ClassicGameRules.h"
#include "Mod/Hud_FreezeStatus.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Show freezestatus in build phase
static ConVar lf_hud_freezestatus_build( "lf_hud_freezestatus_build", "1", FCVAR_ARCHIVE );

// Show in combat phase
static ConVar lf_hud_freezestatus_combat( "lf_hud_freezestatus_combat", "0", FCVAR_ARCHIVE );

DECLARE_HUDELEMENT( CHudFreezeStatus );

CHudFreezeStatus::CHudFreezeStatus( const char *pElementName ) : 
	BaseClass( pElementName, "HudFreezeStatus" )
{
	SetParent( g_pClientMode->GetViewport() );

	vgui::HScheme scheme = vgui::scheme()->GetScheme( "ClientScheme" );
	m_hTextFont = vgui::scheme()->GetIScheme( scheme )->GetFont( "SFHUDLarge" );

	SetHiddenBits( 0 );
}

void CHudFreezeStatus::Init()
{
}

void CHudFreezeStatus::Reset()
{
}

bool CHudFreezeStatus::ShouldDraw()
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;
	if ( !lf_hud_freezestatus_build.GetBool() && ClassicGameRules()->IsBuildPhase() )
		return false;
	if ( !lf_hud_freezestatus_combat.GetBool() && ClassicGameRules()->IsCombatPhase() )
		return false;

	return CModHudElement::ShouldDraw();	
}

void CHudFreezeStatus::Paint()
{
	// CODE DUPLICATION BELOW. FIXME!
	wchar_t sIDString[ MAX_ID_STRING ];
	sIDString[0] = 0;
	m_bBlockInTarget = false;

	C_BaseEntity *pEntity = ProcessTarget();

	// Did we find a target?
	if ( pEntity )
	{
		// Is it a block?
		if ( pEntity->IsBlock() && pEntity->GetHealth() > 0 )
		{
			C_BlockBase *pBlock = static_cast< C_BlockBase * > ( pEntity );

			if ( ClassicGameRules() )
			{
				// Figure out the background position
				if ( ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD )
				{
					m_flBackgroundX = m_flBackgroundX_Build;
					m_flBackgroundY = m_flBackgroundY_Build;
					m_flBackgroundWidth = m_flBackgroundWidth_Build;
					m_flBackgroundHeight = m_flBackgroundHeight_Build;
				}
				else
				{
					m_flBackgroundX = m_flBackgroundX_Combat;
					m_flBackgroundY = m_flBackgroundY_Combat;
					m_flBackgroundWidth = m_flBackgroundWidth_Combat;
					m_flBackgroundHeight = m_flBackgroundHeight_Combat;
				}
			}

			// Decide colors based on team
			if ( LocalTeamNumber() == TEAM_BLUE )
			{
				m_BorderColor = m_BorderColorBlue;
				m_BackgroundColor = m_BackgroundColorBlue;
			}
			else if ( LocalTeamNumber() == TEAM_RED )
			{
				m_BorderColor = m_BorderColorRed;
				m_BackgroundColor = m_BackgroundColorRed;
			}

			// Set the foreground color
			vgui::surface()->DrawSetTextColor( GetTeamColor( pBlock->GetTeamNumber() ) );

			// Do all the drawing
			DrawBackground();
			DrawFrozenName( pBlock->GetLastFrozenByUserID() );
			DrawUnFrozenName( pBlock->GetLastUnFrozenByUserID() );
		}
	}
}

// Draw the background and border
void CHudFreezeStatus::DrawBackground()
{
	int x = m_flBackgroundX;
	int y = m_flBackgroundY;
	int w = m_flBackgroundWidth;
	int h = m_flBackgroundHeight;

	// Draw the border
	DrawHollowBox( x, y, w, h, m_BorderColor, 220 );
	
	// Draw the background
	DrawBox( x, y, w, h, m_BackgroundColor, 220 );
}

// Display the name of the player that froze the block
void CHudFreezeStatus::DrawFrozenName( int UserID )
{
	vgui::surface()->DrawSetTextFont( m_hFont );
	vgui::surface()->DrawSetTextPos( m_flBackgroundX + m_flFrozenTextX, m_flBackgroundY + m_flFrozenTextY );

	// Find the name of the player that froze the block
	int frozenplayerindex = engine->GetPlayerForUserID( UserID );
	C_BasePlayer *pPlayerFrozen = UTIL_PlayerByIndex( frozenplayerindex );

	vgui::surface()->DrawPrintText( L"Frozen By: ", Q_wcslen( L"Frozen By: " ) );

	if ( pPlayerFrozen )
	{
		wchar_t playername[ 64 ] = { '\0' };

		g_pVGuiLocalize->ConvertANSIToUnicode( pPlayerFrozen->GetPlayerName(), playername, sizeof( playername ) - 1 );
			
		vgui::surface()->DrawPrintText( playername, Q_wcslen( playername ) );
	}
	else
	{
		vgui::surface()->DrawPrintText( L"---", Q_wcslen( L"---" ) );
	}
}

// Display the name of the player that unfrozen the block
void CHudFreezeStatus::DrawUnFrozenName( int UserID )
{
	vgui::surface()->DrawSetTextFont( m_hFont );
	vgui::surface()->DrawSetTextPos( m_flBackgroundX + m_flUnFrozenTextX, m_flBackgroundY + m_flUnFrozenTextY );

	// Find the name of the player that froze the block
	int playerindex = engine->GetPlayerForUserID( UserID );
	C_BasePlayer *pPlayerUnFrozen = UTIL_PlayerByIndex( playerindex );

	vgui::surface()->DrawPrintText( L"UnFrozen By: ", Q_wcslen( L"UnFrozen By: " ) );

	if ( pPlayerUnFrozen )
	{
		wchar_t playername[ 64 ] = { '\0' };

		g_pVGuiLocalize->ConvertANSIToUnicode( pPlayerUnFrozen->GetPlayerName(), playername, sizeof( playername ) - 1 );
			
		vgui::surface()->DrawPrintText( playername, Q_wcslen( playername ) );
	}
	else
	{
		vgui::surface()->DrawPrintText( L"---", Q_wcslen( L"---" ) );
	}
}
