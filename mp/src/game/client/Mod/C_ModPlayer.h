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


#ifndef C_MODPLAYER_H
#define C_MODPLAYER_H

#ifdef _WIN32
#pragma once
#endif

#include "c_hl2mp_player.h"

class C_ModPlayer : public C_HL2MP_Player
{
public:
	DECLARE_CLASS( C_ModPlayer, C_HL2MP_Player );
	DECLARE_CLIENTCLASS();
	DECLARE_PREDICTABLE();
	DECLARE_INTERPOLATION();

	C_ModPlayer();
	~C_ModPlayer();
	
	int GetMaxHealth( ) const { return m_iMaxHealth; }

	static C_ModPlayer* GetLocalModPlayer();

	int m_iMaxHealth;
};

extern C_ModPlayer *ToModPlayer( C_BasePlayer *pPlayer );

#endif // C_MODPLAYER_H
