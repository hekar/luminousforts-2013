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
	Client/Server
	Physics Model Entity

	Classic version of CFlag.cpp

	This has been a really neglected piece of code.
	It's very old and poorly done by myself.

	TODO:
		-Rocket firing at flag

	Last Updated Oct 30 2009
===============================================================*/

#include "cbase.h"

#include "team.h"
#include "CModPlayer.h"
#include "triggers.h"

#include "props.h"
#include "igameevents.h"
#include "GameEventListener.h"
#include "Mod/SharedModCvars.h"

#include "in_buttons.h"

#include "Mod/CFlagBase.h"
#include "Mod/CFlagClassic.h"

LINK_ENTITY_TO_CLASS( lfc_prop_flag, CFlagClassic );

BEGIN_DATADESC( CFlagClassic )
	// Kohan: Line to make the CheckFunction() thing shut up about the flag entity.
	DEFINE_THINKFUNC( Think ),
END_DATADESC()

//
//	Name: CFlagClassic
//	Author: Hekar Khani
//	Description: Classic Rollermine flag for CTF
//	Notes:
//
CFlagClassic::CFlagClassic ()
{
	m_bFirstTake = false;
}

CFlagClassic::~CFlagClassic ()
{
}

void CFlagClassic::Precache( void )
{
	PrecacheModel( FLAG_MODEL_CLASSIC );
	BaseClass::Precache();

	PrecacheScriptSound( FLAG_RED_TAKEN );
	PrecacheScriptSound( FLAG_RED_SCORE );
	PrecacheScriptSound( FLAG_RED_RETURN );
	PrecacheScriptSound( FLAG_RED_DROPPED );

	PrecacheScriptSound( FLAG_BLUE_TAKEN );
	PrecacheScriptSound( FLAG_BLUE_SCORE );
	PrecacheScriptSound( FLAG_BLUE_RETURN );
	PrecacheScriptSound( FLAG_BLUE_DROPPED );
}

void CFlagClassic::Spawn( void )
{
	Precache ();

	AddSpawnFlags( SF_TRIG_TOUCH_DEBRIS | SF_TRIGGER_ALLOW_ALL );

	SetModel( FLAG_MODEL_CLASSIC );
	SetThink( &CFlagClassic::Think );
	SetNextThink( gpGlobals->curtime + 0.1f );

	BaseClass::Spawn();

	CreateBoxPhysics();
	CreateEffects();
}

void CFlagClassic::CreateEffects( void )
{
	byte Red (GetRenderColor().r);
	byte Green (GetRenderColor().g);
	byte Blue (GetRenderColor().b);

	int	nAttachment = LookupAttachment( "fuse" );

	// Start up the eye trail
	m_pGlowTrail = CSpriteTrail::SpriteTrailCreate( "sprites/bluelaser1.vmt", GetLocalOrigin(), false );

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

bool CFlagClassic::CreateNoPhysics ()
{
	SetSolid( SOLID_NONE );
	return true;
}

bool CFlagClassic::CreateBoxPhysics ()
{
	// Need to undo what CreateDropPhysics does
	SetMoveType( MOVETYPE_NONE );
	SetSolid( SOLID_BBOX );

	IPhysicsObject *pPhysicsObject = VPhysicsGetObject();
	if ( pPhysicsObject )
	{
		pPhysicsObject->EnableGravity( false );
		Vector velocity( 0.f, 0.f, 0.f );
		AngularImpulse impulse( 0.f, 0.f, 0.f );
		pPhysicsObject->SetVelocity( &velocity, &impulse );
	}
	
	AddSolidFlags( FSOLID_TRIGGER );
	SetCollisionGroup( COLLISION_GROUP_FLAG );
	CollisionProp()->UseTriggerBounds (true, 16);
	CollisionProp()->SetCollisionBounds (-1 * Vector( 16, 16, 16 ), Vector( 16, 16, 16 ));
	CollisionProp()->SetSurroundingBoundsType( USE_BEST_COLLISION_BOUNDS );

	return true;
}

void CFlagClassic::Think()
{
	if( m_bPlayerHasFlag )
	{
		// TODO: Fix
//		if ( m_pPlayerWithFlag->m_nButtons & IN_DROPFLAG )
//		{
//			m_pPlayerWithFlag->DropFlag();
//		}
	}
	else if ( m_bDropped )
	{
		// If return time is up, then return to spawn
		if ( gpGlobals->curtime >= m_TimeTillReturn )
		{
			ReturnToSpawn();
		}
	}

	SetNextThink( gpGlobals->curtime + 0.1f );
}

bool CFlagClassic::CreateDropPhysics ()
{
#if 0
	VPhysicsDestroyObject();
	CPhysicsProp::CreateVPhysics();
	
	SetSolid( SOLID_VPHYSICS );
	SetMoveType( MOVETYPE_VPHYSICS );

	SetAbsOrigin( GetAbsOrigin() + Vector( 0.0f, 0.0f, -50.0f));

	SetSolid( SOLID_BBOX );
	SetMoveType( MOVETYPE_FLYGRAVITY );
	SetCollisionGroup( COLLISION_GROUP_FLAG );
	SetFriction(4.0f);

	SetSolid( SOLID_OBB );
#endif

	IPhysicsObject *pPhysFlag = VPhysicsGetObject();

	if( IsCurrentlyTouching() )
	{
		PhysicsRemoveTouchedList( this );
	}

	const Vector& dropPos = GetAbsOrigin() + Vector( 0.0f, 0.0f, -50.0f);

	SetSolidFlags( FSOLID_NOT_SOLID );
	SetSolid( SOLID_BBOX );	
	SetParent( NULL );
	SetAbsOrigin( dropPos );

	pPhysFlag = VPhysicsInitNormal( SOLID_VPHYSICS, FSOLID_NOT_STANDABLE, true );
	if( pPhysFlag ) 
	{
		Vector av = pPhysFlag->GetInertia();
		pPhysFlag->SetPosition( dropPos, GetAbsAngles(), true);
	}

	pPhysFlag->EnableMotion( true );
	pPhysFlag->EnableCollisions( true );
	pPhysFlag->EnableGravity( true );

	SetMoveType( MOVETYPE_VPHYSICS );
	AddSolidFlags( FSOLID_TRIGGER );

	SetCollisionGroup( COLLISION_GROUP_FLAG );

	VPhysicsGetObject()->Wake();

#if 0
	//Tony; if we're dropped, don't reset to none!!
	SetCollisionGroup( COLLISION_GROUP_FLAG );
	RemoveSolidFlags( FSOLID_NOT_SOLID );
	AddSolidFlags( FSOLID_TRIGGER );
	CollisionProp()->UseTriggerBounds( true, 16 );
	IPhysicsObject *pPhysicsObject = VPhysicsGetObject();
	pPhysicsObject = VPhysicsInitNormal( SOLID_VPHYSICS, FSOLID_NOT_STANDABLE, true );
	pPhysicsObject->EnableGravity( true );
	pPhysicsObject->EnableCollisions( true );
	
	Vector vecDown( 0.f, 0.f, 100.0f );
	pPhysicsObject->AddVelocity( &vecDown, NULL );

	AddSolidFlags( FSOLID_TRIGGER );
	SetCollisionGroup( COLLISION_GROUP_FLAG );
	//CollisionProp()->UseTriggerBounds( true, 16 );

	pPhysicsObject->Wake();
#endif

	return true;
}

// Do not call directly, use player GiveFlag(CBaseEntity *pFlag) instead
void CFlagClassic::Pickup( CModPlayer *pPlayer )
{
	FollowEntity( pPlayer );
	SetLocalOrigin( Vector( 0, 0, 100 ) );
	//CreateNoPhysics();
	//SetMoveType( MOVETYPE_NOCLIP );

	BaseClass::Pickup( pPlayer );
	m_bFirstTake = true;

	switch( GetTeamNumber() )
	{
		case SDK_TEAM_BLUE:
			PlaySound( FLAG_BLUE_TAKEN );
			break;

		case SDK_TEAM_RED:
			PlaySound( FLAG_RED_TAKEN );
			break;
	}

	SetNextThink( gpGlobals->curtime + 0.1f );
}

void CFlagClassic::Drop()
{
	FollowEntity( NULL );
	// Use some trig to drop flag
	CreateDropPhysics();
	
	BaseClass::Drop();

	switch( GetTeamNumber() )
	{
		case SDK_TEAM_BLUE:
			PlaySound( FLAG_BLUE_DROPPED );
			break;

		case SDK_TEAM_RED:
			PlaySound( FLAG_RED_DROPPED );
			break;
	}

}

// Do not call directly if the player has the flag (use SDKPlayer->ReturnFlag())
void CFlagClassic::ReturnToSpawn( bool scored )
{
	CreateBoxPhysics ();

	BaseClass::ReturnToSpawn();
	m_bFirstTake = false;

	if ( !scored )
	{
		switch( GetTeamNumber() )
		{
			case SDK_TEAM_BLUE:
				PlaySound( FLAG_BLUE_RETURN );
				break;

			case SDK_TEAM_RED:
				PlaySound( FLAG_RED_RETURN );
				break;
		}
	}
}

void CFlagClassic::OnScore()
{
	CTeam *pTeam = GetGlobalTeam( ( GetTeamNumber() == SDK_TEAM_RED ) ? SDK_TEAM_BLUE : SDK_TEAM_RED );
	if ( !pTeam )
	{
		Error( "Flag has no team" );
	}

	m_pPlayerWithFlag->AddPoints( FLAG_CLASSIC_PLAYER_CAPTURE_POINTS, false );
	pTeam->AddCapturePoints( FLAG_CLASSIC_CAPTURE_POINTS );

	Msg ( "Score %d\n", pTeam->GetCapturePoints() );

	switch( GetTeamNumber() )
	{
		case SDK_TEAM_BLUE:
			PlaySound( FLAG_RED_SCORE );
			break;

		case SDK_TEAM_RED:
			PlaySound( FLAG_BLUE_SCORE );
			break;
	}
}

void CFlagClassic::OnPickup()
{
	if ( !lf_reverse_flags.GetBool() && !m_pPlayerWithFlag->HasTouched() && !m_bFirstTake )
	{
		m_pPlayerWithFlag->AddPoints( FLAG_CLASSIC_TOUCH_POINTS, false );
		GetGlobalTeam( m_pPlayerWithFlag->GetTeamNumber() )->AddCapturePoints( FLAG_CLASSIC_TOUCH_POINTS );
	}

	m_pPlayerWithFlag->SetTouched( true );

	BaseClass::OnPickup();
}
