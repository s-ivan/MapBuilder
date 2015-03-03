////////////////////////////////////////////////////////
// height editor within terrain editor
////////////////////////////////////////////////////////

#ifndef MBteredit_he_c
#define MBteredit_he_c


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


void		TerEdit_He_Init()
{
	int i;
	STRING* temp_string = str_create("");
	
	teredit_he_panel = pan_create( NULL , 12 );
	set( teredit_he_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	teredit_he_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	teredit_he_panel.pos_y 	= ui_submenupanel_y + 128;						// 256+128;
	teredit_he_panel.red		= panel_font_r;
	teredit_he_panel.green	= panel_font_g;
	teredit_he_panel.blue	= panel_font_b;
	teredit_he_panel.size_x	= 256;
	teredit_he_panel.size_y	= 512-128;
	
	teredit_he_mode			= 1;
	
	//--------------------------------------------------------------------
	// title
	
	// string 1
	pan_setstring(teredit_he_panel, 0,	15,15,	panel_font2,			str_create("EDIT HEIGHT") );
	
	//--------------------------------------------------------------------
	// button 1 2 - radio buttons - height editing mode selection
	pan_setstring(teredit_he_panel, 0,	35,45,	panel_font,			str_create("raise / lower") );
	pan_setbutton(teredit_he_panel, 0,	4, 	15,45,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		TerEdit_He_ToggleMode,		NULL,NULL); 
	if (teredit_he_mode==(var)1) button_state( teredit_he_panel , 1 , ON ); 		// 1 ON, 0 OFF	
	
//	pan_setstring(teredit_he_panel, 0,	95,45,	panel_font,			str_create("lower") );
//	pan_setbutton(teredit_he_panel, 0,	4, 	75,45,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		TerEdit_He_ToggleMode,		NULL,NULL); 
//	if (teredit_he_mode==(var)2) button_state( teredit_he_panel , 2 , ON ); 		// 1 ON, 0 OFF	
	
	pan_setstring(teredit_he_panel, 0,	160,45,	panel_font,			str_create("set / smooth") );
	pan_setbutton(teredit_he_panel, 0,	4, 	140,45,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		TerEdit_He_ToggleMode,		NULL,NULL); 
	if (teredit_he_mode==(var)3) button_state( teredit_he_panel , 3 , ON ); 		// 1 ON, 0 OFF	
	
//	pan_setstring(teredit_he_panel, 0,	225,45,	panel_font,			str_create("set") );
//	pan_setbutton(teredit_he_panel, 0,	4, 	205,45,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		TerEdit_He_ToggleMode,		NULL,NULL); 
//	if (teredit_he_mode==(var)4) button_state( teredit_he_panel , 4 , ON ); 		// 1 ON, 0 OFF	
	
	// slider1 - max
	pan_setdigits(teredit_he_panel, 0,	15,70,		"Max: %0.f",		panel_font,	1, 						teredit_max );
	pan_setslider(teredit_he_panel, 0,	90+12,72,	horiz_slider_bmp,slider_knob_bmp,	0,5000,			teredit_max ); 
	
	// slider2 - min
	pan_setdigits(teredit_he_panel, 0,	15,90,		"Min: %0.f",		panel_font,	1, 						teredit_min );
	pan_setslider(teredit_he_panel, 0,	90+12,92,	horiz_slider_bmp,slider_knob_bmp,	-1000,0,			teredit_min ); 
	
	// slider3 - adj(ust)
	pan_setdigits(teredit_he_panel, 0,	15,110,		"Adjust: %0.f",		panel_font,	1, 					teredit_adj );
	pan_setslider(teredit_he_panel, 0,	90+12,112,	horiz_slider_bmp,slider_knob_bmp,	0,300,			teredit_adj ); 
	
	// slider4 - set
	pan_setdigits(teredit_he_panel, 0,	15,130,		"Set: %0.f",		panel_font,	1, 						teredit_set );
	pan_setslider(teredit_he_panel, 0,	90+12,132,	horiz_slider_bmp,slider_knob_bmp,	-1000,5000,		teredit_set ); 
	
	// buttons 3 4, 5 6, 7 8, 9 10 - arrows
	pan_setbutton(teredit_he_panel, 0,	1, 	90,70,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_He_Button, 		NULL, NULL); 
	pan_setbutton(teredit_he_panel, 0,	1, 	210,70,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_He_Button, 		NULL, NULL); 
	
	pan_setbutton(teredit_he_panel, 0,	1, 	90,90,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_He_Button, 		NULL, NULL); 
	pan_setbutton(teredit_he_panel, 0,	1, 	210,90,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_He_Button, 		NULL, NULL); 
	
	pan_setbutton(teredit_he_panel, 0,	1, 	90,110,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_He_Button, 		NULL, NULL); 
	pan_setbutton(teredit_he_panel, 0,	1, 	210,110,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_He_Button, 		NULL, NULL); 
	
	pan_setbutton(teredit_he_panel, 0,	1, 	90,130,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_He_Button, 		NULL, NULL); 
	pan_setbutton(teredit_he_panel, 0,	1, 	210,130,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_He_Button, 		NULL, NULL); 
	
	// button 11 - raise by adjust
	pan_setstring(teredit_he_panel, 0,	15,155,	panel_font,			str_create("Raise terrain by 'adjust' ") );
	pan_setbutton(teredit_he_panel, 0,	1, 	145,155,		ss_raise_on_bmp,ss_raise_off_bmp,ss_raise_over_bmp,ss_raise_over_bmp,		TerEdit_He_RaiseAll,		NULL,NULL); 	
	
	// button 12 - lower by adjust
	pan_setstring(teredit_he_panel, 0,	15,175,	panel_font,			str_create("Lower terrain by 'adjust' ") );
	pan_setbutton(teredit_he_panel, 0,	1, 	145,175,		ss_lower_on_bmp,ss_lower_off_bmp,ss_lower_over_bmp,ss_lower_over_bmp,		TerEdit_He_LowerAll,		NULL,NULL); 	
		
	// button 13 - flat to level given by set - should be removed !!!
//	pan_setstring(teredit_he_panel, 0,	15,205,	panel_font,			str_create("Flatten terrain to 'set' ") );
//	pan_setbutton(teredit_he_panel, 0,	1, 	145,205,		ss_apply_on_bmp,ss_apply_off_bmp,ss_apply_over_bmp,ss_apply_over_bmp,		TerEdit_He_SetAll,		NULL,NULL);
	
	// button numbers are = -2 !!! below this row
	
	// button 14 - undo
	pan_setbutton(teredit_he_panel, 0,	1, 	25,245,		ss_undo_on_bmp,ss_undo_off_bmp,ss_undo_over_bmp,ss_undo_over_bmp,			TerEdit_He_Undo,		NULL,NULL);
	
	// button 15 - redo
	pan_setbutton(teredit_he_panel, 0,	1, 	100,245,		ss_redo_on_bmp,ss_redo_off_bmp,ss_redo_over_bmp,ss_redo_over_bmp,			TerEdit_He_Redo,		NULL,NULL);
	
	// button 16 - reset
	pan_setbutton(teredit_he_panel, 0,	1, 	175,245,		ss_reset_on_bmp,ss_reset_off_bmp,ss_reset_over_bmp,ss_reset_over_bmp,	TerEdit_He_Reset,		NULL,NULL);
	
	// button 17 - save vertices
	pan_setstring(teredit_he_panel, 0,	15,275,	panel_font,			str_create("MB mesh modifier [.trv]") );
	pan_setbutton(teredit_he_panel, 0,	1, 	145,275,		ss_save_on_bmp,ss_save_off_bmp,ss_save_over_bmp,ss_save_over_bmp,		TerEdit_Save_Vertices,		NULL,NULL);
	
	// button 18 - load vertices
	pan_setbutton(teredit_he_panel, 0,	1, 	200,275,		ss_load_on_bmp,ss_load_off_bmp,ss_load_over_bmp,ss_load_over_bmp,		TerEdit_Load_Vertices,		NULL,NULL);
	
	// button 19 - save hmp
	pan_setstring(teredit_he_panel, 0,	15,300,	panel_font,			str_create("3DGS terrain [.hmp]") );
	pan_setbutton(teredit_he_panel, 0,	1, 	145,300,		ss_save_on_bmp,ss_save_off_bmp,ss_save_over_bmp,ss_save_over_bmp,		TerEdit_Save_Terrain,		NULL,NULL);
	
	// button 20 - update surface
	pan_setstring(teredit_he_panel, 0,	15,325,	panel_font,			str_create("[U] Update terrain hull") );
	pan_setbutton(teredit_he_panel, 0,	1, 	145,325,		ss_update_on_bmp,ss_update_off_bmp,ss_update_over_bmp,ss_update_over_bmp,		TerrHmp_FixNormals,			NULL,NULL);
	
	// button 21 - update entities
	pan_setstring(teredit_he_panel, 0,	15,350,	panel_font,			str_create("[CTRL-U] Update entities") );
	pan_setbutton(teredit_he_panel, 0,	1, 	145,350,		ss_update_on_bmp,ss_update_off_bmp,ss_update_over_bmp,ss_update_over_bmp,		TerEdit_UpdateEntities,		NULL,NULL);

	// string - subtitle
	pan_setstring(teredit_he_panel, 0,	105,15,	panel_font,			str_create("( left-click / right-click )") );
	
	//---
	
	// button 22 - min to set
	pan_setstring(teredit_he_panel, 0,	15,195,	panel_font,			str_create("Set minimum to 'set' ") );
	pan_setbutton(teredit_he_panel, 0,	1, 	145,195,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		TerEdit_He_SetMin,		NULL,NULL); 	
	
	// button 23 - max to set
	pan_setstring(teredit_he_panel, 0,	15,215,	panel_font,			str_create("Set maximum to 'set' ") );
	pan_setbutton(teredit_he_panel, 0,	1, 	145,215,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		TerEdit_He_SetMax,		NULL,NULL); 	
		
	//---	
	
	ptr_remove(temp_string);
}


void		TerEdit_He_Close()
{
	if (teredit_he_panel)
		{
			ptr_remove(teredit_he_panel);
			teredit_he_panel = NULL;
		}
}


void		TerEdit_He_Button(button_number)
{
	if (button_number==(var)3)
		{
			teredit_max -= 10;
			teredit_max = maxv( 0 , teredit_max );
		}
	else if (button_number==(var)4)
		{
			teredit_max += 10;
			teredit_max = minv( 5000 , teredit_max );
		}
	else if (button_number==(var)5)
		{
			teredit_min -= 10;
			teredit_min = maxv( -1000 , teredit_min );
		}
	else if (button_number==(var)6)
		{
			teredit_min += 10;
			teredit_min = minv( 0 , teredit_min );
		}
	else if (button_number==(var)7)
		{
			teredit_adj -= 1;
			teredit_adj = maxv( 0 , teredit_adj );
		}
	else if (button_number==(var)8)
		{
			teredit_adj += 1;
			teredit_adj = minv( 300 , teredit_adj );
		}
	else if (button_number==(var)9)
		{
			teredit_set -= 10;
			teredit_set = maxv( -1000 , teredit_set );
		}
	else if (button_number==(var)10)
		{
			teredit_set += 10;
			teredit_set = minv( 5000 , teredit_set );
		}
}



void		TerEdit_He_ToggleMode(button_number)
{
	teredit_he_mode = button_number;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_He_InitTerrain()													
{
	teredit_he_undostep = 1;														// 0..9 - shows the last undo array written
	teredit_he_firstundo = 0;
	
//	if (teredit_vertexarray) 														// safety check - temp
//		{
//			free(teredit_vertexarray);
//			teredit_vertexarray = NULL;											// reinit after freed 
//		}
	
	int j = ent_status(terrain_entity,0); 										// number of vertices 
	if (teredit_vertexarray == NULL)												// safety check 
		{
sys_marker("te1");
			teredit_vertexarray = (VERTEXSYS*)sys_malloc( j * sizeof(VERTEXSYS) );	 			
		
			int i;			
			// fill values
			for (j=1; j<=ent_status(terrain_entity,0); j++)
				{
					CONTACT* c = ent_getvertex(terrain_entity,NULL,j);   					// vertex number begins with 1 !
					
					teredit_vertexarray[j-1].original	= (var)c.v.y;						// DirectX coordinate ! y up ! float !
//					teredit_vertexarray[j-1].actual		= (var)c.v.y;			
					for (i=0;i<10;i++)
						{
							teredit_vertexarray[j-1].undo[i]		= (var)c.v.y;	
						}
				}
sys_marker(NULL);					
		}
	else
		{
			printf( "TerEdit VertexArray Error: !NULL" );
//			Main_Exit();
		}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_He_SetAll()						// not used - not important - can be removed
{
	return;// disabled
	
	teredit_raiselowerflatten = 1;
	TerEdit_He_SetTerrain( teredit_set, 0 );	
}


void		TerEdit_He_RaiseAll()
{
	teredit_raiselowerflatten = 1;
	TerEdit_He_SetTerrain( teredit_adj, 1 );	
}


void		TerEdit_He_LowerAll()
{
	teredit_raiselowerflatten = 1;
	TerEdit_He_SetTerrain( -teredit_adj, 1 );	
}


////////////////////////////////////////////////////////


void		TerEdit_He_SetTerrain(var worklevel, var workmode)
{
	// to set workmode=0 is removed !!! because of problems with saving, and this feature is not really important !!!
	
	// manage undo array counters
	teredit_he_undostep++;
	if (teredit_he_undostep>9)
		{
			teredit_he_undostep = 0;
		}
	if (teredit_he_undostep==teredit_he_firstundo)
		{
			teredit_he_firstundo++;
			if (teredit_he_firstundo>9)
				{
					teredit_he_firstundo = 0;
				}
		}
	//---------------------------
//	int j = ent_status(terrain_entity,0); 										// number of vertices 
//	for(; j>0; j--) 
	int j;
	for (j=1; j<=ent_status(terrain_entity,0); j++)
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,j);   			// vertex number begins with 1 !
//			if (workmode==(var)0)													// not saved by terrain_resave.dll
//				{
//					c.v.y = (float)worklevel; // + (float)terrain_entity.z;	// DirectX coordinate ! y up ! float !											
//					// or
////					c.z = (float)worklevel; 										// increase the vertex height
////					c.v = NULL; 														// c.x,y,z was changed, instead of c.v 					
//				}
//			else
				{
					c.v.y += (float)worklevel;										// DirectX coordinate ! y up ! float !					
					// or
//					c.z += (float)worklevel; 										// increase the vertex height
//					c.v = NULL; 														// c.x,y,z was changed, instead of c.v 					
				}
			ent_setvertex(terrain_entity,c,j);
			
//			teredit_vertexarray[j-1].actual = (var)c.v.y;
			// new undo step
			teredit_vertexarray[j-1].undo[teredit_he_undostep] = (var)c.v.y;											
		}
//	wait(1);
//	c_setminmax(terrain_entity);			// does not work
//	c_updatehull(terrain_entity,0);		// works fine
//	ent_fixnormals(terrain_entity,0);	// not okay for chunked terrain - cause error around 1st chunk border
	
//	TerrHmp_FixNormals();					// not needed as hull is now updated properly in A8.40.3 and normals did not change
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_He_SetMin()
{
	teredit_raiselowerflatten = 1;
	
	teredit_he_undostep++;
	if (teredit_he_undostep>9)
		{
			teredit_he_undostep = 0;
		}
	if (teredit_he_undostep==teredit_he_firstundo)
		{
			teredit_he_firstundo++;
			if (teredit_he_firstundo>9)
				{
					teredit_he_firstundo = 0;
				}
		}
	//---------------------------
	int j;
	for (j=1; j<=ent_status(terrain_entity,0); j++)
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,j);   			// vertex number begins with 1 !
			
			c.v.y = (float)maxv(teredit_set, (var)c.v.y);					// DirectX coordinate ! y up ! float !					
			
			ent_setvertex(terrain_entity,c,j);
			
			// new undo step
			teredit_vertexarray[j-1].undo[teredit_he_undostep] = (var)c.v.y;											
		}
}


void		TerEdit_He_SetMax()
{
	teredit_raiselowerflatten = 1;
	
	teredit_he_undostep++;
	if (teredit_he_undostep>9)
		{
			teredit_he_undostep = 0;
		}
	if (teredit_he_undostep==teredit_he_firstundo)
		{
			teredit_he_firstundo++;
			if (teredit_he_firstundo>9)
				{
					teredit_he_firstundo = 0;
				}
		}
	//---------------------------
	int j;
	for (j=1; j<=ent_status(terrain_entity,0); j++)
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,j);   			// vertex number begins with 1 !
			
			c.v.y = (float)minv(teredit_set, (var)c.v.y);					// DirectX coordinate ! y up ! float !					
			
			ent_setvertex(terrain_entity,c,j);
			
			// new undo step
			teredit_vertexarray[j-1].undo[teredit_he_undostep] = (var)c.v.y;											
		}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void	 	TerEdit_He_RaiseLowerArea(var raise_lower)				// brush based
{
	if (proc_status(TerEdit_He_RaiseLowerArea) > 0) return;
	
	// manage undo array counters
	teredit_he_undostep++;
	if (teredit_he_undostep>9)
		{
			teredit_he_undostep = 0;
		}
	if (teredit_he_undostep==teredit_he_firstundo)
		{
			teredit_he_firstundo++;
			if (teredit_he_firstundo>9)
				{
					teredit_he_firstundo = 0;
				}
		}
	//---------------------------
	// determine upper left and lower right vertex corners, i.e. get x,y vertex count within cube area
	
sys_marker("R01");	
	
	// x,y vertex size of terrain
	var vert_count_x = ent_status(terrain_entity,2)+1;	
	var vert_count_y = ent_status(terrain_entity,3)+1;
	
	// upleft and downright tile corner positions
	var upleft_pos_x = teredit_placement_cube.x - integer(teredit_tilesize/2)*GetMapTileSize();
	var upleft_pos_y = teredit_placement_cube.y + integer(teredit_tilesize/2)*GetMapTileSize();
	var upleft_pos_z = teredit_placement_cube.z; // PosInfo3D1(upleft_pos_x, upleft_pos_y);
	
	var dnright_pos_x = teredit_placement_cube.x + integer(teredit_tilesize/2)*GetMapTileSize();
	var dnright_pos_y = teredit_placement_cube.y - integer(teredit_tilesize/2)*GetMapTileSize();
	var dnright_pos_z = teredit_placement_cube.z; // PosInfo3D1(dnright_pos_x, dnright_pos_y);
	
	// corner vertices slosest to corner tile middles
	var upleft_vert  = ent_nextvertex(terrain_entity,vector(upleft_pos_x,upleft_pos_y,upleft_pos_z));
	var dnright_vert = ent_nextvertex(terrain_entity,vector(dnright_pos_x,dnright_pos_y,dnright_pos_z));
	
	// protection - no valid vertex found by ent_nextvertex
	if ((upleft_vert==(var)0) || (dnright_vert==(var)0))
		{
			return;
		}
		
	// vertices in a row & column
	var width_vert  = abs( dnright_vert%( ent_status(terrain_entity,2)+1 ) - upleft_vert%( ent_status(terrain_entity,2)+1 ) );
	var height_vert = abs( integer( dnright_vert /( ent_status(terrain_entity,2)+1 )) - integer( upleft_vert /( ent_status(terrain_entity,2)+1 )) );
	
	// actual brush image			
//	BMAP* temp_brush_bmp = bmap_create( teredit_br_filenames[teredit_br_selected] ); 
	BMAP* temp_brush_bmp;
	if (teredit_br_selected==(var)0)
		{
			temp_brush_bmp = teredit_brush1_bmp;
		}
	else if (teredit_br_selected==(var)1)
		{
			temp_brush_bmp = teredit_brush2_bmp;
		}
	else if (teredit_br_selected==(var)2)
		{
			temp_brush_bmp = teredit_brush3_bmp;
		}	
	else if (teredit_br_selected==(var)3)
		{
			temp_brush_bmp = teredit_brush4_bmp;
		}
	else if (teredit_br_selected==(var)4)
		{
			temp_brush_bmp = teredit_brush5_bmp;
		}
		
	// distance between vertices on brush image
	var step_bmp_x = bmap_width( temp_brush_bmp ) / width_vert ;
	var step_bmp_y = bmap_height( temp_brush_bmp ) / height_vert ;
	
	// read brush bitmap
	var format; 
	var pixel;
	COLOR pixel_color;
	var pixel_alpha;
	format = bmap_lock(temp_brush_bmp,0);

sys_marker(NULL);
sys_marker("R02");
	
	// do vertex moves
	int i,j,k;
	var m;
	for (i=0;i<width_vert;i++)
		{
			for (j=0;j<height_vert;j++)
				{
					// vertex number
					k = upleft_vert +j*vert_count_x +i;
					// protection
					if (k>ent_status(terrain_entity,0))
						{
							break;
						}
					
					// height modification based on brush pixel color 0..255 (red or green or blue)					
					// protection
					if ( (step_bmp_x*i>bmap_width(temp_brush_bmp)) || (step_bmp_y*j>bmap_height(temp_brush_bmp)) )
						{
							printf("out of bmap error");
							break;
						}
					// read brush color
					pixel = pixel_for_bmap( temp_brush_bmp , step_bmp_x*i , step_bmp_y*j );
					pixel_to_vec( pixel_color , pixel_alpha , format , pixel);						// gets pixel color and alpha										
					// iterate height modification value
					if (raise_lower<2)
						{
							m = pixel_color.blue *teredit_adj /255;		// 0 to adj			
		//					m = teredit_adj;	// temp test - okay
						}
					else
						{
							m = pixel_color.blue *teredit_set /255;		// 0 to set
						}
					
					CONTACT* c = ent_getvertex(terrain_entity,NULL,k);   			// vertex number begins with 1 !
					if (raise_lower==(var)1)
						{
							c.v.y += (float)m;												// + if raise, - if lower							
							// or
//							c.v = NULL; 														// c.x,y,z is changed, instead of c.v 
//							c.z += (float)m; 													// increase the vertex height												
						}
					else if (raise_lower==(var)0)
						{
							c.v.y -= (float)m;							
							// or
//							c.v = NULL; 														// c.x,y,z is changed, instead of c.v 
//							c.z -= (float)m; 													// increase the vertex height
						}
					else if (raise_lower==(var)2)
						{
							if (teredit_set>(var)c.v.y)									// new level is over surface
								{
									c.v.y = maxv( (float)m , c.v.y );
									// or
//									c.v = NULL; 												// c.x,y,z is changed, instead of c.v 
//									c.z = maxv( (float)m , c.z );							// set the vertex height
								}
							else if (teredit_set<(var)c.v.y)								// new level is below surface - inverse brush values needed
								{
									var n = teredit_set*2 - m;
									c.v.y = minv( (float)n , c.v.y );
									// or
//									c.v = NULL; 												// c.x,y,z is changed, instead of c.v 
//									c.z = minv( (float)m , c.z );							// set the vertex height
								}
						}
//#ifndef A8					
//					ent_setvertex(terrain_entity,c,0);	// due to A7 level.c
//#else
					ent_setvertex(terrain_entity,c,k);	// due to A8 level.c
//#endif
					// actual
//					teredit_vertexarray[k-1].actual = (var)c.v.y;	
					// new undo step
//					teredit_vertexarray[k-1].undo[teredit_he_undostep] = (var)c.v.y;			// BAD - whole evrtex array must be copied to not to keep non-corresponding data !
				}
		}
	
	TerEdit_He_StoreUndoStep();
		
sys_marker(NULL);
//	wait(1);
	bmap_unlock(temp_brush_bmp);
//	ptr_remove(temp_brush_bmp);									// if non created only equal it makes a crash
//	c_updatehull(terrain_entity,0);								// works fine but slow
//	ent_fixnormals(terrain_entity,0);							// not okay for chunked terrain - cause error around 1st chunk border
	
	wait(1);
	TerrHmp_FixNormals_Area(upleft_vert, width_vert, height_vert);
}										


void	 	TerEdit_He_SetArea()										// not brush based
{
	if (proc_status(TerEdit_He_SetArea) > 0) return;
	
	// manage undo array counters
	teredit_he_undostep++;
	if (teredit_he_undostep>9)
		{
			teredit_he_undostep = 0;
		}
	if (teredit_he_undostep==teredit_he_firstundo)
		{
			teredit_he_firstundo++;
			if (teredit_he_firstundo>9)
				{
					teredit_he_firstundo = 0;
				}
		}
	//---------------------------
	// determine upper left and lower right vertex corners, i.e. get x,y vertex count within cube area
	
sys_marker("R01");	
	
	// x,y vertex size of terrain
	var vert_count_x = ent_status(terrain_entity,2)+1;	
	var vert_count_y = ent_status(terrain_entity,3)+1;
	
	// upleft and downright tile corner positions
	var upleft_pos_x = teredit_placement_cube.x - integer(teredit_tilesize/2)*GetMapTileSize();
	var upleft_pos_y = teredit_placement_cube.y + integer(teredit_tilesize/2)*GetMapTileSize();
	var upleft_pos_z = teredit_placement_cube.z; // PosInfo3D1(upleft_pos_x, upleft_pos_y);
	
	var dnright_pos_x = teredit_placement_cube.x + integer(teredit_tilesize/2)*GetMapTileSize();
	var dnright_pos_y = teredit_placement_cube.y - integer(teredit_tilesize/2)*GetMapTileSize();
	var dnright_pos_z = teredit_placement_cube.z; // PosInfo3D1(dnright_pos_x, dnright_pos_y);
	
	// corner vertices slosest to corner tile middles
	var upleft_vert  = ent_nextvertex(terrain_entity,vector(upleft_pos_x,upleft_pos_y,upleft_pos_z));
	var dnright_vert = ent_nextvertex(terrain_entity,vector(dnright_pos_x,dnright_pos_y,dnright_pos_z));
	
	// protection - no valid vertex found by ent_nextvertex
	if ((upleft_vert==(var)0) || (dnright_vert==(var)0))
		{
			return;
		}
		
	// vertices in a row & column
	var width_vert  = abs( dnright_vert%( ent_status(terrain_entity,2)+1 ) - upleft_vert%( ent_status(terrain_entity,2)+1 ) );
	var height_vert = abs( integer( dnright_vert /( ent_status(terrain_entity,2)+1 )) - integer( upleft_vert /( ent_status(terrain_entity,2)+1 )) );

sys_marker(NULL);
sys_marker("R02");

	// do vertex moves
	int i,j,k;
	var m;
	for (i=0;i<width_vert;i++)
		{
			for (j=0;j<height_vert;j++)
				{
					// vertex number
					k = upleft_vert +j*vert_count_x +i;
					// protection
					if (k>ent_status(terrain_entity,0))
						{
							break;
						}
										
					m = teredit_set;	// temp test - okay
					CONTACT* c = ent_getvertex(terrain_entity,NULL,k);   			// vertex number begins with 1 !
					c.v.y = (float)m;
					ent_setvertex(terrain_entity,c,k);
					
					// actual
//					teredit_vertexarray[k-1].actual = (var)c.v.y;	
					// new undo step
//					teredit_vertexarray[k-1].undo[teredit_he_undostep] = (var)c.v.y;				// BAD - whole evrtex array must be copied to not to keep non-corresponding data !
				}
		}
sys_marker(NULL);
	
	TerEdit_He_StoreUndoStep();
	
//	wait(1);
//	c_updatehull(terrain_entity,0);		// works fine but slow
//	ent_fixnormals(terrain_entity,0);	// not okay for chunked terrain - cause error around 1st chunk border

	wait(1);
	TerrHmp_FixNormals_Area(upleft_vert, width_vert, height_vert);
}						


void	 	TerEdit_He_SmoothArea()			// set vertices to average of surrounding vertices, from area border towards middle
{
	if (proc_status(TerEdit_He_SmoothArea) > 0) return;
	
	// manage undo array counters
	teredit_he_undostep++;
	if (teredit_he_undostep>9)
		{
			teredit_he_undostep = 0;
		}
	if (teredit_he_undostep==teredit_he_firstundo)
		{
			teredit_he_firstundo++;
			if (teredit_he_firstundo>9)
				{
					teredit_he_firstundo = 0;
				}
		}
	//---------------------------
	// determine upper left and lower right vertex corners, i.e. get x,y vertex count within cube area
	
sys_marker("S01");	
	
	// x,y vertex size of terrain
	var vert_count_x = ent_status(terrain_entity,2)+1;	
	var vert_count_y = ent_status(terrain_entity,3)+1;
	
	// upleft and downright tile corner positions
	var upleft_pos_x = teredit_placement_cube.x - integer(teredit_tilesize/2)*GetMapTileSize();
	var upleft_pos_y = teredit_placement_cube.y + integer(teredit_tilesize/2)*GetMapTileSize();
	var upleft_pos_z = teredit_placement_cube.z; // PosInfo3D1(upleft_pos_x, upleft_pos_y);
	
	var dnright_pos_x = teredit_placement_cube.x + integer(teredit_tilesize/2)*GetMapTileSize();
	var dnright_pos_y = teredit_placement_cube.y - integer(teredit_tilesize/2)*GetMapTileSize();
	var dnright_pos_z = teredit_placement_cube.z; // PosInfo3D1(dnright_pos_x, dnright_pos_y);
	
	// corner vertices slosest to corner tile middles
	var upleft_vert  = ent_nextvertex(terrain_entity,vector(upleft_pos_x,upleft_pos_y,upleft_pos_z));
	var dnright_vert = ent_nextvertex(terrain_entity,vector(dnright_pos_x,dnright_pos_y,dnright_pos_z));
	
	// protection - no valid vertex found by ent_nextvertex
	if ((upleft_vert==(var)0) || (dnright_vert==(var)0))
		{
			return;
		}
		
	// vertices in a row & column
	var width_vert  = abs( dnright_vert%( ent_status(terrain_entity,2)+1 ) - upleft_vert%( ent_status(terrain_entity,2)+1 ) );
	var height_vert = abs( integer( dnright_vert /( ent_status(terrain_entity,2)+1 )) - integer( upleft_vert /( ent_status(terrain_entity,2)+1 )) );

sys_marker(NULL);
	
	var xpos = 0;
	var ypos = 0;
	var i,j,k;
	while ( (xpos<integer(width_vert/2)) && (ypos<integer(height_vert/2)) )
		{
			for(j=0;j<height_vert;j++)
				{
					// vertex number
					k = upleft_vert +j*vert_count_x +(xpos);
					TerEdit_He_SmoothVertex(k);	
					wait_for(TerEdit_He_SmoothVertex);
					// vertex number
					k = upleft_vert +j*vert_count_x +(width_vert-1-xpos);
					TerEdit_He_SmoothVertex(k);
					wait_for(TerEdit_He_SmoothVertex);	
				}
			for(i=0;i<width_vert;i++)
				{
					// vertex number
					k = upleft_vert +(ypos)*vert_count_x +i;
					TerEdit_He_SmoothVertex(k);	
					wait_for(TerEdit_He_SmoothVertex);
					// vertex number
					k = upleft_vert +(height_vert-1-ypos)*vert_count_x +i;
					TerEdit_He_SmoothVertex(k);	
					wait_for(TerEdit_He_SmoothVertex);
				}
			xpos++;
			ypos++;
		}	
	// midpoint
	TerEdit_He_SmoothVertex(upleft_vert +ypos*vert_count_x +xpos);	
	wait_for(TerEdit_He_SmoothVertex);
	
	TerEdit_He_StoreUndoStep();
	
	wait(1);
	TerrHmp_FixNormals_Area(upleft_vert, width_vert, height_vert);
}						


void	 	TerEdit_He_SmoothVertex(var vertex)
{
//	// vertex number
//	k = upleft_vert +j*vert_count_x +i;
	
	// check validity of mid vertex
	if ( (vertex<1) || (vertex>( ent_status(terrain_entity,0) )) )
		{
			return;
		}
	var temp_vertex;
	var sum = 0;				// add all VALID vertex heights
	var counter = 0;			// divide by it
	//NW
	temp_vertex = vertex - ent_status(terrain_entity,2)+1 -1;
	if ( (temp_vertex>0) && (temp_vertex<(ent_status(terrain_entity,0)+1)) )
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,temp_vertex); 
			sum += c.v.y;
			counter++;
		}
	//N
	temp_vertex = vertex - ent_status(terrain_entity,2)+1;
	if ( (temp_vertex>0) && (temp_vertex<(ent_status(terrain_entity,0)+1)) )
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,temp_vertex); 
			sum += c.v.y;
			counter++;
		}
	//NE
	temp_vertex = vertex - ent_status(terrain_entity,2)+1 +1;
	if ( (temp_vertex>0) && (temp_vertex<(ent_status(terrain_entity,0)+1)) )
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,temp_vertex); 
			sum += c.v.y;
			counter++;
		}
	//W
	temp_vertex = vertex - 1;
	if ( (temp_vertex>0) && (temp_vertex<(ent_status(terrain_entity,0)+1)) )
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,temp_vertex); 
			sum += c.v.y;
			counter++;
		}
	//E
	temp_vertex = vertex + 1;
	if ( (temp_vertex>0) && (temp_vertex<(ent_status(terrain_entity,0)+1)) )
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,temp_vertex); 
			sum += c.v.y;
			counter++;
		}
	//SW
	temp_vertex = vertex + ent_status(terrain_entity,2)+1 -1;
	if ( (temp_vertex>0) && (temp_vertex<(ent_status(terrain_entity,0)+1)) )
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,temp_vertex); 
			sum += c.v.y;
			counter++;
		}
	//S
	temp_vertex = vertex + ent_status(terrain_entity,2)+1;
	if ( (temp_vertex>0) && (temp_vertex<(ent_status(terrain_entity,0)+1)) )
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,temp_vertex); 
			sum += c.v.y;
			counter++;
		}
	//SE
	temp_vertex = vertex + ent_status(terrain_entity,2)+1 +1;
	if ( (temp_vertex>0) && (temp_vertex<(ent_status(terrain_entity,0)+1)) )
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,temp_vertex); 
			sum += c.v.y;
			counter++;
		}
	// setvertex to average off surrounding vertices
	CONTACT* c = ent_getvertex(terrain_entity,NULL,vertex); 
	// harder flattening
//	c.v.y = (float)sum/counter;
	// softer smoothing
	c.v.y = (c.v.y + (float)sum/counter)/2;
	ent_setvertex(terrain_entity,c,vertex);
	
	// actual
//	teredit_vertexarray[vertex-1].actual = (var)c.v.y;	
	// new undo step
//	teredit_vertexarray[vertex-1].undo[teredit_he_undostep] = (var)c.v.y;				// BAD - whole evrtex array must be copied to not to keep non-corresponding data !
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void	 	TerEdit_He_Reset()
{
	wait(3);
	
	// manage undo array counters
	teredit_he_undostep++;
	if (teredit_he_undostep>9)
		{
			teredit_he_undostep = 0;
		}
	if (teredit_he_undostep==teredit_he_firstundo)
		{
			teredit_he_firstundo++;
			if (teredit_he_firstundo>9)
				{
					teredit_he_firstundo = 0;
				}
		}
	//----------------------------
	int j;
	for (j=1; j<=ent_status(terrain_entity,0); j++)
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,j);   			// vertex number begins with 1 !
			c.v.y = (float)teredit_vertexarray[j-1].original; 
			ent_setvertex(terrain_entity,c,j);
			
//			teredit_vertexarray[j-1].actual = (var)c.v.y;	
			// new undo step
			teredit_vertexarray[j-1].undo[teredit_he_undostep] = (var)c.v.y;	
		}
	wait(1);
//	c_updatehull(terrain_entity,0);		// works fine
//	ent_fixnormals(terrain_entity,0);	// not okay for chunked terrain - cause error around 1st chunk border

	TerrHmp_FixNormals();
}


void	 	TerEdit_He_Undo()
{
	wait(3);
	
	// manage undo array counters
	if (teredit_he_undostep==teredit_he_firstundo)			// not possible
		{
			return;
		}
	
	teredit_he_undostep--;										// else, step backwards
	if (teredit_he_undostep<0)
		{
			teredit_he_undostep = 9;
		}
	
	//---------------------------
	int j;
	for (j=1; j<=ent_status(terrain_entity,0); j++)
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,j);   							// read vertex data
			c.v.y = (float)teredit_vertexarray[j-1].undo[teredit_he_undostep]; 		// modify data z read from previous undo step z
			ent_setvertex(terrain_entity,c,j);													// write to vertex data
			
//			teredit_vertexarray[j-1].actual = (var)c.v.y;									// write to actual data (is it used?)
		}
	wait(1);
//	c_updatehull(terrain_entity,0);		// works fine
//	ent_fixnormals(terrain_entity,0);	// not okay for chunked terrain - cause error around 1st chunk border
	
	TerrHmp_FixNormals();
}


void	 	TerEdit_He_Redo()
{
	wait(3);
	
	// manage undo array counters
	teredit_he_undostep++;										// step forward
	if (teredit_he_undostep>9)
		{
			teredit_he_undostep = 0;
		}
	
	if (teredit_he_undostep==teredit_he_firstundo)			// not possible
		{
			teredit_he_undostep--;								// step backwards to original
			if (teredit_he_undostep<0)
				{
					teredit_he_undostep = 9;
				}
			return;
		}
	
	//---------------------------
	int j;
	for (j=1; j<=ent_status(terrain_entity,0); j++)
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,j);   			// vertex number begins with 1 !
			c.v.y = (float)teredit_vertexarray[j-1].undo[teredit_he_undostep]; 
			ent_setvertex(terrain_entity,c,j);
			
//			teredit_vertexarray[j-1].actual = (var)c.v.y;	
		}
	wait(1);
//	c_updatehull(terrain_entity,0);		// works fine
//	ent_fixnormals(terrain_entity,0);	// not okay for chunked terrain - cause error around 1st chunk border
	
	TerrHmp_FixNormals();
}


void		TerEdit_He_StoreUndoStep()
{
	int i;
	for (i=1; i<=ent_status(terrain_entity,0); i++)
		{	
			CONTACT* c = ent_getvertex(terrain_entity, NULL, i);   	
			
			teredit_vertexarray[i-1].undo[teredit_he_undostep] = (var)c.v.y;
		}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Save_Vertices()										// calls filehandling script
{
	wait(3);
	FileMenu_SaveTerrVerts();
}


void		TerEdit_Load_Vertices()
{
	wait(3);
	FileMenu_LoadTerrVerts();
}


/////////////////////////////////////////


void	 	TerEdit_Save_Terrain()											// calls filehandling script
{
	wait(3);
//	if (teredit_raiselowerflatten)		// saving is denied if all vertices have been modified
//		{
//			// message
//			int temp_counter = 0;
//			while(temp_counter<50)
//				{
//					draw_text("Saving as .hmp is disabled by a previous raise/lower/flatten command.",100,384,vector(200,225,225));
//					temp_counter++;
//					wait(1);
//				}			
//			return;
//		}
	FileMenu_SaveHmp();
}


void		TerEdit_Save_Hmp(char* file_name)								// file name with path
{
	if (proc_status(TerEdit_Save_Hmp)>1)
		{
			return;
		}
	// show panel--------------------------------
	set( map_loadpanel , SHOW); 
	set( map_savetext1 , SHOW);
	wait(3);
	//------------------------------------------
	
	STRING* temp_str_1 = str_create("");
	int i;
	
	// DEFORM TERRAIN by Markus Wiedenmeyer for the summer contest 2010 - based script
	long my_size;
//	STRING* temp_str_1 = "#100";
	
//	c_updatehull(terrain_entity,0);											// just for safety - might be needed again
//#ifndef A8
//	wait(1);
//	ent_fixnormals(terrain_entity,0);										// A7 okay with 0
//#else
//	for(i=0;i<ent_status(terrain_entity,16);i++) 
//		{ 
//			ent_fixnormals(terrain_entity,i); 
//		}
//#endif
//	wait(1);
	
	TerrHmp_FixNormals();											// setmesh updates hull too , but not bounding box
	wait_for(TerrHmp_FixNormals);									// slow but better to run it
	wait(1);

#ifndef A8	
//	LPD3DXMESH dx_mesh;
	for (i=0; i<(var)(ent_status(terrain_entity,16)); i++)
		{
			dx_mesh = ent_getmesh(terrain_entity,i,0);		// ent , chunk start with 0 , lod
			D3DXComputeNormals(dx_mesh,NULL);
			ent_setmesh(terrain_entity,dx_mesh,i,0);			// maybe it works with A7 - if not it can be removed
		}
	wait(1);
//	c_setminmax(terrain_entity);									// if no previous fixnormals called
//	wait(1);
#endif

sys_marker("T01");	
	dll_init_terrain();															// init the terrain dll	
sys_marker(NULL);
	
//	// setting entity path and filename - not needed
//sys_marker("T02");
//	dll_path_set_subfolder(_str(""));										// needed?
//sys_marker(NULL);	
//	
//sys_marker("T03");
//	temp_str_1 = dll_path_from_entity(terrain_entity,FALSE);			// needed?
//sys_marker(NULL);
	
	//_--------------------------------
	str_cpy(temp_str_1,file_name);											// overwrite - use MB filename handling
	//---------------------------------
	
sys_marker("T04");
	if (file_exists_onepath (temp_str_1))                          // make a *.bak if the terrain already exists
		{
//			str_cpy (temp_str_1,file_name);
			void* pTerrain = file_load (temp_str_1,NULL,&my_size);   // load the exists terrain into the buffer...
			add_buffer (temp_str_1,pTerrain,my_size);
			str_trunc (temp_str_1,3);                                // ...replace hmp with bak...
			str_cat (temp_str_1,"bak");
			file_save (temp_str_1,pTerrain,my_size);                 // ...and save it
			
			file_load(NULL,pTerrain,NULL);                           // free the buffer
		}
sys_marker(NULL);
	
	//-----------------------------------
	str_cpy(temp_str_1,file_name);											// overwrite - was modified to .bak, so set it again
	//-----------------------------------

//#ifndef A8	
	for (i=1; i<=ent_status(terrain_entity,0); i++)            // A7 convert to world coordinates...
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,i);
			c.v.y += terrain_entity.z;
			ent_setvertex(terrain_entity,c,i);
		}
//#endif

sys_marker("T05");		
	dll_load_terrain_from_entity( terrain_entity );                // save the terrain
sys_marker(NULL);
		
sys_marker("T06");																// A7 - in case of flatten terrain it results in error
	var okay = dll_save_terrain_from_entity( terrain_entity , temp_str_1 );
	if (okay == (var)1)
		{  
//			printf("Saved");
		}
	else
		{
			printf("DLL Error: Terrain is NOT saved");
		}
sys_marker(NULL);
		
//#ifndef A8
	for (i=1; i<=ent_status(terrain_entity,0); i++)            // A7 convert back to local coordinates...
		{
			CONTACT* c = ent_getvertex(terrain_entity,NULL,i);
			c.v.y -= terrain_entity.z;
			ent_setvertex(terrain_entity,c,i);
		}
//#endif
	
sys_marker("T07");
	dll_close_terrain();
sys_marker(NULL);

	TerrHmp_FixNormals();
	wait_for(TerrHmp_FixNormals);
	
	//------------------------------------------------
	reset( map_loadpanel , SHOW); 
	reset( map_savetext1 , SHOW);
	wait(3);
	
	ptr_remove(temp_str_1);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_UpdateEntities()
{
	wait(3);
	
	set( map_loadpanel , SHOW);
	set( map_updatetext1 , SHOW);
	wait(3);
	
	TerrHmp_FixNormals();				// needed to get actual surface
	wait_for(TerrHmp_FixNormals);
	wait(3);
	
	me = ent_next(NULL); // get first entity
 	while (me) // repeat until there are no more entities
		{ 
			if ((me.ENT_CATEGORY < UIMODE_SHADOWSHIFT*2) && (me.ENT_CATEGORY > 0) && (is(my,FLAG2)))		// except terrain models
				{
					Teredit_UpdateEntityZ();
				}				
			
			// maximize function instances
			while (proc_status (Teredit_UpdateEntityZ) > 2000)
				{
					wait(6);
				}
				
	 		me = ent_next(me); // get next entity
		}	
	wait(3);
	reset( map_loadpanel , SHOW);
	reset( map_updatetext1 , SHOW);
}


void		Teredit_UpdateEntityZ()
{
//	var height;
	// reset to dynamic if static
	my.emask |= (DYNAMIC);		
	wait(1);				
	// entities could store in a skinn their origin's distance to grourd 
	// (modify menu - and save it in filehandling) - and it could be added to it
	if (my.ENT_CATEGORY == (var)UIMODE_DETAIL)
		{
			my.z = PosInfo3D1(my.x, my.y) + GetMapTileSize()/2*(my.scale_x*detail_defaultsize/GetMapTileSize()); 
		}
	else if (my.ENT_CATEGORY == (var)UIMODE_SHRUB)
		{
			my.z = PosInfo3D1(my.x, my.y) + GetMapTileSize()/2*(my.scale_x*shrub_defaultsize/GetMapTileSize()); 
		}
	else if (my.ENT_CATEGORY == ((var)UIMODE_SHRUB + (var)UIMODE_SHADOWSHIFT))
		{
			my.z = PosInfo3D1(my.x, my.y) + GetMapTileSize()/2; 
		}
	else
		{
			my.z = PosInfo3D1(my.x,my.y);	
		}				
//	me.z = height;
	wait(1);
	// get proper decal shadow
	if (is(my,SHADOW))
		{
//			my.z += 16;
//			wait(2);
//			my.z -= 16;
//			wait(1);
			
			reset(my,SHADOW);
			wait(1);
			set(my,SHADOW);
			wait(1);
		}
	my.emask &= ~DYNAMIC;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif