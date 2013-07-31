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


#ifndef PHASECONTROL_H
#define PHASECONTROL_H

#ifdef _WIN32
#pragma once
#endif

class CPhaseBase
{
public:
	CPhaseBase( int ID, const char *Name );
	virtual ~CPhaseBase();

	int GetID() {return m_ID;};
	const char *GetName() const {return m_Name;};

	int GetPhaseTime();
	void SetPhaseTime( int Seconds );

	virtual void Init() {}
	virtual void SpawnPlayer( CBasePlayer *pPlayer ) {}
	virtual void RespawnPlayer( CBasePlayer *pPlayer ) {}
	virtual void SwitchTo() {}

private:
	int m_ID;
	char m_Name[ 512 ];

	int m_PhaseTime;
};

class CPhaseManager
{
public:
	CPhaseManager();
	virtual ~CPhaseManager();

	void AddPhase( CPhaseBase *phase );
	
	CPhaseBase *PreviousPhase();
	CPhaseBase *NextPhase();

	void SwitchToPhase( const char *name );
	void SwitchToPhase( CPhaseBase *phase );

	int GetCurrentPhaseID();
	const char *GetCurrentPhaseName() const;
	CPhaseBase *GetCurrentPhase();

private:
	CPhaseBase *m_CurrentPhase;
	CUtlLinkedList<CPhaseBase *> m_PhaseTable;
};

#endif