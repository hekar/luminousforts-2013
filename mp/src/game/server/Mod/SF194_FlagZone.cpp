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
	SF CTF Flag Capturing Zone Brush Entity

	For backwards compatibility with SF 194.

	Brush entity from SF194 that states where flag captures take
	place.

	Last Updated Dec 08, 2009
===============================================================*/

#include "cbase.h"
#ifdef MOD_SF194
#include "CFlagZoneBase.h"
#include "CFlagZoneClassic.h"

class CFlagZoneSF194 : public CFlagZoneClassic
{
	DECLARE_CLASS( CFlagZoneSF194, CFlagZoneClassic )
	DECLARE_DATADESC()
public:
	CFlagZoneSF194();
	~CFlagZoneSF194();
};

LINK_ENTITY_TO_CLASS( func_flagzone, CFlagZoneSF194 );

BEGIN_DATADESC( CFlagZoneSF194 )
	
	DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "team" ),

END_DATADESC()

//
//	Name: CFlagZoneSF194
//	Author: Hekar Khani
//	Description: Flag capture zone for SF194 map backwards compatibility
//	Notes:
//
CFlagZoneSF194::CFlagZoneSF194()
{
}

CFlagZoneSF194::~CFlagZoneSF194()
{
}


#endif // MOD_SF194