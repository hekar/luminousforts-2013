//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================//

#include "cbase.h"
#include "hud.h"
#include "hud_crosshair.h"
#include "iclientmode.h"
#include "view.h"
#include "vgui_controls/Controls.h"
#include "vgui/ISurface.h"
#include "ivrenderview.h"
#include "materialsystem/imaterialsystem.h"
#include "VGuiMatSurface/IMatSystemSurface.h"
#include "client_virtualreality.h"
#include "headtrack/isourcevirtualreality.h"
#include "Mod/HudH_Utils.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// Character to draw crosshair on
ConVar lf_hud_crosshair( "lf_hud_crosshair", "D", FCVAR_ARCHIVE, "Crosshair character to use" );
ConVar lf_hud_crosshair_red( "lf_hud_crosshair_red", "68", FCVAR_ARCHIVE, "Crosshair Red Value" );
ConVar lf_hud_crosshair_green( "lf_hud_crosshair_green", "192", FCVAR_ARCHIVE, "Crosshair Green Value" );
ConVar lf_hud_crosshair_blue( "lf_hud_crosshair_blue", "68", FCVAR_ARCHIVE, "Crosshair Blue Value" );
ConVar lf_hud_crosshair_alpha( "lf_hud_crosshair_alpha", "255", FCVAR_ARCHIVE, "Crosshair Alpha Value" );

ConVar crosshair( "crosshair", "1", FCVAR_ARCHIVE );
ConVar cl_observercrosshair( "cl_observercrosshair", "1", FCVAR_ARCHIVE );

using namespace vgui;

int ScreenTransform( const Vector& point, Vector& screen );

DECLARE_HUDELEMENT( CHudCrosshair );

CHudCrosshair::CHudCrosshair( const char *pElementName ) :
CHudElement( pElementName ), BaseClass( NULL, "HudCrosshair" )
{
	vgui::Panel *pParent = g_pClientMode->GetViewport();
	SetParent( pParent );

	m_cPreviousCrosshair = 0;
	m_pCrosshair = 0;

	m_clrCrosshair = Color( 0, 0, 0, 0 );

	m_vecCrossHairOffsetAngle.Init();

	SetHiddenBits( HIDEHUD_PLAYERDEAD | HIDEHUD_CROSSHAIR );
}

void CHudCrosshair::ApplySchemeSettings( IScheme *scheme )
{
	BaseClass::ApplySchemeSettings( scheme );

	m_pDefaultCrosshair = gHUD.GetIcon( "crosshair_default" );
	SetPaintBackgroundEnabled( false );

	SetSize( ScreenWidth(), ScreenHeight() );
}

//-----------------------------------------------------------------------------
// Purpose: Save CPU cycles by letting the HUD system early cull
// costly traversal.  Called per frame, return true if thinking and 
// painting need to occur.
//-----------------------------------------------------------------------------
bool CHudCrosshair::ShouldDraw( void )
{
	bool bNeedsDraw;

	if ( m_bHideCrosshair )
		return false;

	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer();
	if ( !pPlayer )
		return false;

	C_BaseCombatWeapon *pWeapon = pPlayer->GetActiveWeapon();
	if ( pWeapon && !pWeapon->ShouldDrawCrosshair() )
		return false;

	// draw a crosshair only if alive or spectating in eye
	if ( IsX360() )
	{
		bNeedsDraw = m_pCrosshair &&
			!engine->IsDrawingLoadingImage() &&
			!engine->IsPaused() &&
			(!pPlayer->IsSuitEquipped() || g_pGameRules->IsMultiplayer()) &&
			g_pClientMode->ShouldDrawCrosshair() &&
			!(pPlayer->GetFlags() & FL_FROZEN) &&
			(pPlayer->entindex() == render->GetViewEntity()) &&
			(pPlayer->IsAlive() || (pPlayer->GetObserverMode() == OBS_MODE_IN_EYE) || (cl_observercrosshair.GetBool() && pPlayer->GetObserverMode() == OBS_MODE_ROAMING));
	}
	else
	{
		bNeedsDraw = m_pCrosshair &&
			crosshair.GetInt() &&
			!engine->IsDrawingLoadingImage() &&
			!engine->IsPaused() &&
			g_pClientMode->ShouldDrawCrosshair() &&
			!(pPlayer->GetFlags() & FL_FROZEN) &&
			(pPlayer->entindex() == render->GetViewEntity()) &&
			!pPlayer->IsInVGuiInputMode() &&
			(pPlayer->IsAlive() || (pPlayer->GetObserverMode() == OBS_MODE_IN_EYE) || (cl_observercrosshair.GetBool() && pPlayer->GetObserverMode() == OBS_MODE_ROAMING));
	}

	return (bNeedsDraw && CHudElement::ShouldDraw());
}

void CHudCrosshair::Paint( void )
{
	if ( lf_hud_crosshair.GetString()[0] != m_cPreviousCrosshair )
	{
		// Load the new crosshair that the player has selected
		InitFontTexture( m_pCrosshair, "Crosshairs", lf_hud_crosshair.GetString()[0], "crosshair" );
		m_cPreviousCrosshair = lf_hud_crosshair.GetString()[0];
	}

	if ( !m_pCrosshair )
		return;

	if ( !IsCurrentViewAccessAllowed() )
		return;

	m_curViewAngles = CurrentViewAngles();
	m_curViewOrigin = CurrentViewOrigin();

	float x, y;
	x = ScreenWidth() / 2;
	y = ScreenHeight() / 2;

	// MattB - m_vecCrossHairOffsetAngle is the autoaim angle.
	// if we're not using autoaim, just draw in the middle of the 
	// screen
	if ( m_vecCrossHairOffsetAngle != vec3_angle )
	{
		QAngle angles;
		Vector forward;
		Vector point, screen;

		// this code is wrong
		angles = m_curViewAngles + m_vecCrossHairOffsetAngle;
		AngleVectors( angles, &forward );
		VectorAdd( m_curViewOrigin, forward, point );
		ScreenTransform( point, screen );

		x += 0.5f * screen[0] * ScreenWidth() + 0.5f;
		y += 0.5f * screen[1] * ScreenHeight() + 0.5f;
	}

	m_pCrosshair->DrawSelf(
		x - 0.5f * m_pCrosshair->Width(),
		y - 0.5f * m_pCrosshair->Height(),
		m_clrCrosshair );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudCrosshair::SetCrosshairAngle( const QAngle& angle )
{
	VectorCopy( angle, m_vecCrossHairOffsetAngle );
}

//-----------------------------------------------------------------------------
// Purpose: 
//-----------------------------------------------------------------------------
void CHudCrosshair::SetCrosshair( CHudTexture *texture, Color& clr )
{
	m_pCrosshair = texture;
	m_clrCrosshair = Color( lf_hud_crosshair_red.GetInt(), 
		lf_hud_crosshair_green.GetInt(), 
		lf_hud_crosshair_blue.GetInt(), lf_hud_crosshair_alpha.GetInt() );
}

//-----------------------------------------------------------------------------
// Purpose: Resets the crosshair back to the default
//-----------------------------------------------------------------------------
void CHudCrosshair::ResetCrosshair()
{
	Color color = Color( 255, 255, 255, 255 );
	SetCrosshair( m_pDefaultCrosshair, color );
}

void CHudCrosshair::GetDrawPosition( float *pX, float *pY, bool *pbBehindCamera, QAngle angleCrosshairOffset )
{
	QAngle curViewAngles = CurrentViewAngles( );
	Vector curViewOrigin = CurrentViewOrigin( );

	int vx, vy, vw, vh;
	vgui::surface( )->GetFullscreenViewport( vx, vy, vw, vh );

	float screenWidth = vw;
	float screenHeight = vh;

	float x = screenWidth / 2;
	float y = screenHeight / 2;

	bool bBehindCamera = false;

	C_BasePlayer* pPlayer = C_BasePlayer::GetLocalPlayer( );
	if ( (pPlayer != NULL) && (pPlayer->GetObserverMode( ) == OBS_MODE_NONE) )
	{
		bool bUseOffset = false;

		Vector vecStart;
		Vector vecEnd;

		if ( UseVR( ) )
		{
			// These are the correct values to use, but they lag the high-speed view data...
			vecStart = pPlayer->Weapon_ShootPosition( );
			Vector vecAimDirection = pPlayer->GetAutoaimVector( 1.0f );
			// ...so in some aim modes, they get zapped by something completely up-to-date.
			g_ClientVirtualReality.OverrideWeaponHudAimVectors( &vecStart, &vecAimDirection );
			vecEnd = vecStart + vecAimDirection * MAX_TRACE_LENGTH;

			bUseOffset = true;
		}

#ifdef SIXENSE
		// TODO: actually test this Sixsense code interaction with things like HMDs & stereo.
		if ( g_pSixenseInput->IsEnabled( ) && !UseVR( ) )
		{
			// Never autoaim a predicted weapon (for now)
			vecStart = pPlayer->Weapon_ShootPosition( );
			Vector aimVector;
			AngleVectors( CurrentViewAngles( ) - g_pSixenseInput->GetViewAngleOffset( ), &aimVector );
			// calculate where the bullet would go so we can draw the cross appropriately
			vecEnd = vecStart + aimVector * MAX_TRACE_LENGTH;
			bUseOffset = true;
		}
#endif

		if ( bUseOffset )
		{
			trace_t tr;
			UTIL_TraceLine( vecStart, vecEnd, MASK_SHOT, pPlayer, COLLISION_GROUP_NONE, &tr );

			Vector screen;
			screen.Init( );
			bBehindCamera = ScreenTransform( tr.endpos, screen ) != 0;

			x = 0.5f * (1.0f + screen[0]) * screenWidth + 0.5f;
			y = 0.5f * (1.0f - screen[1]) * screenHeight + 0.5f;
		}
	}

	// MattB - angleCrosshairOffset is the autoaim angle.
	// if we're not using autoaim, just draw in the middle of the 
	// screen
	if ( angleCrosshairOffset != vec3_angle )
	{
		QAngle angles;
		Vector forward;
		Vector point, screen;

		// this code is wrong
		angles = curViewAngles + angleCrosshairOffset;
		AngleVectors( angles, &forward );
		VectorAdd( curViewOrigin, forward, point );
		ScreenTransform( point, screen );

		x += 0.5f * screen[0] * screenWidth + 0.5f;
		y += 0.5f * screen[1] * screenHeight + 0.5f;
	}

	*pX = x;
	*pY = y;
	*pbBehindCamera = bBehindCamera;
}
