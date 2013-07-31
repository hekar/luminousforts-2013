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


#ifndef IRC_EVENTS_H
#define IRC_EVENTS_H

#ifdef MOD_IRC

#include "libircclient.h"

extern void event_join (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
extern void event_connect (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
extern void event_privmsg (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);
extern void event_channel (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count);

extern void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count);
extern void irc_event_dcc_chat (irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid);
extern void irc_event_dcc_send (irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid);

extern void dummy_event( irc_session_t *session, const char *event, const char *origin, const char **params, unsigned int count );

#endif // MOD_IRC

#endif // IRC_EVENTS_H