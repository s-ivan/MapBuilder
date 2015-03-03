
#ifndef MBteredit_tx_h
#define MBteredit_tx_h

////////////////////////////////////////////////////////
// texture editor within terrain editor
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
// variables

// texture selection
var		teredit_tx_count;											// number of texture skins
var 		teredit_tx_switch_type[TEREDIT_MAXPERSET];		// texture to use from the actual set: items 1-5
var 		teredit_tx_selected			= 0;						// 0..4
var 		teredit_tx_thumb_size		= TEREDIT_THUMBSIZE;	// thumbnail image size 
var 		teredit_tx_brush_size		= TEREDIT_BRUSHSIZE;	// texture image size 
var		teredit_tx_opacity 			= 100;					// strength of texture drawing - multiplies brush pixel values
var		teredit_tx_red					= 128;
var		teredit_tx_green				= 128;
var		teredit_tx_blue				= 128;
var		teredit_tx_alpha				= 75;
var		teredit_tx_size;											// 24b skin tetxure size 256,512,1024,2048,4096 ^2

// texture sets
var 		max_teredit_tx_items	= TEREDIT_MAXPERSET;
var 		max_teredit_tx_sets 	= TEREDIT_MAXSET;				// to avoid trying to use invalid sets
var 		teredit_tx_set;											// 10 different sets can be created/used with 5 items in each

// folders
STRING* 	teredit_tx_foldername 		= "textures\\";		// subfolder name for 32b and 24b textures too
STRING* 	teredit_detail_foldername 	= "detailtex\\";		// subfolder name for detail textures

// files
STRING**	teredit_tx_filenames = NULL;							// str_create("") is needed as initialization, before str_cpy() !!! [TEREDIT_MAXPERSET]
TEXT* 	teredit_textures_txt;				

////////////////////////////////////////////////////////
// panels

PANEL*	teredit_tx_panel;

PANEL*	teredit_tx1_panel;		// 
PANEL*	teredit_tx2_panel;		// 
PANEL*	teredit_tx3_panel;		// 
PANEL*	teredit_tx4_panel;		// 
PANEL*	teredit_tx5_panel;		//

////////////////////////////////////////////////////////
// bmaps of: 
// multiskin terrain:	24b copy images of 32b terrain skins, for thumbnail panels
// single skin terrain:	24b texture images, for thumbnail panels	(can be 32b for detail skin)

BMAP*		teredit_tx1_panel_bmp;//		= "brush\\_default.tga";
STRING* 	teredit_tx1_panel_str 		= "bmap = teredit_tx1_panel_bmp;";
BMAP*		teredit_tx2_panel_bmp;//		= "brush\\_default.tga";
STRING* 	teredit_tx2_panel_str 		= "bmap = teredit_tx2_panel_bmp;";
BMAP*		teredit_tx3_panel_bmp;//		= "brush\\_default.tga";
STRING* 	teredit_tx3_panel_str 		= "bmap = teredit_tx3_panel_bmp;";
BMAP*		teredit_tx4_panel_bmp;//		= "brush\\_default.tga";
STRING* 	teredit_tx4_panel_str 		= "bmap = teredit_tx4_panel_bmp;";
BMAP*		teredit_tx5_panel_bmp;//		= "brush\\_default.tga";
STRING* 	teredit_tx5_panel_str 		= "bmap = teredit_tx5_panel_bmp;";

////////////////////////////////////////////////////////
// functions

void		TerEdit_Tx_Init();
		void		TerEdit_Tx_Close();
			
			//------------------------
			// UI
			
			void		TerEdit_Tx_Button24(button_number);
			void		TerEdit_Tx_Button32(button_number);										// sub panel slider fine setting buttons
			void		TerEdit_Tx_Toggle(PANEL* panel);											// click on texture panel
			
			void		TerEdit_Tx_DecSet();															// jump to previous texture/skin set
			void		TerEdit_Tx_IncSet();															// jump to next tetxure/skin set
			
			//------------------------
			// preview update
			
			void		TerEdit_Tx_UpdateSets24();													// update skin thumbnails displayed on main panel - single skin
			void		TerEdit_Tx_UpdateSets32();													// update skin thumbnails displayed on main panel - multi skin
			
				void		TerEdit_Tx_CopyBmaptoBmap(BMAP* from_bmap, BMAP* to_bmap);							// image to thumbnail image
				void		TerEdit_Tx_CopySkintoBmap(ENTITY* from_ent, var skin_num, BMAP* to_bmap);		// get from 24b or 32b terrain texture image a 24b thumbnail image for panels
				void		TerEdit_Tx_EmptySlot32(BMAP* to_bmap);														// empty slot - gray transparent (invisible)
				void		TerEdit_Tx_EmptySlot24(BMAP* to_bmap);														// empty slot - gray (visible)
				
			//------------------------
			// 24b texture filling with color
			
			void		TerEdit_Tx_SetColor24();													// set skin size and color 						- single skin
			void		TerEdit_Tx_SetTexture24();													// tile a texture onto skin, set size		 	- single skin						
			
			//------------------------
			// 32b texture filling with color
			
			void		TerEdit_Tx_SetColor();														// fill current skin rgb color
				void		TerEdit_Tx_Color(BMAP* bmap_pointer, COLOR* to_color);		// change color but keep alpha of an image, pixel by pixel
			
			void		TerEdit_Tx_SetAlpha();														// fill current skin alpha
				void		TerEdit_Tx_Alpha(BMAP* bmap_pointer, var to_alpha);			// change alpha of a 32b image, pixel by pixel	
						
			//------------------------
			// texture drawing - under brush
			
			void	 	TerEdit_Tx_Draw32Area(var mode);											// 1) on left-click, set alpha to brush color - 0) on right-click, set alpha to 0
			void	 	TerEdit_Tx_Draw24Area(var mode);											// on left-click, blend selected texture into actual skin due to brush color (like it would be alpha)
			
			//------------------------
			// managing skins
			
			void		TerEdit_Tx_MoveSkinDown();													// replace with upper skin
			void		TerEdit_Tx_MoveSkinUp();													// replace with lower skin
			
			void	 	TerEdit_Tx_DelSkin32();														// remove a tiling skin, move others backward, last cleared (not removed)
			void	 	TerEdit_Tx_AddSkin32();														// insert a new tiling skin, shift forward current and higher number skins	
			
			void	 	TerEdit_Tx_SetSkin32();														// file menu call
				void		TerEdit_LoadSkin32_tga(char* file_name);							// replace a skin texture by TerrHmp_Tx_LoadSkin_KeepAlpha + editor updates
																											// usually only 2nd trial works after a skin deletion/addition -> check TerrHmp_Tx_LoadSkin_KeepAlpha !
				
		//--------------------------------------
		// load/save
		
		void	 	TerEdit_Save_Textures();														// file menu call
		void	 	TerEdit_Load_Textures();														// file menu call
		
//			void		TerEdit_LoadSkin32_ToSkin_tga(char* file_name, var skin_num);	// not used, pixel by pixel needed always
		
		//---
			
		void	 	TerEdit_Load_Texture24();														// file menu call
		void	 	TerEdit_Save_Texture24();														// file menu call

		void	 	TerEdit_Load_DetailTexture24();												// file menu call
		void	 	TerEdit_Save_DetailTexture24();												// file menu call
		
		//--------------------------------------
		// undo/redo
			
			void		TerEdit_Tx_Store();							// save undo
			void		TerEdit_Tx_Undo();							// save redo, load undo
			void		TerEdit_Tx_Redo();							// laod redo
			void		TerEdit_Tx_Reset();							// load temp
			void		TerEdit_Tx_Restore();						// load original
		
		//---------------------------------------------------
		// helpers
		
		void	 	TerEdit_Bmap_AdaptSize(BMAP* temp_bmap, var temp_size, var bits, var format);			// change bmap size and format, and return new bmap - not used
		
////////////////////////////////////////////////////////
// includes

//#include "MBsavetga.c"


#endif