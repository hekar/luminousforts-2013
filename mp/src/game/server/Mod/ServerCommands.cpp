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
#include "Mod/ClassicGameRules.h"
#include "Mod/ServerCommands.h"

#define PRINT_IS_GAMEMODE( gamemode ) \
	if ( GameRules()->GetGameMode() & gamemode ) \
		Msg( "Current GameMode: %s\n", #gamemode );

void CC_PrintGameMode( const CCommand& args )
{
	PRINT_IS_GAMEMODE( GAMEMODE_SDKBASE );
	PRINT_IS_GAMEMODE( GAMEMODE_CLASSIC );
	PRINT_IS_GAMEMODE( GAMEMODE_SF132 );
}

static ConCommand mds_print_gamemode( "mds_print_gamemode", CC_PrintGameMode, "prints current gamemode (server)" );
