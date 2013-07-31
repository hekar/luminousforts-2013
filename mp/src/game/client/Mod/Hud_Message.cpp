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
#include "Hud_Message.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

//DECLARE_HUDELEMENT( CHudMessageLog );
//DECLARE_HUD_MESSAGE( CHudMessageLog, SendMessage );

CHudMessageLog::CHudMessageLog( const char* pElementName ) :
	CModHudElement( pElementName ),
	BaseClass( NULL, "HudMessageLog" )
{

	// Only show in build phase
	SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_NEEDSUIT | HIDEHUD_BUILDPHASE );
}

CHudMessageLog::~CHudMessageLog()
{
}

void CHudMessageLog::Init( void )
{
}

bool CHudMessageLog::ShouldDraw (void)
{
	return CModHudElement::ShouldDraw();
}

void CHudMessageLog::Paint( void )
{
	// Draw the Background

	int x = 0;
	int y = 10;

	// Iterate through each message and draw it according to its type
	for ( int i = 0; i < m_MessageList.Count(); i++ )
	{
		logmessage_t *message = m_MessageList[ i ];
		vgui::surface()->DrawSetTextPos( x, y );
		vgui::surface()->DrawPrintText( message->message, sizeof( message->message ) );
		y += 10;
	}
}

void CHudMessageLog::ApplySchemeSettings( vgui::IScheme* scheme )
{
	BaseClass::ApplySchemeSettings( scheme );
}

bool CHudMessageLog::AppendMessage( const wchar_t *message, int message_type )
{
	logmessage_t *new_message = new logmessage_t;
	
	Q_wcsncpy( new_message->message, message, sizeof( new_message->message ) );
	new_message->message_type;

	m_MessageList.AddToTail( new_message );

	return true;
}

// Complex protocol for messaging on this Hud element
/*=======================================================
	Protocol:
		MessageType [long] - Index of message to print

	Each message has a different set of arguments.

=======================================================*/
void CHudMessageLog::MsgFunc_SendMessage( bf_read& data )
{

}

CHudMessageLog *g_Message = NULL;
void MessageLog_Write( const wchar_t *message, int message_type )
{
	if ( !g_Message )
	{
		// Find the messagelog
		g_Message = dynamic_cast< CHudMessageLog * > ( gHUD.FindElement( "HudMessageLog" ) );

		AssertMsg( g_Message, "MessageLog is NULL" );
	}

	if ( g_Message )
	{
		// Write the message to the logger
		g_Message->AppendMessage( message, message_type );
	}
}
