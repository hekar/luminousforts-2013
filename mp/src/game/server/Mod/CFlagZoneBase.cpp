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
	Brush Entity

	Mapper defines where the flag capturing zone is

	Last Updated Oct 31 2009
===============================================================*/


#include "cbase.h"
#include "hl2mp_player.h"
#include "team.h"
#include "props.h"
#include "igameevents.h"
#include "GameEventListener.h"
#include "triggers.h"
#include "Mod/SharedModCvars.h"
#include "Mod/CModPlayer.h"
#include "Mod/CFlagBase.h"
#include "Mod/CFlagZoneBase.h"

BEGIN_DATADESC( CFlagZoneBase )
	DEFINE_KEYFIELD( m_bDisabled, FIELD_BOOLEAN, "StartDisabled" ),
	DEFINE_ENTITYFUNC( Touch ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Enable", InputEnable),
	DEFINE_INPUTFUNC( FIELD_VOID, "Disable", InputDisable),
END_DATADESC()

CFlagZoneBase::CFlagZoneBase()
{
	m_bDisabled = false;
}

void CFlagZoneBase::Spawn()
{
	if ( lf_reverse_flags.GetBool() )
	{
		ChangeTeam( GetOtherTeamNumber( GetTeamNumber() ) );
	}

	BaseClass::Spawn();
	InitTrigger();
	SetTouch( &CFlagZoneBase::Touch );
}

void CFlagZoneBase::Touch( CBaseEntity *pOther )
{
	if ( m_bDisabled )
		return;

	if ( !pOther )
	{
		return;
	}
	else if ( !pOther->IsPlayer() )
	{
		return;
	}

	CModPlayer *pPlayer = ToModPlayer( pOther );
	if ( GetTeamNumber() == pPlayer->GetTeamNumber() ||
		GetTeamNumber() == TEAM_SPECTATOR )
	{
		if ( pPlayer->HasFlag () )
		{
			Capture( pPlayer );
		}
	}
}

void CFlagZoneBase::Capture( CModPlayer *pPlayer )
{
	CFlagBase *pFlag = pPlayer->GetFlag();

	pFlag->OnScore();
	pFlag->OnScoreEvent();
	pFlag->OnScoreOutput();

	pPlayer->ReturnFlag( true );

	OnCapture( pPlayer->GetTeamNumber() );
}

void CFlagZoneBase::OnCapture( int teamnum )
{
}

void CFlagZoneBase::InputEnable( inputdata_t &inputData )
{
	m_bDisabled = false;
}

void CFlagZoneBase::InputDisable( inputdata_t &inputData )
{
	m_bDisabled = true;
}
