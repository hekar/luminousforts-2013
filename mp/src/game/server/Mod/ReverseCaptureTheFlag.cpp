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

/**
 * \file ReverseCaptureTheFlag.cpp
 * \author Hekar Khani
 * Handles miscellaneous PostInit (after map load) initializations and events
 * for the reverse capture the flag gamemode (ie. adding the tag to the server)
 */

#include "cbase.h"
#include "Mod/ServerTags.h"
#include "Mod/SharedModCvars.h"

#define REVERSE_CTF_TAG "reverse_ctf"

class CReverseCTFGameSystem : public CAutoGameSystem
{
public:
	CReverseCTFGameSystem() :
	  CAutoGameSystem( "ReverseCTFGameSystem" )
	{
	}

	void LevelInitPostEntity()
	{
		if ( GameRules()->GetGameMode() == GAMEMODE_CLASSIC && 
				lf_reverse_flags.GetBool() )
		{
			AddTag( REVERSE_CTF_TAG );
		}
		else
		{
			RemoveTag( REVERSE_CTF_TAG );
		}
	}
};

static CReverseCTFGameSystem g_ReverseCTFHelper;
