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


/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	Sourceforts
	Projectile

	Missile for Rocket Launcher
	Still cleaning this up, but it's from the missile
	for the HL2 Rocket
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

#include "cbase.h"
#if 0
#include "in_buttons.h"

#ifdef CLIENT_DLL
	#include "c_hl2mp_player.h"
	#include "beamdraw.h"
	#include "fx_line.h"
	#include "view.h"
	#include "input.h"
#else
	#include "engine/IEngineSound.h"
	#include "soundent.h"

	#include "hl2mp_player.h"
	#include "basecombatcharacter.h"
	
	#include "explode.h"
	#include "shake.h"
	#include "smoke_trail.h"
	#include "util.h"

	#include "te_effect_dispatch.h"
#endif

#include "debugoverlay_shared.h"

#include "WPN_RocketLauncher.h"
#include "Proj_Missile.h"

BALANCE_DEFINE( lfm_rocket_launcher_explodeforce, 500.0f );
BALANCE_DEFINE( lfm_rocket_launcher_speed, 1500.0f );

BEGIN_DATADESC( CRocketMissile )

	DEFINE_FIELD( m_hOwner,					FIELD_EHANDLE ),
	DEFINE_FIELD( m_hRocketTrail,			FIELD_EHANDLE ),
	DEFINE_FIELD( m_flMarkDeadTime,			FIELD_TIME ),
	DEFINE_FIELD( m_flGracePeriodEndsAt,	FIELD_TIME ),
	DEFINE_FIELD( m_flDamage,				FIELD_FLOAT ),
	
	// Function Pointers
	DEFINE_FUNCTION( MissileTouch ),
	DEFINE_FUNCTION( AccelerateThink ),

END_DATADESC()

LINK_ENTITY_TO_CLASS( lf_rocket_missile, CRocketMissile );

CRocketMissile::CRocketMissile()
{
	m_hRocketTrail = NULL;
	m_pGlowTrail = NULL;
}

CRocketMissile::~CRocketMissile()
{
}

void CRocketMissile::Precache( void )
{
	PrecacheModel( "models/weapons/w_missile.mdl" );
	PrecacheModel( "models/weapons/w_missile_launch.mdl" );
	PrecacheModel( "models/weapons/w_missile_closed.mdl" );
}

void CRocketMissile::Spawn( void )
{
	Precache();

	SetSolid( SOLID_BBOX );
	SetModel("models/weapons/w_missile_launch.mdl");
	UTIL_SetSize( this, -Vector(4,4,4), Vector(4,4,4) );

	SetTouch( &CRocketMissile::MissileTouch );

	SetMoveType( MOVETYPE_FLYGRAVITY, MOVECOLLIDE_FLY_BOUNCE );
	SetThink( &CRocketMissile::IgniteThink );
	
	SetNextThink( gpGlobals->curtime + 0.3f );

	m_takedamage = DAMAGE_YES;
	m_iHealth = m_iMaxHealth = 100;
	m_bloodColor = DONT_BLEED;
	m_flGracePeriodEndsAt = 0;

	AddFlag( FL_OBJECT );
}

void CRocketMissile::Event_Killed( const CTakeDamageInfo &info )
{
	m_takedamage = DAMAGE_NO;
}

unsigned int CRocketMissile::PhysicsSolidMaskForEntity( void ) const
{ 
	return BaseClass::PhysicsSolidMaskForEntity() | CONTENTS_HITBOX;
}

int CRocketMissile::OnTakeDamage_Alive( const CTakeDamageInfo &info )
{
	if ( ( info.GetDamageType() & (DMG_AIRBOAT) ) == false )
	{
		return 0;
	}

	int nRetVal = BaseClass::OnTakeDamage_Alive( info );

	return nRetVal;
}

void CRocketMissile::SetGracePeriod( float flGracePeriod )
{
	m_flGracePeriodEndsAt = gpGlobals->curtime + flGracePeriod;

	// Go non-solid until the grace period ends
	AddSolidFlags( FSOLID_NOT_SOLID );
}

void CRocketMissile::AccelerateThink( void )
{
	Vector vecForward;

	// !!!UNDONE - make this work exactly the same as HL1 RPG, lest we have looping sound bugs again!
	EmitSound( "Missile.Accelerate" );

	// SetEffects( EF_LIGHT );

	AngleVectors( GetLocalAngles(), &vecForward );
	SetAbsVelocity( vecForward * BALANCE_VALUE( Float, lfm_rocket_launcher_speed ) );

	// Hmm...
	//SetThink( &CRocketMissile::SeekThink );
	//SetNextThink( gpGlobals->curtime + 0.1f );
}

//-----------------------------------------------------------------------------
// The actual explosion 
//-----------------------------------------------------------------------------
void CRocketMissile::DoExplosion( void )
{
	unsigned int spawnflags = SF_ENVEXPLOSION_NOSPARKS | SF_ENVEXPLOSION_NODLIGHTS | SF_ENVEXPLOSION_NOSMOKE;
	float explodeforce = BALANCE_VALUE( Float, lfm_rocket_launcher_explodeforce );

	// Create the actual explosion
	ExplosionCreate( GetAbsOrigin(), GetAbsAngles(), GetOwnerEntity(), 
		GetDamage(), GetDamage() * 2, spawnflags, explodeforce, this);
}

void CRocketMissile::Explode( void )
{
	// Don't explode against the skybox. Just pretend that 
	// the missile flies off into the distance.
	Vector forward;

	GetVectors( &forward, NULL, NULL );

	trace_t tr;
	UTIL_TraceLine( GetAbsOrigin(), GetAbsOrigin() + forward * 16, MASK_SHOT, this, COLLISION_GROUP_NONE, &tr );

	m_takedamage = DAMAGE_NO;
	SetSolid( SOLID_NONE );
	if( tr.fraction == 1.0 || !(tr.surface.flags & SURF_SKY) )
	{
		DoExplosion();
	}

	if( m_hRocketTrail )
	{
		m_hRocketTrail->SetLifetime(0.1f);
		m_hRocketTrail = NULL;
	}

	if( m_pGlowTrail )
	{
		//m_pGlowTrail->SetLifetime(0.1f);
		m_pGlowTrail->StopFollowingEntity();
		m_pGlowTrail->Remove();
		m_pGlowTrail = NULL;
	}

	if ( m_hOwner != NULL )
	{
		m_hOwner->NotifyRocketDied();
		m_hOwner = NULL;
	}

	StopSound( "Missile.Ignite" );
	UTIL_Remove( this );
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pOther - 
//-----------------------------------------------------------------------------
void CRocketMissile::MissileTouch( CBaseEntity *pOther )
{
	Assert( pOther );
	
	// Don't touch triggers (but DO hit weapons)
	if ( pOther->IsSolidFlagSet(FSOLID_TRIGGER|FSOLID_VOLUME_CONTENTS)&& pOther->GetCollisionGroup() != COLLISION_GROUP_WEAPON )
	{
		StopSound( "Missile.Accelerate" );
		StopSound( "Missile.Ignite" );
		this->Remove();
		return;
	}

	Explode();
}

void CRocketMissile::CreateSmokeTrail( void )
{
	if ( m_hRocketTrail )
		return;

	// Smoke trail.
	if ( (m_hRocketTrail = RocketTrail::CreateRocketTrail()) != NULL )
	{
		m_hRocketTrail->m_Opacity = 0.2f;
		m_hRocketTrail->m_SpawnRate = 100;
		m_hRocketTrail->m_ParticleLifetime = 0.5f;
		m_hRocketTrail->m_StartColor.Init( 0.65f, 0.65f , 0.65f );
		m_hRocketTrail->m_EndColor.Init( 0.0, 0.0, 0.0 );
		m_hRocketTrail->m_StartSize = 8;
		m_hRocketTrail->m_EndSize = 32;
		m_hRocketTrail->m_SpawnRadius = 4;
		m_hRocketTrail->m_MinSpeed = 2;
		m_hRocketTrail->m_MaxSpeed = 16;
		
		m_hRocketTrail->SetLifetime( 999 );
		m_hRocketTrail->FollowEntity( this, "0" );
	}

	//const Color& col = g_PR->GetTeamColor();
	Color col = Color (16, 32, 128);
	byte Red (col.r());
	byte Green (col.g());
	byte Blue (col.b());

	m_pGlowTrail = CSpriteTrail::SpriteTrailCreate( "sprites/bluelaser1.vmt", GetLocalOrigin(), false );

	int	nAttachment = LookupAttachment( "fuse" );

	if ( m_pGlowTrail != NULL )
	{
		m_pGlowTrail->FollowEntity( this );
		m_pGlowTrail->SetAttachment( this, nAttachment );
		m_pGlowTrail->SetTransparency( kRenderTransAdd, Red, Green, Blue, 255, kRenderFxNone );
		m_pGlowTrail->SetStartWidth( 25.0f );
		m_pGlowTrail->SetEndWidth( 20.0f );
		m_pGlowTrail->SetLifeTime( 0.8f );
	}
}

void CRocketMissile::IgniteThink( void )
{
	SetMoveType( MOVETYPE_FLY );
	SetModel("models/weapons/w_missile.mdl");
	UTIL_SetSize( this, vec3_origin, vec3_origin );
 	RemoveSolidFlags( FSOLID_NOT_SOLID );

	//TODO: Play opening sound

	Vector vecForward;

	EmitSound( "Missile.Ignite" );

	AngleVectors( GetLocalAngles(), &vecForward );
	SetAbsVelocity( vecForward * BALANCE_VALUE( Float, lfm_rocket_launcher_speed ) );

	// Hmmm...
	//SetThink( &CRocketMissile::SeekThink );
	//SetNextThink( gpGlobals->curtime );

	if ( m_hOwner && m_hOwner->GetOwner() )
	{
		CBasePlayer *pPlayer = ToBasePlayer( m_hOwner->GetOwner() );

		color32 white = { 255,225,205,64 };
		UTIL_ScreenFade( pPlayer, white, 0.1f, 0.0f, FFADE_IN );
	}

	CreateSmokeTrail();
}

CRocketMissile *CRocketMissile::Create( const Vector &vecOrigin, const QAngle &vecAngles, edict_t *pentOwner = NULL )
{
	// Create our missile entity
	CRocketMissile *pMissile = (CRocketMissile *) CBaseEntity::Create
	( 
		"lf_rocket_missile", 
		vecOrigin, 
		vecAngles, 
		CBaseEntity::Instance( pentOwner )
	);

	// Set its settings
	pMissile->SetOwnerEntity( Instance( pentOwner ) );
	pMissile->Spawn();
	pMissile->AddEffects( EF_NOSHADOW );
	
	// Calculate the direction and Fire!
	Vector vecForward;
	AngleVectors( vecAngles, &vecForward );

	pMissile->SetAbsVelocity( vecForward * 300 + Vector( 0,0, 128 ) );

	return pMissile;
}

#endif // 0
