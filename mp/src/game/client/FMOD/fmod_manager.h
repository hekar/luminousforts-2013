#ifndef FMOD_MANAGER_H
#define FMOD_MANAGER_H

#ifdef MOD_FMOD

#ifdef _WIN32
#pragma once
#endif

#include "fmod.hpp"

class CFMODManager
{
public:
	CFMODManager();
	~CFMODManager();
 
	void InitFMOD();
	void ExitFMOD();

	void Resume();
	void Pause();

	void FadeThink();

	bool IsSoundPlaying();
	unsigned int GetSoundLength();

	void PlayAmbientSound( const char* pathToFile, float flVolume, bool repeat = false );
	void StopAmbientSound( bool fadeOut );

	void SeekSound( unsigned int PositionMS );

	void SetRepeatOne( bool repeat );

	void SetVolume( float flVolume );

private:
	const char* GetFullPathToSound( const char* pathToFileFromModFolder );
	const char* GetCurrentSoundName( void );

	char m_LastSound [512];
	char currentSound [512];
	bool m_bFadeIn;
	bool m_bFadeOut;
	float m_fFadeDelay;

	bool m_bRepeat;
	float m_flVolume;
};
 
extern CFMODManager* FMODManager();


#endif // MOD_FMOD

#endif //FMOD_MANAGER_H
