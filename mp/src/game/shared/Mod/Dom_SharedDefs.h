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

#ifndef SHARED_DOM_H
#define SHARED_DOM_H

#ifdef WIN32
#pragma once
#endif // WIN32

#define DOM_NO_PARENT_ZONE -1

enum ParentZones_t
{
	DOM_PARENT_ZONE_ONE = 0,
	DOM_PARENT_ZONE_TWO,
	DOM_PARENT_ZONE_THREE,
	DOM_PARENT_ZONE_COUNT
};

enum CaptureTimerState_t
{
	CAP_NONE = 0,
	CAP_DRAIN_RED,
	CAP_DRAIN_BLUE,
	// Currently capturing the point
	CAP_CAPTURE_RED,
	CAP_CAPTURE_BLUE,
	CAP_STALLED_RED,
	CAP_STALLED_BLUE,
	CAP_OWNED_RED,
	CAP_OWNED_BLUE
};

#endif // SHARED_DOM_H