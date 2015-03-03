////////////////////////////////////////////////////
// the main in program file handling menus
// can load/save and construct different maps
////////////////////////////////////////////////////

#ifndef MBfileeditor_c
#define MBfileeditor_c

///////////////////////////////////////////////////////////////////////////////////////////////
// code


///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////
// main filemenu panel functions //
///////////////////////////////////


void		FileMenu_Init()
{
	//----------------------------------------------------
	
	if (
			(proc_status(FileMenu_Init)>0)
			||
			(proc_status(FileMenu_Load)>0)
			||
			(filemenu_panel)
			||
			(filemenu_load_panel)
		)
		{
			proc_kill2(FileMenu_Init, NULL);
			FileMenu_Close();
			wait(3);
			printf("Double Call : FileMenu_Init");
//			return;
		}
	
	//----------------------------------------------------
		
	FileMenu_Panel_Init();
	
	//----------------------------------------------------
	
	while((UIMode==(var)UIMODE_FILEMENU) || (UIMode==(var)UIMODE_FILEMENU2))			// file menu and file load submenu
		{			
			wait(1);
		}
	
	//----------------------------------------------------
	
	FileMenu_Close();
	
	//----------------------------------------------------
}


void		FileMenu_Close()
{
	if (filemenu_panel != NULL) 
		{
			ptr_remove(filemenu_panel);
			filemenu_panel = NULL;
		}	
	
//	// remove temp string array
//	var i;
//	for(i=0;i<filemenu_max_items;i++)
//		{
//			ptr_remove(filemenu_filenames[i]);
//		}
}


void 		FileMenu_Exit()									
{																						
	wait(3);
	
	//-------------------------------------
	// close MapBuilder
	
//	Main_Exit();	
	
	//-------------------------------------
	// restart startup menu
{}
#ifndef MB_GAME	
	Main_Restart();
#endif	
	//-------------------------------------
}	


void		FileMenu_Panel_Init()
{
	filemenu_whattoload = 0;
	
	filemenu_panel = pan_create( panel_image2 , 11 );
	set(filemenu_panel,SHOW | SHADOW | OVERLAY);						// TRANSLUCENT | 
//	filemenu_panel.alpha = 75;
	filemenu_panel.pos_x = ui_submenupanel_x;							// screen_size.x-256;
	filemenu_panel.pos_y = ui_submenupanel_y;							// 256;
	filemenu_panel.red	= panel_font_r;
	filemenu_panel.green	= panel_font_g;
	filemenu_panel.blue	= panel_font_b;
	filemenu_panel.event = ui_submenu_panelmove;
	
	pan_setstring(filemenu_panel, 0,	15,10,	panel_font2,			str_create("FILE MENU") );
	
	// terrain 						.trr
	pan_setstring(filemenu_panel, 0,			15,50,		panel_font2,			str_create("Terrain set") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,50,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadTerrain,	 	NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,50,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveTerrain,	 	NULL, NULL); 	
	//-----------------------------------------
	// material						.mat
	pan_setstring(filemenu_panel, 0,			15,90,		panel_font2,			str_create("Material settings") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,90,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadMaterial,	 	NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,90,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveMaterial,	 	NULL, NULL); 		
	// sky 							.sky
	pan_setstring(filemenu_panel, 0,			15,120,		panel_font2,			str_create("Sky settngs") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,120,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadSky,	 			NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,120,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveSky,	 			NULL, NULL); 		
	//-----------------------------------------
	// details			 			.grs
	pan_setstring(filemenu_panel, 0,			15,160,		panel_font2,			str_create("Grass sprites") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,160,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadDetails,	 	NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,160,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveDetails,	 	NULL, NULL); 
	// shrubs						.shs
	pan_setstring(filemenu_panel, 0,			15,190,		panel_font2,			str_create("Shrub sprites") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,190,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadShrubs,	 		NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,190,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveShrubs,	 		NULL, NULL); 
	//-----------------------------------------
	// grasses			 			.grm
	pan_setstring(filemenu_panel, 0,			15,230,		panel_font2,			str_create("Grass models") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,230,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadGrasses,	 		NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,230,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveGrasses,	 		NULL, NULL); 
//	// bushes						.shm
//	pan_setstring(filemenu_panel, 0,			15,260,		panel_font2,			str_create("Shrub models") );
//	pan_setbutton(filemenu_panel, 0,	1, 	120,260,		s_load_on_bmp, s_load_on_bmp, s_load_over_bmp, s_load_over_bmp,		NULL,	 		NULL, NULL); 	
//	pan_setbutton(filemenu_panel, 0,	1, 	175,260,		s_save_on_bmp, s_save_on_bmp, s_save_over_bmp, s_save_over_bmp,		NULL,	 		NULL, NULL); 
	//-------------------------------------------
	// obstacles					.obm
	pan_setstring(filemenu_panel, 0,			15,260,		panel_font2,			str_create("Obstacle models") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,260,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadObstacles,	 	NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,260,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveObstacles,	 	NULL, NULL); 
	// mapents						.mem
	pan_setstring(filemenu_panel, 0,			15,290,		panel_font2,			str_create("Map Entities") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,290,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadMapEnts,	 	NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,290,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveMapEnts,	 	NULL, NULL); 
	// terrain models				.tem
	pan_setstring(filemenu_panel, 0,			15,320,		panel_font2,			str_create("Terrain Models") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,320,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadTerrMdls,	 	NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,320,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveTerrMdls,	 	NULL, NULL); 
//	//---------------------------------------------------------------------------------------------------------------------
//	// trees							.trm
//	pan_setstring(filemenu_panel, 0,			15,320,		panel_font2,			str_create("Tree models") );
//	pan_setbutton(filemenu_panel, 0,	1, 	120,400,		s_load_on_bmp, s_load_on_bmp, s_load_over_bmp, s_load_over_bmp,		NULL,	 	NULL, NULL); 	
//	pan_setbutton(filemenu_panel, 0,	1, 	175,400,		s_save_on_bmp, s_save_on_bmp, s_save_over_bmp, s_save_over_bmp,		NULL,	 	NULL, NULL); 
	// buildings					.bum
	pan_setstring(filemenu_panel, 0,			15,350,		panel_font2,			str_create("Building models") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,350,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadBuildings,	 	NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,350,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveBuildings,	 	NULL, NULL); 
//	// units							.unm
//	pan_setstring(filemenu_panel, 0,			15,380,		panel_font2,			str_create("Unit models") );
//	pan_setbutton(filemenu_panel, 0,	1, 	120,460,		s_load_on_bmp, s_load_on_bmp, s_load_over_bmp, s_load_over_bmp,		NULL,	 	NULL, NULL); 	
//	pan_setbutton(filemenu_panel, 0,	1, 	175,460,		s_save_on_bmp, s_save_on_bmp, s_save_over_bmp, s_save_over_bmp,		NULL,	 	NULL, NULL); 
//	// tile-cluster system		.tcs
//	pan_setstring(filemenu_panel, 0,			15,410,		panel_font2,			str_create("Tile-Cluster Sys") );
//	pan_setbutton(filemenu_panel, 0,	1, 	120,490,		s_load_on_bmp, s_load_on_bmp, s_load_over_bmp, s_load_over_bmp,		NULL,	 	NULL, NULL); 	
//	pan_setbutton(filemenu_panel, 0,	1, 	175,490,		s_save_on_bmp, s_save_on_bmp, s_save_over_bmp, s_save_over_bmp,		NULL,	 	NULL, NULL); 	
//	//-----------------------------------------------------------------------------------------------------------------------
	// export wmb and $$m
	pan_setstring(filemenu_panel, 0,			15,410,		panel_font2,			str_create("WMB level") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,410,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadWmb,	 	NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,410,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveWmb,	 	NULL, NULL); 		
	//---------------------------------------
	// terrain + entity maps
	pan_setstring(filemenu_panel, 0,			15,440,		panel_font2,			str_create("Full map") );
	pan_setbutton(filemenu_panel, 0,	1, 	120,440,		s_load_on_bmp, s_load_off_bmp, s_load_over_bmp, s_load_over_bmp,		FileMenu_LoadMap,	 	NULL, NULL); 	
	pan_setbutton(filemenu_panel, 0,	1, 	175,440,		s_save_on_bmp, s_save_off_bmp, s_save_over_bmp, s_save_over_bmp,		FileMenu_SaveMap,	 	NULL, NULL); 	
	
//	// load map
//	pan_setbutton(filemenu_panel, 0,	1, 	24,410,		loadmap_on_bmp, loadmap_on_bmp, loadmap_on_bmp, loadmap_on_bmp,								FileMenu_LoadMap, 	NULL, NULL); 
//	// save map
//	pan_setbutton(filemenu_panel, 0,	1, 	24,440,		savemap_on_bmp, savemap_on_bmp, savemap_on_bmp, savemap_on_bmp,								FileMenu_SaveMap, 	NULL, NULL); 
	// exit
	pan_setbutton(filemenu_panel, 0,	1, 	24,480,		exit_on_bmp, exit_off_bmp, exit_over_bmp, exit_over_bmp,						FileMenu_Exit,			 		NULL, NULL); 			
	
}


///////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// functions calling and be called by common file managing menu //
//////////////////////////////////////////////////////////////////


// .trr file handling
void	FileMenu_LoadTerrain()
{
	wait(3);
	filemenu_whattoload = 1;
	FileMenu_Load(filemenu_terrains_filter);	
}

void	FileMenu_SaveTerrain()
{
	wait(3);
	filemenu_whattoload = 101;
	FileMenu_Load(filemenu_terrains_filter);	
}

// .grs file handling
void	FileMenu_LoadDetails()
{
	wait(3);
	filemenu_whattoload = 2;
	FileMenu_Load(filemenu_details_filter);	
}
void	FileMenu_SaveDetails()
{
	wait(3);
	filemenu_whattoload = 102;
	FileMenu_Load(filemenu_details_filter);	
}

// .shs file handling
void	FileMenu_LoadShrubs()
{
	wait(3);
	filemenu_whattoload = 3;
	FileMenu_Load(filemenu_shrubs_filter);	
}
void	FileMenu_SaveShrubs()
{
	wait(3);
	filemenu_whattoload = 103;
	FileMenu_Load(filemenu_shrubs_filter);	
}

// .obm file handling
void	FileMenu_LoadObstacles()
{
	wait(3);
	filemenu_whattoload = 4;
	FileMenu_Load(filemenu_obstacles_filter);	
}
void	FileMenu_SaveObstacles()
{
	wait(3);
	filemenu_whattoload = 104;
	FileMenu_Load(filemenu_obstacles_filter);	
}

// .mem file handling
void	FileMenu_LoadMapEnts()
{
	wait(3);
	filemenu_whattoload = 7;
	FileMenu_Load(filemenu_mapents_filter);	
}
void	FileMenu_SaveMapEnts()
{
	wait(3);
	filemenu_whattoload = 107;
	FileMenu_Load(filemenu_mapents_filter);	
}

// .grm file handling
void	FileMenu_LoadGrasses()
{
	wait(3);
	filemenu_whattoload = 8;
	FileMenu_Load(filemenu_grasses_filter);	
}
void	FileMenu_SaveGrasses()
{
	wait(3);
	filemenu_whattoload = 108;
	FileMenu_Load(filemenu_grasses_filter);	
}

// .tem file handling
void	FileMenu_LoadTerrMdls()
{
	wait(3);
	filemenu_whattoload = 9;
	FileMenu_Load(filemenu_terrmdls_filter);
}
void	FileMenu_SaveTerrMdls()
{
	wait(3);
	filemenu_whattoload = 109;
	FileMenu_Load(filemenu_terrmdls_filter);
}

// .blm file handling
void	FileMenu_LoadBuildings()
{
	wait(3);
	filemenu_whattoload = 10;
	FileMenu_Load(filemenu_buildings_filter);	
}
void	FileMenu_SaveBuildings()
{
	wait(3);
	filemenu_whattoload = 110;
	FileMenu_Load(filemenu_buildings_filter);	
}

//-----------------------------------------------

// .sky file handling
void	FileMenu_LoadSky()
{
	wait(3);
	filemenu_whattoload = 20;
	FileMenu_Load(filemenu_skies_filter);	
}
void	FileMenu_SaveSky()
{
	wait(3);
	filemenu_whattoload = 120;
	FileMenu_Load(filemenu_skies_filter);	
}

// .mat file handling
void	FileMenu_LoadMaterial()
{
	wait(3);
	filemenu_whattoload = 21;
	FileMenu_Load(filemenu_materials_filter);	
}
void	FileMenu_SaveMaterial()
{
	wait(3);
	filemenu_whattoload = 121;
	FileMenu_Load(filemenu_materials_filter);	
}

//-----------------------------------------------

// terrain + entity maps handling
void	FileMenu_LoadMap()
{
	wait(3);
	filemenu_whattoload = 30;
	FileMenu_Load(filemenu_terrains_filter);	
}
void	FileMenu_SaveMap()
{
	wait(3);
	filemenu_whattoload = 130;
	FileMenu_Load(filemenu_terrains_filter);	
}

//-----------------------------------------------

// terrain .hmp file
void	FileMenu_SaveHmp()
{
	wait(3);
	filemenu_whattoload = 140;
	FileMenu_Load(filemenu_hmp_filter);	
}

// terrain vertices .trv file
void	FileMenu_LoadTerrVerts()
{
	wait(3);
	filemenu_whattoload = 41;
	FileMenu_Load(filemenu_terrverts_filter);	
}
void	FileMenu_SaveTerrVerts()
{
	wait(3);
	filemenu_whattoload = 141;
	FileMenu_Load(filemenu_terrverts_filter);	
}

// export import wmb levels
void	FileMenu_LoadWmb()
{
	wait(3);
	filemenu_whattoload = 42;
	FileMenu_Load(filemenu_wmb_filter);	
}
void	FileMenu_SaveWmb()
{
	wait(3);
	filemenu_whattoload = 142;
	FileMenu_Load(filemenu_wmb_filter);	
}

void	FileMenu_LoadSkins()
{
	wait(3);
	filemenu_whattoload = 43;
	FileMenu_Load(filemenu_skins_filter);	
}

void	FileMenu_SaveSkins()
{
	wait(3);
	filemenu_whattoload = 143;
	FileMenu_Load(filemenu_skins_filter);	
}
void	FileMenu_SetSkin32()
{
	wait(3);
	filemenu_whattoload = 44;
	FileMenu_Load(filemenu_textures_filter);	// change 32b texture image, but keep alpha
}

void	FileMenu_LoadSkin1()
{
	wait(3);
	filemenu_whattoload = 45;
	FileMenu_Load(filemenu_skins_filter);	
}
void	FileMenu_SaveSkin1()
{
	wait(3);
	filemenu_whattoload = 145;
	FileMenu_Load(filemenu_skins_filter);	
}

void	FileMenu_LoadDetailSkin2()
{
	wait(3);
	filemenu_whattoload = 46;
	FileMenu_Load(filemenu_detailtexture_filter);	
}
void	FileMenu_SaveDetailSkin2()
{
	wait(3);
	filemenu_whattoload = 146;
	FileMenu_Load(filemenu_detailtexture_filter);	
}

void	FileMenu_LoadLightMap()
{
	wait(3);
	filemenu_whattoload = 47;
	FileMenu_Load(filemenu_lightmaps_filter);
}
void	FileMenu_SaveLightMap()
{
	wait(3);
	filemenu_whattoload = 147;
	FileMenu_Load(filemenu_lightmaps_filter);
}

//-----------------------------------------------

void	FileMenu_LoadMaterialEffect()
{
	wait(3);
	filemenu_whattoload = 48;
	FileMenu_Load(filemenu_materialfx_filter);
}

//-----------------------------------------------

void	FileMenu_LoadScWater()
{
	wait(3);
	filemenu_whattoload = 49;
	FileMenu_Load(filemenu_scwater_filter);
}
void	FileMenu_SaveScWater()
{
	wait(3);
	filemenu_whattoload = 149;
	FileMenu_Load(filemenu_scwater_filter);
}

void	FileMenu_LoadMWater()
{
	wait(3);
	filemenu_whattoload = 50;
	FileMenu_Load(filemenu_mwater_filter);
}
void	FileMenu_SaveMWater()
{
	wait(3);
	filemenu_whattoload = 150;
	FileMenu_Load(filemenu_mwater_filter);
}

void	FileMenu_LoadScWaterNm()
{
	wait(3);
	filemenu_whattoload = 51;
	FileMenu_Load(filemenu_watermap_filter);
}

void	FileMenu_LoadScWaterFoam()
{
	wait(3);
	filemenu_whattoload = 52;
	FileMenu_Load(filemenu_watermap_filter);
}

//-----------------------------------------------

void	FileMenu_LoadFormation()
{
	wait(3);
	filemenu_whattoload = 53;
	FileMenu_Load(filemenu_formation_filter);
}
void	FileMenu_SaveFormation()
{
	wait(3);
	filemenu_whattoload = 153;
	FileMenu_Load(filemenu_formation_filter);
}

//-----------------------------------------------

void	FileMenu_LoadTerrAutoTex()
{
	wait(3);
	filemenu_whattoload = 54;
	FileMenu_Load(filemenu_terrautotex_filter);
}
void	FileMenu_SaveTerrAutoTex()
{
	wait(3);
	filemenu_whattoload = 154;
	FileMenu_Load(filemenu_terrautotex_filter);
}

//-----------------------------------------------

void	FileMenu_LoadScsm()
{
	wait(3);
	filemenu_whattoload = 55;
	FileMenu_Load(filemenu_scsm_filter);
}
void	FileMenu_SaveScsm()
{
	wait(3);
	filemenu_whattoload = 155;
	FileMenu_Load(filemenu_scsm_filter);
}

//-----------------------------------------------

void	FileMenu_LoadPssm()
{
	wait(3);
	filemenu_whattoload = 56;
	FileMenu_Load(filemenu_pssm_filter);
}
void	FileMenu_SavePssm()
{
	wait(3);
	filemenu_whattoload = 156;
	FileMenu_Load(filemenu_pssm_filter);
}

///////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////
// common file managing menu //
///////////////////////////////


void FileMenu_Load(STRING* filemenu_filter)
{
sys_marker("000");
	
	//----------------------------------------------------------------	
	// protecttion
	
	if (proc_status(FileMenu_Load)>0)	return;
	if (filemenu_load_panel) return;
	
	//----------------------------------------------------------------
	// menu mode
	
	filemenu_orig_uimode = UIMode;	
	UIMode = (var)UIMODE_FILEMENU2;
	
	//----------------------------------------------------------------
	// create panel
	
	filemenu_load_panel = pan_create( panel_image2 , 15 );
	set( filemenu_load_panel, SHADOW | OVERLAY | SHOW );
//	filemenu_load_panel.pos_x = screen_size.x-256;
//	filemenu_load_panel.pos_y = 256;
	filemenu_load_panel.pos_x = ui_submenupanel_x;				// filemenu_panel.pos_x;
	filemenu_load_panel.pos_y = ui_submenupanel_y;				// filemenu_panel.pos_y;
	filemenu_load_panel.red		= panel_font_r;
	filemenu_load_panel.green	= panel_font_g;
	filemenu_load_panel.blue	= panel_font_b;
//	filemenu_load_panel.event = ui_submenu_panelmove;			// not essential, would require further settings in case of outer calls

sys_marker("001");
	if (filemenu_panel != NULL)
		{
			reset(filemenu_panel, SHOW);
		}
	
	//----------------------------------------------------------------
	// displayed filename string array initialization
	
sys_marker("002");		
	int arraysize = FILEMENU_MAXPERSET;
	filemenu_filenames = (STRING**)sys_malloc( arraysize * sizeof(STRING*) );
	
	int i;	
	for(i=0; i<arraysize; i++)
		{
			filemenu_filenames[i] = str_create("");
		}	
	
	//-----------------------------------------------------------------
	// determine how many files are in the folder
	
sys_marker("003");
	filemenu_files_txt = txt_create( (var)FILEMENU_MAXFILES , (var)0 );
	for(i=0; i<FILEMENU_MAXFILES; i++)
		{
			str_cpy( (filemenu_files_txt->pstring)[i] , "" );
		}	
	
sys_marker("004");
	txt_for_dir(filemenu_files_txt, filemenu_filter); 								// folder + filter		
	
	i = 0;
	while (str_len((filemenu_files_txt->pstring)[i]) > 0)							// or i = txt_for_dir(filemenu_files_txt, filemenu_filter); 
		{
			i++;
		}
	
	// one by one file stepping
	
	filemenu_select 				= 1;														// must be reset
	
	filemenu_actual_first_item = 0;	
	filemenu_all_items			= i;
	filemenu_last_item			= maxv(0, filemenu_all_items - (var)FILEMENU_MAXPERSET);
	filemenu_itemslider			= 0;
	
	//------------------------------------------------------------------
	// copy first x filenames to string array

sys_marker("005");	
	for(i=0; i<FILEMENU_MAXPERSET; i++)
		{
			str_cpy( filemenu_filenames[i] , "" );													// or foldername to give subfolder now
			str_cat( filemenu_filenames[i] , (filemenu_files_txt->pstring)[i] );			// filename with extension
			if ( str_cmpi( filemenu_filenames[i] , "" ) )										// (or foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( filemenu_filenames[i] , "-" );
				}
		}	

	//------------------------------------------------------------------
	
sys_marker("006");	
	// x filename strings to display in menu
	for(i=0; i<FILEMENU_MAXPERSET; i++)
		{
			pan_setstring(filemenu_load_panel, 0,	17,24+2+i*25,	panel_font,			str_create( filemenu_filenames[i] ) );
		}	
	
	// x radio buttons under filenames, buttons 1-8 - if 8 items
	for(i=0; i<FILEMENU_MAXPERSET; i++)
		{
			pan_setbutton(filemenu_load_panel, 0,	4, 	15,24+i*25,		fileselect_on_bmp, fileselect_off_bmp, fileselect_over_bmp, fileselect_over_bmp,				FileMenu_Load_Select,	NULL, NULL); 
		}
	// set first file button
	button_state( filemenu_load_panel , 1 , ON );
	
	//------------------------------------------------------------------
	
	// Cancel button - button 9 - if 8 items
	pan_setbutton(filemenu_load_panel, 0,	1, 	128+24,84+25*(var)FILEMENU_MAXPERSET,		s_cancel_on_bmp, s_cancel_off_bmp, s_cancel_over_bmp, s_cancel_over_bmp,								FileMenu_Load_Exit, 		NULL, NULL); 
	
	// title
	if ( filemenu_whattoload<100 )
		{
			pan_setstring(filemenu_load_panel, 0,	( filemenu_load_panel.size_x/2-str_width("LOAD",panel_font2)/2),3,	panel_font2,			str_create("LOAD") );
		}
	else
		{
			pan_setstring(filemenu_load_panel, 0,	( filemenu_load_panel.size_x/2-str_width("SAVE",panel_font2)/2),3,	panel_font2,			str_create("SAVE") );
		}

	// scrollbar arrows - button 10 11 - if 8 items
	pan_setbutton(filemenu_load_panel, 0,	1, 	226,24,												s_arrow_up_on_bmp, s_arrow_up_off_bmp, s_arrow_up_on_bmp, s_arrow_up_off_bmp,						FileMenu_Load_DecSet, 			NULL, NULL); 
	pan_setbutton(filemenu_load_panel, 0,	1, 	226,46+25*((var)FILEMENU_MAXPERSET-2),		s_arrow_down_on_bmp, s_arrow_down_off_bmp, s_arrow_down_on_bmp, s_arrow_down_off_bmp,			FileMenu_Load_IncSet, 			NULL, NULL); 
	
	// scrollbar slider	
	if (filemenu_last_item >= (var)1)
		{			
			// scale slider bar image
			filemenu_slider_bmap = bmap_createblack(20, 25*(var)FILEMENU_MAXPERSET-75, 32);
			wait(1);
			bmap_blitpart(filemenu_slider_bmap, vert_bar_bmp, NULL, vector(20, 25*(var)FILEMENU_MAXPERSET-75, 0), NULL, NULL);
			wait(1);
				
			// item based stepping
			pan_setslider(filemenu_load_panel, 0,	228,54,				filemenu_slider_bmap,vert_bar_knob_bmp,		0,filemenu_last_item,	filemenu_itemslider); 
		}
		
	// Ok button - button 12 - if 8 items
	pan_setbutton(filemenu_load_panel, 0,	1, 	48,84+25*(var)FILEMENU_MAXPERSET,		s_ok_on_bmp, s_ok_off_bmp, s_ok_over_bmp, s_ok_over_bmp,													FileMenu_Load_Load, 				NULL, NULL); 	 
	
	// text button - button 13 - if 8 items
	pan_setbutton(filemenu_load_panel, 0,	1, 	15,54+25*(var)FILEMENU_MAXPERSET,		fileselect_over_bmp, fileselect_over_bmp, fileselect_over_bmp, fileselect_over_bmp,				FileMenu_Load_Input_Button, 	NULL, NULL); 	 	
	
	//--------------------------------------------------------------------
	// optional
	
sys_marker("064");	
	// import option on map load
	filemenu_import = 0;				// set always off
	if ( (filemenu_whattoload>1) && (filemenu_whattoload<20) )
		{
			pan_setstring(filemenu_load_panel, 0,	48,134+25*(var)FILEMENU_MAXPERSET,	panel_font2,			str_create("import") );
			// button 14 - if 8 items
			pan_setbutton(filemenu_load_panel, 0,	2, 	24,134+25*(var)FILEMENU_MAXPERSET,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		FileMenu_Load_ToggleImport,		NULL,NULL); 
		}
	
sys_marker("065");		
	// wmb load shadow info file
	if (filemenu_whattoload == (var)42)
		{
			pan_setstring(filemenu_load_panel, 0,	48,134+25*(var)FILEMENU_MAXPERSET,	panel_font2,			str_create("load wmb information") );
			// button 14 - if 8 items
			pan_setbutton(filemenu_load_panel, 0,	2, 	24,134+25*(var)FILEMENU_MAXPERSET,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		FileMenu_Load_ToggleWmbInfo,		NULL,NULL); 
			button_state(filemenu_load_panel, (var)FILEMENU_MAXPERSET+6, filemenu_wmbinfo);
		}
	
sys_marker("066");
	// shader set file in case of full map or materials
	if ((filemenu_whattoload == (var)30) || (filemenu_whattoload == (var)21) || (filemenu_whattoload == (var)130) || (filemenu_whattoload == (var)121))
		{
			pan_setstring(filemenu_load_panel, 0,	48,134+25*(var)FILEMENU_MAXPERSET,	panel_font2,			str_create("include shader set") );
			// button 14 - if 8 items
			pan_setbutton(filemenu_load_panel, 0,	2, 	24,134+25*(var)FILEMENU_MAXPERSET,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		FileMenu_Load_ToggleShaderSet,	NULL,NULL); 
			button_state(filemenu_load_panel, (var)FILEMENU_MAXPERSET+6, filemenu_shaderset);
		}
		
	//--------------------------------------------------------------------
sys_marker("007");
	
	filemenu_input_txt 			= txt_create(1,17);
	filemenu_input_txt.pos_x 	= filemenu_load_panel.pos_x + 17;
	filemenu_input_txt.pos_y 	= filemenu_load_panel.pos_y + 256 +25*4;
	filemenu_input_txt.font 	= panel_font; 
	filemenu_input_txt.red		= panel_font_r;
	filemenu_input_txt.green	= panel_font_g;
	filemenu_input_txt.blue		= panel_font_b;
	set( filemenu_input_txt, SHADOW | SHOW );
	
	// filename input text over button - call outer loop
	FileMenu_Load_Input_Init(filemenu_filter);
	
	//--------------------------------------------------------------------
sys_marker("008");
	
	while( UIMode == (var)UIMODE_FILEMENU2 )	
		{
			// to hide the main file menu panel
			if (filemenu_panel != NULL)
				{
					filemenu_panel.pos_x = filemenu_load_panel.pos_x;
					filemenu_panel.pos_y = filemenu_load_panel.pos_y;
				}
			
			// move input text according to panel pos
			filemenu_input_txt.pos_x = filemenu_load_panel.pos_x + 17;
			filemenu_input_txt.pos_y = filemenu_load_panel.pos_y + 56+25*(var)FILEMENU_MAXPERSET;
						
			// file names displayed
			if (filemenu_last_item >= (var)1)
				{
					if ( (filemenu_itemslider > filemenu_actual_first_item + 0.5) || (filemenu_itemslider < filemenu_actual_first_item - 0.5 ) )
						{					
							filemenu_actual_first_item = integer(filemenu_itemslider + 0.5);
							filemenu_actual_first_item = clamp(filemenu_actual_first_item, 0, filemenu_last_item);
							
							FileMenu_Load_ChangeSet();	
						}
				}
			
//			// debug test inputs
//			draw_text(str_for_num(NULL, proc_status(FileMenu_Load_Input_Loop)),500,500,COLOR_WHITE);
//			draw_text(str_for_num(NULL, inkey_active),500,525,COLOR_WHITE);
			
			wait(1);
		}
sys_marker(NULL);		
	
	FileMenu_Load_Close();
}


////////////////////////////////////////////////
// close


void FileMenu_Load_Close()
{	
	if (filemenu_panel != NULL)
		{
			set(filemenu_panel, SHOW);								// show main file menu panel is it was used
		}
	
	inkey_active = 0;													// terminate inkey	
	proc_kill2(FileMenu_Load_Input_Loop, NULL);				// terminate ENTER watching loop
	wait(3);	
	
	//-------------------
	// panel
	
	if (filemenu_load_panel != NULL)
		{
			ptr_remove(filemenu_load_panel);
			filemenu_load_panel = NULL;
		}
	
	//-------------------
	// remove temp text
	int i;
//	for (i = 0; i < (filemenu_max_sets*filemenu_max_items) ; i++) 		// filemenu_files_txt->strings
	for (i=0; i<FILEMENU_MAXFILES ;i++) 
		{
			if ((filemenu_files_txt->pstring)[i]) 
				{
					ptr_remove((filemenu_files_txt->pstring)[i]);
				}
		}
	ptr_remove(filemenu_files_txt);
	filemenu_files_txt = NULL;
	
	//-------------------
	// remove temp string array
	
	for(i=0; i<FILEMENU_MAXPERSET; i++)
		{
			ptr_remove(filemenu_filenames[i]);
		}
	sys_free(filemenu_filenames);
	filemenu_filenames = NULL;
//	ptr_remove(trr_filenames);									// not okay if removed
	
	//-------------------
	// input text
	
	ptr_remove((filemenu_input_txt->pstring)[0]);		// changed to non-created
	ptr_remove(filemenu_input_txt);
	filemenu_input_txt = NULL;
	
	//-------------------
	// scroll bar image
	
	if (filemenu_slider_bmap != NULL)
		{
			ptr_remove(filemenu_slider_bmap);
			filemenu_slider_bmap = NULL;
		}
	
	//-------------------
}


////////////////////////////////////////////////
// Cancel - close


void FileMenu_Load_Exit()
{
	wait(3);
		
	UIMode = filemenu_orig_uimode;		// might be outer call, so goes back to the stored mode
}


////////////////////////////////////////////////
// OK - load/save


void FileMenu_Load_Load()
{
	STRING* fname_str = str_create( (filemenu_input_txt->pstring)[0] );	
	
	if (proc_status(FileMenu_Load_Load) > 0)					// this instance is not counted ? it does not work with 0 if it is after the wait(3) line !
		{
			ptr_remove(fname_str);
			return;
		}
	
	wait(3);
	
	//--------------------
	
	inkey_active = 0;													// terminate inkey	
	proc_kill2(FileMenu_Load_Input_Loop, NULL);				// terminate ENTER watching loop
	
	str_cpy(save_dir,"");											// needed because possibly set to "screenshots" earlier ! (results problems only when laoding files from root folder i.e. without path, e.g. shaders)
	
	//--------------------
	
//	var button_number = filemenu_select;
	str_trim( fname_str );																			// A8 - clips spaces from the beginning and end of the string
	
	// remove all spaces
//	str_replaceall((filemenu_input_txt->pstring)[0], " ", "") ;							// A7 - clips all spaces from string - restricted filenames
	
	//--------------------
		
	STRING* temp_str = str_create("");	
	
	if (filemenu_whattoload==(var)140)
		{
			str_cpy( temp_str , hmp_foldername );												// to terrain_hmp\\
		}
	else if ((filemenu_whattoload==(var)42) || (filemenu_whattoload==(var)142))
		{
			str_cpy( temp_str , lev_foldername );												// to levels\\
		}
	else if (filemenu_whattoload==(var)44)
		{
			str_cpy( temp_str , textures_foldername );										// to textures\\
		}
//	else if ((filemenu_whattoload==(var)46) || (filemenu_whattoload==(var)146))
//		{
//			str_cpy( temp_str , detailtex_foldername );										// to detailtex\\ -> maps
//		}
	else if (filemenu_whattoload==(var)48)
		{
			str_cpy( temp_str , "" );																// root folder (there could be a shader folder in future...)
		}
	else if ((filemenu_whattoload==(var)51) || (filemenu_whattoload==(var)52))
		{
			str_cpy( temp_str , hmpw_foldername );												// to water_hmp\\
		}
	else
		{
			str_cpy( temp_str , filemenu_foldername );										// to maps\\
		}
	
	str_cat( temp_str , fname_str );
	
	// check whether extension exists or not-------------------------------------------------	
	
	STRING* temp_str2 = str_create("");
	str_cpy( temp_str2 , temp_str );
	
	int temp0 = str_stri(temp_str,".");					// get . position starting with 1 or 0 if not found 	(10)	
	int temp1 = str_len(temp_str2); 						// get filter length												(13)
	int ext_length = temp1 - temp0 + 1;					// get extension length											(3+1)
	
	str_clip(temp_str2,temp1-ext_length);				// clip all characters except the last 4 -> 3 or 4
	
	if (!str_cmpi(temp_str2,filemenu_extension))		// extension got from filter
		{
			str_cat(temp_str,filemenu_extension);
		}
					
	// load only existing files , save any name----------------------------------------------
	if ( (filemenu_whattoload<100) && (!file_exists_onepath(temp_str)) )		
		{
			ptr_remove(fname_str);
			ptr_remove(temp_str2);
			ptr_remove(temp_str);
			return;
		}	
	
	//---------------------------------------------------------------------------------------
	// load function calls
	
	if (filemenu_whattoload==(var)1)
		{
			FileMenu_Load_trr(temp_str);
			wait_for(FileMenu_Load_trr);																
		}	
	//---------------------------------------------
	else if (filemenu_whattoload==(var)2)
		{
			Detail_Load_Details_grs(temp_str, filemenu_import);
			wait_for(Detail_Load_Details_grs);
		}	
	else if (filemenu_whattoload==(var)3)
		{
			Shrub_Load_Shrubs_shs(temp_str, filemenu_import);
			wait_for(Shrub_Load_Shrubs_shs);
		}
	else if (filemenu_whattoload==(var)4)
		{
			Obstacle_Load_Obstacles_obm(temp_str, filemenu_import);
			wait_for(Obstacle_Load_Obstacles_obm);
		}
	else if (filemenu_whattoload==(var)7)
		{
			MapEnt_Load_MapEnts_mem(temp_str, filemenu_import);
			wait_for(MapEnt_Load_MapEnts_mem);
		}
	else if (filemenu_whattoload==(var)8)
		{
			Grass_Load_Grasses_grm(temp_str, filemenu_import);
			wait_for(Grass_Load_Grasses_grm);
		}
	else if (filemenu_whattoload==(var)9)
		{
			TerrMdl_Load_TerrMdls_tem(temp_str, filemenu_import);
			wait_for(TerrMdl_Load_TerrMdls_tem);
		}
	else if (filemenu_whattoload==(var)10)
		{
			Building_Load_Buildings_blm(temp_str, filemenu_import);
			wait_for(Building_Load_Buildings_blm);
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)20)
		{
			Sky_Load_Sky_sky(temp_str);
			wait_for(Sky_Load_Sky_sky);
		}
	else if (filemenu_whattoload==(var)21)
		{
			Materials_Load_Materials_mat(temp_str);
			wait_for(Materials_Load_Materials_mat);
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)30)
		{
			FileMenu_Load_All(temp_str);
			wait_for(FileMenu_Load_All);
		}
	//---------------------------------------------
	// ... other loading function calls ...
	// 40 is reserved for hmp
	else if (filemenu_whattoload==(var)41)
		{
			TerrHmp_Load_Vertices_trv(temp_str);
			wait_for(TerrHmp_Load_Vertices_trv);
		}
	else if (filemenu_whattoload==(var)42)
		{					
			FileMenu_Load_wmb(temp_str);
			wait_for(FileMenu_Load_wmb);
		}
	else if (filemenu_whattoload==(var)43)
		{
			TerrHmp_Load_Skins_tga(temp_str);
			wait_for(TerrHmp_Load_Skins_tga);
		}
	else if (filemenu_whattoload==(var)44)
		{
			TerEdit_LoadSkin32_tga(temp_str);
			wait_for(TerEdit_LoadSkin32_tga);
		}
	else if (filemenu_whattoload==(var)45)
		{
			TerrHmp_Load_Skin24_tga(temp_str);
			wait_for(TerrHmp_Load_Skin24_tga);
		}
	else if (filemenu_whattoload==(var)46)
		{
			TerrHmp_Load_DetailMap24_dds(temp_str);
			wait_for(TerrHmp_Load_DetailMap24_dds);
		}
	else if (filemenu_whattoload==(var)47)
		{
			TerrHmp_Load_LightMap_tga(temp_str);
			wait_for(TerrHmp_Load_LightMap_tga);
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)48)
		{
			Material_Load_Effect_fx(temp_str);
			wait_for(Material_Load_Effect_fx);
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)49)
		{
			Scwater_Load_ScWater_scw(temp_str);
			wait_for(Scwater_Load_ScWater_scw);
		}
	else if (filemenu_whattoload==(var)50)
		{
			Mwater_Load_MWater_smw(temp_str);
			wait_for(Mwater_Load_MWater_smw);
		}
	else if (filemenu_whattoload==(var)51)
		{
			Scwater_Load_ScWaterNm_tga(temp_str);
			wait_for(Scwater_Load_ScWaterNm_tga);
		}
	else if (filemenu_whattoload==(var)52)
		{
			Scwater_Load_ScWaterFoam_tga(temp_str);
			wait_for(Scwater_Load_ScWaterFoam_tga);
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)53)
		{
{}
#ifndef MB_RELEASE			
			FormEdit_LoadFormation_for(temp_str);
			wait_for(FormEdit_LoadFormation_for);
#endif			
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)54)
		{
			TerrHmp_Load_AutoTex_sat(temp_str);
			wait_for(TerrHmp_Load_AutoTex_sat);
		}
	else if (filemenu_whattoload==(var)55)
		{
			Scsm_Load_ssc(temp_str);
			wait_for(Scsm_Load_ssc);
		}
	else if (filemenu_whattoload==(var)56)
		{
			Pssm_Load_sps(temp_str);
			wait_for(Pssm_Load_sps);
		}
	//---------------------------------------------
	
	// ...
	
	//----------------------------------------------------------------------------------------------
	// save function calls
	
	else if (filemenu_whattoload==(var)101)
		{
			Map_SaveMap_trr(temp_str);
			wait_for(Map_SaveMap_trr);
		}	
	//---------------------------------------------
	else if (filemenu_whattoload==(var)102)
		{
			Detail_Save_Details_grs(temp_str);
			wait_for(Detail_Save_Details_grs);
		}	
	else if (filemenu_whattoload==(var)103)
		{
			Shrub_Save_Shrubs_shs(temp_str);
			wait_for(Shrub_Save_Shrubs_shs);
		}	
	else if (filemenu_whattoload==(var)104)
		{
			Obstacle_Save_Obstacles_obm(temp_str);
			wait_for(Obstacle_Save_Obstacles_obm);
		}
	else if (filemenu_whattoload==(var)107)
		{
			MapEnt_Save_MapEnts_mem(temp_str);
			wait_for(MapEnt_Save_MapEnts_mem);
		}	
	else if (filemenu_whattoload==(var)108)
		{
			Grass_Save_Grasses_grm(temp_str);
			wait_for(Grass_Save_Grasses_grm);
		}
	else if (filemenu_whattoload==(var)109)
		{
			TerrMdl_Save_TerrMdls_tem(temp_str);
			wait_for(TerrMdl_Save_TerrMdls_tem);
		}
	else if (filemenu_whattoload==(var)110)
		{
			Building_Save_Buildings_blm(temp_str);
			wait_for(Building_Save_Buildings_blm);
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)120)
		{
			Sky_Save_Sky_sky(temp_str);
			wait_for(Sky_Save_Sky_sky);
		}
	else if (filemenu_whattoload==(var)121)
		{
			Materials_Save_Materials_mat(temp_str);
			wait_for(Materials_Save_Materials_mat);
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)130)
		{			
			FileMenu_Save_All(temp_str);
			wait_for(FileMenu_Save_All);
		}
	//---------------------------------------------
	// ... other saving function calls ...
	else if (filemenu_whattoload==(var)140)
		{
			TerEdit_Save_Hmp(temp_str);
			wait_for(TerEdit_Save_Hmp);
		}
	else if (filemenu_whattoload==(var)141)
		{
			TerrHmp_Save_Vertices_trv(temp_str);
			wait_for(TerrHmp_Save_Vertices_trv);
		}
	else if (filemenu_whattoload==(var)142)
		{			
			SaveWmb_Save_Wmb(temp_str);
			wait_for(SaveWmb_Save_Wmb);
		}
	else if (filemenu_whattoload==(var)143)
		{
			TerrHmp_Save_Skins_tga(temp_str);		// it handles both 24b and 32b skin sets !
			wait_for(TerrHmp_Save_Skins_tga);
		}
	else if (filemenu_whattoload==(var)145)
		{
			TerrHmp_Save_Skins_tga(temp_str);		// it handles both 24b and 32b skin sets now !
			wait_for(TerrHmp_Save_Skins_tga);	
		}
	else if (filemenu_whattoload==(var)146)
		{
			TerrHmp_Save_DetailMap24_dds(temp_str);
			wait_for(TerrHmp_Save_DetailMap24_dds);
		}
	else if (filemenu_whattoload==(var)147)
		{
			TerrHmp_Save_LightMap_tga(temp_str);
			wait_for(TerrHmp_Save_LightMap_tga);
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)149)
		{
			Scwater_Save_ScWater_scw(temp_str);
			wait_for(Scwater_Save_ScWater_scw);
		}
	else if (filemenu_whattoload==(var)150)
		{
			Mwater_Save_MWater_smw(temp_str);
			wait_for(Mwater_Save_MWater_smw);
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)153)
		{
{}
#ifndef MB_RELEASE				
			FormEdit_SaveFormation_for(temp_str);
			wait_for(FormEdit_SaveFormation_for);
#endif		
		}
	//---------------------------------------------
	else if (filemenu_whattoload==(var)154)
		{
			TerrHmp_Save_AutoTex_sat(temp_str);
			wait_for(TerrHmp_Save_AutoTex_sat);
		}
	else if (filemenu_whattoload==(var)155)
		{
			Scsm_Save_ssc(temp_str);
			wait_for(Scsm_Save_ssc);
		}
	else if (filemenu_whattoload==(var)156)
		{
			Pssm_Save_sps(temp_str);
			wait_for(Pssm_Save_sps);
		}
	//---------------------------------------------
	
	// ...
	
	//----------------------------------------------------------------------------------------------
	else
		{
			printf("filemenu_whattoload error %0.f", filemenu_whattoload);
		}
	
	//----------------------------------------------------------------------------------------------
	// close menu panels and switch back to UIMODE_FILEMENU i.e. file menu - if terrain is loaded it is done by its function !!!
	
	if ( (filemenu_whattoload != (var)1) && (filemenu_whattoload != (var)30) && (filemenu_whattoload != (var)42) )
		{
			FileMenu_Load_Exit();
		}
	
	//----------------------------------------------------------------------------------------------
	
	wait(3);
	ptr_remove(fname_str);
	ptr_remove(temp_str2);
	ptr_remove(temp_str);
	
	//----------------------------------------------------------------------------------------------
}


////////////////////////////////////////////////
// file names displayed


void	FileMenu_Load_IncSet()
{
//	wait(3);										// NO!!!
	
	// item based stepping
	filemenu_itemslider = clamp(filemenu_itemslider+1, 0, filemenu_last_item);					// update is done in panel while loop	
}


void	FileMenu_Load_DecSet()
{
//	wait(3);										// NO!!!
	
	// item based stepping
	filemenu_itemslider = clamp(filemenu_itemslider-1, 0, filemenu_last_item);					// update is done in panel while loop	
}


void	FileMenu_Load_ChangeSet()			//(button_number)
{
//	wait(3);										// NO!!!
	
	// cannot change if only 1 set exists
	if ((filemenu_last_item < (var)1) || (UIMode<(var)UIMODE_FILEMENU2))
		{
			return;
		}			
	
	// item based stepping
	int k = filemenu_actual_first_item;
	
	int i;  
	// refresh strings
	for(i=0; i<FILEMENU_MAXPERSET; i++)
		{
			str_cpy( filemenu_filenames[i] , "" );														// (or trr_foldername) subfolder
			str_cat( filemenu_filenames[i] , (filemenu_files_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi(filemenu_filenames[i],"") )													// (or trr_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( filemenu_filenames[i] , "-" );
				}
		}		
	// refresh 8 filename panel strings to display
	for(i=0; i<FILEMENU_MAXPERSET; i++)
		{
			pan_setstring(filemenu_load_panel, i+1,	17,24+2+i*25,	panel_font,			str_create( filemenu_filenames[i] ) );
		}	
}


////////////////////////////////////////////////
// input text


// 8 buttons behind file names of actual file set
void	FileMenu_Load_Select(button_number)	
{
	wait(3);
	
	filemenu_select = button_number;
	
	// set input text string
	str_cpy( (filemenu_input_txt->pstring)[0] , filemenu_filenames[button_number-1] );
	int j = str_len((filemenu_input_txt->pstring)[0]);					
	int i;
	for (i=0;i<filemenu_max_input_length-j;i++)					// to ensure 24 character length
		{
			str_cat((filemenu_input_txt->pstring)[0]," ");
		}
	//---------------	
//	// restart input text if necessary
//	if (proc_status(FileMenu_Load_Input_Loop) < (var)1)		// does not work !!!
//		{
//			FileMenu_Load_Input_Loop();								// check while loop			
//		}
	if (inkey_active < (var)1)
		{
			inkey((filemenu_input_txt->pstring)[0]);
		}
}


// button behind input text field
void	FileMenu_Load_Input_Button()
{
	wait(3);
	
	//---------------
	// clear name
	str_cpy( (filemenu_input_txt->pstring)[0] , "" );	
	int i;
	for (i=0;i<filemenu_max_input_length;i++)						// to ensure 24 character length
		{
			str_cat((filemenu_input_txt->pstring)[0]," ");
		}
	//---------------	
//	// restart input text if necessary
//	if (proc_status(FileMenu_Load_Input_Loop) < (var)1)
//		{
//			FileMenu_Load_Input_Loop();								// check while loop					
//		}	
	if (inkey_active < (var)1)
		{
			inkey((filemenu_input_txt->pstring)[0]);
		}
}


void	FileMenu_Load_Input_Init(STRING* filemenu_filter)
{
	// reset extension
	str_cpy( filemenu_extension , "" );
	// get extension
	STRING* temp_str = str_create("");
	str_cpy(temp_str,filemenu_filter);
	
	int temp0 = str_stri(temp_str,".");					// get . position starting with 1 or 0 if not found 	(10)
	int temp1 = str_len(temp_str); 						// get filter length												(13)
	int ext_length = temp1 - temp0 + 1;					// get extension length											(3+1)
	
	str_clip(temp_str,temp1-ext_length);				// clip all characters except the last 4	
	str_cpy( filemenu_extension , temp_str);			// store it for future operations
			
	// give content to input text string
	// if save  - current map name
	if ( (filemenu_whattoload>100) && (filemenu_whattoload<200) )
		{
			str_cpy( (filemenu_input_txt->pstring)[0], GetMapName() );					// or to avoid overwriting: "new_terrain"			
			str_cat( (filemenu_input_txt->pstring)[0], filemenu_extension );			// add it to input e.g. ".trr"
		}
	// if load	- copy 1st selected filename	
	else if ( (filemenu_whattoload>0) && (filemenu_whattoload<100) )
		{
			str_cpy( (filemenu_input_txt->pstring)[0], filemenu_filenames[0] );	
		}
	
	int j = str_len((filemenu_input_txt->pstring)[0]);					
	int i;
	for (i=0;i<filemenu_max_input_length-j;i++)							// to ensure 24 character length
		{
			str_cat((filemenu_input_txt->pstring)[0]," ");
		}
//	filemenu_input_txt.flags 	|= SHOW | SHADOW;
	inkey((filemenu_input_txt->pstring)[0]);
	
	if (proc_status(FileMenu_Load_Input_Loop) < (var)1)
		{
			FileMenu_Load_Input_Loop();								// check while loop					
		}
	
	ptr_remove(temp_str);
}


void	FileMenu_Load_Input_Loop()
{
	if (proc_status(FileMenu_Load_Input_Loop) > 0) return;
			
	while(filemenu_load_panel)
		{			
			if (key_enter) 											// watching ENTER pressing
				{
					while (key_enter) {wait (1);}					// to avoid multiple function calls - does not really okay somehow....
//					printf(_chr((filemenu_input_txt->pstring)[0]),0);
					// load
					FileMenu_Load_Load();
					break;												// without this break it causes pointer errors										
				}
				
			// check when to break the loop
			if (UIMode < (var)UIMODE_FILEMENU2)
				{
					break;
				}
						
			wait(1);
		}	
}


////////////////////////////////////////////////
// checkboxes


void		FileMenu_Load_ToggleImport()
{
	wait(3);
	
	filemenu_import = 1 - filemenu_import;
}


void		FileMenu_Load_ToggleWmbInfo()
{
	wait(3);
	
	filemenu_wmbinfo = 1 - filemenu_wmbinfo;
}


void		FileMenu_Load_ToggleShaderSet()
{
	wait(3);
	
	filemenu_shaderset = 1 - filemenu_shaderset;
}


///////////////////////////////////////////////////////////////////////////////////////////////


#endif