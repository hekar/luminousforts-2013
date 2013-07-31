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
#include "c_hl2mp_player.h"
#include "C_Dom_CaptureZone.h"
#include "HudH_MarkerTools.h"
#include "Hud_CapPos.h"

ConVar lf_hud_capture_point_min_w( "lf_hud_capture_point_min_w", "8", FCVAR_ARCHIVE, "Minimum width of Capture Point Marker" );
ConVar lf_hud_capture_point_max_w( "lf_hud_capture_point_max_w", "12", FCVAR_ARCHIVE, "Maximum width of Capture Point Marker" );

ConVar lf_hud_capture_point_min_h( "lf_hud_capture_point_min_h", "8", FCVAR_ARCHIVE, "Minimum height of Capture Point Marker" );
ConVar lf_hud_capture_point_max_h( "lf_hud_capture_point_max_h", "12", FCVAR_ARCHIVE, "Maximum height of Capture Point Marker" );

ConVar lf_hud_capture_point_divider( "lf_hud_capture_point_divider", "1300", FCVAR_ARCHIVE, "Divides the size of markers Capture Points" );

#define LF_CAPTURE_POINT_FONT "LF_game_icon"
#define LF_CAPTURE_POINT_CHAR 'T'

DECLARE_HUDELEMENT( CHudCapPos );

CHudCapPos::CHudCapPos( const char *pElementName ) :
	CModHudElement( pElementName, HUDELEM_CAPPOS ), 
	BaseClass( NULL, "HudCapPos" )
{
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD );
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetGameModes( GAMEMODE_DOM );
}

void CHudCapPos::Init( void )
{
	// TODO: Fix
#if 0
	InitFontTexture( &m_Texture, LF_CAPTURE_POINT_FONT, LF_CAPTURE_POINT_CHAR, "cp_texture" );
#endif // 0
}

void CHudCapPos::LevelInit( void )
{
	m_CPs.RemoveAll();
}

void CHudCapPos::Reset( void )
{
}

bool CHudCapPos::ShouldDraw( void )
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;

	return CModHudElement::ShouldDraw();
}

void CHudCapPos::Paint( void )
{
	DrawMarker marker;
	marker.SetDivider( lf_hud_capture_point_divider.GetFloat() );
	marker.SetWidthDim( lf_hud_capture_point_min_w.GetFloat(), lf_hud_capture_point_max_w.GetFloat() );
	marker.SetHeightDim( lf_hud_capture_point_min_h.GetFloat(), lf_hud_capture_point_max_h.GetFloat() );
	marker.SetTexture( &m_Texture );
	for ( int i = 0; i < m_CPs.Count(); i++ )
	{
		C_DomCaptureZone *cp = m_CPs.Element( i );

		Vector pos = cp->WorldSpaceCenter();
		
		marker.SetOrigin( pos );
		marker.SetColor( GetTeamColor( cp->GetTeamNumber() ) );
		marker.Draw();
	}
}

bool CHudCapPos::AddCaptureZone( C_DomCaptureZone *capzone )
{
	AssertMsg( capzone, "Cannot have NULL capture zone" );
	if ( !m_CPs.HasElement( capzone ) )
	{
		return m_CPs.AddToTail( capzone ) >= 0;
	}

	return false;
}
