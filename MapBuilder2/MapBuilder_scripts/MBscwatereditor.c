#ifndef MBscwatereditor_c
#define MBscwatereditor_c


void Material_ScWaterPanel12_Init()
{	
	material_scwaterpanel1 = pan_create( panel_image2 , 12 );
	set( material_scwaterpanel1 , SHOW | SHADOW | OVERLAY );					
	material_scwaterpanel1.pos_x 	= ui_submenupanel3_x - 256;			// screen_size.x-512-256;
	material_scwaterpanel1.pos_y 	= ui_submenupanel3_y;					// 0;			
	material_scwaterpanel1.red		= panel_font_r;
	material_scwaterpanel1.green	= panel_font_g;
	material_scwaterpanel1.blue	= panel_font_b;
	material_scwaterpanel1.event	= ui_various_panelmove;
	
	//string 1 - title
	pan_setstring(material_scwaterpanel1, 0,	15,10,	panel_font2,			str_create("WATER SETTINGS") );
	
	// reflection--------------------------------------------------
	
	// string 2
	pan_setstring(material_scwaterpanel1, 0,	15,40,	panel_font2,			str_create("Reflection") );
	
	// reflection red----------------------------------------------
	
	// slider 1 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,70,	"Red: %0.2f",				panel_font,	1, 				scwater_reflectcolor[0]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,72,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_reflectcolor[0]); 
	// button 1 2
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,70,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,70,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// reflection green----------------------------------------------
	
	// slider 2 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,90,	"Green: %0.2f",				panel_font,	1, 			scwater_reflectcolor[1]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,92,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_reflectcolor[1]); 
	// button 3 4
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,90,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,90,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// reflection blue----------------------------------------------
	
	// slider 3 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,110,	"Blue: %0.2f",				panel_font,	1, 				scwater_reflectcolor[2]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,112,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_reflectcolor[2]); 
	// button 5 6
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,110,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,110,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// reflection alpha----------------------------------------------
	
	// slider 4 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,130,	"Alpha: %0.2f",				panel_font,	1, 			scwater_reflectcolor[3]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,132,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_reflectcolor[3]); 
	// button 7 8
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,130,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,130,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// reflection distance----------------------------------------------
	
	// slider 5 - 1..10
	pan_setdigits(material_scwaterpanel1, 0,	15,150,	"Distance: %0.1f",				panel_font,	1, 		scwater_reflectiondistance);
	pan_setslider(material_scwaterpanel1, 0,	120+12,152,	horiz_slider_bmp,slider_knob_bmp,	1, 10,		scwater_reflectiondistance); 
	// button 9 10
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,150,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,150,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// reflection strength----------------------------------------------
	
	// slider 6 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,170,	"Strength: %0.2f",				panel_font,	1, 		scwater_reflectionstrength);
	pan_setslider(material_scwaterpanel1, 0,	120+12,172,	horiz_slider_bmp,slider_knob_bmp,	0, 0.5,		scwater_reflectionstrength); 
	// button 11 12
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,170,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,170,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// refraction-----------------------------------------------------
	
	// string 3
	pan_setstring(material_scwaterpanel1, 0,	15,200,	panel_font2,			str_create("Refraction") );
	
	// refraction red----------------------------------------------
	
	// slider 7 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,230,	"Red: %0.2f",				panel_font,	1, 				scwater_refractcolor[0]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,232,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_refractcolor[0]); 
	// button 13 14
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,230,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,230,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// refraction green----------------------------------------------
	
	// slider 8 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,250,	"Green: %0.2f",				panel_font,	1, 			scwater_refractcolor[1]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,252,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_refractcolor[1]); 
	// button 15 16
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,250,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,250,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// refraction blue----------------------------------------------
	
	// slider 9 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,270,	"Blue: %0.2f",				panel_font,	1, 				scwater_refractcolor[2]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,272,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_refractcolor[2]); 
	// button 17 18
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,270,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,270,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// refraction alpha----------------------------------------------
	
	// slider 10 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,290,	"Alpha: %0.2f",				panel_font,	1, 			scwater_refractcolor[3]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,292,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_refractcolor[3]); 
	// button 19 20
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,290,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,290,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// refraction strength----------------------------------------------
	
	// slider 12 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,310,	"Strength: %0.2f",				panel_font,	1, 		scwater_refractionstrength);
	pan_setslider(material_scwaterpanel1, 0,	120+12,312,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_refractionstrength); 
	// button 21 22
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,310,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,310,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// shore---------------------------------------------------------------
	
	// string 4
	pan_setstring(material_scwaterpanel1, 0,	15,340,	panel_font2,			str_create("Shore") );
		
	// shore red----------------------------------------------
	
	// slider 13 - 0.5..1.5
	pan_setdigits(material_scwaterpanel1, 0,	15,370,	"Red: %0.2f",				panel_font,	1, 				scwater_shorecolor[0]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,372,	horiz_slider_bmp,slider_knob_bmp,	0.5, 1.5,	scwater_shorecolor[0]); 
	// button 23 24
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,370,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,370,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// shore green----------------------------------------------
	
	// slider 14 - 0.5..1.5
	pan_setdigits(material_scwaterpanel1, 0,	15,390,	"Green: %0.2f",				panel_font,	1, 			scwater_shorecolor[1]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,392,	horiz_slider_bmp,slider_knob_bmp,	0.5, 1.5,	scwater_shorecolor[1]); 
	// button 25 26
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,390,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,390,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// shore blue----------------------------------------------
	
	// slider 15 - 0.5..1.5
	pan_setdigits(material_scwaterpanel1, 0,	15,410,	"Blue: %0.2f",				panel_font,	1, 				scwater_shorecolor[2]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,412,	horiz_slider_bmp,slider_knob_bmp,	0.5, 1.5,	scwater_shorecolor[2]); 
	// button 27 28
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,410,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,410,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// shore alpha----------------------------------------------
	
	// slider 16 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,430,	"Alpha: %0.2f",				panel_font,	1, 			scwater_shorecolor[3]);
	pan_setslider(material_scwaterpanel1, 0,	120+12,432,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_shorecolor[3]); 
	// button 29 30
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,430,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,430,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// depth------------------------------------------------------------
	// slider 17 - 0..320
	pan_setdigits(material_scwaterpanel1, 0,	15,450,	"Depth: %0.f",				panel_font,	1, 				scwater_waterdepth);
	pan_setslider(material_scwaterpanel1, 0,	120+12,452,	horiz_slider_bmp,slider_knob_bmp,	0, 320,		scwater_waterdepth); 
	// button 31 32
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,450,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,450,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
	
	// global transparency----------------------------------------------
	// slider 18 - 0..1
	pan_setdigits(material_scwaterpanel1, 0,	15,480,	"Transparency: %0.2f",					panel_font,	1, scwater_globaltransparency);
	pan_setslider(material_scwaterpanel1, 0,	120+12,482,	horiz_slider_bmp,slider_knob_bmp,	0, 1,			scwater_globaltransparency); 
	// button 33 34
	pan_setbutton(material_scwaterpanel1, 0,	1, 	120,480,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button1, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel1, 0,	1, 	240,480,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button1, 	NULL, NULL); 
		
	//---------------------------------------------------------------------
	// save load buttons
	
	pan_setbutton(material_scwaterpanel1, 0,	1, 	130,15,		s_load_on_bmp,s_load_off_bmp,s_load_over_bmp,s_load_over_bmp,				FileMenu_LoadScWater			,NULL,NULL); 	
	pan_setbutton(material_scwaterpanel1, 0,	1, 	190,15,		s_save_on_bmp,s_save_off_bmp,s_save_over_bmp,s_save_over_bmp,				FileMenu_SaveScWater			,NULL,NULL); 	
		
	////////////////////////////////////////////////////////////////////
	
	material_scwaterpanel2 = pan_create( panel_image2 , 12 );
	set( material_scwaterpanel2 , SHOW | SHADOW | OVERLAY );					
	material_scwaterpanel2.pos_x 	= ui_submenupanel3_x;					// screen_size.x-512;
	material_scwaterpanel2.pos_y 	= ui_submenupanel3_y;					// 0;			
	material_scwaterpanel2.red		= panel_font_r;
	material_scwaterpanel2.green	= panel_font_g;
	material_scwaterpanel2.blue	= panel_font_b;
	material_scwaterpanel2.event	= ui_various_panelmove;
	
	//string 1 - title - removed
	pan_setstring(material_scwaterpanel2, 0,	15,10,	panel_font2,			str_create(" ") );
	
	// wave---------------------------------------------------------------
	
	// string 2
	pan_setstring(material_scwaterpanel2, 0,	15,10,	panel_font2,			str_create("Waves") );
		
	// speed---------------------------------------------------------
	// slider 1 - 5000..30000
	pan_setdigits(material_scwaterpanel2, 0,	15,40,	"Inv. Speed: %0.f",				panel_font,	1, 			scwater_wavespeed);
	pan_setslider(material_scwaterpanel2, 0,	120+12,42,	horiz_slider_bmp,slider_knob_bmp,	5000, 30000,	scwater_wavespeed); 
	// button 1 2
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,40,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,40,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// height---------------------------------------------------------
	// slider 2 - 0..320
	pan_setdigits(material_scwaterpanel2, 0,	15,60,	"Height: %0.f",				panel_font,	1, 				scwater_displacement);
	pan_setslider(material_scwaterpanel2, 0,	120+12,62,	horiz_slider_bmp,slider_knob_bmp,	0, 320,			scwater_displacement); 
	// button 3 4
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,60,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,60,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// intensity---------------------------------------------------------
	// slider 3 - 0..0.1
	pan_setdigits(material_scwaterpanel2, 0,	15,80,	"Intensity: %0.3f",				panel_font,	1, 			scwater_speed);
	pan_setslider(material_scwaterpanel2, 0,	120+12,82,	horiz_slider_bmp,slider_knob_bmp,	0, 0.1,			scwater_speed); 
	// button 5 6
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// turbulence---------------------------------------------------------
	// slider 4 - 0..1
	pan_setdigits(material_scwaterpanel2, 0,	15,100,	"Turbulence: %0.2f",						panel_font,	1, 	scwater_turbdensity);
	pan_setslider(material_scwaterpanel2, 0,	120+12,102,	horiz_slider_bmp,slider_knob_bmp,	0, 1,				scwater_turbdensity); 
	// button 7 8
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,100,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,100,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// foam--------------------------------------------------------------------
	
	// string 3
	pan_setstring(material_scwaterpanel2, 0,	15,130,	panel_font2,			str_create("Foam") );
	
	// deep limit---------------------------------------------------------
	// slider 5 - 1..1.5
	pan_setdigits(material_scwaterpanel2, 0,	15,240,	"Deep Limit: %0.2f",						panel_font,	1, 	scwater_foamwave_deep);
	pan_setslider(material_scwaterpanel2, 0,	120+12,242,	horiz_slider_bmp,slider_knob_bmp,	0, 1,				scwater_foamwave_deep); 
	// button 9 10
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,240,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,240,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// shore limit-----------------------------------------------------
	// slider 6 - 1..10
	pan_setdigits(material_scwaterpanel2, 0,	15,220,	"Shore Limit: %0.2f",					panel_font,	1, 	scwater_foamwave_shore);
	pan_setslider(material_scwaterpanel2, 0,	120+12,222,	horiz_slider_bmp,slider_knob_bmp,	0, 1,				scwater_foamwave_shore); 
	// button 11 12
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,220,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,220,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// tiling---------------------------------------------------------
	// slider 7 - 1..50
	pan_setdigits(material_scwaterpanel2, 0,	15,160,	"Tiling: %0.f",							panel_font,	1, 	scwater_foamtiling);
	pan_setslider(material_scwaterpanel2, 0,	120+12,162,	horiz_slider_bmp,slider_knob_bmp,	1, 100,			scwater_foamtiling); 
	// button 13 14
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// distortion---------------------------------------------------------
	// slider 8 - 0..0.50
	pan_setdigits(material_scwaterpanel2, 0,	15,180,	"Distortion: %0.3f",						panel_font,	1, 	scwater_foamdistortion);
	pan_setslider(material_scwaterpanel2, 0,	120+12,182,	horiz_slider_bmp,slider_knob_bmp,	0, 0.5,			scwater_foamdistortion); 
	// button 15 16
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,180,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,180,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// speed--------------------------------------------------------- increased interval because of engine bug/limitation
	// slider 9 - 0.001..0.01 (too small interval - bug !)
	pan_setdigits(material_scwaterpanel2, 0,	15,200,	"Speed: %0.3f",							panel_font,	1, 	scwater_foamtranslation);
	pan_setslider(material_scwaterpanel2, 0,	120+12,202,	horiz_slider_bmp,slider_knob_bmp,	0, 10,			scwater_foamtranslation); 
	// button 17 18
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// shore strength---------------------------------------------------------
	// slider 10 - 0..1
	pan_setdigits(material_scwaterpanel2, 0,	15,260,	"Shore Strength: %0.2f",				panel_font,	1, 	scwater_shorefoamscale);
	pan_setslider(material_scwaterpanel2, 0,	120+12,262,	horiz_slider_bmp,slider_knob_bmp,	0, 1,				scwater_shorefoamscale); 
	// button 19 20
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,260,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,260,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// deep strength---------------------------------------------------------
	// slider 11 - 0..1
	pan_setdigits(material_scwaterpanel2, 0,	15,280,	"Deep Strength: %0.2f",					panel_font,	1, 	scwater_foamscale);
	pan_setslider(material_scwaterpanel2, 0,	120+12,282,	horiz_slider_bmp,slider_knob_bmp,	0, 1,				scwater_foamscale); 
	// button 21 22
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,280,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,280,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 	
	
	// sun-------------------------------------------------------------------
	
	// string 4
	pan_setstring(material_scwaterpanel2, 0,	15,310,	panel_font2,			str_create("Sun") );
	
	// sun strength---------------------------------------------------------
	// slider 12 - 0..10
	pan_setdigits(material_scwaterpanel2, 0,	15,340,	"Strength: %0.1f",						panel_font,	1, 	scwater_sunlightstrength);
	pan_setslider(material_scwaterpanel2, 0,	120+12,342,	horiz_slider_bmp,slider_knob_bmp,	0, 10,			scwater_sunlightstrength); 
	// button 23 24
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,340,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,340,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 	
	
	// sun area---------------------------------------------------------
	// slider 13 - 10..1000
	pan_setdigits(material_scwaterpanel2, 0,	15,360,	"Area: %0.f",								panel_font,	1, 	scwater_sunlightarea);
	pan_setslider(material_scwaterpanel2, 0,	120+12,362,	horiz_slider_bmp,slider_knob_bmp,	10, 1000,		scwater_sunlightarea); 
	// button 25 26
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,360,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,360,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 	
	
	// global----------------------------------------------------------
	
	// string 5
	pan_setstring(material_scwaterpanel2, 0,	15,390,	panel_font2,			str_create("Ripples") );
	
	// bump strength----------------------------------------------
	// slider 14 - 0..1
	pan_setdigits(material_scwaterpanel2, 0,	15,420,	"Bump Strength: %0.2f",					panel_font,	1, 	scwater_bumpstrength);
	pan_setslider(material_scwaterpanel2, 0,	120+12,422,	horiz_slider_bmp,slider_knob_bmp,	0, 50,			scwater_bumpstrength); 
	// button 27 28
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,420,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,420,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
		
	// bump scale-------------------------------------------------------
	// slider 15 - 0..50
	pan_setdigits(material_scwaterpanel2, 0,	15,440,	"Bump Scale: %0.1f",						panel_font,	1, 	scwater_bumptexscale);
	pan_setslider(material_scwaterpanel2, 0,	120+12,442,	horiz_slider_bmp,slider_knob_bmp,	0, 100,			scwater_bumptexscale); 
	// button 29 30
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,440,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,440,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	// scwater_2ndwave_speed------------------------------------
	// slider 16 - 0.5..2.0
	pan_setdigits(material_scwaterpanel2, 0,	15,460,	"2nd Wave Speed: %0.2f",				panel_font,	1, 	scwater_2ndwave_speed);
	pan_setslider(material_scwaterpanel2, 0,	120+12,462,	horiz_slider_bmp,slider_knob_bmp,	-2, 2,			scwater_2ndwave_speed); 
	// button 31 32
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,460,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,460,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
		
	// scwater_2ndwave_scale------------------------------------
	// slider 17 - 0.5..2.0
	pan_setdigits(material_scwaterpanel2, 0,	15,480,	"2nd Wave Scale: %0.2f",				panel_font,	1, 	scwater_2ndwave_scale);
	pan_setslider(material_scwaterpanel2, 0,	120+12,482,	horiz_slider_bmp,slider_knob_bmp,	0, 5,				scwater_2ndwave_scale); 
	// button 33 34
	pan_setbutton(material_scwaterpanel2, 0,	1, 	120,480,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_ScWater_Button2, 	NULL, NULL); 
	pan_setbutton(material_scwaterpanel2, 0,	1, 	240,480,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_ScWater_Button2, 	NULL, NULL); 
	
	//-------------------------------------------------------------------
	
	pan_setbutton(material_scwaterpanel2, 0,	1, 	130,390,		ss_load_on_bmp,ss_load_off_bmp,ss_load_over_bmp,ss_load_over_bmp,				FileMenu_LoadScWaterNm			,NULL,NULL); 	
	pan_setbutton(material_scwaterpanel2, 0,	1, 	130,130,		ss_load_on_bmp,ss_load_off_bmp,ss_load_over_bmp,ss_load_over_bmp,				FileMenu_LoadScWaterFoam		,NULL,NULL); 	
	
//	////////////////////////////////////////////////////////////////////
//	
//	while(material_scwaterpanel1 && material_scwaterpanel2)
//		{
//			
//			
//			wait(1);
//		}
}



void	Material_ScWaterPanel12_Close()
{
	if (material_scwaterpanel1 != NULL)
		{
			ptr_remove(material_scwaterpanel1);
			material_scwaterpanel1 = NULL;
		}
	
	if (material_scwaterpanel2 != NULL)
		{
			ptr_remove(material_scwaterpanel2);
			material_scwaterpanel2 = NULL;
		}
}


void	Material_ScWater_Button1(button_number)
{
	wait(3);
	
	//--------------------------------------------
	// scwater_reflectcolor red
	if (button_number==(var)1)
		{
			scwater_reflectcolor[0] = maxv( 0 , scwater_reflectcolor[0]-0.01 );
		}
	else if (button_number==(var)2)
		{
			scwater_reflectcolor[0] = minv( 1 , scwater_reflectcolor[0]+0.01 );
		}
	//--------------------------------------------
	// scwater_reflectcolor green
	else if (button_number==(var)3)
		{
			scwater_reflectcolor[1] = maxv( 0 , scwater_reflectcolor[1]-0.01 );
		}
	else if (button_number==(var)4)
		{
			scwater_reflectcolor[1] = minv( 1 , scwater_reflectcolor[1]+0.01 );
		}
	//--------------------------------------------
	// scwater_reflectcolor blue
	else if (button_number==(var)5)
		{
			scwater_reflectcolor[2] = maxv( 0 , scwater_reflectcolor[2]-0.01 );
		}
	else if (button_number==(var)6)
		{
			scwater_reflectcolor[2] = minv( 1 , scwater_reflectcolor[2]+0.01 );
		}
	//--------------------------------------------
	// scwater_reflectcolor alpha
	else if (button_number==(var)7)
		{
			scwater_reflectcolor[3] = maxv( 0 , scwater_reflectcolor[3]-0.01 );
		}
	else if (button_number==(var)8)
		{
			scwater_reflectcolor[3] = minv( 1 , scwater_reflectcolor[3]+0.01 );
		}
	//--------------------------------------------
	// scwater_reflectiondistance
	else if (button_number==(var)9)
		{
			scwater_reflectiondistance = maxv( 1 , scwater_reflectiondistance-0.1 );
		}
	else if (button_number==(var)10)
		{
			scwater_reflectiondistance = minv( 10 , scwater_reflectiondistance+0.1 );
		}
	//--------------------------------------------
	// scwater_reflectionstrength
	else if (button_number==(var)11)
		{
			scwater_reflectionstrength = maxv( 0 , scwater_reflectionstrength-0.01 );
		}
	else if (button_number==(var)12)
		{
			scwater_reflectionstrength = minv( 0.5 , scwater_reflectionstrength+0.01 );
		}
	//--------------------------------------------
	// scwater_refractcolor red
	else if (button_number==(var)13)
		{
			scwater_refractcolor[0] = maxv( 0 , scwater_refractcolor[0]-0.01 );
		}
	else if (button_number==(var)14)
		{
			scwater_refractcolor[0] = minv( 1 , scwater_refractcolor[0]+0.01 );
		}
	//--------------------------------------------
	// scwater_refractcolor green
	else if (button_number==(var)15)
		{
			scwater_refractcolor[1] = maxv( 0 , scwater_refractcolor[1]-0.01 );
		}
	else if (button_number==(var)16)
		{
			scwater_refractcolor[1] = minv( 1 , scwater_refractcolor[1]+0.01 );
		}
	//--------------------------------------------
	// scwater_refractcolor blue
	else if (button_number==(var)17)
		{
			scwater_refractcolor[2] = maxv( 0 , scwater_refractcolor[2]-0.01 );
		}
	else if (button_number==(var)18)
		{
			scwater_refractcolor[2] = minv( 1 , scwater_refractcolor[2]+0.01 );
		}
	//--------------------------------------------
	// scwater_refractcolor alpha
	else if (button_number==(var)19)
		{
			scwater_refractcolor[3] = maxv( 0 , scwater_refractcolor[3]-0.01 );
		}
	else if (button_number==(var)20)
		{
			scwater_refractcolor[3] = minv( 1 , scwater_refractcolor[3]+0.01 );
		}
	//--------------------------------------------
	// scwater_refractionstrength
	else if (button_number==(var)21)
		{
			scwater_refractionstrength = maxv( 0 , scwater_refractionstrength-0.01 );
		}
	else if (button_number==(var)22)
		{
			scwater_refractionstrength = minv( 1 , scwater_refractionstrength+0.01 );
		}
	//--------------------------------------------
	// scwater_shorecolor red
	else if (button_number==(var)23)
		{
			scwater_shorecolor[0] = maxv( 0.5 , scwater_shorecolor[0]-0.01 );
		}
	else if (button_number==(var)24)
		{
			scwater_shorecolor[0] = minv( 1.5 , scwater_shorecolor[0]+0.01 );
		}
	//--------------------------------------------
	// scwater_shorecolor green
	else if (button_number==(var)25)
		{
			scwater_shorecolor[1] = maxv( 0.5 , scwater_shorecolor[1]-0.01 );
		}
	else if (button_number==(var)26)
		{
			scwater_shorecolor[1] = minv( 1.5 , scwater_shorecolor[1]+0.01 );
		}
	//--------------------------------------------
	// scwater_shorecolor blue
	else if (button_number==(var)27)
		{
			scwater_shorecolor[2] = maxv( 0.5 , scwater_shorecolor[2]-0.01 );
		}
	else if (button_number==(var)28)
		{
			scwater_shorecolor[2] = minv( 1.5 , scwater_shorecolor[2]+0.01 );
		}
	//--------------------------------------------
	// scwater_shorecolor alpha
	else if (button_number==(var)29)
		{
			scwater_shorecolor[3] = maxv( 0 , scwater_shorecolor[3]-0.01 );
		}
	else if (button_number==(var)30)
		{
			scwater_shorecolor[3] = minv( 1 , scwater_shorecolor[3]+0.01 );
		}
	//--------------------------------------------
	// scwater_waterdepth
	else if (button_number==(var)31)
		{
			scwater_waterdepth = maxv( 0 , scwater_waterdepth-1 );
		}
	else if (button_number==(var)32)
		{
			scwater_waterdepth = minv( 320 , scwater_waterdepth+1 );
		}
	//--------------------------------------------
	// scwater_globaltransparency
	else if (button_number==(var)33)
		{
			scwater_globaltransparency = maxv( 0 , scwater_globaltransparency-0.01 );
		}
	else if (button_number==(var)34)
		{
			scwater_globaltransparency = minv( 1 , scwater_globaltransparency+0.01 );
		}	
}


void	Material_ScWater_Button2(button_number)
{
	wait(3);
	
	//--------------------------------------------
	// scwater_wavespeed
	if (button_number==(var)1)
		{
			scwater_wavespeed = maxv( 5000 , scwater_wavespeed-100 );
		}
	else if (button_number==(var)2)
		{
			scwater_wavespeed = minv( 30000 , scwater_wavespeed+100 );
		}
	//--------------------------------------------
	// scwater_displacement
	else if (button_number==(var)3)
		{
			scwater_displacement = maxv( 0 , scwater_displacement-1 );
		}
	else if (button_number==(var)4)
		{
			scwater_displacement = minv( 320 , scwater_displacement+1 );
		}
	//--------------------------------------------
	// scwater_speed
	else if (button_number==(var)5)
		{
			scwater_speed = maxv( 0 , scwater_speed-0.001 );
		}
	else if (button_number==(var)6)
		{
			scwater_speed = minv( 0.1 , scwater_speed+0.001 );
		}
	//--------------------------------------------
	// scwater_turbdensity
	else if (button_number==(var)7)
		{
			scwater_turbdensity = maxv( 0 , scwater_turbdensity-0.01 );
		}
	else if (button_number==(var)8)
		{
			scwater_turbdensity = minv( 1 , scwater_turbdensity+0.01 );
		}
	//--------------------------------------------
	// scwater_foamwave_deep
	else if (button_number==(var)9)
		{
			scwater_foamwave_deep = maxv( 0 , scwater_foamwave_deep-0.01 );
		}
	else if (button_number==(var)10)
		{
			scwater_foamwave_deep = minv( 1 , scwater_foamwave_deep+0.01 );
		}
	//--------------------------------------------
	// scwater_foamwave_shore
	else if (button_number==(var)11)
		{
			scwater_foamwave_shore = maxv( 0 , scwater_foamwave_shore-0.1 );
		}
	else if (button_number==(var)12)
		{
			scwater_foamwave_shore = minv( 1 , scwater_foamwave_shore+0.1 );
		}
	//--------------------------------------------
	// scwater_foamtiling
	else if (button_number==(var)13)
		{
			scwater_foamtiling = maxv( 1 , scwater_foamtiling-1 );
		}
	else if (button_number==(var)14)
		{
			scwater_foamtiling = minv( 100 , scwater_foamtiling+1 );
		}
	//--------------------------------------------
	// scwater_foamdistortion
	else if (button_number==(var)15)
		{
			scwater_foamdistortion = maxv( 0 , scwater_foamdistortion-0.001 );
		}
	else if (button_number==(var)16)
		{
			scwater_foamdistortion = minv( 0.5 , scwater_foamdistortion+0.001 );
		}
	//--------------------------------------------
	// scwater_foamtranslation
	else if (button_number==(var)17)
		{
			scwater_foamtranslation = maxv( 0 , scwater_foamtranslation-0.001 );
		}
	else if (button_number==(var)18)
		{
			scwater_foamtranslation = minv( 10 , scwater_foamtranslation+0.001 );
		}
	//--------------------------------------------
	// scwater_shorefoamscale
	else if (button_number==(var)19)
		{
			scwater_shorefoamscale = maxv( 0 , scwater_shorefoamscale-0.01 );
		}
	else if (button_number==(var)20)
		{
			scwater_shorefoamscale = minv( 1 , scwater_shorefoamscale+0.01 );
		}
	//--------------------------------------------
	// scwater_foamscale
	else if (button_number==(var)21)
		{
			scwater_foamscale = maxv( 0 , scwater_foamscale-0.01 );
		}
	else if (button_number==(var)22)
		{
			scwater_foamscale = minv( 1 , scwater_foamscale+0.01 );
		}
	//--------------------------------------------
	// scwater_sunlightstrength
	else if (button_number==(var)23)
		{
			scwater_sunlightstrength = maxv( 0 , scwater_sunlightstrength-0.1 );
		}
	else if (button_number==(var)24)
		{
			scwater_sunlightstrength = minv( 10 , scwater_sunlightstrength+0.1 );
		}
	//--------------------------------------------
	// scwater_sunlightarea
	else if (button_number==(var)25)
		{
			scwater_sunlightarea = maxv( 10 , scwater_sunlightarea-1 );
		}
	else if (button_number==(var)26)
		{
			scwater_sunlightarea = minv( 1000 , scwater_sunlightarea+1 );
		}
	//--------------------------------------------
	// scwater_bumpstrength
	else if (button_number==(var)27)
		{
			scwater_bumpstrength = maxv( 0 , scwater_bumpstrength-0.1 );
		}
	else if (button_number==(var)28)
		{
			scwater_bumpstrength = minv( 50 , scwater_bumpstrength+0.1 );
		}
	//--------------------------------------------
	// scwater_bumptexscale
	else if (button_number==(var)29)
		{
			scwater_bumptexscale = maxv( 0 , scwater_bumptexscale-0.1 );
		}
	else if (button_number==(var)30)
		{
			scwater_bumptexscale = minv( 100 , scwater_bumptexscale+0.1 );
		}
	//--------------------------------------------
	// scwater_2ndwave_speed
	else if (button_number==(var)31)
		{
			scwater_2ndwave_speed = maxv( -2 , scwater_2ndwave_speed-0.01 );
		}
	else if (button_number==(var)32)
		{
			scwater_2ndwave_speed = minv( 2 , scwater_2ndwave_speed+0.01 );
		}
	//--------------------------------------------
	// scwater_2ndwave_scale
	else if (button_number==(var)33)
		{
			scwater_2ndwave_scale = maxv( 0 , scwater_2ndwave_scale-0.01 );
		}
	else if (button_number==(var)34)
		{
			scwater_2ndwave_scale = minv( 5 , scwater_2ndwave_scale+0.01 );
		}
}


////////////////////////////////////////////////////////////////////////////////////////////////


#endif