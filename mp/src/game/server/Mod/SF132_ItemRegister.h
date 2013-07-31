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


#ifndef ITEM_REGISTER_H
#define ITEM_REGISTER_H

typedef struct
{
	char name [256];
} BoughtItem_t;

class CHL2MP_Player;

class CItemRegister
{
public:
	CItemRegister();
	~CItemRegister();

	void AddItem( const BoughtItem_t& Item );
	void GiveItems( CBasePlayer *pPlayer );
	bool HasItem( CBasePlayer *pPlayer, const char *ItemName );
	void ClearItems();

	void GiveItem( CHL2MP_Player *pPlayer, const char *ItemName );

private:
	CUtlLinkedList< BoughtItem_t > m_Items;
};



#endif // ITEM_REGISTER_H