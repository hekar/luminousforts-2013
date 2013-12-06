/*===============================================================
        Server
        Model Entity

        The Health Charger taken from HL2DM. Thanks HL2DM :D!
        (Also comes with healthkits.)

        Last Updated Oct 31, 2009
===============================================================*/

#include "cbase.h"
#include "player.h"
#include "items.h"
#include "in_buttons.h"
#include "engine/IEngineSound.h"
#include "Mod/ClassicGameRules.h"
#include "Mod/HealthChargers.h"
#include "Mod/SharedModCvars.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

ConVar        sk_healthkit( "sk_healthkit","25" );
ConVar        sk_healthvial( "sk_healthvial","5" );        
ConVar        sk_healthcharger( "sk_healthcharger","100" );

//-----------------------------------------------------------------------------
// Small health kit. Heals the player when picked up.
//-----------------------------------------------------------------------------
class CHealthKit : public CItem
{
public:
        DECLARE_CLASS( CHealthKit, CItem );

        void Spawn( void );
        void Precache( void );
        bool MyTouch( CBasePlayer *pPlayer );
};

LINK_ENTITY_TO_CLASS( item_healthkit, CHealthKit );
PRECACHE_REGISTER(item_healthkit);


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHealthKit::Spawn( void )
{
        Precache();
        SetModel( "models/items/healthkit.mdl" );

        BaseClass::Spawn();
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHealthKit::Precache( void )
{
        PrecacheModel("models/items/healthkit.mdl");

        PrecacheScriptSound( "HealthKit.Touch" );
}


//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pPlayer - 
// Output : 
//-----------------------------------------------------------------------------
bool CHealthKit::MyTouch( CBasePlayer *pPlayer )
{
        if ( pPlayer->TakeHealth( sk_healthkit.GetFloat(), DMG_GENERIC ) )
        {
                CSingleUserRecipientFilter user( pPlayer );
                user.MakeReliable();

                UserMessageBegin( user, "ItemPickup" );
                        WRITE_STRING( GetClassname() );
                MessageEnd();

                CPASAttenuationFilter filter( pPlayer, "HealthKit.Touch" );
                EmitSound( filter, pPlayer->entindex(), "HealthKit.Touch" );

                if ( g_pGameRules->ItemShouldRespawn( this ) )
                {
                        Respawn();
                }
                else
                {
                        UTIL_Remove(this);        
                }

                return true;
        }

        return false;
}

//-----------------------------------------------------------------------------
// Small dynamically dropped health kit
//-----------------------------------------------------------------------------

class CHealthVial : public CItem
{
public:
        DECLARE_CLASS( CHealthVial, CItem );

        void Spawn( void )
        {
                Precache();
                SetModel( "models/healthvial.mdl" );

                BaseClass::Spawn();
        }

        void Precache( void )
        {
                PrecacheModel("models/healthvial.mdl");

                PrecacheScriptSound( "HealthVial.Touch" );
        }

        bool MyTouch( CBasePlayer *pPlayer )
        {
                if ( pPlayer->TakeHealth( sk_healthvial.GetFloat(), DMG_GENERIC ) )
                {
                        CSingleUserRecipientFilter user( pPlayer );
                        user.MakeReliable();

                        UserMessageBegin( user, "ItemPickup" );
                                WRITE_STRING( GetClassname() );
                        MessageEnd();

                        CPASAttenuationFilter filter( pPlayer, "HealthVial.Touch" );
                        EmitSound( filter, pPlayer->entindex(), "HealthVial.Touch" );

                        if ( g_pGameRules->ItemShouldRespawn( this ) )
                        {
                                Respawn();
                        }
                        else
                        {
                                UTIL_Remove(this);        
                        }

                        return true;
                }

                return false;
        }
};

LINK_ENTITY_TO_CLASS( item_healthvial, CHealthVial );
PRECACHE_REGISTER( item_healthvial );

//-----------------------------------------------------------------------
// Wall Charger
//-----------------------------------------------------------------------

LINK_ENTITY_TO_CLASS(item_healthcharger, CNewWallHealth);

BEGIN_DATADESC( CNewWallHealth )

        DEFINE_FIELD( m_flNextCharge, FIELD_TIME),
        DEFINE_FIELD( m_iReactivate, FIELD_INTEGER),
        DEFINE_FIELD( m_iJuice, FIELD_INTEGER),
        DEFINE_FIELD( m_iOn, FIELD_INTEGER),
        DEFINE_FIELD( m_flSoundTime, FIELD_TIME),
        DEFINE_FIELD( m_nState, FIELD_INTEGER ),
        DEFINE_FIELD( m_iCaps, FIELD_INTEGER ),
        DEFINE_FIELD( m_flJuice, FIELD_FLOAT ),
        DEFINE_KEYFIELD( m_iTeamNum, FIELD_INTEGER, "team" ),
        DEFINE_KEYFIELD( m_iSourcefortsTeamNum, FIELD_INTEGER, "TeamNum" ),

        // Function Pointers
        DEFINE_FUNCTION( Off ),
        DEFINE_FUNCTION( Recharge ),

        DEFINE_OUTPUT( m_OnPlayerUse, "OnPlayerUse" ),
        DEFINE_OUTPUT( m_OutRemainingHealth, "OutRemainingHealth"),

END_DATADESC()

#define HEALTH_CHARGER_MODEL_NAME "models/props_combine/health_charger001.mdl"
#define CHARGE_RATE 0.25f
#define CHARGES_PER_SECOND 1.0f / CHARGE_RATE
#define CALLS_PER_SECOND 7.0f * CHARGES_PER_SECOND

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pkvd - 
//-----------------------------------------------------------------------------
bool CNewWallHealth::KeyValue(  const char *szKeyName, const char *szValue )
{
        if (FStrEq(szKeyName, "style") ||
                FStrEq(szKeyName, "height") ||
                FStrEq(szKeyName, "value1") ||
                FStrEq(szKeyName, "value2") ||
                FStrEq(szKeyName, "value3"))
        {
                return(true);
        }
        else if (FStrEq(szKeyName, "dmdelay"))
        {
                m_iReactivate = atoi(szValue);
                return(true);
        }

        return(BaseClass::KeyValue( szKeyName, szValue ));
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNewWallHealth::Spawn(void)
{
        Precache( );

        SetMoveType( MOVETYPE_NONE );
        SetSolid( SOLID_VPHYSICS );
        CreateVPhysics();

        SetModel( HEALTH_CHARGER_MODEL_NAME );
        AddEffects( EF_NOSHADOW );

        ResetSequence( LookupSequence( "idle" ) );

        m_iJuice = sk_healthcharger.GetFloat();

        m_nState = 0;        
        
        m_iReactivate = 0;
        m_iCaps        = FCAP_CONTINUOUS_USE;

        CreateVPhysics();

        m_flJuice = m_iJuice;
        SetCycle( 1.0f - ( m_flJuice /  sk_healthcharger.GetFloat() ) );
        if ( ClassicGameRules()->IsSourcefortsMap() )
        {
                ChangeTeam( m_iSourcefortsTeamNum );
        }
}

int CNewWallHealth::DrawDebugTextOverlays(void) 
{
        int text_offset = BaseClass::DrawDebugTextOverlays();

        if (m_debugOverlays & OVERLAY_TEXT_BIT) 
        {
                char tempstr[512];
                Q_snprintf(tempstr,sizeof(tempstr),"Charge left: %i", m_iJuice );
                EntityText(text_offset,tempstr,0);
                text_offset++;
        }
        return text_offset;
}

//-----------------------------------------------------------------------------

bool CNewWallHealth::CreateVPhysics(void)
{
        VPhysicsInitStatic();
        return true;
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNewWallHealth::Precache(void)
{
        PrecacheModel( HEALTH_CHARGER_MODEL_NAME );

        PrecacheScriptSound( "WallHealth.Deny" );
        PrecacheScriptSound( "WallHealth.Start" );
        PrecacheScriptSound( "WallHealth.LoopingContinueCharge" );
        PrecacheScriptSound( "WallHealth.Recharge" );
}

void CNewWallHealth::StudioFrameAdvance( void )
{
        m_flPlaybackRate = 0;

        float flMaxJuice = sk_healthcharger.GetFloat();
        
        SetCycle( 1.0f - (float)( m_flJuice / flMaxJuice ) );
//        Msg( "Cycle: %f - Juice: %d - m_flJuice :%f - Interval: %f\n", (float)GetCycle(), (int)m_iJuice, (float)m_flJuice, GetAnimTimeInterval() );

        if ( !m_flPrevAnimTime )
        {
                m_flPrevAnimTime = gpGlobals->curtime;
        }

        // Latch prev
        m_flPrevAnimTime = m_flAnimTime;
        // Set current
        m_flAnimTime = gpGlobals->curtime;
}

//-----------------------------------------------------------------------------
// Purpose: 
// Input  : *pActivator - 
//                        *pCaller - 
//                        useType - 
//                        value - 
//-----------------------------------------------------------------------------
void CNewWallHealth::Use( CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value )
{ 
        // Make sure that we have a caller
        if (!pActivator || !pActivator->IsPlayer())
        {
                return;
        }

        // if it's not a player, ignore
        if ( !pActivator->IsPlayer() )
                return;
        CBasePlayer *pPlayer = dynamic_cast<CBasePlayer *>(pActivator);

        if ( GetTeamNumber() != TEAM_SPECTATOR && pActivator->GetTeamNumber() != GetTeamNumber() )
        {
                ClientPrint( pPlayer, HUD_PRINTTALK, "Cannot recharge from enemy charger" );
                CPASAttenuationFilter sndFilter( this, "Player.UseDeny" );
                EmitSound( sndFilter, entindex(), "Player.UseDeny" );
                return;
        }

        // Reset to a state of continuous use.
        m_iCaps = FCAP_CONTINUOUS_USE;

        if ( m_iOn )
        {
                float flCharges = CHARGES_PER_SECOND;
                float flCalls = CALLS_PER_SECOND;

                m_flJuice -= flCharges / flCalls;
                StudioFrameAdvance();
        }

        // if there is no juice left, turn it off
        if (m_iJuice <= 0)
        {
                ResetSequence( LookupSequence( "emptyclick" ) );
                m_nState = 1;                        
                Off();
        }

        // if the player doesn't have the suit, or there is no juice left, make the deny noise.
        // disabled HEV suit dependency for now.
        //if ((m_iJuice <= 0) || (!(pActivator->m_bWearingSuit)))
        if (m_iJuice <= 0)
        {
                if (m_flSoundTime <= gpGlobals->curtime)
                {
                        m_flSoundTime = gpGlobals->curtime + 0.62;
                        EmitSound( "WallHealth.Deny" );
                }
                return;
        }

        if( pActivator->GetHealth() >= pActivator->GetMaxHealth() )
        {
                if( pPlayer )
                {
                        pPlayer->m_afButtonPressed &= ~IN_USE;
                }

                // Make the user re-use me to get started drawing health.
                m_iCaps = FCAP_IMPULSE_USE;
                
                EmitSound( "WallHealth.Deny" );
                return;
        }

        SetNextThink( gpGlobals->curtime + CHARGE_RATE );
        SetThink( &CNewWallHealth::Off );

        // Time to recharge yet?

        if (m_flNextCharge >= gpGlobals->curtime)
                return;

        // Play the on sound or the looping charging sound
        if (!m_iOn)
        {
                m_iOn++;
                EmitSound( "WallHealth.Start" );
                m_flSoundTime = 0.56 + gpGlobals->curtime;

                m_OnPlayerUse.FireOutput( pActivator, this );
        }
        if ((m_iOn == 1) && (m_flSoundTime <= gpGlobals->curtime))
        {
                m_iOn++;
                CPASAttenuationFilter filter( this, "WallHealth.LoopingContinueCharge" );
                filter.MakeReliable();
                EmitSound( filter, entindex(), "WallHealth.LoopingContinueCharge" );
        }

        // charge the player
        if ( pActivator->TakeHealth( 1, DMG_GENERIC ) )
        {
                m_iJuice--;
        }

        // Send the output.
        float flRemaining = m_iJuice / sk_healthcharger.GetFloat();
        m_OutRemainingHealth.Set(flRemaining, pActivator, this);

        // govern the rate of charge
        m_flNextCharge = gpGlobals->curtime + 0.1;
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNewWallHealth::Recharge(void)
{
        EmitSound( "WallHealth.Recharge" );
        m_flJuice = m_iJuice = sk_healthcharger.GetFloat();
        m_nState = 0;

        ResetSequence( LookupSequence( "idle" ) );
        StudioFrameAdvance();

        m_iReactivate = 0;

        SetThink( NULL );
}


//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CNewWallHealth::Off(void)
{
        // Stop looping sound.
        if (m_iOn > 1)
        {
                StopSound( "WallHealth.LoopingContinueCharge" );
        }

        if ( m_nState == 1 )
        {
                SetCycle( 1.0f );
        }

        m_iOn = 0;
        m_flJuice = m_iJuice;

        if ( m_iReactivate == 0 )
        {
                if ((!m_iJuice) && lf_combat_charger_next_reactivate.GetInt() > 0 )
                {
                        m_iReactivate = lf_combat_charger_next_reactivate.GetInt();
                        SetNextThink( gpGlobals->curtime + m_iReactivate );
                        SetThink(&CNewWallHealth::Recharge);
                }
                else
                {
                        SetThink( NULL );
                }
        }
}
