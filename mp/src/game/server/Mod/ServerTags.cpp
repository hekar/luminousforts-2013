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
#include "ServerTags.h"

void AddTag( const char *tag )
{
	ConVar *sv_tags = cvar->FindVar( "sv_tags" );

	if ( Q_strstr( sv_tags->GetString(), tag) != NULL )
	{
		return;
	}

	char contents[ 2048 ];
	Q_strncpy( contents, sv_tags->GetString(), sizeof( contents ) );
	Q_strncat( contents, tag, sizeof( contents ) );
	
	sv_tags->SetValue( contents );
}

void SetTags( const char *tags )
{
	ConVar *sv_tags = cvar->FindVar( "sv_tags" );
	sv_tags->SetValue( tags );
}

void ReplaceTag( const char *tag, const char *replace )
{
	ConVar *sv_tags = cvar->FindVar( "sv_tags" );
	char contents[ 2048 ] = { '\0' };
	while ( Q_StrReplace( sv_tags->GetString(), contents, sizeof( contents ), tag, replace ) ) {}

	sv_tags->SetValue( contents );
}

void RemoveTag( const char *tag )
{
	ConVar *sv_tags = cvar->FindVar( "sv_tags" );

	char contents[ 2048 ] = { '\0' };
	while ( Q_StrReplace( sv_tags->GetString(), contents, sizeof( contents ), tag, "" ) ) {}

	sv_tags->SetValue( contents );
}
