
#ifndef MBteredit_lm_h
#define MBteredit_lm_h


////////////////////////////////////////////////////////
// variables

var		teredit_lm_size				= 1024;					// lightmap bitmap size in pixels
var		teredit_lm_overwrite			= 1;						// overwrite existing lightmap
var		teredit_lm_alpha				= 1;						// alpha texture pixel testing
var		teredit_lm_terrain			= 1;						// cast terrain shadows
var		teredit_lm_noshadow			= 128;					// no shadow color
var		teredit_lm_shadow				= 84;						// normal shadows
var		teredit_lm_alphashadow		= 105;					// terrain shadows, and unresolved alpha shadows
var		teredit_lm_defaulttrace		= 1;						// use default tracing length
var		teredit_lm_length				= 5000;					// custom tracing length
var		teredit_lm_passes				= 1;						// blurring passes
var		teredit_lm_strength			= 5;						// blurring strength
var		teredit_lm_treshold			= 28;						// blurring treshold - below this difference compared to no shadow colour, no blurring applied
var		teredit_lm_accuracy			= 50;						// alpha testing cycle number i.e. accuracy
var		teredit_lm_alphalevel		= 50;						// below this alpha value texture is handled as transparent
var		teredit_lm_iteration			= 0;						// if shadows are shifter around terrain edges use this to adjust

////////////////////////////////////////////////////////
// panels

PANEL*	teredit_lm_panel;

////////////////////////////////////////////////////////
// functions

void		TerEdit_Lm_Init();											// lightmap creation
		void		TerEdit_Lm_Close();									// 
		
		//--------------------------------
		// UI
		
		void		TerEdit_Lm_Button(button_number);	
		void		TerEdit_Lm_ToggleOver();
		void		TerEdit_Lm_ToggleAlpha();
		void		TerEdit_Lm_ToggleTerrain();
		void		TerEdit_Lm_ToggleLength();
		
		//--------------------------------
		// load/save
		
		void		TerEdit_Lm_SaveLightMap();
		void		TerEdit_Lm_LoadLightMap();
		
		//--------------------------------
		// helpers
			
		VECTOR*	TerEdit_Lm_PixelFromPos(ENTITY* terrain, float position_x, float position_y,BMAP* bmap_skin);
		VECTOR*	TerEdit_Lm_PixelToPos(ENTITY* terrain, double pixel_x, double pixel_y, BMAP* bmap_skin);
		
		//--------------------------------
		// lightmap processing
			
		void		TerEdit_Lm_Lightmap_Alpha();																	// lightmap with entity alpha texture support
		void		TerEdit_Lm_Lightmap_Clear();																	// delete lightmap
	
		void TerEdit_Lm_DoBlur();																					// blur lightmap
			void 	TerEdit_Lm_Blur(var passes, BMAP* canvas, var treshold, var strength);
	
		//--------------------------------
		// shrub related
		
			void		TerEdit_Lm_Create_ShrubModels();															// create temporary shrub models based on shrub sprites to be traceable
			void		TerEdit_Lm_Remove_ShrubModels();															// removing temporary shrub models
		
		//--------------------------------

#endif