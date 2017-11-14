
#ifndef MBsettings_c
#define MBsettings_c


/////////////////////////////////////////////////////////////////////////
// code

void	Settings_Init()
{
	//--------------------------------------
	// mapadjustable parameters
	
	settings_tilesize					= GetMapTileSize();
	settings_clustersize				= (var)GetMapClusterSize();
	
	settings_maptilesx				= (var)GetMapTilesX();
	settings_maptilesy				= (var)GetMapTilesY();
		
	//--------------------------------------
	// map info parameters
	
//	settings_scalevalue				= GetMapScaleValue();	
	settings_scalevalue				= settings_tilesize / MAP_DEFAULTGRIDSIZE;
		
	settings_mapzlevels				= (var)GetMapZLevels();
		
	settings_defaultterraintype	= (var)GetMapDefaultTerrainType();
	settings_defaultmovecost		= (var)GetMapDefaultMoveCost();
	
	settings_mapclusters				= ( settings_maptilesx/floor(settings_clustersize) ) * ( settings_maptilesy/floor(settings_clustersize) );						
	
	//--------------------------------------
	// terrain adjustable parameters	
	
	settings_blendfactor_ground	= GetMapGroundBlendFactor();
	settings_ground_detail			= GetMapGroundDetail();
	settings_water_detail			= GetMapWaterDetail();	
	
	settings_terrain_chunk			= terrain_chunk;										// sqrt( ent_status(terrain_entity,16) ); 
	
	//--------------------------------------
	// limit map tiles xy !!!
	
	settings_maxtilesx				= floor( (abs(TerrHmp_GetGroundMaxX()) * 2) / settings_tilesize );
	settings_maxtilesy				= floor( (abs(TerrHmp_GetGroundMaxY()) * 2) / settings_tilesize );
	
	//--------------------------------------
	
	Settings_Panel_Init();																		// contains main loop
	
	//--------------------------------------
}


void	Settings_Close()
{
	// panel
	ptr_remove(settings_panel);
	settings_panel = NULL;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	Settings_Panel_Init()
{
	//----------------------------------------------------------------------------------------------------------
	
	settings_panel = pan_create( panel_image2 , 11 );
	set( settings_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	settings_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	settings_panel.pos_y 	= ui_submenupanel_y;								// 256;
	settings_panel.red		= panel_font_r;
	settings_panel.green		= panel_font_g;
	settings_panel.blue		= panel_font_b;	
	settings_panel.event		= ui_submenu_panelmove;
	
	//----------------------------------------------------------------------------------------------------------
	
	//string 1 - title
	pan_setstring(settings_panel, 0,	15,10,	panel_font2,							str_create("MAP SETTINGS") );
	
	//string 2 - name
	STRING*	settings_mapname = str_create(""); 
	str_cpy(settings_mapname, "Map Name : ");
	str_cat(settings_mapname, GetMapName());
	pan_setstring(settings_panel, 0,	15,40,	panel_font,								settings_mapname );
	
	//----------------------------------------------------------------------------------------------------------
	
	// button 1 , ok -> Apply
	pan_setbutton(settings_panel, 0,	1, 		45,480,		m_apply_on_bmp,m_apply_off_bmp,m_apply_over_bmp,m_apply_over_bmp,						Settings_Ok		,NULL,NULL); 
	
	//----------------------------------------------------------------------------------------------------------
		
	// button 2-3 - detail_size ground
	pan_setdigits(settings_panel, 0,	15,80,				"Ground Detail: %0.f",	panel_font,			1, 					settings_ground_detail);
	pan_setslider(settings_panel, 0,	110+12,82,			horiz_slider_bmp,slider_knob_bmp,			-100,100,			settings_ground_detail); 
	pan_setbutton(settings_panel, 0,	1, 	110,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Settings_Panel_Button, 		NULL, NULL); 
	pan_setbutton(settings_panel, 0,	1, 	230,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Settings_Panel_Button, 		NULL, NULL); 
	
	// button 4-5 - detail_size water
	pan_setdigits(settings_panel, 0,	15,100,				"Water Detail: %0.f",	panel_font,			1, 					settings_water_detail);
	pan_setslider(settings_panel, 0,	110+12,102,			horiz_slider_bmp,slider_knob_bmp,			-100,100,			settings_water_detail); 
	pan_setbutton(settings_panel, 0,	1, 	110,100,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Settings_Panel_Button, 		NULL, NULL); 
	pan_setbutton(settings_panel, 0,	1, 	230,100,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Settings_Panel_Button, 		NULL, NULL); 
	
	// button 6-7 - tilesize 32..64   = 1,2,3 *MAP_DEFAULTGRIDSIZE ???
	pan_setdigits(settings_panel, 0,	15,130,	"Tile Size: %0.f",					panel_font,			1, 					settings_tilesize);
	pan_setslider(settings_panel, 0,	110+12,132,			horiz_slider_bmp,slider_knob_bmp,			16,100,				settings_tilesize); 
	pan_setbutton(settings_panel, 0,	1, 	110,130,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Settings_Panel_Button, 		NULL, NULL); 
	pan_setbutton(settings_panel, 0,	1, 	230,130,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Settings_Panel_Button, 		NULL, NULL); 
	
	// map scale : map_scalevalue 	= GetMapTileSize()/MAP_DEFAULTGRIDSIZE;
	pan_setdigits(settings_panel, 0,	15,150,	"Map Scale: %0.2f",					panel_font,			1, 					settings_scalevalue);
	
	// button 8-9 - cluster size 8..16
	pan_setdigits(settings_panel, 0,	15,180,	"Cluster Size: %0.f",				panel_font,			1, 					settings_clustersize);
	pan_setslider(settings_panel, 0,	110+12,182,			horiz_slider_bmp,slider_knob_bmp,			5,16,					settings_clustersize); 
	pan_setbutton(settings_panel, 0,	1, 	110,180,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Settings_Panel_Button, 		NULL, NULL); 
	pan_setbutton(settings_panel, 0,	1, 	230,180,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Settings_Panel_Button, 		NULL, NULL); 
	
	// button 10-11 - tiles per x side - < terrain_length/tilesize 
	pan_setdigits(settings_panel, 0,	15,210,	"Map Tiles X: %0.f",					panel_font,			1, 																		settings_maptilesx);
	pan_setslider(settings_panel, 0,	110+12,212,			horiz_slider_bmp,slider_knob_bmp,			settings_mintilesxy, settings_maxtilesx,						settings_maptilesx); 
	pan_setbutton(settings_panel, 0,	1, 	110,210,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Settings_Panel_Button, 		NULL, NULL); 
	pan_setbutton(settings_panel, 0,	1, 	230,210,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Settings_Panel_Button, 		NULL, NULL); 
	
	// button 12-13 - tiles per y side - < terrain_length/tilesize 
	pan_setdigits(settings_panel, 0,	15,230,	"Map Tiles Y: %0.f",					panel_font,			1, 																		settings_maptilesy);
	pan_setslider(settings_panel, 0,	110+12,232,			horiz_slider_bmp,slider_knob_bmp,			settings_mintilesxy, settings_maxtilesy,						settings_maptilesy); 
	pan_setbutton(settings_panel, 0,	1, 	110,230,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Settings_Panel_Button, 		NULL, NULL); 
	pan_setbutton(settings_panel, 0,	1, 	230,230,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Settings_Panel_Button, 		NULL, NULL); 	
	
	// button 14-15 - terrain chunk	
	pan_setdigits(settings_panel, 0,	100,260,	"Terrain Chunk: %0.f",				panel_font,			1, 					settings_terrain_chunk);
	pan_setbutton(settings_panel, 0,	1, 	80,260,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Settings_Panel_Button, 		NULL, NULL); 
	pan_setbutton(settings_panel, 0,	1, 	195,260,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Settings_Panel_Button, 		NULL, NULL); 
	
	//------------------------------------------------------------------------------------------------------
	// just to display:		
	
	// settings_mapclusters
	pan_setdigits(settings_panel, 0,	15,290,	"Map Base Clusters: %0.f",			panel_font,			1, 					settings_mapclusters);
	
	// z levels 0-1-2
	pan_setdigits(settings_panel, 0,	15,310,	"Map Z Levels: %0.f",				panel_font,			1, 					settings_mapzlevels);
		
	// other data
	pan_setdigits(settings_panel, 0,	15,330,	"Default Type: %0.f",				panel_font,			1, 					settings_defaultterraintype);
	pan_setdigits(settings_panel, 0,	15,350,	"Default Cost: %0.f",				panel_font,			1, 					settings_defaultmovecost);
	
	//------------------------------------------------------------------------------------------------------
	// button 16 - map info panels and custer representation
{}
#ifndef MB_RELEASE
	pan_setbutton(settings_panel, 0,	1, 	150,480,		m_mapinfo_on_bmp, m_mapinfo_off_bmp, m_mapinfo_over_bmp, m_mapinfo_over_bmp,		UI_MapInfoMode, 		NULL, NULL); 
#else
	// dummy
	pan_setbutton(settings_panel, 0,	1, 	255,480,		fileselect_off_bmp, fileselect_off_bmp, fileselect_off_bmp, fileselect_off_bmp,		NULL, 		NULL, NULL); 
#endif	

	//----------------------------------------------------------------------------------------------------------	
	// button 17-18 - settings_blendfactor_ground
	
	pan_setdigits(settings_panel, 0,	15,60,				"Tile Blending: %0.2f",	panel_font,			1, 					settings_blendfactor_ground);
	pan_setslider(settings_panel, 0,	110+12,62,			horiz_slider_bmp,slider_knob_bmp,			0,1,					settings_blendfactor_ground); 
	pan_setbutton(settings_panel, 0,	1, 	110,60,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Settings_Panel_Button, 		NULL, NULL); 
	pan_setbutton(settings_panel, 0,	1, 	230,60,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Settings_Panel_Button, 		NULL, NULL); 
	
	//------------------------------------------------------------------------------------------------------
	// button 19 20 - Lightmap All : Set / Reset
	
	pan_setstring(settings_panel, 0,			15,380,		panel_font,			str_create("Lightmap (all entities)") );
	pan_setbutton(settings_panel, 0,	1, 	130,380,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,					Settings_LightmapAll,		NULL,NULL); 
	pan_setbutton(settings_panel, 0,	1, 	190,380,		ss_reset_on_bmp,ss_reset_off_bmp,ss_reset_over_bmp,ss_reset_over_bmp,		Settings_LightmapAll,		NULL,NULL); 
	
	//------------------------------------------------------------------------------------------------------
	
	while(UIMode==(var)UIMODE_SETTINGS)
		{
			//------------------------------------------------------
			// round slider adjustable parameters
			
			settings_maptilesx			= (int)(settings_maptilesx + 0.5);
			settings_maptilesx			+= settings_maptilesx % 2;							// keep it even !!!
			
			settings_maptilesy			= (int)(settings_maptilesy + 0.5);
			settings_maptilesy			+= settings_maptilesy % 2;							// keep it even !!!
			
			settings_tilesize				= (int)(settings_tilesize + 0.5);
			settings_clustersize			= (int)(settings_clustersize + 0.5);
			
			//------------------------------------------------------
			// uddate needed - no direct setting allowed
			
			settings_scalevalue			= settings_tilesize / (var)MAP_DEFAULTGRIDSIZE;
			
			//------------------------------------------------------
			// uddate needed - no direct setting allowed
			
			int add_x = 0;
			if (settings_maptilesx % settings_clustersize > 0)
				{
					add_x = 1;
				}
			int add_y = 0;
			if (settings_maptilesy % settings_clustersize > 0)
				{
					add_y = 1;
				}
			settings_mapclusters			= (floor(settings_maptilesx / settings_clustersize) + add_x) * (floor(settings_maptilesy / settings_clustersize) + add_y);						
			
			//------------------------------------------------------
			// uddate needed - real-time (shaders only)
			
			SetMapGroundDetail( settings_ground_detail );
			SetMapGroundBlendFactor( settings_blendfactor_ground );
			
			//------------------------------------------------------
			
			wait(1);
			
			//------------------------------------------------------
		}
	
	//----------------------------------------------------------------------------------------------------------
	
	Settings_Close();
	
	ptr_remove(settings_mapname);
	
	//----------------------------------------------------------------------------------------------------------
}


void	Settings_Panel_Button(button_number)
{
	//-----------------------------------
	// ground detail
	if (button_number==(var)2)
		{
			settings_ground_detail = maxv( -100 , --settings_ground_detail );
		}
	else if (button_number==(var)3)
		{
			settings_ground_detail = minv( 100 , ++settings_ground_detail );
		}
	//-----------------------------------
	// water detail
	else if (button_number==(var)4)
		{
			settings_water_detail = maxv( -100 , --settings_water_detail );
		}
	else if (button_number==(var)5)
		{
			settings_water_detail = minv( 100 , ++settings_water_detail );
		}
	//-----------------------------------
	// tilesize
	else if (button_number==(var)6)
		{
			settings_tilesize = maxv( 16 , --settings_tilesize );
		}
	else if (button_number==(var)7)
		{
			settings_tilesize = minv( 100 , ++settings_tilesize );
		}
	//-----------------------------------
	// settings_clustersize
	else if (button_number==(var)8)
		{
			settings_clustersize = maxv( 5 , --settings_clustersize );
		}
	else if (button_number==(var)9)
		{
			settings_clustersize = minv( 16 , ++settings_clustersize );
		}
	//-----------------------------------
	// settings_maptilesx
	else if (button_number==(var)10)
		{
			settings_maptilesx = maxv( settings_mintilesxy , settings_maptilesx - 2 );
		}
	else if (button_number==(var)11)
		{
			settings_maptilesx = minv( settings_maptilesx , settings_maptilesx + 2 );
		}
	//-----------------------------------
	// settings_maptilesy
	else if (button_number==(var)12)
		{
			settings_maptilesy = maxv( settings_mintilesxy , settings_maptilesy - 2 );
		}
	else if (button_number==(var)13)
		{
			settings_maptilesy = minv( settings_maptilesy , settings_maptilesy + 2 );
		}
	//-----------------------------------
	// settings_terrain_chunk
	else if (button_number==(var)14)
		{
			if ((!map_wmb) && (terrain_entity))							// can be non-chunked if small enough, A8 save hmp works perfectly
				{
					if (ent_status(terrain_entity,0)<65536)
						{
							settings_terrain_chunk = maxv( 0 , settings_terrain_chunk - 8 );		
						}
					else
						{
							settings_terrain_chunk = maxv( 8 , settings_terrain_chunk - 8 );
						}
				}
			else																	// if wmb terrain mesh size cannot be checked
				{
					settings_terrain_chunk = maxv( 8 , settings_terrain_chunk - 8 );
				}
		}
	else if (button_number==(var)15)
		{
			settings_terrain_chunk = minv( 256 , settings_terrain_chunk+=8 );
		}
	//-----------------------------------
	// settings_blendfactor_ground
	else if (button_number==(var)17)
		{
			settings_blendfactor_ground = maxv( 0 , settings_blendfactor_ground - 0.01 );
		}
	else if (button_number==(var)18)
		{
			settings_blendfactor_ground = minv( 1 , settings_blendfactor_ground + 0.01 );
		}
	//-----------------------------------
}


void		Settings_Ok()
{
	//-------------------------------------------------------------
	
	wait(3);
	
	//-------------------------------------------------------------
	
	var reload_needed = 0;	
	
	if (GetMapTileSize() != floor(settings_tilesize))
		{
			reload_needed = 1;
		}
	if (terrain_chunk != floor(settings_terrain_chunk))
		{
			reload_needed = 1;
		}
	
	//-------------------------------------------------------------
	
	var regeneration_needed = 0;
	
	if (GetMapClusterSize() != floor(settings_clustersize))
		{
			regeneration_needed = 1;
		}
	if (GetMapTilesX() != floor(settings_maptilesx))
		{
			regeneration_needed = 1;
		}
	if (GetMapTilesY() != floor(settings_maptilesy))
		{
			regeneration_needed = 1;
		}
	
	//-------------------------------------------------------------
	// map
		
	SetMapTileSize( floor(settings_tilesize) );										// requires map reload
	
	settings_scalevalue 			= GetMapTileSize() / (var)MAP_DEFAULTGRIDSIZE;	
	SetMapScaleValue( settings_scalevalue );
	
	SetMapClusterSize( floor(settings_clustersize) );								// requires regenerating map arrays
	
	SetMapTilesX(settings_maptilesx);													// requires regenerating map arrays (or reload)
	SetMapTilesY(settings_maptilesy);													// requires regenerating map arrays (or reload)
	
#ifndef MB_RELEASE	
	play02_map_tilesize	= GetMapTileSize();											// variables read by RTS debug shaders
	play02_map_maptilesx	= (var)GetMapTilesX();
	play02_map_maptilesy	= (var)GetMapTilesY();
#endif
		
	//-------------------------------------------------------------
//	// no change
//
//	SetMapZLevels( settings_mapzlevels );	
//	SetMapDefaultTerrainType( settings_defaultterraintype );
//	SetMapDefaultMoveCost( settings_defaultmovecost );
//	
	//-------------------------------------------------------------
	// terrain	
	
	SetMapGroundDetail( floor(settings_ground_detail) );
	SetMapWaterDetail( floor(settings_water_detail) );
	
	SetMapTerrainChunk(settings_terrain_chunk);
	
	//-------------------------------------------------------------
	//	check closing action
	
	if (reload_needed)																		// tile size or terrain chunk change
		{
			FileMenu_Reload_Map();															// calls Terrain_StaticFree();
		}
	else if (regeneration_needed)															// xy tile number or cluster size change
		{
			//------------------------------------
{}
#ifndef MB_RELEASE				
			Terrain_StaticFree();
			wait_for(Terrain_StaticFree);						
#endif			
			//------------------------------------
			// recalculate entity abstile position parameters !!!
			
			int counter = 0;
			ENTITY* temp_ent = ent_next(NULL); 											// get first entity
			while(temp_ent)
				{
					temp_ent.XTILE_POS = XCoordToTile(temp_ent.x);
					temp_ent.YTILE_POS = YCoordToTile(temp_ent.y);
//					temp_ent.ZTILE_POS = 0;
					temp_ent.ABSTILE_POS = XYTileToAbsTile((int)temp_ent.XTILE_POS, (int)temp_ent.YTILE_POS);
									
					temp_ent = ent_next(temp_ent); 										// get next entity
					
					counter++;
					counter %= 1000;
					if (counter == 0)
						{
							wait(1);
						}
				}
			temp_ent = NULL;
			ptr_remove(temp_ent);
			
			//------------------------------------
			
			UI_FileMode();																		// close and jump to file menu
			
			//------------------------------------
		}
	else
		{
			UI_FileMode();																		// close and jump to file menu
		}
	
//	//-------------------------------------------------------------
//	//	reload map always
//	
//	FileMenu_Reload_Map();
//	
	//-------------------------------------------------------------
}


void		Settings_LightmapAll(button_number)
{
	//-------------------------------------------------------------
	
	set( map_loadpanel , SHOW);
	wait(3);
	
	//-------------------------------------------------------------
	
	var modified_lightmap = 0;
	
	if (button_number == (var)17)
		{
			modified_lightmap = 1;		// lightmap
		}
	else if (button_number == (var)18)
		{
			modified_lightmap = 0;		// dynamic shadow
		}
	else
		{
			return;
		}
	
	//-------------------------------------------------------------
	
	int counter = 0;
	ENTITY* temp_ent = ent_next(NULL); 																	// get first entity
	while(temp_ent)
		{
//			if (temp_ent.ENT_CATEGORY == (var)UIMODE_OBSTACLE)										// any, checked later, but only shrub, obstacle, mapent, terrmdl can have shadow
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
	
	//-------------------------------------------------------------
	
	wait(1);
	reset( map_loadpanel , SHOW);
	
	ptr_remove(temp_ent);
	
	//-------------------------------------------------------------
}


#endif