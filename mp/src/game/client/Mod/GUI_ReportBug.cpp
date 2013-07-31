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

#include "cbase.h"
#include <vgui/ISurface.h>
#include <vgui_controls/TextEntry.h>
#include "CURL.h"
#include "BaseGUI.h"
#include "MGUI_Frame.h"
#include "GUI_ReportBug.h"


CREATE_GUI_INTERFACE( ReportBugInterface, reportbug );

// Just show the singleton panel
CON_COMMAND( smod_panel_reportbug_show, NULL )
{
	reportbug->Show();
}

// Hide the panel
CON_COMMAND( smod_panel_reportbug_hide, NULL )
{
	reportbug->Hide();
}

CGUIReportBug::CGUIReportBug( vgui::VPANEL parent ) :
	BaseClass( NULL, "GUIReportBug" )
{	
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/sourcescheme.res", "sourcescheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/ReportBug.res" );

	SetVisible( false );
	
	m_Title = dynamic_cast< vgui::TextEntry * > ( FindChildByName( "ReportTitle", true ) );

	m_Description = dynamic_cast< vgui::TextEntry * > ( FindChildByName( "Description", true ) );
	if ( m_Description )
	{
		m_Description->SetMultiline( true );
		m_Description->SetCatchEnterKey( true );
		m_Description->SetVerticalScrollbar( true );
		m_Description->SetHorizontalScrolling( true );
		m_Description->SetMultiline( true );
		m_Description->SetEditable( true );
	}

	SetMenuButtonResponsive( true );
	SetDeleteSelfOnClose( false );
}

CGUIReportBug::~CGUIReportBug ()
{
}

void CGUIReportBug::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetMouseInputEnabled( true );
		Activate();
		RequestFocus();
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

bool CGUIReportBug::ReportBug( const BugReport& report )
{
	//HODO: Remove huge waste of memory
	static char post[ sizeof( report ) * 2 ];
	Q_snprintf( post, sizeof( post ), 
		"map_name=%s&title=%s&author=%s&version=%s&x=%f&y=%f&z=%f&body=%s", 
		report.mapName, report.title, report.author, report.version, 
		report.origin.x, report.origin.y, report.origin.z, report.description);

	//curl::HttpPost( "http://google.ca", post );

	return true;
}

void CGUIReportBug::Cancel()
{
	if ( m_Title )
	{
		m_Title->SetText( "" );
	}

	if ( m_Description )
	{
		m_Description->SetText( "" );
	}

	SetVisible( false );
}

void CGUIReportBug::OnCommand( const char *command )
{
	if ( !Q_stricmp( command, "report" ) )
	{
		static BugReport report;
		CBasePlayer *pPlayer = CBasePlayer::GetLocalPlayer();
		if ( pPlayer )
		{
			Q_strncpy( report.author, "name", sizeof( report.author ) );
			report.origin = pPlayer->GetAbsOrigin();
		}
		else
		{
			Q_strncpy( report.author, "unknown name", sizeof( report.author ) );
		}

		Q_strncpy( report.mapName, engine->GetLevelName(), sizeof( report.mapName ) );
		m_Title->GetText( report.title, sizeof( report.title ) );
		m_Description->GetText( report.description, sizeof( report.description ) );

		ReportBug( report );
		SetVisible( false );
	}
	else if ( !Q_stricmp( command, "cancel" ) )
	{
		Cancel();
	}
	else
	{
		BaseClass::OnCommand( command );
	}
}

void CGUIReportBug::ApplySchemeSettings(vgui::IScheme *pScheme)
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor( "Modfg", GetFgColor() ) );
	SetBgColor( Color( 0, 0, 0, 0 ) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );
}
