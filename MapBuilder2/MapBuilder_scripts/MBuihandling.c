//////////////////////////////////////
// user interface related functions 
// temp key hit function calls
// main key checking loop
// mouse clicking functions
/////////////////////////////////////

#ifndef MBuihandling_c
#define MBuihandling_c

////////////////////////////////////////
// code

void UI_Init()
{
	//----------------------------------------------------
	
	if (proc_status(UI_Init)>0)
		{
//			printf("Double Call : UI_Init");
			return;
		}
	
	//----------------------------------------------------
{}
#ifndef MB_RELEASE	
	set( cluster_data_panel , SHOW );		
	set( tile_data_panel , SHOW );
#endif
	
	set( ui_mainmenu_panel , SHOW );
	set( ui_helptext , SHOW );
	
	//----------------------------------------------------
	
#ifndef MB_RELEASE	
	// set panel font colors
	tile_data_panel.red 			= panel_font_r;
	tile_data_panel.green		= panel_font_g;
	tile_data_panel.blue			= panel_font_b;
	
	cluster_data_panel.red 		= panel_font_r;
	cluster_data_panel.green	= panel_font_g;
	cluster_data_panel.blue		= panel_font_b;
#endif
	
	// not needed data
//	map_data_panel.red 			= panel_font_r;
//	map_data_panel.green			= panel_font_g;
//	map_data_panel.blue			= panel_font_b;

	// not needed to set it to panel text color		
//	ui_helptext.red 				= panel_font_r;
//	ui_helptext.green				= panel_font_g;
//	ui_helptext.blue				= panel_font_b;
//
//	ui_helptext_place1.red 		= panel_font_r;
//	ui_helptext_place1.green	= panel_font_g;
//	ui_helptext_place1.blue		= panel_font_b;
//	
//	ui_helptext_place2.red 		= panel_font_r;
//	ui_helptext_place2.green	= panel_font_g;
//	ui_helptext_place2.blue		= panel_font_b;
//	
//	ui_helptext_teredit.red 	= panel_font_r;
//	ui_helptext_teredit.green	= panel_font_g;
//	ui_helptext_teredit.blue	= panel_font_b;
	
	//----------------------------------------------------
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// these should be used more widely:
	//
	//mouse_ent			mouse is over an entity (but not very practical because of coverage in case of densly placed entities)		- ok
	//mouse_range		event_click/touch max range, default: 2000																						- ok
	//mouse_panel		usage: if (mouse_left && mouse_panel)																								- ok
	//
	//mouse_pos3d		depends on range? (usually a specialized c_trace is better to filter out unwanted entities)
	//mouse_dir3d		with length 1
	//
	//mouse_map			change pointer image due to functionality																							- ok
	//mouse_spot		set active point of the mouse image																									- ok
	//
	//mouse_moving		mouse moved in last 0.25sec
	//mouse_calm		treshold of moving state in pixels, default 3
	//mouse_time		frames for determining mouse moving state, default 4
	//
	//mickey.x.y.z		x,y: mouse movement in screen pixels in last frame, z wheel movement														- ok
	//
	//mouse_mode		0,1,2,4	default 0, used 1																												- ok
	//mose_force		could be used in player01 test mode camera and character rotation control
	//
	//enable_mouse		0 disable mouse clicks, 1 default enable mouse, 2 panels handles right clicks as left clicks						- ok
	//enable_key		0 disable, 1 enable keyboard																											- ok
	//enable_joystick 0 disable, 1 enable
	//
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	var pssm_state = 0;														// required to enable switching on/off
	var temp_scsm_depth = 0;												// required to enable switching on/off
	var temp_stencil_blur = 0;												// required to enable switching on/off
	var temp_window = 1;														// required to switch shadowmapping on/off when window gets active/inactive
	
	ENTITY* temp_ent = ent_create(NULL, nullvector, NULL);		// required by mouse_ent file name display
	
	// final key_any function calls
	while(UIMode < (var)UIMODE_NEWMAP)
		{
//			DEBUG_VAR(UIMode, 25);	
			
			//--------------------------------------------------------------------------------------
//			// test- fps
//			
//			draw_text( str_for_num(NULL, integer(16/time_frame)), 15, screen_size.y-50, COLOR_BLACK);
//			
			//--------------------------------------------------------------------------------------
//			// test - not okay - objects are covered by helper entities, so they need flag UNTOUCHABLE to be set !!!!
//			
////			int entmouse = 0;
//			if (mouse_ent)
//				{
////					entmouse = 100;										
//					temp_ent = mouse_ent;							// to not to get errors
//					if (str_for_entfile(NULL, temp_ent))
//						{
//							if (!str_cmpi(str_for_entfile(NULL, temp_ent), ""))
//								{
//									draw_text( str_for_entfile(NULL, temp_ent), 15, screen_size.y-75, COLOR_BLACK);
//								}
//						}					
//				}
////			draw_text( str_for_int(NULL, entmouse), 15, screen_size.y-125, vector(200,225,225));	
//			
			//--------------------------------------------------------------------------------------
//			// test - okay - can be used for disabling placement/deletion 
//
//			int paneltouch =  0;
//			if (mouse_panel)
//				{
//					paneltouch =  1;
//				}
//			draw_text( str_for_int(NULL, paneltouch), 15, screen_size.y-100, vector(200,225,225));
//
			//--------------------------------------------------------------------------------------
			// switch off shadowmapping when MB window is left to avoid resolution change related rendering issues
			
			if ((!window_focus))// || (!mouse_valid))
				{					
					if (temp_window)	// was focused, //or was not left in previous frame
						{
							temp_window = 0;
							
							// stop shadowmapping-------------------------------------------------
							if (shadow_stencil==(var)8)
								{
									if (pssm_numsplits > (var)0)
										{
											pssm_state = pssm_numsplits;
											
											Pssm_Close();
											wait_for(Pssm_Close);	
										}
								}	
							else if (shadow_stencil==(var)-1)
								{
									if (scsm_run == (var)1)
										{
											temp_scsm_depth 	= scsm_maxdepth;
											
											Scsm_Close();
											wait_for(Scsm_Close);											
										}
								}
							
							// stop water shader-------------------------------------------------							
							if (proc_status(Scwater_ViewsLoop) > 0)
								{
									Scwater_Close();
									wait_for(Scwater_Close);
								}						
						}
				}
			else
				{
					if (!temp_window)		// was not focused, //or was left in previous frame
						{
							temp_window = 1;
							
							// start shadowmapping-------------------------------------------------
							if (shadow_stencil==(var)8)
								{
									if (pssm_numsplits == (var)0)
										{
											pssm_numsplits = pssm_state;
											wait(3);	
											Pssm_Start(pssm_state);
											wait(3);	
										}
								}	
							else if (shadow_stencil==(var)-1)
								{
									if (scsm_run == (var)0)
										{
											Scsm_Start();									// auto calculates depth that suits new resolution, but can be modified afterwards
											wait(15);
											if (temp_scsm_depth>0)
												scsm_maxdepth 	= temp_scsm_depth;
										}
								}
							
							// start water shader-------------------------------------------------							
							if (main_water > (var)0)
								if (water_entity)
									if (proc_status(Scwater_ViewsLoop) == (var)0)
										{
											Scwater_Init(water_entity);
											wait_for(Scwater_Init);
										}								
						}
				}
			
			//--------------------------------------------------------------------------------------
			
			if ((window_focus) && (mouse_valid))
			{
			
			if (key_any) 
				{
					if (UIMode < UIMODE_SHADOWSHIFT * 5)
						{
							// UI mode specific function calls														
							
							//------------------------------------------------
							
							// detail (grass) placement options
							if (UIMode==(var)UIMODE_DETAIL)
								{
									// hide unhide panel
									if(key_i) 
										{
											while (key_i) {wait (1);}
											Detail_Panel_Toggle();
										}
									// decrease creator cube size - HUN y
									if(key_n) 
										{
											while (key_n) {wait (1);}
											Detail_DecSize();
										}
									// increase creator cube size
									if(key_m) 
										{
											while (key_m) {wait (1);}
											Detail_IncSize();			
										}																		
									// change set to be active
									if(key_b) 
										{
											if ((!mouse_left) && (proc_status(Detail_OnClick)==0))
												{
													while (key_b) {wait (1);}
													if (proc_status(Detail_IncSet)==0)
														{
															Detail_IncSet();	
															wait_for(Detail_IncSet);	
															wait(1);
														}	
												}
										}
									if(key_v) 
										{
											if ((!mouse_left) && (proc_status(Detail_OnClick)==0))
												{
													while (key_v) {wait (1);}
													if (proc_status(Detail_DecSet)==0)
														{
															Detail_DecSet();	
															wait_for(Detail_DecSet);	
															wait(1);
														}	
												}
										}
									// switch on/off 1..8 fags of the actual set of detail determining which of them will appear
//									if ((key_1) || (key_2) || (key_3) || (key_4) 
//									 || (key_5) || (key_6) || (key_7) || (key_8) )
									if ( (key_lastpressed>1) && (key_lastpressed<10) )
										{
											if ((!mouse_left) && (proc_status(Detail_OnClick)==0))
												{
													while (key_any) {wait (1);}
													Detail_Flag_Switch(key_lastpressed);	
													wait_for(Detail_Flag_Switch);			
												}
										}							
																	
									// mouse
									if (mouse_panel == NULL)
										{
											if (mouse_left)
												{													
													if (proc_status(Detail_OnClick)==0)
														{
															Detail_OnClick();
														}
												}
											if ((mouse_right) && (!key_shift) && (!key_ctrl))
												{
													Detail_DeleteArea();
												}
											if (mouse_middle)
												{											
													
												}
										}
										
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											toggle(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											reset(ui_helptext,SHOW);
										}
								}
							
							//------------------------------------------------
							
							// shrub placement options
							else if (UIMode==(var)UIMODE_SHRUB)
								{									
									// hide unhide panel
									if(key_i) 
										{
											while (key_i) {wait (1);}
											Shrub_Panel_Toggle();
										}
									// decrease creator cube size - HUN y
									if(key_n) 
										{
											while (key_n) {wait (1);}
											Shrub_DecSize();
										}
									// increase creator cube size
									if(key_m) 
										{
											while (key_m) {wait (1);}
											Shrub_IncSize();			
										}									
									// change set to be active
									if(key_b) 
										{
											if ((!mouse_left) && (proc_status(Shrub_OnClick)==0))
												{
													while (key_b) {wait (1);}
													if (proc_status(Shrub_IncSet)==0)
														{
															Shrub_IncSet();	
															wait_for(Shrub_IncSet);	
															wait(1);
														}	
												}
										}
									if(key_v) 
										{
											if ((!mouse_left) && (proc_status(Shrub_OnClick)==0))
												{
													while (key_v) {wait (1);}
													if (proc_status(Shrub_DecSet)==0)
														{
															Shrub_DecSet();	
															wait_for(Shrub_DecSet);	
															wait(1);
														}	
												}
										}
									// switch on/off 1..8 fags of the actual set of detail determining which of them will appear
//									if ((key_1) || (key_2) || (key_3) || (key_4) 
//									 || (key_5) || (key_6) || (key_7) || (key_8) )
									if ( (key_lastpressed>1) && (key_lastpressed<10) )
										{
											if ((!mouse_left) && (proc_status(Shrub_OnClick)==0))
												{
													while (key_any) {wait (1);}
													Shrub_Flag_Switch(key_lastpressed);		
													wait_for(Shrub_Flag_Switch);			
												}
										}							
																	
									// mouse
									if (mouse_panel == NULL)
										{
											if (mouse_left)
												{	
													if (proc_status(Shrub_OnClick)==0)
														{
															Shrub_OnClick();
														}
												}
											if ((mouse_right) && (!key_shift) && (!key_ctrl))
												{
													Shrub_DeleteArea();
												}
											if (mouse_middle)
												{
													
												}
										}
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											toggle(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											reset(ui_helptext,SHOW);
										}
								}
							
							//------------------------------------------------
							
							// grass placement options
							else if (UIMode==(var)UIMODE_GRASS)
								{									
									// hide unhide panel
									if(key_i) 
										{
											while (key_i) {wait (1);}
											Grass_Panel_Toggle();
										}
									// decrease creator cube size - HUN y
									if(key_n) 
										{
											while (key_n) {wait (1);}
											Grass_DecSize();
										}
									// increase creator cube size
									if(key_m) 
										{
											while (key_m) {wait (1);}
											Grass_IncSize();			
										}									
									// change set to be active
									if(key_b) 
										{
											if ((!mouse_left) && (proc_status(Grass_OnClick)==0))
												{
													while (key_b) {wait (1);}
													if (proc_status(Grass_IncSet)==0)
														{
															Grass_IncSet();	
															wait_for(Grass_IncSet);	
															wait(1);
														}	
												}
										}
									if(key_v) 
										{
											if ((!mouse_left) && (proc_status(Grass_OnClick)==0))
												{
													while (key_v) {wait (1);}
													if (proc_status(Grass_DecSet)==0)
														{
															Grass_DecSet();	
															wait_for(Grass_DecSet);	
															wait(1);
														}	
												}
										}
									// switch on/off 1..8 fags of the actual set of detail determining which of them will appear
//									if ((key_1) || (key_2) || (key_3) || (key_4) 
//									 || (key_5) || (key_6) || (key_7) || (key_8) )
									if ( (key_lastpressed>1) && (key_lastpressed<10) )
										{
											if ((!mouse_left) && (proc_status(Grass_OnClick)==0))
												{
													while (key_any) {wait (1);}
													Grass_Flag_Switch(key_lastpressed);	
													wait_for(Grass_Flag_Switch);	
												}		
										}							
																	
									// mouse
									if (mouse_panel == NULL)
										{
											if (!key_ctrl)
												{
													if (mouse_left)
														{
															if (proc_status(Grass_OnClick)==0)
																{
																	Grass_OnClick();
																}
														}
												}											
											if ((mouse_right) && (!key_shift) && (!key_ctrl))
												{
													Grass_DeleteArea();
												}											
											if (mouse_middle)
												{
													
												}
											
											if (key_ctrl)
												{
													if (mouse_left)
														{													
															mod_origmenu = (var)UIMODE_GRASS;
															while (mouse_left) {wait (1);}
															ModifyObject_Select(NULL);
														}
												}
										}
									
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											toggle(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											reset(ui_helptext,SHOW);
										}
								}
							
							//------------------------------------------------
							
							// obstacle placement options
							else if (UIMode==(var)UIMODE_OBSTACLE)
								{
									// hide unhide panel
									if(key_i) 
										{
											while (key_i) {wait (1);}
											if (proc_status(Obstacle_Panel_Toggle)==0)
												{
													Obstacle_Panel_Toggle();	
													wait_for(Obstacle_Panel_Toggle);	
													wait(1);
												}
										}
									// decrease creator cube size - HUN y
									else if(key_n) 
										{
											while (key_n) {wait (1);}
											if (proc_status(Obstacle_DecSize)==0)
												{
													Obstacle_DecSize();	
													wait_for(Obstacle_DecSize);	
													wait(1);
												}	
										}
									// increase creator cube size
									else if(key_m) 
										{
											while (key_m) {wait (1);}
											if (proc_status(Obstacle_IncSize)==0)
												{
													Obstacle_IncSize();	
													wait_for(Obstacle_IncSize);	
													wait(1);
												}			
										}									
									// change set to be active
									else if(key_b) 
										{
											if ((!mouse_left) && (proc_status(Obstacle_OnClick)==0))
												{
													while (key_b) {wait (1);}
													if (proc_status(Obstacle_IncSet)==0)
														{
															Obstacle_IncSet();	
															wait_for(Obstacle_IncSet);	
															wait(1);
														}	
												}
										}
									else if(key_v) 
										{
											if ((!mouse_left) && (proc_status(Obstacle_OnClick)==0))
												{
													while (key_v) {wait (1);}
													if (proc_status(Obstacle_DecSet)==0)
														{
															Obstacle_DecSet();	
															wait_for(Obstacle_DecSet);	
															wait(1);
														}	
												}
										}
									// rotate preview model - obstacle_model_preview
									else if(key_c) 
										{
											while (key_c) {wait (1);}
											// rotate CW
											Obstacle_RotateModelPreview(-1);
										}
									else if(key_x) 
										{
											while (key_x) {wait (1);}
											// rotate CCW
											Obstacle_RotateModelPreview(1);
										}
									// switch on/off 1..8 fags of the actual set of detail determining which of them will appear
//									if ((key_1) || (key_2) || (key_3) || (key_4) 
//									 || (key_5) || (key_6) || (key_7) || (key_8) )
									else if ( (key_lastpressed>1) && (key_lastpressed<10) )
										{
											if ((!mouse_left) && (proc_status(Obstacle_OnClick)==0))
												{
													while (key_any) {wait (1);}
													if (proc_status(Obstacle_Flag_Switch)==0)
														{
															Obstacle_Flag_Switch(key_lastpressed);	
															wait_for(Obstacle_Flag_Switch);	
															wait(1);
														}	
												}
										}																								
									// mouse
									else if (mouse_panel == NULL)
										{											
											// place on area (once or continuously)
											if ((mouse_left) && (!key_shift) && (!key_ctrl))
												{
													if (proc_status(Obstacle_OnClick) == 0)
														{
															Obstacle_OnClick();
															wait_for(Obstacle_OnClick);	
															wait(1);
														}
												}
											// clone 
											else if ((mouse_left) && (key_shift) && (!key_ctrl) && (!key_alt))
												{
													while (mouse_left) {wait (1);}
													Obstacle_CloneObject();
													wait_for(Obstacle_CloneObject);
												}
											// modify
											else if ((mouse_left) && (!key_shift) && (key_ctrl) && (!key_alt))
												{													
													mod_origmenu = (var)UIMODE_OBSTACLE;
													while (mouse_left) {wait (1);}
													ModifyObject_Select(NULL);
												}
											// delete area
											else if ((mouse_right) && (!key_shift) && (!key_ctrl) && (!key_alt))
												{
													Obstacle_DeleteArea();
												}
											// delete object
											else if ((mouse_right) && (!key_shift) && (key_ctrl) && (!key_alt))
												{
													Obstacle_DeleteItem();
												}
											// other
											else if (mouse_middle)
												{
													//...
												}	
											
											// preview rotation by mouse wheel
											if ((mickey.z != (var)0) && (!key_shift) && (key_ctrl) && (!key_alt))
												{
													if (mickey.z > (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// rotate CW
															Obstacle_RotateModelPreview(-1);
														}
													else if (mickey.z < (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// rotate CCW
															Obstacle_RotateModelPreview(1);
														}
												}
											else // preview turn by mouse wheel
											if ((mickey.z != (var)0) && (!key_shift) && (!key_ctrl) && (key_alt))
												{
													if (mickey.z > (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// turn CW
															Obstacle_TurnModelPreview(-1);
														}
													else if (mickey.z < (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// turn CCW
															Obstacle_TurnModelPreview(1);
														}
												}												
										}
									
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											toggle(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											reset(ui_helptext,SHOW);
										}
								}
							
							//------------------------------------------------
							
							// mapent placement options
							else if (UIMode==(var)UIMODE_MAPENT)
								{
									// hide unhide panelMapEnt
									if(key_i) 
										{
											while (key_i) {wait (1);}
											MapEnt_Panel_Toggle();
										}								
									// change set to be active
									if(key_b) 
										{
											if ((!mouse_left) && (proc_status(MapEnt_CreateObject)==0))
												{
													while (key_b) {wait (1);}
													if (proc_status(MapEnt_IncSet)==0)
														{
															MapEnt_IncSet();	
															wait_for(MapEnt_IncSet);	
															wait(1);
														}	
												}
										}
									if(key_v) 
										{
											if ((!mouse_left) && (proc_status(MapEnt_CreateObject)==0))
												{
													while (key_v) {wait (1);}
													if (proc_status(MapEnt_DecSet)==0)
														{
															MapEnt_DecSet();	
															wait_for(MapEnt_DecSet);	
															wait(1);
														}	
												}
										}
									// rotate preview model - mapent_model_preview
									if(key_c) 
										{
											while (key_c) {wait (1);}
											// rotate CW
											MapEnt_RotateModelPreview(-1);
										}
									if(key_x) 
										{
											while (key_x) {wait (1);}
											// rotate CCW
											MapEnt_RotateModelPreview(1);
										}
									// switch on/off 1..8 fags of the actual set of detail determining which of them will appear
//									if ((key_1) || (key_2) || (key_3) || (key_4) 
//									 || (key_5) || (key_6) || (key_7) || (key_8) )
									if ( (key_lastpressed>1) && (key_lastpressed<10) )
										{
											if ((!mouse_left) && (proc_status(MapEnt_CreateObject)==0))
												{
													while (key_any) {wait (1);}
													MapEnt_Flag_Switch(key_lastpressed);	
													wait_for(MapEnt_Flag_Switch);		
												}	
										}							
																	
									// mouse
									if (mouse_panel == NULL)
										{
											// place object
											if ((mouse_left) && (!key_shift) && (!key_ctrl))
												{
													while (mouse_left) {wait (1);}
													MapEnt_CreateObject();
												}
											// clone 
											else if ((mouse_left) && (key_shift) && (!key_ctrl) && (!key_alt))
												{
													while (mouse_left) {wait (1);}
													MapEnt_CloneObject();
													wait_for(MapEnt_CloneObject);
												}
											// modify
											else if ((mouse_left) && (!key_shift) && (key_ctrl) && (!key_alt))
												{													
													mod_origmenu = (var)UIMODE_MAPENT;
													while (mouse_left) {wait (1);}
													ModifyObject_Select(NULL);
												}
											// delete object
											else if ((mouse_right) && (!key_shift) && (!key_ctrl) && (!key_alt))
												{
													while (mouse_right) {wait (1);}
													MapEnt_DeleteItem();
												}
											else if (mouse_middle)
												{
													//...
												}
											
											// preview rotation by mouse wheel
											if ((mickey.z != (var)0) && (!key_shift) && (key_ctrl) && (!key_alt))
												{
													if (mickey.z > (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// rotate CW
															MapEnt_RotateModelPreview(-1);
														}
													else if (mickey.z < (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// rotate CCW
															MapEnt_RotateModelPreview(1);
														}
												}	
											else // preview turn by mouse wheel
											if ((mickey.z != (var)0) && (!key_shift) && (!key_ctrl) && (key_alt))
												{
													if (mickey.z > (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// turn CW
															MapEnt_TurnModelPreview(-1);
														}
													else if (mickey.z < (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// turn CCW
															MapEnt_TurnModelPreview(1);
														}
												}		
										}
										
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											toggle(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											reset(ui_helptext,SHOW);
										}
								}
							
							//------------------------------------------------
							
							// terrain model placement options
							else if (UIMode==(var)UIMODE_TERRMDL)
								{
									// hide unhide panelMapEnt
									if(key_i) 
										{
											while (key_i) {wait (1);}
											TerrMdl_Panel_Toggle();
										}								
									// change set to be active
									if(key_b) 
										{
											if ((!mouse_left) && (proc_status(TerrMdl_CreateObject)==0))
												{
													while (key_b) {wait (1);}
													if (proc_status(TerrMdl_IncSet)==0)
														{
															TerrMdl_IncSet();	
															wait_for(TerrMdl_IncSet);	
															wait(1);
														}	
												}
										}
									if(key_v) 
										{
											if ((!mouse_left) && (proc_status(TerrMdl_CreateObject)==0))
												{
													while (key_v) {wait (1);}
													if (proc_status(TerrMdl_DecSet)==0)
														{
															TerrMdl_DecSet();	
															wait_for(TerrMdl_DecSet);	
															wait(1);
														}	
												}
										}
									// rotate preview model - obstacle_model_preview
									else if(key_c) 
										{
											while (key_c) {wait (1);}
											// rotate CW
											TerrMdl_RotateModelPreview(-1);
										}
									else if(key_x) 
										{
											while (key_x) {wait (1);}
											// rotate CCW
											TerrMdl_RotateModelPreview(1);
										}
									// switch on/off 1..8 fags of the actual set of detail determining which of them will appear
//									if ((key_1) || (key_2) || (key_3) || (key_4) 
//									 || (key_5) || (key_6) || (key_7) || (key_8) )
									if ( (key_lastpressed>1) && (key_lastpressed<10) )
										{
											if ((!mouse_left) && (proc_status(TerrMdl_CreateObject)==0))
												{
													while (key_any) {wait (1);}
													TerrMdl_Flag_Switch(key_lastpressed);		
													wait_for(TerrMdl_Flag_Switch);		
												}
										}							
																	
									// mouse
									if (mouse_panel == NULL)
										{
											// create
											if ((mouse_left) && (!key_shift) && (!key_ctrl))
												{
													while (mouse_left) {wait (1);}
													TerrMdl_CreateObject();
												}
											// clone 
											else if ((mouse_left) && (key_shift) && (!key_ctrl) && (!key_alt))
												{
													while (mouse_left) {wait (1);}
													TerrMdl_CloneObject();
													wait_for(TerrMdl_CloneObject);
												}
											// modify
											else if ((mouse_left) && (!key_shift) && (key_ctrl) && (!key_alt))
												{													
													mod_origmenu = (var)UIMODE_TERRMDL;
													while (mouse_left) {wait (1);}
													ModifyObject_Select(NULL);
												}
											// delete
											else if ((mouse_right) && (!key_shift) && (!key_ctrl) && (!key_alt))
												{
													while (mouse_right) {wait (1);}
													TerrMdl_DeleteItem();
												}										
											else if (mouse_middle)
												{
													
												}
											
											// preview rotation by mouse wheel
											if ((mickey.z != (var)0) && (!key_shift) && (key_ctrl) && (!key_alt))
												{
													if (mickey.z > (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// rotate CW
															TerrMdl_RotateModelPreview(-1);
														}
													else if (mickey.z < (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// rotate CCW
															TerrMdl_RotateModelPreview(1);
														}
												}
											else // preview turn by mouse wheel
											if ((mickey.z != (var)0) && (!key_shift) && (!key_ctrl) && (key_alt))
												{
													if (mickey.z > (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// turn CW
															TerrMdl_TurnModelPreview(-1);
														}
													else if (mickey.z < (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// turn CCW
															TerrMdl_TurnModelPreview(1);
														}
												}		
										}
										
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											toggle(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											reset(ui_helptext,SHOW);
										}
								}
								
							//------------------------------------------------
							
							// building placement options
							else if (UIMode==(var)UIMODE_BUILDING)
								{
									// hide unhide panel
									if(key_i) 
										{
											while (key_i) {wait (1);}
											if (proc_status(Building_Panel_Toggle)==0)
												{
													Building_Panel_Toggle();	
													wait_for(Building_Panel_Toggle);	
													wait(1);
												}
										}								
									// change set to be active
									else if(key_b) 
										{
											if ((!mouse_left) && (proc_status(Building_CreateObject)==0))
												{
													while (key_b) {wait (1);}
													if (proc_status(Building_IncSet)==0)
														{
															Building_IncSet();	
															wait_for(Building_IncSet);	
															wait(1);
														}	
												}
										}
									else if(key_v) 
										{
											if ((!mouse_left) && (proc_status(Building_CreateObject)==0))
												{
													while (key_v) {wait (1);}
													if (proc_status(Building_DecSet)==0)
														{
															Building_DecSet();	
															wait_for(Building_DecSet);	
															wait(1);
														}	
												}
										}
									// rotate preview model - building_model_preview
									else if(key_c) 
										{
											while (key_c) {wait (1);}
											// rotate CW
											Building_RotateModelPreview(-1);
										}
									else if(key_x) 
										{
											while (key_x) {wait (1);}
											// rotate CCW
											Building_RotateModelPreview(1);
										}
									// switch on/off 1..8 fags of the actual set of detail determining which of them will appear
//									if ((key_1) || (key_2) || (key_3) || (key_4) 
//									 || (key_5) || (key_6) || (key_7) || (key_8) )
									else if ( (key_lastpressed>1) && (key_lastpressed<10) )
										{
											if ((!mouse_left) && (proc_status(Building_CreateObject)==0))
												{
													while (key_any) {wait (1);}
													if (proc_status(Building_Flag_Switch)==0)
														{
															Building_Flag_Switch(key_lastpressed);	
															wait_for(Building_Flag_Switch);	
															wait(1);
														}	
												}
										}																								
									// mouse
									else if (mouse_panel == NULL)
										{											
//											// place on area
//											if ((mouse_left) && (!key_shift) && (!key_ctrl) && (!key_alt))
//												{
//													if (proc_status(Building_OnClick) == 0)
//														{
//															Building_OnClick();
//															wait_for(Building_OnClick);	
//															wait(1);
//														}
//												}
											// place object
											if ((mouse_left) && (!key_shift) && (!key_ctrl))
												{
													while (mouse_left) {wait (1);}
													Building_CreateObject();
												}
											// clone 
											else if ((mouse_left) && (key_shift) && (!key_ctrl) && (!key_alt))
												{
													while (mouse_left) {wait (1);}
													Building_CloneObject();
													wait_for(Building_CloneObject);
												}
											// modify
											else if ((mouse_left) && (!key_shift) && (key_ctrl) && (!key_alt))
												{													
													mod_origmenu = (var)UIMODE_BUILDING;
													while (mouse_left) {wait (1);}
													ModifyObject_Select(NULL);
												}
//											// delete area
//											else if ((mouse_right) && (!key_shift) && (!key_ctrl) && (!key_alt))
//												{
//													Building_RemoveArea();
//												}
											// delete object
											else if ((mouse_right) && (!key_shift) && (!key_ctrl) && (!key_alt))
												{
													while (mouse_right) {wait (1);}
													Building_DeleteItem();
												}
											// other
											else if (mouse_middle)
												{
													//...
												}	
											
											// preview rotation by mouse wheel
											if ((mickey.z != (var)0) && (!key_shift) && (key_ctrl) && (!key_alt))
												{
													if (mickey.z > (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// rotate CW
															Building_RotateModelPreview(-1);
														}
													else if (mickey.z < (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// rotate CCW
															Building_RotateModelPreview(1);
														}
												}
											else // preview turn by mouse wheel
											if ((mickey.z != (var)0) && (!key_shift) && (!key_ctrl) && (key_alt))
												{
													if (mickey.z > (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// turn CW
															Building_TurnModelPreview(-1);
														}
													else if (mickey.z < (var)0)	
														{
															while (mickey.z != (var)0) {wait (1);}
															// turn CCW
															Building_TurnModelPreview(1);
														}
												}										
										}
									
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											toggle(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											reset(ui_helptext,SHOW);
										}
								}
								
							//------------------------------------------------
							
							// material handling mainmenu
							else if (UIMode==(var)UIMODE_MATERIAL)
								{							
									// do nothing !
									
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											toggle(ui_helptext,SHOW);
										}
								}
								
							// sky handling mainmenu
							else if (UIMode==(var)UIMODE_SKY)
								{							
									// do nothing !
									
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											toggle(ui_helptext,SHOW);
										}
								}
#ifndef MB_RELEASE							
							// map info mode
							else if (UIMode==(var)UIMODE_MAPINFO)
								{
									// hide panels and cluster area sprites
									if(key_i) 
										{
											while (key_i) {wait (1);}
											TileClusterPanel_Toggle();
											
											// toggle main menu panel
											if (is(ui_mainmenu_panel, SHOW))
												{
													reset(ui_mainmenu_panel,SHOW);
												}
											else
												{
													set(ui_mainmenu_panel,SHOW);
												}											
										}
									
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											toggle(ui_helptext,SHOW);
										}
								}
#endif							
							// file menu : 10 main, 11 general load submenu, 12..14 are reserved
							else if (UIMode==(var)UIMODE_FILEMENU)
								{							
									// no items
									
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											toggle(ui_helptext,SHOW);
										}
								}
							else if (UIMode==(var)UIMODE_FILEMENU2)
								{							
									// no items
									
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											toggle(ui_helptext,SHOW);
										}
								}
							
							// modify object menu - selection is done - 17..19 are reserved
							else if (UIMode==(var)UIMODE_MODIFYOBJ2)
								{							
									if (mouse_panel == NULL)
										{
											if (mouse_left)
												{												
													// call outer function, with running instance quantity checking to avoid problems because of waits
													if (
															(proc_status(ModifyObject_Gizmo_Move) == (var)0)
															&&
															(proc_status(ModifyObject_Gizmo_Rotate) == (var)0)
															&&
															(proc_status(ModifyObject_Gizmo_Scale) == (var)0)
														)		
														{
															ModifyObject_Click();			// call only once !!!														
														}
												}
										}
									
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											reset(ui_helptext,SHOW);
										}
								}															
							
							// terrain editing mode
							else if ((UIMode==(var)UIMODE_TEREDIT))		// && (!map_wmb)
								{							
									// decrease creator cube size - HUN y
									if((key_n) && (!key_ctrl)) 
										{
											while (key_n) {wait (1);}
											TerEdit_DecSize();
										}
									// increase creator cube size
									if((key_m) && (!key_ctrl)) 
										{
											while (key_m) {wait (1);}
											TerEdit_IncSize();			
										}	
									
									// height editting specific
									if (teredit_he_panel!=NULL)
										{
											// undo z - US z = GER/HUN y = key_y !				
											if((key_y) && (key_ctrl)) 
//											if((key_pressed(key_for_str("z"))) && (key_ctrl)) 			// not okay
												{
													while (key_y) {wait (1);}
													TerEdit_He_Undo();
												}
											// redo y - US y = GER/HUN z = key_z !
											if((key_z) && (key_ctrl)) 
												{
													while (key_z) {wait (1);}
													TerEdit_He_Redo();
												}
											// reset
											if((key_r) && (key_ctrl)) 
												{
													while (key_r) {wait (1);}
													TerEdit_He_Reset();
												}									
											// update surface
											if((key_u) && (!key_ctrl)) 
												{
													while (key_u) {wait (1);}
													TerrHmp_FixNormals();
												}
											// update entities
											if((key_u) && (key_ctrl)) 
												{
													while (key_u) {wait (1);}
													TerEdit_UpdateEntities();
												}
										}
									
//									if ((mouse_pos.x<screen_size.x-256) && (mouse_pos.y<screen_size.y-32))
									if (mouse_panel == NULL)
										{
											if (((mouse_left) || (mouse_right)) && (!key_shift) && (!key_ctrl))
												{
//													while ((mouse_left) || (mouse_right)) {wait (1);}	// buttons are handled in local function
													TerEdit_OnClick();
												}
										}
									// terrain lightmap - simple - faster
//									if ((key_l) && (key_ctrl))
//										{
//											while (key_l) {wait (1);}
//											TerEdit_Tx_Lightmap_Lm();
//										}
//									// terrain lightmap with alpha check
//									if ((key_l) && (!key_ctrl))
//										{
//											while (key_l) {wait (1);}
//											TerEdit_Lm_Lightmap_Alpha();
//										}	
									
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											toggle(ui_helptext_teredit,SHOW);											
											reset(ui_helptext,SHOW);
										}								
								}								
							// player test 01 mode
							else if ((UIMode==(var)UIMODE_PLAYTEST01))		// && (!map_wmb)
								{
									// help text--------------------------------------
									if (key_esc)
										{
											while (key_esc) {wait (1);}
											reset(ui_helptext_place0,SHOW);
											reset(ui_helptext_place1,SHOW);
											reset(ui_helptext_place2,SHOW);
											reset(ui_helptext_teredit,SHOW);											
											toggle(ui_helptext,SHOW);
										}	
								}	
							
							/////////////////////////////////////////////////////
							// general function calls
							
							//--------------------------------------------------------------	
							// draw d3d lines
							
							if ((key_tab) && (!key_alt) && (key_ctrl) && (!key_shift))
								{
									while (key_tab) {wait (1);}
									d3d_lines = 1 - d3d_lines;
									if (d3d_lines)
										{
											UI_StartD3DLoop();
										}
								}	
							
							//--------------------------------------------------------------	
							// wireframe view
							
							if ((key_tab) && (!key_alt) && (!key_ctrl) && (key_shift))
								{
									while (key_tab) {wait (1);}
									ui_wireframe = 1 - ui_wireframe;
									if (ui_wireframe)
										{
											UI_StartWireFrame();
										}
									else
										{
											Materials_ShadersInit();
										}
								}	
							
							//--------------------------------------------------------------	
//							// reload map by saving to temp and loading it back, after destroying level
//
//							if ((key_r) && (key_alt) && (!key_ctrl) && (!key_shift))
//								{
//									while (key_r) {wait (1);}
//									FileMenu_Reload_Map();
//								}
//							
							//--------------------------------------------------------------	
							// quick save map to temp
							
							if ((key_s) && (key_alt) && (!key_ctrl) && (!key_shift))
								{
									while (key_s) {wait (1);}
									FileMenu_QuickSave();	
								}
								
							//--------------------------------------------------------------	
							// exit
							
							if ((key_ctrl) && (key_alt) && (key_end))
								{
									Main_Exit();
								}	
							
							//--------------------------------------------------------------	
//							// test
//							if (key_enter)
//								{
//									while (key_enter) {wait (1);}
//									MessageBox(NULL,"caption","message",MB_OK);		// behind fullscreen
////									printf("message");									// okay
//								}
							
							//--------------------------------------------------------------	
							// toggle shadowmapping
							
							if (shadow_stencil==(var)8)
								{
									if ((key_q) && (!key_alt) && (key_ctrl) && (!key_shift))
										{
											while (key_q) {wait (1);}
											if (pssm_numsplits==(var)0)
												{
													pssm_numsplits = pssm_state;
													wait(3);	
													Pssm_Start(pssm_state);
													wait(3);	
												}
											else
												{
													pssm_state = pssm_numsplits;
													
													Pssm_Close();
													wait_for(Pssm_Close);	
												}
										}
								}	
							else if (shadow_stencil==(var)-1)
								{
									if ((key_q) && (!key_alt) && (key_ctrl) && (!key_shift))
										{
											while (key_q) {wait (1);}
											if (scsm_run==(var)1)
												{
													temp_scsm_depth 	= scsm_maxdepth;
													
													Scsm_Close();
													wait_for(Scsm_Close);												
												}
											else
												{
													Scsm_Start();									// auto calculates depth that suits new resolution, but can be modified afterwards
													wait(15);
													if (temp_scsm_depth>0)
														scsm_maxdepth 	= temp_scsm_depth;
												}
										}
								}
							else if (shadow_stencil==(var)1)
								{
									if ((key_q) && (!key_alt) && (key_ctrl) && (!key_shift))
										{
											while (key_q) {wait (1);}
//											if (fixv(mtl_shadow.skill1) > (var)0)
											if (stencil_blur_value > (var)0)
												{
//													temp_stencil_blur 	= fixv(mtl_shadow.skill1);
													temp_stencil_blur 	= stencil_blur_value;
//													mtl_shadow.skill1 	= floatv(0);
													stencil_blur_value	= 0;
													wait(3);													
												}
											else
												{
//													mtl_shadow.skill1 	= floatv(temp_stencil_blur);
													stencil_blur_value	= temp_stencil_blur;
													wait(3);													
												}
										}									
								}
							
							//--------------------------------------------------------------	
							// toggle camera mode
							
							if ((key_k) && (key_ctrl))
								{
									while (key_k) {wait (1);}
									Camera_ChangeMode();
									wait_for(Camera_ChangeMode);
								}	
							
							//--------------------------------------------------------------	
							// toggle grid
							
							if (key_g)
								{
									while (key_g) {wait (1);}
									ui_drawd3dgrid  = 1 - ui_drawd3dgrid;
								}
							
							//////////////////////////////////////////////////////////////			
							// main menu init calls		
							
//							if ( (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2) )		// exclude modify object menu - not needed
								{
									//---------
									
									// ground terrain placement mode---------------
									if ( (key_f1) && (!key_shift) && (UIMode!=(var)UIMODE_GROUNDTERR) ) 		// ground terrain placement mode
										{
											while (key_f1) {wait (1);}
											UIMode = (var)UIMODE_GROUNDTERR;	
											TerrGround_Init();	
										}	
									// water terrain placement mode---------------
									if ( (key_f1) && (key_shift) && (UIMode!=(var)UIMODE_WATERTERR) ) 		// water terrain placement mode
										{
											while (key_f1) {wait (1);}
											UIMode = (var)UIMODE_WATERTERR;	
											TerrWater_Init();	
										}	
									
									//---------
									
									// grass sprite placement mode------------------
									if ( (key_f3) && (key_shift) && (UIMode!=(var)UIMODE_DETAIL) ) 			// detail placement mode
										{
											while (key_f3) {wait (1);}
											UIMode = (var)UIMODE_DETAIL;	
											Detail_Init();	
										}
									// shrub sprite placement mode------------------
									if ( (key_f4) && (key_shift) && (UIMode!=(var)UIMODE_SHRUB) ) 				// shrub placement mode
										{
											while (key_f4) {wait (1);}
											UIMode = (var)UIMODE_SHRUB;	
											Shrub_Init();	
										}
									// grass modelplacement mode--------------------
									if ( (key_f3) && (!key_shift) && (UIMode!=(var)UIMODE_GRASS) ) 			// grass placement mode
										{
											while (key_f3) {wait (1);}
											UIMode = (var)UIMODE_GRASS;	
											Grass_Init();	
										}
										
									// obstacle model placement mode----------------
									if ( (key_f4) && (!key_shift) && (UIMode!=(var)UIMODE_OBSTACLE) ) 		// obstacle placement mode
										{
											while (key_f4) {wait (1);}
											UIMode = (var)UIMODE_OBSTACLE;	
											Obstacle_Init();	
										}									
									
									// mapent placement mode-----------------------
									if ( (key_f5) && (key_shift) && (UIMode!=(var)UIMODE_MAPENT) ) 			// mapent placement mode
										{
											while (key_f5) {wait (1);}
											UIMode = (var)UIMODE_MAPENT;	
											MapEnt_Init();	
										}	
									
									// terrain model placement mode---------------
									if ( (key_f2) && (!key_shift) && (UIMode!=(var)UIMODE_TERRMDL) ) 			// mapent placement mode
										{
											while (key_f2) {wait (1);}
											UIMode = (var)UIMODE_TERRMDL;	
											TerrMdl_Init();	
										}	
									
									// building model placement mode--------------
									if ( (key_f5) && (!key_shift) && (UIMode!=(var)UIMODE_BUILDING) ) 		// building placement mode
										{
											while (key_f5) {wait (1);}
											UIMode = (var)UIMODE_BUILDING;	
											Building_Init();	
										}									
									
									//---------
										
									// material mode---------------------
									if ( (key_f6) && (key_shift) && (UIMode!=(var)UIMODE_MATERIAL) )			// material settings mode
										{
											while (key_f6) {wait (1);}
											UIMode = (var)UIMODE_MATERIAL;	
											Material_Init();	
										}
									// sky mode---------------------
									if ( (key_f6) && (!key_shift) && (UIMode!=(var)UIMODE_SKY) )				// sky settings mode
										{
											while (key_f6) {wait (1);}
											UIMode = (var)UIMODE_SKY;	
											Sky_Init();	
										}
									// map settings mode---------------------
									if ( (key_f12) && (key_shift) && (UIMode!=(var)UIMODE_SETTINGS) )			// map settings mode
										{
											while (key_f12) {wait (1);}
											UIMode = (var)UIMODE_SETTINGS;	
											Settings_Init();	
										}
#ifndef MB_RELEASE									
									// RTS map info mode---------------------
									if ( (key_f7) && (key_shift) && (UIMode!=(var)UIMODE_MAPINFO) )			// map info mode
										{
											while (key_f7) {wait (1);}
											UIMode = (var)UIMODE_MAPINFO;	
											Cluster_Info_Init();	
										}		
#endif										
									// terrain editor mode---------------------
									if ( (key_f2) && (key_shift) && (UIMode!=(var)UIMODE_TEREDIT) )			// terrain editor mode
										{
											while (key_f2) {wait (1);}
											UIMode = (var)UIMODE_TEREDIT;	
											TerEdit_Init();	
										}	
									// file handling mode-----------------------------
									if ( (key_f12) && (!key_shift) && (UIMode!=(var)UIMODE_FILEMENU) )		// file menu mode
										{
											while (key_f12) {wait (1);}
											UIMode = (var)UIMODE_FILEMENU;	
											FileMenu_Init();
										}	
									
									//---------
#ifndef MB_RELEASE									
									// RTS formation editor mode---------------------
									if ( (key_f7) && (key_shift) && (UIMode!=(var)UIMODE_FORMATION) )			// rts formation editor
										{
											while (key_f8) {wait (1);}
											UIMode = (var)UIMODE_FORMATION;	
											FormEdit_Init();											
										}		
#endif									
									//---------
									
									// player test 01 mode-----------------------------
									if ( (key_t) && (!key_ctrl) && (UIMode!=(var)UIMODE_PLAYTEST01) )			// playar test 01 mode
										{
											while (key_t) {wait (1);}
											UIMode = (var)UIMODE_PLAYTEST01;	
											PlayarTest01_Init();
										}		
									
#ifndef MB_RELEASE								
									// RTS player test 02 mode-----------------------------
									if ( (key_t) && (key_ctrl) && (UIMode!=(var)UIMODE_PLAYTEST02) )			// RTS playar test 02 mode
										{
											while (key_t) {wait (1);}
											UIMode = (var)UIMODE_PLAYTEST02;	
											Play02_Init();
										}
#endif							
									//---------
									
//									// save as wmb
//									if ((key_w) && (key_alt) && (!key_ctrl) && (!key_shift))
//										{
//											while (key_w) {wait (1);}
//											UIMode = (var)UIMODE_FILEMENU;												// to remove temporary editor entities
//											FileMenu_Init();
//											wait(12);
//											SaveWmb_Save_Wmb("levels\\temp.wmb");
//										}
								}	
						}
				}
			}		
			else
			{				
				key_pressed(-1);						// when mouse is out or window focus lost - eliminates some errors when window gets focused again
			}
			
			//---------------------------------
			
//			key_pressed(-1);							// not here! disables continuous movements etc. but useful locally in some cases...
			
			//---------------------------------
			
			wait(1);				
		}	
		
	temp_ent = NULL;
	ptr_remove(temp_ent);	
}


////////////////////////////////////////////
// main menu


void		UI_GroundMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_GROUNDTERR) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_GROUNDTERR;	
			TerrGround_Init();	
		}
}


void		UI_WaterMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_WATERTERR) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_WATERTERR;	
			TerrWater_Init();	
		}
}


void UI_DetailMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_DETAIL) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_DETAIL;	
			Detail_Init();	
		}
}


void UI_ShrubMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_SHRUB) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_SHRUB;	
			Shrub_Init();	
		}
}


void UI_GrassMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_GRASS) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_GRASS;	
			Grass_Init();	
		}
}


void UI_ObstacleMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_OBSTACLE) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_OBSTACLE;	
			Obstacle_Init();	
		}
}


void UI_MapEntMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_MAPENT) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_MAPENT;	
			MapEnt_Init();	
		}
}


void UI_TerrMdlMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_TERRMDL) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_TERRMDL;	
			TerrMdl_Init();	
		}
}


void UI_BuildingMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_BUILDING) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_BUILDING;	
			Building_Init();	
		}
}


void		UI_MaterialMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_MATERIAL) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_MATERIAL;	
			Material_Init();	
		}
}


void		UI_SkyMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_SKY) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_SKY;	
			Sky_Init();	
		}
}


void		UI_SettingsMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_SETTINGS) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_SETTINGS;	
			Settings_Init();	
		}
}


#ifndef MB_RELEASE

void		UI_MapInfoMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_MAPINFO) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_MAPINFO;	
			Cluster_Info_Init();	
		}
}

#endif


void		UI_TerEditMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_TEREDIT) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_TEREDIT;	
			TerEdit_Init();	
		}
}


void		UI_FileMode()
{
	wait(3);
	if ( (UIMode!=(var)UIMODE_FILEMENU) /*&& (UIMode!=(var)UIMODE_MODIFYOBJ) && (UIMode!=(var)UIMODE_MODIFYOBJ2)*/ )
		{
			UIMode = (var)UIMODE_FILEMENU;	
			FileMenu_Init();
		}
}


/////////////////////////////
// general panel event


void		ui_panelmove(PANEL* panel)
{
	// difference
	var diff_orig_x = mouse_pos.x - panel.pos_x;
	var diff_orig_y = mouse_pos.y - panel.pos_y;
	
	if (diff_orig_y > (var)32)	return;					// only upper part can be grabbed
	
	// move
	while(mouse_left)
		{
			// use a 2 pixel precision, and for safety no move out of screen
			panel.pos_x = maxv(0, minv( screen_size.x-panel.size_x, integer((mouse_pos.x - diff_orig_x)/2)*2 ));
			panel.pos_y = maxv(0, minv( screen_size.y-panel.size_y, integer((mouse_pos.y - diff_orig_y)/2)*2 ));
			
			wait(1);
		}
}


void		ui_submenu_panelmove(PANEL* panel)
{
	// difference
	var diff_orig_x = mouse_pos.x - panel.pos_x;
	var diff_orig_y = mouse_pos.y - panel.pos_y;
	
	if (diff_orig_y > (var)32)	return;					// only upper part can be grabbed
	
	// move
	while(mouse_left)
		{
			// use a 2 pixel precision, and for safety no move out of screen
			panel.pos_x = maxv(0, minv( screen_size.x-panel.size_x, integer((mouse_pos.x - diff_orig_x)/2)*2 ));
			panel.pos_y = maxv(0, minv( screen_size.y-panel.size_y, integer((mouse_pos.y - diff_orig_y)/2)*2 ));
			
			ui_submenupanel_x = panel.pos_x;
			ui_submenupanel_y = panel.pos_y;
			
			if (panel == material_matpanel)
				{
					if (material_lowpanel)
						{
							material_lowpanel.pos_x = ui_submenupanel_x;							
							material_lowpanel.pos_y = ui_submenupanel_y;				
						}
					
					if ( (material_colpan1) && (material_colpan2) && (material_colpan3) && (material_colpan4) )
						{
							material_colpan1.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
							material_colpan1.pos_y = ui_submenupanel_y + 75;									// 256+075;
							
							material_colpan2.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
							material_colpan2.pos_y = ui_submenupanel_y + 155;									// 256+155;
							
							material_colpan3.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
							material_colpan3.pos_y = ui_submenupanel_y + 235;									// 256+235;
							
							material_colpan4.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
							material_colpan4.pos_y = ui_submenupanel_y + 315;									// 256+315;
						}	
				}
			else if (panel == teredit_br_panel)
				{
					if (teredit_brush1_panel && teredit_brush2_panel && teredit_brush3_panel && teredit_brush4_panel && teredit_brush5_panel)
						{
							teredit_brush1_panel.pos_x = ui_submenupanel_x + 16;						// screen_size.x-256+16;
							teredit_brush1_panel.pos_y = ui_submenupanel_y + 45;						// 256+45;
							
							teredit_brush2_panel.pos_x = ui_submenupanel_x + 56;						// screen_size.x-256+16+40;
							teredit_brush2_panel.pos_y = ui_submenupanel_y + 45;						// 256+45;
							
							teredit_brush3_panel.pos_x = ui_submenupanel_x + 96;						// screen_size.x-256+16+80;
							teredit_brush3_panel.pos_y = ui_submenupanel_y + 45;						// 256+45;
							
							teredit_brush4_panel.pos_x = ui_submenupanel_x + 136;						// screen_size.x-256+16+120;
							teredit_brush4_panel.pos_y = ui_submenupanel_y + 45;						// 256+45;
							
							teredit_brush5_panel.pos_x = ui_submenupanel_x + 176;						// screen_size.x-256+16+160;
							teredit_brush5_panel.pos_y = ui_submenupanel_y + 45;						// 256+45;							
						}				
					
					if (teredit_he_panel)
						{
							teredit_he_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
							teredit_he_panel.pos_y 	= ui_submenupanel_y + 128;						// 256+128;
						}
					else if (teredit_lm_panel)
						{
							teredit_lm_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
							teredit_lm_panel.pos_y 	= ui_submenupanel_y + 128;						// 256+128;
						}
					else if (teredit_tx_panel)
						{
							teredit_tx_panel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
							teredit_tx_panel.pos_y 	= ui_submenupanel_y + 128;						// 256+128;
							
							if (teredit_tx1_panel && teredit_tx2_panel && teredit_tx3_panel && teredit_tx4_panel && teredit_tx5_panel)
								{
									teredit_tx1_panel.pos_x = ui_submenupanel_x + 16;								// screen_size.x-256+16;
									teredit_tx1_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
									
									teredit_tx2_panel.pos_x = ui_submenupanel_x + 56;								// screen_size.x-256+16+40;
									teredit_tx2_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
									
									teredit_tx3_panel.pos_x = ui_submenupanel_x + 96;								// screen_size.x-256+16+80;
									teredit_tx3_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
									
									teredit_tx4_panel.pos_x = ui_submenupanel_x + 136;								// screen_size.x-256+16+120;
									teredit_tx4_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
									
									teredit_tx5_panel.pos_x = ui_submenupanel_x + 176;								// screen_size.x-256+16+160;
									teredit_tx5_panel.pos_y = ui_submenupanel_y + 173;								// 256+128+45;
								}
						}					
				}
			else if (panel == sky_lowpanel)
				{
					if (sky_colpan1 && sky_colpan2 && sky_colpan3 && sky_colpan4 && sky_colpan5 && sky_colpan6)
						{
							sky_colpan1.pos_x = ui_submenupanel_x + 40;						// screen_size.x-256+40;
							sky_colpan1.pos_y = ui_submenupanel_y + 470;						// 256+470;
							
							sky_colpan2.pos_x = ui_submenupanel_x + 40;						// screen_size.x-256+40;
							sky_colpan2.pos_y = ui_submenupanel_y + 490;						// 256+490;
							
							sky_colpan3.pos_x = ui_submenupanel_x + 120;						// screen_size.x-256+120;
							sky_colpan3.pos_y = ui_submenupanel_y + 470;						// 256+470;
							
							sky_colpan4.pos_x = ui_submenupanel_x + 120;						// screen_size.x-256+120;
							sky_colpan4.pos_y = ui_submenupanel_y + 490;						// 256+490;
							
							sky_colpan5.pos_x = ui_submenupanel_x + 200;						// screen_size.x-256+200;
							sky_colpan5.pos_y = ui_submenupanel_y + 470;						// 256+470;
							
							sky_colpan6.pos_x = ui_submenupanel_x + 200;						// screen_size.x-256+200;
							sky_colpan6.pos_y = ui_submenupanel_y + 490;						// 256+490;
						}
				}
			else if (panel == sky_skypanel)
				{
					sky_lowpanel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
					sky_lowpanel.pos_y 	= ui_submenupanel_y;								// 256;
					
					if (sky_colpan1 && sky_colpan2 && sky_colpan3 && sky_colpan4 && sky_colpan5 && sky_colpan6)
						{
							sky_colpan1.pos_x = ui_submenupanel_x + 40;						// screen_size.x-256+40;
							sky_colpan1.pos_y = ui_submenupanel_y + 470;						// 256+470;
							
							sky_colpan2.pos_x = ui_submenupanel_x + 40;						// screen_size.x-256+40;
							sky_colpan2.pos_y = ui_submenupanel_y + 490;						// 256+490;
							
							sky_colpan3.pos_x = ui_submenupanel_x + 120;						// screen_size.x-256+120;
							sky_colpan3.pos_y = ui_submenupanel_y + 470;						// 256+470;
							
							sky_colpan4.pos_x = ui_submenupanel_x + 120;						// screen_size.x-256+120;
							sky_colpan4.pos_y = ui_submenupanel_y + 490;						// 256+490;
							
							sky_colpan5.pos_x = ui_submenupanel_x + 200;						// screen_size.x-256+200;
							sky_colpan5.pos_y = ui_submenupanel_y + 470;						// 256+470;
							
							sky_colpan6.pos_x = ui_submenupanel_x + 200;						// screen_size.x-256+200;
							sky_colpan6.pos_y = ui_submenupanel_y + 490;						// 256+490;
						}
				}
			
			wait(1);
		}
}


void		ui_small_panelmove(PANEL* panel)
{
	// difference
	var diff_orig_x = mouse_pos.x - panel.pos_x;
	var diff_orig_y = mouse_pos.y - panel.pos_y;
	
	if (diff_orig_y > (var)32)	return;					// only upper part can be grabbed
	
	// move
	while(mouse_left)
		{
			// use a 2 pixel precision, and for safety no move out of screen
			panel.pos_x = maxv(0, minv( screen_size.x-panel.size_x, integer((mouse_pos.x - diff_orig_x)/2)*2 ));
			panel.pos_y = maxv(0, minv( screen_size.y-panel.size_y, integer((mouse_pos.y - diff_orig_y)/2)*2 ));
			
			ui_submenupanel2_x = panel.pos_x;
			ui_submenupanel2_y = panel.pos_y;
			
			if (panel == sky_colchangepanel)
				{
					if (sky_heightfogpanel)
						{
							sky_heightfogpanel.pos_x = panel.pos_x - 256;
							sky_heightfogpanel.pos_y = panel.pos_y;
						}
				}
			
			wait(1);
		}	
}


void		ui_various_panelmove(PANEL* panel)
{
	// difference
	var diff_orig_x = mouse_pos.x - panel.pos_x;
	var diff_orig_y = mouse_pos.y - panel.pos_y;
	
	if (diff_orig_y > (var)32)	return;					// only upper part can be grabbed
	
	// move
	while(mouse_left)
		{
			// use a 2 pixel precision, and for safety no move out of screen
			panel.pos_x = maxv(0, minv( screen_size.x-panel.size_x, integer((mouse_pos.x - diff_orig_x)/2)*2 ));
			panel.pos_y = maxv(0, minv( screen_size.y-panel.size_y, integer((mouse_pos.y - diff_orig_y)/2)*2 ));
			
			if (panel == sky_pssmpanel)
				{
					ui_submenupanel3_x = panel.pos_x + 256;
					ui_submenupanel3_y = panel.pos_y;
					
					if (sky_pssmpanel2)
						{
							sky_pssmpanel2.pos_x = ui_submenupanel3_x;
							sky_pssmpanel2.pos_y = ui_submenupanel3_y;
						}
				}
			else if (panel == material_scwaterpanel1)
				{
					ui_submenupanel3_x = panel.pos_x + 256;
					ui_submenupanel3_y = panel.pos_y;
					
					if (material_scwaterpanel2)
						{
							material_scwaterpanel2.pos_x = ui_submenupanel3_x;
							material_scwaterpanel2.pos_y = ui_submenupanel3_y;
						}
				}
			else
				{
					ui_submenupanel3_x = panel.pos_x;
					ui_submenupanel3_y = panel.pos_y;
					
					if (sky_pssmpanel)
						{
							sky_pssmpanel.pos_x = ui_submenupanel3_x - 256;
							sky_pssmpanel.pos_y = ui_submenupanel3_y;
						}					
					if (material_scwaterpanel1)
						{
							material_scwaterpanel1.pos_x = ui_submenupanel3_x - 256;
							material_scwaterpanel1.pos_y = ui_submenupanel3_y;
						}
				}			
			
			wait(1);
		}	
}


/////////////////////////////
// gui positions read/write


void	UI_GuiPosInfo_Read()
{
	STRING* temp_str = str_create("");
	var file_handle = file_open_read(ui_guipos);
	if (file_handle)
		{
			File_Read_Asc(file_handle, NULL);
			ui_submenupanel_x = str_to_num( File_Read_Asc(file_handle, temp_str) );
			ui_submenupanel_y = str_to_num( File_Read_Asc(file_handle, temp_str) );
			
			File_Read_Asc(file_handle, NULL);
			ui_submenupanel2_x = str_to_num( File_Read_Asc(file_handle, temp_str) );
			ui_submenupanel2_y = str_to_num( File_Read_Asc(file_handle, temp_str) );
			
			File_Read_Asc(file_handle, NULL);
			ui_submenupanel3_x = str_to_num( File_Read_Asc(file_handle, temp_str) );
			ui_submenupanel3_y = str_to_num( File_Read_Asc(file_handle, temp_str) );
{}
#ifndef MB_RELEASE			
			File_Read_Asc(file_handle, NULL);
			tile_data_panel.pos_x = str_to_num( File_Read_Asc(file_handle, temp_str) );
			tile_data_panel.pos_y = str_to_num( File_Read_Asc(file_handle, temp_str) );
			
			File_Read_Asc(file_handle, NULL);
			cluster_data_panel.pos_x = str_to_num( File_Read_Asc(file_handle, temp_str) );
			cluster_data_panel.pos_y = str_to_num( File_Read_Asc(file_handle, temp_str) );
#else
			File_Read_Asc(file_handle, NULL);
			File_Read_Asc(file_handle, NULL);
			File_Read_Asc(file_handle, NULL);
			
			File_Read_Asc(file_handle, NULL);
			File_Read_Asc(file_handle, NULL);
			File_Read_Asc(file_handle, NULL);
#endif			
			File_Read_Asc(file_handle, NULL);
			def_debug_pan.pos_x = str_to_num( File_Read_Asc(file_handle, temp_str) );
			def_debug_pan.pos_y = str_to_num( File_Read_Asc(file_handle, temp_str) );
			
			file_close(file_handle);
		}
	
	// restrict positions into screen size
	
	ui_submenupanel_x = clamp(ui_submenupanel_x , 0 , screen_size.x-256);
	ui_submenupanel_y = clamp(ui_submenupanel_y , 0 , screen_size.y-512);
	
	ui_submenupanel2_x = clamp(ui_submenupanel2_x , 0 , screen_size.x-256);
	ui_submenupanel2_y = clamp(ui_submenupanel2_y , 0 , screen_size.y-256);
	
	ui_submenupanel3_x = clamp(ui_submenupanel3_x , 0 , screen_size.x-256);
	ui_submenupanel3_y = clamp(ui_submenupanel3_y , 0 , screen_size.y-512);
	
#ifndef MB_RELEASE	
	tile_data_panel.pos_x = clamp(tile_data_panel.pos_x , 0 , screen_size.x-256);
	tile_data_panel.pos_y = clamp(tile_data_panel.pos_y , 0 , screen_size.y-512);
	
	cluster_data_panel.pos_x = clamp(cluster_data_panel.pos_x , 0 , screen_size.x-256);
	cluster_data_panel.pos_y = clamp(cluster_data_panel.pos_y , 0 , screen_size.y-512);
#endif
	
	def_debug_pan.pos_x = clamp(def_debug_pan.pos_x , 0 , screen_size.x-256);
	def_debug_pan.pos_y = clamp(def_debug_pan.pos_y , 0 , screen_size.y-512);
	
	ptr_remove(temp_str);
}


void	UI_GuiPosInfo_Write()
{
	STRING* temp_str = str_create("");
	var file_handle = file_open_write(ui_guipos);
	if (file_handle)
		{			
			File_Write_Asc(file_handle, "// SubMenu Panels");
			File_Write_Asc(file_handle, str_for_num(NULL, ui_submenupanel_x));
			File_Write_Asc(file_handle, str_for_num(NULL, ui_submenupanel_y));
			
			File_Write_Asc(file_handle, "// Small Sub-SubMenu Panels");
			File_Write_Asc(file_handle, str_for_num(NULL, ui_submenupanel2_x));
			File_Write_Asc(file_handle, str_for_num(NULL, ui_submenupanel2_y));
			
			File_Write_Asc(file_handle, "// Big Sub-SubMenu Panels");
			File_Write_Asc(file_handle, str_for_num(NULL, ui_submenupanel3_x));
			File_Write_Asc(file_handle, str_for_num(NULL, ui_submenupanel3_y));
{}
#ifndef MB_RELEASE		
			File_Write_Asc(file_handle, "// Tile Data Panel");
			File_Write_Asc(file_handle, str_for_num(NULL, tile_data_panel.pos_x));
			File_Write_Asc(file_handle, str_for_num(NULL, tile_data_panel.pos_y));
			
			File_Write_Asc(file_handle, "// Cluster Data Panel");
			File_Write_Asc(file_handle, str_for_num(NULL, cluster_data_panel.pos_x));
			File_Write_Asc(file_handle, str_for_num(NULL, cluster_data_panel.pos_y));
#else
			File_Write_Asc(file_handle, "// Tile Data Panel");
			File_Write_Asc(file_handle, str_for_num(NULL, screen_size.x-256));
			File_Write_Asc(file_handle, str_for_num(NULL, 0));
			
			File_Write_Asc(file_handle, "// Cluster Data Panel");
			File_Write_Asc(file_handle, str_for_num(NULL, screen_size.x-256));
			File_Write_Asc(file_handle, str_for_num(NULL, 256));
#endif			
			File_Write_Asc(file_handle, "// Default Debug Panel");
			File_Write_Asc(file_handle, str_for_num(NULL, def_debug_pan.pos_x));
			File_Write_Asc(file_handle, str_for_num(NULL, def_debug_pan.pos_y));
			
			file_close(file_handle);
		}
	ptr_remove(temp_str);
}


/////////////////////////////


#define D3DFILL_POINT		1
#define D3DFILL_WIREFRAME	2
#define D3DFILL_SOLID		3


void		UI_StartWireFrame()
{
	effect_load(mtl_detail,		"obj_wireframe_MB.fx");
	effect_load(mtl_shrub,		"obj_wireframe_MB.fx");
	effect_load(mtl_grass,		"obj_wireframe_MB.fx");
	effect_load(mtl_obstacle,	"obj_wireframe_MB.fx");
	effect_load(mtl_tree,		"obj_wireframe_MB.fx");
	effect_load(mtl_mapent,		"obj_wireframe_MB.fx");
	effect_load(mtl_terrmdl,	"obj_wireframe_MB.fx");
	effect_load(mtl_building,	"obj_wireframe_MB.fx");
	
	effect_load(mat_model,		"obj_wireframe_MB.fx");
	effect_load(mat_sprite,		"obj_wireframe_MB.fx");
	effect_load(mat_flat,		"obj_wireframe_MB.fx");
	effect_load(mat_shaded,		"obj_wireframe_MB.fx");
	
//	if (proc_status(UI_StartWireFrame)>0) return;
//	
//	LPDIRECT3DDEVICE9 pd3dDev = (LPDIRECT3DDEVICE9)draw_begin();
//	
//	if (!ui_wireframe)
//		{
//			pd3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);				
//		}		
//	else
//		{
//			pd3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);		
//		}
}


/////////////////////////////


void		UI_StartD3DLoop()
{
	you = ent_next(NULL);		
	while(you)
		{
			if ( (is(you,INVISIBLE)) && (!is(you,PASSABLE)) )
				{
					UI_D3DLoop(you);						   						   						
				}				
			you = ent_next(you); 
		}		
}


void UI_D3DLoop(ENTITY* ent)
{
	VECTOR vMin,vMax;  	
	VECTOR vTemp;
	
	COLOR cTemp;
	vec_set(cTemp,vector(0,255,0));
	
	while(d3d_lines)
		{
			// draws around the collider cube, but its colllision is different - set higher than physical appearance !
//			vec_for_vertex(vTemp, ent, 1);			
//			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
//			vec_for_vertex(vTemp, ent, 2);
//			draw_line3d(vTemp,cTemp,50);
//			vec_for_vertex(vTemp, ent, 3);
//			draw_line3d(vTemp,cTemp,50);
//			vec_for_vertex(vTemp, ent, 4);
//			draw_line3d(vTemp,cTemp,50);
//			vec_for_vertex(vTemp, ent, 1);
//			draw_line3d(vTemp,cTemp,50);
//			
//			vec_for_vertex(vTemp, ent, 1);			
//			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
//			vec_for_vertex(vTemp, ent, 5);
//			draw_line3d(vTemp,cTemp,50);
//			
//			vec_for_vertex(vTemp, ent, 2);			
//			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
//			vec_for_vertex(vTemp, ent, 6);
//			draw_line3d(vTemp,cTemp,50);
//			
//			vec_for_vertex(vTemp, ent, 3);			
//			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
//			vec_for_vertex(vTemp, ent, 7);
//			draw_line3d(vTemp,cTemp,50);
//			
//			vec_for_vertex(vTemp, ent, 4);			
//			draw_line3d(vTemp,NULL,100); 			// move to corner   , coord,color,alpha
//			vec_for_vertex(vTemp, ent, 8);
//			draw_line3d(vTemp,cTemp,50);
			
			//----------------------------------------------------------------------
			vec_set(vMin,ent.min_x);    
//			vec_rotate(vMin,ent.pan);
//			vec_add(vMin,ent.x);
			
			vec_set(vMax,ent.max_x);
//			vec_rotate(vMax,ent.pan);
//			vec_add(vMax,ent.x);
			
			//----------------------------------------------------------------------
			// rotating vectors to get proper d3d lines
			
			// 2 intermediate lines are needed to avoid e.g. blue-green color fade when obstacle placement cube is applied			
			draw_line3d(nullvector,NULL,100);
			draw_line3d(vector(1,1,1),cTemp,0);
			draw_line3d(nullvector,NULL,100);
			
			vec_set(vTemp,vector(vMin.x,vMin.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,NULL,100);
			
			vec_set(vTemp,vector(vMin.x,vMax.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMax.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMin.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMin.x,vMin.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMin.x,vMin.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMin.x,vMax.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMax.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMin.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMin.x,vMin.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMin.x,vMax.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,NULL,100); 
			
			vec_set(vTemp,vector(vMin.x,vMax.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMax.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,NULL,100); 
			
			vec_set(vTemp,vector(vMax.x,vMax.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMin.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,NULL,100); 
			
			vec_set(vTemp,vector(vMax.x,vMin.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			//----------------------------------------------------------------------
//			// also not good - remains axis aligned...
//			draw_line3d(vMin								 ,NULL ,100); // move
//			draw_line3d(vector(vMin.x,vMax.y,vMin.z),cTemp,100); 
//			draw_line3d(vector(vMax.x,vMax.y,vMin.z),cTemp,100); 
//			draw_line3d(vector(vMax.x,vMin.y,vMin.z),cTemp,100); 
//			draw_line3d(vector(vMin.x,vMin.y,vMin.z),cTemp,100); 
//			draw_line3d(vector(vMin.x,vMin.y,vMax.z),cTemp,100); 
//			draw_line3d(vector(vMin.x,vMax.y,vMax.z),cTemp,100); 
//			draw_line3d(vector(vMax.x,vMax.y,vMax.z),cTemp,100); 
//			draw_line3d(vector(vMax.x,vMin.y,vMax.z),cTemp,100); 
//			draw_line3d(vector(vMin.x,vMin.y,vMax.z),cTemp,100); 	
//			
//			draw_line3d(vector(vMin.x,vMax.y,vMin.z),NULL ,100); // move
//			draw_line3d(vector(vMin.x,vMax.y,vMax.z),cTemp,100); 
//			
//			draw_line3d(vector(vMax.x,vMax.y,vMin.z),NULL ,100); // move
//			draw_line3d(vector(vMax.x,vMax.y,vMax.z),cTemp,100); 
//			
//			draw_line3d(vector(vMax.x,vMin.y,vMin.z),NULL ,100); // move
//			draw_line3d(vector(vMax.x,vMin.y,vMax.z),cTemp,100); 
			
			//--------------------------------------------------------------------
			// not good- axis aligned !!!	
//			draw_box3d(vMin,vMax,vector(0,0,255),100);
			
			wait(1); 
		}
}


/////////////////////////////


void UI_DrawD3DGrid()
{
	//--------------------------------------------------------------------
	// default settings
	
	if (terrain_entity)
		{
			ui_drawd3dgrid = 0;
		}
	else
		{
			ui_drawd3dgrid = 1;
		}
	
	//--------------------------------------------------------------------
	// grid colors
	
	COLOR cTemp, cTempMain;
	vec_set(cTempMain,vector(0,0,0));
	vec_set(cTemp,vector(150,150,150));
	
	//--------------------------------------------------------------------
	// loop
	
	while(UIMode < (var)UIMODE_NEWMAP)
		{
			if ((level_ent) && (ui_drawd3dgrid))
				{
					//--------------------------------------------------------------------
					// get min and max of level rounded to tile size
					
					VECTOR vMin,vMax;  
					
					var lex = integer(level_ent.min_x / GetMapTileSize()) * GetMapTileSize();
					var ley = integer(level_ent.min_y / GetMapTileSize()) * GetMapTileSize();
					var lez = integer(level_ent.min_z / GetMapTileSize()) * GetMapTileSize();
					
					vec_set(vMin, vector(lex, ley, lez));    
					
					lex = integer(level_ent.max_x / GetMapTileSize()) * GetMapTileSize();
					ley = integer(level_ent.max_y / GetMapTileSize()) * GetMapTileSize();
					lez = integer(level_ent.max_z / GetMapTileSize()) * GetMapTileSize();
					
					vec_set(vMax, vector(lex, ley, lez));		
					
					//--------------------------------------------------------------------
					
					// 2 intermediate lines to avoid colour fade through			
					draw_line3d(nullvector,NULL,100);
					draw_line3d(vector(1,1,1),cTemp,0);
					draw_line3d(nullvector,NULL,100);
					
					//--------------------------------------------------------------------
					
					VECTOR	vTemp;
					var		i;
					int 		j;
					
					//--------------------------------------------------------------------
					// y axis lines
					
					j = 0;
					for (i = vMin.x; i <= vMax.x ; i += GetMapTileSize() )
						{
							// 2 intermediate lines to avoid colour fade through				
							draw_line3d(nullvector,NULL,75);
							draw_line3d(vector(1,1,1),cTemp,0);
							draw_line3d(nullvector,NULL,75);
							
							// move to line start pos
							vec_set(vTemp, vector(i, vMin.y, 0));
							draw_line3d(vTemp,NULL,100);
							
							// draw towards line end pos
							vec_set(vTemp, vector(i, vMax.y, 0));
							
							if (j % 10 == 0)
								{
									draw_line3d(vTemp, cTempMain, 75); 			
								}
							else
								{
									draw_line3d(vTemp, cTemp, 75); 		
								}
							
							++j;
						}
					
					//--------------------------------------------------------------------
					// x axis lines
					
					j = 0;
					for (i = vMin.y; i <= vMax.y ; i += GetMapTileSize() )
						{
							// 2 intermediate lines to avoid colour fade through				
							draw_line3d(nullvector,NULL,75);
							draw_line3d(vector(1,1,1),cTemp,0);
							draw_line3d(nullvector,NULL,75);
							
							// move to line start pos
							vec_set(vTemp, vector(vMin.x, i, 0));
							draw_line3d(vTemp,NULL,100);
							
							// draw towards line end pos
							vec_set(vTemp, vector(vMax.x, i, 0));
							
							if (j % 10 == 0)
								{
									draw_line3d(vTemp, cTempMain, 75); 			
								}
							else
								{
									draw_line3d(vTemp, cTemp, 75); 		
								}
							
							++j;	
						}
					
					//--------------------------------------------------------------------
				}
			
			wait(1);
		}
}


/////////////////////////////


#endif