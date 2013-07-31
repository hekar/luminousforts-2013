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
#include "Mod/Dom_CaptureZone.h"
#include "Mod/Dom_LinearZone.h"

LINK_ENTITY_TO_CLASS( lfd_linear_zone, CDomLinearZone );

static CDomLinearZoneManager g_LinearZoneManager;
CDomLinearZoneManager *LinearZoneManager()
{
	return &g_LinearZoneManager;
}

void CDomLinearZoneManager::AddLinearZone( CDomLinearZone *linearZone )
{
	linearZones.Insert( linearZone->GetZoneId(), linearZone );
}

void CDomLinearZoneManager::RegisterLinks()
{
	for ( unsigned int i = 0; i < linearZones.Count(); i++ )
	{
		CDomLinearZone *zone = linearZones.Element( i );
		int zoneId = zone->GetZoneId();

		if ( zone != NULL )
		{
			// Link the parents
			for ( int i = 0; i < DOM_PARENT_ZONE_COUNT; i++ )
			{
				int parentIndex = zone->GetParentIndex( i );
				if ( parentIndex != DOM_NO_PARENT_ZONE )
				{
					int index = linearZones.Find( parentIndex );
					CDomLinearZone *linkZone = linearZones.Element( index );
					zone->LinkToParent( parentIndex, linkZone );
				}
			}
		}
	}
}

void CDomLinearZoneManager::Clear()
{
	linearZones.RemoveAll();
}

void CDomLinearZone::Spawn()
{
	for ( int i = 0; i < DOM_PARENT_ZONE_COUNT; i++ )
	{
		parents[ i ] = NULL;
	}
}

void CDomLinearZone::Think()
{
	bool foundRed = false;
	bool foundBlue = false;
	for ( int i = 0; i < DOM_PARENT_ZONE_COUNT; i++ )
	{
		CDomCaptureZone *linearZone = GetParent( i );
		if ( linearZone == NULL )
		{
			continue;
		}

		if ( linearZone->GetLocked() )
		{
			if ( linearZone->GetTeamNumber() == TEAM_RED )
			{
				m_Locked &= ~MOD_TEAM_RED;
				foundRed = true;
			}
			else if ( linearZone->GetTeamNumber() == TEAM_BLUE )
			{
				m_Locked &= ~MOD_TEAM_BLUE;
				foundBlue = true;
			}
		}
	}

	if ( !foundRed )
	{
		m_Locked |= MOD_TEAM_RED;
	}
	
	if ( !foundBlue )
	{
		m_Locked |= MOD_TEAM_BLUE;
	}

	BaseClass::Think();
}

void CDomLinearZone::LinkToParent( int parentIndex, CDomLinearZone *parent )
{
	assert( parentIndex >= 0 && parentIndex < DOM_PARENT_ZONE_COUNT );

	switch( parentIndex )
	{
	case DOM_PARENT_ZONE_ONE:
		m_ParentZone1 = parentIndex;
	case DOM_PARENT_ZONE_TWO:
		m_ParentZone2 = parentIndex;	
	case DOM_PARENT_ZONE_THREE:
		m_ParentZone3 = parentIndex;
	default:
		assert( false );
	}

	parents[ parentIndex ] = parent;
}

void CDomLinearZone::ClearLinks()
{
	m_ParentZone1 = DOM_NO_PARENT_ZONE;
	m_ParentZone2 = DOM_NO_PARENT_ZONE;
	m_ParentZone3 = DOM_NO_PARENT_ZONE;

	for ( int i = 0; i < DOM_PARENT_ZONE_COUNT; i++ )
	{
		parents[ i ] = NULL;
	}
}

int CDomLinearZone::GetParentIndex( int parentIndex )
{
	switch( parentIndex )
	{
	case DOM_PARENT_ZONE_ONE:
		return m_ParentZone1;
	case DOM_PARENT_ZONE_TWO:
		return m_ParentZone2;	
	case DOM_PARENT_ZONE_THREE:
		return m_ParentZone3;
	default:
		assert( false );
	}

	return DOM_NO_PARENT_ZONE;
}

CDomCaptureZone *CDomLinearZone::GetParent( int parentIndex )
{
	assert( parentIndex >= 0 && parentIndex < DOM_PARENT_ZONE_COUNT );
	return parents[ parentIndex ];
}

class CLinearZoneSystem : public CAutoGameSystem
{
public:
	CLinearZoneSystem() :
	  CAutoGameSystem( "LinearZoneSystem" )
	{
	}

	void LevelInitPostEntity()
	{
		LinearZoneManager()->Clear();

		const char *classname = "lfd_linear_zone";
		CBaseEntity *current = gEntList.FindEntityByClassname( gEntList.FirstEnt(), classname );
		while ( current != NULL )
		{
			CDomLinearZone *linearZone = dynamic_cast< CDomLinearZone * > ( current );
			if ( linearZone != NULL )
			{
				LinearZoneManager()->AddLinearZone( linearZone );
				current = gEntList.FindEntityByClassname( current, classname );
			}
		}

		LinearZoneManager()->RegisterLinks();
	}
};

static CLinearZoneSystem g_LinearZoneHelper;
