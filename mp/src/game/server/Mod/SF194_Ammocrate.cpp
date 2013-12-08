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
 * \file SF194_AmmoCrate.cpp
 * \author Hekar Khani
 * \date Dec 08, 2009
 * Another re-referencing of an entity name for the sake
 * of map compatibility
 */

#include "cbase.h"
#ifdef MOD_SF194
#include "CAmmoCrate.h"
#include "CAmmoCrateClassic.h"

/**
 * If the SF132 map compatibility is enabled, it interferes
 * with the SF194 ammocrate :/
 * I could just forget the SF132 ammocrate and make
 * it act the same as the later crates
 *
 */
LINK_ENTITY_TO_CLASS( item_ammo_crate, CAmmoCrateClassic );

#endif // MOD_SF194
