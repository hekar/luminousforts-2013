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


#ifndef CUSTOMGAMERULES_H
#define CUSTOMGAMERULES_H

#ifdef _WIN32
#pragma once
#endif

#include "hl2mp_gamerules.h"
#include "Mod/CustomCvars.h"

#ifndef CLIENT_DLL
	#include "Mod/PhaseControl.h"
	#include "Mod/ClassicPhases.h"
#endif

#define CUSTOM_GAME_DESCRIPTION_BUILD		"Custom Build"
#define CUSTOM_GAME_DESCRIPTION_COMBAT		"Custom Combat"

#define CUSTOM_GAME_CLASSCOUNT 5


#ifdef CLIENT_DLL
	#define CCustomGameRules C_CustomGameRules
	#define CCustomGameRulesProxy C_CustomGameRulesProxy
#endif

class CCustomGameRulesProxy : public CGameRulesProxy
{
public:
	DECLARE_CLASS( CCustomGameRulesProxy, CGameRulesProxy );
	DECLARE_NETWORKCLASS();
};

class CCustomViewVectors : public CViewVectors
{
public:
	CCustomViewVectors( 
		Vector vView,
		Vector vHullMin,
		Vector vHullMax,
		Vector vDuckHullMin,
		Vector vDuckHullMax,
		Vector vDuckView,
		Vector vObsHullMin,
		Vector vObsHullMax,
		Vector vDeadViewHeight
		) :
			CViewVectors( 
				vView,
				vHullMin,
				vHullMax,
				vDuckHullMin,
				vDuckHullMax,
				vDuckView,
				vObsHullMin,
				vObsHullMax,
				vDeadViewHeight )
	{
	}
};

class CCustomGameRules : public CHL2MPRules
{
public:
	DECLARE_CLASS( CCustomGameRules, CHL2MPRules );

	// Get the view vectors for this mod.
	const CViewVectors* GetViewVectors() const;
	const CCustomViewVectors *GetCustomViewVectors() const;

	bool Init( void );

	int GetGameMode( void ) { return GAMEMODE_CLASSIC; }
	int GetGameModeMask( void ) { return GAMEMODE_CLASSIC; }

#ifdef CLIENT_DLL

	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.
	// Temporary Hack - HekarHack
	void SetCurrentPhase( int PhaseID ) { m_iCurrentPhaseID = PhaseID; }

#else

	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.

	CCustomGameRules();
	~CCustomGameRules();

	void LevelInitPostEntity();

	float FlPlayerFallDamage( CBasePlayer *pPlayer );
	const char *GetGameDescription( void );
	bool IsAllowedToSpawn( CBaseEntity *pEntity );
	void Think();

	void CheckWinningTeam();
	void SetWinningTeam( int teamnum );

	void SwitchToPhase( const char *PhaseName, bool respawn = true );

	bool HasBuildPhase() { return lfcus_phase_nobuild.GetBool(); }
	bool HasCombatPhase() { return lfcus_phase_nocombat.GetBool(); }

	bool IsBuildPhase() { return m_PhaseManager.GetCurrentPhaseID() == PHASE_BUILD; }
	bool IsCombatPhase() { return m_PhaseManager.GetCurrentPhaseID() == PHASE_COMBAT; }
	int GetCurrentPhaseID () { return m_iCurrentPhaseID; }

	float GetPhaseTime() { return m_PhaseManager.GetCurrentPhase()->GetPhaseTime(); }
	void SetPhaseTime( float PhaseTime );

	bool GivePlayerClassAttributes( CHL2MP_Player *pPlayer );
	void GivePlayerWeapons( CHL2MP_Player *pPlayer );
	void GiveBuildPhaseItems( CHL2MP_Player *pPlayer );
	void PlayerSpawn( CBasePlayer *pBasePlayer );

	char **GetClassNames( int TeamNum );
	int GetClassCount( void ) { return CUSTOM_GAME_CLASSCOUNT; }
	void GetTaggedConVarList( KeyValues *pCvarTagList );

private:
	void GoToIntermission();
	void TeamsDraw();
	void TeamWins( int TeamID );
public:

#endif // #ifndef CLIENT_DLL

#ifdef CLIENT_DLL
	int m_iCurrentPhaseID;
#else
	float m_flPhaseTimeLeft;
	//CNetworkVar( int, m_iCurrentPhaseID );
	int m_iCurrentPhaseID;

private:
	bool m_bLastRoundOver;
	int m_iBuildRounds;
	int m_iCombatRounds;

	int m_iWinningTeam;

	CBuildPhase *m_BuildPhase;
	CCombatPhase *m_CombatPhase;
	CPhaseManager m_PhaseManager;
	float m_flPhaseLength;
	CountdownTimer m_PhaseTimer; // Countdown timer?
#endif // CLIENT_DLL
};


//-----------------------------------------------------------------------------
// Gets us at the game rules
//-----------------------------------------------------------------------------

inline CCustomGameRules* CustomGameRules()
{
	return static_cast< CCustomGameRules* >( g_pGameRules );
}


#endif // CUSTOMGAMERULES_H
