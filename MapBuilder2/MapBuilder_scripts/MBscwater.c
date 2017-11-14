#ifndef MBscwater_c
#define MBscwater_c


//////////////////////////////////////////////////////////////////////////////////////////////////////
// code


void Scwater_Init(ENTITY* ent)
{
	//-----------------------------------------------
	// protection
	
	if (!ent) return;		
	
	if (!window_focus) return;
	
	//-----------------------------------------------
	// reset shader and bmaps		
	
	Scwater_Close();
	wait_for(Scwater_Close);
	
	//-----------------------------------------------
	
//	printf("scwater init");
	
	set(ent , FLAG1 | TRANSLUCENT);								// FLAG1 to exclude from depthmap (and mirror views) | TRANSLUCENT to avoid rendering errors (disappearing water on skin1 change) !!!
	wait(1);
		
	//-----------------------------------------------
	
	// init depth
	if (main_water >= (var)3)				
		{
			Scwater_Depth();	
			wait_for(Scwater_Depth);		
		}
	
	// init refraction
	if ((main_water == (var)2) || (main_water == (var)4))
		{
			Scwater_Refraction();
			wait_for(Scwater_Refraction);
		}
	
	// init reflection
	Scwater_Reflection();
	wait_for(Scwater_Reflection);	
	
	//-----------------------------------------------
	// bmaps
	
	sc_mtl_water.skin1 = sc_map_reflect;
	sc_mtl_water.skin2 = sc_map_refract;
	
	if (main_water >= (var)3)
		{
			sc_mtl_water.skin3 = sc_map_depth;
		}
	
	//-----------------------------------------------
	// set entity		
	
//	bmap_to_normals(bmap_for_entity(ent, 1), 5);									// old method: convert water terrain 1st texture to a normal map
//	wait(1);
	
	Scwater_Load_ScWaterNm_tga(sc_map_nm_name);
	wait_for(Scwater_Load_ScWaterNm_tga);
	
	Scwater_Load_ScWaterFoam_tga(sc_map_foam_name);
	wait_for(Scwater_Load_ScWaterFoam_tga);
	
	ent_setskin(ent, bmap_create(sc_map_depth_name), 3);						// static depth texture instead of dynamic depth mapping
	
	//set water material
	ent.material = sc_mtl_water;
	
	//-----------------------------------------------
	// set shader
	
	if (shadow_stencil < 0)
		{
			if (main_water%2 == (var)0)												// 2 or 4
				{
					effect_load(sc_mtl_water, "sc_obj_water_MB.fx");			// height fog, sky light, and scsm support	
				}
			else
				{
					effect_load(sc_mtl_water, "sc_obj_waternr_MB.fx");			// no refraction
				}
		}
	else
		{
			effect_load(sc_mtl_water, "sc_water_obj_MB.fx");			
		}
	
	wait(1);
	
	//-----------------------------------------------		
	
	Scwater_ViewsLoop(ent);	
	
}


void Scwater_Close()
{
sys_marker("SWC");
	
	proc_kill2(Scwater_ViewsLoop, NULL);
	wait(3);
		
	//---------------------------------------------
	// remove created bmaps
	
sys_marker("SW1");	

	// depth
	if (sc_map_depth)
		{
			ptr_remove(sc_map_depth);
			sc_map_depth = NULL;
		}
	// reflect
	if (sc_map_reflect)
		{
			ptr_remove(sc_map_reflect);
			sc_map_reflect = NULL;
		}
	// refract	
	if (sc_map_refract)
		{
			ptr_remove(sc_map_refract);
			sc_map_refract = NULL;
		}
	
	//---------------------------------------------
	// remove bmap pointer of views - NO !!!
	
sys_marker("SW2");	

	// depth
	if (sc_view_depth)
		if (sc_view_depth.bmap)
			{
				sc_view_depth.bmap	= NULL;
			}
	
	// refract
	if (sc_view_refract)
		if (sc_view_refract.bmap)
			{
				sc_view_refract.bmap = NULL;
			}
	
	// reflect
	if (sc_view_reflect)
		if (sc_view_reflect.bmap)
			{
				sc_view_reflect.bmap = NULL;
			}
	
	//----------------------------------------
	// remove effect
	
sys_marker("SW3");	

	if (sc_mtl_water)
		{
			effect_load(sc_mtl_water, NULL);
		}
	
//	//----------------------------------------
//	// fresnel bmap
//	if (sc_bmap_waterFresnel)
//		{
//			bmap_purge(sc_bmap_waterFresnel);
//			ptr_remove(sc_bmap_waterFresnel);
//		}
//	sc_bmap_waterFresnel = NULL;

sys_marker(NULL);
}


//////////////////////////////////////////////////////////////////////////////////////////


void Scwater_Depth()
{
	sc_map_depth = bmap_createblack(integer(screen_size.x*0.25),integer(screen_size.y*0.25),888);			//***	0.5 , 32
	sc_view_depth.size_x = bmap_width(sc_map_depth);																	//X***
	sc_view_depth.size_y = bmap_height(sc_map_depth);																	//X*** 
	sc_view_depth.bmap = sc_map_depth;
	
	set(sc_view_depth,NOSKY);						// clear view's background to avoid wrong transparency when no ground terrain under
	sc_view_depth.bg = pixel_for_vec(COLOR_BLACK, 0, 8888);
	
	set(sc_view_depth,SHOW);
	set(sc_view_depth,NOFOG);						//*** needed for depth!!!	
	set(sc_view_depth,NOSHADOW);					//X*** needed for pssm shadows only
	set(sc_view_depth,NOPARTICLE);				//X
	set(sc_view_depth,UNTOUCHABLE);				//X
	reset(sc_view_depth, AUDIBLE);				//XXX
	set(sc_view_depth,NOFLAG1);					//¡¡¡ no detail entities marked by FLAG1 , no water
//	set(sc_view_depth,NOENT);						//~~~ faster, only terrain and blocks are seen by the view 
															// can be used together with view.genius
															// to define an entity which will be visible
															// and set it as parent of others
															// to include them in mirror view as an exception
	
	sc_view_depth.clip_far = camera.clip_far;	//X how far you can see sore depth, immediate change at that distance
	sc_view_depth.clip_near = 1;					//X*** was 0
}


void Scwater_Reflection()
{
	sc_map_reflect = bmap_createblack(integer(screen_size.x*0.25),integer(screen_size.y*0.25),888);		//***	0.5
	sc_view_reflect.size_x = bmap_width(sc_map_reflect);															//***
	sc_view_reflect.size_y = bmap_height(sc_map_reflect);															//***
	 
	sc_view_reflect.bmap = sc_map_reflect;
	sc_view_reflect.clip_near = 1;
	
	set(sc_view_reflect,SHOW);
	set(sc_view_reflect,NOFOG);					//X*** not needed for SM3.0 fog
{}
#ifdef ENGINE_BUG_DECAL_A840	
	if ((shadow_stencil < (var)0 ) || (shadow_stencil > (var)2))
#endif
 		{
 			set(sc_view_reflect,NOSHADOW);		//***			needed by pssm, not needed by decal/stencil shadows because of an engine bug below A8.45.1
 		}
	set(sc_view_reflect,NOPARTICLE);				//***
 	set(sc_view_reflect,NOSHADER);				//*** needed for correct mirror view, e.g. in case of terrain multitexturing
															//¤¤¤ do not set for SM3.0 !!! to get the same look - but close mirrors are shifted...
 	set(sc_view_reflect,NOFLAG1);					//*** ignore detaild objects
// set(sc_view_reflect,NOCULL);					//*** bad if used
	set(sc_view_reflect,PORTALCLIP);				// requires NOPARTICLE and NOSHADER
	set(sc_view_reflect,CULL_CW);
	set(sc_view_reflect,UNTOUCHABLE);	
	reset(sc_view_reflect,AUDIBLE);
	
	sc_view_reflect.aspect = -1;
//	vec_set(sc_view_reflect.portal_x,vector(0,0,-1));
	vec_set(sc_view_reflect.portal_x,vector(0,0,-4));			//*** lower a bit to eliminate reflection errors on water border
} 


void Scwater_Refraction()
{
	sc_map_refract = bmap_createblack(integer(screen_size.x*0.25),integer(screen_size.y*0.25),888);		//***	0.5
	sc_view_refract.size_x = bmap_width(sc_map_refract);															//***
	sc_view_refract.size_y = bmap_height(sc_map_refract);															//***
	 
	sc_view_refract.bmap = sc_map_refract;
	sc_view_refract.clip_near = 1;
	
	set(sc_view_refract,SHOW);
	set(sc_view_refract,NOFOG);					//*** needed for depth!!!
 	if ((shadow_stencil < (var)0 ) || (shadow_stencil > (var)2))
 		{
 			set(sc_view_refract,NOSHADOW);		//*** needed for pssm - if not used random black mirror views appear  - not needed by decal/stencil shadows and stencil shadows disappear if applied
 		}
 	set(sc_view_refract,NOPARTICLE);				//***
//	set(sc_view_refract,NOSHADER);				//*** do not use it, when Dof used because it disables transparency,
 															//*** or if you use, you should set refract color alpha in the shader below 1 for transparency!
//	set(sc_view_refract,NOFLAG1);					//*** they are excluded from mirrors, but looks better at shore if included in refraction
	set(sc_view_refract,UNTOUCHABLE);
	reset(sc_view_refract,AUDIBLE);
}


//////////////////////////////////////////////////////////////////////////////////////////


void Scwater_ViewsLoop(ENTITY* ent)
{
	if (!ent) return;		
	
//	printf("scwater loop start");
	
	while( ent != NULL )																		// no screen resolution changes are allowed during running the loop - passed ent does not work only first time ?!
		{
			proc_mode = PROC_LATE;
			
//			if (!is(ent,CLIPPED)) 
				{
					// 1st depth----------------------------------------------
					
					if (main_water > (var)2)
						{
							vec_set(sc_view_depth.x, 	camera.x);
							vec_set(sc_view_depth.pan, camera.pan);
							
							sc_view_depth.arc 		= camera.arc;							
							sc_view_depth.clip_far 	= camera.clip_far * 0.95;		// required by lod clipped entities not to have refraction siloutte on water							
							
							sc_mtl_depth.skill5 = floatv(ent.z);
							sc_mtl_depth.skill6 = floatv(scwater_waterdepth);
						}
					
					// 2nd refraction------------------------------------------
										
					if (main_water%2 == (var)0)											// 2 or 4
						{
							sc_view_refract.arc 			= camera.arc;					
							sc_view_refract.clip_far	= camera.clip_far * 0.95;	// required by lod clipped entities not to have refraction siloutte on water						
							
							sc_view_refract.fog_start	= camera.fog_start * 8.0;	// needed for proper fog in SM3.0 !!! *2		*4
							sc_view_refract.fog_end		= camera.fog_end * 8.0;		// needed for proper fog in SM3.0 !!! *2		*4
							
							vec_set(sc_view_refract.x,		camera.x);
				 			vec_set(sc_view_refract.pan,	camera.pan);		 					 			
						}
					
		 			// 3rd reflection-------------------------------------------
		 			
					sc_view_reflect.arc 			= camera.arc;					
		//  		vec_set(sc_view_reflect.portal_x,ent.x);							// init to -1 or -4, do not use
		//	  		sc_view_reflect.genius		= camera.genius;						// not necessary
					
					sc_view_reflect.fog_start	= camera.fog_start * 8.0;			// needed for proper fog *1		*4
					sc_view_reflect.fog_end		= camera.fog_end * 8.0;				// needed for proper fog *1		*4
					
					sc_view_reflect.clip_far	= camera.clip_far * 0.67;			// 0.5 not necessary, but faster, similarly to default 3DGS mirror water shader
		//			sc_view_reflect.clip_near	= camera.clip_near * 2;				// was init to 1
					
					sc_view_reflect.x				= camera.x;
					sc_view_reflect.y				= camera.y;
					sc_view_reflect.z				= ent.z - camera.z;					// move the camera at its mirror position
			
					sc_view_reflect.pan 			= camera.pan;
					sc_view_reflect.tilt 		= -camera.tilt;
				}
			
//			draw_text("scwater running", 50,150,COLOR_WHITE);
//			draw_text( str_for_num(NULL, proc_status(Scwater_ViewsLoop)), 250,150,COLOR_WHITE);			
			
			wait(1);
		}
	
//	printf("scwater loop stopped");														// never happens
	
	Scwater_Close();
}


////////////////////////////////////////////////////////////////////////////////////////////////


void		Scwater_Save_ScWater_scw(char* file_name)
{
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_write(temp_str);
	//---------------------------------------------
	if (file_handle)
		{
			file_var_write( file_handle , scwater_reflectcolor[0] );
			file_var_write( file_handle , scwater_reflectcolor[1] );
			file_var_write( file_handle , scwater_reflectcolor[2] );
			file_var_write( file_handle , scwater_reflectcolor[3] );
			file_var_write( file_handle , scwater_reflectiondistance );
			file_var_write( file_handle , scwater_reflectionstrength );
			
			file_var_write( file_handle , scwater_refractcolor[0] );
			file_var_write( file_handle , scwater_refractcolor[1] );
			file_var_write( file_handle , scwater_refractcolor[2] );
			file_var_write( file_handle , scwater_refractcolor[3] );
			file_var_write( file_handle , scwater_refractionstrength );
			
			file_var_write( file_handle , scwater_shorecolor[0] );
			file_var_write( file_handle , scwater_shorecolor[1] );
			file_var_write( file_handle , scwater_shorecolor[2] );
			file_var_write( file_handle , scwater_shorecolor[3] );			
			file_var_write( file_handle , scwater_waterdepth );
			
			file_var_write( file_handle , scwater_wavespeed );
			file_var_write( file_handle , scwater_displacement );
			file_var_write( file_handle , scwater_speed );
			file_var_write( file_handle , scwater_turbdensity );
			
			file_var_write( file_handle , scwater_foamwave_deep );
			file_var_write( file_handle , scwater_foamwave_shore );
			file_var_write( file_handle , scwater_foamtiling );
			file_var_write( file_handle , scwater_foamdistortion );
			file_var_write( file_handle , scwater_foamtranslation );
			file_var_write( file_handle , scwater_shorefoamscale );
			file_var_write( file_handle , scwater_foamscale );
			
			file_var_write( file_handle , scwater_sunlightstrength );
			file_var_write( file_handle , scwater_sunlightarea );
			
			file_var_write( file_handle , scwater_globaltransparency );
			file_var_write( file_handle , scwater_bumptexscale );
			
			file_var_write( file_handle , 10 );														// arbitrary, to check file version
			file_var_write( file_handle , scwater_2ndwave_speed );	
			file_var_write( file_handle , scwater_2ndwave_scale );
			file_var_write( file_handle , scwater_bumpstrength );	
			
			file_asc_write( file_handle , 13); 
			file_asc_write( file_handle , 10);
			
			file_str_write( file_handle , sc_map_nm_name );										// normalmap texture
			
			file_asc_write( file_handle , 13); 
			file_asc_write( file_handle , 10);
			
			file_str_write( file_handle , sc_map_foam_name );									// foam texture
			
			file_asc_write( file_handle , 13); 
			file_asc_write( file_handle , 10);
			
			//---------------------------------------------
			
			file_close(file_handle);
		}
	//----------------------------------------------		
		
	ptr_remove(temp_str);
}


void		Scwater_Load_ScWater_scw(char* file_name)
{
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_read(temp_str);
	//---------------------------------------------
	if (file_handle)
		{
			scwater_reflectcolor[0]		= file_var_read(file_handle); 
			scwater_reflectcolor[1]		= file_var_read(file_handle); 
			scwater_reflectcolor[2]		= file_var_read(file_handle); 
			scwater_reflectcolor[3]		= file_var_read(file_handle); 
			scwater_reflectiondistance	= file_var_read(file_handle); 
			scwater_reflectionstrength	= file_var_read(file_handle); 
			
			scwater_refractcolor[0]		= file_var_read(file_handle); 
			scwater_refractcolor[1]		= file_var_read(file_handle); 
			scwater_refractcolor[2]		= file_var_read(file_handle); 
			scwater_refractcolor[3]		= file_var_read(file_handle); 
			scwater_refractionstrength	= file_var_read(file_handle); 
			
			scwater_shorecolor[0]		= file_var_read(file_handle); 
			scwater_shorecolor[1]		= file_var_read(file_handle); 
			scwater_shorecolor[2]		= file_var_read(file_handle); 
			scwater_shorecolor[3]		= file_var_read(file_handle); 
			scwater_waterdepth								= file_var_read(file_handle); 
			
			scwater_wavespeed				= file_var_read(file_handle); 
			scwater_displacement			= file_var_read(file_handle); 
			scwater_speed					= file_var_read(file_handle); 
			scwater_turbdensity			= file_var_read(file_handle); 
			
			scwater_foamwave_deep		= clamp(file_var_read(file_handle), 0, 1); 
			scwater_foamwave_shore		= clamp(file_var_read(file_handle), 0, 1); 
			scwater_foamtiling			= file_var_read(file_handle); 
			scwater_foamdistortion		= file_var_read(file_handle); 
			scwater_foamtranslation		= file_var_read(file_handle); 
			scwater_shorefoamscale		= file_var_read(file_handle); 
			scwater_foamscale				= file_var_read(file_handle); 
			
			scwater_sunlightstrength	= file_var_read(file_handle); 
			scwater_sunlightarea			= file_var_read(file_handle); 
			
			scwater_globaltransparency	= file_var_read(file_handle); 
			scwater_bumptexscale			= file_var_read(file_handle); 
			
			var temp										= file_var_read(file_handle); 
			if (temp==(var)10)																	// arbitrary
				{
					scwater_2ndwave_speed	= file_var_read(file_handle); 
					scwater_2ndwave_scale	= file_var_read(file_handle); 
					scwater_bumpstrength		= file_var_read(file_handle); 
					
					file_str_read(file_handle, sc_map_nm_name); 							// normalmap
					Scwater_Load_ScWaterNm_tga(sc_map_nm_name);
					wait_for(Scwater_Load_ScWaterNm_tga);
									
					file_str_read(file_handle, sc_map_foam_name); 						// foam texture
					Scwater_Load_ScWaterFoam_tga(sc_map_foam_name);
					wait_for(Scwater_Load_ScWaterFoam_tga);
				}
			//--------------------------
			
			file_close(file_handle);
		}
	//----------------------------------------------	
		
	ptr_remove(temp_str);
}


void		Scwater_Load_ScWaterNm_tga(char* file_name)
{
	if (!water_entity) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, file_name);
	//---------------------------------------------
	
	if ( file_exists_onepath(temp_str) )
		{
			if (ent_getskin(water_entity,1))
				{
					bmap_purge(ent_getskin(water_entity,1));
					ptr_remove(ent_getskin(water_entity,1));
				}
			wait(1);
			ent_setskin(water_entity, bmap_create( temp_str ), 1);				// normal map with alpha channel required by proper alpha handling
			wait(3);
		}
	//-----------------------------------------------
//	// to support sea/ocean without ground (with TRANSLUCENT a ground is required under to not to be totally transparent)
//	
//	if (bmap_format(ent_getskin(water_entity, 1)) == (var)888)
//		{
//			reset(water_entity, TRANSLUCENT);
//		}
//	else if (bmap_format(ent_getskin(water_entity, 1)) == (var)8888)
//		{
//			set(water_entity, TRANSLUCENT);
//		}
//	wait(1);
//	
	//----------------------------------------------		
		
	ptr_remove(temp_str);
}


void		Scwater_Load_ScWaterFoam_tga(char* file_name)
{
	if (!water_entity) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, file_name);
	//---------------------------------------------
	
	if ( file_exists_onepath(temp_str) )
		{
			if (ent_getskin(water_entity,2))
				{
					bmap_purge(ent_getskin(water_entity,2));
					ptr_remove(ent_getskin(water_entity,2));
				}
			wait(1);
			ent_setskin(water_entity, bmap_create( temp_str ), 2);				// normal map with alpha channel required by proper alpha handling
			wait(3);
		}
	
	//----------------------------------------------		
		
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////


#endif