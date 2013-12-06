//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//===========================================================================//

#include "cbase.h"
#include <stdio.h>

#include <cdll_client_int.h>
#include <cdll_util.h>
#include <globalvars_base.h>
#include <igameresources.h>
#include "IGameUIFuncs.h" // for key bindings
#include "inputsystem/iinputsystem.h"
#include "clientscoreboarddialog.h"
#include <voice_status.h>

#include <vgui/IScheme.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IVGui.h>
#include <vstdlib/IKeyValuesSystem.h>

#include <KeyValues.h>
#include <vgui_controls/ImageList.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/SectionedListPanel.h>

#include <game/client/iviewport.h>
#include <igameresources.h>

#include "vgui_avatarimage.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

using namespace vgui;

bool AvatarIndexLessFunc( const int &lhs, const int &rhs )
{
	return lhs < rhs;
}

CClientScoreBoardDialog::CClientScoreBoardDialog( IViewPort *pViewPort ) : EditablePanel( NULL, PANEL_SCOREBOARD )
{
	m_iPlayerIndexSymbol = KeyValuesSystem( )->GetSymbolForString( "playerIndex" );
	m_nCloseKey = BUTTON_CODE_INVALID;

	//memset(s_VoiceImage, 0x0, sizeof( s_VoiceImage ));
	TrackerImage = 0;
	m_pViewPort = pViewPort;

	// initialize dialog
	SetProportional( true );
	SetKeyBoardInputEnabled( false );
	SetMouseInputEnabled( false );

	// set the scheme before any child control is created
	SetScheme( "ClientScheme" );

	m_pBlueList = new SectionedListPanel( this, "BlueList" );
	m_pBlueList->SetVerticalScrollbar( false );

	m_pRedList = new SectionedListPanel( this, "RedList" );
	m_pRedList->SetVerticalScrollbar( false );

	LoadControlSettings( "Resource/UI/ScoreBoard.res" );
	m_iDesiredHeight = GetTall( );
	m_pBlueList->SetVisible( false ); // hide this until we load the images in applyschemesettings
	m_pRedList->SetVisible( false );

	m_HLTVSpectators = 0;

	// update scoreboard instantly if on of these events occure
	ListenForGameEvent( "hltv_status" );
	ListenForGameEvent( "server_spawn" );

	m_pImageList = NULL;

	m_mapAvatarsToImageList.SetLessFunc( AvatarIndexLessFunc );
	m_mapAvatarsToImageList.RemoveAll( );
	memset( &m_iImageAvatars, 0, sizeof(int)* (MAX_PLAYERS + 1) );
}

CClientScoreBoardDialog::~CClientScoreBoardDialog( )
{
	if ( NULL != m_pImageList )
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

//-----------------------------------------------------------------------------
// Call every frame
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::OnThink( )
{
	BaseClass::OnThink( );

	// NOTE: this is necessary because of the way input works.
	// If a key down message is sent to vgui, then it will get the key up message
	// Sometimes the scoreboard is activated by other vgui menus, 
	// sometimes by console commands. In the case where it's activated by
	// other vgui menus, we lose the key up message because this panel
	// doesn't accept keyboard input. It *can't* accept keyboard input
	// because another feature of the dialog is that if it's triggered
	// from within the game, you should be able to still run around while
	// the scoreboard is up. That feature is impossible if this panel accepts input.
	// because if a vgui panel is up that accepts input, it prevents the engine from
	// receiving that input. So, I'm stuck with a polling solution.
	// 
	// Close key is set to non-invalid when something other than a keybind
	// brings the scoreboard up, and it's set to invalid as soon as the 
	// dialog becomes hidden.
	if ( m_nCloseKey != BUTTON_CODE_INVALID )
	{
		if ( !g_pInputSystem->IsButtonDown( m_nCloseKey ) )
		{
			m_nCloseKey = BUTTON_CODE_INVALID;
			gViewPortInterface->ShowPanel( PANEL_SCOREBOARD, false );
			GetClientVoiceMgr( )->StopSquelchMode( );
		}
	}
}

//-----------------------------------------------------------------------------
// Called by vgui panels that activate the client scoreboard
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::OnPollHideCode( int code )
{
	m_nCloseKey = (ButtonCode_t)code;
}

//-----------------------------------------------------------------------------
// Purpose: clears everything in the scoreboard and all it's state
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::Reset( )
{
	// clear
	m_pBlueList->DeleteAllItems( );
	m_pBlueList->RemoveAllSections( );

	m_pRedList->DeleteAllItems( );
	m_pRedList->RemoveAllSections( );

	m_iSectionId = 0;
	m_fNextUpdateTime = 0;
	// add all the sections
	InitScoreboardSections( );
}

//-----------------------------------------------------------------------------
// Purpose: adds all the team sections to the scoreboard
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::InitScoreboardSections( )
{
}

//-----------------------------------------------------------------------------
// Purpose: sets up screen
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::ApplySchemeSettings( IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	if ( m_pImageList )
		delete m_pImageList;
	m_pImageList = new ImageList( false );

	m_mapAvatarsToImageList.RemoveAll( );
	memset( &m_iImageAvatars, 0, sizeof(int)* (MAX_PLAYERS + 1) );

	PostApplySchemeSettings( pScheme );
}

//-----------------------------------------------------------------------------
// Purpose: Does dialog-specific customization after applying scheme settings.
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::PostApplySchemeSettings( vgui::IScheme *pScheme )
{
	// resize the images to our resolution
	for ( int i = 0; i < m_pImageList->GetImageCount( ); i++ )
	{
		int wide, tall;
		m_pImageList->GetImage( i )->GetSize( wide, tall );
		m_pImageList->GetImage( i )->SetSize( scheme( )->GetProportionalScaledValueEx( GetScheme( ), wide ), scheme( )->GetProportionalScaledValueEx( GetScheme( ), tall ) );
	}

	m_pBlueList->SetImageList( m_pImageList, false );
	m_pBlueList->SetVisible( true );

	m_pRedList->SetImageList( m_pImageList, false );
	m_pRedList->SetVisible( true );

	// light up scoreboard a bit
	SetBgColor( Color( 0, 0, 0, 0 ) );
}

void CClientScoreBoardDialog::ShowPanel( bool bShow )
{
	// Catch the case where we call ShowPanel before ApplySchemeSettings, eg when
	// going from windowed <-> fullscreen
	if ( m_pImageList == NULL )
	{
		InvalidateLayout( true, true );
	}

	if ( !bShow )
	{
		m_nCloseKey = BUTTON_CODE_INVALID;
	}

	if ( BaseClass::IsVisible( ) == bShow )
		return;

	if ( bShow )
	{
		Reset( );
		Update( );
		SetVisible( true );
		MoveToFront( );
		SetMouseInputEnabled( true );
	}
	else
	{
		BaseClass::SetVisible( false );
		SetMouseInputEnabled( false );
		SetKeyBoardInputEnabled( false );
	}
}

void CClientScoreBoardDialog::FireGameEvent( IGameEvent *event )
{
	const char * type = event->GetName( );

	if ( Q_strcmp( type, "hltv_status" ) == 0 )
	{
		// spectators = clients - proxies
		m_HLTVSpectators = event->GetInt( "clients" );
		m_HLTVSpectators -= event->GetInt( "proxies" );
	}

	else if ( Q_strcmp( type, "server_spawn" ) == 0 )
	{
		// We'll post the message ourselves instead of using SetControlString()
		// so we don't try to translate the hostname.
		const char *hostname = event->GetString( "hostname" );
		Panel *control = FindChildByName( "ServerName" );
		if ( control )
		{
			PostMessage( control, new KeyValues( "SetText", "text", hostname ) );
			control->MoveToFront( );
		}
	}

	if ( IsVisible( ) )
		Update( );

}

bool CClientScoreBoardDialog::NeedsUpdate( void )
{
	return (m_fNextUpdateTime < gpGlobals->curtime);
}

//-----------------------------------------------------------------------------
// Purpose: Recalculate the internal scoreboard data
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::Update( void )
{
	// Set the title

	// Reset();
	m_pBlueList->DeleteAllItems( );
	m_pRedList->DeleteAllItems( );

	FillScoreBoard( );

	// grow the scoreboard to fit all the players
	int wide, tall;
	m_pBlueList->GetContentSize( wide, tall );
	m_pRedList->GetContentSize( wide, tall );
	tall += GetAdditionalHeight( );
	wide = GetWide( );

	int HalfPanelWidth = wide / 2;
	if ( m_iDesiredHeight < tall )
	{
		SetSize( wide, tall );
		m_pBlueList->SetSize( HalfPanelWidth - 12, tall - 34 );
		m_pRedList->SetSize( HalfPanelWidth - 12, tall - 34 );
	}
	else
	{
		SetSize( wide, m_iDesiredHeight );
		m_pBlueList->SetSize( HalfPanelWidth - 12, m_iDesiredHeight - 34 );
		m_pRedList->SetSize( HalfPanelWidth - 12, m_iDesiredHeight - 34 );
	}

	MoveToCenterOfScreen( );

	// update every second
	m_fNextUpdateTime = gpGlobals->curtime + 1.0f;
}

void CClientScoreBoardDialog::UpdatePlayerInfo( )
{
	m_iSectionId = 0; // 0'th row is a header
	int selectedRow = -1;

	for ( int teamindex = TEAM_START; teamindex <= TEAM_LAST; teamindex++ )
	{
		// walk all the players and make sure they're in the scoreboard
		for ( int i = 1; i <= gpGlobals->maxClients; ++i )
		{
			IGameResources *gr = GameResources( );

			if ( gr && gr->IsConnected( i ) )
			{
				// add the player to the list
				KeyValues *playerData = new KeyValues( "data" );
				GetPlayerScoreInfo( i, playerData );
				UpdatePlayerAvatar( i, playerData );

				const char *oldName = playerData->GetString( "name", "" );
				char newName[MAX_PLAYER_NAME_LENGTH];

				UTIL_MakeSafeName( oldName, newName, MAX_PLAYER_NAME_LENGTH );

				playerData->SetString( "name", newName );

				int itemID = FindItemIDForPlayerIndex( i );
				int sectionID = gr->GetTeam( i );

				if ( gr->IsLocalPlayer( i ) )
				{
					selectedRow = itemID;
				}
				if ( itemID == -1 )
				{
					// add a new row
					itemID = GetList( teamindex )->AddItem( sectionID, playerData );
				}
				else
				{
					// modify the current row
					GetList( teamindex )->ModifyItem( itemID, sectionID, playerData );
				}

				// set the row color based on the players team
				GetList( teamindex )->SetItemFgColor( itemID, gr->GetTeamColor( sectionID ) );

				playerData->deleteThis( );
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
		if ( selectedRow != -1 )
		{
			GetList( teamindex )->SetSelectedItem( selectedRow );
		}
	}
}

// Adds a list to the panel
void CClientScoreBoardDialog::AddList( vgui::SectionedListPanel *pList )
{
	// hmmm?
}

//-----------------------------------------------------------------------------
// Purpose: adds the top header of the scoreboards
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::AddHeader( vgui::SectionedListPanel *pList )
{
	// add the top header
	pList->AddSection( m_iSectionId, "" );
	pList->SetSectionAlwaysVisible( m_iSectionId );
	pList->AddColumnToSection( m_iSectionId, "avatar", "Avatar", 0, scheme( )->GetProportionalScaledValueEx( GetScheme( ), m_iAvatarWidth ) );
	pList->AddColumnToSection( m_iSectionId, "name", "#PlayerName", 0, scheme( )->GetProportionalScaledValueEx( GetScheme( ), m_iNameWidth ) );
	pList->AddColumnToSection( m_iSectionId, "frags", "#PlayerScore", 0, scheme( )->GetProportionalScaledValueEx( GetScheme( ), m_iScoreWidth ) );
	pList->AddColumnToSection( m_iSectionId, "ping", "#PlayerPing", 0, scheme( )->GetProportionalScaledValueEx( GetScheme( ), m_iPingWidth ) );
}

//-----------------------------------------------------------------------------
// Purpose: Adds a new section to the scoreboard (i.e the team header)
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::AddSection( vgui::SectionedListPanel *pList, int teamType, int teamNumber )
{
	if ( teamType == TYPE_TEAM )
	{
		IGameResources *gr = GameResources( );

		if ( !gr )
			return;

		// setup the team name
		wchar_t *teamName = g_pVGuiLocalize->Find( gr->GetTeamName( teamNumber ) );
		wchar_t name[64];
		wchar_t string1[1024];

		if ( !teamName )
		{
			g_pVGuiLocalize->ConvertANSIToUnicode( gr->GetTeamName( teamNumber ), name, sizeof(name) );
			teamName = name;
		}

		g_pVGuiLocalize->ConstructString( string1, sizeof(string1), g_pVGuiLocalize->Find( "#Player" ), 2, teamName );

		pList->AddSection( m_iSectionId, "", StaticPlayerSortFunc );

		// Avatars are always displayed at 32x32 regardless of resolution
		pList->AddColumnToSection( m_iSectionId, "avatar", "", SectionedListPanel::COLUMN_IMAGE, m_iAvatarWidth );
		pList->AddColumnToSection( m_iSectionId, "name", string1, 0, scheme( )->GetProportionalScaledValueEx( GetScheme( ), m_iNameWidth ) - m_iAvatarWidth );
		pList->AddColumnToSection( m_iSectionId, "frags", "", 0, scheme( )->GetProportionalScaledValueEx( GetScheme( ), m_iScoreWidth ) );
		pList->AddColumnToSection( m_iSectionId, "ping", "", 0, scheme( )->GetProportionalScaledValueEx( GetScheme( ), m_iPingWidth ) );
		// Add mute button here
	}
}

vgui::SectionedListPanel *CClientScoreBoardDialog::GetList( int TeamIndex )
{
	if ( TeamIndex == SDK_TEAM_BLUE )
	{
		return m_pBlueList;
	}
	else if ( TeamIndex == SDK_TEAM_RED )
	{
		return m_pRedList;
	}
	else if ( TeamIndex == TEAM_SPECTATOR )
	{
		AssertFatalMsg( 0, "Asking for Spectator Team List" );
		return NULL;
	}
	else
	{
		// Return Spectator
		AssertFatalMsg( 0, "NULL Request Team List" );
		return NULL;
	}
}

//-----------------------------------------------------------------------------
// Purpose: Used for sorting players
//-----------------------------------------------------------------------------
bool CClientScoreBoardDialog::StaticPlayerSortFunc( vgui::SectionedListPanel *list, int itemID1, int itemID2 )
{
	KeyValues *it1 = list->GetItemData( itemID1 );
	KeyValues *it2 = list->GetItemData( itemID2 );
	Assert( it1 && it2 );

	// first compare frags
	int v1 = it1->GetInt( "frags" );
	int v2 = it2->GetInt( "frags" );
	if ( v1 > v2 )
		return true;
	else if ( v1 < v2 )
		return false;

	// next compare their names
	if ( Q_strcmp( it1->GetString( "name" ), it2->GetString( "name" ) ) > 0 )
	{
		return false;
	}
	else
		return true;

	// the same, so compare itemID's (as a sentinel value to get deterministic sorts)
	return itemID1 < itemID2;
}

//-----------------------------------------------------------------------------
// Purpose: Adds a new row to the scoreboard, from the playerinfo structure
//-----------------------------------------------------------------------------
bool CClientScoreBoardDialog::GetPlayerScoreInfo( int playerIndex, KeyValues *kv )
{
	IGameResources *gr = GameResources( );

	if ( !gr )
		return false;

	kv->SetInt( "deaths", gr->GetDeaths( playerIndex ) );
	kv->SetInt( "frags", gr->GetFrags( playerIndex ) );
	kv->SetInt( "ping", gr->GetPing( playerIndex ) );
	kv->SetString( "name", gr->GetPlayerName( playerIndex ) );
	kv->SetInt( "playerIndex", playerIndex );

	return true;
}

void CClientScoreBoardDialog::UpdatePlayerAvatar( int playerIndex, KeyValues *kv )
{
	// Update their avatar
	if ( kv && ShowAvatars( ) && steamapicontext->SteamFriends( ) && steamapicontext->SteamUtils( ) )
	{
		player_info_t pi;
		if ( engine->GetPlayerInfo( playerIndex, &pi ) )
		{
			if ( pi.friendsID )
			{
				CSteamID steamIDForPlayer( pi.friendsID, 1, steamapicontext->SteamUtils( )->GetConnectedUniverse( ), k_EAccountTypeIndividual );

				// See if the avatar's changed
				int iAvatar = steamapicontext->SteamFriends( )->GetMediumFriendAvatar( steamIDForPlayer );
				if ( m_iImageAvatars[playerIndex] != iAvatar )
				{
					m_iImageAvatars[playerIndex] = iAvatar;

					// Now see if we already have that avatar in our list
					int iIndex = m_mapAvatarsToImageList.Find( iAvatar );
					if ( iIndex == m_mapAvatarsToImageList.InvalidIndex( ) )
					{
						CAvatarImage *pImage = new CAvatarImage( );
						pImage->SetAvatarSteamID( steamIDForPlayer );
						pImage->SetAvatarSize( 32, 32 );        // Deliberately non scaling
						int iImageIndex = m_pImageList->AddImage( pImage );

						m_mapAvatarsToImageList.Insert( iAvatar, iImageIndex );
					}
				}

				int iIndex = m_mapAvatarsToImageList.Find( iAvatar );

				if ( iIndex != m_mapAvatarsToImageList.InvalidIndex( ) )
				{
					kv->SetInt( "avatar", m_mapAvatarsToImageList[iIndex] );

					CAvatarImage *pAvIm = (CAvatarImage *)m_pImageList->GetImage( m_mapAvatarsToImageList[iIndex] );
					pAvIm->UpdateFriendStatus( );
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: reload the player list on the scoreboard
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::FillScoreBoard( )
{
	// update totals information
	UpdateTeamInfo( );

	// update player info
	UpdatePlayerInfo( );
}

//-----------------------------------------------------------------------------
// Purpose: searches for the player in the scoreboard
//-----------------------------------------------------------------------------
int CClientScoreBoardDialog::FindItemIDForPlayerIndex( int playerIndex )
{
	for ( int teamindex = TEAM_START; teamindex <= TEAM_LAST; teamindex++ )
	{
		for ( int i = 0; i <= GetList( teamindex )->GetHighestItemID( ); i++ )
		{
			if ( GetList( teamindex )->IsItemIDValid( i ) )
			{
				KeyValues *kv = GetList( teamindex )->GetItemData( i );
				kv = kv->FindKey( m_iPlayerIndexSymbol );
				if ( kv && kv->GetInt( ) == playerIndex )
					return i;
			}
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
// Purpose: Sets the text of a control by name
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::MoveLabelToFront( const char *textEntryName )
{
	Label *entry = dynamic_cast<Label *>(FindChildByName( textEntryName ));
	if ( entry )
	{
		entry->MoveToFront( );
	}
}

//-----------------------------------------------------------------------------
// Purpose: Center the dialog on the screen.  (vgui has this method on
//                        Frame, but we're an EditablePanel, need to roll our own.)
//-----------------------------------------------------------------------------
void CClientScoreBoardDialog::MoveToCenterOfScreen( )
{
	int wx, wy, ww, wt;
	surface( )->GetWorkspaceBounds( wx, wy, ww, wt );
	SetPos( (ww - GetWide( )) / 2, (wt - GetTall( )) / 2 );
}
