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


#ifndef CDISPENSERBASE_H
#define CDISPENSERBASE_H

#ifdef WIN32
#pragma once
#endif // WIN32

class CDispenserBase : public CBaseAnimating
{
	DECLARE_CLASS( CDispenserBase, CBaseAnimating );
	DECLARE_DATADESC();
public:
	CDispenserBase();

	virtual void Precache();
	virtual void Spawn();

	void InputDisable( inputdata_t& data );
	void InputEnable( inputdata_t& data );

	virtual void Think();

protected:
	ProtectedMethod virtual void StockSurroundingPlayers();
	ProtectedMethod virtual void HealPlayer( CBasePlayer *pPlayer );
	ProtectedMethod virtual void StockPlayer( CBasePlayer *pPlayer );

	// Event triggers
	ProtectedMethod virtual void OnHealPlayer( CBasePlayer *pPlayer, int HealthGiven ) {}
	ProtectedMethod virtual void OnStockPlayer( CBasePlayer *pPlayer ) {}

	COutputEvent m_OnDispense;
	bool m_bEnabled;
};


#endif // CDISPENSERBASE_H