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

	Reimplementation of Health Hud Element

	Last Updated May 27, 2010
=================================================================*/

#include "cbase.h"
#include "c_hl2mp_player.h"
#include "view.h"
#include "convar.h"
#include <KeyValues.h>
#include <vgui/ISurface.h>
#include <vgui/ISystem.h>
#include <vgui/ILocalize.h>
#include <vgui_controls/AnimationController.h>

#include "Mod/ClassicGameRules.h"
#include "Mod/ClientGlobals.h"
#include "Mod/Hud_Health.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_HUDELEMENT( CHudPlayerHealth );
DECLARE_HUD_MESSAGE( CHudPlayerHealth, Damage );

#define INIT_HEALTH -1
#define HUD_HEALTH_BAR_TEXTURE ""

ConVar lf_health_display_type( "lf_health_display_type", "0", FCVAR_ARCHIVE, "Display Health as: (0)HP (1)Percentage (2)Percentage %03d (other)HP/MaxHP" );

CHudPlayerHealth::CHudPlayerHealth( const char *pElementName ) : 
	CModHudElement( pElementName ), 
	vgui::Panel( NULL, "HudPlayerHealth" )
{
	SetParent( g_pClientMode->GetViewport() );

	vgui::HScheme scheme = vgui::scheme()->GetScheme( "ClientScheme" );
	m_hTextFont = vgui::scheme()->GetIScheme( scheme )->GetFont( "SFHUDLarge" );

	SetHiddenBits( HIDEHUD_HEALTH | HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_BUILDPHASE );

	m_iLastHealth = 0;
}

void CHudPlayerHealth::Init()
{
	HOOK_HUD_MESSAGE( CHudPlayerHealth, Damage );
	m_iTextureID = surface()->DrawGetTextureId( HUD_HEALTH_BAR_TEXTURE );
	Reset();
}

void CHudPlayerHealth::Reset()
{
	m_iHealth		= INIT_HEALTH;
	m_iMaxHealth	= 100;
	m_bitsDamage	= 0;
}

void CHudPlayerHealth::VidInit()
{
	Reset();
}

bool CHudPlayerHealth::ShouldDraw( void )
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;

	if ( g_pGameRules->GetGameMode() == GAMEMODE_CLASSIC && 
		ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD )
	{
		return false;
	}

	return CModHudElement::ShouldDraw();
}

void CHudPlayerHealth::OnThink( void )
{
	C_BasePlayer *pPlayer = LocalPlayer();
	if ( !pPlayer )
	{
		return;
	}

	int NewHealth = pPlayer->GetHealth();
	m_iMaxHealth = pPlayer->GetMaxHealth();

	if ( NewHealth != m_iHealth )
	{
		m_iHealth = NewHealth;
	}
}

void CHudPlayerHealth::Paint()
{
	// Get the health percentage
	float DisplayHealth = ( float )m_iHealth / m_iMaxHealth;

	Color barcolor;
	// Draw Health Bar
	if ( LocalTeamNumber() == TEAM_BLUE )
	{
		barcolor = m_BarColorBlue;
	}
	else if ( LocalTeamNumber() == TEAM_RED )
	{
		barcolor = m_BarColorRed;
	}

	surface()->DrawSetColor( barcolor );
	surface()->DrawFilledRect( m_flBarInsetX, m_flBarInsetY, 
		m_flBarWidth * ( DisplayHealth / 100.0f ), m_flBarInsetY + m_flBarHeight );
	
	// Draw Health Text
	if ( m_iLastHealth != m_iHealth )
	{
		int healthDisplay = lf_health_display_type.GetInt();
		switch ( healthDisplay )
		{
		case 0:
			m_iHealthTextLength = _snwprintf( m_cswHealthText, HEALTH_TEXT_SIZE, 
				L"Health: %d", m_iHealth );
			break;
		case 1:
			m_iHealthTextLength = _snwprintf( m_cswHealthText, HEALTH_TEXT_SIZE, 
				L"Health: %d%%", (m_iHealth * 100) / (m_iMaxHealth * 100) );
			break;
		case 2:
			m_iHealthTextLength = _snwprintf( m_cswHealthText, HEALTH_TEXT_SIZE, 
				L"Health: %03d%%", (m_iHealth * 100) / (m_iMaxHealth * 100) );
			break;
		case 3:
			m_iHealthTextLength = _snwprintf( m_cswHealthText, HEALTH_TEXT_SIZE, L"Health: %03d%%||%d/%d", 
				(m_iHealth * 100) / (m_iMaxHealth * 100), m_iHealth, m_iMaxHealth );
			break;
		case 4:
			m_iHealthTextLength = _snwprintf( m_cswHealthText, HEALTH_TEXT_SIZE, 
				L"Health: %d/%d", m_iHealth, m_iMaxHealth );
			break;
		case 5:
			m_iHealthTextLength = _snwprintf( m_cswHealthText, HEALTH_TEXT_SIZE, 
				L"Health: %d|%d", m_iHealth, m_iMaxHealth );
			break;
		default:
			m_iHealthTextLength = _snwprintf( m_cswHealthText, HEALTH_TEXT_SIZE, 
				L"Health: %d/%d", m_iHealth, m_iMaxHealth );
			break;
		}
	}

	surface()->DrawSetTextFont( m_hTextFont );
	surface()->DrawSetTextColor( Color( 255, 255, 255, 255 ) );
	surface()->DrawSetTextPos( m_flTextX, m_flTextY );
	
	surface()->DrawPrintText( m_cswHealthText, m_iHealthTextLength );
	
	m_iLastHealth = m_iHealth;
}

void CHudPlayerHealth::MsgFunc_Damage( bf_read &msg )
{
#if 0
	int armor = msg.ReadByte();	// armor
	int damageTaken = msg.ReadByte();	// health
	long bitsDamage = msg.ReadLong(); // damage bits
	bitsDamage; // variable still sent but not used

	Vector vecFrom;

	vecFrom.x = msg.ReadBitCoord();
	vecFrom.y = msg.ReadBitCoord();
	vecFrom.z = msg.ReadBitCoord();

	//HODO: Draw damage indictation in future version(Call of Duty Style)?

	// Actually took damage?
	if ( damageTaken > 0 || armor > 0 )
	{
		if ( damageTaken > 0 )
		{
			// Needs a redraw
			Update();
		}
	}
#endif
}
