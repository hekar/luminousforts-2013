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


#ifndef CSPAWNPOINT_H
#define CSPAWNPOINT_H

#ifdef WIN32
#pragma once
#endif

#ifdef MOD_SF194
	#define GENERIC_SPAWN_COUNT 3
#else 
	#define GENERIC_SPAWN_COUNT 2
#endif // MOD_SF194

// Spawnpoints that use the datadesc to determine their teamnumber
extern const char *g_GenericSpawnPoints[ GENERIC_SPAWN_COUNT ];

#ifdef MOD_SF132
	#define BLUE_SPAWN_COUNT 2
	#define RED_SPAWN_COUNT 2
#else
	#define BLUE_SPAWN_COUNT 1
	#define RED_SPAWN_COUNT 1
#endif // MOD_SF132

// Spawnpoints that are bound to their team by name
extern const char *g_BlueSpawnPoints[ BLUE_SPAWN_COUNT ];
extern const char *g_RedSpawnPoints[ RED_SPAWN_COUNT ];

class CSpawnPoint : public CPointEntity
{
	DECLARE_DATADESC();
public:
	CSpawnPoint();

	bool IsDisabled() { return m_bDisabled; }
	void InputEnable( inputdata_t &inputdata ) { m_bDisabled = false; }
	void InputDisable( inputdata_t &inputdata ) { m_bDisabled = true; }

	void SetDisabled( bool disabled ) { m_bDisabled = disabled; }

private:
	bool m_bDisabled;
};

class CModSpawnPoint : public CSpawnPoint
{
	DECLARE_DATADESC();
public:
	virtual void Spawn();

protected:
	int m_iDesiredTeamNumber;
};

class CBlueSpawnPoint : public CModSpawnPoint
{
public:
	CBlueSpawnPoint();
};

class CRedSpawnPoint : public CModSpawnPoint
{
public:
	CRedSpawnPoint();
};


#endif // CSPAWNPOINT_H