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
	SF Blocks Model Entity

	Rereferencing of block entity names For backwards 
	compatibility with Sourceforts 194 maps

	Last Updated Dec 08, 2009
===============================================================*/
#include "cbase.h"

#ifdef MOD_SF194
#include "CBlockBase.h"
#include "CBlockClassic.h"

// Reference Block classes to SF194 entity names
LINK_ENTITY_TO_CLASS( prop_block_2d_2x3, CBlock2x3 )
LINK_ENTITY_TO_CLASS( prop_block_2d_1x2, CBlock1x2 )
LINK_ENTITY_TO_CLASS( prop_block_2d_1x5, CBlock1x5 )
LINK_ENTITY_TO_CLASS( prop_block_2d_5x4, CBlock1_5x4 )
LINK_ENTITY_TO_CLASS( prop_block_2d_2x2, CBlock2x2 )
LINK_ENTITY_TO_CLASS( prop_block_3d_1x2, CBlock1x1x2 )

#endif // MOD_SF194