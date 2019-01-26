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
#if 0
#include "baseviewport.h"
#include "GUI_MP3Panel.h"
#include "Panel_Listen_Main.h"

static CDllDemandLoader g_GameUI( "gameui" );

CPanelListenMain::CPanelListenMain( vgui::Panel *Parent ) :
	BaseClass( Parent, "PanelListenMain" )
{
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/SourceScheme.res", "SourceScheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/Listen_Main.res" );
	
	m_pGameUI = NULL;
	m_MapList = new vgui::OldSectionedList( this, "maplist" );
	
	InitMapList();
}

CPanelListenMain::~CPanelListenMain()
{
	delete m_MapList;
}

vgui::Panel *CPanelListenMain::CreateControlByName( const char *controlName )
{
	return BaseClass::CreateControlByName( controlName );
}

void CPanelListenMain::InitMapList()
{
	int w, h;
	GetSize( w, h );
	m_MapList->SetPos( w - 100, 20 );
	m_MapList->SetSize( 98, h - 60 );
	m_MapList->SetAutoResize( PIN_TOPLEFT, AUTORESIZE_DOWNANDRIGHT, w - 98, 20, -2, -60 );

	CreateInterfaceFn gameUIFactory = g_GameUI.GetFactory();
	if ( gameUIFactory )
	{
		m_pGameUI = (IGameUI *) gameUIFactory(GAMEUI_INTERFACE_VERSION, NULL );
	}
}

void CPanelListenMain::OnResetData()
{
}

void CPanelListenMain::OnApplyChanges()
{
	// Start the server
	KeyValues *kv = m_MapList->GetItemData( m_MapList->GetSelectedItem() );
	if ( kv )
	{
		const char *mapname = kv->GetString( "mapname" );
		if ( Q_strcmp( mapname, "" ) ) // make sure we have a string
		{
			StartServer( mapname );
		}
	}
}

void CPanelListenMain::OnPageShow()
{
	DisplayMaps();
}

void CPanelListenMain::OnPageHide()
{
	ClearMapList();
}

void CPanelListenMain::OnCommand( const char *command )
{
	BaseClass::OnCommand( command );
}

void CPanelListenMain::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor("Modfg", GetFgColor()) );
	SetBgColor( Color(0, 0, 0, 0) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );
}

void CPanelListenMain::DisplayMapsWithPrefix( const char *prefix )
{
	if ( m_MapList->GetItemCount() > 0 )
	{
		ClearMapList();
	}

	FileFindHandle_t filehandle;
	char wildcard[ MAX_PATH + 1 ];
	Q_snprintf( wildcard, sizeof( wildcard ), "maps/%s*.bsp", prefix );
	
	const char *filename = g_pFullFileSystem->FindFirst( wildcard, &filehandle );

	if ( !filename )
		return;

	m_MapList->AddSection( 0, "" );
	m_MapList->SetSectionAlwaysVisible( 0, true );
	m_MapList->AddColumnToSection( 0, "mapname", "map name", 0, 1024 );

	do
	{
		KeyValues *kv = new KeyValues( "mapname" );
		kv->SetString( "mapname", filename );
		m_MapList->AddItem( 0, kv );

		filename = g_pFullFileSystem->FindNext( filehandle );

	} while ( filename );

	g_pFullFileSystem->FindClose( filehandle );
}

// Creates subsections inside of the list that denote each gamemode's maps
void CPanelListenMain::DisplayMaps()
{
	if ( m_MapList->GetItemCount() > 0 )
	{
		ClearMapList();
	}

	FileFindHandle_t filehandle;
	const char *filename = g_pFullFileSystem->FindFirst( "maps/*.bsp", &filehandle );

	if ( !filename )
		return;

	m_MapList->AddSection( 0, "" );
	m_MapList->SetSectionAlwaysVisible( 0, true );
	m_MapList->AddColumnToSection( 0, "mapname", "Map Name", 0, 1024 );
	m_MapList->AddColumnToSection( 0, "gamemode", "Gamemode", 0, 1024 );

	do
	{
		// Extract this to a separate function!!
		if ( Q_strstr( filename, "lf_" ) || Q_strstr( filename, "lfc_" ) ||
			Q_strstr( filename, "lfcus_" ) || Q_strstr( filename, "sfb_" ) )
		{
			KeyValues *kv = new KeyValues( "" );
			kv->SetString( "mapname", filename );

			m_MapList->AddItem( 0, kv );
		}

		filename = g_pFullFileSystem->FindNext( filehandle );
#if 0
		// not going to work
		if ( Q_strstr( filename, "lfc_" ) == filename )
		{
			kv->SetString( "gamemode", "Sourceforts 1.9.5 Classic" );
		}
		else if ( Q_strstr( filename, "lfcus_" ) == filename )
		{
			kv->SetString( "gamemode", "Sourceforts 1.9.5 Custom" );			
		}
		else if ( Q_strstr( filename, "lf_" ) == filename )
		{
			kv->SetString( "gamemode", "Sourceforts 1.9.4" );
		}
		else if ( Q_strstr( filename, "sfb_" ) == filename )
		{
			kv->SetString( "gamemode", "Sourceforts 1.9.5 Bumrush" );
		}
		
#endif

	} while ( filename );

	m_MapList->SetSelectedItem( 0 );

	g_pFullFileSystem->FindClose( filehandle );
}

void CPanelListenMain::ClearMapList()
{
	m_MapList->RemoveAll();
	m_MapList->RemoveAllSections();
}

void CPanelListenMain::StartServer( const char *mapname )
{
	char command[ MAX_PATH + 4 + 1 ];
	Q_snprintf( command, sizeof( command ), "map %s", mapname );
	engine->ClientCmd_Unrestricted( command );

	if ( m_pGameUI )
	{
		m_pGameUI->OLD_OnConnectToServer( "ListenServer", 0, 0 );
		m_pGameUI->OnLevelLoadingStarted( true );
		//m_pGameUI->SetProgressOnStart();
		//m_pGameUI->UpdateProgressBar( 10.0f, "LOL NOOB!" );
		//m_pGameUI->SetShowProgressText( "lolu" );
	}
}
#endif // 0
