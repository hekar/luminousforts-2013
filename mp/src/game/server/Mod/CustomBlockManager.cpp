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
#include "team.h"
#include "Mod/SharedModCvars.h"
#include "Mod/CustomBlockManager.h"


LINK_ENTITY_TO_CLASS( lfcus_block_manager, CustomBlockManager );

BEGIN_DATADESC( CustomBlockManager )

	// Generic hand spawner inputs
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableHandSpawner", InputDisableHandSpawner ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableHandSpawner", InputEnableHandSpawner ),

	// Team based hand spawner inputs
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableRedHandSpawner", InputDisableRedHandSpawner ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableRedHandSpawner", InputEnableRedHandSpawner ),
	DEFINE_INPUTFUNC( FIELD_VOID, "DisableBlueHandSpawner", InputDisableBlueHandSpawner ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableBlueHandSpawner", InputEnableBlueHandSpawner ),

	DEFINE_INPUTFUNC( FIELD_VOID, "DisableBlockDestruction", InputDisableBlockDestruction ),
	DEFINE_INPUTFUNC( FIELD_VOID, "EnableBlockDestruction", InputEnableBlockDestruction ),

	DEFINE_KEYFIELD( m_iRedLimit, FIELD_INTEGER, "RedBlockLimit" ),
	DEFINE_KEYFIELD( m_iBlueLimit, FIELD_INTEGER, "BlueBlockLimit" ),

	// Blocklimit inputs
	DEFINE_INPUTFUNC( FIELD_INTEGER, "SetBlockLimit", InputSetBlockLimit ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "SetRedBlockLimit", InputSetRedBlockLimit ),
	DEFINE_INPUTFUNC( FIELD_INTEGER, "SetBlueBlockLimit", InputSetBlueBlockLimit ),

	// outputs
	DEFINE_OUTPUT( m_OnRedBlockSpawned, "OnRedBlockSpawned" ),
	DEFINE_OUTPUT( m_OnBlueBlockSpawned, "OnBlueBlockSpawned" ),

	DEFINE_OUTPUT( m_OnRedBlockLimitReached, "OnRedBlockLimitReached" ),
	DEFINE_OUTPUT( m_OnBlueBlockLimitReached, "OnBlueBlockLimitReached" ),
	
END_DATADESC()

CustomBlockManager::CustomBlockManager()
{
}

CustomBlockManager::~CustomBlockManager()
{
}

void CustomBlockManager::Spawn()
{
	m_iRedServerBlockLimit = lf_block_red_limit.GetInt();
	m_iBlueServerBlockLimit = lf_block_blue_limit.GetInt();

	lf_block_red_limit.SetValue( m_iRedLimit );
	lf_block_blue_limit.SetValue( m_iBlueLimit );

	for ( int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++ )
	{
		m_iLastBlockCount[ i ] = 0;
	}

	SetThink( &CustomBlockManager::Think );
	SetNextThink( 1.0f );
}

void CustomBlockManager::UpdateOnRemove()
{
	BaseClass::UpdateOnRemove();

	lf_block_red_limit.SetValue( m_iRedServerBlockLimit );
	lf_block_blue_limit.SetValue( m_iBlueServerBlockLimit );
}

void CustomBlockManager::Think()
{
	// check block counts
	for ( int i = SDK_TEAM_BLUE; i <= SDK_TEAM_RED; i++ )
	{
		CTeam *team = GetGlobalTeam( i );
		int blockcount = team->GetBlockCount();

		for ( int j = 0; j < ( blockcount - m_iLastBlockCount[ i ] ); j++ )
		{
			variant_t v;
			v.SetInt( blockcount );

			// Change to array later?
			if ( i == SDK_TEAM_RED )
			{
				v.SetInt( blockcount );
				m_OnRedBlockSpawned.FireOutput( v, this, this );
			}
			else if ( i == SDK_TEAM_BLUE )
			{
				m_OnBlueBlockSpawned.FireOutput( v, this, this );
			}
		}

		m_iLastBlockCount[ i ] = blockcount;
	}
}

void CustomBlockManager::InputDisableHandSpawner( inputdata_t& data )
{
	lf_hand_spawn_enabled.SetValue( MOD_TEAM_BOTH );
}

void CustomBlockManager::InputEnableHandSpawner( inputdata_t& data )
{
	lf_hand_spawn_enabled.SetValue( -1 );
}

void CustomBlockManager::InputDisableRedHandSpawner( inputdata_t& data )
{
	lf_hand_spawn_enabled.SetValue( lf_hand_spawn_enabled.GetInt() & ~MOD_TEAM_RED );
}

void CustomBlockManager::InputEnableRedHandSpawner( inputdata_t& data )
{
	lf_hand_spawn_enabled.SetValue( lf_hand_spawn_enabled.GetInt() | MOD_TEAM_RED );
}

void CustomBlockManager::InputDisableBlueHandSpawner( inputdata_t& data )
{
	lf_hand_spawn_enabled.SetValue( lf_hand_spawn_enabled.GetInt() & ~MOD_TEAM_BLUE );
}

void CustomBlockManager::InputEnableBlueHandSpawner( inputdata_t& data )
{
	lf_hand_spawn_enabled.SetValue( lf_hand_spawn_enabled.GetInt() | MOD_TEAM_BLUE );
}

void CustomBlockManager::InputDisableBlockDestruction( inputdata_t& data )
{
	lf_block_destructable.SetValue( FALSE );
}

void CustomBlockManager::InputEnableBlockDestruction( inputdata_t& data )
{
	lf_block_destructable.SetValue( TRUE );
}

void CustomBlockManager::InputSetBlockLimit( inputdata_t& data )
{
	lf_block_red_limit.SetValue( data.value.Int() );
	lf_block_blue_limit.SetValue( data.value.Int() );
}

void CustomBlockManager::InputSetRedBlockLimit( inputdata_t& data )
{
	lf_block_red_limit.SetValue( data.value.Int() );
}

void CustomBlockManager::InputSetBlueBlockLimit( inputdata_t& data )
{
	lf_block_blue_limit.SetValue( data.value.Int() );
}
