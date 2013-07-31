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

#ifndef WPN_COMBAT_CANNON_H
#define WPN_COMBAT_CANNON_H

#include "hl2mp/weapon_physcannon.h"

class CBlockBase;

#ifdef CLIENT_DLL
#define CWeaponCombatCannon C_WeaponCombatCannon
#endif

class CWeaponCombatCannon : public CWeaponPhysCannon
{
public:
	DECLARE_CLASS( CWeaponCombatCannon, CWeaponPhysCannon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CWeaponCombatCannon ();

	void Precache( void );
	void Spawn( void );
	bool Holster( CBaseCombatWeapon *pSwitchingTo = NULL );
	void PrimaryAttack( void );
	void ItemPostFrame( void );
	
#ifndef CLIENT_DLL
	void CombatHealIncrement( void );
	void ResetHealing( void );

	void CombatFreezeIncrement( void );
	CBlockBase *FindTraceBlock( void );
	void FreezeBlock( CBlockBase *pBlock );
	void ResetFreezing( bool finished = false );
	void StopLoopingSounds();

	CBlockBase *GetFreezingBlock() const { return m_pFreezingBlock; }
	CBlockBase *GetLastBlock() const { return m_pLastBlock; }
#endif

	SDKWeaponID GetWeaponID( void ) const { return LF_WEAPON_COMBATCANNON; }

private:
#ifndef CLIENT_DLL
	void GenerateSparks( const Vector& Position );

	void StopFreezeSound( void );
	void StopRepairSound( void );

	trace_t m_tr;

	CBlockBase *m_pFreezingBlock;
	CBlockBase *m_pLastBlock;
	CBlockBase *m_pBlockLastUnFroze;

	float m_flLastUnfrozeTime;
#endif

	float m_flLastHeal;
	bool m_bHealing;
	Vector m_vecPlayerHealPosition;

	bool m_bFreezing;

	float m_flStartTime;
	float m_flElapsedTime;
	float m_flLastFreeze;

	// The temporary freeze during combat while a block is being frozen
	bool m_bTempFreeze;

	float m_flFreezeRatio;

	// Time until next block is frozen
	float m_flFreezeTime;

	// Last time we incremented the elasped time
	float m_flLastTime;

	unsigned int m_nReloadDebounce;

private:
	DECLARE_ACTTABLE();
};

#endif // WPN_COMBAT_CANNON_H