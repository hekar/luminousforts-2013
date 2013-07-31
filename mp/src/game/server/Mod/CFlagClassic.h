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


#ifndef CFLAGCLASSIC_H
#define CFLAGCLASSIC_H

#ifdef _WIN32
	#pragma once
#endif

#include "Sprite.h"
#include "SpriteTrail.h"

#define FLAG_CLASSIC_PLAYER_CAPTURE_POINTS 20

#define FLAG_CLASSIC_TOUCH_POINTS 1
#define FLAG_CLASSIC_CAPTURE_POINTS 100

#define FLAG_MODEL_CLASSIC "models/Roller_Spikes.mdl"

// Classic Flag sound definitions
#define FLAG_RED_TAKEN		"CTF.RedTaken"
#define FLAG_BLUE_TAKEN		"CTF.BlueTaken"
#define FLAG_RED_DROPPED	"CTF.RedDropped"
#define FLAG_BLUE_DROPPED	"CTF.BlueDropped"
#define FLAG_RED_SCORE		"CTF.RedScore"
#define FLAG_BLUE_SCORE		"CTF.BlueScore"
#define FLAG_RED_RETURN		"CTF.RedReturn"
#define FLAG_BLUE_RETURN	"CTF.BlueReturn"

class CHL2MP_Player;

// Can have a client class if you want
class CFlagClassic : public CFlagBase
{
public:
	DECLARE_CLASS (CFlagClassic, CFlagBase);
	DECLARE_DATADESC ();

public:
	CFlagClassic ();
	virtual ~CFlagClassic ();

	virtual void Precache( void );
	virtual void Spawn( void );

	virtual void CreateEffects( void );

	virtual bool CreateNoPhysics();
	virtual bool CreateBoxPhysics();
	virtual bool CreateDropPhysics();

	virtual void Think();

	virtual void Pickup( CModPlayer *pPlayer );
	virtual void Drop();
	virtual void ReturnToSpawn( bool scored = false );

	virtual void OnScore();
	virtual void OnPickup();

private:
	CHandle<CSpriteTrail>	m_pGlowTrail;
	bool m_bFirstTake;
};

#endif // CFLAGCLASSIC_H