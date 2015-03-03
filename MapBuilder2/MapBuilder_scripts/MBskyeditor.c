#ifndef MBskyeditor_c
#define MBskyeditor_c


////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


void 	Sky_Init()
{
//	UIMode = (var)UIMODE_SKY;
	
	// pointer definition
	sky_skycurve	= &sky_curve;
	sky_skyclip		= &sky_clip;
	
	sky_sunpan		= &sun_angle.pan;
	sky_suntilt		= &sun_angle.tilt;
	sky_sunlight	= &sun_light;
	
	sky_camclipnear= &camera.clip_near;
	sky_camclipfar	= &camera.clip_far;
	sky_camamb		= &camera.ambient;
	sky_camarc		= &camera.arc;
	
	sky_lod1			= &sky_loddistance1;
	sky_lod2			= &sky_loddistance2;
	sky_lod3			= &sky_loddistance3;
	
	sky_fogstart	= &camera.fog_start;
	sky_fogend		= &camera.fog_end;
	sky_fogcol		= &fog_color;
	
	sky_shadran		= &shadow_range;
	sky_shadalpha	= &mat_shadow.alpha;
	
	sky_fog_minheight			= &sky_hfogminheight;
	sky_fog_maxheight			= &sky_hfogmaxheight;
	sky_fog_heightstrength	= &sky_hfogheightstrength;
	sky_fog_heightblending	= &sky_hfogheightblending;
	sky_fog_heightdensity   = &sky_hfogheightdensity;
	sky_fog_heightmin			= &sky_hfogheightmin;
	
	sky_skylight_start		= &sky_skylightstart;
	sky_skylight_end			= &sky_skylightend;
	sky_skylight_height		= &sky_skylightheight;
	
	Sky_Panel_Init();
	
	if (shadow_stencil==(var)8)
		{
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
				
			sky_pssm_numsplits	= pssm_numsplits;					// 0..4									2 arrow buttons plus an apply button
			sky_pssm_auto			= 1 - *sky_pssm_manual;
			
			sky_pssm_esm			= &pssm_esm;			
			sky_pssm_blurfactor	= &pssm_blurfactor;			
			sky_pssm_blurtype		= &pssm_blurtype;				
			sky_pssm_fogfactor	= &pssm_fogfactor;

			Sky_PssmPanel_Init();
		}
	else if (shadow_stencil==(var)-1)
		{
			sky_scsm_maxdepth				= &scsm_maxdepth;
			sky_scsm_blurpixel			= &scsm_blurpixel;
			sky_scsm_blurtype				= &scsm_blurtype;
			sky_scsm_vsm_epsilon			= &scsm_esm;
			sky_scsm_vsm_epsilon_obj	= &scsm_esm_obj;
			sky_scsm_shadowalpha			= &scsm_shadowalpha;
			sky_scsm_lightmapalpha		= &scsm_lightmapalpha;
			sky_scsm_shadowpixeloffset = &scsm_shadowpixeloffset;			
			
			sky_scsm_resolution			= scsm_resolution;
			sky_scsm_areasize				= scsm_areasize;						
			sky_scsm_lod					= scsm_lod;
			
			sky_scsm_fadeout_start		= &scsm_fadeout_start;
			
			Sky_ScsmPanel_Init();
		}
}


void 	Sky_Close()
{
	//---
	
	Sky_SetSkyClose();
	wait_for(Sky_SetSkyClose);
	
	//---
	
	Sky_ChangeColorClose();
	
	//---
	
	if (sky_colpan1)
		{
			ptr_remove(sky_colpan1);
			sky_colpan1 = NULL;
		}
	if (sky_colpan2)
		{
			ptr_remove(sky_colpan2);
			sky_colpan2 = NULL;
		}
	if (sky_colpan3)
		{
			ptr_remove(sky_colpan3);
			sky_colpan3 = NULL;
		}
	if (sky_colpan4)
		{
			ptr_remove(sky_colpan4);
			sky_colpan4 = NULL;
		}
	if (sky_colpan5)
		{
			ptr_remove(sky_colpan5);
			sky_colpan5 = NULL;
		}
	if (sky_colpan6)
		{
			ptr_remove(sky_colpan6);
			sky_colpan6 = NULL;
		}
	
	if (sky_lowpanel)
		{
			ptr_remove(sky_lowpanel);
			sky_lowpanel = NULL;
		}
	
	//---
	
	Sky_PssmPanel_Close();
	
	//---
	
	Sky_ScsmPanel_Close();
	
	//---
}


////////////////////////////////////////////////////////////////////////////////////////////////////////


void	Sky_Panel_Init()
{
	sky_lowpanel = pan_create( panel_image2 , 11 );
	set( sky_lowpanel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	sky_lowpanel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	sky_lowpanel.pos_y 	= ui_submenupanel_y;								// 256;
	sky_lowpanel.red		= panel_font_r;
	sky_lowpanel.green	= panel_font_g;
	sky_lowpanel.blue		= panel_font_b;
	sky_lowpanel.event	= ui_submenu_panelmove;
	
	//string 1 - title
	pan_setstring(sky_lowpanel, 0,	15,10,	panel_font2,			str_create("SKY-SUN-FOG-CAMERA-LOD SETTINGS") );
	
	//string 2-6 - skies
	pan_setstring(sky_lowpanel, 0,	15,40,	panel_font,			str_create("Model") );	
	pan_setstring(sky_lowpanel, 0,	15,60,	panel_font,			str_create("Cube") );	
	pan_setstring(sky_lowpanel, 0,	15,80,	panel_font,			str_create("Cylinder") );	
	pan_setstring(sky_lowpanel, 0,	15,100,	panel_font,			str_create("Dome 1") );	
	pan_setstring(sky_lowpanel, 0,	15,120,	panel_font,			str_create("Dome 2") );	
	
	//button 1,2 - model layer 0
	pan_setbutton(sky_lowpanel, 0,	1, 	100+22,40,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Sky_SetSky			,NULL,NULL); 	
	pan_setbutton(sky_lowpanel, 0,	1, 	156+22,40,		ss_del_on_bmp,ss_del_off_bmp,ss_del_over_bmp,ss_del_over_bmp,		Sky_RemoveModel	,NULL,NULL); 	
	
	//button 3,4 - cube layer 0
	pan_setbutton(sky_lowpanel, 0,	1, 	100+22,60,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Sky_SetSky			,NULL,NULL); 	
	pan_setbutton(sky_lowpanel, 0,	1, 	156+22,60,		ss_del_on_bmp,ss_del_off_bmp,ss_del_over_bmp,ss_del_over_bmp,		Sky_RemoveCube		,NULL,NULL); 	
	
	//button 5,6 - cylinder layer 1
	pan_setbutton(sky_lowpanel, 0,	1, 	100+22,80,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Sky_SetSky			,NULL,NULL); 	
	pan_setbutton(sky_lowpanel, 0,	1, 	156+22,80,		ss_del_on_bmp,ss_del_off_bmp,ss_del_over_bmp,ss_del_over_bmp,		Sky_RemoveCyl		,NULL,NULL); 	
	
	//button 7,8 - dome 1 layer 2
	pan_setbutton(sky_lowpanel, 0,	1, 	100+22,100,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Sky_SetSky			,NULL,NULL); 	
	pan_setbutton(sky_lowpanel, 0,	1, 	156+22,100,		ss_del_on_bmp,ss_del_off_bmp,ss_del_over_bmp,ss_del_over_bmp,		Sky_RemoveDome1	,NULL,NULL); 	
	
	//button 9,10 - dome 2 layer 3
	pan_setbutton(sky_lowpanel, 0,	1, 	100+22,120,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Sky_SetSky			,NULL,NULL); 	
	pan_setbutton(sky_lowpanel, 0,	1, 	156+22,120,		ss_del_on_bmp,ss_del_off_bmp,ss_del_over_bmp,ss_del_over_bmp,		Sky_RemoveDome2	,NULL,NULL); 	
	
	//slider 1 - sky curve
	pan_setdigits(sky_lowpanel, 0,	15,145,	"Sky curve: %0.2f",	panel_font,					1,			*sky_skycurve);
	pan_setslider(sky_lowpanel, 0,	100+22,145+2,	horiz_slider_bmp,slider_knob_bmp,			0,3,			*sky_skycurve); 	// 1.5-2 okay
	//slider 2 - sky clip
	pan_setdigits(sky_lowpanel, 0,	15,165,	"Sky clip: %0.2f",	panel_font,					1,			*sky_skyclip);
	pan_setslider(sky_lowpanel, 0,	100+22,165+2,	horiz_slider_bmp,slider_knob_bmp,			0,45,			*sky_skyclip); 	// 15-20 okay
	
	//slider 3 - sun pan
	pan_setdigits(sky_lowpanel, 0,	15,190,	"Sun pan: %0.f",	panel_font,						1,			*sky_sunpan);
	pan_setslider(sky_lowpanel, 0,	100+22,190+2,	horiz_slider_bmp,slider_knob_bmp,			0,359,		*sky_sunpan); 	// 315 default
	//slider 4 - sun tilt
	pan_setdigits(sky_lowpanel, 0,	15,210,	"Sun tilt: %0.f",	panel_font,						1,			*sky_suntilt);
	pan_setslider(sky_lowpanel, 0,	100+22,210+2,	horiz_slider_bmp,slider_knob_bmp,			0,90,			*sky_suntilt); 	// 60 default
//		//slider X - sun light - not necessary
//		pan_setdigits(sky_lowpanel, 0,	15,230,	"Sun light: %0.f",	panel_font,				1,			*sky_sunlight);
//		pan_setslider(sky_lowpanel, 0,	100,230+2,	horiz_slider_bmp,slider_knob_bmp,		0,100,		*sky_sunlight); 	// 60 default
	
	//slider 5 - camera clip near
	pan_setdigits(sky_lowpanel, 0,	15,235,	"Clip near: %0.f",	panel_font,					1,			*sky_camclipnear);
	pan_setslider(sky_lowpanel, 0,	100+22,235+2,	horiz_slider_bmp,slider_knob_bmp,			0,64,			*sky_camclipnear); 	// 8 default
	//slider 6 - camera clip far
	pan_setdigits(sky_lowpanel, 0,	15,255,	"Clip far: %0.f",	panel_font,						1,			*sky_camclipfar);
	pan_setslider(sky_lowpanel, 0,	100+22,255+2,	horiz_slider_bmp,slider_knob_bmp,	1000,50000,			*sky_camclipfar); 	// sky system dependent
//		//slider X - camera ambient - not necessary
//		pan_setdigits(sky_lowpanel, 0,	15,275,	"Ambient: %0.f",	panel_font,					1,			*sky_camamb);
//		pan_setslider(sky_lowpanel, 0,	100,275+2,	horiz_slider_bmp,slider_knob_bmp,		0,100,		*sky_camamb); 	// 50
	
	// camera arc slider place at 275
	
	//slider 7,8,9 - lod 1-2-3 in quants
	pan_setdigits(sky_lowpanel, 0,	15,300,	"LOD1: %0.f",	panel_font,								1,			*sky_lod1);
	pan_setslider(sky_lowpanel, 0,	100+22,300+2,	horiz_slider_bmp,slider_knob_bmp,		100,*sky_lod2,			*sky_lod1); 		// 750 default
	pan_setdigits(sky_lowpanel, 0,	15,320,	"LOD2: %0.f",	panel_font,								1,			*sky_lod2);
	pan_setslider(sky_lowpanel, 0,	100+22,320+2,	horiz_slider_bmp,slider_knob_bmp,		1000,*sky_lod3,			*sky_lod2); 	// 2000 default
	pan_setdigits(sky_lowpanel, 0,	15,340,	"LOD3: %0.f",	panel_font,								1,			*sky_lod3);
	pan_setslider(sky_lowpanel, 0,	100+22,340+2,	horiz_slider_bmp,slider_knob_bmp,		1500,10000,			*sky_lod3); 			// 4000 default
	
	//slider 10,11 - fog start-end
	pan_setdigits(sky_lowpanel, 0,	15,365,	"Fog start: %0.f",	panel_font,						1,							*sky_fogstart);
	pan_setslider(sky_lowpanel, 0,	100+22,365+2,	horiz_slider_bmp,slider_knob_bmp,				0,*sky_fogend,				*sky_fogstart); // =lod1 default
	pan_setdigits(sky_lowpanel, 0,	15,385,	"Fog end: %0.f",	panel_font,						1,								*sky_fogend);
	pan_setslider(sky_lowpanel, 0,	100+22,385+2,	horiz_slider_bmp,slider_knob_bmp,	*sky_fogstart,*sky_camclipfar,	*sky_fogend); // =lod1 default
	
	//button 11,12 - fog color = 0-5
	pan_setdigits(sky_lowpanel, 0,	125+22,405,	"Fog color: %0.f",		panel_font,	1, *sky_fogcol);	
	pan_setbutton(sky_lowpanel, 0,	1, 	105+22,405,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_DecFog, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	192+22,405,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_IncFog, 		NULL, NULL); 
	
	//slider 12 - shadow range
	pan_setdigits(sky_lowpanel, 0,	15,430,	"Shadow rng: %0.f",	panel_font,				1,			*sky_shadran);
	pan_setslider(sky_lowpanel, 0,	100+22,430+2,	horiz_slider_bmp,slider_knob_bmp,			0,100,		*sky_shadran); // =lod2 default	
	
	//------------------------------------------------------------------------------------
	//string 7-13, panels 1-6 - color settings - sky,sun,fog1,fo2,fog3,fog4
	pan_setstring(sky_lowpanel, 0,	15,470,	panel_font,			str_create("Sky") );
	pan_setstring(sky_lowpanel, 0,	15,490,	panel_font,			str_create("Sun") );
	pan_setstring(sky_lowpanel, 0,	90,470,	panel_font,			str_create("Fog1") );
	pan_setstring(sky_lowpanel, 0,	90,490,	panel_font,			str_create("Fog2") );
	pan_setstring(sky_lowpanel, 0,	170,470,	panel_font,			str_create("Fog3") );
	pan_setstring(sky_lowpanel, 0,	170,490,	panel_font,			str_create("Fog4") );
	
	//panel1 - sky
	sky_colpan1 = pan_create( panel_color1 , 12 );
	set( sky_colpan1 , SHOW );					
	sky_colpan1.pos_x = ui_submenupanel_x + 40;						// screen_size.x-256+40;
	sky_colpan1.pos_y = ui_submenupanel_y + 470;						// 256+470;
	bmap_fill( panel_color1_bmp , sky_color , 100 );
	sky_colpan1.bmap = panel_color1_bmp;
	sky_colpan1.event = Sky_ChangeColor;
	
	//panel2 - sun
	sky_colpan2 = pan_create( panel_color2 , 12 );
	set( sky_colpan2 , SHOW );					
	sky_colpan2.pos_x = ui_submenupanel_x + 40;						// screen_size.x-256+40;
	sky_colpan2.pos_y = ui_submenupanel_y + 490;						// 256+490;
	bmap_fill( panel_color2_bmp , sun_color , 100 );
	sky_colpan2.bmap = panel_color2_bmp;
	sky_colpan2.event = Sky_ChangeColor;
	
	//panel3 - fog1
	sky_colpan3 = pan_create( panel_color3 , 12 );
	set( sky_colpan3 , SHOW );					
	sky_colpan3.pos_x = ui_submenupanel_x + 120;						// screen_size.x-256+120;
	sky_colpan3.pos_y = ui_submenupanel_y + 470;						// 256+470;
	bmap_fill( panel_color3_bmp , d3d_fogcolor1 , 100 );
	sky_colpan3.bmap = panel_color3_bmp;
	sky_colpan3.event = Sky_ChangeColor;
	
	//panel4 - fog2
	sky_colpan4 = pan_create( panel_color4 , 12 );
	set( sky_colpan4 , SHOW );					
	sky_colpan4.pos_x = ui_submenupanel_x + 120;						// screen_size.x-256+120;
	sky_colpan4.pos_y = ui_submenupanel_y + 490;						// 256+490;
	bmap_fill( panel_color4_bmp , d3d_fogcolor2 , 100 );
	sky_colpan4.bmap = panel_color4_bmp;
	sky_colpan4.event = Sky_ChangeColor;
	
	//panel5 - fog3
	sky_colpan5 = pan_create( panel_color5 , 12 );
	set( sky_colpan5 , SHOW );					
	sky_colpan5.pos_x = ui_submenupanel_x + 200;						// screen_size.x-256+200;
	sky_colpan5.pos_y = ui_submenupanel_y + 470;						// 256+470;
	bmap_fill( panel_color5_bmp , d3d_fogcolor3 , 100 );
	sky_colpan5.bmap = panel_color5_bmp;
	sky_colpan5.event = Sky_ChangeColor;
	
	//panel6 - fog4
	sky_colpan6 = pan_create( panel_color6 , 12 );
	set( sky_colpan6 , SHOW );					
	sky_colpan6.pos_x = ui_submenupanel_x + 200;						// screen_size.x-256+200;
	sky_colpan6.pos_y = ui_submenupanel_y + 490;						// 256+490;
	bmap_fill( panel_color6_bmp , d3d_fogcolor4 , 100 );
	sky_colpan6.bmap = panel_color6_bmp;	
	sky_colpan6.event = Sky_ChangeColor;
	//------------------------------------------------------------------------
	
	//slider 13 - camera arc
	pan_setdigits(sky_lowpanel, 0,	15,275,	"Camera arc: %0.f",	panel_font,					1,			*sky_camarc);
	pan_setslider(sky_lowpanel, 0,	100+22,275+2,	horiz_slider_bmp,slider_knob_bmp,		45,120,		*sky_camarc); 	// 50
	
	//button 13-38
	// sky curve, clip
	pan_setbutton(sky_lowpanel, 0,	1, 	110,145,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,145,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	110,165,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,165,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 	
	// sun pan, tilt
	pan_setbutton(sky_lowpanel, 0,	1, 	110,190,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,190,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	110,210,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,210,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	// camera clip near, far
	pan_setbutton(sky_lowpanel, 0,	1, 	110,235,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,235,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	110,255,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,255,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	// lod 1, 2, 3
	pan_setbutton(sky_lowpanel, 0,	1, 	110,300,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,300,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	110,320,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,320,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	110,340,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,340,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	// fog start, end
	pan_setbutton(sky_lowpanel, 0,	1, 	110,365,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,365,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	110,385,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,385,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	// shadow range
	pan_setbutton(sky_lowpanel, 0,	1, 	110,430,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,430,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	// camera arc
	pan_setbutton(sky_lowpanel, 0,	1, 	110,275,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,275,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	
	//slider 14 - mat_shadow alpha
	pan_setdigits(sky_lowpanel, 0,	15,450,	"Shadow alpha: %0.f",	panel_font,				1,			*sky_shadalpha);
	pan_setslider(sky_lowpanel, 0,	100+22,450+2,	horiz_slider_bmp,slider_knob_bmp,			0,100,		*sky_shadalpha); // 	
	pan_setbutton(sky_lowpanel, 0,	1, 	110,450,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_Button, 		NULL, NULL); 
	pan_setbutton(sky_lowpanel, 0,	1, 	230,450,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_Button, 		NULL, NULL); 
	
	//----------------------------------
	var temp1;
   var temp2;
   var temp3;
   while((UIMode==(var)UIMODE_SKY) || (UIMode==(var)UIMODE_FILEMENU2))
		{
			// LOD update
			if (*sky_lod1>=*sky_lod2)	*sky_lod1 = *sky_lod2-10;
			if (*sky_lod2>=*sky_lod3)	*sky_lod2 = *sky_lod3-10;
			temp1 = *sky_lod1*100;
			temp2 = *sky_lod2*100;
			temp3 = *sky_lod3*100;			
			vec_set(d3d_lodfactor,vector( temp1/camera.clip_far , temp2/camera.clip_far , temp3/camera.clip_far )); 
   		
   		clip_particles = d3d_lodfactor.y;
   		
   		// fog limits update
   		if (*sky_fogend>*sky_camclipfar)
   			{
   				*sky_fogend = *sky_camclipfar;
   			}
   		if (*sky_fogstart>*sky_fogend)
   			{
   				*sky_fogstart = *sky_fogend;
   			}
   		pan_setslider(sky_lowpanel, 10,	100+22,365+2,	horiz_slider_bmp,slider_knob_bmp,							0,*sky_fogend,		*sky_fogstart);
   		pan_setslider(sky_lowpanel, 11,	100+22,385+2,	horiz_slider_bmp,slider_knob_bmp,		*sky_fogstart,*sky_camclipfar,	*sky_fogend);
   		
			wait(1);
		}
	
	Sky_Close();
}


/////////////////////////////////////////////////////////////////////////////////


void	Sky_Button(button_number)
{
	// sky curve
	if (button_number==(var)13)
		{
			*sky_skycurve -= 0.01;
			*sky_skycurve = maxv( 0 , *sky_skycurve );
		}
	else if (button_number==(var)14)
		{
			*sky_skycurve += 0.01;
			*sky_skycurve = minv( 3 , *sky_skycurve );
		}
	// sky clip
	else if (button_number==(var)15)
		{
			*sky_skyclip -= 0.01;
			*sky_skyclip = maxv( 0 , *sky_skyclip );
		}
	else if (button_number==(var)16)
		{
			*sky_skyclip += 0.01;
			*sky_skyclip = minv( 45 , *sky_skyclip );
		}
	// sun pan
	else if (button_number==(var)17)
		{
			*sky_sunpan = maxv( 0 , --*sky_sunpan );
		}
	else if (button_number==(var)18)
		{
			*sky_sunpan = minv( 359 , ++*sky_sunpan );
		}
	// sun tilt
	else if (button_number==(var)19)
		{
			*sky_suntilt = maxv( 0 , --*sky_suntilt );
		}
	else if (button_number==(var)20)
		{
			*sky_suntilt = minv( 90 , ++*sky_suntilt );
		}	
	// camera clip near
	else if (button_number==(var)21)
		{
			*sky_camclipnear = maxv( 0 , --*sky_camclipnear );
		}
	else if (button_number==(var)22)
		{
			*sky_camclipnear = minv( 64 , ++*sky_camclipnear );
		}	
	// camera clip far
	else if (button_number==(var)23)
		{
			*sky_camclipfar -= 100;
			*sky_camclipfar = maxv( 1000 , *sky_camclipfar );
		}
	else if (button_number==(var)24)
		{
			*sky_camclipfar += 100;
			*sky_camclipfar = minv( 50000 , *sky_camclipfar );
		}	
	// lod1
	else if (button_number==(var)25)
		{
			*sky_lod1 -= 10;
			*sky_lod1 = maxv( 100 , *sky_lod1 );
		}
	else if (button_number==(var)26)
		{
			*sky_lod1 += 10;
			*sky_lod1 = minv( *sky_lod2 , *sky_lod1 );
		}		
	// lod2
	else if (button_number==(var)27)
		{
			*sky_lod2 -= 10;
			*sky_lod2 = maxv( 1000 , *sky_lod2 );
		}
	else if (button_number==(var)28)
		{
			*sky_lod2 += 10;
			*sky_lod2 = minv( *sky_lod3 , *sky_lod2 );
		}		
	// lod3
	else if (button_number==(var)29)
		{
			*sky_lod3 -= 10;
			*sky_lod3 = maxv( 1500 , *sky_lod3 );
		}
	else if (button_number==(var)30)
		{
			*sky_lod3 += 10;
			*sky_lod3 = minv( 10000 , *sky_lod3 );
		}		
	// fog start
	else if (button_number==(var)31)
		{
			*sky_fogstart -= 10;
			*sky_fogstart = maxv( 0 , *sky_fogstart );
		}
	else if (button_number==(var)32)
		{
			*sky_fogstart += 10;
			*sky_fogstart = minv( *sky_fogend , *sky_fogstart );
		}		
	// fog end
	else if (button_number==(var)33)
		{
			*sky_fogend -= 10;
			*sky_fogend = maxv( *sky_fogstart , *sky_fogend );
		}
	else if (button_number==(var)34)
		{
			*sky_fogend += 10;
			*sky_fogend = minv( *sky_camclipfar , *sky_fogend );
		}		
	// shadow range
	else if (button_number==(var)35)
		{
			*sky_shadran = maxv( 0 , --*sky_shadran );
		}
	else if (button_number==(var)36)
		{
			*sky_shadran = minv( 100 , ++*sky_shadran );
		}	
	// camera arc
	else if (button_number==(var)37)
		{
			*sky_camarc = maxv( 45 , --*sky_camarc );
		}
	else if (button_number==(var)38)
		{
			*sky_camarc = minv( 120 , ++*sky_camarc );
		}		
	// shadow alpha
	else if (button_number==(var)39)
		{
			*sky_shadalpha = maxv( 0 , --*sky_shadalpha );
		}
	else if (button_number==(var)40)
		{
			*sky_shadalpha = minv( 100 , ++*sky_shadalpha );
		}		
}

/////////////////////////////////////////////////////////////////////////////////

void	Sky_DecFog()
{
	*sky_fogcol--;
	if (*sky_fogcol<0)
		{
			*sky_fogcol = 0;
		}
}


void	Sky_IncFog()
{
	*sky_fogcol++;
	if (*sky_fogcol>4)
		{
			*sky_fogcol = 4;
		}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////


void	Sky_ChangeColor(PANEL* panel)
{	
	while (mouse_left)
		{
			wait(1);
		}
	
	// get color from clicked panel - and exit
	if (sky_colchangepanel)
		{				
			if (panel == sky_colpan1)
				{
					*sky_r = sky_color.red;
					*sky_g = sky_color.green;
					*sky_b = sky_color.blue;
				}
			else if (panel == sky_colpan2)
				{
					*sky_r = sun_color.red;
					*sky_g = sun_color.green;
					*sky_b = sun_color.blue;
				}
			else if (panel == sky_colpan3)
				{
					*sky_r = d3d_fogcolor1.red;
					*sky_g = d3d_fogcolor1.green;
					*sky_b = d3d_fogcolor1.blue;
				}
			else if (panel == sky_colpan4)
				{
					*sky_r = d3d_fogcolor2.red;
					*sky_g = d3d_fogcolor2.green;
					*sky_b = d3d_fogcolor2.blue;
				}
			else if (panel == sky_colpan5)
				{
					*sky_r = d3d_fogcolor3.red;
					*sky_g = d3d_fogcolor3.green;
					*sky_b = d3d_fogcolor3.blue;
				}
			else if (panel == sky_colpan6)
				{
					*sky_r = d3d_fogcolor4.red;
					*sky_g = d3d_fogcolor4.green;
					*sky_b = d3d_fogcolor4.blue;
				}
			
			return;
		}		
	
//	sky_r = NULL;
//	sky_g = NULL;
//	sky_b = NULL;
	
	sky_colchangepanel = pan_create( panel_image , 13 );
	set( sky_colchangepanel , SHOW | SHADOW | OVERLAY );					
	sky_colchangepanel.pos_x 	= ui_submenupanel2_x;				// screen_size.x-512;
	sky_colchangepanel.pos_y 	= ui_submenupanel2_y;				// 512;			//256+194;
	sky_colchangepanel.red		= panel_font_r;
	sky_colchangepanel.green	= panel_font_g;
	sky_colchangepanel.blue		= panel_font_b;
	sky_colchangepanel.event	= ui_small_panelmove;
	
	// caller
	if (panel == sky_colpan1)
		{
			sky_r = &sky_color.red;
			sky_g = &sky_color.green;
			sky_b = &sky_color.blue;
			//string 1 - title
			pan_setstring(sky_colchangepanel, 0,	15,10,	panel_font2,			str_create("SKY COLOR") );	
		}
	else if (panel == sky_colpan2)
		{
			sky_r = &sun_color.red;
			sky_g = &sun_color.green;
			sky_b = &sun_color.blue;						
			//string 1 - title
			pan_setstring(sky_colchangepanel, 0,	15,10,	panel_font2,			str_create("SUN COLOR") );							
		}
	else if (panel == sky_colpan3)
		{
			sky_r = &d3d_fogcolor1.red;
			sky_g = &d3d_fogcolor1.green;
			sky_b = &d3d_fogcolor1.blue;
			//string 1 - title
			pan_setstring(sky_colchangepanel, 0,	15,10,	panel_font2,			str_create("FOG-1 COLOR") );	
		}
	else if (panel == sky_colpan4)
		{
			sky_r = &d3d_fogcolor2.red;
			sky_g = &d3d_fogcolor2.green;
			sky_b = &d3d_fogcolor2.blue;
			//string 1 - title
			pan_setstring(sky_colchangepanel, 0,	15,10,	panel_font2,			str_create("FOG-2 COLOR") );	
		}
	else if (panel == sky_colpan5)
		{
			sky_r = &d3d_fogcolor3.red;
			sky_g = &d3d_fogcolor3.green;
			sky_b = &d3d_fogcolor3.blue;
			//string 1 - title
			pan_setstring(sky_colchangepanel, 0,	15,10,	panel_font2,			str_create("FOG-3 COLOR") );	
		}
	else if (panel == sky_colpan6)
		{
			sky_r = &d3d_fogcolor4.red;
			sky_g = &d3d_fogcolor4.green;
			sky_b = &d3d_fogcolor4.blue;
			//string 1 - title
			pan_setstring(sky_colchangepanel, 0,	15,10,	panel_font2,			str_create("FOG-4 COLOR") );	
		}
	
	//button1 -  ok to close
	pan_setbutton(sky_colchangepanel, 0,	1, 	100,220,		s_ok_on_bmp,s_ok_off_bmp,s_ok_over_bmp,s_ok_over_bmp,							Sky_ChangeColorClose			,NULL,NULL); 	
		
	// 3 sliders, 3 digits r-g-b
	pan_setdigits(sky_colchangepanel, 0,	15,40,	"Red: %0.f",	panel_font,							1,			*sky_r);
	pan_setslider(sky_colchangepanel, 0,	112,40+2,	horiz_slider_bmp,slider_knob_bmp,		0,255,		*sky_r);
	pan_setdigits(sky_colchangepanel, 0,	15,70,	"Green: %0.f",	panel_font,							1,			*sky_g);
	pan_setslider(sky_colchangepanel, 0,	112,70+2,	horiz_slider_bmp,slider_knob_bmp,		0,255,		*sky_g);
	pan_setdigits(sky_colchangepanel, 0,	15,100,	"Blue: %0.f",	panel_font,							1,			*sky_b);
	pan_setslider(sky_colchangepanel, 0,	112,100+2,	horiz_slider_bmp,slider_knob_bmp,		0,255,		*sky_b);
	
	//button 2,3	4,5	6,7
	pan_setbutton(sky_colchangepanel, 0,	1, 	100,40,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_ChangeColor_Button, 		NULL, NULL); 
	pan_setbutton(sky_colchangepanel, 0,	1, 	220,40,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_ChangeColor_Button, 		NULL, NULL); 
	pan_setbutton(sky_colchangepanel, 0,	1, 	100,70,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_ChangeColor_Button, 		NULL, NULL); 
	pan_setbutton(sky_colchangepanel, 0,	1, 	220,70,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_ChangeColor_Button, 		NULL, NULL); 
	pan_setbutton(sky_colchangepanel, 0,	1, 	100,100,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_ChangeColor_Button, 		NULL, NULL); 
	pan_setbutton(sky_colchangepanel, 0,	1, 	220,100,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_ChangeColor_Button, 		NULL, NULL); 
	
	if ((panel == sky_colpan1) && (shadow_stencil == (var)-1))
		{			
			//slider X - sky_skylight_start
			pan_setdigits(sky_colchangepanel, 0,	15,160,	"Start: %0.f",	panel_font,						1,					*sky_skylight_start);
			pan_setslider(sky_colchangepanel, 0,	112,160+2,	horiz_slider_bmp,slider_knob_bmp,		0,20000,			*sky_skylight_start); 				
			//slider X - sky_skylight_end
			pan_setdigits(sky_colchangepanel, 0,	15,190,	"End: %0.f",	panel_font,						1,					*sky_skylight_end);
			pan_setslider(sky_colchangepanel, 0,	112,190+2,	horiz_slider_bmp,slider_knob_bmp,		5000,25000,		*sky_skylight_end); 			
			//slider X - sky_skylight_height
			pan_setdigits(sky_colchangepanel, 0,	15,130,	"Height: %0.2f",	panel_font,					1,					*sky_skylight_height);
			pan_setslider(sky_colchangepanel, 0,	112,130+2,	horiz_slider_bmp,slider_knob_bmp,		0,1,				*sky_skylight_height); 	
			//button 8,9	10,11
			pan_setbutton(sky_colchangepanel, 0,	1, 	100,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SkyLight_Button, 		NULL, NULL); 
			pan_setbutton(sky_colchangepanel, 0,	1, 	220,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SkyLight_Button, 		NULL, NULL); 
			pan_setbutton(sky_colchangepanel, 0,	1, 	100,190,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SkyLight_Button, 		NULL, NULL); 
			pan_setbutton(sky_colchangepanel, 0,	1, 	220,190,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SkyLight_Button, 		NULL, NULL); 
			pan_setbutton(sky_colchangepanel, 0,	1, 	100,130,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SkyLight_Button, 		NULL, NULL); 
			pan_setbutton(sky_colchangepanel, 0,	1, 	220,130,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SkyLight_Button, 		NULL, NULL); 
		}
	else if (panel == sky_colpan2)
		{
								
			//slider X - sun light
			pan_setdigits(sky_colchangepanel, 0,	15,160,	"Sun light: %0.f",	panel_font,				1,			*sky_sunlight);
			pan_setslider(sky_colchangepanel, 0,	112,160+2,	horiz_slider_bmp,slider_knob_bmp,		0,100,	*sky_sunlight); 	// 60 default	-> 100			
			//slider X - camera ambient
			pan_setdigits(sky_colchangepanel, 0,	15,190,	"Ambient: %0.f",	panel_font,					1,			*sky_camamb);
			pan_setslider(sky_colchangepanel, 0,	112,190+2,	horiz_slider_bmp,slider_knob_bmp,		0,100,	*sky_camamb); 		// 50 -> 30			
			//button 8,9	10,11
			pan_setbutton(sky_colchangepanel, 0,	1, 	100,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SunLight_Button, 		NULL, NULL); 
			pan_setbutton(sky_colchangepanel, 0,	1, 	220,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SunLight_Button, 		NULL, NULL); 
			pan_setbutton(sky_colchangepanel, 0,	1, 	100,190,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SunLight_Button, 		NULL, NULL); 
			pan_setbutton(sky_colchangepanel, 0,	1, 	220,190,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SunLight_Button, 		NULL, NULL); 
		}
	else if ((shadow_stencil < 0) && ((panel == sky_colpan3) || (panel == sky_colpan4) || (panel == sky_colpan5) || (panel == sky_colpan6)))
		{
			sky_heightfogpanel = pan_create( panel_image , 13 );
			set( sky_heightfogpanel , SHOW | SHADOW | OVERLAY );					
			sky_heightfogpanel.pos_x 	= ui_submenupanel2_x - 256;		// screen_size.x-512;
			sky_heightfogpanel.pos_y 	= ui_submenupanel2_y;				// 512;			//256+194;
			sky_heightfogpanel.red		= panel_font_r;
			sky_heightfogpanel.green	= panel_font_g;
			sky_heightfogpanel.blue		= panel_font_b;
//			sky_heightfogpanel.event	= ui_small_panelmove;				// attached to actual sky_colchangepanel
			
			//string 1 - title
			pan_setstring(sky_heightfogpanel, 0,	15,10,	panel_font2,			str_create("HEIGHT FOG") );
			
			//slider 1 - sky_fog_minheight
			pan_setdigits(sky_heightfogpanel, 0,	15,40,	"Min height: %0.f",	panel_font,				1,				*sky_fog_minheight);
			pan_setslider(sky_heightfogpanel, 0,	112,40+2,	horiz_slider_bmp,slider_knob_bmp,		-500,1000,	*sky_fog_minheight); 	
			//slider 2 - sky_fog_maxheight
			pan_setdigits(sky_heightfogpanel, 0,	15,70,	"Max height: %0.f",	panel_font,				1,				*sky_fog_maxheight);
			pan_setslider(sky_heightfogpanel, 0,	112,70+2,	horiz_slider_bmp,slider_knob_bmp,		0,3000,		*sky_fog_maxheight); 			
			//slider 3 - sky_fog_heightstrength
			pan_setdigits(sky_heightfogpanel, 0,	15,110,	"Strength: %0.2f",	panel_font,				1,				*sky_fog_heightstrength);
			pan_setslider(sky_heightfogpanel, 0,	112,110+2,	horiz_slider_bmp,slider_knob_bmp,		0,1,			*sky_fog_heightstrength); 		
			//slider 4 - sky_fog_heightblending
			pan_setdigits(sky_heightfogpanel, 0,	15,140,	"Fade-in: %0.1f",	panel_font,					1,				*sky_fog_heightblending);
			pan_setslider(sky_heightfogpanel, 0,	112,140+2,	horiz_slider_bmp,slider_knob_bmp,		0,16,			*sky_fog_heightblending);
			//slider 5 - sky_fog_heightdensity
			pan_setdigits(sky_heightfogpanel, 0,	15,170,	"Density: %0.2f",	panel_font,					1,				*sky_fog_heightdensity);
			pan_setslider(sky_heightfogpanel, 0,	112,170+2,	horiz_slider_bmp,slider_knob_bmp,		0,1,			*sky_fog_heightdensity);
			//slider 6 - sky_fog_heightmin
			pan_setdigits(sky_heightfogpanel, 0,	15,210,	"Min value: %0.2f",	panel_font,				1,				*sky_fog_heightmin);
			pan_setslider(sky_heightfogpanel, 0,	112,210+2,	horiz_slider_bmp,slider_knob_bmp,		0,0.5,		*sky_fog_heightmin);
			
			//button 1,2	3,4	5,6	7,8	9,10	11,12
			pan_setbutton(sky_heightfogpanel, 0,	1, 	100,40,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	220,40,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	100,70,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	220,70,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	100,110,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	220,110,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	100,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	220,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	100,170,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	220,170,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	100,210,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_HeightFog_Button, 		NULL, NULL); 
			pan_setbutton(sky_heightfogpanel, 0,	1, 	220,210,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_HeightFog_Button, 		NULL, NULL); 
		}
	
	while(sky_colchangepanel)
		{
			
//			bmap_fill( panel.bmap , vector(*sky_b,*sky_g,*sky_r) , 100 );	
			
			if (panel == sky_colpan1)
				{
					bmap_fill( panel_color1_bmp , sky_color , 100 );
					sky_colpan1.bmap = panel_color1_bmp;
					
					// sky light color
					sky_skylightcolor[0] = sky_color.red / 255;
					sky_skylightcolor[1] = sky_color.green / 255;
					sky_skylightcolor[2] = sky_color.blue / 255;
				}
			else if (panel == sky_colpan2)
				{
					bmap_fill( panel_color2_bmp , sun_color , 100 );
					sky_colpan2.bmap = panel_color2_bmp;					
				}
			else if (panel == sky_colpan3)
				{
					bmap_fill( panel_color3_bmp , d3d_fogcolor1 , 100 );
					sky_colpan3.bmap = panel_color3_bmp;
				}
			else if (panel == sky_colpan4)
				{
					bmap_fill( panel_color4_bmp , d3d_fogcolor2 , 100 );
					sky_colpan4.bmap = panel_color4_bmp;
				}
			else if (panel == sky_colpan5)
				{
					bmap_fill( panel_color5_bmp , d3d_fogcolor3 , 100 );
					sky_colpan5.bmap = panel_color5_bmp;
				}
			else if (panel == sky_colpan6)
				{
					bmap_fill( panel_color6_bmp , d3d_fogcolor4 , 100 );
					sky_colpan6.bmap = panel_color6_bmp;
				}	
				
			wait(1);
		}
}


void		Sky_ChangeColorClose()
{		
	wait(3);
	
	if (sky_colchangepanel)
		{
			ptr_remove(sky_colchangepanel);	
			sky_colchangepanel = NULL;
		}
	
	if (sky_heightfogpanel)
		{
			ptr_remove(sky_heightfogpanel);	
			sky_heightfogpanel = NULL;
		}
}


void		Sky_ChangeColor_Button(button_number)
{
	// sky r
	if (button_number==(var)2)
		{
			*sky_r = maxv( 0 , --*sky_r );
		}
	else if (button_number==(var)3)
		{
			*sky_r = minv( 255 , ++*sky_r );
		}
	// sky g
	else if (button_number==(var)4)
		{
			*sky_g = maxv( 0 , --*sky_g );
		}
	else if (button_number==(var)5)
		{
			*sky_g = minv( 255 , ++*sky_g );
		}
	// sky b
	else if (button_number==(var)6)
		{
			*sky_b = maxv( 0 , --*sky_b );
		}
	else if (button_number==(var)7)
		{
			*sky_b = minv( 255 , ++*sky_b );
		}
}


void		Sky_SunLight_Button(button_number)
{
	// sky_sunlight
	if (button_number==(var)8)
		{
			*sky_sunlight = maxv( 0 , --*sky_sunlight );			
		}
	else if (button_number==(var)9)
		{
			*sky_sunlight = minv( 100 , ++*sky_sunlight );
		}
	// sky_camamb
	else if (button_number==(var)10)
		{
			*sky_camamb = maxv( 0 , --*sky_camamb );
		}
	else if (button_number==(var)11)
		{
			*sky_camamb = minv( 100 , ++*sky_camamb );
		}
}


void		Sky_SkyLight_Button(button_number)
{
	// sky_skylight_start
	if (button_number==(var)8)
		{
			*sky_skylight_start = maxv( 0 , *sky_skylight_start - 100 );			
		}
	else if (button_number==(var)9)
		{
			*sky_skylight_start = minv( 20000 , *sky_skylight_start + 100 );
		}
	// sky_skylight_end
	else if (button_number==(var)10)
		{
			*sky_skylight_end = maxv( 5000 , *sky_skylight_end - 100 );
		}
	else if (button_number==(var)11)
		{
			*sky_skylight_end = minv( 25000 , *sky_skylight_end + 100 );
		}	
	// sky_skylight_height
	else if (button_number==(var)12)
		{
			*sky_skylight_height = maxv( 0 , *sky_skylight_height - 0.01 );
		}
	else if (button_number==(var)13)
		{
			*sky_skylight_height = minv( 1 , *sky_skylight_height + 0.01 );
		}
}


void		Sky_HeightFog_Button(button_number)
{
	// sky_fog_minheight
	if (button_number==(var)1)
		{
			*sky_fog_minheight = maxv( -500 , *sky_fog_minheight - 10 );
		}
	else if (button_number==(var)2)
		{
			*sky_fog_minheight = minv( 1000 , *sky_fog_minheight + 10 );
		}
	// sky_fog_minheight
	else if (button_number==(var)3)
		{
			*sky_fog_maxheight = maxv( 0 , *sky_fog_maxheight - 10 );
		}
	else if (button_number==(var)4)
		{
			*sky_fog_maxheight = minv( 3000 , *sky_fog_maxheight + 10 );
		}
	// sky_fog_heightstrength
	else if (button_number==(var)5)
		{
			*sky_fog_heightstrength = maxv( 0 , *sky_fog_heightstrength - 0.01 );
		}
	else if (button_number==(var)6)
		{
			*sky_fog_heightstrength = minv( 1 , *sky_fog_heightstrength + 0.01 );
		}
	// sky_fog_heightblending
	else if (button_number==(var)7)
		{
			*sky_fog_heightblending = maxv( 0 , *sky_fog_heightblending - 0.1 );
		}
	else if (button_number==(var)8)
		{
			*sky_fog_heightblending = minv( 16 , *sky_fog_heightblending + 0.1 );
		}
	// sky_fog_heightdensity
	else if (button_number==(var)9)
		{
			*sky_fog_heightdensity = maxv( 0 , *sky_fog_heightdensity - 0.01 );
		}
	else if (button_number==(var)10)
		{
			*sky_fog_heightdensity = minv( 1 , *sky_fog_heightdensity + 0.01 );
		}
	// sky_fog_heightmin
	else if (button_number==(var)11)
		{
			*sky_fog_heightmin = maxv( 0 , *sky_fog_heightmin - 0.01 );
		}
	else if (button_number==(var)12)
		{
			*sky_fog_heightmin = minv( 0.5 , *sky_fog_heightmin + 0.01 );
		}
	
}


///////////////////////////////////////////////////////////////////////////////////////


void		Sky_RemoveModel()
{
	wait(3);
	if (skyModel)     
		{
			ptr_remove(skyModel);
			skyModel			= NULL;
			str_cpy( skyModelSkin , "-" );
		}
}


void		Sky_RemoveCube()
{
	wait(3);
	if (skyCube)     
		{
			ptr_remove(skyCube);
			skyCube			= NULL;
		}
}


void		Sky_RemoveCyl()
{
	wait(3);
	if (skyCylinder) 
		{
			ptr_remove(skyCylinder);
			skyCylinder		= NULL;
		}
}


void		Sky_RemoveDome1()
{
	wait(3);
	if (skyClouds1)  
		{
			ptr_remove(skyClouds1);
			skyClouds1		= NULL;
		}
}


void		Sky_RemoveDome2()
{
	wait(3);
	if (skyClouds2)  
		{
			ptr_remove(skyClouds2);
			skyClouds2		= NULL;
		}
}


///////////////////////////////////////////////////////////////////////////////


// skies\\*.dds for flats or *.tga for cubes
void		Sky_SetSky(var callbutton)
{
	wait(3);
	while (mouse_left)
		{
			wait(1);
		}
	
	// type init - default sky image with default settings
	// model
	if (callbutton == (var)1)
		{
			if (!skyModel)			
				{
					skyModel = ent_createlayer( skyModelName , SKY | SCENE | SHOW , 0 );	
					var actualmapsize = abs(TerrHmp_GetGroundMaxX()) * 2;
					vec_scale( skyModel.scale_x , actualmapsize/1000 );
					skyModel.z = 1024;	//512+128  +128;
					set(skyModel,TRANSLUCENT);
					skyModel.alpha = 90;		
					str_cpy(skyModelSkin,skyDefaultModelSkin);		
					skyModel.material = mat_sky;		
					wait(3);
				}
		}
	// cube
	else if (callbutton == (var)3)
		{
			if (!skyCube)			
				{
					skyCube = ent_createlayer( skyCubeName , SKY | CUBE | SHOW , 0 );	
					skyCube.z = -16;
					set(skyCube,TRANSLUCENT);
					skyCube.alpha = 90;
					skyCube.material = mat_sky;	
					wait(3);
				}			
		}
	// cylinder
	else if (callbutton == (var)5)
		{
			if (!skyCylinder)			
				{
					skyCylinder = ent_createlayer( skyCylinderName , SKY | CYLINDER | SHOW , 1 );	
					skyCylinder.scale_x = 0.15;		// 1 = put once around horizont
					skyCylinder.pan = 0;					// can be rotated
					skyCylinder.tilt = -17.5;			// vertical position 
					set(skyCylinder,TRANSLUCENT);
					skyCylinder.alpha = 45;
					skyCylinder.material = mat_sky;
					wait(3);
				}
		}
	// dome-1
	else if (callbutton == (var)7)
		{
			if (!skyClouds1) 
				{
					skyClouds1 = ent_createlayer( skyClouds1Name , SKY | DOME | SHOW , 2 );
					skyClouds1.scale_x = 2;		
					skyClouds1.u = 0.5;
					skyClouds1.v = 0.7;
					set(skyClouds1,TRANSLUCENT);
					skyClouds1.alpha = 90;	
					skyClouds1.material = mat_sky;
					wait(3);				
				}
		}
	// dome-2
	else if (callbutton == (var)9)
		{
			if (!skyClouds2)
				{
					skyClouds2 = ent_createlayer( skyClouds2Name , SKY | DOME | SHOW , 3 );	
					skyClouds2.scale_x = 3;	
					skyClouds2.u = 2;
					skyClouds2.v = 3;
					set(skyClouds2,TRANSLUCENT);
					skyClouds2.alpha = 90;	
					skyClouds2.material = mat_sky;
					wait(3);
				}
		}
	else
		{
			return;
		}
	//----------------------------------------------------------------
	// obstacle model filename initialization 
sys_marker("sky");	
	int arraysize = (int)SKY_MAXPERSET;
	sky_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );
	int i;	
	// init filename string array elements
	for(i=0;i<max_sky_items;i++)
		{
			sky_filenames[i] = str_create("");
		}	
sys_marker(NULL);		
	//-----------------------------------------------------------------
	// determine how many image files are in the folder, 
	// divide it by 8, and the result is the number of sets
	sky_set = 1;
	max_sky_sets = SKY_MAXSET;
	skies_txt = txt_create( (max_sky_sets*max_sky_items) , 0 );						// create 10*8=80
	for(i=0;i<max_sky_sets*max_sky_items;i++)
		{
			str_cpy( (skies_txt->pstring)[i] , "" );										// empty by default
		}
	i = 0;
	//-----------------------------------------------------------------
	i = Txt_For_Dir_PlusMod(skies_txt,(max_sky_sets*max_sky_items),sky_foldername,"*.DDS|*.TGA|*.BMP|*.PCX");
//	if (callbutton<(var)4)
//		{
//			txt_for_dir(skies_txt, "skies\\*.tga"); 										// file extension	
//		}
//	else
//		{
//			txt_for_dir(skies_txt, "skies\\*.dds"); 										// file extension	
//		}	
//	//-----------------------------------------------------------------
//	while (str_len((skies_txt->pstring)[i]) > 0)
//		{
//			i++;
//		}

//	// remove temp text
//	if (skies_txt!=NULL)
//		{
//			for (i = 0; i < skies_txt->strings; i++) 
//				if ((skies_txt->pstring)[i]) 
//					ptr_remove((skies_txt->pstring)[i]);
//			ptr_remove(skies_txt);
//			skies_txt = NULL;
//		}
//	
//	i = txt_for_dir_plus(skies_txt,sky_foldername,"*.DDS|*.TGA");	
		
	max_sky_sets = integer(i/max_sky_items);
	if ( fraction(i/max_sky_items) > 0)
		{
			max_sky_sets++;		// allow to display not complete sets too
		}
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	for(i=0;i<max_sky_items;i++)
		{		
			str_cpy( sky_filenames[i] , "" );											// (or obstacle_foldername) subfolder
			str_cat( sky_filenames[i] , (skies_txt->pstring)[i] );			// filename with extension
			if ( str_cmpi( sky_filenames[i] , "" ) )								// (or obstacle_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( sky_filenames[i] , "-" );
				}
		}	
	//------------------------------------------------------------------
	// initial value settings	
	for(i=0;i<max_sky_items;i++)
		{
			sky_switch_type[i] = 0;
		}
	sky_switch_type[sky_selected] = 1;
	//-------------------------------------------------------------------
	// init GUI panel
//	int i;
	STRING* temp_string = str_create("");
	
	sky_skypanel = pan_create( panel_image2 , 13 );
	set( sky_skypanel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	sky_skypanel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	sky_skypanel.pos_y 	= ui_submenupanel_y;								// 256;
	sky_skypanel.red		= panel_font_r;
	sky_skypanel.green	= panel_font_g;
	sky_skypanel.blue		= panel_font_b;
	sky_skypanel.event	= ui_submenu_panelmove;
	
	reset(sky_lowpanel, SHOW);
	reset(sky_colpan1, SHOW);
	reset(sky_colpan2, SHOW);
	reset(sky_colpan3, SHOW);
	reset(sky_colpan4, SHOW);
	reset(sky_colpan5, SHOW);
	reset(sky_colpan6, SHOW);
	
	//string 1
	pan_setstring(sky_skypanel, 0,	15,10,	panel_font2,			str_create("SKY ENTITY SETTINGS") );
	//string 2
	pan_setstring(sky_skypanel, 0,	15,40,	panel_font,			str_create(" ") );
	
	//strings 3-10 - sky item numbers 1-8
	for(i=0;i<max_sky_items;i++)
		{
			str_for_int( temp_string , (i+1) );
			pan_setstring( sky_skypanel, 0,	35,340+i*15,	panel_font,			str_create( temp_string ) );
		}
	
	//strings 11-18 - obstacle item filenames 1-8
	for(i=0;i<max_sky_items;i++)
		{
			pan_setstring(sky_skypanel, 0,	55,340+i*15,	panel_font,			str_create( sky_filenames[i] ) );
		}
	
	//buttons 1-8 (checkbox)
	for(i=0;i<max_sky_items;i++)
		{
			pan_setbutton(sky_skypanel, 0,	2, 	15,340+i*15,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Sky_ToggleItem			,NULL,NULL); 
		}
	// button states
	for(i=0;i<max_sky_items;i++)
		{
			button_state( sky_skypanel , 1+i , sky_switch_type[i] ); 		// 1 ON, 0 OFF
		}	
	//buttns 9-10 - model set
	pan_setdigits(sky_skypanel, 0,	40,310,	"Image File Set: %0.f",		panel_font,	1, sky_set);	
	pan_setbutton(sky_skypanel, 0,	1, 	20,310,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_DecSet, 		NULL, NULL); 
	pan_setbutton(sky_skypanel, 0,	1, 	140,310,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_IncSet, 		NULL, NULL); 
				
	//button 11 - okay button - to close this panel
	pan_setbutton(sky_skypanel, 0,	1, 	50,480,		s_ok_on_bmp,s_ok_off_bmp,s_ok_over_bmp,s_ok_over_bmp,							Sky_SetSkyClose			,NULL,NULL); 	
		
	//----------------------------
	// Entity specific settings
	
	// model
	if (callbutton == (var)1)
		{
			pan_setstring(sky_skypanel, 0,	15,60,	panel_font,			str_create("SKY MODEL") );
			
			// var pointers for sliders
			sky_skyz			= &skyModel.z;
			sky_skyalpha	= &skyModel.alpha;
			sky_skylayer	= 0;
			
			//button 12 - load or set - to really set selected sky
			pan_setbutton(sky_skypanel, 0,	1, 	150,480,		s_load_on_bmp,s_load_off_bmp,s_load_over_bmp,s_load_over_bmp,							Sky_ChangeModel	,NULL,NULL); 	
			
			// layer
			pan_setdigits(sky_skypanel, 0,	15,100,	"Layer: %0.f",		panel_font,	1, sky_skylayer );	
			
			//slider z 
			pan_setdigits(sky_skypanel, 0,	15,120,	"Z: %0.f",	panel_font,															1,				*sky_skyz);
			pan_setslider(sky_skypanel, 0,	112,120+2,	horiz_slider_bmp,slider_knob_bmp,		(TerrHmp_GetGroundMinX()/2),(TerrHmp_GetGroundMaxX()/2),		*sky_skyz);			
			
			//slider alpha
			pan_setdigits(sky_skypanel, 0,	15,140,	"Alpha: %0.f",	panel_font,						1,			*sky_skyalpha);
			pan_setslider(sky_skypanel, 0,	112,140+2,	horiz_slider_bmp,slider_knob_bmp,		0,100,		*sky_skyalpha);	
			
			// button 13,14	15,16
			pan_setbutton(sky_skypanel, 0,	1, 	100,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_ModelButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_ModelButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_ModelButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_ModelButton, 		NULL, NULL); 
	
		}
	// cube
	else if (callbutton == (var)3)
		{
			pan_setstring(sky_skypanel, 0,	15,60,	panel_font,			str_create("SKY CUBE") );
			
			// var pointers for sliders			
			sky_skyz			= &skyCube.z;
			sky_skyalpha	= &skyCube.alpha;
			sky_skylayer	= 0;
			
			//button 12 - load or set - to really set selected sky
			pan_setbutton(sky_skypanel, 0,	1, 	150,480,		s_load_on_bmp,s_load_off_bmp,s_load_over_bmp,s_load_over_bmp,							Sky_ChangeCube	,NULL,NULL); 	
			
			// layer
			pan_setdigits(sky_skypanel, 0,	15,100,	"Layer: %0.f",		panel_font,	1, sky_skylayer );	
			
			//slider z 			
			pan_setdigits(sky_skypanel, 0,	15,120,	"Z: %0.f",	panel_font,								1,		*sky_skyz);
			pan_setslider(sky_skypanel, 0,	112,120+2,	horiz_slider_bmp,slider_knob_bmp,		-128,128,	*sky_skyz);			
			
			//slider alpha
			pan_setdigits(sky_skypanel, 0,	15,140,	"Alpha: %0.f",	panel_font,						1,			*sky_skyalpha);
			pan_setslider(sky_skypanel, 0,	112,140+2,	horiz_slider_bmp,slider_knob_bmp,		0,100,		*sky_skyalpha);	
			
			// button 13,14	15,16
			pan_setbutton(sky_skypanel, 0,	1, 	100,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CubeButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CubeButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CubeButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CubeButton, 		NULL, NULL); 
	
		}
	// cylinder
	else if (callbutton == (var)5)
		{
			pan_setstring(sky_skypanel, 0,	15,60,	panel_font,			str_create("SKY CYLINDER") );
			
			// var pointers for sliders
			sky_skyscale_x	= &skyCylinder.scale_x;
			sky_skypan		= &skyCylinder.pan;
			sky_skytilt		= &skyCylinder.tilt;
			sky_skyalpha	= &skyCylinder.alpha;
			sky_skylayer	= 1;
			
			//button 12 - load or set - to really set selected sky
			pan_setbutton(sky_skypanel, 0,	1, 	150,480,		s_load_on_bmp,s_load_off_bmp,s_load_over_bmp,s_load_over_bmp,							Sky_ChangeCylinder	,NULL,NULL); 	
			
			// layer
			pan_setdigits(sky_skypanel, 0,	15,100,	"Layer: %0.f",		panel_font,	1, sky_skylayer );
			
			//slider scale_x
			pan_setdigits(sky_skypanel, 0,	15,120,	"Scale: %0.2f",	panel_font,						1,		*sky_skyscale_x);
			pan_setslider(sky_skypanel, 0,	112,120+2,	horiz_slider_bmp,slider_knob_bmp,		0.1,2,		*sky_skyscale_x);	
						
			//slider pan 
			pan_setdigits(sky_skypanel, 0,	15,140,	"Pan: %0.f",	panel_font,						1,			*sky_skypan);
			pan_setslider(sky_skypanel, 0,	112,140+2,	horiz_slider_bmp,slider_knob_bmp,		-180,180,	*sky_skypan);
			
			//slider tilt			
			pan_setdigits(sky_skypanel, 0,	15,160,	"Tilt: %0.f",	panel_font,						1,			*sky_skytilt);
			pan_setslider(sky_skypanel, 0,	112,160+2,	horiz_slider_bmp,slider_knob_bmp,		-90,90,		*sky_skytilt);
			
			//slider alpha
			pan_setdigits(sky_skypanel, 0,	15,180,	"Alpha: %0.f",	panel_font,						1,			*sky_skyalpha);
			pan_setslider(sky_skypanel, 0,	112,180+2,	horiz_slider_bmp,slider_knob_bmp,		0,100,		*sky_skyalpha);
			
			// button 13,14	15,16		17,18		19,20
			pan_setbutton(sky_skypanel, 0,	1, 	100,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CylinderButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CylinderButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CylinderButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CylinderButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CylinderButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CylinderButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,180,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CylinderButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,180,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CylinderButton, 		NULL, NULL); 
			
		}
	// dome-1
	else if (callbutton == (var)7)
		{
			pan_setstring(sky_skypanel, 0,	15,60,	panel_font,			str_create("SKY DOME-1") );
			
			// var pointers for sliders
			sky_skyscale_x	= &skyClouds1.scale_x;
			sky_skyu			= &skyClouds1.u;
			sky_skyv			= &skyClouds1.v;
			sky_skyalpha	= &skyClouds1.alpha;
			sky_skylayer	= 2;
			
			//button 12 - load or set - to really set selected sky
			pan_setbutton(sky_skypanel, 0,	1, 	150,480,		s_load_on_bmp,s_load_off_bmp,s_load_over_bmp,s_load_over_bmp,							Sky_ChangeDome1	,NULL,NULL); 	
			
			// layer
			pan_setdigits(sky_skypanel, 0,	15,100,	"Layer: %0.f",		panel_font,	1, sky_skylayer );
			
			//slider scale_x
			pan_setdigits(sky_skypanel, 0,	15,120,	"Scale: %0.2f",	panel_font,						1,		*sky_skyscale_x);
			pan_setslider(sky_skypanel, 0,	112,120+2,	horiz_slider_bmp,slider_knob_bmp,		0.1,10,		*sky_skyscale_x);	
			
			//slider u 
			pan_setdigits(sky_skypanel, 0,	15,140,	"U: %0.1f",	panel_font,							1,			*sky_skyu);
			pan_setslider(sky_skypanel, 0,	112,140+2,	horiz_slider_bmp,slider_knob_bmp,		0,10,			*sky_skyu);
			
			//slider v			
			pan_setdigits(sky_skypanel, 0,	15,160,	"V: %0.1f",	panel_font,							1,			*sky_skyv);
			pan_setslider(sky_skypanel, 0,	112,160+2,	horiz_slider_bmp,slider_knob_bmp,		0,10,			*sky_skyv);
			
			//slider alpha
			pan_setdigits(sky_skypanel, 0,	15,180,	"Alpha: %0.f",	panel_font,						1,			*sky_skyalpha);
			pan_setslider(sky_skypanel, 0,	112,180+2,	horiz_slider_bmp,slider_knob_bmp,		0,100,		*sky_skyalpha);
			
			// button 13,14	15,16		17,18		19,20
			pan_setbutton(sky_skypanel, 0,	1, 	100,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,180,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,180,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CloudsButton, 		NULL, NULL); 
			
		}
	// dome-2
	else if (callbutton == (var)9)
		{
			pan_setstring(sky_skypanel, 0,	15,60,	panel_font,			str_create("SKY DOME-2") );
			
			// var pointers for sliders
			sky_skyscale_x	= &skyClouds2.scale_x;
			sky_skyu			= &skyClouds2.u;
			sky_skyv			= &skyClouds2.v;
			sky_skyalpha	= &skyClouds2.alpha;
			sky_skylayer	= 3;
			
			//button 12 - load or set - to really set selected sky
			pan_setbutton(sky_skypanel, 0,	1, 	150,480,		s_load_on_bmp,s_load_off_bmp,s_load_over_bmp,s_load_over_bmp,							Sky_ChangeDome2	,NULL,NULL); 	
			
			// layer
			pan_setdigits(sky_skypanel, 0,	15,100,	"Layer: %0.f",		panel_font,	1, sky_skylayer );
			
			//slider scale_x
			pan_setdigits(sky_skypanel, 0,	15,120,	"Scale: %0.2f",	panel_font,						1,		*sky_skyscale_x);
			pan_setslider(sky_skypanel, 0,	112,120+2,	horiz_slider_bmp,slider_knob_bmp,		0.1,10,		*sky_skyscale_x);	
			
			//slider u 
			pan_setdigits(sky_skypanel, 0,	15,140,	"U: %0.1f",	panel_font,							1,			*sky_skyu);
			pan_setslider(sky_skypanel, 0,	112,140+2,	horiz_slider_bmp,slider_knob_bmp,		0,10,			*sky_skyu);
			
			//slider v			
			pan_setdigits(sky_skypanel, 0,	15,160,	"V: %0.1f",	panel_font,							1,			*sky_skyv);
			pan_setslider(sky_skypanel, 0,	112,160+2,	horiz_slider_bmp,slider_knob_bmp,		0,10,			*sky_skyv);
			
			//slider alpha
			pan_setdigits(sky_skypanel, 0,	15,180,	"Alpha: %0.f",	panel_font,						1,			*sky_skyalpha);
			pan_setslider(sky_skypanel, 0,	112,180+2,	horiz_slider_bmp,slider_knob_bmp,		0,100,		*sky_skyalpha);			
			
			// button 13,14	15,16		17,18		19,20
			pan_setbutton(sky_skypanel, 0,	1, 	100,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	100,180,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Sky_SetSky_CloudsButton, 		NULL, NULL); 
			pan_setbutton(sky_skypanel, 0,	1, 	220,180,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Sky_SetSky_CloudsButton, 		NULL, NULL); 
					
		}	
	//----------------------------
	ptr_remove(temp_string);
}


void		Sky_SetSkyClose()
{	
	wait(3);
	
	if (sky_skypanel)	
		{
			ptr_remove(sky_skypanel);
			sky_skypanel = NULL;
		}
	else
		{
			return;
		}
	
	//----------------------------------------------
	
	set(sky_lowpanel, SHOW);
	set(sky_colpan1, SHOW);
	set(sky_colpan2, SHOW);
	set(sky_colpan3, SHOW);
	set(sky_colpan4, SHOW);
	set(sky_colpan5, SHOW);
	set(sky_colpan6, SHOW);
	
	//----------------------------------------------
	int i;
	
	if (skies_txt)
		{
			for (i = 0; i < skies_txt->strings; i++) 
				if ((skies_txt->pstring)[i]) 
					ptr_remove((skies_txt->pstring)[i]);
			ptr_remove(skies_txt);
			skies_txt = NULL;
		}
	
	if (sky_filenames)
		{
			for(i=0;i<max_sky_items;i++)
				{
					ptr_remove(sky_filenames[i]);
				}
			sys_free(sky_filenames);
			sky_filenames = NULL;
			
//			ptr_remove(sky_filenames);		// causes error
		}
}


void		Sky_SetSky_ModelButton(button_number)
{
	// sky z
	if (button_number==(var)13)
		{
			*sky_skyz -= 10;
			*sky_skyz = maxv( (TerrHmp_GetGroundMaxX()/2) , *sky_skyz );
		}
	else if (button_number==(var)14)
		{
			*sky_skyz += 10;
			*sky_skyz = minv( (TerrHmp_GetGroundMaxX()/2) , *sky_skyz );
		}
	// sky alpha
	else if (button_number==(var)15)
		{
			*sky_skyalpha = maxv( 0 , --*sky_skyalpha );
		}
	else if (button_number==(var)16)
		{
			*sky_skyalpha = minv( 100 , ++*sky_skyalpha );
		}
}


void		Sky_SetSky_CubeButton(button_number)
{
	// sky z
	if (button_number==(var)13)
		{
			*sky_skyz = maxv( -128 , --*sky_skyz );
		}
	else if (button_number==(var)14)
		{
			*sky_skyz = minv( 128 , ++*sky_skyz );
		}
	// sky alpha
	else if (button_number==(var)15)
		{
			*sky_skyalpha = maxv( 0 , --*sky_skyalpha );
		}
	else if (button_number==(var)16)
		{
			*sky_skyalpha = minv( 100 , ++*sky_skyalpha );
		}
}


void		Sky_SetSky_CylinderButton(button_number)
{
	// sky scale
	if (button_number==(var)13)
		{
			*sky_skyscale_x -= 0.01;
			*sky_skyscale_x = maxv( 0.1 , *sky_skyscale_x );
		}
	else if (button_number==(var)14)
		{
			*sky_skyscale_x += 0.01;
			*sky_skyscale_x = minv( 2 , *sky_skyscale_x );
		}
	// sky pan
	else if (button_number==(var)15)
		{
			*sky_skypan = maxv( -180 , --*sky_skypan );
		}
	else if (button_number==(var)16)
		{
			*sky_skypan = minv( 180 , ++*sky_skypan );
		}
	// sky tilt
	else if (button_number==(var)17)
		{
			*sky_skytilt = maxv( -90 , --*sky_skytilt );
		}
	else if (button_number==(var)18)
		{
			*sky_skytilt = minv( 90 , ++*sky_skytilt );
		}
	// sky alpha
	else if (button_number==(var)19)
		{
			*sky_skyalpha = maxv( 0 , --*sky_skyalpha );
		}
	else if (button_number==(var)20)
		{
			*sky_skyalpha = minv( 100 , ++*sky_skyalpha );
		}
}


void		Sky_SetSky_CloudsButton(button_number)
{
	// sky scale
	if (button_number==(var)13)
		{
			*sky_skyscale_x -= 0.01;
			*sky_skyscale_x = maxv( 0.1 , *sky_skyscale_x );
		}
	else if (button_number==(var)14)
		{
			*sky_skyscale_x += 0.01;
			*sky_skyscale_x = minv( 10 , *sky_skyscale_x );
		}
	// sky u
	else if (button_number==(var)15)
		{
			*sky_skyu -= 0.1;
			*sky_skyu = maxv( 0 , *sky_skyu );
		}
	else if (button_number==(var)16)
		{
			*sky_skyu += 0.1;
			*sky_skyu = minv( 10 , *sky_skyu );
		}
	// sky v
	else if (button_number==(var)17)
		{
			*sky_skyv -= 0.1;
			*sky_skyv = maxv( 0 , *sky_skyv );
		}
	else if (button_number==(var)18)
		{
			*sky_skyv += 0.1;
			*sky_skyv = minv( 10 , *sky_skyv );
		}
	// sky alpha
	else if (button_number==(var)19)
		{
			*sky_skyalpha = maxv( 0 , --*sky_skyalpha );
		}
	else if (button_number==(var)20)
		{
			*sky_skyalpha = minv( 100 , ++*sky_skyalpha );
		}	
}


////////////////////////////////////////


void		Sky_ToggleItem(button_number)
{
	int i;
	for(i=0;i<max_sky_items;i++)
		{
			sky_switch_type[i] = 0;							// 0..7
			button_state( sky_skypanel , i+1 , 0 );			// 1..8
		}
	sky_switch_type[button_number-1] = 1;
	button_state( sky_skypanel , button_number , 1 ); 
	sky_selected = button_number-1;
}


void		Sky_IncSet()
{ 
	// jump to next existing set
	sky_set++;
	if (sky_set>max_sky_sets)
		{
			sky_set = max_sky_sets;
			return;
		}
	Sky_UpdateSetNames();
}


void		Sky_DecSet()
{ 
	// jump to next existing set
	sky_set--;
	if (sky_set<1)
		{
			sky_set = 1;
			return;
		}
	Sky_UpdateSetNames();
}


void		Sky_UpdateSetNames()
{
	int i;
	// copy the proper 8 filenames to string array
	int j = sky_set - 1;
	int k = j * max_sky_items;
	for(i=0;i<max_sky_items;i++)
		{
			str_cpy( sky_filenames[i] , "" );										// (or obstacle_foldername) subfolder
			str_cat( sky_filenames[i] , (skies_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi( sky_filenames[i] , "" ) )								// (or obstacle_foldername) if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( sky_filenames[i] , "-" );
				}
		}
	// refresh 8 filename strings to display in menu
	for(i=0;i<max_sky_items;i++)
		{
			pan_setstring(sky_skypanel, 11+i,	55,340+i*15,	panel_font,			str_create( sky_filenames[i] ) );
		}	
}


/////////////////////////////////


void		Sky_ChangeModel()
{	
	if ( !str_cmpi(sky_filenames[sky_selected] , "-" ) )
		{		
			STRING* temp_str = str_create("");			
			str_cpy( temp_str , sky_foldername );
			str_cat( temp_str , sky_filenames[sky_selected] );	
			
//			ent_morphskin( skyModel , temp_str );						// not okay for view entities !
			
			BMAP* temp_bmp = bmap_create( temp_str );
//			wait(1);
			
			str_cpy( skyModelSkin , sky_filenames[sky_selected] );		// store without path for saving / reloading
//			str_cpy( skyModelSkin , temp_str );									// store with path for saving / reloading
			
			// remove skin by getskin and set a new by setskin - to free mamory - but works without it
//			ent_clone( skyModel );	
//			wait(1);		
			ptr_remove( ent_getskin( skyModel , 1 ) );
//			wait(1);
			
			ent_setskin( skyModel , temp_bmp , 1);			
//			wait(1);
			
			ptr_remove(temp_str);
//			ptr_remove(temp_bmp);  // bmp should not be removed - in that case skin is removed too and becomes black !!!
		}	
}


void		Sky_ChangeCube()
{
	if ( !str_cmpi(sky_filenames[sky_selected] , "-" ) )
		{		
			STRING* temp_str = str_create("");			
			str_cpy( temp_str , sky_foldername );
			str_cat( temp_str , sky_filenames[sky_selected] );
			ent_morph( skyCube , temp_str );	
			wait(1);
			ptr_remove(temp_str);
		}
}


void		Sky_ChangeCylinder()
{
	if ( !str_cmpi(sky_filenames[sky_selected] , "-" ) )
		{		
			STRING* temp_str = str_create("");			
			str_cpy( temp_str , sky_foldername );
			str_cat( temp_str , sky_filenames[sky_selected] );
			ent_morph( skyCylinder , temp_str );	
			wait(1);
			ptr_remove(temp_str);
		}
}


void		Sky_ChangeDome1()
{
	if ( !str_cmpi(sky_filenames[sky_selected] , "-" ) )
		{		
			STRING* temp_str = str_create("");			
			str_cpy( temp_str , sky_foldername );
			str_cat( temp_str , sky_filenames[sky_selected] );
			ent_morph( skyClouds1 , temp_str );	
			wait(1);
			ptr_remove(temp_str);
		}
}


void		Sky_ChangeDome2()
{
	if ( !str_cmpi(sky_filenames[sky_selected] , "-" ) )
		{		
			STRING* temp_str = str_create("");			
			str_cpy( temp_str , sky_foldername );
			str_cat( temp_str , sky_filenames[sky_selected] );
			ent_morph( skyClouds2 , temp_str );	
			wait(1);
			ptr_remove(temp_str);
		}
}


///////////////////////////////////////////////////////////////////////////////////


#endif