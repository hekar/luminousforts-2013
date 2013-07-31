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
#include <vstdlib/random.h>
#include "CBlockBase.h"
#include "CRandomBlockSpawner.h"

LINK_ENTITY_TO_CLASS( lfc_random_blockspawner, CRandomBlockSpawner );

BEGIN_DATADESC( CRandomBlockSpawner  )
	DEFINE_INPUTFUNC( FIELD_VOID, "RandomSpawn", InputRandomSpawn ),
END_DATADESC()

CRandomBlockSpawner::CRandomBlockSpawner()
{
}

CRandomBlockSpawner::~CRandomBlockSpawner()
{
}

void CRandomBlockSpawner::InputRandomSpawn( inputdata_t& inputData )
{
	int blockType = random->RandomInt( 0, BLOCK_3x5 );
	if ( SpawnBlock( blockType ) )
	{
		m_OnSpawn.FireOutput( inputData.pActivator, this );
	}
}
