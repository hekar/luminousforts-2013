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


#ifndef CUSTOMBLOCKS_H
#define CUSTOMBLOCKS_H

#ifdef WIN32
#pragma once
#endif // WIN32

#include "CBlockBase.h"

class CCustomBlockBase : public CBlockBase
{
	DECLARE_CLASS( CCustomBlockBase, CBlockBase );
	DECLARE_DATADESC();
public:
	CCustomBlockBase();
	~CCustomBlockBase();

	virtual void Precache();
	virtual void Spawn();

	void InputDispose( inputdata_t& data );
	void InputEnableDestructable( inputdata_t& data );
	void InputDisableDestructable( inputdata_t& data );
	void InputSetHealth( inputdata_t& data );
	void InputSetMass( inputdata_t& data );

	virtual void SetHealth( int iHealth );

	virtual int OnTakeDamage( const CTakeDamageInfo &inputInfo );

	virtual bool UpdateHealth( int iNewHealth, CBaseEntity *pActivator );
	virtual void OnDisposed( CBaseEntity *pActivator );

	virtual void Event_Killed( const CTakeDamageInfo &info );

	void SetDestructable( bool destructable )
	{
		m_bDestructable = destructable;
	}

	bool GetDestructable() const
	{
		return m_bDestructable;
	}

	void SetMass( float mass )
	{
		if ( VPhysicsGetObject() != NULL )
		{
			// Disabled for now.
			// VPhysicsGetObject()->SetMass( mass );
		}
	}

	float GetMass() const
	{
		if ( VPhysicsGetObject() == NULL )
		{
			return 0.0f;
		}
		else
		{
			return VPhysicsGetObject()->GetMass();
		}
	}

private:
	bool m_bDestructable;
	int m_iDesiredWorth;
	float m_Mass;

	COutputEvent m_OnKilled;
	COutputEvent m_OnDisposed;
	COutputEvent m_OnHealthChanged;
};

class CCustomDynamicBlock : public CCustomBlockBase
{
	DECLARE_CLASS( CCustomDynamicBlock, CCustomBlockBase );
	DECLARE_DATADESC();
public:
	CCustomDynamicBlock();
	~CCustomDynamicBlock();

	void Spawn();

private:
	string_t m_iszCustomModelName;
};


#endif // CUSTOMBLOCKS_H