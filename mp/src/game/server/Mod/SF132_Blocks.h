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


#ifndef SF132_BLOCKS_H
#define SF132_BLOCKS_H

#ifdef _WIN32
	#pragma once
#endif

#ifdef MOD_SF132

class CSF132BlockBase : public CBlockBase
{
	DECLARE_CLASS( CSF132BlockBase, CBlockBase )
public:
	CSF132BlockBase();
	~CSF132BlockBase();

	void Spawn( void );

	void ChangeTeam( int iTeamNum );

	int OnTakeDamage( const CTakeDamageInfo &inputInfo );
	void OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason );

	void OnPhaseChange( int Phaseid );
};

class CSF132Block1x5 : public CSF132BlockBase
{
	DECLARE_CLASS( CSF132Block1x5, CBlockBase )
public:
	CSF132Block1x5();
};

class CSF132Block2x3 : public CSF132BlockBase
{
	DECLARE_CLASS( CSF132Block2x3, CBlockBase )
public:
	CSF132Block2x3();
};

class CSF132Block1x2x2 : public CSF132BlockBase
{
	DECLARE_CLASS( CSF132Block1x2x2, CBlockBase )
public:
	CSF132Block1x2x2();
};

#endif // MOD_SF132

#endif // SF132_BLOCKS_H