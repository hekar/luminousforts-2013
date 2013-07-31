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
#include "ServerGlobals.h"

//
//	Name: ExecConfig
//	Author: Hekar Khani
//	Description: Execute a .cfg file found in the cfg/ directory
//	Notes: 
//
void ExecConfig( const char *cfgfile )
{
	char command[ MAX_PATH + 12 ];
	Q_snprintf( command, sizeof( command ), "exec %s\n", cfgfile ); // needs the '\n'
	engine->ServerCommand( command );
}

//
//	Name: PlaySound
//	Author: Hekar Khani
//	Description: Plays sound on all clients
//	Notes: Taken from Sourceforts 194
//
void PlaySound( const char *soundname )
{
	for ( int i = 0; i <= gpGlobals->maxClients; i++ )
	{
		CBasePlayer *pPlayer = ( CBasePlayer * )UTIL_PlayerByIndex(i);
		if ( !pPlayer )
			continue;
			      
		if ( pPlayer->IsConnected() )
		{
			CSingleUserRecipientFilter filter( pPlayer );
			filter.MakeReliable();

			pPlayer->EmitSound( filter, pPlayer->entindex(), soundname, &pPlayer->GetAbsOrigin() );
		}
	}
}