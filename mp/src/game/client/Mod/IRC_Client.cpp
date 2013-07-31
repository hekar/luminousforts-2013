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


// Global Singleton IRC Client
#include "cbase.h"

#ifdef MOD_IRC

#include "IRC_Client.h"
#include "IRC_Events.h"

static IRCClient_Starter g_IRCClient_Starter;

static IRCClient g_StaticIRCClient;
IRCClient *g_IRCClient = &g_StaticIRCClient;

ConVar lf_irc_nick( "lf_irc_nick", "sourceforts", FCVAR_ARCHIVE, "Nickname for IRC Client" );
ConVar lf_irc_server( "lf_irc_server", "irc.gamesurge.net", FCVAR_ARCHIVE, "Server for IRC Client" );
ConVar lf_irc_channel( "lf_irc_channel", "sourceforts", FCVAR_ARCHIVE, "Channel for IRC Client" );
ConVar lf_irc_port( "lf_irc_port", "6667", FCVAR_ARCHIVE, "Port for IRC Client" );

IRCClient::IRCClient()
{
	SetName( "IRCClientThread" );
}

IRCClient::~IRCClient()
{
	DestroyLibIRC();
}

void IRCClient::InitLibIRC()
{
	SetCallBacks();
	Start();
	
	Msg( "IRC CLIENT INITIALIZED\n" );
}

void IRCClient::DestroyLibIRC()
{
	irc_destroy_session( m_session );
}

void IRCClient::SetCallBacks()
{
	irc_callbacks_t& callbacks = m_callbacks;

	memset( &callbacks, 0, sizeof( m_callbacks ) );

	callbacks.event_connect = dummy_event;//event_connect;
	callbacks.event_join = dummy_event;//event_join;
	callbacks.event_nick = dummy_event;
	callbacks.event_quit = dummy_event;
	callbacks.event_part = dummy_event;
	callbacks.event_mode = dummy_event;
	callbacks.event_topic = dummy_event;
	callbacks.event_kick = dummy_event;
	callbacks.event_channel = dummy_event;//event_channel;
	callbacks.event_privmsg = dummy_event;//event_privmsg;
	callbacks.event_notice = dummy_event;
	callbacks.event_invite = dummy_event;
	callbacks.event_umode = dummy_event;
	callbacks.event_ctcp_req = dummy_event;
	callbacks.event_ctcp_rep = dummy_event;
	callbacks.event_ctcp_action = dummy_event;
	callbacks.event_unknown = dummy_event;
	callbacks.event_numeric = event_numeric;

	callbacks.event_dcc_chat_req = irc_event_dcc_chat;
	callbacks.event_dcc_send_req = irc_event_dcc_send;
}

bool IRCClient::Connect( const char *server, const char *channel, unsigned int port, const char *nickname )
{
	m_session = irc_create_session( &m_callbacks );

	m_ctx.channel = (char *)channel;
    m_ctx.nick = (char *)nickname;

	irc_set_ctx( m_session, &m_ctx );

	if ( irc_connect( m_session, server, (unsigned short)port, nickname, channel, 0, 0) )
	{
		// Set error code in a convar
		return false;
	}

	return true;
}

// Asynchronious run
int IRCClient::Run()
{
	if ( Connect( lf_irc_server.GetString(), lf_irc_channel.GetString(), lf_irc_port.GetInt(), lf_irc_nick.GetString() ) )
	{
		irc_run( m_session );
	}
	else
	{
		Warning( "Failed to Connect to IRC Server %s, Channel %s, Port %d\n", 
			lf_irc_server.GetString(), lf_irc_channel.GetString(), lf_irc_port.GetInt() );
	}

	return 1;
}

#endif // MOD_IRC