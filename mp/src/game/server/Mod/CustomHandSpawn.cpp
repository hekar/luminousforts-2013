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
#include "Mod/SharedModCvars.h"
#include "Mod/CustomHandSpawn.h"

bool CHandSpawnerManager::CanSpawnBlock( CBaseEntity *pBlock )
{
	return g_pGameRules->IsAllowedToSpawn( pBlock );
}

static CHandSpawnerManager g_HandBlockSpawner;

CHandSpawnerManager *HandBlockSpawner()
{
	return &g_HandBlockSpawner;
}

void CC_HandSpawnBlock( const CCommand& args )
{
	CBasePlayer *pPlayer = UTIL_GetCommandClient();
	if ( !pPlayer )
	{
		return;
	}

	int teamnum = pPlayer->GetTeamNumber();
	if ( teamnum == TEAM_SPECTATOR )
	{
		return;
	}

	int handspawn = lf_hand_spawn_enabled.GetInt();
	if ( ( teamnum == TEAM_RED && !( handspawn & MOD_TEAM_RED ) ) ||
		( teamnum == TEAM_BLUE && !( handspawn & MOD_TEAM_BLUE ) ) )
	{
		return;
	}

	if ( args.ArgC() < 2 )
	{
		Msg( "Invalid Usage: <BlockName>\n" );
		return;
	}

	trace_t tr;
	Vector start = pPlayer->EyePosition();
	Vector dir;
	AngleVectors( pPlayer->EyeAngles(), &dir);
	Vector end = start + dir * lf_hand_spawn_distance.GetInt();

	CTraceFilterWorldAndPropsOnly filter;
	UTIL_TraceLine( start, end, MASK_SOLID, &filter, &tr );

	QAngle blockAngle = pPlayer->GetAbsAngles();
	char blockName[ 1024 ];
	if ( Q_strstr( args.Arg( 1 ), "lfc_prop_block" ) == NULL )
	{
		Q_snprintf( blockName, sizeof( blockName ), "lfc_prop_block%s", args.Arg( 1 ) );
	}
	else
	{
		Q_snprintf( blockName, sizeof( blockName ), "%s", args.Arg( 1 ) );
	}

	CBaseEntity *pEntity = CBaseEntity::CreateNoSpawn( blockName, tr.endpos, blockAngle, NULL );
	if ( !pEntity )
	{
		Warning( "Could not find block:%s\n", blockName );
		return;
	}

	pEntity->ChangeTeam( pPlayer->GetTeamNumber() );

	if ( HandBlockSpawner()->CanSpawnBlock( pEntity ) )
	{
		CBlockBase *pBlock = dynamic_cast< CBlockBase * > ( pEntity );
		if ( !pBlock )
		{
			Warning( "Could not create block from:%s\n", blockName );
			return;
		}

		CTeam *pTeam = static_cast< CTeam *> ( pBlock->GetTeam() );
		if ( !pTeam )
		{
			Warning( "Could not find block team\n" );
			return;
		}

		pTeam->AddBlockCount( pBlock->GetBlockWorth() );
		pBlock->Spawn();
	}
	else
	{
		UTIL_Remove( pEntity );
	}
}

static ConCommand lf_hand_spawn_block( "lf_hand_spawn_block", CC_HandSpawnBlock, "Spawn <BlockName> in front of you" );
