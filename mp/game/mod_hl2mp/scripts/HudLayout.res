"Resource/HudLayout.res"
{	
	overview
	{
		"fieldname"				"overview"
		"visible"				"1"
		"enabled"				"1"
		"xpos"					"0"
		"ypos"					"480"
		"wide"					"0"
		"tall"					"0"
	}
	
	TargetID
	{
		"fieldName" "TargetID"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HudSuit
	{
		"fieldName"		"HudSuit"
		"xpos"	"140"
		"ypos"	"432"
		"wide"	"108"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"0"

		"text_xpos" "8"
		"text_ypos" "20"
		"digit_xpos" "50"
		"digit_ypos" "2"
	}

	HudAmmo
	{
		"fieldName" "HudAmmo"
		"xpos"	"r150"
		"ypos"	"432"
		"wide"	"136"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"0"

		"text_xpos" "8"
		"text_ypos" "20"
		"digit_xpos" "44"
		"digit_ypos" "2"
		"digit2_xpos" "98"
		"digit2_ypos" "16"
	}

	HudAmmoSecondary
	{
		"fieldName" "HudAmmoSecondary"
		"xpos"	"r76"
		"ypos"	"432"
		"wide"	"60"
		"tall"  "36"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"	"0"

		"text_xpos" "8"
		"text_ypos" "22"
		"digit_xpos" "36"
		"digit_ypos" "2"
	}
	
	HudPosture
	{
		"fieldName" 		"HudPosture"
		"visible" 		"1"
		"PaintBackgroundType"	"2"
		"xpos"	"16"
		"ypos"	"450"
		"tall"  "36"
		"wide"	"36"
		"font"	"WeaponIconsSmall"
		"icon_xpos"	"10"
		"icon_ypos" 	"0"
	}
	
	HudFlashlight
	{
		"fieldName" "HudFlashlight"
		"visible" "1"
		"PaintBackgroundType"	"2"
		"xpos"	"270"		[$WIN32]
		"ypos"	"444"		[$WIN32]
		"xpos_hidef"	"293"		[$X360]		// aligned to left
		"xpos_lodef"	"c-18"		[$X360]		// centered in screen
		"ypos"	"428"		[$X360]				
		"tall"  "24"
		"wide"	"36"
		"font"	"WeaponIconsSmall"
		
		"icon_xpos"	"4"
		"icon_ypos" "-8"
		
		"BarInsetX" "4"
		"BarInsetY" "18"
		"BarWidth" "28"
		"BarHeight" "2"
		"BarChunkWidth" "2"
		"BarChunkGap" "1"
	}
	
	HudVoiceSelfStatus
	{
		"fieldName" "HudVoiceSelfStatus"
		"visible" "1"
		"enabled" "1"
		"xpos" "r43"
		"ypos" "355"
		"wide" "24"
		"tall" "24"
	}

	HudVoiceStatus
	{
		"fieldName" "HudVoiceStatus"
		"visible" "1"
		"enabled" "1"
		"xpos" "r200"
		"ypos" "0"
		"wide" "100"
		"tall" "400"

		"item_tall"	"24"
		"item_wide"	"100"

		"item_spacing" "2"

		"icon_ypos"	"0"
		"icon_xpos"	"0"
		"icon_tall"	"24"
		"icon_wide"	"24"

		"text_xpos"	"26"

		"PaintBackgroundType" "0"
	}
	
	HudDamageIndicator
	{
		"fieldName" "HudDamageIndicator"
		"visible" "1"
		"enabled" "1"
		"DmgColorLeft" "255 0 0 0"
		"DmgColorRight" "255 0 0 0"
		
		"dmg_xpos" "30"
		"dmg_ypos" "100"
		"dmg_wide" "36"
		"dmg_tall1" "240"
		"dmg_tall2" "200"
	}

	HudZoom
	{
		"fieldName" "HudZoom"
		"visible" "1"
		"enabled" "1"
		"Circle1Radius" "66"
		"Circle2Radius"	"74"
		"DashGap"	"16"
		"DashHeight" "4"	[$WIN32]
		"DashHeight" "6"	[$X360]		
		"BorderThickness" "88"
	}
	
	HudWeaponSelection
	{
		"fieldName" "HudWeaponSelection"
		"ypos" 	"16"	[$WIN32]
		"ypos" 	"32"	[$X360]
		"visible" "1"
		"enabled" "1"
		"SmallBoxSize" "32"
		"MediumBoxWide"	"95"
		"MediumBoxWide_hidef"	"78"
		"MediumBoxTall"	"50"
		"MediumBoxTall_hidef"	"50"
		"MediumBoxWide_lodef"	"74"
		"MediumBoxTall_lodef"	"50"
		"LargeBoxWide" "112"
		"LargeBoxTall" "80"
		"BoxGap" "8"
		"SelectionNumberXPos" "4"
		"SelectionNumberYPos" "4"
		"SelectionGrowTime"	"0.4"
		"TextYPos" "64"
	}

	HudCrosshair
	{
		"fieldName" "HudCrosshair"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HudDeathNotice
	{
		"fieldName" "HudDeathNotice"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "400"
	}

	HudVehicle
	{
		"fieldName" "HudVehicle"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	ScorePanel
	{
		"fieldName" "ScorePanel"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HudTrain
	{
		"fieldName" "HudTrain"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HudMOTD
	{
		"fieldName" "HudMOTD"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HudMessage
	{
		"fieldName" "HudMessage"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}
	
	HudGameMessage
	{
		"fieldName" "HudGameMessage"
		"visible" "0"
		"enabled" "0"
		"xpos"	 "10"
		"ypos"	 "152"
		"wide"	 "f0"
		"tall"	 "256"
	}
	HudMenu
	{
		"fieldName" "HudMenu"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HudCloseCaption
	{
		"fieldName" "HudCloseCaption"
		"visible"	"1"
		"enabled"	"1"
		"xpos"		"c-250"
		"ypos"		"276"	[$WIN32]
		"ypos"		"236"	[$X360]
		"wide"		"500"
		"tall"		"136"	[$WIN32]
		"tall"		"176"	[$X360]

		"BgAlpha"	"128"

		"GrowTime"		"0.25"
		"ItemHiddenTime"	"0.2"  // Nearly same as grow time so that the item doesn't start to show until growth is finished
		"ItemFadeInTime"	"0.15"	// Once ItemHiddenTime is finished, takes this much longer to fade in
		"ItemFadeOutTime"	"0.3"
		"topoffset"		"0"		[$WIN32]
		"topoffset"		"0"	[$X360]
	}

	HudChat
	{
		"fieldName" "HudChat"
		"visible" "1"
		"enabled" "1"
		"xpos"	"10"
		"ypos"	"300"
		"wide"	 "400"
		"tall"	 "100"
	}

	HudGeiger
	{
		"fieldName" "HudGeiger"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HUDQuickInfo
	{
		"fieldName" "HUDQuickInfo"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HudWeapon
	{
		"fieldName" "HudWeapon"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}
	HudAnimationInfo
	{
		"fieldName" "HudAnimationInfo"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HudPredictionDump
	{
		"fieldName" "HudPredictionDump"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
	}

	HudHintDisplay
	{
		"fieldName"				"HudHintDisplay"
		"visible"				"0"
		"enabled"				"1"
		"xpos"					"c-240"
		"ypos"					"c60"
		"xpos"	"r148"	[$X360]
		"ypos"	"r338"	[$X360]
		"wide"					"480"
		"tall"					"100"
		"HintSize"				"1"
		"text_xpos"				"8"
		"text_ypos"				"8"
		"center_x"				"0"	// center text horizontally
		"center_y"				"-1"	// align text on the bottom
		"paintbackground"		"0"
	}	

	HudHintKeyDisplay
	{
		"fieldName"	"HudHintKeyDisplay"
		"visible"	"0"
		"enabled" 	"1"
		"xpos"		"r120"	[$WIN32]
		"ypos"		"r340"	[$WIN32]
		"xpos"		"r148"	[$X360]
		"ypos"		"r338"	[$X360]
		"wide"		"100"
		"tall"		"200"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 170 0 220"

		"PaintBackgroundType"	"2"
	}

	HudSquadStatus
	{
		"fieldName"	"HudSquadStatus"
		"visible"	"1"
		"enabled" "1"
		"xpos"	"r120"
		"ypos"	"380"
		"wide"	"104"
		"tall"	"46"
		"text_xpos"	"8"
		"text_ypos"	"34"
		"SquadIconColor"	"255 220 0 160"
		"IconInsetX"	"8"
		"IconInsetY"	"0"
		"IconGap"		"24"

		"PaintBackgroundType"	"2"
	}

	HudPoisonDamageIndicator
	{
		"fieldName"	"HudPoisonDamageIndicator"
		"visible"	"0"
		"enabled" "1"
		"xpos"	"16"
		"ypos"	"346"
		"wide"	"136"
		"tall"	"38"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_ygap" "14"
		"TextColor"	"255 170 0 220"
		"PaintBackgroundType"	"2"
	}
	HudCredits
	{
		"fieldName"	"HudCredits"
		"TextFont"	"Default"
		"visible"	"1"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	"f0"
		"tall"	"480"
		"TextColor"	"255 255 255 192"

	}
	
	HUDAutoAim
	{
		"fieldName" "HUDAutoAim"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"	[$WIN32]
		"tall"	 "480"	[$WIN32]
		"wide"	 "960"	[$X360]
		"tall"	 "720"	[$X360]
	}
	HudCommentary
	{
		"fieldName" "HudCommentary"
		"xpos"	"c-190"
		"ypos"	"350"
		"wide"	"380"
		"tall"  "40"
		"visible" "1"
		"enabled" "1"
		
		"PaintBackgroundType"	"2"
		
		"bar_xpos"		"50"
		"bar_ypos"		"20"
		"bar_height"	"8"
		"bar_width"		"320"
		"speaker_xpos"	"50"
		"speaker_ypos"	"8"
		"count_xpos_from_right"	"10"	// Counts from the right side
		"count_ypos"	"8"
		
		"icon_texture"	"vgui/hud/icon_commentary"
		"icon_xpos"		"0"
		"icon_ypos"		"0"		
		"icon_width"	"40"
		"icon_height"	"40"
	}
	
	HudHDRDemo
	{
		"fieldName" "HudHDRDemo"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	"f0"
		"tall"  "480"
		"visible" "1"
		"enabled" "1"
		
		"Alpha"	"255"
		"PaintBackgroundType"	"2"
		
		"BorderColor"	"0 0 0 255"
		"BorderLeft"	"16"
		"BorderRight"	"16"
		"BorderTop"		"16"
		"BorderBottom"	"64"
		"BorderCenter"	"0"
		
		"TextColor"		"255 255 255 255"
		"LeftTitleY"	"422"
		"RightTitleY"	"422"
	}

	AchievementNotificationPanel	
	{
		"fieldName"				"AchievementNotificationPanel"
		"visible"				"1"
		"enabled"				"1"
		"xpos"					"0"
		"ypos"					"180"
		"wide"					"f10"	[$WIN32]
		"wide"					"f60"	[$X360]
		"tall"					"100"
	}
    
    HudFlagCarrier
    {
 		"fieldName" "HudFlagCarrier"
		"xpos"	"16"
		"ypos"	"22"
		"wide"	"300"
		"tall"  "80"
		"visible" "1"
		"enabled" "1"
		
        "BlueTextColor"     "0 128 220 255"
        "BlueBackgroundColor"	"0 0 0 0"
		
        "BlueAvatarX"        "10"
        "BlueAvatarY"        "43"
        "BlueTextX"          "40"
        "BlueTextY"          "47"
		"BlueRectX"			"2"
		"BlueRectY"			"40"
		"BlueRectWidth"		"142"
		"BlueRectHeight"		"26"		
				
        "RedTextColor"     "128 32 16 255"
        "RedBackgroundColor"	"0 0 0 0"
		
        "RedAvatarX"       "10"
        "RedAvatarY"       "5"
        "RedTextX"         "40"
        "RedTextY"         "9"
		"RedRectX"			"2"
		"RedRectY"			"2"
		"RedRectWidth"		"142"
		"RedRectHeight"	"26"		
		
		"PaintBackgroundType"	"2"
    } 
    
    HudDomScore
    {
 		"fieldName" "HudDomScore"
		"xpos"	"16"
		"ypos"	"22"
		"wide"	"300"
		"tall"  "80"
		"visible" "1"
		"enabled" "1"
		
        "BlueTextColor"     "0 128 220 255"
        "BlueBackgroundColor"	"0 0 0 0"
		
        "BlueTextX"          "20"
        "BlueTextY"          "47"
		"BlueRectX"			"2"
		"BlueRectY"			"40"
		"BlueRectWidth"		"142"
		"BlueRectHeight"		"26"		
				
        "RedTextColor"     "128 32 16 255"
        "RedBackgroundColor"	"0 0 0 0"
		
        "RedTextX"         "20"
        "RedTextY"         "9"
		"RedRectX"			"2"
		"RedRectY"			"2"
		"RedRectWidth"		"142"
		"RedRectHeight"	"26"
		
		"PaintBackgroundType"	"2"
    }
    
    HudPhaseTime
    {
 		"fieldName" "HudPhaseTime"
		"wide"	"f0"
		"tall"  "60"
		"visible" "1"
		"enabled" "1"
		
        "RightJustify"  "1"
        
        "TimerX"        "10"
        "TimerY"        "11"
        "BlockPointsX"  "60"
        "BlockPointsY"  "11"
        "PhaseIconX"    "160"
        "PhaseIconY"    "11"

		"TimeColorBlue"		"0 128 220 255"
		"TimeColorRed"		"128 32 16 255"

		"BlockColorBlue"	"0 128 220 255"
		"BlockColorRed"		"128 32 16 255"

		"PhaseColorBlue"	"0 128 220 255"
		"PhaseColorRed"		"128 32 16 255"
		
		"BorderColorBlue"	"0 128 220 128"
		"BorderColorRed"	"128 32 16 144"
        
        "BackgroundColorBlue"    "0 0 0 160"
        "BackgroundColorRed"    "0 0 0 160"
                
		"BackgroundRectX"		"r230"
		"BackgroundRectY"		"10"
		"BackgroundRectWidth"	"200"
		"BackgroundRectHeight"	"32"
		
		"PaintBackgroundType"	"2"
    }
	
	HudBlockHealth
	{
		"fieldName" "HudBlockHealth"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"
        
        "CenterBesideCrosshair"    "1"
        
        "BackgroundColorBlue"    "0 0 0 200"
        "BackgroundColorRed"    "0 0 0 200"
        "BorderColorBlue"    "0 0 0 200"
        "BorderColorRed"    "0 0 0 200"        
		
		"BackgroundRectX"		"r230"
		"BackgroundRectY"		"10"
		"BackgroundRectWidth"	"170"
		"BackgroundRectHeight"	"32"        
	}
	
	HudPlayerHealth
	{
		"fieldName" "HudPlayerHealth"
		"visible" "1"
		"enabled" "1"
		"xpos"	"8"
		"ypos"	"r32"
		"wide"	"244"
		"tall"	"24"
		
        "BarColorBlue" "0 128 220 128"
        "BarColorRed" "128 32 16 144"
        
		"BackgroundAlpha" "230"
		
		"BarInsetX" "4"
		"BarInsetY" "4"
		"BarWidth" "240"
		"BarHeight" "17"
		"BarChunkWidth" "9"
		"BarChunkGap" "0"

		"TextX" "8"
		"TextY" "8"

		"PaintBackgroundType"	"0"
	}
	
	HudSuitPower
	{
		"fieldName" "HudSuitPower"
		"visible" "1"
		"enabled" "1"
		"xpos"	"8"
		"ypos"	"r44"
		"wide"	"144"
		"tall"	"12"
		
		"ShowText" "0"
		
        "StaminaColorBlue" "0 128 220 128"
        "StaminaColorRed" "128 32 16 144"
            
		"BarInsetX" "4"
		"BarInsetY" "4"
		"BarWidth" "140"
		"BarHeight" "8"
		"BarChunkWidth" "9"
		"BarChunkGap" "0"

		"TextX" "8"
		"TextY" "7"

		"PaintBackgroundType"	"0"
	}	
	
	HudFreeze
	{
		"fieldName" "HudFreeze"
		"visible" "1"
		"enabled" "1"
		"xpos"	"269"
		"ypos"	"227"
		"wide"	"102"
		"tall"	"26"
		
		"Background" "255 0 0 220"
        "ProgressBackground" "255 0 0 220"
		"ProgressForeground" "0 0 255 220"

		"BarInsetX" "8"
		"BarInsetY" "15"
		"BarWidth" "92"
		"BarHeight" "4"
		"BarChunkWidth" "9"
		"BarChunkGap" "0"

		"PaintBackgroundType"	"0"
	}
	
	HudFlagLocations
    {
		"fieldName"      "HudFlagLocations"
		"xpos"   "0"
		"ypos"   "0"
		"wide"   "640"
		"tall"  "480"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"   "0"
    }
	
	HudFlagCompasses
    {
		"fieldName"      "HudFlagCompasses"
		"xpos"   "0"
		"ypos"   "0"
		"wide"   "640"
		"tall"  "480"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"   "0"
    }
    
	HudFreezeStatus
	{
		"fieldName" "HudFreezeStatus"
		"visible" "1"
		"enabled" "1"
		"wide"	 "f0"
		"tall"	 "480"

        // offset from background.
        "FrozenTextX" "8"
        "FrozenTextY" "8"        
        
        // again; offset from background.
        "UnFrozenTextX" "8"
        "UnFrozenTextY" "28"
                
        "BackgroundColorBlue"    "0 0 0 0"
        "BackgroundColorRed"    "0 0 0 0"
		
        "BorderColorBlue"    "0 0 0 0"
        "BorderColorRed"    "0 0 0 0"
                
		"BackgroundRectX_BuildPhase"     "10"
		"BackgroundRectY_BuildPhase"	 "10"
		"BackgroundRectWidth_BuildPhase" "170"
		"BackgroundRectHeight_BuildPhase" "54"
        
		"BackgroundRectX_CombatPhase"     "10"
		"BackgroundRectY_CombatPhase"	 "120"
		"BackgroundRectWidth_CombatPhase" "170"
		"BackgroundRectHeight_CombatPhase" "54"
	}
	
	HudFreezeProgress
	{
		"fieldName"	"HudFreezeProgress"
		"visible"	"0"
		"enabled"	"1"
		"xpos"		"15"
		"ypos"		"r200"
		"wide"		"122"
		"tall"		"44"
		
		"RedBarForeground" 	"128 32 16 214"
		"BlueBarForeground" "0 128 220 228"
		"BarBackground" "0 0 0 192"
		"Background"	"0 0 0 128"
		
		"RedBorder"		"128 32 16 214"
		"BlueBorder"	"0 128 220 228"
		
		"X"			"0"
		"Y"			"0"
		"Width"		"120"
		"Height"	"70"

		"BarX"		"14"
		"BarY"		"14"
		"BarWidth"	"94"
		"BarHeight"	"16"
	}
	
	// HudPlayerMarkers
	// {
		// "fieldName"      "HudPlayerMarkers"
		// "xpos"   "0"
		// "ypos"   "0"
		// "wide"   "640"
		// "tall"  "480"
		// "visible" "1"
		// "enabled" "1"

		// "PaintBackgroundType"   "0"
	// }
	
	HudCapPoints
	{
		"fieldName"		"HudCapPoints"
		"xpos"	"264"
		"ypos"	"r36"
		"wide"	"350"
		"tall"	"28"
		"visible" "1"
		"enabled" "1"
		
		"PaintBackgroundType" "0"
		
		"BorderColorBlue"	"0 128 220 255"
		"BorderColorRed"	"128 32 16 255"
		"BorderColorSpec"	"128 128 128 255"
		
		"ProgressColorBlue"	"0 128 220 192"
		"ProgressColorRed"	"128 32 16 192"
		"ProgressColorSpec"	"128 128 128 32"
		
		"GapWidth"	"12"
		"BarWidth"	"28"
		"BarHeight"	"28"
	}
	
	HudCapPos
	{
		"fieldName"      "HudCapPos"
		"xpos"   "0"
		"ypos"   "0"
		"wide"   "640"
		"tall"  "480"
		"visible" "1"
		"enabled" "1"

		"PaintBackgroundType"   "0"
	}
	
}