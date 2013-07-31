
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
#include "Mod/SharedModCvars.h"
#include "Mod/CustomBlocks.h"

BEGIN_DATADESC( CCustomBlockBase )
	DEFINE_KEYFIELD( m_iDesiredWorth, FIELD_INTEGER, "BlockWorth" ),
	DEFINE_KEYFIELD( m_bDestructable, FIELD_BOOLEAN, "Destructable" ),
	DEFINE_KEYFIELD( m_Mass, FIELD_FLOAT, "Mass" ),

	DEFINE_INPUTFUNC( FIELD_VOID, "Dispose", InputDispose ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableDestructable", InputDisableDestructable ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableDestructable", InputEnableDestructable ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "SetHealth", InputSetHealth ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "SetMass", InputSetMass ),

	DEFINE_OUTPUT( m_OnDisposed, "OnDisposed" ),
END_DATADESC()

//
//	Name: CCustomBlockBase
//	Author: Hekar Khani
//	Description: Base block for custom blocks. Adds some functionality.
//
CCustomBlockBase::CCustomBlockBase()
{
	SetBlockWorth( m_iDesiredWorth );
	m_bDestructable = lf_block_destructable.GetBool();
	m_Mass = 200;
}

CCustomBlockBase::~CCustomBlockBase()
{
}

void CCustomBlockBase::Precache()
{
	PropBreakablePrecacheAll( GetModelName() );

	BaseClass::Precache();
}

void CCustomBlockBase::Spawn()
{
	BaseClass::Spawn();

	if ( lf_block_destructable.GetInt() < 0 )
	{
		m_bDestructable = true;
	}

	//SetMass( m_Mass );
}

void CCustomBlockBase::InputDispose( inputdata_t& data )
{
	UTIL_Remove( this );
}

void CCustomBlockBase::InputEnableDestructable( inputdata_t& data )
{
	m_bDestructable = true;
}

void CCustomBlockBase::InputDisableDestructable( inputdata_t& data )
{
	m_bDestructable = false;
}

void CCustomBlockBase::InputSetHealth( inputdata_t& data )
{
	UpdateHealth( data.value.Int(), data.pActivator );
	SetMaxHealth( data.value.Int() );
}

void CCustomBlockBase::SetHealth( int iHealth )
{
	if ( GetMaxHealth() < iHealth )
	{
		SetMaxHealth( iHealth );
	}

	UpdateHealth( iHealth, this );
}

void CCustomBlockBase::InputSetMass( inputdata_t& data )
{
	SetMass( data.value.Float() );
}

int CCustomBlockBase::OnTakeDamage( const CTakeDamageInfo &inputInfo )
{
	if ( !m_bDestructable )
	{
		return 0;
	}

	return BaseClass::OnTakeDamage( inputInfo );
}

bool CCustomBlockBase::UpdateHealth( int iNewHealth, CBaseEntity *pActivator )
{
	if ( iNewHealth != GetHealth() )
	{
		m_OnHealthChanged.FireOutput( pActivator, this, 0.0f );
	}

	return BaseClass::UpdateHealth( iNewHealth, pActivator );
}

void CCustomBlockBase::OnDisposed( CBaseEntity *pActivator )
{
	m_OnDisposed.FireOutput( pActivator, this, 0.0f );
	BaseClass::OnDisposed( pActivator );
}

void CCustomBlockBase::Event_Killed( const CTakeDamageInfo &info )
{
	m_OnKilled.FireOutput( info.GetAttacker(), this, 0.0f );
	BaseClass::Event_Killed( info );
}


//
//	Name: CCustomDynamicBlock
//	Author: Hekar Khani
//	Description: Prop for a custom block that the mapper defines
//
LINK_ENTITY_TO_CLASS( lfcus_prop_block_custom, CCustomDynamicBlock );

BEGIN_DATADESC( CCustomDynamicBlock )

	DEFINE_KEYFIELD( m_iDesiredHealth, FIELD_INTEGER, "BlockHealth" ),
	DEFINE_KEYFIELD( m_iszCustomModelName, FIELD_STRING, "BlockModel" ),

END_DATADESC()

CCustomDynamicBlock::CCustomDynamicBlock()
{
	SetModelName( m_iszCustomModelName );
}

CCustomDynamicBlock::~CCustomDynamicBlock()
{
}

void CCustomDynamicBlock::Spawn()
{
	BaseClass::Spawn();
}
