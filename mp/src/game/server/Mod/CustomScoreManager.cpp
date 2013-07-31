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
#include "team.h"
#include "Mod/CustomScoreManager.h"

LINK_ENTITY_TO_CLASS( lfcus_score_manager, CCustomScoreManager );

BEGIN_DATADESC( CCustomScoreManager  )

	DEFINE_INPUTFUNC( FIELD_INTEGER, "GiveRedPoints", InputGiveRedPoints ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "GiveBluePoints", InputGiveBluePoints ),

	DEFINE_INPUTFUNC( FIELD_INTEGER, "TakeRedPoints", InputTakeRedPoints ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "TakeBluePoints", InputTakeBluePoints ),

	//DEFINE_OUTPUT( m_OnRedScoreChange, "OnRedScoreChange" ),
	//DEFINE_OUTPUT( m_OnRedScoreChange, "OnBlueScoreChange" ),

END_DATADESC()

CCustomScoreManager::CCustomScoreManager()
{
}

CCustomScoreManager::~CCustomScoreManager()
{
}

void CCustomScoreManager::Spawn()
{
	BaseClass::Spawn();
}

void CCustomScoreManager::InputGiveRedPoints( inputdata_t& data )
{
	GiveTeamPoints( TEAM_RED, data.value.Int() );
}

void CCustomScoreManager::InputGiveBluePoints( inputdata_t& data )
{
	GiveTeamPoints( TEAM_BLUE, data.value.Int() );
}

void CCustomScoreManager::InputTakeRedPoints( inputdata_t& data )
{
	GiveTeamPoints( TEAM_RED, -data.value.Int() );
}

void CCustomScoreManager::InputTakeBluePoints( inputdata_t& data )
{
	GiveTeamPoints( TEAM_BLUE, -data.value.Int() );
}

void CCustomScoreManager::GiveTeamPoints( int teamNum, int points )
{
	CTeam *team = GetGlobalTeam( teamNum );
	if ( team )
	{
		team->AddCapturePoints( points );
	}
}
