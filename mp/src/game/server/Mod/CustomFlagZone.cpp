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
#include "CFlagZoneBase.h"
#include "CFlagZoneClassic.h"
#include "CustomFlagZone.h"

LINK_ENTITY_TO_CLASS( lfcus_trigger_flagzone, CCustomFlagZone );

BEGIN_DATADESC( CCustomFlagZone )
	DEFINE_OUTPUT( m_OnCapture, "OnCapture" ),
	DEFINE_OUTPUT( m_OnRedCapture, "OnRedCapture" ),
	DEFINE_OUTPUT( m_OnBlueCapture, "OnBlueCapture" ),
END_DATADESC()

void CCustomFlagZone::OnCapture( int teamnum )
{
	m_OnCapture.FireOutput( this, this, 0.0f );
	m_OnRedCapture.FireOutput( this, this, 0.0f );
	m_OnBlueCapture.FireOutput( this, this, 0.0f );
}
