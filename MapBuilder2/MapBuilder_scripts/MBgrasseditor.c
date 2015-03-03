
#ifndef MBgrasseditor_c
#define MBgrasseditor_c

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


/////////////////////////////////////////////////////////////////////////////////////////
// editor initialization


void 		Grass_Init()
{		
	//----------------------------------------------------------------
	// grass imagesize determination: 256 (low quality) or 512 (high quality)
	Grass_SetDefaultSize();
	wait_for(Grass_SetDefaultSize);
	//----------------------------------------------------------------
	// grass sprites filename initialization 
sys_marker("gi1");	
	int arraysize = (int)GRASS_MAXPERSET;
	grass_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );
	int i;	
	// init filename string array elements
	for(i=0;i<max_grass_items;i++)
		{
			grass_filenames[i] = str_create("");
		}	
sys_marker(NULL);
	//-----------------------------------------------------------------
	// determine how many dds or tga files are in the folder, 
	// divide it by 8, and the result is the number of sets
//	grass_set = 1;
	max_grass_sets = GRASS_MAXSET;
	grasses_txt = txt_create( (max_grass_sets*max_grass_items) , 0 );		// 10*8=80
	for(i=0;i<max_grass_sets*max_grass_items;i++)
		{
			str_cpy( (grasses_txt->pstring)[i] , "" );
		}
	//-------------------------------------------------------------------
	// read in folder content
	i = 0;
	STRING* temp_str = str_create("");
	str_cpy( temp_str , grass_foldername );
	str_cat( temp_str , "*.mdl" );
	txt_for_dir( grasses_txt, temp_str );								
	while (str_len((grasses_txt->pstring)[i]) > 0)
		{
			i++;
		}
	max_grass_sets = integer(i/max_grass_items);
	if ( fraction(i/max_grass_items) > 0)
		{
			max_grass_sets++;		// allow to display not complete sets too
		}
	grass_set = clamp(grass_set, 1, max_grass_sets);
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	for(i=0;i<max_grass_items;i++)
		{
//			str_cpy( grass_filenames[i] , grass_foldername );							// subfolder
//			str_cat( grass_filenames[i] , (grasses_txt->pstring)[i] );				// filename with extension			
			str_cpy( grass_filenames[i] , "" );												// (or grass_foldername) subfolder
			int j = (grass_set - 1) * max_grass_items;
			str_cat( grass_filenames[i] , (grasses_txt->pstring)[j+i] );			// filename with extension
			if ( str_cmpi( grass_filenames[i] , "" ) )									// (or grass_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( grass_filenames[i] , "-" );
				}
		}	
	//------------------------------------------------------------------
	// initial value settings
	for(i=0;i<2;i++)
		{
			grass_switch_type[i] = 1;
		}
	for(i=4;i<8;i++)
		{
			grass_switch_type[i] = 0;
		}	
//	grass_tilesize = 1;
//	grass_density = 3;	
	//-------------------------------------------------------------------
	// create objects
	collision_mode = 0; 
	grass_placement_cube = ent_create( grass_creator_model , vector(square_coord_x,square_coord_y,square_coord_z) , Grass_Cube );
	wait(1);
	ent_clone(grass_placement_cube);		// CLO ?
	collision_mode = 1; 
//	grass_unlit = 0;
//	grass_light = 1;
	Grass_Panel_Init();
}


void		Grass_Close()
{
	ptr_remove(grass_placement_cube);
	grass_placement_cube = NULL;
	
	ptr_remove(grass_panel);
	grass_panel = NULL;
	
//	ptr_remove(mtl_grass);		
	//--------------------------------------------------------
	int i;
	// remove temp text
	for (i = 0; i < grasses_txt->strings; i++) 
		if ((grasses_txt->pstring)[i]) 
			ptr_remove((grasses_txt->pstring)[i]);
	ptr_remove(grasses_txt);
	grasses_txt = NULL;
	// remove temp string array
	for(i=0;i<max_grass_items;i++)
		{
			ptr_remove(grass_filenames[i]);
		}
	sys_free(grass_filenames);
	grass_filenames = NULL;
//	ptr_remove(grass_filenames);
}


action 	Grass_Cube()
{
	set(my, PASSABLE | TRANSLUCENT | CAST);
	my.flags2 |= UNTOUCHABLE;
	my.alpha = 50;
	vec_scale( my.scale_x , GetMapTileSize() / MAP_DEFAULTGRIDSIZE * grass_tilesize );
	//	my.skill1 = 1;	// actual scale size
	my.material = mtl_placementarea;
	
	VECTOR vTemp;
	COLOR cTemp;
	vec_set(cTemp,vector(0,255,200));
	
	while(UIMode==(var)UIMODE_GRASS)
		{
			//-------------------------
			// mouse 3d position
			
			VECTOR* temp_vec = NULL;
//			temp_vec = MousePos3D4OrZeroLevel();					// on ground terrain + terrain mdl
			if (!key_alt)							
				{							
					temp_vec = MousePos3D4OrZeroLevel();			// on ground terrain + terrain mdl
				}
			else																
				{
					temp_vec = MousePos3D2OrZeroLevel();			// on ground terrain + terrain mdl + building + mapent + blocks (= walkable surfaces)			
				}
			
			if (temp_vec)
				{
					vec_set(my.x, temp_vec);
				}
			
			// snap xy
			vec_set(my.x, SnapCoordToTileMidCoordXY(&my.x));
			
//			// follow tile square - moves only on terrain - better to use c_trace to sense terrain models
//			var posz = PosInfo3D2OrZeroLevel(square_coord_x, square_coord_y);
//			vec_set( my.x , vector(square_coord_x, square_coord_y, posz) );
			
			//-------------------------
			// draw cube
			
			vec_for_vertex(vTemp, me, 1);			
			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
			vec_for_vertex(vTemp, me, 2);
			draw_line3d(vTemp,cTemp,50);
			vec_for_vertex(vTemp, me, 3);
			draw_line3d(vTemp,cTemp,50);
			vec_for_vertex(vTemp, me, 4);
			draw_line3d(vTemp,cTemp,50);
			vec_for_vertex(vTemp, me, 1);
			draw_line3d(vTemp,cTemp,50);
			
			vec_for_vertex(vTemp, me, 1);			
			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
			vec_for_vertex(vTemp, me, 5);
			draw_line3d(vTemp,cTemp,50);
			
			vec_for_vertex(vTemp, me, 2);			
			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
			vec_for_vertex(vTemp, me, 6);
			draw_line3d(vTemp,cTemp,50);
			
			vec_for_vertex(vTemp, me, 3);			
			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
			vec_for_vertex(vTemp, me, 7);
			draw_line3d(vTemp,cTemp,50);
			
			vec_for_vertex(vTemp, me, 4);			
			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
			vec_for_vertex(vTemp, me, 8);
			draw_line3d(vTemp,cTemp,50);
			
			//-------------------------
			
			wait(1);
			
			//-------------------------
		}
	Grass_Close();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor menu


void		Grass_Panel_Init()
{	
	int i;
	STRING* temp_string = str_create(""); 
	
	grass_panel = pan_create( panel_image2 , 11 );
	set( grass_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	grass_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	grass_panel.pos_y 	= ui_submenupanel_y;								// 256;
	grass_panel.red		= panel_font_r;
	grass_panel.green		= panel_font_g;
	grass_panel.blue		= panel_font_b;
	grass_panel.event		= ui_submenu_panelmove;
	
	// string 1
	pan_setstring(grass_panel, 0,	15,10,	panel_font2,			str_create("GRASS MODEL PLACEMENT") );
	// string 2
	pan_setstring(grass_panel, 0,	15,330,	panel_font,			str_create("Toggle objects") );
	
	// strings 3-10 - grass item numbers 1-8
	for(i=0;i<max_grass_items;i++)
		{
			str_for_int( temp_string , (i+1) );
			pan_setstring( grass_panel, 0,	35,350+i*15,	panel_font,			str_create( temp_string ) );
		}
	
	// strings 11-18 - grass item filenames 1-8
	for(i=0;i<max_grass_items;i++)
		{
			pan_setstring(grass_panel, 0,	55,350+i*15,	panel_font,			str_create( grass_filenames[i] ) );
		}
	
	// buttons 1-8 (checkbox)
	for(i=0;i<max_grass_items;i++)
		{
			pan_setbutton(grass_panel, 0,	2, 	15,350+i*15,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Grass_Flag_Toggle,NULL,NULL); 
		}
	// button states
	for(i=0;i<max_grass_items;i++)
		{
			button_state( grass_panel , 1+i , grass_switch_type[i] ); 		// 1 ON, 0 OFF
		}
	
	// button 9 - unlit
	pan_setstring(grass_panel, 0,	40,180,	panel_font,			str_create("set UNLIT") );	
	pan_setbutton(grass_panel, 0,	2, 	15,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Grass_ToggleUnlit,NULL,NULL); 
	button_state( grass_panel , 9 , grass_unlit ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(grass_panel, 0,	15,30,	"COUNT: %0.f",				panel_font,	1, grass_counter);
	
	// buttons 10-11 - set
	pan_setdigits(grass_panel, 0,	180,330,	"Set:    %0.f",		panel_font,	1, grass_set);	
	pan_setbutton(grass_panel, 0,	1, 	165,330,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Grass_DecSet, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	225,330,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			Grass_IncSet, 		NULL, NULL); 
		
	// buttons 12-13 - area
	pan_setdigits(grass_panel, 0,	110+12,240,	"Area size:  %0.f",		panel_font,	1, grass_tilesize);	
	pan_setbutton(grass_panel, 0,	1, 	95+12,241,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Grass_DecSize, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	182+12,241,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			Grass_IncSize, 		NULL, NULL); 
		
	pan_setdigits(grass_panel, 0,	15,200,	"Density: %0.f",		panel_font,	1, 							grass_density);
	pan_setslider(grass_panel, 0,	90+12,202,	horiz_slider_bmp,slider_knob_bmp,	1,max_grass_density,	grass_density); 
	
	pan_setdigits(grass_panel, 0,	15,220,	"Scale: %0.1f",		panel_font,	1, 		grass_scale);
	pan_setslider(grass_panel, 0,	90+12,222,	horiz_slider_bmp,slider_knob_bmp,	0.1,2,	grass_scale);
	
	// button 14 - light
	pan_setstring(grass_panel, 0,	40,100,	panel_font,			str_create("set LIGHT to adjust RGB values:") );	
	pan_setbutton(grass_panel, 0,	2, 	15,100,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Grass_ToggleLight,NULL,NULL); 
	button_state( grass_panel , 14 , grass_light ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(grass_panel, 0,	15,120,	"R: %0.f",				panel_font,	1, 		grass_red);
	pan_setdigits(grass_panel, 0,	15,140,	"G: %0.f",				panel_font,	1, 		grass_green);
	pan_setdigits(grass_panel, 0,	15,160,	"B: %0.f",				panel_font,	1, 		grass_blue);
	pan_setslider(grass_panel, 0,	90+12,122,	horiz_slider_bmp,slider_knob_bmp,	0,255,	grass_red); 
	pan_setslider(grass_panel, 0,	90+12,142,	horiz_slider_bmp,slider_knob_bmp,	0,255,	grass_green); 
	pan_setslider(grass_panel, 0,	90+12,162,	horiz_slider_bmp,slider_knob_bmp,	0,255,	grass_blue); 
		
	pan_setdigits(grass_panel, 0,	15,60,	"Ambient: %0.f",		panel_font,	1, 		grass_ambient);
	pan_setslider(grass_panel, 0,	90+12,62,	horiz_slider_bmp,slider_knob_bmp,	0,100,	grass_ambient); 
	
	pan_setdigits(grass_panel, 0,	15,80,	"Alpha: %0.f",			panel_font,	1, 		grass_alpha);
	pan_setslider(grass_panel, 0,	90+12,82,	horiz_slider_bmp,slider_knob_bmp,	0,100,	grass_alpha); 
	
	// button 15 - overlay
	pan_setstring(grass_panel, 0,	40,260,	panel_font,			str_create("set OVERLAY") );
	pan_setbutton(grass_panel, 0,	2, 	15,260,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Grass_ToggleOverlay,NULL,NULL); 
	button_state( grass_panel , 15 , grass_overlay ); 		// 1 ON, 0 OFF	
	
	// button 16 - clear all
	pan_setbutton(grass_panel, 0,	1, 	45,480,		m_clearall_on_bmp,m_clearall_off_bmp,m_clearall_over_bmp,m_clearall_over_bmp,		Grass_DeleteAll,NULL,NULL); 
	
	// button 17 - material
//	pan_setbutton(grass_panel, 0,	1, 	200,10,		ss_material_on_bmp,ss_material_off_bmp,ss_material_over_bmp,ss_material_over_bmp,		Grass_MaterialSetup,NULL,NULL); 			
	pan_setbutton(grass_panel, 0,	1, 	200,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 18 - model material
	pan_setstring(grass_panel, 0,	40+100,180,	panel_font,			str_create("set model material") );	
	pan_setbutton(grass_panel, 0,	2, 	15+100,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Grass_ToggleModel,NULL,NULL); 
	button_state( grass_panel , 18 , grass_model ); 		// 1 ON, 0 OFF	
	
	// button 19,20	21,22		
	pan_setbutton(grass_panel, 0,	1, 	90,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	210,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	90,220,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	210,220,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Grass_Panel_Button, 		NULL, NULL); 
	// button 23,24	25,26		27,28
	pan_setbutton(grass_panel, 0,	1, 	90,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	210,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	90,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	210,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	90,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	210,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Grass_Panel_Button, 		NULL, NULL); 
	// button 29,30	31,32
	pan_setbutton(grass_panel, 0,	1, 	90,60,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	210,60,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	90,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Grass_Panel_Button, 		NULL, NULL); 
	pan_setbutton(grass_panel, 0,	1, 	210,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Grass_Panel_Button, 		NULL, NULL); 
	
	// button 33 - random rotation
	pan_setstring(grass_panel, 0,	40,280,	panel_font,			str_create("rnd rotation") );	
	pan_setbutton(grass_panel, 0,	2, 	15,280,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Grass_ToggleRndRot,NULL,NULL); 
	button_state( grass_panel , 33 , grass_rnd_rot ); 		// 1 ON, 0 OFF	
	
	// button 34 - align surface (perp. to normal)
	pan_setstring(grass_panel, 0,	140,280,	panel_font,			str_create("align surface") );	
	pan_setbutton(grass_panel, 0,	2, 	115,280,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Grass_ToggleNormal,NULL,NULL); 
	button_state( grass_panel , 34 , grass_normal ); 		// 1 ON, 0 OFF	
	
	// button 35 - modify - removed
//	pan_setbutton(grass_panel, 0,	1, 	177,480,		m_modify_on_bmp,m_modify_off_bmp,m_modify_over_bmp,m_modify_over_bmp,			ModifyObject_Init,		NULL,NULL); 
	pan_setbutton(grass_panel, 0,	1, 	177,580,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,	NULL,							NULL,NULL); 
	
	// button 36 - set random fine scale
	pan_setstring(grass_panel, 0,	40,300,	panel_font,			str_create("rnd fine scale") );	
	pan_setbutton(grass_panel, 0,	2, 	15,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Grass_ToggleRandomize,NULL,NULL); 
	button_state( grass_panel , 36 , grass_randomize ); 		// 1 ON, 0 OFF	
	
	// button 37 - seed
	pan_setbutton(grass_panel, 0,	1, 	150,480,		m_seed_on_bmp,m_seed_off_bmp,m_seed_over_bmp,m_seed_over_bmp,		Seed_Init,	NULL,NULL); 
	
	// button 38
	pan_setstring(grass_panel, 0,	40+100,260,	panel_font,			str_create("set SHADOW") );
	pan_setbutton(grass_panel, 0,	2, 	15+100,260,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Grass_ToggleShadow,NULL,NULL); 
	button_state( grass_panel , 38 , grass_shadow ); 		// 1 ON, 0 OFF	
	
	ptr_remove( temp_string );	
//	while(UIMode==(var)UIMODE_GRASS)
//		{
//			// reset overlay if alpha decreased
//			if ((grass_overlay) && (grass_alpha<100))
//				{
//					grass_overlay = 0;
//					button_state( grass_panel , 15 , grass_overlay ); 		// 1 ON, 0 OFF	
//				}
//			wait(3);
//		}
}


void		Grass_Panel_Toggle()
{
	toggle( grass_panel , SHOW );	
}


////////////////////////////////////////////////////


void		Grass_Panel_Button(button_number)
{
	// grass_density
	if (button_number==(var)19)
		{
			grass_density = maxv( 1 , --grass_density );
		}
	else if (button_number==(var)20)
		{
			grass_density = minv( max_grass_density , ++grass_density );
		}
	// grass_scale
	else if (button_number==(var)21)
		{
			grass_scale -= 0.1;
			grass_scale = maxv( 0.1 , grass_scale );
		}
	else if (button_number==(var)22)
		{
			grass_scale += 0.1;
			grass_scale = minv( 2 , grass_scale );
		}
	// grass_red
	else if (button_number==(var)23)
		{
			grass_red = maxv( 0 , --grass_red );
		}
	else if (button_number==(var)24)
		{
			grass_red = minv( 255 , ++grass_red );
		}
	// grass_green
	else if (button_number==(var)25)
		{
			grass_green = maxv( 0 , --grass_green );
		}
	else if (button_number==(var)26)
		{
			grass_green = minv( 255 , ++grass_green );
		}
	// grass_blue
	else if (button_number==(var)27)
		{
			grass_blue = maxv( 0 , --grass_blue );
		}
	else if (button_number==(var)28)
		{
			grass_blue = minv( 255 , ++grass_blue );
		}
	// grass_ambient
	else if (button_number==(var)29)
		{
			grass_ambient = maxv( 0 , --grass_ambient );
		}
	else if (button_number==(var)30)
		{
			grass_ambient = minv( 100 , ++grass_ambient );
		}
	// grass_alpha
	else if (button_number==(var)31)
		{
			grass_alpha = maxv( 0 , --grass_alpha );
		}
	else if (button_number==(var)32)
		{
			grass_alpha = minv( 100 , ++grass_alpha );
		}	
}


////////////////////////////////////////////////////


//// change by hotkey
//void		Grass_NextSet()
//{ 
//	// jump to next existing set
//	grass_set++;
//	if (grass_set>max_grass_sets)
//		{
//			grass_set = 1;
//		}
//	Grass_UpdateSetNames();
//	wait_for(Grass_UpdateSetNames);
//}


void		Grass_IncSet()
{ 
	// jump to next existing set
	grass_set++;
	if (grass_set>max_grass_sets)
		{
//			grass_set = max_grass_sets;
//			return;
			grass_set = 1;
		}
	Grass_UpdateSetNames();
	wait_for(Grass_UpdateSetNames);
}


void		Grass_DecSet()
{ 
	// jump to next existing set
	grass_set--;
	if (grass_set<1)
		{
//			grass_set = 1;
//			return;
			grass_set = max_grass_sets;
		}
	Grass_UpdateSetNames();
	wait_for(Grass_UpdateSetNames);
}


void		Grass_UpdateSetNames()
{
	int i;
	// copy the proper 8 filenames to string array
	int j = grass_set - 1;
	int k = j * max_grass_items;
	for(i=0;i<max_grass_items;i++)
		{
//			str_cpy( grass_filenames[i] , grass_foldername );							// subfolder
//			str_cat( grass_filenames[i] , (grasses_txt->pstring)[k+i] );			// filename with extension
			str_cpy( grass_filenames[i] , "" );											// (or grass_foldername) subfolder
			str_cat( grass_filenames[i] , (grasses_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi( grass_filenames[i] , "" ) )								// (or grass_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( grass_filenames[i] , "-" );
				}
		}
	// refresh 8 filename strings to display in menu
	for(i=0;i<max_grass_items;i++)
		{
			pan_setstring(grass_panel, 11+i,	55,350+i*15,	panel_font,			str_create( grass_filenames[i] ) );
		}	
	wait(1);
}


///////////////////////////////////


// change by hotkey
void		Grass_Flag_Switch(var setplusone)
{
	int temp = setplusone -1 -1;	// -1 because of scancode, and -1 because of array pos are 0..7
	grass_switch_type[temp] = 1 - grass_switch_type[temp];
	// change checkbox state too
	var temp_state = button_state( grass_panel , temp+1 , -1 ); 				// -1 to read value
	button_state( grass_panel , temp+1 , 1-temp_state ); 						// 1 ON, 0 OFF
	
	wait(1);
}


// change by checkbox
void		Grass_Flag_Toggle(button_number)
{
	grass_switch_type[button_number-1] = 1 - grass_switch_type[button_number-1] ;
	
	wait(1);
}


/////////////////////////////////
// decrease/increase grass densityif left mouse button is used for placement


void 		Grass_DecDens()
{
	grass_density--;
	if (grass_density<1)
		{
			grass_density = 1;
		}
}


void 		Grass_IncDens()
{
	grass_density++;
	if (grass_density>max_grass_density)
		{
			grass_density = max_grass_density;
		}
}


////////////////////////////////////


// change by hotkey
void 		Grass_DecSize()
{
	grass_tilesize -= 2;
	if (grass_tilesize<1)
		{
			grass_tilesize = 1;
		}	
	var temp = grass_tilesize / grass_placement_cube.scale_x ;	
	vec_scale( grass_placement_cube.scale_x , temp );
}


// change by hotkey
void 		Grass_IncSize()
{
	grass_tilesize += 2;
	if (grass_tilesize>max_grass_size)
		{
			grass_tilesize = max_grass_size;
		}
	var temp = grass_tilesize / grass_placement_cube.scale_x ;	
	vec_scale( grass_placement_cube.scale_x , temp );
}


/////////////////////////////////////////////////////
// change by checkbox


void		Grass_ToggleUnlit()
{
	grass_unlit = 1 - grass_unlit;	
	if (grass_unlit)
		{
			// switch off LIGHT
			grass_light = 0;
			button_state( grass_panel , 14 , grass_light );
			// switch off sprite
			grass_model = 0;
			button_state( grass_panel , 18 , grass_model );
		}
}


void		Grass_ToggleLight()
{
	grass_light = 1 - grass_light;	
	if (grass_light)
		{
			// switch off UNLIT
			grass_unlit = 0;
			button_state( grass_panel , 9 , grass_unlit );
		}
}


void		Grass_ToggleModel()
{
	grass_model = 1 - grass_model;
	if (grass_model)
		{
			// switch off UNLIT
			grass_unlit = 0;
			button_state( grass_panel , 9 , grass_unlit );
		}
}


void		Grass_ToggleOverlay()
{
	grass_overlay = 1 - grass_overlay;	
//	if (grass_overlay)
//		{
//			grass_alpha = 100;
//		}
}


void		Grass_ToggleRndRot()
{
	grass_rnd_rot = 1 - grass_rnd_rot;	
	
	if (grass_rnd_rot)
		{
			// swithc off normal
			grass_normal = 0;
			button_state( grass_panel , 34 , grass_normal );
		}
}


void		Grass_ToggleNormal()
{
	grass_normal = 1 - grass_normal;	
	
	if (grass_normal)
		{
			// swithc off rnd rot
			grass_rnd_rot = 0;
			button_state( grass_panel , 33 , grass_rnd_rot );
		}
}


void		Grass_ToggleRandomize()
{
	grass_randomize = 1 - grass_randomize;	
}


void		Grass_ToggleShadow()
{
	grass_shadow = 1 - grass_shadow;	
}


/////////////////////////////////////////////////////////


void		Grass_MaterialSetup()
{
	wait(3);
	material_select = 14;
	Material_MatPanel_Init( mtl_grass , NULL );
//	MaterialSetup_Init();				
}


//////////////////////////////////////////////////////////////////////
// editor placement

void	Grass_OnClick()
{
	if (proc_status(Grass_OnClick)>1)		// protection
		{
			return;
		}
	wait(-0.25);
	
	if (!mouse_left)								// single click
		{
			Grass_CreateArea();
		}
	else												// holding button
		{
			if (proc_status(Grass_CreateArea_Cont)<1)
				{
					Grass_CreateArea_Cont();
				}
		}
}


void	 Grass_CreateArea()
{
	int i,k,temp_int;
	var j;
	STRING* temp_str = str_create("");
	random_seed(0);
	collision_mode = 0; 
	// check number of types to be used to increase loop length if less than 8, or exit if 0
	j = 0;
	for (i=0;i<max_grass_items;i++)
		{
			if(grass_switch_type[i])
			{
				j++;
			}
		}
	if (j==0) { return; }
	// set loop limit
	j = integer( max_grass_items/j );
	j *= integer(grass_density);			// *grass_tilesize*grass_tilesize - now need less than in case of sprites
	//	loop length depends on grass_density and grass_tilesize
	for ( i=0 ; i<j ; i++)
		{
			k = i%max_grass_items;
			if ((grass_switch_type[k]) && (!str_cmpi(grass_filenames[k],"-")))
				{					
					str_cpy( temp_str , grass_foldername );
					str_cat( temp_str , grass_filenames[k] );
					
					// set random pos within creator cube area and shrub_tilesize
					var tempx = grass_placement_cube.x + random(grass_tilesize*GetMapTileSize()) - grass_tilesize*GetMapTileSize()/2;
					var tempy = grass_placement_cube.y + random(grass_tilesize*GetMapTileSize()) - grass_tilesize*GetMapTileSize()/2;					
					
					// set z value 
					var tempz = 0;
					if (!key_alt)							
						{							
							tempz = PosInfo3D4OrZeroLevel(tempx, tempy);			// on ground terrain + terrain mdl
						}
					else																
						{
							tempz = PosInfo3D2OrZeroLevel(tempx, tempy);			// on ground terrain + terrain mdl + building + mapent + blocks (= walkable surfaces)
						}	
					
					// no snap to tile middle
					Grass_CreateObject(temp_str, vector(tempx,tempy,tempz));
//					ent_create( temp_str , vector(tempx,tempy,tempz) , Grass_CreateItem );
					// to get its filename later: str_for_entfile( str_entfilename , my );		
				}
		}
//	wait(1);
	ptr_remove(temp_str);
	collision_mode = 1; 
}


void		Grass_CreateArea_Cont()				// not used and not updated
{
	if (proc_status(Grass_CreateArea_Cont)>1)	return;
	
	int i,k;
	var j;
	STRING* temp_str = str_create("");
	random_seed(0);
	collision_mode = 0; 
	// check number of types to be used to increase loop length if less than 8, or exit if 0
	j = 0;
	for (i=0;i<GRASS_MAXPERSET;i++)
		{
			if(grass_switch_type[i])
			{
				j++;
			}
		}
	if (j==0) { return; }
	i = 0;
	while (mouse_left)
		{
			k = i%max_grass_items;
			if ((grass_switch_type[k]) && (!str_cmpi(grass_filenames[k],"-")))
				{					
					str_cpy( temp_str , grass_foldername );
					str_cat( temp_str , grass_filenames[k] );
					
					// set random pos within creator cube area and shrub_tilesize
					var tempx = grass_placement_cube.x + random(grass_tilesize*GetMapTileSize()) - grass_tilesize*GetMapTileSize()/2;
					var tempy = grass_placement_cube.y + random(grass_tilesize*GetMapTileSize()) - grass_tilesize*GetMapTileSize()/2;					
					
					// set z value 
					var tempz = 0;
					if (!key_alt)							
						{							
							tempz = PosInfo3D4OrZeroLevel(tempx, tempy);			// on ground terrain + terrain mdl
						}
					else																
						{
							tempz = PosInfo3D2OrZeroLevel(tempx, tempy);			// on ground terrain + terrain mdl + building + mapent + blocks (= walkable surfaces)
						}	
					
					// no snap to tile middle
					Grass_CreateObject(temp_str, vector(tempx,tempy,tempz));
//					ent_create( temp_str , vector(tempx,tempy,tempz) , Grass_CreateItem );
					// to get its filename later: str_for_entfile( str_entfilename , my );																		// okay?	- maybe read only				
					
					wait(-0.025);			// sets placement speed
				}
			i++;			
		}	
	collision_mode = 1; 
	ptr_remove(temp_str);
}


/////////////////////////////////////////////////////////////
// seeding


void		Grass_SeedQuantity()
{
	set( map_loadpanel , SHOW);
	wait(3);
	//----------------------------------------
	STRING* temp_str = str_create("");
	int i, j, k, m = 0;
	
	//----------------------------------------
	// check number of types to be used, to exit if 0	
	
	for (i=0;i<max_grass_items;i++)
		{
			if(grass_switch_type[i])
			{
				j++;
			}
		}
	if (j==0) 
		{ 
			reset( map_loadpanel , SHOW);
			return; 
		}
	
	//----------------------------------------
	var outer_areax = 0;
	var outer_areay = 0;
	var inner_areax = 0;
	var inner_areay = 0;
	
	if (seed_area == (var)0)									// full area
		{
			outer_areax = TerrHmp_GetGroundMaxX();
			outer_areay = TerrHmp_GetGroundMaxY();
		}
	else if (seed_area == (var)1)								// game area
		{
			outer_areax = GetMapTilesX() * GetMapTileSize() / 2;
			outer_areay = GetMapTilesY() * GetMapTileSize() / 2;
		}
	else // if (seed_area == (var)2)							// border area
		{
			outer_areax = TerrHmp_GetGroundMaxX();
			outer_areay = TerrHmp_GetGroundMaxY();
			
			inner_areax = GetMapTilesX() * GetMapTileSize() / 2;
			inner_areay = GetMapTilesY() * GetMapTileSize() / 2;
		}
	//----------------------------------------
	collision_mode = 0; 
	
	for (i=0;i<seed_quantity;i++)
		{
			var posx;
			var posy;
			var posz;
			int temp_int;
			var summed_bool = 0;
			
			// get a valid position due to constrains
			do 
				{
					posx = random(outer_areax*2)-outer_areax;
					posy = random(outer_areay*2)-outer_areay;
					
					// no snap to tile middle					
					
					posz = PosInfo3D4OrZeroLevel(posx, posy);
					
					// no snap to tilesize downwards					
					
					// evaluate position data due to constraints					
					summed_bool = 0;
					ANGLE normal_ang;
					vec_zero(normal_ang);
					vec_to_angle(normal_ang, normal.x);
					summed_bool = (posz < seed_minh) || (posz > seed_maxh) || (90-normal_ang.tilt > seed_maxslope)	||	(90-normal_ang.tilt < seed_minslope);	
//					summed_bool = (posz < seed_minh) || (posz > seed_maxh) || (normal.z < ((var)1 - seed_maxslope))	||	(normal.z > ((var)1 - seed_minslope));					
					if (seed_area == (var)2)
						{
							summed_bool += (posx > -inner_areax) && (posx < inner_areax) && (posy > -inner_areay) && (posy < inner_areay);
						}
				}
			while(summed_bool);
			
			// get a selected entity file
			do
				{
					m++;
					k = m % max_grass_items;
				}
			while((!grass_switch_type[k]) || (str_cmpi(grass_filenames[k],"-")));	// repeat until nonselected or empty item found							
				
			str_cpy( temp_str , grass_foldername );
			str_cat( temp_str , grass_filenames[k] );	
			
			// place entity
			Grass_CreateObject(temp_str, vector(posx,posy,posz));
//			ent_create( temp_str , vector(posx,posy,posz) , Grass_CreateItem );	
			
			// progress bar
			if (i%100==0)		// at every 100th entity loading
				{
					Map_Progressbar(i/seed_quantity*100);
					wait_for(Map_Progressbar);
					wait(1);
				}	
		}
	//----------------------------------------
	wait(1);
	collision_mode = 1; 
	ptr_remove(temp_str);
	reset(map_loadbar,SHOW); 
	reset( map_loadpanel , SHOW);
}


void		Grass_SeedArea()
{
	set( map_loadpanel , SHOW);
	wait(3);
	//----------------------------------------
	STRING* temp_str = str_create("");	
	
	//----------------------------------------
	// check number of types to be used, to exit if 0	
	
	int a, b = 0;
	
	for (a=0;a<max_grass_items;a++)
		{
			if(grass_switch_type[a])
			{
				b++;
			}
		}
	if (b==0) 
		{ 
			reset( map_loadpanel , SHOW);
			return; 
		}
	
	//----------------------------------------
	var outer_areax = 0;
	var outer_areay = 0;
	var inner_areax = 0;
	var inner_areay = 0;
	
	if (seed_area == (var)0)									// full area
		{
			outer_areax = TerrHmp_GetGroundMaxX();
			outer_areay = TerrHmp_GetGroundMaxY();
		}
	else if (seed_area == (var)1)								// game area
		{
			outer_areax = GetMapTilesX() * GetMapTileSize() / 2;
			outer_areay = GetMapTilesY() * GetMapTileSize() / 2;
		}
	else // if (seed_area == (var)2)							// border area
		{
			outer_areax = TerrHmp_GetGroundMaxX();
			outer_areay = TerrHmp_GetGroundMaxY();
			
			inner_areax = GetMapTilesX() * GetMapTileSize() / 2;
			inner_areay = GetMapTilesY() * GetMapTileSize() / 2;
		}
	//----------------------------------------
	var i,j,posx, posy;
	int c = 0;
	int temp_int;
	var summed_bool = 0;
	random_seed(0);
	collision_mode = 0; 
	int m = 0;
	int k = 0;
	
	for (i = -outer_areax + seed_distance/2; i < outer_areax - seed_distance/2; i+=seed_distance)
		{
			for (j = -outer_areay + seed_distance/2; j < outer_areay - seed_distance/2; j+=seed_distance)
				{
					// get a position					
					posx = i + random(seed_rnddist*2) - seed_rnddist;
					posy = j + random(seed_rnddist*2) - seed_rnddist;
					
					// no snap to tile middle					
					
					var posz = PosInfo3D4OrZeroLevel(posx, posy);
					
					// no snap to tilesize downwards					
					
					// evaluate position data due to rules
					summed_bool = 0;
					ANGLE normal_ang;
					vec_zero(normal_ang);
					vec_to_angle(normal_ang, normal);
					summed_bool = (posz < seed_minh) || (posz > seed_maxh) || (90-normal_ang.tilt > seed_maxslope)	||	(90-normal_ang.tilt < seed_minslope);	
//					summed_bool = (posz < seed_minh) || (posz > seed_maxh) || (normal.z < ((var)1 - seed_maxslope))	||	(normal.z > ((var)1 - seed_minslope));					
					if (seed_area == (var)2)
						{
							summed_bool += (posx > -inner_areax) && (posx < inner_areax) && (posy > -inner_areay) && (posy < inner_areay);
						}
					
					// when position is valid
					if ( !summed_bool )					
						{
							// get a selected entity file							
							do
								{
									m++;
									k = m % max_grass_items;
								}
							while((!grass_switch_type[k]) || (str_cmpi(grass_filenames[k],"-")));	// repeat until nonselected or empty item found							
								
							str_cpy( temp_str , grass_foldername );
							str_cat( temp_str , grass_filenames[k] );									
							
							// place entity
							Grass_CreateObject(temp_str, vector(posx,posy,posz));
//							ent_create( temp_str , vector(posx,posy,posz) , Grass_CreateItem );	
							c++;
							
							// progress bar
							if (c%100==0)		// at every 100th entity loading
								{
									Map_Progressbar(i / outer_areax * 100);
									wait_for(Map_Progressbar);
									wait(1);
								}	
						}																	
				}
			
			// interrupt loop
			if (c > seed_limit) break;		// safety limit
			if (key_b) break;					// safety exit similar to teredit lightmapping
			
			wait(1);
		}
	//----------------------------------------	
	wait(1);
	collision_mode = 1; 
	ptr_remove(temp_str);
	reset(map_loadbar,SHOW); 
	reset( map_loadpanel , SHOW);
}


///////////////////////////////////////////////


void	 Grass_CreateObject(STRING* ent_name_full, VECTOR* pos_vec)
{		
	STRING* temp_str = str_create("");
	str_cpy(temp_str, ent_name_full);
	
	//----------------------------------------------------------------------------
	// create object
		
	var my_scale = GetMapTileSize()/grass_defaultsize * grass_scale;
	
	var my_scale_x = my_scale;
	var my_scale_y = my_scale;
	var my_scale_z = my_scale;
	
	if (grass_randomize)
		{
//			// uniform randomness to keep object size ratio
//			my_scale *= (1+(random(20)-10)/100);
			
			// 3-fold randomness
			my_scale_x *= (1+(random(30)-15)/100);
			my_scale_y *= (1+(random(30)-15)/100);
			my_scale_z *= (1+(random(30)-15)/100);
		}
	
	//---
	
	ANGLE my_angle;
	vec_zero(my_angle);
	
	if (grass_normal)
		{
			// align to surface by setting pan and tilt
			// use the c_trace result made definitely before
			vec_to_angle (my_angle.pan, normal); 							// sets pan and tilt due to surface normal vector set by c_trace done by PosInfo3D
			my_angle.tilt -= 90;													// rotate to be parallel instead of normal
		}
	else if (grass_rnd_rot)
		{
			my_angle.pan = random(360) -180;
		}
	
	//---
	
	Grass_Create(	temp_str, 																		
						my_scale_x, my_scale_y, my_scale_z,		
						pos_vec.x, pos_vec.y, pos_vec.z, 		
						my_angle.pan, my_angle.tilt, my_angle.roll,									
						grass_alpha, grass_ambient, 
						grass_light, grass_red, grass_green, grass_blue, 
						grass_unlit, grass_overlay, grass_model,										
						grass_shadow);
//			wait_for(Grass_Create);
	
	//----------------------------------------------------------------------------
	
//	wait(1);
	ptr_remove(temp_str);
}


///////////////////////////////////////////////////////////////////////////////////
// deletion


void		Grass_DeleteArea()
{
	Grass_RemoveArea(square_x, square_y, grass_tilesize, grass_tilesize);	
}


void		Grass_DeleteAll()
{
	wait(3);

	Grass_RemoveAll();
	wait_for(Grass_RemoveAll);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif