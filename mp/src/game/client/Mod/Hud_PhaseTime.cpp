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

	Phase timer display for HUD. Also shows block count

	Last Updated Oct 30, 2009
===============================================================*/

#include "cbase.h"
#include "c_hl2mp_player.h"
#include "c_team.h"
#include "c_playerresource.h"
#include "backgroundpanel.h"

#include "Mod/ClassicGameRules.h"
#include "Mod/ClientGlobals.h"
#include "Mod/SharedModCvars.h"
#include "Mod/Hud_PhaseTime.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

DECLARE_HUDELEMENT( CHudPhaseTime );
DECLARE_HUD_MESSAGE( CHudPhaseTime, NewTime );

//
//	Name: CHudPhaseTime
//	Author: Hekar Khani
//	Description: Panel for displaying Phase time and Block count on HUD.
//	Notes:
//
CHudPhaseTime::CHudPhaseTime( const char *pElementName ) :
	CModHudElement( pElementName, HUDELEM_PHASETIME ), vgui::Panel( NULL, "HudPhaseTime" ) 
{
	SetHiddenBits( HIDEHUD_PLAYERDEAD );
	// Set our parent window
	SetParent( g_pClientMode->GetViewport() );

	SetPaintBackgroundEnabled( true );
	
	m_Phasenum = PHASE_BUILD;
	m_PhaseSeconds = lf_build_length_first.GetInt();

	m_PhaseTimer.Reset();

	ListenForGameEvent( "phase_switch" );

	// Only show in the classic gamemode
	SetGameModes( GAMEMODE_CLASSIC );
}

CHudPhaseTime::~CHudPhaseTime()
{
	StopListeningForAllEvents();
}

void CHudPhaseTime::Init( void )
{
	HOOK_HUD_MESSAGE( CHudPhaseTime, NewTime );
}

void CHudPhaseTime::Reset( void )
{
	if ( m_Phasenum == PHASE_BUILD )
	{
		wcscpy( m_TextPhase, L"B" );
	}
	else if ( m_Phasenum == PHASE_COMBAT )
	{
		wcscpy( m_TextPhase, L"C" );
	}
}

void CHudPhaseTime::VidInit( void )
{
	Reset();
}

bool CHudPhaseTime::ShouldDraw()
{
	//HODO: Add in specific options here...
	return CModHudElement::ShouldDraw();
}

bool CHudPhaseTime::NeedsUpdate( void )
{
	//HODO: Fix this to take less processing power
	return true;
}

void CHudPhaseTime::DisplayBlockCount( int x, int y, Color col )
{
	wchar_t wszText[ 63 ];

	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
	{
		return;
	}

	C_Team *pTeam = GetGlobalTeam( pPlayer->GetTeamNumber() );
	if ( !pTeam )
	{
		return;
	}

	int TeamBlockLimit = 0;
	if ( pPlayer->GetTeamNumber() == TEAM_RED )
	{
		TeamBlockLimit = lf_block_red_limit.GetInt();
	}
	else if ( pPlayer->GetTeamNumber() == TEAM_BLUE )
	{
		TeamBlockLimit = lf_block_blue_limit.GetInt();
	}

	int BlockCount = pTeam->GetBlockCount() - pTeam->GetStolenBlockCount();
	int BlockLimit = TeamBlockLimit;
	int StolenBlockCount = pTeam->GetStolenBlockCount();

	// Display differently if the number is negative
	if ( StolenBlockCount >= 0)
	{
		_snwprintf( wszText, 63, L"%d/%d + %d", BlockCount, BlockLimit, StolenBlockCount );
	}
	else
	{
		// Show the negative number with the '-' sign
		_snwprintf( wszText, 63, L"%d/%d - %d", BlockCount, BlockLimit, abs( StolenBlockCount ) );
	}

	wszText[62] = '\0';

	DisplayText( wszText, x, y, col );
}

void CHudPhaseTime::DisplayTimer( int x, int y, Color col )
{

	int TimerSeconds = m_PhaseSeconds - m_PhaseTimer.GetElapsedTime();
	if ( TimerSeconds < 0 || TimerSeconds > 32000 )
		return;

	wchar_t wszText[ 63 ];
	_snwprintf( wszText, 63, L"%d:%02d", (TimerSeconds / 60), (TimerSeconds % 60) );
	wszText[62] = '\0';

	DisplayText( wszText, x, y, col );
}

void CHudPhaseTime::DisplayPhase( int x, int y, Color col )
{
	DisplayText( m_TextPhase, x, y, col );
}

void CHudPhaseTime::Paint ( void )
{
	// Due to some limitations of the HudLayout
	// We need to do some dirty business...
	int x = m_flBackgroundX;
	int y = m_flBackgroundY;

	int Width = m_flBackgroundWidth;
	int Height = m_flBackgroundHeight;

	// Make some changes if we're right justified
	if ( m_bRightJustify )
	{
		x = GetWide() - Width;
		x -= 10;
	}

	// Draw our background
	if ( LocalTeamNumber() == TEAM_BLUE )
	{
		vgui::surface()->DrawSetColor( m_BackgroundColorBlue );
		vgui::surface()->DrawFilledRect( x, y, x + Width, y + Height );

		//DrawBrokenBorder( m_BorderColorBlue, x, y, Width, Height, 15, 10 );

		// Draw the Texts to the screen
		DisplayTimer( x + m_flTimerX, y + m_flTimerY, m_TimeColorBlue );
		DisplayBlockCount( x + m_flBlockPointsX, y + m_flBlockPointsY, m_BlockColorBlue );
		DisplayPhase( x + m_flPhaseIconX, y + m_flPhaseIconY, m_PhaseColorBlue );
	}
	else if ( LocalTeamNumber() == TEAM_RED )
	{
		vgui::surface()->DrawSetColor( m_BackgroundColorRed );
		vgui::surface()->DrawFilledRect( x, y, x + Width, y + Height );

		//DrawBrokenBorder( m_BorderColorRed, x, y, Width, Height, 15, 10 );

		// Draw the Texts to the screen
		DisplayTimer( x + m_flTimerX, y + m_flTimerY, m_TimeColorRed );
		DisplayBlockCount( x + m_flBlockPointsX, y + m_flBlockPointsY, m_BlockColorRed );
		DisplayPhase( x + m_flPhaseIconX, y + m_flPhaseIconY, m_PhaseColorRed );
	}
}

void CHudPhaseTime::PaintBackground()
{
}

void CHudPhaseTime::PaintBorder()
{	
}

void CHudPhaseTime::DisplayText (wchar_t *pText, int x, int y, Color col)
{
	vgui::surface()->DrawSetTextFont( m_hTextFont ); // set the font
	vgui::surface()->DrawSetTextColor( col.r(), col.g(), col.b(), col.a() );
	vgui::surface()->DrawSetTextPos( x, y );

	int TextLength = wcslen( pText );
	vgui::surface()->DrawPrintText( pText, TextLength ); // print text
}

void CHudPhaseTime::FireGameEvent(IGameEvent* pEvent)
{
	CModHudElement::FireGameEvent( pEvent );
	if (!Q_strcmp (pEvent->GetName(), "phase_switch"))
	{
		m_Phasenum = pEvent->GetInt( "phaseid" );
	}
}

void CHudPhaseTime::MsgFunc_NewTime( bf_read &msg )
{
	int newphase = msg.ReadLong();
	float newtime = msg.ReadFloat();

	m_Phasenum = newphase;
	m_PhaseSeconds = newtime;
	m_PhaseTimer.Reset();
	m_PhaseTimer.Start( 0.0f );

	//HODO: remove this hack
//	ClassicGameRules()->m_iCurrentPhaseID = m_Phasenum;
}
