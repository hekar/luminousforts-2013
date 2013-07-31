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

#ifndef DOM_GAMERULES_H
#define DOM_GAMERULES_H

#ifdef WIN32
#pragma once
#endif

#include "ClassicGameRules.h"

class CDomGameRules : public CClassicGameRules
{
public:
	DECLARE_CLASS( CDomGameRules, CClassicGameRules );

	CDomGameRules();
	virtual ~CDomGameRules();

	virtual int GetGameMode( void ) { return GAMEMODE_DOM; }
	virtual int GetGameModeMask( void ) { return GAMEMODE_CLASSIC | GAMEMODE_DOM; }

	virtual const char *GetClassmenuRed() const;
	virtual const char *GetClassmenuBlue() const;
#ifndef CLIENT_DLL
	virtual const char *GetGameDescription( void );
	virtual float FlPlayerFallDamage( CBasePlayer *pPlayer );
#endif

private:
	int m_iBuildFallDamage;
	int m_iCombatFallDamage;
};

inline CDomGameRules* DomGameRules()
{
	return static_cast< CDomGameRules* >( g_pGameRules );
}

#endif // DOM_GAMERULES_H