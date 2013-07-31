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


#ifndef CUSTOMMANAGERPHASES_H
#define CUSTOMMANAGERPHASES_H

#ifdef _WIN32
#pragma once
#endif

#include "igameevents.h"
#include "GameEventListener.h"


class CCustomManagerPhases : public CBaseEntity, public CGameEventListener
{
	DECLARE_CLASS( CCustomManagerPhases, CBaseEntity );
	DECLARE_DATADESC();
public:
	CCustomManagerPhases();
	~CCustomManagerPhases();

	void Spawn();

	void InputDisableBuildPhase(  inputdata_t& data );
	void InputEnableBuildPhase( inputdata_t& data  );
	void InputDisableCombatPhase(  inputdata_t& data  );
	void InputEnableCombatPhase(  inputdata_t& data  );

	void InputDisablePhaseRespawn(  inputdata_t& data  );
	void InputEnablePhaseRespawn(  inputdata_t& data  );

	// Update the on second change
	void Think();

	void FireGameEvent( IGameEvent* pEvent );

private:
	float m_flLastTimeLeft;

private:
	COutputEvent m_OnBuildPhase;
	COutputEvent m_OnCombatPhase;
	COutputFloat m_OnTimeLeftChange;
	COutputFloat m_OnSecondChange; // Time left in phase, updates every second

private:
	bool m_bHasBuildPhase;
	bool m_bHasCombatPhase;
	float m_flBuildLong;
	float m_flBuildShort;
	float m_flCombatLong;
	float m_flCombatShort;
	bool m_bRespawnOnPhaseChange;
};

#endif // CUSTOMMANAGERPHASES_H