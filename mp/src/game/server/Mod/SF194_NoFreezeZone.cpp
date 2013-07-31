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
	SF NoFreezeBaseZone Brush Entity

	For backwards compatibility with SF 194.

	NoFreezeZone for SF194. Have to implement one for LF first.

	Last Updated Dec 08, 2009
===============================================================*/
#include "cbase.h"
#ifdef MOD_SF194
#include "CNoFreezeBase.h"

class NoFreezeSF194 : public CNoFreezeBase
{
	DECLARE_CLASS( NoFreezeSF194, CNoFreezeBase )
	DECLARE_DATADESC()
public:
	NoFreezeSF194();
	~NoFreezeSF194();
};

LINK_ENTITY_TO_CLASS( func_nofreeze, NoFreezeSF194 );

BEGIN_DATADESC( NoFreezeSF194 )
	
	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "TeamNum" ),
	DEFINE_ENTITYFUNC( Touch ),

END_DATADESC()


//
//	Name: NoFreezeSF194
//	Author: Hekar Khani
//	Description: NoFreezeBase zone for Sourceforts 194 map compatibility
//	Notes: 
//
NoFreezeSF194::NoFreezeSF194()
{
}

NoFreezeSF194::~NoFreezeSF194()
{
}

#endif // MOD_SF194