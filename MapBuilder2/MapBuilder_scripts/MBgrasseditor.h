
#ifndef MBgrasseditor_h
#define MBgrasseditor_h

////////////////////////////////////////////////////
// defines

#define GRASS_MAXPERSET 	8
#define GRASS_MAXSET 		32

////////////////////////////////////////////////////
// variables

var grass_density 		= 3;								// multiply with area size to get actual quantity
var grass_tilesize		= 5;								// placement area in tiles
var max_grass_density 	= 8;								// max quantity per tile
var max_grass_size 		= 19;								// max tilesize of placement area

var grass_randomize		= 0;								// using +/-10% random scaling
var grass_rnd_rot			= 0;								// using +/-20° random pan, and +/-30° random tilt and roll
var grass_normal			= 1;								// align model to surface by pan and tilt values (pan can be modified afterwards)

var grass_switch_type[GRASS_MAXPERSET];				// what to place from the actual set: items 1-8

// grass sets
var 		max_grass_items	= GRASS_MAXPERSET;
var 		max_grass_sets 	= GRASS_MAXSET;				// to avoid trying to use invalid sets
var 		grass_set			= 1;								// 10 different grass sets can be created/used with 8 items in each
//STRING* 	grass_foldername = "grass_mdl\\";			// subfolder name
STRING**	grass_filenames = NULL;								// str_create("") is needed as initialization, before str_cpy() !!! [GRASS_MAXPERSET]
TEXT* 	grasses_txt;

var grass_ambient 	= SKY_DEFAULTAMBIENT;
var grass_alpha 		= 50;
var grass_scale 		= 1;
var grass_overlay		= 1;
var grass_shadow		= 0;										// 0 nothing, 1 dynamic/Sprite

var grass_light 		= 0;										// unique RGB coloring
var grass_unlit 		= 0;										// use premade material - no more light response
var grass_model		= 0;										// use premade material

// own color settngs
var grass_red			= 128;
var grass_green		= 128;
var grass_blue			= 128;

// placement area representation
STRING* grass_creator_model = "graphics\\placement_cube_greenyellow.mdl";
ENTITY* grass_placement_cube;

// control panel
PANEL*  grass_panel;

//////////////////////////////////////////////////
// functions

//------------------------------------------
// editor initialization

void 		Grass_Init();								// create tool objects
	
	action	Grass_Cube();							// the cube shaped object, representing selection area
	void		Grass_Close();							// remove all tool objects

//------------------------------------------
// editor menu

void		Grass_Panel_Init();						// shows all the necessary data
	void		Grass_Panel_Toggle();				// hide, unhide
	
	void		Grass_Panel_Button(button_number);		// called by slider value fine setting buttons - common function
	
	void 		Grass_DecSize();						// less area size
	void 		Grass_IncSize();						// more area size
	
	void 		Grass_DecDens();						// less grasses per tile
	void 		Grass_IncDens();						// more grasses per tile
	
	void		Grass_DecSet();						// jump to previous set
	void		Grass_IncSet();						// jump to next set
//	void		Grass_NextSet();						// jump to next set by hotkey
	void		Grass_UpdateSetNames();				// update names displayed on panel
	
	void		Grass_ToggleUnlit();					// set UNLIT flag and mtl_unlit for entity to be created
	void		Grass_ToggleLight();					// set LIGHT flag for enabling colouring by RGB values
	void		Grass_ToggleModel();					// set model material
	
	void		Grass_ToggleOverlay();				// set OVERLAY transparency - lower quality but has a PASS_SOLID
	void		Grass_ToggleRndRot();				// set random pan
	void		Grass_ToggleNormal();				// align model to surface
	void		Grass_ToggleRandomize();			// set a random +/-15% fine scale
	void		Grass_ToggleShadow();				// set SHADOW
	
	void		Grass_Flag_Toggle(button_number);		// switch on/off the 8 item within the active set
	void		Grass_Flag_Switch(var setplusone);		// the same by hotkey
	
	void		Grass_MaterialSetup();				// calls the outer material setup panel - materialsetupclass

//------------------------------------------
// editor placement

void		Grass_OnClick();							// outer call from uihandling
	void	 	Grass_CreateArea();					// on left-click						- place grasses with a given density
	void		Grass_CreateArea_Cont();			// on middle-click and hold		- place grasses until button is pressed
	
	void		Grass_SeedQuantity();
	void		Grass_SeedArea();
	
		void	 Grass_CreateObject(STRING* ent_name_full, VECTOR* pos_vec);		// called by Grass_CreateArea.../Seed... 	- creates one grass model
				
//------------------------------------------
// editor deletion

void		Grass_DeleteArea();						// remove all grass in the area
void		Grass_DeleteAll();						// remove all grass objects

////////////////////////////////////////////////


#endif