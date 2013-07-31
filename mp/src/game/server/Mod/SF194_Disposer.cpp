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
	Server
	SF Disposer Brush Entity

	For backwards compatibility with SF 194.

	Everything in the classic Flag applies to this flag, there's
	just a layer for compatibility.

	Last Updated Dec 08, 2009
===============================================================*/
#include "cbase.h"

#ifdef MOD_SF194
#include "CDisposerBase.h"
#include "CDisposerClassic.h"

// Original disposer is the same as SF 195's
LINK_ENTITY_TO_CLASS( func_disposer, CBlockDisposerClassic );

class CDisposerSF194 : public CBlockDisposerClassic
{
	DECLARE_CLASS( CDisposerSF194, CBlockDisposerClassic )
	DECLARE_DATADESC()
public:
	CDisposerSF194();
	~CDisposerSF194();

	bool OnFlagTouch( CBaseEntity *pOther );
	bool OnBlockTouch( CBaseEntity *pOther );
	bool OnPlayerTouch( CBaseEntity *pOther );

private:
	int m_bKillBlock;
	int m_bKillFlag;

	int m_bKillRed;
	int m_bKillBlue;
};

LINK_ENTITY_TO_CLASS( func_disposer_selective, CDisposerSF194 );
BEGIN_DATADESC( CDisposerSF194 )

	DEFINE_KEYFIELD( m_bKillBlock, FIELD_INTEGER, "KillBlocks" ),
	DEFINE_KEYFIELD( m_bKillFlag, FIELD_INTEGER, "KillFlag" ),
	DEFINE_KEYFIELD( m_bKillRed, FIELD_INTEGER, "KillRed" ),
	DEFINE_KEYFIELD( m_bKillBlue, FIELD_INTEGER, "KillBlue" ),

END_DATADESC()

//
//	Name: CDisposerSF194
//	Author: Hekar Khani
//	Description: Backward compatible disposer for Sourceforts 194 maps
//	Notes:
//
CDisposerSF194::CDisposerSF194()
{
	m_bKillBlock = FALSE;
	m_bKillFlag = FALSE;
	m_bKillRed = FALSE;
	m_bKillBlue = FALSE;
}

CDisposerSF194::~CDisposerSF194()
{
}

bool CDisposerSF194::OnFlagTouch( CBaseEntity *pOther )
{
	if ( m_bKillFlag )
	{
		return BaseClass::OnFlagTouch( pOther );
	}

	return false;
}

bool CDisposerSF194::OnBlockTouch( CBaseEntity *pOther )
{
	if ( m_bKillBlock )
	{
		return BaseClass::OnBlockTouch( pOther );
	}

	return false;
}

bool CDisposerSF194::OnPlayerTouch( CBaseEntity *pOther )
{
	if ( m_bKillRed && pOther->GetTeamNumber() == SDK_TEAM_RED )
	{
		return BaseClass::OnPlayerTouch( pOther );
	}
	else if ( m_bKillBlue && pOther->GetTeamNumber() == SDK_TEAM_BLUE )
	{
		return BaseClass::OnPlayerTouch( pOther );
	}

	return false;
}


#endif // MOD_SF194