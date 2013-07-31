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

#ifndef HUD_CAPPOS_H
#define HUD_CAPPOS_H

#ifdef WIN32
#pragma once 
#endif // WIN32

#include "Hud_Base.h"
#include <vgui_controls/Panel.h>

class C_DomCaptureZone;

class CHudCapPos : public CModHudElement, public vgui::Panel
{
	DECLARE_CLASS_SIMPLE( CHudCapPos, vgui::Panel );
public:
	CHudCapPos( const char *pElementName );

	void Init( void );
	void LevelInit( void );
	void Reset( void );
	bool ShouldDraw( void );
	void Paint( void );

	bool AddCaptureZone( C_DomCaptureZone *capzone );

	void PaintBackground( void ) {}

private:
	CUtlVector< C_DomCaptureZone * > m_CPs;
	CHudTexture m_Texture;
};

#endif // HUD_CAPPOS_H