/////////////////////////////////////////////////////////////
// edit terrain height
// edit terrain texture: 32b for multi, 24 for single skin
// edit terrain lightmap
/////////////////////////////////////////////////////////////
//d3d vertex struct c.v.*:
//typedef struct {
//   float x,y,z;    // position   
//   float nx,ny,nz; // normal
//   float u1,v1;    // coordinate set 1, for base texture
//   float u2,v2;    // coordinate set 2, for light map or detail map
//   float x3,y3,z3,w3; // coordinate set 3, for tangent or other purposes (A7.06 and above only)
//} D3DVERTEX;
//
//#define D3DFVF_D3DVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX3|D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE2(1)|D3DFVF_TEXCOORDSIZE4(2))
/////////////////////////////////////////////////////////////

#ifndef MBteredit_c
#define MBteredit_c

////////////////////////////////////////////////////////
// code

void 	TerEdit_Init()
{
	if (!terrain_entity) return;
		
//	printf( "Vertices : %i" , (int)ent_status(terrain_entity,0) );	// test
	
//	printf( "VerticesX : %i" , (int)ent_status(terrain_entity,2) );
//	printf( "VerticesY : %i" , (int)ent_status(terrain_entity,3) );	// test
	
//	// protection: WMB level, or non-square terrain	
//	if (
//			(ent_status(terrain_entity,2)!=(var)ent_status(terrain_entity,3))
//		)
//		{
//			teredit_br_panel = pan_create( panel_image2 , 11 );
//			set( teredit_br_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
//			teredit_br_panel.pos_x 	= screen_size.x-256;
//			teredit_br_panel.pos_y 	= 256;
//			teredit_br_panel.red		= panel_font_r;
//			teredit_br_panel.green	= panel_font_g;
//			teredit_br_panel.blue	= panel_font_b;
//			
//			// string 1
//			pan_setstring(teredit_br_panel, 0,	15,10,	panel_font2,			str_create("TERRAIN MODIFICATION") );
//			
//			// string 2
////			if (map_wmb)
////				{
////					pan_setstring(teredit_br_panel, 0,	15,100,	panel_font2,			str_create("Terrain editing in [.wmb] level \nis not allowed.") );
////				}
////			else
////				{
//					pan_setstring(teredit_br_panel, 0,	15,100,	panel_font2,			str_create("Editing non-square terrains \nis not supported.") );
////				}
//				
//			// create modification area cube - needed for loop and proper closing
//			collision_mode = 0; 
//			teredit_placement_cube = ent_create( teredit_creator_model , vector(square_coord_x,square_coord_y,square_coord_z) , TerEdit_Cube );
//			ent_clone(teredit_placement_cube);
//			collision_mode = 1; 
//			
////			set(teredit_placement_cube,INVISIBLE); // hide because not needed - but lines are still visible
//			
//			return;
//		}
	
	int i;
	// reset to dynamic if static
	terrain_entity.emask |= (DYNAMIC);
	wait(1);
	//----------------------------------------------------------------
	// store original terrain vertices to orig and act arrays - simple direct access
	TerEdit_He_InitTerrain();
	//----------------------------------------------------------------
	// brush image filename initialization 
sys_marker("tbr");	
	int arraysize = (int)TEREDIT_MAXPERSET;
	teredit_br_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );		
	// init filename string array elements
	for(i=0;i<max_teredit_br_items;i++)
		{
			teredit_br_filenames[i] = str_create("");
		}	
sys_marker(NULL);		
	//-----------------------------------------------------------------
	// determine how many .tga files are in the folder, 
	// divide it by 5, and the result is the number of sets
	teredit_br_set = 1;
	max_teredit_br_sets = TEREDIT_MAXSET;
	teredit_brushes_txt = txt_create( (max_teredit_br_sets*max_teredit_br_items) , 0 );		// 10*8=80
	for(i=0;i<max_teredit_br_sets*max_teredit_br_items;i++)
		{
			str_cpy( (teredit_brushes_txt->pstring)[i] , "" );
		}
	i = 0;
	STRING* temp_str = str_create("");
	str_cpy( temp_str , teredit_brush_foldername );
	str_cat( temp_str , "*.tga" );
	txt_for_dir( teredit_brushes_txt, temp_str ); 									
	while (str_len((teredit_brushes_txt->pstring)[i]) > 0)
		{
			i++;
		}
	max_teredit_br_sets = integer(i/max_teredit_br_items);
	if ( fraction(i/max_teredit_br_items) > 0)
		{
			max_teredit_br_sets++;		// allow to display not complete sets too
		}
	//------------------------------------------------------------------
	// copy first 5 filenames to string array
	for(i=0;i<max_teredit_br_items;i++)
		{
//			str_cpy( teredit_br_filenames[i] , "" );													// subfolder			
			str_cpy( teredit_br_filenames[i] , teredit_brush_foldername );								// subfolder
			str_cat( teredit_br_filenames[i] , (teredit_brushes_txt->pstring)[i] );			// filename with extension	
			
			if ( str_cmpi( teredit_br_filenames[i] , teredit_brush_foldername ) )					// if nothing was added to folder name, clear it to make an empty slot
				{
					str_cat( teredit_br_filenames[i] , "_default.tga" );
				}
		}	
//	bmap_load(teredit_brush1_bmp,teredit_br_filenames[0],1);
//	bmap_load(teredit_brush2_bmp,teredit_br_filenames[1],1);
//	bmap_load(teredit_brush3_bmp,teredit_br_filenames[2],1);
//	bmap_load(teredit_brush4_bmp,teredit_br_filenames[3],1);
//	bmap_load(teredit_brush5_bmp,teredit_br_filenames[4],1);
	teredit_brush1_bmp = bmap_create(teredit_br_filenames[0]);		
	teredit_brush2_bmp = bmap_create(teredit_br_filenames[1]);
	teredit_brush3_bmp = bmap_create(teredit_br_filenames[2]);
	teredit_brush4_bmp = bmap_create(teredit_br_filenames[3]);
	teredit_brush5_bmp = bmap_create(teredit_br_filenames[4]);
		
	//------------------------------------------------------------------
	// initial value settings	
	for(i=0;i<max_teredit_br_items;i++)
		{
			teredit_br_switch_type[i] = 0;
		}
	teredit_br_switch_type[teredit_br_selected] = 1;
//	teredit_tilesize = 1;	
	//----------------------------------------------------------------
	
	// init base panel
	TerEdit_Panel_Br_Init();
	wait_for(TerEdit_Panel_Br_Init);
	
	wait(3);
	// create modification area cube
	collision_mode = 0; 
	teredit_placement_cube = ent_create( teredit_creator_model , vector(square_coord_x,square_coord_y,square_coord_z) , TerEdit_Cube );
	collision_mode = 1; 
//	wait(10);	// needed
//	TerEdit_MorphDecal();
	
	ptr_remove(temp_str);
}


void		TerEdit_Close()
{
	//-------------------------------------------------------
	set( map_loadpanel , SHOW);
	set( map_updatetext1 , SHOW);
	wait(3);
	
	int i;
	
	//-------------------------------------------------------
{}
#ifndef A8
	TerrHmp_FixNormals();											// setmesh updates hull too , but not bounding box
	wait_for(TerrHmp_FixNormals);
	wait(1);
	
	// A7 only update needed by chunk border vertices - in A8 they are updated properly
//	LPD3DXMESH dx_mesh;
	for (i=0; i<(var)(ent_status(terrain_entity,16)); i++)
		{
			dx_mesh = ent_getmesh(terrain_entity,i,0);		// ent , chunk start with 0 , lod
			D3DXComputeNormals(dx_mesh,NULL);
			ent_setmesh(terrain_entity,dx_mesh,i,0);			// maybe it works with A7 - if not it can be removed
		}
	wait(1);
	
//	// should be free on level/game exit - but not this time, because it removes the first chunk (assigned by 0)
////	dx_mesh = ent_getmesh(terrain_entity,0,0);
//	dx_mesh->Release();
////	ent_setmesh(terrain_entity,NULL,0,0);
//	wait(1);
#endif
	//-------------------------------------------------------
	
	TerrHmp_FixNormals();											// needed again to hide errors on terrain chunks
	wait_for(TerrHmp_FixNormals);
	wait(1);
	
	//-------------------------------------------------------
	if (teredit_he_panel!=NULL)
		{
			TerEdit_He_Close();
		}
	if (teredit_tx_panel!=NULL)
		{
			TerEdit_Tx_Close();
		}
	if (teredit_lm_panel!=NULL)
		{
			TerEdit_Lm_Close();
		}
	
	//-------------------------------------------------------
	if (teredit_br_panel!=NULL)
		{
			ptr_remove(teredit_br_panel);	
			teredit_br_panel = NULL;
		}
	if (teredit_placement_cube!=NULL)
		{
			ptr_remove(teredit_placement_cube);
			teredit_placement_cube = NULL;
		}
	
	//-------------------------------------------------------
	if (teredit_brush1_panel!=NULL)
		{	
			ptr_remove(teredit_brush1_panel);		
			teredit_brush1_panel = NULL;
		}
	if (teredit_brush2_panel!=NULL)
		{	
			ptr_remove(teredit_brush2_panel);		
			teredit_brush2_panel = NULL;
		}
	if (teredit_brush3_panel!=NULL)
		{	
			ptr_remove(teredit_brush3_panel);		
			teredit_brush3_panel = NULL;
		}
	if (teredit_brush4_panel!=NULL)
		{	
			ptr_remove(teredit_brush4_panel);		
			teredit_brush4_panel = NULL;
		}
	if (teredit_brush5_panel!=NULL)
		{	
			ptr_remove(teredit_brush5_panel);		
			teredit_brush5_panel = NULL;
		}
		
	//-------------------------------------------------------
	if (teredit_brush1_bmp!=NULL)
		{
			bmap_purge(teredit_brush1_bmp);
			ptr_remove(teredit_brush1_bmp);		
			teredit_brush1_bmp = NULL;
		}
	if (teredit_brush2_bmp!=NULL)
		{
			bmap_purge(teredit_brush2_bmp);
			ptr_remove(teredit_brush2_bmp);		
			teredit_brush2_bmp = NULL;
		}
	if (teredit_brush3_bmp!=NULL)
		{
			bmap_purge(teredit_brush3_bmp);
			ptr_remove(teredit_brush3_bmp);		
			teredit_brush3_bmp = NULL;
		}
	if (teredit_brush4_bmp!=NULL)
		{
			bmap_purge(teredit_brush4_bmp);
			ptr_remove(teredit_brush4_bmp);		
			teredit_brush4_bmp = NULL;
		}
	if (teredit_brush5_bmp!=NULL)
		{
			bmap_purge(teredit_brush5_bmp);
			ptr_remove(teredit_brush5_bmp);		
			teredit_brush5_bmp = NULL;
		}	
	
	//--------------------------------------------------------
	// remove temp text
	if (teredit_brushes_txt!=NULL)
		{
			for (i = 0; i < teredit_brushes_txt->strings; i++) 
				{
					if ((teredit_brushes_txt->pstring)[i]) 
						{
							ptr_remove((teredit_brushes_txt->pstring)[i]);
						}
				}
			ptr_remove(teredit_brushes_txt);
			teredit_brushes_txt = NULL;
		}
	
	//---------------------------------------------------------
	// remove temp string array
	if (teredit_br_filenames!=NULL)
		{
			for(i=0;i<max_teredit_br_items;i++)
				{
					ptr_remove(teredit_br_filenames[i]);
				}
			sys_free(teredit_br_filenames);
			teredit_br_filenames = NULL;
		}
	
	//---------------------------------------------------------
	// vertex z arrays
	if (teredit_vertexarray != NULL)
		{
//			sys_free(teredit_vertexarray.undo);		// okay in XP, crash in Win7 !!!	- a fixed length array within the struct, thus no freeing needed/allowed
			sys_free(teredit_vertexarray);
			teredit_vertexarray = NULL;
		}
	
	//---------------------------------------------------------
	reset( map_loadpanel , SHOW);
	reset( map_updatetext1 , SHOW);
	wait(1);
	//---------------------------------------------------------
	// set back to static
	terrain_entity.emask &= ~DYNAMIC;
	//-------------------------------------------------------
}


/////////////////////////////////////////////////


action 	TerEdit_Cube()
{
	set(my, PASSABLE | TRANSLUCENT | CAST);
	my.flags2 |= UNTOUCHABLE;
	my.alpha = 50;
	vec_scale( my.scale_x , GetMapTileSize() / MAP_DEFAULTGRIDSIZE * teredit_tilesize );
	//	my.skill1 = 1;	// actual scale size
	my.material = mtl_placementarea;
	
	VECTOR vTemp;
	COLOR cTemp;
	vec_set(cTemp,vector(2,114,162));

//	set(my, PASSABLE | TRANSLUCENT | LIGHT | INVISIBLE);	// hidden object
//	my.red = 0;
//	my.green = 255;
//	my.blue = 0;
//	my.alpha = 50;
//	
//	teredit_area_bmp = bmap_create("brush\\_default.tga");
//	wait(1);
//	bmap_to_format(teredit_area_bmp,888);	// for safety
//	wait_for(bmap_to_format);
//	wait(1);
//	bmap_to_alpha(teredit_area_bmp,10);
//	wait_for(bmap_to_alpha);
//	wait(1);
					
	while( (UIMode==(var)UIMODE_TEREDIT) || (UIMode==(var)UIMODE_FILEMENU2))
		{
			vec_set( my.x , vector(square_coord_x,square_coord_y,square_coord_z) );			// follows active tile square - could make c_trace too	
						
			vec_for_vertex(vTemp, me, 1);			
			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
			vec_for_vertex(vTemp, me, 2);
			draw_line3d(vTemp,cTemp,50);
			vec_for_vertex(vTemp, me, 3);
			draw_line3d(vTemp,cTemp,50);
			vec_for_vertex(vTemp, me, 4);
			draw_line3d(vTemp,cTemp,50);
			vec_for_vertex(vTemp, me, 1);
			draw_line3d(vTemp,cTemp,50);
			
			vec_for_vertex(vTemp, me, 1);			
			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
			vec_for_vertex(vTemp, me, 5);
			draw_line3d(vTemp,cTemp,50);
			
			vec_for_vertex(vTemp, me, 2);			
			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
			vec_for_vertex(vTemp, me, 6);
			draw_line3d(vTemp,cTemp,50);
			
			vec_for_vertex(vTemp, me, 3);			
			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
			vec_for_vertex(vTemp, me, 7);
			draw_line3d(vTemp,cTemp,50);
			
			vec_for_vertex(vTemp, me, 4);			
			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
			vec_for_vertex(vTemp, me, 8);
			draw_line3d(vTemp,cTemp,50);
			
//			if (square_coord_z<0)
//				vec_set(my.blue,vector(255,0,0));		// blue if under water level - inside game area
//			else
//				vec_set(my.blue,vector(0,255,0));		// green if over water level - inside game area
//			
//			if (square_outofborder)
//				vec_set(my.blue,vector(0,0,255));		// red if out of game area
//			
//			// draw a decal onto terrain surface representing draw area
//			c_trace(vector(my.x,my.y,my.z+GetMapTileSize()*10),vector(my.x,my.y,my.z-GetMapTileSize()*40), IGNORE_ME | IGNORE_CONTENT | IGNORE_PASSABLE | IGNORE_FLAG2 | IGNORE_SPRITES | SCAN_TEXTURE );	// | SCAN_TEXTURE needed !!!
//			if ((HIT_TARGET) && (teredit_area_bmp))
//				{					
//					if (bmap_format(teredit_area_bmp)==8888)	// to avoid blinks on brush change
//						{																				
//							PARTICLE* area_p = ent_decal(you, teredit_area_bmp, (GetMapTileSize()*teredit_tilesize), my.pan-your.pan);		
//							area_p.material = mtl_placementarea; 		// assign another material when contact material is not suitable for decals like terraintex.fx
//							set(area_p,TRANSLUCENT | BRIGHT | UNLIT );
//							area_p.alpha = 85;		
//							vec_set(mtl_placementarea.ambient_blue, my.blue);		// same color
//							vec_fill(mtl_placementarea.diffuse_blue, 255);			// bright color
//							vec_set(mtl_placementarea.emissive_blue, my.blue);		// bright color
//							area_p.lifespan = 0.01;											// one frame		
//							
//							// not okay on high slope !!!
//							// or put there a ZNEAR sprite - but it would be flat	
//						}								
//				}
			
//			// move sub-panels accordingly - MOVED TO EVENT to not to lag
//			if (teredit_he_panel)
//				{
//					teredit_he_panel.pos_x = ui_submenupanel_x;
//					teredit_he_panel.pos_y = ui_submenupanel_y + 128;
//				}
//			
//			if (teredit_lm_panel)
//				{
//					teredit_lm_panel.pos_x = ui_submenupanel_x;
//					teredit_lm_panel.pos_y = ui_submenupanel_y + 128;
//				}
//			
//			if (teredit_tx_panel)
//				{
//					teredit_tx_panel.pos_x = ui_submenupanel_x;
//					teredit_tx_panel.pos_y = ui_submenupanel_y + 128;
//				}			
			
			wait(1);
		}
//	vec_fill(mtl_placementarea.ambient_blue, 200);		// reset material ambient color
//	vec_fill(mtl_placementarea.diffuse_blue, 0);			// reset material diffuse color
//	vec_fill(mtl_placementarea.emissive_blue, 0);		// reset material emissive color
	
	TerEdit_Close();
}

//void TerEdit_MorphDecal()
//{
//	ptr_remove(teredit_area_bmp);
//	teredit_area_bmp = NULL;
////	wait(1);
//	teredit_area_bmp = bmap_create( teredit_br_filenames[teredit_br_selected] );
////	wait(1);
//	bmap_to_format(teredit_area_bmp,888);	// for safety
//	wait_for(bmap_to_format);
////	wait(1);
//	bmap_to_alpha(teredit_area_bmp,10);
//}

/////////////////////////////////////////////////

void		TerEdit_Panel_Br_Init()
{	
	int i;
	STRING* temp_string = str_create("");
	
	teredit_br_panel = pan_create( panel_image2 , 11 );
	set( teredit_br_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	teredit_br_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	teredit_br_panel.pos_y 	= ui_submenupanel_y;								// 256;
	teredit_br_panel.red		= panel_font_r;
	teredit_br_panel.green	= panel_font_g;
	teredit_br_panel.blue	= panel_font_b;
	teredit_br_panel.event	= ui_submenu_panelmove;
	
	// string 1
	pan_setstring(teredit_br_panel, 0,	15,10,	panel_font2,			str_create("TERRAIN MODIFICATION") );
	
	// brush panel 1
	teredit_brush1_panel = pan_create( teredit_brush1_str , 12 );
	set( teredit_brush1_panel , SHOW );					
	teredit_brush1_panel.pos_x = ui_submenupanel_x + 16;						// screen_size.x-256+16;
	teredit_brush1_panel.pos_y = ui_submenupanel_y + 45;						// 256+45;
//	bmap_load( teredit_brush1_bmp , teredit_br_filenames[0] , 1 );			// done on init
//	teredit_brush1_panel.bmap = teredit_brush1_bmp;								// done on init
	teredit_brush1_panel.scale_x = teredit_br_thumb_size / bmap_width(teredit_brush1_bmp); 
	teredit_brush1_panel.scale_y = teredit_br_thumb_size / bmap_height(teredit_brush1_bmp); 
	teredit_brush1_panel.event = TerEdit_Brush_Toggle;
	
	// brush panel 2
	teredit_brush2_panel = pan_create( teredit_brush2_str , 12 );
	set( teredit_brush2_panel , SHOW );
	teredit_brush2_panel.pos_x = ui_submenupanel_x + 56;						// screen_size.x-256+16+40;
	teredit_brush2_panel.pos_y = ui_submenupanel_y + 45;						// 256+45;
//	bmap_load( teredit_brush2_bmp , teredit_br_filenames[1] , 1 );
//	teredit_brush2_panel.bmap = teredit_brush2_bmp;
	teredit_brush2_panel.scale_x = teredit_br_thumb_size / bmap_width(teredit_brush2_bmp); 
	teredit_brush2_panel.scale_y = teredit_br_thumb_size / bmap_height(teredit_brush2_bmp); 
	teredit_brush2_panel.event = TerEdit_Brush_Toggle;
	
	// brush panel 3
	teredit_brush3_panel = pan_create( teredit_brush3_str , 12 );
	set( teredit_brush3_panel , SHOW );					
	teredit_brush3_panel.pos_x = ui_submenupanel_x + 96;						// screen_size.x-256+16+80;
	teredit_brush3_panel.pos_y = ui_submenupanel_y + 45;						// 256+45;
//	bmap_load( teredit_brush3_bmp , teredit_br_filenames[2] , 1 );
//	teredit_brush3_panel.bmap = teredit_brush3_bmp;
	teredit_brush3_panel.scale_x = teredit_br_thumb_size / bmap_width(teredit_brush3_bmp); 
	teredit_brush3_panel.scale_y = teredit_br_thumb_size / bmap_height(teredit_brush3_bmp); 
	teredit_brush3_panel.event = TerEdit_Brush_Toggle;
	
	// brush panel 4
	teredit_brush4_panel = pan_create( teredit_brush4_str , 12 );
	set( teredit_brush4_panel , SHOW );					
	teredit_brush4_panel.pos_x = ui_submenupanel_x + 136;						// screen_size.x-256+16+120;
	teredit_brush4_panel.pos_y = ui_submenupanel_y + 45;						// 256+45;
//	bmap_load( teredit_brush4_bmp , teredit_br_filenames[3] , 1 );
//	teredit_brush4_panel.bmap = teredit_brush4_bmp;
	teredit_brush4_panel.scale_x = teredit_br_thumb_size / bmap_width(teredit_brush4_bmp); 
	teredit_brush4_panel.scale_y = teredit_br_thumb_size / bmap_height(teredit_brush4_bmp); 
	teredit_brush4_panel.event = TerEdit_Brush_Toggle;
	
	// brush panel 5
	teredit_brush5_panel = pan_create( teredit_brush5_str , 12 );
	set( teredit_brush5_panel , SHOW );					
	teredit_brush5_panel.pos_x = ui_submenupanel_x + 176;						// screen_size.x-256+16+160;
	teredit_brush5_panel.pos_y = ui_submenupanel_y + 45;						// 256+45;
//	bmap_load( teredit_brush5_bmp , teredit_br_filenames[4] , 1 );
//	teredit_brush5_panel.bmap = teredit_brush5_bmp;
	teredit_brush5_panel.scale_x = teredit_br_thumb_size / bmap_width(teredit_brush5_bmp); 
	teredit_brush5_panel.scale_y = teredit_br_thumb_size / bmap_height(teredit_brush5_bmp); 
	teredit_brush5_panel.event = TerEdit_Brush_Toggle;
	
	// button 1 2 - change set
	pan_setbutton(teredit_br_panel, 0,	1, 	220,40,		s_arrow_up_on_bmp, s_arrow_up_off_bmp, s_arrow_up_on_bmp, s_arrow_up_off_bmp,						TerEdit_DecSet, 		NULL, NULL); 
	pan_setbutton(teredit_br_panel, 0,	1, 	220,62,		s_arrow_down_on_bmp, s_arrow_down_off_bmp, s_arrow_down_on_bmp, s_arrow_down_off_bmp,			TerEdit_IncSet, 		NULL, NULL); 
	
	// button 3 - select brush	
	pan_setbutton(teredit_br_panel, 0,	1, 	16-2,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 
	
	// string 2
	pan_setstring(teredit_br_panel, 0,	15,85,	panel_font,			str_create("select brush") );
	
	// button 4 5 - inc/dec modification area size
	pan_setdigits(teredit_br_panel, 0,	110+12,85,	"[Z/X] Area: %0.f",		panel_font,	1, teredit_tilesize);	
	pan_setbutton(teredit_br_panel, 0,	1, 	95+12,86,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_DecSize, 		NULL, NULL); 
	pan_setbutton(teredit_br_panel, 0,	1, 	182+12,86,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_IncSize, 		NULL, NULL); 
		
	// button 6 7 8 - radio buttons - select mode: height adjustment , texture drawing, (create ?)
	pan_setstring(teredit_br_panel, 0,	35,105,	panel_font,			str_create("height") );
	pan_setbutton(teredit_br_panel, 0,	4, 	15,105,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		TerEdit_Mode_Toggle,		NULL,NULL); 
	if (teredit_mode==(var)1) button_state( teredit_br_panel , 6 , ON ); 		// 1 ON, 0 OFF	
	
	pan_setstring(teredit_br_panel, 0,	115,105,	panel_font,			str_create("texture") );
	pan_setbutton(teredit_br_panel, 0,	4, 	95,105,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		TerEdit_Mode_Toggle,		NULL,NULL); 
	if (teredit_mode==(var)2) button_state( teredit_br_panel , 7 , ON ); 		// 1 ON, 0 OFF	
	
	pan_setstring(teredit_br_panel, 0,	195,105,	panel_font,			str_create("lightmap") );
	pan_setbutton(teredit_br_panel, 0,	4, 	175,105,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		TerEdit_Mode_Toggle,		NULL,NULL); 
	if (teredit_mode==(var)3) button_state( teredit_br_panel , 8 , ON ); 		// 1 ON, 0 OFF	
		
	// subpanel calling
	if (teredit_mode==(var)1)
		{
			// height modification
			TerEdit_He_Init();		
		}
	else if (teredit_mode==(var)2)
		{
			// texture editing
			TerEdit_Tx_Init();
		}
	else if (teredit_mode==(var)3)
		{
			// future option, needs more research to make really useful (voronoi, fractals etc.)
			TerEdit_Lm_Init();
		}
		
	ptr_remove(temp_string);
}

/////////////////////////////////////////////////////////////

void		TerEdit_IncSet()
{ 
	// jump to next existing set
	teredit_br_set++;
	if (teredit_br_set>max_teredit_br_sets)
		{
			teredit_br_set = max_teredit_br_sets;
			return;
		}
	TerEdit_UpdateSetNames();
}

void		TerEdit_DecSet()
{ 
	// jump to next existing set
	teredit_br_set--;
	if (teredit_br_set<1)
		{
			teredit_br_set = 1;
			return;
		}
	TerEdit_UpdateSetNames();
}

void		TerEdit_UpdateSetNames()
{
	int i;
	// copy the proper 8 filenames to string array
	int j = teredit_br_set - 1;
	int k = j * max_teredit_br_items;
	// copy first 5 filenames to string array
	for(i=0;i<max_teredit_br_items;i++)
		{
//			str_cpy( teredit_br_filenames[i] , "" );													// subfolder			
			str_cpy( teredit_br_filenames[i] , teredit_brush_foldername );						// subfolder
			str_cat( teredit_br_filenames[i] , (teredit_brushes_txt->pstring)[k+i] );			// filename with extension	
			
			if ( str_cmpi( teredit_br_filenames[i] , teredit_brush_foldername ) )			// if nothing was added to folder name, clear it to make an empty slot
				{
					str_cat( teredit_br_filenames[i] , "_default.tga" );
				}
		}	
	bmap_load(teredit_brush1_bmp,teredit_br_filenames[0],1);
	bmap_load(teredit_brush2_bmp,teredit_br_filenames[1],1);
	bmap_load(teredit_brush3_bmp,teredit_br_filenames[2],1);
	bmap_load(teredit_brush4_bmp,teredit_br_filenames[3],1);
	bmap_load(teredit_brush5_bmp,teredit_br_filenames[4],1);
	
	teredit_brush1_panel.bmap 		= teredit_brush1_bmp;							
	teredit_brush1_panel.scale_x 	= teredit_br_thumb_size / bmap_width(teredit_brush1_bmp); 
	teredit_brush1_panel.scale_y 	= teredit_br_thumb_size / bmap_height(teredit_brush1_bmp); 
	
	teredit_brush2_panel.bmap 		= teredit_brush2_bmp;							
	teredit_brush2_panel.scale_x 	= teredit_br_thumb_size / bmap_width(teredit_brush2_bmp); 
	teredit_brush2_panel.scale_y 	= teredit_br_thumb_size / bmap_height(teredit_brush2_bmp); 
	
	teredit_brush3_panel.bmap 		= teredit_brush3_bmp;							
	teredit_brush3_panel.scale_x 	= teredit_br_thumb_size / bmap_width(teredit_brush3_bmp); 
	teredit_brush3_panel.scale_y 	= teredit_br_thumb_size / bmap_height(teredit_brush3_bmp); 
	
	teredit_brush4_panel.bmap 		= teredit_brush4_bmp;							
	teredit_brush4_panel.scale_x 	= teredit_br_thumb_size / bmap_width(teredit_brush4_bmp); 
	teredit_brush4_panel.scale_y 	= teredit_br_thumb_size / bmap_height(teredit_brush4_bmp); 
	
	teredit_brush5_panel.bmap 		= teredit_brush5_bmp;							
	teredit_brush5_panel.scale_x 	= teredit_br_thumb_size / bmap_width(teredit_brush5_bmp); 
	teredit_brush5_panel.scale_y 	= teredit_br_thumb_size / bmap_height(teredit_brush5_bmp); 
	
//	TerEdit_MorphDecal();
}

//////////////////////////////////////////////////////////////

// change by panel click 
void		TerEdit_Brush_Toggle(PANEL* panel)
{	
	int i;
	for(i=0;i<max_teredit_br_items;i++)
		{
			teredit_br_switch_type[i] = 0;							// 0..4
		}
		
	if (panel == teredit_brush1_panel)
		{
			teredit_br_switch_type[0] = 1;
			pan_setbutton(teredit_br_panel, 3,	1, 	16-2 +0,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 			
			teredit_br_selected = 0;
		}
	else if (panel == teredit_brush2_panel)
		{
			teredit_br_switch_type[1] = 1;
			pan_setbutton(teredit_br_panel, 3,	1, 	16-2 +40,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 			
			teredit_br_selected = 1;
		}
	else if (panel == teredit_brush3_panel)
		{
			teredit_br_switch_type[2] = 1;
			pan_setbutton(teredit_br_panel, 3,	1, 	16-2 +80,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 			
			teredit_br_selected = 2;
		}
	else if (panel == teredit_brush4_panel)
		{
			teredit_br_switch_type[3] = 1;
			pan_setbutton(teredit_br_panel, 3,	1, 	16-2 +120,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 			
			teredit_br_selected = 3;
		}
	else if (panel == teredit_brush5_panel)
		{
			teredit_br_switch_type[4] = 1;
			pan_setbutton(teredit_br_panel, 3,	1, 	16-2 +160,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 			
			teredit_br_selected = 4;
		}
	
//	TerEdit_MorphDecal();
}

////////////////////////////////////

void 		TerEdit_DecSize()
{
	teredit_tilesize -= 2;
	
	// minimum area in tiles = terrain width/ terrain meshes /tilesize
	var minvalue = integer( abs(terrain_entity.max_x-terrain_entity.min_x) / ent_status(terrain_entity,2) / GetMapTileSize() );			
	if (minvalue%2==(var)0)
		{
			minvalue++;		// to be odd number
		}
	if (teredit_tilesize<minvalue)				// error at raise-lower area, sys_marker("R01");, if area is smaller than terrain vertex grid size, i.e. when no vertex is in area
		{
			teredit_tilesize = minvalue;
		}	
	var temp = teredit_tilesize / teredit_placement_cube.scale_x ;	
	vec_scale( teredit_placement_cube.scale_x , temp );
}

void 		TerEdit_IncSize()
{
	teredit_tilesize += 2;
	if (teredit_tilesize>max_teredit_size)
		{
			teredit_tilesize = max_teredit_size;
		}
	var temp = teredit_tilesize / teredit_placement_cube.scale_x ;	
	vec_scale( teredit_placement_cube.scale_x , temp );
}

////////////////////////////////////

void		TerEdit_Mode_Toggle(button_number)
{
	// height
	if ((button_number==6) && (teredit_mode!=1))
		{
			TerEdit_Tx_Close();
			TerEdit_Lm_Close();
			teredit_mode = 1;
			TerEdit_He_Init();
		}
	// texture
	else if ((button_number==7) && (teredit_mode!=2))
		{
			TerEdit_He_Close();
			TerEdit_Lm_Close();
			teredit_mode = 2;
			TerEdit_Tx_Init();
		}
	// lightmap
	else if ((button_number==8) && (teredit_mode!=3))
		{
			TerEdit_He_Close();
			TerEdit_Tx_Close();
			teredit_mode = 3;
			TerEdit_Lm_Init();
		}
}

///////////////////////////////////////////////////////////////////////////////////////

void		TerEdit_OnClick()
{
	if (teredit_he_panel)
		{
			if (proc_status(TerEdit_He_RaiseLowerArea) > 0) return;
			if (proc_status(TerEdit_He_SmoothArea) > 0) return;
			
			if (mouse_left)
				{
					while (mouse_left) {wait (1);}
					if (teredit_he_mode==(var)1)
						{
							TerEdit_He_RaiseLowerArea(1);		// ok
							wait_for(TerEdit_He_RaiseLowerArea);
						}
					else
						{
//							TerEdit_He_SetArea();				// ok - not brush based
							TerEdit_He_RaiseLowerArea(2);
							wait_for(TerEdit_He_RaiseLowerArea);
						}
				}	
			if (mouse_right)
				{
					while (mouse_right) {wait (1);}
					if (teredit_he_mode==(var)1)
						{
							TerEdit_He_RaiseLowerArea(0);		// ok
							wait_for(TerEdit_He_RaiseLowerArea);
						}
					else
						{
							TerEdit_He_SmoothArea();			// ok
							wait_for(TerEdit_He_SmoothArea);
						}
				}
		}
	else if (teredit_tx_panel)
		{
			if (proc_status(TerEdit_Tx_Draw32Area) > 0) return;
			if (proc_status(TerEdit_Tx_Draw24Area) > 0) return;
			
			if (mouse_left)
				{
					while (mouse_left) {wait (1);}
					
					if (teredit_tx_count>2)
						{
							TerEdit_Tx_Draw32Area(1);		
							wait_for(TerEdit_Tx_Draw32Area);										
						}
					else
						{
							TerEdit_Tx_Draw24Area(1);		
							wait_for(TerEdit_Tx_Draw24Area);
						}
				}	
			if (mouse_right)
				{
					while (mouse_right) {wait (1);}
					
					if (teredit_tx_count>2)
						{
							TerEdit_Tx_Draw32Area(0);	
							wait_for(TerEdit_Tx_Draw32Area);
						}
					else
						{
							TerEdit_Tx_Draw24Area(0);
							wait_for(TerEdit_Tx_Draw24Area);
						}
				}
		}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif