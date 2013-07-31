/* ***** BEGIN LICENSE BLOCK *****
Version: MPL 1.1/LGPL 2.1/GPL 2.0

The contents of this file are subject to the Mozilla Public License Version 
1.1 (the "License"); you may not use this file except in compliance with
...
for the specific language governing rights and limitations under the
License.

The Original Code is for Luminous Forts.

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
	Block Entities derieved from CBlockBase
	Last Updated Sept 23, 2009
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/

#include "cbase.h"
#if 0
#include "hl2mp_player.h"
#include "Mod/CBlockBase.h"
#include "Mod/CBlockEntities.h"


// Change health depending on block type?
// Give less health to 3D blocks so people stop using them?
// Don't show block percentage in HUD display, but instead the actual health points
// Increase impact of some blocks

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	CBlock1x5
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
LINK_ENTITY_TO_CLASS( lfc_prop_block1x5, CBlock1x5 )

BEGIN_DATADESC( CBlock1x5 )

END_DATADESC()


CBlock1x5::CBlock1x5 ()
{
	SetModelName( MAKE_STRING ("models/sf/2d_1x5.mdl") );
}

void CBlock1x5::Precache (void)
{
	PropBreakablePrecacheAll(MAKE_STRING("models/sf/2d_1x5.mdl"));

	BaseClass::Precache ();
}


/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	CBlock2x3
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
LINK_ENTITY_TO_CLASS( lfc_prop_block2x3, CBlock2x3 )

BEGIN_DATADESC( CBlock2x3 )

END_DATADESC()

CBlock2x3::CBlock2x3 ()
{
	SetModelName( MAKE_STRING ("models/sf/2d_2x3.mdl") );
}

void CBlock2x3::Precache (void)
{
	PropBreakablePrecacheAll(MAKE_STRING("models/sf/2d_2x3.mdl"));

	BaseClass::Precache ();
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	CBlock2x2
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
LINK_ENTITY_TO_CLASS( lfc_prop_block2x2, CBlock2x2 )

BEGIN_DATADESC( CBlock2x2 )

END_DATADESC()

CBlock2x2::CBlock2x2 ()
{
	SetModelName( MAKE_STRING ("models/sf/2d_2x2.mdl") );
}

void CBlock2x2::Precache (void)
{
	PropBreakablePrecacheAll(MAKE_STRING("models/sf/2d_2x2.mdl"));

	BaseClass::Precache ();
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	CBlock1_5x4		
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
LINK_ENTITY_TO_CLASS( lfc_prop_block1_5x4, CBlock1_5x4 )

BEGIN_DATADESC( CBlock1_5x4 )

END_DATADESC()

CBlock1_5x4::CBlock1_5x4 ()
{
	SetModelName( MAKE_STRING ("models/sf/2d_1o5x4.mdl") );
}

void CBlock1_5x4::Precache (void)
{
	PropBreakablePrecacheAll(MAKE_STRING("models/sf/2d_1o5x4.mdl"));

	BaseClass::Precache ();
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	CBlock1x2
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
LINK_ENTITY_TO_CLASS( lfc_prop_block1x2, CBlock1x2 )

BEGIN_DATADESC( CBlock1x2 )

END_DATADESC()

CBlock1x2::CBlock1x2 ()
{
	SetModelName( MAKE_STRING ("models/sf/2d_1x2.mdl") );
}

void CBlock1x2::Precache (void)
{
	PropBreakablePrecacheAll(MAKE_STRING("models/sf/2d_1x2.mdl"));

	BaseClass::Precache ();
}

/*=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	CBlock1x2
		TODO for CBlock1x2:
			-Change Weight and health characteristics
=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=*/
LINK_ENTITY_TO_CLASS( lfc_prop_block1x1x2, CBlock1x1x2 )

BEGIN_DATADESC( CBlock1x1x2 )

END_DATADESC()

CBlock1x1x2::CBlock1x1x2 ()
{
	SetModelName( MAKE_STRING ("models/sf/3d_1x2.mdl") );
}

void CBlock1x1x2::Precache (void)
{
	PropBreakablePrecacheAll(MAKE_STRING("models/sf/3d_1x2.mdl"));

	BaseClass::Precache ();
}
#endif // 0
