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
	Sourceforts 1.3.2 Gamemode

	Sourceforts 1.3.2 game blocks

	Last Updated Feb 27, 2010
===============================================================*/

#include "cbase.h"
#ifdef MOD_SF132
#include "CBlockBase.h"

#include "SF132_Blocks.h"

CSF132BlockBase::CSF132BlockBase()
{
	SetModelName( MAKE_STRING ("models/sf/2d_1x5.mdl") );
	SetBlockWorth( 0 );
}

CSF132BlockBase::~CSF132BlockBase()
{
}

void CSF132BlockBase::Spawn( void )
{
	BaseClass::Spawn();
}

void CSF132BlockBase::ChangeTeam( int iTeamNum )
{
	CPhysicsProp::ChangeTeam( iTeamNum );
}

int CSF132BlockBase::OnTakeDamage( const CTakeDamageInfo &inputInfo )
{
	return 0;
}

void CSF132BlockBase::OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason )
{
	if ( reason != PUNTED_BY_CANNON )
	{
		SetRenderColorA( byte( 150 ) );
	}
}

void CSF132BlockBase::OnPhaseChange( int Phaseid )
{
	SetRenderColorA( byte( 255 ) );
}

#if 0
LINK_ENTITY_TO_CLASS( CSF132Block1x5 )
CSF132Block1x5::CSF132Block1x5()
{
}

LINK_ENTITY_TO_CLASS( CSF132Block2x3 )
CSF132Block2x3::CSF132Block2x3()
{
}

LINK_ENTITY_TO_CLASS( CSF132Block1x2x2 )
CSF132Block1x2x2::CSF132Block1x2x2()
{
}

#endif // 0

#endif // MOD_SF132