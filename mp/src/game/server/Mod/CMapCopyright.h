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


#ifndef CMAPCOPYRIGHT_H
#define CMAPCOPYRIGHT_H

#ifdef _WIN32
	#pragma once
#endif

class CMapCopyright : public CLogicalEntity
{
public:
	DECLARE_CLASS( CMapCopyright , CLogicalEntity );
	DECLARE_DATADESC();

private:
	// Original Person(s)
	char m_OAuthor[192];
	char m_ODate[96];
	char m_ODescription[1024];

	// Updated Person(s)
	char m_UAuthor[192];
	char m_UDate[96];
	char m_UDescription[1024];
};


#endif // CMAPCOPYRIGHT_H