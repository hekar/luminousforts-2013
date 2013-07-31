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
#include "Classic_SharedDefs.h"

// Classnames for blocks
const char *g_szBlocks[ BLOCK_LAST ] =
{
	"lfc_prop_block1x2",
	"lfc_prop_block1x3",
	"lfc_prop_block1x4",
	"lfc_prop_block1x5",
	"lfc_prop_block2x2",
	"lfc_prop_block2x3",
	"lfc_prop_block2x4",
	"lfc_prop_block2x5",
	"lfc_prop_block3x3",
	"lfc_prop_block3x4",
	"lfc_prop_block3x5",

	"lfc_prop_block1_5x4",
	"lfc_prop_block1x1x2"
};

const char *g_szBlockSpawnerNames[ BLOCK_LAST ] =
{
	"1x2",
	"1x3",
	"1x4",
	"1x5",
	"2x2",
	"2x3",
	"2x4",
	"2x5",
	"3x3",
	"3x4",
	"3x5",

	"1_5x4",
	"1x1x2"
};

const char *g_szBlockModels[ BLOCK_LAST  ] =
{
	"models/construct/blocks/rect_1x2.mdl",
	"models/construct/blocks/rect_1x3.mdl",
	"models/construct/blocks/rect_1x4.mdl",
	"models/construct/blocks/rect_1x5.mdl",
	"models/construct/blocks/rect_2x2.mdl",
	"models/construct/blocks/rect_2x3.mdl",
	"models/construct/blocks/rect_2x4.mdl",
	"models/construct/blocks/rect_2x5.mdl",
	"models/construct/blocks/rect_3x3.mdl",
	"models/construct/blocks/rect_3x4.mdl",
	"models/construct/blocks/rect_3x5.mdl",

	"models/construct/blocks/rect_1x4.mdl",
	"models/construct/blocks/rect_3x5.mdl"
};
