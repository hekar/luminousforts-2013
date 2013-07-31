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


#include "cbase.h"
#include "team.h"
#include "CustomBlocks.h"
#include "CBlockSpawner.h"
#include "CustomBlockSpawner.h"

LINK_ENTITY_TO_CLASS( lfcus_point_blockspawner, CCustomBlockSpawner );

BEGIN_DATADESC( CCustomBlockSpawner )
	DEFINE_KEYFIELD( m_bFollowBlockLimit, FIELD_BOOLEAN, "FollowBlockLimit" ),
	DEFINE_KEYFIELD( m_bBlockDestructable, FIELD_INTEGER, "BlockDestructable" ),
	DEFINE_KEYFIELD( m_iBlockHealth, FIELD_INTEGER, "BlockHealth" ),
	DEFINE_KEYFIELD( m_iBlockWorth, FIELD_INTEGER, "BlockWorth" ),
	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "team" ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Spawn", InputSpawn ),
	DEFINE_OUTPUT( m_OnSpawn, "OnSpawn" ),
END_DATADESC()

CCustomBlockSpawner::CCustomBlockSpawner()
{
}

void CCustomBlockSpawner::Precache()
{
	PropBreakablePrecacheAll( GetModelName() );
	BaseClass::Precache();
}

void CCustomBlockSpawner::Spawn()
{
	Precache();
	BaseClass::Spawn();
}

CBaseEntity *CCustomBlockSpawner::SpawnBlock()
{
	CBaseEntity *pEntity = CBaseEntity::CreateNoSpawn( "lfcus_prop_block_custom", GetAbsOrigin(), GetAbsAngles(), this );
	CCustomBlockBase *pBlock = dynamic_cast< CCustomDynamicBlock * > ( pEntity );

	Assert( pBlock );
	if ( !pBlock )
	{
		Warning( "Failure to spawn block. Check your parameters\n" );
		return NULL;
	}

	pBlock->SetModelName( GetModelName() );
	pBlock->ChangeTeam( GetTeamNumber() );
	pBlock->SetDestructable( m_bBlockDestructable );

	if ( g_pGameRules->IsAllowedToSpawn( pBlock ) || !m_bFollowBlockLimit )
	{
		CTeam *pTeam = static_cast< CTeam * > ( pBlock->GetTeam() );
		
		if ( m_bFollowBlockLimit )
		{
			pTeam->AddBlockCount( m_iBlockWorth );
		}

		pBlock->SetMaxHealth( m_iBlockHealth );
		pBlock->UpdateHealth( m_iBlockHealth, this );

		pBlock->Spawn();
	}
	else
	{
		UTIL_Remove( pBlock );
		return NULL;
	}

	return pEntity;
}

void CCustomBlockSpawner::InputSpawn( inputdata_t &data )
{
	if ( SpawnBlock() )
	{
		m_OnSpawn.FireOutput( this, this, 0.0f );
	}
}
