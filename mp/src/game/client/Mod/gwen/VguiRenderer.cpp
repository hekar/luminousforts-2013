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

// Disable some of the debugging includes from Valve

#include "cbase.h"
#ifdef GWEN_GUI
#define _RETAIL
#undef DBGFLAG_ASSERT
#undef DBGFLAG_ASSERTFATAL

#pragma warning( disable : 4005 )
#include <math.h>
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include "Mod/gwen/VguiRenderer.h"

namespace Gwen
{
	namespace Renderer
	{

		VguiRenderer::VguiRenderer( vgui::ISurface *pSurface, vgui::ISchemeManager *pSchemeManager )
		{
			m_pSurface = pSurface;
			m_pSchemeManager = pSchemeManager;
			m_UseClip = false;
		}

		VguiRenderer::~VguiRenderer()
		{
		}

		void VguiRenderer::Begin()
		{
			m_UseClip = false;
		}

		void VguiRenderer::End()
		{
		}

		void VguiRenderer::SetDrawColor( Color color )
		{
			m_pSurface->DrawSetColor( color.r, color.g, color.b, color.a );
			m_pSurface->DrawSetTextColor( color.r, color.g, color.b, color.a );
		}

		void VguiRenderer::DrawFilledRect( Gwen::Rect rect )
		{
			m_pSurface->DrawFilledRect( rect.x, rect.y, rect.x + rect.w, rect.y + rect.h );
		}

		void VguiRenderer::StartClip()
		{
			m_UseClip = true;
		}

		void VguiRenderer::EndClip()
		{
			m_UseClip = false;
		}

		void VguiRenderer::LoadTexture( Gwen::Texture* pTexture )
		{
			char texture[ 512 ] = {'\0'};
			wcstombs( texture, pTexture->name.GetUnicode().c_str(), sizeof( texture ) );
			int iTextureID = m_pSurface->DrawGetTextureId( texture );

			if (iTextureID == -1)
			{
				iTextureID = m_pSurface->CreateNewTextureID();
				m_pSurface->DrawSetTextureFile( iTextureID, texture, false, true );

				pTexture->data = (int*)iTextureID;
				pTexture->width = 511;
				pTexture->height = 494;
			}
		}

		void VguiRenderer::FreeTexture( Gwen::Texture* pTexture )
		{
		}

		void VguiRenderer::DrawTexturedRect( Gwen::Texture* pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 )
		{
			m_pSurface->DrawSetTexture( reinterpret_cast<int>( pTexture->data ) );
			if ( m_UseClip )
			{
				const Gwen::Rect& clip = ClipRegion();
				m_pSurface->DrawTexturedSubRect(
					clamp( abs( clip.x + rect.x ), clip.x, clip.x + clip.w ),
					clamp( abs( clip.y + rect.y ), clip.y, clip.y + clip.h ),
					clamp( abs( clip.x + rect.x + rect.w ), clip.x, clip.x + clip.w ),
					clamp( abs( clip.y + rect.y + rect.h ), clip.y, clip.y + clip.h ),
					u1, v1, u2, v2
				);
			}
			else
			{
				m_pSurface->DrawTexturedRect( rect.x, rect.y, rect.x + rect.w, rect.y + rect.h );
			}
		}

		void VguiRenderer::DrawMissingImage( Gwen::Rect pTargetRect )
		{
			m_pSurface->DrawSetColor( 255, 0, 0, 255 );
			DrawFilledRect( pTargetRect );
		}

		Gwen::Color VguiRenderer::PixelColour( Gwen::Texture* pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default )
		{
			return col_default;
		}

		void VguiRenderer::LoadFont( Gwen::Font* font )
		{
			char fontName[ 512 ] = {'\0'};
			wcstombs( fontName, font->facename.c_str(), sizeof( fontName ) );

			int fontSize = m_pSchemeManager->GetProportionalScaledValue( font->size );
			static vgui::HFont vFont = m_pSurface->CreateFont();
			m_pSurface->SetFontGlyphSet( vFont, fontName, fontSize,
				font->bold ? 400 : 600, 0, 0, 0 );

			font->data = &vFont;
		}

		void VguiRenderer::FreeFont( Gwen::Font* font )
		{
		}

		void VguiRenderer::RenderText( Gwen::Font* pFont, Gwen::Point pos, const Gwen::UnicodeString& text )
		{
			vgui::HFont font = *((vgui::HFont *)pFont->data);
			m_pSurface->DrawSetTextFont( font );
			if ( m_UseClip )
			{
				const Gwen::Rect& clip = ClipRegion();
				m_pSurface->DrawSetTextPos( clip.x + pos.x, clip.y + pos.y );
			}
			else
			{
				m_pSurface->DrawSetTextPos( pos.x, pos.y );
			}
			m_pSurface->DrawPrintText( text.c_str(), wcslen( text.c_str() ) );
		}

		Gwen::Point VguiRenderer::MeasureText( Gwen::Font* pFont, const Gwen::UnicodeString& text )
		{
			if ( pFont->data <= 0 )
			{
				LoadFont( pFont );
			}

			char fontName[ 512 ] = {'\0'};
			wcstombs( fontName, pFont->facename.c_str(), sizeof( fontName ) );

			int w, h;
			vgui::HFont font = *((vgui::HFont *)pFont->data);
			m_pSurface->GetTextSize( font, text.c_str(), w, h );

			return Gwen::Point( w, h );
		}
	}
}
#endif
