
#ifndef MBobseditor_h
#define MBobseditor_h

////////////////////////////////////////////////////
// defines

#define OBSTACLE_MAXPERSET 	8
#define OBSTACLE_MAXSET 		32

////////////////////////////////////////////////////
// variables

var obstacle_ready			= 0;								// ready for placement to avoid crash

var obstacle_density			= 1;								// multiply with area size to get actual quantity
var max_obstacle_density 	= 3;								// max quantity per placement area

var obstacle_tilesize		= 5;								// placement area in tiles
var obstacle_tilesize_x		= 3;								// entity area in tiles x
var obstacle_tilesize_y		= 3;								// entity area in tiles y
var max_obstacle_size 		= 19;								// max tilesize of placement area

var obstacle_type				= 0;								// (passability) 0 obstacle, 1 terrain default, 2 building, 3 forest - for buildings no rotation allowed!
var obstacle_collision		= 2;								// 0 passable, 1 polygon, 2 tile area
var obstacle_shadow			= 2;								// 0 nothing, 1 dynamic, 2 sprite, 3 model
var obstacle_randomize		= 0;								// using +/-10% random scaling
var obstacle_rnd_rot			= 0;								// using +/-20° random pan, and +/-30° random tilt and roll
var obstacle_istree			= 0;								// 0 - auto detect, 1 tree, 2 not tree
var obstacle_snap				= 1;								// snap models to tile middle
var obstacle_snapz			= 0;								// snap models to tile level in z

var obstacle_switch_type[OBSTACLE_MAXPERSET];			// what to place from the actual set: items 1-8
var obstacle_selected		= 0;								// 0..7

// obstacle image size from file
var 		obstacle_defaultsize	= MAP_DEFAULTGRIDSIZE;		// image size - determined for each model from .obs file
//STRING*	obstaclesize_name = "obstacle_mdl\\_obstaclesize.txt";

// obstacle sets
var 		max_obstacle_items	= OBSTACLE_MAXPERSET;
var 		max_obstacle_sets 	= OBSTACLE_MAXSET;			// to avoid trying to use invalid sets
var 		obstacle_set			= 1;								// 1..max_obstacle_sets - different obstacle sets can be created/used with 8 items in each

//STRING* 	obstacle_foldername 	= "obstacle_mdl\\";		// subfolder name
STRING**	obstacle_filenames = NULL;								// str_create("") is needed as initialization, before str_cpy() !!! [OBSTACLE_MAXPERSET]
TEXT* 	obstacles_txt;

var obstacle_ambient 	= SKY_DEFAULTAMBIENT/2;
var obstacle_alpha 		= 100;
var obstacle_scale 		= 1;
var obstacle_overlay 	= 0;
//var obstacle_transp 	= 0;										// not used anymore
var obstacle_lightmap 	= 0;										// overrides shadow data: 0 use default settings, 1 no shadow -> create lightmap shadow
var obstacle_decal 		= 0;										// force ent_decal shadow in case of pssm

var obstacle_light 		= 0;
var obstacle_unlit 		= 0;
var obstacle_model 		= 0;

// own color settngs
var obstacle_red			= 128;
var obstacle_green		= 128;
var obstacle_blue			= 128;

// placement area representation
STRING* obstacle_creator_model = "graphics\\placement_cube_blue.mdl";
ENTITY* obstacle_placement_cube;
ENTITY* obstacle_model_preview;

// control panel
PANEL*  obstacle_panel;

//////////////////////////////////////////////////
// functions

//------------------------------------------
// editor initialization

void 		Obstacle_Init();													// create tool objects
	void		Obstacle_Close();												// remove all tool objects
	
	action	Obstacle_Cube();												// the cube shaped object, representing selection area
	action	Obstacle_ModelPreview();									// make model preview, show if placement area is 1(x1)
		void		Obstacle_MorphPreviewModel();							// morph on selection change
		void		Obstacle_RotateModelPreview(int rotation);		// +1 to CCW, -1 to CW, and snap to cardinal directions!
		void		Obstacle_TurnModelPreview(int rotation);			// turn by a small angle +/-			

	void		Obstacle_OpenCreate_ObsFile
				(STRING* obstacle_model_filename);						// read model settings from .obs file

//------------------------------------------
// editor menu

void		Obstacle_Panel_Init();						// shows all the necessary data
	void		Obstacle_Panel_Toggle();				// hide, unhide
	
	void		Obstacle_Panel_Button(button_number);		// called by slider value fine setting buttons - common function
	
	void		Obstacle_DecSet();						// jump to previous set
	void		Obstacle_IncSet();						// jump to next set
//	void		Obstacle_NextSet();						// jump to next set by hotkey
	void		Obstacle_UpdateSetNames();				// update names displayed on panel
	
	void		Obstacle_Flag_Switch(var setplusone);		// the same by hotkey
	void		Obstacle_Flag_Toggle(button_number);		// switch on/off the 8 item within the active set
	
	void 		Obstacle_DecDens();						// less obstacles per tile
	void 		Obstacle_IncDens();						// more obstacles per tile
	
	void 		Obstacle_DecSize();						// less area size
	void 		Obstacle_IncSize();						// more area size
	
	void		Obstacle_ToggleUnlit();					// set UNLIT flag for entity to be created
	void		Obstacle_ToggleLight();					// set LIGHT flag for enabling colouring by RGB values
	void		Obstacle_ToggleModel();					// set mat_model material
	
	void		Obstacle_ToggleRandomize();			// set +/-10% random scaling
	void		Obstacle_ToggleOverlay();				// set OVERLAY transparency
	void		Obstacle_ToggleRndRot();				// set +/-20°pan and +/-30°tilt and roll
	void		Obstacle_ToggleTree(button_number);	// 0 auto, 1 set, 2 reset tree
	void		Obstacle_ToggleLightmap();				// include to lightmap creation
	void		Obstacle_ToggleSnap();					// 0 snap off, 1 snap on
	void		Obstacle_ToggleSnapZ();					// 0 snapz off, 1 snapz on
	void		Obstacle_ToggleDecal();
	
	void		Obstacle_LightmapAll(button_number);		// set / reset all obstacles' static lightmap shadows
	
	void		Obstacle_MaterialSetup();				// calls the outer material setup panel - materialsetupclass
	void		Obstacle_EditFile();						// calls outer obs file handling panel

//------------------------------------------
// editor placement

void	Obstacle_OnClick();
	void	 	Obstacle_CreateArea();					// on left-click					- place obstacles with a given density
	void		Obstacle_CreateArea_Cont();			// on left-click and hold		- place obstacles until button is pressed

void		Obstacle_SeedQuantity();
void		Obstacle_SeedArea();

void		Obstacle_CloneObject();						// create a similar copy at the same place and send it to modify object menu

		void	 	Obstacle_CreateObject(STRING* ent_name_full, VECTOR* pos_vec);

//------------------------------------------
// editor deletion

void		Obstacle_DeleteArea();						// remove all obstacles in the area
void		Obstacle_DeleteItem();						// remove selected obstacle object
void		Obstacle_DeleteAll();						// remove all building object

////////////////////////////////////////////////


#endif