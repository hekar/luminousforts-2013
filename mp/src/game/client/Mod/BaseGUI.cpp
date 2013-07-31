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


// To clean up the way GUI panels are created
#include "cbase.h"
#include "BaseGUI.h"

#include <vgui/IVGui.h>
#include "ienginevgui.h"

// Initialize these panels
#include "GUI_CreditsWindow.h"
#include "GUI_WebWindow.h"
#include "GUI_MP3Panel.h"
#include "GUI_BinkVideoPlayer.h"
#include "GUI_Tutorial.h"
#include "GUI_Community.h"
#include "GUI_Titlescreen.h"
#include "GUI_MainMenu.h"
#include "GUI_SideMenu.h"
#include "GUI_Options.h"
#include "GUI_ListenServer.h"
#include "GUI_Editor.h"
#include "GUI_Message.h"
#include "GUI_ReportBug.h"
#include "Gwen.h"
#include "Gwen_Update.h"

CGUILoader *g_pGUIManager = NULL;

CGUILoader::CGUILoader()
{
	m_GameUI = enginevgui->GetPanel( PANEL_GAMEUIDLL );
}

CGUILoader::~CGUILoader()
{
}

void CGUILoader::InitPanels()
{
	g_pVGuiLocalize->AddFile( "resource/mod_%language%.txt", "MOD" );


	creditspanel->Create( m_GameUI ); 
	webwindowpanel->Create( m_GameUI );
//	mp3windowpanel->Create( m_GameUI );
//	binkwindowpanel->Create( m_GameUI );
	tutorialspanel->Create( m_GameUI );
	communitypanel->Create( m_GameUI );
	titlescreen->Create( m_GameUI );
	sidemenupanel->Create( m_GameUI );
	optionspanel->Create( m_GameUI );
//	listenserver->Create( m_GameUI );
//	guieditor->Create( m_GameUI );
	message->Create( m_GameUI );
	reportbug->Create( m_GameUI );

	// Main menu parents CBaseGamePanel
	const int BasePanelIndex = 1; // Bit of debugging to figure this out :/
	VPANEL BaseGamePanel = ipanel()->GetChild( m_GameUI, BasePanelIndex );
	
	mainmenupanel->Create( BaseGamePanel );

	// Bootstrap Gwen
#ifdef MOD_GWEN
	gwenbootstrap->Create( m_GameUI );

	gwenupdate->Create( m_GameUI );
#endif // MOD_GWEN
}

void CGUILoader::DestroyPanels()
{
	creditspanel->Destroy();
	webwindowpanel->Destroy();
//	mp3windowpanel->Destroy();
//	binkwindowpanel->Destroy();
	tutorialspanel->Destroy();
	communitypanel->Destroy();
	titlescreen->Destroy();
	sidemenupanel->Destroy();
	optionspanel->Destroy();
	mainmenupanel->Destroy();
//	listenserver->Destroy();
//	guieditor->Destroy();
	message->Destroy();
	reportbug->Destroy();
#ifdef MOD_GWEN
	gwenupdate->Destroy();

	gwenbootstrap->Destroy();
#endif // MOD_GWEN
}
