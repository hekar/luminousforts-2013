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

	Progress bar while freezing a block in "Combat Phase"

	Last Updated Dec 28, 2009
=================================================================*/


#include "cbase.h"
#include <vgui/ISurface.h>
#include "c_hl2mp_player.h"
#include "Mod/Hud_FreezeProgress.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_HUDELEMENT( CHudFreezeProgress );
DECLARE_HUD_MESSAGE( CHudFreezeProgress, UpdateFreezeProgress );

CHudFreezeProgress::CHudFreezeProgress( const char* pElementName ) :
	CModHudElement( pElementName, HUDELEM_FREEZEPROGRESS ),
	BaseClass( NULL, "HudFreezeProgress" )
{
	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_BUILDPHASE );
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	SetVisible( false );
	SetEnabled( true );
}

CHudFreezeProgress::~CHudFreezeProgress()
{
}

void CHudFreezeProgress::Init( void )
{
	HOOK_HUD_MESSAGE( CHudFreezeProgress, UpdateFreezeProgress );
}

void CHudFreezeProgress::Reset( void )
{
	m_bShow = false;
	m_iProgressType = SFH_FREEZEPROGRESS_NONE;
	m_flProgress = 0;
}

bool CHudFreezeProgress::ShouldDraw (void)
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;

	return m_bShow & CModHudElement::ShouldDraw();
}

void CHudFreezeProgress::Paint( void )
{
	if ( !m_bShow )
	{
		return;
	}

	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return;

	int w, h;
	GetSize( w, h );

	m_Foreground = LocalTeamColor();

	// Draw the Background
	DrawBox( 0, 0, w, h, m_Background, 170, false );

	// Draw the ProgressBar
	float progresslength = 0;

	if (  m_iProgressType == SFH_FREEZEPROGRESS_HEALING )
	{
		progresslength = m_flProgress / 100.0f * m_flBarWidth;
	}
	else if ( m_iProgressType == SFH_FREEZEPROGRESS_FREEZING )
	{
		progresslength = m_flProgress / 100.0f * m_flBarWidth;
	}
	else if ( m_iProgressType == SFH_FREEZEPROGRESS_UNFREEZING )
	{
		 progresslength = m_flBarWidth - m_flProgress / 100.0f * m_flBarWidth;
	}
	else if ( m_iProgressType == SFH_FREEZEPROGRESS_UNFREEZING_ENEMY )
	{
		progresslength = m_flBarWidth - m_flProgress / 100.0f * m_flBarWidth;
		int otherteamnum = GetOtherTeamNumber( pPlayer->GetTeamNumber() );
		if ( otherteamnum == TEAM_BLUE )
		{
			m_Foreground = m_BlueProgressForeground;
		}
		else if ( otherteamnum == TEAM_RED )
		{
			m_Foreground = m_RedProgressForeground;
		}
	}

	vgui::surface()->DrawSetColor( m_Background );
	vgui::surface()->DrawFilledRect( 0, 0, w, h );

	vgui::surface()->DrawSetColor( m_Foreground );
	vgui::surface()->DrawFilledRect( m_flBarX, m_flBarY, m_flBarX + progresslength, m_flBarY + m_flBarHeight );

	vgui::surface()->DrawSetColor( m_ProgressBackground );
	vgui::surface()->DrawFilledRect(m_flBarX + progresslength, m_flBarY, m_flBarX + m_flBarWidth, m_flBarY + m_flBarHeight);	

#if 0
	if ( pPlayer->GetTeamNumber() == TEAM_BLUE )
	{
		DrawBrokenBorder( m_BlueBorder, 0, 0, w, h );
	}
	else if ( pPlayer->GetTeamNumber() == TEAM_RED )
	{
		DrawBrokenBorder( m_RedBorder, 0, 0, w, h );
	}
#endif // 0
}

void CHudFreezeProgress::ApplySchemeSettings( vgui::IScheme* scheme )
{
	BaseClass::ApplySchemeSettings( scheme );
	SetPaintBackgroundEnabled( false );
}

/*
 * Freeze Progress Message
 *  Updates the Progress of the freeze status (meant for combat phase)
 *  Protocol:
 *		Show/Hide	- Byte
 *		Type		- Integer (FreezeProgressType_t)
 *		Progress	- Float (0 - 100)
*/
void CHudFreezeProgress::MsgFunc_UpdateFreezeProgress( bf_read& data )
{
	if ( data.ReadByte() )
	{
		SetVisible( true );
		m_bShow = true;
	}
	else
	{
		SetVisible( false );
		m_bShow = false;
	}

	m_iProgressType = data.ReadLong();

	m_flProgress = data.ReadFloat();
}
