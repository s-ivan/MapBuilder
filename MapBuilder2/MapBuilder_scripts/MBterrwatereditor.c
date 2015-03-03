
#ifndef MBterrwatereditor_c
#define MBterrwatereditor_c 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor initialization


void 		TerrWater_Init()
{
//	terrwater_ready = 0;
	
	//----------------------------------------------------------------
	// terrground filename initialization 

	int arraysize = (int)TERRWATER_MAXPERSET;
	terrwater_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );
		
	// init filename string array elements
	int i;
	for(i=0;i<max_terrwater_items;i++)
		{
			terrwater_filenames[i] = str_create("");
		}	
	
	//-----------------------------------------------------------------
	// determine how many items are in the folder, 
	// divide it by 8, and the result is the number of sets
	
	max_terrwater_sets = TERRWATER_MAXSET;
	terrwaters_txt = txt_create( (max_terrwater_sets*max_terrwater_items) , 0 );		// 10*8=80
	
	for(i=0;i<max_terrwater_sets*max_terrwater_items;i++)
		{
			str_cpy( (terrwaters_txt->pstring)[i] , "" );
		}
	
	//-------------------------------------------------------------------
	// read in folder content
	
	STRING* temp_str = str_create("");
	str_cpy( temp_str , filemenu_hmpw_filter );

	txt_for_dir( terrwaters_txt, temp_str );		
	
	i = 0;						
	while (str_len((terrwaters_txt->pstring)[i]) > 0)
		{
			i++;
		}
		
	max_terrwater_sets = integer(i/max_terrwater_items);
	if ( fraction(i/max_terrwater_items) > 0)
		{
			max_terrwater_sets++;																			// allow to display not complete sets too
		}
	terrwater_set = clamp(terrwater_set, 1, max_terrwater_sets);
	
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	
	for(i=0;i<max_terrwater_items;i++)
		{		
			str_cpy( terrwater_filenames[i] , "" );													
			int j = (terrwater_set - 1) * max_terrwater_items;
			str_cat( terrwater_filenames[i] , (terrwaters_txt->pstring)[j+i] );			// filename with extension
			if ( str_cmpi( terrwater_filenames[i] , "" ) )											// if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( terrwater_filenames[i] , "-" );
				}
		}
	
	//------------------------------------------------------------------
	// initial value settings
	
	for(i=0;i<max_terrwater_items;i++)
		{
			terrwater_switch_type[i] = 0;
		}
	terrwater_switch_type[terrwater_selected] = 1;	

	//-------------------------------------------------------------------
	// start panel
	
	TerrWater_Panel_Init();
	wait_for(TerrWater_Panel_Init);
	
	//-------------------------------------------------------------------
	
	while(UIMode==(var)UIMODE_WATERTERR)
		{
			wait(1);
		}
	
	TerrWater_Close();
}


void		TerrWater_Close()
{
	//--------------------------------------------------------
	
//	terrwater_ready = 0;
	
	ptr_remove(terrwater_panel);
	terrwater_panel = NULL;
		
	//--------------------------------------------------------
	
	int i;
	
	// remove temp text
	for (i = 0; i < terrwaters_txt->strings; i++) 
		if ((terrwaters_txt->pstring)[i]) 
			ptr_remove((terrwaters_txt->pstring)[i]);
	ptr_remove(terrwaters_txt);
	terrwaters_txt = NULL;
	
	// remove temp string array
	for(i=0;i<max_terrwater_items;i++)
		{
			ptr_remove(terrwater_filenames[i]);
		}
	sys_free(terrwater_filenames);
	terrwater_filenames = NULL;
//	ptr_remove(terrwater_filenames);									// no!
	
	//--------------------------------------------------------
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor menu


void		TerrWater_Panel_Init()
{
	//--------------------------------------------------------------
	
	int i;
	STRING* temp_string = str_create(""); 
	
	//--------------------------------------------------------------
	
	terrwater_panel = pan_create( panel_image2 , 11 );
	set( terrwater_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	terrwater_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	terrwater_panel.pos_y 	= ui_submenupanel_y;								// 256;
	terrwater_panel.red		= panel_font_r;
	terrwater_panel.green	= panel_font_g;
	terrwater_panel.blue	= panel_font_b;
	terrwater_panel.event	= ui_submenu_panelmove;
	
	//--------------------------------------------------------------
	
	// string 1
	pan_setstring(terrwater_panel, 0,	15,10,	panel_font2,			str_create("WATER TERRAIN PLACEMENT") );
	
	//--------------------------------------------------------------
	
	// string 2
	pan_setstring(terrwater_panel, 0,	15,330,	panel_font,			str_create("Select item") );
	
	// strings 3-10 - terrwater item numbers 1-8
	for(i=0;i<max_terrwater_items;i++)
		{
			str_for_int( temp_string , (i+1) );
			pan_setstring( terrwater_panel, 0,	35,350+i*15,	panel_font,			str_create( temp_string ) );
		}
	
	// strings 11-18 - terrwater item filenames 1-8
	for(i=0;i<max_terrwater_items;i++)
		{
			pan_setstring(terrwater_panel, 0,	55,350+i*15,	panel_font,			str_create( terrwater_filenames[i] ) );
		}
	
	// buttons 1-8 (checkbox)
	for(i=0;i<max_terrwater_items;i++)
		{
			pan_setbutton(terrwater_panel, 0,	2, 	15,350+i*15,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerrWater_Flag_Toggle,NULL,NULL); 
		}
	// button states
	for(i=0;i<max_terrwater_items;i++)
		{
			button_state( terrwater_panel , 1+i , terrwater_switch_type[i] ); 		// 1 ON, 0 OFF
		}
	
	// buttons 9-10 - set
	pan_setdigits(terrwater_panel, 0,	180,330,	"Set:    %0.f",		panel_font,	1, terrwater_set);	
	pan_setbutton(terrwater_panel, 0,	1, 	165,330,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerrWater_DecSet, 		NULL, NULL); 
	pan_setbutton(terrwater_panel, 0,	1, 	225,330,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerrWater_IncSet, 		NULL, NULL); 
	
	//--------------------------------------------------------------
	
	// button 11 - apply
	pan_setbutton(terrwater_panel, 0,	1, 	45,480,		m_apply_on_bmp,m_apply_off_bmp,m_apply_over_bmp,m_apply_over_bmp,						TerrWater_CreateObject,NULL,NULL); 
	
	//--------------------------------------------------------------
	
	// button 12 - clear all
	pan_setbutton(terrwater_panel, 0,	1, 	145,480,		m_clearall_on_bmp,m_clearall_off_bmp,m_clearall_over_bmp,m_clearall_over_bmp,		TerrWater_DeleteAll,NULL,NULL); 
	
	//--------------------------------------------------------------
	
	ptr_remove( temp_string );	
	
	
}


void		TerrWater_Panel_Toggle()
{
	toggle( terrwater_panel , SHOW );	
}


////////////////////////////////////////////////////


void		TerrWater_Panel_Button(button_number)
{
	// not used
}


////////////////////////////////////////////////////


void		TerrWater_IncSet()
{
	if (proc_status(TerrWater_IncSet) > 0) 			return;
	if (proc_status(TerrWater_UpdateSetNames) > 0) return;
//	if (!terrwater_ready) 									return;
//	terrwater_ready = 0;
	wait(3);
	
	terrwater_set++;
	if (terrwater_set>max_terrwater_sets)
		{
			terrwater_set = 1;
		}
	TerrWater_UpdateSetNames();
	wait_for(TerrWater_UpdateSetNames);
	
	wait(1);
}


void		TerrWater_DecSet()
{
	if (proc_status(TerrWater_IncSet) > 0) 			return;
	if (proc_status(TerrWater_UpdateSetNames) > 0) return;
//	if (!terrwater_ready) 									return;
//	terrwater_ready = 0;
	wait(3);
	
	terrwater_set--;
	if (terrwater_set<1)
		{
			terrwater_set = max_terrwater_sets;
		}
	TerrWater_UpdateSetNames();
	wait_for(TerrWater_UpdateSetNames);
	
	wait(1);
}


void		TerrWater_UpdateSetNames()
{
	if (proc_status(TerrWater_UpdateSetNames) > 0) return;
	
	int i;
	
	// copy the proper 8 filenames to string array
	int j = terrwater_set - 1;
	int k = j * max_terrwater_items;
	for(i=0;i<max_terrwater_items;i++)
		{
			str_cpy( terrwater_filenames[i] , "" );													
			str_cat( terrwater_filenames[i] , (terrwaters_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi( terrwater_filenames[i] , "" ) )											// if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( terrwater_filenames[i] , "-" );
				}
		}
	
	// refresh 8 filename strings to display in menu
	for(i=0;i<max_terrwater_items;i++)
		{
			pan_setstring(terrwater_panel, 11+i,	55,350+i*15,	panel_font,			str_create( terrwater_filenames[i] ) );
		}	
	wait(1);
}


///////////////////////////////////


// change by checkbox (buttons 1-8)
void		TerrWater_Flag_Toggle(button_number)
{
	if (proc_status(TerrWater_Flag_Toggle) > 0) return;
//	if (!terrwater_ready) 								return;
//	terrwater_ready = 0;
	wait(3);
	
	int i;
	for(i=0;i<max_terrwater_items;i++)
		{
			terrwater_switch_type[i] = 0;							// 0..7
			button_state( terrwater_panel , i+1 , 0 );			// 1..8
		}
	terrwater_switch_type[button_number-1] = 1;
	button_state( terrwater_panel , button_number , 1 ); 
	terrwater_selected = button_number-1;
	
	wait(1);
}


// change by hotkey
void		TerrWater_Flag_Switch(var setplusone)
{
	if (proc_status(TerrWater_Flag_Switch) > 0) return;
//	if (!terrwater_ready) 								return;
//	terrwater_ready = 0;
	wait(3);
	
	int temp = setplusone -1 -1;										// -1 because of scancode, and -1 because of array pos are 0..7	
	int i;
	for(i=0;i<max_terrwater_items;i++)
		{
			terrwater_switch_type[i] = 0;							// 0..7
			button_state( terrwater_panel , i+1 , 0 );			// 1..8
		}
	terrwater_switch_type[temp] = 1;
	button_state( terrwater_panel , temp+1 , 1 ); 
	terrwater_selected = temp;
	
	wait(1);
}


/////////////////////////////////////////////////////////


void		TerrWater_MaterialSetup()
{
	wait(3);
	material_select = 11;
	
	if (water_entity)
		{
			if (water_entity.material == water_multi_mat)
				{
					Material_MatPanel_Init( water_multi_mat , water_single_mat );
				}
			else if (water_entity.material == water_single_mat)
				{
					Material_MatPanel_Init( water_single_mat , water_multi_mat );
				}
			else
				{
					Material_MatPanel_Init( water_entity.material , NULL );
				}
								
			if (main_water > (var)0)
				{
					// shade-c water - water editor	
					Material_ScWaterPanel12_Init();
				}
			else if (main_water == (var)0)
				{
					// 3dgs mirror water - water editor
					Material_MWaterPanel_Init();
				}
		}
	else
		{
			Material_MatPanel_Init( water_multi_mat , water_single_mat );	// default
		}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor placement


void	 TerrWater_CreateObject()
{
//	if (!terrwater_ready) return;
	
	STRING* temp_str = str_create("");
	if ( !str_cmpi(terrwater_filenames[terrwater_selected] , "-" ) )
		{					
			str_cpy( temp_str , hmpw_foldername );
			str_cat( temp_str , terrwater_filenames[terrwater_selected] );
			
			//----------------------------------------------------------------------------
			// remove object
			
			TerrHmp_RemoveWater();
			wait_for(TerrHmp_RemoveWater);
			
			SetMapWaterName( "-" ); 	
			
			wait(3);
			
			//----------------------------------------------------------------------------
			// create object
						
			TerrHmp_CreateWater(temp_str);
			wait_for(TerrHmp_CreateWater);
			
			SetMapWaterName( temp_str );
			
			//----------------------------------------------------------------------------
//			
//			ent_create( temp_str , nullvector , TerrHmp_WaterAction );
//			// to get its filename later: str_for_entfile( str_entfilename , my );	
//			
			//----------------------------------------------------------------------------
		}
//	wait(1);
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor deletion


void		TerrWater_DeleteAll()
{
	wait(3);

	TerrHmp_RemoveWater();
	wait_for(TerrHmp_RemoveWater);
	
	SetMapWaterName( "-" ); 	
	
	// recalc level_ent size... not needed, auto set on next load or on terrain creation
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif