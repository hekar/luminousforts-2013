/* ***** BEGIN LICENSE BLOCK *****
Version: MPL 1.1/LGPL 2.1/GPL 2.0

The contents of this file are subject to the Mozilla Public License Version 
1.1 (the "License"); you may not use this file except in compliance with
...
for the specific language governing rights and limitations under the
License.

The Original Code is for Luminous Forts.

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


#ifndef CBASEPLAYERCLASS_H
#define CBASEPLAYERCLASS_H

#ifdef _WIN32
#pragma once
#endif

enum PlayerClassSlots_t
{
	PCLS_SLOT1 = 0,
	PCLS_SLOT2,
	PCLS_SLOT3,
	PCLS_SLOT4,
	PCLS_SLOT5,
	PCLS_SLOT6,
	PCLS_SLOT7,
	PCLS_SLOT8,
	PCLS_SLOT9,
	PCLS_SLOT10
};

enum CTFClassDefines_t
{
	CLS_SCOUT = 0,
	CLS_ENGINEER,
	CLS_SOLIDER,
	CLS_ROCKETEER,
	CLS_SNIPER,
	
	CLS_COUNT
};

struct WeaponDef_t
{
public:
	char *WeaponName;
	char *PriAmmoName;
	char *SecAmmoName;
};

enum WeaponDefines_t
{
	WPN_CROWBAR = 0,
	WPN_STUNSTICK,
	WPN_PISTOL,
	WPN_MAGNUM,
	WPN_SMG,
	WPN_AR2,
	WPN_SHOTGUN,
	WPN_GRENADE,
	WPN_CROSSBOW,
	WPN_RPG,
	WPN_SLAM,
	WPN_PHYSCANNON,

	WPN_COUNT
};

typedef WeaponDef_t WeaponList_t [WPN_COUNT];

class CBasePlayerClass
{
public:
	CBasePlayerClass (CBasePlayer *pPlayer, char *Name);
	virtual ~CBasePlayerClass ();

	virtual void AddWeapon (WeaponDefines_t Weapon);
	virtual void AddWeapon (WeaponDefines_t Weapon, int iAmmoCount, int SecAmmoCount);
	virtual void AddAmmo (char *AmmoName, int iCount);
	virtual void ChangeSpeed (float Speed);
	virtual void ChangeSprint (float Sprint);
	virtual void ChangeHealth (int Health, int Armor);

	virtual void Restock () {};

	//virtual void ChangeModel ();
	
	
	void SetName (char *Name);
	char *GetName ();

private:
	CBasePlayer *m_pPlayer;
	char *m_czName;

};

class CClassicCTFPlayerClass : public CBasePlayerClass 
{
public:
	CClassicCTFPlayerClass (CBasePlayer *pPlayer, char *Name);
	virtual ~CClassicCTFPlayerClass () {};

	virtual void Restock () {};
};

class CScoutPlayerClass : public CClassicCTFPlayerClass
{
public:
	CScoutPlayerClass (CBasePlayer *pPlayer);
	virtual ~CScoutPlayerClass () {};

	//virtual void Restock ();

};

class CEngineerPlayerClass : public CClassicCTFPlayerClass
{
public:
	CEngineerPlayerClass (CBasePlayer *pPlayer);
	virtual ~CEngineerPlayerClass () {};

	//virtual void Restock ();
};

class CSoldierPlayerClass : public CClassicCTFPlayerClass
{
public:
	CSoldierPlayerClass (CBasePlayer *pPlayer);
	virtual ~CSoldierPlayerClass () {};

	//virtual void Restock ();
};

class CRocketeerPlayerClass : public CClassicCTFPlayerClass
{
public:
	CRocketeerPlayerClass (CBasePlayer *pPlayer);
	virtual ~CRocketeerPlayerClass () {};

	//virtual void Restock ();
};

class CSniperPlayerClass : public CClassicCTFPlayerClass
{
public:
	CSniperPlayerClass (CBasePlayer *pPlayer);
	virtual ~CSniperPlayerClass () {};

	//virtual void Restock ();
};
/*
class CPlayerClassManagerBase
{
public:
	CPlayerClassManagerBase () {};
	virtual ~CPlayerClassManagerBase () {};

	virtual CBasePlayerClass *CreatePlayerClass (CHL2_Player *pPlayer, PlayerClassSlots_t Slot) {return NULL;};

	int GetMaxSlot () {return m_iMaxSlot;};

protected:
	void SetMaxSlot (int MaxSlot) {m_iMaxSlot = MaxSlot;};

private:
	//CUtlMap <char *PlayerClassName, CBasePlayerClass *> m_ClassList;
	int m_iMaxSlot;

};
*/
#endif