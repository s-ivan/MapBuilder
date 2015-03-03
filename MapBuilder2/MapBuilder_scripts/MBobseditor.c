
#ifndef MBobseditor_c
#define MBobseditor_c

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


////////////////////////////////////////////////////////////////////////////////////
// editor menu


void 		Obstacle_Init()
{
	obstacle_ready = 0;		
	//----------------------------------------------------------------
	// obstacle model filename initialization 
sys_marker("oi1");	
	int arraysize = (int)OBSTACLE_MAXPERSET;
	obstacle_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );
	int i;	
	// init filename string array elements
	for(i=0;i<max_obstacle_items;i++)
		{
			obstacle_filenames[i] = str_create("");
		}	
sys_marker(NULL);		
	//-----------------------------------------------------------------
	// determine how many .mdl files are in the folder, 
	// divide it by 8, and the result is the number of sets
//	obstacle_set = 1;
	max_obstacle_sets = OBSTACLE_MAXSET;
	obstacles_txt = txt_create( (max_obstacle_sets*max_obstacle_items) , 0 );		// 10*8=80
	for(i=0;i<max_obstacle_sets*max_obstacle_items;i++)
		{
			str_cpy( (obstacles_txt->pstring)[i] , "" );
		}
	//-------------------------------------------------------------------
	// read in folder content
	i = 0;
	STRING* temp_str = str_create("");
	str_cpy( temp_str , obstacle_foldername );
	str_cat( temp_str , "*.mdl" );
	txt_for_dir( obstacles_txt, temp_str ); 										//	better not to use wmb or hmp
	while (str_len((obstacles_txt->pstring)[i]) > 0)
		{
			i++;
		}
	max_obstacle_sets = integer(i/max_obstacle_items);
	if ( fraction(i/max_obstacle_items) > 0)
		{
			max_obstacle_sets++;		// allow to display not complete sets too
		}
	obstacle_set = clamp(obstacle_set, 1, max_obstacle_sets);
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	for(i=0;i<max_obstacle_items;i++)
		{
//			str_cpy( obstacle_filenames[i] , obstacle_foldername );							// subfolder
//			str_cat( obstacle_filenames[i] , (obstacles_txt->pstring)[i] );				// filename with extension			
			str_cpy( obstacle_filenames[i] , "" );													// (or obstacle_foldername) subfolder
			int j = (obstacle_set - 1) * max_obstacle_items;
			str_cat( obstacle_filenames[i] , (obstacles_txt->pstring)[j+i] );				// filename with extension
			if ( str_cmpi( obstacle_filenames[i] , "" ) )										// (or obstacle_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( obstacle_filenames[i] , "-" );
				}
		}	
	//------------------------------------------------------------------
	// initial value settings	
	for(i=0;i<max_obstacle_items;i++)
		{
			obstacle_switch_type[i] = 0;
		}
	obstacle_switch_type[obstacle_selected] = 1;
//	obstacle_tilesize = 1;
//	obstacle_density = 3;	
	//----------------------------------------------------------------
	// read values from .obs file
	Obstacle_OpenCreate_ObsFile( obstacle_filenames[obstacle_selected] );
	wait_for(Obstacle_OpenCreate_ObsFile);
	//-------------------------------------------------------------------
	// create objects
	collision_mode = 0; 
	obstacle_placement_cube = ent_create( obstacle_creator_model , vector(square_coord_x,square_coord_y,square_coord_z) , Obstacle_Cube );
	wait(1);
//	ent_clone(obstacle_placement_cube);		// CLO ?
	collision_mode = 1; 
	
//	obstacle_unlit = 0;
//	obstacle_light = 1;
	
	Obstacle_Panel_Init();
	wait_for(Obstacle_Panel_Init);
	
	//-------------------------------------------------------------------
	collision_mode = 0; 
	obstacle_model_preview = ent_create( NULL, vector(square_coord_x,square_coord_y,square_coord_z), Obstacle_ModelPreview);
	wait(1);
//	ent_clone(obstacle_model_preview);		// CLO
	collision_mode = 1; 
	
	Obstacle_MorphPreviewModel();
	wait_for(Obstacle_MorphPreviewModel);
	//-------------------------------------------------------------------
}


void		Obstacle_Close()
{
	obstacle_ready = 0;
	
	//-------------------------------------------------------
	ptr_remove(obstacle_placement_cube);	
	obstacle_placement_cube = NULL;
	
	ptr_remove(obstacle_panel);
	obstacle_panel = NULL;
	
	ptr_remove(obstacle_model_preview);
	obstacle_model_preview = NULL;
	
//	ptr_remove(mtl_obstacle);		
	//--------------------------------------------------------
	int i;
	// remove temp text
	for (i = 0; i < obstacles_txt->strings; i++) 
		if ((obstacles_txt->pstring)[i]) 
			ptr_remove((obstacles_txt->pstring)[i]);
	ptr_remove(obstacles_txt);
	obstacles_txt = NULL;
	
	// remove temp string array
	for(i=0;i<max_obstacle_items;i++)
		{
			ptr_remove(obstacle_filenames[i]);
		}
	sys_free(obstacle_filenames);
	obstacle_filenames = NULL;
//	ptr_remove(obstacle_filenames);		// causes error
}


action 	Obstacle_Cube()
{
	set(my, PASSABLE | TRANSLUCENT | LIGHT | CAST);
	my.flags2 |= UNTOUCHABLE;
	my.red = 0;
	my.green = 255;
	my.blue = 0;
	my.alpha = 50;
	vec_scale( my.scale_x , GetMapTileSize()/ MAP_DEFAULTGRIDSIZE * obstacle_tilesize );	// cube
//	vec_scale( my.scale_x , GetMapTileSize() / obstacle_defaultsize * obstacle_scale );		// selected model
	//	my.skill1 = 1;	// actual scale size
	my.material = mtl_placementarea;
	
	VECTOR vTemp;
	COLOR cTemp;
	vec_set(cTemp,vector(255,0,0));
	
	BMAP* area_bmp = bmap_createblack(32,32,24);
	wait(1);
	bmap_fill(area_bmp, vector(200,200,200), 100);
	wait(1);
	
	while(UIMode==(var)UIMODE_OBSTACLE)
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
			
//			//-------------------------
//			// cube colour
//			
//			if (square_coord_z<0)
//				vec_set(my.blue,vector(255,0,0));		// blue if under water level - inside game area
//			else
//				vec_set(my.blue,vector(0,255,0));		// green if over water level - inside game area
//			
//			if (square_outofborder)
//				vec_set(my.blue,vector(0,0,255));		// red if out of game area
//			
//			//-------------------------
//			// draw a decal onto terrain surface representing draw area
//
//			c_trace(vector(my.x,my.y,my.z+GetMapTileSize()*10),vector(my.x,my.y,my.z-GetMapTileSize()*40), IGNORE_ME | IGNORE_CONTENT | IGNORE_PASSABLE | IGNORE_FLAG2 | IGNORE_SPRITES | SCAN_TEXTURE );	// | SCAN_TEXTURE needed !!!
//			if (HIT_TARGET)
//				{
//					PARTICLE* area_p = ent_decal(you, area_bmp, (GetMapTileSize()*obstacle_tilesize), my.pan-your.pan);		
//					area_p.material = mtl_placementarea; 		// assign another material when contact material is not suitable for decals like terraintex.fx
//					set(area_p,TRANSLUCENT|FLAG1);
//					area_p.alpha = 50;		
//					vec_set(mtl_placementarea.ambient_blue, my.blue);		// same color
//					area_p.lifespan = 0.01;											// one frame										
//				}
//			
			//-------------------------
			
			wait(1);
			
			//-------------------------
		}
//	vec_fill(mtl_placementarea.ambient_blue, 200);		// reset material ambient color
	
	Obstacle_Close();
}


action	Obstacle_ModelPreview()
{
	set(my, PASSABLE | LIGHT | CAST);	// | TRANSLUCENT
	my.flags2 |= UNTOUCHABLE;
	my.red = 0;
	my.green = 255;
	my.blue = 0;
//	my.alpha = 50;
	vec_scale( my.scale_x , GetMapTileSize() / obstacle_defaultsize * obstacle_scale );		// selected model
	my.material = mtl_placementarea;
	
sys_marker("omr");	
	while(me)
		{
			if (obstacle_ready)
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
					
					if (obstacle_snap) 
						{
							// snap xy
							vec_set(my.x, SnapCoordToTileMidCoordXY(&my.x));
							
							// modify snapping due to object size
							if (obstacle_tilesize_x % 2 == 0)
								{
									my.x += GetMapTileSize() / 2;
								}
							if (obstacle_tilesize_y % 2 == 0)
								{
									my.y += GetMapTileSize() / 2;
								}
						}
					
					//-------------------------
					// snap z
					
					if (obstacle_snapz) 
						{
							vec_set(my.x, SnapCoordToTileCoordZ(&my.x));	
						}
					
					//-------------------------										
					// scale
					
					vec_scale( my.scale_x , GetMapTileSize() / obstacle_defaultsize * obstacle_scale / my.scale_x );
					
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
}


void Obstacle_MorphPreviewModel()
{
	obstacle_ready = 0;
	
	STRING* temp_str = str_create("");
	if ( !str_cmpi(obstacle_filenames[obstacle_selected] , "-" ) )
		{					
			str_cpy( temp_str , obstacle_foldername );
			str_cat( temp_str , obstacle_filenames[obstacle_selected] );
			if (file_exists_onepath(temp_str))
				{
					ent_morph( obstacle_model_preview , temp_str );
					wait(1);
//					ent_clone(obstacle_model_preview);		// CLO
				}
		}
	else
		{
			ent_morph( obstacle_model_preview , "graphics\\null.mdl" );
			wait(1);
//			ent_clone(obstacle_model_preview);				// CLO
		}
	wait(1);		// maybe needed by wait_for				
	
	obstacle_ready = 1;
}


void		Obstacle_RotateModelPreview(int rotation)
{
	int direction = AngleToDirection( obstacle_model_preview.pan );
	
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
	
	obstacle_model_preview.pan = DirectionToAngle( direction );
}


void		Obstacle_TurnModelPreview(int rotation)
{
	var turnrate = rotation * time_step * 16;
	
	obstacle_model_preview.pan += turnrate;
}


/////////////////////////////////////////////////


void		Obstacle_OpenCreate_ObsFile( STRING* obstacle_model_filename )
{	
	if (obstacle_ready) return;
	if (proc_status(Obstacle_OpenCreate_ObsFile) > 0) return;
	
	STRING* temp_str = str_create("");
	//-----------------------------------------------------------------------
	// get filename from model name
	str_cpy( temp_str , obstacle_foldername );
	str_cat( temp_str , obstacle_model_filename );
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
			obstacle_defaultsize = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			obstacle_tilesize_x = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			obstacle_tilesize_y = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			obstacle_shadow = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			obstacle_type = str_to_num( temp_str );
			
				file_str_read( file_handle , temp_str );		// comment line
			file_str_read( file_handle , temp_str );
			obstacle_collision = str_to_num( temp_str );
			
			file_close(file_handle);	
		}
	// create file is non-existing (import model with default setttings)
	else
		{
			obstacle_defaultsize		= (var)MAP_DEFAULTGRIDSIZE;
			obstacle_tilesize_x		= 1;
			obstacle_tilesize_y		= 1;
			obstacle_type				= 0;								// (passability) 0 no access, 1 default, 2 building, 3 forest 
			obstacle_collision		= 2;								// 0 passable, 1 polygon, 2 tile area
			obstacle_shadow			= 2;								// 0 nothing, 1 dynamic, 2 sprite, 3 model

			file_handle = file_open_write( temp_str );
			
			file_str_write( 		file_handle , "// reference tile size // 32 by default // min 16 // max 100" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);						
			str_for_int( 			temp_str 	, obstacle_defaultsize );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_str_write( 		file_handle , "// area width in tiles" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, obstacle_tilesize_x );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_str_write( 		file_handle , "// area height in tiles" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, obstacle_tilesize_y );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// shadow type // 0 no shadow // 1 dynamic shadow // 2 decal shadow // 3 model shadow" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, obstacle_shadow );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// tile type // 0 no access // 1 terrain default // 2 building // 3 forest" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, obstacle_type );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// collision type // 0 passable // 1 polygon // 2 tile area based" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);							
			str_for_int( 			temp_str 	, obstacle_collision );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_close(file_handle);
		}
//	wait(1);		// maybe needed for wait for
	ptr_remove(temp_str);
}


///////////////////////////////////////////////////////////////////////////////////
// editor menu


void		Obstacle_Panel_Init()
{	
	int i;
	STRING* temp_string = str_create("");
	
	obstacle_panel = pan_create( panel_image2 , 11 );
	set( obstacle_panel , SHOW | SHADOW | OVERLAY );		// TRANSLUCENT | 
	obstacle_panel.pos_x 	= ui_submenupanel_x;				// screen_size.x-256;
	obstacle_panel.pos_y 	= ui_submenupanel_y;				// 256;
	obstacle_panel.red		= panel_font_r;
	obstacle_panel.green		= panel_font_g;
	obstacle_panel.blue		= panel_font_b;
	obstacle_panel.event		= ui_submenu_panelmove;
	
	// string 1
	pan_setstring(obstacle_panel, 0,	15,10,	panel_font2,			str_create("OBSTACLE MODEL PLACEMENT") );
	// string 2
	pan_setstring(obstacle_panel, 0,	15,330,	panel_font,			str_create("Select object") );
	
	// strings 3-10 - obstacle item numbers 1-8
	for(i=0;i<max_obstacle_items;i++)
		{
			str_for_int( temp_string , (i+1) );
			pan_setstring( obstacle_panel, 0,	35,350+i*15,	panel_font,			str_create( temp_string ) );
		}
	
	// strings 11-18 - obstacle item filenames 1-8
	for(i=0;i<max_obstacle_items;i++)
		{
			pan_setstring(obstacle_panel, 0,	55,350+i*15,	panel_font,			str_create( obstacle_filenames[i] ) );
		}
	
	// buttons 1-8 (checkbox)
	for(i=0;i<max_obstacle_items;i++)
		{
			pan_setbutton(obstacle_panel, 0,	2, 	15,350+i*15,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_Flag_Toggle, NULL, NULL); 
		}
	// button states
	for(i=0;i<max_obstacle_items;i++)
		{
			button_state( obstacle_panel , 1+i , obstacle_switch_type[i] ); 		// 1 ON, 0 OFF
		}
	
	// button 9 - Unlit
	pan_setstring(obstacle_panel, 0,	40,180,	panel_font,			str_create("set UNLIT") );	
	pan_setbutton(obstacle_panel, 0,	2, 	15,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_ToggleUnlit,NULL,NULL); 
	button_state( obstacle_panel , 9 , obstacle_unlit ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(obstacle_panel, 0,	15,30,	"COUNT: %0.f",				panel_font,	1, obstacle_counter);
	
	// buttns 10-11 - model set      155/140/225,320
	pan_setdigits(obstacle_panel, 0,	180,330,	"Set:    %0.f",		panel_font,	1, obstacle_set);	
	pan_setbutton(obstacle_panel, 0,	1, 	165,330,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Obstacle_DecSet, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	225,330,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			Obstacle_IncSet, 		NULL, NULL); 
		
	// buttons 12-13 - draw area
	pan_setdigits(obstacle_panel, 0,	145,240,	"Area:       %0.f",		panel_font,	1, obstacle_tilesize);	
	pan_setbutton(obstacle_panel, 0,	1, 	175,241,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Obstacle_DecSize, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	210,241,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,			Obstacle_IncSize, 		NULL, NULL); 
	
	// density	
	pan_setdigits(obstacle_panel, 0,	15,200,	"Density: %0.f",		panel_font,	0.85, 							obstacle_density );
	pan_setslider(obstacle_panel, 0,	90+12,202,	horiz_slider_bmp,slider_knob_bmp,	1,max_obstacle_density+0.4,	obstacle_density); 
	
	// scale
	pan_setdigits(obstacle_panel, 0,	15,220,	"Scale: %0.2f",		panel_font,	1, 				obstacle_scale);
	pan_setslider(obstacle_panel, 0,	90+12,222,	horiz_slider_bmp,slider_knob_bmp,	0.75,1.25,		obstacle_scale);
	
	// button 14 - Light
	pan_setstring(obstacle_panel, 0,	40,100,	panel_font,			str_create("set LIGHT to adjust RGB values:") );	
	pan_setbutton(obstacle_panel, 0,	2, 	15,100,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_ToggleLight,NULL,NULL); 
	button_state( obstacle_panel , 14 , obstacle_light ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(obstacle_panel, 0,	15,120,	"R: %0.f",				panel_font,	1, 			obstacle_red);
	pan_setdigits(obstacle_panel, 0,	15,140,	"G: %0.f",				panel_font,	1, 			obstacle_green);
	pan_setdigits(obstacle_panel, 0,	15,160,	"B: %0.f",				panel_font,	1, 			obstacle_blue);
	pan_setslider(obstacle_panel, 0,	90+12,122,	horiz_slider_bmp,slider_knob_bmp,	0,255,		obstacle_red); 
	pan_setslider(obstacle_panel, 0,	90+12,142,	horiz_slider_bmp,slider_knob_bmp,	0,255,		obstacle_green); 
	pan_setslider(obstacle_panel, 0,	90+12,162,	horiz_slider_bmp,slider_knob_bmp,	0,255,		obstacle_blue); 
		
	pan_setdigits(obstacle_panel, 0,	15,60,	"Ambient: %0.f",		panel_font,	1, 			obstacle_ambient);
	pan_setslider(obstacle_panel, 0,	90+12,62,	horiz_slider_bmp,slider_knob_bmp,	0,100,		obstacle_ambient); 
	
	pan_setdigits(obstacle_panel, 0,	15,80,	"Alpha: %0.f",			panel_font,	1, 			obstacle_alpha);
	pan_setslider(obstacle_panel, 0,	90+12,82,	horiz_slider_bmp,slider_knob_bmp,	0,100,		obstacle_alpha); 
	
	// random +/-10% fine scaling with a checkbox and a string - no effect on other data
	// button 15 - RND Scale
	pan_setstring(obstacle_panel, 0,	40+100,280,	panel_font,			str_create("rnd fine scale") );	
	pan_setbutton(obstacle_panel, 0,	2, 	15+100,280,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_ToggleRandomize,NULL,NULL); 
	button_state( obstacle_panel , 15 , obstacle_randomize ); 		// 1 ON, 0 OFF	
	
	// button 16 - Overlay
	pan_setstring(obstacle_panel, 0,	40,300,	panel_font,			str_create("set OVERLAY") );
	pan_setbutton(obstacle_panel, 0,	2, 	15,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_ToggleOverlay,NULL,NULL); 
	button_state( obstacle_panel , 16 , obstacle_overlay ); 		// 1 ON, 0 OFF	
	
	// button 17 - Clear All
	pan_setbutton(obstacle_panel, 0,	1, 	45,480,		m_clearall_on_bmp,m_clearall_off_bmp,m_clearall_over_bmp,m_clearall_over_bmp,		Obstacle_DeleteAll,NULL,NULL); 
	
	// material buttons-----------------------
	
	// button 18 - Material - removed
//	pan_setbutton(obstacle_panel, 0,	1, 	200,10,		ss_material_on_bmp,ss_material_off_bmp,ss_material_over_bmp,ss_material_over_bmp,		Obstacle_MaterialSetup,NULL,NULL); 			
	pan_setbutton(obstacle_panel, 0,	1, 	255,10,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 19 - mat_model
	pan_setstring(obstacle_panel, 0,	40+100,180,	panel_font,			str_create("set model material") );	
	pan_setbutton(obstacle_panel, 0,	2, 	15+100,180,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_ToggleModel,NULL,NULL); 
	button_state( obstacle_panel , 19 , obstacle_model ); 		// 1 ON, 0 OFF	
	
	//----------------------------------------
	
	// button 20 - random rotation
	pan_setstring(obstacle_panel, 0,	40,280,	panel_font,			str_create("rnd rotation") );	
	pan_setbutton(obstacle_panel, 0,	2, 	15,280,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_ToggleRndRot,NULL,NULL); 
	button_state( obstacle_panel , 20 , obstacle_rnd_rot ); 		// 1 ON, 0 OFF	
	
	// button 21-22-23 - auto/set/no tree
	pan_setstring(obstacle_panel, 0,	35,262,	panel_font,			str_create("auto tree") );
	pan_setbutton(obstacle_panel, 0,	4, 	15,262,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		Obstacle_ToggleTree,NULL,NULL); 
	if (obstacle_istree==(var)0) button_state( obstacle_panel , 21 , ON ); 		// 1 ON, 0 OFF	
	
	pan_setstring(obstacle_panel, 0,	115,262,	panel_font,			str_create("set tree") );
	pan_setbutton(obstacle_panel, 0,	4, 	95,262,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		Obstacle_ToggleTree,NULL,NULL); 
	if (obstacle_istree==(var)1) button_state( obstacle_panel , 22 , ON ); 		// 1 ON, 0 OFF	
	
	pan_setstring(obstacle_panel, 0,	195,262,	panel_font,			str_create("not tree") );
	pan_setbutton(obstacle_panel, 0,	4, 	175,262,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		Obstacle_ToggleTree,NULL,NULL); 
	if (obstacle_istree==(var)2) button_state( obstacle_panel , 23 , ON ); 		// 1 ON, 0 OFF	
	
	// button 24 - set to Lightmap
//	if (shadow_stencil==(var)8)
		{
			pan_setstring(obstacle_panel, 0,	40+100,300,	panel_font,			str_create("Lightmap") );
		}
//	else
//		{
//			pan_setstring(obstacle_panel, 0,	40+100,300,	panel_font,			str_create("set to Lightmap") );
//		}
	pan_setbutton(obstacle_panel, 0,	2, 	15+100,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_ToggleLightmap,NULL,NULL); 
	button_state( obstacle_panel , 24 , obstacle_lightmap ); 		// 1 ON, 0 OFF	
	
	// button 25 - Modify - removed
//	pan_setbutton(obstacle_panel, 0,	1, 	177,480,		m_modify_on_bmp,m_modify_off_bmp,m_modify_over_bmp,m_modify_over_bmp,					ModifyObject_Init,		NULL,NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	255,580,		fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,fileselect_off_bmp,			NULL,							NULL,NULL); 
	
	// button 26 - Snap
	pan_setstring(obstacle_panel, 0,	15+20+20,240,	panel_font,			str_create("Snap XY / Z") );
	pan_setbutton(obstacle_panel, 0,	2, 	15,240,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_ToggleSnap,NULL,NULL); 
	button_state( obstacle_panel , 26 , obstacle_snap ); 		// 1 ON, 0 OFF	
	
	// button 27,28	29,30		
	pan_setbutton(obstacle_panel, 0,	1, 	90,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	210,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	90,220,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	210,220,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Obstacle_Panel_Button, 		NULL, NULL); 
	// button 31,32	33,34		35,36
	pan_setbutton(obstacle_panel, 0,	1, 	90,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	210,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	90,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	210,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	90,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	210,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Obstacle_Panel_Button, 		NULL, NULL); 
	// button 37,38	39,40
	pan_setbutton(obstacle_panel, 0,	1, 	90,60,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	210,60,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	90,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Obstacle_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	210,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Obstacle_Panel_Button, 		NULL, NULL); 
	
	// button 41 - Edit                    200,35
	pan_setbutton(obstacle_panel, 0,	1, 	100,330,		ss_edit_on_bmp,ss_edit_off_bmp,ss_edit_over_bmp,ss_edit_over_bmp,		Obstacle_EditFile,		NULL,NULL); 			
	
	// button 42 - Seed
	pan_setbutton(obstacle_panel, 0,	1, 	150,480,		m_seed_on_bmp,m_seed_off_bmp,m_seed_over_bmp,m_seed_over_bmp,		Seed_Init,	NULL,NULL); 
	
	// button 43 - SnapZ
	pan_setstring(obstacle_panel, 0,	75+20,240,	panel_font,			str_create("") );
	pan_setbutton(obstacle_panel, 0,	2, 	15+20,240,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_ToggleSnapZ,NULL,NULL); 
	button_state( obstacle_panel , 43 , obstacle_snapz ); 		// 1 ON, 0 OFF	
	
	// button 44 - force decal shadow (ent_decal) - useful when the level is used in both shadow_stencil=1 and shadow_stencil=8 modes
//	if (shadow_stencil==(var)8)
		{
			pan_setstring(obstacle_panel, 0,	40+175,300,	panel_font,			str_create("Decal") );
			pan_setbutton(obstacle_panel, 0,	2, 	15+175,300,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Obstacle_ToggleDecal,NULL,NULL); 
			button_state( obstacle_panel , 44 , obstacle_decal ); 		// 1 ON, 0 OFF	
		}
	
	// button 45 46 - Lightmap All : Set / Reset
	pan_setstring(obstacle_panel, 0,			130,30,		panel_font,			str_create("Ligthmap All") );
	pan_setbutton(obstacle_panel, 0,	1, 	200,20,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,					Obstacle_LightmapAll,		NULL,NULL); 
	pan_setbutton(obstacle_panel, 0,	1, 	200,40,		ss_reset_on_bmp,ss_reset_off_bmp,ss_reset_over_bmp,ss_reset_over_bmp,		Obstacle_LightmapAll,		NULL,NULL); 
	
	//--------------------------------------------------------------
	
	ptr_remove( temp_string );	
	
	// it can't have a loop because of a wait_for !
}


void		Obstacle_Panel_Toggle()
{
	toggle( obstacle_panel , SHOW );	
}


////////////////////////////////////////////////////


void		Obstacle_Panel_Button(button_number)
{
	// obstacle_density
	if (button_number==(var)27)
		{
			obstacle_density = maxv( 1 , --obstacle_density );
		}
	else if (button_number==(var)28)
		{
			obstacle_density = minv( max_obstacle_density , ++obstacle_density );
		}
	// obstacle_scale
	else if (button_number==(var)29)
		{
			obstacle_scale -= 0.01;
			obstacle_scale = maxv( 0.75 , obstacle_scale );
		}
	else if (button_number==(var)30)
		{
			obstacle_scale += 0.01;
			obstacle_scale = minv( 1.25 , obstacle_scale );
		}
	// obstacle_red
	else if (button_number==(var)31)
		{
			obstacle_red = maxv( 0 , --obstacle_red );
		}
	else if (button_number==(var)32)
		{
			obstacle_red = minv( 255 , ++obstacle_red );
		}
	// obstacle_green
	else if (button_number==(var)33)
		{
			obstacle_green = maxv( 0 , --obstacle_green );
		}
	else if (button_number==(var)34)
		{
			obstacle_green = minv( 255 , ++obstacle_green );
		}
	// obstacle_blue
	else if (button_number==(var)35)
		{
			obstacle_blue = maxv( 0 , --obstacle_blue );
		}
	else if (button_number==(var)36)
		{
			obstacle_blue = minv( 255 , ++obstacle_blue );
		}
	// obstacle_ambient
	else if (button_number==(var)37)
		{
			obstacle_ambient = maxv( 0 , --obstacle_ambient );
		}
	else if (button_number==(var)38)
		{
			obstacle_ambient = minv( 100 , ++obstacle_ambient );
		}
	// obstacle_alpha
	else if (button_number==(var)39)
		{
			obstacle_alpha = maxv( 0 , --obstacle_alpha );
		}
	else if (button_number==(var)40)
		{
			obstacle_alpha = minv( 100 , ++obstacle_alpha );
		}	
}


/////////////////////////////////////////////////////////////////


//// change by hotkey
//void		Obstacle_NextSet()
//{
//	if (proc_status(Obstacle_NextSet) > 0) return;
//	if (proc_status(Obstacle_UpdateSetNames) > 0) return;
//	if (!obstacle_ready) return;
//	obstacle_ready = 0;
//	wait(3);
//	
//	// jump to next existing set
//	obstacle_set++;
//	if (obstacle_set>max_obstacle_sets)
//		{
//			obstacle_set = 1;
//		}
//	Obstacle_UpdateSetNames();
//	wait_for(Obstacle_UpdateSetNames);
//	
//	wait(1);
//}


void		Obstacle_IncSet()
{
	if (proc_status(Obstacle_IncSet) > 0) return;
	if (proc_status(Obstacle_UpdateSetNames) > 0) return;
	if (!obstacle_ready) return;
	obstacle_ready = 0;
	wait(3);
	
	// jump to next existing set
	obstacle_set++;
	if (obstacle_set>max_obstacle_sets)
		{
//			obstacle_set = max_obstacle_sets;
//			return;
			obstacle_set = 1;
		}
	Obstacle_UpdateSetNames();
	wait_for(Obstacle_UpdateSetNames);
	
	wait(1);
}


void		Obstacle_DecSet()
{
	if (proc_status(Obstacle_DecSet) > 0) return;
	if (proc_status(Obstacle_UpdateSetNames) > 0) return;
	if (!obstacle_ready) return;
	obstacle_ready = 0;
	wait(3);
	
	// jump to next existing set
	obstacle_set--;
	if (obstacle_set<1)
		{
//			obstacle_set = 1;
//			return;
			obstacle_set = max_obstacle_sets;
		}
	Obstacle_UpdateSetNames();
	wait_for(Obstacle_UpdateSetNames);
	
	wait(1);
}


void		Obstacle_UpdateSetNames()
{
	if (proc_status(Obstacle_UpdateSetNames) > 0) return;
	
	int i;
	// copy the proper 8 filenames to string array
	int j = obstacle_set - 1;
	int k = j * max_obstacle_items;
	for(i=0;i<max_obstacle_items;i++)
		{
//			str_cpy( obstacle_filenames[i] , obstacle_foldername );						// subfolder
//			str_cat( obstacle_filenames[i] , (obstacles_txt->pstring)[k+i] );			// filename with extension
			str_cpy( obstacle_filenames[i] , "" );											// (or obstacle_foldername) subfolder
			str_cat( obstacle_filenames[i] , (obstacles_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi( obstacle_filenames[i] , "" ) )								// (or obstacle_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( obstacle_filenames[i] , "-" );
				}
		}
	// refresh 8 filename strings to display in menu
	for(i=0;i<max_obstacle_items;i++)
		{
			pan_setstring(obstacle_panel, 11+i,	55,350+i*15,	panel_font,			str_create( obstacle_filenames[i] ) );
		}	
	wait(1);
	
	// read values from .obs file
	Obstacle_OpenCreate_ObsFile( obstacle_filenames[obstacle_selected] );
	wait_for(Obstacle_OpenCreate_ObsFile);
	wait(1);
	
//	Obstacle_MorphCube();
	Obstacle_MorphPreviewModel();
	wait_for(Obstacle_MorphPreviewModel);
	wait(1);
}


///////////////////////////////////


// change by hotkey
void		Obstacle_Flag_Switch(var setplusone)
{
	if (proc_status(Obstacle_Flag_Switch) > 0) return;
	if (!obstacle_ready) return;
	obstacle_ready = 0;
	wait(3);
	
	int temp = setplusone -1 -1;	// -1 because of scancode, and -1 because of array pos are 0..7
	
//	obstacle_switch_type[temp] = 1 - obstacle_switch_type[temp];
//	// change checkbox state too
//	var temp_state = button_state( obstacle_panel , temp+1 , -1 ); 				// -1 to read value
//	button_state( obstacle_panel , temp+1 , 1-temp_state ); 						// 1 ON, 0 OFF
	
	int i;
	for(i=0;i<max_obstacle_items;i++)
		{
			obstacle_switch_type[i] = 0;						// 0..7
			button_state( obstacle_panel , i+1 , 0 );		// 0..8
		}
	obstacle_switch_type[temp] = 1;
	button_state( obstacle_panel , temp+1 , 1 ); 
	obstacle_selected = temp;
	
	// read values from .obs file
	Obstacle_OpenCreate_ObsFile( obstacle_filenames[obstacle_selected] );
	wait_for(Obstacle_OpenCreate_ObsFile);
	wait(1);
	
//	Obstacle_MorphCube();
	Obstacle_MorphPreviewModel();
	wait_for(Obstacle_MorphPreviewModel);
	
	wait(1);
}


// change by checkbox (buttons 1-8)
void		Obstacle_Flag_Toggle(button_number)
{
	if (proc_status(Obstacle_Flag_Toggle) > 0) return;
	if (!obstacle_ready) return;
	obstacle_ready = 0;
	wait(3);
	
//	obstacle_switch_type[button_number-1] = 1 - obstacle_switch_type[button_number-1] ;
	
	int i;
	for(i=0;i<max_obstacle_items;i++)
		{
			obstacle_switch_type[i] = 0;							// 0..7
			button_state( obstacle_panel , i+1 , 0 );			// 1..8
		}
	obstacle_switch_type[button_number-1] = 1;
	button_state( obstacle_panel , button_number , 1 ); 
	obstacle_selected = button_number-1;
	
	// read values from .obs file
	Obstacle_OpenCreate_ObsFile( obstacle_filenames[obstacle_selected] );
	wait_for(Obstacle_OpenCreate_ObsFile);
	wait(1);
	
//	Obstacle_MorphCube();
	Obstacle_MorphPreviewModel();
	wait_for(Obstacle_MorphPreviewModel);
	
	wait(1);
}


/////////////////////////////////
// decrease/increase obstacle densityif left mouse button is used for placement


void 		Obstacle_DecDens()
{
	obstacle_density--;
	if (obstacle_density<1)
		{
			obstacle_density = 1;
		}
}


void 		Obstacle_IncDens()
{
	obstacle_density++;
	if (obstacle_density>max_obstacle_density)
		{
			obstacle_density = max_obstacle_density;
		}
}


////////////////////////////////////


// change by hotkey
void 		Obstacle_DecSize()
{
	obstacle_tilesize -= 2;
	if (obstacle_tilesize<1)
		{
			obstacle_tilesize = 1;
		}	
	// resize cube
	var temp = obstacle_tilesize / obstacle_placement_cube.scale_x ;	
	vec_scale( obstacle_placement_cube.scale_x , temp );
}


// change by hotkey
void 		Obstacle_IncSize()
{
	obstacle_tilesize += 2;
	if (obstacle_tilesize>max_obstacle_size)
		{
			obstacle_tilesize = max_obstacle_size;
		}
	// resize cube
	var temp = obstacle_tilesize / obstacle_placement_cube.scale_x ;	
	vec_scale( obstacle_placement_cube.scale_x , temp );
}


/////////////////////////////////////////////////////
// change by checkbox


void		Obstacle_ToggleUnlit()
{
	obstacle_unlit = 1 - obstacle_unlit;	
	if (obstacle_unlit)
		{
			// switch off LIGHT
			obstacle_light = 0;
			button_state( obstacle_panel , 14 , obstacle_light );
			// switch off model
			obstacle_model = 0;
			button_state( obstacle_panel , 19 , obstacle_model );
		}
}


void		Obstacle_ToggleLight()
{
	obstacle_light = 1 - obstacle_light;	
	if (obstacle_light)
		{
			// switch off UNLIT
			obstacle_unlit = 0;
			button_state( obstacle_panel , 9 , obstacle_unlit );
		}
}


void		Obstacle_ToggleModel()
{
	obstacle_model = 1 - obstacle_model;
	if (obstacle_model)
		{
			// switch off UNLIT
			obstacle_unlit = 0;
			button_state( obstacle_panel , 9 , obstacle_unlit );
		}
}


void		Obstacle_ToggleRandomize()
{
	obstacle_randomize = 1 - obstacle_randomize;	
}


void		Obstacle_ToggleOverlay()
{
	obstacle_overlay = 1 - obstacle_overlay;	
}


void		Obstacle_ToggleRndRot()
{
	obstacle_rnd_rot = 1 - obstacle_rnd_rot;	
	
	// reset rotation
	if (obstacle_rnd_rot)
		{
			obstacle_model_preview.pan = 0;
		}
}


void		Obstacle_ToggleTree(button_number)
{
	if (button_number==21)
		{
			obstacle_istree = 0;
		}
	else if (button_number==22)
		{
			obstacle_istree = 1;
		}
	else if (button_number==23)
		{
			obstacle_istree = 2;
		}
}


void		Obstacle_ToggleLightmap()
{
	obstacle_lightmap = 1 - obstacle_lightmap;
	if (obstacle_lightmap)
		{
			obstacle_decal = 0;
			button_state( obstacle_panel , 44 , obstacle_decal );
		}
}


void		Obstacle_ToggleSnap()
{
	obstacle_snap = 1 - obstacle_snap;
}


void		Obstacle_ToggleSnapZ()
{
	obstacle_snapz = 1 - obstacle_snapz;
}


void		Obstacle_ToggleDecal()
{
	obstacle_decal = 1 - obstacle_decal;
	if (obstacle_decal)
		{
			obstacle_lightmap = 0;
			button_state( obstacle_panel , 24 , obstacle_lightmap );
		}
}


void		Obstacle_LightmapAll(button_number)
{
	set( map_loadpanel , SHOW);
	wait(3);
	
	var modified_lightmap = 0;
	
	if (button_number == (var)45)
		{
			modified_lightmap = 1;		// lightmap
		}
	else if (button_number == (var)46)
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
			if (temp_ent.ENT_CATEGORY == (var)UIMODE_OBSTACLE)	
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


void		Obstacle_MaterialSetup()
{
	wait(3);
	material_select = 15;
//	Material_MatPanel_Init( mtl_obstacle , mtl_obstacle_transp );	
	Material_MatPanel_Init( mtl_obstacle , NULL );								// transparent material not used any more over A8.30
//	MaterialSetup_Init();		// old settings panel		
}


void		Obstacle_EditFile()
{
	wait(3);
	STRING* temp_str = str_create("");
	if ( !str_cmpi(obstacle_filenames[obstacle_selected] , "-" ) )
		{					
			str_cpy( temp_str , obstacle_foldername );
			str_cat( temp_str , obstacle_filenames[obstacle_selected] );
			ObswModify_Init(obstacle_defaultsize, obstacle_tilesize_x, obstacle_tilesize_y, obstacle_type, obstacle_shadow, obstacle_collision, temp_str);
		}
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor placement


void	Obstacle_OnClick()
{
	if (!obstacle_ready) return;
	
	if (proc_status(Obstacle_OnClick) > 0) return;		// protection
		
	wait(-0.25);													// time interval of allowed function instances (determines placment speed)
	
	if (!mouse_left)												// single click
		{
			Obstacle_CreateArea();
		}
	else																// holding button
		{
			if (proc_status(Obstacle_CreateArea_Cont) < 1)
				{
					Obstacle_CreateArea_Cont();
				}
		}
}


void	 Obstacle_CreateArea()
{
	if (!obstacle_ready) return;
	
	int i,j,k,m,temp_int;
	STRING* temp_str = str_create("");
	random_seed(0);
	// set loop length 
	j = integer(obstacle_density);									
	//	loop length depends on obstacle_density 		// and obstacle_tilesize
	for ( i=0 ; i<j ; i++)
		{
			if ( !str_cmpi(obstacle_filenames[obstacle_selected] , "-" ) )
				{					
					str_cpy( temp_str , obstacle_foldername );
					str_cat( temp_str , obstacle_filenames[obstacle_selected] );
					
					// set random pos within creator cube area and shrub_tilesize
					var tempx = obstacle_placement_cube.x + random(obstacle_tilesize*GetMapTileSize()) - obstacle_tilesize*GetMapTileSize()/2;
					var tempy = obstacle_placement_cube.y + random(obstacle_tilesize*GetMapTileSize()) - obstacle_tilesize*GetMapTileSize()/2;					
					
					//snap to tile middle
					if (obstacle_snap) 
						{
							temp_int = XCoordToTile(tempx);
							tempx = XTileToCoord(temp_int);
							
							temp_int = YCoordToTile(tempy);
							tempy = YTileToCoord(temp_int);
					
							// modify snapping due to object size
							if (obstacle_tilesize_x%2 == 0)
								{
									tempx += GetMapTileSize() / 2;
								}
							if (obstacle_tilesize_y%2 == 0)
								{
									tempy += GetMapTileSize() / 2;
								}
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
					
					// snap to tilesize downwards
					if (obstacle_snapz) 
						{
							tempz = integer(tempz/GetMapTileSize())*GetMapTileSize();
						}		
					
					Obstacle_CreateObject(temp_str, vector(tempx,tempy,tempz));					
//					ent_create( temp_str , vector(tempx,tempy,tempz) , Obstacle_CreateItem );						
				}
		}
	ptr_remove(temp_str);
	// update clusters touched should be done when obstacle placement mode is left
}


void		Obstacle_CreateArea_Cont()				
{
	if (!obstacle_ready) return;
	
	if (proc_status(Obstacle_CreateArea_Cont)>1)	return;
	
	int temp_int;
	STRING* temp_str = str_create("");
	random_seed(0);
	while (mouse_left)
		{
			if ( !str_cmpi(obstacle_filenames[obstacle_selected] , "-" ) )
				{					
					str_cpy( temp_str , obstacle_foldername );
					str_cat( temp_str , obstacle_filenames[obstacle_selected] );
					
					// set random pos within creator cube area and shrub_tilesize
					var tempx = obstacle_placement_cube.x + random(obstacle_tilesize*GetMapTileSize()) - obstacle_tilesize*GetMapTileSize()/2;
					var tempy = obstacle_placement_cube.y + random(obstacle_tilesize*GetMapTileSize()) - obstacle_tilesize*GetMapTileSize()/2;					
					
					//snap to tile middle
					if (obstacle_snap) 
						{
							temp_int = XCoordToTile(tempx);
							tempx = XTileToCoord(temp_int);
							
							temp_int = YCoordToTile(tempy);
							tempy = YTileToCoord(temp_int);
					
							// modify snapping due to object size
							if (obstacle_tilesize_x%2 == 0)
								{
									tempx += GetMapTileSize() / 2;
								}
							if (obstacle_tilesize_y%2 == 0)
								{
									tempy += GetMapTileSize() / 2;
								}
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
					
					// snap to tilesize downwards
					if (obstacle_snapz) 
						{
							tempz = integer(tempz/GetMapTileSize())*GetMapTileSize();
						}
					
					Obstacle_CreateObject(temp_str, vector(tempx,tempy,tempz));
//					ent_create( temp_str , vector(tempx,tempy,tempz) , Obstacle_CreateItem );
					// to get its filename later: str_for_entfile( str_entfilename , my );	
					
					wait(-0.2);			// sets placement speed
				}			
		}
	ptr_remove(temp_str);
	// update clusters touched should be done when obstacle placement mode is left
}

///////////////////////////////////////////////////////
// seeding


void		Obstacle_SeedQuantity()
{
	if (!obstacle_ready) return;
	
	set( map_loadpanel , SHOW);
	wait(3);
	//----------------------------------------
	STRING* temp_str = str_create("");
	str_cpy( temp_str , obstacle_foldername );
	str_cat( temp_str , obstacle_filenames[obstacle_selected] );	
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
	int i;
	for (i=0;i<seed_quantity;i++)
		{
			var posx;
			var posy;
			var posz;
			int temp_int;
			var summed_bool = 0;
			
			do 
				{
					posx = random(outer_areax*2)-outer_areax;
					posy = random(outer_areay*2)-outer_areay;
					
					// snap to tile middle
					if (obstacle_snap) 
						{
							temp_int = XCoordToTile(posx);
							posx = XTileToCoord(temp_int);
							
							temp_int = YCoordToTile(posy);
							posy = YTileToCoord(temp_int);
					
							// modify snapping due to object size
							if (obstacle_tilesize_x%2 == 0)
								{
									posx += GetMapTileSize() / 2;
								}
							if (obstacle_tilesize_y%2 == 0)
								{
									posy += GetMapTileSize() / 2;
								}
						}
					
					posz = PosInfo3D4OrZeroLevel(posx, posy);
					
					// snap to tilesize downwards
					if (obstacle_snapz) 
						{
							posz = integer(posz/GetMapTileSize())*GetMapTileSize();
						}
					
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
			
			Obstacle_CreateObject(temp_str, vector(posx,posy,posz));
//			ent_create( temp_str , vector(posx,posy,posz) , Obstacle_CreateItem );	
			
			if (i%10==0)		// at every 10th entity loading
				{
					Map_Progressbar(i/seed_quantity*100);
					wait_for(Map_Progressbar);
					wait(1);
				}	
		}
	//----------------------------------------
	wait(1);
	ptr_remove(temp_str);
	reset(map_loadbar,SHOW); 
	reset( map_loadpanel , SHOW);
}


void		Obstacle_SeedArea()
{
	if (!obstacle_ready) return;
	
	set( map_loadpanel , SHOW);
	wait(3);
	//----------------------------------------
	STRING* temp_str = str_create("");
	str_cpy( temp_str , obstacle_foldername );
	str_cat( temp_str , obstacle_filenames[obstacle_selected] );	
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
	
	for (i = -outer_areax + seed_distance/2; i < outer_areax - seed_distance/2; i+=seed_distance)
		{
			for (j = -outer_areay + seed_distance/2; j < outer_areay - seed_distance/2; j+=seed_distance)
				{
					posx = i + random(seed_rnddist*2) - seed_rnddist;
					posy = j + random(seed_rnddist*2) - seed_rnddist;
					
					// snap to tile middle
					if (obstacle_snap) 
						{
							temp_int = XCoordToTile(posx);
							posx = XTileToCoord(temp_int);
							
							temp_int = YCoordToTile(posy);
							posy = YTileToCoord(temp_int);						
					
							// modify snapping due to object size
							if (obstacle_tilesize_x%2 == 0)
								{
									posx += GetMapTileSize() / 2;
								}
							if (obstacle_tilesize_y%2 == 0)
								{
									posy += GetMapTileSize() / 2;
								}
						}
					
					var posz = PosInfo3D4OrZeroLevel(posx, posy);
					
					// snap to tilesize downwards
					if (obstacle_snapz) 
						{
							posz = integer(posz/GetMapTileSize())*GetMapTileSize();
						}
					
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
					
					if ( !summed_bool )					
						{
							Obstacle_CreateObject(temp_str, vector(posx,posy,posz));
//							ent_create( temp_str , vector(posx,posy,posz) , Obstacle_CreateItem );	
							c++;
							
							if (c%10==0)		// at every 100th entity loading
								{
									Map_Progressbar(i / outer_areax * 100);
									wait_for(Map_Progressbar);
									wait(1);
								}	
						}											
				}
			
			if (c > seed_limit) break;		// safety limit
			if (key_b) break;					// safety exit similar to teredit lightmapping
			
			wait(1);
		}
	//----------------------------------------	
	wait(1);
	ptr_remove(temp_str);
	reset(map_loadbar,SHOW); 
	reset( map_loadpanel , SHOW);
}


///////////////////////////////////////////////


void	 Obstacle_CreateObject(STRING* ent_name_full, VECTOR* pos_vec)
{		
	STRING* temp_str = str_create("");
	str_cpy(temp_str, ent_name_full);
	
	//----------------------------------------------------------------------------
	// create object
	
//	var my_scale = GetMapTileSize()/obstacle_defaultsize * obstacle_scale;
	var my_scale = obstacle_model_preview.scale_x;	
	
	if (obstacle_randomize)
		{
			// uniform randomness to keep object size ratio
			my_scale *= (1+(random(20)-10)/100);
		}		
	
	//---
	
	var my_tree = 0;
	if (obstacle_istree==0)																											// auto detect
		{
			if ( (obstacle_tilesize_x==(var)1) && (obstacle_tilesize_y==(var)1) )
				{
					VECTOR vTop;
					vec_for_max(vTop, obstacle_model_preview); 																// vFeet.z = distance from origin to lowest vertex
					vec_scale(vTop, obstacle_model_preview.scale_z);					
					if (vTop.z > GetMapTileSize()*5)
						{
							my_tree = 1;
						}
				}		
		}
	else if (obstacle_istree==1)																									// set tree
		{
			my_tree = 1;
		}
	
	//---
	
	var my_pan = obstacle_model_preview.pan;
	var my_tilt = obstacle_model_preview.tilt;
	var my_roll = obstacle_model_preview.roll;
	
	if ((obstacle_rnd_rot) && (obstacle_type != (var)2))
		{
			if (my_tree)
				{
					// random pan, tilt and roll
					my_pan  += random(40) -20;		// keep small because of shadow rotation
					my_tilt += random(20) -10;
					my_roll += random(20) -10;
				}
			else if (obstacle_tilesize_x == obstacle_tilesize_y)
				{
					my_pan = random(360) -180;
				}
		}
	
	//---
	
	Obstacle_Create(	temp_str, 										
							obstacle_defaultsize,
							obstacle_tilesize_x, obstacle_tilesize_y,
							obstacle_type, obstacle_collision, obstacle_shadow,																
							my_scale, my_scale, my_scale,								
							pos_vec.x, pos_vec.y, pos_vec.z, 																
							my_pan, my_tilt, my_roll,							
							obstacle_alpha, obstacle_ambient, 
							obstacle_light, obstacle_red, obstacle_green, obstacle_blue, 
							obstacle_unlit, obstacle_overlay, obstacle_model,										
							my_tree, (1-obstacle_lightmap), obstacle_decal
						);
//	wait_for(Obstacle_Create);
//	
	//----------------------------------------------------------------------------			
//			
//	// snapped coordinates - preview model is already snapped if set to snap xy/z
//	ent_create( temp_str, obstacle_model_preview.x, Obstacle_CreateItem );
//			
//	// to get its filename later: str_for_entfile( str_entfilename , my );	
//			
	//----------------------------------------------------------------------------	
	
//	wait(1);
	ptr_remove(temp_str);
}


void		Obstacle_CloneObject()
{
	//-----------------------------------------------------------------------
	// select
	
	mod_origmenu = (var)UIMODE_OBSTACLE;
	
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
	str_cpy( temp_str , obstacle_foldername );
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
	// set obstacle properties from .obs file 
	// defaultsize for scaling, x/y tile area for tile management, type (passability), collision shape, shadow
	
	var obs_defaultsize;
	var obs_tilesize_x;
	var obs_tilesize_y;
	var obs_type;
	var obs_collision;
	var obs_shadow;
	
	Obstacle_ReadObsFileData(str_for_entfile(NULL, you), &obs_defaultsize, &obs_tilesize_x, &obs_tilesize_y, &obs_type, &obs_collision, &obs_shadow);
	
	//-----------------------------------------------------------------------
	// collision
	
//	if (my.shadow != NULL)
	if (((shadow_stencil>(var)-1) && (shadow_stencil<(var)8)) || (is(my, FLAG5)))			// collider entity is needed when decal shadows are applied		// !!!	
		{
			Obstacle_CreateCollider(my);
		}
	
	//-----------------------------------------------------------------------
	// shadow - required by decal and ent_decal shadows
			
	Obstacle_SetShadow(my, (1-is(my, FLAG6)), is(my, FLAG5), obs_shadow, is(my, FLAG7));
	wait_for(Obstacle_SetShadow);
	
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


void		Obstacle_DeleteArea()
{
	Obstacle_RemoveArea(square_x, square_y, obstacle_tilesize, obstacle_tilesize);	
}


void		Obstacle_DeleteItem()
{
	MousePos3D0();
	
	if (HIT_TARGET)
		{
//			you = hit.entity;
			
			Obstacle_RemoveObject(hit.entity);
		}
}


void		Obstacle_DeleteAll()
{
	wait(3);

	Obstacle_RemoveAll();
	wait_for(Obstacle_RemoveAll);
}


////////////////////////////////////////////////////////////////////////////////////


#endif