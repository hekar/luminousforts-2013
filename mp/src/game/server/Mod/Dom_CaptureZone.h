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

#ifndef DOM_CAPTUREZONE_H
#define DOM_CAPTUREZONE_H

#ifdef WIN32
#pragma once
#endif // WIN32

#include "Mod/StallableTimer.h"

class CDomCaptureZone : public CBaseEntity
{
	DECLARE_CLASS( CDomCaptureZone, CBaseEntity )
	DECLARE_DATADESC()
	DECLARE_SERVERCLASS()
public:
	CDomCaptureZone();
	~CDomCaptureZone();

	virtual void InitTrigger();
	virtual void Spawn();

	virtual void OnPhase( int phase );

	virtual void StartTouch( CBaseEntity *pOther );
	virtual void EndTouch( CBaseEntity *pOther );

	virtual void Think();
	virtual void UpdatePointsThink();
	virtual void CaptureThink();

	virtual int GetZoneId() const { return m_ZoneId; }
	virtual int GetLocked() const { return m_Locked; }
	
	int UpdateTransmitState()
	{
		// always send to all clients
		return SetTransmitState( FL_EDICT_ALWAYS );
	}

protected:
	virtual void RedTouching();
	virtual void BlueTouching();
	virtual void BothTeamsTouching();
	virtual void NoTeamsTouching();
	virtual void HandleCapture();

	virtual void InputEnableCaptures( inputdata_t& inputdata );
	virtual void InputDisableCaptures( inputdata_t& inputdata );
	virtual void InputLock( inputdata_t& inputdata );
	virtual void InputUnlock( inputdata_t& inputdata );
	virtual void OnBuild();
	virtual void OnCombat();

protected:
	int GetParentZone1() { return m_ParentZone1; }
	int GetParentZone2() { return m_ParentZone2; }
	int GetParentZone3() { return m_ParentZone3; }

	CNetworkVar( int, m_Locked );
	CNetworkVar( int, m_ZoneId );
	int m_ParentZone1;
	int m_ParentZone2;
	int m_ParentZone3;

private:
	int m_iSecondsToCapture;
	bool m_bDisableInBuild;
	int m_PointIncreaseInterval;
	int m_PointIncrease;
	
	// On capture of the point
	COutputEvent m_Capture;

	// On capture of the point (Includes team)
	COutputInt m_CapturedBy;

	// On team losing the point
	COutputInt m_Lost;

	bool m_bDisabled;
	bool m_bDisableCaptures;
	CUtlVector< CBasePlayer * > *m_TouchingPlayers;
	CountdownTimer m_CaptureTimer;

	CNetworkVar( int, m_CapState );
	
	// Number of players capturing the point
	CNetworkVar( int, m_iCapturePercentage );
	CNetworkVar( int, m_iCapturingCount );
	CNetworkVar( int, m_iCapturingTeam );

	int m_RedTouching;
	int m_BlueTouching;
	float m_LastPointIncrease;
};

#endif // DOM_CAPTUREZONE_H
