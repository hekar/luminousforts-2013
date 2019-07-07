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


#ifndef CAIDIRECTOR_H
#define CAIDIRECTOR_H

class CAiDirector : public CBaseEntity, public IEntityListener
{
public:

	DECLARE_CLASS( CAiDirector, CBaseEntity );

	CAiDirector();
	virtual ~CAiDirector();

	virtual void Spawn( void );
	virtual void Think( void );

	virtual void OnEntityCreated( CBaseEntity *pEntity );
	virtual void OnEntitySpawned( CBaseEntity *pEntity );
	virtual void OnEntityDeleted( CBaseEntity *pEntity );

	virtual bool ShouldSpawn( int classified );
	virtual bool SpawnAtBestNode( int classified );

	virtual CBaseEntity *GetBestSpawnNode( int classified );

	virtual bool IsSpawnNode( CBaseEntity* pEntity );

private:
	EntityList_t m_SpawnNodes;

	double m_dLastThink;
	double m_dThinkInterval;
	int m_iZombieCount;
};


#endif // CAIDIRECTOR_H
