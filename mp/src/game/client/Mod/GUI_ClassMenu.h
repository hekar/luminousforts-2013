//======== Copyright � 1996-2008, Valve Corporation, All rights reserved. =========//
//
// Purpose: 
//
// $NoKeywords: $
//=================================================================================//

#ifndef SDK_CLASSMENU_H
#define SDK_CLASSMENU_H

#include <classmenu.h>
#include <vgui_controls/EditablePanel.h>
#include <filesystem.h>
#include "mouseoverpanelbutton.h"

class CModClassInfoPanel : public vgui::EditablePanel
{
private:
	DECLARE_CLASS_SIMPLE( CModClassInfoPanel, vgui::EditablePanel );

public:
	CModClassInfoPanel( vgui::Panel *parent, const char *panelName ) : vgui::EditablePanel( parent, panelName )
	{
	}

	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );
	virtual vgui::Panel *CreateControlByName( const char *controlName );
};

class CModClassMenu : public CClassMenu
{
private:
	DECLARE_CLASS_SIMPLE( CModClassMenu, CClassMenu );

public:
	CModClassMenu(IViewPort *pViewPort);
	CModClassMenu(IViewPort *pViewPort, const char *panelName, const char *schemename = NULL);
	virtual ~CModClassMenu();

	virtual void LevelInit();

	virtual void Update( void );
	void MoveToCenterOfScreen();
	virtual Panel *CreateControlByName( const char *controlName );
	virtual void OnTick( void );
	virtual void OnKeyCodePressed(vgui::KeyCode code);
	virtual void SetVisible( bool state );
	virtual void ShowPanel(bool bShow);

	virtual bool NeedsUpdate() { return m_bNeedsUpdate; }

	MESSAGE_FUNC_CHARPTR( OnShowPage, "ShowPage", page );

	virtual int GetTeamNumber( void ) = 0;


private:
	CModClassInfoPanel *m_pClassInfoPanel;
	MouseOverButton<CModClassInfoPanel> *m_pInitialButton;

	bool m_bNeedsUpdate;

	int m_iActivePlayerClass;
	int m_iLastPlayerClassCount;
	int	m_iLastClassLimit;

	ButtonCode_t m_iClassMenuKey;

protected:
	virtual void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
	// rounded corners
	Color					 m_bgColor;
	Color					 m_borderColor;
};

class CModClassMenu_NoTeams : public CModClassMenu
{
private:
	DECLARE_CLASS_SIMPLE( CModClassMenu_NoTeams, CModClassMenu );

public:
	CModClassMenu_NoTeams(IViewPort *pViewPort) :
		BaseClass(pViewPort, PANEL_CLASS, "GameScheme")
	{
		LoadControlSettings( "resource/ui/ClassMenu_NoTeams.res" );
	}

	virtual const char *GetName( void )
	{ 
		return PANEL_CLASS;
	}
	virtual int GetTeamNumber( void )
	{
		return TEAM_UNASSIGNED;
	}
};

class CModClassMenu_Blue : public CModClassMenu
{
private:
	DECLARE_CLASS_SIMPLE( CModClassMenu_Blue, CModClassMenu );

public:
	CModClassMenu_Blue(IViewPort *pViewPort) :
		BaseClass(pViewPort, PANEL_CLASS_BLUE, "GameScheme_Blue")
	{
	}

	virtual const char *GetName( void )
	{ 
		return PANEL_CLASS_BLUE; 
	}
	virtual int GetTeamNumber( void )
	{
		return SDK_TEAM_BLUE;
	}
};
class CModClassMenu_Red : public CModClassMenu
{
private:
	DECLARE_CLASS_SIMPLE( CModClassMenu_Red, CModClassMenu );

public:
	CModClassMenu_Red(IViewPort *pViewPort) :
		BaseClass(pViewPort, PANEL_CLASS_RED, "GameScheme_Red")
	{
	}

	virtual const char *GetName( void )
	{ 
		return PANEL_CLASS_RED; 
	}
	virtual int GetTeamNumber( void )
	{
		return SDK_TEAM_RED;
	}
};

#endif //SDK_CLASSMENU_H
