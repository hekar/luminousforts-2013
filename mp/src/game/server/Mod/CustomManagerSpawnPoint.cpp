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
#include "Mod/CSpawnPoint.h"
#include "Mod/CustomGameRules.h"
#include "Mod/CustomManagerSpawnPoint.h"

LINK_ENTITY_TO_CLASS( lfcus_manager_spawnpoint, CCustomManagerSpawnpoint );

BEGIN_DATADESC( CCustomManagerSpawnpoint )
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableRedSpawn", InputDisableRedSpawn ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableRedSpawn", InputEnableRedSpawn ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableBlueSpawn", InputDisableBlueSpawn ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableBlueSpawn", InputEnableBlueSpawn ),

	DEFINE_OUTPUT( m_OnRedPlayerSpawned, "OnRedPlayerSpawned" ),
	DEFINE_OUTPUT( m_OnBluePlayerSpawned, "OnBluePlayerSpawned" ),
END_DATADESC()

CCustomManagerSpawnpoint::CCustomManagerSpawnpoint()
{
}

CCustomManagerSpawnpoint::~CCustomManagerSpawnpoint()
{
}

void CCustomManagerSpawnpoint::InputDisableRedSpawn( inputdata_t& data )
{
	ModifySpawnpoints( SDK_TEAM_RED, false );
}

void CCustomManagerSpawnpoint::InputEnableRedSpawn( inputdata_t& data )
{
	ModifySpawnpoints( SDK_TEAM_RED, true );
}

void CCustomManagerSpawnpoint::InputDisableBlueSpawn( inputdata_t& data )
{
	ModifySpawnpoints( SDK_TEAM_BLUE, false );
}

void CCustomManagerSpawnpoint::InputEnableBlueSpawn( inputdata_t& data )
{
	ModifySpawnpoints( SDK_TEAM_BLUE, true );
}

// Enable or disable spawn points depending on team
PrivateMethod void CCustomManagerSpawnpoint::ModifySpawnpoints( int teamnum, bool enabled )
{
	CBaseEntity *pFoundSpot = NULL;
	for ( int i = 0; i < ARRAYSIZE( g_GenericSpawnPoints ); i++ )
	{
		const char *GenericSpawnPointName = g_GenericSpawnPoints[ i ];

		pFoundSpot = gEntList.FindEntityByClassname( gEntList.FirstEnt(), GenericSpawnPointName );

		while ( pFoundSpot )
		{
			if ( pFoundSpot )
			{
				CSpawnPoint *point = dynamic_cast< CSpawnPoint * > ( pFoundSpot );
				point->SetDisabled( !enabled );
			}

			pFoundSpot = gEntList.FindEntityByClassname( pFoundSpot, GenericSpawnPointName );
		}
	}
}
