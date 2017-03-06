//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef CHL2MPCLIENTSCOREBOARDDIALOG_H
#define CHL2MPCLIENTSCOREBOARDDIALOG_H
#ifdef _WIN32
#pragma once
#endif

#include <clientscoreboarddialog.h>

//-----------------------------------------------------------------------------
// Purpose: Game ScoreBoard
//-----------------------------------------------------------------------------
class CHL2MPClientScoreBoardDialog : public CClientScoreBoardDialog
{
private:
	DECLARE_CLASS_SIMPLE( CHL2MPClientScoreBoardDialog, CClientScoreBoardDialog );

public:
	CHL2MPClientScoreBoardDialog( IViewPort *pViewPort );
	~CHL2MPClientScoreBoardDialog( );

protected:
	// scoreboard overrides
	virtual void InitScoreboardSections( );
	virtual void UpdateTeamInfo( );
	virtual void UpdatePlayerInfo( );

	// vgui overrides for rounded corner background
	virtual void PaintBackground( );
	virtual void PaintBorder( );
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
	virtual void AddHeader( vgui::SectionedListPanel *pList ); // add the start header of the scoreboard
	virtual void AddSection( vgui::SectionedListPanel *pList, int teamType, int teamNumber ); // add a new section header for a team

	int GetSectionFromTeamNumber( int teamNumber );

	vgui::Label *m_pSpecLabel;

	Color m_bgColor;
	Color m_borderColor;
};


#endif // CHL2MPCLIENTSCOREBOARDDIALOG_H
