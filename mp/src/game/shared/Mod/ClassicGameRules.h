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


#ifndef CLASSICGAMERULES_H
#define CLASSICGAMERULES_H

#ifdef _WIN32
#pragma once
#endif

#include "hl2mp_gamerules.h"
#ifndef CLIENT_DLL
	#include "Mod/PhaseControl.h"
	#include "Mod/ClassicPhases.h"
#else
	#define CClassicGameRules C_ClassicGameRules
#endif

class CModPlayer;

enum ClassicPlayerClasses
{
	CLASSIC_CLASS_SCOUT = 0,
	CLASSIC_CLASS_ENGINEER,
	CLASSIC_CLASS_SOLDIER,
	CLASSIC_CLASS_SNIPER,
	CLASSIC_CLASS_ROCKETEER,

	CLASSIC_CLASS_LAST
};

class CClassicGameRules : public CHL2MPRules
{
public:
	DECLARE_CLASS( CClassicGameRules, CHL2MPRules );

	CClassicGameRules();
	~CClassicGameRules();

	virtual bool Init( void );

	virtual int GetGameMode( void ) { return GAMEMODE_CLASSIC; }
	virtual int GetGameModeMask( void ) { return GAMEMODE_CLASSIC; }

#ifndef CLIENT_DLL

	virtual void LevelInitPostEntity();
	virtual void LevelShutdownPostEntity();

	virtual float FlPlayerFallDamage( CBasePlayer *pPlayer );
	virtual const char *GetGameDescription( void );
	virtual bool IsAllowedToSpawn( CBaseEntity *pEntity );
	virtual void Think();

	virtual bool IsPlayerClassOnTeam( int cls, int team );
	virtual int CountPlayerClass( int team, int cls );
	virtual const char *GetPlayerClassName( int cls, int team );

	virtual void CheckWinningTeam();

	float GetPhaseTime() { return m_PhaseManager.GetCurrentPhase()->GetPhaseTime(); }
	void SetPhaseTime( float PhaseTime );

	virtual void SwitchToPhase( const char *PhaseName, bool respawn = true );

	virtual void TeamsDraw();
	virtual void TeamWins( int TeamID );

	virtual void GoToIntermission();

	virtual bool GivePlayerClassAttributes( CModPlayer *pPlayer );
	virtual void GivePlayerWeapons( CModPlayer *pPlayer );
	virtual void GiveBuildPhaseItems( CModPlayer *pPlayer );
	virtual void PlayerSpawn( CBasePlayer *pBasePlayer );

	virtual void RespawnPlayers();

	virtual bool IsSourcefortsMap() { return false; }
	virtual void SetSourcefortsMap(bool t) { }

	virtual bool ClientConnected( edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen );
	virtual void ClientDisconnected( edict_t *pClient );
	virtual void PlayerKilled( CBasePlayer *pVictim, const CTakeDamageInfo &info );

	virtual const char *GetChatPrefix( bool bTeamOnly, CBasePlayer *pPlayer );
	virtual const char *GetChatFormat( bool bTeamOnly, CBasePlayer *pPlayer );

private:
	bool m_bLastRoundOver;
	int m_iBuildRounds;
	int m_iCombatRounds;
	int m_iBuildFallDamage;
	int m_iCombatFallDamage;

	CBuildPhase *m_BuildPhase;
	CCombatPhase *m_CombatPhase;
	CPhaseManager m_PhaseManager;
	float m_flPhaseLength;
	CountdownTimer m_PhaseTimer;
#endif // !CLIENT_DLL

public:
	virtual char **GetClassNames( int TeamNum );
	virtual int GetClassCount( void );
	virtual const char *GetClassmenuRed() const;
	virtual const char *GetClassmenuBlue() const;

#ifdef CLIENT_DLL
	virtual bool IsBuildPhase() { return m_iCurrentPhaseID == PHASE_BUILD; }
	virtual bool IsCombatPhase() { return m_iCurrentPhaseID == PHASE_COMBAT; }
#else
	virtual bool IsBuildPhase() { return m_PhaseManager.GetCurrentPhaseID() == PHASE_BUILD; }
	virtual bool IsCombatPhase() { return m_PhaseManager.GetCurrentPhaseID() == PHASE_COMBAT; }

	virtual float GetPhaseTimeLeft()
	{
		return m_flPhaseLength - m_PhaseTimer.GetElapsedTime();
	}
#endif // CLIENT_DLL

	virtual int GetCurrentPhaseID();
};


//-----------------------------------------------------------------------------
// Gets us at the game rules
//-----------------------------------------------------------------------------

inline CClassicGameRules* ClassicGameRules()
{
	return static_cast< CClassicGameRules* >( g_pGameRules );
}

#endif // CLASSICGAMERULES_H
