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
	SF BlockSpawner Logic Entity

	For backwards compatibility with SF 194.

	SF has a single spawner for every block and it also has seperate
	spawners for each individual blocks.

	Last Updated Dec 08, 2009
===============================================================*/

#include "cbase.h"

#ifdef MOD_SF194
#include "team.h"
#include "Mod/CBlockBase.h"

// This is too tightly coupled

enum LegacyBlockTypes_enum
{
	LEGACY_BLOCK_1x2 = 1,
	LEGACY_BLOCK_2x2,
	LEGACY_BLOCK_2x3,
	LEGACY_BLOCK_1_5x4,
	LEGACY_BLOCK_1x5,
	LEGACY_BLOCK_1x1x2,

	LEGACY_BLOCK_LAST,
};


const static char *g_szLegacyBlocks [] =
{
	NULL,
	"prop_block_2d_1x2",
	"prop_block_2d_2x2",
	"prop_block_2d_2x3",
	"prop_block_2d_5x4",
	"prop_block_2d_1x5",
	"prop_block_3d_1x2",
};


class CLegacyBlockSpawner : public CLogicalEntity
{
public:
	DECLARE_CLASS( CLegacyBlockSpawner , CLogicalEntity );
	DECLARE_DATADESC();

public:
	CLegacyBlockSpawner();

	void SpawnBlock();
	void InputSpawn( inputdata_t &inputData );

protected:
	int	m_iBlockType;
};


LINK_ENTITY_TO_CLASS( env_block_spawner, CLegacyBlockSpawner  );

BEGIN_DATADESC( CLegacyBlockSpawner  )
	
	DEFINE_KEYFIELD( m_iBlockType, FIELD_INTEGER, "shape" ),
	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "skin" ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Spawn", InputSpawn ),

END_DATADESC()


//
//	Name: CLegacyBlockSpawner
//	Author: Hekar Khani
//	Description: Logical entity to spawn blocks. Only for compatibility with SF 194.
//				Do not use this in a new map.
//	Notes:
//
CLegacyBlockSpawner::CLegacyBlockSpawner()
{
	m_iBlockType = LEGACY_BLOCK_1x2;
}

void CLegacyBlockSpawner::SpawnBlock()
{
	Assert( m_iBlockType >= LEGACY_BLOCK_1x2 && m_iBlockType < LEGACY_BLOCK_LAST );

	// Possible crash here, be careful
	const char *entname = g_szLegacyBlocks[ m_iBlockType ];
	CBaseEntity *pEntity = CBaseEntity::CreateNoSpawn( entname, GetAbsOrigin(), GetAbsAngles(), this );
	if ( !pEntity )
	{
		Warning( "Could not load block of name: %s\n", entname );
		return;
	}

	CBlockBase *pBlock = (CBlockBase *)pEntity;
	Assert( pBlock );
	pBlock->ChangeTeam( GetTeamNumber() );

	if ( g_pGameRules->IsAllowedToSpawn( pBlock ) )
	{
		CTeam *pTeam = (CTeam *)pBlock->GetTeam();
		pTeam->AddBlockCount( pBlock->GetBlockWorth() );
#ifdef MOD_CONSTRUCT_BLOCKS
		pBlock->SetAbsAngles( QAngle( GetAbsAngles().x, 
			GetAbsAngles().y - 90, GetAbsAngles().z ) );
#endif
		pBlock->Spawn();
	}
	else
	{
		UTIL_Remove( pBlock );
	}
}



void CLegacyBlockSpawner::InputSpawn( inputdata_t &inputData )
{
	SpawnBlock();
}

/////////////////////////////////////////////////////////////////////////////
// Seperate Spawners
/////////////////////////////////////////////////////////////////////////////

class CLegacyBlockSpawner2d_2x3 : public CLegacyBlockSpawner
{
public:
	CLegacyBlockSpawner2d_2x3()
	{
		m_iBlockType = LEGACY_BLOCK_2x3;
	}
};

class CLegacyBlockSpawner2d_1x2 : public CLegacyBlockSpawner
{
public:
	CLegacyBlockSpawner2d_1x2()
	{
		m_iBlockType = LEGACY_BLOCK_1x2;
	}
};

class CLegacyBlockSpawner2d_1x5 : public CLegacyBlockSpawner
{
public:
	CLegacyBlockSpawner2d_1x5()
	{
		m_iBlockType = LEGACY_BLOCK_1x5;
	}
};

// Dumb name given by SF .fgd for 1.5x4
class CLegacyBlockSpawner2d_5x4 : public CLegacyBlockSpawner
{
public:
	CLegacyBlockSpawner2d_5x4()
	{
		m_iBlockType = LEGACY_BLOCK_1_5x4;
	}
};

class CLegacyBlockSpawner2d_2x2 : public CLegacyBlockSpawner
{
public:
	CLegacyBlockSpawner2d_2x2()
	{
		m_iBlockType = LEGACY_BLOCK_2x2;
	}
};

class CLegacyBlockSpawner3d_1x2 : public CLegacyBlockSpawner
{
public:
	CLegacyBlockSpawner3d_1x2()
	{
		m_iBlockType = LEGACY_BLOCK_1x1x2;
	}
};

LINK_ENTITY_TO_CLASS( env_block_spawner_2d_2x3, CLegacyBlockSpawner2d_2x3  );
LINK_ENTITY_TO_CLASS( env_block_spawner_2d_1x2, CLegacyBlockSpawner2d_1x2  );
LINK_ENTITY_TO_CLASS( env_block_spawner_2d_1x5, CLegacyBlockSpawner2d_1x5  );
LINK_ENTITY_TO_CLASS( env_block_spawner_2d_5x4, CLegacyBlockSpawner2d_5x4  );
LINK_ENTITY_TO_CLASS( env_block_spawner_2d_2x2, CLegacyBlockSpawner2d_2x2  );
LINK_ENTITY_TO_CLASS( env_block_spawner_3d_1x2, CLegacyBlockSpawner3d_1x2  );

#endif // MOD_SF194
