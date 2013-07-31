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
	Client

	Miscellaneous concommands for the client

	Last Updated March 1, 2010
===============================================================*/
#include "cbase.h"
#ifdef MOD_BETA
#include "c_baseentity.h"
#include "c_hl2mp_player.h"
#include "Mod/C_BlockBase.h"
#include "Mod/ClassicGameRules.h"
#include "Mod/ClientCommands.h"

#define PRINT_IS_GAMEMODE( gamemode ) \
	if ( GameRules()->GetGameMode() & gamemode ) \
		Msg( "Current GameMode: %s\n", #gamemode );

void CC_PrintGameMode( const CCommand& args )
{
	PRINT_IS_GAMEMODE( GAMEMODE_SDKBASE );
	PRINT_IS_GAMEMODE( GAMEMODE_CLASSIC );
	PRINT_IS_GAMEMODE( GAMEMODE_SF132 );
}

static ConCommand mdc_print_gamemode( "mdc_print_gamemode", CC_PrintGameMode, "prints current gamemode (client)" );

#define PRINT_IS_PHASE( phase ) \
	if ( ClassicGameRules()->GetCurrentPhaseID() == phase ) \
		Msg( "Current Phase: %s\n", #phase );

void CC_PrintPhase( const CCommand& args )
{
	PRINT_IS_PHASE( PHASE_BUILD );
	PRINT_IS_PHASE( PHASE_COMBAT );

	int curphase = ClassicGameRules()->GetCurrentPhaseID();
	if ( curphase != PHASE_BUILD || 
		curphase != PHASE_COMBAT)
	{
		Msg( "Current Phase: %d\n", curphase );
	}
}

static ConCommand mdc_print_phase( "mdc_print_phase", CC_PrintPhase, "prints current phase (client)" );

#endif // MOD_DEV

// CODE DUPLICATION BELOW. FIXME!
C_BaseEntity *ProcessTarget()
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();

	if ( !pPlayer )
		return NULL;

	// Get our target's ent index
	int iEntIndex = pPlayer->GetIDTarget();

	// Didn't find one?
	if ( !iEntIndex )
	{
		return NULL;
	}

	if ( iEntIndex )
	{
		// set up the vectors and traceline
		trace_t tr;
		Vector	vecStart, vecStop, vecDir;

		// get the angles
		AngleVectors( pPlayer->EyeAngles( ), &vecDir );

		// get the vectors
		vecStart = pPlayer->EyePosition();
		vecStop = vecStart + vecDir * 1024;

		// do the traceline
		UTIL_TraceLine( vecStart, vecStop, MASK_ALL, pPlayer, COLLISION_GROUP_NPC, &tr );

		C_BaseEntity *pEntity = tr.m_pEnt;

		// check to see if we found a block
		if ( !pEntity )
			return NULL;

		return pEntity;
	}

	return NULL;
}

void CC_FreezeStatus( const CCommand& args )
{
	C_BaseEntity *pEntity = ProcessTarget();

	// Did we find a target?
	if ( pEntity )
	{
		// Is it a block?
		if ( pEntity->IsBlock() && pEntity->GetHealth() > 0 )
		{
			C_BlockBase *pBlock = static_cast< C_BlockBase * > ( pEntity );

			if ( ClassicGameRules() )
			{
				int frozenplayerindex = engine->GetPlayerForUserID( pBlock->GetLastFrozenByUserID()  );
				C_BasePlayer *pPlayer = UTIL_PlayerByIndex( frozenplayerindex );
				if ( pPlayer )
				{
					Msg( "\tFrozen by %s\n", pPlayer->GetPlayerName() );
				}
				else
				{
					Msg( "\tFrozen by WORLD\n" );
				}
				
				int unfrozenplayerindex = engine->GetPlayerForUserID( pBlock->GetLastUnFrozenByUserID()  );
				pPlayer = UTIL_PlayerByIndex( unfrozenplayerindex );
				if ( pPlayer )
				{
					Msg( "\tUnFrozen by %s\n", pPlayer->GetPlayerName() );
				}
				else
				{
					Msg( "\tUnFrozen by WORLD\n" );
				}
			}
		}
	}
}

static ConCommand lf_freezestatus_print( "lf_freezestatus_print", CC_FreezeStatus, "prints freezestatus of block being pointed at" );

