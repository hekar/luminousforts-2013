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


#ifndef SF132_GAMERULES_H
#define SF132_GAMERULES_H

#ifdef _WIN32
	#pragma once
#endif

#ifdef MOD_SF132

#include "ClassicGameRules.h"

#ifdef CLIENT_DLL
#define CSF132GameRules C_SF132GameRules
#endif

class CSF132GameRules : public CClassicGameRules
{
public:
	DECLARE_CLASS( CSF132GameRules, CClassicGameRules );

	virtual bool Init( void );

	virtual int GetGameMode( void ) { return GAMEMODE_SF132; }
	virtual int GetGameModeMask( void ) { return GAMEMODE_CLASSIC | GAMEMODE_SF132; }

#ifdef CLIENT_DLL

	DECLARE_CLIENTCLASS_NOBASE(); // This makes datatables able to access our private vars.

#else

	DECLARE_SERVERCLASS_NOBASE(); // This makes datatables able to access our private vars.

	CSF132GameRules();
	~CSF132GameRules();
	virtual const char *GetGameDescription( void );
	virtual bool IsAllowedToSpawn( CBaseEntity *pEntity );

	virtual void GiveBuildPhaseItems( CHL2MP_Player *pPlayer );
	virtual void PlayerSpawn( CBasePlayer *pBasePlayer );

public:
	virtual const char *GetChatPrefix( bool bTeamOnly, CBasePlayer *pPlayer );
	virtual const char *GetChatFormat( bool bTeamOnly, CBasePlayer *pPlayer );

#endif // #ifndef CLIENT_DLL

public:
	virtual char **GetClassNames( int TeamNum );
	virtual int GetClassCount( void );
	virtual const char *GetClassmenuRed() const;
	virtual const char *GetClassmenuBlue() const;
};


//-----------------------------------------------------------------------------
// Gets us at the game rules
//-----------------------------------------------------------------------------

inline CSF132GameRules* SF132GameRules()
{
	return static_cast< CSF132GameRules* >( g_pGameRules );
}

#endif // MOD_SF132

#endif // SF132_GAMERULES