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


#ifndef GUI_EDITOR_H
#define GUI_EDITOR_H

#ifdef _WIN32
#pragma once
#endif

#if 0

#include "BaseGUI.h"
#include "MGUI_Frame.h"
#include "FileSystem.h"

namespace vgui
{
	class TextEntry;
}

class CGUIEditor : public MFrame
{
	DECLARE_CLASS_SIMPLE( CGUIEditor, MFrame );
public:
	CGUIEditor( vgui::VPANEL parent );
	~CGUIEditor();

	void ShowPanel( bool bShow );

	void SetVisible( bool state ){ BaseClass::SetVisible( state ); }

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	bool IsVisible() { return BaseClass::IsVisible(); }
	void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }


protected:
	void ApplySchemeSettings( vgui::IScheme *pScheme );
	void OnCommand( const char *command );
	bool LoadTextArea( const char *filepath );
	bool SaveTextArea( const char *filepath );

private:
	vgui::TextEntry *m_TextArea;
};


DEFINE_GUI_INTERFACE( CGUIEditor, GUIEditorInterface, guieditor );

#endif // 0

#endif // GUI_EDITOR_H
