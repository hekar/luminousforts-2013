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

#ifndef GWEN_FRAME_H
#define GWEN_FRAME_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "Gwen.h"
#include "MGUI_Frame.h"

class CGwenFrame : public MFrame
{
private:
	DECLARE_CLASS_SIMPLE( CGwenFrame, MFrame );

public:
	CGwenFrame( vgui::VPANEL parent );
	~CGwenFrame();

	void PaintBackground();

	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	bool IsVisible() { return BaseClass::IsVisible(); }
	void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

	virtual void SetSize( int wide, int tall );
	virtual void OnSizeChanged( int newWide, int newTall );

	virtual void OnCursorMoved(int x, int y);
	virtual void OnMousePressed(vgui::MouseCode code);
	virtual void OnMouseReleased(vgui::MouseCode code);
	virtual void OnMouseWheeled(int delta);
	virtual void OnKeyCodeTyped(vgui::KeyCode code);

	virtual void SetTitle( const char *title, bool ignored );

protected:
	Gwen::Controls::Canvas *m_Canvas;
	Gwen::Controls::WindowControl *m_InnerPanel;
};

#endif // GWEN_FRAME_H
