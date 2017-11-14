
#ifndef MBmap_c
#define MBmap_c

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


/////////////////////////////////////////////////////////////////////////////////////


void		Map_Init_NULL(char* file_name)
{
sys_marker("nul");

	while (proc_status(Map_DestroyMapData)>0) {wait(1);}
	
	//---------------------------------------------
	
	STRING* temp_mapname = str_create("");	
	str_cpy(temp_mapname, file_name);
	
	//---------------------------------------------
#ifndef MB_GAME	
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);	
	wait(3);
#endif	
	//------------------------------------------------------	
	
	map_wmb = 0;
	
	//------------------------------------------------------
	
	SetMapLevelName( "-" );
	SetMapGroundName( "-" ); 	
	SetMapWaterName( "-" ); 
	
	//------------------------------------------------------
	
	terrain_chunk = main_terrain_chunk;								// reset terrain chunk to default
	
	level_load(NULL);		
//	wait(1);																	// wait can be removed
	
	//------------------------------------------------------
	
	// entity counters
	Map_NullEntCounters();
	
	// temp camera pos for loading   
	Camera_SetDefaultPos();
	
	// common shadow creation
	Map_CreateShadowEnts();	
	
	//------------------------------------------------------
	
	str_clip(temp_mapname, str_len(trr_foldername) );			// cut subfolder
	str_trunc(temp_mapname, 4);										// cut extension	
	
	SetMapName(temp_mapname);
	
	TerrHmp_SetGroundMinToDefault();
	TerrHmp_SetGroundMaxToDefault();
	
	// define map size and build map data 
	SetMapDefaults();										
	wait_for(SetMapDefaults);	
	
	//------------------------------------------------------
	// set level entity (a pseudo entity) size - set earlier to default size
	
	vec_set( level_ent.max_x , TerrHmp_GetGroundMax() );
	vec_set( level_ent.min_x , TerrHmp_GetGroundMin() );
	
	// set level_ent size to default min if smaller - not needed due to new terrain min-max settings
//	SetLevelEntSizeToMin();
	
	level_ent.max_z += 1000;											// used as a top point of c-tracing in PosInfo3Dx functions
	
	//------------------------------------------------------
	
	// does map initialization - starts shadowmapping
	Map_BuildMapData();										
	wait_for(Map_BuildMapData);		
	
	//------------------------------------------------------
	
sys_marker(NULL);		
}


/////////////////////////////////////////////////////////////////////////////////////


void		Map_Init_hmp(char* TerrainFileName, char* WaterFileName, var created)				// 0 non-created when stored in .trr map file; 1 created in startup menu from 2 terrains
{	
sys_marker("hmp");		
	
	while (proc_status(Map_DestroyMapData)>0) {wait(1);}
	
	if (!TerrainFileName) return;
	if (!WaterFileName) return;
	
	//---------------------------------------------
	// needed to store it, it might be removed after calling this function
	
	STRING* temp_groundname = str_create("");	
	str_cpy(temp_groundname, TerrainFileName);
	
	STRING* temp_watername = str_create("");	
	str_cpy(temp_watername, WaterFileName);
	
	//---------------------------------------------
#ifndef MB_GAME	
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);	
	wait(3);
#endif	
	//---------------------------------------------
	
	STRING* temp_str = str_create("");	
	
	//---------------------------------------------
	
	map_wmb = 0;
	
	//---------------------------------------------
	// file names and initial map name	
	
	SetMapLevelName( "-" );
	
	if (created)	
		{		
			SetMapGroundName( temp_groundname ); 					// with path for main menu call or -
			SetMapWaterName( temp_watername ); 						// with path for main menu call or -			
		}
	
	TerrHmp_SetGroundMinToDefault();
	TerrHmp_SetGroundMaxToDefault();
	
	//---------------------------------------------
	
	terrain_chunk			= GetMapTerrainChunk();
	
	level_load(NULL);		
//	wait(1);																	// wait can be removed
	
	//---------------------------------------------
	// terrain entity creation
		
	TerrHmp_CreateGround(temp_groundname);
	wait_for(TerrHmp_CreateGround);
	
	TerrHmp_CreateWater(temp_watername);
	wait_for(TerrHmp_CreateWater);
	
	//---------------------------------------------
	
	// entity counters
	Map_NullEntCounters();
	
	// temp camera pos for loading   
	Camera_SetDefaultPos();
	
	// common shadow creation
	Map_CreateShadowEnts();	
	
	//------------------------------------------------------
	// unknown data (not via a .trr)
	
	if (created)																// created means not loaded from a .trr, thus map parameters are not set
		{
			// map name from hmp if created		
			str_for_entfile(temp_str, terrain_entity);				// read file name without path
			str_trunc(temp_str, 4);											// cut extension			
			SetMapName(temp_str);
			
			// define map size and build map data 
			SetMapDefaults();										
			wait_for(SetMapDefaults);	
		}
	
	//------------------------------------------------------
	
	// does map initialization - starts shadowmapping
	Map_BuildMapData();										
	wait_for(Map_BuildMapData);
	
	//------------------------------------------------------
		
	wait(3);
	ptr_remove(temp_str);
	ptr_remove(temp_groundname);
	ptr_remove(temp_watername);
	
sys_marker(NULL);	
}


/////////////////////////////////////////////////////////////////////////////////////


void		Map_Init_wmb(char* LevelFileName, var created)		// 0 non-created when stored in .trr map file; 1 created when an unknown wmb is loaded
{
sys_marker("wm0");
	
	while (proc_status(Map_DestroyMapData)>0) {wait(1);}
	
	if (!LevelFileName) return;
	
	//---------------------------------------------
	// needed to store it, it might be removed after calling this function
	
	STRING* temp_filename = str_create("");	
	str_cpy(temp_filename, LevelFileName);
	
	//---------------------------------------------
#ifndef MB_GAME	
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);	
	wait(3);
#endif		
	//------------------------------------------	
	
	STRING* temp_str = str_create("");				
	
	//------------------------------------------	
	
	map_wmb = 1;
	
	//------------------------------------------	
	// file names and initial map name	
				
	SetMapGroundName( "-" ); 	
	SetMapWaterName( "-" ); 
	
	if (created)	
		{	
			SetMapLevelName( temp_filename );	 					// with path for main menu call or -		
		}
	
	TerrHmp_SetGroundMinToDefault();
	TerrHmp_SetGroundMaxToDefault();
	
	//------------------------------------------	
	
	if ( !file_exists_onepath(temp_filename) )
		{
			Map_Init_NULL(map_emptyname);
			return;
		}

	//------------------------------------------	
	//******************************************
sys_marker("wm1");	
	
	if (filemenu_wmbinfo)												// always from startup, selectable from file menu
		{
			// try to load here presaved shadow system and video info
			
			str_cpy(temp_str, temp_filename);						// filename with path
			str_trunc(temp_str, 4);										// cut extension
			str_cat(temp_str, filemenu_wmbi_ext);					// add wmb info file extension
			
			var file_handle = file_open_read(temp_str);
			
			if (file_handle)
				{
					//---------------------------------
					// reset stencil buffer
					
					render_stencil = NULL; 	
					
					stencil_blur_value = 0;
					effect_load(mtl_shadow, NULL);			
					wait(3);
					
					//----------------------------------
					// set real values
					
					// shadow_stencil
					file_str_read( file_handle , temp_str );
					var temp_shadow_stencil = str_to_num( temp_str );
					
					// limit by edition
					if (main_engineversion < 1)
						{
							temp_shadow_stencil = clamp(temp_shadow_stencil,0,1);
						}
					
					// shadow
					shadow_stencil = temp_shadow_stencil;
					
					// blur
					file_str_read( file_handle , temp_str );
					main_blur = str_to_num( temp_str );
					
					// fog
					file_str_read( file_handle , temp_str );
					fog_color = str_to_num( temp_str );
					
#ifdef MB_GAME					
					// entity quantity
					if (file_str_read( file_handle , temp_str ) > 0)
						{
							filemenu_entinwmb = str_to_num( temp_str );															
						}
					else
						{
							filemenu_entinwmb = 0;
						}
#endif
					
					file_close(file_handle);
						
//					//---------------------------------
//					wait(3);				// test - because of more or less fixed engine bugs
//					level_load(NULL);	// eliminates some rare stencil buffer related issues (totally wrong stencil shadows),
//											// when loading a lot of levels after each other having different shadow modes,
//											// but there are some engine bugs when mirror views applied in a level
//											// AND stencill buffer is utilized (shadow_stencil 1 and 8),
//											// resulting not only in disapperring decal or stencil shadows,
//											// but even in terrain texture rendering onto screen instead of terrain !!! (shadow_stencil 8)
//											// which can be eliminated by using terraintexMB.fx fixed function effect
//											// instead of terrain shaders.
//											// apparently blurred stencil shadows do not like changing shadow mode... !!!
//					wait(3);				// test
//					//---------------------------------
				}
			else
				{
					// keep shadowm mode,
					// keep blur value
					fog_color = 1; // white by WED default
				}
		}
	//******************************************	
	//------------------------------------------	
	// load level
sys_marker("wm2");	
	
	terrain_chunk		= GetMapTerrainChunk();
	detail_size   		= GetMapGroundDetail();
	
	level_load(temp_filename);				
	
	//------------------------------------------	
sys_marker("wm3");	
		
	// entity counters
	Map_NullEntCounters();
	
	// temp camera pos for loading   
	Camera_SetDefaultPos();
	
	// common shadow creation
	Map_CreateShadowEnts();	
	
	//******************************************************
	//------------------------------------------------------
	// check for wmb info file, if exists it was created by mapbuilder, so it is possible to load settings from skills
	// if not, set only terrain actions	
sys_marker("wm4");	
	
	str_cpy(temp_str, temp_filename);						// filename with path
	str_trunc(temp_str, 4);										// cut extension
	str_cat(temp_str, filemenu_wmbi_ext);					// add wmb info file extension
	
	if (file_exists_onepath(temp_str))
		{
			ENTITY* temp_ent = ent_create( NULL , nullvector , NULL);
			
			temp_ent = ent_next(NULL); 				// get first entity
			
			int counter_int = 0; 						// for progress bar
			
			while (temp_ent) 								// repeat until there are no more entities
			{
				// terrains - assign only actions when they are unassigned - needed for terrain editing!
				if (ent_type(temp_ent) == (var)4)
					{
//						printf("terrain founc : %s", _chr(str_for_entfile(NULL, temp_ent)));
						
						if (is(temp_ent, PASSABLE))
							{
								wait(1);									
//								printf("Water found : %s", _chr(str_for_entfile(NULL, temp_ent)));
//								if (proc_status(TerrHmp_WaterAction) == (var)0)
									{
										my = temp_ent;				// must be set!	MB.2.31 fix
										TerrHmp_WaterAction();																
									}
							}
						else
							{
								wait(1);	
//								printf("Ground found : %s", _chr(str_for_entfile(NULL, temp_ent)));	
//								if (proc_status(TerrHmp_GroundAction) == (var)0)
									{
										my = temp_ent;				// must be set!	MB.2.31.fix
										TerrHmp_GroundAction();
									}
							}	
					}
				
				// needed to set if imported from WED. if it is from MB export, must be already set
				// FLAG8 entities are not saved into entity map files, cause they are already made by the wmb file !
				// this way a map can consist of a wmb file and some separate entity map files
				set(temp_ent, FLAG8); // set too at compilation
				
				// entity type counting to set counters properly - counters are nulled later in uihandlingclass, if not wmb
				if ( temp_ent.ENT_CATEGORY == (var)UIMODE_DETAIL )
				{
					detail_counter++;
				}
				else if ( temp_ent.ENT_CATEGORY == (var)UIMODE_SHRUB )
				{
					shrub_counter++;
				}
				else if ( temp_ent.ENT_CATEGORY == (var)UIMODE_SHRUB+(var)UIMODE_SHADOWSHIFT )
				{
					temp_ent.group = 1;
				}
				else if ( temp_ent.ENT_CATEGORY == (var)UIMODE_GRASS )
				{
					grass_counter++;
				}
				else if ( temp_ent.ENT_CATEGORY == (var)UIMODE_OBSTACLE )
				{
					obstacle_counter++;
					temp_ent.group = 3;
				}
				else if ( temp_ent.ENT_CATEGORY == (var)UIMODE_OBSTACLE+(var)UIMODE_SHADOWSHIFT )
				{
					temp_ent.group = 1;
				}
				else if ( temp_ent.ENT_CATEGORY == (var)UIMODE_MAPENT )
				{
					mapent_counter++;
					temp_ent.group = 3;
				}
				else if ( temp_ent.ENT_CATEGORY == (var)UIMODE_TERRMDL )
				{
					terrmdl_counter++;
					temp_ent.group = 4;
				}
				else if ( temp_ent.ENT_CATEGORY == (var)UIMODE_BUILDING )
				{
					building_counter++;
					temp_ent.group = 3;
				}
				else if ( temp_ent.ENT_CATEGORY == (var)UIMODE_BUILDING+(var)UIMODE_SHADOWSHIFT )
				{
					temp_ent.group = 1;
				}
				
				// set bounding box based on skills 1-6
				if ( temp_ent.ENT_CATEGORY < ((var)UIMODE_SHADOWSHIFT*2) )	// check type - okay for hidden entities too
				{
					Update_BBox(temp_ent);					
//					c_setminmax(temp_ent);
//					temp_ent.eflags |= FAT | NARROW; 													// probably a wait is needed !!!
//					if (temp_ent.MINX != (var)0)	temp_ent.min_x = temp_ent.MINX;				// skill1
//					if (temp_ent.MINY != (var)0)	temp_ent.min_y = temp_ent.MINY;
//					if (temp_ent.MINZ != (var)0)	temp_ent.min_z = temp_ent.MINZ;
//					if (temp_ent.MAXX != (var)0)	temp_ent.max_x = temp_ent.MAXX;
//					if (temp_ent.MAXY != (var)0)	temp_ent.max_y = temp_ent.MAXY;
//					if (temp_ent.MAXZ != (var)0)	temp_ent.max_z = temp_ent.MAXZ;				// skill6
					
					if ((temp_ent.MYALPHA>(var)0) && (temp_ent.MYALPHA<=(var)100)) temp_ent.alpha = temp_ent.MYALPHA;		// skill7		
					
					if (temp_ent.skill8 != (var)0) temp_ent.eflags |= CLIP0;				// skill8-9
					if (temp_ent.skill9 != (var)0) temp_ent.eflags |= CLIP1;
				}				
				
				// set tree shader - for lods too (shadow shift is used for lod models, must be compiled without invisible none!)
				if ( ((temp_ent.ENT_CATEGORY==(var)UIMODE_OBSTACLE) || (temp_ent.ENT_CATEGORY==((var)UIMODE_OBSTACLE+(var)UIMODE_SHADOWSHIFT)))
						&& 
						(is(temp_ent,FLAG7)) )	// check type - okay for hidden entities too
					{				
		//				fx_animTree();										// not needed !!! shader is set by material
						temp_ent.skill41 = floatv(10);						// speed x
						temp_ent.skill42 = floatv(25);						// speed	y
						temp_ent.skill43 = floatv(35);						//	oscillation	
						temp_ent.skill44 = floatv(temp_ent.max_z/2);				// bending position
						// set normals--------------------------------																
						// set material - needed one frame later than mtl_obstacle !!!
						Obstacle_SetTreeMaterial(temp_ent);					// set material(s) and normals 
						wait_for(Obstacle_SetTreeMaterial);									
					}
				
				// set decal shadow for non-shadow, non-collider entities
				if ( temp_ent.ENT_CATEGORY < (var)UIMODE_SHADOWSHIFT )				// check type - okay for unique and default decal shadows too
					{	
						if (temp_ent.string1)													// has a shadow entity (model or sprite) in string1
							{
								str_cpy( temp_str , temp_ent.string1 );					// read in entity string1 (which is a read only char*)				
								if (!str_cmpi( temp_str , "" ))								// has a valid string1
									{
										ENTITY* temp_shadowent = ent_create( NULL , nullvector , NULL);
										
										// look for created unique shadow entities
										temp_shadowent = ent_next(NULL);
										while (temp_shadowent) 																// repeat until there are no more entities
											{
												if (	
														(
															(temp_shadowent.ENT_CATEGORY >= (var)UIMODE_SHADOWSHIFT) 
															&& 
															( temp_shadowent.ENT_CATEGORY < ((var)UIMODE_SHADOWSHIFT*2) )
														)
														||
														(temp_shadowent.ENT_CATEGORY == (var)UIMODE_SHADOWENT) 
													)
													{												
														if ( str_cmpi( str_for_entfile(NULL,temp_shadowent) , temp_str ) )
															{
																if (
																		(!is(temp_ent,FLAG5))	
																		&&
																		(shadow_stencil > (var)-1)
																		&&
																		(shadow_stencil < (var)8)
																		&&
																		(main_blur == (var)0)								// incompatible with blur							
																	)
																	{													
																		Decal_Assign_DecalShadow(temp_ent, temp_shadowent);
																		break;
																	}
																else if (
																				(is(temp_ent,FLAG5))							// only if decal is enforced over shadowmapping														
																				||
																				(
																					(shadow_stencil > (var)-1)
																					&&
																					(shadow_stencil < (var)8)
																					&&
																					(main_blur > (var)0)						// blurred stencil shadows
																				)
																			)
																	{
																		Decal_Set_Decal(temp_ent, bmap_for_entity(temp_shadowent,0));	// ent_decal shadow, create decal from sprite entity bmap
																		reset(temp_ent,SHADOW);														// for safety
																		break;
																	}
															}
													}											
												temp_shadowent = ent_next(temp_shadowent);																								
											}	
										temp_shadowent = NULL;	
										ptr_remove(temp_shadowent);																															
									}
							}
					}				
				temp_ent = ent_next(temp_ent); 		// get next entity
				
#ifndef MB_GAME				
				// progress bar
				if (counter_int%100==0)		// at every 100th entity loading
					{
						if (proc_status(on_level_event) < (var)1)
							{
								Map_Progressbar(counter_int/num_entities*100);
								wait_for(Map_Progressbar);
								wait(1);
							}
					}
				counter_int++;
#endif	// MB_GAME				
			}	
		temp_ent = NULL;	
		ptr_remove(temp_ent);				
		}
	else
		{
			ENTITY* temp_ent = ent_create( NULL , nullvector , NULL);
			
			temp_ent = ent_next(NULL); 				// get first entity						
			
			while (temp_ent) 								// repeat until there are no more entities
			{
				// terrains - assign only actions when they are unassigned - needed for terrain editing!
				if (ent_type(temp_ent) == (var)4)
					{
//						printf("terrain found : %s", _chr(str_for_entfile(NULL, temp_ent)));
						
//						if (is(temp_ent, PASSABLE))													// does not work from WED !!!
//							{
//								wait(1);									
//								printf("Water found : %s", _chr(str_for_entfile(NULL, temp_ent)));
////								if (proc_status(TerrHmp_WaterAction) == (var)0)
//									{
//										my = temp_ent;			// must be set!	MB.2.31 fix
//										TerrHmp_WaterAction();																
//									}
//							}
//						else 
						if ( (!is(temp_ent, PASSABLE)) && (str_stri(str_for_entfile(NULL, temp_ent), "water") == (var)0) )
							{
								wait(1);	
//								printf("Ground found : %s", _chr(str_for_entfile(NULL, temp_ent)));	
//								if (proc_status(TerrHmp_GroundAction) == (var)0)
									{
										my = temp_ent;			// must be set!	MB.2.31.fix
										TerrHmp_GroundAction();
									}
							}
						else
							{
								wait(1);									
//								printf("Water found : %s", _chr(str_for_entfile(NULL, temp_ent)));
//								if (proc_status(TerrHmp_WaterAction) == (var)0)
									{
										my = temp_ent;			// must be set!	MB.2.31 fix
										TerrHmp_WaterAction();																
									}
							}	
					}
				
				temp_ent = ent_next(temp_ent); 		// get next entity
			}
			
			wait(1);
			temp_ent = NULL;	
			ptr_remove(temp_ent);
		}
	
#ifndef MB_GAME	
	wait(1);
	reset(map_loadbar,SHOW); 
#endif
		
	// enable non-terrain levels
	wait(3);
	if (!terrain_entity)
		{
			vec_set(terrhmp_vgroundmin, level_ent.min_x);
			vec_set(terrhmp_vgroundmax, level_ent.max_x);
		}
	
//	wait(3);	
//	printf("okay wmbload");
		
	//------------------------------------------------------
	//******************************************************
	// unknown data (not via a .trr)
	
	// *** empty pointer error here, on level creation only ***
	
	if (created)	
		{
sys_marker("wm5");	
//	printf("wm5");
				
			// map name from wmb if created (created means not set yet)
			str_cpy(temp_str, temp_filename);
			str_clip(temp_str, str_len(lev_foldername) );			// cut subfolder
			str_trunc(temp_str, 4);											// cut extension		
sys_marker("wm6");	
//	printf("wm6");				
			SetMapName(temp_str);
			
sys_marker("wm7");	
//	printf("wm7");
						
			// define map size and build map data (created means not called from load terrain)
			SetMapDefaults();											
			wait_for(SetMapDefaults);	
		}
	
	//------------------------------------------------------		
sys_marker("wm8");	
		
	// does map initialization - starts shadowmapping
	Map_BuildMapData();										
	wait_for(Map_BuildMapData);
	
	//------------------------------------------------------
sys_marker("wm9");	
		
	wait(3);
	ptr_remove(temp_str);
	ptr_remove(temp_filename);
	
sys_marker(NULL);	
}


/////////////////////////////////////////////////////////////////////////////////////


void Map_SaveMap_trr(char* file_name)
{
{}
#ifndef MB_GAME
	// save panel
	set( map_loadpanel , SHOW);
	set( map_savetext1 , SHOW);
	wait(1);
#endif
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_write(temp_str);
	//---------------------------------------------
	
	int old_map_maptiles = 0;
	
//	if ( GetMapTilesX() == GetMapTilesY() )
//		{
//			old_map_maptiles = GetMapTilesX();									// not needed, use the new approach for saving while keep compatibility at loading
//		}
	
	str_for_int( 			temp_str 	, old_map_maptiles );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_int( 			temp_str 	, GetMapZLevels() );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_int( 			temp_str 	, 0 );									// not needed - map_mapclusters
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_int( 			temp_str 	, GetMapTileSize() );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_int( 			temp_str 	, GetMapClusterSize() );
	file_str_write( 		file_handle , temp_str );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_num( 			temp_str 	, GetMapScaleValue() );				// var
	file_str_write( 		file_handle , temp_str );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_int( 			temp_str 	, GetMapDefaultTerrainType() );
	file_str_write(	 	file_handle , temp_str );	
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_int( 			temp_str 	, GetMapDefaultMoveCost() );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10); 
	//--------------------------------------------
	str_for_num( 			temp_str 	, GetMapGroundDetail() );			// var			
	file_str_write( 		file_handle , temp_str );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	file_str_write( 		file_handle , GetMapGroundName() );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	str_for_num( 			temp_str 	, GetMapWaterDetail() );			// var		
	file_str_write( 		file_handle , temp_str );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	file_str_write( 		file_handle , GetMapWaterName() );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	//----------------------------------------------
	file_str_write( 		file_handle , GetMapLevelName() );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	//----------------------------------------------	
	// old : not needed data, 11 ints:
	// now : 3 new data and 8 reserves
	
//	if ( GetMapTilesX() != GetMapTilesY() )									// not needed, compatibility is handled on loading only
		{
			str_for_int( 			temp_str 	, GetMapTilesX() );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			str_for_int( 			temp_str 	, GetMapTilesY() );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
		}
	
	str_for_num( 			temp_str 	, GetMapTerrainChunk() );			// var		
	file_str_write( 		file_handle , temp_str );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	//----------------------------------------------
	// reserve for further development
	
	int i;
	for(i=0; i<8; i++)																
		{
			str_for_int( 			temp_str 	, 0 );							// 0
			file_str_write( 		file_handle , temp_str );			
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
		}
	
//	//----------------------------------------------
//	// level_ent size - used when there is neither terrain nor wmb present in the level
//	
//	str_for_num( 			temp_str 	, level_ent.min_x );					// var		
//	file_str_write( 		file_handle , temp_str );			
//		file_asc_write(	file_handle , 13); 
//		file_asc_write(	file_handle , 10);
//	
//	str_for_num( 			temp_str 	, level_ent.min_y );					// var		
//	file_str_write( 		file_handle , temp_str );			
//		file_asc_write(	file_handle , 13); 
//		file_asc_write(	file_handle , 10);
//	
//	str_for_num( 			temp_str 	, level_ent.min_z );					// var		
//	file_str_write( 		file_handle , temp_str );			
//		file_asc_write(	file_handle , 13); 
//		file_asc_write(	file_handle , 10);
//	
//	str_for_num( 			temp_str 	, level_ent.max_x );					// var		
//	file_str_write( 		file_handle , temp_str );			
//		file_asc_write(	file_handle , 13); 
//		file_asc_write(	file_handle , 10);
//	
//	str_for_num( 			temp_str 	, level_ent.max_y );					// var		
//	file_str_write( 		file_handle , temp_str );			
//		file_asc_write(	file_handle , 13); 
//		file_asc_write(	file_handle , 10);
//	
//	str_for_num( 			temp_str 	, level_ent.max_z );					// var		
//	file_str_write( 		file_handle , temp_str );			
//		file_asc_write(	file_handle , 13); 
//		file_asc_write(	file_handle , 10);
//	
	//----------------------------------------------
	
	str_for_num( 			temp_str 	, GetMapGroundBlendFactor() );	// var
	file_str_write( 		file_handle , temp_str );			
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
		
	//----------------------------------------------
	file_close(file_handle);
	
//	printf( "Terrain Saved: %s" , _chr(file_name) );
	
#ifndef MB_GAME
	// save panel
	reset( map_loadpanel , SHOW);
	reset( map_savetext1 , SHOW);
#endif
		
	ptr_remove(temp_str);
}


/////////////////////////////////////////////////////////////////////////////////////


void Map_LoadMap_trr(char* file_name)
{
	if (!file_name)		return;
	
	//------------------------------------------------------
	
	STRING* temp_filename = str_create("");
	str_cpy(temp_filename, file_name);
	
	//------------------------------------------------------
{}
#ifndef MB_GAME
	// load panel
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);
	wait(3);
#endif
	
	//------------------------------------------------------
	
	var temp;
	int i;
	
	//------------------------------------------------------
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, temp_filename); 
	str_clip(temp_str, str_len(filemenu_foldername));					// cut off folder name and backslashes
	//str_trunc(temp_str, str_len(filemenu_extension));				// cut of extension and its preceding dot
	str_trunc(temp_str, 4);		
	SetMapName(temp_str);
	
	//------------------------------------------------------
	
	var file_handle = file_open_read(temp_filename);
		
	//------------------------------------------------------
	
	if (file_handle)
		{
			//----------------------------------------------
					
			map_wmb = 0;															// reset wmb flag			
			
			//----------------------------------------------
			// terrain size data			
			
			file_str_read( file_handle , temp_str );
			int old_map_maptiles = str_to_int( temp_str );
			
			if (old_map_maptiles > 0)
				{
					SetMapTilesXY( old_map_maptiles );						// to keep compatibility with old trr format
				}
	
			file_str_read( file_handle , temp_str );
			SetMapZLevels( str_to_int( temp_str ) );
		
			file_str_read( file_handle , temp_str );
//			map_mapclusters = str_to_int( temp_str );						// not needed, and not used any more
			
			file_str_read( file_handle , temp_str );
			SetMapTileSize( str_to_num( temp_str ) );
		
			file_str_read( file_handle , temp_str );
			SetMapClusterSize( str_to_int( temp_str ) );
		
			file_str_read( file_handle , temp_str );
			SetMapScaleValue( str_to_num( temp_str ) );
		
			file_str_read( file_handle , temp_str );
			SetMapDefaultTerrainType( str_to_int( temp_str ) );
			
			file_str_read( 	file_handle , temp_str );
			SetMapDefaultMoveCost( str_to_int( temp_str ) );
			
			//----------------------------------------------
			// ground and water terrain entity, and level data
			
			file_str_read( file_handle , temp_str );
			SetMapGroundDetail( str_to_num( temp_str ) );
		
			file_str_read( file_handle , temp_str );
			SetMapGroundName( temp_str );

			file_str_read( file_handle , temp_str );
			SetMapWaterDetail( str_to_num( temp_str ) );
			
			file_str_read( file_handle , temp_str );
			SetMapWaterName( temp_str );
		
			file_str_read( file_handle , temp_str );
			SetMapLevelName( temp_str );									
		
			//----------------------------------------------
			// not needed data - was 11 ints, now 3 used, 8 reserve
			
			if (old_map_maptiles == 0)									
				{
					file_str_read( 	file_handle , temp_str );
					SetMapTilesX( str_to_int( temp_str ) );
				
					file_str_read( 	file_handle , temp_str );
					SetMapTilesY( str_to_int( temp_str ) );
					
					file_str_read( 	file_handle , temp_str );
					if (str_to_num( temp_str ) > 0)
						{
							SetMapTerrainChunk( str_to_num( temp_str ) );
						}
					else
						{
							SetMapTerrainChunk( main_terrain_chunk );				// set to default if not saved
						}
					
					// reserves
					for(i=0; i<8; i++)												
						{
							file_str_read( file_handle , temp_str );		
						}
				}
			else
				{
					// reserves
					for(i=0; i<11; i++)													// 3 used + 8 reserves = 11 reserves in old version
						{
							file_str_read( file_handle , temp_str );		
						}
				}
			
//			//----------------------------------------------
//			// level_ent - its size can be set now, if there is a wmb or terrain, they will overwrite it according to thir size!
//			
//			file_str_read( file_handle , temp_str );
//			temp = str_to_num( temp_str );
//			
//			// new trr
//			if (temp != (var)0)
//				{
//					level_ent.min_x = temp;
//					
//					file_str_read( file_handle , temp_str );
//					level_ent.min_y = str_to_num( temp_str );
//					
//					file_str_read( file_handle , temp_str );
//					level_ent.min_z = str_to_num( temp_str );
//					
//					file_str_read( file_handle , temp_str );
//					level_ent.max_x = str_to_num( temp_str );
//					
//					file_str_read( file_handle , temp_str );
//					level_ent.max_y = str_to_num( temp_str );
//					
//					file_str_read( file_handle , temp_str );
//					level_ent.max_z = str_to_num( temp_str );
//				}
//			else
//				{
//					for(i=0; i<6; i++)													// 3 used + 2 reserves = 5 reserves in old version
//						{
//							file_str_read( file_handle , temp_str );		
//						}
//				}
//			
			//----------------------------------------------
			// terrain shader base colormap - blended tiled textures blend factor
			
			if ( file_str_read( file_handle , temp_str ) > (var)-1)			// not eof
				{
					SetMapGroundBlendFactor( str_to_num( temp_str ) );
				}
	
			//----------------------------------------------
			
			file_close(file_handle);
			
			//----------------------------------------------
			// create the level and the map
			
			if (!str_cmpi(GetMapLevelName(), "-"))
				{
					Map_Init_wmb(GetMapLevelName(), (var)0);
					wait_for(Map_Init_wmb);
				}
			else
			if ( (!str_cmpi(GetMapGroundName(), "-")) || (!str_cmpi(GetMapWaterName(), "-")) )
				{
//					printf( "HMP Map Loading: %s" , _chr(file_name) );
//					printf( "HMP ground: %s" , _chr(GetMapGroundName()) );
//					printf( "HMP water: %s" , _chr(GetMapWaterName()) );
					
					Map_Init_hmp(GetMapGroundName(), GetMapWaterName(), (var)0);
					wait_for(Map_Init_hmp);
				}			
			else
				{
					Map_Init_NULL(temp_filename);								
					wait_for(Map_Init_NULL);
				}
		
			//--------------------------------------
			
			//printf( "Map Loaded: %s" , _chr(file_name) );
		}
	else
		{
{}			
#ifndef MB_GAME			
			printf( "Load Error: %s" , _chr(temp_filename) );
#endif			
		}
	
	//------------------------------------------------------
	
#ifndef MB_GAME	
	// load panel - handled by Map_BuildData()
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
#endif	
	
	//------------------------------------------------------
	
	ptr_remove(temp_str);
	
	//------------------------------------------------------
}


/////////////////////////////////////////////////////////////////////////////////////


void		Map_BuildMapData()
{
sys_marker("bmd");	
	//-----------------------------------------------------------
{}	
#ifndef MB_GAME
	
	str_cpy( (map_loadmap_text7.pstring)[0] , "" );						// clear this info text
	
	//-----------------------------------------------------------	
	// loading panel's bar should be updated among function calls
	//-----------------------------------------------------------
	
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);	
	wait(3);
	
	teredit_raiselowerflatten = 0;											// required by a .hmp saving error caused by totally flattened terrains - probably not needed anymore !
#endif
	
	//-----------------------------------------------------------
	// set visual properties to defaults
		
	Materials_ShadersInit();													// material effects
	wait(1);
		
	Sky_DefaultInit();															// default sky settings with a sky cube image and default camera, sun, lod, fog settings
	wait_for(Sky_DefaultInit);	
	wait(1);
	
#ifndef MB_RELEASE	
	play02_map_tilesize	= GetMapTileSize();								// variables read by RTS debug shaders
	play02_map_maptilesx	= (var)GetMapTilesX();
	play02_map_maptilesy	= (var)GetMapTilesY();
#endif
	
	if (main_engineversion>0)
		{								
			if (shadow_stencil==(var)8)										// pssm shadows
				{
					Pssm_Init();
				}
			else if (shadow_stencil==(var)-1)								// scsm shadows
				{
					Scsm_Start();			
				}
		}
	
	wait(5);
	
	//-----------------------------------------------------------
	// start Editor Menu system
	
#ifndef MB_GAME
	
	UIMode = (var)UIMODE_FILEMENU;											// required by camera loop and FileMenu_Init()
	
	Camera_Init();																	// start default camera
	wait_for(Camera_Init);
	
	UI_GuiPosInfo_Read();														// set last panel positions
	wait_for(UI_GuiPosInfo_Read);
	
	FileMenu_Init();																// by default start menu with File Menu
	
	UI_Init();																		// start user interface, hotkeys and mouse clicks		
	
	UI_DrawD3DGrid();																// start grid loop
	
#endif
		
	//-----------------------------------------------------------
	
#ifndef MB_GAME
	wait(3);
	
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
	
	reset( map_loadmap_text1 , SHOW);
	reset( map_loadmap_text2 , SHOW);
	reset( map_loadmap_text3 , SHOW);
	reset( map_loadmap_text3skip , SHOW);
	reset( map_loadmap_text4 , SHOW);	
	reset( map_loadmap_text4skip , SHOW);
	reset( map_loadmap_text5 , SHOW);
	reset( map_loadmap_text6 , SHOW);	
#endif
	
	//-----------------------------------------------------------
sys_marker(NULL);
}


/////////////////////////////////////////////////////////////////////////////////////



void		Map_DestroyMapData()
{
	//------------------------------------------------------------------------------------
{}
#ifndef MB_GAME
	set( map_loadpanel , SHOW);
	//	set( map_loadtext1 , SHOW);
	//	printf( "destroy: start" );
	
	// store panel positions
	UI_GuiPosInfo_Write();
	wait_for(UI_GuiPosInfo_Write);
#endif		
	
	//------------------------------------------------------------------------------------
	// stop loops
	
	UIMode = (var)UIMODE_NEWMAP;
	wait(5);	
	
	terrain_chunk = main_terrain_chunk;						// reset terrain chunk to default
	
	map_wmb = 0;													// reset wmb level flag
	
#ifndef MB_GAME	
	Camera_Close();												// close camera system
#endif
	
	//------------------------------------------------------------------------------------
	// remove entity shaders
	
	Materials_ShadersRemove();
	
	//------------------------------------------------------------------------------------
	// stop shadowmapping
	
	if (shadow_stencil==(var)8)
		{
			Pssm_Close();
			wait_for(Pssm_Close);									
		}
	else if (shadow_stencil==(var)-1)
		{
			Scsm_Close();
			wait_for(Scsm_Close);
		}
	
	//------------------------------------------------------------------------------------
	// terrain, sky entities
	
	TerrHmp_RemoveGround();
	wait_for(TerrHmp_RemoveGround);
	
	TerrHmp_RemoveWater();
	wait_for(TerrHmp_RemoveWater);
	
	Sky_RemoveSkies();
	wait_for(Sky_RemoveSkies);
	
	Map_RemoveShadowEnts();
	wait_for(Map_RemoveShadowEnts);	
	
	//------------------------------------------------------------------------------------
	// RTS map decomposition
	
#ifndef MB_RELEASE	
	Terrain_StaticFree();
	wait_for(Terrain_StaticFree);
#endif
	
	//------------------------------------------------------------------------------------
	
	wait(1);
}


/////////////////////////////////////////////////////////////////////////////////////


void		Map_CreateShadowEnts()
{
	if (ent_unitshadow_rot == NULL)		ent_unitshadow_rot = ent_create("shadows\\0dot_shadow.dds",nullvector, Decal_ShadowAction);
	if (ent_treeshadow_fix == NULL)		ent_treeshadow_fix = ent_create("shadows\\0tree_shadow.dds",nullvector, Decal_ShadowAction);
	if (ent_model_unitshadow == NULL)	ent_model_unitshadow = ent_create("shadows\\0cylinder_shadow.mdl",nullvector, Decal_ShadowAction);
}


void		Map_RemoveShadowEnts()
{
	if (ent_unitshadow_rot!=NULL)     
		{
			ptr_remove(ent_unitshadow_rot);		
		}
	ent_unitshadow_rot = NULL;
	
	if (ent_treeshadow_fix!=NULL)     
		{
			ptr_remove(ent_treeshadow_fix);		
		}
	ent_treeshadow_fix = NULL;
	
	if (ent_model_unitshadow!=NULL)     
		{
			ptr_remove(ent_model_unitshadow);		
		}
	ent_model_unitshadow = NULL;
}


/////////////////////////////////////////////////////////////////////////////////////


void		Map_NullEntCounters()
{
	detail_counter 	= 0;
	shrub_counter 		= 0;
	grass_counter 		= 0;
	obstacle_counter 	= 0;
	mapent_counter 	= 0;
	terrmdl_counter 	= 0;
	building_counter 	= 0;
}


/////////////////////////////////////////////////////////////////////////////////////


void		Map_Progressbar(percent)
{
	set(map_loadbar,SHOW | LIGHT);   																// switch load bar on
	vec_set(map_loadbar.blue,vector(225,200,200));												// set own light color
	map_loadbar.pos_x = 0;
	map_loadbar.pos_y = screen_size.y - 32;
	map_loadbar.size_x = (screen_size.x%100) + integer(screen_size.x/100)*percent;   // 24..1024 pixels
	map_loadbar.size_y = 32;
}


void		Map_Progressbar_Hide()
{
	reset(map_loadbar, SHOW); 
}


////////////////////////////////////////////////////////////////////


#ifndef MB_GAME

void			Map_NexusCheck()
{		
//	return;	// disable temporarily for nexus overflow testing
{}
#ifndef A8	
	if (max_nexus - nexus < 10000)					// kB	: A7
		{
			set( ui_nexustext , SHOW );			
			wait(3);
			wait(-1);
			reset( ui_nexustext , SHOW );
//			FileMenu_Reload_Map();
		}	
#else
	if (compatibility == (var)13)						// MB -> kB : similarly to A7
		{
			if (1024*max_nexus - nexus < 10000)
				{
					set( ui_nexustext , SHOW );			
					wait(3);
					wait(-1);
					reset( ui_nexustext , SHOW );
//					FileMenu_Reload_Map();
				}
		}
	else														// MB	: A8 default
		{
			if (max_nexus - nexus < 10)
				{
					set( ui_nexustext , SHOW );
					wait(3);
					wait(-1);
					reset( ui_nexustext , SHOW );
//					FileMenu_Reload_Map();
				}	
		}
#endif
}

#endif


////////////////////////////////////////////////////////////////////


#endif