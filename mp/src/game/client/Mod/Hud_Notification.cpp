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
#include <vgui/ISurface.h>
#include "Mod/C_ModPlayer.h"
#include "Mod/Hud_Notification.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// DECLARE_HUDELEMENT( CHudNotification );
// DECLARE_HUD_MESSAGE( CHudNotification, NotificationIcon );

CHudNotification::CHudNotification( const char* pElementName ) :
	CModHudElement( pElementName ),
	BaseClass( NULL, "HudNotification" )
{
	SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT );
}

CHudNotification::~CHudNotification()
{
}

void CHudNotification::Init( void )
{
}

void CHudNotification::LevelInit( void )
{
	for ( int i = 0; i < m_IconList.Count(); i++ )
	{
		m_IconList.Element( i ) = NULL;
	}
}

bool CHudNotification::ShouldDraw (void)
{
	C_HL2MP_Player *pPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();
	if ( !pPlayer )
		return false;

	return CModHudElement::ShouldDraw();
}

void CHudNotification::Paint( void )
{
}

/**
 *
 * short - 
 * long - Duration in seconds to display (-1 means forever)
 */
void CHudNotification::MsgFunc_NotificationIcon( bf_read& data )
{
}
