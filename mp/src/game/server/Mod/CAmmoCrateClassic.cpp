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

	Based on HL2DM ammo crate code.

	Last Updated Dec 28, 2009
===============================================================*/

#include "cbase.h"
#include "player.h"
#include "gamerules.h"
#include "items.h"
#include "ammodef.h"
#include "eventlist.h"
#include "npcevent.h"

#include "team.h"
#include "gamerules.h"
#include "playerclass_info_parse.h"

#include "CAmmoCrate.h"
#include "CAmmoCrateClassic.h"
#include "CModPlayer.h"
#include "Mod/ClassicGameRules.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

LINK_ENTITY_TO_CLASS( lfc_item_ammo_crate, CAmmoCrateClassic );

BEGIN_DATADESC( CAmmoCrateClassic )

	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "team" ),
	DEFINE_KEYFIELD( m_iSourcefortsTeam, FIELD_INTEGER, "TeamNum" ),

END_DATADESC()


void CAmmoCrateClassic::Spawn()
{
	if ( ClassicGameRules()->IsSourcefortsMap() )
	{
		ChangeTeam( m_iSourcefortsTeam );
	}

	BaseClass::Spawn();
}

void CAmmoCrateClassic::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{
	CBasePlayer *pPlayer = ToBasePlayer( pActivator );

	if ( pPlayer == NULL )
		return;


	if ( !pActivator || !pActivator->IsPlayer())
	{
		return;
	}
	else if (GameRules()->GetGameMode() == GAMEMODE_CLASSIC)
	{
		if (ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD)
		{
			CPASAttenuationFilter sndFilter( this, "Player.UseDeny" );
			EmitSound( sndFilter, entindex(), "Player.UseDeny" );

			return;
		}
		else if (GetTeamNumber() != pActivator->GetTeamNumber() &&
			GetTeamNumber() != TEAM_SPECTATOR)
		{
			ClientPrint( pPlayer, HUD_PRINTTALK, "#lf_ammo_crate_deny" );

			CPASAttenuationFilter sndFilter( this, "Player.UseDeny" );
			EmitSound( sndFilter, entindex(), "Player.UseDeny" );

			return;
		}
	}

	BaseClass::Use( pActivator, pCaller, useType, value );
}

