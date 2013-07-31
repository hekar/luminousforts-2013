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


/*===============================================================
	Client

	A checkbox thats value is bound to a cvar (both ways).

	Last Updated Jan 26, 2009
===============================================================*/
#include "cbase.h"
#include "MGUI_CvarCheckButton.h"

MCvarCheckButton::MCvarCheckButton( vgui::Panel *Parent, const char *Name, const char *Text ) :
	BaseClass( Parent, Name, Text )
{
	_ActivatedValue = 1;
	_DeactivatedValue = 0;
}

MCvarCheckButton::~MCvarCheckButton()
{
}

bool MCvarCheckButton::IsActivated()
{
	return IsSelected();
}

void MCvarCheckButton::SetActivatedValue( int Value )
{
	_ActivatedValue = Value;
}

void MCvarCheckButton::SetDeactivatedValue( int Value )
{
	_DeactivatedValue = Value;
}

void MCvarCheckButton::SetSelected( bool state )
{
	BaseClass::SetSelected( state );

	if ( IsSelected() )
	{
		_ButtonCvar->SetValue( _ActivatedValue );
	}
	else
	{
		_ButtonCvar->SetValue( _DeactivatedValue );
	}
}

void MCvarCheckButton::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );
	const char *convar = inResourceData->GetString( "convar", NULL );
	_ButtonCvar = cvar->FindVar( convar );
	AssertMsg( _ButtonCvar, "MCvarCheckButton: ConVar Invalid" );
}

void MCvarCheckButton::OnThink()
{
	if ( _ButtonCvar )
	{
		SetSelected( _ButtonCvar->GetBool() );
	}
	else
	{
		SetCheckButtonCheckable( false );
	}
}
