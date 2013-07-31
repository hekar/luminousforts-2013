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

	This is a pretty old file from probably around March 2009.
	Only the phase manager really applies. If you want you can
	use the other stuff though.

	Last Updated Sept 13 2009
===============================================================*/


#include "cbase.h"
#include "PhaseControl.h"
#include "ClassicPhases.h"

#define LUMINOUSFORTS_SOUND_BUILD "Luminousforts.Build"
#define LUMINOUSFORTS_SOUND_COMBAT "Luminousforts.Combat"

class CPrecachePhaseSounds : public CBaseEntity
{
public:
	void Precache()
	{
		PrecacheScriptSound( LUMINOUSFORTS_SOUND_BUILD );
		PrecacheScriptSound( LUMINOUSFORTS_SOUND_COMBAT );

		//HODO: Move hacky precaches
		PrecacheScriptSound( "CTF.Draw" );
		PrecacheScriptSound( "CTF.BlueWins" );
		PrecacheScriptSound( "CTF.RedWins" );
	}
};

LINK_ENTITY_TO_CLASS( precache_sourceforts_sounds, CPrecachePhaseSounds );

//
//	Name: CBuildPhase
//	Author: Hekar Khani
//	Description: On the switch to build phase
//	Notes:
//
CBuildPhase::CBuildPhase() :
	CPhaseBase( PHASE_BUILD, "BuildPhase" )
{
}

CBuildPhase::~CBuildPhase()
{
}

void CBuildPhase::SwitchTo()
{
	Msg( "[LF] Build Phase Has Begun\n" );
	PlaySound( LUMINOUSFORTS_SOUND_BUILD );
}


//
//	Name: CCombatPhase
//	Author: Hekar Khani
//	Description: On the switch to combat phase
//	Notes:
//
CCombatPhase::CCombatPhase() :
	CPhaseBase( PHASE_COMBAT, "CombatPhase" )
{
}

CCombatPhase::~CCombatPhase()
{
}

void CCombatPhase::SwitchTo()
{
	Msg( "[LF] Combat Phase Has Begun\n" );
	PlaySound( LUMINOUSFORTS_SOUND_COMBAT );
}
