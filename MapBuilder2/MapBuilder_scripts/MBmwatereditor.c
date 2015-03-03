#ifndef MBmwatereditor_c
#define MBmwatereditor_c


// mirror water
void Material_MWaterPanel_Init()
{
	material_mwaterpanel = pan_create( panel_image2 , 12 );
	set( material_mwaterpanel , SHOW | SHADOW | OVERLAY );					
	material_mwaterpanel.pos_x 	= ui_submenupanel3_x;					// screen_size.x-512;
	material_mwaterpanel.pos_y 	= ui_submenupanel3_y;					// 0;			
	material_mwaterpanel.red		= panel_font_r;
	material_mwaterpanel.green		= panel_font_g;
	material_mwaterpanel.blue		= panel_font_b;
	material_mwaterpanel.event		= ui_various_panelmove;
	
	//string 1 - title
	pan_setstring(material_mwaterpanel, 0,	15,10,	panel_font2,			str_create("WATER SETTINGS") );
	
	// color---------------------------------------------------------------
	
	// string 2
	pan_setstring(material_mwaterpanel, 0,	15,40,	panel_font2,			str_create("Color") );
		
	// entity red----------------------------------------------
	
	// slider 1 - 0..255
	pan_setdigits(material_mwaterpanel, 0,	15,70,	"Red: %0.f",				panel_font,	1, 		water_entity.red);
	pan_setslider(material_mwaterpanel, 0,	120+12,72,	horiz_slider_bmp,slider_knob_bmp,	0, 255,					water_entity.red); 
	// button 1 2
	pan_setbutton(material_mwaterpanel, 0,	1, 	120,70,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MWater_Button, 	NULL, NULL); 
	pan_setbutton(material_mwaterpanel, 0,	1, 	240,70,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MWater_Button, 	NULL, NULL); 
	
	// entity green----------------------------------------------
	
	// slider 2 - 0..255
	pan_setdigits(material_mwaterpanel, 0,	15,90,	"Green: %0.f",				panel_font,	1, 		water_entity.green);
	pan_setslider(material_mwaterpanel, 0,	120+12,92,	horiz_slider_bmp,slider_knob_bmp,	0, 255,					water_entity.green); 
	// button 3 4
	pan_setbutton(material_mwaterpanel, 0,	1, 	120,90,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MWater_Button, 	NULL, NULL); 
	pan_setbutton(material_mwaterpanel, 0,	1, 	240,90,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MWater_Button, 	NULL, NULL); 
	
	// entity blue----------------------------------------------
	
	// slider 3 - 0..255
	pan_setdigits(material_mwaterpanel, 0,	15,110,	"Blue: %0.f",				panel_font,	1, 		water_entity.blue);
	pan_setslider(material_mwaterpanel, 0,	120+12,112,	horiz_slider_bmp,slider_knob_bmp,	0, 255,					water_entity.blue); 
	// button 5 6
	pan_setbutton(material_mwaterpanel, 0,	1, 	120,110,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MWater_Button, 	NULL, NULL); 
	pan_setbutton(material_mwaterpanel, 0,	1, 	240,110,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MWater_Button, 	NULL, NULL); 
	
	// entity alpha----------------------------------------------
	
	// slider 4 - 0..100
	pan_setdigits(material_mwaterpanel, 0,	15,130,	"Alpha: %0.f",				panel_font,	1, 		water_entity.alpha);
	pan_setslider(material_mwaterpanel, 0,	120+12,132,	horiz_slider_bmp,slider_knob_bmp,	0, 100,					water_entity.alpha); 
	// button 7 8
	pan_setbutton(material_mwaterpanel, 0,	1, 	120,130,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MWater_Button, 	NULL, NULL); 
	pan_setbutton(material_mwaterpanel, 0,	1, 	240,130,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MWater_Button, 	NULL, NULL); 
	
	// waves---------------------------------------------------------------
	
	// string 3
	pan_setstring(material_mwaterpanel, 0,	15,160,	panel_font2,			str_create("Waves") );
	
	// speed x----------------------------------------------
	
	// slider 5 - 0..255
	pan_setdigits(material_mwaterpanel, 0,	15,190,	"Speed X: %0.f",				panel_font,	1, 		mwater_speedx);
	pan_setslider(material_mwaterpanel, 0,	120+12,192,	horiz_slider_bmp,slider_knob_bmp,	0, 255,					mwater_speedx); 
	// button 9 10
	pan_setbutton(material_mwaterpanel, 0,	1, 	120,190,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MWater_Button, 	NULL, NULL); 
	pan_setbutton(material_mwaterpanel, 0,	1, 	240,190,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MWater_Button, 	NULL, NULL); 
	
	// speed y----------------------------------------------
	
	// slider 6 - 0..255
	pan_setdigits(material_mwaterpanel, 0,	15,210,	"Speed Y: %0.f",				panel_font,	1, 		mwater_speedy);
	pan_setslider(material_mwaterpanel, 0,	120+12,212,	horiz_slider_bmp,slider_knob_bmp,	0, 255,					mwater_speedy); 
	// button 11 12
	pan_setbutton(material_mwaterpanel, 0,	1, 	120,210,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MWater_Button, 	NULL, NULL); 
	pan_setbutton(material_mwaterpanel, 0,	1, 	240,210,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MWater_Button, 	NULL, NULL); 
	
	// ripple----------------------------------------------
	
	// slider 7 - 1..255
	pan_setdigits(material_mwaterpanel, 0,	15,240,	"Ripple: %0.f",				panel_font,	1, 		mwater_ripple);
	pan_setslider(material_mwaterpanel, 0,	120+12,242,	horiz_slider_bmp,slider_knob_bmp,	1, 255,					mwater_ripple); 
	// button 13 14
	pan_setbutton(material_mwaterpanel, 0,	1, 	120,240,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MWater_Button, 	NULL, NULL); 
	pan_setbutton(material_mwaterpanel, 0,	1, 	240,240,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MWater_Button, 	NULL, NULL); 
	
	// scale----------------------------------------------
	
	// slider 8 - 1..255
	pan_setdigits(material_mwaterpanel, 0,	15,260,	"Scale: %0.f",				panel_font,	1, 		mwater_scale);
	pan_setslider(material_mwaterpanel, 0,	120+12,262,	horiz_slider_bmp,slider_knob_bmp,	1, 255,					mwater_scale); 
	// button 15 16
	pan_setbutton(material_mwaterpanel, 0,	1, 	120,260,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MWater_Button, 	NULL, NULL); 
	pan_setbutton(material_mwaterpanel, 0,	1, 	240,260,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MWater_Button, 	NULL, NULL); 
	
	//---------------------------------------------------
	// load save
	
	pan_setbutton(material_mwaterpanel, 0,	1, 	50 ,480,		s_load_on_bmp,s_load_off_bmp,s_load_over_bmp,s_load_over_bmp,				FileMenu_LoadMWater			,NULL,NULL); 	
	pan_setbutton(material_mwaterpanel, 0,	1, 	150,480,		s_save_on_bmp,s_save_off_bmp,s_save_over_bmp,s_save_over_bmp,				FileMenu_SaveMWater			,NULL,NULL); 		
	
	////////////////////////////////////////////////////////////////////
	
	while(material_mwaterpanel)
		{
//			water_entity.skill41 = floatv(mwater_speedx);		// 10 speed x
//			water_entity.skill42 = floatv(mwater_speedy);		// 25 speed	y
//			water_entity.skill43 = floatv(mwater_ripple);		//	25 ripple - waves
//			water_entity.skill44 = floatv(mwater_scale);			// 100 scale
			
			if (water_entity.alpha > (var)99)
				{
					reset(water_entity, TRANSLUCENT);	
				}
			else
				{
					set(water_entity, TRANSLUCENT);	
				}
			
			wait(1);
		}
}


void	Material_MWaterPanel_Close()
{
	if (material_mwaterpanel != NULL)
		{
			ptr_remove(material_mwaterpanel);
			material_mwaterpanel = NULL;
		}
}


void	Material_MWater_Button(button_number)
{
	wait(3);
	
	//--------------------------------------------
	// water_entity.red
	if (button_number==(var)1)
		{
			water_entity.red = maxv( 0 , water_entity.red-1 );
		}
	else if (button_number==(var)2)
		{
			water_entity.red = minv( 255 , water_entity.red+1 );
		}
	//--------------------------------------------
	// water_entity.green
	else if (button_number==(var)3)
		{
			water_entity.green = maxv( 0 , water_entity.green-1 );
		}
	else if (button_number==(var)4)
		{
			water_entity.green = minv( 255 , water_entity.green+1 );
		}
	//--------------------------------------------
	// water_entity.blue
	else if (button_number==(var)5)
		{
			water_entity.blue = maxv( 0 , water_entity.blue-1 );
		}
	else if (button_number==(var)6)
		{
			water_entity.blue = minv( 255 , water_entity.blue+1 );
		}
	//--------------------------------------------
	// water_entity.alpha
	else if (button_number==(var)7)
		{
			water_entity.alpha = maxv( 0 , water_entity.alpha-1 );
		}
	else if (button_number==(var)8)
		{
			water_entity.alpha = minv( 100 , water_entity.alpha+1 );
		}
	//--------------------------------------------
	// mwater_speedx
	else if (button_number==(var)9)
		{
			mwater_speedx = maxv( 0 , mwater_speedx-1 );
		}
	else if (button_number==(var)10)
		{
			mwater_speedx = minv( 255 , mwater_speedx+1 );
		}
	//--------------------------------------------
	// mwater_speedy
	else if (button_number==(var)11)
		{
			mwater_speedy = maxv( 0 , mwater_speedy-1 );
		}
	else if (button_number==(var)12)
		{
			mwater_speedy = minv( 255 , mwater_speedy+1 );
		}
	//--------------------------------------------
	// mwater_ripple
	else if (button_number==(var)13)
		{
			mwater_ripple = maxv( 1 , mwater_ripple-1 );
		}
	else if (button_number==(var)14)
		{
			mwater_ripple = minv( 255 , mwater_ripple+1 );
		}
	//--------------------------------------------
	// mwater_scale
	else if (button_number==(var)15)
		{
			mwater_scale = maxv( 1 , mwater_scale-1 );
		}
	else if (button_number==(var)16)
		{
			mwater_scale = minv( 255 , mwater_scale+1 );
		}
	
}


////////////////////////////////////////////////////////////////////////////////////////


#endif