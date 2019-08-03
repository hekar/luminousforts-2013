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

#ifndef VGUI_RENDERER_H
#define VGUI_RENDERER_H

#include "Gwen/Utility.h"
#include "Gwen/Font.h"
#include "Gwen/Texture.h"
#include "Gwen/BaseRender.h"

namespace Gwen
{
	namespace Renderer
	{
		class VguiRenderer : public Base
		{
		public:
			VguiRenderer( vgui::ISurface *pSurface, vgui::ISchemeManager *pSchemeManager );
			virtual ~VguiRenderer();

			virtual void Begin();
			virtual void End();
			virtual void SetDrawColor( Color color );
			virtual void DrawFilledRect( Gwen::Rect rect );

			virtual void StartClip();
			virtual void EndClip();

			virtual void LoadTexture( Gwen::Texture* pTexture );
			virtual void FreeTexture( Gwen::Texture* pTexture );

			virtual void DrawTexturedRect( Gwen::Texture *pTexture, Gwen::Rect rect, float u1, float v1, float u2, float v2 );
			virtual void DrawMissingImage( Gwen::Rect pTargetRect );

			virtual Gwen::Color PixelColour( Gwen::Texture *pTexture, unsigned int x, unsigned int y, const Gwen::Color& col_default );

			virtual void LoadFont( Gwen::Font *font );
			virtual void FreeFont( Gwen::Font *font );

			virtual void RenderText( Gwen::Font *pFont, Gwen::Point pos, const Gwen::UnicodeString& text );
			virtual Gwen::Point MeasureText( Gwen::Font *pFont, const Gwen::UnicodeString& text );

		private:
			vgui::ISurface *m_pSurface;
			vgui::ISchemeManager *m_pSchemeManager;
			bool m_UseClip;
		};
	};
};

#endif // VGUI_RENDERER_H
