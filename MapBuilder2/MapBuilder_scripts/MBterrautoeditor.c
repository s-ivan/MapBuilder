
#ifndef MBterrautoeditor_c
#define MBterrautoeditor_c




void	Material_TerrhmpAutoTexPanel_Init()
{
	material_terrautotexpanel = pan_create( panel_image2 , 12 );
	set( material_terrautotexpanel , SHOW | SHADOW | OVERLAY );					
	material_terrautotexpanel.pos_x 	= ui_submenupanel3_x;					// screen_size.x-512;
	material_terrautotexpanel.pos_y 	= ui_submenupanel3_y;					// 0;			
	material_terrautotexpanel.red		= panel_font_r;
	material_terrautotexpanel.green	= panel_font_g;
	material_terrautotexpanel.blue	= panel_font_b;
	material_terrautotexpanel.event	= ui_various_panelmove;
	
	//string 1 - title
	pan_setstring(material_terrautotexpanel, 0,	15,10,	panel_font2,			str_create("AUTO TEXTURING SETTINGS") );
	
	// basecolor_tiling----------------------------------------------
	
	// slider 1 - 1..10
	pan_setdigits(material_terrautotexpanel, 0,		15,	 40,	"Base Color Tiling: %0.f",				panel_font,	1, 						basecolor_tiling);
	pan_setslider(material_terrautotexpanel, 0,		120+12,42,	horiz_slider_bmp,slider_knob_bmp,	1, 10,									basecolor_tiling); 
	// button 1 2
	pan_setbutton(material_terrautotexpanel, 0,	1, 	120,40,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	pan_setbutton(material_terrautotexpanel, 0,	1, 	240,40,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	
	// grass_height_border----------------------------------------------
	
	// slider 1 - 0..1000
	pan_setdigits(material_terrautotexpanel, 0,		15,	 70,	"Grass Border: %0.f",					panel_font,	1, 						grass_height_border);
	pan_setslider(material_terrautotexpanel, 0,		120+12,72,	horiz_slider_bmp,slider_knob_bmp,	0, 1000,									grass_height_border); 
	// button 3 4
	pan_setbutton(material_terrautotexpanel, 0,	1, 	120,70,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	pan_setbutton(material_terrautotexpanel, 0,	1, 	240,70,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	
	// grass_height_width----------------------------------------------
	
	// slider 1 - 0..500
	pan_setdigits(material_terrautotexpanel, 0,		15,	 90,	"Grass Transition: %0.f",				panel_font,	1, 						grass_height_width);
	pan_setslider(material_terrautotexpanel, 0,		120+12,92,	horiz_slider_bmp,slider_knob_bmp,	0, 500,									grass_height_width); 
	// button 5 6
	pan_setbutton(material_terrautotexpanel, 0,	1, 	120,90,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	pan_setbutton(material_terrautotexpanel, 0,	1, 	240,90,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	
	// shore_height_width----------------------------------------------
	
	// slider 1 - 0..100
	pan_setdigits(material_terrautotexpanel, 0,		15,	 120,	"Shore Depth/Height: %0.f",			panel_font,	1, 						shore_height_width);
	pan_setslider(material_terrautotexpanel, 0,		120+12,122,	horiz_slider_bmp,slider_knob_bmp,	0, 100,									shore_height_width); 
	// button 7 8
	pan_setbutton(material_terrautotexpanel, 0,	1, 	120,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	pan_setbutton(material_terrautotexpanel, 0,	1, 	240,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	
	// cliff_slope_min----------------------------------------------
	
	// slider 1 - 0..90
	pan_setdigits(material_terrautotexpanel, 0,		15,	 150,	"Cliff Min. Slope: %0.f",				panel_font,	1, 						cliff_slope_min);
	pan_setslider(material_terrautotexpanel, 0,		120+12,152,	horiz_slider_bmp,slider_knob_bmp,	0, 90,									cliff_slope_min); 
	// button 9 10
	pan_setbutton(material_terrautotexpanel, 0,	1, 	120,150,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	pan_setbutton(material_terrautotexpanel, 0,	1, 	240,150,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	
	// cliff_slope_width----------------------------------------------
	
	// slider 1 - 1..10
	pan_setdigits(material_terrautotexpanel, 0,		15,	 170,	"Cliff Transition: %0.f",				panel_font,	1, 						cliff_slope_width);
	pan_setslider(material_terrautotexpanel, 0,		120+12,172,	horiz_slider_bmp,slider_knob_bmp,	1, 10,									cliff_slope_width); 
	// button 11 12
	pan_setbutton(material_terrautotexpanel, 0,	1, 	120,170,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	pan_setbutton(material_terrautotexpanel, 0,	1, 	240,170,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_TerrhmpAutoTexPanel_Button, 	NULL, NULL); 
	
	//---------------------------------------------------
	// load save
	
	pan_setbutton(material_terrautotexpanel, 0,	1, 	50 ,480,		s_load_on_bmp,s_load_off_bmp,s_load_over_bmp,s_load_over_bmp,				FileMenu_LoadTerrAutoTex			,NULL,NULL); 	// calls FileMenu_...
	pan_setbutton(material_terrautotexpanel, 0,	1, 	150,480,		s_save_on_bmp,s_save_off_bmp,s_save_over_bmp,s_save_over_bmp,				FileMenu_SaveTerrAutoTex			,NULL,NULL); 	// calls FileMenu_...
	
	//---------------------------------------------------
}


void	Material_TerrhmpAutoTexPanel_Close()
{
	if (material_terrautotexpanel)
		{
			ptr_remove(material_terrautotexpanel);
			material_terrautotexpanel = NULL;
		}
}


void	Material_TerrhmpAutoTexPanel_Button(button_number)
{
	wait(3);
	
	//--------------------------------------------
	// basecolor_tiling
	if (button_number==(var)1)
		{
			basecolor_tiling = maxv( 1 , basecolor_tiling-1 );
		}
	else if (button_number==(var)2)
		{
			basecolor_tiling = minv( 10 , basecolor_tiling+1 );
		}
	//--------------------------------------------
	// grass_height_border
	else if (button_number==(var)3)
		{
			grass_height_border = maxv( 0 , grass_height_border-1 );
		}
	else if (button_number==(var)4)
		{
			grass_height_border = minv( 1000 , grass_height_border+1 );
		}
	//--------------------------------------------
	// grass_height_width
	else if (button_number==(var)5)
		{
			grass_height_width = maxv( 0 , grass_height_width-1 );
		}
	else if (button_number==(var)6)
		{
			grass_height_width = minv( 500 , grass_height_width+1 );
		}
	//--------------------------------------------
	// shore_height_width
	else if (button_number==(var)7)
		{
			shore_height_width = maxv( 0 , shore_height_width-1 );
		}
	else if (button_number==(var)8)
		{
			shore_height_width = minv( 100 , shore_height_width+1 );
		}
	//--------------------------------------------
	// cliff_slope_min
	else if (button_number==(var)9)
		{
			cliff_slope_min = maxv( 0 , cliff_slope_min-1 );
		}
	else if (button_number==(var)10)
		{
			cliff_slope_min = minv( 90 , cliff_slope_min+1 );
		}
	//--------------------------------------------
	// cliff_slope_width
	else if (button_number==(var)11)
		{
			cliff_slope_width = maxv( 1 , cliff_slope_width-1 );
		}
	else if (button_number==(var)12)
		{
			cliff_slope_width = minv( 10 , cliff_slope_width+1 );
		}
	//--------------------------------------------
}




#endif