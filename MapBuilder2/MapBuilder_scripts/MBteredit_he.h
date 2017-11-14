
#ifndef MBteredit_he_h
#define MBteredit_he_h

////////////////////////////////////////////////////////
// defines

#define MBTEREDIT_USENEWDLL

////////////////////////////////////////////////////////
// dll access

#ifndef MBTEREDIT_USENEWDLL

//#define PRAGMA_PLUGIN "terrain_hmp\\terrain_resave.dll"		// should be at exe folder after publishing

BOOL dll_init_terrain();                              			// prototype of the dll
BOOL dll_load_terrain_from_entity (ENTITY* ent);
BOOL dll_save_terrain_from_entity (ENTITY* ent, STRING* nFl);
BOOL dll_close_terrain();

STRING* dll_path_from_entity( ENTITY* fl , BOOL b_get_absolute);
STRING* dll_path_from_file( STRING* fl , BOOL b_get_absolute);
BOOL dll_path_set_subfolder( STRING* subfolder );

#else

// other dll access save_hmp.dll

int save_hmp(char* filename, ENTITY* terrain, int skip_original_pixels);		// to save modified skins, 1 to save with modified skin, 0 with original

#endif

////////////////////////////////////////////////////////
// variables

var		teredit_he_mode			= 1;							// 1 raise/lower, 2 set/smooth

var		teredit_max = 3000;
var		teredit_min = -500;
var		teredit_adj = 50;
var		teredit_set = 500;
var		teredit_raiselowerflatten = 0;

int teredit_he_undostep;											// the last undo step stored
int teredit_he_firstundo;											// the earliest undo step stored (can differ after some undo/redo steps)

////////////////////////////////////////////////////
// panels

PANEL*	teredit_he_panel;

////////////////////////////////////////////////////////
// functions

	void		TerEdit_He_Init();
		void		TerEdit_He_Close();
			
			void		TerEdit_He_Button(button_number);				// sub panel slider fine setting buttons
			void		TerEdit_He_ToggleMode(button_number);			// what to do by mouse click
		
		//-------------------------------------------------
			
			void		TerEdit_He_InitTerrain();							// initialize vertex struct array and undo counter - outer call from teredit
			
		//-------------------------------------------------
		// move all terrain vertices
			
			void		TerEdit_He_RaiseAll();								// move all vertices up, store them in act array
			void		TerEdit_He_LowerAll();								//...................down.......................
			void		TerEdit_He_SetAll();									//..................to a given level............		
			
				void		TerEdit_He_SetTerrain( var worklevel, var workmode);		// set terrain vertices to array values
			
		//-------------------------------------------------
		// clamp all terrain vertices
				
			void		TerEdit_He_SetMin();									// limit vertex height to min
			void		TerEdit_He_SetMax();									// limit vertex height to max			
			
		//-------------------------------------------------						
		// modify area vertices
			
			void	 	TerEdit_He_RaiseLowerArea(var raise_lower);	// on left-click raise (1), on right-click lower (0)		
			
			void	 	TerEdit_He_SetArea();								// on left-click
			
			void	 	TerEdit_He_SmoothArea();							// on right-click
				void	 	TerEdit_He_SmoothVertex(var vertex);		// measue neighbouring vertices and set the given vertex height to their average			
			
		//-------------------------------------------------
		// undo
		
			void	 	TerEdit_He_Reset();									// reset to initial vertex height values stored in dynamic array on init
			void	 	TerEdit_He_Undo();									// undo last mod, needs more on the fly vertex caching, not only initial
			void	 	TerEdit_He_Redo();									// redo last mod			
			void		TerEdit_He_StoreUndoStep();						// store all vertex heights
			
		//-------------------------------------------------
		// load/save
		
		void		TerEdit_Save_Vertices();								// the preferred and safe way of saving terrain deformation based on an original hmp terrain
		void		TerEdit_Load_Vertices();								// load and set vertex z values
		
		void	 	TerEdit_Save_Terrain();									// FileMenu call - new terrain entity name -> project .trr should be resaved to use it
		void		TerEdit_Save_Hmp(char* file_name);					// back call from FileMenu - outer dll call works only with non-chunked terrains... !!!
																					// should be moved to file menu because after saving no further height editing possible !!!!!!!!!!!!		
		//-------------------------------------------------
		// entities
		
		void		TerEdit_UpdateEntities();								// place on terrain top after deformation (nearly same as importing entity maps again)
			void		Teredit_UpdateEntityZ();							// *** should be tested again ***

////////////////////////////////////////////////////////
// includes



#endif