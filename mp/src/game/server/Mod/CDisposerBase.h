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


#ifndef CBLOCKDISPOSER_H
#define CBLOCKDISPOSER_H

#ifdef _WIN32
	#pragma once
#endif

#include "triggers.h"

class CBlockDisposer : public CBaseToggle
{
public:
	DECLARE_CLASS( CBlockDisposer, CBaseToggle );
	DECLARE_DATADESC();

public:
	CBlockDisposer();

	virtual void OnDisposeBlock( CBaseEntity *pBlock );
	virtual void OnDisposePlayer( CBaseEntity *pPlayer );
	
	virtual bool CreateVPhysics()
	{
		VPhysicsInitShadow( false, false );
		return true;
	}

	virtual void Spawn ();

	virtual bool OnFlagTouch( CBaseEntity *pOther );
	virtual bool OnBlockTouch( CBaseEntity *pOther );
	virtual bool OnPlayerTouch( CBaseEntity *pOther );
	virtual bool OnMiscTouch( CBaseEntity *pOther );

	virtual void Touch( CBaseEntity *pOther );

protected:
	COutputEvent m_OnDisposeBlock;
	COutputEvent m_OnDisposePlayer;

	bool m_bDissolve;
};


#endif // CBLOCKDISPOSER_H