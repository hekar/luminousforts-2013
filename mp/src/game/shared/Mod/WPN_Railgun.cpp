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
#if 0
#include "weapon_sdkbase.h"

#ifdef CLIENT_DLL
	#include "iviewrender_beams.h"
	#include "fx.h"
	#include "c_te_effect_dispatch.h"
#else
	#include "beam_shared.h"
#endif

#include "particle_parse.h"

#if defined( CLIENT_DLL )

	#define CWeaponRailgun C_WeaponRailgun
	#include "c_hl2mp_player.h"

#else

	#include "hl2mp_player.h"

#endif

BALANCE_DEFINE( lfm_railgun_fire_delay, 1.0 );

class CWeaponRailgun : public CWeaponSDKBase
{
public:
	DECLARE_CLASS( CWeaponRailgun, CWeaponSDKBase );
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();
	
	CWeaponRailgun();

	void Precache();

	void PrimaryAttack();
	bool UsesClipsForAmmo1() { return false; }

	virtual SDKWeaponID GetWeaponID( void ) const { return LF_WEAPON_RAILGUN; }
	virtual int GetFireMode() const { return FM_SEMIAUTOMATIC; }

private:

	CWeaponRailgun( const CWeaponRailgun & );
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponRailgun, DT_WeaponRailgun )

BEGIN_NETWORK_TABLE( CWeaponRailgun, DT_WeaponRailgun )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponRailgun )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_lf_railgun, CWeaponRailgun );
PRECACHE_WEAPON_REGISTER( weapon_lf_railgun );

//
//	Name: CWeaponRailgun
//	Author: Hekar Khani
//	Description: A Railgun
//		(kinda like a laser rifle, but it can shoot through certain object)
//	Notes:
//
CWeaponRailgun::CWeaponRailgun()
{
}

void CWeaponRailgun::Precache()
{
	PrecacheParticleSystem( "lf_railgun" );
}

void CWeaponRailgun::PrimaryAttack()
{
	// If my clip is empty (and I use clips) start reload
	if ( UsesClipsForAmmo1() && !m_iClip1 ) 
	{
		Reload();
		return;
	}

	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( !pPlayer )
		return;

	trace_t tr;

	// Now do the vector calculations for the ray of the shot
	Vector start = pPlayer->Weapon_ShootPosition();
	Vector dir, end;
	
	// Get the angles of the player's eyes
	pPlayer->EyeVectors( &dir );

	end = start + dir * 1000.f;

	// Perform our trace for some hits
	UTIL_TraceLine( start, end, MASK_ALL, NULL, COLLISION_GROUP_NONE, &tr );

	end = tr.endpos;

	// End the hit
	CPVSFilter filter( end );

	// Don't send this to the owning player, they already had it predicted
	if ( IsPredicted() )
	{
		filter.UsePredictionRules();
	}

	// Fire off the visual of the ray
	DispatchParticleEffect( "lf_railgun", start, pPlayer->EyeAngles(), pPlayer );

	// Check if we've hit another player then do some damage
	if ( tr.m_pEnt )
	{
		if ( tr.m_pEnt->IsPlayer() )
		{
			// Do some damage
		}
	}



	AddViewKick();
	pPlayer->RemoveAmmo( 1, m_iPrimaryAmmoType );

	m_flNextPrimaryAttack = BALANCE_VALUE( Float, lfm_railgun_fire_delay );
}

acttable_t CWeaponRailgun::m_acttable[] = 
{
	{ ACT_MP_STAND_IDLE,				ACT_HL2MP_IDLE_PISTOL,					false },
	{ ACT_MP_CROUCH_IDLE,				ACT_HL2MP_IDLE_CROUCH_PISTOL,			false },

	{ ACT_MP_RUN,						ACT_HL2MP_RUN_PISTOL,					false },
	{ ACT_MP_CROUCHWALK,				ACT_HL2MP_WALK_CROUCH_PISTOL,			false },

	{ ACT_MP_ATTACK_STAND_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,	false },
	{ ACT_MP_ATTACK_CROUCH_PRIMARYFIRE,	ACT_HL2MP_GESTURE_RANGE_ATTACK_PISTOL,	false },

	{ ACT_MP_RELOAD_STAND,				ACT_HL2MP_GESTURE_RELOAD_PISTOL,		false },
	{ ACT_MP_RELOAD_CROUCH,				ACT_HL2MP_GESTURE_RELOAD_PISTOL,		false },

	{ ACT_MP_JUMP,						ACT_HL2MP_JUMP_PISTOL,					false },
};

IMPLEMENT_ACTTABLE( CWeaponRailgun );

#endif
