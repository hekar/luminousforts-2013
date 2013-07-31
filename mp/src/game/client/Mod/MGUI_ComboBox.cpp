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
#include <vgui_controls/Panel.h>
#include "MGUI_ComboBox.h"

// We don't want to check more than this many keyvalue fields for existance
#define MAX_COMBOBOX_OPTIONS 25

MComboBox::MComboBox( vgui::Panel *Parent, const char *Name ) :
	BaseClass( Parent, Name, 1, false )
{
	m_convar = NULL;
}

MComboBox::~MComboBox()
{
}

void MComboBox::OnMenuItemSelected()
{
	BaseClass::OnMenuItemSelected();
	if ( m_convar )
	{
		// Change the current value in the convar
		KeyValues *kv = GetActiveItemUserData();
		const char *value = kv->GetString( "value" );
		m_convar->SetValue( value );
	}
}

void MComboBox::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );
}

void MComboBox::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );

	const char *convar = inResourceData->GetString( "convar", NULL );
	if ( !convar )
	{
		Warning( "No Convar set for Combobox! %s\n", GetName() );
		SetEnabled( false );
		return;
	}

	m_convar = cvar->FindVar( convar );

	if ( !m_convar )
	{
		Warning( "Convar not found for Combobox! %s\n", GetName() );
		SetEnabled( false );
		return;
	}

	const char *previous_value = m_convar->GetString();
	int selected_index = 0;
	for ( int i = 0; i < MAX_COMBOBOX_OPTIONS; i++ )
	{
		char key[ 32 ] = { '\0' };

		// Get the text of item in the combobox
		Q_snprintf( key, sizeof( key ), "text_%d", i + 1 );

		const char *text = inResourceData->GetString(key, NULL);
		if ( !text )
		{
			text = "UNDEFINED";
		}

		// Get the value of the item in the combobox
		Q_snprintf( key, sizeof( key ), "value_%d", i + 1 );
		
		const char *value = inResourceData->GetString(key, NULL);
		if ( !value )
		{
			// Key doesn't exist
			continue;
		}

		KeyValues *kv = new KeyValues("UserData");
		kv->SetString( "value", value );
		this->AddItem( text, kv );


		// HODO: Delete this keyvalue?

		if ( !Q_stricmp( previous_value, value ) )
		{
			selected_index = i;
		}
	}

	if ( GetItemCount() > 0 )
	{
		ActivateItemByRow( selected_index );
	}

	const char *numrows = inResourceData->GetString( "numrows", NULL );
	if ( numrows )
	{
		int rows = clamp( Q_atoi( numrows ), 3, 15 );
		SetNumberOfEditLines( rows );
	}

	// Force override for custom editting
	SetEditable( false );
}
