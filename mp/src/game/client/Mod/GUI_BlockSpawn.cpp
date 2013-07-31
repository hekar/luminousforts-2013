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
#ifdef MOD_HAND_BLOCK_SPAWN
#include <vgui/IInput.h>
#include <vgui/ISurface.h>
#include <vgui/ILocalize.h>
#include "baseviewport.h"
#include "IGameUIFuncs.h"
#include "Mod/Classic_SharedDefs.h"
#include "Mod/GUI_BlockSpawn.h"

ConVar smod_panel_block_rows( "smod_panel_block_rows", "4" );
ConVar smod_panel_block_columns( "smod_panel_block_columns", "4" );

ConVar smod_panel_block_total_width( "smod_panel_block_total_width", "480" );
ConVar smod_panel_block_total_height( "smod_panel_block_total_height", "640" );

ConVar smod_panel_block_width( "smod_panel_block_width", "64" );
ConVar smod_panel_block_height( "smod_panel_block_height", "64" );
ConVar smod_panel_block_gap_x( "smod_panel_block_gap_x", "8" );
ConVar smod_panel_block_gap_y( "smod_panel_block_gap_y", "8" );

#define BLOCK_COMMAND_PREFIX "block_"

bool g_IsVisible = false;


// Toggle visibility of the buy menu
CON_COMMAND( lf_show_block_spawn, "Toggle the Display of the Block Spawn GUI" )
{
	if ( gViewPortInterface )
	{
		g_IsVisible = !g_IsVisible;
		gViewPortInterface->ShowPanel( PANEL_BLOCK, g_IsVisible );
	}
}

CBlockSpawn *spawn = NULL;

// Toggle visibility of the buy menu
CON_COMMAND( smod_panel_block_reload, "" )
{
	if ( spawn )
	{
		spawn->SetupButtonDisplay();
	}
}

CBlockSpawn::CBlockSpawn( vgui::Panel *Parent, IViewPort *pViewPort ) : 
	BaseClass( NULL, PANEL_BLOCK )
{
	SetScheme( "sourcescheme" );
	LoadControlSettings( "resource/ui/BlockSpawn.res" );
	
	BaseClass::SetParent( Parent );
	
	m_pViewPort = pViewPort;

	SetTitle( "#lf_block_spawn_title", true );

	m_iBlockStart = 0;
	m_iBlockCount = BLOCK_COUNT;
	m_iToggleMenuKey = BUTTON_CODE_INVALID;

	SetVisible( g_IsVisible );
	SetEnabled( g_IsVisible );
	SetSizeable( false );
	SetMinimizeButtonVisible( false );
	SetMaximizeButtonVisible( false );

	for ( int i = 0; i < BLOCK_COUNT; i++ )
	{
		m_pButtons[ i ] = NULL;
	}

	for ( int i = m_iBlockStart; i < m_iBlockCount; i++ )
	{
		char command[ 512 ] = { '\0' };
		Q_snprintf( command, sizeof( command ), "%s%s", BLOCK_COMMAND_PREFIX, g_szBlocks[ i ] );
		m_pButtons[ i ] = new vgui::Button( this, g_szBlocks[ i ], g_szBlockSpawnerNames[ i ], this, command );
		m_pButtons[ i ]->SetEnabled( false );
	}

	SetupButtonDisplay();
	spawn = this;
}

CBlockSpawn::~CBlockSpawn()
{
	for ( int i = 0; i < BLOCK_COUNT; i++ )
	{
		if ( m_pButtons[ i ] != NULL )
		{
			delete m_pButtons[ i ];
		}
	}
}

void CBlockSpawn::Update()
{
}

void CBlockSpawn::ShowPanel( bool bShow )
{
	static int mouseX = 0, mouseY = 0;

	if ( IsVisible() == bShow )
		return;

	if ( bShow )
	{
		m_iToggleMenuKey = gameuifuncs->GetButtonCodeForBind( "lf_show_block_spawn" );

		SetEnabled( true );
		SetMouseInputEnabled( true );
		SetKeyBoardInputEnabled( true );

		if ( mouseX != 0 && mouseY != 0 )
		{
			vgui::input()->SetCursorPos( mouseX, mouseY );
		}

		RequestFocus();
	}
	else
	{
		vgui::input()->GetCursorPos( mouseX, mouseY );

		SetEnabled( false );
		SetMouseInputEnabled( false );
		SetKeyBoardInputEnabled( false );
	}

	SetVisible( bShow );
	g_IsVisible = bShow;
}

void CBlockSpawn::OnKeyCodePressed( vgui::KeyCode code )
{
	if ( code == m_iToggleMenuKey )
	{
		ShowPanel( false );
	}
	else
	{
		int blockIndex = code - KEY_F1;
		if ( blockIndex >= BLOCK_START && blockIndex < BLOCK_COUNT )
		{
			char command[ 512 ];
			Q_snprintf( command, sizeof( command ), "lf_hand_spawn_block %s", g_szBlocks[ blockIndex ] );
			engine->ClientCmd( command );
			ShowPanel( false );
		}
	}
}

void CBlockSpawn::OnCommand( const char *command )
{
	// Check for "block_" at beginning of command
	if ( Q_stristr( command, BLOCK_COMMAND_PREFIX ) == command )
	{
		char blockName[ 512 ];
		Q_StrReplace( command, blockName, sizeof( blockName ), BLOCK_COMMAND_PREFIX, "" );
		char conCommand[ 512 ];
		Q_snprintf( conCommand, sizeof( conCommand ), "lf_hand_spawn_block %s", blockName );
		engine->ClientCmd( conCommand );
		ShowPanel( false );
	}
	else
	{
		BaseClass::OnCommand( command );
	}
}

void CBlockSpawn::SetupButtonDisplay()
{
	SetSize( smod_panel_block_total_width.GetInt(), smod_panel_block_total_height.GetInt() );

	int rows = smod_panel_block_rows.GetInt();	
	int columns = smod_panel_block_columns.GetInt();

	int width = smod_panel_block_width.GetInt();
	int height = smod_panel_block_height.GetInt();
	int gapx = smod_panel_block_gap_x.GetInt();
	int gapy = smod_panel_block_gap_y.GetInt();
	
	int x = 0; int y = 15;
	for ( int index = 0; index < columns * rows; index++ )
	{
		if ( index >= m_iBlockCount )
		{
			break;
		}

		m_pButtons[ index ]->SetPos( x + gapx, y + gapy );
		m_pButtons[ index ]->SetSize( width - gapx, height - gapy );
		m_pButtons[ index ]->SetEnabled( true );

		x += width;
		if ( ( ( index + 1 ) % columns ) == 0 )
		{
			x = 0;
			y += height;
		}
	}
}

#endif // MOD_HAND_BLOCK_SPAWN
