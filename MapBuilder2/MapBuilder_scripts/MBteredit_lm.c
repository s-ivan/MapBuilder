////////////////////////////////////////////////////////
// lightmap editor within terrain editor
////////////////////////////////////////////////////////

#ifndef MBteredit_lm_c
#define MBteredit_lm_c


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


void		TerEdit_Lm_Init()
{
	int i;
//	STRING* temp_string = str_create("");
	
	teredit_lm_panel = pan_create( NULL , 12 );
	set( teredit_lm_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	teredit_lm_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	teredit_lm_panel.pos_y 	= ui_submenupanel_y + 128;						// 256+128;
	teredit_lm_panel.red		= panel_font_r;
	teredit_lm_panel.green	= panel_font_g;
	teredit_lm_panel.blue	= panel_font_b;
	teredit_lm_panel.size_x	= 256;
	teredit_lm_panel.size_y	= 512-128;
	
	// lightmap size
//	teredit_lm_size = bmap_width( bmap_for_entity(terrain_entity,1) );	
	
	// tracing length
	teredit_lm_length = GetMapTileSize() * GetMapTilesX()/2;
	
	//-----------------------------------------------------------------------
	// title
	
	// string 1
	pan_setstring(teredit_lm_panel, 0,	15,15,	panel_font2,			str_create("TERRAIN LIGHTMAP") );
		
	// string 2
	pan_setstring(teredit_lm_panel, 0,	15,80,	panel_font,			str_create("Skin size:") );
	pan_setdigits(teredit_lm_panel, 0,	115,80,							"%0.f", 	panel_font, 	1, 	teredit_lm_size);
	// button 1 2 - skin texture size - read on init, and can be set by color fill			
	pan_setbutton(teredit_lm_panel, 0,	1, 	90,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	150,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 																							
	
	// string 3
	pan_setstring(teredit_lm_panel, 0,	15,50,	panel_font,			str_create("Lightmap") );
	// button 3 - fill with selected texture + set skin size
	pan_setbutton(teredit_lm_panel, 0,	1, 	70,50,		ss_apply_on_bmp,ss_apply_off_bmp,ss_apply_over_bmp,ss_apply_over_bmp,		TerEdit_Lm_Lightmap_Alpha,		NULL,NULL); 	
	
	// button 4 - overwrite
	pan_setstring(teredit_lm_panel, 0,	40,100,	panel_font,			str_create("Overwrite Lightmap") );	
	pan_setbutton(teredit_lm_panel, 0,	2, 	15,100,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerEdit_Lm_ToggleOver,NULL,NULL); 
	button_state( teredit_lm_panel , 4 , teredit_lm_overwrite ); 		// 1 ON, 0 OFF	
	
	// button 5 - alpha support
	pan_setstring(teredit_lm_panel, 0,	40,120,	panel_font,			str_create("Alpha Test") );	
	pan_setbutton(teredit_lm_panel, 0,	2, 	15,120,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerEdit_Lm_ToggleAlpha,NULL,NULL); 
	button_state( teredit_lm_panel , 5 , teredit_lm_alpha ); 		// 1 ON, 0 OFF	
	
	// button 6 - alpha support
	pan_setstring(teredit_lm_panel, 0,	100+40,120,	panel_font,			str_create("Terrain Shadows") );	
	pan_setbutton(teredit_lm_panel, 0,	2, 	100+15,120,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerEdit_Lm_ToggleTerrain,NULL,NULL); 
	button_state( teredit_lm_panel , 6 , teredit_lm_terrain ); 		// 1 ON, 0 OFF	
	
	// slider 1 - non-shadow colour, normally 128
	pan_setdigits(teredit_lm_panel, 0,	15,140,	"Base Color: %0.f",				panel_font,	1, 		teredit_lm_noshadow);
	pan_setslider(teredit_lm_panel, 0,	120+12,142,	horiz_slider_bmp,slider_knob_bmp,	0,255,	teredit_lm_noshadow); 
	// button 7 8
	pan_setbutton(teredit_lm_panel, 0,	1, 	120,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	240,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 
	
	// slider 2 - shadow colour, 84
	pan_setdigits(teredit_lm_panel, 0,	15,160,	"Shadow Color: %0.f",				panel_font,	1, 		teredit_lm_shadow);
	pan_setslider(teredit_lm_panel, 0,	120+12,162,	horiz_slider_bmp,slider_knob_bmp,	0,255,	teredit_lm_shadow); 
	// button 9 10
	pan_setbutton(teredit_lm_panel, 0,	1, 	120,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	240,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 
	
	// slider 3 - alpha shadow colour, 105
	pan_setdigits(teredit_lm_panel, 0,	15,180,	"Terrain Shadow: %0.f",				panel_font,	1, 		teredit_lm_alphashadow);
	pan_setslider(teredit_lm_panel, 0,	120+12,182,	horiz_slider_bmp,slider_knob_bmp,	0,255,	teredit_lm_alphashadow); 
	// button 11 12
	pan_setbutton(teredit_lm_panel, 0,	1, 	120,180,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	240,180,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 
	
	// button 13 - use default trace length
	pan_setstring(teredit_lm_panel, 0,	40,200,	panel_font,			str_create("Use Default Tracing Length") );	
	pan_setbutton(teredit_lm_panel, 0,	2, 	15,200,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerEdit_Lm_ToggleLength,NULL,NULL); 
	button_state( teredit_lm_panel , 13 , teredit_lm_defaulttrace ); 		// 1 ON, 0 OFF	
	
	// slider 4 - tracing length, 105
	pan_setdigits(teredit_lm_panel, 0,	15,220,	"Trace Length: %0.f",				panel_font,	1, 		teredit_lm_length);
	pan_setslider(teredit_lm_panel, 0,	120+12,222,	horiz_slider_bmp,slider_knob_bmp,	1000,10000,	teredit_lm_length); 
	// button 14 15
	pan_setbutton(teredit_lm_panel, 0,	1, 	120,220,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	240,220,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 
	
	// button 16 - save
	pan_setbutton(teredit_lm_panel, 0,	1, 	130,50,		ss_save_on_bmp,ss_save_off_bmp,ss_save_over_bmp,ss_save_over_bmp,		TerEdit_Lm_SaveLightMap,		NULL,NULL); 	
	
	// button 17 - load
	pan_setbutton(teredit_lm_panel, 0,	1, 	190,50,		ss_load_on_bmp,ss_load_off_bmp,ss_load_over_bmp,ss_load_over_bmp,		TerEdit_Lm_LoadLightMap,		NULL,NULL); 	
	
	// button 18 - clear or delete
	pan_setbutton(teredit_lm_panel, 0,	1, 	190,80,		ss_clear_on_bmp,ss_clear_off_bmp,ss_clear_over_bmp,ss_clear_over_bmp,		TerEdit_Lm_Lightmap_Clear,		NULL,NULL); 	
	
	//------------------------------------------------
	// string x
	pan_setstring(teredit_lm_panel, 0,	15,320,	panel_font,			str_create("Blur") );
	// button 19 - blur
	pan_setbutton(teredit_lm_panel, 0,	1, 	70,320,		ss_apply_on_bmp,ss_apply_off_bmp,ss_apply_over_bmp,ss_apply_over_bmp,		TerEdit_Lm_DoBlur,		NULL,NULL); 	
	
	// string x
	pan_setstring(teredit_lm_panel, 0,	145,320,	panel_font,			str_create("Passes:") );
	pan_setdigits(teredit_lm_panel, 0,	215,320,							"%0.f", 	panel_font, 	1, 	teredit_lm_passes);
	// button 20 21 - blurring passes
	pan_setbutton(teredit_lm_panel, 0,	1, 	190,320,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	240,320,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 																							
	
	// string x
	pan_setstring(teredit_lm_panel, 0,	145,340,	panel_font,			str_create("Strength:") );
	pan_setdigits(teredit_lm_panel, 0,	215,340,							"%0.f", 	panel_font, 	1, 	teredit_lm_strength);
	// button 22 23 - blurring strength
	pan_setbutton(teredit_lm_panel, 0,	1, 	190,340,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	240,340,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 																							
	
	// slider 5 - blurring treshhold, 28
	pan_setdigits(teredit_lm_panel, 0,	15,360,	"Treshold: %0.f",				panel_font,	1, 		teredit_lm_treshold);
	pan_setslider(teredit_lm_panel, 0,	120+12,362,	horiz_slider_bmp,slider_knob_bmp,	0,64,	teredit_lm_treshold); 
	// button 24 25
	pan_setbutton(teredit_lm_panel, 0,	1, 	120,360,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	240,360,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 
	
	//-------------------------------------------------
	
	// slider 5 - teredit_lm_accuracy
	pan_setdigits(teredit_lm_panel, 0,	15,240,	"Alpha Accuracy: %0.f",				panel_font,	1, 		teredit_lm_accuracy);
	pan_setslider(teredit_lm_panel, 0,	120+12,242,	horiz_slider_bmp,slider_knob_bmp,	5,200,	teredit_lm_accuracy); 
	// button 26 27
	pan_setbutton(teredit_lm_panel, 0,	1, 	120,240,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	240,240,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 
	
	// slider 6 - teredit_lm_alphalevel
	pan_setdigits(teredit_lm_panel, 0,	15,260,	"Alpha Treshold: %0.f",				panel_font,	1, 		teredit_lm_alphalevel);
	pan_setslider(teredit_lm_panel, 0,	120+12,262,	horiz_slider_bmp,slider_knob_bmp,	0,100,	teredit_lm_alphalevel); 
	// button 28 29
	pan_setbutton(teredit_lm_panel, 0,	1, 	120,260,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	240,260,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 
	
	// slider 7 - teredit_lm_iteration
	pan_setdigits(teredit_lm_panel, 0,	15,280,	"Iterated offset: %0.f",				panel_font,	1, 		teredit_lm_iteration);
	pan_setslider(teredit_lm_panel, 0,	120+12,282,	horiz_slider_bmp,slider_knob_bmp,	0,64,	teredit_lm_iteration); 
	// button 30 31
	pan_setbutton(teredit_lm_panel, 0,	1, 	120,280,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Lm_Button, 		NULL, NULL); 
	pan_setbutton(teredit_lm_panel, 0,	1, 	240,280,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Lm_Button, 		NULL, NULL); 
	
	//-------------------------------------------------
	
//	ptr_remove(temp_string);
}


void		TerEdit_Lm_Close()
{
	// normal closing, so no need to store anymore, thus delete backup files
	file_remove_onepath("maps\\lightmap_backup.tga");
	file_remove_onepath("maps\\lightmap_backup.lmp");
	
	if (teredit_lm_panel)
		{
			ptr_remove(teredit_lm_panel);
			teredit_lm_panel = NULL;
		}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Lm_Button(button_number)
{
	wait(3);
	
	// lightmap bitmap size
	if (button_number==(var)1)
		{
			teredit_lm_size /= 2;
			teredit_lm_size = maxv( 512 , teredit_lm_size );
		}
	else if (button_number==(var)2)
		{
			teredit_lm_size *= 2;
			teredit_lm_size = minv( d3d_texlimit , teredit_lm_size );
		}
	//--------------------------------------------	
	// non-shadow color, normally 128
	else if (button_number==(var)7)
		{
			teredit_lm_noshadow = maxv( 0 , --teredit_lm_noshadow );
		}
	else if (button_number==(var)8)
		{
			teredit_lm_noshadow = minv( 255 , ++teredit_lm_noshadow );
		}
	//--------------------------------------------	
	// non-shadow color, normally 84
	else if (button_number==(var)9)
		{
			teredit_lm_shadow = maxv( 0 , --teredit_lm_shadow );
		}
	else if (button_number==(var)10)
		{
			teredit_lm_shadow = minv( 255 , ++teredit_lm_shadow );
		}
	//--------------------------------------------	
	// alpha shadow color, normally 105
	else if (button_number==(var)11)
		{
			teredit_lm_alphashadow = maxv( 0 , --teredit_lm_alphashadow );
		}
	else if (button_number==(var)12)
		{
			teredit_lm_alphashadow = minv( 255 , ++teredit_lm_alphashadow );
		}
	//--------------------------------------------	
	// trace length, normally tilesize*tiles/2
	else if (button_number==(var)14)
		{
			teredit_lm_length = maxv( 1000 , teredit_lm_length-50 );
		}
	else if (button_number==(var)15)
		{
			teredit_lm_length = minv( 10000 , teredit_lm_length+50 );
		}	
	//--------------------------------------------	
	// number of blurring passes
	else if (button_number==(var)20)
		{
			teredit_lm_passes = maxv( 1 , --teredit_lm_passes );
		}
	else if (button_number==(var)21)
		{
			teredit_lm_passes = minv( 5 , ++teredit_lm_passes );
		}	
	//--------------------------------------------	
	// blurring strength
	else if (button_number==(var)22)
		{
			teredit_lm_strength = maxv( 1 , --teredit_lm_strength );
		}
	else if (button_number==(var)23)
		{
			teredit_lm_strength = minv( 10 , ++teredit_lm_strength );
		}	
	//--------------------------------------------	
	// blurring treshold
	else if (button_number==(var)24)
		{
			teredit_lm_treshold = maxv( 0 , --teredit_lm_treshold );
		}
	else if (button_number==(var)25)
		{
			teredit_lm_treshold = minv( 64 , ++teredit_lm_treshold );
		}	
	//--------------------------------------------	
	// alpha accuracy
	else if (button_number==(var)26)
		{
			teredit_lm_accuracy = maxv( 5 , --teredit_lm_accuracy );
		}
	else if (button_number==(var)27)
		{
			teredit_lm_accuracy = minv( 200 , ++teredit_lm_accuracy );
		}
	//--------------------------------------------	
	// alpha treshold
	else if (button_number==(var)28)
		{
			teredit_lm_alphalevel = maxv( 0 , --teredit_lm_alphalevel );
		}
	else if (button_number==(var)29)
		{
			teredit_lm_alphalevel = minv( 100 , ++teredit_lm_alphalevel );
		}
	//--------------------------------------------
	// teredit_lm_iteration
	else if (button_number==(var)30)
		{
			teredit_lm_iteration = maxv( 0 , --teredit_lm_iteration );
		}
	else if (button_number==(var)31)
		{
			teredit_lm_iteration = minv( 64 , ++teredit_lm_iteration );
		}
}


void		TerEdit_Lm_ToggleOver()
{
	teredit_lm_overwrite = 1 - teredit_lm_overwrite;	
}


void		TerEdit_Lm_ToggleAlpha()
{
	teredit_lm_alpha = 1 - teredit_lm_alpha;	
}


void		TerEdit_Lm_ToggleTerrain()
{
	teredit_lm_terrain = 1 - teredit_lm_terrain;	
}


void		TerEdit_Lm_ToggleLength()
{
	teredit_lm_defaulttrace = 1 - teredit_lm_defaulttrace;	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Lm_SaveLightMap()
{
	wait(3);
	
	if (terrain_entity.lightmap)
		{
			FileMenu_SaveLightMap();
		}
}


void		TerEdit_Lm_LoadLightMap()
{
	wait(3);
	
	FileMenu_LoadLightMap();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// helpers from A8 level.c

//// calculate the world size of a terrain skin pixel
//#define TEREDIT_PIXELSIZE_TX(terrain,bmap) ((terrain.max_x-terrain.min_x)/bmap_width(bmap))
//#define TEREDIT_PIXELSIZE_TY(terrain,bmap) ((terrain.max_y-terrain.min_y)/bmap_height(bmap))


// convert a world position on terrain to a pixel position on a bitmap
VECTOR* TerEdit_Lm_PixelFromPos(ENTITY* terrain, float position_x, float position_y,BMAP* bmap_skin)
{
//	var pixel_x = (position_x-terrain.x-terrain.min_x) / TEREDIT_PIXELSIZE_TX(terrain,bmap_skin);
	// pixel pos x = (pos + terrain half) / (terrain size/skin size)
	float pixel_x  = ( position_x - terrain.min_x )/( (terrain.max_x-terrain.min_x) / bmap_width(bmap_skin) ) ;		
	
	if( (pixel_x < 0) || (pixel_x >= bmap_width(bmap_skin)) ) 
		return NULL;
	
//	var pixel_y = (terrain.y+terrain.max_y-position_y) / TEREDIT_PIXELSIZE_TY(terrain,bmap_skin);
	// pixel pos y = (terrain half - pos) / (terrain size/skin size)
	float pixel_y  = ( terrain.max_y - position_y )/( (terrain.max_y-terrain.min_y) / bmap_height(bmap_skin) ) ;
	
	if( (pixel_y < 0) || (pixel_y >= bmap_height(bmap_skin)) )
		return NULL;
		
	return vector((var)pixel_x, (var)pixel_y, 0);	
}


// convert a bitmap pixel position to a world position on terrain - with corrected origo (0..size-1)
VECTOR* TerEdit_Lm_PixelToPos(ENTITY* terrain, double pixel_x, double pixel_y, BMAP* bmap_skin)
{
	double pixel_sizex = ((double)(terrain.max_x)-(double)(terrain.min_x)) / (double)(bmap_width(bmap_skin));
	double pixel_sizey = ((double)(terrain.max_y)-(double)(terrain.min_y)) / (double)(bmap_height(bmap_skin));
	
	// (terrain left vertex pos + half_pixelx_shift) + (pixel_x) * pixelx_size_in_quants
//	var position_x = ( terrain.min_x + 0.5*TEREDIT_PIXELSIZE_TX(terrain,bmap_skin) ) + ( pixel_x*TEREDIT_PIXELSIZE_TX(terrain,bmap_skin) );
	double position_x = (double)(terrain.min_x) + (double)0.5 * pixel_sizex + pixel_x * pixel_sizex;
	
	// (terrain top vertex pos - half_pixely_shift) - (pixel_y) * pixely_size_in_quants
//	var position_y = ( terrain.max_y - 0.5*TEREDIT_PIXELSIZE_TY(terrain,bmap_skin) ) - ( pixel_y*TEREDIT_PIXELSIZE_TY(terrain,bmap_skin) );
	double position_y = (double)(terrain.max_y) - (double)0.5 * pixel_sizey - pixel_y * pixel_sizey ;
	
	return( vector((var)position_x, (var)position_y, 0) );
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Lm_Lightmap_Alpha()
{
	wait(3);
	
	if(proc_status(TerEdit_Lm_Lightmap_Alpha)>1)
		return;
	
	set( map_loadpanel , SHOW);
	wait(3);
	
	//------------------------------------------------
	// continue from backup file - set start pixel if continued from a backup image
	
	// backup files are removed when terrain lightmap editor is left - until that they are kept and read back thus automatically continued !!!
	
	var loadbackup = 0;
	int starti = 0;
	int startj = 0;
	int backup_counter = 0;
	var file_handle;
	
	if ( (file_exists_onepath("maps\\lightmap_backup.tga")) && (file_exists_onepath("maps\\lightmap_backup.lmp")) )
		{
			loadbackup = 1;						
		}
	
	//------------------------------------------------
	// lighmap skin
	
	BMAP* lightmap_skin_bmp = NULL;	
	
	if ( (terrain_entity.lightmap) && (!teredit_lm_overwrite) && (!loadbackup) )		// lightmap is 24b white - it can be kept
		{
			lightmap_skin_bmp = terrain_entity.lightmap;											// if wmb compiled to create terrain lightmap
		}
	else
		{													
			// read position data from backup file
			if (loadbackup)
				{		
					TerrHmp_Load_LightMap_tga( "maps\\lightmap_backup.tga" );
					wait(1);
					
					file_handle = file_open_read( "maps\\lightmap_backup.lmp" );
					
					if (file_handle)
						{
							File_Read_Asc( file_handle , NULL );
							
							starti = (int)file_var_read(file_handle);
							startj = (int)file_var_read(file_handle);
							
							// safety, always must be zero
							starti = 0;
							
							file_close(file_handle);
							
//							printf("backup lmp loaded ( %f , %f )", (double)starti, (double)startj);
						}
					
					lightmap_skin_bmp = terrain_entity.lightmap;	
				}			
			else
				{
					// 24b white, default 1024,1024,24 
					lightmap_skin_bmp = bmap_createblack(teredit_lm_size, teredit_lm_size, 24);		
					wait(1);
					// 128 no brightness change, 255 would result in a bright terrain
					bmap_fill(lightmap_skin_bmp, vector(teredit_lm_noshadow, teredit_lm_noshadow, teredit_lm_noshadow), 100);									
					wait(1);
					
					// assign new or overwrite existing
					terrain_entity.lightmap = lightmap_skin_bmp;	
				}
		}
	
	// safety check
	
	wait(1);
	
	if (!lightmap_skin_bmp)		return;
	
	if (bmap_format(lightmap_skin_bmp)!=888)
		{			
			bmap_to_format(lightmap_skin_bmp, 888);	 				
			wait(1);
		}	
	
	//------------------------------------------------
	
	// x,y skin size of lighmap 
	var skin_sizex = bmap_width( lightmap_skin_bmp );	
	var skin_sizey = bmap_height( lightmap_skin_bmp );
	
//	printf("backup lmp loaded ( %.0f , %.0f )", (double)starti, (double)startj);
	
	//------------------------------------------------
	VECTOR pixel_pos;	
	VECTOR target_pos;
	
//	// tracing origin: sun pos
//	var temp_roll = sun_angle.roll;																// store original value
//	sun_angle.roll = 50000;																			// Sun distance - back to default far value
//	VECTOR sun_vec;
//	vec_set( sun_vec , sun_pos );	
	// inverse Sun ray unit vector
	VECTOR sun_vec;
	vec_for_angle(sun_vec, sun_angle);
	
	// set tracing length
	var trace_length = 5000;
	if (teredit_lm_defaulttrace)
		{
			trace_length = GetMapTileSize() * (var)200;											// 32*32*0.5 = 512 ?
		}
	else
		{
			trace_length = teredit_lm_length;
		}
	vec_normalize(sun_vec, trace_length);														// set Sun distance by normlization
	
//	// reset sun distance
////	sun_angle.roll = 1.25*maxv(vec_length(terrhmp_vgroundmax),vec_length(terrhmp_vgroundmin));		// Sun distance - as in shadows.c when pssm used
//	sun_angle.roll = temp_roll;
	
	//------------------------------------------------
	var format = bmap_lock(lightmap_skin_bmp,0);
	
	// double check
	if (format < 565)
		{
			reset( map_loadpanel , SHOW);
			wait(3);
			return;		
		}
	
	var pixel 		 = pixel_for_vec( vector(teredit_lm_shadow,teredit_lm_shadow,teredit_lm_shadow), 					100, format);	
	var pixel_alpha = pixel_for_vec( vector(teredit_lm_alphashadow,teredit_lm_alphashadow,teredit_lm_alphashadow), 100, format);	
	
	bmap_unlock(lightmap_skin_bmp);
	wait(1);
	
	//------------------------------------------------
	// create temporary models instead of shrub sprites
	
	TerEdit_Lm_Create_ShrubModels();
	wait_for(TerEdit_Lm_Create_ShrubModels);
	
	//------------------------------------------------
	var alphahit = 1;		
	var nonshadowhit = 1;		
	int i,j,k;	
		
	//------------------------------------------------
	for (j = startj; j < (int)skin_sizey; ++j)
		{
			bmap_lock(lightmap_skin_bmp,0);
			
			for (i = starti; i < (int)skin_sizex; ++i)
				{					
					// pixel to world coordinates x,y okay !!! z is 0 !!!						
					vec_set( pixel_pos , TerEdit_Lm_PixelToPos(terrain_entity, (double)(var)i, (double)(var)j, lightmap_skin_bmp) );
					
//					draw_text( "Pixel pos:" , 500 , 125 , vector(255,255,255) );
//					draw_text( str_for_num(NULL, pixel_pos.x) , 500 , 150 , vector(255,255,255) );
//					draw_text( str_for_num(NULL, pixel_pos.y) , 550 , 150 , vector(255,255,255) );
					
					// error iteration +/-16 quants - works!!! for 002 size - maybe because of coordinates 0,0 and -0,-0
//					teredit_lm_iteration = (var)(int)GetMapTileSize(); // test
					if (teredit_lm_iteration > (var)0)
						{
							double x_iteration = (double)teredit_lm_iteration * ( (double)pixel_pos.x / ((double)terrain_entity.max_x - (double)terrain_entity.min_x) );		// * ( (terrain_entity.max_x - terrain_entity.min_x) / 20480);
							double y_iteration = (double)teredit_lm_iteration * ( (double)pixel_pos.y / ((double)terrain_entity.max_y - (double)terrain_entity.min_y) );		// * ( (terrain_entity.max_x - terrain_entity.min_x) / 20480) ;					
							pixel_pos.x += (var)x_iteration;
							pixel_pos.y += (var)y_iteration;
						}
					
					// use local trace to detect exact terrain z
					
					you = NULL;								// set to invalid because of traces within one frame
					memset(hit,0,sizeof(CONTACT));	// removes hit struct errors	
					
					VECTOR pos1, pos2;
					vec_set( pos1 , vector(pixel_pos.x, pixel_pos.y, TerrHmp_GetGroundMinZ() - GetMapTileSize()) );
					vec_set( pos2 , vector(pixel_pos.x, pixel_pos.y, TerrHmp_GetGroundMaxZ() + GetMapTileSize()) );       											
					c_trace (pos2, pos1, IGNORE_SPRITES | IGNORE_MODELS | IGNORE_MAPS | IGNORE_PASSABLE | IGNORE_CONTENT );  					
					
					if (HIT_TARGET) 
						{
//							draw_text( "HIT_TARGET-1 okay" , 150 , 125 , vector(255,255,255) );
																	
							// define z only now !!! raise a bit (1..5) to be over terrain level to avoid terrain self shadow, the closer the more precise, to avoid shadow shift (or shift this point towards Sun)
							pixel_pos.z = hit.z + 0.01; // 1
							
							// add pixel pos z and normalized sun_vec to get c_trace target					
//							vec_set(target_pos , sun_vec);
//							vec_add(target_pos , pixel_pos);	// to get really parallel sun rays in each pixel						
							vec_set(target_pos , pixel_pos);
							vec_add(target_pos , sun_vec);	// to get really parallel sun rays in each pixel	
							// or
//							vec_set(target_pos , sun_vec);	// sun direction changes in large levels!
							
							// more trials if argb texture is hit - c_trace from hit as new origin to target
							alphahit = 1;
							nonshadowhit = 1;
							
							// max XX trials, and check tracing vector length
							while(
										(alphahit > 0) 
										&& 
										(alphahit < teredit_lm_accuracy) 
										&& 
										(vec_length(vec_diff(NULL, target_pos, pixel_pos)) > 0)
									)								
								{
									you = NULL;								// set to invalid because of traces within one frame
									memset(hit,0,sizeof(CONTACT));	// removes hit struct errors		
															
//									me = camera_cameraobject;			// its group value is 1 just like collider entities - ignorable by push - A7
									c_ignore(1,0);							// ignore: obstacle and shrub collider entities									
									
									// trace from sun because models hits are not sensed from inside
									if (teredit_lm_terrain)
										{
											c_trace( target_pos, pixel_pos, IGNORE_CONTENT | USE_POLYGON | SCAN_TEXTURE | IGNORE_SPRITES );	// IGNORE_ME | IGNORE_PUSH | 					
										}
									else
										{
											c_trace( target_pos, pixel_pos, IGNORE_CONTENT | USE_POLYGON | SCAN_TEXTURE | IGNORE_SPRITES | IGNORE_WORLD );	// IGNORE_ME | IGNORE_PUSH | 							
										}
									
									// SCAN_TEXTURE for getting alpha from hit.skin1 hit.skin2					
									// POLYGON for trees shrubs and other passable entities
									// ME and PUSH for ignoring collider entities in A7 - c_ignore is fine alone in A8
									
									if (HIT_TARGET)								// you or HIT_TARGET or hit.entity
										{
//											draw_text( "HIT_TARGET-2 okay" , 250 , 125 , vector(255,255,255) );
											
//											you = hit.entity;
											
											if (!is(you,FLAG6))					// entity do not already has shadow - decal or sprite or model or stencil - ent_category is because of errors at 0.0.0 (hit.entity.ENT_CATEGORY > 0) && 
												{
													//(hit.subset<1)				// 1st mesh group or skin hit	
													// hit.u1 hit.v1				// texture uv cooridnates to get pixel data 0..1 so shoukd be * by skin bmap size x or y respectively
													// hit.skin1					// texture BMAP* to get pixel data																										
													
													if (ent_type(you)==(var)4)											// terrain hit - half shadow
														{
															draw_text( "TERRAIN HIT" , 150 , 125 , vector(255,255,0) );
															draw_text( str_for_entname(NULL, you) , 325 , 125 , vector(255,255,0) );
															
//															if (teredit_lm_terrain) // double check, set at c_trace now
																pixel_to_bmap( lightmap_skin_bmp , (var)i , (var)j , pixel_alpha );
															
															vec_set( target_pos , hit.x );
															alphahit++;	
														}
													else if (ent_type(you)==(var)5)									// model hit
														{														
															BMAP* temp_bmap = bmap_createblack(16,16,8888);							// nulled and removed later
															wait_for(bmap_createblack);
															
															temp_bmap = hit.skin1;
															
															// get hit texture from sprite or model/mapentity
//															if (ent_type(you)==(var)6)
//																{
//																	temp_bmap = NULL;									// do not process sprites!
//																	temp_bmap = bmap_for_entity(you, 0);		// okay for sprites too, else hit.skin1 could be used
//																	//temp_bmap = bmap_for_entity(you, 1);			// not okay for sprites !!!
//																}
//															else
//																{
//																	temp_bmap = hit.skin1;													// model or map-entity - can have multi skins
//																}
															
															if (temp_bmap!=NULL)																// safety - if hit.entity has no skin
																{
																	if ( (!is(temp_bmap,BMPF_ARGB)) || (!teredit_lm_alpha) )																			
//																	if ( (!(temp_bmap.flags&BMPF_ARGB)) || (!teredit_lm_alpha) )	// not alpha texture hit - full shadow										
																		{																	
																			draw_text( "HIT" , 150 , 175 , vector(0,0,255) );
																			draw_text( str_for_entname(NULL, you) , 325 , 175 , vector(0,0,255) );
																			
																			pixel_to_bmap( lightmap_skin_bmp , (var)i , (var)j , pixel );
																			
																			alphahit = 0;													
																			break;																// no more trials																
																		}
//																	else if (ent_type(you)==(var)6)										// sprite - test
//																		{
//																			draw_text( "SPRITE HIT" , 150 , 300 , vector(255,128,0) );
//																			draw_text( str_for_entname(NULL, you) , 325 , 300 , vector(255,128,0) );
//																			
//																			random_seed(1);								// alawys the same if >0 	$$$
//																			var temp_alpha = random(100);				// 0..100 
//																			
//																			if (temp_alpha<teredit_lm_alphalevel)
//																				{
//																					pixel_to_bmap( lightmap_skin_bmp , (var)i , (var)j , pixel );		// or pixel_alpha
//																				}
//																			
//																			vec_set( target_pos , hit.x );
//																			alphahit++;
//																		}
																	else // if (teredit_lm_alpha)												// alpha texture hit
																		{										
																			// bmap format conversion																			
																			if (bmap_format(temp_bmap)!=(var)8888)
																				{
																					var conv_error1 = bmap_to_format(temp_bmap, (var)8888);	
																					wait_for(bmap_to_format); 	
																					var conv_error2 = bmap_to_mipmap( temp_bmap );	
																					wait_for(bmap_to_mipmap); 
																					
																					if ((!conv_error1) || (!conv_error2))
																						{
																							draw_text( "BMAP CONVERSION ERROR" , 150 , 300 , vector(100,255,100) );
																							draw_text( str_for_entname(NULL, you) , 325 , 300 , vector(100,255,100) );
																							alphahit = 0;
																							break;
																						}
																				}																					
																			
																			// reading pixel alpha
																			var temp_format = bmap_lock(temp_bmap,0);
																			var temp_pixel;
																			temp_pixel = pixel_for_bmap( temp_bmap, hit.u1, hit.v1 );	// uv coord multiplied with bmap size																		
																			COLOR temp_c;
																			var temp_alpha;
																			pixel_to_vec(temp_c, temp_alpha, temp_format, temp_pixel);																			
																			
//																			// workaround of permanent alpha hit for sprites
//																			if (ent_type(you)==(var)6)
//																				{
//																					random_seed(0);
//																					temp_alpha = random(80);								// 0..100 but better to limit to 80
//																				}
																			
																			// process alpha
																			if (temp_alpha<teredit_lm_alphalevel)						// transparent alpha hit - no shadow
																				{
																					draw_text( "ALPHA HIT" , 150 , 200 , vector(0,255,255) );
																					draw_text( str_for_entname(NULL, you) , 325 , 200 , vector(0,255,255) );
																					
																					vec_set( target_pos , hit.x );
																					alphahit++;													// further c_traces are needed														
																				}
																			else																	// not transparent alpha hit - full shadow - or half should be? 
																				{
																					draw_text( "NON-ALPHA HIT" , 150 , 225 , vector(255,0,255) );
																					draw_text( str_for_entname(NULL, you) , 325 , 225 , vector(255,0,255) );
																					
																					pixel_to_bmap( lightmap_skin_bmp , (var)i , (var)j , pixel );
																					
																					alphahit = 0;
																					break;														// no more trials
																				}
																			
																			bmap_unlock(temp_bmap);																																																
																		}
//																	temp_bmap = NULL;											// free/unassign temp bmap
																}	
															else																	// hit.entity without skin
																{
																	draw_text( "NO SKIN HIT" , 150 , 150 , vector(175,175,175) );
																	draw_text( str_for_entname(NULL, you) , 325 , 150 , vector(175,175,175) );
																					
//																	pixel_to_bmap( lightmap_skin_bmp , (var)i , (var)j , pixel );		// for testing
																	
																	alphahit = 0;												
																	break;														// no more trials
																}	
															
															temp_bmap = NULL;													// free/unassign temp bmap
															ptr_remove(temp_bmap);											// remove temp bmap																																														
														}	
													else if ((ent_type(you)==(var)2)	|| (ent_type(you)==(var)3))			// ABT or BSP block and map entitiy
														{
															draw_text( "BLOCK/MAPENT HIT" , 150 , 175 , vector(0,0,255) );
															draw_text( str_for_entname(NULL, you) , 325 , 175 , vector(0,0,255) );
															
															pixel_to_bmap( lightmap_skin_bmp , (var)i , (var)j , pixel );
															
															alphahit = 0;													
															break;
														}																					
												}
											else																					// FLAG6 entity hit - no shadow, continue
												{
													draw_text( "NON-SHADOW HIT" , 150 , 250 , vector(255,0,0) );
													draw_text( str_for_entname(NULL , you)  , 325 , 250 , vector(255,0,0) );
													
													vec_set( target_pos , hit.x );
													alphahit++;									
													nonshadowhit++;															// because of low alpha accuracy can result in shadow casting by non shadow objects
													
//													printf( _chr( str_for_entname( NULL , hit.entity ) ) );		// debug
												}
										}
									else																							// no hit
										{
				//							pixel  = pixel_for_vec( vector(0,0,0), 25, format);					// test
				//							pixel_to_bmap( lightmap_skin_bmp , (var)i , (var)j , pixel );		// test									
											
											draw_text( "NO HIT" , 150 , 250 , vector(0,255,0) );									
											alphahit = 0;
											break; 																				// no more trials, if no entity found
										}
								}					// while alphahit
							
							// when multiple transparent alpha textures were hit, draw a lighter dot	
							if ((alphahit >= (var)teredit_lm_accuracy) && (nonshadowhit!=alphahit))			// over accuracy limit, but not only non-shadow models were hit
								{
									draw_text( "MULTI-ALPHA HIT" , 150 , 275 , vector(255,200,200) );
									draw_text( str_for_entname(NULL, you) , 325 , 275 , vector(255,200,200) );
									
									pixel_to_bmap( lightmap_skin_bmp , (var)i , (var)j , pixel_alpha );
									
									bmap_unlock(lightmap_skin_bmp);
									
									draw_text( "PRESS 'B' TO BREAK LIGHTMAP CREATION!" , 150 , 100 , vector(255,255,255) );
									if (key_b) break;
									
									wait(1);																						// to avoid overload and script crash
									bmap_lock(lightmap_skin_bmp,0);
								}
						}							// hit
					
					//--------------------------------------------
					// for large lightmap images an additional unlock/lock is needed
					if (i%1024 == 0) 
						{
							bmap_unlock(lightmap_skin_bmp);
							
							draw_text( "PRESS 'B' TO BREAK LIGHTMAP CREATION!" , 150 , 100 , vector(255,255,255) );
							if (key_b) break;
							
							wait(1);																					// to avoid overload and script crash
							bmap_lock(lightmap_skin_bmp,0);
						}
				}									// for i			
			
			//--------------------------------------------
			bmap_unlock(lightmap_skin_bmp);
//			bmap_to_mipmap(lightmap_skin_bmp); // no!!!
			
			Map_Progressbar(j/skin_sizey*100);
			wait_for(Map_Progressbar);
			
//			draw_text( str_for_num(NULL,sun_pos.x) , 75 , 100 , temp_col );
//			draw_text( str_for_num(NULL,sun_pos.y) , 75 , 125 , temp_col );
//			draw_text( str_for_num(NULL,sun_pos.z) , 75 , 150 , temp_col );
			
			draw_text( "PRESS 'B' TO BREAK LIGHTMAP CREATION!" , 150 , 100 , vector(255,255,255) );
			if (key_b) break;
			
			//--------------------------------------------
			// save backup image and position data
			++backup_counter;
			backup_counter %= 10;
			
			if (backup_counter == 0)
				{
					bmap_save( terrain_entity.lightmap, "maps\\lightmap_backup.tga" );
					wait_for(bmap_save);
					
					// write position data					
					file_handle = file_open_write( "maps\\lightmap_backup.lmp" );
					
					if (file_handle)
						{
							var vi = (var)0;																					// i was always == xwidth, thus made the loop broken (y is the relevant value)
							var vj = (var)j;
							
							File_Write_Asc(file_handle, "// lightmap loop position");					
							file_var_write(file_handle, vi);
							file_var_write(file_handle, vj);
							
							file_close(file_handle);
						}
				}			
			
			//--------------------------------------------
			wait(1);																												// to avoid overload and script crash
		}								// for j
		
	reset(map_loadbar,SHOW); 
	reset( map_loadpanel , SHOW);
	wait(1);
	
//	// for testing-----------------------------------------------------------------------------------
//	if (lightmap_skin_bmp!=NULL)																	// double check
//		{
//			TerEdit_Lm_Blur( 1, lightmap_skin_bmp, 28, 5 );									// pass , BMAP*, treshold, strength
//			wait_for(TerEdit_Lm_Blur);
//			
//			// save resulting image
//			STRING* temp_str = str_create ("");
//			str_cpy( temp_str , filemenu_foldername );										// to maps\\
//			str_cat( temp_str , GetMapName() );
//			str_cat( temp_str , ".tga" );
//			wait(1);
//			bmap_save( lightmap_skin_bmp, temp_str );
//			wait_for(bmap_save);
//			wait(1);
//			ptr_remove(temp_str);
//		}
//	//-------------------------------------------------------------------------------------------------
	
	// remove temporary shrub models
	TerEdit_Lm_Remove_ShrubModels();
	wait_for(TerEdit_Lm_Remove_ShrubModels);
	
	wait(1);
	// free temp bmap
	lightmap_skin_bmp = NULL;
	ptr_remove(lightmap_skin_bmp);
}


void		TerEdit_Lm_Lightmap_Clear()
{
	wait(3);
	
	if (terrain_entity.lightmap)
		{
			bmap_purge(terrain_entity.lightmap);	//?
			ptr_remove(terrain_entity.lightmap);	
			terrain_entity.lightmap = NULL;
		}
	
	// delete temp file to avoid unwated continuing of interrupted trials
	file_remove_onepath("maps\\lightmap_backup.tga");
	file_remove_onepath("maps\\lightmap_backup.lmp");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void 		TerEdit_Lm_DoBlur()
{
	wait(3);
	
	if (terrain_entity.lightmap == NULL)
		return;
	
	if ( (proc_status(TerEdit_Lm_DoBlur)>1) || (proc_status(TerEdit_Lm_Blur)>0) )
		return;
		
	TerEdit_Lm_Blur(teredit_lm_passes, terrain_entity.lightmap, teredit_lm_treshold, teredit_lm_strength);
	wait_for(TerEdit_Lm_Blur);
}


void TerEdit_Lm_Blur(var passes, BMAP* canvas, var treshold, var strength)
{
	sys_marker("b01");
	
	set( map_loadpanel , SHOW);
	wait(3);
	
	var size_x = bmap_width(canvas);
	var size_y = bmap_height(canvas);
	
	VECTOR canvas_size;
	vec_set(canvas_size,vector(size_x, size_y, 0));
	
	var i;
	var px;
	var py;
	
	var 	format;
	var 	pixel;
	var 	pixelalpha;										// not used only dummy value holder	
	COLOR pixelcolor;
	vec_set(pixelcolor,vector(128,128,128));
	
	// own color and 8 neighbours
	VECTOR sample1;
	VECTOR sample2;
	VECTOR sample3;
	VECTOR sample4;
	VECTOR sample5;
	VECTOR sample6;
	VECTOR sample7;
	VECTOR sample8;
	VECTOR sample9; 
	
	VECTOR sampleX;
	vec_set(sampleX,vector(128,128,128));
	
	sys_marker(NULL);
	
	i=0;
	for (i=0;i<passes;i++)
		{
			for (py=0;py<canvas_size.y;py++)
				{					
					format = bmap_lock(canvas,0);  
					
//					draw_text( "PRESS 'B' TO BREAK LIGHTMAP CREATION!" , 150 , 100 , vector(255,255,255) );
//					if (key_b) break;
//					wait(1);
					
					for (px=0;px<canvas_size.x;px++)
						{
							sys_marker("b01");														
//							pixel=pixel_for_bmap(canvas,clamp((canvas_size.x-1)-px,0,canvas_size.x-1),clamp(py,0,canvas_size.x-1));
							pixel=pixel_for_bmap(canvas,px,py);
							pixel_to_vec(sample1,pixelalpha,format,pixel);
							
							if ( abs(128-sample1.x) < treshold)			// (sample1.x+sample1.y+sample1.z)/3 < 128-treshold - both +/- treshold
								{
									
									sys_marker(NULL);
									sys_marker("b02");
									if (px>0)
										{
				//							pixel=pixel_for_bmap(canvas,clamp((canvas_size.x-1)-px-1,0,canvas_size.x-1),clamp(py,0,canvas_size.x-1));
											pixel=pixel_for_bmap(canvas,px-1,py);
											pixel_to_vec(sample2,pixelalpha,format,pixel);
										}
									else
										{
											vec_set(sample2,sampleX);
										}
									
									sys_marker(NULL);
									sys_marker("b03");
									if ((px>0) && (py<canvas_size.y-2))
										{
				//							pixel=pixel_for_bmap(canvas,clamp((canvas_size.x-1)-px-1,0,canvas_size.x-1),clamp(py+1,0,canvas_size.x-1));
											pixel=pixel_for_bmap(canvas,px-1,py+1);
											pixel_to_vec(sample3,pixelalpha,format,pixel);
										}
									else
										{
											vec_set(sample3,sampleX);
										}
									
									sys_marker(NULL);
									sys_marker("b04");
									if (py<canvas_size.y-2)
										{
				//							pixel=pixel_for_bmap(canvas,clamp((canvas_size.x-1)-px,0,canvas_size.x-1),clamp(py+1,0,canvas_size.x-1));
											pixel=pixel_for_bmap(canvas,px,py+1);
											pixel_to_vec(sample4,pixelalpha,format,pixel);
										}
									else
										{
											vec_set(sample4,sampleX);
										}
									
									sys_marker(NULL);
									sys_marker("b05");
									if ((px<canvas_size.x-2) && (py<canvas_size.y-2))
										{
				//							pixel=pixel_for_bmap(canvas,clamp((canvas_size.x-1)-px+1,0,canvas_size.x-1),clamp(py+1,0,canvas_size.x-1));
											pixel=pixel_for_bmap(canvas,px+1,py+1);
											pixel_to_vec(sample5,pixelalpha,format,pixel);
										}
									else
										{
											vec_set(sample5,sampleX);
										}
									
									sys_marker(NULL);
									sys_marker("b06");
									if (px<canvas_size.x-2)
										{
				//							pixel=pixel_for_bmap(canvas,clamp((canvas_size.x-1)-px+1,0,canvas_size.x-1),clamp(py,0,canvas_size.x-1));
											pixel=pixel_for_bmap(canvas,px+1,py);
											pixel_to_vec(sample6,pixelalpha,format,pixel);
										}
									else
										{
											vec_set(sample6,sampleX);
										}
									
									sys_marker(NULL);
									sys_marker("b07");
									if ((px<canvas_size.x-2) && (py>0))
										{
				//							pixel=pixel_for_bmap(canvas,clamp((canvas_size.x-1)-px+1,0,canvas_size.x-1),clamp(py-1,0,canvas_size.x-1));
											pixel=pixel_for_bmap(canvas,px+1,py-1);
											pixel_to_vec(sample7,pixelalpha,format,pixel);
										}
									else
										{
											vec_set(sample7,sampleX);
										}
									
									sys_marker(NULL);
									sys_marker("b08");
									if (py>0)
										{
				//							pixel=pixel_for_bmap(canvas,clamp((canvas_size.x-1)-px,0,canvas_size.x-1),clamp(py-1,0,canvas_size.x-1));
											pixel=pixel_for_bmap(canvas,px,py-1);
											pixel_to_vec(sample8,pixelalpha,format,pixel);
										}
									else
										{
											vec_set(sample8,sampleX);
										}
									
									sys_marker(NULL);
									sys_marker("b09");
									if ((px>0) && (py>0))
										{
				//							pixel=pixel_for_bmap(canvas,clamp((canvas_size.x-1)-px-1,0,canvas_size.x-1),clamp(py-1,0,canvas_size.x-1));
											pixel=pixel_for_bmap(canvas,px-1,py-1);
											pixel_to_vec(sample9,pixelalpha,format,pixel);
										}
									else
										{
											vec_set(sample9,sampleX);
										}
										
									sys_marker(NULL);
									sys_marker("b10");
									// weighted average = ( own*7 , W*2 , NW , N*2 , NE , E*2 , SE , S*2 , SW ) / 19
									pixelcolor.red		= integer((sample1.x*strength + sample2.x*2 + sample3.x + sample4.x*2 + sample5.x + sample6.x*2 + sample7.x + sample8.x*2 + sample9.x)/(12+strength));
									pixelcolor.green	= integer((sample1.y*strength + sample2.y*2 + sample3.y + sample4.y*2 + sample5.y + sample6.y*2 + sample7.y + sample8.y*2 + sample9.y)/(12+strength));
									pixelcolor.blue	= integer((sample1.z*strength + sample2.z*2 + sample3.z + sample4.z*2 + sample5.z + sample6.z*2 + sample7.z  +sample8.z*2 + sample9.z)/(12+strength));
									
									sys_marker(NULL);
									sys_marker("b11");
									pixel=pixel_for_vec(pixelcolor,100,format);
		//							pixel_to_bmap(canvas,(canvas_size.x-1)-px,py,pixel);
									pixel_to_bmap(canvas,px,py,pixel);														
									sys_marker(NULL);
									
								}	// treshold check
						}			// for px
					
					bmap_unlock(canvas);
					
					Map_Progressbar(py/canvas_size.y*100);
					wait_for(Map_Progressbar);					
					
					draw_text( "PRESS 'B' TO BREAK BLURRING!" , 150 , 100 , vector(255,255,255) );
					if (key_b) break;
					
					wait(1); // without a wait after each line the loop could get too big if the shadow map is huge
					
				} 					// for py
		}							// for i
	
	reset(map_loadbar,SHOW); 
	reset( map_loadpanel , SHOW);
	wait(1);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Lm_Create_ShrubModels()
{
	you = ent_next(NULL); 											// get first entity
 	while (you) 														// repeat until there are no more entities
		{ 
			if (you.ENT_CATEGORY == (var)UIMODE_SHRUB)
				{
//					var spritesizex = you.scale_x * bmap_width( bmap_for_entity(you, 0) );
//					var spritesizey = you.scale_y * bmap_height( bmap_for_entity(you, 0) );										
					
					// get x2 or x3 info
					STRING* temp_str = str_create("");
					temp_str = str_for_entfile(NULL,you);
					
					int temp_pieces = 1;
					if (str_stri(temp_str,"x2.")>0)
						{
							temp_pieces = 2;
						}
					else if (str_stri(temp_str,"x3.")>0)
						{
							temp_pieces = 3;
						}
					
					// go as many object as many pieces of the sprite consists of: 1 or x2 or x3
					int i;
					for(i=0;i<temp_pieces;i++)
						{
							// create square model
							me = ent_create("graphics\\square512stand.mdl", you.x, NULL);				// stands facing towards x axis, as a sprite on creation
							while(!me) {wait(1);}					
							
							// can have a unique skin
							ent_cloneskin(my);																		// CLO ? - only if ent_morph is used with cloning
							
							// set category, required for fast deletion
							my.ENT_CATEGORY = (var)UIMODE_TEMPSHRUB;	
							
							// scale square model
							my.scale_x = you.scale_x / 2;
							my.scale_y = you.scale_x / 2;
							my.scale_z = you.scale_y / 2;
							
							// rotate if consists of more pieces
							my.pan = you.pan + 180/temp_pieces*i;
							
							// set unique texture get from shrub sprite
							ent_setskin(my, bmap_for_entity(you, 0), 1);
							c_setminmax(me);
						}
				}
			you = ent_next(you); 									// get next entity
		}	
	wait(1);
}


void		TerEdit_Lm_Remove_ShrubModels()
{
	wait(3);
	
	Remove_EntCats( UIMODE_TEMPSHRUB );
	wait_for(Remove_EntCats);
}


/////////////////////////////////////////////////////////////////////////////////////////////
















#endif