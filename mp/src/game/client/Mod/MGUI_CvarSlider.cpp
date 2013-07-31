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

	A Slider thats value is bound to a cvar (both ways).

	Last Updated Jan 26, 2009
===============================================================*/
#include "cbase.h"
#include "MGUI_CvarSlider.h"


MCvarSlider::MCvarSlider( vgui::Panel *parent, const char *panelName ) :
	BaseClass( parent, panelName )
{
}

MCvarSlider::~MCvarSlider()
{
}

void MCvarSlider::SetScale( float scale )
{
	_flScale = scale;
}

void MCvarSlider::SetScaledValue( float value )
{
	SetValue( value / _flScale );
}

float MCvarSlider::GetScaledValue()
{
	int value = GetValue();
	return value * _flScale;
}

void MCvarSlider::ApplySettings( KeyValues *inResourceData )
{
	BaseClass::ApplySettings( inResourceData );
	const char *convar = inResourceData->GetString( "convar", NULL );
	_ButtonCvar = cvar->FindVar( convar );
	
	if ( !_ButtonCvar )
	{
		Warning( "MCvarCheckButton: ConVar Invalid %s\n", convar );
		SetEnabled( false );
		return;
	}

	int max_value = inResourceData->GetInt( "maxValue", 100 );
	int min_value = inResourceData->GetInt( "minValue", 0 );
	SetRange( min_value, max_value );
	SetSliderThumbSubRange( true, min_value, max_value );
	SetNumTicks( ( max_value - min_value ) / 10 );

	_flScale = inResourceData->GetFloat( "scaleValue", 1.0f );
	SetScaledValue( _ButtonCvar->GetFloat() );
}

void MCvarSlider::OnSliderMoved( int newValue )
{
	if (_ButtonCvar) {
		_ButtonCvar->SetValue( GetScaledValue() );
	}
}

void MCvarSlider::OnThink()
{
	if ( _ButtonCvar )
	{
		SetScaledValue( _ButtonCvar->GetFloat() );
	}
	else
	{
		// figure out how to do error checking without the costly SetEnable
	}

	BaseClass::OnThink();
}
