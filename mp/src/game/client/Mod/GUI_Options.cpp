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


#include "cbase.h"
#include <vgui_controls/PropertySheet.h>
#include "Panel_Options_Game.h"
#include "Panel_Options_FMOD.h"
#include "Panel_Options_Hud.h"
#include "Panel_Options_Title.h"
#include "Panel_Options_Misc.h"
#include "GUI_Options.h"

CREATE_GUI_INTERFACE(OptionsInterface, optionspanel);

// Just show the singleton panel
CON_COMMAND( smod_panel_options_toggle, NULL )
{
	optionspanel->Toggle();
}

// Show the panel
CON_COMMAND( smod_panel_options_show, NULL )
{
	optionspanel->Show();
}

// Hide the panel
CON_COMMAND( smod_panel_options_hide, NULL )
{
	optionspanel->Hide();
}

//
//	Name: CGUIOptions
//	Author: Hekar Khani
//	Description: 
//	Notes: 
//
CGUIOptions::CGUIOptions( vgui::VPANEL parent ) :
	BaseClass( NULL, "GUIOptions" )
{
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/sourcescheme.res", "sourcescheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/Options.res" );

	SetVisible( false );
	SetTitle( "#lf_options_title", true );
	SetSizeable( true );
	SetSmallCaption( false );
	SetMinimizeButtonVisible( true );
	SetMenuButtonResponsive( false );
	SetDeleteSelfOnClose( false );

	GetPropertySheet()->SetSmallTabs( false );
	GetPropertySheet()->SetDraggableTabs( false );

	AddPage( new CPanelOptionsGame( this ), "Game", false );
	AddPage( new CPanelOptionsFMOD( this ), "Music", false );
	AddPage( new CPanelOptionsTitle( this ), "Titlescreen", false );
	AddPage( new CPanelOptionsHud( this ), "Hud", false );
	AddPage( new CPanelOptionsMisc( this ), "Misc", false );

	VidInit();
}

CGUIOptions::~CGUIOptions()
{
}

void CGUIOptions::VidInit()
{
	SetSize( 400, 500 );
	MoveToCenterOfScreen();
}

void CGUIOptions::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetMouseInputEnabled( true );
		Activate();
		MoveToCenterOfScreen();
		RequestFocus();
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CGUIOptions::OnScreenSizeChanged( int oldwide, int oldtall )
{
	VidInit();
}

ProtectedMethod void CGUIOptions::OnCommand( const char *command )
{
	BaseClass::OnCommand( command );
}

ProtectedMethod void CGUIOptions::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor("Modfg", GetFgColor()) );
	SetBgColor( Color(0, 0, 0, 0) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );
}
