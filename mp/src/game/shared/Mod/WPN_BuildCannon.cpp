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
#include "in_buttons.h"
#include "hl2mp/weapon_physcannon.h"
#ifndef CLIENT_DLL
#include "Mod/CBlockBase.h"
#endif
#include "Mod/ClassicGameRules.h"

// Time between being allow to unfreeze another block
#define UNFREEZE_ANOTHER_LEETIME 0.5f

// Mode of freezing
ConVar lf_weapon_build_cannon_mode( "lf_weapon_build_cannon_mode", "0", FCVAR_REPLICATED, "0 - Sourceforts Style, 1 - Luminousforts style" );

// Used in Sourceforts freezing mode
#define SOURCEFORTS_FREEZETIME 0.035f

// Used in Luminousforts freezing mode
#define NEXT_BUILD_FREEZE 2.0f
const float g_FreezeTimings[] =
{
	NEXT_BUILD_FREEZE - 1.95f, 
	NEXT_BUILD_FREEZE - 1.90f, 
	NEXT_BUILD_FREEZE - 1.80f, 
	NEXT_BUILD_FREEZE - 1.65f, 
	NEXT_BUILD_FREEZE - 0.50f
};

ConVar lf_freeze_distance_build("lf_freeze_distance_build", "600", FCVAR_REPLICATED | FCVAR_NOTIFY, "", true, 600, true, 600 );

#ifdef CLIENT_DLL
#define CWeaponBuildCannon C_WeaponBuildCannon
#endif

class CWeaponBuildCannon : public CWeaponPhysCannon
{
public:
	DECLARE_CLASS( CWeaponBuildCannon, CWeaponPhysCannon );

	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();

	CWeaponBuildCannon();

	void Precache( void );

	void ItemPostFrame( void );
	
	void FreezeBlock( void );

	SDKWeaponID GetWeaponID( void ) const { return LF_WEAPON_BUILDCANNON; }

private:
#ifndef CLIENT_DLL
	CBlockBase *m_pBlockLastUnFroze;
#endif

	bool m_bDebounceReload;

	int m_iFreezeCount;

	float m_flLastUnfrozeTime;
	float m_flNextFreezeTime;

	DECLARE_ACTTABLE();
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponBuildCannon, DT_WeaponBuildCannon )

BEGIN_NETWORK_TABLE( CWeaponBuildCannon, DT_WeaponBuildCannon )
#ifdef CLIENT_DLL
#else
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CWeaponBuildCannon )
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_lf_build_cannon, CWeaponBuildCannon );

PRECACHE_WEAPON_REGISTER( weapon_lf_build_cannon );

acttable_t	CWeaponBuildCannon::m_acttable[] = 
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

IMPLEMENT_ACTTABLE(CWeaponBuildCannon);

CWeaponBuildCannon::CWeaponBuildCannon ()
{
	m_flNextFreezeTime = gpGlobals->curtime + NEXT_BUILD_FREEZE;
	m_flLastUnfrozeTime = gpGlobals->curtime;

	m_bDebounceReload = false;
	m_iFreezeCount = 0;

#ifndef CLIENT_DLL
	m_pBlockLastUnFroze = NULL;
#endif
}

void CWeaponBuildCannon::Precache( void )
{
	BaseClass::Precache();
}

void CWeaponBuildCannon::ItemPostFrame( void )
{
	BaseClass::ItemPostFrame();

	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if( pOwner == NULL )
	{
		// We found an object. Debounce the button
		m_nAttack2Debounce = 0;
		return;
	}

#ifndef CLIENT_DLL
	if ( m_bDebounceReload )
	{
		if ( !( pOwner->m_nButtons & IN_RELOAD ) )
		{
			m_bDebounceReload = false;
		}
		else
		{
			pOwner->m_nButtons &= ~IN_RELOAD;
		}
	}

	if( pOwner->m_nButtons & IN_RELOAD )
	{
		if( gpGlobals->curtime >= m_flNextFreezeTime )
		{
			FreezeBlock();
		}
	}
#endif

	// Creating a mess by trying to replicate the SF194 physcannon
	if( gpGlobals->curtime >= m_flNextFreezeTime + 1.0f )
	{
		m_iFreezeCount = 0;
	}
}

void CWeaponBuildCannon::FreezeBlock( void )
{
#ifndef CLIENT_DLL
	// Only do this on the server side. Though
	// it's less responsive, it saves
	// bandwidth and doesn't make a large
	// difference - Hekar
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	// Find our block!
	Vector vecSrc, vecDir, vecEnd;

	// Get the starting point of the traceline
	vecSrc = pPlayer->Weapon_ShootPosition();
	pPlayer->EyeVectors( &vecDir );

	// Set the freeze distance
	vecEnd = vecSrc + vecDir * lf_freeze_distance_build.GetInt();
	
	// Create the traceline
	trace_t tr;
	UTIL_TraceLine( vecSrc, vecEnd, MASK_SHOT, GetOwner( ), COLLISION_GROUP_NONE, &tr );
	
	CBaseEntity* pEntity = tr.m_pEnt;

	// Is it already in our hand :O?!
	if ( m_grabController.GetAttached() )
	{
		pEntity = m_grabController.GetAttached();
	}
	
	if ( !pEntity || !pEntity->IsBlock() )
	{
		WeaponSound( SPECIAL3 );
		m_flNextFreezeTime = gpGlobals->curtime + 0.1f;
		return;
	}
	
	CBlockBase *pBlock = dynamic_cast< CBlockBase * > ( pEntity );

	if( !pBlock )
	{
		return;
	}

	// No spamming the freeze button!
	if( pBlock->IsFrozen() )
	{
		if( m_pBlockLastUnFroze != pBlock )
		{
			if( m_flLastUnfrozeTime + UNFREEZE_ANOTHER_LEETIME >= gpGlobals->curtime )
			{
				ClientPrint( pPlayer, HUD_PRINTCENTER, "Please wait to unfreeze another object.\n" );
				m_flNextFreezeTime = gpGlobals->curtime + UNFREEZE_ANOTHER_LEETIME;
				return;
			}

			m_pBlockLastUnFroze = pBlock;
			m_flLastUnfrozeTime = gpGlobals->curtime;
		}
	}

	m_iFreezeCount++;

	// You can't freeze a block in another person's hand
	if ( !pBlock->IsBeingFrozen() ||
		pBlock->GetCurrentFreezer() == pPlayer )
	{
		pBlock->ToggleFreeze( pPlayer );
		m_bDebounceReload = true;
	}

	if ( lf_weapon_build_cannon_mode.GetInt() == 0 )
	{
		// Sourceforts style freezing
		m_flNextFreezeTime = gpGlobals->curtime + SOURCEFORTS_FREEZETIME;
	}
	else if ( lf_weapon_build_cannon_mode.GetInt() == 1 )
	{
		// Luminousforts style freezing
		int index = ( m_iFreezeCount - 1 ) % 5;
		m_flNextFreezeTime = gpGlobals->curtime + g_FreezeTimings[ index ];

		if ( m_iFreezeCount >= 5 )
		{
			m_iFreezeCount = 0;
		}
	}
#endif
}
