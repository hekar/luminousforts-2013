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
#include "ammodef.h"
#include "hl2mp_player.h"
#include "team.h"
#include "Mod/CDispenserBase.h"


ConVar lf_dispenser_radius( "lf_dispenser_radius", "10.f", FCVAR_ARCHIVE );
BALANCE_DEFINE( lfm_dispenser_thinktime, 1 );
BALANCE_DEFINE( lfm_dispenser_health_rate, 20 );

#define DISPENSER_MODELNAME "models/crate/crate.mdl"

BEGIN_DATADESC( CDispenserBase )
	DEFINE_KEYFIELD( m_bEnabled, FIELD_BOOLEAN, "Enabled" ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable ),
	DEFINE_OUTPUT( m_OnDispense, "OnDispense" ),
END_DATADESC()

CDispenserBase::CDispenserBase()
{
	SetModelName( MAKE_STRING( DISPENSER_MODELNAME ) );
}

void CDispenserBase::Precache()
{
	PrecacheModel( GetModelName().ToCStr() );
	BaseClass::Precache();
}

void CDispenserBase::Spawn()
{
	Precache();

	BaseClass::Spawn();

	SetNextThink( BALANCE_VALUE( Float, lfm_dispenser_thinktime ) );
	SetThink( &CDispenserBase::Think );
}

void CDispenserBase::InputDisable( inputdata_t& data )
{
	m_bEnabled = false;
}

void CDispenserBase::InputEnable( inputdata_t& data )
{
	m_bEnabled = true;
}

void CDispenserBase::Think()
{
	if ( m_bEnabled )
	{
		StockSurroundingPlayers();
	}

	SetNextThink( BALANCE_VALUE( Float, lfm_dispenser_thinktime ) );
}

ProtectedMethod void CDispenserBase::StockSurroundingPlayers()
{
	CEntitySphereQuery spherequery( GetAbsOrigin(), lf_dispenser_radius.GetFloat() );
	
	for( CBaseEntity *ent = spherequery.GetCurrentEntity(); ent != NULL; spherequery.NextEntity() )
	{
		if ( ent->IsPlayer() )
		{
			CBasePlayer *pPlayer = ToBasePlayer( ent );
			HealPlayer( pPlayer );
			StockPlayer( pPlayer );

			m_OnDispense.FireOutput( pPlayer, this, 0.0f );
		}
	}
}

ProtectedMethod void CDispenserBase::HealPlayer( CBasePlayer *pPlayer )
{
	int prevhealth = pPlayer->GetHealth();
	int newhealth = pPlayer->GetHealth() + BALANCE_VALUE( Int, lfm_dispenser_health_rate );
	if ( newhealth <= pPlayer->GetMaxHealth() )
	{
		pPlayer->SetHealth( newhealth );
		OnHealPlayer( pPlayer, newhealth - prevhealth );
	}
}

// shit code, omg
ProtectedMethod void CDispenserBase::StockPlayer( CBasePlayer *pPlayer )
{
#if 0
	CHL2MP_Player *hl2player = ToHL2MPPlayer( pPlayer );
	int team = hl2player->GetTeamNumber();
	CTeam *pTeam = GetGlobalTeam( team );

	int playerclass = hl2player->m_Shared.PlayerClass();
	const CHL2MP_PlayerClassInfo &pClassInfo = pTeam->GetPlayerClassInfo( playerclass );

	for (int i = 0; i < pClassInfo.m_iWeaponCount; i++)
	{
		// Weapon is empty, skip loop
		if ( pClassInfo.m_WeaponVector [i] == WEAPON_NONE )
			continue;

		CWeaponSDKBase *pWeapon = (CWeaponSDKBase *) hl2player->GetWeapon( i );
		// No weapon? Give it to them
		if ( !pWeapon )
		{
			char buf[64];
			Q_snprintf( buf, sizeof( buf ), "weapon_%s", WeaponIDToAlias( pClassInfo.m_WeaponVector [i] ) );
			pWeapon = dynamic_cast< CWeaponSDKBase * > ( hl2player->GiveNamedItem( buf ) );
		}

		const char *AmmoName = pWeapon->GetSDKWpnData().szAmmo1;
		int AmmoIndex = GetAmmoDef()->Index (AmmoName);

		int iClipSize = pWeapon->GetSDKWpnData().iMaxClip1;
		int iAmmoToGive = iClipSize;
		
		int iNumClip = pWeapon->GetSDKWpnData().m_iDefaultAmmoClips - 1; //account for one clip in the gun
		int iMaxGive = clamp (iNumClip, 1, 35) * iClipSize;

		if ( iMaxGive >= iAmmoToGive + hl2player->GetAmmoCount( AmmoIndex ) )
		{
			hl2player->GiveAmmo( iAmmoToGive, AmmoName );
			OnStockPlayer( hl2player );
		}
	}
#endif // 0
}
