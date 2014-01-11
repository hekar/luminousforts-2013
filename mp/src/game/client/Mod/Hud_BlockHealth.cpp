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

	Taken from the targetID info hud display.
	Just shows the block health to the player when
	they put their crosshair over a block and the panel
	decides it's okay to display the health.
	
	Modified by Vonce8 for traceline distancing

	Last Updated July 6, 2010
=================================================================*/

#include "cbase.h"
#include "c_hl2mp_player.h"
#include "c_playerresource.h"
#include "vgui/ILocalize.h"

#include "Mod/C_BlockBase.h"
#include "Mod/Hud_BlockHealth.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define BLOCK_HINT_DISTANCE	150
#define BLOCK_HINT_DISTANCE_SQ	( BLOCK_HINT_DISTANCE * BLOCK_HINT_DISTANCE )

DECLARE_HUDELEMENT( CHUDBlockHealth );

//
//	Name: CHUDBlockHealth
//	Author: Hekar Khani
//	Description: HUD display for showing block health
//	Notes: 
//
CHUDBlockHealth::CHUDBlockHealth( const char *pElementName, const char *pPanelName ) :
	CModHudElement( pElementName, HUDELEM_BLOCKHEALTH ),
	BaseClass( NULL, pPanelName )
{
	SetHiddenBits( 0 );
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	m_flLastChangeTime = 0;
	m_iLastEntIndex = 0;

	SetGameModes( GAMEMODE_CLASSIC );
}

void CHUDBlockHealth::Init( void )
{
}

void CHUDBlockHealth::VidInit()
{
	m_flLastChangeTime = 0;
	m_iLastEntIndex = 0;
}

bool CHUDBlockHealth::ShouldDraw( void )
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer || !pPlayer->GetActiveWeapon() )
		return false;

	// Make sure they're holding one of the physcannons
	if ( Q_strcmp( pPlayer->GetActiveWeapon()->GetName(), "weapon_lf_build_cannon" ) &&
		Q_strcmp( pPlayer->GetActiveWeapon()->GetName(), "weapon_lf_combat_cannon" ) )
	{
		return false;
	}

	return CModHudElement::ShouldDraw();
}

C_BaseEntity *CHUDBlockHealth::ProcessTarget( void )
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();

	if ( !pPlayer )
		return NULL;

	// Get our target's ent index
	int iEntIndex = pPlayer->GetIDTarget();

	// Didn't find one?
	if ( !iEntIndex )
	{
		// Check to see if we should clear our ID
		if ( m_flLastChangeTime && (gpGlobals->curtime > (m_flLastChangeTime + 0.5)) )
		{
			m_flLastChangeTime = 0;
			m_iLastEntIndex = 0;
			m_bBlockInTarget = false;
		}
		else
		{
			// Keep re-using the old one
			iEntIndex = m_iLastEntIndex;
		}
	}
	else
	{
		m_flLastChangeTime = gpGlobals->curtime;
	}

	if ( iEntIndex )
	{
		// set up the vectors and traceline
		trace_t tr;
		Vector	vecStart, vecStop, vecDir;

		// get the angles
		AngleVectors( pPlayer->EyeAngles(), &vecDir );

		// get the vectors
		vecStart = pPlayer->EyePosition();
		vecStop = vecStart + vecDir * BLOCK_HINT_DISTANCE;

		// do the traceline
		UTIL_TraceLine( vecStart, vecStop, MASK_ALL, pPlayer, COLLISION_GROUP_NPC, &tr );

		C_BaseEntity *pEntity = tr.m_pEnt;

		// check to see if we found a block
		if ( !pEntity )
			return NULL;

		return pEntity;
	}

	return NULL;
}

//-----------------------------------------------------------------------------
// Purpose: Draw function for the element
//-----------------------------------------------------------------------------
void CHUDBlockHealth::Paint()
{
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

			Color c = GetTeamColor( pBlock->GetTeamNumber() );

			float BlockHealth = pBlock->GetHealth();
			float MaxBlockHealth = pBlock->GetMaxHealth();
			float HealthRatio = ( BlockHealth / MaxBlockHealth ) * 100;

			wchar_t wszHealthText[ 32 ];
			unsigned int HealthTextLen = ARRAYSIZE( wszHealthText );
			_snwprintf( wszHealthText, HealthTextLen - 1, L"%.0f%% | %d", HealthRatio, pBlock->GetBlockWorth( ) );
			wszHealthText[ HealthTextLen - 1 ] = '\0';

			g_pVGuiLocalize->ConstructString( sIDString, sizeof( sIDString ), 
				L"Health | %s1", 1, wszHealthText );

			// Check if we have anything to output
			if ( sIDString[0] )
			{
				int wide, tall;
				vgui::surface()->GetTextSize( m_hFont, sIDString, wide, tall );
				
				int ypos = YRES(260);
				int xpos = (ScreenWidth() - wide) / 2;

				vgui::surface()->DrawSetTextFont( m_hFont );
				vgui::surface()->DrawSetTextPos( xpos, ypos );
				vgui::surface()->DrawSetTextColor( c );
				vgui::surface()->DrawPrintText( sIDString, wcslen(sIDString) );
			}
		}
	}
}

void CHUDBlockHealth::PaintBackground()
{
}

void CHUDBlockHealth::ApplySchemeSettings( vgui::IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );
}
