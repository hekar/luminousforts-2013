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
#include "MGUI_Frame.h"

// Delegate the work to the child class
MFrame::MFrame(Panel *parent, const char *panelName, bool showTaskbarIcon ) :
	BaseClass( parent, panelName, showTaskbarIcon )
{
}

MFrame::~MFrame()
{
}

vgui::Panel *MFrame::CreateControlByName( const char *controlName )
{
	vgui::Panel *ModPanel = CreateModControlByName( this, controlName );
	if ( ModPanel )
	{
		return ModPanel;
	}
	else
	{
		return BaseClass::CreateControlByName( controlName );
	}
}

bool MFrame::IsMinimized()
{
	return m_bMinimized;
}

void MFrame::OnMinimize()
{
	// surface()->SetMinimized(GetVPanel(), true);

	m_bMinimized = !m_bMinimized;

	if ( m_bMinimized )
	{
		int prevW, prevH;
		GetSize( prevW, prevH );
		m_iPrevSizeX = prevW;
		m_iPrevSizeY = prevH;

		SetSize( 180, 20 );
	}
	else
	{
		SetSize( m_iPrevSizeX, m_iPrevSizeY );
	}
}

void MFrame::OnMaximize()
{
	if ( m_bMinimized )
	{
		m_bMaximized = false;
	}
	else
	{
		m_bMaximized = !m_bMaximized;
	}

	m_bMinimized = false;

	if ( m_bMaximized )
	{
		int prevW, prevH;
		GetSize( prevW, prevH );
		m_iPrevSizeX = prevW;
		m_iPrevSizeY = prevH;

		int prevX, prevY;
		GetPos( prevX, prevY );
		m_iPrevPosX = prevX;
		m_iPrevPosY = prevY;

		int sw, sh;
		vgui::surface()->GetScreenSize( sw, sh );
		SetPos( 0, 0 );
		SetSize( sw, sh );
	}
	else
	{
		SetPos( m_iPrevPosX, m_iPrevPosY );
		SetSize( m_iPrevSizeX, m_iPrevSizeY );
	}
}

void MFrame::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
	{
		return;
	}

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
