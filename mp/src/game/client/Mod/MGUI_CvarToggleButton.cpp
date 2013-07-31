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

	A toggle button thats value is bound to a cvar (both ways).

	Last Updated Jan 26, 2009
===============================================================*/
#include "cbase.h"
#include "MGUI_CvarToggleButton.h"

CvarToggleButton::CvarToggleButton( vgui::Panel *Parent, const char *Name, const char *Text, ConVar& ButtonConVar ) :
	BaseClass( Parent, Name, Text ),
	_ButtonConVar( ButtonConVar )
{
	_ActivatedValue = 1;
	_DeactivatedValue = 0;
	SetSelected( ButtonConVar.GetBool() );
}

CvarToggleButton::~CvarToggleButton()
{
}

bool CvarToggleButton::IsActivated()
{
	return IsSelected();
}

void CvarToggleButton::SetActivatedValue( int Value )
{
	_ActivatedValue = Value;
}

void CvarToggleButton::SetDeactivatedValue( int Value )
{
	_DeactivatedValue = Value;
}

void CvarToggleButton::SetSelected( bool state )
{
	BaseClass::SetSelected( state );

	if ( IsSelected() )
	{
		_ButtonConVar.SetValue( _ActivatedValue );
	}
	else
	{
		_ButtonConVar.SetValue( _DeactivatedValue );
	}
}
