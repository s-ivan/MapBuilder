#ifndef MBpssmeditor_c
#define MBpssmeditor_c

////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Sky_PssmPanel_Init()
{
	sky_pssmpanel = pan_create( panel_image2 , 14 );
	set( sky_pssmpanel , SHOW | SHADOW | OVERLAY );					
	sky_pssmpanel.pos_x 	= ui_submenupanel3_x - 256;			// screen_size.x-512;
	sky_pssmpanel.pos_y 	= ui_submenupanel3_y;					// 0;			
	sky_pssmpanel.red		= panel_font_r;
	sky_pssmpanel.green	= panel_font_g;
	sky_pssmpanel.blue	= panel_font_b;
	sky_pssmpanel.event	= ui_various_panelmove;
	
	//string 1 - title
	pan_setstring(sky_pssmpanel, 0,	15,10,	panel_font2,			str_create("PSSM SHADOW SETTINGS") );
	
	////////////////////////////////////////////////////////////////////
	// real time settings
	
	// string 2
	pan_setstring(sky_pssmpanel, 0,	15,40,	panel_font2,			str_create("Real-time parameters") );
	
	// shadow transparency----------------------------------------------
	
	// slider 1 - 1..100
	pan_setdigits(sky_pssmpanel, 0,	15,70,	"Transparency: %0.2f",				panel_font,	1, 									*sky_pssm_transp);
	pan_setslider(sky_pssmpanel, 0,	120+12,72,	horiz_slider_bmp,slider_knob_bmp,	0,1,											*sky_pssm_transp); 
	// button 1 2
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,70,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,70,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// shadow image resolution------------------------------------------
	
	// string 3
	pan_setstring(sky_pssmpanel, 0,	15,90,	panel_font,			str_create("Resolution:") );
	pan_setdigits(sky_pssmpanel, 0,	145,90,							"%0.f", 	panel_font, 	1, 										*sky_pssm_res);
	// button 3 4 - skin texture size - read on init, and can be set by color fill			
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,90,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 		NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	180,90,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 		NULL, NULL); 																							
	
	// shadow split weight----------------------------------------------
	
	// slider 2 - 1..100
	pan_setdigits(sky_pssmpanel, 0,	15,150,	"Slpit weight: %0.2f",				panel_font,	1, 									*sky_pssm_splitw);
	pan_setslider(sky_pssmpanel, 0,	120+12,152,	horiz_slider_bmp,slider_knob_bmp,	0,1,											*sky_pssm_splitw); 
	// button  5 6
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,150,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,150,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// shadow bias----------------------------------------------------
	
	// slider 3 - 0.5..0.1
	pan_setdigits(sky_pssmpanel, 0,	15,330,	"Bias: %0.3f",				panel_font,		1,											 	*sky_pssm_bias );
	pan_setslider(sky_pssmpanel, 0,	120+12,332,	horiz_slider_bmp,slider_knob_bmp,	0.001, 1,									*sky_pssm_bias ); 
	// button 7 8
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,330,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,330,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// shadow limit-----------------------------------------------------
	
	// string 4
	pan_setstring(sky_pssmpanel, 0,	15,390,	panel_font,			str_create("Limit:") );
	pan_setdigits(sky_pssmpanel, 0,	145,390,							"%0.f", 	panel_font, 	1, 										*sky_pssm_limit);
	// button 9 10
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,390,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	165,390,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 																							
	
	// sky_pssm_fadeout_perc---------------------------------------------
	
	// slider 4 - 1..100
	pan_setdigits(sky_pssmpanel, 0,	15,370,	"Fade-out perc.: %0.2f",				panel_font,	1, 								*sky_pssm_fadeout_perc);
	pan_setslider(sky_pssmpanel, 0,	120+12,372,	horiz_slider_bmp,slider_knob_bmp,	0,1,											*sky_pssm_fadeout_perc); 
	// button 11 12
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,370,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,370,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// sky_pssm_max------------------------------------------------------
	
	// slider 5
	pan_setdigits(sky_pssmpanel, 0,	15,170,	"Clip far perc.: %0.2f",				panel_font,	1, 								*sky_pssm_max);
	pan_setslider(sky_pssmpanel, 0,	120+12,172,	horiz_slider_bmp,slider_knob_bmp,	0,1,											*sky_pssm_max); 
	// button 13 14
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,170,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,170,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// sky_pssm_manual--------------------------------------------------
	
	// string 5
	pan_setstring(sky_pssmpanel, 0,	45,205,	panel_font,			str_create("Use manual distances:") );
	// button 15
	pan_setbutton(sky_pssmpanel, 0,	4, 	15,205,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Sky_Pssm_ToggleManual			,NULL,NULL);
	button_state(sky_pssmpanel, 15, *sky_pssm_manual); 	
	
	// sky_pssm_dist1---------------------------------------------------
	
	// slider 6
	pan_setdigits(sky_pssmpanel, 0,	15,230,	"Distance 1: %0.f",				panel_font,	1, 										*sky_pssm_dist1);
	pan_setslider(sky_pssmpanel, 0,	120+12,232,	horiz_slider_bmp,slider_knob_bmp,	0, *sky_pssm_dist2,						*sky_pssm_dist1); 
	// button 16 17
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,230,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,230,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// sky_pssm_dist2---------------------------------------------------
	
	// slider 7
	pan_setdigits(sky_pssmpanel, 0,	15,250,	"Distance 2: %0.f",				panel_font,	1, 										*sky_pssm_dist2);
	pan_setslider(sky_pssmpanel, 0,	120+12,252,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist1, *sky_pssm_dist3,	*sky_pssm_dist2); 
	// button 18 19
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,250,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,250,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// sky_pssm_dist3---------------------------------------------------
	
	// slider 8
	pan_setdigits(sky_pssmpanel, 0,	15,270,	"Distance 3: %0.f",				panel_font,	1, 										*sky_pssm_dist3);
	pan_setslider(sky_pssmpanel, 0,	120+12,272,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist2, *sky_pssm_dist4,	*sky_pssm_dist3); 
	// button 20 21
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,270,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,270,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// sky_pssm_dist4---------------------------------------------------
	
	// slider 9
	pan_setdigits(sky_pssmpanel, 0,	15,290,	"Distance 4: %0.f",				panel_font,	1, 										*sky_pssm_dist4);
	pan_setslider(sky_pssmpanel, 0,	120+12,292,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist3, camera.clip_far,	*sky_pssm_dist4); 
	// button 22 23
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,290,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,290,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// sky_pssm_shadowweight--------------------------------------------
	
	// slider 10
	pan_setdigits(sky_pssmpanel, 0,	15,350,	"Shadow weight: %0.2f",			panel_font,	1, 										*sky_pssm_shadowweight);
	pan_setslider(sky_pssmpanel, 0,	120+12,352,	horiz_slider_bmp,slider_knob_bmp,	2.5, 10,										*sky_pssm_shadowweight); 
	// button 24 25
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,350,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,350,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// sky_pssm_auto----------------------------------------------------
	
	// string 6
	pan_setstring(sky_pssmpanel, 0,	45,125,	panel_font,			str_create("Use auto split distances:") );
	// button 26
	pan_setbutton(sky_pssmpanel, 0,	4, 	15,125,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Sky_Pssm_ToggleManual			,NULL,NULL);
	button_state(sky_pssmpanel, 26, sky_pssm_auto); 	
		
	// sky_pssm_esm-----------------------------------------------------
	
	// slider 11
	pan_setdigits(sky_pssmpanel, 0,	15,420,	"ESM factor: %0.f",			panel_font,	1, 											*sky_pssm_esm);
	pan_setslider(sky_pssmpanel, 0,	120+12,422,	horiz_slider_bmp,slider_knob_bmp,	1, 2500,										*sky_pssm_esm); 
	// button 27 28
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,420,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,420,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// sky_pssm_blurtype------------------------------------------------
	
	// string 7
	pan_setstring(sky_pssmpanel, 0,	15,440,	panel_font,			str_create("Blur type:") );
	pan_setdigits(sky_pssmpanel, 0,	145,440,							"%0.f", 	panel_font, 	1, 										*sky_pssm_blurtype);
	// button 29 30
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,440,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	165,440,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 																							
		
	// sky_pssm_blurfactor----------------------------------------------
	
	// slider 12
	pan_setdigits(sky_pssmpanel, 0,	15,460,	"Blur factor: %0.2f",			panel_font,	1, 										*sky_pssm_blurfactor);
	pan_setslider(sky_pssmpanel, 0,	120+12,462,	horiz_slider_bmp,slider_knob_bmp,	0, 5,											*sky_pssm_blurfactor); 
	// button 31 32
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,460,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,460,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	// sky_pssm_fogfactor-----------------------------------------------
	
	// slider 13
	pan_setdigits(sky_pssmpanel, 0,	15,490,	"Fog factor: %0.1f",			panel_font,	1, 											*sky_pssm_fogfactor);
	pan_setslider(sky_pssmpanel, 0,	120+12,492,	horiz_slider_bmp,slider_knob_bmp,	0, 25,										*sky_pssm_fogfactor); 
	// button 33 34
	pan_setbutton(sky_pssmpanel, 0,	1, 	120,490,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel, 0,	1, 	240,490,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button, 	NULL, NULL); 
	
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
	// small panel for static parameters
	
	sky_pssmpanel2 = pan_create( panel_image , 14 );
	set( sky_pssmpanel2 , SHOW | SHADOW | OVERLAY );					
	sky_pssmpanel2.pos_x 	= ui_submenupanel3_x;					// screen_size.x-256;
	sky_pssmpanel2.pos_y 	= ui_submenupanel3_y;					// 0;			
	sky_pssmpanel2.red		= panel_font_r;
	sky_pssmpanel2.green		= panel_font_g;
	sky_pssmpanel2.blue		= panel_font_b;
	sky_pssmpanel2.event		= ui_various_panelmove;
	
	//string 1 - title
	pan_setstring(sky_pssmpanel2, 0,	15,10,	panel_font2,			str_create("PSSM SHADOW SETTINGS") );
	
	////////////////////////////////////////////////////////////////////
	// static settings
	
	// string 2
	pan_setstring(sky_pssmpanel2, 0,	15,40,	panel_font2,			str_create("Static parameters") );
	
	// shadow number of splits------------------------------------------
	
	// string 3
	pan_setstring(sky_pssmpanel2, 0,	15,70,	panel_font,			str_create("Splits:") );
	pan_setdigits(sky_pssmpanel2, 0,	145,70,							"%0.f", 	panel_font, 	1, 										sky_pssm_numsplits);
	// button 1 2
	pan_setbutton(sky_pssmpanel2, 0,	1, 	120,70,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button2, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel2, 0,	1, 	165,70,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button2, 	NULL, NULL); 																							
	
	// sky_pssm_splitmode------------------------------------------------
	
	// string 4
	pan_setstring(sky_pssmpanel2, 0,	15,100,	panel_font,			str_create("Split mode:") );
	pan_setdigits(sky_pssmpanel2, 0,	145,100,							"%0.f", 		panel_font, 	1, 									*sky_pssm_splitmode);
	// button 3 4
	pan_setbutton(sky_pssmpanel2, 0,	1, 	120,100,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button2, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel2, 0,	1, 	165,100,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button2, 	NULL, NULL); 																							
	
	// sky_pssm_lod-----------------------------------------------------
	
	// string 5
	pan_setstring(sky_pssmpanel2, 0,	15,130,	panel_font,			str_create("LOD:") );
	pan_setdigits(sky_pssmpanel2, 0,	145,130,							"%0.f", 		panel_font, 	1, 									*sky_pssm_lod);
	// button 5 6
	pan_setbutton(sky_pssmpanel2, 0,	1, 	120,130,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Pssm_Button2, 	NULL, NULL); 
	pan_setbutton(sky_pssmpanel2, 0,	1, 	165,130,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Pssm_Button2, 	NULL, NULL); 																							
	
	// apply static changes---------------------------------------------
	
	// button 7 - apply non real time settings
	pan_setbutton(sky_pssmpanel2, 0,	1, 	100,170,		ss_apply_on_bmp,ss_apply_off_bmp,ss_apply_over_bmp,ss_apply_over_bmp,		Sky_Pssm_Apply,		NULL,NULL); 		
	
	//---------------------------------------------------
	// load save
	
	pan_setbutton(sky_pssmpanel2, 0,	1, 	50 ,224,		s_load_on_bmp,s_load_off_bmp,s_load_over_bmp,s_load_over_bmp,				FileMenu_LoadPssm			,NULL,NULL); 	
	pan_setbutton(sky_pssmpanel2, 0,	1, 	150,224,		s_save_on_bmp,s_save_off_bmp,s_save_over_bmp,s_save_over_bmp,				FileMenu_SavePssm			,NULL,NULL); 		
	
	///////////////////////////////////////////////////////////////////
	
	while(sky_pssmpanel)
		{
			// limit slider values	
			if (*sky_pssm_dist1>=*sky_pssm_dist2)	*sky_pssm_dist1 = *sky_pssm_dist2-10;
			if (*sky_pssm_dist2>=*sky_pssm_dist3)	*sky_pssm_dist2 = *sky_pssm_dist3-10;
			if (*sky_pssm_dist3>=*sky_pssm_dist4)	*sky_pssm_dist3 = *sky_pssm_dist4-10;
			if (*sky_pssm_dist4>=camera.clip_far)	*sky_pssm_dist4 = camera.clip_far-10;
			
			// update slider boundaries because of mutual boundary dependencies
			pan_setslider(sky_pssmpanel, 6,	120+12,232,	horiz_slider_bmp,slider_knob_bmp,	0, *sky_pssm_dist2,						*sky_pssm_dist1); 
			pan_setslider(sky_pssmpanel, 7,	120+12,252,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist1, *sky_pssm_dist3,	*sky_pssm_dist2); 
			pan_setslider(sky_pssmpanel, 8,	120+12,272,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist2, *sky_pssm_dist4,	*sky_pssm_dist3); 
			pan_setslider(sky_pssmpanel, 9,	120+12,292,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist3, camera.clip_far,	*sky_pssm_dist4); 
			
			wait(1);
		}
}


void		Sky_PssmPanel_Close()
{
	if (sky_pssmpanel)
		{
			ptr_remove(sky_pssmpanel);
			sky_pssmpanel = NULL;
		}
	
	if (sky_pssmpanel2)
		{
			ptr_remove(sky_pssmpanel2);
			sky_pssmpanel2 = NULL;
		}
}


void		Sky_Pssm_Button(button_number)
{
	wait(3);
	
	//--------------------------------------------
	// shadow alpha
	if (button_number==(var)1)
		{
			*sky_pssm_transp = maxv( 0 , *sky_pssm_transp-0.01 );
		}
	else if (button_number==(var)2)
		{
			*sky_pssm_transp = minv( 1 , *sky_pssm_transp+0.01 );
		}
	//--------------------------------------------
	// shadow bitmap size
	else if (button_number==(var)3)
		{
			*sky_pssm_res /= 2;
			*sky_pssm_res = maxv( 512 , *sky_pssm_res );
		}
	else if (button_number==(var)4)
		{
			*sky_pssm_res *= 2;
			*sky_pssm_res = minv( d3d_texlimit , *sky_pssm_res );
		}
	//--------------------------------------------	
	// slpit weight
	else if (button_number==(var)5)
		{
			*sky_pssm_splitw = maxv( 0 , *sky_pssm_splitw-0.01 );
		}
	else if (button_number==(var)6)
		{
			*sky_pssm_splitw = minv( 1 , *sky_pssm_splitw+0.01 );
		}
	//--------------------------------------------
	// sky_pssm_bias
	else if (button_number==(var)7)
		{
			*sky_pssm_bias = maxv( 0.01 , *sky_pssm_bias-0.001 );
		}
	else if (button_number==(var)8)
		{
			*sky_pssm_bias = minv( 1 , *sky_pssm_bias+0.001 );
		}
	//--------------------------------------------
	// sky_pssm_limit
	else if (button_number==(var)9)
		{
			*sky_pssm_limit = maxv( 1 , --*sky_pssm_limit );
		}
	else if (button_number==(var)10)
		{
			*sky_pssm_limit = minv( sky_pssm_numsplits , ++*sky_pssm_limit );
		}
	//--------------------------------------------
	// sky_pssm_fadeout_perc
	else if (button_number==(var)11)
		{
			*sky_pssm_fadeout_perc = maxv( 0 , *sky_pssm_fadeout_perc-0.01 );
		}
	else if (button_number==(var)12)
		{
			*sky_pssm_fadeout_perc = minv( 1 , *sky_pssm_fadeout_perc+0.01 );
		}
	//--------------------------------------------
	// sky_pssm_max	
	else if (button_number==(var)13)
		{
			*sky_pssm_max = maxv( 0 , *sky_pssm_max-0.01 );
		}
	else if (button_number==(var)14)
		{
			*sky_pssm_max = minv( 1 , *sky_pssm_max+0.01 );
		}
	//--------------------------------------------
	// sky_pssm_dist1
	else if (button_number==(var)16)
		{
			*sky_pssm_dist1 = maxv( 0 , *sky_pssm_dist1-10 );
			pan_setslider(sky_pssmpanel, 6,	120+12,232,	horiz_slider_bmp,slider_knob_bmp,	0, *sky_pssm_dist2,						*sky_pssm_dist1); 
		}
	else if (button_number==(var)17)
		{
			*sky_pssm_dist1 = minv( *sky_pssm_dist2 , *sky_pssm_dist1+10 );
			pan_setslider(sky_pssmpanel, 6,	120+12,232,	horiz_slider_bmp,slider_knob_bmp,	0, *sky_pssm_dist2,						*sky_pssm_dist1); 
		}
	//--------------------------------------------
	// sky_pssm_dist2
	else if (button_number==(var)18)
		{
			*sky_pssm_dist2 = maxv( sky_pssm_dist1 , *sky_pssm_dist2-10 );
			pan_setslider(sky_pssmpanel, 7,	120+12,252,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist1, *sky_pssm_dist3,	*sky_pssm_dist2); 
		}
	else if (button_number==(var)19)
		{
			*sky_pssm_dist2 = minv( *sky_pssm_dist3 , *sky_pssm_dist2+10 );
			pan_setslider(sky_pssmpanel, 7,	120+12,252,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist1, *sky_pssm_dist3,	*sky_pssm_dist2); 
		}
	//--------------------------------------------
	// sky_pssm_dist3
	else if (button_number==(var)20)
		{
			*sky_pssm_dist3 = maxv( sky_pssm_dist2 , *sky_pssm_dist3-10 );
			pan_setslider(sky_pssmpanel, 8,	120+12,272,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist2, *sky_pssm_dist4,	*sky_pssm_dist3); 
		}
	else if (button_number==(var)21)
		{
			*sky_pssm_dist3 = minv( *sky_pssm_dist4 , *sky_pssm_dist3+10 );
			pan_setslider(sky_pssmpanel, 8,	120+12,272,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist2, *sky_pssm_dist4,	*sky_pssm_dist3); 
		}
	//--------------------------------------------
	// sky_pssm_dist4
	else if (button_number==(var)22)
		{
			*sky_pssm_dist4 = maxv( sky_pssm_dist3 , *sky_pssm_dist4-10 );
			pan_setslider(sky_pssmpanel, 9,	120+12,292,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist3, camera.clip_far,	*sky_pssm_dist4); 
		}
	else if (button_number==(var)23)
		{
			*sky_pssm_dist4 = minv( camera.clip_far , *sky_pssm_dist4+10 );
			pan_setslider(sky_pssmpanel, 9,	120+12,292,	horiz_slider_bmp,slider_knob_bmp,	*sky_pssm_dist3, camera.clip_far,	*sky_pssm_dist4); 
		}
	//--------------------------------------------
	// sky_pssm_shadowweight	
	else if (button_number==(var)24)
		{
			*sky_pssm_shadowweight = maxv( 2.5 , *sky_pssm_shadowweight-0.01 );
		}
	else if (button_number==(var)25)
		{
			*sky_pssm_shadowweight = minv( 10 , *sky_pssm_shadowweight+0.01 );
		}
	//--------------------------------------------
	// sky_pssm_esm
	else if (button_number==(var)27)
		{
			*sky_pssm_esm = maxv( 1 , *sky_pssm_esm-1 );
		}
	else if (button_number==(var)28)
		{
			*sky_pssm_esm = minv( 2500 , *sky_pssm_esm+1 );
		}
	//--------------------------------------------
	// sky_pssm_blurtype
	else if (button_number==(var)29)
		{
			*sky_pssm_blurtype = maxv( 0 , *sky_pssm_blurtype-1 );
			Sky_Pssm_SetBlurType();
		}
	else if (button_number==(var)30)
		{
			*sky_pssm_blurtype = minv( 2 , *sky_pssm_blurtype+1 );
			Sky_Pssm_SetBlurType();
		}
	//--------------------------------------------
	// sky_pssm_blurfactor
	else if (button_number==(var)31)
		{
			*sky_pssm_blurfactor = maxv( 0 , *sky_pssm_blurfactor-0.01 );
		}
	else if (button_number==(var)32)
		{
			*sky_pssm_blurfactor = minv( 5 , *sky_pssm_blurfactor+0.01 );
		}
	//--------------------------------------------
	// sky_pssm_fogfactor
	else if (button_number==(var)33)
		{
			*sky_pssm_fogfactor = maxv( 0 , *sky_pssm_fogfactor-0.1 );
		}
	else if (button_number==(var)34)
		{
			*sky_pssm_fogfactor = minv( 25 , *sky_pssm_fogfactor+0.1 );
		}	
}


void		Sky_Pssm_Button2(button_number)
{
	wait(3);
	
	//--------------------------------------------	
	// sky_pssm_numsplits
	if (button_number==(var)1)
		{
			sky_pssm_numsplits = maxv( 0 , --sky_pssm_numsplits );
			*sky_pssm_limit = minv( sky_pssm_numsplits , *sky_pssm_limit );	// check limit when numsplits decreased
		}
	else if (button_number==(var)2)
		{
			sky_pssm_numsplits = minv( 4 , ++sky_pssm_numsplits );
		}
	//--------------------------------------------
	// sky_pssm_splitmode
	else if (button_number==(var)3)
		{
			*sky_pssm_splitmode = maxv( 0 , --*sky_pssm_splitmode );
		}
	else if (button_number==(var)4)
		{
			*sky_pssm_splitmode = minv( 3 , ++*sky_pssm_splitmode );
		}
	//--------------------------------------------
	// sky_pssm_lod
	else if (button_number==(var)5)
		{
			*sky_pssm_lod = maxv( -1 , --*sky_pssm_lod );
		}
	else if (button_number==(var)6)
		{
			*sky_pssm_lod = minv( 3 , ++*sky_pssm_lod );
		}
}


void		Sky_Pssm_ToggleManual()
{
	*sky_pssm_manual 	= 1 - *sky_pssm_manual;
	sky_pssm_auto		= 1 - sky_pssm_auto;
}


void		Sky_Pssm_SetBlurType()
{
	wait(3);
	if (pssm_splitmode < (var)2) return;				// protection against invalid settings
	
	effect_load(pssm_mtlDepthBlur, NULL);
	wait(3);
	
	if (*sky_pssm_blurtype == (var)0)
		{
				effect_load(pssm_mtlDepthBlur, "vp_esmDepthBlurN_MB.fx");
		}
	else if (*sky_pssm_blurtype == (var)1)
		{
				effect_load(pssm_mtlDepthBlur, "vp_esmDepthBlurB_MB.fx");
		}
	else // if (*sky_pssm_blurtype == (var)2)
		{
				effect_load(pssm_mtlDepthBlur, "vp_esmDepthBlurP_MB.fx");
		}
}


void		Sky_Pssm_Apply()
{
	wait(3);
	if (proc_status(Sky_Pssm_Apply)>1)	return;
	
	//----------------------------
	// stop pssm shadows
	
	Pssm_Close();
	wait_for(Pssm_Close);				
	
	//----------------------------
	// update data
	
	// needed?
	pssm_transparency = *sky_pssm_transp;			// 0.5 - 0.3
	pssm_res 			= *sky_pssm_res;				// 2048 - 1024 - 512
	pssm_splitweight 	= *sky_pssm_splitw;			// 0.75-0.85 : 0.5 - 0.95		
	pssm_fadeout_perc = *sky_pssm_fadeout_perc;	// around 0.75
	
	pssm_bias 			= *sky_pssm_bias;
	pssm_limit 			= *sky_pssm_limit;
	
	pssm_max 			= *sky_pssm_max;
	pssm_manual 		= *sky_pssm_manual;
	pssm_dist1 			= *sky_pssm_dist1;
	pssm_dist2 			= *sky_pssm_dist2;
	pssm_dist3 			= *sky_pssm_dist3;
	pssm_dist4 			= *sky_pssm_dist4;
	pssm_shadowweight = *sky_pssm_shadowweight;
	pssm_lod 			= *sky_pssm_lod;
	pssm_splitmode		= *sky_pssm_splitmode;
	
	// must be done
	pssm_numsplits 	= sky_pssm_numsplits;
	
	wait(3);
	
	// restart pssm shadows
	Pssm_Start(sky_pssm_numsplits);
	wait(3);
	
	// reinitialize non-real-time data - needed
	sky_pssm_numsplits	= pssm_numsplits;					// 0..4									2 arrow buttons plus an apply button
	
	// reinitialize real-time data - needed?	
	sky_pssm_transp		= &pssm_transparency;			// 0.100									slider w buttons
	sky_pssm_res			= &pssm_res;						// 512, 1024, 2048, 4096			2 arrow buttons
	sky_pssm_splitw		= &pssm_splitweight;				// 0..1									slider w buttons
	sky_pssm_fadeout_perc= &pssm_fadeout_perc;			// 0..1
	
	sky_pssm_bias			= &pssm_bias;						// 0.0001..0.0005	-> 0.1..0.5		slider w buttons
	sky_pssm_limit			= &pssm_limit;						// 0..4									2 arrow buttons
	
	sky_pssm_max			= &pssm_max;
	sky_pssm_manual		= &pssm_manual;
	sky_pssm_dist1			= &pssm_dist1;
	sky_pssm_dist2			= &pssm_dist2;
	sky_pssm_dist3			= &pssm_dist3;
	sky_pssm_dist4			= &pssm_dist4;
	sky_pssm_shadowweight= &pssm_shadowweight;
	sky_pssm_lod			= &pssm_lod;
	sky_pssm_splitmode	= &pssm_splitmode;
	
	wait(3);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif