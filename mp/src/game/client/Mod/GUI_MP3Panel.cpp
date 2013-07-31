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
	Client

	Very similar to the webpanel found in GUI_WebWindow.cpp
	This is a singleton window in the main menu that deals
	with playing mp3s.

	Uses FMOD rather than the Miles engine to play mp3

	Last Updated Jan 19, 2010
===============================================================*/

#include "cbase.h"

#if 0

#include <vgui/IVGui.h>
#include <vgui/ILocalize.h>
#include <vgui/ISurface.h>
#include <vgui/IScheme.h>

#include <vgui_controls/Controls.h>
#include <vgui_controls/Label.h>
#include <vgui_controls/FileOpenDialog.h>
#include <vgui_controls/Slider.h>

#include "MGUI_CvarCheckButton.h"
#include "MGUI_CvarSlider.h"
#include "OldSectionedList.h"

#include "FMOD/fmod_manager.h"

#include "GUI_MP3Panel.h"


CREATE_GUI_INTERFACE(MP3WinInterface, mp3windowpanel)

#define PLAYLIST_FILE "Resource/Playlist/main.txt"

// Saved cvar settings
ConVar smod_panel_mp3_game_startup_music( "smod_panel_mp3_game_startup_music", "1", FCVAR_ARCHIVE );
ConVar smod_panel_mp3_startup_music( "smod_panel_mp3_startup_music", "0", FCVAR_ARCHIVE );
ConVar smod_panel_mp3_play_ingame( "smod_panel_mp3_play_ingame", "0", FCVAR_ARCHIVE );
ConVar smod_panel_mp3_volume( "smod_panel_mp3_volume", "0.1", FCVAR_ARCHIVE );
ConVar smod_panel_mp3_repeat( "smod_panel_mp3_repeat", "0", FCVAR_ARCHIVE );
ConVar smod_panel_mp3_shuffle( "smod_panel_mp3_shuffle", "0", FCVAR_ARCHIVE );
ConVar smod_panel_mp3_homepath( "smod_panel_mp3_homepath", "0", FCVAR_ARCHIVE );

CON_COMMAND(smod_panel_mp3_toggle, "MP3 Player Window (Toggles display)")
{	
	mp3windowpanel->Toggle();
}

//
//	Name: CMP3Window
//	Author: Hekar Khani
//	Description: Background MP3 Player panel for main menu
//	Notes: 
//		This is a pretty poor piece of code and not really of any use.
//
CMP3Window::CMP3Window( vgui::VPANEL parent ) : 
	BaseClass( NULL, "MP3Window" )
{	
	SetParent( parent );
	vgui::HScheme scheme = vgui::scheme()->LoadSchemeFromFile( "resource/sourcescheme.res", "sourcescheme" );
	SetScheme( scheme );
	LoadControlSettings( "resource/ui/MP3Panel.res" );

	SetVisible( false );
	SetSizeable( true );
	SetTitleBarVisible( true );
	SetMaximizeButtonVisible( true );
	SetMinimizeButtonVisible( false );

	SetMinimumSize( 387, 443 );

	m_iSectionID = 0;

	smod_panel_mp3_repeat.SetValue( 0 );;
	m_bShuffle = false;

	m_bStopped = false;
	m_bPaused = false;
	m_bPlaying = false;
	m_CurrentSongIndex = 0;

	m_MP3List = new vgui::OldSectionedList( this, "MP3List" );
	m_FileDialog = new vgui::FileOpenDialog( this, "Select Folder", vgui::FOD_SELECT_DIRECTORY );
}

CMP3Window::~CMP3Window()
{
	ClearList();
	delete m_MP3List;
	delete m_FileDialog;
}

void CMP3Window::InitList( bool loadlist )
{
	ClearList();

	int w, h;
	GetSize( w, h );
	m_MP3List->SetPos( 10, 30 );
	m_MP3List->SetSize( w - 15, h - 150 );
	m_MP3List->SetAutoResize( PIN_TOPLEFT, AUTORESIZE_DOWNANDRIGHT, 10, 30, -10, -150 );
#if 0
	// using this as reference to move it to the resource file
	m_VolumeSlider->SetPos( 15, h - 100 );
	m_VolumeSlider->SetSize( 150, 35 );
	m_VolumeSlider->SetRange( 0, 10 );
	m_VolumeSlider->SetSliderThumbSubRange( true, 0, 10 );
	m_VolumeSlider->SetAutoResize( PIN_BOTTOMLEFT, AUTORESIZE_NO, 15, 100, w - 15 + 150, h + 100 - 35 );
	m_VolumeSlider->SetValue( smod_panel_mp3_volume.GetFloat() );
#endif // 0
	if ( loadlist )
	{
		LoadList();
	}
}

void CMP3Window::ClearList()
{
	Stop();
	m_MP3List->DeleteAllItems();
	m_MP3List->RemoveAllSections();

	m_FolderList.Purge();
	m_MP3FileList.Purge();

	DefaultListHeader();
}

void CMP3Window::DefaultListHeader()
{
	m_MP3List->AddSection( 0, "" );
	m_MP3List->SetSectionAlwaysVisible( 0, true );
	m_MP3List->AddColumnToSection( 0, "filename", "Filename", 0, 1024 );
}

void CMP3Window::SaveList()
{
	KeyValues *kv = new KeyValues( "MP3List" );

	for (int i = 0; i < m_FolderList.Count(); i++)
	{
		char buf[ 64 ] = {0};
		Q_snprintf( buf, sizeof( buf ), "%d", i );
		kv->SetString( buf, m_FolderList.Element( i ).path );
	}

	kv->SaveToFile( g_pFullFileSystem, PLAYLIST_FILE, "GAME" );
}

void CMP3Window::LoadList()
{
	ClearList();

	KeyValues *kv = new KeyValues( "MP3List" );

	kv->LoadFromFile( g_pFullFileSystem, PLAYLIST_FILE, "GAME" );

	for (int i = 0; i < 99; i++)
	{
		char buf[ 64 ] = { '\0' };
		Q_snprintf( buf, sizeof( buf ), "%d", i );
		
		const char *foldername = kv->GetString( buf, NULL );
		if ( !foldername )
		{
			break;
		}

		AddFolder( foldername );
	}
}

void CMP3Window::AddFile( mp3file_t *mp3file )
{
	m_MP3FileList.AddToTail( *mp3file );

	KeyValues *kv = new KeyValues( "mp3filename" );
	kv->SetString( "filename", mp3file->filename );
	m_MP3List->AddItem( m_iSectionID, kv );
}

void CMP3Window::AddFolder( const char *folder, bool soundsfolder, const char *extension )
{
	if ( !folder || !extension )
	{
		return;
	}

	// Also add Oggs!
	const char ogg[] = ".ogg";
	if ( Q_strcmp( extension, ogg ) )
	{
		AddFolder( folder, soundsfolder, ogg );
	}

	char filterpath[ MAX_PATH + 1 ];
	// Unfortunately, anything goes because *.mp3 and *.ogg are supported.
	Q_snprintf( filterpath, sizeof( filterpath ), "%s\\*%s", folder, extension );
	Q_FixSlashes( filterpath );

	int MP3FilesFound = 0;
	FileFindHandle_t filehandle;
	const char *filename = g_pFullFileSystem->FindFirst( filterpath, &filehandle );

	if ( !filename )
	{
		// bail out of this suck'er
		return;
	}

	do
	{
		if ( filename != NULL )
		{
			mp3file_t mp3file = {0};

			if ( soundsfolder )
			{
				char FullPath[ _MAX_PATH + 1 ];
				g_pFullFileSystem->RelativePathToFullPath( folder, "MOD", FullPath, sizeof( FullPath ) );
				Q_snprintf( mp3file.folder, sizeof( mp3file.folder ), "%s", FullPath);
			}
			else
			{
				Q_strncpy( mp3file.folder, folder, sizeof( mp3file.folder ) );
			}

			Q_strncat( mp3file.filename, filename, sizeof( mp3file.filename ) );

			if ( mp3file.filename )
			{
				if (MP3FilesFound < 1)
				{
					m_iSectionID++;
					m_MP3List->AddSection( m_iSectionID, "" );
					m_MP3List->SetSectionAlwaysVisible( m_iSectionID, true );
					m_MP3List->AddColumnToSection( m_iSectionID, "filename", mp3file.folder, 0, 1024 );

					folder_t addfolder;
					Q_strncpy( addfolder.path, mp3file.folder, sizeof( addfolder.path ) );
					m_FolderList.AddToTail( addfolder );
				}

				AddFile( &mp3file );
				MP3FilesFound++;
			}
		}
	} while ( ( filename = g_pFullFileSystem->FindNext( filehandle ) ) != NULL );

	g_pFullFileSystem->FindClose( filehandle );
}

void CMP3Window::ShowPanel( bool bShow )
{
	if ( BaseClass::IsVisible() == bShow )
		return;

	if ( bShow )
	{
		SetVisible( true );
		Activate();
		MoveToCenterOfScreen();
		SetMouseInputEnabled( true );
	}
	else
	{
		SetVisible( false );
		SetMouseInputEnabled( false );
	}
}

bool CMP3Window::IsPlaying() const
{
	return m_bPlaying;
}

int CMP3Window::GetSongCount() const
{
	return m_MP3FileList.Count();
}

void CMP3Window::FadeOut()
{
	if ( m_bPlaying )
	{
		FMODManager()->StopAmbientSound( true );

		m_bPlaying = false;
		m_bStopped = true;
	}
}

void CMP3Window::PlayPause()
{
	m_bPaused = !m_bPaused;

	if ( m_bPaused )
	{
		FMODManager()->Pause();
	}
	else
	{
		FMODManager()->Resume();
	}
}

void CMP3Window::Pause()
{
	m_bPaused = true;
	FMODManager()->Pause();
}

void CMP3Window::Resume()
{
	m_bPaused = true;
	FMODManager()->Resume();
}

void CMP3Window::Play()
{
	Play( m_CurrentSongIndex );
}

void CMP3Window::Play( int SongIndex )
{
	if ( m_bPlaying )
	{
		Stop();
	}

	if ( !m_MP3FileList.IsValidIndex( SongIndex ) )
	{
		Warning( "Bad MP3 List Index\n" );
		return;
	}

	char soundname[ 512 ];
	soundname[ 0 ] = '\0';

	mp3file_t *mp3file = &m_MP3FileList.Element( SongIndex );

	Q_snprintf( soundname, sizeof( soundname ), "%s\\%s", mp3file->folder, mp3file->filename );
	Q_FixSlashes( soundname );

	FMODManager()->PlayAmbientSound( soundname, smod_panel_mp3_volume.GetFloat() );

	m_bStopped = false;
	m_bPaused = false;
	m_bPlaying = true;
	m_CurrentSongIndex = SongIndex;

	m_MP3List->SetSelectedItem( m_CurrentSongIndex );
}

void CMP3Window::PlayRandom()
{
	int SongIndex = RandomInt( 0, GetSongCount() - 1 );
	Play( SongIndex );
}

void CMP3Window::Stop()
{
	if ( m_bPlaying )
	{
		FMODManager()->StopAmbientSound( false );

		m_bPlaying = false;
		m_bStopped = true;
	}
}

void CMP3Window::Next()
{
	if ( (m_CurrentSongIndex + 1) > (GetSongCount() - 1) )
	{
		return;
	}

	if (m_bPlaying)
	{
		Stop();
		Play( m_CurrentSongIndex + 1 );
	}
	else
	{
		m_CurrentSongIndex += 1;
	}
}

void CMP3Window::Previous()
{
	if ( (m_CurrentSongIndex - 1) < 0 )
	{
		return;
	}

	if (m_bPlaying)
	{
		Stop();
		Play( m_CurrentSongIndex - 1 );
	}
	else
	{
		m_CurrentSongIndex -= 1;
	}
}

void CMP3Window::SetVolume( float flVolume )
{
	FMODManager()->SetVolume( flVolume );
}

void CMP3Window::OnTick()
{
	BaseClass::OnTick();

	m_bPlaying = FMODManager()->IsSoundPlaying();

	// Play next song on playlist
	if ( !m_bPlaying && !m_bStopped && !m_bPaused  )
	{
		if ( smod_panel_mp3_repeat.GetBool() && !m_bShuffle )
		{
			Play();
		}
		else if ( m_bShuffle )
		{
			PlayRandom();
		}
		else
		{
			Next();
		}
	}
}

void CMP3Window::OnItemClicked( int itemID )
{
	Play( itemID );
}

void CMP3Window::OnFileSelected( const char *fullpath )
{
	AddFolder( fullpath );
}

void CMP3Window::OnSliderMoved( int position )
{
	if ( IsPlaying() )
	{
		SetVolume( smod_panel_mp3_volume.GetFloat() );
	}
}

ProtectedMethod void CMP3Window::OnCommand( const char *command )
{
	// WHAT IS DIS
	if ( !Q_strcmp( command, "addfolder" ) )
	{
		if ( smod_panel_mp3_homepath.GetBool() )
		{
			m_FileDialog->SetStartDirectory( "%HOMEPATH%" );
		}
		else
		{
			m_FileDialog->SetStartDirectory( "%HOMEDRIVE%" );
		}

		m_FileDialog->DoModal();
	}
	else if ( !Q_strcmp( command, "save" ) )
	{
		SaveList();
	}
	else if ( !Q_strcmp( command, "load" ) )
	{
		LoadList();
	}
	else if ( !Q_strcmp( command, "clear" ) )
	{
		ClearList();
	}
	else if ( !Q_strcmp( command, "play" ) )
	{
		Play( m_CurrentSongIndex );
	}
	else if ( !Q_strcmp( command, "stop" ) )
	{
		Stop();
	}
	else if ( !Q_strcmp( command, "pause" ) )
	{
		if ( !m_bPaused )
			Pause();
		else
			Resume();
	}
	else if ( !Q_strcmp( command, "prev" ) )
	{
		Previous();
	}
	else if ( !Q_strcmp( command, "next" ) )
	{
		Next();
	}
	else if ( !Q_strcmp( command, "repeat" ) )
	{
		smod_panel_mp3_repeat.SetValue( smod_panel_mp3_repeat.GetBool() );

		FMODManager()->SetRepeatOne( smod_panel_mp3_repeat.GetBool() );

		if ( smod_panel_mp3_repeat.GetBool() )
		{
			m_bShuffle = false;
		}
	}
	else if ( !Q_strcmp( command, "shuffle" ) )
	{
		m_bShuffle = !m_bShuffle;
		
		if ( m_bShuffle )
		{
			smod_panel_mp3_repeat.SetValue( false );
		}
	}

	BaseClass::OnCommand( command );
}

ProtectedMethod void CMP3Window::ApplySchemeSettings( vgui::IScheme *pScheme )
{
	BaseClass::ApplySchemeSettings( pScheme );

	SetFgColor( pScheme->GetColor("Modfg", GetFgColor()) );
	SetBgColor( Color(0, 0, 0, 0) );
	SetBorder( pScheme->GetBorder( "FrameBorder" ) );
}


// AutoGameSystem to help with setup of FMOD and automatic playing of music
// from the MP3 panel.
class CMP3PanelGameSystem : public CAutoGameSystem
{
public:
	CMP3PanelGameSystem() :
	  CAutoGameSystem( "MP3PanelGameSystem" )
	{
	}

	bool Init()
	{
		// Setup randomseed for others to use
		// This should be somewhere else - Hekar
		// Multiple by 1000.0f to get good random seed
		RandomSeed( engine->Time() * 1000.0f );

		return true;
	}

	// On first loading of the game
	void PostInit()
	{
		// Start FMOD
		FMODManager()->InitFMOD();

		CMP3Window *mp3win = mp3windowpanel->GetFullPanel();

		mp3win->InitList();

		if ( smod_panel_mp3_game_startup_music.GetBool() )
		{
			mp3win->AddFolder( "sound/mod_music", true );
			mp3win->PlayRandom();
			mp3win->SetVolume( smod_panel_mp3_volume.GetFloat() );
		}
		else if ( smod_panel_mp3_startup_music.GetBool() )
		{
			//mp3win->LoadList();
			//mp3win->PlayRandom();
			//mp3win->SetVolume( smod_panel_mp3_volume.GetFloat() );
		}
	}

	void Shutdown()
	{
		// Shutdown FMOD
		FMODManager()->StopAmbientSound( false );
		FMODManager()->ExitFMOD();
	}

	// When map loading is nearly finished
	void LevelInitPostEntity()
	{
		CMP3Window *mp3win = mp3windowpanel->GetFullPanel();
		if ( !smod_panel_mp3_play_ingame.GetBool() )
		{
			mp3win->FadeOut();
		}
	}
};

static CMP3PanelGameSystem g_MP3PanelHelper;

#endif // 0
