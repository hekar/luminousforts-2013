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

#include "cbase.h"
#ifdef GWEN_GUI
#include "BaseGUI.h"
#include <vgui/IScheme.h>
#include <vgui/ISurface.h>
#include "Gwen_Update.h"

CREATE_GUI_INTERFACE( CGwenUpdateFactory, gwenupdate );

CGwenUpdate::CGwenUpdate( vgui::VPANEL parent ) :
	BaseClass( parent )
{
	LoadControlSettings( "resource/ui/GwenUpdate.res" );

	m_Desc = new Gwen::Controls::Label( m_InnerPanel );
	m_Desc->SetTextColor( Gwen::Colors::Black );
	m_Desc->SetBounds( 10, 5, 500, 24 );
	m_Desc->SetText( "Updating LuminousForts" );

	m_Progress = new Gwen::Controls::ProgressBar( m_InnerPanel );
	m_Progress->SetBounds( 10, 38, 350, 24 );

	m_Button = new Gwen::Controls::Button( m_InnerPanel );
	m_Button->SetBounds( 375, 30, 72, 32 );
	m_Button->SetText( "Cancel" );
	m_Button->SetTextColorOverride( Gwen::Colors::Black );
	//m_Button->onPress.Add( this, &CGwenUpdate::OnCancel );

	SetTitle( "Auto Updater", false );
	SetSize( 475, 115 );
	SetSizeable( false );
}

CGwenUpdate::~CGwenUpdate()
{
	// TODO: Does Gwen auto delete?
	delete m_Desc;
	delete m_Progress;
	delete m_Button;
}

void CGwenUpdate::Paint()
{
}

void CGwenUpdate::OnCancel( Gwen::Controls::Base *control )
{

	// Cancel update
	SetVisible( false );
}
#endif
