
#ifndef MBseedpanel_c
#define MBseedpanel_c


/////////////////////////////////////////////////////////////
// code

void		Seed_Init()
{
	if (seed_panel) return;
	
	wait(3);
	
	////////////////
	// init panel
	seed_panel = pan_create( panel_image2 , 12 );
	set( seed_panel , SHOW | SHADOW | OVERLAY );					
	seed_panel.pos_x 	= ui_submenupanel3_x;						// screen_size.x-256;
	seed_panel.pos_y 	= ui_submenupanel3_y;						// 256;		
	seed_panel.red		= panel_font_r;
	seed_panel.green	= panel_font_g;
	seed_panel.blue	= panel_font_b;
	seed_panel.event	= ui_various_panelmove;
	
	////////////////
	// set max and step quantities
	if (UIMode==(var)UIMODE_DETAIL)
		{
			seed_max = 10000;
			seed_quantity = 10000;
			seed_step = 100;
		}
	else if (UIMode==(var)UIMODE_SHRUB)
		{
			seed_max = 1000;						
			seed_quantity = 1000;
			seed_step = 10;
		}
	else if (UIMode==(var)UIMODE_GRASS)
		{
			seed_max = 5000;
			seed_quantity = 5000;
			seed_step = 100;
		}
	else if (UIMode==(var)UIMODE_OBSTACLE)
		{
			seed_max = 500;					
			seed_quantity = 500;
			seed_step = 1;
		}
	
	////////////////
	//string 1 - title
	pan_setstring(seed_panel, 0,	15,10,				panel_font2,									str_create("MASS PLACE ENTITIES") );	
	
	////////////////////////////
	// seed_area--------------------------------------------------
	
	// string 2 3 4 5
	pan_setstring(seed_panel, 0,	15,350,				panel_font2,									str_create("Area to cover") );
	
	// button 1 2 3
	pan_setstring(seed_panel, 0,	132,350,	panel_font,			str_create("full") );
	pan_setbutton(seed_panel, 0,	4, 	112,350,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		Seed_ToggleArea,	NULL,NULL); 
	if (seed_area==(var)0) 	button_state(seed_panel, 1, ON ); 		// 1 ON, 0 OFF	
	
	pan_setstring(seed_panel, 0,	132,370,	panel_font,			str_create("game") );
	pan_setbutton(seed_panel, 0,	4, 	112,370,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		Seed_ToggleArea,	NULL,NULL); 
	if (seed_area==(var)1) 	button_state(seed_panel, 2, ON ); 		// 1 ON, 0 OFF	
	
	pan_setstring(seed_panel, 0,	132,390,	panel_font,			str_create("border") );
	pan_setbutton(seed_panel, 0,	4, 	112,390,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		Seed_ToggleArea,	NULL,NULL); 
	if (seed_area==(var)2) 	button_state(seed_panel, 3, ON ); 		// 1 ON, 0 OFF	
	
	////////////////
	// seed_random--------------------------------------------------
	
	// string 
	pan_setstring(seed_panel, 0,	45,60,				panel_font2,									str_create("Random placement") );
	// button  4
	pan_setbutton(seed_panel, 0,	2, 	15,60,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Seed_ToggleRandom,	NULL,NULL);
	button_state( seed_panel, 4, seed_random); 	
	
	// seed_quantity--------------------------------------------------
	
	//slider
	pan_setdigits(seed_panel, 0,	15,80,				"Quantity: %0.f",	panel_font,				1,					seed_quantity);
	pan_setslider(seed_panel, 0,	112,80+2,			horiz_slider_bmp,slider_knob_bmp,		0,seed_max,		seed_quantity); 				
	//button	5 6
	pan_setbutton(seed_panel, 0,	1, 	100,80,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Seed_DecQuantity, 		NULL, NULL); 
	pan_setbutton(seed_panel, 0,	1, 	220,80,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Seed_IncQuantity, 		NULL, NULL); 
	
	/////////////////
	// seed_coverage--------------------------------------------------
	
	// string 
	pan_setstring(seed_panel, 0,	45,120,				panel_font2,									str_create("Full coverage") );
	// button 7
	pan_setbutton(seed_panel, 0,	2, 	15,120,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		Seed_ToggleCoverage,		NULL,NULL);
	button_state( seed_panel, 7, seed_coverage); 	
	
	// seed_distance--------------------------------------------------
	
	//slider
	pan_setdigits(seed_panel, 0,	15,140,				"Distance: %0.f",	panel_font,				1,					seed_distance);
	pan_setslider(seed_panel, 0,	112,140+2,			horiz_slider_bmp,slider_knob_bmp,		16,640,			seed_distance); 				
	//button 8 9
	pan_setbutton(seed_panel, 0,	1, 	100,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Seed_DecDistance, 		NULL, NULL); 
	pan_setbutton(seed_panel, 0,	1, 	220,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Seed_IncDistance, 		NULL, NULL); 
	
	// seed_rnddist--------------------------------------------------
	
	//slider
	pan_setdigits(seed_panel, 0,	15,160,				"Randomness: %0.f",	panel_font,				1,				seed_rnddist);
	pan_setslider(seed_panel, 0,	112,160+2,			horiz_slider_bmp,slider_knob_bmp,		0,512,			seed_rnddist); 				
	//button 10 11
	pan_setbutton(seed_panel, 0,	1, 	100,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Seed_DecRndDist, 		NULL, NULL); 
	pan_setbutton(seed_panel, 0,	1, 	220,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Seed_IncRndDist, 		NULL, NULL); 
	
	// seed_limit----------------------------------------------------
	
	//slider
	pan_setdigits(seed_panel, 0,	15,180,				"Entity limit: %0.f",	panel_font,			1,				seed_limit);
	pan_setslider(seed_panel, 0,	112,180+2,			horiz_slider_bmp,slider_knob_bmp,		0,50000,			seed_limit); 				
	//button 12 13
	pan_setbutton(seed_panel, 0,	1, 	100,180,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Seed_DecLimit, 		NULL, NULL); 
	pan_setbutton(seed_panel, 0,	1, 	220,180,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Seed_IncLimit, 		NULL, NULL); 
		
	/////////////////
	// common settings--------------------------------------------
	// string 
	pan_setstring(seed_panel, 0,	15,220,				panel_font2,									str_create("Common settings") );
	
	// seed_minh--------------------------------------------------
	
	//slider
	pan_setdigits(seed_panel, 0,	15,250,				"Min height: %0.f",	panel_font,				1,				seed_minh);
	pan_setslider(seed_panel, 0,	112,250+2,			horiz_slider_bmp,slider_knob_bmp,		-256,2048,		seed_minh); 				
	//button
	pan_setbutton(seed_panel, 0,	1, 	100,250,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Seed_DecMinH, 		NULL, NULL); 
	pan_setbutton(seed_panel, 0,	1, 	220,250,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Seed_IncMinH, 		NULL, NULL); 
	
	// seed_maxh--------------------------------------------------
	
	//slider
	pan_setdigits(seed_panel, 0,	15,270,				"Max height: %0.f",	panel_font,				1,				seed_maxh);
	pan_setslider(seed_panel, 0,	112,270+2,			horiz_slider_bmp,slider_knob_bmp,		0,3072,			seed_maxh); 				
	//button
	pan_setbutton(seed_panel, 0,	1, 	100,270,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Seed_DecMaxH, 		NULL, NULL); 
	pan_setbutton(seed_panel, 0,	1, 	220,270,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Seed_IncMaxH, 		NULL, NULL); 
	
	// seed_minslope----------------------------------------------
	
	//slider
	pan_setdigits(seed_panel, 0,	15,300,				"Min slope: %0.1f",	panel_font,				1,				seed_minslope);
	pan_setslider(seed_panel, 0,	112,300+2,			horiz_slider_bmp,slider_knob_bmp,		0,90,				seed_minslope); 				
	//button
	pan_setbutton(seed_panel, 0,	1, 	100,300,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Seed_DecMinSlope, 		NULL, NULL); 
	pan_setbutton(seed_panel, 0,	1, 	220,300,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Seed_IncMinSlope, 		NULL, NULL); 
	
	// seed_maxslope----------------------------------------------
	
	//slider
	pan_setdigits(seed_panel, 0,	15,320,				"Max slope: %0.1f",	panel_font,				1,				seed_maxslope);
	pan_setslider(seed_panel, 0,	112,320+2,			horiz_slider_bmp,slider_knob_bmp,		0,90,				seed_maxslope); 				
	//button
	pan_setbutton(seed_panel, 0,	1, 	100,320,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Seed_DecMaxSlope, 		NULL, NULL); 
	pan_setbutton(seed_panel, 0,	1, 	220,320,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Seed_IncMaxSlope, 		NULL, NULL); 
	
	////////////////////////////
	// ok/cancel
	
	//button -  ok to place
	pan_setbutton(seed_panel, 0,	1, 	75,480,		s_ok_on_bmp,s_ok_off_bmp,s_ok_over_bmp,s_ok_over_bmp,							Seed_Place		,NULL,NULL); 	
	
	//button -  cancel to close
	pan_setbutton(seed_panel, 0,	1, 	150,480,		s_cancel_on_bmp,s_cancel_off_bmp,s_cancel_over_bmp,s_cancel_over_bmp,	Seed_Close		,NULL,NULL); 	
	
	///////////////////////////
	
//	while(seed_panel)
//		{
//			DEBUG_VAR(seed_random, 100);
//			DEBUG_VAR(seed_coverage, 125);
//			DEBUG_VAR(seed_area, 150);
//			
//			wait(1);
//		}
}

///////////////////////////////////////////////////////


void		Seed_ToggleRandom()
{
	seed_random   = 1;
	button_state( seed_panel, 4, seed_random); 
	seed_coverage = 0;		
	button_state( seed_panel, 7, seed_coverage);
}


void		Seed_ToggleCoverage()
{
	seed_coverage = 1;	
	button_state( seed_panel, 7, seed_coverage);
	seed_random   = 0;
	button_state( seed_panel, 4, seed_random); 
}


void		Seed_ToggleArea(var button_number)
{	
	if (button_number==(var)1)
		{
			seed_area = 0;
		}
	else if (button_number==(var)2)
		{
			seed_area = 1;
		}
	else if (button_number==(var)3)
		{
			seed_area = 2;
		}	
}

///////////////////////////////////////////////////////

void		Seed_DecQuantity()
{
	seed_quantity -= seed_step;
	seed_quantity = maxv(0,seed_quantity);
}

void		Seed_IncQuantity()
{
	seed_quantity += seed_step;
	seed_quantity = minv(seed_max,seed_quantity);
}

//----------------------------
void		Seed_DecDistance()
{
	seed_distance -= 1;
	seed_distance = maxv(16,seed_distance);
}

void		Seed_IncDistance()
{
	seed_distance += 1;
	seed_distance = minv(640,seed_distance);
}

//----------------------------
void		Seed_DecRndDist()		
{
	seed_rnddist -= 1;
	seed_rnddist = maxv(0,seed_rnddist);
}

void		Seed_IncRndDist()
{
	seed_rnddist += 1;
	seed_rnddist =minv(512,seed_rnddist);
}

//----------------------------
void		Seed_DecLimit()		
{
	seed_limit -= 100;
	seed_limit = maxv(100,seed_limit);
}

void		Seed_IncLimit()
{
	seed_limit += 100;
	seed_limit =minv(50000,seed_limit);
}

//----------------------------
void		Seed_DecMinH()
{
	seed_minh -= 10;
	seed_minh = maxv(-256,seed_minh);
}

void		Seed_IncMinH()
{
	seed_minh += 10;
	seed_minh = minv(1024,seed_minh);
}

//----------------------------
void		Seed_DecMaxH()
{
	seed_maxh -= 10;
	seed_maxh = maxv(0,seed_maxh);
}

void		Seed_IncMaxH()
{
	seed_maxh += 10;
	seed_maxh = minv(3072,seed_maxh);
}

//----------------------------
void		Seed_DecMinSlope()
{
	seed_minslope -= 0.5;
	seed_minslope = maxv(0,seed_minslope);
}

void		Seed_IncMinSlope()
{
	seed_minslope += 0.5;
	seed_minslope = minv(90,seed_minslope);
}

//----------------------------
void		Seed_DecMaxSlope()
{
	seed_maxslope -= 0.5;
	seed_maxslope = maxv(0,seed_maxslope);
}

void		Seed_IncMaxSlope()
{
	seed_maxslope += 0.5;
	seed_maxslope = minv(90,seed_maxslope);
}

///////////////////////////////////////////////////////

void		Seed_Close()
{		
	wait(3);
	while(mouse_left)	{wait(1);}
	
	ptr_remove(seed_panel);	
	seed_panel = NULL;
}

void		Seed_Place()
{		
	wait(3);
		
	// mass place models - modify: 0 full, 1 game, 2 border area (it was: 0 game, 1 full) !!!!!!!
	if (UIMode==(var)UIMODE_DETAIL)
		{
			if (seed_random)
				{
					Detail_SeedQuantity();
				}
			else
				{
					Detail_SeedArea();
				}			
		}
	else if (UIMode==(var)UIMODE_SHRUB)
		{
			if (seed_random)
				{
					Shrub_SeedQuantity();
				}
			else
				{
					Shrub_SeedArea();
				}
		}
	else if (UIMode==(var)UIMODE_GRASS)
		{
			if (seed_random)
				{
					Grass_SeedQuantity();
				}
			else
				{
					Grass_SeedArea();
				}	
		}
	else if (UIMode==(var)UIMODE_OBSTACLE)
		{
			if (seed_random)
				{
					Obstacle_SeedQuantity();
				}
			else
				{
					Obstacle_SeedArea();
				}			
		}	
	
	ptr_remove(seed_panel);	
	seed_panel = NULL;
}


#endif