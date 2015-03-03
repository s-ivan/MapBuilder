
#ifndef MBfileeditor_h
#define MBfileeditor_h

//////////////////////////////////////////////////////////////////////
// variables

PANEL* 	filemenu_panel;
PANEL* 	filemenu_load_panel;

#define 	FILEMENU_MAXPERSET 				14
#define	FILEMENU_MAXFILES					600

BMAP*		filemenu_slider_bmap				= NULL;							// scaleable image to adjust its height to the number of items in the file list displayed
var		filemenu_actual_first_item		= 0;								// 0..max-1 , shows the id of the 1st filename displayed
var		filemenu_all_items				= 0;								// number of file names stored by the TEXT object in its strings
var		filemenu_last_item				= 0;								// the last item that can be filemenu_actual_first_item, to hide empty file slots
var		filemenu_itemslider				= 0;								// actual position of the scrollbar slider, separated from filemenu_actual_first_item, but updated in each frame

STRING**	filemenu_filenames 				= NULL;							// should be created [FILEMENU_MAXPERSET]
TEXT*		filemenu_files_txt;
var		filemenu_select 					= 1;								// if loading - 1st button i.e. 1st file is selected, if nothing i.e. save = 0

//--------------------------------------------------------------------
// filters

STRING*	filemenu_terrains_filter		= "maps\\*.trr";
STRING*	filemenu_materials_filter		= "maps\\*.mat";
STRING*	filemenu_skies_filter			= "maps\\*.sky";

STRING*	filemenu_details_filter			= "maps\\*.grs";
STRING*	filemenu_shrubs_filter			= "maps\\*.shs";
STRING*	filemenu_grasses_filter			= "maps\\*.grm";
STRING*	filemenu_obstacles_filter		= "maps\\*.obm";
STRING*	filemenu_mapents_filter			= "maps\\*.mem";
STRING*	filemenu_terrmdls_filter		= "maps\\*.tem";
STRING*	filemenu_buildings_filter		= "maps\\*.blm";

STRING*	filemenu_hmp_filter				= "terrain_hmp\\*.hmp";		// terrain HMP7 .hmp file
STRING*	filemenu_terrverts_filter		= "maps\\*.trv";				// terrain vertices .trv
STRING*	filemenu_skins_filter			= "maps\\*.tga";
STRING*	filemenu_textures_filter		= "textures\\*.tga";
STRING*	filemenu_detailtexture_filter	= "maps\\*.dds";
STRING*	filemenu_lightmaps_filter		= "maps\\*.tga";

STRING*	filemenu_wmb_filter				= "levels\\*.wmb";

STRING* 	filemenu_materialfx_filter		= "*.fx";
STRING* 	filemenu_scwater_filter			= "maps\\*.scw";
STRING* 	filemenu_mwater_filter			= "maps\\*.smw";
STRING* 	filemenu_terrautotex_filter	= "maps\\*.sat";
STRING* 	filemenu_scsm_filter				= "maps\\*.ssc";
STRING* 	filemenu_pssm_filter				= "maps\\*.sps";

STRING*	filemenu_hmpw_filter				= "water_hmp\\*.hmp";		// terrain HMP7 .hmp file
STRING*	filemenu_watermap_filter		= "water_hmp\\*.tga";

STRING* 	filemenu_formation_filter			= "maps\\*.for";
//STRING*	filemenu_trees_filter			= "maps\\*.trm";
//STRING*	filemenu_floordata_filter		= "maps\\*.fld";
//STRING*	filemenu_armies_filter			= "maps\\*.arm";
//STRING*	filemenu_units_filter			= "maps\\*.unm";
//STRING*	filemenu_tilecluster_filter	= "maps\\*.tcs";
//STRING*	filemenu_map_filter				= "maps\\*.map";

//--------------------------------------------------------------------
// common file menu helpers

var		filemenu_whattoload			= 0;					// 0 or 1,2,3,4,5,6,7,8,9 - based on previous filters, +100 if save
STRING*	filemenu_extension			= "";					// "#4" last 4 characters of actual filter - sometimes it can be 3

var		filemenu_max_input_length	= 32;					// was 24
TEXT* 	filemenu_input_txt 			= NULL;

var		filemenu_import				= 0;					// if loading an object map, it can be decided whether to measure the surface height or not
																		// it can be used for applying the same environment for a different terrain
var		filemenu_orig_uimode;

///////////////////////////////////////////////////////////////////////
// functions

//--------------------------------------------------------
// main filehandling menu

void		FileMenu_Init();
	void		FileMenu_Panel_Init();
		void		FileMenu_Close();
		void 		FileMenu_Exit();

//--------------------------------------------------------
// load/save menu initialization functions

// map-----------------------------
void	FileMenu_LoadTerrain();
void	FileMenu_SaveTerrain();
// entities by category------------
void	FileMenu_LoadDetails();
void	FileMenu_SaveDetails();
void	FileMenu_LoadShrubs();
void	FileMenu_SaveShrubs();
void	FileMenu_LoadGrasses();
void	FileMenu_SaveGrasses();
void	FileMenu_LoadObstacles();
void	FileMenu_SaveObstacles();
void	FileMenu_LoadMapEnts();
void	FileMenu_SaveMapEnts();
void	FileMenu_LoadTerrMdls();
void	FileMenu_SaveTerrMdls();
void	FileMenu_LoadBuildings();
void	FileMenu_SaveBuildings();
//general propertiers---------------
void	FileMenu_LoadSky();
void	FileMenu_SaveSky();
void	FileMenu_LoadMaterial();
void	FileMenu_SaveMaterial();
void	FileMenu_LoadMap();
void	FileMenu_SaveMap();
// terrain editor-------------------
void	FileMenu_SaveHmp();
void	FileMenu_LoadTerrVerts();
void	FileMenu_SaveTerrVerts();
void	FileMenu_LoadWmb();
void	FileMenu_SaveWmb();
void	FileMenu_SaveSkins();
void	FileMenu_LoadSkins();
void	FileMenu_SetSkin32();
void	FileMenu_LoadSkin1();
void	FileMenu_SaveSkin1();
void	FileMenu_LoadDetailSkin2();
void	FileMenu_SaveDetailSkin2();
void	FileMenu_SaveLightMap();
void	FileMenu_LoadLightMap();
// material edittor-----------------
void	FileMenu_LoadMaterialEffect();	// no save
void	FileMenu_LoadScWater();
void	FileMenu_SaveScWater();
void	FileMenu_LoadMWater();
void	FileMenu_SaveMWater();
void	FileMenu_LoadScWaterNm();
void	FileMenu_LoadScWaterFoam();
// rts editors----------------------
void	FileMenu_LoadFormation();
void	FileMenu_SaveFormation();
// material editor cont------------
void	FileMenu_LoadTerrAutoTex();
void	FileMenu_SaveTerrAutoTex();
void	FileMenu_LoadScsm();
void	FileMenu_SaveScsm();
void	FileMenu_LoadPssm();
void	FileMenu_SavePssm();

//--------------------------------------------------------
// common file menu functions

	void	FileMenu_Load(STRING* filemenu_filter);
		void	FileMenu_Load_Exit();												// click Cancel button
			void	FileMenu_Load_Close();
		void	FileMenu_Load_Load();												// click OK button
		void	FileMenu_Load_IncSet();
		void	FileMenu_Load_DecSet();
			void	FileMenu_Load_ChangeSet();
		void	FileMenu_Load_Select(button_number);							// click one of 8 buttons behind file names of actual file set		
		void	FileMenu_Load_Input_Init(STRING* filemenu_filter);
			void	FileMenu_Load_Input_Button();									// click button behind input text field
			void	FileMenu_Load_Input_Loop();			
		void		FileMenu_Load_ToggleImport();
		void		FileMenu_Load_ToggleWmbInfo();
		void		FileMenu_Load_ToggleShaderSet();

//--------------------------------------------------------



#endif