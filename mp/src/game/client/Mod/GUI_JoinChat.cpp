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


/**
 * \file GUI_JoinChat.cpp
 * \author Hekar Khani
 */

#include "cbase.h"
#include "BaseGUI.h"
#include <vgui/ISystem.h>
#include <vgui/IVGui.h>
#include "ienginevgui.h"
#include <vgui_controls/MessageDialog.h>
#include "GUI_Message.h"

#define LF_CHAT_STEAM_SHELL "steam://friends/joinchat/103582791431524631"

CON_COMMAND( smod_panel_join_chat_show, NULL )
{
	vgui::system()->ShellExecute( "open", LF_CHAT_STEAM_SHELL );
	KeyValues *data = new KeyValues( "WebWindowData" );
	data->SetString( "title", "LuminousForts Steam Chat" );
	data->SetString( "message", "Please Open the Game Overlay" );

	message->GetFullPanel()->SetData( data );
	message->Show();

	data->deleteThis();
}
