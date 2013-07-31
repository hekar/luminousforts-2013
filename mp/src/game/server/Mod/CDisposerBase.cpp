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

	Disposes of players, disposes of blocks and returns flag.

	Last Updated Feb 27, 2010
===============================================================*/

#include "cbase.h"
#include "team.h"
#include "triggers.h"
#include "model_types.h"
#include "Mod/CModPlayer.h"
#include "Mod/CFlagBase.h"
#include "Mod/CBlockBase.h"
#include "Mod/CDisposerBase.h"

BEGIN_DATADESC( CBlockDisposer )
	
	DEFINE_ENTITYFUNC( Touch ),

	DEFINE_KEYFIELD( m_bDissolve, FIELD_BOOLEAN, "dissolve" ),
	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "team" ),
	DEFINE_OUTPUT( m_OnDisposeBlock, "OnDisposeBlock" ),
	DEFINE_OUTPUT( m_OnDisposePlayer, "OnDisposePlayer" ),

END_DATADESC()

CBlockDisposer::CBlockDisposer()
{
	m_bDissolve = false;
}

void CBlockDisposer::Spawn ()
{
	// Set the touch functions, etc.
	SetTouch( &CBlockDisposer::Touch );
#if 0
	AddSolidFlags( FSOLID_NOT_SOLID | FSOLID_TRIGGER );
	BaseClass::Spawn();
#endif

	SetSolid( SOLID_BSP );
	SetMoveType( MOVETYPE_NONE );
	SetModel( STRING( GetModelName( ) ) );
	AddEffects( EF_NODRAW );
	CreateVPhysics();
#if 0
	AddSpawnFlags( SF_TRIG_TOUCH_DEBRIS | SF_TRIGGER_ALLOW_ALL );
	SetCollisionGroup( COLLISION_GROUP_FLAG );
#endif
}

void CBlockDisposer::OnDisposeBlock( CBaseEntity *pBlock )
{
}

void CBlockDisposer::OnDisposePlayer( CBaseEntity *pPlayer )
{
}

bool CBlockDisposer::OnFlagTouch( CBaseEntity *pOther )
{
	// check if the entity is the flag
	if ( pOther->IsFlag() )
	{
		Msg( "Returning Flag\n" );
		CFlagBase *pFlag = dynamic_cast < CFlagBase * > ( pOther );
		if ( pFlag )
		{
			pFlag->ReturnToSpawn();
		}

		return true;
	}

	return false;
}

bool CBlockDisposer::OnBlockTouch( CBaseEntity *pOther )
{
	if ( pOther->IsBlock() ) // If the entity is a block
	{
		CBlockBase *pBlock = dynamic_cast< CBlockBase * > ( pOther );

		CTeam *pTeam = dynamic_cast< CTeam * > ( pBlock->GetTeam() );
		Assert( pTeam );
		pTeam->AddBlockCount( -pBlock->GetBlockWorth() );

		if ( m_bDissolve )
		{
			pBlock->Dissolve( NULL, gpGlobals->curtime, false, ENTITY_DISSOLVE_ELECTRICAL );
		}
		else
		{
			pBlock->Remove();
		}

		OnDisposeBlock( pBlock );
		return true;
	}

	return false;
}

bool CBlockDisposer::OnPlayerTouch( CBaseEntity *pOther )
{
	if ( pOther->IsPlayer() )
	{
		CModPlayer *pPlayer = ToModPlayer( pOther );
		
		// Return the flag if the player has it
		if ( pPlayer->HasFlag() )
		{
			pPlayer->ReturnFlag();
		}

		// Kill the player if they're not on the safe team
		if ( pOther->GetTeamNumber() != GetTeamNumber() )
		{
			pPlayer->SetArmorValue( 0 );
			CTakeDamageInfo info( this, this, pPlayer->GetHealth(), DMG_DISSOLVE );
			pPlayer->TakeDamage( info );

			OnDisposePlayer( pPlayer );
		}

		return true;
	}

	return false;
}

bool CBlockDisposer::OnMiscTouch( CBaseEntity *pOther )
{
	return false;
}

// Where the actual work of the disposer is done
void CBlockDisposer::Touch( CBaseEntity *pOther )
{
	if ( OnFlagTouch( pOther ) )
	{
		return;
	}
	else if ( OnBlockTouch( pOther ) )
	{
		return;
	}
	else if ( OnPlayerTouch( pOther ) )
	{
		return;
	}

	OnMiscTouch( pOther );
}
