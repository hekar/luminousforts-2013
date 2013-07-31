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
	Client

	Global functions for the client

	Last Updated March 1, 2010
===============================================================*/
#include "cbase.h"
#include "c_playerresource.h"
#include "ClientGlobals.h"

//
//	Name: LocalPlayer()
//	Author: Hekar Khani
//	Description: Get the local player
//	Notes:
//
C_BasePlayer *LocalPlayer()
{
	return C_BasePlayer::GetLocalPlayer();
}

//
//	Name: LocalPlayerIndex()
//	Author: Hekar Khani
//	Description: Get the localplayer's index (not userid)
//	Notes:
//
int LocalPlayerIndex()
{
	return GetLocalPlayerIndex();
}

//
//	Name: LocalTeamNumber()
//	Author: Hekar Khani
//	Description: Get the localplayer's teamnumber
//	Notes:
//
int LocalTeamNumber()
{
	return GetLocalPlayerTeam();
}

//
//	Name: LocalTeamColor( int TeamColorType )
//	Author: Hekar Khani
//	Description: Get the team color of the localplayer according
//				to the type
//	Notes:
//
 Color LocalTeamColor( int TeamColorType )
{
	C_BasePlayer *pPlayer = C_BasePlayer::GetLocalPlayer();
	Assert( pPlayer );

	return GetTeamColor( pPlayer->GetTeamNumber(), TeamColorType );
}
