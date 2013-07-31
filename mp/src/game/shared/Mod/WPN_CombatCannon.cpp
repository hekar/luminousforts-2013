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
#ifndef CLIENT_DLL
#include "Mod/CBlockBase.h"
#endif
#include "Mod/WPN_CombatCannon.h"
#include "Mod/ClassicGameRules.h"

#define COMBAT_FREEZE_THRESHOLD 98.0f

// Fix these stupid convars
ConVar lf_heal_increment_combat(	"lf_heal_increment_combat", "10", FCVAR_REPLICATED | FCVAR_NOTIFY );
ConVar lf_heal_delay_combat(	"lf_heal_delay_combat", "0.3", FCVAR_REPLICATED );
ConVar lf_heal_distance_combat( "lf_heal_distance_combat", "96", FCVAR_REPLICATED | FCVAR_NOTIFY, "", true, 30, true, 256 );
ConVar lf_freeze_distance_combat( "lf_freeze_distance_combat", "128", FCVAR_REPLICATED | FCVAR_NOTIFY );

ConVar lf_combat_unfreeze_friendly(	"lf_combat_unfreeze_friendly", "6.5", FCVAR_REPLICATED );
ConVar lf_combat_freeze_friendly( "lf_combat_freeze_friendly", "6.0", FCVAR_REPLICATED );
ConVar lf_combat_unfreeze_enemy( "lf_combat_unfreeze_enemy", "10.0", FCVAR_REPLICATED );

IMPLEMENT_NETWORKCLASS_ALIASED( WeaponCombatCannon, DT_WeaponCombatCannon )

BEGIN_NETWORK_TABLE( CWeaponCombatCannon, DT_WeaponCombatCannon )
#ifdef CLIENT_DLL
#else
#endif
END_NETWORK_TABLE()

#ifdef CLIENT_DLL
BEGIN_PREDICTION_DATA( CWeaponCombatCannon )
END_PREDICTION_DATA()
#endif

LINK_ENTITY_TO_CLASS( weapon_lf_combat_cannon, CWeaponCombatCannon );
PRECACHE_WEAPON_REGISTER( weapon_lf_combat_cannon );

acttable_t	CWeaponCombatCannon::m_acttable[] = 
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

IMPLEMENT_ACTTABLE(CWeaponCombatCannon);

CWeaponCombatCannon::CWeaponCombatCannon ()
{
#ifndef CLIENT_DLL
	ResetFreezing();
#endif
	// Required time to freeze a block
	m_flFreezeTime = 20.0f;
}

void CWeaponCombatCannon::Precache( void )
{
	BaseClass::Precache();
	PrecacheScriptSound( "Luminousforts.Repair" );
	PrecacheScriptSound( "Luminousforts.Freeze" );
}

void CWeaponCombatCannon::Spawn( void )
{
#ifndef CLIENT_DLL
	ResetFreezing();
	ResetHealing();
#endif // CLIENT_DLL
	BaseClass::Spawn();
}

bool CWeaponCombatCannon::Holster( CBaseCombatWeapon *pSwitchingTo )
{
#ifndef CLIENT_DLL
	ResetFreezing();
	ResetHealing();
#endif // CLIENT_DLL
	return BaseClass::Holster( pSwitchingTo );
}

void CWeaponCombatCannon::PrimaryAttack( void )
{
	BaseClass::PrimaryAttack();
}

void CWeaponCombatCannon::ItemPostFrame( void )
{
	CBasePlayer *pOwner = ToBasePlayer( GetOwner() );
	if ( pOwner == NULL )
	{
		// We found an object. Debounce the button
		m_nReloadDebounce = 0;
		return;
	}

	if ( ( pOwner->m_nButtons & IN_ATTACK ) && !m_grabController.GetAttached() )
	{
#ifndef CLIENT_DLL
		// Don't let the user heal and freeze at sametime
		CombatHealIncrement();
#endif
		return;
	}
	else
	{
#ifndef CLIENT_DLL
		if ( m_bHealing )
		{
			ResetHealing();
		}
#endif // CLIENT_DLL
	}

#ifndef CLIENT_DLL
	if ( pOwner->m_nButtons & IN_ATTACK && m_grabController.GetAttached() )
	{
		PrimaryAttack();
		BaseClass::ItemPostFrame();
		return;
	}

	if ( m_flFreezeRatio >= 0.98f )
	{
		if ( m_bFreezing )
		{
			m_pFreezingBlock->Freeze( pOwner );
		}
		else
		{
			m_pFreezingBlock->UnFreeze( pOwner );
		}

		ResetFreezing( true );
		DevMsg( "Finished Freezing!\n" );
		// Debounce key
		pOwner->m_nButtons &= ~IN_RELOAD;
	}
	else if ( pOwner->m_nButtons & (~m_nReloadDebounce) && pOwner->m_nButtons & IN_RELOAD )
	{
		CombatFreezeIncrement();
	}
	else
	{
		if ( m_bTempFreeze )
		{
			m_pFreezingBlock->UnFreeze( pOwner );
		}

		m_nReloadDebounce = 0;

		ResetFreezing();
	}

#endif

	BaseClass::ItemPostFrame();
}

#ifndef CLIENT_DLL
void CWeaponCombatCannon::CombatHealIncrement( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	// Find and check the player's punch angle for
	// a block in sight
	CBlockBase *pBlock = FindTraceBlock();
	if( !pBlock )
	{
		ResetHealing();
		return;
	}

	if ( pBlock->GetHealth() + lf_heal_increment_combat.GetFloat() >= pBlock->GetMaxHealth() )
	{
		return;
	}
	else if ( pPlayer->GetTeamNumber() != pBlock->GetTeamNumber() )
	{
		// You may not heal a block not belonging to your team
		return;
	}

	m_vecPlayerHealPosition = m_tr.endpos;

	// Check if the player has moved too far back from the freezing point
	float healDistance = fabs( ( pPlayer->GetAbsOrigin() - m_vecPlayerHealPosition ).Length() );
	if ( healDistance > lf_heal_distance_combat.GetFloat() )
	{
		ResetHealing();
		return;
	}

	// First time heaing, so start the sound
	if ( m_flLastHeal <= 0.0f )
	{
		// Play our repairing noise
		pBlock->EmitSound( "Luminousforts.Repair" );
		m_pLastBlock = pBlock;
	}

	// Start healing the block
	if ( m_flLastHeal + lf_heal_delay_combat.GetFloat() <= gpGlobals->curtime )
	{
		pBlock->SetHealth( pBlock->GetHealth() + lf_heal_increment_combat.GetFloat() );

		SendWeaponAnim( ACT_VM_PRIMARYATTACK );
		DoEffect( EFFECT_HOLDING );

		float health = pBlock->GetHealth();
		float maxHealth = pBlock->GetMaxHealth();

		float percentage = ( health / maxHealth ) * 100;

		// Send a message with information on the amount frozen.
		CSingleUserRecipientFilter user( ToBasePlayer( GetOwner() ) );

		// Send our stuff over the wire
		UserMessageBegin( user, "UpdateFreezeProgress" );
			WRITE_BYTE( 1 ); // Letting the HUD know it's time to turn on
			WRITE_LONG( SFH_FREEZEPROGRESS_HEALING );
			WRITE_FLOAT( percentage );
		MessageEnd();

		m_bHealing = true;
		m_flLastHeal = gpGlobals->curtime;
	}
}

void CWeaponCombatCannon::ResetHealing( void )
{
	m_flLastHeal = -1.0f;
	m_bHealing = false;
	StopRepairSound();

	if ( GetOwner() )
	{
		// Turn off the hud element
		CSingleUserRecipientFilter user( ToBasePlayer( GetOwner() ) );
		user.MakeReliable();
		UserMessageBegin( user, "UpdateFreezeProgress" );
			WRITE_BYTE( 0 );
			WRITE_LONG( 0 );
			WRITE_FLOAT( 0 );
		MessageEnd();
	}

	m_pLastBlock = NULL;
}

void CWeaponCombatCannon::CombatFreezeIncrement( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	// Find and check the player's punch angle for
	// a block in sight
	CBlockBase *pBlock = FindTraceBlock();
	if( !pBlock )
	{
		ResetFreezing();
		return;
	}

	if ( !pBlock->IsFreezeable() || 
		( pBlock->IsBeingFrozen() && pBlock->GetCurrentFreezer() != pPlayer ) )
	{
		ResetFreezing();
		return;
	}

	// Let's presume we're not unfreezing a block
	bool IsUnFreezing = false;

	// Are we looking at the same entity we started to freeze?
	if ( pBlock != m_pFreezingBlock )
	{
		if ( m_flFreezeTime > 0.0f && m_flElapsedTime > 0.0 )
		{
			ResetFreezing();
		}
		m_pFreezingBlock = pBlock;
	}
	
	if ( !m_grabController.GetAttached() )
	{
		// Check if the player has moved too far back from the freezing point
		float freezeDistance = fabs( ( pPlayer->Weapon_ShootPosition() - m_tr.endpos ).Length() );
		if ( freezeDistance > lf_freeze_distance_combat.GetFloat() )
		{
			ResetFreezing();
			return;
		}
	}

	// The player has started freezing a block
	if ( m_flLastFreeze <= 0.0f )
	{
		m_flStartTime = gpGlobals->curtime;

		// Amount of time required to Freeze/UnFreeze a block in combat
		float flFreezeTime = lf_combat_unfreeze_enemy.GetFloat();

		// Check if the block is on our team
		if ( pBlock->GetTeamNumber() == GetOwner()->GetTeamNumber() )
		{
			// Calculate our freeze time
			if ( pBlock->IsFrozen() )
			{
				flFreezeTime =  lf_combat_unfreeze_friendly.GetFloat();
			}
			else
			{
				flFreezeTime = lf_combat_freeze_friendly.GetFloat();
				m_bTempFreeze = true;
				pBlock->Freeze( pPlayer );
			}
		}
		else
		{
			// Completely different values for unfreezing on enemy team
			float flHealthRatio = (float)pBlock->GetHealth() / (float)pBlock->GetMaxHealth();
			flFreezeTime = (float)lf_combat_unfreeze_enemy.GetFloat() * (float)flHealthRatio;
		}

		// Play our freezing noise
		float duration = 10.0f;
		pBlock->EmitSound( "Luminousforts.Freeze", 0.0f, &duration );
		m_pLastBlock = pBlock;

		m_flFreezeTime = flFreezeTime;
	}

	// Check if we're unfreezing (Block will be frozen if we're unfreezing)
	if ( pBlock->IsFrozen() && !m_bTempFreeze )
	{
		IsUnFreezing = true;
	}

	// Calculate the freezing ratio
	m_flFreezeRatio = 0.0f;
	if ( m_flFreezeTime > 0.0f && m_flElapsedTime > 0.0 )
	{
		m_flFreezeRatio = ( m_flElapsedTime / m_flFreezeTime );
	}

	// Send a message with information on the amount frozen.
	CSingleUserRecipientFilter user( ToBasePlayer(GetOwner( )) );

	float percentage = m_flFreezeRatio * 100.0f;

	// Make Reliable for first packet sent
	if( percentage < 2.0f )
	{
		user.MakeReliable();
	}

	int iFreezingType = SFH_FREEZEPROGRESS_NONE;

	// Do all our checks here and only send a single enum
	// to save a little bit of our precious bandwidth
	if ( IsUnFreezing )
	{
		if ( GetOwner()->GetTeamNumber() != m_pFreezingBlock->GetTeamNumber() )
		{
			iFreezingType = SFH_FREEZEPROGRESS_UNFREEZING_ENEMY;
		}
		else
		{
			iFreezingType = SFH_FREEZEPROGRESS_UNFREEZING;
		}
		m_bFreezing = false;
	}
	else
	{
		iFreezingType = SFH_FREEZEPROGRESS_FREEZING;
		m_bFreezing = true;
	}

	// Send our stuff over the wire
	UserMessageBegin( user, "UpdateFreezeProgress" );
		WRITE_BYTE( 1 ); // Letting the HUD know it's time to turn on
		WRITE_LONG( iFreezingType );
		WRITE_FLOAT( percentage );
	MessageEnd();

	// Display the welding sparks
	GenerateSparks( m_tr.endpos );

	pBlock->SetCurrentFreezer( pPlayer );

	m_flElapsedTime = gpGlobals->curtime - m_flStartTime;
	m_flLastFreeze = gpGlobals->curtime;
}

CBlockBase *CWeaponCombatCannon::FindTraceBlock( void )
{
	CHL2MP_Player *pPlayer = ToHL2MPPlayer( GetOwner() );
	CBaseEntity *pEntity = NULL;
	
	if ( m_grabController.GetAttached() )
	{
		pEntity = m_grabController.GetAttached();
	}
	else
	{
		Vector vecSrc, vecEnd, vecDir;
		vecSrc	= pPlayer->Weapon_ShootPosition();
		pPlayer->EyeVectors( &vecDir );

		// Calculate max distance as twice the radius of the block 
		float max_distance = pPlayer->CollisionProp()->BoundingRadius()
			+ lf_freeze_distance_combat.GetFloat();

		vecEnd = vecSrc + vecDir * max_distance;
		UTIL_TraceLine( vecSrc, vecEnd, MASK_SHOT, GetOwner(), COLLISION_GROUP_NONE, &m_tr );

		pEntity = m_tr.m_pEnt;
	}

	if ( !pEntity || !pEntity->IsBlock() )
	{
		ResetFreezing();
		return NULL;
	}

	CBlockBase *pBlock = dynamic_cast < CBlockBase * > ( pEntity );

	if( !pBlock )
	{
		DevMsg( "Not a block\n" );
		return NULL;
	}

	return pBlock;
}

void CWeaponCombatCannon::FreezeBlock( CBlockBase *pBlock )
{
	// Only do this on the server side. Though
	// it's less responsive, it saves
	// bandwidth and doesn't make a large
	// difference, but also prevents easy hacking - Hekar
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );

	if ( !pBlock || !pBlock->IsBlock() )
	{
		WeaponSound( SPECIAL3 );
		return;
	}
	
	if( pBlock->IsFrozen() )
	{
		if( m_pBlockLastUnFroze != pBlock )
		{
			m_pBlockLastUnFroze = pBlock;
			m_flLastUnfrozeTime = gpGlobals->curtime;
		}
	}

	pBlock->ToggleFreeze( pPlayer );
}

void CWeaponCombatCannon::ResetFreezing( bool finished )
{
	CBasePlayer *pPlayer = ToBasePlayer( GetOwner() );
	if ( m_pFreezingBlock )
	{
		if ( !finished )
		{
			if ( m_pFreezingBlock->IsFrozen() && m_bTempFreeze )
			{
				if ( pPlayer )
				{
					DevMsg( "Unfreezing temp freeze\n" );
					m_pFreezingBlock->UnFreeze( pPlayer );
				}
				else
				{
					DevMsg( "Unfreezing temp freeze\n" );
					m_pFreezingBlock->UnFreeze( NULL, FROZEN_BY_WORLD );
				}
			}
		}

		m_pFreezingBlock->SetCurrentFreezer( NULL );
	}

	// lolu, wut?! - Hekar
	m_flLastFreeze = -1.0f;
	m_flLastTime = -1.0f;
	m_flFreezeRatio = 0.0f;
	m_flFreezeTime = 0.0f;
	m_flElapsedTime = 0.0f;

	m_bTempFreeze = false;
	m_bFreezing = false;

	m_pFreezingBlock = NULL;

	if ( pPlayer )
	{
		m_nReloadDebounce |= pPlayer->m_nButtons;
	}

	if ( GetOwner() )
	{
		CSingleUserRecipientFilter user( ToBasePlayer( GetOwner() ) );
		user.MakeReliable();

		UserMessageBegin( user, "UpdateFreezeProgress" );
			WRITE_BYTE( 0 ); // Turn off the HUD
			WRITE_LONG( 0 );
			WRITE_FLOAT( 0 );
		MessageEnd();
	}

	StopFreezeSound();

	m_pLastBlock = NULL;
}

void CWeaponCombatCannon::StopLoopingSounds()
{
	StopRepairSound();
	StopFreezeSound();
}

void CWeaponCombatCannon::GenerateSparks( const Vector& Position )
{
	// Generate the sparks that can be seen by all clients
	CDisablePredictionFiltering cSuppressFiltering( true );
	Vector vecSparkDir = ( GetAbsOrigin() - Position );
	vecSparkDir.NormalizeInPlace();

	g_pEffects->Sparks( Position + Vector( 0, 0, 2 ) );
	g_pEffects->Sparks( Position + Vector( 0, 0, 2 ), 5, 2, &vecSparkDir );

	// Generate sparks coming out the back too.
	vecSparkDir.Negate();
	g_pEffects->Sparks( Position + Vector( 0, 0, 2 ), 5, 2, &vecSparkDir);
}

void CWeaponCombatCannon::StopRepairSound()
{
	if ( m_pLastBlock )
	{
		m_pLastBlock->StopSound( "Luminousforts.Repair" );
	}
}

void CWeaponCombatCannon::StopFreezeSound( void )
{
	if ( m_pLastBlock )
	{
		m_pLastBlock->StopSound( "Luminousforts.Freeze" );
	}
}
#endif
