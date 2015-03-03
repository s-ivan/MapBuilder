#ifndef MBscsm_c
#define MBscsm_c


////////////////////////////////////////////////////////////////////////////////////////////////
// code


void Scsm_Start()		
{
sys_marker("scs");
	// ----------------------------
	// protection
	
	if (!window_focus) return;
	
//	if (proc_status(Scsm_Start) > 0) return;								
	// or set scsm_run to 0 and wait while it is larger than 0
	scsm_run 			= 0;	
	while (proc_status(Scsm_Start) > 0) 									// the caller function is not counted
		{
			wait(1);
		}
	wait(3);		
	
	// ----------------------------
	// initialization
	
	scsm_run 			= 1;														// run loop
	
//	scsm_resolution 	= maxv(scsm_resolution, screen_size.x);		// min screen size
	scsm_resolution 	= maxv(scsm_resolution, 512);						// min screen size only at first run
	scsm_resolution 	= minv(scsm_resolution, d3d_texlimit);			// max card's texture limit
	
	scsm_areasize		= maxv(scsm_areasize, 500);
	scsm_areasize		= minv(scsm_areasize, 4000);
	
	scsm_maxdepth 		= 0.1 * scsm_areasize * 0.5;						// was ESM *1.25 ; VSM *1.5 because of * 1.5 in D3DXMatrixOrthoLH
//	scsm_sundistance 	= scsm_areasize * 2.0;								// ÷÷÷ set exactly to area*1.25 or earlier 1.5 to avoid black area artifacts
	scsm_sundistance 	= scsm_areasize * 1.25;
	
	// ----------------------------
	// blur shader
	
	Scsm_SetBlurType();
	wait_for(Scsm_SetBlurType);
	
//	printf("scsm 1");
	
	// ----------------------------
	// blur view
	
//	blurDepthView = (VIEW*)sys_malloc(sizeof(VIEW));				
	blurDepthView = view_create(-2);
	set(blurDepthView,PROCESS_TARGET);
	set(blurDepthView,CHILD);
	blurDepthView.material = sc_mtl_shadowBlur;
	
	// ----------------------------	
	// left-handed orthographic projection matrix
	
	sc_orthoMat = 0;
	
	while(camera == NULL) wait(1);
	
//	D3DXMatrixOrthoLH(sc_orthoMat, scsm_areasize * 1.5, scsm_areasize, camera.clip_near, camera.clip_far);			// out matrix, width (was *1.5), height, z-near (was 0.0), z-far (was *5)					
//	// width * 1.5 (more square area, but little dark artifacts on shadow border in some camera angles..., SMDepth should be * 0.1 * 1.5)	
	
	// new better projection matrix (left right bottom top minz maxz):
//	D3DXMatrixOrthoOffCenterLH(sc_orthoMat, -scsm_areasize, scsm_areasize, -scsm_areasize/2, scsm_areasize/2, camera.clip_near, camera.clip_far);		// ÷÷÷ bad shadow area border
	D3DXMatrixOrthoOffCenterLH(sc_orthoMat, -scsm_areasize/2, scsm_areasize/2, -scsm_areasize/2, scsm_areasize/2, camera.clip_near, camera.clip_far);	// nice shadow area border fade-out
	
	mat_set(sc_mtl_shadowDepthSun.matrix, sc_orthoMat);
	mat_effect1 = sc_orthoMat;
	
	// ----------------------------
	// depth view
		
//	shadowDepthView = (VIEW*)sys_malloc(sizeof(VIEW));				
	
//	sc_map_shadowDepthSun = (BMAP*)sys_malloc(sizeof(BMAP));		// global bmap
	sc_mtl_shadowDepthSun.flags |= ENABLE_VIEW;
	
	//setup sun depth view
	shadowDepthView 				= view_create(-21);						
	shadowDepthView.material 	= sc_mtl_shadowDepthSun;
	shadowDepthView.pan 			= 180 + sun_angle.pan;				//302;								
	shadowDepthView.tilt 		= -sun_angle.tilt;					//-38;								
		
	set(shadowDepthView,SHOW);				//*** was VISIBLE
	set(shadowDepthView,UNTOUCHABLE);
	set(shadowDepthView,NOSHADOW);	
	set(shadowDepthView,NOPARTICLE);
	set(shadowDepthView,NOCAST);			//*** to enable certain entities not to cast and recieve shadows - if SHADOW is also used for this view, they will recieve shadows but will not cast - perfect for lightmap!!!
//	set(shadowDepthView,NOFLAG1);			//*** same as NOCAST - used for detail entities - they have no shadow nor mirror view
	set(shadowDepthView,SHADOW);			//*** use if only entities with SHADOW flag should cast shadows
//	set(shadowDepthView,ISOMETRIC);		//	not needed, could be used, but material matrix determines things... causes d3dlines casting shadows
//	set(shadowDepthView,NOSKY);
	
////	set(shadowDepthView,NOLOD);			//¤¤¤ it should be used to avoid lod0 disappearing shadows when only e.g lod0 and lod1 model exist
//													//¤¤ it ahppens only in case of large area (>2500) or low sun tilt (<30)
//													//¤¤¤ but when used it is slower
//													//¤¤¤ when not used, shadows are nicer because the lod0 model casts the shadow even when the lod3 is visible
//													//¤¤¤ thus the shadow does not change when lod stage changes
//	shadowDepthView.lod = 0;				// fine
	
	if (scsm_lod<(var)0)
		{
			set(shadowDepthView,NOLOD);				//¤¤¤ no errors, shadow changes with lod model change
			shadowDepthView.lod = 0;
		}
	else
		{
			shadowDepthView.lod = scsm_lod;			//¤¤¤ shadow offset, 0 is fine
		}
	
	shadowDepthView.size_x = scsm_resolution;
	shadowDepthView.size_y = scsm_resolution;
	
	// ----------------------------
	
	sc_map_shadowDepthSun 	= bmap_createblack(scsm_resolution,scsm_resolution,14);		// 12 : 16 bit floating point format , 14 : 32 bit floating point format
	
//	printf("scsm 2");
	
	// ----------------------------
	//setup blur view
	
	shadowDepthView.bmap		= sc_map_shadowDepthSun;												// ? works
//	shadowDepthView.bmap 	= bmap_createblack(scsm_resolution,scsm_resolution,12);		// works
	
	shadowDepthView.stage 	= blurDepthView;
	blurDepthView.bmap 		= sc_map_shadowDepthSun;
	
	// ----------------------------
	//set depthmap depth
	
//	sc_orthoLightPos = 0;
//	sc_orthoLightPos[3] 			= (float)(SMdepth); 			//depthmap max depth - can be put into the loop for realtime settings **************
	
	// ----------------------------
	//set depthmap blur
		
//	SMblur *= (float)1/(float)scsm_resolution;					// should be in ratio with texel size - can be put into the loop for realtime settings **************
//	sc_mtl_shadowBlur.skill1 	= floatv(SMblur); 				//shadow blur factor; - can be put into the loop for realtime settings **************11      
	
   scsm_blursize = (float)1/(float)scsm_resolution * (float)scsm_blurpixel;
   
//   printf("scsm init");
   
	//-----------------------------
	// main loop to set depth view and other parameters
	
	VECTOR temp;
	VECTOR cam_dist;
	vec_set(temp, nullvector);
	vec_set(cam_dist, nullvector);
	
	while(scsm_run)
	{
		proc_mode = PROC_LATE;														// to avoid artifacts
		
		//----------------------------------------------------  
//		// test
//		sun_angle.pan += 1.0*time_step;
//   	sun_angle.tilt = (sinv(total_ticks % 360) + 1.0) *60.0 /2.0 +30.0;
   	
   	//----------------------------------------------------  
   	// value updates for real time settings by gui - results in flickering shadows without PROC_LATE - enough to have a loop at its settings menu
   	
   	scsm_blursize 				= (float)1 / (float)scsm_resolution * (float)scsm_blurpixel;
   	
   	// variance shadowmapping parameter conversion to float
//   	scsm_vsm_epsilon_f 		= (float)scsm_vsm_epsilon * (float)0.001;
//   	scsm_vsm_epsilon_obj_f 	= (float)scsm_vsm_epsilon_obj * (float)0.001;
   	
   	scsm_shadowoffset_f		= (float)1 / (float)scsm_resolution * (float)scsm_shadowpixeloffset;
   	
		//----------------------------------------------------   	
		// align depthmap view to camera position								// XXX
				   
		vec_set(temp, sun_pos);														// set temp vec to sun pos
//		vec_scale(temp,0.05);														// only if sun_angle.roll == 50000 i.e. default value 2500 !!!
		vec_normalize(temp, scsm_sundistance);									// was 2500 - general solution = 50000 * 0.05 -> always to scsm_sundistance to avoid black areas !!!
		vec_set(shadowDepthView.x, temp);										// scaled or normalized sun vector				
		
		// set view position
		var comp_dist = c_trace(camera.x, vector(camera.x,camera.y,-500), IGNORE_MAPS | IGNORE_MODELS | IGNORE_SPRITES | IGNORE_CONTENT | IGNORE_PASSENTS | IGNORE_FLAG2);
		if (comp_dist<=(var)0)
			comp_dist = camera.z;
		
		cam_dist.x = scsm_areasize * 0.33 + comp_dist;				// compensation to get good result for any camera height - much better!				
		vec_set(temp, cam_dist);											// scsm_areasize distance on x
		vec_rotate(temp, camera.pan);										// rotate distance towards camera look
		vec_add(temp, camera.x);											// add camera position to get it relative to camera, in front of it - z could be lower towards terrain!
		vec_add(shadowDepthView.x, temp);								// move pos to dummy ent pos
		
		// set angles due to Sun
		shadowDepthView.pan  = 180 + sun_angle.pan;
		shadowDepthView.tilt = -sun_angle.tilt;
		shadowDepthView.roll = 0;  
		   	
   	//----------------------------------------------------
//		//set global lightpos
//		vec_set(temp,shadowDepthView.x);
//		
//   	sc_orthoLightPos[0] = (float)(temp.x);
//   	sc_orthoLightPos[1] = (float)(temp.y);
//   	sc_orthoLightPos[2] = (float)(temp.z);
   	
//   	mat_effect2 = sc_orthoLightPos;									// not used only its .w as max depth
		
		//----------------------------------------------------
{}		
#ifdef DEBUG_DEPTHMAP		
		
		DEBUG_BMAP(shadowDepthView.bmap, 25, 0.25);
		DEBUG_BMAP(blurDepthView.bmap, 500, 0.25);
		
#endif
		
//		DEBUG_VAR(proc_status(sc_smSunSetup),700);
		
//		draw_text("scsm running", 50,200,COLOR_WHITE);
//		draw_text( str_for_num(NULL, proc_status(Scsm_Start)), 250,200,COLOR_WHITE);
		
		wait(1);
	}
	
	Scsm_Close();	

sys_marker(NULL);
}


////////////////////////////////////////////////////////////////////////


void Scsm_Close()
{
	scsm_run = 0;
	
	proc_kill2(Scsm_Start, NULL);
	wait(1);
	
	//---------------------------------
	// blur view
	
//	ptr_remove(blurDepthView->material);		// no, not created material !!!
	
	if (blurDepthView != NULL)
		{
			if (blurDepthView->bmap)
				{
					bmap_purge(blurDepthView->bmap);
					ptr_remove(blurDepthView->bmap);
				}
			blurDepthView->bmap = NULL;
			
			ptr_remove(blurDepthView);
			blurDepthView = NULL;
		}
	
	//----------------------------------
	// depth view
	
//	ptr_remove(shadowDepthView->material);		// no, not created material !!!
	
	if (shadowDepthView != NULL)
		{
			if (shadowDepthView->bmap)
				{
					bmap_purge(shadowDepthView->bmap);
					ptr_remove(shadowDepthView->bmap);
				}
			shadowDepthView->bmap = NULL;
				
			ptr_remove(shadowDepthView);
			shadowDepthView = NULL;
		}	
	
	//---------------------------------
	// depthmap
	
	if (sc_map_shadowDepthSun)
		{
			bmap_purge(sc_map_shadowDepthSun);
			ptr_remove(sc_map_shadowDepthSun);
			sc_map_shadowDepthSun = NULL;
		}	
	
	//----------------------------------
	
	// matrices
	sc_orthoMat = 0;
//	sc_orthoLightPos = 0;
	sc_sunLightView = 0;
	mat_effect1 = 0;
//	mat_effect2 = 0;
	mat_effect3 = 0;
	
//	printf("scsm freed");
}


////////////////////////////////////////////////////////////////////////


void		Scsm_SetBlurType()
{
	wait(3);
	effect_load(sc_mtl_shadowBlur, NULL);
	wait(3);
	
	if (scsm_blurtype == (var)0)
		{
			effect_load(sc_mtl_shadowBlur, "sc_esmDepthBlurN_MB.fx");
		}
	else if (scsm_blurtype == (var)1)
		{
			effect_load(sc_mtl_shadowBlur, "sc_esmDepthBlurB_MB.fx");
		}
	else if (scsm_blurtype == (var)2)
		{
			effect_load(sc_mtl_shadowBlur, "sc_esmDepthBlurP_MB.fx");
		}
}


////////////////////////////////////////////////////////////////////////
// load/save


void		Scsm_Save_ssc(char* file_name)
{
	//---------------------------------------------
{}
#ifndef MB_GAME	
	// save panel
	set( map_loadpanel , SHOW);
	set( map_savetext1 , SHOW);
	wait(1);
#endif	
	//---------------------------------------------
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_write(temp_str);
	//---------------------------------------------
	if (file_handle)
		{
			//-----------------
			// scsm_maxdepth
			file_var_write( file_handle , scsm_maxdepth );
			// scsm_blurpixel
			file_var_write( file_handle , scsm_blurpixel );
			// scsm_blurtype
			file_var_write( file_handle , scsm_blurtype );
			// scsm_vsm_epsilon
			file_var_write( file_handle , 0 );									// scsm_vsm_epsilon - obsolete
			// scsm_vsm_epsilon_obj
			file_var_write( file_handle , 0 );									// scsm_vsm_epsilon_obj - obsolete
			// scsm_shadowalpha
			file_var_write( file_handle , scsm_shadowalpha );
			// scsm_lightmapalpha
			file_var_write( file_handle , scsm_lightmapalpha );
			// scsm_resolution
			file_var_write( file_handle , scsm_resolution );
			// scsm_areasize
			file_var_write( file_handle , scsm_areasize );
			//
			// scsm_esm
			file_var_write( file_handle , scsm_esm );							// new
			// scsm_esm_obj
			file_var_write( file_handle , scsm_esm_obj );					// new
			// scsm_shadowpixeloffset
			file_var_write( file_handle , scsm_shadowpixeloffset );		// new
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
			
			//----------------
			// scsm_lod
			file_var_write( file_handle , scsm_lod+1 );						// new new		- saved as pssm_lod because of eof check , thus default value is -1 even if file reading because of eof results in 0
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
			//-----------------
			// scsm fade-out
			
			file_var_write( file_handle , scsm_fadeout_start );			// new new new
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
			//-----------------
			
			file_close(file_handle);
		}
	//---------------------------------------------
	
//	printf( "Scsm Settings Saved: %s" , _chr(file_name) );
	
#ifndef MB_GAME	
	// save panel
	reset( map_loadpanel , SHOW);
	reset( map_savetext1 , SHOW);
#endif	
	
	ptr_remove(temp_str);
	
	//---------------------------------------------
}


void		Scsm_Load_ssc(char* file_name)
{
	//---------------------------------------------
{}
#ifndef MB_GAME
	// load panel
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);
	wait(3);
#endif	// MB_GAME
	//---------------------------------------------	
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	// shade-c shadowmapping parameters
	
	var 	temp_scsm_maxdepth			= scsm_maxdepth;
	var 	temp_scsm_resolution 		= scsm_resolution;
	var 	temp_scsm_areasize			= scsm_areasize;
	
	var 	temp_scsm_blurpixel			= scsm_blurpixel;
	var 	temp_scsm_blurtype			= scsm_blurtype;
	var 	temp_scsm_vsm_epsilon		= 0;									// scsm_vsm_epsilon - obsolete
	var 	temp_scsm_vsm_epsilon_obj	= 0;									// scsm_vsm_epsilon_obj - obsolete
	var 	temp_scsm_shadowalpha		= scsm_shadowalpha;
	var 	temp_scsm_lightmapalpha		= scsm_lightmapalpha;
	
	var 	temp_scsm_esm					= scsm_esm;							// new
	var 	temp_scsm_esm_obj				= scsm_esm_obj;					// new
	var 	temp_scsm_shadowpixeloffset= scsm_shadowpixeloffset;		// new
	
	var 	temp_scsm_lod					= scsm_lod;							// new new
	
	var	temp_scsm_fadeout_start		= scsm_fadeout_start;			// new new new
	
	//---------------------------------------------
	var file_handle = file_open_read(temp_str);	
	//---------------------------------------------
	if (file_handle)
		{			
			//------------------
			temp_scsm_maxdepth			= file_var_read(file_handle);	
			temp_scsm_blurpixel			= file_var_read(file_handle);
			temp_scsm_blurtype			= file_var_read(file_handle);
			temp_scsm_vsm_epsilon		= file_var_read(file_handle);			// not used, obsolete vsm
			temp_scsm_vsm_epsilon_obj	= file_var_read(file_handle);			// not used, obsolete vsm
			temp_scsm_shadowalpha		= file_var_read(file_handle);
			temp_scsm_lightmapalpha		= file_var_read(file_handle);
			
			temp_scsm_resolution			= file_var_read(file_handle);
			temp_scsm_areasize			= file_var_read(file_handle);
			
			//------------------
			// new esm parameters
			temp_scsm_esm					= file_var_read(file_handle);
			temp_scsm_esm_obj				= file_var_read(file_handle);
			temp_scsm_shadowpixeloffset= file_var_read(file_handle);
			
			//------------------
			// new new parameter
			temp_scsm_lod					= file_var_read(file_handle)-1;		// new lod, no eof check because can be 0, btu it's fine, used as pssm_lod, shifted by 1
			
			//------------------
			// new new new parameter
			temp_scsm_fadeout_start		= file_var_read(file_handle);
			
			//------------------
						
			file_close(file_handle);	
		}
	//---------------------------------------------
	// apply new parameters
	
	//------------------
	// stop shadowmapping
	if (shadow_stencil == (var)-1)														// with file version protection
		{
			if ((temp_scsm_maxdepth > 0) && (temp_scsm_esm > 0))
				{
					Scsm_Close();
					wait_for(Scsm_Close);					
				}
		}
	//------------------
	// apply parameters read
	scsm_maxdepth				= temp_scsm_maxdepth;
			
	scsm_blurpixel				= temp_scsm_blurpixel;
	scsm_blurtype				= temp_scsm_blurtype;
//	scsm_vsm_epsilon			= temp_scsm_vsm_epsilon;				// obsolete vsm
//	scsm_vsm_epsilon_obj		= temp_scsm_vsm_epsilon_obj;			// obsolete vsm
	scsm_shadowalpha			= temp_scsm_shadowalpha;
	scsm_lightmapalpha		= temp_scsm_lightmapalpha;
	
	scsm_resolution 			= temp_scsm_resolution;					
	scsm_areasize				= temp_scsm_areasize;						
	
	scsm_esm						= temp_scsm_esm;
	scsm_esm_obj				= temp_scsm_esm_obj;					
	scsm_shadowpixeloffset	= temp_scsm_shadowpixeloffset;										
	
	scsm_lod						= temp_scsm_lod;					
	
	scsm_fadeout_start		= temp_scsm_fadeout_start;		
	//------------------
	// restart shadowmapping
	if (shadow_stencil == (var)-1)														// with file version protection
		{
			if ((temp_scsm_maxdepth > 0) && (temp_scsm_esm > 0))
				{			
					Scsm_Start();																// auto calculates depth that suits new resolution, but can be modified afterwards
					wait(15);																	// needs time to start...
					scsm_maxdepth 				= temp_scsm_maxdepth;					// needs to be set again after starting shadowmapping !!!
				}
		}
	//---------------------------------------------
	
//	printf( "Scsm Settings Loaded: %s" , _chr(file_name) );
	
#ifndef MB_GAME	
	// load panel
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);	
#endif	// MB_GAME
		
	ptr_remove(temp_str);
	
	//---------------------------------------------
}


////////////////////////////////////////////////////////////////////////


#endif