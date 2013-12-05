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
#include "Mod/ClassicGameRules.h"
#include "Mod/ClassicCommands.h"

#ifndef CLIENT_DLL
#include "Mod/CModPlayer.h"
#endif // !CLIENT_DLL

#ifdef CLIENT_DLL
void CC_PrintCurrentPhase( const CCommand& args )
{
	if ( GameRules()->GetGameModeMask() != GAMEMODE_CLASSIC )
	{
		Msg( "Not in classic gamemode\n" );
		return;
	}

	int phase = ClassicGameRules()->GetCurrentPhaseID();
	
	Msg( "Client: " );

	if ( phase == PHASE_BUILD )
	{
		Msg( "It is the Build Phase\n" );
	}
	else if ( phase == PHASE_COMBAT )
	{
		Msg( "It is the Combat Phase\n" );
	}
	else
	{
		Msg( "Bad Phase %d\n", phase );
	}
}

static ConCommand mdc_print_phase( "mdc_print_phase", CC_PrintCurrentPhase, "print current phase (client)" );


#else
// SERVER DLL COMMANDS

// Toggle phase console command
void CC_TogglePhase( const CCommand& args )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() )
		return;

	// Needs to be classic or a child of the classic gamerules
	if ( GameRules()->GetGameModeMask() & GAMEMODE_CLASSIC )
	{
		bool BuildPhase = ClassicGameRules()->IsBuildPhase();
		if (BuildPhase)
		{
			ClassicGameRules()->SwitchToPhase( "CombatPhase" );
		}
		else
		{
			ClassicGameRules()->SwitchToPhase( "BuildPhase" );
		}
	}
	else
	{
		ClientPrint( UTIL_GetCommandClient(), HUD_PRINTCONSOLE, "Cannot Execute command. Not in Classic GameRules.\n");
	}
}
static ConCommand lf_togglephase( "lf_togglephase", CC_TogglePhase, "Goes to next phase" );
static ConCommand togglephase( "togglephase", CC_TogglePhase, "Sourceforts: Goes to next phase" );

int AddTimeSeconds( int seconds )
{
	if ( GameRules()->GetGameModeMask() & GAMEMODE_CLASSIC )
	{
		int DiffTime = seconds;
		ClassicGameRules()->SetPhaseTime( ClassicGameRules()->GetPhaseTimeLeft() + DiffTime );
	}
	else
	{
		return 0;
		ClientPrint( UTIL_GetCommandClient(), HUD_PRINTCONSOLE, "Cannot Execute command. Not in Classic GameRules.\n");
	}

	return 1;
}

// Add time console command
void CC_AddTime( const CCommand& args )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() && args.ArgC() < 2 )
	{
		Msg ( "Invalid Usage: AddTime <Integer>\n" );
		return;
	}

	AddTimeSeconds( atoi( args.Arg( 1 ) ) );
}

static ConCommand lf_addtime( "lf_addtime", CC_AddTime, "Addtime <Integer> to current phase" );
static ConCommand addtime( "addtime", CC_AddTime, "Sourceforts: Addtime <Integer> to Current Phase" );

// Sub time console command
void CC_SubTime( const CCommand& args )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() && args.ArgC() < 2 )
	{
		Msg ( "Invalid Usage: SubTime <Integer>\n" );
		return;
	}

	AddTimeSeconds( -atoi( args.Arg( 1 ) ) );
}

static ConCommand lf_subtime( "lf_subtime", CC_SubTime, "Subtime <Integer> from current phase" );

void CC_AddMinute( const CCommand& args )
{
	if ( !UTIL_IsCommandIssuedByServerAdmin() && args.ArgC() < 2 )
	{
		Msg ( "Invalid Usage: SubTime <Integer>\n" );
		return;
	}

	AddTimeSeconds( atoi( args.Arg( 1 ) ) * 60 );
}

static ConCommand lf_addminute( "lf_addminute", CC_AddMinute, "Add minute(s) to current phase" );

// Print the current phase
void CC_PrintCurrentPhase( const CCommand& args )
{
	if ( GameRules()->GetGameMode() != GAMEMODE_CLASSIC )
	{
		Msg( "Not in classic gamemode\n" );
		return;
	}

	int phase = ClassicGameRules()->GetCurrentPhaseID();
	
	Msg( "Server: " );
	
	if ( phase == PHASE_BUILD )
	{
		Msg( "It is the Build Phase\n" );
	}
	else if ( phase == PHASE_COMBAT )
	{
		Msg( "It is the Combat Phase\n" );
	}
	else
	{
		Msg( "Bad Phase %d\n", phase );
	}
}

static ConCommand mds_print_phase( "mds_print_phase", CC_PrintCurrentPhase, "print current phase (server)" );

//
//	Name: CC_SwitchToPhyscannon
//	Author: Hekar Khani
//	Description: Concommand. Switches between Physcannon and last weapon
//	Notes:
//
void CC_SwitchToPhyscannon( void )
{
	CBasePlayer *pPlayer = ToBasePlayer( UTIL_GetCommandClient() );
	
	if ( pPlayer )
	{
		CBaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();

		if ( pWeapon )
		{
			// Tell the client to stop selecting weapons
			engine->ClientCommand( UTIL_GetCommandClient()->edict(), "cancelselect" );

			const char *strWeaponName = pWeapon->GetName();

			if ( !Q_stricmp( strWeaponName, "weapon_lf_combat_cannon" ) )
			{
				pPlayer->SelectLastItem();
			}
			else
			{
				pPlayer->SelectItem( "weapon_lf_combat_cannon" );
			}
		}
	}
}

static ConCommand lf_switch_to_grav_gun( "lf_switch_to_grav_gun", CC_SwitchToPhyscannon, "Quick Swap to gravity gun" );
static ConCommand phys_swap( "phys_swap", CC_SwitchToPhyscannon, "Quick Swap to gravity gun" );

void CC_ReturnToSpawn( const CCommand& args )
{
	if ( GameRules()->GetGameModeMask() & GAMEMODE_CLASSIC )
	{
		if ( ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD )
		{
			CBasePlayer *pPlayer = UTIL_GetCommandClient();
			if ( !pPlayer )
			{
				return;
			}

			pPlayer->ForceRespawn();
		}
	}
}

static ConCommand lf_return_to_spawn( "lf_return_to_spawn", CC_ReturnToSpawn, "Teleport to spawn" );


//void CC_MotdClosed( const CCommand& args )
//{
//	CModPlayer *pPlayer = ToModPlayer( UTIL_GetCommandClient() );
//
//	ModDebugMsg( pPlayer->GetPlayerName() );
//}
//
//static ConCommand mod_motd_closed( "mod_motd_closed", CC_MotdClosed, "The motd dialog has been closed by a player" );

#endif // CLIENT_DLL
