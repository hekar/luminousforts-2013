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


#ifndef CBLOCKBASE_H
#define CBLOCKBASE_H

#ifdef _WIN32
	#pragma once
#endif

#include "c_physicsprop.h"

class C_BlockBase : public C_PhysicsProp
{
public:
	DECLARE_CLASS( C_BlockBase, C_PhysicsProp );
	DECLARE_CLIENTCLASS ();

	C_BlockBase();
	~C_BlockBase();

	int	GetHealth() const { return m_iHealth; }
	int	GetMaxHealth( ) const { return m_iMaxHealth; }
	int	GetBlockType( ) const { return m_BlockType; }
	int	GetBlockWorth( ) const { return m_BlockWorth; }
	int GetLastFrozenByUserID() const { return m_iUserIDFreezer; }
	int GetLastUnFrozenByUserID() const { return m_iUserIDUnFreezer; }

	bool IsBlock() { return true; }

private:
	int m_iHealth;
	int m_iMaxHealth;
	int m_BlockType;
	int m_BlockWorth;
	int m_iUserIDFreezer;
	int m_iUserIDUnFreezer;
};


#endif