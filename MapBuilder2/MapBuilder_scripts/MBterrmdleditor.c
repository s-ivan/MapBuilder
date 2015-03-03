
#ifndef MBterrmdleditor_c
#define MBterrmdleditor_c 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor initialization


void 		TerrMdl_Init()
{
	terrmdl_ready = 0;
		
	//----------------------------------------------------------------
	// terrmdl imagesize determination: 256 (low quality) or 512 (high quality)
	TerrMdl_SetDefaultSize();
	wait_for(TerrMdl_SetDefaultSize);
	//----------------------------------------------------------------
	// terrmdl sprites filename initialization 
sys_marker("ti1");
	int arraysize = (int)TERRMDL_MAXPERSET;
	terrmdl_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );
	int i;	
	// init filename string array elements
	for(i=0;i<max_terrmdl_items;i++)
		{
			terrmdl_filenames[i] = str_create("");
		}	
sys_marker(NULL);		
	//-----------------------------------------------------------------
	// determine how many dds or tga files are in the folder, 
	// divide it by 8, and the result is the number of sets
//	terrmdl_set = 1;
	max_terrmdl_sets = TERRMDL_MAXSET;
	terrmdls_txt = txt_create( (max_terrmdl_sets*max_terrmdl_items) , 0 );		// 10*8=80
	for(i=0;i<max_terrmdl_sets*max_terrmdl_items;i++)
		{
			str_cpy( (terrmdls_txt->pstring)[i] , "" );
		}
	//-------------------------------------------------------------------
	// read in folder content
	i = 0;
	STRING* temp_str = str_create("");
	str_cpy( temp_str , terrmdl_foldername );
	str_cat( temp_str , "*.mdl" );
	txt_for_dir( terrmdls_txt, temp_str );								
	while (str_len((terrmdls_txt->pstring)[i]) > 0)
		{
			i++;
		}
	max_terrmdl_sets = integer(i/max_terrmdl_items);
	if ( fraction(i/max_terrmdl_items) > 0)
		{
			max_terrmdl_sets++;		// allow to display not complete sets too
		}
	terrmdl_set = clamp(terrmdl_set, 1, max_terrmdl_sets);
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	for(i=0;i<max_terrmdl_items;i++)
		{
//			str_cpy( terrmdl_filenames[i] , terrmdl_foldername );							// subfolder
//			str_cat( terrmdl_filenames[i] , (terrmdls_txt->pstring)[i] );				// filename with extension			
			str_cpy( terrmdl_filenames[i] , "" );												// (or terrmdl_foldername) subfolder
			int j = (terrmdl_set - 1) * max_terrmdl_items;
			str_cat( terrmdl_filenames[i] , (terrmdls_txt->pstring)[j+i] );			// filename with extension
			if ( str_cmpi( terrmdl_filenames[i] , "" ) )										// (or terrmdl_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( terrmdl_filenames[i] , "-" );
				}
		}	
	//------------------------------------------------------------------
	// initial value settings
	for(i=0;i<max_terrmdl_items;i++)
		{
			terrmdl_switch_type[i] = 0;
		}
	terrmdl_switch_type[terrmdl_selected] = 1;	
//	terrmdl_tilesize = 1;
//	terrmdl_density = 3;	
	//-------------------------------------------------------------------
	// create objects
	collision_mode = 0;
	terrmdl_model_preview = ent_create( NULL , vector(square_coord_x,square_coord_y,square_coord_z) , TerrMdl_ModelPreview );
	wait(1);
//	ent_clone(terrmdl_model_preview);		// CLO
	collision_mode = 1;
	//-------------------------------------------------------------------
//	terrmdl_unlit = 0;
//	terrmdl_light = 1;
	TerrMdl_Panel_Init();	
	wait_for(TerrMdl_Panel_Init);
	//-------------------------------------------------------------------
	TerrMdl_MorphPreviewModel();
	//-------------------------------------------------------------------
}


void		TerrMdl_Close()
{
	terrmdl_ready = 0;
	
	ptr_remove(terrmdl_model_preview);
	terrmdl_model_preview = NULL;
	
	ptr_remove(terrmdl_panel);
	terrmdl_panel = NULL;
	
//	ptr_remove(mtl_terrmdl);		
	//--------------------------------------------------------
	int i;
	// remove temp text
	for (i = 0; i < terrmdls_txt->strings; i++) 
		if ((terrmdls_txt->pstring)[i]) 
			ptr_remove((terrmdls_txt->pstring)[i]);
	ptr_remove(terrmdls_txt);
	terrmdls_txt = NULL;
	
	// remove temp string array
	for(i=0;i<max_terrmdl_items;i++)
		{
			ptr_remove(terrmdl_filenames[i]);
		}
	sys_free(terrmdl_filenames);
	terrmdl_filenames = NULL;
//	ptr_remove(terrmdl_filenames);
}


void		TerrMdl_SetDefaultSize()
{
	// terrmdl imagesize determination: 256 (low quality) or 512 (high quality)
	STRING* temp_str = str_create("");
	var file_handle = file_open_read(terrmdlsize_name);
	if (file_handle)
		{
			file_str_read( file_handle , temp_str );
			terrmdl_defaultsize = str_to_num( temp_str );
			file_close(file_handle);
		}
	else
		{
			terrmdl_defaultsize = TERRMDL_SIZE1;
		}
	ptr_remove(temp_str);
}


action 	TerrMdl_ModelPreview()
{
sys_marker("pv0");
	set(my, PASSABLE | LIGHT | CAST);	// | TRANSLUCENT
	my.flags2 |= UNTOUCHABLE;
	my.red = 0;
	my.green = 255;
	my.blue = 0;
//	my.alpha = 50;
	vec_scale( my.scale_x , GetMapTileSize()/terrmdl_defaultsize*terrmdl_tilesize );
	my.material = mtl_placementarea;
	my.group = 1;
	
//	VECTOR vTemp;
//	COLOR cTemp;
//	vec_set(cTemp,vector(0,255,200));
	
	while(UIMode==(var)UIMODE_TERRMDL)
		{		
			if (terrmdl_ready)
				{
					//-------------------------
					// mouse 3d position
					
					VECTOR* temp_vec = NULL;									// E1501 empty pointer error can happen if MousePos3Dx() directly sets a vector - returns NULL if no hit
					
					if (!key_alt)							
						{							
							temp_vec = MousePos3D1OrZeroLevel();			// onto ground terrain only
						}
					else																
						{
							temp_vec = MousePos3D4OrZeroLevel();			// on ground terrain + terrain mdl						
						}										
					
					if (temp_vec)
						{
							vec_set(my.x, temp_vec);
						}
					
					//-------------------------
					// align
					
					if (terrmdl_normal)
						{
sys_marker("pv3");								
							you = NULL;																		// set to invalid because of traces within one frame
							memset(hit, 0, sizeof(CONTACT));											// removes hit struct errors	
sys_marker("pv4");								
							// do tracing again, but now vertically, to get proper normal
							if (!key_alt)							
								{
									PosInfo3D1OrZeroLevel(my.x, my.y);
								}
							else
								{
									PosInfo3D4OrZeroLevel(my.x, my.y);									
								}
sys_marker("pv5");								
							if (HIT_TARGET)
								{
//									if (normal)																// causes error message in compiler, but compilation completes
										{
											vec_to_angle(my.pan, normal); 							// sets pan and tilt due to surface normal vector set by c_trace done by PosInfo3D
											my.tilt -= 90;													// rotate to be parallel instead of normal
										}
								}
						}
sys_marker("pv6");	
					//-------------------------
					// scale
					
					vec_scale( my.scale_x , GetMapTileSize()/terrmdl_defaultsize * terrmdl_scale / my.scale_x );	
					
					//-------------------------
					// preview colour
					
					if (my.z < 0)
						vec_set(my.blue,vector(255,0,0));		// blue if under water level - inside game area
					else
						vec_set(my.blue,vector(0,255,0));		// green if over water level - inside game area
					
					if (square_outofborder)
						vec_set(my.blue,vector(0,0,255));		// red if out of game area
					
					//-------------------------
				}
			
			wait(1);
		}
	
	TerrMdl_Close();
}


void TerrMdl_MorphPreviewModel()
{
sys_marker("mp0");
	terrmdl_ready = 0;
	
	STRING* temp_str = str_create("");
	if ( !str_cmpi(terrmdl_filenames[terrmdl_selected] , "-" ) )
		{					
			str_cpy( temp_str , terrmdl_foldername );
			str_cat( temp_str , terrmdl_filenames[terrmdl_selected] );
			if (file_exists(temp_str))
				ent_morph( terrmdl_model_preview , temp_str );
		}
	else
		{
			ent_morph( terrmdl_model_preview , "graphics\\null.mdl" );
		}
	
	wait(1);
	
	terrmdl_ready = 1;
}


void		TerrMdl_RotateModelPreview(int rotation)
{
	if (terrmdl_normal) return;
	
	int direction = AngleToDirection( terrmdl_model_preview.pan );
	
	if (rotation > 0)
		{
			direction = DirectionRotateLeft(direction);
			
			if (DirectionDiagonal)
				{
					direction = DirectionRotateLeft(direction);
				}
		}
	else if (rotation < 0)
		{
			direction = DirectionRotateRight(direction);
			
			if (DirectionDiagonal)
				{
					direction = DirectionRotateRight(direction);
				}
		}
	
	terrmdl_model_preview.pan = DirectionToAngle( direction );
}


void		TerrMdl_TurnModelPreview(int rotation)
{
	if (terrmdl_normal) return;
	
	var turnrate = rotation * time_step * 16;
	
	terrmdl_model_preview.pan += turnrate;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor menu


void		TerrMdl_Panel_Init()
{	
	int i;
	STRING* temp_string = str_create(""); 
	
	terrmdl_panel = pan_create( panel_image2 , 11 );
	set( terrmdl_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	terrmdl_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	terrmdl_panel.pos_y 	= ui_submenupanel_y;								// 256;
	terrmdl_panel.red		= panel_font_r;
	terrmdl_panel.green	= panel_font_g;
	terrmdl_panel.blue	= panel_font_b;
	terrmdl_panel.event	= ui_submenu_panelmove;
	
	// string 1
	pan_setstring(terrmdl_panel, 0,	15,10,	panel_font2,			str_create("TERRAIN MODEL PLACEMENT") );
	// string 2
	pan_setstring(terrmdl_panel, 0,	15,330,	panel_font,			str_create("Select item") );
	
	// strings 3-10 - terrmdl item numbers 1-8
	for(i=0;i<max_terrmdl_items;i++)
		{
			str_for_int( temp_string , (i+1) );
			pan_setstring( terrmdl_panel, 0,	35,350+i*15,	panel_font,			str_create( temp_string ) );
		}
	
	// strings 11-18 - terrmdl item filenames 1-8
	for(i=0;i<max_terrmdl_items;i++)
		{
			pan_setstring(terrmdl_panel, 0,	55,350+i*15,	panel_font,			str_create( terrmdl_filenames[i] ) );
		}
	
	// buttons 1-8 (checkbox)
	for(i=0;i<max_terrmdl_items;i++)
		{
			pan_setbutton(terrmdl_panel, 0,	2, 	15,350+i*15,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerrMdl_Flag_Toggle,NULL,NULL); 
		}
	// button states
	for(i=0;i<max_terrmdl_items;i++)
		{
			button_state( terrmdl_panel , 1+i , terrmdl_switch_type[i] ); 		// 1 ON, 0 OFF
		}
	
	// button 9 - unlit
	pan_setstring(terrmdl_panel, 0,	40,180,	panel_font,			str_create("set UNLIT") );	
	pan_setbutton(terrmdl_panel, 0,	2, 	15,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerrMdl_ToggleUnlit,NULL,NULL); 
	button_state( terrmdl_panel , 9 , terrmdl_unlit ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(terrmdl_panel, 0,	15,30,	"COUNT: %0.f",				panel_font,	1, terrmdl_counter);
	
	// buttons 10-11 - set
	pan_setdigits(terrmdl_panel, 0,	180,330,	"Set:    %0.f",		panel_font,	1, terrmdl_set);	
	pan_setbutton(terrmdl_panel, 0,	1, 	165,330,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerrMdl_DecSet, 		NULL, NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	225,330,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			TerrMdl_IncSet, 		NULL, NULL); 
	
	//	
	pan_setdigits(terrmdl_panel, 0,	15,200,	"Scale: %0.1f",		panel_font,	1, 		terrmdl_scale);
	pan_setslider(terrmdl_panel, 0,	90+12,202,	horiz_slider_bmp,slider_knob_bmp,	0.1,5,	terrmdl_scale);
	
	// button 12 - light
	pan_setstring(terrmdl_panel, 0,	40,100,	panel_font,			str_create("set LIGHT to adjust RGB values:") );	
	pan_setbutton(terrmdl_panel, 0,	2, 	15,100,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerrMdl_ToggleLight,NULL,NULL); 
	button_state( terrmdl_panel , 12 , terrmdl_light ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(terrmdl_panel, 0,	15,120,	"R: %0.f",				panel_font,	1, 		terrmdl_red);
	pan_setdigits(terrmdl_panel, 0,	15,140,	"G: %0.f",				panel_font,	1, 		terrmdl_green);
	pan_setdigits(terrmdl_panel, 0,	15,160,	"B: %0.f",				panel_font,	1, 		terrmdl_blue);
	pan_setslider(terrmdl_panel, 0,	90+12,122,	horiz_slider_bmp,slider_knob_bmp,	0,255,	terrmdl_red); 
	pan_setslider(terrmdl_panel, 0,	90+12,142,	horiz_slider_bmp,slider_knob_bmp,	0,255,	terrmdl_green); 
	pan_setslider(terrmdl_panel, 0,	90+12,162,	horiz_slider_bmp,slider_knob_bmp,	0,255,	terrmdl_blue); 
		
	pan_setdigits(terrmdl_panel, 0,	15,60,	"Ambient: %0.f",		panel_font,	1, 		terrmdl_ambient);
	pan_setslider(terrmdl_panel, 0,	90+12,62,	horiz_slider_bmp,slider_knob_bmp,	0,100,	terrmdl_ambient); 
	
//	pan_setdigits(terrmdl_panel, 0,	15,80,	"Alpha: %0.f",			panel_font,	1, 		terrmdl_alpha);
//	pan_setslider(terrmdl_panel, 0,	90+12,82,	horiz_slider_bmp,slider_knob_bmp,	0,100,	terrmdl_alpha); 
	
//	// button 15 - overlay
//	pan_setstring(terrmdl_panel, 0,	40,260,	panel_font,			str_create("set OVERLAY") );
//	pan_setbutton(terrmdl_panel, 0,	2, 	15,260,		checkbox_on_bmp,checkbox_off_bmp,checkbox_on_bmp,checkbox_off_bmp,		TerrMdl_ToggleOverlay,NULL,NULL); 
//	button_state( terrmdl_panel , 15 , terrmdl_overlay ); 		// 1 ON, 0 OFF	
	
	// button 13 - clear all
	pan_setbutton(terrmdl_panel, 0,	1, 	45,480,		m_clearall_on_bmp,m_clearall_off_bmp,m_clearall_over_bmp,m_clearall_over_bmp,		TerrMdl_DeleteAll,NULL,NULL); 
	
	// button 14 - material
//	pan_setbutton(terrmdl_panel, 0,	1, 	200,10,		ss_material_on_bmp,ss_material_off_bmp,ss_material_over_bmp,ss_material_over_bmp,		TerrMdl_MaterialSetup,NULL,NULL); 			
	pan_setbutton(terrmdl_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 15 - terrain material
	pan_setstring(terrmdl_panel, 0,	40+100,180,	panel_font,			str_create("set terrain material") );	
	pan_setbutton(terrmdl_panel, 0,	2, 	15+100,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerrMdl_ToggleModel,NULL,NULL); 
	button_state( terrmdl_panel , 15 , terrmdl_model ); 		// 1 ON, 0 OFF	
	
	// button 	16 17
//	pan_setbutton(terrmdl_panel, 0,	1, 	90,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerrMdl_Panel_Button, 		NULL, NULL); 
//	pan_setbutton(terrmdl_panel, 0,	1, 	210,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerrMdl_Panel_Button, 		NULL, NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	90,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerrMdl_Panel_Button, 		NULL, NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	210,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerrMdl_Panel_Button, 		NULL, NULL); 
	// button  18 19		20 21		22 23
	pan_setbutton(terrmdl_panel, 0,	1, 	90,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerrMdl_Panel_Button, 		NULL, NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	210,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerrMdl_Panel_Button, 		NULL, NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	90,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerrMdl_Panel_Button, 		NULL, NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	210,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerrMdl_Panel_Button, 		NULL, NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	90,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerrMdl_Panel_Button, 		NULL, NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	210,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerrMdl_Panel_Button, 		NULL, NULL); 
	// button 	24 25
	pan_setbutton(terrmdl_panel, 0,	1, 	90,60,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerrMdl_Panel_Button, 		NULL, NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	210,60,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerrMdl_Panel_Button, 		NULL, NULL); 
//	pan_setbutton(terrmdl_panel, 0,	1, 	90,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerrMdl_Panel_Button, 		NULL, NULL); 
//	pan_setbutton(terrmdl_panel, 0,	1, 	210,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerrMdl_Panel_Button, 		NULL, NULL); 
	
//	// button 33 - random rotation
//	pan_setstring(terrmdl_panel, 0,	40,280,	panel_font,			str_create("rnd rotation") );	
//	pan_setbutton(terrmdl_panel, 0,	2, 	15,280,		checkbox_on_bmp,checkbox_off_bmp,checkbox_on_bmp,checkbox_off_bmp,				TerrMdl_ToggleRndRot,NULL,NULL); 
//	button_state( terrmdl_panel , 33 , terrmdl_rnd_rot ); 		// 1 ON, 0 OFF	
	
	// button 26 - align surface (perp. to normal)
	pan_setstring(terrmdl_panel, 0,	40,220,	panel_font,			str_create("align surface") );	
	pan_setbutton(terrmdl_panel, 0,	2, 	15,220,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerrMdl_ToggleNormal,NULL,NULL); 
	button_state( terrmdl_panel , 26 , terrmdl_normal ); 		// 1 ON, 0 OFF	
	
	// button 27 - modify - removed
//	pan_setbutton(terrmdl_panel, 0,	1, 	150,480,		m_modify_on_bmp,m_modify_off_bmp,m_modify_over_bmp,m_modify_over_bmp,			ModifyObject_Init,		NULL,NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	255,580,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,	NULL,							NULL,NULL); 
	
//	// button 36 - set random fine scale
//	pan_setstring(terrmdl_panel, 0,	40,300,	panel_font,			str_create("rnd fine scale") );	
//	pan_setbutton(terrmdl_panel, 0,	2, 	15,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_on_bmp,checkbox_off_bmp,				TerrMdl_ToggleRandomize,NULL,NULL); 
//	button_state( terrmdl_panel , 36 , terrmdl_randomize ); 		// 1 ON, 0 OFF	
	
	// button 28 - dynamic shadow
	pan_setstring(terrmdl_panel, 0,	40,240,	panel_font,			str_create("set SHADOW") );	
	pan_setbutton(terrmdl_panel, 0,	2, 	15,240,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerrMdl_ToggleShadow,NULL,NULL); 
	button_state( terrmdl_panel , 28 , terrmdl_shadow ); 		// 1 ON, 0 OFF	
	
	// button 29 30 - Lightmap All : Set / Reset
	pan_setstring(terrmdl_panel, 0,			130,30,		panel_font,			str_create("Ligthmap All") );
	pan_setbutton(terrmdl_panel, 0,	1, 	200,20,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,					TerrMdl_LightmapAll,		NULL,NULL); 
	pan_setbutton(terrmdl_panel, 0,	1, 	200,40,		ss_reset_on_bmp,ss_reset_off_bmp,ss_reset_over_bmp,ss_reset_over_bmp,		TerrMdl_LightmapAll,		NULL,NULL); 
	
	//--------------------------------------------------------------
	
	ptr_remove( temp_string );	
	
//	while(UIMode==(var)UIMODE_TERRMDL)
//		{
//			wait(1);
//		}
//	TerrMdl_Close();
}


void		TerrMdl_Panel_Toggle()
{
	toggle( terrmdl_panel , SHOW );	
}


////////////////////////////////////////////////////


void		TerrMdl_Panel_Button(button_number)
{
//	// terrmdl_density
//	if (button_number==(var)19)
//		{
//			terrmdl_density = maxv( 1 , --terrmdl_density );
//		}
//	else if (button_number==(var)20)
//		{
//			terrmdl_density = minv( max_terrmdl_density , ++terrmdl_density );
//		}
	// terrmdl_scale
	if (button_number==(var)16)
		{
			terrmdl_scale -= 0.1;
			terrmdl_scale = maxv( 0.1 , terrmdl_scale );
		}
	else if (button_number==(var)17)
		{
			terrmdl_scale += 0.1;
			terrmdl_scale = minv( 2 , terrmdl_scale );
		}
	// terrmdl_red
	else if (button_number==(var)18)
		{
			terrmdl_red = maxv( 0 , --terrmdl_red );
		}
	else if (button_number==(var)19)
		{
			terrmdl_red = minv( 255 , ++terrmdl_red );
		}
	// terrmdl_green
	else if (button_number==(var)20)
		{
			terrmdl_green = maxv( 0 , --terrmdl_green );
		}
	else if (button_number==(var)21)
		{
			terrmdl_green = minv( 255 , ++terrmdl_green );
		}
	// terrmdl_blue
	else if (button_number==(var)22)
		{
			terrmdl_blue = maxv( 0 , --terrmdl_blue );
		}
	else if (button_number==(var)23)
		{
			terrmdl_blue = minv( 255 , ++terrmdl_blue );
		}
	// terrmdl_ambient
	else if (button_number==(var)24)
		{
			terrmdl_ambient = maxv( 0 , --terrmdl_ambient );
		}
	else if (button_number==(var)25)
		{
			terrmdl_ambient = minv( 100 , ++terrmdl_ambient );
		}
//	// terrmdl_alpha
//	else if (button_number==(var)31)
//		{
//			terrmdl_alpha = maxv( 0 , --terrmdl_alpha );
//		}
//	else if (button_number==(var)32)
//		{
//			terrmdl_alpha = minv( 100 , ++terrmdl_alpha );
//		}	
}


////////////////////////////////////////////////////


//// change by hotkey
//void		TerrMdl_NextSet()
//{ 
//	// jump to next existing set
//	terrmdl_set++;
//	if (terrmdl_set>max_terrmdl_sets)
//		{
//			terrmdl_set = 1;
//		}
//	TerrMdl_UpdateSetNames();
//	wait_for(TerrMdl_UpdateSetNames);
//}
//

void		TerrMdl_IncSet()
{
	if (proc_status(TerrMdl_IncSet) > 0) return;
	if (proc_status(TerrMdl_UpdateSetNames) > 0) return;
	if (!terrmdl_ready) return;
	terrmdl_ready = 0;
	wait(3);
	
	// jump to next existing set
	terrmdl_set++;
	if (terrmdl_set>max_terrmdl_sets)
		{
//			terrmdl_set = max_terrmdl_sets;
//			return;
			terrmdl_set = 1;
		}
	TerrMdl_UpdateSetNames();
	wait_for(TerrMdl_UpdateSetNames);
	
	wait(1);
}


void		TerrMdl_DecSet()
{
	if (proc_status(TerrMdl_IncSet) > 0) return;
	if (proc_status(TerrMdl_UpdateSetNames) > 0) return;
	if (!terrmdl_ready) return;
	terrmdl_ready = 0;
	wait(3);
	
	// jump to next existing set
	terrmdl_set--;
	if (terrmdl_set<1)
		{
//			terrmdl_set = 1;
//			return;
			terrmdl_set = max_terrmdl_sets;
		}
	TerrMdl_UpdateSetNames();
	wait_for(TerrMdl_UpdateSetNames);
	
	wait(1);
}


void		TerrMdl_UpdateSetNames()
{
	if (proc_status(TerrMdl_UpdateSetNames) > 0) return;
	
	int i;
	// copy the proper 8 filenames to string array
	int j = terrmdl_set - 1;
	int k = j * max_terrmdl_items;
	for(i=0;i<max_terrmdl_items;i++)
		{
//			str_cpy( terrmdl_filenames[i] , terrmdl_foldername );							// subfolder
//			str_cat( terrmdl_filenames[i] , (terrmdls_txt->pstring)[k+i] );			// filename with extension
			str_cpy( terrmdl_filenames[i] , "" );											// (or terrmdl_foldername) subfolder
			str_cat( terrmdl_filenames[i] , (terrmdls_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi( terrmdl_filenames[i] , "" ) )								// (or terrmdl_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( terrmdl_filenames[i] , "-" );
				}
		}
	// refresh 8 filename strings to display in menu
	for(i=0;i<max_terrmdl_items;i++)
		{
			pan_setstring(terrmdl_panel, 11+i,	55,350+i*15,	panel_font,			str_create( terrmdl_filenames[i] ) );
		}	
	wait(1);
	
	TerrMdl_MorphPreviewModel();
	wait_for(TerrMdl_MorphPreviewModel);
	wait(1);
}


///////////////////////////////////


// change by hotkey
void		TerrMdl_Flag_Switch(var setplusone)
{
	if (proc_status(TerrMdl_Flag_Switch) > 0) return;
	if (!terrmdl_ready) return;
	terrmdl_ready = 0;
	wait(3);
	
	int temp = setplusone -1 -1;	// -1 because of scancode, and -1 because of array pos are 0..7	
	int i;
	for(i=0;i<max_terrmdl_items;i++)
		{
			terrmdl_switch_type[i] = 0;						// 0..7
			button_state( terrmdl_panel , i+1 , 0 );		// 0..8
		}
	terrmdl_switch_type[temp] = 1;
	button_state( terrmdl_panel , temp+1 , 1 ); 
	terrmdl_selected = temp;
	
	TerrMdl_MorphPreviewModel();
	wait_for(TerrMdl_MorphPreviewModel);
	
	wait(1);
}


// change by checkbox (buttons 1-8)
void		TerrMdl_Flag_Toggle(button_number)
{
	if (proc_status(TerrMdl_Flag_Toggle) > 0) return;
	if (!terrmdl_ready) return;
	terrmdl_ready = 0;
	wait(3);
	
//	terrmdl_switch_type[button_number-1] = 1 - terrmdl_switch_type[button_number-1] ;
	
	int i;
	for(i=0;i<max_terrmdl_items;i++)
		{
			terrmdl_switch_type[i] = 0;							// 0..7
			button_state( terrmdl_panel , i+1 , 0 );			// 1..8
		}
	terrmdl_switch_type[button_number-1] = 1;
	button_state( terrmdl_panel , button_number , 1 ); 
	terrmdl_selected = button_number-1;
	
	TerrMdl_MorphPreviewModel();
	wait_for(TerrMdl_MorphPreviewModel);
	
	wait(1);
}


/////////////////////////////////////////////////////
// change by checkbox


void		TerrMdl_ToggleUnlit()
{
	terrmdl_unlit = 1 - terrmdl_unlit;	
	if (terrmdl_unlit)
		{
			// switch off LIGHT
			terrmdl_light = 0;
			button_state( terrmdl_panel , 12 , terrmdl_light );
			// switch off sprite
			terrmdl_model = 0;
			button_state( terrmdl_panel , 15 , terrmdl_model );
		}
}


void		TerrMdl_ToggleLight()
{
	terrmdl_light = 1 - terrmdl_light;	
	if (terrmdl_light)
		{
			// switch off UNLIT
			terrmdl_unlit = 0;
			button_state( terrmdl_panel , 9 , terrmdl_unlit );
		}
}


void		TerrMdl_ToggleModel()
{
	terrmdl_model = 1 - terrmdl_model;
	if (terrmdl_model)
		{
			// switch off UNLIT
			terrmdl_unlit = 0;
			button_state( terrmdl_panel , 9 , terrmdl_unlit );
		}
}


void		TerrMdl_ToggleNormal()
{
	terrmdl_normal = 1 - terrmdl_normal;	
	
//	if (terrmdl_normal)
//		{
//			// swithc off rnd rot
//			terrmdl_rnd_rot = 0;
//			button_state( terrmdl_panel , 33 , terrmdl_rnd_rot );
//		}
	
	// reset preview's angles
	if (!terrmdl_normal)
		{
			vec_fill(terrmdl_model_preview.pan, 0); 
		}
}


void		TerrMdl_ToggleShadow()
{
	terrmdl_shadow = 1 - terrmdl_shadow;		
}


void		TerrMdl_LightmapAll(button_number)
{
	set( map_loadpanel , SHOW);
	wait(3);
	
	var modified_lightmap = 0;
	
	if (button_number == (var)29)
		{
			modified_lightmap = 1;		// lightmap
		}
	else if (button_number == (var)30)
		{
			modified_lightmap = 0;		// dynamic shadow
		}
	else
		{
			return;
		}
	
	int counter = 0;
	ENTITY* temp_ent = ent_next(NULL); 																	// get first entity
	while(temp_ent)
		{
			if (temp_ent.ENT_CATEGORY == (var)UIMODE_TERRMDL)	
				{
					ModifyObject_UpdateShadows(temp_ent, modified_lightmap);
//					wait_for(ModifyObject_UpdateShadows);
				}
			
			temp_ent = ent_next(temp_ent); 																// get next entity
			
			counter++;
			counter %= 1000;
			if (counter == 0)
				{
					wait(1);
				}
		}
	temp_ent = NULL;
	
	wait(1);
	reset( map_loadpanel , SHOW);
	
	ptr_remove(temp_ent);
}


/////////////////////////////////////////////////////////


void		TerrMdl_MaterialSetup()
{
	wait(3);
	material_select = 17;
	Material_MatPanel_Init( mtl_terrmdl , NULL );
//	MaterialSetup_Init();				
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor placement


void	 TerrMdl_CreateObject()
{
	if (!terrmdl_ready) return;
	
	STRING* temp_str = str_create("");
	if ( !str_cmpi(terrmdl_filenames[terrmdl_selected] , "-" ) )
		{					
			str_cpy( temp_str , terrmdl_foldername );
			str_cat( temp_str , terrmdl_filenames[terrmdl_selected] );
			
			//----------------------------------------------------------------------------
			// create object
			
//			var my_scale = GetMapTileSize()/terrmdl_defaultsize * terrmdl_scale;
			var my_scale = terrmdl_model_preview.scale_x;	
			
			TerrMdl_Create(	temp_str, 												
									
									my_scale, my_scale, my_scale,		
									terrmdl_model_preview.x, terrmdl_model_preview.y, terrmdl_model_preview.z, 									
									terrmdl_model_preview.pan, terrmdl_model_preview.tilt, terrmdl_model_preview.roll,
									
									terrmdl_ambient, 
									terrmdl_light, terrmdl_red, terrmdl_green, terrmdl_blue, 
									terrmdl_unlit, terrmdl_model,	terrmdl_shadow
								);
//			wait_for(TerrMdl_Create);
//			
			//----------------------------------------------------------------------------
//			
//			ent_create( temp_str , terrmdl_model_preview.x , TerrMdl_CreateItem );
//			// to get its filename later: str_for_entfile( str_entfilename , my );	
//			
			//----------------------------------------------------------------------------
		}
//	wait(1);
	ptr_remove(temp_str);
}


void		TerrMdl_CloneObject()
{
	//-----------------------------------------------------------------------
	// select
	
	mod_origmenu = (var)UIMODE_TERRMDL;
	
	you = ModifyObject_SelectByClick();	
	
	if (you == NULL)
		{
			return;
		}
	
	//-----------------------------------------------------------------------
	// debug
	
////	printf( _chr(str_for_entfile(NULL, you)) );
//	draw_text( str_for_entfile(NULL, you), 		100, 		650 , COLOR_WHITE );
	
	//-----------------------------------------------------------------------
	// copy
	
	STRING* temp_str = str_create("");
	str_cpy( temp_str , terrmdl_foldername );
	str_cat( temp_str , str_for_entfile(NULL, you) );
	
	my = ent_create( temp_str , you.x , NULL );
	
	while(!my) {wait(1);}
	
//	my.link 			= you.link;
//	my.type 			= you.type;
	
	vec_set(my.pan, you.pan);
	vec_set(my.scale_x, you.scale_x);
	
	my.flags 		= you.flags;
	my.frame 		= you.frame;
	my.next_frame 	= you.next_frame;
	my.skin 			= you.skin;
	
	my.ambient 		= you.ambient;
	my.albedo 		= you.albedo;
	my.alpha 		= you.alpha;
	my.lightrange 	= you.lightrange;
	vec_set(my.blue, you.blue);
	
	my.emask 		= you.emask;
	my.eflags 		= you.eflags;
	
	vec_set(my.min_x, you.min_x);
	vec_set(my.max_x, you.max_x);
	
	my.trigger_range = you.trigger_range;
	my.push 			= you.push;
	my.floor_dist 	= you.floor_dist;
	my.smask 		= you.smask;
//	my.client_id 	= you.client_id;
	
	memcpy( my.skill, you.skill, 100 * sizeof(var) );
	
	my.pose 			= you.pose;
	my.material 	= you.material;
	my.u 				= you.u;
	my.v 				= you.v;
	my.group 		= you.group;
	my.flags2 		= you.flags2;
//	my.attachname 	= you.attachname;
	my.event 		= you.event;
	my.local 		= you.local;
	my.layer 		= you.layer;
	my.vmask 		= you.vmask;
//	my.string1 		= you.string1;
//	my.string2 		= you.string2;
	my.fRadius 		= you.fRadius;
	my.path_index 	= you.path_index;
//	my.model 		= you.model;
	my.shadow 		= you.shadow;
	my.parent 		= you.parent;
//	my.lightmap 	= you.lightmap;
//	my.body 			= you.body;
	my.clipfactor 	= you.clipfactor;
	
	//-----------------------------------------------------------------------
	// shadow - required by decal and ent_decal shadows
			
	TerrMdl_SetShadow(my, is(my, SHADOW));
	wait_for(TerrMdl_SetShadow);
	
	//-----------------------------------------------------------------------
	// level_ent
	
	if (!terrain_entity)
		{
			SentLevelEntSizeToEnt(my);
		}
	
	//-----------------------------------------------------------------------
	// modify
	
	ModifyObject_Select(my);
	
	//-----------------------------------------------------------------------
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor deletion


void		TerrMdl_DeleteItem()
{
	MousePos3D0();
	
	if (HIT_TARGET)
		{
//			you = hit.entity;
			
			TerrMdl_RemoveObject(hit.entity);
		}
}


void		TerrMdl_DeleteAll()
{
	wait(3);

	TerrMdl_RemoveAll();
	wait_for(TerrMdl_RemoveAll);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif