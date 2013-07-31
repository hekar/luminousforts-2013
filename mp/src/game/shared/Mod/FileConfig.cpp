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
#include "filesystem.h"
#include "FileConfig.h"

#define GLOBAL_CONFIG_FILENAME "config/global.txt"

static CConfigManager g_ConfigManager;

CConfigManager *ConfigManager()
{
	return &g_ConfigManager;
}

CConfigManager::CConfigManager()
{
}

CConfigManager::~CConfigManager()
{
	for ( int i = 0; i < m_Configurations.Count(); i++ )
	{
		delete m_Configurations.Element(i);
	}
	
	m_Configurations.RemoveAll();
}

void CConfigManager::AddConfig( CFileConfig *config )
{
	m_Configurations.AddToTail( config );
}

void CConfigManager::LoadConfiguration()
{
	for ( int i = 0; i < m_Configurations.Count(); i++ )
	{
		m_Configurations.Element(i)->Load();
	}
}

static CFileConfig g_GlobalConfig( "GlobalConfig", GLOBAL_CONFIG_FILENAME );
CFileConfig *GlobalConfig()
{
	if ( !g_GlobalConfig.IsLoaded() )
	{
		g_GlobalConfig.Load();
	}

	return &g_GlobalConfig;
}

CFileConfig::CFileConfig( const char *name, const char *filename, bool init )
{
	Q_strncpy( m_Name, name, sizeof( m_Name ) );
	Q_strncpy( m_Filename, filename, sizeof( m_Filename ) );

	m_Loaded = false;
	if ( init )
	{
		ConfigManager()->AddConfig( this );
	}
}

bool CFileConfig::Load()
{
	m_kv = new KeyValues( m_Name );
	m_Loaded = true;
	return m_kv->LoadFromFile( g_pFullFileSystem, m_Filename, "MOD" );
}

bool CFileConfig::Load( const char *name, const char *filename )
{
	m_kv = new KeyValues( name );
	return m_kv->LoadFromFile( g_pFullFileSystem, filename, "MOD" );
}

bool CFileConfig::Save()
{
	return m_kv->SaveToFile( g_pFullFileSystem, m_Filename, "MOD" );
}

bool CFileConfig::Save( const char *filename )
{
	return m_kv->SaveToFile( g_pFullFileSystem, filename, "MOD" );
}

KeyValues *CFileConfig::All()
{
	if ( !m_Loaded )
	{
		Load();
	}

	return m_kv;
}

class CConfigGameSystem : public CAutoGameSystem
{
public:
	CConfigGameSystem() :
	  CAutoGameSystem( "ConfigGameSystem" )
	{
	}

	// DLL Initialization
	bool Init()
	{
		// Initialize the configurations
		ConfigManager()->LoadConfiguration();
		return true;
	}
};

static CConfigGameSystem g_ConfigGameSystem;