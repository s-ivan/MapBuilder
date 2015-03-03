
#ifndef MBdetaileditor_h
#define MBdetaileditor_h

////////////////////////////////////////////////////
// defines

#define DETAIL_MAXPERSET 	8
#define DETAIL_MAXSET 		32

////////////////////////////////////////////////////
// variables

var detail_density 		= 3;								// multiply with area size to get actual quantity
var detail_tilesize		= 5;								// placement area in tiles
var max_detail_density 	= 8;								// max quantity per tile
var max_detail_size 		= 19;								// max tilesize of placement area

var detail_switch_type[DETAIL_MAXPERSET];				// what to place from the actual set: items 1-8

// detail sets
var 		max_detail_items	= DETAIL_MAXPERSET;
var 		max_detail_sets 	= DETAIL_MAXSET;				// to avoid trying to use invalid sets
var 		detail_set			= 1;								// 10 different grass sets can be created/used with 8 items in each
//STRING* 	detail_foldername = "grass_dds\\";			// subfolder name
STRING**	detail_filenames 	= NULL;							// str_create("") is needed as initialization, before str_cpy() !!! [DETAIL_MAXPERSET]
TEXT* 	details_txt;

var detail_ambient 	= SKY_DEFAULTAMBIENT;
var detail_alpha 		= 60;
var detail_scale 		= 1;
var detail_overlay	= 1;

var detail_light 		= 0;										// unique RGB coloring
var detail_unlit 		= 0;										// use premade material - no more light response
var detail_sprite		= 0;										// use premade material

// own color settngs
var detail_red			= 128;
var detail_green		= 128;
var detail_blue		= 128;

// placement area representation
STRING* detail_creator_model = "graphics\\placement_cube_green.mdl";
ENTITY* detail_placement_cube;

// control panel
PANEL*  detail_panel;

//////////////////////////////////////////////////
// functions

//------------------------------------------
// editor initialization

void 		Detail_Init();								// create tool objects
	
	action	Detail_Cube();							// the cube shaped object, representing selection area
	void		Detail_Close();						// remove all tool objects

//------------------------------------------
// editor menu

void		Detail_Panel_Init();						// shows all the necessary data
	void		Detail_Panel_Toggle();				// hide, unhide
	
	void		Detail_Panel_Button(button_number);		// called by slider value fine setting buttons - common function
	
	void 		Detail_DecSize();						// less area size
	void 		Detail_IncSize();						// more area size
	
	void 		Detail_DecDens();						// less details per tile
	void 		Detail_IncDens();						// more details per tile
	
	void		Detail_DecSet();						// jump to previous set
	void		Detail_IncSet();						// jump to next set
//	void		Detail_NextSet();						// jump to next set by hotkey
	void		Detail_UpdateSetNames();			// update names displayed on panel
	
	void		Detail_ToggleUnlit();				// set UNLIT flag and mtl_unlit for entity to be created
	void		Detail_ToggleLight();				// set LIGHT flag for enabling colouring by RGB values
	void		Detail_ToggleSprite();				// set sprite material
	
	void		Detail_ToggleOverlay();				// set OVERLAY transparency - lower quality but has a PASS_SOLID
	
	void		Detail_Flag_Toggle(button_number);		// switch on/off the 8 item within the active set
	void		Detail_Flag_Switch(var setplusone);		// the same by hotkey
	
	void		Detail_MaterialSetup();				// calls the outer material setup panel - materialsetupclass

//------------------------------------------
// editor placement

void		Detail_OnClick();							// outer call from uihandling
	void	 	Detail_CreateArea();					// on left-click						- place details with a given density
	void		Detail_CreateArea_Cont();			// on left-click and hold			- place details until button is pressed

void		Detail_SeedQuantity();
void		Detail_SeedArea();

		void	 Detail_CreateObject(STRING* ent_name_full, VECTOR* pos_vec);		// called by Detail_CreateArea.../Seed... 	- creates one grass sprite

//------------------------------------------
// editor deletion

void		Detail_DeleteArea();						// remove all details in the area
void		Detail_DeleteAll();						// remove all detail objects

////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif