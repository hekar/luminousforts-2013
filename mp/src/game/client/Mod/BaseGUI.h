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


#ifndef BASE_GUI_H
#define BASE_GUI_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui_controls/Panel.h>

template <typename T>
class CGameGUI
{
public:
	CGameGUI()
	{
		m_Panel = NULL;
	}

	virtual void Create( vgui::VPANEL parent )
	{
		m_Panel = new T( parent );
	}

	virtual void Destroy()
	{
		if( m_Panel )
		{
			m_Panel->SetParent( NULL );
			delete m_Panel;
		}
	}

	virtual vgui::Panel *GetPanel()
	{
		return m_Panel;
	}

	virtual T *GetFullPanel()
	{
		return m_Panel;
	}

	virtual void Toggle()
	{
		m_Panel->ShowPanel( !m_Panel->IsVisible() );
	}

	virtual void Show()
	{
		m_Panel->ShowPanel( true );
	}

	virtual void Hide()
	{
		m_Panel->ShowPanel( false );
	}

private:
	T *m_Panel;
};

// Define in a header
#define DEFINE_GUI_INTERFACE(type,name,variable) \
	typedef CGameGUI< type > C##name; \
	extern C##name * variable;

#define CREATE_GUI_INTERFACE(name, variable) \
	static C##name g_C##name; \
	C##name * variable = &g_C##name; \
	CON_COMMAND( smod_panel_show_##variable, NULL ) \
	{ \
		(variable)->Show(); \
	} \
	CON_COMMAND( smod_panel_hide_##variable, NULL ) \
	{ \
		(variable)->Hide(); \
	}


class CGUILoader
{
public:
	CGUILoader();
	~CGUILoader();

	void InitPanels();
	void DestroyPanels();

private:
	vgui::VPANEL m_GameUI; 
};

extern CGUILoader *g_pGUIManager;

#endif // BASE_GUI_H
