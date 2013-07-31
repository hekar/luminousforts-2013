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


#ifndef CUSTOMNOFREEZE_H
#define CUSTOMNOFREEZE_H

#ifdef WIN32
#pragma once
#endif // WIN32

class CCustomNoFreeze : public CNoFreezeBase
{
	DECLARE_CLASS( CCustomNoFreeze, CNoFreezeBase );
	DECLARE_DATADESC();
public:
	CCustomNoFreeze();

	virtual void Spawn();

	void InputAllowFreeze( inputdata_t& data );
	void InputDisallowFreeze( inputdata_t& data );
	void InputAllowRedFreeze( inputdata_t& data );
	void InputDisallowRedFreeze( inputdata_t& data );
	void InputAllowBlueFreeze( inputdata_t& data );
	void InputDisallowBlueFreeze( inputdata_t& data );

	virtual void StartTouch( CBaseEntity *pOther );
	virtual void EndTouch( CBaseEntity *pOther );

private:
	COutputEvent m_OnBlockTouch;
	COutputEvent m_OnRedBlockTouch;
	COutputEvent m_OnBlueBlockTouch;
	COutputEvent m_OnRedPlayerTouch;
	COutputEvent m_OnBluePlayerTouch;

	int m_AllowTeamMask;
};

#endif // CUSTOMNOFREEZE_H
