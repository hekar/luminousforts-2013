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

/*===============================================
 LuminousForts:
	Bootstrap Gwen
===============================================*/

#include "cbase.h"
#ifdef MOD_GWEN
#include "vgui_controls/Panel.h"
#include <vgui/ISurface.h>
#include "BaseGUI.h"
#include "Gwen.h"

static Gwen::Renderer::Base *g_pRenderer  = NULL;
static Gwen::Skin::Base *g_pSkin = NULL;

CREATE_GUI_INTERFACE( CGwenStartFactory, gwenbootstrap );

namespace Gwen
{
	Gwen::Renderer::Base *GetGlobalRenderer()
	{
		Assert( g_pRenderer );
		return g_pRenderer;
	}

	Gwen::Skin::Base *GetGlobalSkin()
	{
		Assert( g_pSkin );
		return g_pSkin;
	}
};

CGwenStart::CGwenStart( vgui::VPANEL parent ) : 
	vgui::Panel( NULL, "GwenStart" )
{
	SetParent( parent );
	SetVisible( false );
	int x, y, w, h;
	vgui::surface()->GetWorkspaceBounds( x, y, w, h );
	SetBounds( x, y, w, h );

	vgui::ISurface *surface = vgui::surface();
	if ( !g_pRenderer )
	{
		g_pRenderer = new Gwen::Renderer::VguiRenderer( surface, vgui::scheme() );
	}

	if ( !g_pSkin )
	{
		Gwen::Skin::TexturedBase *skin = new Gwen::Skin::TexturedBase();
		skin->SetRender( g_pRenderer );
		skin->Init( "gwen/DefaultSkin" );
		skin->SetDefaultFont( L"Tahoma", 11 );
		g_pSkin = skin;
	}
}

CGwenStart::~CGwenStart()
{
	delete g_pRenderer;
	//delete g_pSkin;
}

#endif // MOD_GWEN
