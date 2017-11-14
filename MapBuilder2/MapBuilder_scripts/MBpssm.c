////////////////////////////////////////////////////////////////
// shadows.c - parallel-split shadow mapping
// (c) oP group 2010
//
// Call Pssm_Start(3) for adding pssm to an outdoor level
// Read shader workshop 7 for the basic algorithm
// We use DirectX classes and generally write '->' here
////////////////////////////////////////////////////////////////

#ifndef MBpssm_c
#define MBpssm_c

/////////////////////////////////////////////////////////////////////////


void		Pssm_Start(var numsplits)
{
	pssm_numsplits = clamp(numsplits,0,4);																		
	if (!numsplits) return;			// switch off shadow mapping
	
	COLOR COLOR_DARK;
	vec_set(COLOR_DARK,vector(1,0,0));
	
	// okay
	sun_angle.roll = camera.clip_far*1.41*1.1;	
	
	// create the shadow material and view	
	MATERIAL* mtlShadow = mtl_create();
	// choose splitting mode		******
	if (pssm_splitmode==(var)0)
		{
			effect_load(mtlShadow,fx_shadow0);
		}
	else if (pssm_splitmode==(var)1)
		{
			effect_load(mtlShadow,fx_shadow1);
		}
	else if (pssm_splitmode==(var)2)
		{
			effect_load(mtlShadow,fx_shadow2);
		}	
	else if (pssm_splitmode==(var)3)
		{
			effect_load(mtlShadow,fx_shadow3);
		}	
//	effect_load(mtlShadow,fx_shadow);
	
	BMAP** pShadow = &mtlShadow->skin1; 																													// pointer array for accessing skin1...skin4 
		
	VIEW* viewShadow = view_create(-1);
	viewShadow->flags |= SHOW|UNTOUCHABLE|NOSHADOW|NOPARTICLE|NOSKY|NOFOG;																		// ßßß remove |NOLOD to use lod models - by Painkiller
	viewShadow->material = mtlShadow;
	viewShadow->bg = pixel_for_vec(COLOR_DARK,0,8888); 																								// clear to transparent black COLOR_BLACK *** no white needed by pssm_limit
	if (pssm_lod<(var)0)
		{
			viewShadow->flags |= viewShadow->flags|NOLOD;																								// ßßß don't apply lod offset, preferred
			viewShadow->lod = 0;
		}
	else
		{
			viewShadow->lod = pssm_lod;																														// ßßß shadow offset - by Painkiller 
		}
	
	// use the render_stencil bitmap for automatically rendering shadows in the camera view
	viewShadow->bmap = bmap_createblack(screen_size.x,screen_size.y,32);
//	viewShadow->bmap = bmap_fill(bmap_createblack(screen_size.x,screen_size.y,32), COLOR_DARK, 0);										// *** transp needed by pssm_limit!
	render_stencil = viewShadow->bmap;	
	
	// create the depth materials and views
	VIEW* viewSplit[4];																								
	var i;
	for(i=0; i<pssm_numsplits; i++)
		{ 
			viewSplit[i] = view_create(-2);
			viewSplit[i]->flags |= viewShadow->flags|ISOMETRIC|SHADOW; 																				// *** NOSHADOW for depth map (got from viewShadow->flags) + SHADOW: render only shadow entities
			if (pssm_lod>=(var)0)
				{
					viewSplit[i]->lod = pssm_lod;																												// was shadow_lod;
				}
			viewSplit[i]->bg = pixel_for_vec(COLOR_DARK,0,8888);																						// *** okay
			viewSplit[i]->bmap = bmap_createblack(pssm_res,pssm_res,14);	
//			viewSplit[i]->bmap = bmap_fill(bmap_createblack(pssm_res,pssm_res,14), COLOR_DARK, 0);											// *** 32b floating point format - transp needed by pssm_limit!
			
			pShadow[i] = viewSplit[i]->bmap;
			viewSplit[i]->material = mtl_create();
			
			if (pssm_splitmode<2)
				{
					effect_load(viewSplit[i]->material,fx_depth01);
				}
			else
				{
					effect_load(viewSplit[i]->material,fx_depth23);
				}
//			effect_load(viewSplit[i]->material,fx_depth);
		}
	
	// increase the z buffer to cover the shadow maps
	bmap_zbuffer(bmap_createblack(maxv(screen_size.x,pssm_res),maxv(screen_size.y,pssm_res),32));
//	bmap_zbuffer( bmap_fill(bmap_createblack(maxv(screen_size.x,pssm_res),maxv(screen_size.y,pssm_res),32), COLOR_DARK, 0) );	// *** transp needed by pssm_limit!
	
	//¤¤¤ needed because of an engine bug below A8.45.1, resulting in 1st terrain texture rendering onto screen, when terrain shader used
#ifdef ENGINE_BUG_PSSM_A840	
	if ((main_blur == (var)0))
		{
			effect_load(mtl_shadow, "st_stencilBlur.fx"); 				// poisson blur 
		}
#endif
	//--------------------------------------------------------------
	// blur in log space
	
//	VIEW* blurDepthView[4];
	if (pssm_splitmode > 1)
		{
			// blur material with effect
			pssm_mtlDepthBlur = mtl_create();	// has to be globalt to modify blur type i.e. its effect (shader) easily
			
			if (pssm_blurtype == (var)0)
				{
					effect_load(pssm_mtlDepthBlur,"vp_esmDepthBlurN_MB.fx");					// no blur, bad
				}
			else if (pssm_blurtype == (var)1)
				{
					effect_load(pssm_mtlDepthBlur,"vp_esmDepthBlurB_MB.fx");					// 5 tap box, not nice
				}
			else //if (pssm_blurtype == (var)2)
				{
					effect_load(pssm_mtlDepthBlur,"vp_esmDepthBlurP_MB.fx");					// 12 tap poisson, best
				}
			
			//shadow blur factor;
//			float SMblur = (float)pssm_blurfactor * (float)1/(float)pssm_res;
//			pssm_mtlDepthBlur.skill1 	= floatv(SMblur); 
			pssm_blur 	= (float)pssm_blurfactor * (float)1 / (float)pssm_res; 
			
			// blur views
			VIEW* blurDepthView[4];
			for(i=0; i<pssm_numsplits; i++)
				{
					blurDepthView[i] = view_create(-2);
					set(blurDepthView[i], PROCESS_TARGET);
					set(blurDepthView[i], CHILD);
					blurDepthView[i].material = pssm_mtlDepthBlur;
					
					viewSplit[i].stage 		= blurDepthView[i];
					blurDepthView[i].bmap 	= viewSplit[i].bmap;						
				}
				
			wait(1); // ?
		}
	
	//--------------------------------------------------------------
	
	// PSSM main loop
	while(pssm_numsplits>0)
		{
			// update the views _after_ the camera was updated! 		// *** (and after shade-c, so pssm should initialized after shade-c)
			proc_mode = PROC_LATE;
			
			pssm_fbias = (float)pssm_bias*0.001;							// var to float /1000
			
			if (pssm_splitmode > 1)
				{
					pssm_blur 	= (float)pssm_blurfactor * (float)1 / (float)pssm_res; 
				}
			
			// set up the split distances and the shadow view
			Pssm_Split(camera, pssm_numsplits, pssm_splitweight);		
//			wait_for(pssm_split);												// *** HELPS to avoid black areas when only pssm_limit is used
			Pssm_ViewCopy(camera, viewShadow);
//			wait_for(Pssm_ViewCopy);											// *** HELPS to avoid black areas when only pssm_limit is used
			
			// set up the split view transformation matrices
			D3DXMATRIX matSplit[4]; 
																	
	//		for(i=0; i<pssm_numsplits; i++) 
			for(i=0; i<minv(pssm_limit,pssm_numsplits); i++) 			// *** process only limited splits, e.g. 2 of 4
				{
					// look from the sun onto the scene			
					viewSplit[i]->pan = 180 + sun_angle.pan;
					viewSplit[i]->tilt = -sun_angle.tilt;
					vec_set(viewSplit[i]->x,sun_pos);
		
					// calculate the split view clipping borders and transformation matrix	
					if (pssm_splitmode%2 < 1)
						{		
							view_to_split(camera, pssm_splitdist[i], pssm_splitdist[i+1], viewSplit[i], &matSplit[i]);			// default, uses level_ent size internally !!!
						}
					else
						{
							Pssm_ViewToSplitCustom(camera, pssm_splitdist[i], pssm_splitdist[i+1], viewSplit[i], &matSplit[i]);	// custom by Slin
						}
					
					LPD3DXEFFECT fx = viewSplit[i]->material->d3deffect; 																		// %%%
					if(fx)
						{
							fx->SetMatrix("matSplitViewProj", &matSplit[i]);
						}
					
					// create a texture matrix from the split view proj matrix			
					D3DXMatrixMultiply(&matSplit[i], &matSplit[i], Pssm_TexScale(pssm_res));
					
					#ifdef DEBUG_PSSM		
						DEBUG_BMAP(viewSplit[i]->bmap,300 + i*220,0.2);
						var pssm_fps = 16/time_frame;
						DEBUG_VAR(pssm_fps,200);
						DEBUG_VAR(pssm_splitdist[i+1],220 + i*20);
					#endif			
				}
	
			// use a DX function to copy the 4 texture matrices to the shadow shader
			LPD3DXEFFECT fx = viewShadow->material->d3deffect; 
			if(fx)
				{
					fx->SetMatrixArray("matTex", matSplit, pssm_numsplits);
				}
		
			#ifdef DEBUG_PSSM		
				DEBUG_BMAP(viewShadow->bmap,20,0.2);
			#endif
			
			wait(1);
		}
	
	// disable pssm, clean up and remove all objects	
	ptr_remove(viewShadow->material);
	ptr_remove(viewShadow->bmap);
	ptr_remove(viewShadow);
	render_stencil = NULL;
	for(i=0; i<numsplits; i++) 
		{
			ptr_remove(viewSplit[i]->material);
			ptr_remove(viewSplit[i]->bmap);
			ptr_remove(viewSplit[i]);
//			if (pssm_splitmode > 1)
//				{
//					// common material is removed afterwards
//					ptr_remove(blurDepthView[i]->bmap);
//					ptr_remove(blurDepthView[i]);
//				}
		}
	if (pssm_splitmode > 1)
		{
			ptr_remove(pssm_mtlDepthBlur);
			pssm_mtlDepthBlur = NULL;
		}
#ifdef ENGINE_BUG_PSSM_A840
	if ((main_blur == (var)0))
		{
			effect_load(mtl_shadow,NULL); //¤¤¤ needed because of an engine bug below A8.45.1, resulting in 1st terrain texture rendering onto screen, when terrain shader used
		}
#endif
	
	wait(1);									//*** when switching on off by hotkey, wait_for requires it
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// pssm start with default settings


void		Pssm_Init()
{
	// safety
	if (terrain_entity)								// not really needed to be included
		{
			set(terrain_entity,SHADOW);			// cast shadows
			reset(terrain_entity,CAST);			// recieve shadows
		}
	if (water_entity)									// not really needed to be included
		{
			reset(water_entity,SHADOW);			// was set but not needed - results in too dark water
			reset(water_entity,CAST);				// to have shadows on surface - somehow no underwater pssm shadows are visible here, only in separate demo	
		}
	
	wait(3);
	
	//-------------------------------------------------
	// stop pssm shadows if runs for safety
	
	Pssm_Close();
	wait_for(Pssm_Close);	
	
	//-------------------------------------------------
	pssm_transparency = 0.35;						// 0.5 - 0.3
	pssm_res 			= 1024;						// 2048 - 1024 - 512
	pssm_splitweight 	= 0.75;						// 0.75-0.85 : 0.5 - 0.95				
	pssm_bias 			= 0.1;							
//	pssm_bias 			= 0.4;	
//	if (water_entity) 
//		pssm_bias		= 0.1;	
	pssm_numsplits 	= 3;
	
	pssm_limit 			= pssm_numsplits;			// 2-3 - it was useful to set lower than pssm_numsplits when no fade out and manual distance settings were implemented, not necessary to be used
	pssm_max 			= 0.50;
	pssm_fadeout_perc = 0.75;						// default 0.75 is normally okay
	
	pssm_manual 		= 1;							// 1 use manual distances, 0 use auto calculation based on pssm_splitweight
	pssm_dist1 			= 750;						// manual split distance settings - can be switched back to auto in MBshadows
	pssm_dist2 			= 1500;
	pssm_dist3 			= 3000;
	pssm_dist4 			= 6000;
	
	pssm_shadowweight = 2.5;						// 2.5 (orig) - 5.5 (max useful), used in vp_pssm pixel shader, last line
	pssm_lod				= -1;							// 0 no lod offset (same shadow as model - better quality), 1-2-3 apply lod offset (faster)
	pssm_splitmode		= 1;							// 0 default 3dgs, 1 custom by Slin
	
	wait(3);
	
	Pssm_Start(pssm_numsplits);
	
	wait(3);	
}


void	Pssm_Close()
{
	// stop pssm shadows if runs for safety
	Pssm_Start(0);
	pssm_numsplits = 0;
	wait(10);
	while (proc_status(Pssm_Start)>0)	{wait(1);}
	wait(3);																// was 30
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PSSM helper functions


// Calculate a practical split scheme
// weight parameter scales between logarithmic and uniform distances
void		Pssm_Split(VIEW* view,var num,var weight)
{
	if (!pssm_manual)
		{
			// original calculations
			var i;
			for(i=0; i<num; i++)
				{
					var idm = i/num;
					var cli = view->clip_near * pow(view->clip_far*pssm_max/view->clip_near , idm);
					var cui = view->clip_near + (view->clip_far*pssm_max-view->clip_near)*idm;
					pssm_splitdist[i] = cli*weight + cui*(1-weight);
				}
			pssm_splitdist[0] = view->clip_near;
			pssm_splitdist[num] = view->clip_far*pssm_max;					// 0.80 experimental value when num is 4
		}
	else
		{
			// manually tweakable distances - view distances not camera!
			pssm_splitdist[0] = view->clip_near;
			pssm_splitdist[1] = pssm_dist1;										// or sky_loddistance1*2;
			pssm_splitdist[2] = pssm_dist2;										// or sky_loddistance2*2;
			pssm_splitdist[3] = pssm_dist3;										// or sky_loddistance3*2;
			pssm_splitdist[4] = pssm_dist4;										// or sky_loddistance3*3;		//	or view->clip_far *pssm_max;
			pssm_splitdist[num] = pssm_splitdist[pssm_limit];				// last one, or [4]
		}
	
	pssm_fadeoutend = pssm_splitdist[pssm_limit];
	pssm_fadeoutstart = pssm_fadeoutend * pssm_fadeout_perc;		// flexible start given as percentage/100
}

/////////////////////////////////////////////////////////////////////////

// Calculate a matrix to transform points to shadowmap texture coordinates
float*		Pssm_TexScale(float fSize)
{
	static float fTexScale[16] =
		{
			0.5, 0.0, 0.0, 0.0,
			0.0,-0.5, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0
		};
	fTexScale[12] = 0.5 + (0.5/fSize);
	fTexScale[13] = 0.5 + (0.5/fSize);
	return fTexScale;
}

/////////////////////////////////////////////////////////////////////////

// copy view frustum geometry to another view
void		Pssm_ViewCopy(VIEW* from,VIEW* to)
{
	vec_set(to->x,from->x);
	vec_set(to->pan,from->pan);
	to->arc = from->arc;
 	to->aspect = from->aspect;
	to->clip_near = from->clip_near;
	to->clip_far = from->clip_far;
	to->size_x = from->size_x;
	to->size_y = from->size_y;
}

/////////////////////////////////////////////////////////////////////////

void		Pssm_ViewToSplitCustom(VIEW *frustview, var nearplane, var farplane, VIEW *split, float *matSplit)
{
	VECTOR nearcenter;
	VECTOR farleft;
	VECTOR farright;
	VECTOR center;
	
	//--------
	
	nearcenter.x = screen_size.x*0.5;
	nearcenter.y = screen_size.y*0.5;
	nearcenter.z = nearplane;
	vec_for_screen(&nearcenter, frustview);
	
	//--------
	
	farleft.x = 0.0;
	farleft.y = 0.0;
	farleft.z = farplane;
	vec_for_screen(&farleft, frustview);
	
	//--------
	
	farright.x = screen_size.x;
	farright.y = screen_size.y;										// or 0 ?
	farright.z = farplane;
	vec_for_screen(&farright, frustview);
	
	//--------
	
	vec_set(&center, &farleft);
	vec_add(&center, &farright);
	vec_scale(&center, 0.5);
	vec_add(&center, &nearcenter);
	vec_scale(&center, 0.5);
	
	//--------
	
//	float dist = vec_dist(center, farleft);
	float dist = floor(vec_dist(center, farleft)+0.5)+1.0;
	
	split->left 	= -dist;
	split->right 	= dist;
	split->bottom 	= -dist;
	split->top 		= dist;
	
	//--------
	
	vec_set(split.x, &center);
	vec_add(split.x, &sun_pos);
	
	//--------------------------------------------------------
	//move light view in pixel steps, to remove flickering
	
	D3DXMATRIX rot, invrot;
	ang_to_matrix(vector(sun_angle.pan+180, -sun_angle.tilt, 0), &rot);
	D3DXMatrixInverse(invrot, NULL, rot);
	float pxsize = 2.0*dist/(float)pssm_res;
	D3DXVECTOR4 pos;
	D3DXVECTOR3 pos3;
	pos3.x = split.x;
	pos3.y = split.z;
	pos3.z = split.y;
	D3DXVec3Transform(&pos, &pos3, &invrot);
	pos3.x = pos.x;
	pos3.y = pos.y;
	pos3.z = pos.z;
	D3DXVec3Scale(&pos3, &pos3, 1.0/pxsize);
	pos3.x = floor(pos3.x);
	pos3.y = floor(pos3.y);
	pos3.z = floor(pos3.z);
	D3DXVec3Scale(&pos3, &pos3, pxsize);
	D3DXVec3Transform(&pos, &pos3, &rot);
	split.x = pos.x;
	split.y = pos.z;
	split.z = pos.y;
	
	//--------------------------------------------------------
	
	D3DXMATRIX matview;
	D3DXMATRIX matproj;
	view_to_matrix(split, &matview, &matproj);
	D3DXMatrixMultiply(matSplit, &matview, &matproj);
	
}


/////////////////////////////////////////////////////////////////////////
// load/save


void		Pssm_Save_sps(char* file_name)
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
			//------------------
			// pssm_res
			file_var_write( file_handle , pssm_res );
			// pssm_splitweight
			file_var_write( file_handle , pssm_splitweight );
			// pssm_transparency
			file_var_write( file_handle , pssm_transparency );
			// pssm_fbias * 1000
			file_var_write( file_handle , pssm_bias );				// instead of pssm_fbias*1000, thus compatible with old values
			// pssm_limit
			if (pssm_limit > (var)0)
				{
					file_var_write( file_handle , pssm_limit );
				}
			else
				{
					pssm_limit = pssm_numsplits;
					pssm_limit = clamp(pssm_limit, 1, 4);
					file_var_write( file_handle , pssm_limit );		// default value - not essential
				}
			// pssm_numsplits
			if (pssm_numsplits > (var)0)
				{
					file_var_write( file_handle , pssm_numsplits );
				}
			else
				{
					pssm_numsplits = pssm_limit;							// probably was set just before
					file_var_write( file_handle , pssm_numsplits );	// default value - required when saving from scsm?
				}
			// pssm_max
			file_var_write( file_handle , pssm_max );
			// pssm_fadeout_perc
			file_var_write( file_handle , pssm_fadeout_perc );
			// pssm_manual
			file_var_write( file_handle , pssm_manual );
			// pssm_dist1
			file_var_write( file_handle , pssm_dist1 );
			// pssm_dist2
			file_var_write( file_handle , pssm_dist2 );
			// pssm_dist3
			file_var_write( file_handle , pssm_dist3 );
			// pssm_dist4
			file_var_write( file_handle , pssm_dist4 );
			// pssm_shadowweight
			file_var_write( file_handle , pssm_shadowweight );
			// pssm_lod
			file_var_write( file_handle , pssm_lod+1 );						// because of backward compatibility of loading (-1 applied)
			// pssm_splitmode
			file_var_write( file_handle , pssm_splitmode );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
			
			//-----------------
			// pssm_esm
			file_var_write( file_handle , pssm_esm );							// new
			// pssm_blurtype
			file_var_write( file_handle , pssm_blurtype );					// new
			// pssm_blurfactor
			file_var_write( file_handle , pssm_blurfactor );				// new
			// pssm_fogfactor
			file_var_write( file_handle , pssm_fogfactor );					// new
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
			//-----------------			
			
			file_close(file_handle);
		}
	//---------------------------------------------
	
//	printf( "Pssm Settings Saved: %s" , _chr(file_name) );
	
#ifndef MB_GAME	
	// save panel
	reset( map_loadpanel , SHOW);
	reset( map_savetext1 , SHOW);
#endif	
	
	ptr_remove(temp_str);
	
	//---------------------------------------------
}


void		Pssm_Load_sps(char* file_name)
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
	
	// pssm runs or not
//	var 	pssm_state 					= 0;
	
	// original pssm parameters
	var 	temp_pssm_res 				= pssm_res;
	var 	temp_pssm_splitweight 	= pssm_splitweight;
	var 	temp_pssm_transparency 	= pssm_transparency;
	var	temp_pssm_bias 			= pssm_bias;
	var 	temp_pssm_limit 			= pssm_limit;
	var 	temp_pssm_numsplits 		= pssm_numsplits;
	
	// new pssm parameters
	var	temp_pssm_max 				= pssm_max;
	var	temp_pssm_fadeout_perc 	= pssm_fadeout_perc;
	var	temp_pssm_manual 			= pssm_manual;
	var	temp_pssm_dist1 			= pssm_dist1;
	var	temp_pssm_dist2 			= pssm_dist2;
	var	temp_pssm_dist3 			= pssm_dist3;
	var	temp_pssm_dist4 			= pssm_dist4;
	var	temp_pssm_shadowweight 	= pssm_shadowweight;
	var	temp_pssm_lod 				= pssm_lod;
	var	temp_pssm_splitmode 		= pssm_splitmode;
	
	// new ESM parameters
	var	temp_pssm_esm		 		= pssm_esm;
	var	temp_pssm_blurtype 		= pssm_blurtype;
	var	temp_pssm_blurfactor		= pssm_blurfactor;
	var	temp_pssm_fogfactor 		= pssm_fogfactor;
	
	//---------------------------------------------
	var file_handle = file_open_read(temp_str);	
	//---------------------------------------------
	if (file_handle)
		{			
			//------------------
			temp_pssm_res 				= file_var_read(file_handle);
			temp_pssm_splitweight 	= file_var_read(file_handle);
			temp_pssm_transparency	= file_var_read(file_handle);
			temp_pssm_bias 			= file_var_read(file_handle);
			temp_pssm_limit 			= file_var_read(file_handle);
			temp_pssm_numsplits 		= file_var_read(file_handle);
			
			//------------------
			// new parameters
			temp_pssm_max 				= file_var_read(file_handle);
			temp_pssm_fadeout_perc 	= file_var_read(file_handle);
			temp_pssm_manual 			= file_var_read(file_handle);
			temp_pssm_dist1 			= file_var_read(file_handle);
			temp_pssm_dist2 			= file_var_read(file_handle);
			temp_pssm_dist3 			= file_var_read(file_handle);
			temp_pssm_dist4 			= file_var_read(file_handle);
			temp_pssm_shadowweight 	= file_var_read(file_handle);
			temp_pssm_lod 				= file_var_read(file_handle)-1;					// -1 because of backward compatibility
			temp_pssm_splitmode		= file_var_read(file_handle);
			
			//------------------
			// new pssm settings
			temp_pssm_esm				= file_var_read(file_handle);
			temp_pssm_blurtype		= file_var_read(file_handle);
			temp_pssm_blurfactor		= file_var_read(file_handle);
			temp_pssm_fogfactor		= file_var_read(file_handle);		
			
			//------------------
			
			file_close(file_handle);	
		}
	//---------------------------------------------
	// apply new parameters
	
	//------------------
	// stop shadowmapping
	if (shadow_stencil == (var)8)
		{
			if ((temp_pssm_res > 0) && (temp_pssm_max > 0) && (temp_pssm_esm > 0))		// protection because of file format changes				 
				{					
					Pssm_Close();
					wait_for(Pssm_Close);	
				}
		}
	//------------------
	// apply parameters read
	pssm_res 				= temp_pssm_res;
	pssm_splitweight 		= temp_pssm_splitweight;
	pssm_transparency		= temp_pssm_transparency;
	pssm_bias 				= temp_pssm_bias;
	pssm_limit				= temp_pssm_limit;
	pssm_numsplits 		= temp_pssm_numsplits;
	
	pssm_max					= temp_pssm_max;
	pssm_fadeout_perc 	= temp_pssm_fadeout_perc;
	pssm_manual 			= temp_pssm_manual;
	pssm_dist1 				= temp_pssm_dist1;
	pssm_dist2 				= temp_pssm_dist2;
	pssm_dist3 				= temp_pssm_dist3;
	pssm_dist4 				= temp_pssm_dist4;
	pssm_shadowweight 	= temp_pssm_shadowweight;
	pssm_lod 				= temp_pssm_lod;
	pssm_splitmode			= temp_pssm_splitmode;
	
	pssm_esm					= temp_pssm_esm;
	pssm_blurtype			= temp_pssm_blurtype;
	pssm_blurfactor		= temp_pssm_blurfactor;
	pssm_fogfactor			= temp_pssm_fogfactor;
	//------------------
	// restart shadowmapping
	if (shadow_stencil == (var)8)
		{
			if ((temp_pssm_res > 0) && (temp_pssm_max > 0) && (temp_pssm_esm > 0))		// protection because of file format changes				 
				{
					Pssm_Start(temp_pssm_numsplits);
				}
		}
	//---------------------------------------------	
	
//	printf( "Pssm Settings Loaded: %s" , _chr(file_name) );
	
#ifndef MB_GAME	
	// load panel
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);	
#endif	// MB_GAME
		
	ptr_remove(temp_str);
	
	//---------------------------------------------
}


/////////////////////////////////////////////////////////////////////////


#endif