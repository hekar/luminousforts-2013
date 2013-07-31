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


/**
 * \file GUI_Editor.cpp
 * \author Hekar Khani
 *
 * Simple Text editor Window
 */

#include "cbase.h"
#if 0
#include <vgui_controls/TextEntry.h>
#include "GUI_Editor.h"

// 2 megabytes
#define INITIAL_FILE_SIZE 2097152
#define LF_CONFIG_FILE "cfg/autoexec.cfg"

CREATE_GUI_INTERFACE( GUIEditorInterface, guieditor );

CON_COMMAND( smod_panel_editor_show, NULL )
{
	guieditor->Show();
}

CON_COMMAND( smod_panel_editor_hide, NULL )
{
	guieditor->Hide();
}

CGUIEditor::CGUIEditor( vgui::VPANEL parent ) :
	BaseClass( NULL, "GUIEditor" )
{
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/sourcescheme.res", "sourcescheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/Editor.res" );

	m_TextArea = NULL;
}

CGUIEditor::~CGUIEditor()
{
}

void CGUIEditor::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		// Find the text area if we don't already have it
		if ( !m_TextArea )
		{
			m_TextArea = dynamic_cast< vgui::TextEntry * > ( FindChildByName( "TextArea", false ) );
			if ( !m_TextArea )
			{
				Error( "Failure to initialize CGUIEditor window" );
			}

			m_TextArea->SetCatchEnterKey( true );
			m_TextArea->SetVerticalScrollbar( true );
			m_TextArea->SetHorizontalScrolling( true );
			m_TextArea->SetMultiline( true );
			m_TextArea->SetEditable( true );
		}

		SetMouseInputEnabled( true );
		Activate();
		RequestFocus();
		LoadTextArea( LF_CONFIG_FILE );
		m_TextArea->GotoTextStart();
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

void CGUIEditor::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
}

void CGUIEditor::OnCommand( const char *command )
{
	if ( !Q_strcmp( command, "save" ) )
	{
		SaveTextArea( LF_CONFIG_FILE );
	}

	BaseClass::OnCommand( command );
}

bool CGUIEditor::LoadTextArea( const char *filepath )
{
	m_TextArea->SelectAllText( true );
	m_TextArea->DeleteSelected();

#define MAX_LOAD_SIZE 50000
	CUtlBuffer buffer( 0, MAX_LOAD_SIZE );
	bool success = g_pFullFileSystem->ReadFile( filepath, "MOD", buffer );
	if ( success )
	{
		char buf[ MAX_LOAD_SIZE ] = { '\0' };
		buffer.GetString( buf, MAX_LOAD_SIZE );
		char *curpos = buf;
		while ( (*curpos) != '\0' )
		{
			m_TextArea->InsertChar( *curpos++ );
		}
	}

	return success;
}

bool CGUIEditor::SaveTextArea( const char *filepath )
{
	CUtlBuffer buffer;
	int read = 0;
	char buf[ 1024 ];
	unsigned int len = sizeof( buf ) - 1;
	while ( read < m_TextArea->GetTextLength() )
	{
		m_TextArea->GetTextRange( buf, read, len );
		read += len;
		buffer.Put( buf, Q_strlen( buf ) );
	}

	return g_pFullFileSystem->WriteFile( filepath, "MOD", buffer );
}

#endif // 0
