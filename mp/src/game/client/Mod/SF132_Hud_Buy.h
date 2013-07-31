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


#ifndef SF132_HUD_BUY_H
#define SF132_HUD_BUY_H

#ifdef WIN32
#pragma once
#endif

#include <vgui_controls/Frame.h>

class CSF132HudBuy : public vgui::Frame, public IViewPortPanel
{
	DECLARE_CLASS_SIMPLE( CSF132HudBuy, vgui::Frame );

public:
	CSF132HudBuy( vgui::Panel *Parent );

	const char *GetName( void ) { return PANEL_BUYMENU; }
	void SetData(KeyValues *data) {}
	void Reset( void ) {}
	void Update( void ) {}
	bool NeedsUpdate( void ) { return true; }
	bool HasInputElements( void ) { return true; }

	bool ShouldDraw( void );
	void PaintBackground( void );
	void ShowPanel( bool bShow );
	void OnKeyCodePressed( vgui::KeyCode code );

	void HidePanel();

	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	bool IsVisible() { return BaseClass::IsVisible(); }
	void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

private:
	CPanelAnimationVarAliasType( float, m_flRectX, "RectX", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRectY, "RectY", "2", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRectWidth, "RectWidth", "96", "proportional_float" );
	CPanelAnimationVarAliasType( float, m_flRectHeight, "RectHeight", "26", "proportional_float" );

	int m_iToggleMenuKey;
};


#endif // SF132_HUD_BUY_H