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
#include "MGUI_Base.h"
#include "MGUI_Factory.h"

#include "MGUI_BorderLessButton.h"
#include "MGUI_CvarCheckButton.h"
#include "MGUI_CvarSlider.h"
#include "MGUI_CvarToggleButton.h"
#include "MGUI_ComboBox.h"
#include "MGUI_Button.h"

// Create controls for Modification Panels
vgui::Panel *CreateModControlByName( vgui::Panel *parent, const char *controlName )
{
	if ( !Q_stricmp( controlName, "CvarCheckButton" ) )
	{
		return new MCvarCheckButton( parent, "CvarCheckButton", "" );
	}
	else if ( !Q_stricmp( controlName, "CvarSlider" ) )
	{
		return new MCvarSlider( parent, "CvarSlider" );
	}
	else if ( !Q_stricmp( controlName, "CvarComboBox" ) )
	{
		return new MComboBox( parent, "CvarComboBox" );
	}
	else if ( !Q_stricmp( controlName, "ConButton" ) )
	{
		return new MButton( parent, "ConButton" );
	}

	return NULL;
}