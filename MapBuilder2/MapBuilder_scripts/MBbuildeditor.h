
#ifndef MBbuildeditor_h
#define MBbuildeditor_h

////////////////////////////////////////////////////
// defines

#define BUILDING_MAXPERSET 	8
#define BUILDING_MAXSET 		32

////////////////////////////////////////////////////
// variables

var building_ready			= 0;									// ready for placement to avoid crash

//var building_density			= 1;								// multiply with area size to get actual quantity
//var max_building_density 	= 3;								// max quantity per placement area

var building_defaultsize	= MAP_DEFAULTGRIDSIZE;			// determined for each model from .obs file

var building_tilesize		= 5;									// placement area in tiles
var building_tilesize_x		= 3;									// entity area in tiles x
var building_tilesize_y		= 3;									// entity area in tiles y
//var max_building_size 		= 19;								// max tilesize of placement area
var building_type				= 0;									// (passability) 0 obstacle, 1 terrain default, 2 building, 3 forest - for buildings no rotation allowed!
var building_collision		= 2;									// 0 passable, 1 polygon, 2 tile area
var building_shadow			= 2;									// 0 nothing, 1 dynamic, 2 sprite, 3 model
//var building_randomize		= 0;								// using +/-10% random scaling
//var building_rnd_rot			= 0;								// using +/-20° random pan, and +/-30° random tilt and roll
//var building_istree			= 0;								// 0 - auto detect, 1 tree, 2 not tree
var building_snap				= 1;									// snap models to tile middle
var building_snapz			= 0;									// snap models to tile level in z

var building_switch_type[BUILDING_MAXPERSET];				// what to place from the actual set: items 1-8
var building_selected		= 0;									// 0..7

// obstacle sets
var max_building_items	= BUILDING_MAXPERSET;
var max_building_sets 	= BUILDING_MAXSET;					// to avoid trying to use invalid sets
var building_set			= 1;										// 1..max_building_sets - different obstacle sets can be created/used with 8 items in each

//STRING* 	building_foldername 	= "building_mdl\\";		// subfolder name
STRING**	building_filenames = NULL;								// str_create("") is needed as initialization, before str_cpy() !!! [BUILDING_MAXPERSET]
TEXT* 	buildings_txt;

var building_ambient 	= SKY_DEFAULTAMBIENT/2;
var building_alpha 		= 100;
var building_scale 		= 1;
var building_overlay 	= 0;
//var building_transp 	= 0;										// not used anymore
var building_lightmap 	= 0;										// overrides shadow data: 0 use default settings, 1 no shadow -> create lightmap shadow
var building_decal 		= 0;										// force ent_decal shadow in case of pssm

var building_light 		= 0;
var building_unlit 		= 0;
var building_model 		= 0;

// own color settngs
var building_red			= 128;
var building_green		= 128;
var building_blue			= 128;

// placement area representation
//STRING* building_creator_model = "graphics\\placement_cube_blue.mdl";
//ENTITY* building_placement_cube;
ENTITY* building_model_preview;

// control panel
PANEL*  building_panel;

//////////////////////////////////////////////////
// functions

//------------------------------------------
// editor initialization

void 		Building_Init();													// create tool objects
	void		Building_Close();												// remove all tool objects
	
//	action	Building_Cube();												// the cube shaped object, representing selection area
	action	Building_ModelPreview();									// make model preview
		void		Building_MorphPreviewModel();							// morph on selection change
		void		Building_RotateModelPreview(int rotation);		// +1 to CCW, -1 to CW, and snap to cardinal directions!
		void		Building_TurnModelPreview(int rotation);			// turn by a small angle +/-
		
	void		Building_OpenCreate_ObsFile
				(STRING* building_model_filename);						// read model settings from .obs file

//------------------------------------------
// editor menu

void		Building_Panel_Init();								// shows all the necessary data
	void		Building_Panel_Toggle();						// hide, unhide
	
	void		Building_Panel_Button(button_number);		// called by slider value fine setting buttons - common function
	
	void		Building_DecSet();								// jump to previous set
	void		Building_IncSet();								// jump to next set
//	void		Building_NextSet();								// jump to next set by hotkey
	void		Building_UpdateSetNames();						// update names displayed on panel
	
	void		Building_Flag_Switch(var setplusone);		// the same by hotkey
	void		Building_Flag_Toggle(button_number);		// switch on/off the 8 item within the active set
	
//	void 		Building_DecDens();								// less obstacles per tile
//	void 		Building_IncDens();								// more obstacles per tile
//	
//	void 		Building_DecSize();								// less area size
//	void 		Building_IncSize();								// more area size
	
	void		Building_ToggleUnlit();							// set UNLIT flag for entity to be created
	void		Building_ToggleLight();							// set LIGHT flag for enabling colouring by RGB values
	void		Building_ToggleModel();							// set mat_model material
	
//	void		Building_ToggleRandomize();					// set +/-10% random scaling
	void		Building_ToggleOverlay();						// set OVERLAY transparency
//	void		Building_ToggleRndRot();						// set +/-20°pan and +/-30°tilt and roll
//	void		Building_ToggleTree(button_number);			// 0 auto, 1 set, 2 reset tree
	void		Building_ToggleLightmap();						// include to lightmap creation
	void		Building_ToggleSnap();							// 0 snap off, 1 snap on
	void		Building_ToggleSnapZ();							// 0 snapz off, 1 snapz on
	void		Building_ToggleDecal();
	
	void		Building_LightmapAll(button_number);		// set / reset all obstacles' static lightmap shadows
	
	void		Building_MaterialSetup();						// calls the outer material setup panel - materialsetupclass
	void		Building_EditFile();								// calls outer obs file handling panel

//------------------------------------------
// editor placement

	void	 	Building_CreateObject();						// on left-click						- place a building
	void		Building_CloneObject();							// create a similar copy at the same place and send it to modify object menu

//------------------------------------------
// editor deletion

void		Building_DeleteItem();								// remove selected building object
void		Building_DeleteAll();								// remove all building object

////////////////////////////////////////////////


#endif