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
#include "c_hl2mp_player.h"
#include "Mod/HudH_View.h"
#include "Mod/HudH_MarkerTools.h"

DrawMarker::DrawMarker()
{
	m_Divider = 1300;
	m_WidthMin = 16;
	m_HeightMin = 28;
	m_WidthMax = 16;
	m_HeightMax = 28;
}

DrawMarker::~DrawMarker()
{
}

void DrawMarker::Draw()
{
	C_HL2MP_Player *pLocalPlayer = C_HL2MP_Player::GetLocalHL2MPPlayer();

	if ( !pLocalPlayer )
		return;

	Vector v = m_Origin;
	ViewTools viewTools;
	if( viewTools.InPlayerView( pLocalPlayer, v ) )
	{
		int length = viewTools.CalculateDistanceLength( pLocalPlayer->GetAbsOrigin(), v );
		//int alpha = viewTools.CalculateAlphaFade( length, 60000 );

		int x, y = 0;
		viewTools.VectorTnScreenSpace( v, x, y, NULL );

		float divider = m_Divider;
		float width = clamp( m_Texture->EffectiveWidth( length / divider ), m_WidthMin, m_WidthMax );
		float height = clamp( m_Texture->EffectiveWidth( length / divider ), m_HeightMin, m_HeightMax );

		if ( m_Texture )
		{
			m_Texture->DrawSelf( x - ( width / 2 ), y - ( height / 2 ), width, height, m_Color );
		}
	}
}
