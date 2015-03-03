
#ifndef MBdetaileditor_c
#define MBdetaileditor_c

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


////////////////////////////////////////////////////////////////////////////////////////////////////////////


void 		Detail_Init()
{		
	//----------------------------------------------------------------
	// detail imagesize determination: 256 (low quality) or 512 (high quality)
	Detail_SetDefaultSize();
	wait_for(Detail_SetDefaultSize);
	//----------------------------------------------------------------
	// detail sprites filename initialization 
sys_marker("di1");	
	int arraysize = (int)DETAIL_MAXPERSET;
	detail_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );
	int i;	
	// init filename string array elements
	for(i=0;i<max_detail_items;i++)
		{
			detail_filenames[i] = str_create("");
		}	
sys_marker(NULL);
	//-----------------------------------------------------------------
	// determine how many dds or tga files are in the folder, 
	// divide it by 8, and the result is the number of sets
//	detail_set = 1;
	max_detail_sets = DETAIL_MAXSET;
	details_txt = txt_create( (max_detail_sets*max_detail_items) , 0 );		// 10*8=80		// emtpy string said to be created automatically
	for(i=0;i<max_detail_sets*max_detail_items;i++)
		{
			str_cpy( (details_txt.pstring)[i] , "" );
		}
	//-------------------------------------------------------------------
	// read in folder content
	i = 0;
	STRING* temp_str = str_create("");
	str_cpy( temp_str , detail_foldername );
	str_cat( temp_str , "*.dds" );
	txt_for_dir( details_txt, temp_str );								
	while (str_len((details_txt.pstring)[i]) > 0) // && (i<max_detail_sets*max_detail_items))			// count them
		{
			i++;
		}
	max_detail_sets = integer(i/max_detail_items);
	if ( fraction(i/max_detail_items) > 0)
		{
			max_detail_sets++;		// allow to display not complete sets too
		}
	detail_set = clamp(detail_set, 1, max_detail_sets);
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	for(i=0;i<max_detail_items;i++)
		{
//			str_cpy( detail_filenames[i] , detail_foldername );							// subfolder
//			str_cat( detail_filenames[i] , (details_txt->pstring)[i] );					// filename with extension			
			str_cpy( detail_filenames[i] , "" );												// (or detail_foldername) subfolder
			int j = (detail_set - 1) * max_detail_items;
			str_cat( detail_filenames[i] , (details_txt.pstring)[j+i] );				// filename with extension
			if ( str_cmpi( detail_filenames[i] , "" ) )										// (or detail_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( detail_filenames[i] , "-" );
				}
		}	
	//------------------------------------------------------------------
	// initial value settings
	for(i=0;i<4;i++)
		{
			detail_switch_type[i] = 1;
		}
	for(i=4;i<8;i++)
		{
			detail_switch_type[i] = 0;
		}	
//	detail_tilesize = 1;
//	detail_density = 3;	
	//-------------------------------------------------------------------
	// create objects
	collision_mode = 0; 
	detail_placement_cube = ent_create( detail_creator_model , vector(square_coord_x,square_coord_y,square_coord_z) , Detail_Cube );
	wait(1);
//	ent_clone(detail_placement_cube);		// CLO ?
	collision_mode = 1; 
//	detail_unlit = 0;
//	detail_light = 1;
	Detail_Panel_Init();
}


void		Detail_Close()
{
	ptr_remove(detail_placement_cube);
	detail_placement_cube = NULL;
	
	ptr_remove(detail_panel);
	detail_panel = NULL;
	
//	ptr_remove(mtl_detail);		
	//--------------------------------------------------------
	int i;
	// remove temp text
	for (i = 0; i < details_txt.strings; i++) 
		{
			if ((details_txt.pstring)[i]) 
				{
					str_remove((details_txt.pstring)[i]);
				}
		}
	ptr_remove(details_txt);
	details_txt = NULL;
	//--------------------------------------------------------
	// remove temp string array
	for(i=0;i<max_detail_items;i++)
		{
			ptr_remove(detail_filenames[i]);
		}
	sys_free(detail_filenames);
	detail_filenames = NULL;
//	ptr_remove(detail_filenames);
}


action 	Detail_Cube()
{
	set(my, PASSABLE | TRANSLUCENT | CAST);
	my.flags2 |= UNTOUCHABLE;
	my.alpha = 50;
	vec_scale( my.scale_x , GetMapTileSize() / MAP_DEFAULTGRIDSIZE * detail_tilesize );
	//	my.skill1 = 1;	// actual scale size
	my.material = mtl_placementarea;
	
	VECTOR vTemp;
	COLOR cTemp;
	vec_set(cTemp,vector(0,255,0));
	
	while(UIMode==(var)UIMODE_DETAIL)
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
	Detail_Close();
}


//////////////////////////////////////////////////////////////////////////////////////////////////


void		Detail_Panel_Init()
{	
	int i;
	STRING* temp_string = str_create(""); 
	
	detail_panel = pan_create( panel_image2 , 11 );
	set( detail_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	detail_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	detail_panel.pos_y 	= ui_submenupanel_y;								// 256;
	detail_panel.red		= panel_font_r;
	detail_panel.green	= panel_font_g;
	detail_panel.blue		= panel_font_b;
	detail_panel.event	= ui_submenu_panelmove;
	
	// string 1
	pan_setstring(detail_panel, 0,	15,10,	panel_font2,			str_create("GRASS SPRITE PLACEMENT") );
	// string 2
	pan_setstring(detail_panel, 0,	15,330,	panel_font,			str_create("Toggle objects") );
	
	// strings 3-10 - detail item numbers 1-8
	for(i=0;i<max_detail_items;i++)
		{
			str_for_int( temp_string , (i+1) );
			pan_setstring( detail_panel, 0,	35,350+i*15,	panel_font,			str_create( temp_string ) );
		}
	
	// strings 11-18 - detail item filenames 1-8
	for(i=0;i<max_detail_items;i++)
		{
			pan_setstring(detail_panel, 0,	55,350+i*15,	panel_font,			str_create( detail_filenames[i] ) );
		}
	
	// buttons 1-8 (checkbox)
	for(i=0;i<max_detail_items;i++)
		{
			pan_setbutton(detail_panel, 0,	2, 	15,350+i*15,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Detail_Flag_Toggle,NULL,NULL); 
		}
	// button states
	for(i=0;i<max_detail_items;i++)
		{
			button_state( detail_panel , 1+i , detail_switch_type[i] ); 		// 1 ON, 0 OFF
		}
	
	// button 9
	pan_setstring(detail_panel, 0,	40,180,	panel_font,			str_create("set UNLIT") );	
	pan_setbutton(detail_panel, 0,	2, 	15,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Detail_ToggleUnlit,NULL,NULL); 
	button_state( detail_panel , 9 , detail_unlit ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(detail_panel, 0,	15,30,	"COUNT: %0.f",				panel_font,	1, detail_counter);
	
	// buttons 10-11
	pan_setdigits(detail_panel, 0,	180,330,	"Set:    %0.f",		panel_font,	1, detail_set);	
	pan_setbutton(detail_panel, 0,	1, 	165,330,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Detail_DecSet, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	225,330,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			Detail_IncSet, 		NULL, NULL); 
		
	// buttons 12-13
	pan_setdigits(detail_panel, 0,	110+12,240,	"Area size:  %0.f",		panel_font,	1, detail_tilesize);	
	pan_setbutton(detail_panel, 0,	1, 	95+12,241,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Detail_DecSize, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	182+12,241,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			Detail_IncSize, 		NULL, NULL); 
		
	pan_setdigits(detail_panel, 0,	15,200,	"Density: %0.f",		panel_font,	1, 							detail_density);
	pan_setslider(detail_panel, 0,	90+12,202,	horiz_slider_bmp,slider_knob_bmp,	1,max_detail_density,	detail_density); 
	
	pan_setdigits(detail_panel, 0,	15,220,	"Scale: %0.1f",		panel_font,	1, 		detail_scale);
	pan_setslider(detail_panel, 0,	90+12,222,	horiz_slider_bmp,slider_knob_bmp,	0.1,2,	detail_scale);
	
	// button 14
	pan_setstring(detail_panel, 0,	40,100,	panel_font,			str_create("set LIGHT to adjust RGB values:") );	
	pan_setbutton(detail_panel, 0,	2, 	15,100,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Detail_ToggleLight,NULL,NULL); 
	button_state( detail_panel , 14 , detail_light ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(detail_panel, 0,	15,120,	"R: %0.f",				panel_font,	1, 		detail_red);
	pan_setdigits(detail_panel, 0,	15,140,	"G: %0.f",				panel_font,	1, 		detail_green);
	pan_setdigits(detail_panel, 0,	15,160,	"B: %0.f",				panel_font,	1, 		detail_blue);
	pan_setslider(detail_panel, 0,	90+12,122,	horiz_slider_bmp,slider_knob_bmp,	0,255,	detail_red); 
	pan_setslider(detail_panel, 0,	90+12,142,	horiz_slider_bmp,slider_knob_bmp,	0,255,	detail_green); 
	pan_setslider(detail_panel, 0,	90+12,162,	horiz_slider_bmp,slider_knob_bmp,	0,255,	detail_blue); 
		
	pan_setdigits(detail_panel, 0,	15,60,	"Ambient: %0.f",		panel_font,	1, 		detail_ambient);
	pan_setslider(detail_panel, 0,	90+12,62,	horiz_slider_bmp,slider_knob_bmp,	0,100,	detail_ambient); 
	
	pan_setdigits(detail_panel, 0,	15,80,	"Alpha: %0.f",			panel_font,	1, 		detail_alpha);
	pan_setslider(detail_panel, 0,	90+12,82,	horiz_slider_bmp,slider_knob_bmp,	0,100,	detail_alpha); 
	
	// button 15
	pan_setstring(detail_panel, 0,	40,260,	panel_font,			str_create("set OVERLAY") );
	pan_setbutton(detail_panel, 0,	2, 	15,260,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Detail_ToggleOverlay,NULL,NULL); 
	button_state( detail_panel , 15 , detail_overlay ); 		// 1 ON, 0 OFF	
	
	// button 16
	pan_setbutton(detail_panel, 0,	1, 	45,480,		m_clearall_on_bmp,m_clearall_off_bmp,m_clearall_over_bmp,m_clearall_over_bmp,		Detail_DeleteAll,NULL,NULL); 
	
	// material buttons:	
	
	// button 17
//	pan_setbutton(detail_panel, 0,	1, 	200,10,		ss_material_on_bmp,ss_material_off_bmp,ss_material_over_bmp,ss_material_over_bmp,		Detail_MaterialSetup,NULL,NULL); 			
	pan_setbutton(detail_panel, 0,	1, 	200,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 18
	pan_setstring(detail_panel, 0,	40+100,180,	panel_font,			str_create("set sprite material") );	
	pan_setbutton(detail_panel, 0,	2, 	15+100,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Detail_ToggleSprite,NULL,NULL); 
	button_state( detail_panel , 18 , detail_sprite ); 		// 1 ON, 0 OFF	
	
	// button 19,20	21,22		
	pan_setbutton(detail_panel, 0,	1, 	90,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	210,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	90,220,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	210,220,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Detail_Panel_Button, 		NULL, NULL); 
	// button 23,24	25,26		27,28
	pan_setbutton(detail_panel, 0,	1, 	90,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	210,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	90,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	210,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	90,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	210,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Detail_Panel_Button, 		NULL, NULL); 
	// button 29,30	31,32
	pan_setbutton(detail_panel, 0,	1, 	90,60,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	210,60,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	90,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Detail_Panel_Button, 		NULL, NULL); 
	pan_setbutton(detail_panel, 0,	1, 	210,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Detail_Panel_Button, 		NULL, NULL); 
	
	// button 33 - seed
	pan_setbutton(detail_panel, 0,	1, 	150,480,		m_seed_on_bmp,m_seed_off_bmp,m_seed_over_bmp,m_seed_over_bmp,		Seed_Init,	NULL,NULL); 
	
	ptr_remove( temp_string );	
//	while(UIMode==(var)UIMODE_DETAIL)
//		{
//			// reset overlay if alpha decreased
//			if ((detail_overlay) && (detail_alpha<98))
//				{
//					detail_overlay = 0;
//					button_state( detail_panel , 15 , detail_overlay ); 		// 1 ON, 0 OFF	
//				}
//			wait(1);
//		}
	
}


void		Detail_Panel_Toggle()
{
	toggle( detail_panel , SHOW );	
}


////////////////////////////////////////////////////


void		Detail_Panel_Button(button_number)
{
	// detail_density
	if (button_number==(var)19)
		{
			detail_density = maxv( 1 , --detail_density );
		}
	else if (button_number==(var)20)
		{
			detail_density = minv( max_detail_density , ++detail_density );
		}
	// detail_scale
	else if (button_number==(var)21)
		{
			detail_scale -= 0.1;
			detail_scale = maxv( 0.1 , detail_scale );
		}
	else if (button_number==(var)22)
		{
			detail_scale += 0.1;
			detail_scale = minv( 2 , detail_scale );
		}
	// detail_red
	else if (button_number==(var)23)
		{
			detail_red = maxv( 0 , --detail_red );
		}
	else if (button_number==(var)24)
		{
			detail_red = minv( 255 , ++detail_red );
		}
	// detail_green
	else if (button_number==(var)25)
		{
			detail_green = maxv( 0 , --detail_green );
		}
	else if (button_number==(var)26)
		{
			detail_green = minv( 255 , ++detail_green );
		}
	// detail_blue
	else if (button_number==(var)27)
		{
			detail_blue = maxv( 0 , --detail_blue );
		}
	else if (button_number==(var)28)
		{
			detail_blue = minv( 255 , ++detail_blue );
		}
	// detail_ambient
	else if (button_number==(var)29)
		{
			detail_ambient = maxv( 0 , --detail_ambient );
		}
	else if (button_number==(var)30)
		{
			detail_ambient = minv( 100 , ++detail_ambient );
		}
	// detail_alpha
	else if (button_number==(var)31)
		{
			detail_alpha = maxv( 0 , --detail_alpha );
		}
	else if (button_number==(var)32)
		{
			detail_alpha = minv( 100 , ++detail_alpha );
		}	
}


////////////////////////////////////////////////////


//// change by hotkey
//void		Detail_NextSet()
//{ 
//	// jump to next existing set
//	detail_set++;
//	if (detail_set>max_detail_sets)
//		{
//			detail_set = 1;
//		}
//	Detail_UpdateSetNames();
//	wait_for(Detail_UpdateSetNames);
//}


void		Detail_IncSet()
{ 
	// jump to next existing set
	detail_set++;
	if (detail_set>max_detail_sets)
		{
//			detail_set = max_detail_sets;
//			return;
			detail_set = 1;
		}
	Detail_UpdateSetNames();
	wait_for(Detail_UpdateSetNames);
}


void		Detail_DecSet()
{ 
	// jump to next existing set
	detail_set--;
	if (detail_set<1)
		{
//			detail_set = 1;
//			return;
			detail_set = max_detail_sets;
		}
	Detail_UpdateSetNames();
	wait_for(Detail_UpdateSetNames);
}


void		Detail_UpdateSetNames()
{
	int i;
	// copy the proper 8 filenames to string array
	int j = detail_set - 1;
	int k = j * max_detail_items;
	for(i=0;i<max_detail_items;i++)
		{
//			str_cpy( detail_filenames[i] , detail_foldername );							// subfolder
//			str_cat( detail_filenames[i] , (details_txt->pstring)[k+i] );			// filename with extension
			str_cpy( detail_filenames[i] , "" );											// (or detail_foldername) subfolder
			str_cat( detail_filenames[i] , (details_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi( detail_filenames[i] , "" ) )								// (or detail_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( detail_filenames[i] , "-" );
				}
		}
	// refresh 8 filename strings to display in menu
	for(i=0;i<max_detail_items;i++)
		{
			pan_setstring(detail_panel, 11+i,	55,350+i*15,	panel_font,			str_create( detail_filenames[i] ) );
		}	
	wait(1);
}


///////////////////////////////////


// change by hotkey
void		Detail_Flag_Switch(var setplusone)
{
	int temp = setplusone -1 -1;	// -1 because of scancode, and -1 because of array pos are 0..7
	detail_switch_type[temp] = 1 - detail_switch_type[temp];
	// change checkbox state too
	var temp_state = button_state( detail_panel , temp+1 , -1 ); 				// -1 to read value
	button_state( detail_panel , temp+1 , 1-temp_state ); 						// 1 ON, 0 OFF
	
	wait(1);
}


// change by checkbox
void		Detail_Flag_Toggle(button_number)
{
	detail_switch_type[button_number-1] = 1 - detail_switch_type[button_number-1] ;
	
	wait(1);
}


/////////////////////////////////
// decrease/increase detail densityif left mouse button is used for placement


void 		Detail_DecDens()
{
	detail_density--;
	if (detail_density<1)
		{
			detail_density = 1;
		}
}


void 		Detail_IncDens()
{
	detail_density++;
	if (detail_density>max_detail_density)
		{
			detail_density = max_detail_density;
		}
}


////////////////////////////////////
// change by hotkey


void 		Detail_DecSize()
{
	detail_tilesize -= 2;
	if (detail_tilesize<1)
		{
			detail_tilesize = 1;
		}	
	var temp = detail_tilesize / detail_placement_cube.scale_x ;	
	vec_scale( detail_placement_cube.scale_x , temp );
}


void 		Detail_IncSize()
{
	detail_tilesize += 2;
	if (detail_tilesize>max_detail_size)
		{
			detail_tilesize = max_detail_size;
		}
	var temp = detail_tilesize / detail_placement_cube.scale_x ;	
	vec_scale( detail_placement_cube.scale_x , temp );
}


/////////////////////////////////////////////////////
// change by checkbox


void		Detail_ToggleUnlit()
{
	detail_unlit = 1 - detail_unlit;	
	if (detail_unlit)
		{
			// switch off LIGHT
			detail_light = 0;
			button_state( detail_panel , 14 , detail_light );
			// switch off sprite
			detail_sprite = 0;
			button_state( detail_panel , 18 , detail_sprite );
		}
}


void		Detail_ToggleLight()
{
	detail_light = 1 - detail_light;	
	if (detail_light)
		{
			// switch off UNLIT
			detail_unlit = 0;
			button_state( detail_panel , 9 , detail_unlit );
		}
}


void		Detail_ToggleSprite()
{
	detail_sprite = 1 - detail_sprite;
	if (detail_sprite)
		{
			// switch off UNLIT
			detail_unlit = 0;
			button_state( detail_panel , 9 , detail_unlit );
		}
}


void		Detail_ToggleOverlay()
{
	detail_overlay = 1 - detail_overlay;	
//	if (detail_overlay)
//		{
//			detail_alpha = 100;
//		}
}


/////////////////////////////////////////////////////////


void		Detail_MaterialSetup()
{
	wait(3);
	material_select = 12;
	Material_MatPanel_Init( mtl_detail , NULL );				
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor placement


void	Detail_OnClick()
{
	if (proc_status(Detail_OnClick) > 1)		// protection
		{
			return;
		}
	wait(-0.25);										// time interval of allowed function instances
	
	if (!mouse_left)									// single click
		{
			Detail_CreateArea();
		}
	else													// holding button
		{
			if (proc_status(Detail_CreateArea_Cont) < 1)		
				{
					Detail_CreateArea_Cont();
				}
		}
}


void	 Detail_CreateArea()
{	
	int i,k,temp_int;
	var j;
	STRING* temp_str = str_create("");
	random_seed(0);
	collision_mode = 0; 
	// check number of types to be used to increase loop length if less than 8, or exit if 0
	j = 0;
	for (i=0;i<max_detail_items;i++)
		{
			if(detail_switch_type[i])
			{
				j++;
			}
		}
	if (j==0) { return; }
	// set loop limit
	j = integer( max_detail_items/j );
	j *= integer(detail_density) *detail_tilesize*detail_tilesize;
	//	loop length depends on detail_density and detail_tilesize
	for ( i=0 ; i<j ; i++)
		{
			k = i%max_detail_items;
			if ((detail_switch_type[k]) && (!str_cmpi(detail_filenames[k],"-")))
				{					
					str_cpy( temp_str , detail_foldername );
					str_cat( temp_str , detail_filenames[k] );
					
					// set random pos within creator cube area and shrub_tilesize
					var tempx = detail_placement_cube.x + random(detail_tilesize*GetMapTileSize()) - detail_tilesize*GetMapTileSize()/2;
					var tempy = detail_placement_cube.y + random(detail_tilesize*GetMapTileSize()) - detail_tilesize*GetMapTileSize()/2;
//					var tempz = detail_placement_cube.z;
					
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
					Detail_CreateObject(temp_str, vector(tempx,tempy,tempz));
//					ent_create( temp_str , vector(tempx,tempy,tempz) , Detail_CreateItem );
				}
		}
	wait(1);
	collision_mode = 1; 
}


void		Detail_CreateArea_Cont()
{	
	if (proc_status(Detail_CreateArea_Cont) > 1)	return;
		
	int i,k;
	var j;
	STRING* temp_str = str_create("");
	random_seed(0);
	collision_mode = 0; 
	// check number of types to be used to increase loop length if less than 8, or exit if 0
	j = 0;
	for (i=0;i<DETAIL_MAXPERSET;i++)
		{
			if(detail_switch_type[i])
			{
				j++;
			}
		}
	if (j==0) { return; }
	i = 0;
	while (mouse_left)
		{
			k = i%max_detail_items;
			if ((detail_switch_type[k]) && (!str_cmpi(detail_filenames[k],"-")))
				{					
					str_cpy( temp_str , detail_foldername );
					str_cat( temp_str , detail_filenames[k] );
					
					// set random pos within creator cube area and shrub_tilesize
					var tempx = detail_placement_cube.x + random(detail_tilesize*GetMapTileSize()) - detail_tilesize*GetMapTileSize()/2;
					var tempy = detail_placement_cube.y + random(detail_tilesize*GetMapTileSize()) - detail_tilesize*GetMapTileSize()/2;
//					var tempz = detail_placement_cube.z;
					
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
					Detail_CreateObject(temp_str, vector(tempx,tempy,tempz));
//					ent_create( temp_str , vector(tempx,tempy,tempz) , Detail_CreateItem );
				}
			if (k==1) 	wait(-0.01);
			i++;			
		}
	collision_mode = 1; 
	ptr_remove(temp_str);
}


/////////////////////////////////////////////////////////////////////
// seeding


void		Detail_SeedQuantity()
{
	set( map_loadpanel , SHOW);
	wait(3);
	//----------------------------------------
	STRING* temp_str = str_create("");
	int i, j, k, m = 0;
	
	//----------------------------------------
	// check number of types to be used, to exit if 0	
	
	for (i=0;i<max_detail_items;i++)
		{
			if(detail_switch_type[i])
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
					vec_to_angle(normal_ang, normal);
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
					k = m % max_detail_items;
				}
			while((!detail_switch_type[k]) || (str_cmpi(detail_filenames[k],"-")));	// repeat until nonselected or empty item found							
				
			str_cpy( temp_str , detail_foldername );
			str_cat( temp_str , detail_filenames[k] );	
			
			// place entity
			Detail_CreateObject(temp_str, vector(posx,posy,posz));
//			ent_create( temp_str , vector(posx,posy,posz) , Detail_CreateItem );	
			
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


void		Detail_SeedArea()
{
	set( map_loadpanel , SHOW);
	wait(3);
	//----------------------------------------
	STRING* temp_str = str_create("");	
	
	//----------------------------------------
	// check number of types to be used, to exit if 0	
	
	int a, b = 0;
	
	for (a=0;a<max_detail_items;a++)
		{
			if(detail_switch_type[a])
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
									k = m % max_detail_items;
								}
							while((!detail_switch_type[k]) || (str_cmpi(detail_filenames[k],"-")));	// repeat until nonselected or empty item found							
								
							str_cpy( temp_str , detail_foldername );
							str_cat( temp_str , detail_filenames[k] );									
							
							// place entity
							Detail_CreateObject(temp_str, vector(posx,posy,posz));
//							ent_create( temp_str , vector(posx,posy,posz) , Detail_CreateItem );	
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


////////////////////////////////////////////////////////////


void	 Detail_CreateObject(STRING* ent_name_full, VECTOR* pos_vec)
{
	STRING* temp_str = str_create("");
	str_cpy(temp_str, ent_name_full);
	
	//----------------------------------------------------------------------------
	// create object
	
	var my_scale = GetMapTileSize() / detail_defaultsize * detail_scale;
	
//	if (detail_randomize)
//		{
//			// uniform randomness to keep object size ratio
//			my_scale *= (1+(random(20)-10)/100);
//		}
	
	//---
	// adjust over surface
	
	BMAP* temp_bmap = bmap_create(ent_name_full);
	
	pos_vec.z += bmap_height(temp_bmap) * my_scale * 0.5;	// up by 0.x of its image actual size in quants - 0.5 means at surface
	
	//---
	
	Detail_Create( temp_str, 
						my_scale, 
						pos_vec.x, pos_vec.y, pos_vec.z, 
						detail_alpha, detail_ambient, 
						detail_light, detail_red, detail_green, detail_blue, 
						detail_unlit, detail_overlay, detail_sprite);
//	wait_for(Detail_Create);
	
	//----------------------------------------------------------------------------			
//			
//	ent_create( temp_str, pos_vec, Detail_CreateItem );	
//			
	//----------------------------------------------------------------------------
	
//	wait(1);
	ptr_remove(temp_str);
	ptr_remove(temp_bmap);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// deletion


void		Detail_DeleteArea()
{
	Detail_RemoveArea(square_x, square_y, detail_tilesize, detail_tilesize);	
}


void		Detail_DeleteAll()
{
	wait(3);

	Detail_RemoveAll();
	wait_for(Detail_RemoveAll);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif