///////////////////////////////////////////////////////////
// general object property setting script
// defines skills containing all necessary data of any game object (unit,building,vegetation)
// creating dynamic array for moving units path data
//////////////////////////////////////////////////////////

//		 skill1..skill20 are used for entering default properties in WED. SAVED INTO WMB !!!
//		skill21..skill40 were used by the A6 template scripts, and are otherwise free for the user. 
//		skill41..skill50 are used for shaders. 
//		skill61..skill70 are used for storing internal data by the template scripts and the script libraries. 
//		skill71..skill80 are used for data exchange between template script functions, f.i. for input parameters. 
//		skill81..skill100 are reserved for future extensions. 

#ifndef MBdefines_rts_h
#define MBdefines_rts_h

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//----------------------------------------
//----------------------------------------
// map decomposition

//#define CLUSTER_SUBDIVISION_INFO		// show during cluster subdivision which cluster is processed

//----------------------------------------
//----------------------------------------
// general playertest02 related

#define DEBUG_PLAY02						// for testing - debug data onscreen - group selection by mouse - unit events (touch, click)
//#define DEBUG_BMAP_PLAY02				// for testing - representing tile or area data by a bitmap drawn onto normal object shaders
//#define DEBUG_SHADER_PLAY02				// for testing - use a simlplified faster common shader - requires DEBUG_BMAP_PLAY02 - works with stencil_shadow -1,1,8 too

//#define DEBUG_CLUSTERCORNERS			// for testing - show red flags at cluster corner tiles used for cluster subdivision (a bit slower processing)

#define PLAY02_BONE							// always! - faster animation - not okay with stencil shadows - in PSSM shadow and depth, or SCSM depth shaders BONE should be defined
#define PLAY02_PS								// always! - use pixel shader in unit bone animation shader - needed for adding player colour display - and for self shadows
#define PLAY02_SPREADUNITUPDATE			// always! - better performance, worse unit response - spread units "behave" update over several frames (only half of them are processed within a frame)
//#define PLAY02_PHYSX						// never! - slower movement/collision - should be reworked to have unit push/bounce ! currently they can get stuck on collision !
													// problem after loading a new level - units do not move !!!

//----------------------------------------
//----------------------------------------
// unit update related

#define DEBUG_UNIT_INFO					// for testing - show selected unit's basic information - should be reworked as a panel !
//#define DEBUG_UNIT_TARGETING			// for testing - show pathfinder data of targeting of selected unit

#define FAR_LOD_UNIT_STATIC_PATH			// always! - no unit map used for far lod unit pathfindings - not only own group, but all units are excluded !

#define MAX_UNIT_SPEED_BOOST		1.5	// when unit is getting far from its desired group formation position
#define MIN_UNIT_SPEED_BOOST		0.5

#define HIT_PASSABLE_LIMIT			20		// CAN BE LOWERED - 30 frames until it simply pushes away other unit, and bounces off, over it does not, and becomes to use passable movement (teleport without collision check)

//----------------------------------------
// unit pathfinding related

#define ASTAR_OPEN_LINKED_TILE			// always! - faster!!! - a pseudo linked list is used in tile astar pathfinder, placed in astar struct

//#define DEBUG_UNITPATH					// for testing - show selected group's units paths by lines, and selected unit's path by dots
//#define DEBUG_UNITAREA					// for testing - show all the astar area values of selected unit - if DEBUG_GROUPUNITAREA set, the group's pseudo unit's path is represented
//#define DEBUG_OWNMAP						// for testing - show own clearance map created on path search of selected unit, instead of astar area - requires DEBUG_UNITAREA !!!

													// should be stored in a skill depending on type,
#define TURN_NOLIMIT	0						// no limit in turn allowed within path steps				- original operation, fine
#define TURN_MAX45	1						// max 45° turn limit is allowed within path steps			- produces funny/ugly result, too many pathfindings and moving around
#define TURN_MAX90	2						// max 90° turn limit is allowed within path steps			- can be okay for groups having only a few units, all as leaders

#define AREASIZE_MULTIPLIER		4.0	// area size for tile astar search area = (3 * map_clustersize) + 1 = 25					
													//	3.0 is fine for small groups (very performance sensitive!), 4.0 is very good!
													// dependent on group size !!! (the larger group needs the largr value)
													// dependent on cluster size !!! (the smaller cluster needs the larger value)

#define MAXPATHLOOP_MULTIPLIER 	4.0	// loop limit of unit tile path search = map_clustersize * map_clustersize * 5	
													// 5.0 is fine (very performance sensitive!). 10.0 is perfect, but too slow!
#define UNIT_FARLOD_DISTANCE		50.0	// in tiles

//----------------------------------------
//----------------------------------------
// group update related

#define DEGUG_GROUP_TARGETING			// for testing - show units desired and real target positions
#define DEBUG_GROUP_FGP					// for testing - show the tile path of the group used consisting of OGP and NGP producing FGP, functioning as reference path tiles of the group

//#define DEBUG_GROUPUNITAREA				// for testing - show pseudo unit's path area of selected group, instead of selected unit path area, requires DEBUG_UNITAREA !!!

//#define DEBUG_GROUP_PATHUPDATE			// for testing - play sound of path increment type of selected group

#define GROUP_STUCK_LIMIT			5

//----------------------------------------
// group pathfinding related

#define ASTAR_OPEN_LINKED_CLUSTER		// always! - faster!!!

//#define DEBUG_GROUP_PATH					// for testing - show cluster path by flags of selected group (blue for cluster steps, large green for target tile, small green for nect cluster step)
//#define DEBUG_GROUP_PATHAREA				// for testing - show cluster path astar area values of selected group

//----------------------------------------
//----------------------------------------
// orders states behaviour

// group and unit - not really used - only info
#define STATE_IDLE				1
#define STATE_MOVE				2

// group and unit - used only for groups as an additional info
#define ORDER_IDLE				1
#define ORDER_MOVE				2

//---

#define ANIM_IDLE					1
#define ANIM_MOVE					2

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////
// defines - constants

#define MAX_UNIT_SIZE 	11					// = CL_LARGESHIP

///////////////////////////////////////////////////////////////
// defines - skills

#define ORIG_AREA_SIZEX		skill66		// area occupied by entity in tiles - before dynamic map decompositions
#define ORIG_AREA_SIZEY		skill67		// area occupied by entity in tiles - before dynamic map decompositions

// entity parameters

#define ENT_SHADOW			skill61
#define ENT_FEETZ				skill62

#define ENT_HANDLE			skill63		// * entity specific handle value - used by the RTS building cluster identification system
#define ENT_CLIP				skill64		// * clipping distance 0-1-2-3-4
#define ENT_ANIM				skill65		// * 1-states: animated, or 0: non-animated model

// soldier animation

#define ANIM_PERC 			skill22		// animation frame-percentage of actual state
#define ANIM_STATE 			skill23		// animation state - idle,walk,(run),attack,(shoot),die,(climb),(defend) - NOT USED
#define ANIM_SPEED 			skill24		// animation speed factor (*time_step) of actual animation state

// soldier related skills: - should be set in the editor

#define UNIT_OWNER			skill25		// player num
#define UNIT_ARMY		 		skill26		// more armies of a player can be present in a battle
#define UNIT_GROUP			skill27		// group id number within an army
#define UNIT_TYPE		 		skill28		// unity type
#define UNIT_CAPABILITY 	skill29		// which capability map to use
#define UNIT_ARRAYNUM		skill30		// entity and group array number - not flexible - must be stored in a skill !!!

// animation and move speed related:

#define SPEED_WALK 			skill31
#define SPEED_RUN	 			skill32
#define SPEED_CLIMB 			skill33
#define SPEED_ROTATE			skill34

#define SPEED_CHARGE			skill35
#define SPEED_ACCEL			skill36

// skills 41-50-60 are reserved for shaders !!!
// skill 99 is used by shade-c !!!

// battle and strategic AI related:
//
//#define UNIT_HEALTH		skill32
//#define UNIT_FATIGUE	 	skill33
//#define UNIT_MORALE		skill34

//#define SKILL_LEVEL 		skill77		// experience level				(general effect on behaviour)
//#define SKILL_ATT_1		skill78		// attack bonus of weapon 1	(ranged)
//#define SKILL_ATT_2		skill79		// attack bonus of weapon 2	(melee)
//#define SKILL_DEF_1 		skill80		// defence bonus melee
//#define SKILL_DEF_2		skill81		// defence bonus ranged
//
//#define WEAPON_1			skill48		// equipment types (attached models to unit body)
//#define WEAPON_2			skill49
//#define HELMET				skill50
//#define SHIELD				skill51
//#define ARMOUR				skill52		// equipment type (skin of unit body)
//
//#define ATT_INF_W1			skill53		// hit points of weapons - read from tech tree
//#define ATT_CAV_W1			skill54
//#define ATT_BLD_W1			skill55
//#define ATT_INF_W2			skill56
//#define ATT_CAV_W2			skill57
//#define ATT_BLD_W2			skill58
//
//#define DEF_MELEE			skill59		// summed hit protection points of equipments - read from tech tree
//#define DEF_PIERC			skill60

// animation and move related:
//
//#define BONE_TO_ROTATE	skill67		// e.g. horse-archer , chariot archer , soldiers on elephant , siege weapons on towers...
//#define RELOAD_TIME		skill68		// wait until nextweapon usage (not essential - can be part of animation settings)

/////////////////////////////////////////////////////////////
// constants

const var defrts_speed_infantry_walk	= 5;		// was 7
const var defrts_speed_infantry_run		= 8;		// was 10
const var defrts_speed_infantry_climb	= 3;		// was 5
const var defrts_speed_infantry_charge	= 2;		// charge bonus
const var defrts_speed_infantry_turn	= 50;

// acceleration
// deceleration1
// deceleration2

const var defrts_speed_cavalry_walk		= 12;


SOUND*	okay_snd = "ding.wav";
SOUND*	error_snd = "chord.wav";
SOUND*	note_snd = "error.wav";


#endif