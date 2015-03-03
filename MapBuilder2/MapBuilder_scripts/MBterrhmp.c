
#ifndef MBterrhmp_c
#define MBterrhmp_c


///////////////////////////////////////////////////////////////////////////////////////////////////


void		TerrHmp_CreateGround(char* TerrainFileName)
{
	if ( (!str_cmpi(TerrainFileName,"-")) && (file_exists_onepath(TerrainFileName)) )
		{					
			detail_size   		= GetMapGroundDetail();
			
			terrain_entity 	= ent_create( TerrainFileName , nullvector, TerrHmp_GroundAction);
			while(!terrain_entity) { wait(1); }
		}
}


void		TerrHmp_RemoveGround()
{
	//---------------------------------------------------------------------------
	// remove skins stored for texture editing
{}
#ifndef MB_GAME
	if (terrain_entity)
		{
			// store skins into terrain_hmp folder for enabling reset to original stored in hmp
			STRING* temp_str = str_create("");
			STRING* temp2_str = str_create("");
			str_cpy(temp_str, hmp_foldername);					// set folder name
			str_for_entfile(temp2_str, terrain_entity);		// get terrain entity name
			str_cat(temp_str,temp2_str);							// attach file name to folder name
			
			TerrHmp_Tx_DeleteSkinFiles(temp_str,0);			// delete temporary files saved for proper reseting and further editing
			wait_for(TerrHmp_Tx_DeleteSkinFiles);	
			wait(1);
			
			ptr_remove(temp_str);
			ptr_remove(temp2_str);
		}
#endif	// MB_GAME
	
	//------------------------------------------------------------------------------------
	// terrain entity
	
	if (terrain_entity != NULL)  
		{
			if (terrain_entity.lightmap != NULL)
				{
					bmap_purge(terrain_entity.lightmap);
					ptr_remove(terrain_entity.lightmap);
					terrain_entity.lightmap = NULL;
				}		
			if (terrain_entity.material.skin1 != NULL)
				{
					bmap_purge(terrain_entity.material.skin1);
					ptr_remove(terrain_entity.material.skin1);
					terrain_entity.material.skin1 = NULL;
				}
			if (terrain_entity.material.skin2 != NULL)
				{	
					bmap_purge(terrain_entity.material.skin2);
					ptr_remove(terrain_entity.material.skin2);
					terrain_entity.material.skin2 = NULL;
				}
			if (terrain_entity.material.skin3 != NULL)
				{		
					bmap_purge(terrain_entity.material.skin3);
					ptr_remove(terrain_entity.material.skin3);
					terrain_entity.material.skin3 = NULL;
				}
			if (terrain_entity.material.skin4 != NULL)
				{		
					bmap_purge(terrain_entity.material.skin4);
					ptr_remove(terrain_entity.material.skin4);
					terrain_entity.material.skin4 = NULL;
				}
			
			ent_purge(terrain_entity);																			// remove textures and decals
			
			// dx mesh - used in teredit and on tree creation, must be released manually
			// should be free on level/game exit - it removes the first chunk (assigned by 0)
			dx_mesh = ent_getmesh(terrain_entity,0,0);
			dx_mesh->Release();
			ent_setmesh(terrain_entity,NULL,0,0);
			wait(3);
			
			ptr_remove(terrain_entity);				
		}
	
	//------------------------------------------------------------------------------------
	
	terrain_entity		= NULL;																					// here because of wmb support !!! always needed !
	
	//------------------------------------------------------------------------------------
}


///////////////////////////////////////////////////////////////////////////////////////////////////


action TerrHmp_GroundAction()
{
	//---------------------------------------------------------------------------
sys_marker("ter");
	if (!terrain_entity) terrain_entity = me;
	
	my.group = 4;
	
	set(my,SHADOW);
	reset(my,CAST);
	
	my.ENT_CATEGORY = (var)UIMODE_GROUNDTERR;
	
	//---------------------------------------------------------------------------
	// store size
		
	if (!terrain_lod)																							// works only when no LOD applied
		{
			vec_set( terrhmp_vgroundmax , my.max_x );
			vec_set( terrhmp_vgroundmin , my.min_x );
		}
	else																											// a bit tricky when LOD is applied
		{
			CONTACT* c = ent_getvertex(my, NULL, 1);
			vec_set(terrhmp_vgroundmax, vector((var)c.v.x, (var)c.v.z, (var)c.v.y));		// works, needed to have it in 1st frame to scale water terrain properly
			terrhmp_vgroundmax.x = abs(terrhmp_vgroundmax.x);										// ?
			
			// hull update is possible only after the 1st frame
			wait(1);
			c_updatehull(me,0);																				// needed to min/max show correect values when LOD applied
			wait(1);
			
			vec_set( terrhmp_vgroundmax , my.max_x );
			vec_set( terrhmp_vgroundmin , my.min_x );						
		}
	
	// set level entity (a pseudo entity) size
	vec_set( level_ent.max_x , my.max_x );
	vec_set( level_ent.min_x , my.min_x );
	
	level_ent.max_z += 1000;																				// used as a top point of c-tracing in PosInfo3Dx functions
	
	//---------------------------------------------------------------------------
	// material settings
	
//	if (shadow_stencil == (var)-1)
//		{
//			my.material = sc_mtl_obj_terrain4cc_A8_MB;				// 4 tile clor maps + base color map, A8 materials	
////			my.material = terrain_multi_mat;
////			effect_load(my.material, "sc_obj_terrain4_A8.fx");	
////			my.material.skin1 = bmap_for_entity(my, 5);			// similarly to sc_obj_terrain7_MB
////			my.material.skin3 = bmap_for_entity(my, 7);			// similarly to sc_obj_terrain7_MB
//		}
//	else
		{
			// set material - needed for levels created in WED, MB levels are not affected
			if ( ent_status(me,8)>2 )				
			{
		//		// set matrix for detail tiling - for testing - no effect
		//		mat_identity(terrain_multi_mat.matrix);
		//		terrain_multi_mat.matrix11 = GetMapGroundDetail();
		//		terrain_multi_mat.matrix22 = GetMapGroundDetail();	
				
				my.material = terrain_multi_mat;
				
				//--------------------------------------------------------
				if (main_engineversion > 0)
					{												
						if (shadow_stencil == (var)-1)
							{
								effect_load(my.material, "sc_obj_terrain7_MB.fx");			// 3dgs terraintex compatible, with    shadwmapping, SM 3.0						
							}
						else
							{
								effect_load(my.material, "terraintex7MB.fx");				// 3dgs terraintex compatible, without shadwmapping, SM 2.0
							}
//						else
//							{
//								effect_load(my.material, "terraintexMB.fx");					// modified terraintex.fx ffe
//							}
						
						TerrHmp_Tx_UpdateMaterialSkins();										// set material skins 1-4 to entity skins 5-8 because of shader texture transfer
						wait_for(TerrHmp_Tx_UpdateMaterialSkins);
					}
			}
			else
			{
				// set matrix for detail tiling - needed when lightmap applied - have to be set here
				mat_identity(terrain_single_mat.matrix);
				terrain_single_mat.matrix11 = GetMapGroundDetail();
				terrain_single_mat.matrix22 = GetMapGroundDetail();	
				
				my.material = terrain_single_mat;		
				
				//--------------------------------------------------------
				if (main_engineversion > 0)
					{
						if (shadow_stencil == (var)-1)
							{
								effect_load(my.material, "sc_obj_terrain1_MB.fx");			// 3dgs terraintex compatible, with shadwmapping, per pixel lights				
							}
						else
							{
								effect_load(my.material, "terraintex1MB.fx");				// 3dgs terraintex compatible, without shadwmapping, per pixel lights
							}
//						else
//							{
//								effect_load(my.material, NULL);									// no effect, engine ffe blends skin and detail map
//							}
					}				
			}
		}
	
	//---------------------------------------------------------------------------
	
	wait(1);
	
	TerrHmp_FixNormals();																			// custom terrain normals - resulting in different gouraud shading (should be ready before camera init)
	wait_for(TerrHmp_FixNormals);
	
	//---------------------------------------------------------------------------
	// store skin for texture editing		

#ifndef MB_GAME

	// store skins into terrain_hmp folder for enabling reset to original stored in hmp
	STRING* temp_str = str_create("");
	STRING* temp2_str = str_create("");
	str_cpy(temp_str,hmp_foldername);					// set folder name
	str_for_entfile(temp2_str,my);						// get terrain entity name
	str_cat(temp_str,temp2_str);							// attach file name to folder name
	
	TerrHmp_Tx_StoreSkins(temp_str,0);					// save skins as _x.tga - it is needed for proper reseting and further editing  // probably it causes DX2000 crash after lightmap used ?!?!
	wait_for(TerrHmp_Tx_StoreSkins);	
	wait(1);
	
	ptr_remove(temp_str);
	ptr_remove(temp2_str);
	
#endif	// MB_GAME
	
	//----------------------------------- 
	// terrain editor testing info	
	//	while(me)
	//	{	
		//		draw_text( str_for_num(NULL,ent_status(me,8)), 100,100, vector(200,200,200) );
		//		draw_text( str_for_num(NULL,ent_skins(me)), 100,125, vector(200,200,200) );
		//		draw_text( str_for_num(NULL,teredit_tx_count), 100,150, vector(255,255,255) );
		//		wait(1);
	//	}
	//----------------------------------- 
	
	my.emask &= ~DYNAMIC;	// because needed by wmb_save() by Emre
	
sys_marker(NULL);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// load/save


void		TerrHmp_Save_Vertices_trv(char* file_name)			// simply saves terrain z positions from getvertex
{
	if (!terrain_entity)		return;
//	if (map_wmb)				return;
	// show panel--------------------------------
	set( map_loadpanel , SHOW); 
	set( map_savetext1 , SHOW);
	wait(3);
	//--------------------------------------------
	var i;
	var vertex_count = ent_status(terrain_entity,0);	
	//create file
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_write(temp_str);
	//---------------------------------------------
	if (file_handle)
		{
			// write vertex count
			file_var_write( file_handle , vertex_count );
			
			// write vertex z values
			for (i=1;i<=(var)vertex_count;i++)
				{
					// active array pos
//					file_var_write( file_handle , teredit_vertexarray[i-1].actual );		// not used for other purposes so removed			
					// or vertex pos
					CONTACT* c = ent_getvertex(terrain_entity,NULL,i); 
					file_var_write( file_handle , (var)c.v.y );	
				}
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
			//--------------------------------------
			
			file_close(file_handle);
		}	
	
	//------------------------------------------------
	reset( map_loadpanel , SHOW); 
	reset( map_savetext1 , SHOW);
	wait(3);
	
	ptr_remove(temp_str);
}


void		TerrHmp_Load_Vertices_trv(char* file_name)
{
	if (!terrain_entity) return;
//	if (map_wmb)			return;
	// show panel--------------------------------
{}
#ifndef MB_GAME
	set( map_loadpanel , SHOW); 
	set( map_loadtext1 , SHOW);
	wait(3);
#endif
	//------------------------------------------
	var i;
	var vertex_count = ent_status(terrain_entity,0);	
	// open file
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	var temp;
	//---------------------------------------------
	var file_handle = file_open_read(temp_str);	
	//---------------------------------------------
	if (file_handle)
		{
			// write vertex count
			temp = file_var_read(file_handle); 
			
			// check vertex quantity
			if (temp!=(var)vertex_count)
				{
					file_close(file_handle);
					return;
				}
			
			// read vertex z values
			for (i=1;i<=(var)vertex_count;i++)
				{
					// active array pos
//					file_var_write( file_handle , teredit_vertexarray[i-1].actual );					
					// or vertex pos
					CONTACT* c = ent_getvertex(terrain_entity,NULL,i); 
					temp = file_var_read(file_handle); 
					c.v.y = (float)temp;
					ent_setvertex(terrain_entity,c,i);
{}					
#ifndef MB_GAME					
					// if called from teredit menu panel - else if called from terrain initiaization, it must be skipped
					if (teredit_vertexarray)
						{
							// actual
//							teredit_vertexarray[i-1].actual = (var)c.v.y;	
							// new undo step
							teredit_vertexarray[i-1].undo[teredit_he_undostep] = (var)c.v.y;	
						}
#endif						
				}
			
			file_close(file_handle);
		}
	//----------------------------------------------
	
	wait(1);																// needed !!!
	TerrHmp_FixNormals();
	wait_for(TerrHmp_FixNormals);
	wait(1);
	
#ifndef A8	
	// A7 only update needed by chunk border vertices - in A8 they are updated properly
//	LPD3DXMESH dx_mesh;
	for (i=0; i<(var)(ent_status(terrain_entity,16)); i++)
		{
			dx_mesh = ent_getmesh(terrain_entity,i,0);		// ent , chunk start with 0 , lod
			D3DXComputeNormals(dx_mesh,NULL);
			ent_setmesh(terrain_entity,dx_mesh,i,0);			// maybe it works with A7 - if not it can be removed
		}
	wait(1);
	TerrHmp_FixNormals();											// for proper chunk borders
	wait_for(TerrHmp_FixNormals);
	wait(1);
#endif
	
	//------------------------------------------------
#ifndef MB_GAME	
	reset( map_loadpanel , SHOW); 
	reset( map_loadtext1 , SHOW);
	wait(3);
#endif
	
	ptr_remove(temp_str);
	
//	// update terrain tiles based on new surface-----------
//	Terrain_StaticCreate();
//	wait_for(Terrain_StaticCreate);
//	//-----------------------------------------------------
}


/////////////////////////////////////////////////


void		TerrHmp_Save_LightMap_tga(char* file_name)
{	
	if (!terrain_entity) return;
	
	if (terrain_entity.lightmap)
		{
			bmap_save( terrain_entity.lightmap, file_name );
		}
}


void		TerrHmp_Load_LightMap_tga(char* file_name)
{
	if (!terrain_entity) return;
	
	if (!file_exists_onepath(file_name))	return;
	
	if (terrain_entity.lightmap == NULL)
		{
			terrain_entity.lightmap = bmap_createblack(1024,1024,24);					// needed because bmap_load does not create the skin, only overwrites the image data
			wait(1);
		}	
	
	bmap_load(terrain_entity.lightmap, file_name, 1);
	wait(1);
}


/////////////////////////////////////////////////


void		TerrHmp_Save_Skins_tga(char* file_name)					// file name with path
{
	if (!terrain_entity)		return;
	//-------------------------------------------
{}
#ifndef MB_GAME
	set( map_loadpanel , SHOW); 
	set( map_savetext1 , SHOW);
	wait(3);
#endif
	//-------------------------------------------
	TerrHmp_Tx_StoreSkins(file_name,0);					// save skins as 0.tga(2.png) - it is needed for proper reseting and further editing
	wait_for(TerrHmp_Tx_StoreSkins);	
	//-------------------------------------------
#ifndef MB_GAME
	wait(1);
	reset( map_loadpanel , SHOW); 
	reset( map_savetext1 , SHOW);
#endif
	//-------------------------------------------
}


void		TerrHmp_Load_Skins_tga(char* file_name)					// file name with path
{
	if (!terrain_entity)		return;
	//-------------------------------------------
	if (!file_exists_onepath(file_name))	return;					// protection
	//-------------------------------------------	
{}
#ifndef MB_GAME
	set( map_loadpanel , SHOW); 
	set( map_loadtext1 , SHOW);
	wait(3);
#endif
	//-------------------------------------------
	STRING* temp_str = str_create("");
	STRING* temp2_str = str_create("");					
	int i=0;
	
	while(1)
		{
			// reset to original hmp			
			str_cpy(temp_str,file_name);															// set folder name
			str_trunc(temp_str,5);																	// cut extension and skin number 1..9 to get base name	
			str_for_int(temp2_str , i+1);															// get skin number								
			str_cat(temp_str,temp2_str);															// attach skin number
			str_cat(temp_str,".tga");	
			
			if (file_exists_onepath(temp_str))
				{
					if (i+1 > ent_status(terrain_entity,8))									// if no more skins - add a skin then copy
						{
							// create dummy
							BMAP* temp_bmap = bmap_create(temp_str);							// flipped loading
							ent_setskin(terrain_entity,temp_bmap,i+1);						// add skin																			
							bmap_to_mipmap(temp_bmap);												// goes back to created image ! so a new loading is needed
							wait_for(bmap_to_mipmap);						
							wait(1);				
							
							temp_bmap = NULL;
							ptr_remove(temp_bmap);		
{}							
#ifndef MB_GAME							
							// editor only - when loading set from file, called by terrain tx editor
							if (UIMode==(var)UIMODE_FILEMENU2) 									// ignore in case of outer calls - always out !
								teredit_tx_count = ent_status(terrain_entity,8);			// inc texture counter												
#endif								
						}
					
					TerrHmp_Tx_LoadSkin(temp_str, i+1);									// pixel by pixel										
					wait_for(TerrHmp_Tx_LoadSkin);
					
					wait(1);
				}
			else
				{
					if (i+1 <= ent_status(terrain_entity,8))									// if no file but skin exists - do nothing or clear (set to transparent black)
						{
//{}
//#ifndef MB_GAME							
							BMAP* temp2_bmap = bmap_createblack(512,512,32);
							ent_setskin(terrain_entity,bmap_to_mipmap(temp2_bmap),i+1);	// set to entity
							wait(1);
							
							temp2_bmap = NULL;
							ptr_remove(temp2_bmap);		
//#endif									
						}
					else																					// if no more files nor skins - exit loop 
						{
							break;																		// no  more files
						}
				}
				
			i++;
		}
	//-------------------------------------------
#ifndef MB_GAME
	// editor only
	if (UIMode==(var)UIMODE_FILEMENU2)															// ignore in case of outer calls
		{
			wait(1);
			
			TerEdit_Tx_UpdateSets32();																// refreshing thumbnails can be called only from within texture editor !
			wait_for(TerEdit_Tx_UpdateSets32);
			
			TerrHmp_Tx_UpdateMaterialSkins();
			wait_for(TerrHmp_Tx_UpdateMaterialSkins);
		}
#endif
	//-------------------------------------------
	wait(1);
	//-------------------------------------------
#ifndef MB_GAME
	reset( map_loadpanel , SHOW); 
	reset( map_loadtext1 , SHOW);
#endif
}


////////////////////////////////


void		TerrHmp_Tx_StoreSkins(char* file_name, var mode)						
{
	//-----------------------------------------------
	if (!terrain_entity)		return;
	//-----------------------------------------------
	int i;
	STRING* temp_str = str_create("");
	STRING* temp2_str = str_create("");
	var tex_count = ent_status(terrain_entity, 8);		// number of skins = teredit_tx_count , but enables outer call
//	var tex_count = ent_skins(terrain_entity);
	//-----------------------------------------------
	for(i=0; i<tex_count; i++)
		{
			if (bmap_for_entity(terrain_entity, i+1) != NULL)
				{					
					// hmp filename
					str_cpy(temp_str,file_name);					
					str_trunc(temp_str,4);						// new ending
					str_cat(temp_str,"_skin");											
					str_for_int(temp2_str,i+1);				// skin number								
					str_cat(temp_str,temp2_str);
					
					// save with desired extension(s)
					if (mode==(var)0)								// save .tga
						{					
							str_cat(temp_str,".tga");							
						}
					else if (mode==(var)1)						// save .dds - if the skin is dds it saves as compressed dds, if tga it is saved as uncompressed dds
						{		
							str_cat(temp_str,".dds");
						}
					else if (mode==(var)2)						// save .png 
						{		
							str_cat(temp_str,".png");
						}	
					
					// okay only with bmap_load() - produces a flipped 32b image if bmap_create() is used for the stored image !!! 					
					if ((tex_count<3) && (i==0))
						{
							TerrHmp_Save_Skin24_tga(temp_str);
							wait_for(TerrHmp_Save_Skin24_tga);
							break; 									// do not save detail map !!!
						}
					else 
						{
							bmap_save(bmap_for_entity(terrain_entity, i+1), temp_str);
							wait_for(bmap_save);			
						}
					
					wait(1);	
				}			
		}
	//-----------------------------------------------
	wait(1);	
	ptr_remove(temp_str);
	ptr_remove(temp2_str);
	//-----------------------------------------------
}


void		TerrHmp_Tx_DeleteSkinFiles(char* file_name, var mode)						
{
	//-----------------------------------------------
	if (!terrain_entity)		return;
	//-----------------------------------------------
	int i;
	STRING* temp_str = str_create("");
	STRING* temp2_str = str_create("");
	var tex_count = ent_status(terrain_entity, 8);		// number of skins = teredit_tx_count , but enables outer call
//	var tex_count = ent_skins(terrain_entity);
	//-----------------------------------------------
	for(i=0; i<tex_count; i++)
		{				
			// hmp filename
			str_cpy(temp_str, file_name);					
			str_trunc(temp_str, str_len(filemenu_hmp_ext));						// remove extension .hmp
			str_cat(temp_str, "_skin");											
			str_for_int(temp2_str, i+1);												// attach skin number								
			str_cat(temp_str, temp2_str);
			
			// add desired extension
			if (mode==(var)0)								// save .tga
				{					
					str_cat(temp_str,".tga");							
				}
			else if (mode==(var)1)						// save .dds - if the skin is dds it saves as compressed dds, if tga it is saved as uncompressed dds
				{		
					str_cat(temp_str,".dds");
				}
			else if (mode==(var)2)						// save .png 
				{		
					str_cat(temp_str,".png");
				}	
			
			file_remove_onepath(temp_str);			// remove file safely, only from the terrain subfolder			
		}
	//-----------------------------------------------
	ptr_remove(temp_str);
	ptr_remove(temp2_str);
	//-----------------------------------------------
}


////////////////////////////////


void		TerrHmp_Tx_LoadSkin(STRING* temp_str, var skin_num)
{
	//--------------------------------------------------------
	// protections
	
	if (!terrain_entity)										return;
	if ( skin_num > ent_status(terrain_entity,8) )	return;				
	if (!file_exists_onepath(temp_str))					return;					
	
	//--------------------------------------------------------
	// new texture
	
	BMAP* temp_bmap = bmap_createblack(512,512,32);
	wait(1);
		{
			bmap_load( temp_bmap , temp_str , 1 );					
			wait_for(bmap_load);
			wait(1);
		}		
	
	//--------------------------------------------------------
	// old texture
	
	BMAP* skin_bmap = bmap_for_entity(terrain_entity , skin_num);
	wait(1);			
	
	// if dds, convert
	if (bmap_format(skin_bmap) < 565)
		{			
			ent_setskin( terrain_entity , bmap_to_mipmap(bmap_to_format(skin_bmap, 8888)) , skin_num );	// 32b works 				
			wait(1);
		}	
	
	//--------------------------------------------------------
	
	var pixel_format1 = bmap_lock(temp_bmap,0);														
	var pixel_format2 = bmap_lock(skin_bmap,0);		
	
	// only if the 2 texture size is the same
	if ( 
			(bmap_width(temp_bmap) == bmap_width(skin_bmap)) 
			&& 
			(bmap_height(temp_bmap) == bmap_height(skin_bmap)) 
		)
		{
			var pixel;
			COLOR pixel_color;
			var pixel_alpha;
			int i,j;
			for(i=0;i<bmap_width(skin_bmap);i++)
				{
					for(j=0;j<bmap_height(skin_bmap);j++)
						{
							pixel = pixel_for_bmap( temp_bmap , i , j );										// get pixel of loaded image
							
//							pixel_to_vec( pixel_color , pixel_alpha , pixel_format1 , pixel );		// get color and alpha of loaded image pixel							
//							pixel = pixel_for_vec( pixel_color , pixel_alpha , pixel_format2 );		// set pixel color and alpha of skin image to loaded image

							pixel_to_bmap( skin_bmap , i , j , pixel );										// set pixel of new image
						}
				}
			
			bmap_unlock(temp_bmap);	
			bmap_unlock(skin_bmap);
			bmap_to_mipmap(skin_bmap);																				// update mipmaps after unlocking, to avoid the original skin to appear from big distance	
		}
	else
		{
			bmap_unlock(temp_bmap);	
			bmap_unlock(skin_bmap);
		}		
	wait(1);
	
	//--------------------------------------------------------
	
	TerrHmp_Tx_UpdateMaterialSkins();
	wait(1);
	
	//--------------------------------------------------------
	
	bmap_purge(temp_bmap);
	temp_bmap = NULL;
	ptr_remove(temp_bmap);
	
	skin_bmap = NULL;
	ptr_remove(skin_bmap);
	
	//--------------------------------------------------------
}


void		TerrHmp_Tx_LoadSkin_KeepAlpha(STRING* temp_str, var skin_num)
{
	//--------------------------------------------------------
	// protections
	
	if (!terrain_entity)										return;
	if ( skin_num > ent_status(terrain_entity,8) )	return;				
	if (!file_exists_onepath(temp_str))					return;									
	
	//--------------------------------------------------------
	// new texture
	
	BMAP* temp_bmap = bmap_createblack(512,512,32);
	wait(1);
		{
			bmap_load( temp_bmap , temp_str , 1 );				
			wait_for(bmap_load);
			wait(1);
		}
	
	//--------------------------------------------------------
	// old texture
	
	BMAP* skin_bmap = bmap_for_entity(terrain_entity , skin_num);	
	wait(1);
	
	// if dds, convert
	if (bmap_format(skin_bmap) < 565)
		{			
			ent_setskin( terrain_entity , bmap_to_mipmap(bmap_to_format(skin_bmap, 8888)) , skin_num );	// 32b works 				
			wait(1);
		}	
	
	//--------------------------------------------------------
	// new texture resized to old texture size
	
	BMAP* sized_bmap = bmap_createblack(bmap_width(skin_bmap), bmap_height(skin_bmap), 32);
	wait(1);	
	// convert new texture to old skin sized bmap - 24 bit only !!!
	bmap_blit(sized_bmap, temp_bmap, NULL, vector(bmap_width(skin_bmap), bmap_height(skin_bmap), 0) );
	wait(1);	
	
	//--------------------------------------------------------
	
	var pixel_format1 = bmap_lock(sized_bmap,0);														
	var pixel_format2 = bmap_lock(skin_bmap,0);	
	
	// only if the 2 texture size is the same
	if ( 
			(bmap_width(sized_bmap) == bmap_width(skin_bmap)) 
			&& 
			(bmap_height(sized_bmap) == bmap_height(skin_bmap)) 
		)
		{
			var pixel1, pixel2;
			COLOR pixel_color1, pixel_color2;
			var pixel_alpha2;
			int i,j;
			for(i=0;i<bmap_width(skin_bmap);i++)
				{
					for(j=0;j<bmap_height(skin_bmap);j++)
						{
							pixel1 = pixel_for_bmap( sized_bmap , i , j );										// 1 get pixel of new image
							pixel2 = pixel_for_bmap( skin_bmap , i , j );										// 2 get pixel of old image
							
							pixel_to_vec( pixel_color1 , NULL			, pixel_format1 , pixel1 );		// 1 get color and alpha of new image pixel							
							pixel_to_vec( pixel_color2 , pixel_alpha2	, pixel_format2 , pixel2 );		// 2 get color and alpha of old image pixel	
							
							pixel1 = pixel_for_vec( pixel_color1 , pixel_alpha2 , pixel_format2 );		// 1 set pixel color to new, and 2 alpha to old of skin image to old image
							
							pixel_to_bmap( skin_bmap , i , j , pixel1 );											// 1-2 set pixel of new image
						}
				}
			bmap_unlock(sized_bmap);	
			bmap_unlock(skin_bmap);
			bmap_to_mipmap(skin_bmap);																					// @@@ update mipmaps after unlocking, to avoid the original skin to appear from big distance	
		}
	else
		{
			bmap_unlock(sized_bmap);	
			bmap_unlock(skin_bmap);
		}	
	wait(1);		
	
	//--------------------------------------------------------
	
	TerrHmp_Tx_UpdateMaterialSkins();	
	wait(1);
	
	//--------------------------------------------------------
	
	bmap_purge(temp_bmap);
	temp_bmap = NULL;
	ptr_remove(temp_bmap);
	
	skin_bmap = NULL;
	ptr_remove(skin_bmap);
	
	bmap_purge(sized_bmap);
	sized_bmap = NULL;
	ptr_remove(sized_bmap);
	
	//--------------------------------------------------------
}


void		TerrHmp_Tx_UpdateMaterialSkins()
{
	if (!terrain_entity)		return;
	
	if (main_engineversion > 0)
		{
			terrain_entity.material.skin1 = bmap_for_entity(terrain_entity, 5);
			terrain_entity.material.skin2 = bmap_for_entity(terrain_entity, 6);
			terrain_entity.material.skin3 = bmap_for_entity(terrain_entity, 7);
			terrain_entity.material.skin4 = bmap_for_entity(terrain_entity, 8);	
		}
}


/////////////////////////////////////////////////


void		TerrHmp_Save_Skin24_tga(char* file_name)											
{
	if (!terrain_entity)		return;
	
	//--------------------------------------------------------
	
	bmap_save(bmap_to_format(ent_getskin(terrain_entity, 1), 888), file_name);											// getskin!
	wait_for(bmap_save);	
	wait(1);	
	
	//--------------------------------------------------------
	
//	bmap_to_mipmap(bmap_for_entity(terrain_entity, 1));																		// okay but on next pixel drawing it becomes black								
	ent_setskin(terrain_entity , bmap_to_mipmap(bmap_to_format(ent_getskin(terrain_entity, 1), 888)), 1 );	// getskin! all elements are required ! looks a bit ugly, and slower, but perfect
	wait(1);	
	
//	//--------------------------------------------------------
//	// or	- custom save tga (slower but okay with bmap_create)
//	SaveTga_SaveFile(bmap_to_mipmap(bmap_to_format(ent_getskin(terrain_entity, 1), 888)), file_name, 24);			
//	wait_for(SaveTga_SaveFile);
//	wait(1);	
//	//--------------------------------------------------------
}


void		TerrHmp_Load_Skin24_tga(char* file_name)
{	
	//--------------------------------------------------------
	
	if (!terrain_entity)							return;
	if (!file_exists_onepath(file_name))	return;						// protection
		
	//--------------------------------------------------------
	BMAP* temp_bmap = bmap_createblack(512, 512, 24);
	wait(1);
	
	bmap_load(temp_bmap, file_name, 1 );					
	wait_for(bmap_load);
	wait(1);
	
	//--------------------------------------------------------
	BMAP* skin_bmap = bmap_for_entity(terrain_entity , 1);
	wait(1);		
		
	//--------------------------------------------------------
	bmap_blit(skin_bmap, temp_bmap, NULL, vector(skin_bmap.width, skin_bmap.height, 0));							// works with size restrictions determined by current skin, it requires mipmap() afterwards
																																				// larger files not loaded, smallers are copied into part if destsize is NULL
	wait(1);
	
	ent_setskin(terrain_entity , bmap_to_mipmap(bmap_to_format(ent_getskin(terrain_entity, 1), 888)), 1 );	// all elements are required ! looks a bit ugly, and slower, but perfect
	wait(1);		
	
//	//--------------------------------------------------------
//	// or	- if custom save tga used - not okay with bmap_save
//	ent_setskin(terrain_entity, bmap_to_mipmap(bmap_to_format(bmap_create(file_name), 888)), 1);					// E2000 DX crash if large is loaded onto small image
//	wait(1);
//	//--------------------------------------------------------
}


/////////////////////////////////////////////////


void		TerrHmp_Save_DetailMap24_dds(char* file_name)
{
	if (!terrain_entity)		return;
	//--------------------------------------------------------
	bmap_save( bmap_for_entity(terrain_entity,2) , file_name );																// 32b
	wait_for(bmap_save);
	wait(1);
	//--------------------------------------------------------
}


void		TerrHmp_Load_DetailMap24_dds(char* file_name)
{
	//--------------------------------------------------------
	
	if (!terrain_entity)							return;
	if (!file_exists_onepath(file_name))	return;																				// protection
	
	//--------------------------------------------------------
	
	BMAP* temp_bmap = bmap_create(file_name);
	wait(1);
	
	bmap_load(temp_bmap,file_name,1);
	wait_for(bmap_load);
	wait(1);
	
	//--------------------------------------------------------
	
	ent_setskin(terrain_entity, bmap_to_mipmap(bmap_to_format(temp_bmap,8888)),2);									// in reality 32b	
	wait(1);
	
	//--------------------------------------------------------
	
	temp_bmap = NULL;
	ptr_remove(temp_bmap);
	
	//--------------------------------------------------------
}


/////////////////////////////////////////////////


void		TerrHmp_Save_AutoTex_sat(char* file_name)
{
	STRING* temp_str = str_create("");
	str_cpy(temp_str, file_name);
	//---------------------------------------------
	var file_handle = file_open_write(temp_str);
	//---------------------------------------------
	if (file_handle)
		{
			file_var_write( file_handle , basecolor_tiling );
			file_var_write( file_handle , grass_height_border );
			file_var_write( file_handle , grass_height_width );
			file_var_write( file_handle , shore_height_width );
			file_var_write( file_handle , cliff_slope_min );
			file_var_write( file_handle , cliff_slope_width );
			
			// reserves
			file_var_write( file_handle , 0 );
			file_var_write( file_handle , 0 );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
			//------------------------------
			
			file_close(file_handle);
		}
	//----------------------------------------------
		
	ptr_remove(temp_str);
}


void		TerrHmp_Load_AutoTex_sat(char* file_name)
{
	STRING* temp_str = str_create("");
	str_cpy(temp_str, file_name);
	//---------------------------------------------
	var file_handle = file_open_read(temp_str);
	//---------------------------------------------
	if (file_handle)
		{
			basecolor_tiling 			= file_var_read(file_handle); 
			grass_height_border 		= file_var_read(file_handle); 
			grass_height_width 		= file_var_read(file_handle); 
			shore_height_width 		= file_var_read(file_handle); 
			cliff_slope_min 			= file_var_read(file_handle); 
			cliff_slope_width 		= file_var_read(file_handle); 
			
			// reserves
//			... 	= file_var_read(file_handle); 
//			... 	= file_var_read(file_handle); 
			
			//------------------------------------
			
			file_close(file_handle);
		}
	//----------------------------------------------
	
	ptr_remove(temp_str);
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// get / set


VECTOR*	TerrHmp_GetGroundMin()
{
	return terrhmp_vgroundmin;
}


VECTOR*	TerrHmp_GetGroundMax()
{
	return terrhmp_vgroundmax;
}

//------------------------------------

function	TerrHmp_GetGroundMinX()
{
	return terrhmp_vgroundmin.x;
}


function	TerrHmp_GetGroundMinY()
{
	return terrhmp_vgroundmin.y;
}


function	TerrHmp_GetGroundMinZ()
{
	return terrhmp_vgroundmin.z;
}

//------------------------------------

function	TerrHmp_GetGroundMaxX()
{
	return terrhmp_vgroundmax.x;
}


function	TerrHmp_GetGroundMaxY()
{
	return terrhmp_vgroundmax.y;
}


function	TerrHmp_GetGroundMaxZ()
{
	return terrhmp_vgroundmax.z;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


void	TerrHmp_SetGroundMinToDefault()
{
	vec_set(terrhmp_vgroundmin, vector(-5000,-5000,-1000));
}


void	TerrHmp_SetGroundMaxToDefault()
{
	vec_set(terrhmp_vgroundmax, vector(5000,5000,1000));
}


//////////////////////////////////////////////////////////////////////////////////////////

// okay and has better result than engine default gouraud shading
void		TerrHmp_FixNormals()
{
	if (!terrain_entity)		return;
	
	// maybe needed, but updating hull is done automatically in A8.40.3
//	c_setminmax(terrain_entity);
//	wait(1);
	// in A8.40.3 it is done automatically
//	c_updatehull(terrain_entity,0);
//	//	c_updatehull(terrain_entity,my.frame);
//	wait(3);	
	
	// set custom normals - in terrain editing mode, it would be enough only on selected area
	var i;
	for (i=1; i<=(var)(ent_status(terrain_entity,0)); i++)
		{	
			CONTACT* c = ent_getvertex(terrain_entity,NULL,i);   			// vertex number begins with 1 !			
			c_trace( vector((var)c.v.x, (var)c.v.z, TerrHmp_GetGroundMaxZ() + GetMapTileSize() * 10), 
						vector((var)c.v.x, (var)c.v.z, TerrHmp_GetGroundMinZ() - GetMapTileSize() * 10), 
						IGNORE_ME | IGNORE_SPRITES | IGNORE_MODELS | IGNORE_MAPS | IGNORE_PASSABLE ); 			
			if (HIT_TARGET)	
				{
					// normal 
//					c.v.nx = (float)normal.x; 			
//					c.v.nz = (float)normal.y; 
//					c.v.ny = (float)normal.z; 	
					// or the same with hit
					c.v.nx = (float)hit.nx; 			
					c.v.nz = (float)hit.ny; 
					c.v.ny = (float)hit.nz; 																													
					ent_setvertex(terrain_entity,c,i);
					// or more precisely it should be the average of normals of all neighbouring faceces !!! 
					// it is stored in hmp in an approximated value of 162 stored normal vectors !!!
				}
			
		}		
	
	// set again min and max values , used in PosInfo measuerements , used by camera z (height) positioning
	vec_set( terrhmp_vgroundmax , terrain_entity.max_x );
	vec_set( terrhmp_vgroundmin , terrain_entity.min_x );
}


void		TerrHmp_FixNormals_Area(var upleft_vert, var width_vert, var height_vert)
{
	if (!terrain_entity)		return;
	
	// vertices per side = mesh squares per side + 1
	var vert_count_x = ent_status(terrain_entity, 2) + 1;	
	
	int i,j,k;
	for (i=0;i<width_vert;i++)
		{
			for (j=0;j<height_vert;j++)
				{
					// vertex number
					k = upleft_vert +j*vert_count_x +i;
					// protection
					if (k>ent_status(terrain_entity,0))
						{
							break;
						}
					
					CONTACT* c = ent_getvertex(terrain_entity,NULL,k);   			// vertex number begins with 1 !			
					c_trace( vector((var)c.v.x, (var)c.v.z, TerrHmp_GetGroundMaxZ() + GetMapTileSize() * 10), 
								vector((var)c.v.x, (var)c.v.z, TerrHmp_GetGroundMinZ() - GetMapTileSize() * 10), 
								IGNORE_ME | IGNORE_SPRITES | IGNORE_MODELS | IGNORE_MAPS | IGNORE_PASSABLE ); 			
					if (HIT_TARGET)	
						{
							// normal 
		//					c.v.nx = (float)normal.x; 			
		//					c.v.nz = (float)normal.y; 
		//					c.v.ny = (float)normal.z; 	
							// or the same with hit
							c.v.nx = (float)hit.nx; 			
							c.v.nz = (float)hit.ny; 
							c.v.ny = (float)hit.nz; 																													
							ent_setvertex(terrain_entity,c,k);
							// or more precisely it should be the average of normals of all neighbouring faceces !!! 
							// it is stored in hmp in an approximated value of 162 stored normal vectors !!!
						}
				}				
		}
	
	// set again min and max values , used in PosInfo measuerements , used by camera z (height) positioning
	vec_set( terrhmp_vgroundmax , terrain_entity.max_x );
	vec_set( terrhmp_vgroundmin , terrain_entity.min_x );
}


///////////////////////////////////////////////////////////////////////////////////////////////////


#endif