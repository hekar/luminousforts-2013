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
#include "Mod/ClassicGameRules.h"
#include "Mod/CAiSpawnNode.h"
#include "Mod/CAiDirector.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar lfs_difficulty(
	"lfs_difficulty",
	"1",
	FCVAR_NOTIFY,
	"Survival difficulty mode - 0: easy, 1: normal, 2: hard, 3: exhard"
);

ConVar lfs_max_zombie_count(
	"lfs_max_zombie_count",
	"150.0f",
	FCVAR_NOTIFY,
	"Maximum Zombies to spawn"
);


LINK_ENTITY_TO_CLASS( lfs_ai_director, CAiDirector );

CAiDirector::CAiDirector()
{
	m_dLastThink = 0.0;
	// Think every 5 seconds
	m_dThinkInterval = 5.0;
}

CAiDirector::~CAiDirector()
{
	gEntList.RemoveListenerEntity( this );
}

void CAiDirector::Spawn( void )
{
	CBaseEntity *pEnt = NULL;
	while ( ( pEnt = gEntList.NextEnt( pEnt ) ) != NULL )
	{
		OnEntitySpawned( pEnt );
	}

	gEntList.AddListenerEntity( this );
	SetNextThink( gpGlobals->curtime );
}

void CAiDirector::Think( void )
{
	if ( gpGlobals->curtime - m_dLastThink > m_dThinkInterval )
	{
		if ( ShouldSpawn( CLASS_ZOMBIE ) )
		{
			SpawnAtBestNode( CLASS_ZOMBIE );
		}

		m_dLastThink = gpGlobals->curtime;
	}
	SetNextThink( gpGlobals->curtime + m_dThinkInterval );
}

void CAiDirector::OnEntityCreated( CBaseEntity *pEntity )
{
}

void CAiDirector::OnEntitySpawned( CBaseEntity *pEntity )
{
	if ( IsSpawnNode( pEntity ) )
	{
		m_SpawnNodes.AddToHead( pEntity );
	}

	if ( pEntity->Classify() == CLASS_ZOMBIE )
	{
		// Catch Zombies not necessarily created by the director
		m_iZombieCount++;
	}
}

void CAiDirector::OnEntityDeleted( CBaseEntity *pEntity )
{
	if ( IsSpawnNode( pEntity ) )
	{
		m_SpawnNodes.FindAndRemove( pEntity );
	}

	if ( pEntity->Classify() == CLASS_ZOMBIE )
	{
		m_iZombieCount = max( m_iZombieCount - 1, 0 );
	}
}

bool CAiDirector::ShouldSpawn( int classified )
{
	if ( classified == CLASS_ZOMBIE &&
		m_iZombieCount < lfs_max_zombie_count.GetInt() )
	{
		return true;
	}

	return false;
}

bool CAiDirector::SpawnAtBestNode( int classified )
{
	CAiSpawnNode *pSpawnNode = GetBestSpawnNode( classified );
	if ( pSpawnNode )
	{
		if ( classified == CLASS_ZOMBIE )
		{
			return pSpawnNode->SpawnZombie();
		}
	}

	return false;
}

CAiSpawnNode *CAiDirector::GetBestSpawnNode( int classified )
{
	for (int i = 0; i < m_SpawnNodes.Count(); i++)
	{
		CBaseEntity *pEntity = m_SpawnNodes[ i ];
		Assert( pEntity != NULL );
		Assert( IsSpawnNode( pEntity ) );
		if ( !pEntity )
		{
			continue;
		}

		CAiSpawnNode *pSpawnNode = ( CAiSpawnNode * )pEntity;
		if ( !pSpawnNode )
		{
			continue;
		}

		return pSpawnNode;
	}

	return NULL;
}

bool CAiDirector::IsSpawnNode( CBaseEntity* pEntity )
{
	return pEntity && Q_strcmp( pEntity->GetClassname(), "lfs_ai_spawn_node" ) == 0;
}
