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
	Client/Server

	Client side of Blocks.
	Again for graphical effects if desired.

	Last Updated Aug 02, 2009
===============================================================*/
#include "cbase.h"
#include "C_BlockBase.h"

IMPLEMENT_CLIENTCLASS_DT( C_BlockBase, DT_BlockBase, CBlockBase )
	RecvPropInt( RECVINFO( m_iHealth ) ),
	RecvPropInt( RECVINFO( m_iMaxHealth ) ),
	RecvPropInt( RECVINFO( m_iUserIDFreezer ) ),
	RecvPropInt( RECVINFO( m_iUserIDUnFreezer ) ),
END_RECV_TABLE()

C_BlockBase::C_BlockBase ()
{
}

C_BlockBase::~C_BlockBase ()
{
}
