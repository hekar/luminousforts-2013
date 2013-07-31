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
#include "Hud_CapPos.h"
#include "Hud_CapPoints.h"
#include "C_Dom_CaptureZone.h"

LINK_ENTITY_TO_CLASS( lfd_capture_zone, C_DomCaptureZone );
IMPLEMENT_CLIENTCLASS_DT( C_DomCaptureZone, DT_DomCaptureZone, CDomCaptureZone )
	RecvPropInt( RECVINFO( m_Locked ) ),
	RecvPropInt( RECVINFO( m_ZoneId ) ),
	RecvPropInt( RECVINFO( m_CapState ) ),
	RecvPropInt( RECVINFO( m_iCapturePercentage ) ),
	RecvPropInt( RECVINFO( m_iCapturingCount ) ),
	RecvPropInt( RECVINFO( m_iCapturingTeam ) ),
END_RECV_TABLE()

C_DomCaptureZone::C_DomCaptureZone()
{
}

C_DomCaptureZone::~C_DomCaptureZone()
{
}

void C_DomCaptureZone::UpdatePos( const Vector& origin )
{
	CHudCapPoints *hudcp = dynamic_cast< CHudCapPoints * > ( gHUD.FindElement( "CHudCapPoints" ) );
	AssertMsg( hudcp, "Failed to find HudCapPoints" );
	if ( hudcp )
	{
		hudcp->AddCaptureZone( this );
		m_vecLastOrigin = origin;
	}

	CHudCapPos *hudcpos = dynamic_cast< CHudCapPos * > ( gHUD.FindElement( "CHudCapPos" ) );
	AssertMsg( hudcpos, "Failed to find HudCapPos" );
	if ( hudcpos )
	{
		hudcpos->AddCaptureZone( this );
		m_vecLastOrigin = origin;
	}
}

void C_DomCaptureZone::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );

	if ( updateType == DATA_UPDATE_CREATED )
	{
		UpdatePos( WorldSpaceCenter() );
	}
}
