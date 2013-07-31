//======== Copyright � 1996-2008, Valve Corporation, All rights reserved. =========//
//
// Purpose: 
//
// $NoKeywords: $
//=================================================================================//

#ifndef GUI_TEAMMENU_H
#define GUI_TEAMMENU_H

#include "teammenu.h"

class CModTeamMenu : public CTeamMenu
{
private:
	DECLARE_CLASS_SIMPLE( CModTeamMenu, CTeamMenu );
	// VGUI2 override
	void OnCommand( const char *command);
	// helper functions
	void SetVisibleButton(const char *textEntryName, bool state);

public:
	CModTeamMenu(IViewPort *pViewPort);
	virtual ~CModTeamMenu();

	void OnKeyCodePressed( vgui::KeyCode code );

	void Update();
	virtual void SetVisible(bool state);
	void MoveToCenterOfScreen();

protected:
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
	// background
	Color					 m_bgColor;
	Color					 m_borderColor;

};
#endif // GUI_CLASSMENU_H
