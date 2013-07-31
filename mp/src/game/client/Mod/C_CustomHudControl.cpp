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
#include "Hud_Base.h"
#include "C_CustomHudControl.h"

LINK_ENTITY_TO_CLASS( lfcus_manager_hud, C_CustomHudControl );
IMPLEMENT_CLIENTCLASS_DT( C_CustomHudControl, DT_CustomHudControl, CCustomHudControl )
	RecvPropInt(RECVINFO(m_iHudElements))
END_RECV_TABLE()

C_CustomHudControl::C_CustomHudControl()
{
	m_iLastHudElements = 0;
	m_iHudElements = -1;
}

C_CustomHudControl::~C_CustomHudControl()
{
}

void C_CustomHudControl::UpdateShownHudElements()
{
	g_ModHudManager.SetEnabledHudElementMask( m_iHudElements );
}

void C_CustomHudControl::OnDataChanged( DataUpdateType_t updateType )
{
	BaseClass::OnDataChanged( updateType );
	
	if ( m_iLastHudElements != m_iHudElements )
	{
		UpdateShownHudElements();
		m_iLastHudElements = m_iHudElements;
	}
}
