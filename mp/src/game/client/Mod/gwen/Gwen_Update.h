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

#ifndef GWEN_UPDATE_H
#define GWEN_UPDATE_H

#ifdef _WIN32
#pragma once
#endif // _WIN32

#include "GwenFrame.h"

class CGwenUpdate : public CGwenFrame, public Gwen::Event::Handler
{
private:
	DECLARE_CLASS_SIMPLE( CGwenUpdate, CGwenFrame );

public:
	CGwenUpdate( vgui::VPANEL parent );
	~CGwenUpdate();

	void Paint();

	void OnCancel( Gwen::Controls::Base* control );

private:
	Gwen::Controls::Label *m_Desc;
	Gwen::Controls::Button *m_Button;
	Gwen::Controls::ProgressBar *m_Progress;
};

DEFINE_GUI_INTERFACE( CGwenUpdate, CGwenUpdateFactory, gwenupdate );

#endif // GWEN_UPDATE_H
