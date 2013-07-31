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


//========= Copyright © 1996-2005, Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#ifndef GRENADE_BEACON_H
#define GRENADE_BEACON_H
#ifdef _WIN32
#pragma once
#endif

#include "basegrenade_shared.h"

class CBeam;

// for constraints
#include "vphysics/constraints.h"


class CBeaconGrenade : public CBaseGrenade
{
public:
	DECLARE_CLASS( CBeaconGrenade, CBaseGrenade );

	CBeaconGrenade();
	~CBeaconGrenade();

	void Spawn( void );
	void Precache( void );
	
	void WarningThink( void );
	void PowerupThink( void );

	void KillParticle( void );
	bool MakeParticle( void );

	// Added to create a constraint
	void AttachToEntity( CBaseEntity *pOther );
	bool MakeConstraint( CBaseEntity *pOther );

public:
	EHANDLE		m_hOwner;
	// Added for following
	EHANDLE		m_hAttachEntity;

private:
	float		m_flPowerUp;
	Vector		m_vecDir;
	Vector		m_vecEnd;
	float		m_flBeamLength;

	Vector		m_posOwner;
	Vector		m_angleOwner;

	// signifies if we're attached to something, and need to update slightly differently.
	bool		m_bAttached;
	IPhysicsConstraint	*m_pConstraint;
	Vector		m_vAttachedPosition;	// if the attached position changes, we need to detonate

	DECLARE_DATADESC();
};

#endif // GRENADE_BEACON_H
