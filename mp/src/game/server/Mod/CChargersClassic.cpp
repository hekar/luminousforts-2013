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

	Wall mounted chargers found in HL2MP

	Last Updated Dec 08, 2009
===============================================================*/
#include "cbase.h"
#if 0
#include "hl2mp_player.h"
#include "team.h"
#include "Mod/CArmorCharger.h"
#include "Mod/HealthChargers.h"
#include "Mod/CChargersClassic.h"
#include "Mod/ClassicGameRules.h"

LINK_ENTITY_TO_CLASS( lfc_item_suitcharger, CHealthRechargerClassic );
#if 0
LINK_ENTITY_TO_CLASS( lfc_item_suitcharger, CArmorRechargerClassic );

//
//	Name: CArmorRechargerClassic
//	Author: Hekar Khani
//	Description: Wall mounted armor recharger
//	Notes:
//
CArmorRechargerClassic::CArmorRechargerClassic()
{
}

void CArmorRechargerClassic::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	// if it's not a player, ignore
	if ( !pActivator || !pActivator->IsPlayer())
	{
		return;
	}
	else if (GameRules()->GetGameMode() == GAMEMODE_CLASSIC)
	{
		if (ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD)
		{
			if (m_flSoundTime <= gpGlobals->curtime)
			{
				m_flSoundTime = gpGlobals->curtime + 0.62;
				EmitSound( "SuitRecharge.Deny" );
			}

			return;
		}
		else if (GetTeamNumber() != pActivator->GetTeamNumber() &&
			GetTeamNumber() != TEAM_SPECTATOR)
		{
			if (m_flSoundTime <= gpGlobals->curtime)
			{
				CBasePlayer *pPlayer = ToBasePlayer( pActivator );
				Assert( pPlayer );
				ClientPrint( pPlayer, HUD_PRINTTALK, "#lf_armor_charger_deny" );

				m_flSoundTime = gpGlobals->curtime + 0.62;
				EmitSound( "SuitRecharge.Deny" );
			}

			return;
		}
	}

	CHL2MP_Player *pPlayer = ToSDKPlayer( pCaller );
	AssertMsg( pPlayer, "Failure SDKPlayer Armor charger\n" );
	CTeam *pTeam = GetGlobalSDKTeam( pPlayer->GetTeamNumber() );
	const CHL2MP_PlayerClassInfo &pClassInfo = pTeam->GetPlayerClassInfo( pPlayer->m_Shared.PlayerClass() );

	SetMaxArmor( pClassInfo.m_iArmor );

	BaseClass::Use( pActivator, pCaller, useType, value );
}
#endif // 0

LINK_ENTITY_TO_CLASS( lfc_item_healthcharger, CHealthRechargerClassic );

//
//	Name: CHealthRechargerClassic
//	Author: Hekar Khani
//	Description: Wall mounted health recharger
//	Notes:
//
CHealthRechargerClassic::CHealthRechargerClassic()
{
}

void CHealthRechargerClassic::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	if (!pActivator || !pActivator->IsPlayer())
	{
		return;
	}
	else if (GameRules()->GetGameMode() == GAMEMODE_CLASSIC)
	{
		if (ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD)
		{
			if (m_flSoundTime <= gpGlobals->curtime)
			{
				m_flSoundTime = gpGlobals->curtime + 0.62;
				EmitSound( "WallHealth.Deny" );
			}
			return;
		}
		else if (GetTeamNumber() != pActivator->GetTeamNumber() &&
			GetTeamNumber() != TEAM_SPECTATOR)
		{
			if (m_flSoundTime <= gpGlobals->curtime)
			{
				CBasePlayer *pPlayer = ToBasePlayer( pActivator );
				Assert( pPlayer );
				ClientPrint( pPlayer, HUD_PRINTTALK, "#lf_health_charger_deny" );

				m_flSoundTime = gpGlobals->curtime + 0.62;
				EmitSound( "WallHealth.Deny" );
			}
			return;
		}
	}

	BaseClass::Use( pActivator, pCaller, useType, value );
}

#endif // 0
