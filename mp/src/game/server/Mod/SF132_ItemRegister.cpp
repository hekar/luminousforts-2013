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
	Server
	Sourceforts 1.3.2 Gamemode

	Item storing and buying system. Accepts a concommand gives 
	the items to the player if request is valid.

	Last Updated Feb 27, 2010
===============================================================*/

#include "cbase.h"
#ifdef MOD_SF132
#include "hl2mp_player.h"
#include "CModPlayer.h"
#include "SF132_GameRules.h"
#include "SF132_ItemRegister.h"

//
//	Name: CC_SF132BuyItem
//	Author: Hekar Khani
//	Description: Buys an item in the SF132 GameMode
//	Notes:
//
void CC_SF132BuyItem( const CCommand& args )
{
	if ( GameRules()->GetGameMode() != GAMEMODE_SF132 )
	{
		Msg( "Cannot execute this command in gamemodes other than SF132\n" );
		return;
	}
	else if ( args.ArgC() != 2 )
	{
		Msg( "sf132_buyitem <item>\n" );
		return;		
	}
	
	int WeaponIndex = -1;
	CBasePlayer *pPlayer = UTIL_GetCommandClient();

	CModPlayer *pModPlayer = ToModPlayer( pPlayer );
	CItemRegister *itemregister = pModPlayer->GetItemRegister();
	
	char itemname[ 256 ] = {'\0'};

	for (int i = 0; i < SF132_BUY_WEAPON_COUNT; i++)
	{
		// Not all that secure :/
		const char *Command = Q_strstr( g_WeaponCommands[i].Command, args[ 1 ] );
		if ( Command )
		{
			Q_strncpy( itemname, Command, sizeof( itemname ) );
			WeaponIndex = i;
			break;
		}
	}

	if ( itemname [0] )
	{
		BoughtItem_t NewItem;
		Q_strncpy( NewItem.name, itemname, sizeof( NewItem.name ) );
		itemregister->AddItem( NewItem );
	
		if ( SF132GameRules()->GetCurrentPhaseID() == PHASE_COMBAT )
		{
			if ( !itemregister->HasItem( pModPlayer, NewItem.name ) )
			{
				itemregister->GiveItem( pModPlayer, NewItem.name );
			}
		}

		CSingleUserRecipientFilter user( pModPlayer );
		user.MakeReliable();

		char youbought [128] = {0};
		Q_snprintf( youbought, sizeof( youbought ), "You've bought %s", g_WeaponCommands[ WeaponIndex ].WeaponName );
		UTIL_ClientPrintFilter( user, HUD_PRINTTALK, youbought );
	}
}

static ConCommand sf132_buyitem( "sf132_buyitem", CC_SF132BuyItem, "lf_132buyitem <itemname>" );

CItemRegister::CItemRegister()
{
}

CItemRegister::~CItemRegister()
{
}

void CItemRegister::AddItem( const BoughtItem_t& Item )
{
	m_Items.AddToTail( Item );
}

void CItemRegister::GiveItems( CBasePlayer *pPlayer )
{
	CModPlayer *pModPlayer = ToModPlayer(pPlayer);
	for (int i = 0; i < m_Items.Count(); i++)
	{
		GiveItem(pModPlayer, m_Items[i].name);
	}
}

bool CItemRegister::HasItem( CBasePlayer *pPlayer, const char *ItemName )
{
	for (int i = 0; i < m_Items.Count(); i++)
	{
		if ( !Q_strcmp( m_Items[ i ].name, ItemName ) )
		{
			return true;
		}
	}

	return false;
}

void CItemRegister::ClearItems()
{
	m_Items.Purge();
}

void CItemRegister::GiveItem( CHL2MP_Player *pPlayer, const char *ItemName )
{
	pPlayer->GiveNamedItem( ItemName );
}

#endif // MOD_SF132