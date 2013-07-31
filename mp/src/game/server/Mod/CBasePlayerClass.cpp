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


#include "cbase.h"
#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
#else
	#include "player.h"
	#include "hl2mp_player.h"
#endif
#include "hl2mp_player_shared.h"
#include "Mod/CBasePlayerClass.h"

static WeaponList_t g_PlayerWeaponList =
{
	{"weapon_crowbar", NULL, NULL},
	{"weapon_stunstick", NULL, NULL},
	{"weapon_pistol", "Pistol", NULL},
	{"weapon_357", "357", NULL },
	{"weapon_smg1", "SMG1", "SMG1_Grenade"},
	{"weapon_ar2", "AR2", "AR2AltFire"},
	{"weapon_shotgun", "BuckShot", NULL},
	{"weapon_frag", "Grenade", NULL},
	{"weapon_crossbow", "XBowBolt", NULL},
	{"weapon_rpg", "RPG_Round", NULL},
	{"weapon_slam", "slam", NULL},
	{"weapon_physcannon", NULL, NULL}	
};

/**
 * TODO: Make static instances of player classes
 */

CBasePlayerClass::CBasePlayerClass (CBasePlayer *pPlayer, char *Name)
{
	m_pPlayer = pPlayer;
	SetName (Name);
}

CBasePlayerClass::~CBasePlayerClass ()
{
	delete m_czName;
}

void CBasePlayerClass::AddWeapon (WeaponDefines_t Weapon)
{
#ifndef CLIENT_DLL
	m_pPlayer->GiveNamedItem (g_PlayerWeaponList[Weapon].WeaponName);
#endif
}

void CBasePlayerClass::AddAmmo (char *AmmoName, int iCount)
{
#ifndef CLIENT_DLL
	m_pPlayer->GiveAmmo (iCount, AmmoName);
#endif
}

void CBasePlayerClass::AddWeapon (WeaponDefines_t Weapon, int iAmmoCount, int iSecAmmoCount = 0)
{
#ifndef CLIENT_DLL
	m_pPlayer->GiveAmmo (iAmmoCount, g_PlayerWeaponList[Weapon].PriAmmoName);
	if (iSecAmmoCount > 0)
	{
		m_pPlayer->GiveAmmo (iAmmoCount, g_PlayerWeaponList[Weapon].SecAmmoName);
	}

	m_pPlayer->GiveNamedItem (g_PlayerWeaponList[Weapon].WeaponName);
#endif
}

void CBasePlayerClass::ChangeSpeed (float Speed)
{
#ifndef CLIENT_DLL
	m_pPlayer->SetMaxSpeed (Speed);
#endif
}

void CBasePlayerClass::ChangeSprint (float Sprint)
{
	//m_pPlayer->m_HL2Local.m_flSuitPower = 
}

void CBasePlayerClass::ChangeHealth (int Health, int Armor)
{
#ifndef CLIENT_DLL
	m_pPlayer->SetHealth (Health);
	m_pPlayer->IncrementArmorValue( Armor, 150 );
#endif
}

void CBasePlayerClass::SetName (char *Name)
{
	m_czName = new char [Q_strlen (Name)];
	Q_strcpy (m_czName, Name);
}

char *CBasePlayerClass::GetName ()
{
	return m_czName;
}

CClassicCTFPlayerClass::CClassicCTFPlayerClass (CBasePlayer *pPlayer, char *Name) : CBasePlayerClass (pPlayer, Name)
{
	AddWeapon (WPN_CROWBAR);
	AddWeapon (WPN_PISTOL, 75, 0);
	AddWeapon (WPN_GRENADE, 5, 0);

	//ChangeSprint ();
	//ChangeSpeed ();
	//ChangeHealth (100, 0);
}

CScoutPlayerClass::CScoutPlayerClass (CBasePlayer *pPlayer) : CClassicCTFPlayerClass (pPlayer, "Scout")
{
#ifndef CLIENT_DLL
	AddWeapon (WPN_SMG, 90, 0);
#endif
}

CEngineerPlayerClass::CEngineerPlayerClass (CBasePlayer *pPlayer) : CClassicCTFPlayerClass (pPlayer, "Engineer")
{
#ifndef CLIENT_DLL
	AddWeapon (WPN_SHOTGUN, 30, 0);
	AddWeapon (WPN_PHYSCANNON);
#endif
}

CSoldierPlayerClass::CSoldierPlayerClass (CBasePlayer *pPlayer) : CClassicCTFPlayerClass (pPlayer, "Soldier")
{
#ifndef CLIENT_DLL
	AddWeapon (WPN_AR2, 270, 0);
#endif
}

CRocketeerPlayerClass::CRocketeerPlayerClass (CBasePlayer *pPlayer) : CClassicCTFPlayerClass (pPlayer, "Rocketeer")
{
#ifndef CLIENT_DLL
	AddWeapon (WPN_RPG, 5, 0);
	AddWeapon (WPN_SLAM, 3, 0);
#endif
}

CSniperPlayerClass::CSniperPlayerClass (CBasePlayer *pPlayer) : CClassicCTFPlayerClass (pPlayer, "Sniper")
{
#ifndef CLIENT_DLL
	AddWeapon (WPN_CROSSBOW, 10, 0);
#endif
}

