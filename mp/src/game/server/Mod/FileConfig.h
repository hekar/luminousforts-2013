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
#ifndef FILECONFIG_H
#define FILECONFIG_H

#ifdef WIN32
#pragma once
#endif // WIN32

class CFileConfig
{
public:
	CFileConfig( const char *name, const char *filename, bool init = false );

	bool IsLoaded() { return m_Loaded; }
	bool Load();
	bool Load( const char *name, const char *filename );

	bool Save();
	bool Save( const char *filename );

	KeyValues *All();

protected:
	KeyValues *m_kv;

	bool m_Loaded;
	char m_Name[MAX_PATH + 1];
	char m_Filename[MAX_PATH + 1];
};

extern CFileConfig *GlobalConfig();

class CConfigManager
{
public:
	CConfigManager();
	~CConfigManager();

	void AddConfig( CFileConfig *config );
	void LoadConfiguration();
private:
	CUtlLinkedList< CFileConfig * > m_Configurations;
};

extern CConfigManager *ConfigManager();

#endif // FILECONFIG_H