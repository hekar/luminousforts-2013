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


/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	Client/Server

	Base Block Entity for all other blocks to derive from.

	TODO for next dev cycle:
		Build a block creator similar to the playerclass
		system that reads keyvalues and then create a modified
		block entity based on it. Call it CBlockDynamic.

	// Cleaned up Jan 21, 2010
	Last Updated Sept 19, 2009
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

#include "cbase.h"
#include "team.h"
#include "hl2mp_player.h"
#include "hl2mp_player_shared.h"
#include "Mod/ClassicGameRules.h"
#include "Mod/SharedModCvars.h"
#include "Mod/FileConfig.h"
#include "Mod/CBlockBase.h"

CFileConfig g_Block( "blocks", "config/entities/blocks.txt" );

static int g_iBlockHealthList [LF_BLOCK_SKIN_COUNT] = {101, 80, 50, 30, 15, 7, 0};

LINK_ENTITY_TO_CLASS (BlockBase, CBlockBase)

BEGIN_DATADESC(CBlockBase)

	DEFINE_KEYFIELD( m_bSpawnBlockCount, FIELD_BOOLEAN, "CountBlock" ),
	DEFINE_KEYFIELD( m_bSpawnFrozen, FIELD_BOOLEAN, "Frozen" ),
	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "team" ),

	DEFINE_ENTITYFUNC( Touch ),
	DEFINE_THINKFUNC( Think ),
	DEFINE_INPUTFUNC( FIELD_VOID, "Freeze", InputFreeze ),
	DEFINE_INPUTFUNC( FIELD_VOID, "UnFreeze", InputUnFreeze ),

	DEFINE_OUTPUT( m_OnUnFrozen, "OnUnFrozen" ),
	DEFINE_OUTPUT( m_OnFrozen, "OnFrozen" ),

END_DATADESC()

IMPLEMENT_SERVERCLASS_ST( CBlockBase, DT_BlockBase )
	SendPropInt( SENDINFO( m_iHealth ) ),
	SendPropInt( SENDINFO( m_iMaxHealth ) ),
	SendPropInt( SENDINFO( m_BlockType ) ),
	SendPropInt( SENDINFO( m_BlockWorth ) ),
	SendPropInt( SENDINFO( m_iUserIDFreezer ) ),
	SendPropInt( SENDINFO( m_iUserIDUnFreezer ) ),
END_SEND_TABLE()


//
//	Name: CBlockBase
//	Author: Hekar Khani
//	Description: Base class for all damageable blocks in Sourceforts
//	Notes: (This was one of the first pieces of code written for LF)
//
CBlockBase::CBlockBase()
{
	if ( !g_Block.IsLoaded() )
	{
		g_Block.Load();
	}

	SetModelName( MAKE_STRING( NULL ) );

	m_takedamage = DAMAGE_YES;

	m_bSpawnBlockCount = false;
	m_bSpawnFrozen = false;

	m_bFrozen = false;
	m_bFreezeable = true;

	m_bHeldByPlayer = false;

	m_iDesiredHealth = lf_block_health.GetInt();

	SetBlockType( BLOCK_NONE );
	SetBlockWorth( LF_BLOCK_DEFAULT_WORTH );

	SetCurrentFreezer( NULL );

	ListenForGameEvent( "phase_switch" );
}

CBlockBase::~CBlockBase()
{
	StopListeningForAllEvents();
}

void CBlockBase::Precache()
{
	BaseClass::Precache();
}

void CBlockBase::Spawn()
{
	Precache();

	SetModel( GetModelName().ToCStr() );
	SetSolid( SOLID_VPHYSICS );

	SetTouch( &CBlockBase::Touch );
	SetThink( &CBlockBase::Think );
	SetNextThink( 5.0f );

	m_nSkin = TeamStartSkinNum();
	BaseClass::Spawn();

	SetThink( &CBlockBase::Think );
	SetNextThink( gpGlobals->curtime + 0.5f );

	ChangeTeam( GetTeamNumber() );

	SetMaxHealth( m_iDesiredHealth );
	UpdateHealth( m_iDesiredHealth, this );
	SetRenderMode( kRenderTransTexture );

	if ( m_bSpawnFrozen )
	{
		Freeze( NULL );
	}
	else if ( m_bSpawnBlockCount && 
		( GetTeamNumber() == SDK_TEAM_BLUE || GetTeamNumber() == SDK_TEAM_RED ) )
	{
		CTeam *pTeam = static_cast< CTeam * >( GetTeam() );
		pTeam->AddBlockCount( GetBlockWorth() );
	}
}

void CBlockBase::Touch( CBaseEntity *pOther )
{
	BaseClass::Touch( pOther );
}

void CBlockBase::Think()
{
	const color32& clr = GetRenderColor();
	if ( ( clr.a < 255 ) && !m_bHeldByPlayer )
	{
		SetRenderColorA( 255 );
	}

	// Can a think run at the sametime as a touch? (ie. is it multithreaded?)
	SetNextThink( gpGlobals->curtime + 5.0f );
}

void CBlockBase::OnPhysGunPickup( CBasePlayer *pPhysGunUser, PhysGunPickup_t reason )
{
	BaseClass::OnPhysGunPickup( pPhysGunUser, reason );
	if ( reason != PUNTED_BY_CANNON )
	{
		if ( IsPlayingTeam( pPhysGunUser->GetTeamNumber() ) && GetTeamNumber() != pPhysGunUser->GetTeamNumber() )
		{
			CTeam *pTeam = (CTeam *) GetTeam();
			if ( pTeam )
			{
				pTeam->AddBlockCount( -GetBlockWorth( ) );
				pTeam->AddStolenBlockCount( -GetBlockWorth( ) );
			}
			
			CTeam *pNewTeam = (CTeam *) pPhysGunUser->GetTeam();
			if ( pNewTeam )
			{
				pNewTeam->AddBlockCount( GetBlockWorth( ) );
				pNewTeam->AddStolenBlockCount( GetBlockWorth( ) );
			}

			ChangeTeam( pPhysGunUser->GetTeamNumber() );
		}

		SetRenderColorA( byte( 150 ) );

		CHL2MP_Player *pPlayer = dynamic_cast< CHL2MP_Player * > ( pPhysGunUser );
		if ( pPlayer )
		{
			//pPlayer->m_Shared.SetAllowedToSprint( false );
		}
		
		SetCurrentFreezer( pPhysGunUser );

		// Allow players to pass through block
		SetCollisionGroup( COLLISION_GROUP_BLOCKHELD );
		m_bHeldByPlayer = true;
	}
	else
	{
		SetCollisionGroup( COLLISION_GROUP_BLOCKBASE );
		m_bHeldByPlayer = false;
	}
}

void CBlockBase::OnPhysGunDrop( CBasePlayer *pPhysGunUser, PhysGunDrop_t Reason )
{
	BaseClass::OnPhysGunDrop( pPhysGunUser, Reason );
	SetRenderColorA( byte( 255 ) );

	SetCurrentFreezer( NULL );

	CHL2MP_Player *pPlayer = dynamic_cast< CHL2MP_Player * > ( pPhysGunUser );
	if ( pPlayer )
	{
		//pPlayer->m_Shared.SetAllowedToSprint( true );
	}

	SetCollisionGroup( COLLISION_GROUP_BLOCKBASE );
	m_bHeldByPlayer = false;
}

int CBlockBase::OnTakeDamage( const CTakeDamageInfo &inputInfo )
{
	if ( GameRules()->GetGameModeMask() & GAMEMODE_CLASSIC )
	{
		if ( ClassicGameRules()->GetCurrentPhaseID() == PHASE_BUILD)
		{
			return 0;
		}
	}

	CTakeDamageInfo Info = inputInfo;
	Info.AddDamageType( DMG_NEVERGIB );

	if ( IsFrozen() )
	{
		Info.ScaleDamage( LF_BLOCK_DMGSCALE_FROZEN );
	}
	else
	{
		Info.ScaleDamage( LF_BLOCK_DMGSCALE_UNFROZEN );
	}

	// Scale weapon damage here
	if ( !Q_strcmp ( Info.GetAmmoName(), "RPG_Round" ) )
	{
		DevMsg ("Damage Not Scaled %f\n", Info.GetDamage());
		Info.ScaleDamage( 0.3f );
		DevMsg ("Damage Scaled %f\n", Info.GetDamage());
	}
	int RetValue = BaseClass::OnTakeDamage( Info );

	if (!UpdateHealth( GetHealth() - Info.GetDamage(), this ))
	{
		return RetValue;
	}

	ChangeSkinOnHealth( GetHealth() );

	return RetValue;
}

void CBlockBase::InputFreeze( inputdata_t &data )
{
	// Freeze by World
	Freeze( NULL, FROZEN_BY_WORLD );
}

void CBlockBase::InputUnFreeze( inputdata_t &data )
{
	// UnFreeze by World
	UnFreeze( NULL, FROZEN_BY_WORLD );
}

void CBlockBase::InputKill( inputdata_t &data )
{
	UTIL_Remove( this );
}

void CBlockBase::ChangeTeam( int iTeamNum )
{
	m_nSkin -= TeamStartSkinNum();

	BaseClass::ChangeTeam( iTeamNum );

	m_nSkin += TeamStartSkinNum();
}

// yes, this function is brilliant...
int CBlockBase::TeamStartSkinNum()
{
	if ( GetTeamNumber() == TEAM_RED )
	{
		return 14;
	}
	else if ( GetTeamNumber() == TEAM_BLUE )
	{
		return 7;
	}

	return 0;
}

void CBlockBase::ChangeSkinOnHealth( int iHealth )
{
	int StartNumber = TeamStartSkinNum();
	for (int i = 0; i < LF_BLOCK_SKIN_COUNT; i++)
	{
		int iUpperHealth = (int) (GetMaxHealth() * ((float)g_iBlockHealthList [i] / 100));
		int iLowerHealth = (int) (GetMaxHealth() * ((float)g_iBlockHealthList [i + 1] / 100));

		if (iUpperHealth > iHealth && iLowerHealth < iHealth)
		{
			m_nSkin = i + StartNumber;
		}
	}
}

void CBlockBase::ToggleFreeze( CBasePlayer *pPlayer )
{
	if ( m_bFreezeable && !m_bFrozen )
	{
		if ( pPlayer )
		{
			ClientPrint( pPlayer, HUD_PRINTCENTER, "Block frozen" );
		}

		Freeze( pPlayer );
	}
	else if ( !m_bFreezeable )
	{
		if ( pPlayer )
		{
			ClientPrint( pPlayer, HUD_PRINTCENTER, "Cannot freeze here" );
		}
	}
	else
	{
		if ( pPlayer )
		{
			ClientPrint( pPlayer, HUD_PRINTCENTER, "Block unfrozen" );
		}

		UnFreeze( pPlayer );
	}
}

bool CBlockBase::UpdateHealth( int iNewHealth, CBaseEntity *pActivator )
{
	if ( iNewHealth != m_iHealth )
	{
		m_iHealth = iNewHealth;

		if ( m_iMaxHealth == 0 )
		{
			Assert( false );
			m_iMaxHealth = 1;
		}

		// Output the new health as a percentage of max health [0..1]
		float flRatio = clamp( (float)m_iHealth / (float)m_iMaxHealth, 0, 1 );
		m_OnHealthChanged.Set( flRatio, pActivator, this );

		if ( m_iHealth <= 0 )
		{
			CTakeDamageInfo info;
			info.SetAttacker( this );
			Dissolve( NULL, gpGlobals->curtime, false, ENTITY_DISSOLVE_ELECTRICAL );
			return false;
		}
	}

	return true;
}

void CBlockBase::Event_Killed( const CTakeDamageInfo &info )
{
	BaseClass::Event_Killed( info );
}

void CBlockBase::OnDisposed( CBaseEntity *pActivator )
{
}

void CBlockBase::OnPhaseChange( int Phaseid )
{
	// Get rid of transparency on phase switch
	SetRenderColorA( byte( 255 ) );
	Freeze( NULL, FROZEN_BY_NONE );

	m_nSkin = TeamStartSkinNum();
	
	UpdateHealth( m_iMaxHealth, this );
}

void CBlockBase::FireGameEvent( IGameEvent* pEvent )
{
	if ( !Q_strcmp( "phase_switch", pEvent->GetName() ) )
	{
		OnPhaseChange( pEvent->GetInt( "phaseid" ) );
	}
}

void CBlockBase::Freeze( CBasePlayer *pPlayer, int FrozenBy )
{
	if ( m_bFreezeable )
	{
		IPhysicsObject *pPhysicsObject = VPhysicsGetObject();
		if ( pPhysicsObject != NULL )
		{
			pPhysicsObject->EnableMotion( false );

			switch ( FrozenBy )
			{
			case FROZEN_BY_PLAYER:
				if ( pPlayer )
				{
					SetLastFrozenUserID( pPlayer->GetUserID() );
				}
				else
				{
					SetLastFrozenUserID( FROZEN_BY_WORLD );
				}
				break;

			case FROZEN_BY_WORLD:
				SetLastFrozenUserID( FROZEN_BY_WORLD );
				break;
			case FROZEN_BY_NONE:
				// Do Nothing...
				break;
			default:
				break;
			}

			m_bFrozen = true;
			m_OnFrozen.FireOutput( this, this, 0.0f );
		}
	}
}

void CBlockBase::UnFreeze( CBasePlayer *pPlayer, int FrozenBy )
{
	EnableMotion();

	m_bFrozen = false;
	m_OnUnFrozen.FireOutput( this, this, 0.0f );
	
	switch ( FrozenBy )
	{
	case FROZEN_BY_PLAYER:
		if ( pPlayer )
		{
			SetLastUnFrozenUserID( pPlayer->GetUserID() );
		}
		else
		{
			SetLastUnFrozenUserID( FROZEN_BY_WORLD );
		}
		break;

	case FROZEN_BY_WORLD:
		SetLastUnFrozenUserID( FROZEN_BY_WORLD );
		break;
	case FROZEN_BY_NONE:
		// Do Nothing...
		break;
	default:
		break;
	}
}

void CBlockBase::SetBlockWorthFromConfig( const char *blockworthkey )
{
	if ( lf_block_worth_system.GetBool() )
	{
		SetBlockWorth( g_Block.All()->GetInt( blockworthkey, LF_BLOCK_DEFAULT_WORTH ) );
	}
	else
	{
		SetBlockWorth( 1 );
	}
}

void CBlockBase::SetBlockHealthFromConfig( const char *healthKey )
{
	if ( lf_block_unique_health.GetBool() )
	{
		SetHealth( g_Block.All()->GetInt( healthKey, lf_block_health.GetInt() ) );
	}
	else
	{
		SetHealth( lf_block_health.GetInt() );
	}
}

void CBlockBase::SetLastFrozenUserID( int UserID )
{
	m_iUserIDFreezer = UserID;
}

void CBlockBase::SetLastUnFrozenUserID( int UserID )
{
	m_iUserIDUnFreezer = UserID;
}
