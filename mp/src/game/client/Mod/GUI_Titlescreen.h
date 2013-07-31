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


#ifndef GUI_TITLESCREEN_H
#define GUI_TITLESCREEN_H

#ifdef _WIN32
#pragma once
#endif

#include "BaseGUI.h"
#include <vgui_controls/Label.h>
#include <vgui_controls/EditablePanel.h>
#include <vgui_controls/ImagePanel.h>
#include "vgui_video.h"

#ifdef MOD_BACKGROUND_VIDEO
	#include "GUI_BinkVideoPlayer.h"
#endif

#ifdef MOD_TITLESCREEN_THEMES

#endif

using namespace vgui;

class CBackgroundImage : public vgui::ImagePanel
{
private:
	DECLARE_CLASS_SIMPLE( CBackgroundImage, vgui::ImagePanel );

public:
	CBackgroundImage( Panel *Parent, vgui::VPANEL parent );
	~CBackgroundImage();

public:
	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	bool IsVisible() { return BaseClass::IsVisible(); }
	void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }
};


class CTitleScreen : public vgui::EditablePanel
{
private:
	DECLARE_CLASS_SIMPLE( CTitleScreen, vgui::EditablePanel );

public:
	CTitleScreen( vgui::VPANEL parent );
	~CTitleScreen();

	void InitSettings();

	void ShowPanel( bool bShow );
	void SetVisible( bool state ) { BaseClass::SetVisible( state ); }

	void OnClose();

	void OnScreenSizeChanged( int oldwide, int oldtall );
	
	void OnThink( void );

	void DrawHover( int x0, int y0, int x1, int y1, const char *toolTip );
	void HideHover();

	void Paint();

public:
	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	bool IsVisible() { return BaseClass::IsVisible(); }
	void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

private:
	Label *m_pTitle;
	CBackgroundImage *m_pBackground;
	HFont m_hoverFont;
	bool m_drawHover;
	int m_hoverX;
	int m_hoverY;
	int m_hoverW;
	int m_hoverH;
	wchar_t m_hoverToolTip[512];

#ifdef MOD_BACKGROUND_VIDEO
	CBinkVideoPanel *m_pLoopedBackground;
#endif

#ifdef MOD_TITLESCREEN_THEMES
	
#endif

#if defined( MOD_TITLESCREEN_THEMES ) || defined( MOD_TITLESCREEN_VIDEO )
	char m_VideoFileName [MAX_PATH + 1];
#endif

protected:
	// VGUI2 overrides
	void ApplySchemeSettings(vgui::IScheme *pScheme) {}

	// Do not paint a background or border
	void PaintBackground() {}
	void PaintBorder() {}
};

DEFINE_GUI_INTERFACE( CTitleScreen, TitleScreenInterface, titlescreen )

#endif // GUI_TITLESCREEN_H