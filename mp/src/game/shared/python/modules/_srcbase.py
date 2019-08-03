from srcpy.module_generators import SharedModuleGenerator
from pyplusplus import code_creators


class SrcBase(SharedModuleGenerator):
    module_name = '_srcbase'

    required_files = [
        '$SRCDIR\game\shared\python\srcpy_base.cpp',
        '$SRCDIR\game\shared\python\srcpy_base.h',
    ]
    
    files = [
        'cbase.h',
        'tier0/dbg.h',
        
        'srcpy_base.h',
        'igamemovement.h',
        'in_buttons.h',
    ]
    
    def Parse(self, mb):
        # Exclude everything by default
        mb.decls().exclude()      
                           
        # RBTree
        cls = mb.class_('PyUtlRBTree')
        cls.include()
        cls.rename('UtlRBTree')
        cls.member_operators('=').exclude() # Breaks debug mode and don't really need it
        
        # CUtlFlags
        mb.class_('PyUtlFlags').include()
        cls = mb.class_('PyUtlFlags')
        cls.rename('UtlFlags')
        
        # //--------------------------------------------------------------------------------------------------------------------------------
        mb.add_registration_code( "bp::scope().attr( \"MAX_PLAYERS\" ) = MAX_PLAYERS;" )
        mb.add_registration_code( "bp::scope().attr( \"TEAM_INVALID\" ) = TEAM_INVALID;" )
        mb.add_registration_code( "bp::scope().attr( \"TEAM_UNASSIGNED\" ) = TEAM_UNASSIGNED;" )
        mb.add_registration_code( "bp::scope().attr( \"TEAM_SPECTATOR\" ) = TEAM_SPECTATOR;" )
        mb.add_registration_code( "bp::scope().attr( \"FIRST_GAME_TEAM\" ) = FIRST_GAME_TEAM;" )

        # MASKS
        mb.add_registration_code( "bp::scope().attr( \"MASK_ALL\" ) = MASK_ALL;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_SOLID\" ) = MASK_SOLID;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_PLAYERSOLID\" ) = MASK_PLAYERSOLID;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_NPCSOLID\" ) = MASK_NPCSOLID;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_WATER\" ) = MASK_WATER;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_OPAQUE\" ) = MASK_OPAQUE;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_OPAQUE_AND_NPCS\" ) = MASK_OPAQUE_AND_NPCS;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_BLOCKLOS\" ) = MASK_BLOCKLOS;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_BLOCKLOS_AND_NPCS\" ) = MASK_BLOCKLOS_AND_NPCS;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_VISIBLE\" ) = MASK_VISIBLE;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_VISIBLE_AND_NPCS\" ) = MASK_VISIBLE_AND_NPCS;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_SHOT\" ) = MASK_SHOT;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_SHOT_HULL\" ) = MASK_SHOT_HULL;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_SHOT_PORTAL\" ) = MASK_SHOT_PORTAL;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_SOLID_BRUSHONLY\" ) = MASK_SOLID_BRUSHONLY;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_PLAYERSOLID_BRUSHONLY\" ) = MASK_PLAYERSOLID_BRUSHONLY;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_NPCSOLID_BRUSHONLY\" ) = MASK_NPCSOLID_BRUSHONLY;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_NPCWORLDSTATIC\" ) = MASK_NPCWORLDSTATIC;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_SPLITAREAPORTAL\" ) = MASK_SPLITAREAPORTAL;" )
        mb.add_registration_code( "bp::scope().attr( \"MASK_CURRENT\" ) = MASK_CURRENT;" )
        
        # Flags
        mb.add_registration_code( "bp::scope().attr( \"FL_ONGROUND\" ) = FL_ONGROUND;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_DUCKING\" ) = FL_DUCKING;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_WATERJUMP\" ) = FL_WATERJUMP;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_ONTRAIN\" ) = FL_ONTRAIN;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_INRAIN\" ) = FL_INRAIN;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_FROZEN\" ) = FL_FROZEN;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_ATCONTROLS\" ) = FL_ATCONTROLS;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_CLIENT\" ) = FL_CLIENT;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_FAKECLIENT\" ) = FL_FAKECLIENT;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_INWATER\" ) = FL_INWATER;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_FLY\" ) = FL_FLY;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_SWIM\" ) = FL_SWIM;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_CONVEYOR\" ) = FL_CONVEYOR;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_NPC\" ) = FL_NPC;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_GODMODE\" ) = FL_GODMODE;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_NOTARGET\" ) = FL_NOTARGET;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_AIMTARGET\" ) = FL_AIMTARGET;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_PARTIALGROUND\" ) = FL_PARTIALGROUND;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_STATICPROP\" ) = FL_STATICPROP;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_GRAPHED\" ) = FL_GRAPHED;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_GRENADE\" ) = FL_GRENADE;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_STEPMOVEMENT\" ) = FL_STEPMOVEMENT;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_DONTTOUCH\" ) = FL_DONTTOUCH;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_BASEVELOCITY\" ) = FL_BASEVELOCITY;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_WORLDBRUSH\" ) = FL_WORLDBRUSH;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_OBJECT\" ) = FL_OBJECT;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_KILLME\" ) = FL_KILLME;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_ONFIRE\" ) = FL_ONFIRE;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_DISSOLVING\" ) = FL_DISSOLVING;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_TRANSRAGDOLL\" ) = FL_TRANSRAGDOLL;" )
        mb.add_registration_code( "bp::scope().attr( \"FL_UNBLOCKABLE_BY_PLAYER\" ) = FL_UNBLOCKABLE_BY_PLAYER;" )
        
        # Life states
        mb.add_registration_code( "bp::scope().attr( \"LIFE_ALIVE\" ) = LIFE_ALIVE;" )
        mb.add_registration_code( "bp::scope().attr( \"LIFE_DYING\" ) = LIFE_DYING;" )
        mb.add_registration_code( "bp::scope().attr( \"LIFE_DEAD\" ) = LIFE_DEAD;" )
        mb.add_registration_code( "bp::scope().attr( \"LIFE_RESPAWNABLE\" ) = LIFE_RESPAWNABLE;" )
        mb.add_registration_code( "bp::scope().attr( \"LIFE_DISCARDBODY\" ) = LIFE_DISCARDBODY;" )
        
        # Damage
        mb.add_registration_code( "bp::scope().attr( \"DAMAGE_NO\" ) = DAMAGE_NO;" )
        mb.add_registration_code( "bp::scope().attr( \"DAMAGE_EVENTS_ONLY\" ) = DAMAGE_EVENTS_ONLY;" )
        mb.add_registration_code( "bp::scope().attr( \"DAMAGE_YES\" ) = DAMAGE_YES;" )
        mb.add_registration_code( "bp::scope().attr( \"DAMAGE_AIM\" ) = DAMAGE_AIM;" )
        
        # Damage types
        mb.add_registration_code( "bp::scope().attr( \"DMG_GENERIC\" ) = DMG_GENERIC;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_CRUSH\" ) = DMG_CRUSH;" )
        
        mb.add_registration_code( "bp::scope().attr( \"DMG_BULLET\" ) = DMG_BULLET;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_SLASH\" ) = DMG_SLASH;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_BURN\" ) = DMG_BURN;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_VEHICLE\" ) = DMG_VEHICLE;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_FALL\" ) = DMG_FALL;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_BLAST\" ) = DMG_BLAST;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_CLUB\" ) = DMG_CLUB;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_SHOCK\" ) = DMG_SHOCK;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_SONIC\" ) = DMG_SONIC;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_ENERGYBEAM\" ) = DMG_ENERGYBEAM;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_PREVENT_PHYSICS_FORCE\" ) = DMG_PREVENT_PHYSICS_FORCE;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_NEVERGIB\" ) = DMG_NEVERGIB;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_ALWAYSGIB\" ) = DMG_ALWAYSGIB;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_DROWN\" ) = DMG_DROWN;" )
        
        mb.add_registration_code( "bp::scope().attr( \"DMG_PARALYZE\" ) = DMG_PARALYZE;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_NERVEGAS\" ) = DMG_NERVEGAS;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_POISON\" ) = DMG_POISON;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_RADIATION\" ) = DMG_RADIATION;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_DROWNRECOVER\" ) = DMG_DROWNRECOVER;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_ACID\" ) = DMG_ACID;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_SLOWBURN\" ) = DMG_SLOWBURN;" )
        
        mb.add_registration_code( "bp::scope().attr( \"DMG_REMOVENORAGDOLL\" ) = DMG_REMOVENORAGDOLL;" )
        
        mb.add_registration_code( "bp::scope().attr( \"DMG_PHYSGUN\" ) = DMG_PHYSGUN;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_PLASMA\" ) = DMG_PLASMA;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_AIRBOAT\" ) = DMG_AIRBOAT;" )
        
        mb.add_registration_code( "bp::scope().attr( \"DMG_DISSOLVE\" ) = DMG_DISSOLVE;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_BLAST_SURFACE\" ) = DMG_BLAST_SURFACE;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_DIRECT\" ) = DMG_DIRECT;" )
        mb.add_registration_code( "bp::scope().attr( \"DMG_BUCKSHOT\" ) = DMG_BUCKSHOT;" )
        
        mb.add_registration_code( "bp::scope().attr( \"DMG_LASTGENERICFLAG\" ) = DMG_LASTGENERICFLAG;" )
        
        # Effects
        mb.add_registration_code( "bp::scope().attr( \"EF_BONEMERGE\" ) = (int)EF_BONEMERGE;" )
        mb.add_registration_code( "bp::scope().attr( \"EF_BRIGHTLIGHT\" ) = (int)EF_BRIGHTLIGHT;" )
        mb.add_registration_code( "bp::scope().attr( \"EF_DIMLIGHT\" ) = (int)EF_DIMLIGHT;" )
        mb.add_registration_code( "bp::scope().attr( \"EF_NOINTERP\" ) = (int)EF_NOINTERP;" )
        mb.add_registration_code( "bp::scope().attr( \"EF_NOSHADOW\" ) = (int)EF_NOSHADOW;" )
        mb.add_registration_code( "bp::scope().attr( \"EF_NODRAW\" ) = (int)EF_NODRAW;" )
        mb.add_registration_code( "bp::scope().attr( \"EF_NORECEIVESHADOW\" ) = (int)EF_NORECEIVESHADOW;" )
        mb.add_registration_code( "bp::scope().attr( \"EF_BONEMERGE_FASTCULL\" ) = (int)EF_BONEMERGE_FASTCULL;" )
        mb.add_registration_code( "bp::scope().attr( \"EF_ITEM_BLINK\" ) = (int)EF_ITEM_BLINK;" )
        mb.add_registration_code( "bp::scope().attr( \"EF_PARENT_ANIMATES\" ) = (int)EF_PARENT_ANIMATES;" )
        mb.add_registration_code( "bp::scope().attr( \"EF_MAX_BITS\" ) = (int)EF_MAX_BITS;" )
        
        # Misc
        mb.add_registration_code( "bp::scope().attr( \"COORD_INTEGER_BITS\" ) = (int)COORD_INTEGER_BITS;" )
        mb.add_registration_code( "bp::scope().attr( \"COORD_FRACTIONAL_BITS\" ) = (int)COORD_FRACTIONAL_BITS;" )
        mb.add_registration_code( "bp::scope().attr( \"COORD_DENOMINATOR\" ) = (int)COORD_DENOMINATOR;" )
        mb.add_registration_code( "bp::scope().attr( \"COORD_RESOLUTION\" ) = (int)COORD_RESOLUTION;" )
        
        mb.add_registration_code( "bp::scope().attr( \"COORD_INTEGER_BITS_MP\" ) = (int)COORD_INTEGER_BITS_MP;" )
        mb.add_registration_code( "bp::scope().attr( \"COORD_FRACTIONAL_BITS_MP_LOWPRECISION\" ) = (int)COORD_FRACTIONAL_BITS_MP_LOWPRECISION;" )
        mb.add_registration_code( "bp::scope().attr( \"COORD_DENOMINATOR_LOWPRECISION\" ) = (int)COORD_DENOMINATOR_LOWPRECISION;" )
        mb.add_registration_code( "bp::scope().attr( \"COORD_RESOLUTION_LOWPRECISION\" ) = (int)COORD_RESOLUTION_LOWPRECISION;" )
        
        mb.add_registration_code( "bp::scope().attr( \"NORMAL_FRACTIONAL_BITS\" ) = (int)NORMAL_FRACTIONAL_BITS;" )
        mb.add_registration_code( "bp::scope().attr( \"NORMAL_DENOMINATOR\" ) = (int)NORMAL_DENOMINATOR;" )
        mb.add_registration_code( "bp::scope().attr( \"NORMAL_RESOLUTION\" ) = (int)NORMAL_RESOLUTION;" )
        
        mb.add_registration_code( "bp::scope().attr( \"DIST_EPSILON\" ) = (int)DIST_EPSILON;" )
        
        mb.add_registration_code( "bp::scope().attr( \"MAX_COORD_INTEGER\" ) = (int)MAX_COORD_INTEGER;" )
        mb.add_registration_code( "bp::scope().attr( \"MIN_COORD_INTEGER\" ) = (int)MIN_COORD_INTEGER;" )
        mb.add_registration_code( "bp::scope().attr( \"MAX_COORD_FRACTION\" ) = (float)MAX_COORD_FRACTION;" )
        mb.add_registration_code( "bp::scope().attr( \"MIN_COORD_FRACTION\" ) = (float)MIN_COORD_FRACTION;" )
        mb.add_registration_code( "bp::scope().attr( \"MAX_COORD_FLOAT\" ) = (float)MAX_COORD_FLOAT;" )
        mb.add_registration_code( "bp::scope().attr( \"MIN_COORD_FLOAT\" ) = (float)MIN_COORD_FLOAT;" )
        mb.add_registration_code( "bp::scope().attr( \"COORD_EXTENT\" ) = (float)COORD_EXTENT;" )
        mb.add_registration_code( "bp::scope().attr( \"MAX_TRACE_LENGTH\" ) = (float)MAX_TRACE_LENGTH;" )
        mb.add_registration_code( "bp::scope().attr( \"MAX_COORD_RANGE\" ) = (float)MAX_COORD_RANGE;" )

        # Buttons
        mb.add_registration_code( "bp::scope().attr( \"IN_ATTACK\" ) = (int)IN_ATTACK;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_JUMP\" ) = (int)IN_JUMP;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_DUCK\" ) = (int)IN_DUCK;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_FORWARD\" ) = (int)IN_FORWARD;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_BACK\" ) = (int)IN_BACK;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_USE\" ) = (int)IN_USE;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_CANCEL\" ) = (int)IN_CANCEL;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_LEFT\" ) = (int)IN_LEFT;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_RIGHT\" ) = (int)IN_RIGHT;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_MOVELEFT\" ) = (int)IN_MOVELEFT;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_MOVERIGHT\" ) = (int)IN_MOVERIGHT;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_ATTACK2\" ) = (int)IN_ATTACK2;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_RUN\" ) = (int)IN_RUN;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_RELOAD\" ) = (int)IN_RELOAD;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_ALT1\" ) = (int)IN_ALT1;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_ALT2\" ) = (int)IN_ALT2;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_SCORE\" ) = (int)IN_SCORE;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_SPEED\" ) = (int)IN_SPEED;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_WALK\" ) = (int)IN_WALK;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_ZOOM\" ) = (int)IN_ZOOM;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_WEAPON1\" ) = (int)IN_WEAPON1;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_WEAPON2\" ) = (int)IN_WEAPON2;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_BULLRUSH\" ) = (int)IN_BULLRUSH;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_GRENADE1\" ) = (int)IN_GRENADE1;" )
        mb.add_registration_code( "bp::scope().attr( \"IN_GRENADE2\" ) = (int)IN_GRENADE2;" )
        
        # Content bit flags
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_EMPTY\" ) = (int)CONTENTS_EMPTY;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_SOLID\" ) = (int)CONTENTS_SOLID;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_WINDOW\" ) = (int)CONTENTS_WINDOW;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_AUX\" ) = (int)CONTENTS_AUX;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_GRATE\" ) = (int)CONTENTS_GRATE;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_SLIME\" ) = (int)CONTENTS_SLIME;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_WATER\" ) = (int)CONTENTS_WATER;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_BLOCKLOS\" ) = (int)CONTENTS_BLOCKLOS;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_OPAQUE\" ) = (int)CONTENTS_OPAQUE;" )
        mb.add_registration_code( "bp::scope().attr( \"LAST_VISIBLE_CONTENTS\" ) = (int)LAST_VISIBLE_CONTENTS;" )
        
        mb.add_registration_code( "bp::scope().attr( \"ALL_VISIBLE_CONTENTS\" ) = (int)ALL_VISIBLE_CONTENTS;" )
        
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_TESTFOGVOLUME\" ) = (int)CONTENTS_TESTFOGVOLUME;" )
        #mb.add_registration_code( "bp::scope().attr( \"CONTENTS_UNUSED\" ) = (int)CONTENTS_UNUSED;" )
        #mb.add_registration_code( "bp::scope().attr( \"CONTENTS_UNUSED6\" ) = (int)CONTENTS_UNUSED6;" )
        
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_TEAM1\" ) = (int)CONTENTS_TEAM1;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_TEAM2\" ) = (int)CONTENTS_TEAM2;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_IGNORE_NODRAW_OPAQUE\" ) = (int)CONTENTS_IGNORE_NODRAW_OPAQUE;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_MOVEABLE\" ) = (int)CONTENTS_MOVEABLE;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_AREAPORTAL\" ) = (int)CONTENTS_AREAPORTAL;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_PLAYERCLIP\" ) = (int)CONTENTS_PLAYERCLIP;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_MONSTERCLIP\" ) = (int)CONTENTS_MONSTERCLIP;" )
        
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_CURRENT_0\" ) = (int)CONTENTS_CURRENT_0;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_CURRENT_90\" ) = (int)CONTENTS_CURRENT_90;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_CURRENT_180\" ) = (int)CONTENTS_CURRENT_180;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_CURRENT_270\" ) = (int)CONTENTS_CURRENT_270;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_CURRENT_UP\" ) = (int)CONTENTS_CURRENT_UP;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_CURRENT_DOWN\" ) = (int)CONTENTS_CURRENT_DOWN;" )
        
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_ORIGIN\" ) = (int)CONTENTS_ORIGIN;" )
        
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_MONSTER\" ) = (int)CONTENTS_MONSTER;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_DEBRIS\" ) = (int)CONTENTS_DEBRIS;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_DETAIL\" ) = (int)CONTENTS_DETAIL;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_TRANSLUCENT\" ) = (int)CONTENTS_TRANSLUCENT;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_LADDER\" ) = (int)CONTENTS_LADDER;" )
        mb.add_registration_code( "bp::scope().attr( \"CONTENTS_HITBOX\" ) = (int)CONTENTS_HITBOX;" )
        
        # Surf
        mb.add_registration_code( "bp::scope().attr( \"SURF_LIGHT\" ) = (int)SURF_LIGHT;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_SKY2D\" ) = (int)SURF_SKY2D;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_SKY\" ) = (int)SURF_SKY;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_WARP\" ) = (int)SURF_WARP;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_TRANS\" ) = (int)SURF_TRANS;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_NOPORTAL\" ) = (int)SURF_NOPORTAL;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_TRIGGER\" ) = (int)SURF_TRIGGER;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_NODRAW\" ) = (int)SURF_NODRAW;" )
        
        mb.add_registration_code( "bp::scope().attr( \"SURF_HINT\" ) = (int)SURF_HINT;" )
        
        mb.add_registration_code( "bp::scope().attr( \"SURF_SKIP\" ) = (int)SURF_SKIP;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_NOLIGHT\" ) = (int)SURF_NOLIGHT;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_BUMPLIGHT\" ) = (int)SURF_BUMPLIGHT;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_NOSHADOWS\" ) = (int)SURF_NOSHADOWS;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_NODECALS\" ) = (int)SURF_NODECALS;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_NOCHOP\" ) = (int)SURF_NOCHOP;" )
        mb.add_registration_code( "bp::scope().attr( \"SURF_HITBOX\" ) = (int)SURF_HITBOX;" )
        
        # Tick
        mb.add_registration_code( "bp::scope().attr( \"TICK_NEVER_THINK\" ) = (int)TICK_NEVER_THINK;" )
        
        # Blood
        mb.add_registration_code( "bp::scope().attr( \"DONT_BLEED\" ) = (int)DONT_BLEED;" )
        mb.add_registration_code( "bp::scope().attr( \"BLOOD_COLOR_RED\" ) = (int)BLOOD_COLOR_RED;" )
        mb.add_registration_code( "bp::scope().attr( \"BLOOD_COLOR_YELLOW\" ) = (int)BLOOD_COLOR_YELLOW;" )
        mb.add_registration_code( "bp::scope().attr( \"BLOOD_COLOR_GREEN\" ) = (int)BLOOD_COLOR_GREEN;" )
        mb.add_registration_code( "bp::scope().attr( \"BLOOD_COLOR_MECH\" ) = (int)BLOOD_COLOR_MECH;" )
    
        # Hud defines
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_WEAPONSELECTION\" ) = (int)HIDEHUD_WEAPONSELECTION;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_FLASHLIGHT\" ) = (int)HIDEHUD_FLASHLIGHT;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_ALL\" ) = (int)HIDEHUD_ALL;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_HEALTH\" ) = (int)HIDEHUD_HEALTH;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_PLAYERDEAD\" ) = (int)HIDEHUD_PLAYERDEAD;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_NEEDSUIT\" ) = (int)HIDEHUD_NEEDSUIT;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_MISCSTATUS\" ) = (int)HIDEHUD_MISCSTATUS;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_CHAT\" ) = (int)HIDEHUD_CHAT;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_CROSSHAIR\" ) = (int)HIDEHUD_CROSSHAIR;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_VEHICLE_CROSSHAIR\" ) = (int)HIDEHUD_VEHICLE_CROSSHAIR;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_INVEHICLE\" ) = (int)HIDEHUD_INVEHICLE;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_BONUS_PROGRESS\" ) = (int)HIDEHUD_BONUS_PROGRESS;" )
        mb.add_registration_code( "bp::scope().attr( \"HIDEHUD_BITCOUNT\" ) = (int)HIDEHUD_BITCOUNT;" )
        
    def AddAdditionalCode(self, mb):
        header = code_creators.include_t('coordsize.h')
        mb.code_creator.adopt_include(header)
        
        super(SrcBase, self).AddAdditionalCode(mb)
