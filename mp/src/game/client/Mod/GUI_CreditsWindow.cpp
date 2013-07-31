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


/*===============================================================
	Client

	Simple panel for showing credits.
	Again this is a	singleton.

	Last Updated Sept 05, 2009
===============================================================*/

#include "cbase.h"
#include "Mod/GUI_CreditsWindow.h"
#include "Mod/GUI_WebWindow.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define SF_CREDITS_PAGE	"documentation/full_credits.html"

CREATE_GUI_INTERFACE( CreditInterface, creditspanel )

CON_COMMAND( smod_panel_credits_show, NULL )
{
	creditspanel->Toggle();
}

CON_COMMAND( smod_panel_credits_hide, NULL )
{
	creditspanel->Hide();
}

//
//	Name: CCreditPanel
//	Author: Hekar Khani
//	Description: Pretty straight forward panel for just holding credits
//	Notes: 
//
CCreditPanel::CCreditPanel (vgui::VPANEL parent) :
	BaseClass( NULL, "credits" )
{	
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/sourcescheme.res", "sourcescheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/CreditsPanel.res" );

	SetVisible( false );
	SetSizeable( false );

	SetMaximizeButtonVisible( false );
	SetMinimizeButtonVisible( false );
}

CCreditPanel::~CCreditPanel ()
{
}

ProtectedMethod void CCreditPanel::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetMouseInputEnabled( true );
		Activate();
		RequestFocus();
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CCreditPanel::OnCommand( const char *command )
{
	if ( !Q_strcmp( command, "displayall" ) )
	{
		Close();
		OpenLocalPage( "#lf_credits_title", SF_CREDITS_PAGE );
	}

	BaseClass::OnCommand( command );
}

void CCreditPanel::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor("Modfg", GetFgColor()) );
	SetBgColor( Color(0, 0, 0, 0) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );
}
