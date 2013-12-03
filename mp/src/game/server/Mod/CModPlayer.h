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


#ifndef CMODPLAYER_H
#define CMODPLAYER_H


#ifdef _WIN32
#pragma once
#endif

#ifdef MOD_ALPHA_PLAYER
#include "hl2mp_player.h"
#include "SF132_ItemRegister.h"


extern EHANDLE g_pLastDMSpawn;
extern EHANDLE g_pLastBlueSpawn;
extern EHANDLE g_pLastRedSpawn;

class CFlagBase;

class CModPlayer : public CHL2MP_Player
{
public:
	DECLARE_CLASS( CModPlayer, CHL2MP_Player );
	DECLARE_SERVERCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_DATADESC();

	CModPlayer();
	~CModPlayer();

	static CModPlayer *CreatePlayer( const char *className, edict_t *ed );

	CFlagBase *GetFlag();

	// Has touched the flag before
	bool HasTouched();
	void SetTouched( bool touched );

	void GiveFlag (CBaseEntity *pFlag);
	bool HasFlag();

	void ReturnFlag( bool scored = false );
	void DropFlag();

	virtual void ChangeTeam( int iTeamNum );

	virtual void Event_Disconnected();
	virtual void Event_Killed( const CTakeDamageInfo &info );

	virtual void Precache();
	virtual void InitialSpawn();
	virtual void Spawn();

	virtual void DetonateTripmines();
	virtual void RemoveBeacons();

	virtual CBaseEntity *EntSelectSpawnPoint();
	virtual bool SelectTeamSpawnSpot( int iTeamNum, CBaseEntity* &pSpot );

	virtual bool SelectSpawnSpot( const char *pEntClassName, CBaseEntity* &pSpot );

	virtual int GetPlayerClass() const { return m_HL2Local.m_iPlayerClass; }
	virtual void SetPlayerClass(int playerclass) { m_HL2Local.m_iPlayerClass = playerclass; }

	virtual int GetDesiredPlayerClass() const { return m_HL2Local.m_iDesiredPlayerClass; }
	virtual void SetDesiredPlayerClass(int playerclass) { m_HL2Local.m_iDesiredPlayerClass = playerclass; }

	virtual float GetRunSpeed() const  { return 400; }
	virtual void SetRunSpeed(float speed) { }

	virtual float GetSprintSpeed() const { return 420; }
	virtual void SetSprintSpeed(float speed) { }

	virtual void RemoveFromHud( unsigned int hud );
	virtual void AddToHud( unsigned int hud );

	virtual bool HandleCommand_JoinClass( int iClass );

private:
	bool m_bHasFlag;
	CFlagBase *m_pFlag;

	bool m_bHasTouched;

#ifdef MOD_SF132
public:
	CItemRegister *GetItemRegister();

private:
	CItemRegister m_ItemRegister;
#endif // MOD_SF132
};

extern CModPlayer *ToModPlayer( CBaseEntity *pEntity );


#endif // MOD_ALPHA_PLAYER
#endif // CMODPLAYER_H
