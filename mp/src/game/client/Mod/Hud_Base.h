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


#ifndef HUD_BASE_H
#define HUD_BASE_H


#ifdef WIN32
#pragma once
#endif

#include "iclientmode.h"
#include "hud_macros.h"
#include "hudelement.h"

#define HUDELEM_NONE			( 1<<0 )
#define HUDELEM_ALL				( 1<<1 )
#define HUDELEM_BLOCKHEALTH		( 1<<2 )
#define HUDELEM_FLAGCARRIER		( 1<<3 )
#define HUDELEM_FLAGCOMPASS		( 1<<4 )
#define HUDELEM_FLAGPOS			( 1<<5 )
#define HUDELEM_PHASETIME		( 1<<6 )
#define HUDELEM_PLAYERMARKER	( 1<<7 )
#define HUDELEM_FREEZEPROGRESS	( 1<<8 )
#define HUDELEM_CAPPOINTS		( 1<<9 )
#define HUDELEM_CAPPOS			( 1<<10 )
#define HUDELEM_DOMSCORE		( 1<<11 )

class CModHudElement : public CHudElement
{
	DECLARE_CLASS( CModHudElement, CHudElement );
public:
	CModHudElement( const char *pElementName, int HudElementMask = HUDELEM_ALL );
	~CModHudElement();

	virtual bool ShouldDraw();

	virtual int GetElementMask() const;
	virtual void SetElementMask( int flag = HUDELEM_ALL );

	virtual void LevelStart();
	virtual void LevelEnd();

	int GetHiddenBits() { return m_iHiddenBits; }

	virtual const int GetGameModes() const
	{
		return m_iGamemodes;
	}

	virtual void SetGameModes( int iGamemodes );

private:
	void LevelInit();
	void LevelShutdown();

private:
	int m_iElementMask;
	bool m_bLevelStarted;
	int m_iGamemodes;
};

class CModHudManager
{
public:
	CModHudManager();
	~CModHudManager();

	int GetEnabledHudElementMask();
	void SetEnabledHudElementMask( int enabledmask );

private:
	int m_iEnabledMask;
};

extern CModHudManager g_ModHudManager;

#endif // HUD_BASE_H
