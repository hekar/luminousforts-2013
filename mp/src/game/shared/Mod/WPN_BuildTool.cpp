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
#include "weapon_hl2mpbasehlmpcombatweapon.h"

#ifdef CLIENT_DLL
	#define CWeaponBuildTool C_WeaponBuildTool
	#include "c_hl2mp_player.h"
#else
	#include "hl2mp_player.h"
	#include "Mod/CBlockBase.h"
	#include "Mod/BlockUtils.h"
#endif

BALANCE_DEFINE( lfm_build_tool_snap_x, 1 );
BALANCE_DEFINE( lfm_build_tool_snap_y, 1 );
BALANCE_DEFINE( lfm_build_tool_firerate, 1 );
BALANCE_DEFINE( lfm_build_tool_distance, 5000 );

class CWeaponBuildTool : public CBaseHL2MPCombatWeapon
{
public:
	DECLARE_CLASS( CWeaponBuildTool, CBaseHL2MPCombatWeapon );
	DECLARE_ACTTABLE();

	CWeaponBuildTool()
	{
		lastTime = gpGlobals->curtime;
	}
	
	virtual SDKWeaponID GetWeaponID( void ) const { return LF_WEAPON_BUILDTOOL; }
	virtual int GetFireMode() const { return FM_SEMIAUTOMATIC; }

	// Overridden for a quicker firerate
	virtual float GetFireRate( void ) { return BALANCE_VALUE( Float, lfm_build_tool_firerate ); }

	virtual void ItemPostFrame();
	virtual void PrimaryAttack();
	virtual void SecondaryAttack();

	virtual bool UsesClipsForAmmo1()
	{
		return false;
	}

	virtual bool UsesClipsForAmmo2()
	{
		return false;
	}

	virtual bool UsesPrimaryAmmo()
	{
		return false;
	}

	virtual bool UsesSecondaryAmmo()
	{
		return false;
	}

private:
	CWeaponBuildTool( const CWeaponBuildTool & );

	float lastTime;
};

LINK_ENTITY_TO_CLASS( weapon_lf_build_tool, CWeaponBuildTool );
PRECACHE_WEAPON_REGISTER( weapon_lf_build_tool );

// Activities will need to match with models later on
acttable_t CWeaponBuildTool::m_acttable[] = 
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

IMPLEMENT_ACTTABLE( CWeaponBuildTool );

void CWeaponBuildTool::ItemPostFrame()
{
	BaseClass::ItemPostFrame();

	// Draw the debug layout of the block
}

void CWeaponBuildTool::PrimaryAttack()
{
	// Place a block
#ifndef CLIENT_DLL

	if ( ( gpGlobals->curtime - lastTime ) > 1.0f )
	{
		CBasePlayer *player = (CBasePlayer *) GetOwner();

		trace_t tr;
		GetPlayerTraceLine( tr, player, BALANCE_VALUE( Int, lfm_build_tool_distance ) );

		const Vector& end = tr.endpos;
		Vector pos = Vector( end );
		SnapVector( pos );

		//QAngle angles = player->EyeAngles();
		QAngle angles;
		VectorAngles( tr.plane.normal, angles );
		SnapAngle( angles, SNAP_ANGLE_NONE );

		CBaseEntity *ent = SpawnBlock( 0, player->GetTeamNumber(), pos, angles, this );
		if ( ent->IsBlock() )
		{
			CBlockBase *block = dynamic_cast< CBlockBase * > ( ent );
			block->Freeze( player, FROZEN_BY_PLAYER );
		}

		lastTime = gpGlobals->curtime;
	}

#endif // SERVER
}

void CWeaponBuildTool::SecondaryAttack()
{
	// Delete a block
#ifndef CLIENT_DLL

	CBasePlayer *player = (CBasePlayer *) GetOwner();

	trace_t tr;
	GetPlayerTraceLine( tr, player, BALANCE_VALUE( Int, lfm_build_tool_distance ),
		COLLISION_GROUP_NONE );

	CBaseEntity *ent = tr.m_pEnt;

	if ( ent != NULL && ent->IsBlock() )
	{
		DisposeBlock( ent );
	}

	m_flNextSecondaryAttack = gpGlobals->curtime + 1.0f;

#endif // SERVER
}
