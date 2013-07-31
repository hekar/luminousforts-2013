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
#include "Mod/CBlockBase.h"
#include "Mod/BlockUtils.h"

ConVar lf_block_dissolve( "lf_block_dissolve", "0" );
ConVar lf_block_grid_x( "lf_block_grid_x", "64" );
ConVar lf_block_grid_y( "lf_block_grid_y", "64" );
ConVar lf_block_grid_z( "lf_block_grid_z", "32" );

CBlockBase *FindBlock( CBasePlayer *player, int distance )
{
	Vector vecSrc, vecDir, vecEnd;

	vecSrc = player->Weapon_ShootPosition();
	player->EyeVectors( &vecDir );

	vecEnd = vecSrc + vecDir * distance;
	
	// Create the traceline
	trace_t tr;
	UTIL_TraceLine( vecSrc, vecEnd, MASK_SHOT, player, COLLISION_GROUP_NONE, &tr );
	
	CBaseEntity* pEntity = tr.m_pEnt;
	
	CBlockBase *pBlock = dynamic_cast< CBlockBase * > ( pEntity );

	if( !pBlock )
	{
		return NULL;
	}

	return pBlock;
}

void GetPlayerTraceLine( trace_t& tr, CBasePlayer *player, int distance, int collisionGroup )
{
	Vector vecSrc, vecDir, vecEnd;

	vecSrc = player->Weapon_ShootPosition();
	player->EyeVectors( &vecDir );

	vecEnd = vecSrc + vecDir * distance;
	
	// Create the traceline
	UTIL_TraceLine( vecSrc, vecEnd, MASK_ALL, player, collisionGroup, &tr );
}

Vector GetPlayerTraceLineEnd( CBasePlayer *player, int distance )
{
	trace_t tr;
	GetPlayerTraceLine( tr, player, distance );
	return tr.endpos;
}

CBaseEntity *SpawnBlock( int blockType, int team, const Vector& origin, 
	const QAngle& angles, CBaseEntity *parent )
{
	if ( blockType >= BLOCK_1x2 && blockType < BLOCK_LAST )
	{
		CBaseEntity *pEntity = CBaseEntity::CreateNoSpawn ( 
			g_szBlocks[ blockType ], origin, angles, parent );
		if ( !pEntity )
		{
			return NULL;
		}

		CBlockBase *pBlock = dynamic_cast< CBlockBase * > ( pEntity );
		if ( !pBlock )
		{
			return NULL;
		}

		pBlock->ChangeTeam( team );

		if ( g_pGameRules->IsAllowedToSpawn( pBlock ) )
		{
			CTeam *pTeam = static_cast< CTeam *> ( pBlock->GetTeam() );
			pTeam->AddBlockCount( pBlock->GetBlockWorth() );
			pBlock->Spawn();
		}
		else
		{
			UTIL_Remove( pBlock );
			return NULL;
		}

		return pEntity;
	}
	else
	{
		return NULL;
	}
}

bool DisposeBlock( CBaseEntity *ent )
{
	if ( ent->IsBlock() )
	{
		CBlockBase *pBlock = dynamic_cast< CBlockBase * > ( ent );

		CTeam *pTeam = dynamic_cast< CTeam * > ( pBlock->GetTeam() );
		Assert( pTeam );
		pTeam->AddBlockCount( -pBlock->GetBlockWorth() );

		if ( lf_block_dissolve.GetBool() )
		{
			pBlock->Dissolve( NULL, gpGlobals->curtime, false, ENTITY_DISSOLVE_ELECTRICAL );
		}
		else
		{
			pBlock->Remove();
		}

		return true;
	}
	else
	{
		return false;
	}
}

void SnapVector( Vector& vector )
{
	
	vector.x = RoundFloatToInt(vector.x / lf_block_grid_x.GetFloat()) * lf_block_grid_x.GetFloat();
	vector.y = RoundFloatToInt(vector.y / lf_block_grid_y.GetFloat()) * lf_block_grid_y.GetFloat();
	vector.z = RoundFloatToInt(vector.z / lf_block_grid_z.GetFloat()) * lf_block_grid_z.GetFloat();
}

void SnapAngle( QAngle& angles, int style )
{
	float snap = 45.0f;
	if ( style == SNAP_ANGLE_WALL )
	{
		angles.x = 90.0f;
		// TODO: Take player viewing angle into consideration
		angles.y = 90.0f;
	}
	else
	{
		angles.x = floor(angles.x / snap) * snap;
		angles.y = floor(angles.y / snap) * snap;
	}

	angles.z = RoundFloatToInt(angles.z / snap) * snap;
}
