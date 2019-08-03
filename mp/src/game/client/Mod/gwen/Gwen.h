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

#ifndef GWEN_START_H
#define GWEN_START_H

#include "Mod/BaseGUI.h"
#include <vgui_controls/Panel.h>
#include <Gwen/Skins/TexturedBase.h>
#include "Mod/gwen/VguiRenderer.h"
#include "Gwen/Controls/Base.h"
#include "Gwen/Controls/Canvas.h"
// Includes Label already
#include "Gwen/Controls/Button.h"
#include "Gwen/Controls/ProgressBar.h"
#include "Gwen/Controls/WindowControl.h"

class CGwenStart : public vgui::Panel
{
private:
	DECLARE_CLASS_SIMPLE( CGwenStart, vgui::Panel );

public:
	CGwenStart( vgui::VPANEL parent );
	~CGwenStart();

	// Do nothing...
	void ShowPanel( bool bShow ) {}

	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	bool IsVisible() { return BaseClass::IsVisible(); }
	void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }
};

namespace Gwen
{
	extern Gwen::Renderer::Base *GetGlobalRenderer();
	extern Gwen::Skin::Base *GetGlobalSkin();
};

DEFINE_GUI_INTERFACE( CGwenStart, CGwenStartFactory, gwenbootstrap );

#endif // GWEN_START_H
