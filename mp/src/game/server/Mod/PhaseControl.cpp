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


#include "cbase.h"
#include "PhaseControl.h"

//
//	Name: CPhaseBase
//	Author: Hekar Khani
//	Description: Base for each phase
//	Notes:
//
CPhaseBase::CPhaseBase( int ID, const char *Name ) :
	m_ID (ID)
{
	Q_strncpy( m_Name, Name, sizeof( m_Name ) );
}

CPhaseBase::~CPhaseBase()
{
}

int CPhaseBase::GetPhaseTime()
{
	return m_PhaseTime;
}

void CPhaseBase::SetPhaseTime( int Seconds )
{
	m_PhaseTime = Seconds;
	
	IGameEvent* pEvent = gameeventmanager->CreateEvent( "phase_time", true );
	
	Assert( pEvent );

	pEvent->SetInt( "newtime", ( int )m_PhaseTime ); // BEWARE OF FLOAT TO INT
	gameeventmanager->FireEvent( pEvent, false );
}


//
//	Name: CPhaseManager
//	Author: Hekar Khani
//	Description: Manager for game phases
//	Notes: This is not a Singleton or global.
//		Use as your own object.
//
CPhaseManager::CPhaseManager ()
{
	UTIL_PrecacheOther( "precache_sourceforts_sounds" );
}

CPhaseManager::~CPhaseManager ()
{
	m_PhaseTable.Purge();
}

void CPhaseManager::AddPhase( CPhaseBase *Phase )
{
	m_PhaseTable.AddToTail( Phase );
}

CPhaseBase *CPhaseManager::PreviousPhase()
{
	unsigned short index = m_PhaseTable.Find( m_CurrentPhase );
	return m_PhaseTable.Element( index % m_PhaseTable.Count() );
}

CPhaseBase *CPhaseManager::NextPhase()
{
	unsigned short index = m_PhaseTable.Find( m_CurrentPhase );
	return m_PhaseTable.Element( index % m_PhaseTable.Count() );
}

void CPhaseManager::SwitchToPhase( const char *name )
{
	CPhaseBase *foundPhase = NULL;
	for ( int i = 0; i < m_PhaseTable.Count(); i++ )
	{
		if ( !Q_stricmp( m_PhaseTable.Element( i )->GetName(), name ) )
		{
			foundPhase = m_PhaseTable.Element( i );
		}
	}


	if ( foundPhase )
	{
		SwitchToPhase( foundPhase );
	}
}

void CPhaseManager::SwitchToPhase( CPhaseBase *phase )
{
	m_CurrentPhase = phase;
	m_CurrentPhase->SwitchTo();

	IGameEvent* pEvent = gameeventmanager->CreateEvent( "phase_switch", true );
	// We always want this event for now
	Assert( pEvent );
	if ( pEvent )
	{
		pEvent->SetInt( "phaseid", GetCurrentPhaseID() );
		gameeventmanager->FireEvent( pEvent, false );
	}
}

int CPhaseManager::GetCurrentPhaseID()
{
	return m_CurrentPhase->GetID();
}

const char *CPhaseManager::GetCurrentPhaseName() const
{
	return m_CurrentPhase->GetName();
}

CPhaseBase *CPhaseManager::GetCurrentPhase()
{
	return m_CurrentPhase;
}
