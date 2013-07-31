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


#ifndef GUI_BINKVIDEOPLAYER_H
#define GUI_BINKVIDEOPLAYER_H

#ifdef _WIN32
#pragma once
#endif

#ifdef MOD_BINK

#include "BaseGUI.h"
#include "MGUI_Frame.h"
#include "vgui_video.h"


using namespace vgui;

enum PlayState_enum
{
	PLAYER_PLAYING = 0,
	PLAYER_PAUSED,
	PLAYER_STOPPED
};

typedef VideoPanel CBinkVideoPanel;

class CBinkWindow : public MFrame
{
private:
	DECLARE_CLASS_SIMPLE( CBinkWindow, MFrame );

public:
	CBinkWindow (vgui::VPANEL parent);
	~CBinkWindow ();

	void ShowPanel( bool bShow );
	void SetVisible( bool state ) { BaseClass::SetVisible( state ); }

	void Play( const char *filename );
	void Stop();

	void OnClose();

	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	bool IsVisible() { return BaseClass::IsVisible(); }
	void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

protected:
	ProtectedMethod void OnCommand( const char *command );

private:
	CBinkVideoPanel *m_pVideoPanel;
	bool m_bPlaying;
};

DEFINE_GUI_INTERFACE(CBinkWindow,BinkInterface,binkwindowpanel)

#endif // MOD_BINK

#endif // GUI_BINKVIDEOPLAYER_H
