
#ifndef MBfile_h
#define MBfile_h

//////////////////////////////////////////////////////////////////////
// variables

var		filemenu_shaderset			= 1;					// load saved shader set corresponding to level (might be inclompatible with shadow_stencil setting)
var		filemenu_wmbinfo				= 1;					// load wmb shadow info file if exists

STRING*	filemenu_foldername 			= "maps\\";  

//--------------------------------------------------------------------
// extensions

STRING*	filemenu_terrains_ext		= ".trr";
STRING*	filemenu_materials_ext		= ".mat";
STRING*	filemenu_shaderset_ext		= ".msh";
STRING*	filemenu_skies_ext			= ".sky";
STRING*	filemenu_details_ext			= ".grs";
STRING*	filemenu_shrubs_ext			= ".shs";
STRING*	filemenu_grasses_ext			= ".grm";
STRING*	filemenu_obstacles_ext		= ".obm";
STRING*	filemenu_mapents_ext			= ".mem";
STRING*	filemenu_terrmdls_ext		= ".tem";
STRING*	filemenu_buildings_ext		= ".blm";

STRING*	filemenu_hmp_ext				= ".hmp";
STRING*	filemenu_terrverts_ext		= ".trv";
STRING*	filemenu_skins_ext			= ".tga";
STRING*	filemenu_detailtexture_ext	= ".dds";
STRING*	filemenu_lightmaps_ext		= ".tga";

STRING*	filemenu_wmb_ext				= ".wmb";
STRING*	filemenu_wmbi_ext				= ".i";				// wmb info

STRING*	filemenu_scwater_ext			= ".scw";
STRING*	filemenu_mwater_ext			= ".smw";
STRING*	filemenu_terrautotex_ext	= ".sat";
STRING*	filemenu_scsm_ext				= ".ssc";
STRING*	filemenu_pssm_ext				= ".sps";

STRING*	filemenu_watermap_ext		= ".tga";

STRING*	filemenu_formation_ext		= ".for";

//...

//--------------------------------------------------------------------
// in-game overall progressbar related

#ifdef MB_GAME

	var		filemenu_progressbar			= 0;					// display or not the overall progressbar
	var		filemenu_entquantity			= 0;					// quantity of all the entities to be placed by FileMenu_Load_All()
	var		filemenu_entcounter			= 0;					// actual number of loaded entities = sum of different entity group counters
	var		filemenu_maploadstate		= 0;					// proc_status cannot be used if called from the same function because own instance is not counted
	var		filemenu_entinwmb				= 0;

#endif

///////////////////////////////////////////////////////////////////////
// functions

//--------------------------------------------------------
// map load/save helpers

void		FileMenu_QuickSave();
void		FileMenu_Reload_Map();

void 		FileMenu_Save_All(char* file_name);
void		FileMenu_Load_All(char* file_name);

void		FileMenu_Load_trr(char* file_name);
void		FileMenu_Load_wmb(char* file_name);

//--------------------------------------------------------
// game progress bar for FileMenu_Load_All

#ifdef MB_GAME

	void		FileMenu_Progressbar(STRING* temp_str);
		void		FileMenu_Entity_SetQuantity(STRING* file_name);
			var		FileMenu_Entity_ReadQuantity(STRING* file_name, STRING* extension);
		void		FileMenu_Entity_UpdateCounter();

#endif

//--------------------------------------------------------


#endif