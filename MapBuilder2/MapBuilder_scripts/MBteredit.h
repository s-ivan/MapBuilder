
#ifndef MBteredit_h
#define MBteredit_h

/////////////////////////////////////////////////////////////
// defines

#define TEREDIT_MAXPERSET 	5										// one row
#define TEREDIT_MAXSET 		32
#define TEREDIT_THUMBSIZE	32
#define TEREDIT_BRUSHSIZE	256									// not essetnial: bmap_width should be used

//#define TEREDIT_MAXUNDO		10									// cannot be used in structs...

////////////////////////////////////////////////////
// variables

var 		teredit_tilesize			= 15;							// placement area in tiles
var 		max_teredit_size 			= 39;							// max tilesize of placement area
var		teredit_mode				= 1;							// 1 height editor, 2 texture editor, 3 terrain creator

// brush selection
var 		teredit_br_switch_type[TEREDIT_MAXPERSET];		// brush to use from the actual set: items 1-8
var 		teredit_br_selected			= 0;						// 0..4
var 		teredit_br_thumb_size		= TEREDIT_THUMBSIZE;	// thumbnail image size 
var 		teredit_br_brush_size		= TEREDIT_BRUSHSIZE;	// brush image size 

// brush sets
var 		max_teredit_br_items	= TEREDIT_MAXPERSET;
var 		max_teredit_br_sets 	= TEREDIT_MAXSET;				// to avoid trying to use invalid sets
var 		teredit_br_set;											// 10 different sets can be created/used with 5 items in each

STRING* 	teredit_brush_foldername 	= "brush\\";			// subfolder name
STRING**	teredit_br_filenames = NULL;							// str_create("") is needed as initialization, before str_cpy() !!! [TEREDIT_MAXPERSET]
TEXT* 	teredit_brushes_txt;

// placement area representation
STRING* 	teredit_creator_model = "graphics\\placement_cube_brown.mdl";
ENTITY* 	teredit_placement_cube;

////////////////////////////////////////////////////
// panels

PANEL*	teredit_br_panel;

PANEL*	teredit_brush1_panel;		// 
PANEL*	teredit_brush2_panel;		// 
PANEL*	teredit_brush3_panel;		// 
PANEL*	teredit_brush4_panel;		// 
PANEL*	teredit_brush5_panel;		// 

BMAP*		teredit_brush1_bmp;//		= "brush\\_default.tga";
STRING* 	teredit_brush1_str 		= "bmap = teredit_brush1_bmp;";
BMAP*		teredit_brush2_bmp;//		= "brush\\_default.tga";
STRING* 	teredit_brush2_str 		= "bmap = teredit_brush2_bmp;";
BMAP*		teredit_brush3_bmp;//		= "brush\\_default.tga";
STRING* 	teredit_brush3_str 		= "bmap = teredit_brush3_bmp;";
BMAP*		teredit_brush4_bmp;//		= "brush\\_default.tga";
STRING* 	teredit_brush4_str 		= "bmap = teredit_brush4_bmp;";
BMAP*		teredit_brush5_bmp;//		= "brush\\_default.tga";
STRING* 	teredit_brush5_str 		= "bmap = teredit_brush5_bmp;";

//BMAP* teredit_area_bmp;				// not used - because of ent_decal high slope terrain alignment error

////////////////////////////////////////////////////////
// functions

void 	TerEdit_Init();
void 	TerEdit_Close();

	action	TerEdit_Cube();										// the cube shaped object, representing modification area
//	void 		TerEdit_MorphDecal();								// when decal is used instead of cube, change its skin due to brush - wrong rotation on high slope terrain (engine bug)
	
	void		TerEdit_Panel_Br_Init();							// main panel - upper small part		
		
		void 		TerEdit_DecSize();								// less area size
		void 		TerEdit_IncSize();								// more area size
				
		void		TerEdit_DecSet();									// jump to previous brush set
		void		TerEdit_IncSet();									// jump to next brush set
		void		TerEdit_UpdateSetNames();						// update brush thumbnails displayed on main panel
		
		void		TerEdit_Brush_Toggle(PANEL* panel);			// select one among the 5 brush within the active set
		
		void		TerEdit_Mode_Toggle(button_number);			// select editor mode, 1 height, 2 texture, 3 random mod terrain
		
		void		TerEdit_OnClick();								// calls the proper function on mouse click
		
////////////////////////////////////////////////////////
// includes

//#include "MBteredit_he.c"
//#include "MBteredit_tx.c"
//#include "MBteredit_lm.c"



#endif