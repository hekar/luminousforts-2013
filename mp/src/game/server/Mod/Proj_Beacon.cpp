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


//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: Implements the tripmine grenade.
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "beam_shared.h"
#include "shake.h"
#include "Proj_Beacon.h"
#include "vstdlib/random.h"
#include "engine/IEngineSound.h"
#include "explode.h"
#include "particle_parse.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS( npc_beacon, CBeaconGrenade );

BEGIN_DATADESC( CBeaconGrenade )

	DEFINE_FIELD( m_hOwner,		FIELD_EHANDLE ),
	DEFINE_FIELD( m_flPowerUp,	FIELD_TIME ),
	DEFINE_FIELD( m_vecDir,		FIELD_VECTOR ),
	DEFINE_FIELD( m_vecEnd,		FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_posOwner,		FIELD_POSITION_VECTOR ),
	DEFINE_FIELD( m_angleOwner,	FIELD_VECTOR ),

	// Function Pointers
	DEFINE_THINKFUNC( WarningThink ),
	DEFINE_THINKFUNC( PowerupThink ),

END_DATADESC()

CBeaconGrenade::CBeaconGrenade()
{
	m_vecDir.Init();
	m_vecEnd.Init();
	m_posOwner.Init();
	m_angleOwner.Init();

	m_pConstraint = NULL;
	m_bAttached = false;
	m_hAttachEntity = NULL;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
CBeaconGrenade::~CBeaconGrenade( void )
{
	if (m_pConstraint)
	{
		physenv->DestroyConstraint(m_pConstraint);
		m_pConstraint = NULL;
	}
}
void CBeaconGrenade::Spawn( void )
{
	Precache( );
	// motor
	SetMoveType( MOVETYPE_FLY );
	SetSolid( SOLID_BBOX );
	SetModel( "models/Weapons/w_slam.mdl" );

    IPhysicsObject *pObject = VPhysicsInitNormal( SOLID_BBOX, GetSolidFlags() | FSOLID_TRIGGER, true );
	pObject->EnableMotion( false );
	
	// Do not collide with anything, except the world and some other random
	// stuff - Hekar
	SetCollisionGroup( COLLISION_GROUP_DEBRIS );

	SetCycle( 0.0f );
	m_nBody			= 3;
	m_flDamage		= 0;
	m_DmgRadius		= 0;

	ResetSequenceInfo( );
	m_flPlaybackRate	= 0;
	
	UTIL_SetSize(this, Vector( -4, -4, -2), Vector(4, 4, 2));

	m_flPowerUp = gpGlobals->curtime + 2.0;
	
	SetThink( &CBeaconGrenade::PowerupThink );
	SetNextThink( gpGlobals->curtime + 0.2 );

	m_takedamage = DAMAGE_YES;

	m_iHealth = 1;

	EmitSound( "BeaconGrenade.Place" );
	SetDamage ( 200 );

	// Beacon sits at 90 on wall so rotate back to get m_vecDir
	QAngle angles = GetAbsAngles();
	angles.x -= 90;

	AngleVectors( angles, &m_vecDir );
	m_vecEnd = GetAbsOrigin() + m_vecDir * 2048;

	AddEffects( EF_NOSHADOW );
}


void CBeaconGrenade::Precache( void )
{
	PrecacheModel("models/Weapons/w_slam.mdl"); 

	PrecacheParticleSystem( "electrical_arc_01" );

	PrecacheScriptSound( "TripmineGrenade.Place" );
	PrecacheScriptSound( "TripmineGrenade.Activate" );
}


void CBeaconGrenade::WarningThink( void  )
{
	// set to power up
	SetThink( &CBeaconGrenade::PowerupThink );
	SetNextThink( gpGlobals->curtime + 1.0f );
}


void CBeaconGrenade::PowerupThink( void  )
{
	if (gpGlobals->curtime > m_flPowerUp)
	{
		// Make that particle
		MakeParticle();
		m_bIsLive			= true;

		// The moment it's live, then do this - incase the attach entity moves between placing it, and activation
		// use the absorigin of what we're attaching to for the check, if it moves - we blow up.
		if ( m_bAttached && m_hAttachEntity.Get() != NULL )
			m_vAttachedPosition = m_hAttachEntity.Get()->GetAbsOrigin();

		// play enabled sound
		EmitSound( "BeaconGrenade.Activate" );
	}

	SetNextThink( gpGlobals->curtime + 0.1f );
}

void CBeaconGrenade::KillParticle( void )
{
	StopParticleEffects( this );
}

bool CBeaconGrenade::MakeParticle( void )
{
	//DispatchParticleEffect( "electrical_arc_01", PATTACH_ABSORIGIN, this, "beam_attach", true);
	DispatchParticleEffect( "electrical_arc_01", GetAbsOrigin(), GetAbsAngles(), this );

	return true;
}

bool CBeaconGrenade::MakeConstraint( CBaseEntity *pObject )
{
	return true;
}

void CBeaconGrenade::AttachToEntity(CBaseEntity *pOther)
{
	if (!pOther)
		return;

	if ( !VPhysicsGetObject() )
		return;

	m_bAttached			= true;
	m_hAttachEntity		= pOther;

	SetMoveType			( MOVETYPE_NONE );

	if (pOther->GetSolid() == SOLID_VPHYSICS && pOther->VPhysicsGetObject() != NULL )
	{
		SetSolid(SOLID_NONE); //Tony; switch to bbox solid instead of vphysics, because we've made the physics object non-solid
		//SetMoveType		( MOVETYPE_VPHYSICS ); // use vphysics while constrained!!
	}
	//if it isnt vphysics or bsp, use SetParent to follow it.
	else if (pOther->GetSolid() != SOLID_BSP)
	{
		SetSolid(SOLID_NONE); //Tony; switch to bbox solid instead of vphysics, because we've made the physics object non-solid
		//SetParent( m_hAttachEntity.Get() );
	}
}