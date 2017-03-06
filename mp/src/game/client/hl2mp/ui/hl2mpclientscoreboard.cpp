//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "hud.h"
#include "hl2mpclientscoreboard.h"
#include "c_team.h"
#include "c_playerresource.h"
#include "Mod/ClassicGameRules.h"
#include "Mod/HudH_Utils.h"

#include <KeyValues.h>

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vgui_controls/SectionedListPanel.h>

using namespace vgui;
// id's of sections used in the scoreboard
enum EScoreboardSections
{
	SCORESECTION_TEAM1 = 1,
	SCORESECTION_TEAM2 = 2,
	SCORESECTION_FREEFORALL = 3
};

CHL2MPClientScoreBoardDialog::CHL2MPClientScoreBoardDialog( IViewPort *pViewPort ) :CClientScoreBoardDialog( pViewPort )
{
	// load the new scheme early!!
	SetScheme( "SourceScheme" );

	m_pSpecLabel = NULL;
}

CHL2MPClientScoreBoardDialog::~CHL2MPClientScoreBoardDialog( )
{
}

void CHL2MPClientScoreBoardDialog::PaintBackground( )
{
	m_pBlueList->SetBgColor( Color( 0, 0, 0, 0 ) );
	m_pBlueList->SetBorder( NULL );

	m_pRedList->SetBgColor( Color( 0, 0, 0, 0 ) );
	m_pRedList->SetBorder( NULL );

	int wide, tall;
	GetSize( wide, tall );

	DrawBackground( m_bgColor, 0, 0, wide, tall );
}

void CHL2MPClientScoreBoardDialog::PaintBorder( )
{
	int wide, tall;
	GetSize( wide, tall );

	DrawBorder( m_borderColor, wide, tall );
}

void CHL2MPClientScoreBoardDialog::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	m_bgColor = GetSchemeColor( "SectionedListPanel.BgColor", GetBgColor( ), pScheme );
	m_borderColor = pScheme->GetColor( "Lum.GameBorder", Color( 0, 0, 0, 0 ) );

	int Alpha = 192;
	m_bgColor.SetColor( m_bgColor.r( ), m_bgColor.g( ), m_bgColor.b( ), Alpha );
	m_borderColor.SetColor( m_borderColor.r( ), m_borderColor.g( ), m_borderColor.b( ), Alpha );

	SetBgColor( Color( 0, 0, 0, 0 ) );
	SetBorder( pScheme->GetBorder( "BaseBorder" ) );

	m_pSpecLabel = dynamic_cast< vgui::Label * > (FindChildByName( "SpecLabel", true ));

	AssertMsg( m_pSpecLabel, "Spectator Label Not Set in Resources/UI/Scoreboard.res!!" );
}


//-----------------------------------------------------------------------------
// Purpose: sets up base sections
//-----------------------------------------------------------------------------
void CHL2MPClientScoreBoardDialog::InitScoreboardSections( )
{
	if ( ClassicGameRules( )->IsTeamplay( ) )
	{
		m_pBlueList->SetBgColor( Color( 0, 0, 0, 0 ) );
		m_pBlueList->SetBorder( NULL );
		m_pRedList->SetBgColor( Color( 0, 0, 0, 0 ) );
		m_pRedList->SetBorder( NULL );

		AddHeader( m_pBlueList );
		AddHeader( m_pRedList );

		// add the team sections
		AddSection( m_pBlueList, TYPE_TEAM, SDK_TEAM_BLUE );
		AddSection( m_pRedList, TYPE_TEAM, SDK_TEAM_RED );
	}
}

//-----------------------------------------------------------------------------
// Purpose: resets the scoreboard team info
//-----------------------------------------------------------------------------
void CHL2MPClientScoreBoardDialog::UpdateTeamInfo( )
{
	if ( GameResources( ) == NULL )
		return;

	int iNumPlayersInGame = 0;

	for ( int j = 1; j <= gpGlobals->maxClients; j++ )
	{
		if ( GameResources( )->IsConnected( j ) )
		{
			iNumPlayersInGame++;
		}
	}

	// update the team sections in the scoreboard
	// Do not do Spectators!
	for ( int i = TEAM_START; i <= TEAM_LAST; i++ )
	{
		wchar_t *teamName = NULL;
		int sectionID = 0;
		C_Team *team = GetGlobalTeam( i );

		if ( team )
		{
			sectionID = GetSectionFromTeamNumber( i );

			// update team name
			wchar_t name[64];
			wchar_t string1[1024];
			wchar_t wNumPlayers[6];

			_snwprintf( wNumPlayers, 6, L"%i", team->Get_Number_Players( ) );

			if ( !teamName && team )
			{
				g_pVGuiLocalize->ConvertANSIToUnicode( team->Get_Name( ), name, sizeof(name) );
				teamName = name;
			}

			if ( team->Get_Number_Players( ) == 1 )
			{
				g_pVGuiLocalize->ConstructString( string1, sizeof(string1), g_pVGuiLocalize->Find( "#SDK_ScoreBoard_Player" ), 2, teamName, wNumPlayers );
			}
			else
			{
				g_pVGuiLocalize->ConstructString( string1, sizeof(string1), g_pVGuiLocalize->Find( "#SDK_ScoreBoard_Players" ), 2, teamName, wNumPlayers );
			}

			// update stats
			wchar_t val[6];
			swprintf( val, L"%d", team->Get_Score( ) );
			GetList( i )->ModifyColumn( sectionID, "frags", val );
			if ( team->Get_Ping( ) < 1 )
			{
				GetList( i )->ModifyColumn( sectionID, "ping", L"" );
			}
			else
			{
				swprintf( val, L"%d", team->Get_Ping( ) );
				GetList( i )->ModifyColumn( sectionID, "ping", val );
			}

			GetList( i )->ModifyColumn( sectionID, "name", string1 );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: adds the top header of the scoreboars
//-----------------------------------------------------------------------------
void CHL2MPClientScoreBoardDialog::AddHeader( vgui::SectionedListPanel *pList )
{
	// add the top header
	pList->AddSection( 0, "" );
	pList->SetSectionAlwaysVisible( 0, true );
	HFont hFallbackFont = scheme( )->GetIScheme( GetScheme( ) )->GetFont( "ClassMenuDefaultVerySmall", false );

	pList->AddColumnToSection( 0, "avatar", "", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_CENTER, m_iAvatarWidth );
	pList->AddColumnToSection( 0, "name", "Name", 0, m_iNameWidth, hFallbackFont );
	pList->AddColumnToSection( 0, "class", "Class", 0, m_iClassWidth, hFallbackFont );
	pList->AddColumnToSection( 0, "frags", "Score", 0 | SectionedListPanel::COLUMN_RIGHT, m_iScoreWidth, hFallbackFont );
	pList->AddColumnToSection( 0, "ping", "", 0 | SectionedListPanel::COLUMN_RIGHT, m_iPingWidth, hFallbackFont );
}

//-----------------------------------------------------------------------------
// Purpose: Adds a new section to the scoreboard (i.e the team header)
//-----------------------------------------------------------------------------
void CHL2MPClientScoreBoardDialog::AddSection( vgui::SectionedListPanel *pList, int teamType, int teamNumber )
{
	HFont hFallbackFont = scheme( )->GetIScheme( GetScheme( ) )->GetFont( "ClassMenuDefaultVerySmall", false );

	int sectionID = GetSectionFromTeamNumber( teamNumber );
	if ( teamType == TYPE_TEAM )
	{
		pList->AddSection( sectionID, "", StaticPlayerSortFunc );

		// setup the columns
		pList->AddColumnToSection( sectionID, "avatar", "", SectionedListPanel::COLUMN_IMAGE | SectionedListPanel::COLUMN_CENTER, m_iAvatarWidth );
		pList->AddColumnToSection( sectionID, "name", "", 0, m_iNameWidth, hFallbackFont );
		pList->AddColumnToSection( sectionID, "class", "", 0, m_iClassWidth, hFallbackFont );
		pList->AddColumnToSection( sectionID, "frags", "", SectionedListPanel::COLUMN_RIGHT, m_iScoreWidth );
		pList->AddColumnToSection( sectionID, "ping", "", SectionedListPanel::COLUMN_RIGHT, m_iPingWidth );

		// set the section to have the team color
		if ( GameResources( ) )
		{
			const Color& col = GetTeamColor( teamNumber, TEAM_CLR_DARK );
			pList->SetSectionFgColor( 1, col );
		}

		//Tony; don't make unassigned always visible when using teams.
		if ( teamNumber != TEAM_UNASSIGNED )
		{
			pList->SetSectionAlwaysVisible( sectionID );
		}
	}
}

int CHL2MPClientScoreBoardDialog::GetSectionFromTeamNumber( int teamNumber )
{
	switch ( teamNumber )
	{
#if defined ( SDK_USE_TEAMS )
	case SDK_TEAM_BLUE:
		return SCORESECTION_TEAM1;
	case SDK_TEAM_RED:
		return SCORESECTION_TEAM2;
#endif
	case TEAM_SPECTATOR:
		AssertMsg( 0, "Scoreboard trying to get section for Spectator!" );
		return -1;
	default:
		return SCORESECTION_FREEFORALL;
	}
	return SCORESECTION_FREEFORALL;
}

void CHL2MPClientScoreBoardDialog::UpdatePlayerInfo( )
{
	m_iSectionId = 0; // 0'th row is a header
	int selectedRow = -1;
	int i;

	CBasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer( );

	if ( !pPlayer || !GameResources( ) )
		return;

	IGameResources *sdkPR = GameResources( );


	int localteam = pPlayer->GetTeamNumber( );
	C_BasePlayer *pOther = NULL;

	// Do not do spectators!! Save that for later.
	for ( int teamindex = TEAM_START; teamindex <= TEAM_LAST; teamindex++ )
	{
		// walk all the players and make sure they're in the scoreboard
		for ( i = 1; i <= gpGlobals->maxClients; i++ )
		{
			if ( sdkPR->GetTeam( i ) != teamindex )
				continue;

			bool shouldShow = sdkPR->IsConnected( i );
			if ( shouldShow )
			{
				// add the player to the list
				KeyValues *kv = new KeyValues( "data" );
				kv->SetInt( "playerIndex", i );
				kv->SetInt( "team", sdkPR->GetTeam( i ) );
				kv->SetString( "name", sdkPR->GetPlayerName( i ) );
				kv->SetInt( "deaths", sdkPR->GetDeaths( i ) );
				kv->SetInt( "frags", sdkPR->GetPlayerScore( i ) );

				//Tony; for player classname, just look up directly from the player.
				pOther = UTIL_PlayerByIndex( i );
				if ( pOther )
				{
					int ot = pOther->GetTeamNumber( );
					if ( ot == localteam || localteam == TEAM_UNASSIGNED )
					{
						kv->SetString( "class", ClassicGameRules( )->GetPlayerClassName( 0, ot ) );
					}
					else
					{
						kv->SetString( "class", "" );
					}
				}
				else
				{
					kv->SetString( "class", "" );
				}

				UpdatePlayerAvatar( i, kv );

				if ( sdkPR->GetPing( i ) < 1 )
				{
					if ( sdkPR->IsFakePlayer( i ) )
					{
						kv->SetString( "ping", "BOT" );
					}
					else
					{
						kv->SetString( "ping", "" );
					}
				}
				else
				{
					kv->SetInt( "ping", sdkPR->GetPing( i ) );
				}

				int itemID = FindItemIDForPlayerIndex( i );
				int sectionID = GetSectionFromTeamNumber( sdkPR->GetTeam( i ) );

				if ( itemID == -1 )
				{
					// add a new row
					itemID = GetList( teamindex )->AddItem( sectionID, kv );
				}
				else
				{
					// modify the current row
					GetList( teamindex )->ModifyItem( itemID, sectionID, kv );
				}

				if ( i == pPlayer->entindex( ) )
				{
					selectedRow = itemID;        // this is the local player, hilight this row
				}

				// set the row color based on the player's team
				GetList( teamindex )->SetItemFgColor( itemID, GetTeamColor( sdkPR->GetTeam( i ) ) );

				kv->deleteThis( );
			}
			else
			{
				// remove the player
				int itemID = FindItemIDForPlayerIndex( i );
				if ( itemID != -1 )
				{
					GetList( teamindex )->RemoveItem( itemID );
				}
			}
		}

		if ( selectedRow != -1 && localteam == teamindex )
		{
			GetList( teamindex )->SetSelectedItem( selectedRow );
		}
	}

	// Create the spectator label
#define SPECLIST_LEN 1024
	wchar_t speclist[SPECLIST_LEN];
	wcsncpy( speclist, L"Spectators: ", SPECLIST_LEN );
	C_Team *spec = GetGlobalTeam( TEAM_SPECTATOR );
	if ( spec )
	{
		int spectatorCount = 0;
		for ( i = 1; i <= gpGlobals->maxClients; i++ )
		{
			if ( sdkPR->GetTeam( i ) != TEAM_SPECTATOR )
				continue;

			wchar_t playername[128];
			g_pVGuiLocalize->ConvertANSIToUnicode( sdkPR->GetPlayerName( i ), playername, 128 );

			if ( i != 1 )
			{
				wcsncat( speclist, L",", SPECLIST_LEN );
			}
			wcsncat( speclist, playername, SPECLIST_LEN );
			spectatorCount++;
		}

		if ( m_pSpecLabel )
		{
			if ( spectatorCount > 0 )
			{
				m_pSpecLabel->SetText( speclist );
			}
			else
			{
				m_pSpecLabel->SetText( "" );
			}
		}
	}
}
