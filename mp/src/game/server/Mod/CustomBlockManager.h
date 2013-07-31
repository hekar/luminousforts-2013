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


#ifndef CUSTOMBLOCKMANAGER_H
#define CUSTOMBLOCKMANAGER_H

class CustomBlockManager : public CBaseEntity
{
	DECLARE_DATADESC();
	DECLARE_CLASS( CustomBlockManager, CBaseEntity );

public:
	CustomBlockManager();
	~CustomBlockManager();

	void Spawn();
	void UpdateOnRemove();
	void Think();

	void InputDisableHandSpawner( inputdata_t& data );
	void InputEnableHandSpawner( inputdata_t& data );

	void InputDisableRedHandSpawner( inputdata_t& data );
	void InputEnableRedHandSpawner( inputdata_t& data );

	void InputDisableBlueHandSpawner( inputdata_t& data );
	void InputEnableBlueHandSpawner( inputdata_t& data );

	void InputDisableBlockDestruction( inputdata_t& data );
	void InputEnableBlockDestruction( inputdata_t& data );

	void InputSetBlockLimit( inputdata_t& data );
	void InputSetRedBlockLimit( inputdata_t& data );
	void InputSetBlueBlockLimit( inputdata_t& data );

private:
	COutputInt m_OnRedBlockSpawned;
	COutputInt m_OnBlueBlockSpawned;

	COutputEvent m_OnRedBlockLimitReached;
	COutputEvent m_OnBlueBlockLimitReached;

	int m_iRedLimit;
	int m_iBlueLimit;

	int m_iRedServerBlockLimit;
	int m_iBlueServerBlockLimit;

	int m_iLastBlockCount[ SDK_TEAM_RED + 1 ];
};


#endif // CUSTOMBLOCKMANAGER_H