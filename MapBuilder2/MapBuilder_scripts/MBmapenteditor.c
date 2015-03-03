
#ifndef MBmapenteditor_c
#define MBmapenteditor_c

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


//////////////////////////////////////////////////////////////////////////////////////////////
// editor initialization


void 		MapEnt_Init()
{
	mapent_ready = 0;		
	//----------------------------------------------------------------
	// mapent model filename initialization 
sys_marker("mi1");	
	int arraysize = (int)MAPENT_MAXPERSET;
	mapent_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );
	int i;	
	// init filename string array elements
	for(i=0;i<max_mapent_items;i++)
		{
			mapent_filenames[i] = str_create("");
		}	
sys_marker(NULL);		
	//-----------------------------------------------------------------
	// determine how many .mdl files are in the folder, 
	// divide it by 8, and the result is the number of sets
//	mapent_set = 1;
	max_mapent_sets = MAPENT_MAXSET;
	mapents_txt = txt_create( (max_mapent_sets*max_mapent_items) , 0 );		// 10*8=80
	for(i=0;i<max_mapent_sets*max_mapent_items;i++)
		{
			str_cpy( (mapents_txt->pstring)[i] , "" );
		}
	//-------------------------------------------------------------------
	// read in folder content
	i = 0;
	STRING* temp_str = str_create("");
	str_cpy( temp_str , mapent_foldername );
	str_cat( temp_str , "*.wmb" );
	txt_for_dir( mapents_txt, temp_str ); 										//	better not to use wmb or hmp
	while (str_len((mapents_txt->pstring)[i]) > 0)
		{
			i++;
		}
	max_mapent_sets = integer(i/max_mapent_items);
	if ( fraction(i/max_mapent_items) > 0)
		{
			max_mapent_sets++;		// allow to display not complete sets too
		}
	mapent_set = clamp(mapent_set, 1, max_mapent_sets);
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	for(i=0;i<max_mapent_items;i++)
		{
//			str_cpy( mapent_filenames[i] , mapent_foldername );							// subfolder
//			str_cat( mapent_filenames[i] , (mapents_txt->pstring)[i] );					// filename with extension			
			str_cpy( mapent_filenames[i] , "" );												// (or mapent_foldername) subfolder
			int j = (mapent_set - 1) * max_mapent_items;
			str_cat( mapent_filenames[i] , (mapents_txt->pstring)[j+i] );				// filename with extension
			if ( str_cmpi( mapent_filenames[i] , "" ) )										// (or mapent_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( mapent_filenames[i] , "-" );
				}
		}	
	//------------------------------------------------------------------
	// initial value settings	
	for(i=0;i<max_mapent_items;i++)
		{
			mapent_switch_type[i] = 0;
		}
	mapent_switch_type[mapent_selected] = 1;
//	mapent_tilesize = 1;
//	mapent_density = 3;	
	//----------------------------------------------------------------
	// read values from .obs file
	MapEnt_OpenCreate_ObsFile( mapent_filenames[mapent_selected] );
	wait_for(MapEnt_OpenCreate_ObsFile);
	//-------------------------------------------------------------------
	// create objects
	collision_mode = 0; 
	mapent_model_preview = ent_create( NULL , vector(square_coord_x,square_coord_y,square_coord_z) , MapEnt_ModelPreview );
	wait(1);
//	ent_clone(mapent_model_preview);		// CLO
	collision_mode = 1; 
	
//	mapent_unlit = 0;
//	mapent_light = 1;

	MapEnt_Panel_Init();
	wait_for(MapEnt_Panel_Init);
	
	//-------------------------------------------------------
	MapEnt_MorphModelPreview();
	wait_for(MapEnt_MorphModelPreview);
	//-------------------------------------------------------
}


void		MapEnt_Close()
{
	mapent_ready = 0;		
	
	//-------------------------------------------------------
	ptr_remove(mapent_model_preview);
	mapent_model_preview = NULL;
	
	ptr_remove(mapent_panel);
	mapent_panel = NULL;
	
//	ptr_remove(mtl_mapent);		
	//--------------------------------------------------------
	int i;
	// remove temp text
	for (i = 0; i < mapents_txt->strings; i++) 
		if ((mapents_txt->pstring)[i]) 
			ptr_remove((mapents_txt->pstring)[i]);
	ptr_remove(mapents_txt);
	mapents_txt = NULL;
	 
	// remove temp string array
	for(i=0;i<max_mapent_items;i++)
		{
			ptr_remove(mapent_filenames[i]);
		}
	sys_free(mapent_filenames);
	mapent_filenames = NULL;
//	ptr_remove(mapent_filenames);		// causes error
}


action 	MapEnt_ModelPreview()
{
	set(my, PASSABLE | LIGHT | CAST);	// | TRANSLUCENT
	my.flags2 |= UNTOUCHABLE;
	my.red = 0;
	my.green = 255;
	my.blue = 0;
//	my.alpha = 50;
	vec_scale( my.scale_x , GetMapTileSize()/mapent_defaultsize*mapent_tilesize );
//	my.material = mtl_placementarea;																	// can't be different in case of map entities ! results in bad rendering
	my.group = 1;
	
	while(UIMode==(var)UIMODE_MAPENT)
		{
			if (mapent_ready)
				{
					//-------------------------
					// mouse 3d position
					
					VECTOR* temp_vec = NULL;
//					temp_vec = MousePos3D4OrZeroLevel();					// on ground terrain + terrain mdl
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
					
					if (temp_vec)
						{
							vec_set(my.x, temp_vec);
						}
					
					//-------------------------
					// snap xy					
					
					if (mapent_snap) 
						{
							// snap xy
							vec_set(my.x, SnapCoordToTileMidCoordXY(&my.x));
							
							// modify snapping due to object size
							if (mapent_tilesize_x % 2 == 0)
								{
									my.x += GetMapTileSize() / 2;
								}
							if (mapent_tilesize_y % 2 == 0)
								{
									my.y += GetMapTileSize() / 2;
								}
						}
					
					//-------------------------
					// snap z
					
					if (mapent_snapz) 
						{
							vec_set(my.x, SnapCoordToTileCoordZ(&my.x));	
						}
					
					//-------------------------
					// scale
					
					vec_scale( my.scale_x , GetMapTileSize() / mapent_defaultsize * mapent_scale / my.scale_x );
					
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
	MapEnt_Close();
}


void MapEnt_MorphModelPreview()
{
	mapent_ready = 0;		
	
	STRING* temp_str = str_create("");
	if ( !str_cmpi(mapent_filenames[mapent_selected] , "-" ) )
		{					
			str_cpy( temp_str , mapent_foldername );
			str_cat( temp_str , mapent_filenames[mapent_selected] );
			if (file_exists(temp_str))
				ent_morph( mapent_model_preview , temp_str );
		}
	else
		{
			ent_morph( mapent_model_preview , "graphics\\null.mdl" );
		}
		
	wait(1);
	
	mapent_ready = 1;		
}


void		MapEnt_RotateModelPreview(int rotation)
{
	int direction = AngleToDirection( mapent_model_preview.pan );
	
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
	
	mapent_model_preview.pan = DirectionToAngle( direction );
}


void		MapEnt_TurnModelPreview(int rotation)
{
	var turnrate = rotation * time_step * 16;
	
	mapent_model_preview.pan += turnrate;
}


/////////////////////////////////////////////////


void		MapEnt_OpenCreate_ObsFile( STRING* mapent_model_filename )
{
	if (mapent_ready) return;
	if (proc_status(MapEnt_OpenCreate_ObsFile) > 0) return;
	
	STRING* temp_str = str_create("");
	//-----------------------------------------------------------------------
	// get filename from model name
	str_cpy( temp_str , mapent_foldername );
	str_cat( temp_str , mapent_model_filename );
	str_trunc( temp_str , 4 ); 
	str_cat( temp_str , ".obs" );	
	//-----------------------------------------------------------------------
	var file_handle = file_open_read( temp_str );
	//-----------------------------------------------------------------------
	// read file if exists
	if (file_handle)
		{
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			mapent_defaultsize = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			mapent_tilesize_x = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			mapent_tilesize_y = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			mapent_shadow = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			mapent_type = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			mapent_collision = str_to_num( temp_str );
			
			file_close(file_handle);	
		}
	// create file is non-existing (import model with default setttings)
	else
		{
			mapent_defaultsize		= (var)MAP_DEFAULTGRIDSIZE;
			mapent_tilesize_x		= 3;
			mapent_tilesize_y		= 3;			
			mapent_type				= 0;								// (passability) 0 no access, 1 default, 2 building, (3 forest)
			mapent_collision		= 1;								// 0 passable, 1 polygon, 2 tile area
			mapent_shadow			= 2;								// 0 nothing, 1 dynamic, 2 sprite, 3 model
			
			file_handle = file_open_write( temp_str );
			
			file_str_write( 		file_handle , "// reference tile size // 32 by default // min 16 // max 100" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);						
			str_for_int( 			temp_str 	, mapent_defaultsize );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_str_write( 		file_handle , "// area width in tiles" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, mapent_tilesize_x );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_str_write( 		file_handle , "// area height in tiles" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, mapent_tilesize_y );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// shadow type // 0 no shadow // 1 dynamic shadow // 2 decal shadow // 3 model shadow" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, mapent_shadow );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// tile type // 0 no access // 1 terrain default // 2 building" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, mapent_type );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// collision type // 0 passable // 1 polygon // 2 tile area based" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, mapent_collision );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_close(file_handle);
		}		
//	wait(1);
	ptr_remove(temp_str);
}


///////////////////////////////////////////////////////////////////////////////////


void		MapEnt_Panel_Init()
{	
	int i;
	STRING* temp_string = str_create("");
	
	mapent_panel = pan_create( panel_image2 , 11 );
	set( mapent_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	mapent_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	mapent_panel.pos_y 	= ui_submenupanel_y;								// 256;
	mapent_panel.red		= panel_font_r;
	mapent_panel.green	= panel_font_g;
	mapent_panel.blue		= panel_font_b;
	mapent_panel.event	= ui_submenu_panelmove;
	
	// string 1
	pan_setstring(mapent_panel, 0,	15,10,	panel_font2,			str_create("MAP-ENTITY PLACEMENT") );
	// string 2
	pan_setstring(mapent_panel, 0,	15,330,	panel_font,			str_create("Select object") );
	
	// strings 3-10 - mapent item numbers 1-8
	for(i=0;i<max_mapent_items;i++)
		{
			str_for_int( temp_string , (i+1) );
			pan_setstring( mapent_panel, 0,	35,350+i*15,	panel_font,			str_create( temp_string ) );
		}
	
	// strings 11-18 - mapent item filenames 1-8
	for(i=0;i<max_mapent_items;i++)
		{
			pan_setstring(mapent_panel, 0,	55,350+i*15,	panel_font,			str_create( mapent_filenames[i] ) );
		}
	
	// buttons 1-8 (checkbox)
	for(i=0;i<max_mapent_items;i++)
		{
			pan_setbutton(mapent_panel, 0,	2, 	15,350+i*15,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_Flag_Toggle,NULL,NULL); 
		}
	// button states
	for(i=0;i<max_mapent_items;i++)
		{
			button_state( mapent_panel , 1+i , mapent_switch_type[i] ); 		// 1 ON, 0 OFF
		}
	
	// button 9 - Unlit
	pan_setstring(mapent_panel, 0,	40,180,	panel_font,			str_create("set UNLIT") );	
	pan_setbutton(mapent_panel, 0,	2, 	15,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_ToggleUnlit,NULL,NULL); 
	button_state( mapent_panel , 9 , mapent_unlit ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(mapent_panel, 0,	15,30,	"COUNT: %0.f",				panel_font,	1, mapent_counter);
	
	// buttns 10-11 - mapent set
	pan_setdigits(mapent_panel, 0,	180,330,	"Set:    %0.f",		panel_font,	1, mapent_set);	
	pan_setbutton(mapent_panel, 0,	1, 	165,330,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				MapEnt_DecSet, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	225,330,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			MapEnt_IncSet, 		NULL, NULL); 
		
	// buttons 12-13 - draw area - not used !!! dummy
	pan_setdigits(mapent_panel, 0,	255 /*110+12*/,240,	" ",		panel_font,	1, mapent_tilesize);	
	pan_setbutton(mapent_panel, 0,	1, 	255 /*95+12*/,241,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,NULL,				NULL, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	255 /*182+12*/,241,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,NULL,				NULL, 		NULL, NULL); 
	
	// density	- not used !!! dummy
//	pan_setdigits(mapent_panel, 0,	15,200,	" ",		panel_font,	0.85, 							mapent_density );
//	pan_setslider(mapent_panel, 0,	90+12,202,	horiz_slider_bmp,slider_knob_bmp,	1,max_mapent_density+0.4,	mapent_density); 
	
	// scale
	pan_setdigits(mapent_panel, 0,	15,220,	"Scale: %0.2f",		panel_font,	1, 				mapent_scale);
	pan_setslider(mapent_panel, 0,	90+12,222,	horiz_slider_bmp,slider_knob_bmp,	0.75,1.25,		mapent_scale);
	
	// button 14 - Light
	pan_setstring(mapent_panel, 0,	40,100,	panel_font,			str_create("set LIGHT to adjust RGB values:") );	
	pan_setbutton(mapent_panel, 0,	2, 	15,100,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_ToggleLight,NULL,NULL); 
	button_state( mapent_panel , 14 , mapent_light ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(mapent_panel, 0,	15,120,	"R: %0.f",				panel_font,	1, 			mapent_red);
	pan_setdigits(mapent_panel, 0,	15,140,	"G: %0.f",				panel_font,	1, 			mapent_green);
	pan_setdigits(mapent_panel, 0,	15,160,	"B: %0.f",				panel_font,	1, 			mapent_blue);
	pan_setslider(mapent_panel, 0,	90+12,122,	horiz_slider_bmp,slider_knob_bmp,	0,255,		mapent_red); 
	pan_setslider(mapent_panel, 0,	90+12,142,	horiz_slider_bmp,slider_knob_bmp,	0,255,		mapent_green); 
	pan_setslider(mapent_panel, 0,	90+12,162,	horiz_slider_bmp,slider_knob_bmp,	0,255,		mapent_blue); 
		
	pan_setdigits(mapent_panel, 0,	15,60,	"Ambient: %0.f",		panel_font,	1, 			mapent_ambient);
	pan_setslider(mapent_panel, 0,	90+12,62,	horiz_slider_bmp,slider_knob_bmp,	0,100,		mapent_ambient); 
	
	pan_setdigits(mapent_panel, 0,	15,80,	"Alpha: %0.f",			panel_font,	1, 			mapent_alpha);
	pan_setslider(mapent_panel, 0,	90+12,82,	horiz_slider_bmp,slider_knob_bmp,	0,100,		mapent_alpha); 
	
	// button 15 - random fine Scale
//	pan_setstring(mapent_panel, 0,	40,260,	panel_font,			str_create("rnd fine scale") );	
//	pan_setbutton(mapent_panel, 0,	2, 	15,260,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_ToggleRandomize,NULL,NULL); 
//	button_state( mapent_panel , 15 , mapent_randomize ); 		// 1 ON, 0 OFF	
	pan_setstring(mapent_panel, 0,	255,260,	panel_font,			str_create("") );
	pan_setbutton(mapent_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 16 - Overlay
	pan_setstring(mapent_panel, 0,	40,300,	panel_font,			str_create("set OVERLAY") );
	pan_setbutton(mapent_panel, 0,	2, 	15,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_ToggleOverlay,NULL,NULL); 
	button_state( mapent_panel , 16 , mapent_overlay ); 		// 1 ON, 0 OFF	
	
	// button 17 - Clear All
	pan_setbutton(mapent_panel, 0,	1, 	45,480,		m_clearall_on_bmp,m_clearall_off_bmp,m_clearall_on_bmp,m_clearall_off_bmp,		MapEnt_DeleteAll,NULL,NULL); 
	
	// material buttons-----------------------
	
	// button 18 - Material - removed
//	pan_setbutton(mapent_panel, 0,	1, 	200,10,		ss_material_on_bmp,ss_material_off_bmp,ss_material_over_bmp,ss_material_over_bmp,		MapEnt_MaterialSetup,NULL,NULL); 			
	pan_setbutton(mapent_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 19 - mat_model
	pan_setstring(mapent_panel, 0,	40+100,180,	panel_font,			str_create("set shaded material") );	
	pan_setbutton(mapent_panel, 0,	2, 	15+100,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_ToggleModel,NULL,NULL); 
	button_state( mapent_panel , 19 , mapent_model ); 		// 1 ON, 0 OFF	
	
	//----------------------------------------
	
	// button 20 - random rotation - removed
//	pan_setstring(mapent_panel, 0,	40,280,	panel_font,			str_create("rnd rotation") );	
//	pan_setbutton(mapent_panel, 0,	2, 	15,280,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_ToggleRndRot,NULL,NULL); 
//	button_state( mapent_panel , 20 , mapent_rnd_rot ); 		// 1 ON, 0 OFF	
	pan_setbutton(mapent_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 21-22-23 - auto tree - removed
	pan_setstring(mapent_panel, 0,	35,262,	panel_font,			str_create("") );
	pan_setbutton(mapent_panel, 0,	4, 	255,262,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,NULL,		NULL,NULL,NULL); 
//	if (mapent_istree==(var)0) button_state( mapent_panel , 21 , ON ); 		// 1 ON, 0 OFF	
	pan_setstring(mapent_panel, 0,	115,262,	panel_font,			str_create("") );
	pan_setbutton(mapent_panel, 0,	4, 	255,262,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,NULL,		NULL,NULL,NULL); 
//	if (mapent_istree==(var)1) button_state( mapent_panel , 22 , ON ); 		// 1 ON, 0 OFF	
	pan_setstring(mapent_panel, 0,	195,262,	panel_font,			str_create("") );
	pan_setbutton(mapent_panel, 0,	4, 	255,262,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,NULL,		NULL,NULL,NULL); 
//	if (mapent_istree==(var)2) button_state( mapent_panel , 23 , ON ); 		// 1 ON, 0 OFF	
	
	// button 24 - set to Lightmap
//	if (shadow_stencil==(var)8)
		{
			pan_setstring(mapent_panel, 0,	40+100,300,	panel_font,			str_create("Lightmap") );
		}
//	else
//		{
//			pan_setstring(mapent_panel, 0,	40+100,300,	panel_font,			str_create("set to Lightmap") );
//		}
	pan_setbutton(mapent_panel, 0,	2, 	15+100,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_ToggleLightmap,NULL,NULL); 
	button_state( mapent_panel , 24 , mapent_lightmap ); 		// 1 ON, 0 OFF	
	
	// button 25 - Modify - removed
//	pan_setbutton(mapent_panel, 0,	1, 	150,480,		m_modify_on_bmp,m_modify_off_bmp,m_modify_over_bmp,m_modify_over_bmp,				ModifyObject_Init,		NULL,NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	255,580,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,		NULL,							NULL,NULL); 
	
	// button 26 - Snap
	pan_setstring(mapent_panel, 0,	15+20+20,240,	panel_font,			str_create("Snap XY / Z") );
	pan_setbutton(mapent_panel, 0,	2, 	15,240,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_ToggleSnap,NULL,NULL); 
	button_state( mapent_panel , 26 , mapent_snap ); 		// 1 ON, 0 OFF	
	
	// button 27,28	29,30		
	pan_setbutton(mapent_panel, 0,	1, 	255,200,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,NULL,													NULL, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	255,200,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,NULL,													NULL, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	90,220,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				MapEnt_Panel_Button, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	210,220,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		MapEnt_Panel_Button, 		NULL, NULL); 
	// button 31,32	33,34		35,36
	pan_setbutton(mapent_panel, 0,	1, 	90,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				MapEnt_Panel_Button, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	210,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		MapEnt_Panel_Button, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	90,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				MapEnt_Panel_Button, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	210,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		MapEnt_Panel_Button, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	90,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				MapEnt_Panel_Button, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	210,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		MapEnt_Panel_Button, 		NULL, NULL); 
	// button 37,38	39,40
	pan_setbutton(mapent_panel, 0,	1, 	90,60,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				MapEnt_Panel_Button, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	210,60,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		MapEnt_Panel_Button, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	90,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				MapEnt_Panel_Button, 		NULL, NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	210,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		MapEnt_Panel_Button, 		NULL, NULL); 
	
	// button 41 - Edit
	pan_setbutton(mapent_panel, 0,	1, 	100,330,		ss_edit_on_bmp,ss_edit_off_bmp,ss_edit_over_bmp,ss_edit_over_bmp,		MapEnt_EditFile,		NULL,NULL); 			
	
	// button 42 - SnapZ
	pan_setstring(mapent_panel, 0,	75+20,240,	panel_font,			str_create("") );
	pan_setbutton(mapent_panel, 0,	2, 	15+20,240,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_ToggleSnapZ,NULL,NULL); 
	button_state( mapent_panel , 42 , mapent_snapz ); 		// 1 ON, 0 OFF	
	
	// button 43 - force decal shadow (ent_decal) - useful when the evel is used in both shadow_stencil=1 and shadow_stencil=8 modes
//	if (shadow_stencil==(var)8)
		{
			pan_setstring(mapent_panel, 0,	40+175,300,	panel_font,			str_create("Decal") );
			pan_setbutton(mapent_panel, 0,	2, 	15+175,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		MapEnt_ToggleDecal,NULL,NULL); 
			button_state( mapent_panel , 43 , mapent_decal ); 		// 1 ON, 0 OFF	
		}
	
	// button 44 45 - Lightmap All : Set / Reset
	pan_setstring(mapent_panel, 0,			130,30,		panel_font,			str_create("Ligthmap All") );
	pan_setbutton(mapent_panel, 0,	1, 	200,20,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,					MapEnt_LightmapAll,		NULL,NULL); 
	pan_setbutton(mapent_panel, 0,	1, 	200,40,		ss_reset_on_bmp,ss_reset_off_bmp,ss_reset_over_bmp,ss_reset_over_bmp,		MapEnt_LightmapAll,		NULL,NULL); 
	
	//--------------------------------------------------------------
	
	ptr_remove( temp_string );	
	
//	while(UIMode==(var)UIMODE_MAPENT)
//		{
//			wait(1);
//		}
//	MapEnt_Close();
}


void		MapEnt_Panel_Toggle()
{
	toggle( mapent_panel , SHOW );	
}


////////////////////////////////////////////////////


void		MapEnt_Panel_Button(button_number)
{
	// mapent_density
	if (button_number==(var)27)
		{
//			mapent_density = maxv( 1 , --mapent_density );
		}
	else if (button_number==(var)28)
		{
//			mapent_density = minv( max_mapent_density , ++mapent_density );
		}
	// mapent_scale
	else if (button_number==(var)29)
		{
			mapent_scale -= 0.01;
			mapent_scale = maxv( 0.75 , mapent_scale );
		}
	else if (button_number==(var)30)
		{
			mapent_scale += 0.01;
			mapent_scale = minv( 1.25 , mapent_scale );
		}
	// mapent_red
	else if (button_number==(var)31)
		{
			mapent_red = maxv( 0 , --mapent_red );
		}
	else if (button_number==(var)32)
		{
			mapent_red = minv( 255 , ++mapent_red );
		}
	// mapent_green
	else if (button_number==(var)33)
		{
			mapent_green = maxv( 0 , --mapent_green );
		}
	else if (button_number==(var)34)
		{
			mapent_green = minv( 255 , ++mapent_green );
		}
	// mapent_blue
	else if (button_number==(var)35)
		{
			mapent_blue = maxv( 0 , --mapent_blue );
		}
	else if (button_number==(var)36)
		{
			mapent_blue = minv( 255 , ++mapent_blue );
		}
	// mapent_ambient
	else if (button_number==(var)37)
		{
			mapent_ambient = maxv( 0 , --mapent_ambient );
		}
	else if (button_number==(var)38)
		{
			mapent_ambient = minv( 100 , ++mapent_ambient );
		}
	// mapent_alpha
	else if (button_number==(var)39)
		{
			mapent_alpha = maxv( 0 , --mapent_alpha );
		}
	else if (button_number==(var)40)
		{
			mapent_alpha = minv( 100 , ++mapent_alpha );
		}	
}


/////////////////////////////////


//// change by hotkey
//void		MapEnt_NextSet()
//{
//	if (proc_status(MapEnt_NextSet) > 0) return;
//	if (proc_status(MapEnt_UpdateSetNames) > 0) return;
//	if (!mapent_ready) return;
//	mapent_ready = 0;
//	wait(3);
//	
//	// jump to next existing set
//	mapent_set++;
//	if (mapent_set>max_mapent_sets)
//		{
//			mapent_set = 1;
//		}
//	MapEnt_UpdateSetNames();
//	wait_for(MapEnt_UpdateSetNames);
//}
//

void		MapEnt_IncSet()
{
	if (proc_status(MapEnt_IncSet) > 0) return;
	if (proc_status(MapEnt_UpdateSetNames) > 0) return;
	if (!mapent_ready) return;
	mapent_ready = 0;
	wait(3);
	
	// jump to next existing set
	mapent_set++;
	if (mapent_set>max_mapent_sets)
		{
//			mapent_set = max_mapent_sets;
//			return;
			mapent_set = 1;
		}
	MapEnt_UpdateSetNames();
	wait_for(MapEnt_UpdateSetNames);
}


void		MapEnt_DecSet()
{
	if (proc_status(MapEnt_DecSet) > 0) return;
	if (proc_status(MapEnt_UpdateSetNames) > 0) return;
	if (!mapent_ready) return;
	mapent_ready = 0;
	wait(3);
	
	// jump to next existing set
	mapent_set--;
	if (mapent_set<1)
		{
//			mapent_set = 1;
//			return;
			mapent_set = max_mapent_sets;
		}
	MapEnt_UpdateSetNames();
	wait_for(MapEnt_UpdateSetNames);
}


void		MapEnt_UpdateSetNames()
{
	if (proc_status(MapEnt_UpdateSetNames) > 0) return;
	
	int i;
	// copy the proper 8 filenames to string array
	int j = mapent_set - 1;
	int k = j * max_mapent_items;
	for(i=0;i<max_mapent_items;i++)
		{
//			str_cpy( mapent_filenames[i] , mapent_foldername );						// subfolder
//			str_cat( mapent_filenames[i] , (mapents_txt->pstring)[k+i] );			// filename with extension
			str_cpy( mapent_filenames[i] , "" );											// (or mapent_foldername) subfolder
			str_cat( mapent_filenames[i] , (mapents_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi( mapent_filenames[i] , "" ) )								// (or mapent_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( mapent_filenames[i] , "-" );
				}
		}
	// refresh 8 filename strings to display in menu
	for(i=0;i<max_mapent_items;i++)
		{
			pan_setstring(mapent_panel, 11+i,	55,350+i*15,	panel_font,			str_create( mapent_filenames[i] ) );
		}
	wait(1);
		
	// read values from .obs file
	MapEnt_OpenCreate_ObsFile( mapent_filenames[mapent_selected] );
	wait_for(MapEnt_OpenCreate_ObsFile);
	wait(1);
	
	MapEnt_MorphModelPreview();
	wait_for(MapEnt_MorphModelPreview);
	wait(1);
}


///////////////////////////////////


// change by hotkey
void		MapEnt_Flag_Switch(var setplusone)
{
	if (proc_status(MapEnt_Flag_Switch) > 0) return;
	if (!mapent_ready) return;
	mapent_ready = 0;
	wait(3);
	
	int temp = setplusone -1 -1;	// -1 because of scancode, and -1 because of array pos are 0..7
	
//	mapent_switch_type[temp] = 1 - mapent_switch_type[temp];
//	// change checkbox state too
//	var temp_state = button_state( mapent_panel , temp+1 , -1 ); 				// -1 to read value
//	button_state( mapent_panel , temp+1 , 1-temp_state ); 						// 1 ON, 0 OFF
	
	int i;
	for(i=0;i<max_mapent_items;i++)
		{
			mapent_switch_type[i] = 0;						// 0..7
			button_state( mapent_panel , i+1 , 0 );		// 0..8
		}
	mapent_switch_type[temp] = 1;
	button_state( mapent_panel , temp+1 , 1 ); 
	mapent_selected = temp;
	
	// read values from .obs file
	MapEnt_OpenCreate_ObsFile( mapent_filenames[mapent_selected] );
	wait_for(MapEnt_OpenCreate_ObsFile);
	wait(1);
	
	MapEnt_MorphModelPreview();
	wait_for(MapEnt_MorphModelPreview);
	wait(1);
}


// change by checkbox (buttons 1-8)
void		MapEnt_Flag_Toggle(button_number)
{
	if (proc_status(MapEnt_Flag_Toggle) > 0) return;
	if (!mapent_ready) return;
	mapent_ready = 0;
	wait(3);
	
//	mapent_switch_type[button_number-1] = 1 - mapent_switch_type[button_number-1] ;
	
	int i;
	for(i=0;i<max_mapent_items;i++)
		{
			mapent_switch_type[i] = 0;							// 0..7
			button_state( mapent_panel , i+1 , 0 );			// 1..8
		}
	mapent_switch_type[button_number-1] = 1;
	button_state( mapent_panel , button_number , 1 ); 
	mapent_selected = button_number-1;
	
	// read values from .obs file
	MapEnt_OpenCreate_ObsFile( mapent_filenames[mapent_selected] );
	wait_for(MapEnt_OpenCreate_ObsFile);
	wait(1);
	
	MapEnt_MorphModelPreview();
	wait_for(MapEnt_MorphModelPreview);
	wait(1);
}


/////////////////////////////////
// decrease/increase mapent densityif left mouse button is used for placement


//void 		MapEnt_DecDens()
//{
//	mapent_density--;
//	if (mapent_density<1)
//		{
//			mapent_density = 1;
//		}
//}
//
//
//void 		MapEnt_IncDens()
//{
//	mapent_density++;
//	if (mapent_density>max_mapent_density)
//		{
//			mapent_density = max_mapent_density;
//		}
//}


////////////////////////////////////


//// change by hotkey
//void 		MapEnt_DecSize()
//{
//	mapent_tilesize -= 2;
//	if (mapent_tilesize<1)
//		{
//			mapent_tilesize = 1;
//		}	
//	var temp = mapent_tilesize / mapent_placement_cube.scale_x ;	
//	vec_scale( mapent_placement_cube.scale_x , temp );
//}
//
//
//// change by hotkey
//void 		MapEnt_IncSize()
//{
//	mapent_tilesize += 2;
//	if (mapent_tilesize>max_mapent_size)
//		{
//			mapent_tilesize = max_mapent_size;
//		}
//	var temp = mapent_tilesize / mapent_placement_cube.scale_x ;	
//	vec_scale( mapent_placement_cube.scale_x , temp );
//}


/////////////////////////////////////////////////////
// change by checkbox


void		MapEnt_ToggleUnlit()
{
	mapent_unlit = 1 - mapent_unlit;	
	if (mapent_unlit)
		{
			// switch off LIGHT
			mapent_light = 0;
			button_state( mapent_panel , 14 , mapent_light );
			// switch off model
			mapent_model = 0;
			button_state( mapent_panel , 19 , mapent_model );
		}
}


void		MapEnt_ToggleLight()
{
	mapent_light = 1 - mapent_light;	
	if (mapent_light)
		{
			// switch off UNLIT
			mapent_unlit = 0;
			button_state( mapent_panel , 9 , mapent_unlit );
		}
}


void		MapEnt_ToggleModel()
{
	mapent_model = 1 - mapent_model;
	if (mapent_model)
		{
			// switch off UNLIT
			mapent_unlit = 0;
			button_state( mapent_panel , 9 , mapent_unlit );
		}
}


////void		MapEnt_ToggleRandomize()
//{
//	mapent_randomize = 1 - mapent_randomize;	
//}


void		MapEnt_ToggleOverlay()
{
	mapent_overlay = 1 - mapent_overlay;	
}


//void		MapEnt_ToggleRndRot()
//{
//	mapent_rnd_rot = 1 - mapent_rnd_rot;	
//	
//	// reset rotation
//	if (mapent_rnd_rot)
//		{
//			mapent_model_preview.pan = 0;
//		}
//}


void		MapEnt_ToggleLightmap()
{
	mapent_lightmap = 1 - mapent_lightmap;
	if (mapent_lightmap)
		{
			mapent_decal = 0;
			button_state( mapent_panel , 43 , mapent_decal );
		}
}


void		MapEnt_ToggleSnap()
{
	mapent_snap = 1 - mapent_snap;
}


void		MapEnt_ToggleSnapZ()
{
	mapent_snapz = 1 - mapent_snapz;
}


void		MapEnt_ToggleDecal()
{
	mapent_decal = 1 - mapent_decal;
	if (mapent_decal)
		{
			mapent_lightmap = 0;
			button_state( mapent_panel , 24 , mapent_lightmap );
		}
}


void		MapEnt_LightmapAll(button_number)
{
	set( map_loadpanel , SHOW);
	wait(3);
	
	var modified_lightmap = 0;
	
	if (button_number == (var)44)
		{
			modified_lightmap = 1;		// lightmap
		}
	else if (button_number == (var)45)
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
			if (temp_ent.ENT_CATEGORY == (var)UIMODE_MAPENT)	
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


void		MapEnt_MaterialSetup()
{
	wait(3);
	material_select = 16;
	Material_MatPanel_Init( mtl_mapent , NULL );	
//	MaterialSetup_Init();		// old settings panel		
}


void		MapEnt_EditFile()
{
	STRING* temp_str = str_create("");
	if ( !str_cmpi(mapent_filenames[mapent_selected] , "-" ) )
		{					
			str_cpy( temp_str , mapent_foldername );
			str_cat( temp_str , mapent_filenames[mapent_selected] );			
			ObswModify_Init( mapent_defaultsize,  mapent_tilesize_x,  mapent_tilesize_y,  mapent_type,  mapent_shadow,  mapent_collision, temp_str);			
		}
	ptr_remove(temp_str);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// editor placement


void	 MapEnt_CreateObject()
{
	if (!mapent_ready) return;
		
	STRING* temp_str = str_create("");
	if ( !str_cmpi(mapent_filenames[mapent_selected] , "-" ) )
		{					
			str_cpy( temp_str , mapent_foldername );
			str_cat( temp_str , mapent_filenames[mapent_selected] );
			
			//----------------------------------------------------------------------------
			// create object
			
//			var my_scale = GetMapTileSize()/mapent_defaultsize * mapent_scale;
			var my_scale = mapent_model_preview.scale_x;	
			
			MapEnt_Create(		temp_str, 
												
									mapent_defaultsize,
									mapent_tilesize_x, mapent_tilesize_y,
									mapent_type, mapent_collision, mapent_shadow,	
																	
									my_scale, my_scale, my_scale,		
									mapent_model_preview.x, mapent_model_preview.y, mapent_model_preview.z, 									
									mapent_model_preview.pan, mapent_model_preview.tilt, mapent_model_preview.roll,
									
									mapent_alpha, mapent_ambient, 
									mapent_light, mapent_red, mapent_green, mapent_blue, 
									mapent_unlit, mapent_overlay, mapent_model,										
									(1-mapent_lightmap), mapent_decal
								);
//			wait_for(Building_Create);
//			
			//----------------------------------------------------------------------------
//			
//			ent_create(temp_str, mapent_model_preview.x, MapEnt_CreateItem);
//			// to get its filename later: str_for_entfile( str_entfilename , my );	
//			
			//----------------------------------------------------------------------------	
		}
	wait(1);
	ptr_remove(temp_str);
}


void		MapEnt_CloneObject()
{
	//-----------------------------------------------------------------------
	// select
	
	mod_origmenu = (var)UIMODE_MAPENT;
	
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
	str_cpy( temp_str , mapent_foldername );
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
	// set building properties from .obs file 
	// defaultsize for scaling, x/y tile area for tile management, type (passability), collision shape, shadow
	
	var obs_defaultsize;
	var obs_tilesize_x;
	var obs_tilesize_y;
	var obs_type;
	var obs_collision;
	var obs_shadow;
	
	MapEnt_ReadObsFileData(str_for_entfile(NULL, you), &obs_defaultsize, &obs_tilesize_x, &obs_tilesize_y, &obs_type, &obs_collision, &obs_shadow);
	
	//-----------------------------------------------------------------------
	// collision
	
//	if (my.shadow != NULL)
	if (((shadow_stencil>(var)-1) && (shadow_stencil<(var)8)) || (is(my, FLAG5)))			// collider entity is needed when decal shadows are applied		// !!!	
		{
			MapEnt_CreateCollider(my);
		}
	
	//-----------------------------------------------------------------------
	// shadow - required by decal and ent_decal shadows
			
	MapEnt_SetShadow(my, (1-is(my, FLAG6)), is(my, FLAG5), obs_shadow);
	wait_for(MapEnt_SetShadow);
	
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


///////////////////////////////////////////////////////////////////////////////////////////////////////
// editor deletion


void		MapEnt_DeleteItem()
{
	MousePos3D0();
	
	if (HIT_TARGET)
		{
//			you = hit.entity;
			
			MapEnt_RemoveObject(hit.entity);
		}
}

void		MapEnt_DeleteAll()
{
	wait(3);

	MapEnt_RemoveAll();
	wait_for(MapEnt_RemoveAll);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////


#endif