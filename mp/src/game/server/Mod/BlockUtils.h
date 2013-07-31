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

#ifndef BLOCK_UTILS_H
#define BLOCK_UTILS_H

class CBlockBase;

CBlockBase *FindBlock( CBasePlayer *player, int distance );
void GetPlayerTraceLine( trace_t& tr, CBasePlayer *player, int distance, int collisionGroup = COLLISION_GROUP_NONE );
Vector GetPlayerTraceLineEnd( CBasePlayer *player, int distance );
CBaseEntity *SpawnBlock( int blockType, int team, 
	const Vector& origin, const QAngle& angles, 
	CBaseEntity *parent );
bool DisposeBlock( CBaseEntity *ent );
void SnapVector( Vector& vector );

enum SnapAngleStyle
{
	// Straight like a wall
	SNAP_ANGLE_WALL = 0,
	// Flat on the ground
	SNAP_ANGLE_FLAT,
	// Diagonal like a ramp
	SNAP_ANGLE_RAMP,
	// Do not snap on anything
	SNAP_ANGLE_NONE
};

void SnapAngle( QAngle& angle, int style = SNAP_ANGLE_WALL );

#endif // BLOCK_UTILS_H