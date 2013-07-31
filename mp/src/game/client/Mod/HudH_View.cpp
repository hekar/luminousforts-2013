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
#include "HudH_View.h"

bool ViewTools::InPlayerView( C_BasePlayer *pLocalPlayer, Vector position )
{
	// Clip text that is far away
	if( ( pLocalPlayer->GetAbsOrigin() - position ).LengthSqr() > 90000000 )
	{
		return false;
	}

	// Clip text that is behind the client
	Vector clientForward;
	pLocalPlayer->EyeVectors( &clientForward );

	Vector toText = position - pLocalPlayer->GetAbsOrigin();
	float  dotPr = DotProduct( clientForward, toText );

	if ( dotPr < 0 )
	{
		return false;
	}

	return true;
}

int ViewTools::CalculateDistanceLength( Vector playerPosition, Vector position )
{
	return ( position - playerPosition ).Length();
}

int ViewTools::CalculateAlphaFade( int distance, int distanceStart )
{
	int alpha = 255;
	if( distance > distanceStart )
	{
		alpha = 6255 - distance;

		if ( alpha < 0 )
		{
			alpha = 0;
		}
	}

	return alpha;
}

void ViewTools::VectorTnScreenSpace( Vector& position, int& x, int& y, Vector *offset )
{
	GetVectorInScreenSpace( position, x, y, NULL );
}
