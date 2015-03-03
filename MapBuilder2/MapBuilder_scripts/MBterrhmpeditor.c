
#ifndef MBterrhmpeditor_c
#define MBterrhmpeditor_c 

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor initialization


void 		TerrGround_Init()
{
//	terrground_ready = 0;
	
	//----------------------------------------------------------------
	// terrground filename initialization 

	int arraysize = (int)TERRGROUND_MAXPERSET;
	terrground_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );
		
	// init filename string array elements
	int i;
	for(i=0;i<max_terrground_items;i++)
		{
			terrground_filenames[i] = str_create("");
		}	
	
	//-----------------------------------------------------------------
	// determine how many items are in the folder, 
	// divide it by 8, and the result is the number of sets
	
	max_terrground_sets = TERRGROUND_MAXSET;
	terrgrounds_txt = txt_create( (max_terrground_sets*max_terrground_items) , 0 );		// 10*8=80
	
	for(i=0;i<max_terrground_sets*max_terrground_items;i++)
		{
			str_cpy( (terrgrounds_txt->pstring)[i] , "" );
		}
	
	//-------------------------------------------------------------------
	// read in folder content
	
	STRING* temp_str = str_create("");
	str_cpy( temp_str , filemenu_hmp_filter );

	txt_for_dir( terrgrounds_txt, temp_str );		
	
	i = 0;						
	while (str_len((terrgrounds_txt->pstring)[i]) > 0)
		{
			i++;
		}
		
	max_terrground_sets = integer(i/max_terrground_items);
	if ( fraction(i/max_terrground_items) > 0)
		{
			max_terrground_sets++;																			// allow to display not complete sets too
		}
	terrground_set = clamp(terrground_set, 1, max_terrground_sets);
	
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	
	for(i=0;i<max_terrground_items;i++)
		{		
			str_cpy( terrground_filenames[i] , "" );													
			int j = (terrground_set - 1) * max_terrground_items;
			str_cat( terrground_filenames[i] , (terrgrounds_txt->pstring)[j+i] );			// filename with extension
			if ( str_cmpi( terrground_filenames[i] , "" ) )											// if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( terrground_filenames[i] , "-" );
				}
		}
	
	//------------------------------------------------------------------
	// initial value settings
	
	for(i=0;i<max_terrground_items;i++)
		{
			terrground_switch_type[i] = 0;
		}
	terrground_switch_type[terrground_selected] = 1;	

	//-------------------------------------------------------------------
	// start panel
	
	TerrGround_Panel_Init();
	wait_for(TerrGround_Panel_Init);
	
	//-------------------------------------------------------------------
	
	while(UIMode==(var)UIMODE_GROUNDTERR)
		{
			wait(1);
		}
	
	TerrGround_Close();
}


void		TerrGround_Close()
{
	//--------------------------------------------------------
	
//	terrground_ready = 0;
	
	ptr_remove(terrground_panel);
	terrground_panel = NULL;
		
	//--------------------------------------------------------
	
	int i;
	
	// remove temp text
	for (i = 0; i < terrgrounds_txt->strings; i++) 
		if ((terrgrounds_txt->pstring)[i]) 
			ptr_remove((terrgrounds_txt->pstring)[i]);
	ptr_remove(terrgrounds_txt);
	terrgrounds_txt = NULL;
	
	// remove temp string array
	for(i=0;i<max_terrground_items;i++)
		{
			ptr_remove(terrground_filenames[i]);
		}
	sys_free(terrground_filenames);
	terrground_filenames = NULL;
//	ptr_remove(terrground_filenames);									// no!
	
	//--------------------------------------------------------
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor menu


void		TerrGround_Panel_Init()
{
	//--------------------------------------------------------------
	
	int i;
	STRING* temp_string = str_create(""); 
	
	//--------------------------------------------------------------
	
	terrground_panel = pan_create( panel_image2 , 11 );
	set( terrground_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	terrground_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	terrground_panel.pos_y 	= ui_submenupanel_y;								// 256;
	terrground_panel.red		= panel_font_r;
	terrground_panel.green	= panel_font_g;
	terrground_panel.blue	= panel_font_b;
	terrground_panel.event	= ui_submenu_panelmove;
	
	//--------------------------------------------------------------
	
	// string 1
	pan_setstring(terrground_panel, 0,	15,10,	panel_font2,			str_create("GROUND TERRAIN PLACEMENT") );
	
	//--------------------------------------------------------------
	
	// string 2
	pan_setstring(terrground_panel, 0,	15,330,	panel_font,			str_create("Select item") );
	
	// strings 3-10 - terrground item numbers 1-8
	for(i=0;i<max_terrground_items;i++)
		{
			str_for_int( temp_string , (i+1) );
			pan_setstring( terrground_panel, 0,	35,350+i*15,	panel_font,			str_create( temp_string ) );
		}
	
	// strings 11-18 - terrground item filenames 1-8
	for(i=0;i<max_terrground_items;i++)
		{
			pan_setstring(terrground_panel, 0,	55,350+i*15,	panel_font,			str_create( terrground_filenames[i] ) );
		}
	
	// buttons 1-8 (checkbox)
	for(i=0;i<max_terrground_items;i++)
		{
			pan_setbutton(terrground_panel, 0,	2, 	15,350+i*15,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		TerrGround_Flag_Toggle,NULL,NULL); 
		}
	// button states
	for(i=0;i<max_terrground_items;i++)
		{
			button_state( terrground_panel , 1+i , terrground_switch_type[i] ); 		// 1 ON, 0 OFF
		}
	
	// buttons 9-10 - set
	pan_setdigits(terrground_panel, 0,	180,330,	"Set:    %0.f",		panel_font,	1, terrground_set);	
	pan_setbutton(terrground_panel, 0,	1, 	165,330,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerrGround_DecSet, 		NULL, NULL); 
	pan_setbutton(terrground_panel, 0,	1, 	225,330,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerrGround_IncSet, 		NULL, NULL); 
	
	//--------------------------------------------------------------
	
	// button 11 - apply
	pan_setbutton(terrground_panel, 0,	1, 	45,480,		m_apply_on_bmp,m_apply_off_bmp,m_apply_over_bmp,m_apply_over_bmp,						TerrGround_CreateObject,NULL,NULL); 
	
	//--------------------------------------------------------------
	
	// button 12 - clear all
	pan_setbutton(terrground_panel, 0,	1, 	145,480,		m_clearall_on_bmp,m_clearall_off_bmp,m_clearall_over_bmp,m_clearall_over_bmp,		TerrGround_DeleteAll,NULL,NULL); 
	
	//--------------------------------------------------------------
	
	ptr_remove( temp_string );	
	
	
}


void		TerrGround_Panel_Toggle()
{
	toggle( terrground_panel , SHOW );	
}


////////////////////////////////////////////////////


void		TerrGround_Panel_Button(button_number)
{
	// not used
}


////////////////////////////////////////////////////


void		TerrGround_IncSet()
{
	if (proc_status(TerrGround_IncSet) > 0) 			return;
	if (proc_status(TerrGround_UpdateSetNames) > 0) return;
//	if (!terrground_ready) 									return;
//	terrground_ready = 0;
	wait(3);
	
	terrground_set++;
	if (terrground_set>max_terrground_sets)
		{
			terrground_set = 1;
		}
	TerrGround_UpdateSetNames();
	wait_for(TerrGround_UpdateSetNames);
	
	wait(1);
}


void		TerrGround_DecSet()
{
	if (proc_status(TerrGround_IncSet) > 0) 			return;
	if (proc_status(TerrGround_UpdateSetNames) > 0) return;
//	if (!terrground_ready) 									return;
//	terrground_ready = 0;
	wait(3);
	
	terrground_set--;
	if (terrground_set<1)
		{
			terrground_set = max_terrground_sets;
		}
	TerrGround_UpdateSetNames();
	wait_for(TerrGround_UpdateSetNames);
	
	wait(1);
}


void		TerrGround_UpdateSetNames()
{
	if (proc_status(TerrGround_UpdateSetNames) > 0) return;
	
	int i;
	
	// copy the proper 8 filenames to string array
	int j = terrground_set - 1;
	int k = j * max_terrground_items;
	for(i=0;i<max_terrground_items;i++)
		{
			str_cpy( terrground_filenames[i] , "" );													
			str_cat( terrground_filenames[i] , (terrgrounds_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi( terrground_filenames[i] , "" ) )											// if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( terrground_filenames[i] , "-" );
				}
		}
	
	// refresh 8 filename strings to display in menu
	for(i=0;i<max_terrground_items;i++)
		{
			pan_setstring(terrground_panel, 11+i,	55,350+i*15,	panel_font,			str_create( terrground_filenames[i] ) );
		}	
	wait(1);
}


///////////////////////////////////


// change by checkbox (buttons 1-8)
void		TerrGround_Flag_Toggle(button_number)
{
	if (proc_status(TerrGround_Flag_Toggle) > 0) return;
//	if (!terrground_ready) 								return;
//	terrground_ready = 0;
	wait(3);
	
	int i;
	for(i=0;i<max_terrground_items;i++)
		{
			terrground_switch_type[i] = 0;							// 0..7
			button_state( terrground_panel , i+1 , 0 );			// 1..8
		}
	terrground_switch_type[button_number-1] = 1;
	button_state( terrground_panel , button_number , 1 ); 
	terrground_selected = button_number-1;
	
	wait(1);
}


// change by hotkey
void		TerrGround_Flag_Switch(var setplusone)
{
	if (proc_status(TerrGround_Flag_Switch) > 0) return;
//	if (!terrground_ready) 								return;
//	terrground_ready = 0;
	wait(3);
	
	int temp = setplusone -1 -1;										// -1 because of scancode, and -1 because of array pos are 0..7	
	int i;
	for(i=0;i<max_terrground_items;i++)
		{
			terrground_switch_type[i] = 0;							// 0..7
			button_state( terrground_panel , i+1 , 0 );			// 1..8
		}
	terrground_switch_type[temp] = 1;
	button_state( terrground_panel , temp+1 , 1 ); 
	terrground_selected = temp;
	
	wait(1);
}


/////////////////////////////////////////////////////////


void		TerrGround_MaterialSetup()
{
	wait(3);
	material_select = 10;
	
	if (terrain_entity)
		{
			if (terrain_entity.material == terrain_multi_mat)
				{
					Material_MatPanel_Init( terrain_multi_mat , terrain_single_mat );
				}
			else if (terrain_entity.material == terrain_single_mat)
				{
					Material_MatPanel_Init( terrain_single_mat , terrain_multi_mat );
				}
			else
				{
					Material_MatPanel_Init( terrain_entity.material , NULL );
				}
			
			if (str_stri(terrain_entity.material.effect, "auto"))
				{
					Material_TerrhmpAutoTexPanel_Init();
				}
		}
	else
		{
			Material_MatPanel_Init( terrain_single_mat , terrain_multi_mat );		// default
		}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor placement


void	 TerrGround_CreateObject()
{
//	if (!terrground_ready) return;
	
	STRING* temp_str = str_create("");
	if ( !str_cmpi(terrground_filenames[terrground_selected] , "-" ) )
		{					
			str_cpy( temp_str , hmp_foldername );
			str_cat( temp_str , terrground_filenames[terrground_selected] );
			
			//----------------------------------------------------------------------------
			// remove object
			
			TerrHmp_RemoveGround();
			wait_for(TerrHmp_RemoveGround);
			
			SetMapGroundName( "-" ); 	
			
			wait(3);
			
			//----------------------------------------------------------------------------
			// create object
						
			TerrHmp_CreateGround(temp_str);
			wait_for(TerrHmp_CreateGround);
			
			SetMapGroundName( temp_str );
			
			//----------------------------------------------------------------------------
//			
//			ent_create( temp_str , nullvector , TerrHmp_GroundAction );
//			// to get its filename later: str_for_entfile( str_entfilename , my );	
//			
			//----------------------------------------------------------------------------
			// scale water
			
			if (water_entity)
				{
					TerrHmp_RemoveWater();
					wait_for(TerrHmp_RemoveWater);
					
					wait(3);
					
					TerrHmp_CreateWater(GetMapWaterName());
					wait_for(TerrHmp_CreateWater);
				}
			
			//----------------------------------------------------------------------------
		}
//	wait(1);
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// editor deletion


void		TerrGround_DeleteAll()
{
	wait(3);

	TerrHmp_RemoveGround();
	wait_for(TerrHmp_RemoveGround);
	
	SetMapGroundName( "-" ); 	
	
	// recalc level_ent size... not needed, auto set on next load or on terrain creation
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif