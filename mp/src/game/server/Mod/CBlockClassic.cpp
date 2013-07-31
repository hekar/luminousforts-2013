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
	Block Model Entities

	The block entities for the classic game mode

	// Huge cleanup March 31, 2010
	Last Updated Dec 28, 2009
===============================================================*/

#include "cbase.h"
#include "hl2mp_player.h"
#include "Mod/SharedModCvars.h"
#include "Mod/FileConfig.h"
#include "Mod/CBlockBase.h"
#include "Mod/CBlockClassic.h"

// Change health depending on block type?
// Give less health to 3D blocks so people stop using them?
// Don't show block percentage in HUD display, but instead the actual health points
// Increase impact of some blocks

void CClassicBlockBase::Precache()
{
	PropBreakablePrecacheAll( GetModelName() );
	BaseClass::Precache();
}

void CClassicBlockBase::OnPhaseChange( int phaseid )
{
	BaseClass::OnPhaseChange( phaseid );
	StopSound( "Luminousforts.Repair" );
	StopSound( "Luminousforts.Freeze" );
}

void CClassicBlockBase::Think()
{
	StopSound( "Luminousforts.Repair" );
	BaseClass::Think();
}

DECLARE_BLOCK_CLASSIC( lfc_prop_block1x2, CBlock1x2 )
	SetBlockType( BLOCK_1x2 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "1x2_worth" );
	SetBlockHealthFromConfig( "1x2_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block1x3, CBlock1x3 )
	SetBlockType( BLOCK_1x3 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "1x3_worth" );
	SetBlockHealthFromConfig( "1x3_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block1x4, CBlock1x4 )
	SetBlockType( BLOCK_1x4 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "1x4_worth" );
	SetBlockHealthFromConfig( "1x4_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block1x5, CBlock1x5 )
	SetBlockType( BLOCK_1x5 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "1x5_worth" );
	SetBlockHealthFromConfig( "1x5_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block2x2, CBlock2x2 )
	SetBlockType( BLOCK_2x2 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "2x2_worth" );
	SetBlockHealthFromConfig( "2x2_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block2x3, CBlock2x3 )
	SetBlockType( BLOCK_2x3 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "2x3_worth" );
	SetBlockHealthFromConfig( "2x3_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block2x4, CBlock2x4 )
	SetBlockType( BLOCK_2x4 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "2x4_worth" );
	SetBlockHealthFromConfig( "2x4_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block2x5, CBlock2x5 )
	SetBlockType( BLOCK_2x5 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "2x5_worth" );
	SetBlockHealthFromConfig( "2x5_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block3x3, CBlock3x3 )
	SetBlockType( BLOCK_3x3 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "3x3_worth" );
	SetBlockHealthFromConfig( "3x3_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block3x4, CBlock3x4 )
	SetBlockType( BLOCK_3x4 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "3x4_worth" );
	SetBlockHealthFromConfig( "3x4_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block3x5, CBlock3x5 )
	SetBlockType( BLOCK_3x5 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "3x5_worth" );
	SetBlockHealthFromConfig( "3x5_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block1_5x4, CBlock1_5x4 )
	SetBlockType( BLOCK_1_5x4 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "1_5x4_worth" );
	SetBlockHealthFromConfig( "1_5x4_health" );
END_BLOCK_CLASSIC()

DECLARE_BLOCK_CLASSIC( lfc_prop_block1x1x2, CBlock1x1x2 )
	SetBlockType( BLOCK_1x1x2 );
	SetModelName( MAKE_STRING( g_szBlockModels[ GetBlockType() ] ) );
	SetBlockWorthFromConfig( "1x1x2_worth" );
	SetBlockHealthFromConfig( "1x1x2_health" );
END_BLOCK_CLASSIC()
