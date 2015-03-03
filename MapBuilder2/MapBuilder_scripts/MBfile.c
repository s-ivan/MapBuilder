
#ifndef MBfile_c
#define MBfile_c


///////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////
// complete map loading, saving and reloading //
////////////////////////////////////////////////


void		FileMenu_QuickSave()
{
	STRING* temp_str = str_create("");
	str_cpy( temp_str , filemenu_foldername );
	str_cat( temp_str , "temp.trr" );
	
	FileMenu_Save_All(temp_str);
	wait_for(FileMenu_Save_All);
	
	wait(3);
	ptr_remove(temp_str);
}


void		FileMenu_Reload_Map()
{
	STRING* temp_str = str_create("");
	str_cpy( temp_str , filemenu_foldername );
	str_cat( temp_str , "temp.trr" );
	
	FileMenu_Save_All(temp_str);
	wait_for(FileMenu_Save_All);
	
	FileMenu_Load_All(temp_str);			// no wait_for can be applied for load all
	wait_for(FileMenu_Load_All);
	
	wait(3);
	ptr_remove(temp_str);
}


////////////////////////////////////////////////


void		FileMenu_Save_All(char* file_name)						// e.g. "temp.trr" with path!!!
{
	STRING* temp_str = str_create("");
//	str_cpy( temp_str , filemenu_foldername );
//	str_cat( temp_str , file_name );
	str_cpy( temp_str , file_name );
	//-----------------------------------------
	//---------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_terrains_ext );
	file_remove_onepath(temp_str);									// delete existing file with similar name (to avoid i.e unwanted old lightmap loading, terrain deformation etc.)	
	Map_SaveMap_trr(temp_str);
		wait_for(Map_SaveMap_trr);
	//-----------------------------------------
	//-----------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_terrverts_ext );	
	file_remove_onepath(temp_str);									// delete existing file with similar name (to avoid i.e unwanted old lightmap loading, terrain deformation etc.)	
	TerrHmp_Save_Vertices_trv(temp_str);
		wait_for(TerrHmp_Save_Vertices_trv);	
	//-----------------------------------------
//	str_trunc(temp_str,4);												// NO !!! useful images can be lost!
//	int i;
//	for (i=1; i<10; i++)													// _skin1-9.tga , clolormap + 8 blendmaps (but default shader can handle only max 7, the 8 blending shader requires compound tile textures incompatible with terrain texture editor currently)
//		{
//			str_cat(temp_str, "_skin");							
//			str_cat(temp_str, str_for_int(NULL, i));			
//			str_cat(temp_str, ".tga");
//			
//			file_remove_onepath(temp_str);							// delete existing file with similar name (to avoid i.e unwanted old lightmap loading, terrain deformation etc.)	
//			
//			str_trunc(temp_str, 10);
//		}	
	//-----------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_lightmaps_ext );	
	file_remove_onepath(temp_str);									// delete existing file with similar name (to avoid i.e unwanted old lightmap loading, terrain deformation etc.)	
	TerrHmp_Save_LightMap_tga(temp_str);
		wait_for(TerrHmp_Save_LightMap_tga);
	//-----------------------------------------
	//-----------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_terrmdls_ext );	
	TerrMdl_Save_TerrMdls_tem(temp_str);
		wait_for(TerrMdl_Save_TerrMdls_tem);
	//-----------------------------------------
	//-----------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_details_ext );	
	Detail_Save_Details_grs(temp_str);
		wait_for(Detail_Save_Details_grs);
	//-----------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_shrubs_ext );
	Shrub_Save_Shrubs_shs(temp_str);
		wait_for(Shrub_Save_Shrubs_shs);
	//-----------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_grasses_ext );	
	Grass_Save_Grasses_grm(temp_str);
		wait_for(Grass_Save_Grasses_grm);
	//-----------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_obstacles_ext );	
	Obstacle_Save_Obstacles_obm(temp_str);
		wait_for(Obstacle_Save_Obstacles_obm);
	//-----------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_mapents_ext );	
	MapEnt_Save_MapEnts_mem(temp_str);
		wait_for(MapEnt_Save_MapEnts_mem);	
	//-----------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_buildings_ext );	
	Building_Save_Buildings_blm(temp_str);
		wait_for(Building_Save_Buildings_blm);		
	//-----------------------------------------
	//---------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_materials_ext );
	Materials_Save_Materials_mat(temp_str);
		wait_for(Materials_Save_Materials_mat);
	//-----------------------------------------
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_skies_ext );
	Sky_Save_Sky_sky(temp_str);
		wait_for(Sky_Save_Sky_sky);
	//-----------------------------------------
//	//-----------------------------------------		
//	str_trunc(temp_str,4);
//	str_cat( temp_str , filemenu_mwater_ext );
//	if (main_engineversion > (var)0)
//		{		
//			if (main_water == (var)0)
//				{						
//					Mwater_Save_MWater_smw(temp_str);
//					wait_for(Mwater_Save_MWater_smw);						
//				}
//		}
//	//-----------------------------------------	
//	str_trunc(temp_str,4);
//	str_cat( temp_str , filemenu_scwater_ext );
//	if (main_engineversion > (var)0)
//		{
//			if (main_water > (var)0)
//				{
//					Scwater_Save_ScWater_scw(temp_str);
//					wait_for(Scwater_Save_ScWater_scw);						
//				}
//		}
	//-----------------------------------------	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_scsm_ext );
//	if (main_engineversion > (var)0)
		{
//			if (shadow_stencil == (var)-1)
				{
					Scsm_Save_ssc(temp_str);
					wait_for(Scsm_Save_ssc);						
				}
		}
	//-----------------------------------------	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_pssm_ext );
//	if (main_engineversion > (var)0)
		{
//			if (shadow_stencil == (var)8)
				{
					Pssm_Save_sps(temp_str);
					wait_for(Pssm_Save_sps);						
				}
		}
	//-----------------------------------------
	
	ptr_remove(temp_str);
}


////////////////////////////////////////////////


#ifndef MB_GAME

void		FileMenu_Load_All(char* file_name)																// e.g. "temp.trr" with path!!!
{
	//-----------------------------------------
	// protection
	
	if ( !file_exists_onepath(file_name) )
		{
			return;
		}
	
	//-----------------------------------------		
	// store file name locally to avoid empty pointer error when parameter string is removed
	
	STRING* temp_str = str_create("");
	str_cpy( temp_str , file_name );
	
	//-----------------------------------------
	
	Map_DestroyMapData();			
	wait_for(Map_DestroyMapData);
	wait(3);			
	
	//-----------------------------------------
	// display info text  what file is loaded currently
	
	set(map_loadmap_text7,SHOW);
	str_cpy( (map_loadmap_text7.pstring)[0] , "" );														// editor only
	
	//-----------------------------------------
	//-----------------------------------------
	// trr
	
//	str_trunc(temp_str,4);																						// not needed
//	str_cat( temp_str , filemenu_terrains_ext );
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Creating terrain..." );					// editor only
			
			Map_LoadMap_trr(temp_str);		
			wait_for(Map_LoadMap_trr);
		}
//	while(UIMode>=(var)UIMODE_NEWMAP) { wait(1); }														// not needed - ensuring closing editor loops, editor only
	
	//-----------------------------------------
	//-----------------------------------------
	
//	var surface_update_needed = 0;
	
	//-----------------------------------------
	// trv
		
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_terrverts_ext );	
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Processing terrain mesh..." );			// editor only
			
			TerrHmp_Load_Vertices_trv(temp_str);			
			wait_for(TerrHmp_Load_Vertices_trv);
//			surface_update_needed = 1;																			// need to call surface update later
		}
	
	//-----------------------------------------	
	// ground textures
	
	if (terrain_entity)
		{
			if (ent_status(terrain_entity,8)>2)
				{			
					// multiskin
					STRING* temp2_str = str_create("");
					str_cpy(temp2_str,temp_str);
					str_trunc(temp2_str,4);
					str_cat(temp2_str,"_skin1.tga");
					if (file_exists_onepath(temp_str))
						{
							str_cpy( (map_loadmap_text7.pstring)[0] , "Replacing terrain skin textures..." );	// editor only		
//							wait(1);
							
							TerrHmp_Load_Skins_tga(temp2_str);															// mod to enable calls out of terrain editing mode
							wait_for(TerrHmp_Load_Skins_tga);
						}
					ptr_remove(temp2_str);
				}
			else
				{			
					// single skin								
					STRING* temp2_str = str_create("");
					str_cpy(temp2_str,temp_str);
					str_trunc(temp2_str,4);
					str_cat(temp2_str,"_skin1.tga");																				// must be saved as _skin1 !!!
					if (file_exists_onepath(temp2_str))
						{
							str_cpy( (map_loadmap_text7.pstring)[0] , "Replacing terrain textures..." );			// editor only		
//							wait(1);
							
							TerrHmp_Load_Skin24_tga(temp2_str);
							wait_for(TerrHmp_Load_Skin24_tga);
						}
					// detail map
					str_trunc(temp_str,4);
					str_cat( temp_str , filemenu_detailtexture_ext );
					if ( file_exists_onepath(temp_str) )
						{
							TerrHmp_Load_DetailMap24_dds(temp_str);
							wait_for(TerrHmp_Load_DetailMap24_dds);
						}
					ptr_remove(temp2_str);
				}
		}
	//-----------------------------------------
	// ground lightmap
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_lightmaps_ext );
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Loading terrain lightmap..." );								// editor only	
//			wait(1);
			
			TerrHmp_Load_LightMap_tga(temp_str);
			wait_for(TerrHmp_Load_LightMap_tga);		
		}
	
	//-----------------------------------------
	//-----------------------------------------
	// terrmdl
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_terrmdls_ext );															// terrmdl should be here (after terrain) because of the following surface checks
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Creating terrain models..." );			// editor only	
			
			TerrMdl_Load_TerrMdls_tem(temp_str, 0);			
			wait_for(TerrMdl_Load_TerrMdls_tem);	
//			surface_update_needed = 1;																			// need to call surface update later
		}
	
//	//-----------------------------------------
//	// surface update - long, moved, done only if needed
//	
//	if (surface_update_needed)																					// call only once surface update because it is very long (moved to battle init or save/load to/from file)
//		{
//			str_cpy( (map_loadmap_text7.pstring)[0] , "Updating terrain surface normals..." );				// editor only		
//			wait(1);
//
//			Terrain_StaticCreate();
//			wait_for(Terrain_StaticCreate);
//		}
//
	//-----------------------------------------
	//-----------------------------------------
	// details
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_details_ext );	
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Creating grass sprites..." );								// editor only	
					
			Detail_Load_Details_grs(temp_str, 0);
			wait_for(Detail_Load_Details_grs);
		}
	
	//-----------------------------------------
	// shrubs
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_shrubs_ext );
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Creating shrubs..." );											// editor only	
			
			Shrub_Load_Shrubs_shs(temp_str, 0);
			wait_for(Shrub_Load_Shrubs_shs);
		}
		
	//-----------------------------------------
	// grasses
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_grasses_ext );
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Creating grass models..." );									// editor only	
			
			Grass_Load_Grasses_grm(temp_str, 0);
			wait_for(Grass_Load_Grasses_grm);
		}
	
	//-----------------------------------------
	// obstacles
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_obstacles_ext );	
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Creating obstacle models..." );								// editor only	
				
			Obstacle_Load_Obstacles_obm(temp_str, 0);
			wait_for(Obstacle_Load_Obstacles_obm);
		}
	
	//-----------------------------------------
	// mapents
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_mapents_ext );	
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Creating map entities..." );									// editor only	
				
			MapEnt_Load_MapEnts_mem(temp_str, 0);
			wait_for(MapEnt_Load_MapEnts_mem);	
		}
	
	//-----------------------------------------
	// buildings
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_buildings_ext );	
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Creating building models..." );								// editor only	
				
			Building_Load_Buildings_blm(temp_str, 0);
			wait_for(Building_Load_Buildings_blm);
		}
	
	//-----------------------------------------
	//-----------------------------------------
	// materials
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_materials_ext );
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Setting materials..." );										// editor only	
//			wait(1);
			
			Materials_Load_Materials_mat(temp_str);
			wait_for(Materials_Load_Materials_mat);
		}
	
	//-----------------------------------------
	// sky
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_skies_ext );
	if ( file_exists_onepath(temp_str) )
		{
			str_cpy( (map_loadmap_text7.pstring)[0] , "Processing sun, sky, fog, lod, and camera..." );		// editor only		
					
			Sky_Load_Sky_sky(temp_str);																							// should be here because pssm/scsm restarted !!!
			wait_for(Sky_Load_Sky_sky);
		}
	
	//-----------------------------------------
	//-----------------------------------------
	// terrain auto texturing shader data (not saved with save all)
	
	str_trunc(temp_str,4);
	str_cat( temp_str , filemenu_terrautotex_ext );
	if ( file_exists_onepath(temp_str) )
		{
			TerrHmp_Load_AutoTex_sat(temp_str);
			wait_for(TerrHmp_Load_AutoTex_sat);
		}
	
	//-----------------------------------------
	// water shader (not saved with save all)
	
	if (main_engineversion > (var)0)
		{		
			if (main_water == (var)0)
				{
					str_trunc(temp_str,4);
					str_cat( temp_str , filemenu_mwater_ext );
					if ( file_exists_onepath(temp_str) )
						{
							str_cpy( (map_loadmap_text7.pstring)[0] , "Setting water shader..." );					// editor only	
//							wait(1);
							
							Mwater_Load_MWater_smw(temp_str);
							wait_for(Mwater_Load_MWater_smw);
						}
				}
			else if (main_water > (var)0)
				{
					str_trunc(temp_str,4);
					str_cat( temp_str , filemenu_scwater_ext );
					if ( file_exists_onepath(temp_str) )
						{
							str_cpy( (map_loadmap_text7.pstring)[0] , "Setting water shader..." );					// editor only	
//							wait(1);
							
							Scwater_Load_ScWater_scw(temp_str);
							wait_for(Scwater_Load_ScWater_scw);
						}
				}
		}
	
	//-----------------------------------------
	// shadowmapping shader (always saved with save all)
	
//	if (main_engineversion > (var)0)
		{		
//			if (shadow_stencil == (var)-1)
				{
					str_trunc(temp_str,4);
					str_cat( temp_str , filemenu_scsm_ext );
					if ( file_exists_onepath(temp_str) )
						{
							str_cpy( (map_loadmap_text7.pstring)[0] , "Setting shadowmapping..." );					// editor only	
//							wait(1);
							
							Scsm_Load_ssc(temp_str);
							wait_for(Scsm_Load_ssc);
						}
				}
//			else if (shadow_stencil == (var)8)
				{
					str_trunc(temp_str,4);
					str_cat( temp_str , filemenu_pssm_ext );
					if ( file_exists_onepath(temp_str) )
						{
							str_cpy( (map_loadmap_text7.pstring)[0] , "Setting shadowmapping..." );					// editor only	
//							wait(1);
							
							Pssm_Load_sps(temp_str);
							wait_for(Pssm_Load_sps);
						}
				}
		}
	
	//-----------------------------------------
	//-----------------------------------------
	
	reset(map_loadmap_text7,SHOW);																								// editor only	
	str_cpy( (map_loadmap_text7.pstring)[0] , "" );	
	
	//-----------------------------------------
	
	ptr_remove(temp_str);
	
	//-----------------------------------------
}

#else		// MB_GAME

void		FileMenu_Load_All(char* file_name)										// e.g. "temp.trr" with path!!!
{
	//-----------------------------------------
	// protection
	
	if ( !file_exists_onepath(file_name) )
		{
			return;
		}
	
	//-----------------------------------------
	
	Map_DestroyMapData();			
	wait_for(Map_DestroyMapData);
	wait(3);			
	
	//-----------------------------------------
	// overall map progressbar															// game only
	
	// null entity counters	
	filemenu_entquantity = 0;
	filemenu_entcounter = 0;
	filemenu_entinwmb = 0;
	
	filemenu_maploadstate = 1;															// >0 while loading
		
	if (filemenu_progressbar)															// show only if non-zero
		{
			FileMenu_Progressbar(file_name);											// of overall map, but wmb can't be estimated (max 3000 ents)
		}
	
	//-----------------------------------------	
			
	STRING* temp_str = str_create("");
	str_cpy( temp_str , file_name );
	
	//-----------------------------------------
	//-----------------------------------------
	// trr
	
//	str_trunc(temp_str,4);																// not needed
//	str_cat( temp_str , filemenu_terrains_ext );
	if ( file_exists_onepath(temp_str) )
		{
			++filemenu_maploadstate;													// game only
			
			Map_LoadMap_trr(temp_str);
			wait_for(Map_LoadMap_trr);
			
			if (map_wmb)																	// game only
				{																
					filemenu_entquantity += filemenu_entinwmb;
				}
		}
//	while(UIMode>=(var)UIMODE_NEWMAP) { wait(1); }								// not needed
	
	//-----------------------------------------
	//-----------------------------------------
	
//	var surface_update_needed = 0;
	
	//-----------------------------------------
	// trv
	
	if (map_load_terrdeform)
		{	
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_terrverts_ext );	
			if ( file_exists_onepath(temp_str) )
				{
					++filemenu_maploadstate;											// game only
					
					TerrHmp_Load_Vertices_trv(temp_str);			
					wait_for(TerrHmp_Load_Vertices_trv);
//					surface_update_needed = 1;											// it would need to call surface update...		
				}
		}
	
	//-----------------------------------------
	// ground texture
		
	if (map_load_terrtext)
		{
			if (terrain_entity)
				{
					if (ent_status(terrain_entity,8)>2)
						{			
							// multiskin
							STRING* temp2_str = str_create("");
							str_cpy(temp2_str,temp_str);
							str_trunc(temp2_str,4);
							str_cat(temp2_str,"_skin1.tga");
							if (file_exists_onepath(temp_str))
								{
									++filemenu_maploadstate;							// game only
									
									TerrHmp_Load_Skins_tga(temp2_str);
									wait_for(TerrHmp_Load_Skins_tga);
								}
							ptr_remove(temp2_str);
						}
					else
						{			
							// single skin			
							STRING* temp2_str = str_create("");
							str_cpy(temp2_str,temp_str);
							str_trunc(temp2_str,4);
							str_cat(temp2_str,"_skin1.tga");							// must be saved as _skin1 !!!
							if (file_exists_onepath(temp2_str))
								{
									++filemenu_maploadstate;							// game only
									
									TerrHmp_Load_Skin24_tga(temp2_str);
									wait_for(TerrHmp_Load_Skin24_tga);
								}
							// detail map
							str_trunc(temp_str,4);
							str_cat( temp_str , filemenu_detailtexture_ext );
							if ( file_exists_onepath(temp_str) )
								{
									TerrHmp_Load_DetailMap24_dds(temp_str);
									wait_for(TerrHmp_Load_DetailMap24_dds);
								}
							ptr_remove(temp2_str);
						}
				}
		}
	
	//-----------------------------------------
	// ground lightmap
	
	if (map_load_terrlm)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_lightmaps_ext );
			if ( file_exists_onepath(temp_str) )
				{
					++filemenu_maploadstate;											// game only
					
					TerrHmp_Load_LightMap_tga(temp_str);
					wait_for(TerrHmp_Load_LightMap_tga);		
				}
		}
	
	//-----------------------------------------
	//-----------------------------------------
	// terrmdls
	
	if (map_load_terrmdls)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_terrmdls_ext );	
			if ( file_exists_onepath(temp_str) )
				{
					TerrMdl_Load_TerrMdls_tem(temp_str, 0);			
					wait_for(TerrMdl_Load_TerrMdls_tem);	
//					surface_update_needed = 1;											// it would need to call surface update...
				}
		}
	
	//-----------------------------------------
	//-----------------------------------------
	// details
	
	if (map_load_details)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_details_ext );	
			if ( file_exists_onepath(temp_str) )
				{
					Detail_Load_Details_grs(temp_str, 0);
					wait_for(Detail_Load_Details_grs);
					wait(1);
				}
		}

	//-----------------------------------------
	// shrubs
	
	if (map_load_shrubs)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_shrubs_ext );
			if ( file_exists_onepath(temp_str) )
				{
					Shrub_Load_Shrubs_shs(temp_str, 0);
					wait_for(Shrub_Load_Shrubs_shs);
					wait(1);
				}
		}
	
	//-----------------------------------------
	// grasses
	
	if (map_load_grass)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_grasses_ext );
			if ( file_exists_onepath(temp_str) )
				{
					Grass_Load_Grasses_grm(temp_str, 0);
					wait_for(Grass_Load_Grasses_grm);
					wait(1);
				}
		}

	//-----------------------------------------
	// obstacles
	
	if (map_load_obstacles)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_obstacles_ext );	
			if ( file_exists_onepath(temp_str) )
				{
					Obstacle_Load_Obstacles_obm(temp_str, 0);
					wait_for(Obstacle_Load_Obstacles_obm);
					wait(1);
				}
		}
	
	//-----------------------------------------
	// mapents
	
	if (map_load_mapents)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_mapents_ext );	
			if ( file_exists_onepath(temp_str) )
				{
					MapEnt_Load_MapEnts_mem(temp_str, 0);
					wait_for(MapEnt_Load_MapEnts_mem);	
					wait(1);
				}
		}
	
	//-----------------------------------------
	// buildings
	
	if (map_load_buildings)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_buildings_ext );	
			if ( file_exists_onepath(temp_str) )
				{	
					Building_Load_Buildings_blm(temp_str, 0);
					wait_for(Building_Load_Buildings_blm);
					wait(1);
				}
		}
	
	//-----------------------------------------
	//-----------------------------------------
	// materials
	
	if (map_load_materials)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_materials_ext );
			if ( file_exists_onepath(temp_str) )
				{
					Materials_Load_Materials_mat(temp_str);
					wait_for(Materials_Load_Materials_mat);
				}
		}
	
	//-----------------------------------------
	// sky
	
	if (map_load_sky)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_skies_ext );
			if ( file_exists_onepath(temp_str) )
				{
					filemenu_maploadstate += 10;										// game only
					
					Sky_Load_Sky_sky(temp_str);
					wait_for(Sky_Load_Sky_sky);
				}
		}
	
	//-----------------------------------------
	//-----------------------------------------
	// terrain auto texturing shader data (not saved with save all)
	
	if (map_load_materials)
		{
			str_trunc(temp_str,4);
			str_cat( temp_str , filemenu_terrautotex_ext );
			if ( file_exists_onepath(temp_str) )
				{
					TerrHmp_Load_AutoTex_sat(temp_str);
					wait_for(TerrHmp_Load_AutoTex_sat);
				}
		}
	
	//-----------------------------------------
	// water shaders (not saved with save all)
	
	if (map_load_watershaders)
		{
			if (main_engineversion > (var)0)
				{		
					if (main_water == (var)0)
						{
							str_trunc(temp_str,4);
							str_cat( temp_str , filemenu_mwater_ext );
							if ( file_exists_onepath(temp_str) )
								{
									Mwater_Load_MWater_smw(temp_str);
									wait_for(Mwater_Load_MWater_smw);
								}
						}
					else if (main_water > (var)0)
						{
							str_trunc(temp_str,4);
							str_cat( temp_str , filemenu_scwater_ext );
							if ( file_exists_onepath(temp_str) )
								{
									Scwater_Load_ScWater_scw(temp_str);
									wait_for(Scwater_Load_ScWater_scw);
								}
						}
				}
		}
	
	//-----------------------------------------
	// shadowmapping shader (always saved with save all)
	
	if (map_load_sky)
		{
		//	if (main_engineversion > (var)0)
				{		
		//			if (shadow_stencil == (var)-1)
						{
							str_trunc(temp_str,4);
							str_cat( temp_str , filemenu_scsm_ext );
							if ( file_exists_onepath(temp_str) )
								{
									Scsm_Load_ssc(temp_str);
									wait_for(Scsm_Load_ssc);
								}
						}
		//			else if (shadow_stencil == (var)8)
						{
							str_trunc(temp_str,4);
							str_cat( temp_str , filemenu_pssm_ext );
							if ( file_exists_onepath(temp_str) )
								{
									Pssm_Load_sps(temp_str);
									wait_for(Pssm_Load_sps);
								}
						}
				}
		}
	
	//-----------------------------------------
	//-----------------------------------------
	
	ptr_remove(temp_str);
	
	//-----------------------------------------
	
	filemenu_maploadstate = 0;															// game only
	
	//-----------------------------------------
}

#endif	// MB_GAME


////////////////////////////////////////////////


void		FileMenu_Load_trr(char* file_name)	
{
	Map_DestroyMapData();			
	wait_for(Map_DestroyMapData);		
	wait(3);
	
	Map_LoadMap_trr(file_name);	
	wait_for(Map_LoadMap_trr);	
}


void		FileMenu_Load_wmb(char* file_name)	
{
{}
#ifdef MB_GAME
	filemenu_entquantity = 0;
	filemenu_entcounter = 0;
	filemenu_entinwmb = 0;
#endif

	//------------------------------------	
	
	Map_DestroyMapData();			
	wait_for(Map_DestroyMapData);
	wait(3);							
	
	Map_Init_wmb( file_name , 1 );														// mapinit.c, 1 to create new
	wait_for(Map_Init_wmb);
}


///////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// reading summed entity quantity of a full map from different entity map files //
// to be used in game progressbar															  //
//////////////////////////////////////////////////////////////////////////////////


#ifdef MB_GAME

void	FileMenu_Entity_SetQuantity(STRING* file_name)
{
	filemenu_entquantity = 100;	// base value
	
	// a wmb entity counter would be needed... 
	// but not so simple, stored in .i file of the .wmb but its name is stored in the .trr
	// so this is added after level load separately
	
	// entity maps
	if (map_load_details)	filemenu_entquantity += FileMenu_Entity_ReadQuantity(file_name, filemenu_details_ext);
	if (map_load_shrubs)		filemenu_entquantity += FileMenu_Entity_ReadQuantity(file_name, filemenu_shrubs_ext);
	if (map_load_grass)		filemenu_entquantity += FileMenu_Entity_ReadQuantity(file_name, filemenu_grasses_ext);
	if (map_load_obstacles)	filemenu_entquantity += FileMenu_Entity_ReadQuantity(file_name, filemenu_obstacles_ext);
	if (map_load_mapents)	filemenu_entquantity += FileMenu_Entity_ReadQuantity(file_name, filemenu_mapents_ext);
	if (map_load_terrmdls)	filemenu_entquantity += FileMenu_Entity_ReadQuantity(file_name, filemenu_terrmdls_ext);
	if (map_load_buildings)	filemenu_entquantity += FileMenu_Entity_ReadQuantity(file_name, filemenu_buildings_ext);
	
	// other maps
	STRING* temp_str = str_create(file_name);
	
	// trv
	str_trunc(temp_str, 4);
	str_cat( temp_str , filemenu_terrverts_ext );	
	if ( file_exists_onepath(temp_str) )
		filemenu_entquantity += 100;
		
	// texture(s)		
	STRING* temp2_str = str_create("");
	str_cpy(temp2_str,temp_str);
	str_trunc(temp2_str,4);
	str_cat(temp2_str,"_skin1.tga");
	if ( file_exists_onepath(temp2_str) )
		filemenu_entquantity += 100;
	
	// lightmap
	str_trunc(temp_str, 4);
	str_cat( temp_str , filemenu_lightmaps_ext );	
	if ( file_exists_onepath(temp_str) )
		filemenu_entquantity += 100;
	
	// sky
	str_trunc(temp_str, 4);
	str_cat( temp_str , filemenu_skies_ext );	
	if ( file_exists_onepath(temp_str) )
		filemenu_entquantity += 1000;
}


var	FileMenu_Entity_ReadQuantity(STRING* file_name, STRING* extension)
{
	var ents_to_load = 0;
	
	STRING* temp_str = str_create(file_name);
	str_trunc(temp_str, 4);
	str_cat(temp_str, extension);
	
	var file_handle = file_open_read(temp_str);	
	if (file_handle)
		{
			if (file_str_read(file_handle, temp_str) > 0)
				{
					ents_to_load = str_to_num(temp_str);
					if (ents_to_load < 0)
						{
							ents_to_load = 0;
						}
				}
			
			file_close(file_handle);
		}
	
	ptr_remove(temp_str);
	
	return(ents_to_load);
}


void	FileMenu_Entity_UpdateCounter()
{
	// entities created
	filemenu_entcounter = detail_counter + shrub_counter + grass_counter + obstacle_counter + mapent_counter + terrmdl_counter + building_counter;
	
	// trr, trv, texture, lightmap, and sky
	filemenu_entcounter += 100 * filemenu_maploadstate;		
}


void	FileMenu_Progressbar(STRING* temp_str)
{
	// instead of wait_for() it is possible to display a simple progress bar 
	// by using some helper functions and global variables of MapBuilder
	// you can customize it to display a better quality one
	
	FileMenu_Entity_SetQuantity(temp_str);
	wait_for(FileMenu_Entity_SetQuantity);
	
//	printf("quantity %0.f", (double)filemenu_entquantity);	// okay
	
	while(filemenu_maploadstate)	// proc_status of Load_All cannot be used here because it is also called from Load_All thus it would result 0
		{
			proc_mode = PROC_LATE;
			
			FileMenu_Entity_UpdateCounter();
			Map_Progressbar(filemenu_entcounter/filemenu_entquantity*100);
			
//			DEBUG_VAR(filemenu_entquantity, 200);
//			DEBUG_VAR(filemenu_entcounter, 225);
			
			wait(1);
		}
	Map_Progressbar_Hide();
}

#endif	// MB_GAME


///////////////////////////////////////////////////////////////////////////////////////////////


#endif