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


/*===============================================================
	Shared

	"Balance Macros"

	Used for tweaking and balancing weapons during the development
	phase.

	In a Debug Build, the variables for balancing are actually
	ConVars that can be modified. In a Release Build they're just
	global unions (to allow for float or integer interpretation.)

	MAKE SURE BALANCE VARIABLES ARE SHARED (across client and server)
	OR HAVE UNDESIRABLE SIDE EFFECTS.

	Last Updated March 1, 2010
===============================================================*/

#include "cbase.h"
#include "convar.h"
#include "Mod/BalanceMacros.h"

#ifdef MOD_DEV

#ifdef DEBUG
//
//	Name: CC_BalanceMacroReport( const CCommand& args )
//	Author: Hekar Khani
//	Description: ConCommand to display a simple report of the current balance macros
//	Notes:
//
void CC_BalanceMacroReport( const CCommand& args )
{
	ConCommandBase *Command = cvar->GetCommands();
	for ( ; Command != NULL; Command = Command->GetNext())
	{
		ConVar *convar = dynamic_cast< ConVar * > ( Command );
		if ( !convar )
		{
			continue;
		}

		const char *convarname = convar->GetName();
		if ( Q_strstr( convarname, "lfm" ) )
		{
			Msg( "Name: %s\t\tValue: %s\t\tDesc: %s\n", convar->GetName(), convar->GetString(), convar->GetHelpText() );
		}
	}
}

static ConCommand lfm_macro_report( "lfm_macro_report", CC_BalanceMacroReport, "Balance Macro Report" );

//
//	Name: CC_BalanceMacroReport( const CCommand& args )
//	Author: Hekar Khani
//	Description: ConCommand to display a report of the current balance macros in CSV format
//	Notes:
//
void CC_BalanceMacroReportCSV( const CCommand& args )
{
	// HODO: add ability to write to file
	ConCommandBase *Command = cvar->GetCommands();
	for ( ; Command != NULL; Command = Command->GetNext())
	{
		ConVar *convar = dynamic_cast< ConVar * > ( Command );
		if ( !convar )
		{
			continue;
		}

		Msg( "Convar, Value, Description\n" );
		const char *convarname = convar->GetName();
		if ( Q_strstr( convarname, "lfm" ) )
		{
			Msg( "%s, %s, %s\n", convar->GetName(), convar->GetString(), convar->GetHelpText() );
		}
	}
}

static ConCommand lfm_macro_report_csv( "lfm_macro_report_csv", CC_BalanceMacroReportCSV, "Balance Macros in CSV format" );
#endif // DEBUG

#endif // MOD_DEV
