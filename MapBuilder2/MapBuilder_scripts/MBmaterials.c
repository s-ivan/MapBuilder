
#ifndef MBmaterials_c
#define MBmaterials_c

///////////////////////////////////////////////////////////////////////////
// code


void		Materials_ShadersInit()
{	
sys_marker("shi");

{}	
#ifndef MB_GAME	
	
	set(terrain_multi_mat, 	AUTORELOAD);
	set(terrain_single_mat, AUTORELOAD);
	
	set(mtl_obstacle, AUTORELOAD);
	set(mtl_tree, 		AUTORELOAD);
	set(mtl_mapent, 	AUTORELOAD);
	set(mtl_terrmdl, 	AUTORELOAD);
	set(mtl_building, AUTORELOAD);
	
	set(mtl_model, 	AUTORELOAD);
	set(mtl_flat, 		AUTORELOAD);
	set(mtl_shaded, 	AUTORELOAD);
	
#endif
	
	//-------------------------------
	
	if (main_engineversion>0)
		{
			if (main_blur)
				{
//					stencil_blur(main_blur); 		// activate blurred shadows        (included in mtlView.c, look better)
//					mtl_shadow.skill1 = floatv(main_blur);
					stencil_blur_value = main_blur * 4;
					effect_load(mtl_shadow, "st_stencilBlur.fx");
				}
			
			if (main_hdr == (var)3)
				{
					mtl_hdr.skill1 = floatv(5);	// strength
					mtl_hdr.skill2 = floatv(35);	// treshold
					mtl_hdr.skill3 = floatv(0); 	// exposure compensation factor
					mtl_hdr.skill4 = floatv(1);	// time
					pp_set(camera,mtl_hdr);			//*** camera was NULL
				}	
			else if (main_hdr == (var)2)
				{
					mtl_bloomblur.skill11 = 1.75;							// blur strength that tweaks brighness dependent blur strength : 0 no change
					mtl_bloomblur.skill12 = 1.25;							// lower limit 0-3 : 0 no limit, 3 add bloom to all pixels (less dynamic appearance)
					mtl_bloomblur.skill13 = 2.00;							// dynamic brightness (contrast) : 0 no change, 		
					mtl_bloomblur.skill14 = 0.35;							// 1 no change, static brightness   (for overall brightness corrigation, less dynamic appearance)
					
					pp_set(camera, mtl_bloomblur);
				}
			else if (main_hdr == (var)1)
				{
					mtl_bloom.skill12 = 1.25;							// lower limit 0-3 : 0 no limit, 3 add bloom to all pixels (less dynamic appearance)
					mtl_bloom.skill13 = 2.00;							// dynamic brightness (contrast) : 0 no change, 		
					mtl_bloom.skill14 = 0.35;							// 1 no change, static brightness   (for overall brightness corrigation, less dynamic appearance)
					
					pp_set(camera, mtl_bloom);
				}
			
			if (shadow_stencil > (var)-1)
				{				
					effect_load(mtl_detail		,"obj_animSprite1_MB.fx");
					effect_load(mtl_shrub		,"obj_animShrub1_MB.fx");
					effect_load(mtl_grass		,"obj_animGrass1_MB.fx");
					effect_load(mtl_obstacle	,"obj_model_MB.fx");
					effect_load(mtl_tree			,"obj_animTree1_MB.fx");
					if (shadow_stencil == (var)8)
						{
							effect_load(mtl_mapent		,"obj_level_MB.fx");					// no lightmap, only for PSSM
						}
					else
						{
							effect_load(mtl_mapent,		NULL);
						}
					effect_load(mtl_terrmdl		,"obj_model_MB.fx");
					effect_load(mtl_building	,"obj_model_MB.fx");
					
					effect_load(mtl_model		,"obj_model_MB.fx");
					effect_load(mtl_sprite		,"obj_animSprite1_MB.fx");
					if (shadow_stencil == (var)8)
						{
							effect_load(mtl_flat			,"obj_level_MB.fx");					// no lightmap, only for PSSM
							effect_load(mtl_shaded		,"obj_level_MB.fx");					// no lightmap, only for PSSM
						}
					else
						{
							effect_load(mtl_flat,		NULL);
							effect_load(mtl_shaded,		NULL);
						}
					effect_load(mtl_sky			,		NULL);
					
//					mtl_tree.flags &= ~ENABLE_TREE;
//					mtl_tree.event = NULL;
					
					// RTS unit materials
					effect_load(mtl_bonesanim	,"obj_bones_MB.fx");		// FIL		// no pixel shader
				}
			else //if (shadow_stencil == (var)-1)
				{
					// MB materials got shader to have self shadows
					effect_load(mtl_detail		,"sc_obj_sprite_MB.fx");				// obj_animGrass1_MB (no shadow response, bad with height fog) or sc_obj_sprite_MB
					effect_load(mtl_shrub		,"sc_obj_sprite_MB.fx");
					effect_load(mtl_grass 		,"sc_obj_grass1_MB.fx");
					effect_load(mtl_obstacle	,"sc_obj_model_MB.fx");
					effect_load(mtl_tree			,"sc_obj_tree1_MB.fx");
					effect_load(mtl_mapent		,"sc_obj_level_MB.fx");
					effect_load(mtl_terrmdl		,"sc_obj_model_MB.fx");
					effect_load(mtl_building	,"sc_obj_model_MB.fx");
					
					// 3DGS materials can be used to not to have self shadow
//					effect_load(mtl_terrain		,"sc_obj_model_MB.fx");					// not used, in action own shader
					effect_load(mtl_model		,"sc_obj_model_MB.fx");
					effect_load(mtl_sprite		,"sc_obj_sprite_st_MB.fx");	
					effect_load(mtl_flat			,"sc_obj_level_MB.fx");
					effect_load(mtl_shaded		,"sc_obj_level_MB.fx");
					effect_load(mtl_sky			,"sc_obj_sky_MB.fx");
					
//					mtl_tree.flags |= ENABLE_TREE;											// ENABLE_RENDER or ENABLE_TREE
//					mtl_tree.event = sc_tree_billboard_event;
					
					// RTS unit materials 
{}
#ifdef PLAY02_PS
					effect_load(mtl_bonesanim	,"sc_obj_bones_MB.fx");	// FIL	 	// slower SM3.0 shader with pixel shader - capability of player colour display
#else
					effect_load(mtl_bonesanim	,"obj_bones_MB.fx");		// FIL		// no pixel shader
#endif					
				}						
		
			Materials_ShadersLoop();
		}
}


void		Materials_ShadersLoop()
{
	while(1)
		{
			// slightly faster to set here material skills than by material events
			
			mtl_tree->skill1 = floatv( sinv(camera->pan) );
			mtl_tree->skill2 = floatv( cosv(camera->pan) );			
			
			wait(1);
		}
}


void		Materials_ShadersRemove()
{
	if (main_engineversion > 0) 
		{
			proc_kill2(Materials_ShadersLoop, NULL);
			
//			if (main_blur)
				{
					effect_load(mtl_shadow, NULL);
					stencil_blur_value = 0;	
//					wait(3);
				}			
			
			if (main_hdr)
				{
					pp_set(camera, NULL);	
//					wait(3);
				}
			
//			mtl_tree.flags &= ~ENABLE_TREE;
//			mtl_tree.event = NULL;
				
			if ((shadow_stencil == (var)-1) || (shadow_stencil == (var)8))
				{
					// MB materials got shader to have self shadows
					effect_load(mtl_detail		,NULL);					// obj_animGrass1_MB (no shadow response, bad with height fog) or sc_obj_sprite_MB
					effect_load(mtl_shrub		,NULL);
					effect_load(mtl_grass 		,NULL);
					effect_load(mtl_obstacle	,NULL);
					effect_load(mtl_tree			,NULL);
					effect_load(mtl_mapent		,NULL);
					effect_load(mtl_terrmdl		,NULL);
					effect_load(mtl_building	,NULL);
					
					// 3DGS materials can be used to not to have self shadow
		//			effect_load(mtl_terrain		,NULL);
					effect_load(mtl_model		,NULL);
					effect_load(mtl_sprite		,NULL);
					effect_load(mtl_flat			,NULL);
					effect_load(mtl_shaded		,NULL);
					effect_load(mtl_sky			,NULL);
					
					// terrain is done in terrain action !!! because of material skin settings
		//			effect_load(terrain_multi_mat		,NULL);
		//			effect_load(terrain_single_mat	,NULL);						
					
					// RTS unit material
					effect_load(mtl_bonesanim	,NULL);
				}			
		}	
}

///////////////////////////////////////////////////////////////////////////


void 		Materials_Save_Materials_mat(char* file_name)
{
	var temp_var = 0;
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_write(temp_str);
	//---------------------------------------------
	if (file_handle)
		{
			//mat_terrain		
			Materials_Save_Materials_Any( mat_terrain , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mat_sprite
			Materials_Save_Materials_Any( mat_sprite , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mat_model
			Materials_Save_Materials_Any( mat_model , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mat_unlit
			Materials_Save_Materials_Any( mat_unlit , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mat_shadow
			Materials_Save_Materials_Any( mat_shadow , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mat_flat
			Materials_Save_Materials_Any( mat_flat , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mat_shaded
			Materials_Save_Materials_Any( mat_shaded , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mat_metal
			Materials_Save_Materials_Any( mat_metal , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mat_sky
			Materials_Save_Materials_Any( mat_sky , file_handle );
			wait_for(Materials_Save_Materials_Any);
			
			//terrain
			if (terrain_entity)
				{
					Materials_Save_Materials_Any( terrain_entity.material , file_handle );
				}
			else
				{
					Materials_Save_Materials_Any( terrain_multi_mat , file_handle );
				}
			wait_for(Materials_Save_Materials_Any);
			//water
			if (water_entity)
				{
					Materials_Save_Materials_Any( water_entity.material , file_handle );
				}
			else
				{
					Materials_Save_Materials_Any( water_multi_mat , file_handle );
				}
			wait_for(Materials_Save_Materials_Any);
			//mtl_detail
			Materials_Save_Materials_Any( mtl_detail , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mtl_shrub
			Materials_Save_Materials_Any( mtl_shrub , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mtl_grass
			Materials_Save_Materials_Any( mtl_grass , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mtl_obstacle (mtl_obstacle_transp removed)
			Materials_Save_Materials_Any( mtl_obstacle , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mtl_mapent
			Materials_Save_Materials_Any( mtl_mapent , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mtl_terrmdl
			Materials_Save_Materials_Any( mtl_terrmdl , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mtl_tree
			Materials_Save_Materials_Any( mtl_tree , file_handle );
			wait_for(Materials_Save_Materials_Any);
			//mtl_building
			Materials_Save_Materials_Any( mtl_building , file_handle );
			wait_for(Materials_Save_Materials_Any);
//			//mtl_unit
//			Materials_Save_Materials_Any( mtl_unit , file_handle );
//			wait_for(Materials_Save_Materials_Any);
			
			file_close(file_handle);
		}
	
//	printf( "Material Settings Saved: %s" , _chr(file_name) );	
	
	//----------------------------------------------
	// shader set file
	
	if ((filemenu_shaderset) && (main_engineversion > (var)0))
		{
			str_cpy(temp_str, file_name);			
			str_trunc(temp_str, str_len(filemenu_materials_ext));
			str_cat(temp_str, filemenu_shaderset_ext);
			
			file_handle = file_open_write(temp_str);
			
			if (file_handle)
				{
					// shadow_stencil
					File_Write_Asc(file_handle, str_for_num(NULL, shadow_stencil));
					
					// system
					Materials_Save_Shader_Any(mat_terrain, file_handle);
					Materials_Save_Shader_Any(mat_sprite, file_handle);
					Materials_Save_Shader_Any(mat_model, file_handle);
					Materials_Save_Shader_Any(mat_unlit, file_handle);
					Materials_Save_Shader_Any(mat_shadow, file_handle);
					Materials_Save_Shader_Any(mat_flat, file_handle);
					Materials_Save_Shader_Any(mat_shaded, file_handle);
					Materials_Save_Shader_Any(mat_metal, file_handle);
					Materials_Save_Shader_Any(mat_sky, file_handle);					
					
					// reserve
					File_Write_Asc(file_handle, "-");
					File_Write_Asc(file_handle, "-");
					File_Write_Asc(file_handle, "-");
					
					// custom
					if (terrain_entity)
						{
							Materials_Save_Shader_Any( terrain_entity.material , file_handle );
						}
					else
						{
							Materials_Save_Shader_Any( terrain_multi_mat , file_handle );
						}
					if (water_entity)
						{
							Materials_Save_Shader_Any( water_entity.material , file_handle );
						}
					else
						{
							Materials_Save_Shader_Any( water_multi_mat , file_handle );
						}
					Materials_Save_Shader_Any(mtl_detail, file_handle);
					Materials_Save_Shader_Any(mtl_shrub, file_handle);
					Materials_Save_Shader_Any(mtl_grass, file_handle);
					Materials_Save_Shader_Any(mtl_obstacle, file_handle);
					Materials_Save_Shader_Any(mtl_mapent, file_handle);
					Materials_Save_Shader_Any(mtl_terrmdl, file_handle);
					Materials_Save_Shader_Any(mtl_tree, file_handle);
					Materials_Save_Shader_Any(mtl_building, file_handle);
					
					// reserve
					File_Write_Asc(file_handle, "-");
					File_Write_Asc(file_handle, "-");
					File_Write_Asc(file_handle, "-");
					File_Write_Asc(file_handle, "-");
					
					file_close(file_handle);
				}						
		}
	
	//----------------------------------------------
	
	ptr_remove(temp_str);
}


void		Materials_Save_Materials_Any(MATERIAL* mat_to_save , var file_handle)
{
	file_var_write( file_handle , mat_to_save.ambient_red );
	file_var_write( file_handle , mat_to_save.ambient_green );
	file_var_write( file_handle , mat_to_save.ambient_blue );
	
	file_var_write( file_handle , mat_to_save.diffuse_red );
	file_var_write( file_handle , mat_to_save.diffuse_green );
	file_var_write( file_handle , mat_to_save.diffuse_blue );
	
	file_var_write( file_handle , mat_to_save.emissive_red );
	file_var_write( file_handle , mat_to_save.emissive_green );
	file_var_write( file_handle , mat_to_save.emissive_blue );
	
	file_var_write( file_handle , mat_to_save.specular_red );
	file_var_write( file_handle , mat_to_save.specular_green );
	file_var_write( file_handle , mat_to_save.specular_blue );
	
	file_var_write( file_handle , mat_to_save.power );
	file_var_write( file_handle , mat_to_save.albedo );
	
	file_asc_write(	file_handle , 13); 
	file_asc_write(	file_handle , 10);
}


void		Materials_Save_Shader_Any(MATERIAL* material_actual , var file_handle)
{
	STRING* temp_str = str_create("");
	
	if (material_actual.effect != NULL)
		{
			str_cpy(temp_str, material_actual.effect);
			
			File_Write_Asc(file_handle, temp_str);
		}
	else
		{
			File_Write_Asc(file_handle, "-");	// NULL or "" results in wrong reading!!!
		}
	ptr_remove(temp_str);
}


///////////////////////////////////////////////////////////////////////////


void		Materials_Load_Materials_mat(char* file_name)
{
{}
#ifndef MB_GAME
	// load panel
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);
	wait(3);
#endif	// MB_GAME
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	var temp;
	//---------------------------------------------
	var file_handle = file_open_read(temp_str);	
	//---------------------------------------------
	if (file_handle)
		{
			//mat_terrain		
			Materials_Load_Materials_Any( mat_terrain , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mat_sprite
			Materials_Load_Materials_Any( mat_sprite , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mat_model
			Materials_Load_Materials_Any( mat_model , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mat_unlit
			Materials_Load_Materials_Any( mat_unlit , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mat_shadow
			Materials_Load_Materials_Any( mat_shadow , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mat_flat
			Materials_Load_Materials_Any( mat_flat , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mat_shaded
			Materials_Load_Materials_Any( mat_shaded , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mat_metal
			Materials_Load_Materials_Any( mat_metal , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mat_sky
			Materials_Load_Materials_Any( mat_sky , file_handle );
			wait_for(Materials_Load_Materials_Any);
			
//			printf( "Basic materials loaded");
			
			//terrain
			if (terrain_entity)
				{
					Materials_Load_Materials_Any( terrain_entity.material , file_handle );
					wait_for(Materials_Load_Materials_Any);			
						// copy to single_mat and multi_mat
						Materials_Load_Materials_Pair( terrain_entity.material , terrain_multi_mat );	
						Materials_Load_Materials_Pair( terrain_entity.material , terrain_single_mat );	
				}
			else
				{
					Materials_Load_Materials_Any( terrain_multi_mat , file_handle );
					wait_for(Materials_Load_Materials_Any);			
						// copy to single_mat
						Materials_Load_Materials_Pair( terrain_multi_mat , terrain_single_mat );	
				}											
			//water
			if (water_entity)
				{
					Materials_Load_Materials_Any( water_entity.material , file_handle );
					wait_for(Materials_Load_Materials_Any);			
						// copy to single_mat and multi_mat
						Materials_Load_Materials_Pair( water_entity.material , water_multi_mat );	
						Materials_Load_Materials_Pair( water_entity.material , water_single_mat );	
				}
			else
				{
					Materials_Load_Materials_Any( water_multi_mat , file_handle );
					wait_for(Materials_Load_Materials_Any);			
						// copy to terrain_single_mat
						Materials_Load_Materials_Pair( water_multi_mat , water_single_mat );	
				}	
			
//			printf( "Terrain / Water materials loaded");
						
			//mtl_detail
			Materials_Load_Materials_Any( mtl_detail , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mtl_shrub
			Materials_Load_Materials_Any( mtl_shrub , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mtl_grass
			Materials_Load_Materials_Any( mtl_grass , file_handle );
			wait_for(Materials_Load_Materials_Any);
//			//mtl_bush
//			Materials_Load_Materials_Any( mtl_bush , file_handle );
//			wait_for(Materials_Load_Materials_Any);			
			//mtl_obstacle , mtl_obstacle_transp (same)
			Materials_Load_Materials_Any( mtl_obstacle , file_handle );
			wait_for(Materials_Load_Materials_Any);
//			Materials_Load_Materials_Pair( mtl_obstacle , mtl_obstacle_transp );	// no real loading just set them equal - temp before A8.30
			//mtl_mapent
			Materials_Load_Materials_Any( mtl_mapent , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mtl_terrmdl
			Materials_Load_Materials_Any( mtl_terrmdl , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mtl_tree
			Materials_Load_Materials_Any( mtl_tree , file_handle );
			wait_for(Materials_Load_Materials_Any);
			//mtl_building
			Materials_Load_Materials_Any( mtl_building , file_handle );
			wait_for(Materials_Load_Materials_Any);
//			//mtl_unit
//			Materials_Load_Materials_Any( mtl_unit , file_handle );
//			wait_for(Materials_Load_Materials_Any);
			
//			printf( "MB materials loaded");
			
			file_close(file_handle);
		}
	
//	printf( "Material Settings Loaded: %s" , _chr(file_name) );
	
	//----------------------------------------------
	// shader set file
	
	file_handle = 0;
	if ((filemenu_shaderset) && (main_engineversion > (var)0))
		{
			str_cpy(temp_str, file_name);			
			str_trunc(temp_str, str_len(filemenu_materials_ext));
			str_cat(temp_str, filemenu_shaderset_ext);
			
			if (file_exists_onepath(temp_str))
				{				
					file_handle = file_open_read(temp_str);
					
					if (file_handle)
						{
							// shadow_stencil
							File_Read_Asc(file_handle, temp_str);
							
							// only allowed to be loaded when current shadow_stencil value matches it, but e.g. scsm_obj_ shaders work with pssm or any other...
							if (shadow_stencil == str_to_num(temp_str))
								{
									
									// clear current effects
									effect_load(mtl_terrain, NULL);
									effect_load(mtl_sprite, NULL);
									effect_load(mtl_model, NULL);
									effect_load(mtl_unlit, NULL);
									effect_load(mtl_shadow, NULL);
									effect_load(mtl_flat, NULL);
									effect_load(mtl_shaded, NULL);
									effect_load(mtl_metal, NULL);
									effect_load(mtl_sky, NULL);
									
									if (terrain_entity)
										{							
											effect_load(terrain_entity.material, NULL);
										}
									else
										{							
											effect_load(terrain_multi_mat, NULL);
											effect_load(terrain_single_mat, NULL);
										}
									if (water_entity)
										{
											effect_load(water_entity.material, NULL);
										}
									else
										{
											effect_load(water_multi_mat, NULL);
											effect_load(water_single_mat, NULL);					
										}
									effect_load(mtl_detail, NULL);
									effect_load(mtl_shrub, NULL);
									effect_load(mtl_grass, NULL);
									effect_load(mtl_obstacle, NULL);
									effect_load(mtl_mapent, NULL);
									effect_load(mtl_terrmdl, NULL);
									effect_load(mtl_tree, NULL);
									effect_load(mtl_building, NULL);
									
									wait(3);
									
//									printf( "Shaders cleared");
									
									// load system	material effects	
									Materials_Load_Shader_Any(mtl_terrain, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_sprite, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_model, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_unlit, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_shadow, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_flat, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_shaded, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_metal, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_sky, file_handle);									
									wait_for(Materials_Load_Shader_Any);
									
//									printf( "Basic Shaders loaded");
									
									// reserve	
									File_Read_Asc(file_handle, NULL);						
									File_Read_Asc(file_handle, NULL);
									File_Read_Asc(file_handle, NULL);
									
									// load custom material effects
									if (terrain_entity)
										{							
											Materials_Load_Shader_Any(terrain_entity.material, file_handle);
										}
									else
										{				
											Materials_Load_Shader_Any(terrain_multi_mat, file_handle);
											
											effect_load(terrain_single_mat, terrain_multi_mat.effect);	// can be NULL
										}
									wait_for(Materials_Load_Shader_Any);
									if (water_entity)
										{
											Materials_Load_Shader_Any(water_entity.material, file_handle);
										}
									else
										{
											Materials_Load_Shader_Any(water_multi_mat, file_handle);	
											
											effect_load(water_single_mat, water_multi_mat.effect);		// can be NULL				
										}			
										
//									printf( "Terrain / Water Shaders loaded");	
									
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_detail, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_shrub, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_grass, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_obstacle, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_mapent, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_terrmdl, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_tree, file_handle);
									wait_for(Materials_Load_Shader_Any);
									Materials_Load_Shader_Any(mtl_building, file_handle);
									wait_for(Materials_Load_Shader_Any);
									
//									printf( "MB Shaders loaded");
									
//									// reserve
//									file_str_read(file_handle, NULL);
//									file_str_read(file_handle, NULL);
//									file_str_read(file_handle, NULL);
//									file_str_read(file_handle, NULL);																		
								}
							
							file_close(file_handle);
						}
//					else
//						{
//							printf("msh file open error");	// test
//						}						
				}
//			else
//				{
//					printf("no msh file exists");				// test
//				}
		}
	
	//----------------------------------------------
	
#ifndef MB_GAME	
	// load panel
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
#endif	// MB_GAME
	
	ptr_remove(temp_str);
}


void		Materials_Load_Materials_Any(MATERIAL* mat_to_save , var file_handle)
{
	mat_to_save.ambient_red 	= file_var_read(file_handle); 
	mat_to_save.ambient_green 	= file_var_read(file_handle); 
	mat_to_save.ambient_blue 	= file_var_read(file_handle); 
	
	mat_to_save.diffuse_red 	= file_var_read(file_handle); 
	mat_to_save.diffuse_green 	= file_var_read(file_handle); 
	mat_to_save.diffuse_blue 	= file_var_read(file_handle); 
	
	mat_to_save.emissive_red 	= file_var_read(file_handle); 
	mat_to_save.emissive_green = file_var_read(file_handle); 
	mat_to_save.emissive_blue 	= file_var_read(file_handle); 
	
	mat_to_save.specular_red 	= file_var_read(file_handle); 
	mat_to_save.specular_green = file_var_read(file_handle); 
	mat_to_save.specular_blue 	= file_var_read(file_handle); 
	
	mat_to_save.power			 	= file_var_read(file_handle); 
	mat_to_save.albedo		 	= file_var_read(file_handle); 
	
	// effect could be saved/loaded !!!
}


void		Materials_Load_Materials_Pair(MATERIAL* material_primary, MATERIAL* material_secondary)
{
	material_secondary.ambient_red 		= material_primary.ambient_red;
	material_secondary.ambient_green 	= material_primary.ambient_green;
	material_secondary.ambient_blue 		= material_primary.ambient_blue;
	
	material_secondary.diffuse_red 		= material_primary.diffuse_red;
	material_secondary.diffuse_green 	= material_primary.diffuse_green;
	material_secondary.diffuse_blue 		= material_primary.diffuse_blue;
	
	material_secondary.emissive_red 		= material_primary.emissive_red;
	material_secondary.emissive_green 	= material_primary.emissive_green;
	material_secondary.emissive_blue 	= material_primary.emissive_blue;
	
	material_secondary.specular_red 		= material_primary.specular_red;
	material_secondary.specular_green 	= material_primary.specular_green;
	material_secondary.specular_blue 	= material_primary.specular_blue;
	
	material_secondary.power 				= material_primary.power;
	material_secondary.albedo 				= material_primary.albedo;
	
	// effect could be saved/loaded !!!
}


void		Materials_Load_Shader_Any(MATERIAL* material_actual, var file_handle)
{
	STRING* temp_str = str_create("");
	
	File_Read_Asc(file_handle, temp_str);
	
	if (file_exists_onepath(temp_str))
		{
			effect_load(material_actual, temp_str);
		}
	
	ptr_remove(temp_str);
}


///////////////////////////////////////////////////////////////////////////


#endif