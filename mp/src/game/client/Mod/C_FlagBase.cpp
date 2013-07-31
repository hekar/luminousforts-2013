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
	Client/Server

	Client side of Flag.
	Really used for updating the flag position HUD, but I guess
	you could add some cool effects here?

	Last Updated Dec 18, 2009
===============================================================*/
#include "cbase.h"
#include "Hud_FlagPos.h"
#include "hud.h"
#include "C_FlagBase.h"


IMPLEMENT_CLIENTCLASS_DT( C_FlagBase, DT_FlagBase, CFlagBase )
END_RECV_TABLE()


C_FlagBase::C_FlagBase()
{
}

C_FlagBase::~C_FlagBase()
{
}

void C_FlagBase::UpdatePos( const Vector& origin )
{
	CHudFlagLocations *hudflagloc = dynamic_cast <CHudFlagLocations *> (gHUD.FindElement( "CHudFlagLocations" ));
	Assert( hudflagloc );
	
	hudflagloc->UpdateFlagPosition( GetTeamNumber(), GetAbsOrigin() );

	m_vecLastOrigin = origin;
}

void C_FlagBase::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	if ( updateType == DATA_UPDATE_CREATED )
	{
		UpdatePos( GetAbsOrigin() );
		SetNextClientThink( CLIENT_THINK_ALWAYS );
	}
}

void C_FlagBase::ClientThink()
{
	if ( GetAbsOrigin() != m_vecLastOrigin )
	{
		UpdatePos( GetAbsOrigin() );
	}

	SetNextClientThink( CLIENT_THINK_ALWAYS );
	C_PhysicsProp::ClientThink();
}
