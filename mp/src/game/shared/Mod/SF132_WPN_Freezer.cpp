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

	Pistol to freeze blocksS

	Last Updated Feb 27, 2010
===============================================================*/


#include "cbase.h"
#ifdef MOD_SF132
#include "ieffects.h"

#if defined( CLIENT_DLL )

	#define CWeaponFreezer C_WeaponFreezer
	#include "c_hl2mp_player.h"

#else

	#include "hl2mp_player.h"
	#include "props.h"

#endif

#define FREEZE_DELAY 5.0f
#define UNFREEZE_DELAY 5.0f


class CWeaponFreezer : public CBaseCombatWeapon
{
public:
	DECLARE_CLASS(CWeaponFreezer, CBaseCombatWeapon);
	DECLARE_NETWORKCLASS(); 
	DECLARE_PREDICTABLE();
	DECLARE_ACTTABLE();
	
	CWeaponFreezer();

	void PrimaryAttack( void );
	void SecondaryAttack( void );

	bool HasPrimaryAmmo() { return true; }
	bool UsesSecondaryAmmo() { return false; }

	CBaseEntity *TraceItem( Vector *End = NULL );
	bool AllowFreeze( CBaseEntity *pEntity );
	void AddViewKick( void );

#ifndef CLIENT_DLL
	void GenerateSparks( const Vector& Position );
#endif CLIENT_DLL

	virtual SDKWeaponID GetWeaponID( void ) const { return SF132_WEAPON_FREEZER; }
	virtual int GetFireMode() const { return FM_SEMIAUTOMATIC; }

private:

	CWeaponFreezer( const CWeaponFreezer & );
};

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponFreezer, DT_WeaponFreezer )

BEGIN_NETWORK_TABLE( CWeaponFreezer, DT_WeaponFreezer )
END_NETWORK_TABLE()

BEGIN_PREDICTION_DATA( CWeaponFreezer )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( weapon_sf132_freezer, CWeaponFreezer );
PRECACHE_WEAPON_REGISTER( weapon_sf132_freezer );

CWeaponFreezer::CWeaponFreezer()
{
}

void CWeaponFreezer::PrimaryAttack()
{
#ifndef CLIENT_DLL
	if ( gpGlobals->curtime < m_flNextPrimaryAttack )
	{
		SendWeaponAnim( ACT_VM_DRYFIRE );
		return;
	}

	Vector End;
	CBaseEntity *pEntity = TraceItem( &End );

	if ( pEntity )
	{
		if ( AllowFreeze( pEntity ) )
		{
			IPhysicsObject *pPhysicsObject = pEntity->VPhysicsGetObject();
			if ( pPhysicsObject != NULL )
			{
				pPhysicsObject->EnableMotion( false );
				SendWeaponAnim( ACT_VM_PRIMARYATTACK );
				GenerateSparks( End );
				AddViewKick();
				return;
			}
		}
	}

	SendWeaponAnim( ACT_VM_DRYFIRE );

	m_flNextPrimaryAttack = gpGlobals->curtime + FREEZE_DELAY;
#endif
}

void CWeaponFreezer::SecondaryAttack()
{
#ifndef CLIENT_DLL
	if ( gpGlobals->curtime < m_flNextSecondaryAttack )
	{
		SendWeaponAnim( ACT_VM_DRYFIRE );
		return;
	}

	CBaseEntity *pEntity = TraceItem();

	if ( pEntity )
	{
		if ( AllowFreeze( pEntity ) )
		{
			CPhysicsProp *pProp = dynamic_cast< CPhysicsProp * > ( pEntity );
			if ( pProp )
			{
				pProp->EnableMotion();
				SendWeaponAnim( ACT_VM_PRIMARYATTACK );
				AddViewKick();
				return;
			}
		}
	}
	
	SendWeaponAnim( ACT_VM_DRYFIRE );
	m_flNextSecondaryAttack = gpGlobals->curtime + UNFREEZE_DELAY;
#endif
}

CBaseEntity *CWeaponFreezer::TraceItem( Vector *End )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	Vector vecSrc, vecDir, vecEnd;

	// Get the starting point of the traceline
	vecSrc = pPlayer->Weapon_ShootPosition();
	pPlayer->EyeVectors( &vecDir );

	// Set the freeze distance
	vecEnd = vecSrc + vecDir * 1024;
	
	// Create the traceline
	trace_t tr;
	UTIL_TraceLine( vecSrc, vecEnd, MASK_SHOT, GetOwner( ), COLLISION_GROUP_NONE, &tr );

	if ( End )
	{
		*End = vecEnd;
	}

	return tr.m_pEnt;
}

bool CWeaponFreezer::AllowFreeze( CBaseEntity *pEntity )
{
	if ( !pEntity )
		return false;

	if ( pEntity->IsPlayer() || pEntity->GetCollisionGroup() == COLLISION_GROUP_FLAG )
	{
		return false;
	}

	return true;
}

void CWeaponFreezer::AddViewKick( void )
{
	CBasePlayer *pPlayer  = ToBasePlayer( GetOwner() );
	
	if ( pPlayer == NULL )
		return;

	QAngle	viewPunch;

	viewPunch.x = SharedRandomFloat( "pistolpax", 0.25f, 0.5f );
	viewPunch.y = SharedRandomFloat( "pistolpay", -.6f, .6f );
	viewPunch.z = 0.0f;

	//Add it to the view punch
	pPlayer->ViewPunch( viewPunch );
}

#ifndef CLIENT_DLL
void CWeaponFreezer::GenerateSparks( const Vector& Position )
{
	// Generate the sparks for all.
	CDisablePredictionFiltering cSuppressFiltering( true );
	Vector vecSparkDir = ( GetAbsOrigin() - Position );
	vecSparkDir.NormalizeInPlace();
	g_pEffects->Sparks( Position + Vector( 0, 0, 2 ) );
	g_pEffects->Sparks( Position + Vector( 0, 0, 2 ), 5, 2, &vecSparkDir );
	vecSparkDir.Negate(); // Generate sparks coming out the back too.
	g_pEffects->Sparks( Position + Vector( 0, 0, 2 ), 5, 2, &vecSparkDir);
}
#endif // CLIENT_DLL

//Tony; todo; add ACT_MP_PRONE* activities, so we have them.
acttable_t CWeaponFreezer::m_acttable[] = 
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

IMPLEMENT_ACTTABLE( CWeaponFreezer );

#endif // MOD_SF132