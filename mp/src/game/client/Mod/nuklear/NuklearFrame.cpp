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
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include "NuklearFrame.h"

CNuklearFrame::CNuklearFrame( vgui::VPANEL parent ) :
	MFrame( NULL, "NuklearFrame" )
{
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/SourceScheme.res", "SourceScheme" );
	SetScheme( scheme );

	SetCloseButtonVisible( false );
	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );
	SetMenuButtonVisible( false );
	SetTitleBarVisible( false );

	nk_init_default(&m_ctx, NULL);
}

CNuklearFrame::~CNuklearFrame()
{
	nk_end(&m_ctx);
}

void CNuklearFrame::PaintBackground()
{
	BaseClass::PaintBackground();
	int w;
	int h;
	GetSize( w, h );
	if (nk_begin(&m_ctx, "Show", nk_rect(5, 25, w, h),
		NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE))
	{
		nk_clear(&m_ctx);
		// fixed widget pixel width
		nk_layout_row_static(&m_ctx, 30, 80, 1);
		if (nk_button_label(&m_ctx, "button"))
		{
			// event handling
		}
		// fixed widget window ratio width
		nk_layout_row_dynamic(&m_ctx, 30, 2);
		nk_option_label(&m_ctx, "easy", true);
		nk_option_label(&m_ctx, "hard", true);
		// custom widget pixel width
		nk_layout_row_begin(&m_ctx, NK_STATIC, 30, 2);
		{
			nk_layout_row_push(&m_ctx, 50);
			nk_label(&m_ctx, "Volume:", NK_TEXT_LEFT);
			nk_layout_row_push(&m_ctx, 110);
		}
		nk_layout_row_end(&m_ctx);
	}
}

void CNuklearFrame::SetSize( int wide, int tall )
{
	BaseClass::SetSize( wide, tall );
}

void CNuklearFrame::OnSizeChanged( int newWide, int newTall )
{
	BaseClass::OnSizeChanged( newWide, newTall );
}

void CNuklearFrame::OnCursorMoved(int x, int y)
{
	m_iX = x;
	m_iY = y;
	nk_input_begin(&m_ctx);
	nk_input_motion(&m_ctx, x, y);
	nk_input_end(&m_ctx);
}

void CNuklearFrame::OnMousePressed(vgui::MouseCode code)
{
	nk_buttons btn;
	if (code == MOUSE_LEFT)
	{
		btn = NK_BUTTON_LEFT;
	}
	else if (code == MOUSE_RIGHT)
	{
		btn = NK_BUTTON_RIGHT;
	}
	else if (code == MOUSE_MIDDLE)
	{
		btn = NK_BUTTON_MIDDLE;
	}

	nk_input_begin(&m_ctx);
	nk_input_button(&m_ctx, btn, m_iX, m_iY, 1);
	nk_input_end(&m_ctx);
}

void CNuklearFrame::OnMouseReleased(vgui::MouseCode code)
{
	nk_buttons btn;
	if (code == MOUSE_LEFT)
	{
		btn = NK_BUTTON_LEFT;
	}
	else if (code == MOUSE_RIGHT)
	{
		btn = NK_BUTTON_RIGHT;
	}
	else if (code == MOUSE_MIDDLE)
	{
		btn = NK_BUTTON_MIDDLE;
	}

	nk_input_begin(&m_ctx);
	nk_input_button(&m_ctx, btn, m_iX, m_iY, 0);
	nk_input_end(&m_ctx);
}

void CNuklearFrame::OnMouseWheeled(int delta)
{
	struct nk_vec2 vec = nk_vec2(0, delta);
	nk_input_begin(&m_ctx);
	nk_input_scroll(&m_ctx, vec);
	nk_input_end(&m_ctx);
}

void CNuklearFrame::OnKeyCodeTyped(vgui::KeyCode code)
{
	nk_input_begin(&m_ctx);
	nk_input_char(&m_ctx, code);
	nk_input_end(&m_ctx);
}

void CNuklearFrame::SetTitle( const char *title, bool ignored )
{
}
