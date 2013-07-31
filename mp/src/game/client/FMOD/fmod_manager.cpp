#include "cbase.h"
#ifdef MOD_FMOD
#include "GUI_MP3Panel.h"
#include "FMOD/fmod_manager.h"

using namespace FMOD;

System			*pSystem;
Sound			*pSound;
SoundGroup		*pSoundGroup;
Channel			*pChannel;
ChannelGroup	*pChannelGroup;
FMOD_RESULT		result;

CFMODManager gFMODMng;
CFMODManager* FMODManager()
{
	return &gFMODMng;
}

CFMODManager::CFMODManager()
{
	m_fFadeDelay = 0.0;
	currentSound [0] = '\0';
	m_bFadeIn = false;
	m_bFadeOut = false;
	m_flVolume = 0.0;
}

CFMODManager::~CFMODManager()
{
}

// Starts FMOD
void CFMODManager::InitFMOD( void )
{
	result = System_Create( &pSystem ); // Create the main system object.

	if (result != FMOD_OK)
		Warning("FMOD ERROR: System creation failed!\n");
	else
		DevMsg("FMOD system successfully created.\n");

	result = pSystem->init(100, FMOD_INIT_NORMAL, 0);   // Initialize FMOD system.

	if (result != FMOD_OK)
		Warning("FMOD ERROR: Failed to initialize properly!\n");
	else
		DevMsg("FMOD initialized successfully.\n");
}

// Stops FMOD
void CFMODManager::ExitFMOD( void )
{
	result = pSystem->release();

	if (result != FMOD_OK)
		Warning("FMOD ERROR: System did not terminate properly!\n");
	else
		DevMsg("FMOD system terminated successfully.\n");
}

void CFMODManager::Resume()
{
	pChannel->setPaused( false );
}

void CFMODManager::Pause()
{
	pChannel->setPaused( true );
}

const char *CFMODManager::GetCurrentSoundName( void )
{
	return currentSound;
}

// Handles all fade-related sound stuffs
// Called every frame when the client is in-game
void CFMODManager::FadeThink( void )
{
	if ( m_bFadeOut )
	{
		if ( gpGlobals->curtime >= m_fFadeDelay )
		{
			float tempvol;
			pChannel->getVolume( &tempvol );

			if ( tempvol > 0.0 )
			{
				SetVolume( tempvol - 0.005 );
				m_fFadeDelay = gpGlobals->curtime + 0.1;
			}
			else
			{
				SetVolume( 0.0 );
				m_bFadeOut = false;
				m_fFadeDelay = 0.0;
			}
		}
	}
}

bool CFMODManager::IsSoundPlaying()
{
	bool isPlaying = false;
	pChannel->isPlaying( &isPlaying );
	return isPlaying;
}

unsigned int CFMODManager::GetSoundLength()
{
	if ( !IsSoundPlaying())
	{
		Warning( "No Sound Playing: Failed to get sound length\n" );
		return 0;
	}

	FMOD::Sound **snd = NULL;
	if ( pChannel->getCurrentSound( snd ) != FMOD_OK )
	{
		Warning( "Failed to get sound length\n" );
		return 0;
	}

	unsigned int length;
	(*snd)->getLength( &length, FMOD_TIMEUNIT_MS );

	return length;
}

void CFMODManager::PlayAmbientSound( const char* pathToFile, float flVolume, bool repeat )
{
	SetVolume( flVolume );

	if ( repeat )
	{
		result = pSystem->createStream( pathToFile, FMOD_LOOP_NORMAL, 0, &pSound );
	}
	else
	{
		result = pSystem->createStream( pathToFile, FMOD_DEFAULT, 0, &pSound );
	}

	if (result != FMOD_OK)
	{	
		Warning("FMOD: Failed to create stream of sound '%s' ! (ERROR NUMBER: %i)\n", pathToFile, result);
		return;
	}

	result = pSystem->playSound( FMOD_CHANNEL_REUSE, pSound, false, &pChannel);

	if (result != FMOD_OK)
	{
		Warning("FMOD: Failed to play sound '%s' ! (ERROR NUMBER: %i)\n", pathToFile, result);
		return;
	}

	Q_strncpy( currentSound, pathToFile, sizeof( currentSound ) );
}

// Stops playing all ambient sounds
void CFMODManager::StopAmbientSound( bool fadeOut )
{
	if ( fadeOut )
	{
		SetVolume( m_flVolume );
		m_bFadeOut = true;
	}
	else
	{
		SetVolume( 0.0 );
	}

	Q_strncpy( m_LastSound, currentSound, sizeof( m_LastSound ) );
	currentSound [0] = '\0';
}

void CFMODManager::SeekSound( unsigned int PositionMS )
{
	if ( !IsSoundPlaying())
	{
		Msg( "Failure seeking. Sound Not playing.\n" );
		return;
	}

	pChannel->setPosition( PositionMS, FMOD_TIMEUNIT_MS );	
}

void CFMODManager::SetRepeatOne( bool repeat )
{
	m_bRepeat = repeat;
}

void CFMODManager::SetVolume( float flVolume )
{
	m_flVolume = flVolume;
	pChannel->setVolume( m_flVolume );
}

#endif // MOD_FMOD
