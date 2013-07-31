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

	Button that only works in build phase.

	Last Updated Dec 28, 2009
===============================================================*/

#include "cbase.h"
#include "locksounds.h"
#include "buttons.h"

#include "Mod/BuildOnlyButton.h"
#include "Mod/ClassicGameRules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


LINK_ENTITY_TO_CLASS( lfc_button_build, CBaseButton );

void CBuildOnlyButton::ButtonTouch( ::CBaseEntity *pOther )
{
	// Ignore touches by anything but players
	if ( !pOther->IsPlayer() )
		return;

	if (GameRules()->GetGameMode() == GAMEMODE_CLASSIC)
	{
		if (ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD)
		{
			return;
		}
	}

	BaseClass::ButtonTouch( pOther );
}

void CBuildOnlyButton::ButtonUse( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if (GameRules()->GetGameMode() == GAMEMODE_CLASSIC)
	{
		if (ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD)
		{
			return;
		}
	}

	BaseClass::ButtonUse( pActivator, pCaller, useType, value );
}
