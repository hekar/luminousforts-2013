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

	Dock for extra titlescreen options

	Last Updated -- Unknown --
===============================================================*/

#include "cbase.h"
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>
#include "GUI_MP3Panel.h"
#include "GUI_Options.h"

#include "GUI_SideMenu.h"

CREATE_GUI_INTERFACE(GUISideMenuInterface, sidemenupanel);

CGUISideMenu::CGUISideMenu( VPANEL parent ) :
	BaseClass( NULL, "GUISideMenu" )
{
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/sourcescheme.res", "sourcescheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/SideMenu.res" );
}

CGUISideMenu::~CGUISideMenu()
{
}

void CGUISideMenu::InitDefaults( vgui::IScheme *pScheme )
{
	int sw, sh;
	surface()->GetScreenSize(sw, sh);

	int old_w, old_h;
	GetSize( old_w, old_h );

	int newheight = sh / 3.5;
	SetSize( old_w, clamp( newheight, 200, 700 ) );
	SetPos( 0, (sh - GetTall()) / 2 );

	SetAlpha( 168 );

	if ( pScheme )
	{
		const char *resource = pScheme->GetResourceString("visible");
		if ( Q_strcmp( resource, "0" ) )
		{
			MakePopup();
		}
	}
}

void CGUISideMenu::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetVisible( true );
		InitDefaults( NULL );
		SetMouseInputEnabled( true );
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CGUISideMenu::OnScreenSizeChanged( int oldwide, int oldtall )
{
	InitDefaults( NULL );
}

ProtectedMethod void CGUISideMenu::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor("Modfg", GetFgColor()) );
	SetBgColor( Color(0, 0, 0, 128) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );

	InitDefaults( pScheme );
}
