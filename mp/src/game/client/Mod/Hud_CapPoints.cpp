/* ***** BEGIN LICENSE BLOCK *****
Version: MPL 1.1/LGPL 2.1/GPL 2.0

The contents of this file are subject to the Mozilla Public License Version 
1.1 (the "License") you may not use this file except in compliance with
...
 you may not use this file except in compliance with
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

/**
 * \file Hud_CapPoints.cpp
 * \author Hekar Khani
 * Hud element for displaying capture points across the map.
 * This is usually used in the domination gamemode, but
 * can be useful in any gamemode with capture points.
 */

#include "cbase.h"
#include "Mod/Dom_SharedDefs.h"
#include "Mod/C_Dom_CaptureZone.h"
#include "Mod/C_ModPlayer.h"
#include "Mod/Hud_CapPoints.h"

DECLARE_HUDELEMENT( CHudCapPoints );

CHudCapPoints::CHudCapPoints( const char *pElementName ) :
	CModHudElement( pElementName, HUDELEM_CAPPOINTS ), 
	BaseClass( NULL, "HudCapPoints" )
{
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetGameModes( GAMEMODE_DOM );
}

void CHudCapPoints::Init( void )
{
}

void CHudCapPoints::LevelInit( void )
{
	m_CPs.RemoveAll();
}

void CHudCapPoints::Reset( void )
{
}

bool CHudCapPoints::ShouldDraw( void )
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;

	return CModHudElement::ShouldDraw();
}

/**
 * TODO: Find a way to minimize the calculations performed
 *		in this paint
 */
void CHudCapPoints::Paint( void )
{
	int x = 0;
	for ( int i = 0; i < m_CPs.Count(); i++ )
	{
		C_DomCaptureZone *cp = m_CPs.Element( i );
		
		if ( !cp )
		{
			continue;
		}

		int w = m_BarWidth;
		int percentage = cp->GetCapturePercentage();
		int pw = w * (((float)percentage) / 100);
		
		Color color = m_ProgressColorSpec;
		Color borderColor = m_BorderColorSpec;
		if ( cp->GetTeamNumber() != TEAM_SPECTATOR &&
			( cp->GetCapState() == CAP_DRAIN_RED || cp->GetCapState() == CAP_DRAIN_BLUE ) )
		{
			// Reverse the progress bar
			percentage = clamp( 100 - percentage, 0, 100 );
			pw = w * (((float)percentage) / 100);

			if ( cp->GetTeamNumber() == TEAM_RED )
			{
				color = m_ProgressColorRed;
				borderColor = m_BorderColorBlue;
			}
			else if ( cp->GetTeamNumber() == TEAM_BLUE )
			{
				color = m_ProgressColorBlue;
				borderColor = m_BorderColorRed;
			}
		}
		else
		{
			if ( cp->GetTeamNumber() == TEAM_RED || cp->GetCapturingTeam() == TEAM_RED )
			{
				color = m_ProgressColorRed;
				borderColor = m_BorderColorRed;
			}
			else if ( cp->GetTeamNumber() == TEAM_BLUE || cp->GetCapturingTeam() == TEAM_BLUE )
			{
				color = m_ProgressColorBlue;
				borderColor = m_BorderColorBlue;
			}
		}

		if ( cp->GetCapturePercentage() >= 99 )
		{
			borderColor = Color( LF_CLR_BLACK );
		}

		// TODO: Fix
#if 0
		DrawBackground( color, x, y, x + pw, y + ph );
		DrawEmptyBorder( borderColor, x, y, x + w, y + h, 1 );
#endif // 0
		x += w + m_GapWidth;
	}
}

bool CHudCapPoints::AddCaptureZone( C_DomCaptureZone *capzone )
{
	bool hasZone = false;
	for ( int i = 0; i < m_CPs.Count(); i++ )
	{
		if ( m_CPs.Element( i )->GetZoneId() == capzone->GetZoneId() )
		{
			hasZone = true;
			break;
		}
	}

	if ( !hasZone )
	{
		return m_CPs.AddToTail( capzone ) >= 0;
	}

	return false;
}
