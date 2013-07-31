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
	Shared
	SF194

	Sourceforts 1.9.4 Spawnpoint backward compatiblity

	Last Updated March 12, 2010
==============================================================*/

#include "cbase.h"

#ifdef MOD_SF194
#ifndef CLIENT_DLL
#include "Mod/CSpawnPoint.h"

class CSF194SpawnPoint : public CModSpawnPoint
{
	DECLARE_CLASS( CSF194SpawnPoint, CModSpawnPoint );
	DECLARE_DATADESC();
public:
	CSF194SpawnPoint();

	void Spawn();

private:
	// Sourceforts has a bad FGD and sometimes the teams are just weird, so I had to put in a hack
	int m_iTeamNumberHack;
	int m_iSourcefortsTeamNumber;
};

BEGIN_DATADESC( CSF194SpawnPoint )
	DEFINE_KEYFIELD( m_iTeamNumberHack, FIELD_INTEGER, "TeamNum" ),
	DEFINE_KEYFIELD( m_iSourcefortsTeamNumber, FIELD_INTEGER, "skin" ),
END_DATADESC();

LINK_ENTITY_TO_CLASS( info_player_team_spawn, CSF194SpawnPoint );

CSF194SpawnPoint::CSF194SpawnPoint()
{
}

void CSF194SpawnPoint::Spawn()
{
	BaseClass::Spawn();
	// Hack
	if ( m_iTeamNumberHack != TEAM_SPECTATOR || 
		m_iTeamNumberHack != TEAM_RED || 
		m_iTeamNumberHack != TEAM_BLUE )
	{
		ChangeTeam( m_iSourcefortsTeamNumber );
	}
	else
	{
		ChangeTeam( m_iTeamNumberHack );
	}
}

#endif // !CLIENT_DLL
#endif // MOD_SF194
