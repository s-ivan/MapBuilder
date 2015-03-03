
#ifndef MBbuildeditor_c
#define MBbuildeditor_c

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


////////////////////////////////////////////////////////////////////////////////////
// editor menu


void 		Building_Init()
{
	building_ready = 0;		
	//----------------------------------------------------------------
	// obstacle model filename initialization 
sys_marker("oi1");	
	int arraysize = (int)BUILDING_MAXPERSET;
	building_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );
	int i;	
	// init filename string array elements
	for(i=0;i<max_building_items;i++)
		{
			building_filenames[i] = str_create("");
		}	
sys_marker(NULL);		
	//-----------------------------------------------------------------
	// determine how many .mdl files are in the folder, 
	// divide it by 8, and the result is the number of sets
//	building_set = 1;
	max_building_sets = BUILDING_MAXSET;
	buildings_txt = txt_create( (max_building_sets*max_building_items) , 0 );		// 10*8=80
	for(i=0;i<max_building_sets*max_building_items;i++)
		{
			str_cpy( (buildings_txt->pstring)[i] , "" );
		}
	//-------------------------------------------------------------------
	// read in folder content
	i = 0;
	STRING* temp_str = str_create("");
	str_cpy( temp_str , building_foldername );
	str_cat( temp_str , "*.mdl" );
	txt_for_dir( buildings_txt, temp_str ); 										//	better not to use wmb or hmp
	while (str_len((buildings_txt->pstring)[i]) > 0)
		{
			i++;
		}
	max_building_sets = integer(i/max_building_items);
	if ( fraction(i/max_building_items) > 0)
		{
			max_building_sets++;		// allow to display not complete sets too
		}
	building_set = clamp(building_set, 1, max_building_sets);
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	for(i=0;i<max_building_items;i++)
		{
//			str_cpy( building_filenames[i] , building_foldername );							// subfolder
//			str_cat( building_filenames[i] , (buildings_txt->pstring)[i] );				// filename with extension			
			str_cpy( building_filenames[i] , "" );													// (or building_foldername) subfolder
			int j = (building_set - 1) * max_building_items;
			str_cat( building_filenames[i] , (buildings_txt->pstring)[j+i] );				// filename with extension
			if ( str_cmpi( building_filenames[i] , "" ) )										// (or building_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( building_filenames[i] , "-" );
				}
		}	
	//------------------------------------------------------------------
	// initial value settings	
	for(i=0;i<max_building_items;i++)
		{
			building_switch_type[i] = 0;
		}
	building_switch_type[building_selected] = 1;
//	building_tilesize = 1;
//	building_density = 3;	
	//----------------------------------------------------------------
	// read values from .obs file
	Building_OpenCreate_ObsFile( building_filenames[building_selected] );
	wait_for(Building_OpenCreate_ObsFile);
	//-------------------------------------------------------------------
	// create objects
	
//	collision_mode = 0; 
//	building_placement_cube = ent_create( building_creator_model , vector(square_coord_x,square_coord_y,square_coord_z) , Building_Cube );
//	wait(1);
////	ent_clone(building_placement_cube);		// CLO
//	collision_mode = 1; 
	
//	building_unlit = 0;
//	building_light = 1;
	
	Building_Panel_Init();
	wait_for(Building_Panel_Init);
	
	//-------------------------------------------------------------------
	
	collision_mode = 0; 
	building_model_preview = ent_create( NULL, vector(square_coord_x,square_coord_y,square_coord_z), Building_ModelPreview);
	wait(1);
//	ent_clone(building_model_preview);		// CLO
	collision_mode = 1; 
	
	Building_MorphPreviewModel();
	wait_for(Building_MorphPreviewModel);
	
	//-------------------------------------------------------------------
}


void		Building_Close()
{
	building_ready = 0;
	
	//-------------------------------------------------------
//	ptr_remove(building_placement_cube);	
//	building_placement_cube = NULL;
	
	ptr_remove(building_panel);
	building_panel = NULL;
	
	ptr_remove(building_model_preview);
	building_model_preview = NULL;
	
//	ptr_remove(mtl_building);		
	//--------------------------------------------------------
	int i;
	// remove temp text
	for (i = 0; i < buildings_txt->strings; i++) 
		if ((buildings_txt->pstring)[i]) 
			ptr_remove((buildings_txt->pstring)[i]);
	ptr_remove(buildings_txt);
	buildings_txt = NULL;
	
	// remove temp string array
	for(i=0;i<max_building_items;i++)
		{
			ptr_remove(building_filenames[i]);
		}
	sys_free(building_filenames);
	building_filenames = NULL;
//	ptr_remove(building_filenames);		// causes error
}


action	Building_ModelPreview()
{
	set(my, PASSABLE | LIGHT | CAST);	// | TRANSLUCENT
	my.flags2 |= UNTOUCHABLE;
	my.red = 0;
	my.green = 255;
	my.blue = 0;
//	my.alpha = 50;
	vec_scale( my.scale_x , GetMapTileSize() / building_defaultsize * building_scale );		// selected model
	my.material = mtl_placementarea;
	my.group = 1;
	
sys_marker("omr");	
	
//	while(me)
	while(UIMode==(var)UIMODE_BUILDING)
		{
			if (building_ready)
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
					
					//-------------------------
					// snap xy					
					
					if (building_snap) 
						{
							// snap xy
							vec_set(my.x, SnapCoordToTileMidCoordXY(&my.x));
							
							// modify snapping due to object size
							if (building_tilesize_x % 2 == 0)
								{
									my.x += GetMapTileSize() / 2;
								}
							if (building_tilesize_y % 2 == 0)
								{
									my.y += GetMapTileSize() / 2;
								}
						}
					
					//-------------------------
					// snap z
					
					if (building_snapz) 
						{
							vec_set(my.x, SnapCoordToTileCoordZ(&my.x));	
						}
					
					//-------------------------
					// scale
					
					vec_scale( my.scale_x , GetMapTileSize() / building_defaultsize * building_scale / my.scale_x );	
					
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
	
sys_marker(NULL);
	
	Building_Close();
}


void Building_MorphPreviewModel()
{
	building_ready = 0;
	
	STRING* temp_str = str_create("");
	if ( !str_cmpi(building_filenames[building_selected] , "-" ) )
		{					
			str_cpy( temp_str , building_foldername );
			str_cat( temp_str , building_filenames[building_selected] );
			if (file_exists_onepath(temp_str))
				{
					ent_morph( building_model_preview , temp_str );
					wait(1);
//					ent_clone(building_model_preview);		// CLO
				}
		}
	else
		{
			ent_morph( building_model_preview , "graphics\\null.mdl" );
			wait(1);
//			ent_clone(building_model_preview);				// CLO
		}
	wait(1);		// maybe needed by wait_for				
	
	building_ready = 1;
}


void		Building_RotateModelPreview(int rotation)
{
	int direction = AngleToDirection( building_model_preview.pan );
	
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
	
	building_model_preview.pan = DirectionToAngle( direction );
}


void		Building_TurnModelPreview(int rotation)
{
	var turnrate = rotation * time_step * 16;
	
	building_model_preview.pan += turnrate;
}

/////////////////////////////////////////////////


void		Building_OpenCreate_ObsFile( STRING* building_model_filename )
{	
	if (building_ready) return;
	if (proc_status(Building_OpenCreate_ObsFile) > 0) return;
	
	STRING* temp_str = str_create("");
	//-----------------------------------------------------------------------
	// get filename from model name
	str_cpy( temp_str , building_foldername );
	str_cat( temp_str , building_model_filename );
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
			building_defaultsize = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			building_tilesize_x = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			building_tilesize_y = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			building_shadow = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			building_type = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			building_collision = str_to_num( temp_str );
			
			file_close(file_handle);	
		}
	// create file is non-existing (import model with default setttings)
	else
		{
			building_defaultsize		= (var)MAP_DEFAULTGRIDSIZE;
			building_tilesize_x		= 1;
			building_tilesize_y		= 1;
			building_type				= 0;								// (passability) 0 no access, 1 default, 2 building, 3 forest 
			building_collision		= 2;								// 0 passable, 1 polygon, 2 tile area
			building_shadow			= 2;								// 0 nothing, 1 dynamic, 2 sprite, 3 model

			file_handle = file_open_write( temp_str );
			
			file_str_write( 		file_handle , "// reference tile size // 32 by default // min 16 // max 100" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);						
			str_for_int( 			temp_str 	, building_defaultsize );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_str_write( 		file_handle , "// area width in tiles" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, building_tilesize_x );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_str_write( 		file_handle , "// area height in tiles" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, building_tilesize_y );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// shadow type // 0 no shadow // 1 dynamic shadow // 2 decal shadow // 3 model shadow" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, building_shadow );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// tile type // 0 no access // 1 terrain default // 2 building" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, building_type );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// collision type // 0 passable // 1 polygon // 2 tile area based" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, building_collision );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_close(file_handle);
		}
//	wait(1);		
	ptr_remove(temp_str);
}


///////////////////////////////////////////////////////////////////////////////////
// editor menu


void		Building_Panel_Init()
{	
	int i;
	STRING* temp_string = str_create("");
	
	building_panel = pan_create( panel_image2 , 11 );
	set( building_panel , SHOW | SHADOW | OVERLAY );		// TRANSLUCENT | 
	building_panel.pos_x 	= ui_submenupanel_x;				// screen_size.x-256;
	building_panel.pos_y 	= ui_submenupanel_y;				// 256;
	building_panel.red		= panel_font_r;
	building_panel.green		= panel_font_g;
	building_panel.blue		= panel_font_b;
	building_panel.event		= ui_submenu_panelmove;
	
	// string 1
	pan_setstring(building_panel, 0,	15,10,	panel_font2,			str_create("BUILDING MODEL PLACEMENT") );
	// string 2
	pan_setstring(building_panel, 0,	15,330,	panel_font,			str_create("Select object") );
	
	// strings 3-10 - building item numbers 1-8
	for(i=0;i<max_building_items;i++)
		{
			str_for_int( temp_string , (i+1) );
			pan_setstring( building_panel, 0,	35,350+i*15,	panel_font,			str_create( temp_string ) );
		}
	
	// strings 11-18 - building item filenames 1-8
	for(i=0;i<max_building_items;i++)
		{
			pan_setstring(building_panel, 0,	55,350+i*15,	panel_font,			str_create( building_filenames[i] ) );
		}
	
	// buttons 1-8 (checkbox)
	for(i=0;i<max_building_items;i++)
		{
			pan_setbutton(building_panel, 0,	2, 	15,350+i*15,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_Flag_Toggle, NULL, NULL); 
		}
	// button states
	for(i=0;i<max_building_items;i++)
		{
			button_state( building_panel , 1+i , building_switch_type[i] ); 		// 1 ON, 0 OFF
		}
	
	// button 9 - Unlit
	pan_setstring(building_panel, 0,	40,180,	panel_font,			str_create("set UNLIT") );	
	pan_setbutton(building_panel, 0,	2, 	15,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_ToggleUnlit,NULL,NULL); 
	button_state( building_panel , 9 , building_unlit ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(building_panel, 0,	15,30,	"COUNT: %0.f",				panel_font,	1, building_counter);
	
	// buttns 10-11 - model set      155/140/225,320
	pan_setdigits(building_panel, 0,	180,330,	"Set:    %0.f",		panel_font,	1, building_set);	
	pan_setbutton(building_panel, 0,	1, 	165,330,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Building_DecSet, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	225,330,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			Building_IncSet, 		NULL, NULL); 
		
	// buttons 12-13 - draw area - removed
//	pan_setdigits(building_panel, 0,	145,240,	"Area:       %0.f",		panel_font,	1, building_tilesize);	
//	pan_setbutton(building_panel, 0,	1, 	175,241,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Building_DecSize, 		NULL, NULL); 
//	pan_setbutton(building_panel, 0,	1, 	210,241,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			Building_IncSize, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	pan_setbutton(building_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// density - removed
//	pan_setdigits(building_panel, 0,	15,200,	"Density: %0.f",		panel_font,	0.85, 							building_density );
//	pan_setslider(building_panel, 0,	90+12,202,	horiz_slider_bmp,slider_knob_bmp,	1,max_building_density+0.4,	building_density); 
	
	// scale
	pan_setdigits(building_panel, 0,	15,220,	"Scale: %0.2f",		panel_font,	1, 				building_scale);
	pan_setslider(building_panel, 0,	90+12,222,	horiz_slider_bmp,slider_knob_bmp,	0.75,1.25,		building_scale);
	
	// button 14 - Light
	pan_setstring(building_panel, 0,	40,100,	panel_font,			str_create("set LIGHT to adjust RGB values:") );	
	pan_setbutton(building_panel, 0,	2, 	15,100,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_ToggleLight,NULL,NULL); 
	button_state( building_panel , 14 , building_light ); 		// 1 ON, 0 OFF	
	
	// rgb
	pan_setdigits(building_panel, 0,	15,120,	"R: %0.f",				panel_font,	1, 			building_red);
	pan_setdigits(building_panel, 0,	15,140,	"G: %0.f",				panel_font,	1, 			building_green);
	pan_setdigits(building_panel, 0,	15,160,	"B: %0.f",				panel_font,	1, 			building_blue);
	pan_setslider(building_panel, 0,	90+12,122,	horiz_slider_bmp,slider_knob_bmp,	0,255,		building_red); 
	pan_setslider(building_panel, 0,	90+12,142,	horiz_slider_bmp,slider_knob_bmp,	0,255,		building_green); 
	pan_setslider(building_panel, 0,	90+12,162,	horiz_slider_bmp,slider_knob_bmp,	0,255,		building_blue); 
	
	// ambient
	pan_setdigits(building_panel, 0,	15,60,	"Ambient: %0.f",		panel_font,	1, 			building_ambient);
	pan_setslider(building_panel, 0,	90+12,62,	horiz_slider_bmp,slider_knob_bmp,	0,100,		building_ambient); 
	
	// alpha
	pan_setdigits(building_panel, 0,	15,80,	"Alpha: %0.f",			panel_font,	1, 			building_alpha);
	pan_setslider(building_panel, 0,	90+12,82,	horiz_slider_bmp,slider_knob_bmp,	0,100,		building_alpha); 
	
	// button 15 - random fine scale - removed
//	pan_setstring(building_panel, 0,	40+100,280,	panel_font,			str_create("rnd fine scale") );	
//	pan_setbutton(building_panel, 0,	2, 	15+100,280,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_ToggleRandomize,NULL,NULL); 
//	button_state( building_panel , 15 , building_randomize ); 		// 1 ON, 0 OFF	
	pan_setbutton(building_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 16 - Overlay
	pan_setstring(building_panel, 0,	40,300,	panel_font,			str_create("set OVERLAY") );
	pan_setbutton(building_panel, 0,	2, 	15,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_ToggleOverlay,NULL,NULL); 
	button_state( building_panel , 16 , building_overlay ); 		// 1 ON, 0 OFF	
	
	// button 17 - Clear All
	pan_setbutton(building_panel, 0,	1, 	45,480,		m_clearall_on_bmp,m_clearall_off_bmp,m_clearall_over_bmp,m_clearall_over_bmp,		Building_DeleteAll,NULL,NULL); 
	
	// material buttons-----------------------
	
	// button 18 - Material - removed
//	pan_setbutton(building_panel, 0,	1, 	200,10,		ss_material_on_bmp,ss_material_off_bmp,ss_material_over_bmp,ss_material_over_bmp,		Building_MaterialSetup,NULL,NULL); 			
	pan_setbutton(building_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 19 - mat_model
	pan_setstring(building_panel, 0,	40+100,180,	panel_font,			str_create("set model material") );	
	pan_setbutton(building_panel, 0,	2, 	15+100,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_ToggleModel,NULL,NULL); 
	button_state( building_panel , 19 , building_model ); 		// 1 ON, 0 OFF	
	
	//----------------------------------------
	
	// button 20 - random rotation - removed
//	pan_setstring(building_panel, 0,	40,280,	panel_font,			str_create("rnd rotation") );	
//	pan_setbutton(building_panel, 0,	2, 	15,280,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_ToggleRndRot,NULL,NULL); 
//	button_state( building_panel , 20 , building_rnd_rot ); 		// 1 ON, 0 OFF	
	pan_setbutton(building_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 21-22-23 - auto/set/no tree - removed
//	pan_setstring(building_panel, 0,	35,262,	panel_font,			str_create("auto tree") );
//	pan_setbutton(building_panel, 0,	4, 	15,262,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		Building_ToggleTree,NULL,NULL); 
//	if (building_istree==(var)0) button_state( building_panel , 21 , ON ); 		// 1 ON, 0 OFF	
//	
//	pan_setstring(building_panel, 0,	115,262,	panel_font,			str_create("set tree") );
//	pan_setbutton(building_panel, 0,	4, 	95,262,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		Building_ToggleTree,NULL,NULL); 
//	if (building_istree==(var)1) button_state( building_panel , 22 , ON ); 		// 1 ON, 0 OFF	
//	
//	pan_setstring(building_panel, 0,	195,262,	panel_font,			str_create("not tree") );
//	pan_setbutton(building_panel, 0,	4, 	175,262,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		Building_ToggleTree,NULL,NULL); 
//	if (building_istree==(var)2) button_state( building_panel , 23 , ON ); 		// 1 ON, 0 OFF	
	pan_setbutton(building_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	pan_setbutton(building_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	pan_setbutton(building_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 24 - set to Lightmap
//	if (shadow_stencil==(var)8)
		{
			pan_setstring(building_panel, 0,	40+100,300,	panel_font,			str_create("Lightmap") );
		}
//	else
//		{
//			pan_setstring(building_panel, 0,	40+100,300,	panel_font,			str_create("set to Lightmap") );
//		}
	pan_setbutton(building_panel, 0,	2, 	15+100,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_ToggleLightmap,NULL,NULL); 
	button_state( building_panel , 24 , building_lightmap ); 		// 1 ON, 0 OFF	
	
	// button 25 - Modify - removed
//	pan_setbutton(building_panel, 0,	1, 	177,480,		m_modify_on_bmp,m_modify_off_bmp,m_modify_over_bmp,m_modify_over_bmp,					ModifyObject_Init,		NULL,NULL); 
	pan_setbutton(building_panel, 0,	1, 	255,580,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 26 - Snap xy
	pan_setstring(building_panel, 0,	15+20+20,240,	panel_font,			str_create("Snap XY / Z") );
	pan_setbutton(building_panel, 0,	2, 	15,240,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_ToggleSnap,NULL,NULL); 
	button_state( building_panel , 26 , building_snap ); 		// 1 ON, 0 OFF	
	
	// button 27,28 - removed	
//	pan_setbutton(building_panel, 0,	1, 	90,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Building_Panel_Button, 		NULL, NULL); 
//	pan_setbutton(building_panel, 0,	1, 	210,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Building_Panel_Button, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	255,580,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,								NULL,								NULL,NULL); 
	pan_setbutton(building_panel, 0,	1, 	255,580,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,								NULL,								NULL,NULL); 
	// button 29,30	
	pan_setbutton(building_panel, 0,	1, 	90,220,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Building_Panel_Button, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	210,220,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Building_Panel_Button, 		NULL, NULL); 
	// button 31,32	33,34		35,36
	pan_setbutton(building_panel, 0,	1, 	90,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Building_Panel_Button, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	210,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Building_Panel_Button, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	90,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Building_Panel_Button, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	210,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Building_Panel_Button, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	90,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Building_Panel_Button, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	210,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Building_Panel_Button, 		NULL, NULL); 
	// button 37,38	39,40
	pan_setbutton(building_panel, 0,	1, 	90,60,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Building_Panel_Button, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	210,60,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Building_Panel_Button, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	90,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Building_Panel_Button, 		NULL, NULL); 
	pan_setbutton(building_panel, 0,	1, 	210,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Building_Panel_Button, 		NULL, NULL); 
	
	// button 41 - Edit                    200,35
	pan_setbutton(building_panel, 0,	1, 	100,330,		ss_edit_on_bmp,ss_edit_off_bmp,ss_edit_over_bmp,ss_edit_over_bmp,		Building_EditFile,		NULL,NULL); 			
		
	// button 42 - SnapZ
	pan_setstring(building_panel, 0,	75+20,240,	panel_font,			str_create("") );
	pan_setbutton(building_panel, 0,	2, 	15+20,240,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_ToggleSnapZ,NULL,NULL); 
	button_state( building_panel , 42 , building_snapz ); 		// 1 ON, 0 OFF	
	
	// button 43 - force decal shadow (ent_decal) - useful when the level is used in both shadow_stencil=1 and shadow_stencil=8 modes
//	if (shadow_stencil==(var)8)
		{
			pan_setstring(building_panel, 0,	40+175,300,	panel_font,			str_create("Decal") );
			pan_setbutton(building_panel, 0,	2, 	15+175,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Building_ToggleDecal,NULL,NULL); 
			button_state( building_panel , 43 , building_decal ); 		// 1 ON, 0 OFF	
		}
	
	// button 44 45 - Lightmap All : Set / Reset
	pan_setstring(building_panel, 0,			130,30,		panel_font,			str_create("Ligthmap All") );
	pan_setbutton(building_panel, 0,	1, 	200,20,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,					Building_LightmapAll,		NULL,NULL); 
	pan_setbutton(building_panel, 0,	1, 	200,40,		ss_reset_on_bmp,ss_reset_off_bmp,ss_reset_over_bmp,ss_reset_over_bmp,		Building_LightmapAll,		NULL,NULL); 
		
	//--------------------------------------------------------------
	
	ptr_remove( temp_string );	
	
	// it can't have a loop because of a wait_for !
}


void		Building_Panel_Toggle()
{
	toggle( building_panel , SHOW );	
}


////////////////////////////////////////////////////


void		Building_Panel_Button(button_number)
{
	// building_density
	if (button_number==(var)27)
		{
//			building_density = maxv( 1 , --building_density );
		}
	else if (button_number==(var)28)
		{
//			building_density = minv( max_building_density , ++building_density );
		}
	// building_scale
	else if (button_number==(var)29)
		{
			building_scale -= 0.01;
			building_scale = maxv( 0.75 , building_scale );
		}
	else if (button_number==(var)30)
		{
			building_scale += 0.01;
			building_scale = minv( 1.25 , building_scale );
		}
	// building_red
	else if (button_number==(var)31)
		{
			building_red = maxv( 0 , --building_red );
		}
	else if (button_number==(var)32)
		{
			building_red = minv( 255 , ++building_red );
		}
	// building_green
	else if (button_number==(var)33)
		{
			building_green = maxv( 0 , --building_green );
		}
	else if (button_number==(var)34)
		{
			building_green = minv( 255 , ++building_green );
		}
	// building_blue
	else if (button_number==(var)35)
		{
			building_blue = maxv( 0 , --building_blue );
		}
	else if (button_number==(var)36)
		{
			building_blue = minv( 255 , ++building_blue );
		}
	// building_ambient
	else if (button_number==(var)37)
		{
			building_ambient = maxv( 0 , --building_ambient );
		}
	else if (button_number==(var)38)
		{
			building_ambient = minv( 100 , ++building_ambient );
		}
	// building_alpha
	else if (button_number==(var)39)
		{
			building_alpha = maxv( 0 , --building_alpha );
		}
	else if (button_number==(var)40)
		{
			building_alpha = minv( 100 , ++building_alpha );
		}	
}


/////////////////////////////////////////////////////////////////


//// change by hotkey
//void		Building_NextSet()
//{
//	if (proc_status(Building_NextSet) > 0) return;
//	if (proc_status(Building_UpdateSetNames) > 0) return;
//	if (!building_ready) return;
//	building_ready = 0;
//	wait(3);
//	
//	// jump to next existing set
//	building_set++;
//	if (building_set>max_building_sets)
//		{
//			building_set = 1;
//		}
//	Building_UpdateSetNames();
//	wait_for(Building_UpdateSetNames);
//	
//	wait(1);
//}


void		Building_IncSet()
{
	if (proc_status(Building_IncSet) > 0) return;
	if (proc_status(Building_UpdateSetNames) > 0) return;
	if (!building_ready) return;
	building_ready = 0;
	wait(3);
	
	// jump to next existing set
	building_set++;
	if (building_set>max_building_sets)
		{
//			building_set = max_building_sets;
//			return;
			building_set = 1;
		}
	Building_UpdateSetNames();
	wait_for(Building_UpdateSetNames);
	
	wait(1);
}


void		Building_DecSet()
{
	if (proc_status(Building_DecSet) > 0) return;
	if (proc_status(Building_UpdateSetNames) > 0) return;
	if (!building_ready) return;
	building_ready = 0;
	wait(3);
	
	// jump to next existing set
	building_set--;
	if (building_set<1)
		{
//			building_set = 1;
//			return;
			building_set = max_building_sets;
		}
	Building_UpdateSetNames();
	wait_for(Building_UpdateSetNames);
	
	wait(1);
}


void		Building_UpdateSetNames()
{
	if (proc_status(Building_UpdateSetNames) > 0) return;
	
	int i;
	// copy the proper 8 filenames to string array
	int j = building_set - 1;
	int k = j * max_building_items;
	for(i=0;i<max_building_items;i++)
		{
//			str_cpy( building_filenames[i] , building_foldername );						// subfolder
//			str_cat( building_filenames[i] , (buildings_txt->pstring)[k+i] );			// filename with extension
			str_cpy( building_filenames[i] , "" );											// (or building_foldername) subfolder
			str_cat( building_filenames[i] , (buildings_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi( building_filenames[i] , "" ) )								// (or building_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( building_filenames[i] , "-" );
				}
		}
	// refresh 8 filename strings to display in menu
	for(i=0;i<max_building_items;i++)
		{
			pan_setstring(building_panel, 11+i,	55,350+i*15,	panel_font,			str_create( building_filenames[i] ) );
		}	
	wait(1);
	
	// read values from .obs file
	Building_OpenCreate_ObsFile( building_filenames[building_selected] );
	wait_for(Building_OpenCreate_ObsFile);
	wait(1);
	
//	Building_MorphCube();
	Building_MorphPreviewModel();
	wait_for(Building_MorphPreviewModel);
	wait(1);
}


///////////////////////////////////


// change by hotkey
void		Building_Flag_Switch(var setplusone)
{
	if (proc_status(Building_Flag_Switch) > 0) return;
	if (!building_ready) return;
	building_ready = 0;
	wait(3);
	
	int temp = setplusone -1 -1;	// -1 because of scancode, and -1 because of array pos are 0..7
	
//	building_switch_type[temp] = 1 - building_switch_type[temp];
//	// change checkbox state too
//	var temp_state = button_state( building_panel , temp+1 , -1 ); 				// -1 to read value
//	button_state( building_panel , temp+1 , 1-temp_state ); 						// 1 ON, 0 OFF
	
	int i;
	for(i=0;i<max_building_items;i++)
		{
			building_switch_type[i] = 0;						// 0..7
			button_state( building_panel , i+1 , 0 );		// 0..8
		}
	building_switch_type[temp] = 1;
	button_state( building_panel , temp+1 , 1 ); 
	building_selected = temp;
	
	// read values from .obs file
	Building_OpenCreate_ObsFile( building_filenames[building_selected] );
	wait_for(Building_OpenCreate_ObsFile);
	wait(1);
	
//	Building_MorphCube();
	Building_MorphPreviewModel();
	wait_for(Building_MorphPreviewModel);
	
	wait(1);
}


// change by checkbox (buttons 1-8)
void		Building_Flag_Toggle(button_number)
{
	if (proc_status(Building_Flag_Toggle) > 0) return;
	if (!building_ready) return;
	building_ready = 0;
	wait(3);
	
//	building_switch_type[button_number-1] = 1 - building_switch_type[button_number-1] ;
	
	int i;
	for(i=0;i<max_building_items;i++)
		{
			building_switch_type[i] = 0;							// 0..7
			button_state( building_panel , i+1 , 0 );			// 1..8
		}
	building_switch_type[button_number-1] = 1;
	button_state( building_panel , button_number , 1 ); 
	building_selected = button_number-1;
	
	// read values from .obs file
	Building_OpenCreate_ObsFile( building_filenames[building_selected] );
	wait_for(Building_OpenCreate_ObsFile);
	wait(1);
	
//	Building_MorphCube();
	Building_MorphPreviewModel();
	wait_for(Building_MorphPreviewModel);
	
	wait(1);
}


/////////////////////////////////
// decrease/increase building densityif left mouse button is used for placement


//void 		Building_DecDens()
//{
//	building_density--;
//	if (building_density<1)
//		{
//			building_density = 1;
//		}
//}
//
//
//void 		Building_IncDens()
//{
//	building_density++;
//	if (building_density>max_building_density)
//		{
//			building_density = max_building_density;
//		}
//}


////////////////////////////////////


//// change by hotkey
//void 		Building_DecSize()
//{
//	building_tilesize -= 2;
//	if (building_tilesize<1)
//		{
//			building_tilesize = 1;
//		}	
//	// resize cube
//	var temp = building_tilesize / building_placement_cube.scale_x ;	
//	vec_scale( building_placement_cube.scale_x , temp );
//}
//
//
//// change by hotkey
//void 		Building_IncSize()
//{
//	building_tilesize += 2;
//	if (building_tilesize>max_building_size)
//		{
//			building_tilesize = max_building_size;
//		}
//	// resize cube
//	var temp = building_tilesize / building_placement_cube.scale_x ;	
//	vec_scale( building_placement_cube.scale_x , temp );
//}


/////////////////////////////////////////////////////
// change by checkbox


void		Building_ToggleUnlit()
{
	building_unlit = 1 - building_unlit;	
	if (building_unlit)
		{
			// switch off LIGHT
			building_light = 0;
			button_state( building_panel , 14 , building_light );
			// switch off model
			building_model = 0;
			button_state( building_panel , 19 , building_model );
		}
}


void		Building_ToggleLight()
{
	building_light = 1 - building_light;	
	if (building_light)
		{
			// switch off UNLIT
			building_unlit = 0;
			button_state( building_panel , 9 , building_unlit );
		}
}


void		Building_ToggleModel()
{
	building_model = 1 - building_model;
	if (building_model)
		{
			// switch off UNLIT
			building_unlit = 0;
			button_state( building_panel , 9 , building_unlit );
		}
}


//void		Building_ToggleRandomize()
//{
//	building_randomize = 1 - building_randomize;	
//}


void		Building_ToggleOverlay()
{
	building_overlay = 1 - building_overlay;	
}


//void		Building_ToggleRndRot()
//{
//	building_rnd_rot = 1 - building_rnd_rot;	
//	
//	// reset rotation
//	if (building_rnd_rot)
//		{
//			building_model_preview.pan = 0;
//		}
//}


//void		Building_ToggleTree(button_number)
//{
//	if (button_number==21)
//		{
//			building_istree = 0;
//		}
//	else if (button_number==22)
//		{
//			building_istree = 1;
//		}
//	else if (button_number==23)
//		{
//			building_istree = 2;
//		}
//}


void		Building_ToggleLightmap()
{
	building_lightmap = 1 - building_lightmap;
	if (building_lightmap)
		{
			building_decal = 0;
			button_state( building_panel , 44 , building_decal );
		}
}


void		Building_ToggleSnap()
{
	building_snap = 1 - building_snap;
}


void		Building_ToggleSnapZ()
{
	building_snapz = 1 - building_snapz;
}


void		Building_ToggleDecal()
{
	building_decal = 1 - building_decal;
	if (building_decal)
		{
			building_lightmap = 0;
			button_state( building_panel , 24 , building_lightmap );
		}
}


void		Building_LightmapAll(button_number)
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
			reset( map_loadpanel , SHOW);
			return;
		}
	
	int counter = 0;
	ENTITY* temp_ent = ent_next(NULL); 																	// get first entity
	while(temp_ent)
		{
			if (temp_ent.ENT_CATEGORY == (var)UIMODE_BUILDING)	
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


void		Building_MaterialSetup()
{
	wait(3);
	material_select = 15;
//	Material_MatPanel_Init( mtl_building , mtl_building_transp );	
	Material_MatPanel_Init( mtl_building , NULL );								// transparent material not used any more
//	MaterialSetup_Init();		// old settings panel		
}


void		Building_EditFile()
{
	wait(3);
	STRING* temp_str = str_create("");
	if ( !str_cmpi(building_filenames[building_selected] , "-" ) )
		{					
			str_cpy( temp_str , building_foldername );
			str_cat( temp_str , building_filenames[building_selected] );
			ObswModify_Init(building_defaultsize, building_tilesize_x, building_tilesize_y, building_type, building_shadow, building_collision, temp_str);
		}
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor placement


void	 Building_CreateObject()
{
	if (!building_ready) return;
		
	STRING* temp_str = str_create("");
	if ( !str_cmpi(building_filenames[building_selected] , "-" ) )
		{					
			str_cpy( temp_str , building_foldername );
			str_cat( temp_str , building_filenames[building_selected] );
			
			//----------------------------------------------------------------------------
			// create object
			
//			var my_scale = GetMapTileSize()/building_defaultsize * building_scale;
			var my_scale = building_model_preview.scale_x;	
			
			Building_Create(	temp_str, 
												
									building_defaultsize,
									building_tilesize_x, building_tilesize_y,
									building_type, building_collision, building_shadow,	
																	
									my_scale, my_scale, my_scale,		
									building_model_preview.x, building_model_preview.y, building_model_preview.z, 									
									building_model_preview.pan, building_model_preview.tilt, building_model_preview.roll,
									
									building_alpha, building_ambient, 
									building_light, building_red, building_green, building_blue, 
									building_unlit, building_overlay, building_model,										
									(1-building_lightmap), building_decal
								);
//			wait_for(Building_Create);
//			
			//----------------------------------------------------------------------------			
//			
//			// snapped coordinates - preview model is already snapped if set to snap xy/z
//			ent_create( temp_str, building_model_preview.x, Building_CreateItem );
//			
//			// to get its filename later: str_for_entfile( str_entfilename , my );	
//			
			//----------------------------------------------------------------------------	
		}
	
//	wait(1);
	ptr_remove(temp_str);
}


///////////////////////////////////////////////


void		Building_CloneObject()
{
	//-----------------------------------------------------------------------
	// select
	
	mod_origmenu = (var)UIMODE_BUILDING;
	
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
	str_cpy( temp_str , building_foldername );
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
	
	Building_ReadObsFileData(str_for_entfile(NULL, you), &obs_defaultsize, &obs_tilesize_x, &obs_tilesize_y, &obs_type, &obs_collision, &obs_shadow);
	
	//-----------------------------------------------------------------------
	// collision
	
//	if (my.shadow != NULL)
	if (((shadow_stencil>(var)-1) && (shadow_stencil<(var)8)) || (is(my, FLAG5)))			// collider entity is needed when decal shadows are applied		// !!!	
		{
			Building_CreateCollider(my);
		}
	
	//-----------------------------------------------------------------------
	// shadow - required by decal and ent_decal shadows
			
	Building_SetShadow(my, (1-is(my, FLAG6)), is(my, FLAG5), obs_shadow);
	wait_for(Building_SetShadow);
	
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


////////////////////////////////////////////////////////////////////////////////////
// editor deletion


void		Building_DeleteItem()
{
	MousePos3D0();
	
	if (HIT_TARGET)
		{
//			you = hit.entity;
			
			Building_RemoveObject(hit.entity);
		}
}


void		Building_DeleteAll()
{
	wait(3);

	Building_RemoveAll();
	wait_for(Building_RemoveAll);
}


////////////////////////////////////////////////////////////////////////////////////


#endif