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
	Logic Entity

	An entity with its sole purpose being to spawn blocks.

	Last Updated Sept 02 2009
===============================================================*/

#include "cbase.h"
#include "Mod/BlockUtils.h"
#include "Mod/CBlockBase.h"
#include "Mod/CBlockSpawner.h"

LINK_ENTITY_TO_CLASS( lfc_point_blockspawner, CBlockSpawner  );

BEGIN_DATADESC( CBlockSpawner  )
	
	DEFINE_KEYFIELD( m_iBlockType, FIELD_INTEGER, "block" ),
	DEFINE_KEYFIELD( m_iTeam, FIELD_INTEGER, "team" ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Spawn", InputSpawn ),
	DEFINE_OUTPUT( m_OnSpawn, "OnSpawn" ),

END_DATADESC()

//
//	Name: CBlockSpawner
//	Author: Hekar Khani
//	Description: Logical entity to spawn blocks.
//
CBlockSpawner::CBlockSpawner()
{
	m_iBlockType = BLOCK_1x2;
}

CBaseEntity *CBlockSpawner::SpawnBlock( int blockType )
{
	return ::SpawnBlock( blockType, m_iTeam, GetAbsOrigin(), GetAbsAngles(), this );
}

void CBlockSpawner::InputSpawn( inputdata_t &inputData )
{
	if ( SpawnBlock( m_iBlockType - 1 ) )
	{
		m_OnSpawn.FireOutput( inputData.pActivator, this );
	}
}
