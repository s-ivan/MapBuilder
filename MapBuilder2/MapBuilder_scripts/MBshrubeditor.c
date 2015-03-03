////////////////////////////////////////////////////
// placing shrub sprites (passable decoration), mainly grass
// max 8 items in a set, max 10 sets , i.e. max 80 different grass types must be enough
// size of creation area is mod by uihandlingclass "y" and "x" keys - multiple preview of selected sprites are needed !
// properties of shrubs can be altered by control panel 
/////////////////////////////////////////////////////

#ifndef MBshrubeditor_c
#define MBshrubeditor_c


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor initialization


void 		Shrub_Init()
{		
	//----------------------------------------------------------------
	// shrub imagesize determination: 256 (low quality) or 512 (high quality)
	Shrub_SetDefaultSize();
	wait_for(Shrub_SetDefaultSize);
	//----------------------------------------------------------------
	// shrub sprites filename initialization 
sys_marker("si1");
	int arraysize = (int)SHRUB_MAXPERSET;
	shrub_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );
	int i;	
	// init filename string array elements
	for(i=0;i<max_shrub_items;i++)
		{
			shrub_filenames[i] = str_create("");
		}	
sys_marker(NULL);		
	//-----------------------------------------------------------------
	// determine how many dds or tga files are in the folder, 
	// divide it by 8, and the result is the number of sets
//	shrub_set = 1;
	max_shrub_sets = SHRUB_MAXSET;
	shrubs_txt = txt_create( (max_shrub_sets*max_shrub_items) , 0 );		// 10*8=80
	for(i=0;i<max_shrub_sets*max_shrub_items;i++)
		{
			str_cpy( (shrubs_txt->pstring)[i] , "" );
		}
	//-------------------------------------------------------------------
	// read in folder content
	i = 0;
	STRING* temp_str = str_create("");
	str_cpy( temp_str , shrub_foldername );
	str_cat( temp_str , "*.dds" );
	txt_for_dir( shrubs_txt, temp_str ); 										//	better not to use tga
	while (str_len((shrubs_txt->pstring)[i]) > 0)
		{
			i++;
		}
	max_shrub_sets = integer(i/max_shrub_items);
	if ( fraction(i/max_shrub_items) > 0)
		{
			max_shrub_sets++;		// allow to display not complete sets too
		}
	shrub_set = clamp(shrub_set, 1, max_shrub_sets);
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	for(i=0;i<max_shrub_items;i++)
		{
//			str_cpy( shrub_filenames[i] , shrub_foldername );							// subfolder
//			str_cat( shrub_filenames[i] , (shrubs_txt->pstring)[i] );				// filename with extension			
			str_cpy( shrub_filenames[i] , "" );												// (or shrub_foldername) subfolder
			int j = (shrub_set - 1) * max_shrub_items;
			str_cat( shrub_filenames[i] , (shrubs_txt->pstring)[j+i] );				// filename with extension
			if ( str_cmpi( shrub_filenames[i] , "" ) )									// (or shrub_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( shrub_filenames[i] , "-" );
				}
		}	
	//------------------------------------------------------------------
	// initial value settings
	for(i=0;i<4;i++)
		{
			shrub_switch_type[i] = 1;
		}
	for(i=4;i<8;i++)
		{
			shrub_switch_type[i] = 0;
		}	
//	shrub_tilesize = 1;
//	shrub_density = 3;	
	//-------------------------------------------------------------------
	// create objects
	collision_mode = 0; 
	shrub_placement_cube = ent_create( shrub_creator_model , vector(square_coord_x,square_coord_y,square_coord_z) , Shrub_Cube );
	wait(1);
//	ent_clone(shrub_placement_cube);		// CLO ?
	collision_mode = 1; 
//	shrub_unlit = 0;
//	shrub_light = 1;
	Shrub_Panel_Init();
}


void		Shrub_Close()
{
	//-------------------------------------------------------
	ptr_remove(shrub_placement_cube);
	ptr_remove(shrub_panel);
	
	shrub_placement_cube = NULL;
	shrub_panel = NULL;
	
//	ptr_remove(mtl_shrub);		
	//--------------------------------------------------------
	int i;
	// remove temp text
	for (i = 0; i < shrubs_txt->strings; i++) 
		if ((shrubs_txt->pstring)[i]) 
			ptr_remove((shrubs_txt->pstring)[i]);
	ptr_remove(shrubs_txt);
	shrubs_txt = NULL;
	// remove temp string array
	for(i=0;i<max_shrub_items;i++)
		{
			ptr_remove(shrub_filenames[i]);
		}
	sys_free(shrub_filenames);
	shrub_filenames = NULL;
//	ptr_remove(shrub_filenames);
}


action 	Shrub_Cube()
{
	set(my, PASSABLE | TRANSLUCENT | CAST);
	my.flags2 |= UNTOUCHABLE;
	my.alpha = 50;
	vec_scale( my.scale_x , GetMapTileSize() / MAP_DEFAULTGRIDSIZE * shrub_tilesize );
	//	my.skill1 = 1;	// actual scale size
	my.material = mtl_placementarea;
	
	VECTOR vTemp;
	COLOR cTemp;
	vec_set(cTemp,vector(0,0,255));
	
	while(UIMode==(var)UIMODE_SHRUB)
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
	Shrub_Close();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor menu

void		Shrub_Panel_Init()
{	
	int i;
	STRING* temp_string = str_create("");
	
	shrub_panel = pan_create( panel_image2 , 11 );
	set( shrub_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	shrub_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	shrub_panel.pos_y 	= ui_submenupanel_y;								// 256;
	shrub_panel.red		= panel_font_r;
	shrub_panel.green		= panel_font_g;
	shrub_panel.blue		= panel_font_b;
	shrub_panel.event		= ui_submenu_panelmove;
	
	// string 1
	pan_setstring(shrub_panel, 0,	15,10,	panel_font2,			str_create("SHRUB SPRITE PLACEMENT") );
	// string 2
	pan_setstring(shrub_panel, 0,	15,330,	panel_font,			str_create("Toggle objects") );
	
	// strings 3-10 - shrub item numbers 1-8
	for(i=0;i<max_shrub_items;i++)
		{
			str_for_int( temp_string , (i+1) );
			pan_setstring( shrub_panel, 0,	35,350+i*15,	panel_font,			str_create( temp_string ) );
		}
	
	// strings 11-18 - shrub item filenames 1-8
	for(i=0;i<max_shrub_items;i++)
		{
			pan_setstring(shrub_panel, 0,	55,350+i*15,	panel_font,			str_create( shrub_filenames[i] ) );
		}
	
	// buttons 1-8 (checkbox)
	for(i=0;i<max_shrub_items;i++)
		{
			pan_setbutton(shrub_panel, 0,	2, 	15,350+i*15,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Shrub_Flag_Toggle,NULL,NULL); 
		}
	// button states
	for(i=0;i<max_shrub_items;i++)
		{
			button_state( shrub_panel , 1+i , shrub_switch_type[i] ); 		// 1 ON, 0 OFF
		}
	
	// button 9
	pan_setstring(shrub_panel, 0,	40,180,	panel_font,			str_create("set UNLIT") );	
	pan_setbutton(shrub_panel, 0,	2, 	15,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Shrub_ToggleUnlit,NULL,NULL); 
	button_state( shrub_panel , 9 , shrub_unlit ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(shrub_panel, 0,	15,30,	"COUNT: %0.f",				panel_font,	1, shrub_counter);
	
	// buttns 10-11
	pan_setdigits(shrub_panel, 0,	180,330,	"Set:    %0.f",		panel_font,	1, shrub_set);	
	pan_setbutton(shrub_panel, 0,	1, 	165,330,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Shrub_DecSet, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	225,330,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			Shrub_IncSet, 		NULL, NULL); 
		
	// buttons 12-13
	pan_setdigits(shrub_panel, 0,	110+12,260,	"Area size:  %0.f",		panel_font,	1, shrub_tilesize);	
	pan_setbutton(shrub_panel, 0,	1, 	95+12,261,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Shrub_DecSize, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	182+12,261,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			Shrub_IncSize, 		NULL, NULL); 
		
	pan_setdigits(shrub_panel, 0,	15,200,	"Density: %0.f",		panel_font,	1, 							shrub_density);
	pan_setslider(shrub_panel, 0,	90+12,202,	horiz_slider_bmp,slider_knob_bmp,	1,max_shrub_density+0.2,	shrub_density); 
	
	pan_setdigits(shrub_panel, 0,	15,220,	"Scale: %0.1f",		panel_font,	1, 			shrub_scale);
	pan_setslider(shrub_panel, 0,	90+12,222,	horiz_slider_bmp,slider_knob_bmp,	0.1,2,	shrub_scale);
	
	// button 14
	pan_setstring(shrub_panel, 0,	40,100,	panel_font,			str_create("set LIGHT to adjust RGB values:") );	
	pan_setbutton(shrub_panel, 0,	2, 	15,100,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Shrub_ToggleLight,NULL,NULL); 
	button_state( shrub_panel , 14 , shrub_light ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(shrub_panel, 0,	15,120,	"R: %0.f",				panel_font,	1, 			shrub_red);
	pan_setdigits(shrub_panel, 0,	15,140,	"G: %0.f",				panel_font,	1, 			shrub_green);
	pan_setdigits(shrub_panel, 0,	15,160,	"B: %0.f",				panel_font,	1, 			shrub_blue);
	pan_setslider(shrub_panel, 0,	90+12,122,	horiz_slider_bmp,slider_knob_bmp,	0,255,	shrub_red); 
	pan_setslider(shrub_panel, 0,	90+12,142,	horiz_slider_bmp,slider_knob_bmp,	0,255,	shrub_green); 
	pan_setslider(shrub_panel, 0,	90+12,162,	horiz_slider_bmp,slider_knob_bmp,	0,255,	shrub_blue); 
		
	pan_setdigits(shrub_panel, 0,	15,60,	"Ambient: %0.f",		panel_font,	1, 			shrub_ambient);
	pan_setslider(shrub_panel, 0,	90+12,62,	horiz_slider_bmp,slider_knob_bmp,	0,100,	shrub_ambient); 
	
	pan_setdigits(shrub_panel, 0,	15,80,	"Alpha: %0.f",			panel_font,	1, 			shrub_alpha);
	pan_setslider(shrub_panel, 0,	90+12,82,	horiz_slider_bmp,slider_knob_bmp,	0,100,	shrub_alpha); 
	
	pan_setdigits(shrub_panel, 0,	15,240,	"Pieces: %0.f",			panel_font,	1, 		shrub_pieces);
	pan_setslider(shrub_panel, 0,	90+12,242,	horiz_slider_bmp,slider_knob_bmp,	1,5,		shrub_pieces); 
	
	// button 15
	pan_setstring(shrub_panel, 0,	40,280,	panel_font,			str_create("set SHADOW") );
	pan_setbutton(shrub_panel, 0,	2, 	15,280,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Shrub_ToggleShadow,NULL,NULL); 
	button_state( shrub_panel , 15 , shrub_shadow ); 		// 1 ON, 0 OFF	
	
	// button 16
	pan_setstring(shrub_panel, 0,	40,300,	panel_font,			str_create("set OVERLAY") );
	pan_setbutton(shrub_panel, 0,	2, 	15,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Shrub_ToggleOverlay,NULL,NULL); 
	button_state( shrub_panel , 16 , shrub_overlay ); 		// 1 ON, 0 OFF	
	
	// button 17
	pan_setbutton(shrub_panel, 0,	1, 	45,480,		m_clearall_on_bmp,m_clearall_off_bmp,m_clearall_over_bmp,m_clearall_over_bmp,		Shrub_DeleteAll,NULL,NULL); 
	
	// material buttons:	
	
	// button 18
//	pan_setbutton(shrub_panel, 0,	1, 	200,10,		ss_material_on_bmp,ss_material_off_bmp,ss_material_over_bmp,ss_material_over_bmp,		Shrub_MaterialSetup,NULL,NULL); 			
	pan_setbutton(shrub_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 19
	pan_setstring(shrub_panel, 0,	40+100,180,	panel_font,			str_create("set sprite material") );	
	pan_setbutton(shrub_panel, 0,	2, 	15+100,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Shrub_ToggleSprite,NULL,NULL); 
	button_state( shrub_panel , 19 , shrub_sprite ); 		// 1 ON, 0 OFF	
	
	// button 20,21	22,23		
	pan_setbutton(shrub_panel, 0,	1, 	90,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	210,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	90,220,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	210,220,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Shrub_Panel_Button, 		NULL, NULL); 
	// button 24,25	26,27		28,29
	pan_setbutton(shrub_panel, 0,	1, 	90,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	210,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	90,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	210,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	90,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	210,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Shrub_Panel_Button, 		NULL, NULL); 
	// button 30,31	32,33		34,35
	pan_setbutton(shrub_panel, 0,	1, 	90,60,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	210,60,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	90,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	210,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	90,240,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Shrub_Panel_Button, 		NULL, NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	210,240,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Shrub_Panel_Button, 		NULL, NULL); 	
	
	// button 36 - Snap
	pan_setstring(shrub_panel, 0,	15+20,260,	panel_font,			str_create("Snap XY") );
	pan_setbutton(shrub_panel, 0,	2, 	15,260,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Shrub_ToggleSnap,NULL,NULL); 
	button_state( shrub_panel , 36 , shrub_snap ); 		// 1 ON, 0 OFF	
	
	// button 37 - Seed
	pan_setbutton(shrub_panel, 0,	1, 	150,480,		m_seed_on_bmp,m_seed_off_bmp,m_seed_over_bmp,m_seed_over_bmp,		Seed_Init,	NULL,NULL); 
	
	// button 38 - Lightmap
//	if (shadow_stencil==(var)8)
		{
			pan_setstring(shrub_panel, 0,	40+100,300,	panel_font,			str_create("Lightmap") );
		}
//	else
//		{
//			pan_setstring(shrub_panel, 0,	40+100,300,	panel_font,			str_create("set to Lightmap") );
//		}
	pan_setbutton(shrub_panel, 0,	2, 	15+100,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Shrub_ToggleLightmap,NULL,NULL); 
	button_state( shrub_panel , 38 , shrub_lightmap ); 		// 1 ON, 0 OFF	
	
	// button 39 - force decal shadow (ent_decal) - useful when the evel is used in both shadow_stencil=1 and shadow_stencil=8 modes
//	if (shadow_stencil==(var)8)
		{
			pan_setstring(shrub_panel, 0,	40+175,300,	panel_font,			str_create("Decal") );
			pan_setbutton(shrub_panel, 0,	2, 	15+175,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Shrub_ToggleDecal,NULL,NULL); 
			button_state( shrub_panel , 39 , shrub_decal ); 		// 1 ON, 0 OFF	
		}
	
	// button 40 41 - Lightmap All : Set / Reset
	pan_setstring(shrub_panel, 0,			130,30,		panel_font,			str_create("Ligthmap All") );
	pan_setbutton(shrub_panel, 0,	1, 	200,20,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,					Shrub_LightmapAll,		NULL,NULL); 
	pan_setbutton(shrub_panel, 0,	1, 	200,40,		ss_reset_on_bmp,ss_reset_off_bmp,ss_reset_over_bmp,ss_reset_over_bmp,		Shrub_LightmapAll,		NULL,NULL); 
	
	//--------------------------------------------------------------
	
	ptr_remove( temp_string );	
//	while(UIMode==(var)UIMODE_SHRUB)
//		{
//			// reset overlay if alpha decreased
//			if ((shrub_overlay) && (shrub_alpha<98))
//				{
//					shrub_overlay = 0;
//					button_state( shrub_panel , 16 , shrub_overlay ); 		// 1 ON, 0 OFF	
//				}
//			wait(1);
//		}
	
}


void		Shrub_Panel_Toggle()
{
	toggle( shrub_panel , SHOW );	
}


////////////////////////////////////////////////////


void		Shrub_Panel_Button(button_number)
{
	// shrub_density
	if (button_number==(var)20)
		{
			shrub_density = maxv( 1 , --shrub_density );
		}
	else if (button_number==(var)21)
		{
			shrub_density = minv( max_shrub_density , ++shrub_density );
		}
	// shrub_scale
	else if (button_number==(var)22)
		{
			shrub_scale -= 0.1;
			shrub_scale = maxv( 0.1 , shrub_scale );
		}
	else if (button_number==(var)23)
		{
			shrub_scale += 0.1;
			shrub_scale = minv( 2 , shrub_scale );
		}
	// shrub_red
	else if (button_number==(var)24)
		{
			shrub_red = maxv( 0 , --shrub_red );
		}
	else if (button_number==(var)25)
		{
			shrub_red = minv( 255 , ++shrub_red );
		}
	// shrub_green
	else if (button_number==(var)26)
		{
			shrub_green = maxv( 0 , --shrub_green );
		}
	else if (button_number==(var)27)
		{
			shrub_green = minv( 255 , ++shrub_green );
		}
	// shrub_blue
	else if (button_number==(var)28)
		{
			shrub_blue = maxv( 0 , --shrub_blue );
		}
	else if (button_number==(var)29)
		{
			shrub_blue = minv( 255 , ++shrub_blue );
		}
	// shrub_ambient
	else if (button_number==(var)30)
		{
			shrub_ambient = maxv( 0 , --shrub_ambient );
		}
	else if (button_number==(var)31)
		{
			shrub_ambient = minv( 100 , ++shrub_ambient );
		}
	// shrub_alpha
	else if (button_number==(var)32)
		{
			shrub_alpha = maxv( 0 , --shrub_alpha );
		}
	else if (button_number==(var)33)
		{
			shrub_alpha = minv( 100 , ++shrub_alpha );
		}	
	// shrub_pieces
	else if (button_number==(var)34)
		{
			shrub_pieces = maxv( 1 , --shrub_pieces );
		}
	else if (button_number==(var)35)
		{
			shrub_pieces = minv( 5 , ++shrub_pieces );
		}	
}


////////////////////////////////////////////////////


//// change by hotkey
//void		Shrub_NextSet()
//{ 
//	// jump to next existing set
//	shrub_set++;
//	if (shrub_set>max_shrub_sets)
//		{
//			shrub_set = 1;
//		}
//	Shrub_UpdateSetNames();
//	wait_for(Shrub_UpdateSetNames);
//}


void		Shrub_IncSet()
{ 
	// jump to next existing set
	shrub_set++;
	if (shrub_set>max_shrub_sets)
		{
//			shrub_set = max_shrub_sets;
//			return;
			shrub_set = 1;
		}
	Shrub_UpdateSetNames();
	wait_for(Shrub_UpdateSetNames);
}


void		Shrub_DecSet()
{ 
	// jump to next existing set
	shrub_set--;
	if (shrub_set<1)
		{
//			shrub_set = 1;
//			return;
			shrub_set = max_shrub_sets;
		}
	Shrub_UpdateSetNames();
	wait_for(Shrub_UpdateSetNames);
}


void		Shrub_UpdateSetNames()
{
	int i;
	// copy the proper 8 filenames to string array
	int j = shrub_set - 1;
	int k = j * max_shrub_items;
	for(i=0;i<max_shrub_items;i++)
		{
//			str_cpy( shrub_filenames[i] , shrub_foldername );						// subfolder
//			str_cat( shrub_filenames[i] , (shrubs_txt->pstring)[k+i] );			// filename with extension
			str_cpy( shrub_filenames[i] , "" );											// (or shrub_foldername) subfolder
			str_cat( shrub_filenames[i] , (shrubs_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi( shrub_filenames[i] , "" ) )								// (or shrub_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( shrub_filenames[i] , "-" );
				}
		}
	// refresh 8 filename strings to display in menu
	for(i=0;i<max_shrub_items;i++)
		{
			pan_setstring(shrub_panel, 11+i,	55,350+i*15,	panel_font,			str_create( shrub_filenames[i] ) );
		}	
	wait(1);
}


///////////////////////////////////


// change by hotkey
void		Shrub_Flag_Switch(var setplusone)
{
	int temp = setplusone -1 -1;	// -1 because of scancode, and -1 because of array pos are 0..7
	shrub_switch_type[temp] = 1 - shrub_switch_type[temp];
	// change checkbox state too
	var temp_state = button_state( shrub_panel , temp+1 , -1 ); 				// -1 to read value
	button_state( shrub_panel , temp+1 , 1-temp_state ); 						// 1 ON, 0 OFF
	
	wait(1);
}


// change by checkbox
void		Shrub_Flag_Toggle(button_number)
{
	shrub_switch_type[button_number-1] = 1 - shrub_switch_type[button_number-1] ;
	
	wait(1);
}


/////////////////////////////////
// decrease/increase shrub densityif left mouse button is used for placement


void 		Shrub_DecDens()
{
	shrub_density--;
	if (shrub_density<1)
		{
			shrub_density = 1;
		}
}


void 		Shrub_IncDens()
{
	shrub_density++;
	if (shrub_density>max_shrub_density)
		{
			shrub_density = max_shrub_density;
		}
}


////////////////////////////////////
// change by hotkey


void 		Shrub_DecSize()
{
	shrub_tilesize -= 2;
	if (shrub_tilesize<1)
		{
			shrub_tilesize = 1;
		}	
	var temp = shrub_tilesize / shrub_placement_cube.scale_x ;	
	vec_scale( shrub_placement_cube.scale_x , temp );
}


void 		Shrub_IncSize()
{
	shrub_tilesize += 2;
	if (shrub_tilesize>max_shrub_size)
		{
			shrub_tilesize = max_shrub_size;
		}
	var temp = shrub_tilesize / shrub_placement_cube.scale_x ;	
	vec_scale( shrub_placement_cube.scale_x , temp );
}


/////////////////////////////////////////////////////
// change by checkbox


void		Shrub_ToggleUnlit()
{
	shrub_unlit = 1 - shrub_unlit;	
	if (shrub_unlit)
		{
			// switch off LIGHT
			shrub_light = 0;
			button_state( shrub_panel , 14 , shrub_light );
			// switch off sprite
			shrub_sprite = 0;
			button_state( shrub_panel , 19 , shrub_sprite );
		}
}


void		Shrub_ToggleLight()
{
	shrub_light = 1 - shrub_light;	
	if (shrub_light)
		{
			// switch off UNLIT
			shrub_unlit = 0;
			button_state( shrub_panel , 9 , shrub_unlit );
		}
}


void		Shrub_ToggleSprite()
{
	shrub_sprite = 1 - shrub_sprite;
	if (shrub_sprite)
		{
			// switch off UNLIT
			shrub_unlit = 0;
			button_state( shrub_panel , 9 , shrub_unlit );
		}
}


void		Shrub_ToggleShadow()
{
	shrub_shadow = 1 - shrub_shadow;		
	if (shrub_shadow)
		{
			shrub_lightmap = 0;
			button_state( shrub_panel , 38 , shrub_lightmap );
		}
	else
		{
			shrub_decal = 0;
			button_state( shrub_panel , 39 , shrub_decal );
		}
}


void		Shrub_ToggleOverlay()
{
	shrub_overlay = 1 - shrub_overlay;	
//	if (shrub_overlay)
//		{
//			shrub_alpha = 100;
//		}
}


void		Shrub_ToggleSnap()
{
	shrub_snap = 1 - shrub_snap;
}


void		Shrub_ToggleLightmap()
{
	shrub_lightmap = 1 - shrub_lightmap;
	if (shrub_lightmap)
		{
			shrub_decal = 0;
			button_state( shrub_panel , 39 , shrub_decal );
			shrub_shadow = 0;
			button_state( shrub_panel , 15 , shrub_shadow );
		}	
}


void		Shrub_ToggleDecal()
{
	shrub_decal = 1 - shrub_decal;
	if (shrub_decal)
		{
			shrub_lightmap = 0;
			button_state( shrub_panel , 38 , shrub_lightmap );
			shrub_shadow = 1;
			button_state( shrub_panel , 15 , shrub_shadow );
		}	
}


void		Shrub_LightmapAll(button_number)
{
	set( map_loadpanel , SHOW);
	wait(3);
	
	var modified_lightmap = 0;
	
	if (button_number == (var)40)
		{
			modified_lightmap = 1;		// lightmap
		}
	else if (button_number == (var)41)
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
			if (temp_ent.ENT_CATEGORY == (var)UIMODE_SHRUB)	
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


void		Shrub_MaterialSetup()
{
	wait(3);
	material_select = 13;
	Material_MatPanel_Init( mtl_shrub , NULL );
//	MaterialSetup_Init();				
}


//////////////////////////////////////////////////////////////////////////////////////
// editor placement


void	Shrub_OnClick()
{
	if (proc_status(Shrub_OnClick)>1)		// protection
		{
			return;
		}
	wait(-0.25);									// time interval of allowed function instances
	
	if (!mouse_left)								// single click
		{
			Shrub_CreateArea();
		}
	else												// holding button
		{
			if (proc_status(Shrub_CreateArea_Cont)<1)
				{
					Shrub_CreateArea_Cont();
				}
		}
}


void	 Shrub_CreateArea()
{	
	int i,k,m,temp_int;
	var j;
	STRING* temp_str = str_create("");
	random_seed(0);
//	collision_mode = 0; // no! it would disable collision of collision entitites!
	// check number of types to be used to increase loop length if less than 8, or exit if 0
	j = 0;
	for (i=0;i<max_shrub_items;i++)
		{
			if(shrub_switch_type[i])
			{
				j++;
			}
		}
	if (j==0) { return; }
	// set loop limit
	j = integer( max_shrub_items/j );
	j *= integer(shrub_density);			// *shrub_tilesize*shrub_tilesize; - needed much less than grass
	//	loop length depends on shrub_density 		// and shrub_tilesize
	for ( i=0 ; i<j ; i++)
		{
			k = i%max_shrub_items;
			if ((shrub_switch_type[k]) && (!str_cmpi(shrub_filenames[k],"-")))
				{					
					str_cpy( temp_str , shrub_foldername );
					str_cat( temp_str , shrub_filenames[k] );
					
					// set random pos within creator cube area and shrub_tilesize
					var tempx = shrub_placement_cube.x + random(shrub_tilesize*GetMapTileSize()) - shrub_tilesize*GetMapTileSize()/2;
					var tempy = shrub_placement_cube.y + random(shrub_tilesize*GetMapTileSize()) - shrub_tilesize*GetMapTileSize()/2;
//					var tempz = shrub_placement_cube.z;					
					
					//snap to tile middle
					if (shrub_snap) 
						{
							temp_int = XCoordToTile(tempx);
							tempx = XTileToCoord(temp_int);
							
							temp_int = YCoordToTile(tempy);
							tempy = YTileToCoord(temp_int);
						}
					
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
					
					// put a shrub consisting of more crossing sprites
					shrub_piece_counter = 0;
					for ( m=0 ; m<shrub_pieces ; m++)
						{							
							shrub_piece_counter++;
							Shrub_CreateObject(temp_str, vector(tempx,tempy,tempz));
//							ent_create( temp_str , vector(tempx,tempy,tempz) , Shrub_CreateItem );						
						}
				}
		}
//	wait(1);
//	collision_mode = 1; 
	ptr_remove(temp_str);
}


void		Shrub_CreateArea_Cont()
{
	if (proc_status(Shrub_CreateArea_Cont) > 1)	return;
	
	int i,k,m,temp_int;
	var j;
	STRING* temp_str = str_create("");
	random_seed(0);
//	collision_mode = 0; 
	// check number of types to be used to increase loop length if less than 8, or exit if 0
	j = 0;
	for (i=0;i<SHRUB_MAXPERSET;i++)
		{
			if(shrub_switch_type[i])
			{
				j++;
			}
		}
	if (j==0) { return; }
	i = 0;
	while (mouse_left)
		{
			k = i%max_shrub_items;
			if ((shrub_switch_type[k]) && (!str_cmpi(shrub_filenames[k],"-")))
				{					
					str_cpy( temp_str , shrub_foldername );
					str_cat( temp_str , shrub_filenames[k] );
					
					// set random pos within creator cube area and shrub_tilesize
					var tempx = shrub_placement_cube.x + random(shrub_tilesize*GetMapTileSize()) - shrub_tilesize*GetMapTileSize()/2;
					var tempy = shrub_placement_cube.y + random(shrub_tilesize*GetMapTileSize()) - shrub_tilesize*GetMapTileSize()/2;
//					var tempz = shrub_placement_cube.z;
					
					//snap to tile middle
					if (shrub_snap) 
						{
							temp_int = XCoordToTile(tempx);
							tempx = XTileToCoord(temp_int);
							
							temp_int = YCoordToTile(tempy);
							tempy = YTileToCoord(temp_int);
						}
					
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
					
					shrub_piece_counter = 0;
					for ( m=0 ; m<shrub_pieces ; m++)
						{							
							shrub_piece_counter++;
							Shrub_CreateObject(temp_str, vector(tempx,tempy,tempz));
//							ent_create( temp_str , vector(tempx,tempy,tempz) , Shrub_CreateItem );							
						}
					
					wait(-0.1);		// sets placement speed
				}
			i++;			
		}
//	collision_mode = 1; 
	ptr_remove(temp_str);
	// update clusters touched should be done when shrub placement mode is left
}


/////////////////////////////////////////////////////////////
// seeding


void		Shrub_SeedQuantity()
{
	set( map_loadpanel , SHOW);
	wait(3);
	//----------------------------------------
	STRING* temp_str = str_create("");
	int h, i, j, k, m = 0;
	
	//----------------------------------------
	// check number of types to be used, to exit if 0	
	
	for (i=0;i<max_shrub_items;i++)
		{
			if(shrub_switch_type[i])
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
//	collision_mode = 0; 
	
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
					
					//snap to tile middle
					if (shrub_snap) 
						{
							temp_int = XCoordToTile(posx);
							posx = XTileToCoord(temp_int);
							
							temp_int = YCoordToTile(posy);
							posy = YTileToCoord(temp_int);
						}				
					
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
			while((!shrub_switch_type[k]) || (str_cmpi(shrub_filenames[k],"-")));	// repeat until nonselected or empty item found							
				
			str_cpy( temp_str , shrub_foldername );
			str_cat( temp_str , shrub_filenames[k] );	
						
			// put a shrub consisting of more crossing sprites
			shrub_piece_counter = 0;
			for ( h=0 ; h<shrub_pieces ; h++)
				{							
					shrub_piece_counter++;
					
					// place entity
					Shrub_CreateObject(temp_str, vector(posx,posy,posz));
//					ent_create( temp_str , vector(posx,posy,posz) , Shrub_CreateItem );							
				}	
			
			// progress bar
			if (i%10==0)		// at every 100th entity loading
				{
					Map_Progressbar(i/seed_quantity*100);
					wait_for(Map_Progressbar);
					wait(1);
				}	
		}
	//----------------------------------------
	wait(1);
//	collision_mode = 1; 
	ptr_remove(temp_str);
	reset(map_loadbar,SHOW); 
	reset( map_loadpanel , SHOW);
}


void		Shrub_SeedArea()
{
	set( map_loadpanel , SHOW);
	wait(3);
	//----------------------------------------
	STRING* temp_str = str_create("");	
	
	//----------------------------------------
	// check number of types to be used, to exit if 0	
	
	int a, b = 0;
	
	for (a=0;a<max_shrub_items;a++)
		{
			if(shrub_switch_type[a])
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
	var h,i,j,posx, posy;
	int c = 0;
	int temp_int;
	var summed_bool = 0;
	random_seed(0);
//	collision_mode = 0; 
	int m = 0;
	int k = 0;
	
	for (i = -outer_areax + seed_distance/2; i < outer_areax - seed_distance/2; i+=seed_distance)
		{
			for (j = -outer_areay + seed_distance/2; j < outer_areay - seed_distance/2; j+=seed_distance)
				{
					// get a position					
					posx = i + random(seed_rnddist*2) - seed_rnddist;
					posy = j + random(seed_rnddist*2) - seed_rnddist;
					
					// snap to tile middle
					if (shrub_snap) 
						{
							temp_int = XCoordToTile(posx);
							posx = XTileToCoord(temp_int);
							
							temp_int = YCoordToTile(posy);
							posy = YTileToCoord(temp_int);
						}				
					
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
									k = m % max_shrub_items;
								}
							while((!shrub_switch_type[k]) || (str_cmpi(shrub_filenames[k],"-")));	// repeat until nonselected or empty item found							
								
							str_cpy( temp_str , shrub_foldername );
							str_cat( temp_str , shrub_filenames[k] );									
							
							// put a shrub consisting of more crossing sprites
							shrub_piece_counter = 0;
							for ( h=0 ; h<shrub_pieces ; h++)
								{							
									shrub_piece_counter++;
									
									// place entity
									Shrub_CreateObject(temp_str, vector(posx,posy,posz));
//									ent_create( temp_str , vector(posx,posy,posz) , Shrub_CreateItem );							
								}	
							c++;
							
							// progress bar
							if (c%10==0)		// at every 100th entity loading
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
//	collision_mode = 1; 
	ptr_remove(temp_str);
	reset(map_loadbar,SHOW); 
	reset( map_loadpanel , SHOW);
}


////////////////////////////////////////////////////////////


void	 Shrub_CreateObject(STRING* ent_name_full, VECTOR* pos_vec)
{
	STRING* temp_str = str_create("");
	str_cpy(temp_str, ent_name_full);
	
	//----------------------------------------------------------------------------
	// create object
	
	var my_scale = GetMapTileSize() / shrub_defaultsize * shrub_scale * 2;
	
	var my_scale_x = my_scale;
	var my_scale_y = my_scale;
	
//	if (shrub_randomize)
//		{
//			// uniform randomness to keep object size ratio
//			my_scale *= (1+(random(20)-10)/100);
//			
////			// 2-fold randomness
////			my_scale_x *= (1+(random(30)-15)/100);
////			my_scale_y *= (1+(random(30)-15)/100);
//		}
	
	//---
	
	// randomize x,y position to avoid rendering problems because of changing sorting of transparent entities having the same origin
	pos_vec.x += random(2) -1;	// +/-1
	pos_vec.y += random(2) -1;	// +/-1
	
	//---
	// adjust over surface
	
	BMAP* temp_bmap = bmap_create(ent_name_full);
	
	pos_vec.z += bmap_height(temp_bmap) * my_scale_y * 0.35;	// up by 0.x of its image actual size in quants - 0.5 means at surface
	
	//---
	
	var my_pan = 0;
	var my_tilt = 0;
	
	// random rotation - no billboard behaviour
	if (shrub_pieces > (var)1)
		{
   		my_pan = 360 / shrub_pieces * shrub_piece_counter + random(30) - 15;
   		my_tilt = random(30);   
   	}
	
	//---
	
	var my_clip = 0;
	
	if (shrub_piece_counter==(var)1)				
		{
			my_clip = 2;									// 75%
		}
	else if (shrub_piece_counter==(var)2)		
		{
			my_clip = 1;									// 50%
		}
//	else														// most of shrub components
//		{
//			my_clip = 0;									// 25%
//		}
		
	//---
	
	var my_collider = 0;
	
	if (shrub_piece_counter == (var)1)
		{
			my_collider = 1;
		}
	
	//---
	
	Shrub_Create(	temp_str, 
						my_scale_x, my_scale_y, 
						pos_vec.x, pos_vec.y, pos_vec.z, 
						my_pan, my_tilt, 
						my_collider, shrub_alpha, shrub_ambient, 
						shrub_light, shrub_red, shrub_green, shrub_blue, 
						shrub_unlit, shrub_shadow, shrub_overlay, shrub_sprite,
						1-shrub_lightmap, shrub_decal, my_clip);
//	wait_for(Shrub_Create);
	
	//----------------------------------------------------------------------------			
//			
//	ent_create( temp_str, pos_vec, Shrub_CreateItem );	
//			
	//----------------------------------------------------------------------------
	
//	wait(1);
	ptr_remove(temp_str);
	ptr_remove(temp_bmap);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// deletion


void		Shrub_DeleteArea()
{
	Shrub_RemoveArea(square_x, square_y, shrub_tilesize, shrub_tilesize);	
}


void		Shrub_DeleteAll()
{
	wait(3);

	Shrub_RemoveAll();
	wait_for(Shrub_RemoveAll);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif