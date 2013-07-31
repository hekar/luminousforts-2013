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

#ifndef CBLOCKCLASSIC_H
#define CBLOCKCLASSIC_H

#ifdef _WIN32
	#pragma once
#endif

#include "CustomBlocks.h"

/*!
 * Macro definitions for making it easier to create new blocks
 */
#define DEFINE_BLOCK_CLASSIC(classname) \
	class classname : public CClassicBlockBase \
	{ \
		DECLARE_CLASS( classname , CClassicBlockBase ); \
	public: \
		classname (); \
	};

#define DECLARE_BLOCK_CLASSIC(entityname, classname) \
	LINK_ENTITY_TO_CLASS( entityname, classname ) \
	classname::classname() \
	{
		
#define END_BLOCK_CLASSIC() \
	}

class CClassicBlockBase : public CCustomBlockBase
{
	DECLARE_CLASS( CClassicBlockBase, CCustomBlockBase );
public:
	void Precache();
	virtual void OnPhaseChange( int phaseid );
	virtual void Think();
};

DEFINE_BLOCK_CLASSIC(CBlock1x2)
DEFINE_BLOCK_CLASSIC(CBlock1x3)
DEFINE_BLOCK_CLASSIC(CBlock1x4)
DEFINE_BLOCK_CLASSIC(CBlock1x5)
DEFINE_BLOCK_CLASSIC(CBlock2x2)
DEFINE_BLOCK_CLASSIC(CBlock2x3)
DEFINE_BLOCK_CLASSIC(CBlock2x4)
DEFINE_BLOCK_CLASSIC(CBlock2x5)
DEFINE_BLOCK_CLASSIC(CBlock3x3)
DEFINE_BLOCK_CLASSIC(CBlock3x4)
DEFINE_BLOCK_CLASSIC(CBlock3x5)

DEFINE_BLOCK_CLASSIC(CBlock1_5x4)
DEFINE_BLOCK_CLASSIC(CBlock1x1x2)


#endif // CBLOCKCLASSIC_H
