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

#ifndef WEB_WINDOW_H
#define WEB_WINDOW_H

#ifdef _WIN32
#pragma once
#endif

#include "BaseGUI.h"

extern void OpenWebWindow( const char *title, const char *url );
extern void OpenLocalPage( const char *title, const char *path );

using namespace vgui;
class CWebWindow : public vgui::Frame
{
private:
	DECLARE_CLASS_SIMPLE( CWebWindow, vgui::Frame );

public:
	CWebWindow( vgui::VPANEL parent );
	virtual ~CWebWindow();

	virtual vgui::Panel *CreateControlByName( const char *controlName );

	virtual void SetData( KeyValues *data );
	virtual void ShowPanel( bool bShow );
	virtual void SetVisible( bool state ) { BaseClass::SetVisible( state ); }

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	virtual bool IsVisible() { return BaseClass::IsVisible(); }
	virtual void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

	// When the html window starts loading a URL
	MESSAGE_FUNC_CHARPTR_CHARPTR( OnStartLoading, "StartLoading", url, target );

	// When the html window has finished loading
	MESSAGE_FUNC_CHARPTR( OnFinishedLoading, "FinishedLoading", url );

protected:
	ProtectedMethod virtual void Refresh();
	ProtectedMethod virtual void Stop();

	// VGUI2 overrides
	ProtectedMethod virtual void OnCommand( const char *command );
	ProtectedMethod virtual void ApplySettings( KeyValues *inResourceData );
	ProtectedMethod virtual void ApplySchemeSettings(vgui::IScheme *pScheme);

protected:
	vgui::HTML *m_pHTMLMessage;

	vgui::TextEntry *m_LocationBar;

	char m_czTitle [256];
	char m_czURL [256];
};

DEFINE_GUI_INTERFACE(CWebWindow, WebWindowInterface, webwindowpanel);

#endif // WEB_WINDOW_H
