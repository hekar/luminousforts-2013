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
#ifdef MOD_ALPHA_PLAYER
#include "c_hl2mp_player.h"
#include "C_ModPlayer.h"

// main table
IMPLEMENT_CLIENTCLASS_DT( C_ModPlayer, DT_ModPlayer, CModPlayer )
	RecvPropInt( RECVINFO( m_iMaxHealth ) ),
END_RECV_TABLE()

BEGIN_PREDICTION_DATA( C_ModPlayer )
END_PREDICTION_DATA()

LINK_ENTITY_TO_CLASS( player, C_ModPlayer );

C_ModPlayer::C_ModPlayer()
{
	m_iMaxHealth = 100;
}

C_ModPlayer::~C_ModPlayer()
{
}

C_ModPlayer* C_ModPlayer::GetLocalModPlayer()
{
	return ToModPlayer( C_BasePlayer::GetLocalPlayer() );
}

C_ModPlayer *ToModPlayer( C_BasePlayer *pPlayer )
{
	return dynamic_cast< C_ModPlayer * > ( pPlayer );
}


class CPlayerInitGameSystem : public CAutoGameSystem
{
public:
	CPlayerInitGameSystem() :
	  CAutoGameSystem( "PlayerInitGameSystem" )
	{
	}

	// When map loading is nearly finished
	void LevelInitPostEntity()
	{
	}
};

static CPlayerInitGameSystem g_PlayerInitGameSystem;

#endif // MOD_ALPHA_PLAYER
