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
	Server
	Brush Entity

	Disposes of players, disposes of blocks and returns flag.

	Last Updated Dec 08, 2009
===============================================================*/

#include "cbase.h"
#include "team.h"
#include "hl2mp_player.h"
#include "model_types.h"
#include "Mod/CBlockBase.h"
#include "Mod/CDisposerBase.h"
#include "Mod/CDisposerClassic.h"



LINK_ENTITY_TO_CLASS( lfc_trigger_disposer, CBlockDisposerClassic );

BEGIN_DATADESC( CBlockDisposerClassic )
END_DATADESC()

//
//	Name: CBlockDisposerClassic
//	Author: Hekar Khani
//	Description: Simple Disposer for LF
//	Notes:
//
CBlockDisposerClassic::CBlockDisposerClassic()
{
}

void CBlockDisposerClassic::OnDisposeBlock( CBlockBase *pBlock )
{
	pBlock->OnDisposed( this );
	m_OnDisposeBlock.FireOutput( this, NULL, 0.0f );
}

void CBlockDisposerClassic::OnDisposePlayer( CBasePlayer *pPlayer )
{
	m_OnDisposePlayer.FireOutput( this, NULL, 0.0f );
}
