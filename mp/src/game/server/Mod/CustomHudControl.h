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


#ifndef CUSTOMHUDCONTROL_H
#define CUSTOMHUDCONTROL_H

class CCustomHudControl : public CBaseEntity
{
	DECLARE_CLASS( CCustomHudControl, CBaseEntity )
	DECLARE_DATADESC()
	DECLARE_SERVERCLASS()
public:
	CCustomHudControl();
	~CCustomHudControl();

	void Spawn();

	void InputEnableHudElement( inputdata_t& data );
	void InputDisableHudElement( inputdata_t& data );

	void UpdateOnRemove( void );

	// Force updates to entity state regardless of client and regardless location of that
	// client
	int UpdateTransmitState()
	{
		return SetTransmitState( FL_EDICT_ALWAYS );
	}

private:
	// make sure these elements start off enabled/disabled
	int m_iEnabledHudElements;

	// can only have 31 custom hudelements :(
	CNetworkVar( int, m_iHudElements );
};


#endif // CUSTOMHUDCONTROL_H