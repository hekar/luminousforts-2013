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


#ifndef CUSTOMMANAGERGAMERULES_H
#define CUSTOMMANAGERGAMERULES_H

#ifdef _WIN32
#pragma once
#endif

#if 0

#ifdef CLIENT_DLL
	#define CCustomManagerGameRules C_CustomManagerGameRules
#endif // CLIENT_DLL

// Custom gamerules only
class CCustomManagerGameRules : public CBaseEntity
{
	DECLARE_CLASS( CCustomManagerGameRules, CBaseEntity );
	DECLARE_DATADESC();
public:
	CCustomManagerGameRules();

#ifndef CLIENT_DLL
	void InputBlueWins( inputdata_t& data );
	void InputRedWins( inputdata_t& data );
	void InputDraw( inputdata_t& data );

private:
	COutputEvent m_OnRedWins;
	COutputEvent m_OnBlueWins;
	COutputEvent m_OnDraw;
#endif

private:
	bool m_bEnabled;
};

#endif // 0

#endif // CUSTOMMANAGERGAMERULES_H
