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

#define BASE_TAG L"Base"
#define PLAYER_TAG L"%s"
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

	for (int i = 0; i < TEAM_COUNT; i++)
	{
		m_pAvatar [i] = NULL;
		m_iTakenByPlayer[ i ] = TAKEN_INVALID_PLAYER;
	}

	m_pAvatar[ SDK_TEAM_BLUE ] = new CAvatarImage();
	m_pAvatar[ SDK_TEAM_RED ] = new CAvatarImage();

	m_pAvatar[ SDK_TEAM_BLUE ]->SetPos( 10, 10 );
	m_pAvatar[ SDK_TEAM_RED ]->SetPos( 10, 52 );
	
	m_iTakenByPlayer[ SDK_TEAM_BLUE ] = TAKEN_INVALID_PLAYER;
	m_iTakenByPlayer[ SDK_TEAM_RED ] = TAKEN_INVALID_PLAYER;

	m_DroppedMaxTime = lf_flag_return.GetFloat();

	for (int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++)
	{
		m_FlagState[ i ] = HFLG_BASE;
		m_LastFlagState[ i ] = 0;
	}

	SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_BUILDPHASE );
}

CHudFlagCarrier::~CHudFlagCarrier ()
{
	for (int i = 0; i < TEAM_COUNT; i++)
	{
		if ( m_pAvatar[ i ])
		{
			delete m_pAvatar[ i ];
		}
	}
}

void CHudFlagCarrier::Init( void )
{
	HOOK_HUD_MESSAGE( CHudFlagCarrier, UpdateFlagCarrier );
}

void CHudFlagCarrier::LevelInit( void )
{
	m_iTakenByPlayer[ SDK_TEAM_BLUE ] = TAKEN_INVALID_PLAYER;
	m_iTakenByPlayer[ SDK_TEAM_RED ] = TAKEN_INVALID_PLAYER;

	m_DroppedMaxTime = lf_flag_return.GetFloat();

	for (int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++)
	{
		m_FlagState[ i ] = HFLG_BASE;
		m_LastFlagState[ i ] = 0;
	}
}

void CHudFlagCarrier::Reset( void )
{
	for (int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++)
	{
		UpdateFlagState( i );
	}
}

void CHudFlagCarrier::VidInit( void )
{
	for (int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++)
	{
		m_pAvatar[ i ]->ClearAvatarSteamID();
	}

	for (int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++)
	{
		m_iTakenByPlayer[ i ] = TAKEN_INVALID_PLAYER;
		Update();
	}

	m_pAvatar[ SDK_TEAM_BLUE ]->SetPos( m_flBlueAvatarX, m_flBlueAvatarY );
	m_pAvatar[ SDK_TEAM_RED ]->SetPos( m_flRedAvatarX, m_flRedAvatarY );

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
	}
	else if ( GameRules()->GetGameMode() == GAMEMODE_DOM )
	{
		return false;
	}

	return CModHudElement::ShouldDraw();
}

bool CHudFlagCarrier::NeedsUpdate ( void )
{
	for (int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++)
	{
		if ( m_LastFlagState [i] != m_FlagState [i] ||
			m_FlagState [i] == HFLG_DROPPED_TIMER)
		{
			return true;
		}
	}

	return false;
}

void CHudFlagCarrier::Update( void )
{
	for (int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++)
	{
		if ( m_LastFlagState [i] != m_FlagState [i] ||
			m_FlagState [i] == HFLG_DROPPED_TIMER)
		{
			m_LastFlagState [i] = m_FlagState [i];
			UpdateFlagState( i );
		}

		if ( m_iTakenByPlayer [i] == TAKEN_INVALID_PLAYER )
		{
			UpdateTakenAvatar( i );
		}
	}
}

void CHudFlagCarrier::UpdateFlagState( int TeamIndex )
{
	if ( m_FlagState [TeamIndex] != HFLG_DROPPED_TIMER )
	{
		m_DroppedTimer [TeamIndex].Reset();
	}

	switch ( m_FlagState [TeamIndex] )
	{
	case HFLG_BASE:
		wcscpy( m_Text[TeamIndex], BASE_TAG );
		break;
	case HFLG_PICKUP:
		if ( g_PR )
		{
			if ( TeamIndex == SDK_TEAM_BLUE ||
				TeamIndex == SDK_TEAM_RED )
			{
				int playerindex = engine->GetPlayerForUserID( m_iTakenByPlayer[ TeamIndex ] );
				const char *playername = g_PR->GetPlayerName( playerindex );
				g_pVGuiLocalize->ConvertANSIToUnicode( playername, m_Text [TEAM_UNASSIGNED], sizeof( m_Text [TEAM_UNASSIGNED] ) );
				swprintf( m_Text[ TeamIndex ], 256, PLAYER_TAG, m_Text [TEAM_UNASSIGNED] );
			}
		}
		break;
	case HFLG_DROPPED:
		wcscpy( m_Text [TeamIndex], DROPPED_TAG );
		break;
	case HFLG_DROPPED_TIMER:
		double DroppedSeconds = m_DroppedMaxTime - m_DroppedTimer [TeamIndex].GetElapsedTime();
		DroppedSeconds = clamp( DroppedSeconds, 0.0, abs( DroppedSeconds ) );
		swprintf( m_Text[ TeamIndex ], 256, DROPPED_TIMER_TAG, DroppedSeconds );
		break;
	}
}

// CODE DUPLICATION. FIXME!
void CHudFlagCarrier::UpdateTakenAvatar( int TeamIndex )
{
	if ( m_iTakenByPlayer[ TeamIndex ] != TAKEN_INVALID_PLAYER )
	{
		int playerindex = engine->GetPlayerForUserID( m_iTakenByPlayer[ TeamIndex ] );
		C_BasePlayer *pPlayer = UTIL_PlayerByIndex( playerindex );

		if ( pPlayer && steamapicontext->SteamUtils() )
		{
			player_info_t pi;
			if ( engine->GetPlayerInfo(playerindex, &pi) )
			{
				if ( pi.friendsID )
				{
					CSteamID steamIDForPlayer( pi.friendsID, 1, steamapicontext->SteamUtils()->GetConnectedUniverse(), k_EAccountTypeIndividual );

					CAvatarImage *pAvImage = m_pAvatar[ TeamIndex ];
					if ( pAvImage->SetAvatarSteamID( steamIDForPlayer ) )
					{
						Warning( "Failed to Get Player Avatar\n" );
					}
					
					// Indent the image. These are deliberately non-resolution-scaling.
					pAvImage->SetAvatarSize( 32, 32 );
					pAvImage->SetSize( pAvImage->GetWide(), 32 );
				}
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

	// Display avatars if required
	for (int i = 0; i < TEAM_COUNT; i++)
	{
		if ( !m_pAvatar[ i ] || m_iTakenByPlayer[ i ] == TAKEN_INVALID_PLAYER )
		{
			continue;
		}

		m_pAvatar[ i ]->Paint();
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
			GetTeamColor( SDK_TEAM_BLUE ).r(), 
			GetTeamColor( SDK_TEAM_BLUE ).g(), 
			GetTeamColor( SDK_TEAM_BLUE ).b(), 
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
			GetTeamColor( SDK_TEAM_RED ).r(), 
			GetTeamColor( SDK_TEAM_RED ).g(), 
			GetTeamColor( SDK_TEAM_RED ).b(), 
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
	vgui::surface()->DrawSetTextFont( m_hTextFont ); // set the font	
	vgui::surface()->DrawSetTextColor( col.r(), col.g(), col.b(), col.a() );
	vgui::surface()->DrawSetTextPos( x, y );

	int TextLength = wcslen( pText );
	vgui::surface()->DrawPrintText( pText, TextLength ); // print text
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
		int TeamIndex = teamid;
		TeamIndex = flagteamid;//GetOtherTeamNumber( TeamIndex );
		m_FlagState[ TeamIndex ] = HFLG_PICKUP;
		UpdateFlagState( TeamIndex );

		m_iTakenByPlayer[ TeamIndex ] = userid;
		UpdateTakenAvatar( TeamIndex );
	}
	else if ( event == HFLG_DROPPED || event == HFLG_DROPPED_TIMER )
	{
		m_DroppedMaxTime = data.ReadFloat();

		int TeamIndex = teamid;
		TeamIndex = flagteamid;//GetOtherTeamNumber( TeamIndex );
		m_FlagState[ TeamIndex ] = HFLG_DROPPED_TIMER;
		m_DroppedTimer[ TeamIndex ].Reset();
		m_DroppedTimer[ TeamIndex ].Start( 0.0f );
		UpdateFlagState( TeamIndex );

		m_iTakenByPlayer [TeamIndex] = TAKEN_INVALID_PLAYER;
	}
	else if ( event == HFLG_RETURNED )
	{
		int TeamIndex = teamid;
		TeamIndex = flagteamid;//GetOtherTeamNumber( TeamIndex );
		m_FlagState[ TeamIndex ] = HFLG_BASE;
		m_DroppedTimer[ TeamIndex ].Reset();
		UpdateFlagState( TeamIndex );

		m_iTakenByPlayer[ TeamIndex ] = TAKEN_INVALID_PLAYER;
	}
}
