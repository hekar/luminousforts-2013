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
	Brush Entity

	This brush entity defines a no freeze for which blocks cannot
	be frozen in.

	Last Updated Nov 25, 2009
===============================================================*/

#include "cbase.h"
#include "props.h"
#include "igameevents.h"
#include "GameEventListener.h"
#include "triggers.h"
#include "CBlockBase.h"
#include "CNoFreezeBase.h"

BEGIN_DATADESC( CNoFreezeBase )
	
	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "team" ),

END_DATADESC()


//
//	Name: CNoFreezeBase
//	Author: Hekar Khani
//	Description: Defines a no freeze for which blocks cannot be frozen in.
//	Notes: 
//
CNoFreezeBase::CNoFreezeBase()
{
}

CNoFreezeBase::~CNoFreezeBase()
{
}

void CNoFreezeBase::Spawn( void )
{
	BaseClass::Spawn();
	InitTrigger();
}

void CNoFreezeBase::StartTouch( CBaseEntity *pOther )
{
	// Do team checks
	if ( GetTeamNumber() != SDK_TEAM_SPECTATOR &&
		pOther->GetTeamNumber() != GetTeamNumber() )
	{
		return;
	}

	DisallowFreeze( pOther );
}

void CNoFreezeBase::EndTouch( CBaseEntity *pOther )
{
	AllowFreeze( pOther );
}

void CNoFreezeBase::AllowFreeze( CBaseEntity *pOther )
{
	// Allow freezing on end of touch
	if ( pOther->IsBlock() )
	{
		CBlockBase *pBlock = dynamic_cast< CBlockBase* > ( pOther );
		Assert( pBlock );
		pBlock->SetFreezeable( true );
	}
}

void CNoFreezeBase::DisallowFreeze( CBaseEntity *pOther )
{
	// Disallow freezing on touch
	if ( pOther->IsBlock() )
	{
		CBlockBase *pBlock = dynamic_cast< CBlockBase* > ( pOther );
		Assert( pBlock );

		// Unfreeze if frozen
		if( pBlock->IsFrozen() )
		{
			pBlock->ToggleFreeze( NULL );
		}
		
		pBlock->SetFreezeable( false );
	}
}
