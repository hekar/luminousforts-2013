/* ***** BEGIN LICENSE BLOCK *****
Version: MPL 1.1/LGPL 2.1/GPL 2.0

The contents of this file are subject to the Mozilla Public License Version 
1.1 (the "License"); you may not use this file except in compliance with
...
for the specific language governing rights and limitations under the
License.

The Original Code is for Luminous Forts.

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

	Disposes of players, disposes of blocks and returns flag.

	Last Updated Oct 2 2009
===============================================================*/

#include "cbase.h"
#if 0
#include "team.h"
#include "hl2mp_player.h"
#include "triggers.h"
#include "model_types.h"

// Flag headers
#include "props.h"
#include "igameevents.h"
#include "GameEventListener.h"
#include "Mod/CFlagBase.h"

#include "Mod/CBlockDisposer.h"

#define FLAG_CAPTURE_POINTS 100



LINK_ENTITY_TO_CLASS( lfc_trigger_disposer, CBlockDisposer );

BEGIN_DATADESC( CBlockDisposer )
	
	DEFINE_ENTITYFUNC( Touch ),

	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "team" ),
	DEFINE_OUTPUT( m_OnDisposeBlock, "OnDisposeBlock" ),
	DEFINE_OUTPUT( m_OnDisposePlayer, "OnDisposePlayer" ),

END_DATADESC()

CBlockDisposer::CBlockDisposer()
{
}

void CBlockDisposer::Spawn ()
{
	// Set the touch functions, etc.
	SetTouch( &CBlockDisposer::Touch );
	InitTrigger ();
	CreateVPhysics();
}

bool CBlockDisposer::CreateVPhysics( void )
{
	// We don't want a shadow from the disposer
	VPhysicsInitShadow( false, false );
	return true;
}

// Where the actual work of the disposer is done
void CBlockDisposer::Touch( CBaseEntity *pOther )
{
	// check if the entity is the flag
	if (pOther->GetCollisionGroup() == COLLISION_GROUP_FLAG)
	{
		Msg( "Returning Flag\n" );
		CFlagClassic *pFlag = dynamic_cast < CFlagClassic* > ( pOther );
		if (pFlag)
		{
			pFlag->ReturnToSpawn();
		}
	}
	else if (pOther->IsBlock()) // If the entity is a block
	{
		CTeam *pTeam = dynamic_cast <CTeam *> (pOther->GetTeam());
		Assert( pTeam );
		pTeam->SetBlockCount( pTeam->GetBlockCount () - 1 );
		pOther->Remove();

		m_OnDisposeBlock.FireOutput( this, NULL, 0.0f );
	}
	else if (pOther->IsPlayer())
	{
		CModPlayer *pPlayer = dynamic_cast< CModPlayer* >( pOther );
		
		// Return the flag if the player has it
		if (pPlayer->HasFlag())
		{
			pPlayer->ReturnFlag();
		}

		// Kill the player if they're not on the safe team
		if (pOther->GetTeamNumber() != GetTeamNumber())
		{
			pPlayer->SetArmorValue( 0 );
			CTakeDamageInfo info( this, this, pPlayer->GetHealth(), DMG_DISSOLVE );
			pPlayer->TakeDamage( info );

			m_OnDisposePlayer.FireOutput( this, NULL, 0.0f );
		}
	}
}
#endif // 0
