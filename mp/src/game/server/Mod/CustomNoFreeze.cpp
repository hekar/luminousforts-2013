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
#include "CNoFreezeBase.h"
#include "CustomNoFreeze.h"

LINK_ENTITY_TO_CLASS( lfc_trigger_nofreeze, CCustomNoFreeze );

BEGIN_DATADESC( CCustomNoFreeze )
	DEFINE_INPUTFUNC( FIELD_VOID, "AllowFreeze", InputAllowFreeze ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisallowFreeze", InputDisallowFreeze ),
	DEFINE_INPUTFUNC( FIELD_VOID, "AllowRedFreeze", InputAllowRedFreeze ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisallowRedFreeze", InputDisallowRedFreeze ),
	DEFINE_INPUTFUNC( FIELD_VOID, "AllowBlueFreeze", InputAllowBlueFreeze ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisallowBlueFreeze", InputDisallowBlueFreeze ),

	DEFINE_OUTPUT( m_OnBlockTouch, "OnBlockTouch" ),
	DEFINE_OUTPUT( m_OnRedBlockTouch, "OnRedBlockTouch" ),
	DEFINE_OUTPUT( m_OnBlueBlockTouch, "OnBlueBlockTouch" ),
	DEFINE_OUTPUT( m_OnRedPlayerTouch, "OnRedPlayerTouch" ),
	DEFINE_OUTPUT( m_OnBluePlayerTouch, "OnBluePlayerTouch" ),
END_DATADESC()

CCustomNoFreeze::CCustomNoFreeze()
{
}

void CCustomNoFreeze::Spawn()
{
	BaseClass::Spawn();
}

void CCustomNoFreeze::InputAllowFreeze( inputdata_t& data )
{
	m_AllowTeamMask	|= MOD_TEAM_BOTH;
}

void CCustomNoFreeze::InputDisallowFreeze( inputdata_t& data )
{
	m_AllowTeamMask	&= ~MOD_TEAM_BOTH;
}

void CCustomNoFreeze::InputAllowRedFreeze( inputdata_t& data )
{
	m_AllowTeamMask	|= MOD_TEAM_RED;
}

void CCustomNoFreeze::InputDisallowRedFreeze( inputdata_t& data )
{
	m_AllowTeamMask	&= ~MOD_TEAM_RED;
}

void CCustomNoFreeze::InputAllowBlueFreeze( inputdata_t& data )
{
	m_AllowTeamMask	|= MOD_TEAM_BLUE;
}

void CCustomNoFreeze::InputDisallowBlueFreeze( inputdata_t& data )
{
	m_AllowTeamMask	&= ~MOD_TEAM_BLUE;
}

void CCustomNoFreeze::StartTouch( CBaseEntity *pOther )
{
	if ( pOther->GetTeamNumber() != TeamMaskToNumber( m_AllowTeamMask ) )
	{
		DisallowFreeze( pOther );
	}

	if ( pOther->IsBlock() )
	{
		switch( pOther->GetTeamNumber() )
		{
		case SDK_TEAM_BLUE:
			m_OnBlueBlockTouch.FireOutput( pOther, this, 0.0f );
			break;
		case SDK_TEAM_RED:
			m_OnRedBlockTouch.FireOutput( pOther, this, 0.0f );
			break;
		default:
			m_OnBlockTouch.FireOutput( pOther, this, 0.0f );
		}
	}
	else if ( pOther->IsPlayer() )
	{
		switch( pOther->GetTeamNumber() )
		{
		case SDK_TEAM_BLUE:
			m_OnBluePlayerTouch.FireOutput( pOther, this, 0.0f );
			break;
		case SDK_TEAM_RED:
			m_OnRedPlayerTouch.FireOutput( pOther, this, 0.0f );
			break;
		}
	}
}

void CCustomNoFreeze::EndTouch( CBaseEntity *pOther )
{
	BaseClass::EndTouch( pOther );
}
