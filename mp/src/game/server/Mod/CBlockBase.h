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


#ifndef CBLOCKBASE_H
#define CBLOCKBASE_H

#ifdef _WIN32
	#pragma once
#endif

#include "props.h"
#include "igameevents.h"
#include "GameEventListener.h"

#define BLOCK_NONE -1
#define LF_BLOCK_DMGSCALE_FROZEN	1.0f
#define LF_BLOCK_DMGSCALE_UNFROZEN	4.0f
#define LF_BLOCK_DEFAULT_WORTH 5
#define LF_BLOCK_SKIN_COUNT 7

enum BlockStatus_t
{
	BLKSTATUS_FROZEN = 0,
	BLKSTATUS_UNFROZEN,
	BLKSTATUS_NONE
};

class CBlockBase : public CPhysicsProp, public CGameEventListener
{
public:
	DECLARE_CLASS (CBlockBase, CPhysicsProp);
	DECLARE_SERVERCLASS();
	DECLARE_DATADESC ();

	CBlockBase ();
	virtual ~CBlockBase ();

	bool IsBlock() { return true; }

	virtual void Precache( void );
	virtual void Spawn( void );
	
	virtual void Touch( CBaseEntity *pOther );
	virtual void Think();
	
	virtual void OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason );
	virtual void OnPhysGunDrop( CBasePlayer *pPhysGunUser, PhysGunDrop_t Reason );

	virtual int OnTakeDamage( const CTakeDamageInfo &inputInfo );

	void InputFreeze( inputdata_t &data );
	void InputUnFreeze( inputdata_t &data );
	void InputKill( inputdata_t &data );

	virtual void ChangeTeam( int iTeamNum );

	virtual int TeamStartSkinNum();
	virtual void ChangeSkinOnHealth( int Health );

	virtual void ToggleFreeze( CBasePlayer *pPlayer );

	virtual bool UpdateHealth( int iNewHealth, CBaseEntity *pActivator );
	virtual void Event_Killed( const CTakeDamageInfo &info );

	virtual void OnDisposed( CBaseEntity *pActivator );

	virtual void OnPhaseChange( int Phaseid );
	virtual void FireGameEvent( IGameEvent* pEvent );

	bool IsFrozen() { return m_bFrozen; }

	bool IsFreezeable() const { return m_bFreezeable; }
	void SetFreezeable( bool Enabled ) { m_bFreezeable = Enabled; }

	int GetBlockType() const { return m_BlockType; }
	void SetBlockType( int BlockType ) { m_BlockType = BlockType; }

	int GetBlockWorth() const { return m_BlockWorth; }
	void SetBlockWorth( int BlockWorth ) { m_BlockWorth = BlockWorth; }

	void SetDesiredHealth( int iDesiredHealth ) { m_iDesiredHealth = iDesiredHealth; }

	void Freeze( CBasePlayer *pPlayer, int FrozenBy = FROZEN_BY_PLAYER );
	void UnFreeze( CBasePlayer *pPlayer, int FrozenBy = FROZEN_BY_PLAYER );

	void SetBlockWorthFromConfig( const char *blockworthkey );
	void SetBlockHealthFromConfig( const char *healthKey );

	void SetSpawnFrozen( bool val ) { m_bSpawnFrozen = val; }

private:
	PrivateMethod void SetLastFrozenUserID( int UserID );
	PrivateMethod void SetLastUnFrozenUserID( int UserID );

protected:
	int m_iDesiredHealth;

private:
	// For Mappers to decide if the block takes team count or not
	bool m_bSpawnBlockCount;
	bool m_bSpawnFrozen;

	int m_BlockType;
	int m_BlockWorth;
	char *m_czModelName;
	bool m_bFrozen;

	bool m_bFreezeable;

	COutputEvent m_OnFrozen;
	COutputEvent m_OnUnFrozen;

	CNetworkVar( int, m_iUserIDFreezer );
	CNetworkVar( int, m_iUserIDUnFreezer );
	bool m_bHeldByPlayer;

public:
	// For dealing with combat phase freezing and
	// who is currently freezing the blocks
	CBasePlayer *GetCurrentFreezer()
	{
		return m_pCurrentFreezer;
	}

	// Who is holding the block right now?
	void SetCurrentFreezer( CBasePlayer *pPlayer )
	{
		m_pCurrentFreezer = pPlayer;
	}

	bool IsBeingFrozen()
	{
		return m_pCurrentFreezer != NULL;
	}

private:
	CBasePlayer *m_pCurrentFreezer;
};

#endif