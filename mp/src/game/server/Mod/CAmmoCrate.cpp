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
	Server
	Model Entity

	Taken from HL2DM ammo crate code.

	Last Updated Sept 15 2009
===============================================================*/

#include "cbase.h"
#include "CAmmoCrate.h"
#include "player.h"
#include "gamerules.h"
#include "items.h"
#include "ammodef.h"
#include "eventlist.h"
#include "npcevent.h"
#include "team.h"
#include "playerclass_info_parse.h"

#include "CModPlayer.h"
#include "Mod/ClassicGameRules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#define AMMO_CRATE_MODELNAME "models/crate/crate.mdl"

//---------------------------------------------------------
// Applies ammo quantity scale.
//---------------------------------------------------------
int GivePlayerAmmo( CBasePlayer *pPlayer, float flCount, const char *pszAmmoName, bool bSuppressSound = false )
{
	if (!strcmp (pszAmmoName, ""))
		return 0;

	int iAmmoType = GetAmmoDef()->Index(pszAmmoName);
	if (iAmmoType == -1)
	{
		Msg("ERROR: Attempting to give unknown ammo type (%s)\n", pszAmmoName);
		return 0;
	}

	flCount *= g_pGameRules->GetAmmoQuantityScale( iAmmoType );

	// Don't give out less than 1 of anything.
	flCount = max( 1.0f, flCount );

	return pPlayer->GiveAmmo( flCount, iAmmoType, bSuppressSound );
}

BEGIN_DATADESC( CAmmoCrate )

	DEFINE_FIELD( m_flCloseTime, FIELD_FLOAT ),
	DEFINE_FIELD( m_hActivator, FIELD_EHANDLE ),

	DEFINE_OUTPUT( m_OnUsed, "OnUsed" ),

	DEFINE_INPUTFUNC( FIELD_VOID, "Kill", InputKill ),

	DEFINE_THINKFUNC( CrateThink ),

END_DATADESC()

#define	AMMO_CRATE_CLOSE_DELAY	1.5f

void CAmmoCrate::Spawn( void )
{
	Precache();

	BaseClass::Spawn();

	SetModel( AMMO_CRATE_MODELNAME );
	SetMoveType( MOVETYPE_NONE );
	SetSolid( SOLID_VPHYSICS );
	CreateVPhysics();

	ResetSequence( LookupSequence( "Idle" ) );
	SetBodygroup( 1, true );

	m_flCloseTime = gpGlobals->curtime;
	m_flAnimTime = gpGlobals->curtime;
	m_flPlaybackRate = 0.0;
	SetCycle( 0 );

	if ( GetTeamNumber() == TEAM_BLUE )
	{
		m_nSkin = 1;
	}
	else if ( GetTeamNumber() == TEAM_RED )
	{
		m_nSkin = 2;
	}
	else
	{
		m_nSkin = 0;
	}

	if ( ClassicGameRules()->IsSourcefortsMap() )
	{
		SetAbsAngles( QAngle( GetAbsAngles().x, GetAbsAngles().y - 90, GetAbsAngles().z ) );
	}

	m_takedamage = DAMAGE_NO;
}

bool CAmmoCrate::CreateVPhysics( void )
{
	return ( VPhysicsInitStatic() != NULL );
}

void CAmmoCrate::Precache( void )
{
	PrecacheModel( AMMO_CRATE_MODELNAME );

	PrecacheScriptSound( "AmmoCrate.Open" );
	PrecacheScriptSound( "AmmoCrate.Close" );
	PrecacheScriptSound( "Player.UseDeny" );
}

void CAmmoCrate::OnRestore( void )
{
	BaseClass::OnRestore();
}

// +Use
void CAmmoCrate::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	CBasePlayer *pPlayer = ToBasePlayer( pActivator );

	if ( pPlayer == NULL )
		return;

	m_OnUsed.FireOutput( pActivator, this );

	int iSequence = LookupSequence( "Open" );

	// See if we're not opening already
	if ( GetSequence() != iSequence )
	{
		Vector mins, maxs;
		trace_t tr;

		CollisionProp()->WorldSpaceAABB( &mins, &maxs );

		Vector vOrigin = GetAbsOrigin();
		vOrigin.z += ( maxs.z - mins.z );
		mins = (mins - GetAbsOrigin()) * 0.2f;
		maxs = (maxs - GetAbsOrigin()) * 0.2f;
		mins.z = ( GetAbsOrigin().z - vOrigin.z );  
		
		UTIL_TraceHull( vOrigin, vOrigin, mins, maxs, MASK_SOLID, this, COLLISION_GROUP_NONE, &tr );

		if ( tr.startsolid || tr.allsolid )
			 return;
			
		m_hActivator = pPlayer;

		// Animate!
		ResetSequence( iSequence );

		CPASAttenuationFilter sndFilter( this, "AmmoCrate.Open" );
		EmitSound( sndFilter, entindex(), "AmmoCrate.Open" );

		SetThink( &CAmmoCrate::CrateThink );
		SetNextThink( gpGlobals->curtime + 0.1f );
	}

	m_flCloseTime = gpGlobals->curtime + AMMO_CRATE_CLOSE_DELAY;
}

void CAmmoCrate::GiveAmmo( CBasePlayer *pBasePlayer )
{
	CModPlayer *pPlayer = ToModPlayer( pBasePlayer );

	int team = pPlayer->GetTeamNumber();
	CTeam *pTeam = GetGlobalTeam( team );

	int playerclass = pPlayer->GetPlayerClass();
	const FilePlayerClassInfo_t &pClassInfo = pTeam->GetPlayerClassInfo( playerclass );

	for (int i = 0; i < pClassInfo.m_iWeaponCount; i++)
	{
		// Weapon is empty, skip loop
		if (pClassInfo.m_WeaponVector [i] == WEAPON_NONE)
			continue;

		CBaseCombatWeapon *pWeapon = (CBaseCombatWeapon *)pPlayer->GetWeapon( i );

		// No weapon? Give it to them
		if ( !pWeapon )
		{
			char buf[64];
			Q_snprintf( buf, sizeof( buf ), "weapon_%s", WeaponIDToAlias( pClassInfo.m_WeaponVector [i] ) );
			pWeapon = dynamic_cast< CBaseCombatWeapon * > (pPlayer->GiveNamedItem( buf ));
		}

		// Do not restock the slam
		if ( !Q_strcmp( pWeapon->GetClassname(), "weapon_hl2_slam" ) )
		{
			continue;
		}

		const char *AmmoName = pWeapon->GetWpnData().szAmmo1;
		int AmmoIndex = GetAmmoDef()->Index( AmmoName );

		int iMaxAmmo = pClassInfo.m_AmmoVector[ i ];

		int iAmmoCount = pPlayer->GetAmmoCount( AmmoIndex );
		if (iAmmoCount < iMaxAmmo)
		{
			int AmmoToGive = iMaxAmmo - iAmmoCount;
			pPlayer->GiveAmmo( AmmoToGive, AmmoIndex, false );
		}
	}
}

//-----------------------------------------------------------------------------
// Purpose: Catches the monster-specific messages that occur when tagged
//			animation frames are played.
// Input  : *pEvent - 
//-----------------------------------------------------------------------------
void CAmmoCrate::HandleAnimEvent( animevent_t *pEvent )
{
	if ( pEvent->event == AE_AMMOCRATE_PICKUP_AMMO )
	{
		if ( m_hActivator )
		{
			CBasePlayer *pPlayer = m_hActivator.Get ();
			GiveAmmo (pPlayer);
			//SetBodygroup( 1, false );
			m_hActivator = NULL;
		}
		return;
	}

	BaseClass::HandleAnimEvent( pEvent );
}

void CAmmoCrate::CrateThink( void )
{
	StudioFrameAdvance();
	DispatchAnimEvents( this );

	SetNextThink( gpGlobals->curtime + 0.1f );

	// Start closing if we're not already
	if ( GetSequence() != LookupSequence( "Close" ) )
	{
		// Not ready to close?
		if ( m_flCloseTime <= gpGlobals->curtime )
		{
			m_hActivator = NULL;

			ResetSequence( LookupSequence( "Close" ) );
		}
	}
	else
	{
		// See if we're fully closed
		if ( IsSequenceFinished() )
		{
			// Stop thinking
			SetThink( NULL );
			CPASAttenuationFilter sndFilter( this, "AmmoCrate.Close" );
			EmitSound( sndFilter, entindex(), "AmmoCrate.Close" );

			// FIXME: We're resetting the sequence here
			// but setting Think to NULL will cause this to never have
			// StudioFrameAdvance called. What are the consequences of that?
			ResetSequence( LookupSequence( "Idle" ) );
			SetBodygroup( 1, true );
		}
	}
}

void CAmmoCrate::InputKill( inputdata_t &data )
{
	UTIL_Remove( this );
}
