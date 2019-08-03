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
#include "ai_basenpc.h"

#include "Mod/CPowerCore.h"
#include "Mod/CAiSpawnNode.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar lfs_node_spawn_radius(
	"lfs_node_spawn_radius",
	"128.0f",
	FCVAR_NOTIFY,
	"Radius to spawn zombies without a spawn point"
);

LINK_ENTITY_TO_CLASS( lfs_ai_spawn_node, CAiSpawnNode );

CAiSpawnNode::CAiSpawnNode()
{
}

CAiSpawnNode::~CAiSpawnNode()
{
}

void CAiSpawnNode::Spawn( void )
{
}

bool CAiSpawnNode::SpawnZombie( void )
{
	const Vector& origin = GetAbsOrigin();

	CBaseEntity *ent;
	for ( CEntitySphereQuery sphere( origin, lfs_node_spawn_radius.GetInt() ); (ent = sphere.GetCurrentEntity()) != NULL; sphere.NextEntity() )
	{
		bool isPlayer = ent->IsPlayer();
		bool isNPC = ent->IsNPC();
		// TODO: Avoid collisions with walls and other solid objects
		if ( isPlayer || isNPC )
			return false;
	}

	const Vector& powerCoreOrigin = m_pPowerCore->GetAbsOrigin();

	trace_t trace;
	UTIL_TraceLine(
		origin,
		powerCoreOrigin,
		MASK_SOLID_BRUSHONLY,
		NULL,
		COLLISION_GROUP_NONE,
		&trace
	);

	CBaseEntity *pAttemptZombie = CreateNoSpawn(
		"npc_zombie",
		origin,
		vec3_angle,
		this
	);

	if ( !pAttemptZombie || !pAttemptZombie->Classify() == CLASS_ZOMBIE )
	{
		return false;
	}

	CAI_BaseNPC *pZombie = (CAI_BaseNPC*) pAttemptZombie;
	if ( !pZombie )
	{
		return false;
	}

	pAttemptZombie->SetHealth( 500 );
	pAttemptZombie->SetMaxHealth( 1500 );
	pAttemptZombie->SetDamage( 100 );
	pZombie->SetRelationshipString( AllocPooledString( "player d_ht 150" ) );
	pZombie->SetRelationshipString( AllocPooledString( "lfc_prop_block2x3 d_ht 125" ) );
	pZombie->SetRelationshipString( AllocPooledString( "lfc_prop_block2x2 d_ht 100" ) );
	pZombie->SetRelationshipString( AllocPooledString( "lfc_prop_block1x5 d_ht 200" ) );
	pZombie->SetRelationshipString( AllocPooledString( "lfc_prop_block3x5 d_ht 100" ) );
	pZombie->SetRelationshipString( AllocPooledString( "lfc_prop_block1x2 d_ht 50" ) );
	pZombie->SetRelationshipString( AllocPooledString( "lfc_prop_block3x5 d_ht 100" ) );

	if ( DispatchSpawn( pAttemptZombie ) == -1 )
	{
		return false;
	}

	return true;
}

void CAiSpawnNode::Think( void )
{
}
