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

/**
 * \file StallableTimer.cpp
 * \author Hekar Khani
 *
 * Timer that can be stalled in a state of a limbo.
 * Useful for Capture points when two teams on a single point
 * and the timer needs to be stalled at point it can remember.
 */


#include "cbase.h"
#include "Mod/StallableTimer.h"

CStallableTimer::CStallableTimer()
{
	Reset();
}

CStallableTimer::~CStallableTimer()
{
}

void CStallableTimer::Start( float duration )
{
	m_duration = duration;
	if ( m_hasStarted )
	{
		m_unstalled = Now();
		m_isStalled = false;
	}
	else
	{
		m_start = Now();
	}

	m_hasStarted = true;
}

bool CStallableTimer::HasStarted() const
{
	return m_start > 0.05f;
}

void CStallableTimer::Stall()
{
	m_stalled = Now();
	m_isStalled = true;
}

bool CStallableTimer::IsStalled() const
{
	return m_isStalled;
}

/**
 * Have we elapsed, while taking the stalled
 * period into account
 */
bool CStallableTimer::HasElapsed()
{
	return Now() - (m_start + m_duration + 
		((Now() - m_stalled) - (Now() - m_unstalled))) > 0;
}

void CStallableTimer::Reset()
{
	m_hasStarted = false;
	m_isStalled = false;
	m_duration = 0.0;
	m_elapsed = 0.0;
	m_stalled = 0.0;
	m_start = 0.0;
}

float CStallableTimer::GetTimeLeft()
{
	return 0;
}

float CStallableTimer::GetTimeElapsed()
{
	return 0;
}

float CStallableTimer::Now() const
{
	return gpGlobals->curtime;
}
