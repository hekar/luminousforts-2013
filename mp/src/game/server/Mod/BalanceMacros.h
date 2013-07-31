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


#ifndef BALANCEMACROS_H
#define BALANCEMACROS_H

#ifdef WIN32
#pragma once
#endif

// Force ConVar load before Balance Macros are defined
#include "convar.h"

// Allow for float and integer interpretation
typedef union
{
	int ValueInt;
	float ValueFloat;

} BalanceMacro_t;

// In a debug build we're gonna use convars
// and const variables in a release build
#ifdef DEBUG
	#define BALANCE_DEFINEH( name, defaultvalue, helptext ) \
		ConVar name( #name, #defaultvalue, FCVAR_ARCHIVE | FCVAR_REPLICATED, (helptext) );
	#define BALANCE_DEFINE( name, defaultvalue ) \
		BALANCE_DEFINEH( name, defaultvalue, "Balance Macro" )
	#define BALANCE_EXTERN( name ) \
		extern ConVar name;
#else
	#define BALANCE_DEFINEH( name, defaultvalue, helptext ) \
		const BalanceMacro_t name = {defaultvalue};
	#define BALANCE_DEFINE( name, defaultvalue ) \
		BALANCE_DEFINEH( name, defaultvalue, "Balance Macro" )
	#define BALANCE_EXTERN( name ) \
		extern BalanceMacro_t name;
#endif

// We need a generic way to get at the variables.
#ifdef DEBUG
	#define BALANCE_VALUE( type, name ) \
		name.Get##type()
#else
	#define BALANCE_VALUE( type, name ) \
		name.Value##type
#endif

#endif // BALANCEMACROS_H