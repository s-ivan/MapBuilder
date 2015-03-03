
#ifndef MBteredit_tx_c
#define MBteredit_tx_c

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


void		TerEdit_Tx_Init()
{
	int i;
	STRING* temp_str = str_create("");
	STRING* temp2_str = str_create("");
	// create panel-------------------------------------------------------
	teredit_tx_panel = pan_create( NULL , 12 );
	set( teredit_tx_panel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	teredit_tx_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	teredit_tx_panel.pos_y 	= ui_submenupanel_y + 128;						// 256+128;
	teredit_tx_panel.red		= panel_font_r;
	teredit_tx_panel.green	= panel_font_g;
	teredit_tx_panel.blue	= panel_font_b;
	teredit_tx_panel.size_x	= 256;
	teredit_tx_panel.size_y	= 512-128;	
	//------------------------------------------------------------------
	// title
	
//	// string 1
//	pan_setstring(teredit_tx_panel, 0,	15,15,	panel_font2,			str_create("EDIT TEXTURES") );
	
	//-------------------------------------------------------------------
	// initial value settings - common for single and multi skin		
	teredit_tx_selected = 0;
	
	// reset selection switches (selection can remain the same)
	for(i=0;i<max_teredit_tx_items;i++)
		{
			teredit_tx_switch_type[i] = 0;
		}
	teredit_tx_switch_type[teredit_tx_selected] = 1;
//	teredit_tilesize = 15;			
	teredit_tx_set = 1;												// show first set of skins/textures
	
	//---------------------------------------------------------------------------------------
	// number of skins
	teredit_tx_count = ent_status(terrain_entity,8);		// ent_skins(terran_entity);	// does not change		
	
	//---------------------------------------------------------------------------------------
	// convert DDS skin textures to uncompressed
	for(i=0; i<ent_status(terrain_entity, 8); i++)	
		{
			// convert skin if necessary
			if (bmap_format(ent_getskin(terrain_entity, i+1)) < (var)565)			// test: does it contain actual format???
//			if (bmap_format(bmap_for_entity(terrain_entity, i+1)) < (var)565)		// probably it returns original format!!!
				{			
					if ((ent_status(terrain_entity,8) < (var)3) && (i==0))
						{
//							ent_setskin( terrain_entity , bmap_to_mipmap(bmap_to_format(ent_getskin(terrain_entity,i+1), 888)) , i+1 );			// error if single skin was modified, tx edit was left, and init again 
							ent_setskin( terrain_entity , bmap_to_mipmap(bmap_to_format(bmap_for_entity(terrain_entity,i+1), 888)) , i+1 );	// works - but becomes black if tex set or colour fill was applied before
//							ent_setskin( terrain_entity , bmap_to_format(bmap_for_entity(terrain_entity,i+1), 888) , i+1 );							// becomes black immediately
						}
					else
						{
//							ent_setskin( terrain_entity , bmap_to_mipmap(bmap_to_format(ent_getskin(terrain_entity,i+1), 8888)) , i+1 );		// error if single skin was modified, tx edit was left, and init again
							ent_setskin( terrain_entity , bmap_to_mipmap(bmap_to_format(bmap_for_entity(terrain_entity,i+1), 8888)) , i+1 );	// works - mipmap needed ! 
						}					
				}	
			wait(1);	// needed
		}
	
	//---------------------------------------------------------------------------------------
	// store skins for reseting, and for enabling editing dds skins as tga
	
	str_cpy(temp_str,map_foldername);					// set folder name
//	str_for_entfile(temp2_str,terrain_entity);		// get terrain entity name
	str_cpy(temp2_str,"temp.hmp");						// or set a temporary file name
	str_cat(temp_str,temp2_str);							// attach file name to folder name
	
	TerrHmp_Tx_StoreSkins(temp_str,0);					// save skins as 0)tga,1)dds,2)png
	wait_for(TerrHmp_Tx_StoreSkins);
	wait(1);
	
	//------------------------------------------------------------------------------------------
	// 1st 24b tga skin, 2nd detail skin (24b or 32b tga) 
	// replace, color fill, and draw onto 1st skin, replace 2nd skin, delete 2nd skin?
	// texture panels are filled from subfolder, and used for drawing	
	if (teredit_tx_count<3)
		{			
			//----------------------------------------------------------------
			// brush image filename initialization 
sys_marker("tx1");			
			int arraysize = (int)TEREDIT_MAXPERSET;
			teredit_tx_filenames = (STRING**)sys_malloc( (int)arraysize * sizeof(STRING*) );		
			// init filename string array elements
			for(i=0;i<max_teredit_tx_items;i++)
				{
					teredit_tx_filenames[i] = str_create("");
				}	
sys_marker(NULL);				
			//-----------------------------------------------------------------
			// determine how many .tga files are in the folder, 
			// divide it by 5, and the result is the number of sets
			teredit_tx_set = 1;
			max_teredit_tx_sets = TEREDIT_MAXSET;
			teredit_textures_txt = txt_create( (max_teredit_tx_sets*max_teredit_tx_items) , 0 );		// 10*8=80
			for(i=0;i<max_teredit_tx_sets*max_teredit_tx_items;i++)
				{
					str_cpy( (teredit_textures_txt->pstring)[i] , "" );
				}
			i = 0;
//			STRING* temp_str = str_create("");
			str_cpy( temp_str , teredit_tx_foldername );
			str_cat( temp_str , "*.tga" );
			txt_for_dir( teredit_textures_txt, temp_str ); 									
			while (str_len((teredit_textures_txt->pstring)[i]) > 0)
				{
					i++;
				}
			max_teredit_tx_sets = integer(i/max_teredit_tx_items);
			if ( fraction(i/max_teredit_tx_items) > 0)
				{
					max_teredit_tx_sets++;		// allow to display not complete sets too
				}
			//------------------------------------------------------------------
			// copy first 5 filenames to string array
			for(i=0;i<max_teredit_tx_items;i++)
				{
		//			str_cpy( teredit_tx_filenames[i] , "" );														// subfolder			
					str_cpy( teredit_tx_filenames[i] , teredit_tx_foldername );								// subfolder
					str_cat( teredit_tx_filenames[i] , (teredit_textures_txt->pstring)[i] );			// filename with extension	
					
					if ( str_cmpi( teredit_tx_filenames[i] , teredit_tx_foldername ) )					// if nothing was added to folder name, clear it to make an empty slot
						{
							str_cpy( teredit_tx_filenames[i] , "panels\\_empty.tga" );
						}
				}			
			//-----------------------------------------------------------------
			// set the 5 panel bmaps			
			teredit_tx1_panel_bmp = bmap_create(teredit_tx_filenames[0]);
			teredit_tx2_panel_bmp = bmap_create(teredit_tx_filenames[1]);
			teredit_tx3_panel_bmp = bmap_create(teredit_tx_filenames[2]);
			teredit_tx4_panel_bmp = bmap_create(teredit_tx_filenames[3]);
			teredit_tx5_panel_bmp = bmap_create(teredit_tx_filenames[4]);					
			
			//------------------------------------------------------------------
			// initial value settings	
			for(i=0;i<max_teredit_tx_items;i++)
				{
					teredit_tx_switch_type[i] = 0;
				}
			teredit_tx_switch_type[teredit_tx_selected] = 1;
		//	teredit_tilesize = 1;													
			
			//----------------------------------------------------------------			
			// create panels - same as at 32b multiskin - images scaled to panel size
			
			// texture panel 1
			teredit_tx1_panel = pan_create( teredit_tx1_panel_str , 13 );				// assign bitmap
			set( teredit_tx1_panel , SHOW );					
			teredit_tx1_panel.pos_x = ui_submenupanel_x + 16;								// screen_size.x-256+16;
			teredit_tx1_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
		//	bmap_load( teredit_tx1_panel_bmp , teredit_tx_filenames[0] , 1 );			// done on init
//			teredit_tx1_panel.bmap = teredit_tx1_panel_bmp;									// done on init
			teredit_tx1_panel.scale_x = teredit_tx_thumb_size / bmap_width(teredit_tx1_panel_bmp); 
			teredit_tx1_panel.scale_y = teredit_tx_thumb_size / bmap_height(teredit_tx1_panel_bmp); 
			teredit_tx1_panel.event = TerEdit_Tx_Toggle;
			
			// texture panel 2
			teredit_tx2_panel = pan_create( teredit_tx2_panel_str , 13 );
			set( teredit_tx2_panel , SHOW );					
			teredit_tx2_panel.pos_x = ui_submenupanel_x + 56;								// screen_size.x-256+16+40;
			teredit_tx2_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
		//	bmap_load( teredit_tx2_panel_bmp , teredit_tx_filenames[1] , 1 );			// done on init
//			teredit_tx2_panel.bmap = teredit_tx2_panel_bmp;									// done on init
			teredit_tx2_panel.scale_x = teredit_tx_thumb_size / bmap_width(teredit_tx2_panel_bmp); 
			teredit_tx2_panel.scale_y = teredit_tx_thumb_size / bmap_height(teredit_tx2_panel_bmp); 
			teredit_tx2_panel.event = TerEdit_Tx_Toggle;
			
			// texture panel 3
			teredit_tx3_panel = pan_create( teredit_tx3_panel_str , 13 );
			set( teredit_tx3_panel , SHOW );					
			teredit_tx3_panel.pos_x = ui_submenupanel_x + 96;								// screen_size.x-256+16+80;
			teredit_tx3_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
		//	bmap_load( teredit_tx3_panel_bmp , teredit_tx_filenames[2] , 1 );			// done on init
//			teredit_tx3_panel.bmap = teredit_tx3_panel_bmp;									// done on init
			teredit_tx3_panel.scale_x = teredit_tx_thumb_size / bmap_width(teredit_tx3_panel_bmp); 
			teredit_tx3_panel.scale_y = teredit_tx_thumb_size / bmap_height(teredit_tx3_panel_bmp); 
			teredit_tx3_panel.event = TerEdit_Tx_Toggle;
			
			// texture panel 4
			teredit_tx4_panel = pan_create( teredit_tx4_panel_str , 13 );
			set( teredit_tx4_panel , SHOW );					
			teredit_tx4_panel.pos_x = ui_submenupanel_x + 136;								// screen_size.x-256+16+120;
			teredit_tx4_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
		//	bmap_load( teredit_tx4_panel_bmp , teredit_tx_filenames[3] , 1 );			// done on init
//			teredit_tx4_panel.bmap = teredit_tx4_panel_bmp;									// done on init
			teredit_tx4_panel.scale_x = teredit_tx_thumb_size / bmap_width(teredit_tx4_panel_bmp); 
			teredit_tx4_panel.scale_y = teredit_tx_thumb_size / bmap_height(teredit_tx4_panel_bmp); 
			teredit_tx4_panel.event = TerEdit_Tx_Toggle;
			
			// texture panel 5
			teredit_tx5_panel = pan_create( teredit_tx5_panel_str , 13 );
			set( teredit_tx5_panel , SHOW );					
			teredit_tx5_panel.pos_x = ui_submenupanel_x + 176;								// screen_size.x-256+16+160;
			teredit_tx5_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
		//	bmap_load( teredit_tx5_panel_bmp , teredit_tx_filenames[4] , 1 );			// done on init
//			teredit_tx5_panel.bmap = teredit_tx5_panel_bmp;									// done on init
			teredit_tx5_panel.scale_x = teredit_tx_thumb_size / bmap_width(teredit_tx5_panel_bmp); 
			teredit_tx5_panel.scale_y = teredit_tx_thumb_size / bmap_height(teredit_tx5_panel_bmp); 
			teredit_tx5_panel.event = TerEdit_Tx_Toggle;
			
			//----------------------------------------------------------------
//			// update panel images - more exaclty image names and then the images themselves	- not needed here
//			TerEdit_Tx_UpdateSets24();
//			wait_for(TerEdit_Tx_UpdateSets24);
			
			// skin size
			teredit_tx_size = bmap_width( ent_getskin(terrain_entity,1) );	
//			teredit_tx_size = bmap_width( bmap_for_entity(terrain_entity,1) );	
			
			//------------------------------------------------------------------
			// string 1
			pan_setstring(teredit_tx_panel, 0,	15,15,	panel_font2,			str_create("EDIT TEXTURE") );
	
			// button 1 2 - change set
			pan_setbutton(teredit_tx_panel, 0,	1, 	220,40,		s_arrow_up_on_bmp, s_arrow_up_off_bmp, s_arrow_up_on_bmp, s_arrow_up_off_bmp,						TerEdit_Tx_DecSet, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	220,62,		s_arrow_down_on_bmp, s_arrow_down_off_bmp, s_arrow_down_on_bmp, s_arrow_down_off_bmp,			TerEdit_Tx_IncSet, 		NULL, NULL); 
			
			// button 3 - select brush	
			pan_setbutton(teredit_tx_panel, 0,	1, 	16-2,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 
			
			// string 2
			pan_setstring(teredit_tx_panel, 0,	15,85,	panel_font,			teredit_tx_filenames[teredit_tx_selected] );	// str_create"select texture")
			
			// button 4 5 - slider 1 - opacity
			pan_setdigits(teredit_tx_panel, 0,	15,110,		"Opacity: %0.f",		panel_font,	1, 					teredit_tx_opacity );
			pan_setslider(teredit_tx_panel, 0,	90+12,112,	horiz_slider_bmp,slider_knob_bmp,	0,100,			teredit_tx_opacity ); 
			
			pan_setbutton(teredit_tx_panel, 0,	1, 	90,110,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button24, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	210,110,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button24, 		NULL, NULL); 			
			
			// string 3
			pan_setstring(teredit_tx_panel, 0,	15,170,	panel_font,			str_create("Set skin color") );
			// button 6 - color fill + set skin size
			pan_setbutton(teredit_tx_panel, 0,	1, 	95,170,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		TerEdit_Tx_SetColor24,		NULL,NULL); 	
			
			// slider 2 3 4 - rgb settings for color fill
			pan_setdigits(teredit_tx_panel, 0,	15,190,	"R: %0.f",				panel_font,	1, 		teredit_tx_red);
			pan_setdigits(teredit_tx_panel, 0,	15,210,	"G: %0.f",				panel_font,	1, 		teredit_tx_green);
			pan_setdigits(teredit_tx_panel, 0,	15,230,	"B: %0.f",				panel_font,	1, 		teredit_tx_blue);
			pan_setslider(teredit_tx_panel, 0,	90+12,192,	horiz_slider_bmp,slider_knob_bmp,	0,255,	teredit_tx_red); 
			pan_setslider(teredit_tx_panel, 0,	90+12,212,	horiz_slider_bmp,slider_knob_bmp,	0,255,	teredit_tx_green); 
			pan_setslider(teredit_tx_panel, 0,	90+12,232,	horiz_slider_bmp,slider_knob_bmp,	0,255,	teredit_tx_blue); 
			
			// button 7 8 , 9 10 , 11 12 - rgb settings for color fill
			pan_setbutton(teredit_tx_panel, 0,	1, 	90,190,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button24, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	210,190,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button24, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	90,210,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button24, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	210,210,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button24, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	90,230,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button24, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	210,230,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button24, 		NULL, NULL); 																	
			
			// string 4
			pan_setstring(teredit_tx_panel, 0,	15,130,	panel_font,			str_create("Skin size:") );
			pan_setdigits(teredit_tx_panel, 0,	115,130,							"%0.f", 	panel_font, 	1, 	teredit_tx_size);
			// button 13 14 - skin texture size - read on init, and can be set by color fill			
			pan_setbutton(teredit_tx_panel, 0,	1, 	90,130,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button24, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	150,130,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button24, 		NULL, NULL); 																							
			
			// string 5
			pan_setstring(teredit_tx_panel, 0,	15,150,	panel_font,			str_create("Set texture") );
			// button 15 - fill with selected texture + set skin size
			pan_setbutton(teredit_tx_panel, 0,	1, 	95,150,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		TerEdit_Tx_SetTexture24,		NULL,NULL); 	
			
			// string 6
			pan_setstring(teredit_tx_panel, 0,	15,260,	panel_font,			str_create("Skin texture") );
			// button 16 17 - load save : relpace skin1 with image -> filemenu call
			pan_setbutton(teredit_tx_panel, 0,	1, 	95,260,		ss_save_on_bmp,ss_save_off_bmp,ss_save_over_bmp,ss_save_over_bmp,		TerEdit_Save_Texture24,		NULL,NULL); 				
			pan_setbutton(teredit_tx_panel, 0,	1, 	150,260,		ss_load_on_bmp,ss_load_off_bmp,ss_load_over_bmp,ss_load_over_bmp,		TerEdit_Load_Texture24,		NULL,NULL); 				
			
			// string 7
			pan_setstring(teredit_tx_panel, 0,	15,280,	panel_font,			str_create("Detail map") );
			// button 18 19 - load save : relpace skin2 with image -> filemenu call
			pan_setbutton(teredit_tx_panel, 0,	1, 	95,280,		ss_save_on_bmp,ss_save_off_bmp,ss_save_over_bmp,ss_save_over_bmp,		TerEdit_Save_DetailTexture24,		NULL,NULL); 				
			pan_setbutton(teredit_tx_panel, 0,	1, 	150,280,		ss_load_on_bmp,ss_load_off_bmp,ss_load_over_bmp,ss_load_over_bmp,		TerEdit_Load_DetailTexture24,		NULL,NULL); 				
			
			// string 8
			pan_setstring(teredit_tx_panel, 0,	170,30,	panel_font,			str_create("select texture") );	// 15,85,
			
//			// UNDO-REDO SYSTEM WORKS WITH ERRORS MAYBE BECAUSE OF HUGE TEXTURE FILES (ITS 43B VERSION IS FINE), AND NOT USER FRIENDLY !
//			// button 20 - store
//			pan_setbutton(teredit_tx_panel, 0,	1, 	100,320,		ss_store_on_bmp,ss_store_off_bmp,ss_store_over_bmp,ss_store_over_bmp,			TerEdit_Tx_Store,		NULL,NULL);
//			
//			// button 21 - undo
//			pan_setbutton(teredit_tx_panel, 0,	1, 	25,320,		ss_undo_on_bmp,ss_undo_off_bmp,ss_undo_over_bmp,ss_undo_over_bmp,					TerEdit_Tx_Undo,		NULL,NULL);
//			
//			// button 22 - redo
//			pan_setbutton(teredit_tx_panel, 0,	1, 	25,350,		ss_redo_on_bmp,ss_redo_off_bmp,ss_redo_over_bmp,ss_redo_over_bmp,					TerEdit_Tx_Redo,		NULL,NULL);
//			
//			// button 23 - reset
//			pan_setbutton(teredit_tx_panel, 0,	1, 	100,350,		ss_reset_on_bmp,ss_reset_off_bmp,ss_reset_over_bmp,ss_reset_over_bmp,			TerEdit_Tx_Reset,		NULL,NULL);
//			
//			// button 24 - restore
//			pan_setbutton(teredit_tx_panel, 0,	1, 	175,350,		ss_restore_on_bmp,ss_restore_off_bmp,ss_restore_over_bmp,ss_restore_over_bmp,	TerEdit_Tx_Restore,		NULL,NULL);
		}
	
	//---------------------------------------------------------------------------------------
	// if >2 
	// multiskin : 32b tga skins
	// replace, color fill, draw alpha, delete any skin
	// texture panels are filled with skin images, but with alpha = 100, or transformed into 24b images
	if (teredit_tx_count>2)
		{
			// panel images - needed to create to enable them to be passed as function parameters
			teredit_tx1_panel_bmp = bmap_create("panels\\_empty.tga");
			teredit_tx2_panel_bmp = bmap_create("panels\\_empty.tga");
			teredit_tx3_panel_bmp = bmap_create("panels\\_empty.tga");
			teredit_tx4_panel_bmp = bmap_create("panels\\_empty.tga");
			teredit_tx5_panel_bmp = bmap_create("panels\\_empty.tga");
						
			max_teredit_tx_sets = integer(teredit_tx_count/max_teredit_tx_items) +1;	// check real amount of skins and determine set settings						
			
			// read in 32b skin images and convert them to 24b panel images having the same size
			TerEdit_Tx_UpdateSets32();
			wait_for(TerEdit_Tx_UpdateSets32);
			wait(1);
			
			//----------------------------------
			
			// texture panel 1
			teredit_tx1_panel = pan_create( teredit_tx1_panel_str , 13 );
			set( teredit_tx1_panel , SHOW );					
			teredit_tx1_panel.pos_x = ui_submenupanel_x + 16;								// screen_size.x-256+16;
			teredit_tx1_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
		//	bmap_load( teredit_tx1_panel_bmp , teredit_tx_filenames[0] , 1 );			// done on init
		//	teredit_tx1_panel.bmap = teredit_tx1_panel_bmp;									// done on init
			teredit_tx1_panel.scale_x = teredit_tx_thumb_size / bmap_width(teredit_tx1_panel_bmp); 
			teredit_tx1_panel.scale_y = teredit_tx_thumb_size / bmap_height(teredit_tx1_panel_bmp); 
			teredit_tx1_panel.event = TerEdit_Tx_Toggle;
			
			// texture panel 2
			teredit_tx2_panel = pan_create( teredit_tx2_panel_str , 13 );
			set( teredit_tx2_panel , SHOW );					
			teredit_tx2_panel.pos_x = ui_submenupanel_x + 56;								// screen_size.x-256+16+40;
			teredit_tx2_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
		//	bmap_load( teredit_tx2_panel_bmp , teredit_tx_filenames[1] , 1 );			// done on init
		//	teredit_tx2_panel.bmap = teredit_tx2_panel_bmp;									// done on init
			teredit_tx2_panel.scale_x = teredit_tx_thumb_size / bmap_width(teredit_tx2_panel_bmp); 
			teredit_tx2_panel.scale_y = teredit_tx_thumb_size / bmap_height(teredit_tx2_panel_bmp); 
			teredit_tx2_panel.event = TerEdit_Tx_Toggle;
			
			// texture panel 3
			teredit_tx3_panel = pan_create( teredit_tx3_panel_str , 13 );
			set( teredit_tx3_panel , SHOW );					
			teredit_tx3_panel.pos_x = ui_submenupanel_x + 96;								// screen_size.x-256+16+80;
			teredit_tx3_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
		//	bmap_load( teredit_tx3_panel_bmp , teredit_tx_filenames[2] , 1 );			// done on init
		//	teredit_tx3_panel.bmap = teredit_tx3_panel_bmp;									// done on init
			teredit_tx3_panel.scale_x = teredit_tx_thumb_size / bmap_width(teredit_tx3_panel_bmp); 
			teredit_tx3_panel.scale_y = teredit_tx_thumb_size / bmap_height(teredit_tx3_panel_bmp); 
			teredit_tx3_panel.event = TerEdit_Tx_Toggle;
			
			// texture panel 4
			teredit_tx4_panel = pan_create( teredit_tx4_panel_str , 13 );
			set( teredit_tx4_panel , SHOW );					
			teredit_tx4_panel.pos_x = ui_submenupanel_x + 136;								// screen_size.x-256+16+120;
			teredit_tx4_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
		//	bmap_load( teredit_tx4_panel_bmp , teredit_tx_filenames[3] , 1 );			// done on init
		//	teredit_tx4_panel.bmap = teredit_tx4_panel_bmp;									// done on init
			teredit_tx4_panel.scale_x = teredit_tx_thumb_size / bmap_width(teredit_tx4_panel_bmp); 
			teredit_tx4_panel.scale_y = teredit_tx_thumb_size / bmap_height(teredit_tx4_panel_bmp); 
			teredit_tx4_panel.event = TerEdit_Tx_Toggle;
			
			// texture panel 5
			teredit_tx5_panel = pan_create( teredit_tx5_panel_str , 13 );
			set( teredit_tx5_panel , SHOW );					
			teredit_tx5_panel.pos_x = ui_submenupanel_x + 176;								// screen_size.x-256+16+160;
			teredit_tx5_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
		//	bmap_load( teredit_tx5_panel_bmp , teredit_tx_filenames[4] , 1 );			// done on init
		//	teredit_tx5_panel.bmap = teredit_tx5_panel_bmp;									// done on init
			teredit_tx5_panel.scale_x = teredit_tx_thumb_size / bmap_width(teredit_tx5_panel_bmp); 
			teredit_tx5_panel.scale_y = teredit_tx_thumb_size / bmap_height(teredit_tx5_panel_bmp); 
			teredit_tx5_panel.event = TerEdit_Tx_Toggle;
			
			//-----------------------------
			// string 1
			pan_setstring(teredit_tx_panel, 0,	15,15,	panel_font2,			str_create("EDIT TEXTURES") );
			
			// button 1 2 - change set
			pan_setbutton(teredit_tx_panel, 0,	1, 	220,40,		s_arrow_up_on_bmp, s_arrow_up_off_bmp, s_arrow_up_on_bmp, s_arrow_up_off_bmp,						TerEdit_Tx_DecSet, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	220,62,		s_arrow_down_on_bmp, s_arrow_down_off_bmp, s_arrow_down_on_bmp, s_arrow_down_off_bmp,			TerEdit_Tx_IncSet, 		NULL, NULL); 
			
			// button 3 - select brush	
			pan_setbutton(teredit_tx_panel, 0,	1, 	16-2,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 
			
			// button 4 5 - slider 1 - opacity
			pan_setdigits(teredit_tx_panel, 0,	15,110,		"Opacity: %0.f",		panel_font,	1, 					teredit_tx_opacity );
			pan_setslider(teredit_tx_panel, 0,	90+12,112,	horiz_slider_bmp,slider_knob_bmp,	0,100,			teredit_tx_opacity ); 
			
			pan_setbutton(teredit_tx_panel, 0,	1, 	90,110,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button32, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	210,110,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button32, 		NULL, NULL); 
			
			// string 2
			pan_setstring(teredit_tx_panel, 0,	180,30,	panel_font,			str_create("select skin") );	// 15,85,
			
			// button 6 - color fill
			pan_setstring(teredit_tx_panel, 0,	15,140,	panel_font,			str_create("Set skin color:") );
			pan_setbutton(teredit_tx_panel, 0,	1, 	95,140,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		TerEdit_Tx_SetColor,		NULL,NULL); 	
			
			// slider 2 3 4 - rgb settings for color fill
			pan_setdigits(teredit_tx_panel, 0,	15,160,	"R: %0.f",				panel_font,	1, 		teredit_tx_red);
			pan_setdigits(teredit_tx_panel, 0,	15,180,	"G: %0.f",				panel_font,	1, 		teredit_tx_green);
			pan_setdigits(teredit_tx_panel, 0,	15,200,	"B: %0.f",				panel_font,	1, 		teredit_tx_blue);
			pan_setslider(teredit_tx_panel, 0,	90+12,162,	horiz_slider_bmp,slider_knob_bmp,	0,255,	teredit_tx_red); 
			pan_setslider(teredit_tx_panel, 0,	90+12,182,	horiz_slider_bmp,slider_knob_bmp,	0,255,	teredit_tx_green); 
			pan_setslider(teredit_tx_panel, 0,	90+12,202,	horiz_slider_bmp,slider_knob_bmp,	0,255,	teredit_tx_blue); 
			
			// button 7 8 , 9 10 , 11 12 - rgb settings for color fill
			pan_setbutton(teredit_tx_panel, 0,	1, 	90,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button32, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	210,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button32, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	90,180,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button32, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	210,180,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button32, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	90,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button32, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	210,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button32, 		NULL, NULL); 
									
			// button 13 - *** change/mod/set selected skin with image -> filemenu call
			pan_setbutton(teredit_tx_panel, 0,	1, 	205,85,		ss_change_on_bmp,ss_change_off_bmp,ss_change_over_bmp,ss_change_over_bmp,		TerEdit_Tx_SetSkin32,		NULL,NULL); 	
						
			// button 14 - set skin alpha to alpha
			pan_setstring(teredit_tx_panel, 0,	15,230,	panel_font,			str_create("Set skin alpha:") );
			pan_setbutton(teredit_tx_panel, 0,	1, 	95,230,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		TerEdit_Tx_SetAlpha,		NULL,NULL); 	
						
			// button 15 16 - slider 5 - alpha
			pan_setdigits(teredit_tx_panel, 0,	15,250,	"Alpha: %0.f",				panel_font,	1, 		teredit_tx_alpha);
			pan_setslider(teredit_tx_panel, 0,	90+12,252,	horiz_slider_bmp,slider_knob_bmp,	0,100,	teredit_tx_alpha); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	90,250,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button32, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	210,250,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button32, 		NULL, NULL); 
						
			// button 17 - add skin after selected skin
			pan_setbutton(teredit_tx_panel, 0,	1, 	95,85,		ss_add_on_bmp,ss_add_off_bmp,ss_add_over_bmp,ss_add_over_bmp,		TerEdit_Tx_AddSkin32,		NULL,NULL); 	
			
			// button 18 - del selected skin - shift following skins forward -> clear
			pan_setbutton(teredit_tx_panel, 0,	1, 	150,85,		ss_clear_on_bmp,ss_clear_off_bmp,ss_clear_over_bmp,ss_clear_over_bmp,		TerEdit_Tx_DelSkin32,		NULL,NULL); 	
			
			// button 19 - load skins from images
			pan_setstring(teredit_tx_panel, 0,	15,280,	panel_font,			str_create("All skins") );
			pan_setbutton(teredit_tx_panel, 0,	1, 	150,280,		ss_load_on_bmp,ss_load_off_bmp,ss_load_over_bmp,ss_load_over_bmp,		TerEdit_Load_Textures,		NULL,NULL); 										
			// button 20 - save skins to images
			pan_setbutton(teredit_tx_panel, 0,	1, 	95,280,		ss_save_on_bmp,ss_save_off_bmp,ss_save_over_bmp,ss_save_over_bmp,		TerEdit_Save_Textures,		NULL,NULL); 				
			// bmap_save( bmap_for_entity(terrain_entity , 0) ,"test_1.tga"); // works for 24b!, 32b? tga ; png jpg bmp dds : said to be okay
					
			// button 21,22 - move skin down/up
//			pan_setstring(teredit_tx_panel, 0,			125+12,20,	panel_font,			str_create("Move skin") );
//			pan_setbutton(teredit_tx_panel, 0,	1, 	125,20,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button32, 		NULL, NULL); 
//			pan_setbutton(teredit_tx_panel, 0,	1, 	210,20,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button32, 		NULL, NULL); 
			pan_setstring(teredit_tx_panel, 0,			15+12,85,	panel_font,			str_create("Move skin") );
			pan_setbutton(teredit_tx_panel, 0,	1, 	15,85,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				TerEdit_Tx_Button32, 		NULL, NULL); 
			pan_setbutton(teredit_tx_panel, 0,	1, 	80,85,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		TerEdit_Tx_Button32, 		NULL, NULL); 
			
//			// UNDO.REDO SYSTEM WORKS FINE BUT TON USER FRIENDLY !!!
//			// button 23 - store
//			pan_setbutton(teredit_tx_panel, 0,	1, 	100,320,		ss_store_on_bmp,ss_store_off_bmp,ss_store_over_bmp,ss_store_over_bmp,			TerEdit_Tx_Store,		NULL,NULL);
//			
//			// button 24 - undo
//			pan_setbutton(teredit_tx_panel, 0,	1, 	25,320,		ss_undo_on_bmp,ss_undo_off_bmp,ss_undo_over_bmp,ss_undo_over_bmp,					TerEdit_Tx_Undo,		NULL,NULL);
//			
//			// button 25 - redo
//			pan_setbutton(teredit_tx_panel, 0,	1, 	25,350,		ss_redo_on_bmp,ss_redo_off_bmp,ss_redo_over_bmp,ss_redo_over_bmp,					TerEdit_Tx_Redo,		NULL,NULL);
//			
//			// button 26 - reset
//			pan_setbutton(teredit_tx_panel, 0,	1, 	100,350,		ss_reset_on_bmp,ss_reset_off_bmp,ss_reset_over_bmp,ss_reset_over_bmp,			TerEdit_Tx_Reset,		NULL,NULL);
//			
//			// button 27 - restore
//			pan_setbutton(teredit_tx_panel, 0,	1, 	175,350,		ss_restore_on_bmp,ss_restore_off_bmp,ss_restore_over_bmp,ss_restore_over_bmp,	TerEdit_Tx_Restore,		NULL,NULL);
		}
		
//	ptr_remove(temp_str);
//	ptr_remove(temp2_str);
}


void		TerEdit_Tx_Close()
{
	//-------------------------------------	
	if (teredit_tx_panel)
		{
			ptr_remove(teredit_tx_panel);
			teredit_tx_panel = NULL;
		}
	//-------------------------------------	
	if (teredit_tx1_panel)
		{
			ptr_remove(teredit_tx1_panel);		
			teredit_tx1_panel = NULL;
		}
	if (teredit_tx2_panel)
		{
			ptr_remove(teredit_tx2_panel);		
			teredit_tx2_panel = NULL;
		}
	if (teredit_tx3_panel)
		{
			ptr_remove(teredit_tx3_panel);		
			teredit_tx3_panel = NULL;
		}
	if (teredit_tx4_panel)
		{
			ptr_remove(teredit_tx4_panel);		
			teredit_tx4_panel = NULL;
		}
	if (teredit_tx5_panel)
		{
			ptr_remove(teredit_tx5_panel);		
			teredit_tx5_panel = NULL;
		}
	//-------------------------------------	
	if (teredit_tx1_panel_bmp)
		{
			bmap_purge(teredit_tx1_panel_bmp);
			ptr_remove(teredit_tx1_panel_bmp);		
			teredit_tx1_panel_bmp = NULL;
		}
	if (teredit_tx2_panel_bmp)
		{
			bmap_purge(teredit_tx2_panel_bmp);
			ptr_remove(teredit_tx2_panel_bmp);		
			teredit_tx2_panel_bmp = NULL;
		}
	if (teredit_tx3_panel_bmp)
		{
			bmap_purge(teredit_tx3_panel_bmp);
			ptr_remove(teredit_tx3_panel_bmp);		
			teredit_tx3_panel_bmp = NULL;
		}
	if (teredit_tx4_panel_bmp)
		{
			bmap_purge(teredit_tx4_panel_bmp);
			ptr_remove(teredit_tx4_panel_bmp);		
			teredit_tx4_panel_bmp = NULL;
		}
	if (teredit_tx5_panel_bmp)
		{
			bmap_purge(teredit_tx5_panel_bmp);
			ptr_remove(teredit_tx5_panel_bmp);		
			teredit_tx5_panel_bmp = NULL;
		}
	//-------------------------------------	
	// remove temp files
	int i = 1;
	while(1)
		{
			STRING* temp_str = str_create("");
			STRING* temp2_str = str_create("");
			str_cpy(temp_str,map_foldername);					// set folder name
			str_cat(temp_str,"temp_skin");						// or set a temporary file name
			str_for_int(temp2_str,i);
			str_cat(temp_str,temp2_str);							// attach file name to folder name	
			str_cat(temp_str,".tga");
			
//			long temp_long = file_date(temp_str);			
////			if ( file_exists(temp_str) )							// not okay if file exists in another folder it returns true!!!
//			if (temp_long!=(long)0)
			if (file_exists_onepath(temp_str))
				{
					remove( _chr(temp_str) );	
//					file_delete(temp_str);							// could delete in another folder if it does not exists at given path !
				}
			else
				{
					break;
				}
			i++;
		}
	//-------------------------------------	
	// remove undo files - not surely created for all skins, so check all possible cases
	i = 1;
	while(i<ent_status(terrain_entity,8)+1)
		{
			STRING* temp_str = str_create("");
			STRING* temp2_str = str_create("");
			str_cpy(temp_str,map_foldername);					// set folder name
			str_cat(temp_str,"undo_skin");						// or set a temporary file name
			str_for_int(temp2_str,i);
			str_cat(temp_str,temp2_str);							// attach file name to folder name	
			str_cat(temp_str,".tga");
			
//			long temp_long = file_date(temp_str);			
////			if ( file_exists(temp_str) )							// not okay if file exists in another folder it returns true!!!
//			if (temp_long!=(long)0)
			if (file_exists_onepath(temp_str))
				{
					remove( _chr(temp_str) );	
//					file_delete(temp_str);							// could delete in another folder if it does not exists at given path !
				}
			i++;
		}
	//-------------------------------------	
	// remove redo files - not surely created for all skins, so check all possible cases
	i = 1;
	while(i<ent_status(terrain_entity,8)+1)
		{
			STRING* temp_str = str_create("");
			STRING* temp2_str = str_create("");
			str_cpy(temp_str,map_foldername);					// set folder name
			str_cat(temp_str,"redo_skin");						// or set a temporary file name
			str_for_int(temp2_str,i);
			str_cat(temp_str,temp2_str);							// attach file name to folder name	
			str_cat(temp_str,".tga");
			
//			long temp_long = file_date(temp_str);			
////			if ( file_exists(temp_str) )							// not okay if file exists in another folder it returns true!!!
//			if (temp_long!=(long)0)
			if (file_exists_onepath(temp_str))
				{
					remove( _chr(temp_str) );						// or file_delete(temp_str);
//					file_delete(temp_str);							// could delete in another folder if it does not exists at given path !
				}
			i++;
		}
	//-------------------------------------	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Tx_Button24(button_number)
{
	wait(3);
	
	if (button_number==(var)4)
		{
			teredit_tx_opacity -= 1;
			teredit_tx_opacity = maxv( 0 , teredit_tx_opacity );
		}
	else if (button_number==(var)5)
		{
			teredit_tx_opacity += 1;
			teredit_tx_opacity = minv( 100 , teredit_tx_opacity );
		}
	//-------------------------------
	else if (button_number==(var)7)
		{
			teredit_tx_red -= 1;
			teredit_tx_red = maxv( 0 , teredit_tx_red );
		}
	else if (button_number==(var)8)
		{
			teredit_tx_red += 1;
			teredit_tx_red = minv( 255 , teredit_tx_red );
		}
	else if (button_number==(var)9)
		{
			teredit_tx_green -= 1;
			teredit_tx_green = maxv( 0 , teredit_tx_green );
		}
	else if (button_number==(var)10)
		{
			teredit_tx_green += 1;
			teredit_tx_green = minv( 255 , teredit_tx_green );
		}
	else if (button_number==(var)11)
		{
			teredit_tx_blue -= 1;
			teredit_tx_blue = maxv( 0 , teredit_tx_blue );
		}
	else if (button_number==(var)12)
		{
			teredit_tx_blue += 1;
			teredit_tx_blue = minv( 255 , teredit_tx_blue );
		}
	//------------------------------------
	else if (button_number==(var)13)
		{
			teredit_tx_size /= 2;
			teredit_tx_size = maxv( 512 , teredit_tx_size );
		}
	else if (button_number==(var)14)
		{
			teredit_tx_size *= 2;
			teredit_tx_size = minv( d3d_texlimit , teredit_tx_size );
		}
}


void		TerEdit_Tx_Button32(button_number)
{
	wait(3);
	
	if (button_number==(var)4)
		{
			teredit_tx_opacity -= 1;
			teredit_tx_opacity = maxv( 0 , teredit_tx_opacity );
		}
	else if (button_number==(var)5)
		{
			teredit_tx_opacity += 1;
			teredit_tx_opacity = minv( 100 , teredit_tx_opacity );
		}
	//-------------------------------
	else if (button_number==(var)7)
		{
			teredit_tx_red -= 1;
			teredit_tx_red = maxv( 0 , teredit_tx_red );
		}
	else if (button_number==(var)8)
		{
			teredit_tx_red += 1;
			teredit_tx_red = minv( 255 , teredit_tx_red );
		}
	else if (button_number==(var)9)
		{
			teredit_tx_green -= 1;
			teredit_tx_green = maxv( 0 , teredit_tx_green );
		}
	else if (button_number==(var)10)
		{
			teredit_tx_green += 1;
			teredit_tx_green = minv( 255 , teredit_tx_green );
		}
	else if (button_number==(var)11)
		{
			teredit_tx_blue -= 1;
			teredit_tx_blue = maxv( 0 , teredit_tx_blue );
		}
	else if (button_number==(var)12)
		{
			teredit_tx_blue += 1;
			teredit_tx_blue = minv( 255 , teredit_tx_blue );
		}
	//------------------------------------
	else if (button_number==(var)15)
		{
			teredit_tx_alpha -= 1;
			teredit_tx_alpha = maxv( 0 , teredit_tx_alpha );
		}
	else if (button_number==(var)16)
		{
			teredit_tx_alpha += 1;
			teredit_tx_alpha = minv( 100 , teredit_tx_alpha );
		}
	//------------------------------------
	else if (button_number==(var)21)
		{
			TerEdit_Tx_MoveSkinDown();
		}
	else if (button_number==(var)22)
		{
			TerEdit_Tx_MoveSkinUp();
		}
}

		
////////////////////////////////////////////////////////////////////


void		TerEdit_Tx_Toggle(PANEL* panel)
{	
	int i;
	for(i=0;i<max_teredit_tx_items;i++)
		{
			teredit_tx_switch_type[i] = 0;							// 0..4
		}
		
	if (panel == teredit_tx1_panel)
		{
			teredit_tx_switch_type[0] = 1;
			pan_setbutton(teredit_tx_panel, 3,	1, 	16-2 +0,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 			
			teredit_tx_selected = 0;
		}
	else if (panel == teredit_tx2_panel)
		{
			teredit_tx_switch_type[1] = 1;
			pan_setbutton(teredit_tx_panel, 3,	1, 	16-2 +40,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 			
			teredit_tx_selected = 1;
		}
	else if (panel == teredit_tx3_panel)
		{
			teredit_tx_switch_type[2] = 1;
			pan_setbutton(teredit_tx_panel, 3,	1, 	16-2 +80,45-2,		select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 			
			teredit_tx_selected = 2;
		}
	else if (panel == teredit_tx4_panel)
		{
			teredit_tx_switch_type[3] = 1;
			pan_setbutton(teredit_tx_panel, 3,	1, 	16-2 +120,45-2,	select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 			
			teredit_tx_selected = 3;
		}
	else if (panel == teredit_tx5_panel)
		{
			teredit_tx_switch_type[4] = 1;
			pan_setbutton(teredit_tx_panel, 3,	1, 	16-2 +160,45-2,	select_thumb_bmp, select_thumb_bmp, select_thumb_bmp, select_thumb_bmp,			NULL, 		NULL, NULL); 			
			teredit_tx_selected = 4;
		}
		
	if (ent_status(terrain_entity,8)<3)
		pan_setstring(teredit_tx_panel, 2,	15,85,	panel_font,			teredit_tx_filenames[teredit_tx_selected] );	
}


////////////////////////////////////


void		TerEdit_Tx_IncSet()
{ 
	// jump to next existing set
	teredit_tx_set++;
	if (teredit_tx_set>max_teredit_tx_sets)
		{
			teredit_tx_set = max_teredit_tx_sets;
			return;
		}		
	if (teredit_tx_count<3)
		{
			TerEdit_Tx_UpdateSets24();		// 1skin
		}
	else
		{
			TerEdit_Tx_UpdateSets32();		// multiskin
		}
}


void		TerEdit_Tx_DecSet()
{ 
	// jump to next existing set
	teredit_tx_set--;
	if (teredit_tx_set<1)
		{
			teredit_tx_set = 1;
			return;
		}
	if (teredit_tx_count<3)
		{
			TerEdit_Tx_UpdateSets24();		// 1skin
		}
	else
		{
			TerEdit_Tx_UpdateSets32();		// multiskin
		}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Tx_UpdateSets24()	
{
	int i;
	
	// copy the proper 8 filenames to string array
	int j = teredit_tx_set - 1;
	int k = j * max_teredit_tx_items;
	
	// copy first 5 filenames to string array
	for(i=0;i<max_teredit_tx_items;i++)
		{		
			str_cpy( teredit_tx_filenames[i] , teredit_tx_foldername );								// subfolder
			str_cat( teredit_tx_filenames[i] , (teredit_textures_txt->pstring)[k+i] );			// filename with extension	
			
			if ( str_cmpi( teredit_tx_filenames[i] , teredit_tx_foldername ) )			// if nothing was added to folder name, clear it to make an empty slot
				{
					str_cpy( teredit_tx_filenames[i] , "panels\\_empty.tga" );
				}
			
			BMAP* temp_bmap = bmap_create( teredit_tx_filenames[i] );
			
			if (i==0)	
				{		
					if ( str_cmpi( teredit_tx_filenames[i] , "panels\\_empty.tga" ) )
						{
							TerEdit_Tx_EmptySlot24(teredit_tx1_panel_bmp);							
						}
					else
						{
							TerEdit_Tx_CopyBmaptoBmap(temp_bmap, teredit_tx1_panel_bmp);	// scaled by bmap_blit to old panel image size							
						}
				}
			else if (i==1)	
				{		
					if ( str_cmpi( teredit_tx_filenames[i] , "panels\\_empty.tga" ) )
						{
							TerEdit_Tx_EmptySlot24(teredit_tx2_panel_bmp);							
						}
					else
						{
							TerEdit_Tx_CopyBmaptoBmap(temp_bmap, teredit_tx2_panel_bmp);							
						}		
				}
			else if (i==2)	
				{		
					if ( str_cmpi( teredit_tx_filenames[i] , "panels\\_empty.tga" ) )
						{
							TerEdit_Tx_EmptySlot24(teredit_tx3_panel_bmp);						
						}
					else
						{
							TerEdit_Tx_CopyBmaptoBmap(temp_bmap, teredit_tx3_panel_bmp);							
						}	
				}
			else if (i==3)	
				{
					if ( str_cmpi( teredit_tx_filenames[i] , "panels\\_empty.tga" ) )
						{
							TerEdit_Tx_EmptySlot24(teredit_tx4_panel_bmp);						
						}
					else
						{
							TerEdit_Tx_CopyBmaptoBmap(temp_bmap, teredit_tx4_panel_bmp);						
						}							
				}
			else if (i==4)	
				{		
					if ( str_cmpi( teredit_tx_filenames[i] , "panels\\_empty.tga" ) )
						{
							TerEdit_Tx_EmptySlot24(teredit_tx5_panel_bmp);						
						}
					else
						{
							TerEdit_Tx_CopyBmaptoBmap(temp_bmap, teredit_tx5_panel_bmp);						
						}
				}			
			wait_for(TerEdit_Tx_EmptySlot24);
			wait_for(TerEdit_Tx_CopyBmaptoBmap);
			wait(1);
			
			bmap_purge(temp_bmap);
			ptr_remove(temp_bmap);
			temp_bmap = NULL;
		}						
}


void		TerEdit_Tx_UpdateSets32()							
{	
	if (teredit_tx_count>=(1+(teredit_tx_set-1)*max_teredit_tx_items)) 
		{		
			TerEdit_Tx_CopySkintoBmap(terrain_entity, 1+(teredit_tx_set-1)*max_teredit_tx_items, teredit_tx1_panel_bmp);
		}
	else
		{
			TerEdit_Tx_EmptySlot32(teredit_tx1_panel_bmp);
		}
	
	if (teredit_tx_count>=(2+(teredit_tx_set-1)*max_teredit_tx_items)) 
		{
			TerEdit_Tx_CopySkintoBmap(terrain_entity, 2+(teredit_tx_set-1)*max_teredit_tx_items, teredit_tx2_panel_bmp);
		}
	else
		{
			TerEdit_Tx_EmptySlot32(teredit_tx2_panel_bmp);
		}
		
	if (teredit_tx_count>=(3+(teredit_tx_set-1)*max_teredit_tx_items))
		{
			TerEdit_Tx_CopySkintoBmap(terrain_entity, 3+(teredit_tx_set-1)*max_teredit_tx_items, teredit_tx3_panel_bmp);
		}
	else
		{
			TerEdit_Tx_EmptySlot32(teredit_tx3_panel_bmp);
		}
		
	if (teredit_tx_count>=(4+(teredit_tx_set-1)*max_teredit_tx_items))
		{
			TerEdit_Tx_CopySkintoBmap(terrain_entity, 4+(teredit_tx_set-1)*max_teredit_tx_items, teredit_tx4_panel_bmp);
		}
	else
		{
			TerEdit_Tx_EmptySlot32(teredit_tx4_panel_bmp);
		}
		
	if (teredit_tx_count>=(5+(teredit_tx_set-1)*max_teredit_tx_items))
		{
			TerEdit_Tx_CopySkintoBmap(terrain_entity, 5+(teredit_tx_set-1)*max_teredit_tx_items, teredit_tx5_panel_bmp);
		}
	else
		{
			TerEdit_Tx_EmptySlot32(teredit_tx5_panel_bmp);
		}
}		


////////////////////////////////////////////


void		TerEdit_Tx_CopyBmaptoBmap(BMAP* from_bmap, BMAP* to_bmap)
{		
	if (!from_bmap)
		{
			return;
//			from_bmap = bmap_create("panels\\_empty.tga");
		}
	
	if (!to_bmap)
		{
			return;
//			to_bmap = bmap_create("panels\\_empty.tga");
		}
	
	bmap_blit(to_bmap, from_bmap, nullvector, vector(bmap_width(to_bmap), bmap_height(to_bmap), 0));
	bmap_to_format(to_bmap, 888);
}


void		TerEdit_Tx_CopySkintoBmap(ENTITY* from_ent, var skin_num, BMAP* to_bmap)
{	
	if (skin_num > teredit_tx_count)	return;			// protection
	
	if (!to_bmap)												// it is created on init and not removed
		{
			return;
//			to_bmap = bmap_create("panels\\_empty.tga");
		}
	
//	bmap_blit(to_bmap,ent_getskin(from_ent, skin_num),nullvector,vector(bmap_width(to_bmap), bmap_height(to_bmap),0));
	bmap_blit(to_bmap, bmap_for_entity(from_ent, skin_num), nullvector, vector(bmap_width(to_bmap), bmap_height(to_bmap), 0));	
	bmap_to_format(to_bmap, 888);
}


void		TerEdit_Tx_EmptySlot32(BMAP* to_bmap)
{
	bmap_to_format(to_bmap, 8888);
	bmap_fill(to_bmap, vector(128,128,128), 0);
}


void		TerEdit_Tx_EmptySlot24(BMAP* to_bmap)
{
	// does not work, shown black -> grey , but okay
	to_bmap = bmap_to_format(to_bmap, 8888);			
	bmap_fill(to_bmap, vector(128,128,128), 0);
	
//	// texture memory increase always
//	bmap_purge(to_bmap);
//	ptr_remove(to_bmap);
//	to_bmap = NULL;
//	wait(1);
//	to_bmap = bmap_create("panels\\_empty.tga");
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Tx_SetColor24()
{
	if (proc_status(TerEdit_Tx_SetColor24) > (var)0) return;
	if (proc_status(TerEdit_Tx_SetTexture24) > (var)0) return;
	
	wait(3);
	
	var temp_size = minv(d3d_texlimit, teredit_tx_size);
		
//	TerEdit_Bmap_AdaptSize(ent_getskin(terrain_entity, 1), temp_size, 24, 888);
//	wait_for(TerEdit_Bmap_AdaptSize);
	ent_setskin(terrain_entity , bmap_to_mipmap(bmap_to_format(bmap_createblack(temp_size, temp_size, 24), 888)), 1 );
	wait(1);	
	
	bmap_fill(ent_getskin(terrain_entity, 1), vector(teredit_tx_blue, teredit_tx_green, teredit_tx_red), 100);
	wait_for(bmap_fill);
	wait(1);	
	
	ent_setskin(terrain_entity , bmap_to_mipmap(bmap_to_format(bmap_for_entity(terrain_entity, 1), 888)), 1 );	// all elements are required ! looks a bit ugly, and slower, but perfect
	wait(1);	
	
	//	bmap_to_mipmap(bmap_for_entity(terrain_entity, 1));		// result is black skin !!!
	//	wait(1);
}


void		TerEdit_Tx_SetTexture24()
{
	if (proc_status(TerEdit_Tx_SetColor24) > (var)0) return;
	if (proc_status(TerEdit_Tx_SetTexture24) > (var)0) return;
	
	wait(3);
	
	var temp_size = minv(d3d_texlimit, teredit_tx_size);
	
//	TerEdit_Bmap_AdaptSize(ent_getskin(terrain_entity, 1), temp_size, 24, 888);
//	wait_for(TerEdit_Bmap_AdaptSize);
	ent_setskin(terrain_entity , bmap_to_mipmap(bmap_to_format(bmap_createblack(temp_size, temp_size, 24), 888)), 1 );
	wait(1);
	
	BMAP* texture_bmap = bmap_create(teredit_tx_filenames[teredit_tx_selected]);
	BMAP* newskin_bmap = ent_getskin(terrain_entity, 1);
	wait(1);		
	
	int xx = integer(temp_size / bmap_width(texture_bmap));
	if (xx<1) xx=1;
	int yy = integer(temp_size / bmap_height(texture_bmap));
	if (yy<1) yy=1;
	
	int j,k;
	for(j=0;j<xx;j++)	
		{
			for(k=0;k<yy;k++)	
				{	
					bmap_blit(newskin_bmap, texture_bmap, vector(j*bmap_width(texture_bmap), k*bmap_height(texture_bmap), 0), NULL);
				}
			
			wait(1);	// protection, because vector() has a limited quantity ! (64)
		}	
	wait(1);
	
	ent_setskin(terrain_entity , bmap_to_mipmap(bmap_to_format(bmap_for_entity(terrain_entity, 1), 888)), 1 );	// all elements are required ! looks a bit ugly, and slower, but perfect	
	wait(1);	
	
//	bmap_to_mipmap(bmap_for_entity(terrain_entity, 1));		// result is black skin !!!
//	wait(1);
	
	bmap_purge(texture_bmap);
	texture_bmap = NULL;
	ptr_remove(texture_bmap);
	
//	bmap_purge(newskin_bmap);	// no! it was only a pointer to a skin bmap
	newskin_bmap = NULL;
	ptr_remove(newskin_bmap);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Tx_SetColor()
{
	wait(3);
	
	if (teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items > teredit_tx_count)	return;			// protection
	
	TerEdit_Tx_Color( bmap_for_entity(terrain_entity , teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items) , vector(teredit_tx_blue,teredit_tx_green,teredit_tx_red) );
	wait_for(TerEdit_Tx_Color);
}


void		TerEdit_Tx_Color(BMAP* bmap_pointer, COLOR* to_color)				
{
	var pixel_format  = bmap_lock(bmap_pointer,0);														// lock and get format
	if (pixel_format >= 565) 
		{
			var pixel;
			COLOR pixel_color;
			var pixel_alpha;
			int i,j;
			for(i=0;i<bmap_width(bmap_pointer);i++)
				{
					for(j=0;j<bmap_height(bmap_pointer);j++)
						{
							pixel = pixel_for_bmap( bmap_pointer , i , j );								// get pixel
							pixel_to_vec( pixel_color , pixel_alpha , pixel_format , pixel );		// get color and alpha
							pixel = pixel_for_vec( to_color , pixel_alpha , pixel_format );		// set alpha
							pixel_to_bmap( bmap_pointer , i , j , pixel );								// set pixel
						}
				}
		}	
	bmap_unlock(bmap_pointer);	
	bmap_to_mipmap(bmap_pointer);	// @@@ update mipmaps after unlocking, to avoid the original skin to appear from big distance		
	wait(1);
	
	TerEdit_Tx_UpdateSets32();
	TerrHmp_Tx_UpdateMaterialSkins();
	wait(1);
}


////////////////////////////////////////////////


void		TerEdit_Tx_SetAlpha()
{
	wait(3);
	
	if (teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items > teredit_tx_count)	return;			// protection
	
	TerEdit_Tx_Alpha( bmap_for_entity(terrain_entity , teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items) , teredit_tx_alpha );
	wait_for(TerEdit_Tx_Alpha);
}


void		TerEdit_Tx_Alpha(BMAP* bmap_pointer, var to_alpha)			
{
	var pixel_format  = bmap_lock(bmap_pointer,0);														// lock and get format
	if (pixel_format >= 565) 
		{
			var pixel;
			COLOR pixel_color;
			var pixel_alpha;
			int i,j;
			for(i=0;i<bmap_width(bmap_pointer);i++)
				{
					for(j=0;j<bmap_height(bmap_pointer);j++)
						{
							pixel = pixel_for_bmap( bmap_pointer , i , j );								// get pixel
							pixel_to_vec( pixel_color , pixel_alpha , pixel_format , pixel );		// get color and alpha
							pixel = pixel_for_vec( pixel_color , to_alpha , pixel_format );		// set alpha
							pixel_to_bmap( bmap_pointer , i , j , pixel );								// set pixel
						}
				}
		}	
	bmap_unlock(bmap_pointer);	
	bmap_to_mipmap(bmap_pointer);																				// @@@ update mipmaps after unlocking, to avoid the original skin to appear from big distance	
	wait(1);
	
	TerEdit_Tx_UpdateSets32();
	TerrHmp_Tx_UpdateMaterialSkins();
	wait(1);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Tx_Draw32Area(var mode )
{
	if (proc_status(TerEdit_Tx_Draw32Area) > 0) return;				// protection
	
	int skin_int = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items;
	
	if (skin_int > teredit_tx_count)		return;							// protection
	if (skin_int == 1)						return;							// protection	
	
	BMAP* temp_skin_bmp = bmap_for_entity(terrain_entity , (var)skin_int);
//	BMAP* temp_skin_bmp = ent_getskin(terrain_entity , (var)skin_int);
	wait(1);	// in A7 it is needed !!! A8 works without it
	
	if (temp_skin_bmp == NULL) return;										// protection
	
	// x,y skin size of terrain
	var skin_x = bmap_width( temp_skin_bmp );	
	var skin_y = bmap_height( temp_skin_bmp );
	
	// upleft and downright tile corner positions
	var upleft_pos_x = teredit_placement_cube.x - integer(teredit_tilesize/2)*GetMapTileSize();
	var upleft_pos_y = teredit_placement_cube.y + integer(teredit_tilesize/2)*GetMapTileSize();
//	var upleft_pos_z = teredit_placement_cube.z; // PosInfo3D1(upleft_pos_x, upleft_pos_y);
	
	var dnright_pos_x = teredit_placement_cube.x + integer(teredit_tilesize/2)*GetMapTileSize();
	var dnright_pos_y = teredit_placement_cube.y - integer(teredit_tilesize/2)*GetMapTileSize();
//	var dnright_pos_z = teredit_placement_cube.z; // PosInfo3D1(dnright_pos_x, dnright_pos_y);
	
	// corner pixels closest to corner tile middles
	// by using level.c A8
//	VECTOR upleft_pixel_vec  = TerEdit_PixelFromPos(terrain_entity,upleft_pos_x,upleft_pos_y,temp_skin_bmp);
//	VECTOR dnright_pixel_vec = TerEdit_PixelFromPos(terrain_entity,dnright_pos_x,dnright_pos_y,temp_skin_bmp);
	
	VECTOR upleft_pixel_vec;
	// pixel pos x = (pos - terrain half) / (terrain size/skin size)
	upleft_pixel_vec.x  = ( upleft_pos_x - terrain_entity.min_x )/( (terrain_entity.max_x-terrain_entity.min_x) / skin_x ) ;	
	// pixel pos x = (terrain half - pos) / (terrain size/skin size)
	upleft_pixel_vec.y  = ( terrain_entity.max_y - upleft_pos_y )/( (terrain_entity.max_y-terrain_entity.min_y) / skin_y ) ;
	upleft_pixel_vec.z  = 0 ;
	
	VECTOR dnright_pixel_vec;
	// pixel pos x = (pos - terrain half) / (terrain size/skin size)
	dnright_pixel_vec.x  = ( dnright_pos_x - terrain_entity.min_x )/( (terrain_entity.max_x-terrain_entity.min_x) / skin_x ) ;	
	// pixel pos x = (terrain half - pos) / (terrain size/skin size)
	dnright_pixel_vec.y  = ( terrain_entity.max_y - dnright_pos_y )/( (terrain_entity.max_y-terrain_entity.min_y) / skin_y ) ;
	dnright_pixel_vec.z  = 0 ;
		
	// pixels in a row & column
	var widht_pixel  = abs( dnright_pixel_vec.x - upleft_pixel_vec.x );
	var height_pixel = abs( upleft_pixel_vec.y - dnright_pixel_vec.y );
	
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
	
	if (temp_brush_bmp == NULL) return;		// protection
	
	// distance between skin pixels on brush image
	var step_bmp_x = bmap_width( temp_brush_bmp ) / widht_pixel ;
	var step_bmp_y = bmap_height( temp_brush_bmp ) / height_pixel ;
	
	// read brush bitmap
	var 	brush_format, 			skin_format; 
	var 	brush_pixel, 			skin_pixel;
	COLOR brush_pixel_color, 	skin_pixel_color;
	var 	brush_pixel_alpha, 	skin_pixel_alpha;
	
	brush_format = bmap_lock(temp_brush_bmp,0);
	skin_format  = bmap_lock(temp_skin_bmp,0);
	
//	if (brush_format<565) bmap_to_format(temp_brush_bmp,888);		
//	if (skin_format<565) bmap_to_format(skin_format,8888);
	
	int i,j,k;
	var m;
	for (i=0;i<widht_pixel;i++)
		{
			for (j=0;j<height_pixel;j++)
				{
					// protection
					if ( (step_bmp_x*i>bmap_width(temp_brush_bmp)) || (step_bmp_y*j>bmap_height(temp_brush_bmp)) )
						{
							printf("out of brush bmap error");
							break;
						}
					
					// read brush color
					brush_pixel = pixel_for_bmap( temp_brush_bmp , step_bmp_x*i , step_bmp_y*j );						// get brush pixel
					pixel_to_vec( brush_pixel_color , brush_pixel_alpha , brush_format , brush_pixel);				// get brush color and alpha
					
					// read skin color
					skin_pixel = pixel_for_bmap( temp_skin_bmp , upleft_pixel_vec.x+i , upleft_pixel_vec.y+j );	// get skin pixel
					pixel_to_vec( skin_pixel_color , skin_pixel_alpha , skin_format , skin_pixel);					// get skin color and alpha
					
					// write skin alpha 
					if (mode==(var)1)
						{
							// draw - blending with original skin - multiply with opacity percentage
							skin_pixel = pixel_for_vec( skin_pixel_color , maxv(brush_pixel_color.blue/255 *teredit_tx_opacity,skin_pixel_alpha) , skin_format );// set skin alpha to brush color
						}
					else
						{
							// delete - reverse of drawing
							skin_pixel = pixel_for_vec( skin_pixel_color , minv((100-brush_pixel_color.blue/255 *teredit_tx_opacity),skin_pixel_alpha) , skin_format );// set skin alpha to brush color
						}
					
					pixel_to_bmap( temp_skin_bmp , upleft_pixel_vec.x+i , upleft_pixel_vec.y+j , skin_pixel );	// set skin pixel
				}
				
		}
//	wait(1);			// in A7 it is okay without wait
	bmap_unlock(temp_brush_bmp);
	bmap_unlock(temp_skin_bmp);
	bmap_to_mipmap(temp_skin_bmp);	// @@@ not essential!
	wait(1);
	
	temp_brush_bmp = NULL;
	ptr_remove(temp_brush_bmp);
	
	temp_skin_bmp = NULL;
	ptr_remove(temp_skin_bmp);
}


void		TerEdit_Tx_Draw24Area(var mode )
{
	if (proc_status(TerEdit_Tx_Draw24Area) > 0) return;		// protection
	
	if ( str_cmpi( teredit_tx_filenames[teredit_tx_selected] , "panels\\_empty.tga" ) ) return;	// protection	
	
	// skin1 texture
//	BMAP* temp_skin_bmp = ent_getskin(terrain_entity , 1);
	BMAP* temp_skin_bmp = bmap_for_entity(terrain_entity , 1);
	
	wait(1);	// in A7 it is needed !!! A8 works without it
	
	if (temp_skin_bmp == NULL) return;		// protection
	
	// x,y skin size of terrain
	var skin_x = bmap_width( temp_skin_bmp );	
	var skin_y = bmap_height( temp_skin_bmp );
	
	// upleft and downright tile corner positions
	var upleft_pos_x = teredit_placement_cube.x - integer(teredit_tilesize/2)*GetMapTileSize();
	var upleft_pos_y = teredit_placement_cube.y + integer(teredit_tilesize/2)*GetMapTileSize();
//	var upleft_pos_z = teredit_placement_cube.z; // PosInfo3D1(upleft_pos_x, upleft_pos_y);
	
	var dnright_pos_x = teredit_placement_cube.x + integer(teredit_tilesize/2)*GetMapTileSize();
	var dnright_pos_y = teredit_placement_cube.y - integer(teredit_tilesize/2)*GetMapTileSize();
//	var dnright_pos_z = teredit_placement_cube.z; // PosInfo3D1(dnright_pos_x, dnright_pos_y);
	
	// corner pixels closest to corner tile middles
	// by using level.c A8
//	VECTOR upleft_pixel_vec  = TerEdit_PixelFromPos(terrain_entity,upleft_pos_x,upleft_pos_y,temp_skin_bmp);
//	VECTOR dnright_pixel_vec = TerEdit_PixelFromPos(terrain_entity,dnright_pos_x,dnright_pos_y,temp_skin_bmp);
	
	VECTOR upleft_pixel_vec;
	// pixel pos x = (pos + terrain half) / (terrain size/skin size)
	upleft_pixel_vec.x  = ( upleft_pos_x - terrain_entity.min_x )/( (terrain_entity.max_x-terrain_entity.min_x) / skin_x ) ;	
	// pixel pos x = (terrain half - pos) / (terrain size/skin size)
	upleft_pixel_vec.y  = ( terrain_entity.max_y - upleft_pos_y )/( (terrain_entity.max_y-terrain_entity.min_y) / skin_y ) ;
	upleft_pixel_vec.z  = 0 ;
	
	VECTOR dnright_pixel_vec;
	// pixel pos x = (pos + terrain half) / (terrain size/skin size)
	dnright_pixel_vec.x  = ( dnright_pos_x - terrain_entity.min_x )/( (terrain_entity.max_x-terrain_entity.min_x) / skin_x ) ;	
	// pixel pos x = (terrain half - pos) / (terrain size/skin size)
	dnright_pixel_vec.y  = ( terrain_entity.max_y - dnright_pos_y )/( (terrain_entity.max_y-terrain_entity.min_y) / skin_y ) ;
	dnright_pixel_vec.z  = 0 ;
		
	// pixels in a row & column
	var widht_pixel  = abs( dnright_pixel_vec.x - upleft_pixel_vec.x );
	var height_pixel = abs( upleft_pixel_vec.y - dnright_pixel_vec.y );
	
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
	
	// actual texture image		
//	BMAP* temp_tx_bmp = bmap_create( teredit_tx_filenames[teredit_tx_selected] );	
	BMAP* temp_tx_bmp;
	if (teredit_tx_selected==(var)0)
		{
			temp_tx_bmp = teredit_tx1_panel_bmp;
		}
	else if (teredit_tx_selected==(var)1)
		{
			temp_tx_bmp = teredit_tx2_panel_bmp;
		}
	else if (teredit_tx_selected==(var)2)
		{
			temp_tx_bmp = teredit_tx3_panel_bmp;
		}	
	else if (teredit_tx_selected==(var)3)
		{
			temp_tx_bmp = teredit_tx4_panel_bmp;
		}
	else if (teredit_tx_selected==(var)4)
		{
			temp_tx_bmp = teredit_tx5_panel_bmp;
		}
	
	if (temp_brush_bmp == NULL) return;		// protection
	if (temp_tx_bmp == NULL) return;		// protection
	
	// distance between skin pixels on brush image
	var step_bmp_x = bmap_width( temp_brush_bmp ) / widht_pixel ;
	var step_bmp_y = bmap_height( temp_brush_bmp ) / height_pixel ;
	
	// distance between skin pixels on texture image
	var step_tx_bmp_x = bmap_width( temp_tx_bmp ) / widht_pixel ;
	var step_tx_bmp_y = bmap_height( temp_tx_bmp ) / height_pixel ;
	
	// read brush bitmap
	var 	brush_format, 			skin_format,			texture_format; 
	var 	brush_pixel, 			skin_pixel,				texture_pixel;
	COLOR brush_pixel_color, 	skin_pixel_color,		texture_pixel_color,		temp_pixel_color;
	var 	brush_pixel_alpha, 	skin_pixel_alpha,		texture_pixel_alpha;
	
	brush_format 	= bmap_lock(temp_brush_bmp,0);
	skin_format  	= bmap_lock(temp_skin_bmp,0);
	texture_format = bmap_lock(temp_tx_bmp,0);
	
//	if (brush_format<565) 	bmap_to_format(temp_brush_bmp,888);		
//	if (skin_format<565) 	bmap_to_format(temp_skin_bmp,888);
//	if (texture_format<565) bmap_to_format(temp_tx_bmp,888);
	
	int i,j,k;
	var m;
	for (i=0;i<widht_pixel;i++)
		{
			for (j=0;j<height_pixel;j++)
				{
					// protection
					if ( (step_bmp_x*i>bmap_width(temp_brush_bmp)) || (step_bmp_y*j>bmap_height(temp_brush_bmp)) )
						{
							printf("out of brush bmap error");
							return;
						}
					
					// read brush color
					brush_pixel = pixel_for_bmap( temp_brush_bmp , step_bmp_x*i , step_bmp_y*j );						// get brush pixel
					pixel_to_vec( brush_pixel_color , brush_pixel_alpha , brush_format , brush_pixel);				// get brush color and alpha
					
					// read skin color
					skin_pixel = pixel_for_bmap( temp_skin_bmp , upleft_pixel_vec.x+i , upleft_pixel_vec.y+j );	// get skin pixel
					pixel_to_vec( skin_pixel_color , skin_pixel_alpha , skin_format , skin_pixel);					// get skin color and alpha
					
					// read texture color - should be tiled onto skin bitmap
					texture_pixel = pixel_for_bmap( temp_tx_bmp , (upleft_pixel_vec.x+i)%bmap_width( temp_tx_bmp ) , (upleft_pixel_vec.y+j)%bmap_height( temp_tx_bmp ) );				// get texture pixel tiled onto skin
					pixel_to_vec( texture_pixel_color , texture_pixel_alpha , texture_format , texture_pixel);	// get texture color and alpha
										
					// write texture to skin, blending by alpha*opacity 
					if (mode==(var)1)
						{
							// setting RGB values 
							temp_pixel_color.blue =  skin_pixel_color.blue	*(100-brush_pixel_color.blue/255*teredit_tx_opacity)/100 	+ 	texture_pixel_color.blue	*(brush_pixel_color.blue/255*teredit_tx_opacity)/100;						
							temp_pixel_color.green = skin_pixel_color.green *(100-brush_pixel_color.blue/255*teredit_tx_opacity)/100 	+ 	texture_pixel_color.green	*(brush_pixel_color.blue/255*teredit_tx_opacity)/100;
							temp_pixel_color.red =   skin_pixel_color.red	*(100-brush_pixel_color.blue/255*teredit_tx_opacity)/100 	+ 	texture_pixel_color.red		*(brush_pixel_color.blue/255*teredit_tx_opacity)/100;												
							// setting new pixel corol values
							skin_pixel = pixel_for_vec( temp_pixel_color , 100 , skin_format );	
						}
					else
						{
							// setting RGB values 
							temp_pixel_color.blue =  skin_pixel_color.blue	*(brush_pixel_color.blue/255*teredit_tx_opacity)/100 	+ 	texture_pixel_color.blue	*(100-brush_pixel_color.blue/255*teredit_tx_opacity)/100;						
							temp_pixel_color.green = skin_pixel_color.green *(brush_pixel_color.blue/255*teredit_tx_opacity)/100 	+ 	texture_pixel_color.green	*(100-brush_pixel_color.blue/255*teredit_tx_opacity)/100;
							temp_pixel_color.red =   skin_pixel_color.red	*(brush_pixel_color.blue/255*teredit_tx_opacity)/100 	+ 	texture_pixel_color.red		*(100-brush_pixel_color.blue/255*teredit_tx_opacity)/100;												
							// setting new pixel corol values
							skin_pixel = pixel_for_vec( temp_pixel_color , 100 , skin_format );								
						}
					
					pixel_to_bmap( temp_skin_bmp , upleft_pixel_vec.x+i , upleft_pixel_vec.y+j , skin_pixel );	// set skin pixel
				}
				
		}
//	wait(1);			// in A7 it is okay without wait
	bmap_unlock(temp_brush_bmp);
	bmap_unlock(temp_skin_bmp);
	bmap_unlock(temp_tx_bmp);
//	bmap_to_mipmap(temp_skin_bmp);	// no! it causes black texture if color or texture fill was applied previously, and mipmaps are okay without it !	
	wait(1);
	
//	temp_brush_bmp = NULL;
//	ptr_remove(temp_brush_bmp);
//	temp_skin_bmp = NULL;
//	ptr_remove(temp_skin_bmp);
//	temp_tx_bmp = NULL;
//	ptr_remove(temp_tx_bmp);
//	wait(1);
	
	ent_setskin( terrain_entity , bmap_to_mipmap(bmap_to_format(bmap_for_entity(terrain_entity, 1), 888)) , 1 );	// all elements are required ! looks a bit ugly, and slower, but perfect
//	bmap_to_mipmap(ent_getskin(terrain_entity , 1));	// needed to see changed skin from far distance view			// nearly okay, requires 2 modifications to set mipmaps properly
	wait(1);
	
	temp_skin_bmp = NULL;
	ptr_remove(temp_skin_bmp);
	
	temp_tx_bmp = NULL;
	ptr_remove(temp_tx_bmp);
	
	temp_brush_bmp = NULL;
	ptr_remove(temp_brush_bmp);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Tx_MoveSkinDown()
{
	wait(3);
	if (teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items < 2)	return;					// protection	
	
	var skin_num = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items ;					// selected skin	
	
	BMAP* temp_bmap = bmap_for_entity(terrain_entity,skin_num);												// store selected
	wait(1);	
	ent_setskin( terrain_entity , bmap_for_entity(terrain_entity,skin_num-1) , skin_num );			// get next below, and set to selected
	wait(1);			
	ent_setskin( terrain_entity , temp_bmap , skin_num-1 );													// set stored selected to next above
	wait(1);		
	
	TerEdit_Tx_UpdateSets32();	
	TerrHmp_Tx_UpdateMaterialSkins();
	wait(1);
	
	temp_bmap = NULL;
	ptr_remove(temp_bmap);
}


void		TerEdit_Tx_MoveSkinUp()
{
	wait(3);
	if (teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items > teredit_tx_count-1)	return;			// protection	
	
	var skin_num = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items ;									// selected skin	
	
	BMAP* temp_bmap = bmap_for_entity(terrain_entity,skin_num);																// store selected
	wait(1);	
	ent_setskin( terrain_entity , bmap_for_entity(terrain_entity,skin_num+1) , skin_num );							// get next above, and set to selected
	wait(1);			
	ent_setskin( terrain_entity , temp_bmap , skin_num+1 );																	// set stored selected to next above
	wait(1);		
	
	TerEdit_Tx_UpdateSets32();	
	TerrHmp_Tx_UpdateMaterialSkins();
	wait(1);
	
	temp_bmap = NULL;
	ptr_remove(temp_bmap);
}


//////////////////////////////////////////////////


void	 	TerEdit_Tx_DelSkin32()
{
	//----------------------------------------
	wait(3);
	if (teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items > teredit_tx_count)	return;			// protection	teredit_tx_count
	
	//----------------------------------------
	// copy skins
	var skin_num = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items ;								// selected skin	
	var i = skin_num;																													// loop starts from deleted skin	
	
	while(i < teredit_tx_count)																									// = ent_status(terrain_entity,8)
		{
			ent_setskin( terrain_entity , bmap_for_entity(terrain_entity,i+1) , i );					
			i++;
			wait(1);		
		}		
	
	//----------------------------------------
	// put a small transparent black skin to deleted skin because deletion works but later new skin BMAP* points to each other...
	BMAP* temp_bmap = bmap_createblack(512,512,32);
	ent_setskin( terrain_entity , temp_bmap , i );
	wait(1);
	
	//----------------------------------------
	teredit_tx_count = ent_status(terrain_entity,8);
	max_teredit_tx_sets = integer(teredit_tx_count/max_teredit_tx_items) +1;	
	wait(1);
	
	//----------------------------------------
	TerEdit_Tx_UpdateSets32();
	TerrHmp_Tx_UpdateMaterialSkins();
	wait(1);
	
	//----------------------------------------
//	temp_bmap = NULL;
//	ptr_remove(temp_bmap);
	//----------------------------------------
}


void	 	TerEdit_Tx_AddSkin32()
{
	//----------------------------------------
	wait(3);
	if (teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items > teredit_tx_count+1)	return;			// protection	teredit_tx_count+1
	
	//----------------------------------------
	// put a small transparent black skin to last skin 
	BMAP* temp1_bmap = bmap_createblack(512,512,32);
	ent_setskin( terrain_entity , temp1_bmap , teredit_tx_count+1 );	
	wait(1);
	
	//----------------------------------------
	// copy skins
	var skin_num = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items ;	// selected skin place
	var i = teredit_tx_count+1;																		// loop starts from last-1 skin, but there is one more skin now
	
	while(i > skin_num)																					// over selected skin
		{										
			ent_setskin( terrain_entity , bmap_for_entity(terrain_entity,i-1) , i );					
			i--;
			wait(1);
		}	
	
	//----------------------------------------	
	// put a small transparent black skin to selected skin
	BMAP* temp2_bmap = bmap_createblack(512,512,32);
	ent_setskin( terrain_entity , temp2_bmap , i );
	bmap_to_mipmap(temp2_bmap);																		// to ensure successful loading on change skin
	wait_for(bmap_to_mipmap);
	wait(1);
	
	//----------------------------------------
	teredit_tx_count = ent_status(terrain_entity,8);
	max_teredit_tx_sets = integer(teredit_tx_count/max_teredit_tx_items) +1;
	wait(1);
	
	//----------------------------------------
	TerEdit_Tx_UpdateSets32();
	TerrHmp_Tx_UpdateMaterialSkins();
	wait(1);
	
	//----------------------------------------
	temp1_bmap = NULL;
	ptr_remove(temp1_bmap);
	
	temp2_bmap = NULL;
	ptr_remove(temp2_bmap);
	//----------------------------------------
}


//////////////////////////////////////////////////


void	 	TerEdit_Tx_SetSkin32()
{
	wait(3);
	if (teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items > teredit_tx_count)	return;			// protection	+1 clod be allowed
	
	FileMenu_SetSkin32();	
}


void		TerEdit_LoadSkin32_tga(char* file_name)																			// file name with path
{
	//-------------------------------------------
	if (proc_status(TerEdit_LoadSkin32_tga)>1)		return;
	//-------------------------------------------
	set( map_loadpanel , SHOW); 
	set( map_loadtext1 , SHOW);
	wait(3);
	//-------------------------------------------	
	var skin_num = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items ;								// selected skin	place
	
	STRING* temp_str = str_create(file_name);
	
	TerrHmp_Tx_LoadSkin_KeepAlpha(temp_str, skin_num);
	wait_for(TerrHmp_Tx_LoadSkin_KeepAlpha);
	//-------------------------------------------
	wait(1);	
	TerEdit_Tx_UpdateSets32();
	TerrHmp_Tx_UpdateMaterialSkins();
	//-------------------------------------------
	wait(1);
	reset( map_loadpanel , SHOW); 
	reset( map_loadtext1 , SHOW);
	//-------------------------------------------
}


/////////////////////////////////////////////////////////////////////////////////////////////


void	 	TerEdit_Save_Textures()											// calls filehandling script
{
	wait(3);
	FileMenu_SaveSkins();
}


void	 	TerEdit_Load_Textures()											// calls filehandling script
{
	wait(3);
	FileMenu_LoadSkins();
}


//// !!! not for MB internal usage, include it only for exported scripts, as it is not compatible with TerrHmp_Tx_LoadSkin_KeepAlpha
//void		TerEdit_LoadSkin32_ToSkin_tga(char* file_name, var skin_num)
//{
//	if (!file_exists_onepath(file_name))	return;
//	
//	var bit;
//	if (skin_num<2)
//		{
//			bit = 24;
//		}
//	else
//		{
//			bit = 32;
//		}
//	
//	BMAP* temp_bmap = bmap_createblack(64,64,bit);
//	wait(1);
//	
//	bmap_load(temp_bmap,file_name,1);
//	wait_for(bmap_load);
//	wait(1);
//	
//	var temp_size_x = bmap_width(temp_bmap);
//	var temp_size_y = bmap_height(temp_bmap);
//	
//	BMAP* newskin_bmap = bmap_createblack(temp_size_x, temp_size_y, bit);
//	wait(1);
//	
////	temp_size = minv(d3d_texlimit,temp_size);
//	bmap_blit( newskin_bmap , temp_bmap , nullvector , nullvector );
//	wait_for(bmap_blit);
//	wait(1);
//	
//	ent_setskin(terrain_entity, newskin_bmap, skin_num);
//////	bmap_to_mipmap( ent_setskin(terrain_entity,newskin_bmap,skin_num) );		// skin diappears !!!
//	wait(1);
//	
//	// free temp bmap
//	bmap_purge(temp_bmap);
//	temp_bmap = NULL;
//	ptr_remove(temp_bmap);
//	
//	newskin_bmap = NULL;
//	ptr_remove(newskin_bmap);
//}


//////////////////////////////////////////////////////////


void	 	TerEdit_Load_Texture24()											// calls filehandling script
{
	wait(3);
	FileMenu_LoadSkin1();
}


void	 	TerEdit_Save_Texture24()																									// calls filehandling script
{
	wait(3);
	FileMenu_SaveSkin1();
}


void	 	TerEdit_Load_DetailTexture24()									// calls filehandling script
{
	wait(3);
	FileMenu_LoadDetailSkin2();
}


void	 	TerEdit_Save_DetailTexture24()									// calls filehandling script
{
	wait(3);
	FileMenu_SaveDetailSkin2();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		TerEdit_Tx_Store()
{
	// protections
	if (proc_status(TerEdit_Tx_Undo) > (var)0) return;
	if (proc_status(TerEdit_Tx_Redo) > (var)0) return;
	if (proc_status(TerEdit_Tx_Store) > (var)0) return;
	if (proc_status(TerEdit_Tx_Reset) > (var)0) return;
	if (proc_status(TerEdit_Tx_Restore) > (var)0) return;
	
	wait(3);		
	
	// select skin to be manipulated	
	int skin_int;
	if (ent_status(terrain_entity,8)<3)
		{
			skin_int = 1;
		}
	else
		{
			skin_int = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items ;
		}
	//---------------------------------------------------------------------
	STRING* temp_str = str_create("");
	str_cpy(temp_str,map_foldername);
	str_cat(temp_str,"undo");	
	str_cat(temp_str,"_skin");
	str_cat(temp_str,str_for_int(NULL,skin_int));
	str_cat(temp_str,".tga");
	//---------------------------------------------------------------------	
	if (skin_int>1)
		{
//			BMAP* temp_bmap = bmap_for_entity(terrain_entity,skin_int);
//			bmap_save(temp_bmap,temp_str);
			bmap_save(bmap_for_entity(terrain_entity, skin_int), temp_str);
			wait_for(bmap_save);					
//			wait(1);
//			// free temp bmap
//			temp_bmap = NULL;
//			ptr_remove(temp_bmap);								
		}
	else
		{							
			TerrHmp_Save_Skin24_tga(temp_str);
			wait_for(TerrHmp_Save_Skin24_tga);			
		}	
	//---------------------------------------------------------------------
	wait(3);
	ptr_remove(temp_str);	
}


void		TerEdit_Tx_Undo()
{
	// protections
	if (proc_status(TerEdit_Tx_Undo) > (var)0) return;
	if (proc_status(TerEdit_Tx_Redo) > (var)0) return;
	if (proc_status(TerEdit_Tx_Store) > (var)0) return;
	if (proc_status(TerEdit_Tx_Reset) > (var)0) return;
	if (proc_status(TerEdit_Tx_Restore) > (var)0) return;
	
	wait(3);
	
	// select skin to be manipulated	
	int skin_int;
	if (ent_status(terrain_entity,8)<3)
		{
			skin_int = 1;
		}
	else
		{
			skin_int = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items ;
		}
	//---------------------------------------------------------------------
	// redo bmap save
	STRING* temp_str = str_create("");
	str_cpy(temp_str,map_foldername);
	str_cat(temp_str,"redo");	
	str_cat(temp_str,"_skin");
	str_cat(temp_str,str_for_int(NULL,skin_int));
	str_cat(temp_str,".tga");	
	//---------------------------------------------------------------------	
	if (skin_int>1)
		{
//			BMAP* temp_bmap = bmap_for_entity(terrain_entity,skin_int);
//			bmap_save(temp_bmap,temp_str);
			bmap_save(bmap_for_entity(terrain_entity, skin_int), temp_str);
			wait_for(bmap_save);					
//			wait(1);
//			// free temp bmap
//			temp_bmap = NULL;
//			ptr_remove(temp_bmap);								
		}
	else
		{							
			TerrHmp_Save_Skin24_tga(temp_str);
			wait_for(TerrHmp_Save_Skin24_tga);			
		}	
	//---------------------------------------------------------------------
	wait(1);
	
	// undo bmap load
//	STRING* temp_str = str_create("");
	str_cpy(temp_str,map_foldername);
	str_cat(temp_str,"undo");
	str_cat(temp_str,"_skin");
	str_cat(temp_str,str_for_int(NULL,skin_int));
	str_cat(temp_str,".tga");
	//---------------------------------------------------------------------	
	// back to temp if no undo was saved previously
	if (!file_exists_onepath(temp_str))
		{
//			TerEdit_Tx_Reset();			
		}
	else
		{
			if (skin_int>1)			
				{
					TerrHmp_Tx_LoadSkin(temp_str, skin_int);
					wait_for(TerrHmp_Tx_LoadSkin);
					
					TerEdit_Tx_UpdateSets32();
					TerrHmp_Tx_UpdateMaterialSkins();
				}
			else
				{
					TerrHmp_Load_Skin24_tga(temp_str);
					wait_for(TerrHmp_Load_Skin24_tga);
				}
		}
	//---------------------------------------------------------------------
	wait(3);
	ptr_remove(temp_str);
}


void		TerEdit_Tx_Redo()
{
	// protections
	if (proc_status(TerEdit_Tx_Undo) > (var)0) return;
	if (proc_status(TerEdit_Tx_Redo) > (var)0) return;
	if (proc_status(TerEdit_Tx_Store) > (var)0) return;
	if (proc_status(TerEdit_Tx_Reset) > (var)0) return;
	if (proc_status(TerEdit_Tx_Restore) > (var)0) return;
	
	wait(3);
	
	// select skin to be manipulated	
	int skin_int;
	if (ent_status(terrain_entity,8)<3)
		{
			skin_int = 1;
		}
	else
		{
			skin_int = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items ;
		}
	//-----------------------------
	// redo bmap load
	STRING* temp_str = str_create("");
	str_cpy(temp_str,map_foldername);
	str_cat(temp_str,"redo");
	str_cat(temp_str,"_skin");
	str_cat(temp_str,str_for_int(NULL,skin_int));
	str_cat(temp_str,".tga");
	//---------------------------------------------------------------------	
	if (file_exists_onepath(temp_str))
		{
			if (skin_int>1)
				{
					TerrHmp_Tx_LoadSkin(temp_str, skin_int);
					wait_for(TerrHmp_Tx_LoadSkin);
					
					TerEdit_Tx_UpdateSets32();
					TerrHmp_Tx_UpdateMaterialSkins();
				}
			else
				{
					TerrHmp_Load_Skin24_tga(temp_str);
					wait_for(TerrHmp_Load_Skin24_tga);
				}
		}
	//---------------------------------------------------------------------
	wait(3);
	ptr_remove(temp_str);
}


void		TerEdit_Tx_Reset()
{
	// protections
	if (proc_status(TerEdit_Tx_Undo) > (var)0) return;
	if (proc_status(TerEdit_Tx_Redo) > (var)0) return;
	if (proc_status(TerEdit_Tx_Store) > (var)0) return;
	if (proc_status(TerEdit_Tx_Reset) > (var)0) return;
	if (proc_status(TerEdit_Tx_Restore) > (var)0) return;
	
	wait(3);
	
	// select skin to be manipulated	
	int skin_int;
	if (ent_status(terrain_entity,8)<3)
		{
			skin_int = 1;
		}
	else
		{
			skin_int = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items ;
		}
	//-----------------------------
	// temp bmap load
	STRING* temp_str = str_create("");
	str_cpy(temp_str,map_foldername);
	str_cat(temp_str,"temp");
	str_cat(temp_str,"_skin");
	str_cat(temp_str,str_for_int(NULL,skin_int));
	str_cat(temp_str,".tga");
	//---------------------------------------------------------------------	
	if (file_exists_onepath(temp_str))
		{
			if (skin_int>1)
				{
					TerrHmp_Tx_LoadSkin(temp_str, skin_int);
					wait_for(TerrHmp_Tx_LoadSkin);
					
					TerEdit_Tx_UpdateSets32();
					TerrHmp_Tx_UpdateMaterialSkins();
				}
			else
				{
					TerrHmp_Load_Skin24_tga(temp_str);
					wait_for(TerrHmp_Load_Skin24_tga);
				}
		}
	//---------------------------------------------------------------------
	wait(3);
	ptr_remove(temp_str);
}


void		TerEdit_Tx_Restore()
{
	// protections
	if (proc_status(TerEdit_Tx_Undo) > (var)0) return;
	if (proc_status(TerEdit_Tx_Redo) > (var)0) return;
	if (proc_status(TerEdit_Tx_Store) > (var)0) return;
	if (proc_status(TerEdit_Tx_Reset) > (var)0) return;
	if (proc_status(TerEdit_Tx_Restore) > (var)0) return;
	
	wait(3);
	
	// select skin to be manipulated	
	int skin_int;
	if (ent_status(terrain_entity,8)<3)
		{
			skin_int = 1;
		}
	else
		{
			skin_int = teredit_tx_selected+1+(teredit_tx_set-1)*max_teredit_tx_items ;
		}
	//-----------------------------
	STRING* temp_str = str_create("");
	STRING* temp2_str = str_create("");
	// reset to original hmp			
	str_cpy(temp_str,hmp_foldername);					// set folder name
	str_for_entfile(temp2_str,terrain_entity);		// get terrain entity name
	str_cat(temp_str,temp2_str);							// attach file name to folder name
	str_trunc(temp_str,4);									// cut extension
	str_cat(temp_str,"_skin");								// attach _skin					
	str_for_int(temp2_str , skin_int);					// get skin number								
	str_cat(temp_str,temp2_str);							// attach skin number
	str_cat(temp_str,".tga");								// attach proper extension		
	
	//---------------------------------------------------------------------	
	if (file_exists_onepath(temp_str))
		{
			if (skin_int>1)
				{
					TerrHmp_Tx_LoadSkin(temp_str, skin_int);
					wait_for(TerrHmp_Tx_LoadSkin);
					
					TerEdit_Tx_UpdateSets32();
					TerrHmp_Tx_UpdateMaterialSkins();
				}
			else
				{
					TerrHmp_Load_Skin24_tga(temp_str);
					wait_for(TerrHmp_Load_Skin24_tga);
				}
		}
	//---------------------------------------------------------------------
	wait(3);
	ptr_remove(temp_str);
	ptr_remove(temp2_str);	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// helpers


void		TerEdit_Bmap_AdaptSize(BMAP* temp_bmap, var temp_size, var bits, var format)
{
	if (temp_bmap) 
		{
	   	if (bmap_width(temp_bmap) != temp_size || bmap_height(temp_bmap) != temp_size)
		   {
//		      var format = bmap_format(temp_bmap);
		      bmap_purge(temp_bmap);
		      ptr_remove(temp_bmap);
		      temp_bmap = NULL;
		      wait(1);
		      temp_bmap = bmap_to_mipmap(bmap_to_format(bmap_createblack(temp_size, temp_size, bits), format));
		      wait(1);  
//		      bmap_to_mipmap(bmap_fill(temp_bmap, COLOR_WHITE, 100));
//		      wait(1); 
		      
		      if (temp_bmap) 								// if successful
		      	{
		      		teredit_tx_size = temp_size;		// refresh to new size
	      		}
		   }   
		}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////






#endif