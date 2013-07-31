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


#ifndef GUI_BLOCKSPAWN_H
#define GUI_BLOCKSPAWN_H

#ifdef _WIN32
#pragma once
#endif

#include "MGUI_Frame.h"
#include <game/client/iviewport.h>
#include <vgui_controls/Button.h>

#define PANEL_BLOCK "BlockSpawn"

class CBlockSpawn : public MFrame, public IViewPortPanel
{
	DECLARE_CLASS_SIMPLE( CBlockSpawn, MFrame );
public:
	CBlockSpawn( vgui::Panel *Parent, IViewPort *pViewPort );
	~CBlockSpawn();

	const char *GetName( void ) { return PANEL_BLOCK; }
	void SetData(KeyValues *data) {};
	void Reset() {};
	void Update();
	bool NeedsUpdate( void ) { return false; }
	bool HasInputElements( void ) { return false; }
	void ShowPanel( bool bShow );

	void OnKeyCodePressed( vgui::KeyCode code );
	void OnCommand( const char *command );

	void SetupButtonDisplay();

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
	bool IsVisible() { return BaseClass::IsVisible(); }
	void SetParent(vgui::VPANEL parent) { BaseClass::SetParent(parent); }

private:
	IViewPort *m_pViewPort;
	vgui::Button *m_pButtons[ BLOCK_COUNT ];

	int m_iBlockStart;
	int m_iBlockCount;
	int m_iToggleMenuKey;
};

#endif // GUI_BLOCKSPAWN_H
