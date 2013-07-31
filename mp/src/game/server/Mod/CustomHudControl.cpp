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
	Server
	Custom Gamemode Entity

	Tedonitis has been killing lately, but i'm getting better at 
	using my keyboard setup and i can finally type again...
	feels so good, thought i'd lose my world for a while.

	The job of the custom hud control is to define which hud
	elements are shown on the client's screens. This entity
	has a server side version and a client side version that decide
	these various settings. The idea of creating a server side
	version is that sourcemod and other scripts could plugin and
	control the hud elements realtime from the server only
	(with client side interaction).

	Bleh, 30 minutes of typing and it's back... Back to reading
	I guess...

	Last Updated April 26, 2010
===============================================================*/
#include "cbase.h"
#include "CustomHudControl.h"

LINK_ENTITY_TO_CLASS( lfcus_manager_hud, CCustomHudControl );
BEGIN_DATADESC( CCustomHudControl )
	
	DEFINE_KEYFIELD( m_iEnabledHudElements, FIELD_INTEGER, "EnabledHudElements" ),

	DEFINE_INPUTFUNC( FIELD_INTEGER, "EnableHudElement", InputEnableHudElement ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "DisableHudElement", InputDisableHudElement ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CCustomHudControl, DT_CustomHudControl )
	SendPropInt(SENDINFO(m_iHudElements))
END_SEND_TABLE()

// 
// Name: CCustomHudControl
// Author: Hekar Khani
// Description: Server side version of custom hud control
// Notes:
//
CCustomHudControl::CCustomHudControl()
{
}

CCustomHudControl::~CCustomHudControl()
{
}

void CCustomHudControl::Spawn()
{
	m_iEnabledHudElements = GetSpawnFlags();
	m_iHudElements = m_iEnabledHudElements;
}

void CCustomHudControl::InputEnableHudElement( inputdata_t& data )
{
	// Send a hud message or use a transit entity?
	m_iHudElements |= data.value.Int();
}

void CCustomHudControl::InputDisableHudElement( inputdata_t& data )
{
	m_iHudElements &= ~data.value.Int();
}

void CCustomHudControl::UpdateOnRemove( void )
{
	// Reset hud before killing
	BaseClass::UpdateOnRemove();
}