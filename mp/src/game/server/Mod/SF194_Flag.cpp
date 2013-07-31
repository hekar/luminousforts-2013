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
	Client/Server
	SF CTF Flag Physics Model Entity

	For backwards compatibility with SF 194.

	Everything in the classic Flag applies to this flag, there's
	just a layer for compatibility.

	Last Updated Dec 08, 2009
===============================================================*/

#include "cbase.h"

#include "team.h"
#include "hl2mp_player.h"

#include "props.h"
#include "igameevents.h"
#include "GameEventListener.h"
#include "Mod/CFlagBase.h"
#include "Mod/CFlagClassic.h"

class CFlagSF194 : public CFlagClassic
{
public:
	DECLARE_CLASS( CFlagSF194, CFlagClassic );
	DECLARE_DATADESC();

public:
	CFlagSF194()
	{
	}

	void Spawn()
	{
		if ( m_iTeam == 2 ) // Combine
		{
			ChangeTeam( SDK_TEAM_BLUE );
		}
		else if ( m_iTeam == 3 ) // Rebels
		{
			ChangeTeam( SDK_TEAM_RED );
		}
		
		BaseClass::Spawn();
	}

	virtual void OnPickupOutput()
	{
		m_OnGrab.FireOutput( this, NULL, 0.0f );
	}

	virtual void OnScoreOutput()
	{
		m_OnScore.FireOutput( this, NULL, 0.0f );
	}

	virtual void OnDropOutput()
	{
		m_OnDrop.FireOutput( this, NULL, 0.0f );
	}

	virtual void OnReturnOutput()
	{
		m_OnReturn.FireOutput( this, NULL, 0.0f );
	}

protected:
	COutputEvent m_OnGrab;
	COutputEvent m_OnScore;
	COutputEvent m_OnDrop;
	COutputEvent m_OnReturn;

	int m_iTeam;
};

LINK_ENTITY_TO_CLASS( prop_flag, CFlagSF194 );

BEGIN_DATADESC( CFlagSF194 )

	// This needs to be integrated also
	// input Return(void) : "Force flag return"

	DEFINE_KEYFIELD( m_iTeam, FIELD_INTEGER, "skin" ),

	// OnGrab is inaccurate as pick and grab are different things
	DEFINE_OUTPUT( m_OnPickup, "OnGrab" ),

	// This is probably the most important and needs to be implemented
	DEFINE_OUTPUT( m_OnScore, "OnScore" ),

	DEFINE_OUTPUT( m_OnDrop, "OnDrop" ),
	DEFINE_OUTPUT( m_OnReturn, "OnReturn" ),

END_DATADESC()
