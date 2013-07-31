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
	Sourceforts 1.3.2 Gamemode

	Sourceforts 1.3.2 flag

	Last Updated Feb 27, 2010
===============================================================*/
#include "cbase.h"
#ifdef MOD_SF132
#include "sdk_team.h"
#include "hl2mp_player.h"
#include "CFlagBase.h"
#include "CFlagClassic.h"
#include "SF132_Flag.h"

void CSF132FlagBase::OnScore()
{
	CTeam *pTeam = GetGlobalSDKTeam( GetTeamNumber() );
	if ( !pTeam )
	{
		Error( "Flag has no team" );
	}

	pTeam->AddCapturePoints( FLAG_CLASSIC_CAPTURE_POINTS );

	Msg ( "Score %d\n", pTeam->GetCapturePoints() );

	switch( GetTeamNumber() )
	{
		case SDK_TEAM_BLUE:
			PlaySound( FLAG_RED_SCORE );
			break;

		case SDK_TEAM_RED:
			PlaySound( FLAG_BLUE_SCORE );
			break;
	}
}

void CSF132FlagBase::OnPlayerTouch( CBasePlayer *pPlayer )
{
	if ( !m_bDropped )
	{
		CHL2MP_Player *pSDKPlayer = ToSDKPlayer( pPlayer );
		if ( pSDKPlayer )
		{
			if ( pSDKPlayer->HasFlag() )
			{
				OnScore();
				OnScoreEvent();
				OnScoreOutput();

				pSDKPlayer->ReturnFlag( true );
			}
		}
	}
}


LINK_ENTITY_TO_CLASS( red_flag, CSF132FlagRed )

CSF132FlagRed::CSF132FlagRed()
{
}

CSF132FlagRed::~CSF132FlagRed()
{
}

void CSF132FlagRed::Spawn( void )
{
	ChangeTeam( SDK_TEAM_RED );

	BaseClass::Spawn();
}

LINK_ENTITY_TO_CLASS( blue_flag, CSF132FlagBlue )

CSF132FlagBlue::CSF132FlagBlue()
{
}

CSF132FlagBlue::~CSF132FlagBlue()
{
}

void CSF132FlagBlue::Spawn( void )
{
	ChangeTeam( SDK_TEAM_BLUE );

	BaseClass::Spawn();
}

#endif // MOD_SF132