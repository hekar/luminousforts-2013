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
#ifdef CLIENT_DLL
#include "c_playerresource.h"
#endif // CLIENT_DLL
#include "SharedGlobals.h"

 Color GetTeamColorNormal( int nTeamIndex );
 Color GetTeamColorLite( int nTeamIndex );
 Color GetTeamColorMed( int nTeamIndex );
 Color GetTeamColorDark( int nTeamIndex );

//
//	Name: GetOtherTeamNumber( int iTeamNum )
//	Author: Hekar Khani
//	Description: Gets the opposing team color
//	Notes:
//
int GetOtherTeamNumber( int iTeamNum )
{
	return ( iTeamNum == SDK_TEAM_BLUE ) ? SDK_TEAM_RED : SDK_TEAM_BLUE;
}

//
//	Name: TeamMaskToNumber( int TeamMask )
//	Author: Hekar Khani
//	Description: Convert team number to a team mask
//	Notes:
//
int TeamMaskToNumber( int TeamMask )
{
	if ( TeamMask & MOD_TEAM_BOTH )
	{
		return TEAM_SPECTATOR;
	}
	else if ( ( TeamMask & MOD_TEAM_BLUE ) 
		&& ( TeamMask & MOD_TEAM_RED ) )
	{
		return TEAM_SPECTATOR;
	}
	else if ( TeamMask & MOD_TEAM_BLUE )
	{
		return SDK_TEAM_BLUE;
	}
	else if ( TeamMask & MOD_TEAM_RED )
	{
		return SDK_TEAM_RED;
	}
	else if ( TeamMask & MOD_TEAM_SPEC )
	{
		return TEAM_SPECTATOR;
	}
	else
	{
		return TEAM_INVALID;
	}
}

//
//	Name: GetTeamColor( int nTeamIndex, int TeamColorType )
//	Author: Hekar Khani
//	Description: Get the color for corresponding team according to the
//				requested color type
//	Notes:
//
 Color GetTeamColor( int nTeamIndex, int TeamColorType )
{
	switch ( TeamColorType )
	{
	case TEAM_CLR_NORMAL:
		return GetTeamColorNormal( nTeamIndex );
		break;
	case TEAM_CLR_LITE:
		return GetTeamColorLite( nTeamIndex );
		break;
	case TEAM_CLR_MED:
		return GetTeamColorMed( nTeamIndex );
		break;
	case TEAM_CLR_DARK:
		return GetTeamColorDark( nTeamIndex );
		break;
	default:
		return Color( 255, 255, 255, 255 );
		break;
	}
}

//
//	Name: GetTeamColorNormal( int nTeamIndex )
//	Author: Hekar Khani
//	Description: Get the default team color of the
//				requested team
//	Notes:
//
 Color GetTeamColorNormal( int nTeamIndex )
{
#ifdef CLIENT_DLL
	return g_PR->GetTeamColor( nTeamIndex );
#endif // CLIENT_DLL
	switch ( nTeamIndex )
	{
	case SDK_TEAM_RED:
		return Color( LF_CLR_TEAM_RED );
		break;
	case SDK_TEAM_BLUE:
		return Color( LF_CLR_TEAM_BLUE );
		break;
	default:
		return Color( LF_CLR_TEAM_UNASSIGNED );
		break;
	}
}

//
//	Name: GetTeamColorLite( int nTeamIndex )
//	Author: Hekar Khani
//	Description: Get the light team color
//	Notes:
//
 Color GetTeamColorLite( int nTeamIndex )
{
	switch ( nTeamIndex )
	{
	case SDK_TEAM_RED:
		return Color( LF_CLR_TEAM_RED_LITE );
		break;
	case SDK_TEAM_BLUE:
		return Color( LF_CLR_TEAM_BLUE_LITE );
		break;
	default:
		return Color( LF_CLR_TEAM_UNASSIGNED );
		break;
	}
}

//
//	Name: GetTeamColorMed( int nTeamIndex )
//	Author: Hekar Khani
//	Description: Get the medium team colorS
//	Notes:
//
 Color GetTeamColorMed( int nTeamIndex )
{
	switch ( nTeamIndex )
	{
	case SDK_TEAM_RED:
		return Color( LF_CLR_TEAM_RED_MED );
		break;
	case SDK_TEAM_BLUE:
		return Color( LF_CLR_TEAM_BLUE_MED );
		break;
	default:
		return Color( LF_CLR_TEAM_UNASSIGNED );
		break;
	}
}

//
//	Name: GetTeamColorDark( int nTeamIndex )
//	Author: Hekar Khani
//	Description: Get the dark team color
//	Notes:
//
 Color GetTeamColorDark( int nTeamIndex )
{
	switch ( nTeamIndex )
	{
	case SDK_TEAM_RED:
		return Color( LF_CLR_TEAM_RED_DARK );
		break;
	case SDK_TEAM_BLUE:
		return Color( LF_CLR_TEAM_BLUE_DARK );
		break;
	default:
		return Color( LF_CLR_TEAM_UNASSIGNED );
		break;
	}
}

 bool IsPlayingTeam( int teamNumber )
 {
	 return teamNumber == TEAM_RED || TEAM_BLUE;
 }

int Q_StrReplace( const char *str, char *output, int size, const char *original, const char *replace )
{
	char *pos = NULL;
	if(  ( pos = Q_strstr( str, original ) ) == NULL )
		return FALSE;

	Q_strncpy( output, str, clamp( pos - str, 0, size - 1 ) );
	output[ clamp( pos - str, 0, size - 1 ) ] = '\0';

	Q_snprintf( output + clamp( pos - str, 0, size - 1 ) , size, "%s%s", replace, pos + strlen( original ) );

	return TRUE;
}
