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
#ifdef MOD_GWEN
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include "GwenFrame.h"

CGwenFrame::CGwenFrame( vgui::VPANEL parent ) :
	MFrame( NULL, "GwenFrame" )
{
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/SourceScheme.res", "SourceScheme" );
	SetScheme( scheme );

	SetCloseButtonVisible( false );
	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );
	SetMenuButtonVisible( false );
	SetTitleBarVisible( false );

	m_Canvas = new Gwen::Controls::Canvas( Gwen::GetGlobalSkin() );

	m_InnerPanel = new Gwen::Controls::WindowControl( m_Canvas );
	//m_InnerPanel->Dock( Gwen::Pos::Fill );
	m_InnerPanel->SetClosable( false );
}

CGwenFrame::~CGwenFrame()
{
}

void CGwenFrame::PaintBackground()
{
	BaseClass::PaintBackground();
	m_Canvas->RenderCanvas();
}

void CGwenFrame::SetSize( int wide, int tall )
{
	m_Canvas->SetSize( wide, tall );
	m_InnerPanel->SetSize( wide, tall );
	BaseClass::SetSize( wide, tall );
}

void CGwenFrame::OnSizeChanged( int newWide, int newTall )
{
	m_Canvas->SetSize( newWide, newTall );
	m_InnerPanel->SetSize( newWide, newTall );
	BaseClass::OnSizeChanged( newWide, newTall );
}

void CGwenFrame::OnCursorMoved(int x, int y)
{
	m_Canvas->InputMouseMoved( x, y, 0, 0 );
}

void CGwenFrame::OnMousePressed(vgui::MouseCode code)
{
	m_Canvas->InputMouseButton( 0, true );
}

void CGwenFrame::OnMouseReleased(vgui::MouseCode code)
{
	m_Canvas->InputMouseButton( 0, false );
}

void CGwenFrame::OnMouseWheeled(int delta)
{
	m_Canvas->InputMouseWheel( delta );
}

void CGwenFrame::OnKeyCodeTyped(vgui::KeyCode code)
{
	// TODO:
}

void CGwenFrame::SetTitle( const char *title, bool ignored )
{
	m_InnerPanel->SetTitle( Gwen::String( title ) );
}

#endif // MOD_GWEN
