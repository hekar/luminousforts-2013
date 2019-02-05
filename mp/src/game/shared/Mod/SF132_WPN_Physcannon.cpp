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


/*===============================================================
	Shared
	Sourceforts 1.3.2 Gamemode

	Sourceforts 1.3.2 gravity gun

	Last Updated Feb 27, 2010
===============================================================*/

#include "cbase.h"

#ifdef MOD_SF132

#include "cbase.h"
#include "in_buttons.h"
#include "hl2mp/weapon_physcannon.h"
#ifndef CLIENT_DLL
#include "Mod/CBlockBase.h"
#endif
#include "SF132_GameRules.h"

#ifdef CLIENT_DLL
	#define CWeaponSF132Cannon C_WeaponSF132Cannon
#else
	#include "player_resource.h"
#endif

#define SF132_MAX_PLAYERS_PHYS_FREEZE 4
#define SF132_ASK_DELAY 1.5f

ConVar sf132_allow_physcannon_freeze( "sf132_allow_physcannon_freeze", "1", FCVAR_ARCHIVE | FCVAR_REPLICATED | FCVAR_NOTIFY );

class CWeaponSF132Cannon : public CWeaponPhysCannon
{
	DECLARE_CLASS( CWeaponSF132Cannon, CWeaponPhysCannon );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
public:
	CWeaponSF132Cannon();
	~CWeaponSF132Cannon();

	void Precache( void );

	void ItemPostFrame( void );
	void FreezeHeldBlock( void );

	SDKWeaponID GetWeaponID( void ) const { return SF132_WEAPON_PHYSCANNON; }

private:
	float m_flLastFreezeTime;
	float m_flLastAskTime;

	DECLARE_ACTTABLE();
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponSF132Cannon, DT_WeaponSF132Cannon )

BEGIN_NETWORK_TABLE( CWeaponSF132Cannon, DT_WeaponSF132Cannon )
#ifdef CLIENT_DLL
#else
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CWeaponSF132Cannon )
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_sf132_physcannon, CWeaponSF132Cannon );
PRECACHE_WEAPON_REGISTER( weapon_sf132_physcannon );

acttable_t CWeaponSF132Cannon::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_PHYSGUN,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_PHYSGUN,			false },

	{ ACT_MP_RUN,						ACT_HL2MP_RUN_PHYSGUN,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_PHYSGUN,			false },

	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PHYSGUN,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PHYSGUN,	false },

	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_PHYSGUN,		false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_PHYSGUN,		false },

	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_PHYSGUN,					false },
};

IMPLEMENT_ACTTABLE( CWeaponSF132Cannon );

CWeaponSF132Cannon::CWeaponSF132Cannon()
{
	m_flLastFreezeTime = gpGlobals->curtime;
	m_flLastAskTime = gpGlobals->curtime;
}

CWeaponSF132Cannon::~CWeaponSF132Cannon()
{
}

void CWeaponSF132Cannon::Precache( void )
{
	BaseClass::Precache();
}

void CWeaponSF132Cannon::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner == NULL )
	{
		// We found an object. Debounce the button
		m_nAttack2Debounce = 0;
		return;
	}

#ifndef CLIENT_DLL
	if ( pOwner->m_nButtons & IN_RELOAD &&
		gpGlobals->curtime - m_flLastFreezeTime >= 1.0 &&
		sf132_allow_physcannon_freeze.GetBool())
	{
		int Count = 0;
		for (int i = 1; i < MAX_PLAYERS; i++)
		{
			CBasePlayer *pPlayer = ( CBasePlayer * )UTIL_PlayerByIndex( i );
			if ( !pPlayer )
				continue;

			if( pPlayer->IsConnected() )
			{
				Count++;
			}
		}

		if ( Count <= SF132_MAX_PLAYERS_PHYS_FREEZE )
		{
			FreezeHeldBlock();
			m_flLastFreezeTime = gpGlobals->curtime;
		}
	}
#else
	// TODO: Fix
//	if ( pOwner->m_nButtons & IN_DROPFLAG )
//	{
//		if ( gpGlobals->curtime - m_flLastAskTime > SF132_ASK_DELAY )
//		{
//			engine->ClientCmd( "say Freeze Please." );
//			m_flLastAskTime = gpGlobals->curtime;
//
//			pOwner->m_nButtons &= ~IN_DROPFLAG;
//		}
//	}
#endif // CLIENT_DLL

	BaseClass::ItemPostFrame();
}

void CWeaponSF132Cannon::FreezeHeldBlock( void )
{
#ifndef CLIENT_DLL
	CBaseEntity *pEntity = m_grabController.GetAttached();
	if ( pEntity )
	{
		IPhysicsObject *pPhysicsObject = pEntity->VPhysicsGetObject();
		if ( pPhysicsObject != NULL )
		{
			pPhysicsObject->EnableMotion( false );
		}
	}

#endif // CLIENT_DLL
}

#endif // MOD_SF132
