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


#ifndef GUI_REPORTBUG_H
#define GUI_REPORTBUG_H

#ifdef _WIN32
#pragma once
#endif

#include "BaseGUI.h"
#include <vgui_controls/PropertyDialog.h>

struct BugReport
{
	char author[ 256 ];
	char title[ 256 ];
	char mapName[ 512 ];
	char version[ 32 ];
	char description[ 9600 ];
	Vector origin;
};

class CGUIReportBug : public MFrame
{
private:
	DECLARE_CLASS_SIMPLE( CGUIReportBug, MFrame );

public:
	CGUIReportBug( vgui::VPANEL parent );
	~CGUIReportBug();

	void ShowPanel( bool bShow );
	
	bool ReportBug( const BugReport& report);
	void Cancel();

	void SetVisible( bool state ){ BaseClass::SetVisible( state ); }

	// both vgui::Frame and IViewPortPanel define these, so explicitly define them here as passthroughs to vgui
	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	bool IsVisible() { return BaseClass::IsVisible(); }
	void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

protected:
	void OnCommand( const char *command );
	void ApplySchemeSettings( vgui::IScheme *pScheme );

	vgui::TextEntry *m_Title;
	vgui::TextEntry *m_Description;
};

DEFINE_GUI_INTERFACE( CGUIReportBug, ReportBugInterface, reportbug );

#endif // GUI_REPORTBUG_H
