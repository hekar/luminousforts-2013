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

	Displays the position of the blue and red flag in the map.

	Last Updated April 1, 2010
=================================================================*/


#include "cbase.h"
#include <vgui/ISurface.h>
#include "c_hl2mp_player.h"
#include "c_team.h"
#include "c_playerresource.h"

#include "Mod/ClassicGameRules.h"
#include "Mod/HudH_MarkerTools.h"
#include "Mod/Hud_FlagPos.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


ConVar lf_hud_flagpos_min( "lf_hud_flagpos_min", "16", FCVAR_REPLICATED, "The minimum width/height of the flag marker" );
ConVar lf_hud_flagpos_max( "lf_hud_flagpos_max", "28", FCVAR_REPLICATED, "The maximum width/height of the flag marker" );
ConVar lf_hud_flagpos_scale_divider( "lf_hud_flagpos_scale_divider", "1300", FCVAR_ARCHIVE, "Divides the distance from a the flag with this number" );

//
//	Name: CHudFlagLocations
//	Author: Hekar Khani
//	Description: Displays locations of flags on the HUD
//				(as a marker translating from a 3D pos to 2D pos)
//	Notes: 
//
DECLARE_HUDELEMENT( CHudFlagLocations );

CHudFlagLocations::CHudFlagLocations( const char *pElementName ) : 
	CModHudElement( pElementName, HUDELEM_FLAGPOS ), BaseClass( NULL, "HudFlagLocations" )
{
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_BUILDPHASE );
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetVisible( true );
	SetEnabled( true );

	m_BlueTexture = NULL;
	m_RedTexture = NULL;

	m_bNeedsUpdate = false;
}

void CHudFlagLocations::VidInit( void )
{
	if ( !m_BlueTexture )
	{
		m_BlueTexture = gHUD.GetIcon( "flag_icon_blue" );
		if ( !m_BlueTexture )
		{
			Error( "Failed to load flag_maker_blue\n" );
		}
	}

	if ( !m_RedTexture )
	{
		m_RedTexture = gHUD.GetIcon( "flag_icon_red" );
		if ( !m_RedTexture )
		{
			Error( "Failed to load flag_maker_red\n" );
		}
	}
}

bool CHudFlagLocations::ShouldDraw()
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;

	if ( GameRules()->GetGameModeMask() & GAMEMODE_CLASSIC )
	{
		if ( ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD )
		{
			return false;
		}
	}
	else if ( GameRules()->GetGameMode() == GAMEMODE_DOM )
	{
		return false;
	}

	return CModHudElement::ShouldDraw();
}

void CHudFlagLocations::ApplySchemeSettings( vgui::IScheme* scheme )
{
	BaseClass::ApplySchemeSettings( scheme );
	SetPaintBackgroundEnabled( false );
	m_hFont = scheme->GetFont( "HudFlagLocations", true );
}

void CHudFlagLocations::Paint()
{
	DrawMarker marker;
	marker.SetDivider( lf_hud_flagpos_scale_divider.GetFloat() );
	marker.SetWidthDim( lf_hud_flagpos_min.GetFloat(), lf_hud_flagpos_max.GetFloat() );
	marker.SetHeightDim( lf_hud_flagpos_min.GetFloat(), lf_hud_flagpos_max.GetFloat() );

	if ( m_BlueTexture )
	{
		Vector v = GetFlagLocation( TEAM_BLUE );
		marker.SetColor( GetTeamColor( TEAM_BLUE ) );
		marker.SetOrigin( v );
		marker.SetTexture( m_BlueTexture );
		marker.Draw();		
	}

	if ( m_RedTexture )
	{
		Vector v = GetFlagLocation( TEAM_RED );
		marker.SetColor( GetTeamColor( TEAM_RED ) );
		marker.SetOrigin( v );
		marker.SetTexture( m_RedTexture );
		marker.Draw();
	}
}

Vector CHudFlagLocations::GetFlagLocation( int team )
{
	return ( team == SDK_TEAM_BLUE ) ? m_vBlueLocation : m_vRedLocation;
}

void CHudFlagLocations::UpdateFlagPosition( int Team, const Vector& Pos )
{
	m_bNeedsUpdate = true;
	switch ( Team )
	{
	case SDK_TEAM_BLUE:
		m_vBlueLocation = Pos;
		break;
	case SDK_TEAM_RED:
		m_vRedLocation = Pos;
		break;
	default:
		m_bNeedsUpdate = false;
		break;
	}
}
