//=========== (C) Copyright 1999 Valve, L.L.C. All rights reserved. ===========
//
// The copyright to the contents herein is the property of Valve, L.L.C.
// The contents may be used and/or copied only with the written permission of
// Valve, L.L.C., or in accordance with the terms and conditions stipulated in
// the agreement/contract under which the contents have been supplied.
//=============================================================================

// No spaces in event names, max length 32
// All strings are case sensitive
//
// valid data key types are:
//   string : a zero terminated string
//   bool   : unsigned int, 1 bit
//   byte   : unsigned int, 8 bit
//   short  : signed int, 16 bit
//   long   : signed int, 32 bit
//   float  : float, 32 bit
//   local  : any data, but not networked to clients
//
// following key names are reserved:
//   local      : if set to 1, event is not networked to clients
//   unreliable : networked, but unreliable
//   suppress   : never fire this event
//   time	: firing server time
//   eventid	: holds the event ID

"modevents"
{
	"player_death"				// a game event, name may be 32 charaters long
	{
		"userid"	"short"   	// user ID who died				
		"attacker"	"short"	 	// user ID who killed
		"weapon"	"string" 	// weapon name killed used 
	}
	
	"teamplay_round_start"			// round restart
	{
		"full_reset"	"bool"		// is this a full reset of the map
	}
	
	"spec_target_updated"
	{
	}
	
	"achievement_earned"
	{
		"player"	"byte"		// entindex of the player
		"achievement"	"short"		// achievement ID
	}
	
	"player_hurt"
	{
		"userid"	"short"   	// user ID who was hurt			
		"attacker"	"short"	 	// user ID who attacked
		"weapon"	"string" 	// weapon name attacker used
	}
	
	"player_changeclass"
	{
		"userid"	"short"		// user ID who changed class
		"class"		"short"		// class that they changed to
	}

	"spec_target_updated"
	{
	}
    
    "phase_switch"
    {
        "phaseid"   short   // the ID of the new phase
    }
    
    "phase_time"
    {
        "newtime"   float   // new phase time in seconds
    }    
	
	"block_frozen"
	{
		"userid"	short  	// user ID who froze/unfroze the block
		"blockid"	short 	// block ID
	}

    "flag_request_drop"
	{
		"userid"	short  	// user ID who requested drop
		"teamid"	short 	// teamnumber of player
	}
    
    "flag_pickup"
	{
		"userid"	short  	// user ID who picked flag
		"teamid"	short 	// teamnumber of player
		"flagteamid"	short 	// teamnumber of player
	}
    
	"flag_dropped"
	{
		"userid"	short  	// user ID who dropped flag
		"teamid"	short 	// teamnumber of player
		"flagteamid"	short 	// teamnumber of player
	}
    
    "flag_returned"
	{
		"userid"	short  	// user who returned the flag
		"teamid"	short 	// teamnumber of player
		"flagteamid"	short 	// teamnumber of player
	}
}
