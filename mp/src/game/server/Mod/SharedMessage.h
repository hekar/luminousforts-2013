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


#ifndef SHARED_MESSAGE_H
#define SHARED_MESSAGE_H

// Look at client/Mod/Hud_Message.cpp for more info
enum MessageLog_enum
{
	// Invalid is 0 on purpose
	MESS_INVALID = 0, // doesn't take any arguments
	MESS_FROZEN, // Takes player index
	MESS_UNFROZEN, // Takes player index
	MESS_PHASE // Takes PhaseID index
};

#endif // SHARED_MESSAGE_H