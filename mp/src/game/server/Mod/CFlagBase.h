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


#ifndef CFLAG_H
#define CFLAG_H

#ifdef _WIN32
	#pragma once
#endif

#include "props.h"
#include "igameevents.h"
#include "GameEventListener.h"

// Forward declaration
class CModPlayer;

class CFlagBase : public CPhysicsProp, public CGameEventListener
{
public:
	DECLARE_CLASS( CFlagBase, CPhysicsProp );
	DECLARE_DATADESC();
	DECLARE_SERVERCLASS();

public:
	CFlagBase();
	~CFlagBase();

	bool IsFlag() { return true; }

	virtual void Precache( void );
	virtual void Spawn( void );

	// Override CreateVPhysics so physics isn't enabled on spawn
	bool CreateVPhysics();

	const bool PlayerHasFlag() const;

	virtual void SetSpawnPoint( const Vector& Origin );

	virtual void Pickup( CModPlayer *pPlayer );
	virtual void Drop();
	virtual void ReturnToSpawn( bool scored = false );

	int OnTakeDamage( const CTakeDamageInfo &info );

	void InputEnable( inputdata_t& data );
	void InputDisable( inputdata_t& data );
	void InputChangeSpawnLocation( inputdata_t& data );

	virtual void Touch( CBaseEntity *pOther );

	virtual void FireGameEvent( IGameEvent* pEvent );

	virtual void OnPickup() {}
	virtual void OnScore() {}
	virtual void OnDrop() {}
	virtual void OnReturn() {}

	// Event handlers for derived classes
	virtual void OnPickupEvent();
	virtual void OnScoreEvent();
	virtual void OnDropEvent();
	virtual void OnReturnEvent();

	// Outputs for derived classes
	virtual void OnPickupOutput();
	virtual void OnScoreOutput();
	virtual void OnDropOutput();
	virtual void OnReturnOutput();

	virtual void OnPlayerTouch( CBasePlayer *pPlayer ) {}

	virtual void SetFlagStatus( int status, int teamid, int flagteamid, int userid );
	virtual void UpdateFlagStatus();
	virtual void UpdateFlagStatus( CBasePlayer *pPlayer );

	virtual void EnableFlag();
	virtual void DisableFlag();

protected:
	int m_iSourcefortsTeamNumber;

	// Not visible and not touchable
	bool m_bFullyDisabled;
	bool m_bDisabled;
	bool m_bDropped;
	bool m_bPlayerHasFlag;

	float m_TimeDropped;
	float m_TimeTillReturn;

	int m_FlagStatus;
	int m_StatusTeamId;
	int m_StatusFlagTeamId;
	int m_StatusUserId;

	// output events
	COutputEvent m_OnScore;
	COutputEvent m_OnPickup;
	COutputEvent m_OnDropped;
	COutputEvent m_OnReturned;

	CModPlayer *m_pPlayerWithFlag;
	CModPlayer *m_pPlayerDroppedFlag;
	CModPlayer *m_pPlayerReturnedFlag;

	Vector m_vecSpawnOrigin;
};

#endif