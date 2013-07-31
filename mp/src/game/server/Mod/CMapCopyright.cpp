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
#include "CMapCopyright.h"

LINK_ENTITY_TO_CLASS( lf_copyright, CMapCopyright  );

BEGIN_DATADESC( CMapCopyright  )
	
	DEFINE_KEYFIELD( m_OAuthor, FIELD_STRING, "oauthor" ),
	DEFINE_KEYFIELD( m_ODate, FIELD_STRING, "odate" ),
	DEFINE_KEYFIELD( m_ODescription, FIELD_STRING, "odescription" ),

	DEFINE_KEYFIELD( m_UAuthor, FIELD_STRING, "uauthor" ),
	DEFINE_KEYFIELD( m_UDate, FIELD_STRING, "udate" ),
	DEFINE_KEYFIELD( m_UDescription, FIELD_STRING, "udescription" ),

END_DATADESC()