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


#ifndef PANEL_LISTEN_MAIN_H
#define PANEL_LISTEN_MAIN_H

#if 0

#include <vgui_controls/PropertyPage.h>
#include <GameUI/IGameUI.h>

namespace vgui
{
	class OldSectionedList;
};

struct mapname_t
{
	char mapname[ MAX_PATH + 1 ];
};

class CPanelListenMain : public vgui::PropertyPage
{
private:
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// MAKE SURE THIS IS THE FIRST ELEMENT IN THE CLASS
	IGameUI *m_pGameUI;
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	DECLARE_CLASS_SIMPLE( CPanelListenMain, vgui::PropertyPage );
public:
	CPanelListenMain( vgui::Panel *Parent );
	~CPanelListenMain();

	vgui::Panel *CreateControlByName( const char *controlName );

	void InitMapList();

	MESSAGE_FUNC( OnResetData, "ResetData" );
	MESSAGE_FUNC( OnApplyChanges, "ApplyChanges" );

	MESSAGE_FUNC( OnPageShow, "PageShow" );
	MESSAGE_FUNC( OnPageHide, "PageHide" );

protected:
	void OnCommand( const char *command );
	void ApplySchemeSettings( vgui::IScheme *pScheme );

private:
	void DisplayMapsWithPrefix( const char *prefix );
	void DisplayMaps(); // Displays all maps in the map list
	void ClearMapList();
	void StartServer( const char *mapname );
	
	vgui::OldSectionedList *m_MapList;
};


#endif // 0

#endif // PANEL_LISTEN_MAIN_H
