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

	TODO:
		- Add team scores here
		- Associate avatars with C_MOD_Player and allow a single
		  get method to get the player's avatar

	Last Updated Sept 12, 2009
=================================================================*/

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

#include "Mod/ClassicGameRules.h"
#include "Mod/ClientGlobals.h"
#include "Mod/SharedModCvars.h"
#include "Mod/Hud_FlagCarrier.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define TAKEN_INVALID_PLAYER -1

#define DROPPED_TAG L"Dropped"
#define DROPPED_TIMER_TAG L"Dropped: %.1f"

DECLARE_HUDELEMENT( CHudFlagCarrier );
DECLARE_HUD_MESSAGE( CHudFlagCarrier, UpdateFlagCarrier );

//
//	Name: CHudFlagCarrier
//	Author: Hekar Khani
//	Description: HUD panel for flag carrier
//	Notes: 
//
CHudFlagCarrier::CHudFlagCarrier( const char *pElementName ) :
	CModHudElement( pElementName, HUDELEM_FLAGCARRIER ), vgui::Panel( NULL, "HudFlagCarrier" ) 
{
	// Set our parent window
	SetParent( g_pClientMode->GetViewport() );
	vgui::HScheme scheme = vgui::scheme()->GetScheme( "ClientScheme" );
	m_hTextFont = vgui::scheme()->GetIScheme( scheme )->GetFont( "SFHUDLarge" );

	SetPaintBackgroundEnabled( true );

	SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_BUILDPHASE );
}

CHudFlagCarrier::~CHudFlagCarrier ()
{
}

void CHudFlagCarrier::Init( void )
{
	HOOK_HUD_MESSAGE( CHudFlagCarrier, UpdateFlagCarrier );
}

void CHudFlagCarrier::LevelInit( void )
{
	m_iTakenByPlayer[ TEAM_UNASSIGNED ] = TAKEN_INVALID_PLAYER;
	m_iTakenByPlayer[ TEAM_SPECTATOR ] = TAKEN_INVALID_PLAYER;
	m_iTakenByPlayer[ TEAM_BLUE ] = TAKEN_INVALID_PLAYER;
	m_iTakenByPlayer[ TEAM_RED ] = TAKEN_INVALID_PLAYER;

	m_DroppedMaxTime = lf_flag_return.GetFloat();

	m_FlagState[ TEAM_BLUE ] = HFLG_BASE;
	m_LastFlagState[ TEAM_BLUE ] = 0;

	m_FlagState[ TEAM_RED ] = HFLG_BASE;
	m_LastFlagState[ TEAM_RED ] = 0;
}

void CHudFlagCarrier::Reset( void )
{
	UpdateFlagState( TEAM_BLUE );
	UpdateFlagState( TEAM_RED );
}

void CHudFlagCarrier::VidInit( void )
{
	m_pAvatar[ TEAM_BLUE ].ClearAvatarSteamID();
	m_pAvatar[ TEAM_RED ].ClearAvatarSteamID();

	m_iTakenByPlayer[ TEAM_BLUE ] = TAKEN_INVALID_PLAYER;
	m_iTakenByPlayer[ TEAM_RED ] = TAKEN_INVALID_PLAYER;

	m_pAvatar[ TEAM_BLUE ].SetPos( m_flBlueAvatarX, m_flBlueAvatarY );
	m_pAvatar[ TEAM_RED ].SetPos( m_flRedAvatarX, m_flRedAvatarY );

	Update();
	Reset();
}

bool CHudFlagCarrier::ShouldDraw()
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;

	if ( GameRules()->GetGameMode() == GAMEMODE_CLASSIC )
	{
		if ( ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD )
		{
			return false;
		}
		else
		{
			return m_FlagState[ TEAM_RED ] != HFLG_BASE ||
				m_FlagState[ TEAM_BLUE ] != HFLG_BASE;
		}
	}
	else if ( GameRules()->GetGameMode() == GAMEMODE_DOM )
	{
		return false;
	}

	return CModHudElement::ShouldDraw();
}

bool CHudFlagCarrier::NeedsUpdate ( void )
{
	for ( int i = TEAM_BLUE; i <= TEAM_RED; i++ )
	{
		if ( m_LastFlagState[ i ] != m_FlagState[ i ] ||
			m_FlagState[ i ] == HFLG_DROPPED_TIMER )
		{
			return true;
		}
	}

	return false;
}

void CHudFlagCarrier::Update( void )
{
	for ( int i = TEAM_BLUE; i <= TEAM_RED; i++ )
	{
		if ( m_LastFlagState[ i ] != m_FlagState[ i ] ||
			m_FlagState[ i ] == HFLG_DROPPED_TIMER)
		{
			m_LastFlagState[ i ] = m_FlagState[ i ];
			UpdateFlagState( i );
		}

		if ( m_iTakenByPlayer[ i ] == TAKEN_INVALID_PLAYER )
		{
			UpdateTakenAvatar( i );
		}
	}
}

void CHudFlagCarrier::UpdateFlagState( int teamIndex )
{
	if ( m_FlagState[ teamIndex ] != HFLG_DROPPED_TIMER )
	{
		m_DroppedTimer[ teamIndex ].Reset();
	}

	switch ( m_FlagState[ teamIndex ] )
	{
	case HFLG_BASE:
		m_Text[ teamIndex ][ 0 ] = '\0';
		break;
	case HFLG_PICKUP:
		if ( g_PR && ( teamIndex == TEAM_BLUE || teamIndex == TEAM_RED ) )
		{
			int playerindex = engine->GetPlayerForUserID( m_iTakenByPlayer[ teamIndex ] );
			const char *playername = g_PR->GetPlayerName( playerindex );
			g_pVGuiLocalize->ConvertANSIToUnicode( playername, m_Text[ TEAM_UNASSIGNED ], 256 );
			Q_wcsncpy( m_Text[ teamIndex ], m_Text[ TEAM_UNASSIGNED ], 256 );
		}
		break;
	case HFLG_DROPPED:
		wcscpy( m_Text[ teamIndex ], DROPPED_TAG );
		break;
	case HFLG_DROPPED_TIMER:
		double DroppedSeconds = m_DroppedMaxTime - m_DroppedTimer[ teamIndex ].GetElapsedTime();
		DroppedSeconds = clamp( DroppedSeconds, 0.0, abs( DroppedSeconds ) );
		swprintf( m_Text[ teamIndex ], 256, DROPPED_TIMER_TAG, DroppedSeconds );
		break;
	}
}

// CODE DUPLICATION. FIXME!
void CHudFlagCarrier::UpdateTakenAvatar( int teamIndex )
{
	if ( m_iTakenByPlayer[ teamIndex ] != TAKEN_INVALID_PLAYER )
	{
		int playerindex = engine->GetPlayerForUserID( m_iTakenByPlayer[ teamIndex ] );
		C_BasePlayer *pPlayer = UTIL_PlayerByIndex( playerindex );

		if ( pPlayer && steamapicontext->SteamUtils() )
		{
			player_info_t pi;
			if ( engine->GetPlayerInfo(playerindex, &pi) && pi.friendsID )
			{
				CSteamID steamIDForPlayer(
					pi.friendsID,
					1,
					steamapicontext->SteamUtils()->GetConnectedUniverse(),
					k_EAccountTypeIndividual
				);

				CAvatarImage& pAvImage = m_pAvatar[ teamIndex ];
				if ( !pAvImage.SetAvatarSteamID( steamIDForPlayer ) )
				{
					Warning( "Failed to Get Player Avatar\n" );
				}
				
				// Indent the image. These are deliberately non-resolution-scaling.
				pAvImage.SetAvatarSize( 32, 32 );
				pAvImage.SetSize( pAvImage.GetWide(), 32 );
			}
		}
	}
}

void CHudFlagCarrier::OnThink( void )
{
	if ( NeedsUpdate() )
	{
		Update();
	}
}

void CHudFlagCarrier::Paint ( void )
{
	SetBgColor( Color( GetBgColor().r(), GetBgColor().g(), GetBgColor().b(), 128 ) );

	DisplayText
	(
		m_Text[ TEAM_BLUE ], 
		m_flBlueTextX, 
		m_flBlueTextY,
		m_BlueForegroundColor
	);
	
	DisplayText
	(
		m_Text[ TEAM_RED ], 
		m_flRedTextX,
		m_flRedTextY, 
		m_RedForegroundColor
	);

	if ( m_iTakenByPlayer[ TEAM_RED ] != TAKEN_INVALID_PLAYER )
	{
		m_pAvatar[ TEAM_RED ].Paint();
	}

	if ( m_iTakenByPlayer[ TEAM_BLUE ] != TAKEN_INVALID_PLAYER )
	{
		m_pAvatar[ TEAM_BLUE ].Paint();
	}
}

void CHudFlagCarrier::PaintBackground( void )
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
			GetTeamColor( TEAM_BLUE ).r(), 
			GetTeamColor( TEAM_BLUE ).g(), 
			GetTeamColor( TEAM_BLUE ).b(), 
			200
		);

#if 0
		DrawBrokenBorder
		( 
			BlueBorderColor, 
			m_flBlueRectX, 
			m_flBlueRectY, 
			m_flBlueRectWidth,
			m_flBlueRectHeight,
			12, 8
		);
#endif // 0
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
			GetTeamColor( TEAM_RED ).r(), 
			GetTeamColor( TEAM_RED ).g(), 
			GetTeamColor( TEAM_RED ).b(), 
			200
		);

#if 0
		DrawBrokenBorder
		(
			RedBorderColor, 
			m_flRedRectX, 
			m_flRedRectY, 
			m_flRedRectWidth, 
			m_flRedRectHeight,
			12, 8
		);
#endif // 0
	}
}

void CHudFlagCarrier::DisplayText (wchar_t *pText, int x, int y, Color col)
{
	vgui::surface()->DrawSetTextFont( m_hTextFont );
	vgui::surface()->DrawSetTextColor( col.r(), col.g(), col.b(), col.a() );
	vgui::surface()->DrawSetTextPos( x, y );

	int TextLength = wcslen( pText );
	vgui::surface()->DrawPrintText( pText, TextLength );
}

/*=======================================================
	Protocol:
		EventType	[long]
		teamid		[long]
		userid		[long]
=======================================================*/
void CHudFlagCarrier::MsgFunc_UpdateFlagCarrier( bf_read& data )
{
	int event = data.ReadLong();
	int teamid = data.ReadLong();
	int flagteamid = data.ReadLong();
	int userid = data.ReadLong();

	if ( event == HFLG_PICKUP )
	{
		int teamIndex = flagteamid;
		m_FlagState[ teamIndex ] = HFLG_PICKUP;
		UpdateFlagState( teamIndex );

		m_iTakenByPlayer[ teamIndex ] = userid;
		UpdateTakenAvatar( teamIndex );
	}
	else if ( event == HFLG_DROPPED || event == HFLG_DROPPED_TIMER )
	{
		m_DroppedMaxTime = data.ReadFloat();

		int teamIndex = flagteamid;
		m_FlagState[ teamIndex ] = HFLG_DROPPED_TIMER;
		m_DroppedTimer[ teamIndex ].Reset();
		m_DroppedTimer[ teamIndex ].Start( 0.0f );
		UpdateFlagState( teamIndex );

		m_iTakenByPlayer[ teamIndex ] = TAKEN_INVALID_PLAYER;
	}
	else if ( event == HFLG_RETURNED )
	{
		int teamIndex = flagteamid;
		m_FlagState[ teamIndex ] = HFLG_BASE;
		m_DroppedTimer[ teamIndex ].Reset();
		UpdateFlagState( teamIndex );

		m_iTakenByPlayer[ teamIndex ] = TAKEN_INVALID_PLAYER;
	}
}
