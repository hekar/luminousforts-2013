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

#if 0

#ifndef GUI_MP3PANEL_H
#define GUI_MP3PANEL_H

#ifdef _WIN32
#pragma once
#endif

#include "BaseGUI.h"
#include "MGUI_Frame.h"
#include "FileSystem.h"
#include "UtlSymbol.h"

extern ConVar smod_panel_mp3_game_startup_music;
extern ConVar smod_panel_mp3_startup_music;
extern ConVar smod_panel_mp3_play_ingame;
extern ConVar smod_panel_mp3_volume;
extern ConVar smod_panel_mp3_repeat;
extern ConVar smod_panel_mp3_shuffle;
extern ConVar smod_panel_mp3_homepath;

namespace vgui
{
	class OldSectionedList;
	class Slider;
};

struct folder_t
{
	char path[ MAX_PATH + 1 ];
};

struct mp3file_t
{
	char folder[ MAX_PATH + 1 ];
	char filename[ MAX_PATH + 1 ];
};

class CMP3Window : public MFrame
{
private:
	DECLARE_CLASS_SIMPLE( CMP3Window, MFrame );

public:
	CMP3Window( vgui::VPANEL parent );
	~CMP3Window();

	void InitList( bool loadlist = false );
	void ClearList();

	void DefaultListHeader();

	void SaveList();
	void LoadList();

	void AddFile( mp3file_t *mp3file );
	void AddFolder( const char *folder, bool soundsfolder = false, const char *extension = ".mp3" );

	void ShowPanel( bool bShow );
	void SetVisible( bool state ) { BaseClass::SetVisible( state ); }

	bool IsPlaying() const;
	int GetSongCount() const;
	void FadeOut();

	void PlayPause();

	void Pause();
	void Resume();

	void Play();
	void Play( int SongIndex );
	void PlayRandom();

	void Stop();
	void Next();
	void Previous();

	void SetVolume( float flVolume );

	void OnTick();

	MESSAGE_FUNC_INT( OnItemClicked, "ItemLeftClick", itemID );
	MESSAGE_FUNC_CHARPTR( OnFileSelected, "FileSelected", fullpath );
	MESSAGE_FUNC_INT( OnSliderMoved, "SliderMoved", position );

	vgui::VPANEL GetVPanel( void ) { return BaseClass::GetVPanel(); }
  	bool IsVisible() { return BaseClass::IsVisible(); }
	void SetParent( vgui::VPANEL parent ) { BaseClass::SetParent( parent ); }

protected:
	ProtectedMethod void OnCommand( const char *command );
	ProtectedMethod void ApplySchemeSettings(vgui::IScheme *pScheme);

private:
	CUtlLinkedList< folder_t > m_FolderList;
	CUtlLinkedList< mp3file_t > m_MP3FileList;

	vgui::Slider *m_VolumeSlider;

	vgui::OldSectionedList *m_MP3List;
	vgui::FileOpenDialog *m_FileDialog;

private:
	int m_iSectionID;

	bool m_bShuffle;

	bool m_bStopped;
	bool m_bPaused;
	bool m_bPlaying;

	int	m_CurrentSongIndex;
};

DEFINE_GUI_INTERFACE(CMP3Window, MP3WinInterface, mp3windowpanel)

#endif

#endif // 0
