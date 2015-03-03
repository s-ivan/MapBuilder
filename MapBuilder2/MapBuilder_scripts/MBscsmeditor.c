
#ifndef MBscsmeditor_c
#define MBscsmeditor_c

////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Sky_ScsmPanel_Init()
{
	sky_scsmpanel = pan_create( panel_image2 , 14 );
	set( sky_scsmpanel , SHOW | SHADOW | OVERLAY );					
	sky_scsmpanel.pos_x 	= ui_submenupanel3_x;						// screen_size.x-512;
	sky_scsmpanel.pos_y 	= ui_submenupanel3_y;						// 0;			
	sky_scsmpanel.red		= panel_font_r;
	sky_scsmpanel.green	= panel_font_g;
	sky_scsmpanel.blue	= panel_font_b;
	sky_scsmpanel.event	= ui_various_panelmove;
	
	//string 1 - title
	pan_setstring(sky_scsmpanel, 0,	15,10,	panel_font2,			str_create("SHADE_C SHADOW SETTINGS") );
	
	////////////////////////////////////////////////////////////////////
	// real time settings 1
	
	// string 2
	pan_setstring(sky_scsmpanel, 0,	15,40,	panel_font2,			str_create("Real-time parameters") );
	
	// shadow transparency----------------------------------------------
	
	// slider 1 - 0..1
	pan_setdigits(sky_scsmpanel, 0,	15,70,	"Shadow alpha: %0.2f",				panel_font,	1, 			*sky_scsm_shadowalpha);
	pan_setslider(sky_scsmpanel, 0,	120+12,72,	horiz_slider_bmp,slider_knob_bmp,	0,1,					*sky_scsm_shadowalpha); 
	// button 1 2
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,70,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	240,70,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 
	
	// lightmap transparency----------------------------------------------
	
	// slider 2 - 0..1
	pan_setdigits(sky_scsmpanel, 0,	15,90,	"Lightmap alpha: %0.2f",				panel_font,	1, 		*sky_scsm_lightmapalpha);
	pan_setslider(sky_scsmpanel, 0,	120+12,92,	horiz_slider_bmp,slider_knob_bmp,	0,1,					*sky_scsm_lightmapalpha); 
	// button 3 4
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,90,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	240,90,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 
	
	// terrain esm----------------------------------------------
	
	// slider 3 - 1..25
	pan_setdigits(sky_scsmpanel, 0,	15,120,	"Terr. ESM factor: %0.1f",				panel_font,	1, 		*sky_scsm_vsm_epsilon);
	pan_setslider(sky_scsmpanel, 0,	120+12,122,	horiz_slider_bmp,slider_knob_bmp,	1,25,					*sky_scsm_vsm_epsilon); 
	// button 5 6
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	240,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 
	
	// object esm----------------------------------------------
	
	// slider 4 - 1..25
	pan_setdigits(sky_scsmpanel, 0,	15,140,	"Obj. ESM factor: %0.1f",				panel_font,	1, 		*sky_scsm_vsm_epsilon_obj);
	pan_setslider(sky_scsmpanel, 0,	120+12,142,	horiz_slider_bmp,slider_knob_bmp,	1,25,					*sky_scsm_vsm_epsilon_obj); 
	// button 7 8
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	240,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 
	
	// blur type----------------------------------------------------
		
	// string 3
	pan_setstring(sky_scsmpanel, 0,	15,170,	panel_font,			str_create("Blur type:") );
	pan_setdigits(sky_scsmpanel, 0,	145,170,							"%0.f", 		panel_font, 	1, 			*sky_scsm_blurtype);
	// button 9 10
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,170,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	165,170,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 																							
	
	// blur sampling----------------------------------------------
	
	// slider 5 - 0.1..5
	pan_setdigits(sky_scsmpanel, 0,	15,190,	"Blur sampling: %0.2f",				panel_font,	1, 			*sky_scsm_blurpixel);
	pan_setslider(sky_scsmpanel, 0,	120+12,192,	horiz_slider_bmp,slider_knob_bmp,	0.1,5,				*sky_scsm_blurpixel); 
	// button 11 12
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,190,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	240,190,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 
	
	// max depth----------------------------------------------
	
	// slider 6 - 10..1000
	pan_setdigits(sky_scsmpanel, 0,	15,240,	"Depth: %0.f",				panel_font,	1, 						*sky_scsm_maxdepth);
	pan_setslider(sky_scsmpanel, 0,	120+12,242,	horiz_slider_bmp,slider_knob_bmp,	10,1500,				*sky_scsm_maxdepth); 
	// button 13 14
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,240,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	240,240,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 
	
	///////////////////////////////////////////////////////////////////
	// non real time settings
	
	// string 4
	pan_setstring(sky_scsmpanel, 0,	15,290,	panel_font2,			str_create("Static parameters") );
	
	// resolution----------------------------------------------
	
//	// slider 7 - screen_size.x..4096
//	pan_setdigits(sky_scsmpanel, 0,	15,310,	"Resolution: %0.f",				panel_font,	1, 						sky_scsm_resolution);
//	pan_setslider(sky_scsmpanel, 0,	120+12,312,	horiz_slider_bmp,slider_knob_bmp,	512,4096,		sky_scsm_resolution); 
//	// button 15 16
//	pan_setbutton(sky_scsmpanel, 0,	1, 	120,310,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
//	pan_setbutton(sky_scsmpanel, 0,	1, 	240,310,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 
	
	// string 5
	pan_setstring(sky_scsmpanel, 0,	15,320,	panel_font,			str_create("Resolution:") );
	pan_setdigits(sky_scsmpanel, 0,	145,320,							"%0.f", 	panel_font, 	1, 						sky_scsm_resolution);
	// button 15 16 - skin texture size - read on init, and can be set by color fill			
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,320,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 		NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	180,320,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 		NULL, NULL); 																							
	
	// area----------------------------------------------
	
	// slider 7 - 500..5000
	pan_setdigits(sky_scsmpanel, 0,	15,340,	"Area: %0.f",				panel_font,	1, 								sky_scsm_areasize);
	pan_setslider(sky_scsmpanel, 0,	120+12,342,	horiz_slider_bmp,slider_knob_bmp,	500,4000,					sky_scsm_areasize); 
	// button 17 18
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,340,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	240,340,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 
	
	// apply-----------------------------------------------
	
	// button 19 - apply non real time settings
	pan_setbutton(sky_scsmpanel, 0,	1, 	132,290,		ss_apply_on_bmp,ss_apply_off_bmp,ss_apply_over_bmp,ss_apply_over_bmp,										Sky_Scsm_Apply,	NULL,NULL); 	
	
	///////////////////////////////////////////////////////////////////
	// real time settings 2
	
	// shadow offset----------------------------------------------
	
	// slider 8 - 0..5
	pan_setdigits(sky_scsmpanel, 0,	15,220,	"Shadow offset: %0.2f",				panel_font,	1, 			*sky_scsm_shadowpixeloffset);
	pan_setslider(sky_scsmpanel, 0,	120+12,222,	horiz_slider_bmp,slider_knob_bmp,	0,5,					*sky_scsm_shadowpixeloffset); 
	// button 20 21
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,220,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	240,220,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 
	
	///////////////////////////////////////////////////////////////////	
	// non-real-time settings 2
	
	// lod --------------------------------------------------------
		
	// string 9 - 0..4
	pan_setstring(sky_scsmpanel, 0,	15,360,	panel_font,			str_create("LOD:") );
	pan_setdigits(sky_scsmpanel, 0,	145,360,							"%0.f", 		panel_font, 	1, 			sky_scsm_lod);
	// button 22 23
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,360,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	165,360,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 																							
	
	///////////////////////////////////////////////////////////////////
	// real time settings 3
	
	// fade-out start----------------------------------------------
	
	// slider 10 - 0..1
	pan_setdigits(sky_scsmpanel, 0,	15,260,	"Fade-out start: %0.2f",				panel_font,	1, 		*sky_scsm_fadeout_start);
	pan_setslider(sky_scsmpanel, 0,	120+12,262,	horiz_slider_bmp,slider_knob_bmp,	0,1,					*sky_scsm_fadeout_start); 
	// button 24 25
	pan_setbutton(sky_scsmpanel, 0,	1, 	120,260,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Scsm_Button, 	NULL, NULL); 
	pan_setbutton(sky_scsmpanel, 0,	1, 	240,260,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Scsm_Button, 	NULL, NULL); 
	
	//---------------------------------------------------
	// load save
	
	pan_setbutton(sky_scsmpanel, 0,	1, 	50 ,480,		s_load_on_bmp,s_load_off_bmp,s_load_over_bmp,s_load_over_bmp,				FileMenu_LoadScsm			,NULL,NULL); 	
	pan_setbutton(sky_scsmpanel, 0,	1, 	150,480,		s_save_on_bmp,s_save_off_bmp,s_save_over_bmp,s_save_over_bmp,				FileMenu_SaveScsm			,NULL,NULL); 		
	
//	///////////////////////////////////////////////////////////////////
//	
//	while(sky_scsmpanel)
//		{
//			
//			
//			wait(1);
//		}
}


void		Sky_ScsmPanel_Close()
{
	if (sky_scsmpanel)
		{
			ptr_remove(sky_scsmpanel);
			sky_scsmpanel = NULL;
		}	
}


void		Sky_Scsm_Button(button_number)
{
	wait(3);
	
	//--------------------------------------------
	// shadow alpha
	if (button_number==(var)1)
		{
			*sky_scsm_shadowalpha = maxv( 0 , *sky_scsm_shadowalpha-0.01 );
		}
	else if (button_number==(var)2)
		{
			*sky_scsm_shadowalpha = minv( 1 , *sky_scsm_shadowalpha+0.01 );
		}
	//--------------------------------------------
	// lightmap alpha
	else if (button_number==(var)3)
		{
			*sky_scsm_lightmapalpha = maxv( 0 , *sky_scsm_lightmapalpha-0.01 );
		}
	else if (button_number==(var)4)
		{
			*sky_scsm_lightmapalpha = minv( 1 , *sky_scsm_lightmapalpha+0.01 );
		}
	//--------------------------------------------
	// terrain esm
	else if (button_number==(var)5)
		{
			*sky_scsm_vsm_epsilon = maxv( 1 , *sky_scsm_vsm_epsilon-0.1 );
		}
	else if (button_number==(var)6)
		{
			*sky_scsm_vsm_epsilon = minv( 25 , *sky_scsm_vsm_epsilon+0.1 );
		}
	//--------------------------------------------
	// object esm
	else if (button_number==(var)7)
		{
			*sky_scsm_vsm_epsilon_obj = maxv( 1 , *sky_scsm_vsm_epsilon_obj-0.1 );
		}
	else if (button_number==(var)8)
		{
			*sky_scsm_vsm_epsilon_obj = minv( 25 , *sky_scsm_vsm_epsilon_obj+0.1 );
		}
	//--------------------------------------------
	// blur type
	else if (button_number==(var)9)
		{
			*sky_scsm_blurtype = maxv( 0 , *sky_scsm_blurtype-1 );
			Scsm_SetBlurType();
		}
	else if (button_number==(var)10)
		{
			*sky_scsm_blurtype = minv( 2 , *sky_scsm_blurtype+1 );
			Scsm_SetBlurType();
		}
	//--------------------------------------------
	// blur sampling
	else if (button_number==(var)11)
		{
			*sky_scsm_blurpixel = maxv( 0.1 , *sky_scsm_blurpixel-0.01 );
		}
	else if (button_number==(var)12)
		{
			*sky_scsm_blurpixel = minv( 5 , *sky_scsm_blurpixel+0.01 );
		}
	//--------------------------------------------
	// max depth
	else if (button_number==(var)13)
		{
			*sky_scsm_maxdepth = maxv( 10 , *sky_scsm_maxdepth-1 );
		}
	else if (button_number==(var)14)
		{
			*sky_scsm_maxdepth = minv( 1500 , *sky_scsm_maxdepth+1 );
		}
	//--------------------------------------------
	//--------------------------------------------
	// resolution
	else if (button_number==(var)15)
		{
//			sky_scsm_resolution = maxv( 512 , sky_scsm_resolution-1 );
			sky_scsm_resolution = maxv( 512 , sky_scsm_resolution/2 );
		}
	else if (button_number==(var)16)
		{
//			sky_scsm_resolution = minv( 4096 , sky_scsm_resolution+1 );
			sky_scsm_resolution = minv( d3d_texlimit , sky_scsm_resolution*2 );
		}
	//--------------------------------------------
	// area
	else if (button_number==(var)17)
		{
			sky_scsm_areasize = maxv( 500 , sky_scsm_areasize-1 );
		}
	else if (button_number==(var)18)
		{
			sky_scsm_areasize = minv( 4000 , sky_scsm_areasize+1 );
		}
	//--------------------------------------------
	//--------------------------------------------
	// shadow offset
	else if (button_number==(var)20)
		{
			*sky_scsm_shadowpixeloffset = maxv( 0 , *sky_scsm_shadowpixeloffset-0.01 );
		}
	else if (button_number==(var)21)
		{
			*sky_scsm_shadowpixeloffset = minv( 5 , *sky_scsm_shadowpixeloffset+0.01 );
		}
	//--------------------------------------------
	//--------------------------------------------
	// lod
	else if (button_number==(var)22)
		{
			sky_scsm_lod = maxv( -1 , sky_scsm_lod-1 );
		}
	else if (button_number==(var)23)
		{
			sky_scsm_lod = minv( 3 , sky_scsm_lod+1 );
		}
	//--------------------------------------------
	//--------------------------------------------
	// fade-out
	else if (button_number==(var)24)
		{
			*sky_scsm_fadeout_start = maxv( 0 , *sky_scsm_fadeout_start-0.01 );
		}
	else if (button_number==(var)25)
		{
			*sky_scsm_fadeout_start = minv( 1 , *sky_scsm_fadeout_start+0.01 );
		}
}


void		Sky_Scsm_Apply()
{
	wait(3);
	if (proc_status(Sky_Scsm_Apply)>1)	return;
	
	Scsm_Close();
	wait_for(Scsm_Close);
	
	scsm_resolution 	= sky_scsm_resolution;
	scsm_areasize		= sky_scsm_areasize;
	scsm_lod				= sky_scsm_lod;
	wait(3);
	
	Scsm_Start();		// auto calculates depth that suits new resolution, but can be modified afterwards
	
	wait(10);
}


//void		Sky_Scsm_SetBlurType()
//{
//	wait(3);
//	effect_load(sc_mtl_shadowBlur, NULL);
//	wait(3);
//	
//	if (*sky_scsm_blurtype == (var)0)
//		{
//			effect_load(sc_mtl_shadowBlur, "sc_esmDepthBlurN_MB.fx");
//		}
//	else if (*sky_scsm_blurtype == (var)1)
//		{
//			effect_load(sc_mtl_shadowBlur, "sc_esmDepthBlurB_MB.fx");
//		}
//	else if (*sky_scsm_blurtype == (var)2)
//		{
//			effect_load(sc_mtl_shadowBlur, "sc_esmDepthBlurP_MB.fx");
//		}
//}


////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif