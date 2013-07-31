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


#ifndef IRC_CLIENT_H
#define IRC_CLIENT_H

#ifdef MOD_IRC

#pragma warning ( disable : 4005 )
#include <libircclient.h>

extern ConVar lf_irc_nick;
extern ConVar lf_irc_server;
extern ConVar lf_irc_channel;
extern ConVar lf_irc_port;

typedef struct
{

} irc_user_t;

typedef struct
{
	char *channel;
	char *nick;

} irc_ctx_t;

typedef struct 
{
	char *message;
	unsigned int timestamp;
} irc_message_t;

class IRCClient : public CWorkerThread
{
public:
	IRCClient();
	~IRCClient();

	void AddRecievedMessage( irc_message_t *message );

	void SendMessage( irc_message_t *message );

	void InitLibIRC();
private:
	void DestroyLibIRC();

	void SetCallBacks();
	bool Connect( const char *server, const char *channel, unsigned int port, const char *nickname );

	int Run();

	irc_callbacks_t	m_callbacks;
	irc_ctx_t m_ctx;
	irc_session_t *m_session;
};

extern IRCClient *g_IRCClient;

class IRCClient_Starter : public CAutoGameSystem
{
public:
	IRCClient_Starter() : CAutoGameSystem( "IRC Client Starter" )
	{
	}

	void PostInit()
	{
		g_IRCClient->InitLibIRC();
	}
};

#endif // MOD_IRC

#endif // IRC_CLIENT_H