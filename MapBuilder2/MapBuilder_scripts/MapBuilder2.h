#ifndef mapbuilder2_h
#define mapbuilder2_h


//////////////////////////////////////////////
// defines


#define MB_RELEASE							// always must be defined - no RTS related scripts (source is removed from published editor)

//#define MB_GAME								// never define it here - must be defined in your game
													// to access in-game functions only, excluding unused editor functions requiring editor only data

//--------------------------------------------
// some engine bugs under fixing, comment them out when said to be fixed

//#define ENGINE_BUG_PSSM_A840			// blur needed to avoid rendering errors - fixed in A8.44

#define ENGINE_BUG_DECAL_A840				// NOSHADOW can't be used in mirror views - said to be, but not really fixed in A8.45

//--------------------------------------------
// editor (0..499), game (500..998), and map initialization related modes (999...):

#define UIMODE_INIT			1001
#define UIMODE_STARTUP		1000
#define UIMODE_NEWMAP		999

//---
// special entity categories (300..399):

#define UIMODE_GROUNDTERR	300			// was UIMODE_TERRAIN both for ground and water terrain, now they are separated and renamed
#define UIMODE_SHADOWENT	301			// on update, their value should be kept for backward wmb compatibility
#define UIMODE_WATERTERR	302

//---
// camera and tile representation related modes (250..299):

#define UIMODE_CAMERA		250
#define UIMODE_ACTIVETILE	251
#define UIMODE_CLUSTERFLAG	252
#define UIMODE_CLUSTERTILE	253

//---
// entity placement modes and entity categories (0..199):

#define UIMODE_NULL			0				// on update, their value should be kept for backward wmb compatibility
#define UIMODE_DETAIL		1
#define UIMODE_SHRUB			2
#define UIMODE_GRASS			3
#define UIMODE_OBSTACLE		4
#define UIMODE_MAPENT		5
#define UIMODE_TERRMDL		6
#define UIMODE_BUILDING		7

#define UIMODE_TEMPSHRUB	8				// for lightmapping, generated temporary models of sprites, and deleted afterwards

//#define UIMODE_RIVER		9
//#define UIMODE_ROAD		10
//#define UIMODE_PLAYER		15

#define UIMODE_SHADOWSHIFT	100			// used with entity categories, thus entity ent_decal shadow or sprite shadow or model shadow is between 101..108 (>=100 <200)

//---
// other editor modes (200..249):

#define UIMODE_MAPINFO		200		
#define UIMODE_SETTINGS		201
#define UIMODE_TEREDIT		203

#define UIMODE_FILEMENU		210
#define UIMODE_FILEMENU2	211

#define UIMODE_MODIFYOBJ	215
#define UIMODE_MODIFYOBJ2	216

#define UIMODE_SKY			220
#define UIMODE_MATERIAL		222

#define UIMODE_PLAYTEST01			230

//---
// RTS modes

#define UIMODE_PLAYTEST02			231
#define UIMODE_PLAYTEST02_UNITS 	232

#define UIMODE_FORMATION	240


/////////////////////////////////////////////////////////////
// DirectX


LPD3DXMESH dx_mesh;		// for setting terrain, grass model,  and tree model leaf normals by DX function - released manually on level closing


/////////////////////////////////////////////////////////////
// variables


var		stencil_blur_value	= 0;		// use it instead of vecskill1.x
var 		UIMode;								// user interface mode - defines what to do

var		main_water				= 4;		// -1 texture water, 0 3dgs mirror water, >=1 use shade-c water, >= 3 with depth, 1,3 no refraction
var		main_hdr 				= 2;		// switch on/off mtl_hdr pp shader
var		main_engineversion 	= 2;		// 0 : free/extra , 1 : commercial, 2 : pro
var		main_blur 				= 1;		// stencil or pssm blur
var		main_resolution 		= 0;		// shows available resolutions table position - should be initialized
var		main_video_screen 	= 2;		// 0 nothing 1 full 2 window

var		main_camera_mode 		= 0;		// 0 free camera, 1 rts camera
													
var		main_terrain_lod		= 3;		// 0 default (no lod), pro engine features: 1 (cache), 2-4 (use lods 1-3) but shadowmapping issues
													// 1 perfect (better performance, same quality),
													// 2 okay for even very early lods,
													// 3 good quality/performance compromise
													// 4 requires a bit farer lod distances or high res terrain or smaller chunks, but last switch is clipped by sky cube...

var		main_terrain_chunk	= 32;		// default value storage, required for good terrain lod performance, own value is saved for each map
													// (too small lowers fps, but more dynamic lighs can appear within the same chunk area)

#ifdef MB_RELEASE
var		main_minresx			= 1024;	// max compound panel width is 768
var		main_minresy			= 768;	// max panel height is 512
#else
var		main_minresx			= 800;	// max compound panel width is 768
var		main_minresy			= 600;	// max panel height is 512
#endif


////////////////////////
// functions


	void	Key_Do_Nothing();
	
#ifndef MB_GAME
	void	Main_Restart();	
#endif
	
	void	Main_Exit();	
	void	Exit_Event();
	void	ScreenSize_Event();
	
#ifndef MB_GAME	
	function on_level_event(percent);
#endif


///////////////////////////////
// includes

//--------------------------------------------------------------
// .h

//--------
// common

#include "MBdefines.h"								// no .c						// general editor defines
#include "MBgui_common.h"							// no .c						// shared GUI elements

#include "MBarray.h"																	// there are some things to fix... (void to char in some cases)
#include "MBhelpers.h"																// ray tracing, file operations etc.

//--------
// shader cores and parameter editors, available from sky editor (shadowmappings)

#include "MBscsm.h"
#ifndef MB_GAME
	#include "MBscsmeditor.h"
#endif

#include "MBpssm.h"
#ifndef MB_GAME
	#include "MBpssmeditor.h"
#endif

//--------
// shader cores and parameter editors, available from material editor (waters, auto terrain multitexturing)

#include "MBmwater.h"
#ifndef MB_GAME
	#include "MBmwatereditor.h"
#endif

#include "MBscwater.h"
#ifndef MB_GAME
	#include "MBscwatereditor.h"
#endif

#ifndef MB_GAME
	#include "MBterrautoeditor.h"
#endif

//--------
// material and sky system

#include "MBdecalshadows.h"														// decal-shadows and ent_decal shadows

#include "MBmaterials.h"
#ifndef MB_GAME
	#include "MBmaterialeditor.h"
#endif

#include "MBsky.h"
#ifndef MB_GAME
	#include "MBskyeditor.h"
#endif

//--------
// map system core

#include "MBmap.h"
#include "MBmaphelp.h"																// helpers : gets/sets, tile-coordinate conversions
#include "MBmaphelp2.h"																// helpers : angle-direction conversions, get neighbouring tiles

#ifndef MB_GAME
	#include "MBsettings.h"															// could be called as MBmapeditor
#endif

//--------
// file handling

#include "MBfile.h"
#ifndef MB_GAME	
	#include "MBfileeditor.h"

	#include "MBsaveaswmb.h"
#endif

//--------
// test mode

#include "MBweather.h"																// snow, rain (performance optimized)
#include "MBplayertest01.h"														// 3rd person game mode - might be used in game

//--------
// terrain editor

#ifndef MB_GAME
	#include "MBteredit.h"															// common frame with brush selection
	#include "MBteredit_he.h"														// deformation
	#include "MBteredit_tx.h"														// texture editing
	#include "MBteredit_lm.h"														// lightmapping
#endif

//---------
// entity placement related - core and editor

#include "MBterrhmp.h"
#ifndef MB_GAME
	#include "MBterrhmpeditor.h"
#endif

#include "MBterrwaterhmp.h"
#ifndef MB_GAME
	#include "MBterrwatereditor.h"
#endif

#include "MBdetail.h"
#ifndef MB_GAME
	#include "MBdetaileditor.h"
#endif

#include "MBshrub.h"	
#ifndef MB_GAME
	#include "MBshrubeditor.h"	
#endif

#include "MBgrass.h"			
#ifndef MB_GAME
	#include "MBgrasseditor.h"	
#endif

#include "MBobstacle.h"	
#ifndef MB_GAME
	#include "MBobseditor.h"
#endif

#include "MBmapent.h"		
#ifndef MB_GAME
	#include "MBmapenteditor.h"
#endif

#include "MBterrmdl.h"		
#ifndef MB_GAME
	#include "MBterrmdleditor.h"	
#endif

#include "MBbuilding.h"		
#ifndef MB_GAME
	#include "MBbuildeditor.h"	
#endif

//--------
// entity placement editor common

#ifndef MB_GAME
	#include "MBmodifyobject.h"
	#include "MBobsfilemodify.h"
	#include "MBseedpanel.h"
#endif

//--------
// UI

#include "MBcamerahandling.h"

#ifndef MB_GAME
	#include "MBuihandling.h"
#endif

//--------
// RTS

#ifndef MB_GAME
																						// RTS game related
	#ifndef MB_RELEASE	
		
		#include "MBphysx_rts.h"
		
		#include "MBdefines_rts.h"												// no .c
		
		
		#include "MBformation_rts.h"											// RTS group formation	
		#include "MBformeditor_rts.h"											// RTS group formation editor	
		
		
		#include "MBposinfopanel_rts.h"
		
		#include "MBterrtile_rts.h"											// tile system
		#include "MBterrtilehelp_rts.h"										// gets sets protections conversions - without taking into account level differences
		#include "MBterrtilehelp2_rts.h"										// gets sets protections conversions - taking into account level differences
		
		#include "MBterrclus_rts.h"											// cluster system
		#include "MBterrclus2_rts.h"											// additional functionalities
		#include "MBterrclushelp_rts.h"										// gets sets protections conversions
		
		#include "MBterrain_rts.h"												// terrain system - handling modifications by buildings
		
			
		#include "MBplayertest02_rts.h"										// initialize RTS game test mode
		
		#include "MBrtsunitinit_rts.h"										// unit struct initialization
		#include "MBrtsunitcreate_rts.h"										// unit entity creation
		#include "MBrtsunitupdate_rts.h"										// unit update and old state machine
		#include "MBrtsunitupdate2_rts.h"									// new behaviour tree
		
		#include "MBrtsgroupinit_rts.h"
		#include "MBrtsgroupupdate_rts.h"
			
		#include "MBpathcluster_rts.h"										// cluster level pathfinding used by groups
		#include "MBpathtile_rts.h"											// tile level pathfinding used by units		
		
	#endif
#endif

//--------------------------------------------------------------
// customized system files

#include "MBdefault.c"															// customized default.c
#include "MBmtlView.c"															// hdr mod by commenting out a #define

//--------------------------------------------------------------
// .c

#include "MBvideores.c"															// to get available video resolutions, based on DJFeer's code
//#include "MBsavetga.c"					// no .h							// not used, okay but slow
#include "MBtxtfordirplus.c"				// no .h							// allows file collection with multiple extensions

//--------

#include "MBarray.c"
#include "MBhelpers.c"

//--------

#include "MBscsm.c"
#ifndef MB_GAME
	#include "MBscsmeditor.c"
#endif

#include "MBpssm.c"
#ifndef MB_GAME
	#include "MBpssmeditor.c"
#endif

//--------

#include "MBmwater.c"
#ifndef MB_GAME
	#include "MBmwatereditor.c"
#endif

#include "MBscwater.c"
#ifndef MB_GAME
	#include "MBscwatereditor.c"
#endif

#ifndef MB_GAME
	#include "MBterrautoeditor.c"
#endif

//--------

#include "MBdecalshadows.c"

#include "MBmaterials.c"
#ifndef MB_GAME
	#include "MBmaterialeditor.c"
#endif

#include "MBsky.c"
#ifndef MB_GAME
	#include "MBskyeditor.c"
#endif

//--------

#include "MBmap.c"
#include "MBmaphelp.c"
#include "MBmaphelp2.c"													

#ifndef MB_GAME
	#include "MBsettings.c"
#endif

//--------

#include "MBfile.c"
#ifndef MB_GAME	
	#include "MBfileeditor.c"

	#include "MBsaveaswmb.c"
#endif

//--------

#include "MBweather.c"
#include "MBplayertest01.c"

//--------

#ifndef MB_GAME
	#include "MBteredit.c"
	#include "MBteredit_he.c"
	#include "MBteredit_tx.c"
	#include "MBteredit_lm.c"
#endif

//--------																	

#include "MBterrhmp.c"
#ifndef MB_GAME
	#include "MBterrhmpeditor.c"
#endif

#include "MBterrwaterhmp.c"
#ifndef MB_GAME
	#include "MBterrwatereditor.c"
#endif

#include "MBdetail.c"
#ifndef MB_GAME
	#include "MBdetaileditor.c"													
#endif

#include "MBshrub.c"
#ifndef MB_GAME
	#include "MBshrubeditor.c"
#endif

#include "MBgrass.c"
#ifndef MB_GAME
	#include "MBgrasseditor.c"
#endif

#include "MBobstacle.c"
#ifndef MB_GAME
	#include "MBobseditor.c"
#endif

#include "MBmapent.c"
#ifndef MB_GAME
	#include "MBmapenteditor.c"
#endif

#include "MBterrmdl.c"
#ifndef MB_GAME
	#include "MBterrmdleditor.c"
#endif

#include "MBbuilding.c"
#ifndef MB_GAME
	#include "MBbuildeditor.c"
#endif

//--------

#ifndef MB_GAME
	#include "MBmodifyobject.c"
	#include "MBobsfilemodify.c"
	#include "MBseedpanel.c"
#endif

//--------

#include "MBcamerahandling.c"

#ifndef MB_GAME
	#include "MBuihandling.c"
#endif

//--------
// RTS

#ifndef MB_GAME
	#ifndef MB_RELEASE
		
		#include "MBphysx_rts.c"
		
		
		#include "MBformation_rts.c"											// RTS group formation
		#include "MBformeditor_rts.c"											// RTS group formation editor
		
		
		#include "MBposinfopanel_rts.c"
			
		#include "MBterrtile_rts.c"											// tile system
		#include "MBterrtilehelp_rts.c"										// gets sets protections conversions - without taking into account level differences
		#include "MBterrtilehelp2_rts.c"										// gets sets protections conversions - taking into account level differences	
			
		#include "MBterrclus_rts.c"											// cluster system
		#include "MBterrclus2_rts.c"											// additional functionalities
		#include "MBterrclushelp_rts.c"										// gets sets protections conversions
		
		#include "MBterrain_rts.c"												// terrain system - handling modifications by buildings
		
			
		#include "MBplayertest02_rts.c"										// initialize RTS game test mode		
		
		#include "MBrtsunitinit_rts.c"
		#include "MBrtsunitcreate_rts.c"
		#include "MBrtsunitupdate_rts.c"	
		#include "MBrtsunitupdate2_rts.c"	
		
		#include "MBrtsgroupinit_rts.c"
		#include "MBrtsgroupupdate_rts.c"
		
		#include "MBpathcluster_rts.c"										// cluster level pathfinding used by groups
		#include "MBpathtile_rts.c"											// tile level pathfinding used by units
		
	#endif
#endif

//--------------------------------------------------------------


#endif