//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef CLIENTSCOREBOARDDIALOG_H
#define CLIENTSCOREBOARDDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/EditablePanel.h>
#include <game/client/iviewport.h>
#include "GameEventListener.h"

// This is dumb
#define TYPE_NOTEAM			0	// NOTEAM must be zero :)
#define TYPE_TEAM			1	// a section for a single team	
#define TYPE_PLAYERS		2
#define TYPE_SPECTATORS		3	// a section for a spectator group
#define TYPE_BLANK			4


//-----------------------------------------------------------------------------
// Purpose: Game ScoreBoard
//-----------------------------------------------------------------------------
class CClientScoreBoardDialog : public vgui::EditablePanel, public IViewPortPanel, public CGameEventListener
{
private:
	DECLARE_CLASS_SIMPLE( CClientScoreBoardDialog, vgui::EditablePanel );

public:
	CClientScoreBoardDialog( IViewPort *pViewPort );
	~CClientScoreBoardDialog( );

	virtual const char *GetName( void ) { return PANEL_SCOREBOARD; }
	virtual void SetData( KeyValues *data ) {};
	virtual void Reset( );
	virtual void Update( );
	virtual bool NeedsUpdate( void );
	virtual bool HasInputElements( void ) { return true; }
	virtual void ShowPanel( bool bShow );

	virtual bool ShowAvatars( ) { return IsPC( ); }

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel( ); }
	virtual bool IsVisible( ) { return BaseClass::IsVisible( ); }
	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

	// IGameEventListener interface:
	virtual void FireGameEvent( IGameEvent *event );

	virtual void UpdatePlayerAvatar( int playerIndex, KeyValues *kv );

protected:
	MESSAGE_FUNC_INT( OnPollHideCode, "PollHideCode", code );

	// functions to override
	virtual bool GetPlayerScoreInfo( int playerIndex, KeyValues *outPlayerInfo );
	virtual void InitScoreboardSections( );

	// Left blank for inheriter to override
	virtual void UpdateTeamInfo( ) {};
	virtual void UpdatePlayerInfo( );

	virtual void OnThink( );
	virtual void AddList( vgui::SectionedListPanel *pList );
	virtual void AddHeader( vgui::SectionedListPanel *pList ); // add the start header of the scoreboard
	virtual void AddSection( vgui::SectionedListPanel *pList, int teamType, int teamNumber ); // add a new section header for a team
	virtual int GetAdditionalHeight( ) { return 0; }

	virtual vgui::SectionedListPanel *GetList( int TeamIndex );

	// sorts players within a section
	static bool StaticPlayerSortFunc( vgui::SectionedListPanel *list, int itemID1, int itemID2 );

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

	virtual void PostApplySchemeSettings( vgui::IScheme *pScheme );

	// finds the player in the scoreboard
	int FindItemIDForPlayerIndex( int playerIndex );

	int m_iNumTeams;

	int	m_iSectionId; // the current section we are entering into

	vgui::SectionedListPanel *m_pBlueList;
	vgui::SectionedListPanel *m_pRedList;

	int s_VoiceImage[5];
	int TrackerImage;
	int	m_HLTVSpectators;
	float m_fNextUpdateTime;

	void MoveLabelToFront( const char *textEntryName );
	void MoveToCenterOfScreen( );

	vgui::ImageList				*m_pImageList;
	int							m_iImageAvatars[MAX_PLAYERS + 1];
	CUtlMap<int, int>			m_mapAvatarsToImageList;

	// Avatar proportional value
	CPanelAnimationVarAliasType( int, m_iAvatarWidth, "avatar_width", "42", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iNameWidth, "name_width", "96", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iClassWidth, "class_width", "50", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iScoreWidth, "score_width", "50", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iPingWidth, "ping_width", "30", "proportional_int" );
	CPanelAnimationVarAliasType( int, m_iVoiceWidth, "voice_width", "30", "proportional_int" );

private:
	int			m_iPlayerIndexSymbol;
	int			m_iDesiredHeight;
	IViewPort	*m_pViewPort;
	ButtonCode_t m_nCloseKey;


	// methods
	void FillScoreBoard( );
};


#endif // CLIENTSCOREBOARDDIALOG_H
