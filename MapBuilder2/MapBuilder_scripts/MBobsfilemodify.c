
#ifndef MBobsfilemodify_c
#define MBobsfilemodify_c


/////////////////////////////////////////
// code

void		ObswModify_Init(var tsize, var tilex, var tiley, var ttype, var tshadow, var tcoll, char* tfilename)
{
	obsw_origmenu 		= UIMode;	// to check whether placement panel was changed or not
	
	// values to modify
	obsw_tilesize		= tsize;
	
	obsw_areax 			= tilex;
	obsw_areay 			= tiley;
	
	obsw_passability 	= ttype;			
	obsw_shadow 		= tshadow;			
	obsw_collision 	= tcoll;
	
	str_cpy(obsw_filename,tfilename);
//	obsw_filename		= str_create(tfilename);
	
	// new sub-panel
	ObswModify_Panel_Init();
	
	// uimode checking loop for auto-closing sub-panel when uimode changes
	while(obsw_origmenu==(var)UIMode)
		{			
			wait(1);
		}
		
	ObswModify_Close();
}

void		ObswModify_Close()
{
	wait(3);
	while(mouse_left)	{wait(1);}
	
//	ptr_remove(obsw_filename);
//	obsw_filename = NULL;
	
//	if (obsw_panel!=NULL)
	ptr_remove(obsw_panel);	
	obsw_panel = NULL;
}

void		ObswModify_OK()
{
	wait(3);
	// remove panel
	ObswModify_Close();
	//-----------------------------------------------------------------------
	// get filename and set extension
	STRING* temp_str = str_create("");		
	str_cpy( temp_str , obsw_filename );
	str_trunc( temp_str , 4 ); 
	if (obsw_origmenu==(var)UIMODE_OBSTACLE)
		{
			str_cat( temp_str , ".obs" );	
		}
	else if (obsw_origmenu==(var)UIMODE_MAPENT)
		{
			str_cat( temp_str , ".obs" );	
		}
	else if (obsw_origmenu==(var)UIMODE_BUILDING)
		{
			str_cat( temp_str , ".obs" );	
		}
	//-----------------------------------------------------------------------
	// write file
	var file_handle = file_open_write( temp_str );
	
	file_str_write( 		file_handle , "// reference tile size // 32 by default // min 16 // max 100" );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);						
	str_for_int( 			temp_str 	, obsw_tilesize );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
		
	file_str_write( 		file_handle , "// area width in tiles" );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);							
	str_for_int( 			temp_str 	, obsw_areax );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
		
	file_str_write( 		file_handle , "// area height in tiles" );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);							
	str_for_int( 			temp_str 	, obsw_areay );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	file_str_write( 		file_handle , "// shadow type // 0 no shadow // 1 dynamic shadow // 2 decal shadow // 3 model shadow" );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);							
	str_for_int( 			temp_str 	, obsw_shadow );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	file_str_write( 		file_handle , "// tile type // 0 no access // 1 terrain default // 2 building // 3 forest" );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);							
	str_for_int( 			temp_str 	, obsw_passability );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
	
	file_str_write( 		file_handle , "// collision type // 0 passable // 1 polygon // 2 tile area based" );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);							
	str_for_int( 			temp_str 	, obsw_collision );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
				
	file_close(file_handle);
	//---------------------------------------------------------------	
	// reload full map (or entity map would be nicer but might increase nexus too high)
	FileMenu_Reload_Map();	
	
	ptr_remove(temp_str);	
}

void		ObswModify_Panel_Init()
{
	if (obsw_panel) return;
	
	int i;
//	STRING* temp_string = str_create("");
	
	obsw_panel = pan_create( panel_image , 13 );
	set( obsw_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	obsw_panel.pos_x 		= ui_submenupanel2_x;						// screen_size.x-256;
	obsw_panel.pos_y 		= ui_submenupanel2_y;						// 0;
	obsw_panel.red			= panel_font_r;
	obsw_panel.green		= panel_font_g;
	obsw_panel.blue		= panel_font_b;
	obsw_panel.event		= ui_small_panelmove;
	
	// string 1
	pan_setstring(obsw_panel, 0,	15,10,	panel_font2,			str_create("OBJECT PROPERTY MODIFICATION") );
	
	// button 1 - cancel
	pan_setbutton(obsw_panel, 0,	1, 	128+24,224,		s_cancel_on_bmp, s_cancel_off_bmp, s_cancel_over_bmp, s_cancel_over_bmp,		ObswModify_Close, 	NULL, NULL); 
	
	// button 2 - ok
	pan_setbutton(obsw_panel, 0,	1, 	48,224,			s_ok_on_bmp, s_ok_off_bmp, s_ok_over_bmp, s_ok_over_bmp,								ObswModify_OK, 		NULL, NULL); 	 
	
	// digit 1, button 3-4, slider 1 - tilesize
	pan_setdigits(obsw_panel, 0,	15,40,	"Tile Size: %0.f",		panel_font,	1, 				obsw_tilesize);
	pan_setslider(obsw_panel, 0,	90+12,42,	horiz_slider_bmp,slider_knob_bmp,	16,100,		obsw_tilesize);
	pan_setbutton(obsw_panel, 0,	1, 	90,40,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ObswModify_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obsw_panel, 0,	1, 	210,40,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ObswModify_Panel_Button, 		NULL, NULL); 
	
	// digit 2, button 5-6 - areax
	pan_setdigits(obsw_panel, 0,	15,65,	"Area Width:                  %0.f",		panel_font,	1, 				obsw_areax);
//	pan_setslider(obsw_panel, 0,	90+12,67,	horiz_slider_bmp,slider_knob_bmp,	1,9,		obsw_areax);
	pan_setbutton(obsw_panel, 0,	1, 	90,65,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ObswModify_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obsw_panel, 0,	1, 	150,65,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ObswModify_Panel_Button, 		NULL, NULL); 
	
	// digit 3, button 7-8 - areay
	pan_setdigits(obsw_panel, 0,	15,90,	"Area Height:                 %0.f",		panel_font,	1, 				obsw_areay);
//	pan_setslider(obsw_panel, 0,	90+12,92,	horiz_slider_bmp,slider_knob_bmp,	1,9,		obsw_areay);
	pan_setbutton(obsw_panel, 0,	1, 	90,90,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ObswModify_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obsw_panel, 0,	1, 	150,90,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ObswModify_Panel_Button, 		NULL, NULL); 
	
	// digit 4, button 9-10 - passability
	pan_setdigits(obsw_panel, 0,	15,115,	"Tile Type:                   %0.f",		panel_font,	1, 				obsw_passability);
//	pan_setslider(obsw_panel, 0,	90+12,117,	horiz_slider_bmp,slider_knob_bmp,	0,3,		obsw_passability);
	pan_setbutton(obsw_panel, 0,	1, 	90,115,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ObswModify_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obsw_panel, 0,	1, 	150,115,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ObswModify_Panel_Button, 		NULL, NULL); 
	
	// digit 5, button 11-12 - shadow
	pan_setdigits(obsw_panel, 0,	15,140,	"Shadow:                      %0.f",		panel_font,	1, 				obsw_shadow);
//	pan_setslider(obsw_panel, 0,	90+12,142,	horiz_slider_bmp,slider_knob_bmp,	0,3,		obsw_shadow);
	pan_setbutton(obsw_panel, 0,	1, 	90,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ObswModify_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obsw_panel, 0,	1, 	150,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ObswModify_Panel_Button, 		NULL, NULL); 
	
	// digit 6, button 13-14 - collision
	pan_setdigits(obsw_panel, 0,	15,165,	"Collision:                   %0.f",		panel_font,	1, 				obsw_collision);
//	pan_setslider(obsw_panel, 0,	90+12,167,	horiz_slider_bmp,slider_knob_bmp,	0,2,		obsw_collision);
	pan_setbutton(obsw_panel, 0,	1, 	90,165,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ObswModify_Panel_Button, 		NULL, NULL); 
	pan_setbutton(obsw_panel, 0,	1, 	150,165,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ObswModify_Panel_Button, 		NULL, NULL); 
	
	// string 2
	pan_setstring(obsw_panel, 0,	15,190,	panel_font,			str_create(obsw_filename) );	
	
	// string 3 4 5 6 7 8
		
	//... short description of actual parameters next to them
	
}


void		ObswModify_Panel_Button(var button_number)
{
	// obsw_tilesize
	if (button_number==(var)3)
		{
			obsw_tilesize = maxv( 16 , --obsw_tilesize );
		}
	else if (button_number==(var)4)
		{
			obsw_tilesize = minv( 100 , ++obsw_tilesize );
		}
	// obsw_areax
	else if (button_number==(var)5)
		{
			obsw_areax -= 1;
			obsw_areax = maxv( 1 , obsw_areax );
		}
	else if (button_number==(var)6)
		{
			obsw_areax += 1;
			obsw_areax = minv( 19 , obsw_areax );
		}
	// obsw_areay
	else if (button_number==(var)7)
		{
			obsw_areay -= 1;
			obsw_areay = maxv( 1 , obsw_areay );
		}
	else if (button_number==(var)8)
		{
			obsw_areay += 1;
			obsw_areay = minv( 19 , obsw_areay );
		}
	// obsw_passability
	else if (button_number==(var)9)
		{
			obsw_passability = maxv( 0 , --obsw_passability );
		}
	else if (button_number==(var)10)
		{
			obsw_passability = minv( 3 , ++obsw_passability );
		}
	// obsw_shadow
	else if (button_number==(var)11)
		{
			obsw_shadow = maxv( 0 , --obsw_shadow );
		}
	else if (button_number==(var)12)
		{
			obsw_shadow = minv( 3 , ++obsw_shadow );
		}
	// obsw_collision
	else if (button_number==(var)13)
		{
			obsw_collision = maxv( 0 , --obsw_collision );
		}
	else if (button_number==(var)14)
		{
			obsw_collision = minv( 2 , ++obsw_collision );
		}
}


#endif