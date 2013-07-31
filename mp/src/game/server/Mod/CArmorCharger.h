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


#ifndef CARMORCHARGER_H
#define CARMORCHARGER_H

#ifdef _WIN32
	#pragma once
#endif

#if 0

class CNewRecharge : public CBaseAnimating
{
public:
	DECLARE_CLASS( CNewRecharge, CBaseAnimating );

	void Spawn( );
	bool CreateVPhysics();
	int DrawDebugTextOverlays(void);
	void Off(void);
	void Recharge(void);
	bool KeyValue( const char *szKeyName, const char *szValue );
	void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
	virtual int	ObjectCaps( void ) { return (BaseClass::ObjectCaps() | m_iCaps ); }

	void SetMaxArmor( int Armor ) { m_iMaxArmor = Armor; }
	void SetInitialCharge( void );

protected:
	float   m_flSoundTime;

private:
	void InputRecharge( inputdata_t &inputdata );
	void InputSetCharge( inputdata_t &inputdata );
	float MaxJuice() const;
	void UpdateJuice( int newJuice );
	void Precache( void );

	DECLARE_DATADESC();

	float	m_flNextCharge; 
	int		m_iReactivate ; // DeathMatch Delay until reactvated
	int		m_iJuice;
	int		m_iOn;			// 0 = off, 1 = startup, 2 = going
	
	int		m_nState;
	int		m_iCaps;
	int		m_iMaxJuice;

	int		m_iMaxArmor;
	
	COutputFloat m_OutRemainingCharge;
	COutputEvent m_OnHalfEmpty;
	COutputEvent m_OnEmpty;
	COutputEvent m_OnFull;
	COutputEvent m_OnPlayerUse;

	virtual void StudioFrameAdvance ( void );
	float m_flJuice;
};

#endif // 0
#endif // CARMORCHARGER_H
