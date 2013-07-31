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

	Base Hud element for the mod's hud elements to derive from.
	Adds some kinda useless stuff, but mostly is there to
	take care of multiple gamemodes and any extraneous features
	that would otherwise require duplication.

	Also adds part of the ability to disable specific elements from
	the server side.

	Last Updated March 1, 2010
=================================================================*/

#include "cbase.h"
#include "c_hl2mp_player.h"
#include "Mod/Hud_Base.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// hud manager for enabling and disabling custom hud elements
CModHudManager g_ModHudManager;

CModHudElement::CModHudElement( const char *pElementName, int HudElementMask ) :
	BaseClass( pElementName )
{
	SetElementMask( HudElementMask );
	m_bLevelStarted = false;
	m_iGamemodes = GAMEMODE_ALL;
}

CModHudElement::~CModHudElement()
{
}

bool CModHudElement::ShouldDraw()
{
	C_HL2MP_Player *player = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !player )
		return false;

	if ( m_bLevelStarted )
	{
		if ( GameRules()->GetGameModeMask() & m_iGamemodes )
		{
			return BaseClass::ShouldDraw();
		}
		else
		{
			return false;
		}
	}
	else if ( !( g_ModHudManager.GetEnabledHudElementMask() & GetElementMask() ) &&
		g_ModHudManager.GetEnabledHudElementMask() != HUDELEM_ALL )
	{
		return false;
	}
	else if ( g_ModHudManager.GetEnabledHudElementMask() & GetElementMask() &&
		g_ModHudManager.GetEnabledHudElementMask() != HUDELEM_ALL )
	{
		return true;
	}

	return BaseClass::ShouldDraw();
}

int CModHudElement::GetElementMask() const
{
	return m_iElementMask;
}

void CModHudElement::SetElementMask( int flag )
{
	m_iElementMask = flag;
}

void CModHudElement::LevelStart()
{
	// The Level has been started
	m_bLevelStarted = true;
}

void CModHudElement::LevelEnd()
{
	// The Level has ended
	m_bLevelStarted = false;
}

void CModHudElement::SetGameModes( int iGamemodes )
{
	// Gamemodes to display hud element on
	m_iGamemodes = iGamemodes;
}

PrivateMethod void CModHudElement::LevelInit()
{
	LevelStart();
}

PrivateMethod void CModHudElement::LevelShutdown()
{
	LevelEnd();
}

CModHudManager::CModHudManager()
{
	m_iEnabledMask = HUDELEM_ALL;
}

CModHudManager::~CModHudManager()
{
}

int CModHudManager::GetEnabledHudElementMask()
{
	return m_iEnabledMask;
}

void CModHudManager::SetEnabledHudElementMask( int enabledmask )
{
	m_iEnabledMask = enabledmask;
}

class CModHudManagerSystem : public CAutoGameSystem
{
public:
 	CModHudManagerSystem() :
	  CAutoGameSystem( "ModHudManagerSystem" )
	{
	}

	void LevelInitPreEntity()
	{
		g_ModHudManager.SetEnabledHudElementMask( HUDELEM_ALL );
	}
};

static CModHudManagerSystem g_ModHudManagerHelper;
