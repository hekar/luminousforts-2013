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


#ifndef CUSTOMFLAGZONE_H
#define CUSTOMFLAGZONE_H

#ifdef WIN32
#pragma once
#endif // WIN32


class CCustomFlagZone : public CFlagZoneClassic
{
	DECLARE_CLASS( CCustomFlagZone, CFlagZoneClassic );
	DECLARE_DATADESC();
public:
	void OnCapture( int teamnum );

private:
	COutputEvent m_OnCapture;
	COutputEvent m_OnRedCapture;
	COutputEvent m_OnBlueCapture;
};


#endif // CUSTOMFLAGZONE_H