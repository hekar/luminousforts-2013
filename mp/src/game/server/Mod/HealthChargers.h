#ifndef CHEALTHCHARGER_H
#define CHEALTHCHARGER_H

#ifdef _WIN32
        #pragma once
#endif

class CNewWallHealth : public CBaseAnimating
{
        DECLARE_CLASS( CNewWallHealth, CBaseAnimating );
        DECLARE_DATADESC();
public:
        void Spawn( );
        void Precache( void );
        int  DrawDebugTextOverlays(void);
        bool CreateVPhysics(void);
        void Off(void);
        void Recharge(void);
        bool KeyValue(  const char *szKeyName, const char *szValue );
        void Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value );
        virtual int        ObjectCaps( void ) { return BaseClass::ObjectCaps() | m_iCaps; }

        float m_flNextCharge; 
        int                m_iReactivate ; // DeathMatch Delay until reactvated
        int                m_iJuice;
        int                m_iOn;                        // 0 = off, 1 = startup, 2 = going
        float   m_flSoundTime;

        int                m_nState;
        int                m_iCaps;

        COutputFloat m_OutRemainingHealth;
        COutputEvent m_OnPlayerUse;

        void StudioFrameAdvance ( void );

        float m_flJuice;
private:
        int m_iSourcefortsTeamNum;
};


#endif // CHEALTHCHARGER_H