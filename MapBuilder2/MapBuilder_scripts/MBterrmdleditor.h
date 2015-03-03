
#ifndef MBterrmdleditor_h
#define MBterrmdleditor_h

////////////////////////////////////////////////////
// defines

#define TERRMDL_MAXPERSET 	8
#define TERRMDL_MAXSET 		32
#define TERRMDL_SIZE1		32.0
#define TERRMDL_SIZE2 		64.0

////////////////////////////////////////////////////
// variables

var terrmdl_ready			= 0;									// ready for placement to avoid crash

//var terrmdl_density 		= 1;								// multiply with area size to get actual quantity
var terrmdl_tilesize		= 1;									// placement area in tiles
//var max_terrmdl_density 	= 1;								// max quantity per tile
//var max_terrmdl_size 		= 1;								// max tilesize of placement area

//var terrmdl_randomize		= 1;								// using +/-10% random scaling
//var terrmdl_rnd_rot			= 1;								// using +/-20° random pan, and +/-30° random tilt and roll
var terrmdl_normal			= 0;								// align model to surface by pan and tilt values (pan can be modified afterwards)

var terrmdl_switch_type[TERRMDL_MAXPERSET];				// what to place from the actual set: items 1-8
var terrmdl_selected		= 0;									// 0..7

// terrmdl image size from file
var 		terrmdl_defaultsize	= TERRMDL_SIZE1;			// image size - could be determined for each sprite 
STRING*	terrmdlsize_name = "terrmdl_mdl\\_terrmdlsize.txt";

// terrmdl sets
var 		max_terrmdl_items	= TERRMDL_MAXPERSET;
var 		max_terrmdl_sets 	= TERRMDL_MAXSET;				// to avoid trying to use invalid sets
var 		terrmdl_set			= 1;								// 10 different terrmdl sets can be created/used with 8 items in each
//STRING* 	terrmdl_foldername = "terrmdl_mdl\\";		// subfolder name
STRING**	terrmdl_filenames = NULL;							// str_create("") is needed as initialization, before str_cpy() !!! [TERRMDL_MAXPERSET]
TEXT* 	terrmdls_txt;

var terrmdl_ambient 		= SKY_DEFAULTAMBIENT;
//var terrmdl_alpha 		= 50;
var terrmdl_scale 		= 1;
//var terrmdl_overlay	= 0;

var terrmdl_shadow		= 1;

var terrmdl_light 		= 0;									// unique RGB coloring
var terrmdl_unlit 		= 0;									// use premade material - no more light response
var terrmdl_model			= 0;									// use premade material

// own color settngs
var terrmdl_red			= 128;
var terrmdl_green			= 128;
var terrmdl_blue			= 128;

// placement area representation
STRING* terrmdl_creator_model = "graphics\\placement_cube_greenyellow.mdl";
ENTITY* terrmdl_model_preview;

// control panel
PANEL*  terrmdl_panel;

//////////////////////////////////////////////////
// functions

//------------------------------------------
// editor initialization

void 		TerrMdl_Init();								// create tool objects
	void		TerrMdl_Close();							// remove all tool objects
	
	void		TerrMdl_SetDefaultSize();				// sets default image size based on file value or to 256 high
	action	TerrMdl_ModelPreview();					// make model preview
		void 		TerrMdl_MorphPreviewModel();		// morph entity to be the selected model

//------------------------------------------
// editor menu

void		TerrMdl_Panel_Init();						// shows all the necessary data
	void		TerrMdl_Panel_Toggle();					// hide, unhide
	
	void		TerrMdl_Panel_Button(button_number);// called by slider value fine setting buttons - common function
	
	void		TerrMdl_DecSet();							// jump to previous set
	void		TerrMdl_IncSet();							// jump to next set
//	void		TerrMdl_NextSet();						// jump to next set by hotkey
	void		TerrMdl_UpdateSetNames();				// update names displayed on panel
	
	void		TerrMdl_ToggleUnlit();					// set UNLIT flag and mtl_unlit for entity to be created
	void		TerrMdl_ToggleLight();					// set LIGHT flag for enabling colouring by RGB values
	void		TerrMdl_ToggleModel();					// set model material
	void		TerrMdl_ToggleNormal();					// align model to surface
	void		TerrMdl_ToggleShadow();					// set SHADOW - stencil or pssm only
	
	void		TerrMdl_Flag_Toggle(button_number);	// switch on/off the 8 item within the active set
	void		TerrMdl_Flag_Switch(var setplusone);// the same by hotkey
	
	void		TerrMdl_LightmapAll(button_number);	// set / reset all terrain models' static lightmap shadows
	
	void		TerrMdl_MaterialSetup();				// calls the outer material setup panel - materialsetupclass

//------------------------------------------
// editor placement

void	 	TerrMdl_CreateObject();						// on left-click							- place a terrmdl
void		TerrMdl_CloneObject();						// create a similar copy at the same place and send it to modify object menu

//------------------------------------------
// editor deletion

void		TerrMdl_DeleteItem();						// remove selected object
void		TerrMdl_DeleteAll();							// remove all objects

////////////////////////////////////////////////


#endif