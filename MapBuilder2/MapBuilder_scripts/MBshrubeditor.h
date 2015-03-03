
#ifndef MBshrubeditor_h
#define MBshrubeditor_h

////////////////////////////////////////////////////
// defines

#define SHRUB_MAXPERSET 	8
#define SHRUB_MAXSET 		32

////////////////////////////////////////////////////
// variables

var shrub_density			= 3;								// multiply with area size to get actual quantity
var shrub_tilesize		= 5;								// placement area in tiles
var max_shrub_density 	= 8;								// max quantity per placement area
var max_shrub_size 		= 19;								// max tilesize of placement area

var shrub_snap 			= 1;								// place shrub to tile middle for proper tile pathfinding

var shrub_switch_type[SHRUB_MAXPERSET];				// what to place from the actual set: items 1-8

// shrub sets
var 		max_shrub_items	= SHRUB_MAXPERSET;
var 		max_shrub_sets 	= SHRUB_MAXSET;			// to avoid trying to use invalid sets
var 		shrub_set			= 1;							// 10 different grass sets can be created/used with 8 items in each
//STRING* 	shrub_foldername 	= "shrub_dds\\";		// subfolder name
STRING**	shrub_filenames	= NULL;						// str_create("") is needed as initialization, before str_cpy() !!! [SHRUB_MAXPERSET]
TEXT* 	shrubs_txt;

var shrub_ambient 		= SKY_DEFAULTAMBIENT;
var shrub_alpha 			= 75;
var shrub_scale 			= 1;
var shrub_overlay			= 1;
var shrub_shadow			= 1;								// 0 nothing, (1 dynamic,) 2 sprite, (3 model)
var shrub_lightmap		= 0;								// overrides shadow data: 0 use default settings, 1 no shadow -> create lightmap shadow
var shrub_decal 			= 0;								// 1 force ent_decal shadow in case of pssm

var shrub_pieces 			= 3;
var shrub_piece_counter = 0;								// shadow sprite is placed only in case of the first piece of a bush

var shrub_light 			= 0;								// unique RGB coloring
var shrub_unlit 			= 0;								// use premade material - no more light response
var shrub_sprite			= 0;								// use premade material

// own color settngs
var shrub_red				= 128;
var shrub_green			= 128;
var shrub_blue				= 128;

// placement area representation
STRING* shrub_creator_model = "graphics\\placement_cube_red.mdl";
ENTITY* shrub_placement_cube;

// control panel
PANEL*  shrub_panel;

//////////////////////////////////////////////////
// functions

//------------------------------------------
// editor initialization

void 		Shrub_Init();							// create tool objects
	
	action	Shrub_Cube();							// the cube shaped object, representing selection area
	void		Shrub_Close();							// remove all tool objects

//------------------------------------------
// editor menu

void		Shrub_Panel_Init();					// shows all the necessary data
	void		Shrub_Panel_Toggle();				// hide, unhide
	
	void		Shrub_Panel_Button(button_number);		// called by slider value fine setting buttons - common function
	
	void 		Shrub_DecSize();						// less area size
	void 		Shrub_IncSize();						// more area size
	
	void 		Shrub_DecDens();						// less shrubs per tile
	void 		Shrub_IncDens();						// more shrubs per tile
	
	void		Shrub_DecSet();						// jump to previous set
	void		Shrub_IncSet();						// jump to next set
//	void		Shrub_NextSet();						// jump to next set by hotkey
	void		Shrub_UpdateSetNames();				// update names displayed on panel
	
	void		Shrub_ToggleUnlit();					// set UNLIT flag for entity to be created
	void		Shrub_ToggleLight();					// set LIGHT flag for enabling colouring by RGB values
	void		Shrub_ToggleSprite();				// set sprite material
	
	void		Shrub_ToggleShadow();				// set SHADOW flag for casting a round fading out sprite shadow
	void		Shrub_ToggleOverlay();				// set OVERLAY flag for using black pixels for transparency instead of alpha value - refined by d3d_alpharef 
	void		Shrub_ToggleLightmap();				// set entity to cast shadow only if a lightmap generated
	void		Shrub_ToggleDecal();					// force to have ent_decal shadow in pssm mode
	
	void		Shrub_ToggleSnap();					// set snapping to tile middle
	
	void		Shrub_Flag_Toggle(button_number);		// switch on/off the 8 item within the active set
	void		Shrub_Flag_Switch(var setplusone);		// the same by hotkey
	
	void		Shrub_LightmapAll(button_number);		// set / reset all shrubs' static lightmap shadows
	
	void		Shrub_MaterialSetup();				// calls the outer material setup panel - materialsetupclass

//------------------------------------------
// editor placement

void		Shrub_OnClick();							// outer call from uihandling
	void	 	Shrub_CreateArea();					// on left-click						- place shrubs with a given density
	void		Shrub_CreateArea_Cont();			// on middle-click and hold		- place shrubs until button is pressed
	
	void		Shrub_SeedQuantity();
	void		Shrub_SeedArea();
	
		void	 Shrub_CreateObject(STRING* ent_name_full, VECTOR* pos_vec);		// called by Shrub_CreateArea.../Seed... 	- creates one shrub sprite
				
//------------------------------------------
// editor deletion

void		Shrub_DeleteArea();						// remove all shrubs in the area
void		Shrub_DeleteAll();						// remove all shrub objects

////////////////////////////////////////////////////////////////////////////////////////////////



#endif