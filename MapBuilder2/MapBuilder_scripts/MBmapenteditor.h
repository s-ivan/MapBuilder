
#ifndef MBmapenteditor_h
#define MBmapenteditor_h

////////////////////////////////////////////////////
// defines

#define MAPENT_MAXPERSET 	8
#define MAPENT_MAXSET 		32

//#define MAPENT_ADD_BUILDING_CLUSTERS					// only for testing !!! - it would result in too many clusters in design time

////////////////////////////////////////////////////
// variables

var mapent_ready			= 0;								// ready for placement to avoid crash

//var mapent_density		= 1;								// multiply with area size to get actual quantity
//var max_mapent_density 	= 1;								// max quantity per placement area

var mapent_tilesize		= 1;								// placement area in tiles - not used, should be removed
var mapent_tilesize_x	= 3;								// object area in tiles x
var mapent_tilesize_y	= 3;								// object area in tiles y
var max_mapent_size 		= 1;								// max tilesize of placement area

var mapent_type			= 0;								// (passability) 0 no access, 1 default, 2 building
var mapent_collision		= 0;								// 0 passable, 1 polygon, 2 tile area
var mapent_shadow			= 0;								// 0 nothing, 1 dynamic, 2 sprite, 3 model
//var mapent_randomize		= 0;								// using +/-10% random scaling
//var mapent_rnd_rot		= 0;								// using +/-20° random pan, and +/-30° random tilt and roll
//var mapent_istree			= 0;								// 0 - auto detect, 1 tree, 2 not tree
var mapent_snap			= 1;								// snap models to tile middle
var mapent_snapz			= 1;								// snap models to tile middle

var mapent_switch_type[MAPENT_MAXPERSET];				// what to place from the actual set: items 1-8
var mapent_selected		= 0;								// 0..7

// mapent image size from file
var 		mapent_defaultsize	= MAP_DEFAULTGRIDSIZE;	// image size - determined for each model from .obs file
//STRING*	mapentsize_name = "obstacle_wmb\\_mapentsize.txt";

// mapent sets
var 		max_mapent_items	= MAPENT_MAXPERSET;
var 		max_mapent_sets 	= MAPENT_MAXSET;				// to avoid trying to use invalid sets
var 		mapent_set			= 1;								// 10 different mapent sets can be created/used with 8 items in each

//STRING* 	mapent_foldername 	= "obstacle_wmb\\";	// subfolder name
STRING**	mapent_filenames = NULL;							// str_create("") is needed as initialization, before str_cpy() !!! [MAPENT_MAXPERSET]
TEXT* 	mapents_txt;

var mapent_ambient = SKY_DEFAULTAMBIENT/2;
var mapent_alpha = 100;
var mapent_scale = 1;
var mapent_overlay = 0;
//var mapent_transp = 0;										// not used
var mapent_lightmap = 0;										// to place of transp - overrides shadow data: 0 use default settings, 1 no shadow -> create lightmap shadow
var mapent_decal = 0;											// force ent_decal shadow in case of pssm

var mapent_light = 0;
var mapent_unlit = 0;
var mapent_model = 0;

// own color settngs
var mapent_red		= 128;
var mapent_green	= 128;
var mapent_blue		= 128;

// placement area representation
//STRING* mapent_creator_model = "graphics\\placement_cube_purple.mdl";
//ENTITY* mapent_placement_cube;
ENTITY* mapent_model_preview;

// control panel
PANEL*  mapent_panel;

//////////////////////////////////////////////////
// functions

//------------------------------------------
// editor initialization

void 		MapEnt_Init();														// create tool objects
	void		MapEnt_Close();												// remove all tool objects
	
	action	MapEnt_ModelPreview();										// the cube shaped object, representing selection area
		void 		MapEnt_MorphModelPreview();							// morph entity to be the selected model
		void		MapEnt_RotateModelPreview(int rotation);			// +1 to CCW, -1 to CW, and snap to cardinal directions!	
		void		MapEnt_TurnModelPreview(int rotation);				// turn by a small angle +/-

	void		MapEnt_OpenCreate_ObsFile
				(STRING* mapent_model_filename);							// read model settings from .obs file

//------------------------------------------
// editor menu
			
void		MapEnt_Panel_Init();							// shows all the necessary data
	void		MapEnt_Panel_Toggle();					// hide, unhide
	
	void		MapEnt_Panel_Button(button_number);	// called by slider value fine setting buttons - common function
	
//	void 		MapEnt_DecSize();							// less area size
//	void 		MapEnt_IncSize();							// more area size
	
//	void 		MapEnt_DecDens();							// less mapents per tile
//	void 		MapEnt_IncDens();							// more mapents per tile
	
	void		MapEnt_DecSet();							// jump to previous set
	void		MapEnt_IncSet();							// jump to next set
//	void		MapEnt_NextSet();							// jump to next set by hotkey
	void		MapEnt_UpdateSetNames();				// update names displayed on panel
	
	void		MapEnt_ToggleUnlit();					// set UNLIT flag for entity to be created
	void		MapEnt_ToggleLight();					// set LIGHT flag for enabling colouring by RGB values
	void		MapEnt_ToggleModel();					// set mat_model material
	
//	void		MapEnt_ToggleRandomize();				// set +/-10% random scaling
	void		MapEnt_ToggleOverlay();					// set OVERLAY transparency
//	void		MapEnt_ToggleRndRot();					// set +/-20°pan and +/-30°tilt and roll
	void		MapEnt_ToggleLightmap();				// 0 not added to lightmap, 1 added to lightmap
	void		MapEnt_ToggleSnap();						// 0 snap off, 1 snap on
	void		MapEnt_ToggleSnapZ();
	void		MapEnt_ToggleDecal();
	
	void		MapEnt_Flag_Toggle(button_number);	// switch on/off the 8 item within the active set
	void		MapEnt_Flag_Switch(var setplusone);	// the same by hotkey
	
	void		MapEnt_LightmapAll(button_number);	// set / reset all mapents' static lightmap shadows
	
	void		MapEnt_MaterialSetup();					// calls the outer material setup panel - materialsetupclass
	void		MapEnt_EditFile();						// calls outer obw file handling panel

//------------------------------------------
// editor placement

void	 	MapEnt_CreateObject();						// on left-click						- place a mapent

void		MapEnt_CloneObject();						// create a similar copy at the same place and send it to modify object menu

//	action	MapEnt_CreateItem();						// called by MapEnt_CreateArea 	- creates one mapent model

//------------------------------------------
// editor deletion

void		MapEnt_DeleteItem();							// remove selected building object
void		MapEnt_DeleteAll();							// remove all building object

//void		MapEnt_Remove_MapEntities();
//	function		MapEnt_Remove_MapEntity();

////////////////////////////////////////////////


#endif