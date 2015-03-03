///////////////////////////////////////////////////////////////////////
// camera control
// defines where mouse touching terrain surface
// shows selected tile by a square-shaped sprite/model
///////////////////////////////////////////////////////////////////////

#ifndef MBcamerahandling_c
#define MBcamerahandling_c


/////////////////////////////////////////////////////////////////////////////
// code


void		Camera_SetDefaultPos()
{
	vec_set(camera.x,vector(0,-2000,2000));
	camera.pan = 90;
	camera.tilt = -45;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////


void 		Camera_Init()
{
	// safety
	Camera_Close();
	wait(3);
	
	// within border
	square_outofborder = 0;
	
	// set camera position and zoom
	camera_height = (var)CAMZ;							
	camera_angle = (var)CAMA; 						// to N
	camera_object_height = (var)CAMH;							
	
	camera_zoomdownlimit = (var)CAMZDWN;					// const
	camera_zoomuplimit = (var)CAMZUP;						// can change
	camera_movespeed = (var)CAMMSP;
	
//	// set camera move limits - in-game parameters
//	camera_object_maxx =  GetMapTilesX() * GetMapTileSize()/2;
//	camera_object_minx = -GetMapTilesX() * GetMapTileSize()/2;
//	camera_object_maxy =  GetMapTilesY() * GetMapTileSize()/2;
//	camera_object_miny = -GetMapTilesY() * GetMapTileSize()/2;	
	
	// set camera move limits - development parameters - can be problem in wmb levels
	camera_object_maxx = TerrHmp_GetGroundMaxX();
	camera_object_maxy = TerrHmp_GetGroundMaxY();
	
	camera_object_minx = TerrHmp_GetGroundMinX();	
	camera_object_miny = TerrHmp_GetGroundMinY();
	
	// necessary function calls - startup	------------------------------		
	
	if (main_camera_mode == (var)1)
		{
			Camera_Init_RTS(); 				// camera_cameraobject entity creation , to be followed by camera		
		}
	else
		{
			Camera_Init_Simple();
		}
	wait(3);		
		
	Camera_Square_Init();   		// camera_tilesquare creation and settings	
	wait(3);				
}


void		Camera_Close()
{
	// close simple camera
	Camera_Simple_Close();
	
	// close RTS camera
	Camera_Object_Close();
	
	// close active tile square positioning
	Camera_Square_Close();	
}


void		Camera_ChangeMode()
{		
	if (main_camera_mode)											// 1 rts to free
		{
			main_camera_mode = 1 - main_camera_mode;
			
			Camera_Object_Close();
			wait(3);
			
			Camera_Init_Simple();
		}
	else																	// 0 free to rts
		{
			main_camera_mode = 1 - main_camera_mode;
			
			Camera_Simple_Close();
			
			Camera_Init_RTS(); 
		}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// simple camera

void		Camera_Simple_Close()
{
	proc_kill2(Camera_Init_Simple , NULL);	
}

void		Camera_Init_Simple()
{
	VECTOR force, speed, dist;
	ANGLE aforce, aspeed; 
	vec_zero(speed);
	vec_zero(aspeed);
	vec_zero(dist);
	
	while(UIMode<(var)UIMODE_NEWMAP)
		{	
			if ((window_focus) && (mouse_valid))
				{
					// move fwd bckw left right
					if (!key_ctrl)										
						{
							if (key_cuu)
								{
									camera.y += sin(camera.pan) * camera_movespeed * time_step + (key_shift * sin(camera.pan) * camera_movespeed * time_step);
									camera.x += cos(camera.pan) * camera_movespeed * time_step + (key_shift * cos(camera.pan) * camera_movespeed * time_step);
								}
							if (key_cud)
								{
									camera.y -= sin(camera.pan) * camera_movespeed * time_step + (key_shift * sin(camera.pan) * camera_movespeed * time_step);
									camera.x -= cos(camera.pan) * camera_movespeed * time_step + (key_shift * cos(camera.pan) * camera_movespeed * time_step);
								}
							if (key_cul)
								{
									camera.y += cos(camera.pan) * camera_movespeed * time_step + (key_shift * cos(camera.pan) * camera_movespeed * time_step);
									camera.x -= sin(camera.pan) * camera_movespeed * time_step + (key_shift * sin(camera.pan) * camera_movespeed * time_step);
								}
							if (key_cur)
								{
									camera.y -= cos(camera.pan) * camera_movespeed * time_step + (key_shift * cos(camera.pan) * camera_movespeed * time_step);
									camera.x += sin(camera.pan) * camera_movespeed * time_step + (key_shift * sin(camera.pan) * camera_movespeed * time_step);
								}
						}
					// move up/down, rotate pan
					else													
						{
							if (key_cuu)
								{
									camera.z += camera_movespeed * time_step + key_shift * camera_movespeed * time_step;
								}
							if (key_cud)
								{
									camera.z -= camera_movespeed * time_step + key_shift * camera_movespeed * time_step;
								}
							if (key_cul)
								{
									camera.pan += camera_rotatespeed * time_step + key_shift * camera_rotatespeed * time_step;
								}
							if (key_cur)
								{
									camera.pan -= camera_rotatespeed * time_step + key_shift * camera_rotatespeed * time_step;
								}
						}
					
					// move up/down
					if (key_home)
								{
									camera.z += camera_movespeed * time_step + key_shift * camera_movespeed * time_step;
								}
					if (key_end)
						{
							camera.z -= camera_movespeed * time_step + key_shift * camera_movespeed * time_step;
						}
						
					// move fwd/bckw
					if ((mickey.z != (var)0) && (!key_ctrl) && (!key_alt))	// shift can be used optionally
						{							
							camera.y += sin(camera.pan) * camera_zoomspeed * time_step * mickey.z + (4 * key_shift * sin(camera.pan) * camera_zoomspeed * time_step * mickey.z);
							camera.x += cos(camera.pan) * camera_zoomspeed * time_step * mickey.z + (4 * key_shift * cos(camera.pan) * camera_zoomspeed * time_step * mickey.z);
							camera.z += sin(camera.tilt) * camera_zoomspeed * time_step * mickey.z + (4 * key_shift * sin(camera.tilt) * camera_zoomspeed * time_step * mickey.z);
						}
					
					// mouse look - rotate tilt, pan					
					if (key_ctrl)
						{
							camera.tilt += camera_rotatespeed*time_step*(key_pgup - key_pgdn + mouse_right*mouse_force.y);
							camera.pan -= camera_rotatespeed*time_step*(mouse_right*mouse_force.x);
						}
					else
						{
							camera.tilt += camera_rotatespeed*time_step*(key_pgup - key_pgdn + mouse_middle*mouse_force.y);
							camera.pan -= camera_rotatespeed*time_step*(mouse_middle*mouse_force.x);
						}
				}
				
			wait(1);
		}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RTS camera and camera-object handling functions

void		Camera_Object_Close()
{
	proc_kill2(Camera_Object_RTS , NULL);
	
	if (camera_cameraobject!=NULL)
		{
			ptr_remove(camera_cameraobject);
			camera_cameraobject = NULL;
		}
}

// set min and max coordinates of camera object
void		Camera_Object_TestLimits()
{
	my.x = maxv(minv(my.x, camera_object_maxx), camera_object_minx);
	my.y = maxv(minv(my.y, camera_object_maxy), camera_object_miny);
}

// camera object to follow
action Camera_Object_RTS()
{
	my.ENT_CATEGORY = (var)UIMODE_CAMERA;
	set(me , PASSABLE | INVISIBLE | FLAG2);
	my.flags2 |= UNTOUCHABLE;
	my.group = 1;	// needed by modifyobjectclass A7 c_tracing as it uses ignore_push because c_ignore does not work - it could be any other dummy entity
	my.material = mtl_placementarea;
	
	var temp = 0;
	VECTOR vTemp;
	
	while(UIMode<(var)UIMODE_NEWMAP)
		{	
			if ((window_focus) && (mouse_valid))
				{
					//--------------------------------------------------------   
					// camera objext x y and movements by mouse and keyboard
					// forward
					if ((mouse_pos.y <= 2) || (key_cuu))
						{
							my.y += sin(camera.pan) * camera_movespeed * time_step + (key_shift * sin(camera.pan) * camera_movespeed * time_step);
							my.x += cos(camera.pan) * camera_movespeed * time_step + (key_shift * cos(camera.pan) * camera_movespeed * time_step);
						}
					// back
					if ((mouse_pos.y >= screen_size.y -2) || (key_cud))
						{
							my.y -= sin(camera.pan) * camera_movespeed * time_step + (key_shift * sin(camera.pan) * camera_movespeed * time_step);
							my.x -= cos(camera.pan) * camera_movespeed * time_step + (key_shift * cos(camera.pan) * camera_movespeed * time_step);
						}
					// left
					if ( ((mouse_pos.x <= 2) && (mouse_pos.y > screen_size.y/2)) || (key_cul))
						{
							my.y += cos(camera.pan) * camera_movespeed * time_step + (key_shift * cos(camera.pan) * camera_movespeed * time_step);
							my.x -= sin(camera.pan) * camera_movespeed * time_step + (key_shift * sin(camera.pan) * camera_movespeed * time_step);
						}
					// right
					if ( ((mouse_pos.x >= screen_size.x -2) && (mouse_pos.y > screen_size.y/2)) || (key_cur))
						{
							my.y -= cos(camera.pan) * camera_movespeed * time_step + (key_shift * cos(camera.pan) * camera_movespeed * time_step);
							my.x += sin(camera.pan) * camera_movespeed * time_step + (key_shift * sin(camera.pan) * camera_movespeed * time_step);
						}
					
					Camera_Object_TestLimits();
					
					// rotate left-------
					if ( ((mouse_pos.x <= 2) && (mouse_pos.y <= screen_size.y/2)) || (key_pgup))
						{
							camera_angle += camera_rotatespeed * time_step;                        
						}
					// rotate right
					if ( ((mouse_pos.x >= screen_size.x -2) && (mouse_pos.y <= screen_size.y/2)) || (key_pgdn))
						{
							camera_angle -= camera_rotatespeed * time_step;                      
						}
					
					// zoom	------------
					if (((!(mickey.z==0)) || (key_home) || (key_end)) && (!key_shift) && (!key_ctrl) && (!key_alt)) 
						{
							if (!(mickey.z==0)) camera_height -= camera_zoomspeed * time_step * mickey.z ;
							if (key_home) camera_height += camera_zoomspeed * time_step *2;
							if (key_end) camera_height -= camera_zoomspeed * time_step *2;
							
							if (camera_height < camera_zoomdownlimit)
								{
									camera_height = camera_zoomdownlimit;
								}	
							if (camera_height > camera_zoomuplimit)
								{
									camera_height = camera_zoomuplimit;
								}
						}
					
					// camera object level
					if (((!(mickey.z==0)) || (key_home) || (key_end)) && (key_shift) && (!key_ctrl) && (!key_alt)) 
						{
							if (!(mickey.z==0)) camera_object_height += camera_zoomspeed * time_step * mickey.z ;
							if (key_home) camera_object_height += camera_zoomspeed * time_step *2;
							if (key_end) camera_object_height -= camera_zoomspeed * time_step *2;
							
							if (camera_object_height < camera_object_minz)
								{
									camera_object_height = camera_object_minz;
								}	
							if (camera_object_height > camera_object_maxz)
								{
									camera_object_height = camera_object_maxz;
								}
						}	
					
					// camera object z ---------------------------------------			
					
					PosInfo3D3OrZeroLevel(my.x, my.y);		
					
					if (HIT_TARGET) 
					{
						temp = my.z;										// previous position
						my.z = hit.z + camera_object_height;		// new pos
						
						temp = my.z - temp;								// difference between previous and current pos
						
						camera_height += temp;							// terrain following camera.z
						camera_zoomdownlimit += temp;
						camera_zoomuplimit += temp;
					}				
					
					// camera x y z ----------------------------------------------
					
					camera.z = camera_height;
					
					// prevent camera from going under surface - and under water
					
					PosInfo3D2OrZeroLevel(camera.x, camera.y);		
					
					if (HIT_TARGET) 
						{
							 camera.z = maxv( camera.z , hit.z + (var)GetMapTileSize() );						 
	//						 camera.z = maxv( camera.z , 0     + (var)GetMapTileSize() );		// ?		 
							 camera_height  = camera.z;
						}			
					camera.x = my.x + camera_height * cos(camera_angle); 
					camera.y = my.y + camera_height * sin(camera_angle); 				
					
					vec_set(vTemp,my.x);
					vec_sub(vTemp,camera.x);
					vec_to_angle(camera.pan,vTemp);              
					camera_angle %= 360;    
					
					//--------------------------------------------------------------
					
					camera_movespeed = (var)CAMMSP + camera_height/40;
					
				}
			
			wait(1);
		}
}

// place camera object entity to be followed by camera
void		Camera_Init_RTS()
{  	
	collision_mode = 0; 
	camera_cameraobject = ent_create(camera_object_str, vector(16,16,camera_object_height), Camera_Object_RTS);
	collision_mode = 1; 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// camera_tilesquare related functions


void	Camera_Square_Close()
{
	proc_kill2(Camera_Square , NULL);		
	if (camera_tilesquare!=NULL)
		{
			ptr_remove(camera_tilesquare);
			camera_tilesquare = NULL;	
		}	
}


// snapping to tile midpoint, within grid limits - limit if not free placement mode is active - i.e. in free placement mode no limiting
void		Camera_SnapIt()
{  
	if (my.x < 0)
		{
			my.x = GetMapTileSize() * (integer(my.x / GetMapTileSize())) - (GetMapTileSize() / 2);		
//			// limit   
//			if (my.x < -((GetMapTilesX() / 2) * GetMapTileSize() - (GetMapTileSize() / 2)))
//				{
//					my.x = -((GetMapTilesX() / 2) * GetMapTileSize() - (GetMapTileSize() / 2));
//				}
		}
	else if (my.x >= 0)
		{
			my.x = GetMapTileSize() * (integer(my.x / GetMapTileSize())) + (GetMapTileSize() / 2);
			// limit
//			if (my.x > ((GetMapTilesX() / 2) * GetMapTileSize() - (GetMapTileSize() / 2)))
//				{
//					my.x = ((GetMapTilesX() / 2) * GetMapTileSize() - (GetMapTileSize() / 2));
//				}
		}       
	
	if (my.y < 0)
		{
			my.y = GetMapTileSize() * (integer(my.y / GetMapTileSize())) - (GetMapTileSize() / 2);
			// limit
//			if (my.y < -((GetMapTilesY() / 2) * GetMapTileSize() - (GetMapTileSize() / 2)))
//				{
//					my.y = -((GetMapTilesY() / 2) * GetMapTileSize() - (GetMapTileSize() / 2));
//				}
		}
	else if (my.y >= 0)
		{
			my.y = GetMapTileSize() * (integer(my.y / GetMapTileSize())) + (GetMapTileSize() / 2);
			// limit
//			if (my.y > ((GetMapTilesY() / 2) * GetMapTileSize() - (GetMapTileSize() / 2)))
//				{
//					my.y = ((GetMapTilesY() / 2) * GetMapTileSize() - (GetMapTileSize() / 2));
//				}
		}  
}

// determine camera_tilesquare x,y tile and tilearray position and value, for further calculations and display
void		Camera_ShowPosition()
{  
	// safety check to avoid invalid calls
	if (UIMode>=(var)UIMODE_NEWMAP)
		{
			return;
		}		
	
	if (camera_tilesquare)
		{
			square_coord_x = camera_tilesquare.x;
			square_coord_y = camera_tilesquare.y;
			square_coord_z = camera_tilesquare.z;
			
			square_x = XCoordToTile( camera_tilesquare.x );
			square_y = YCoordToTile( camera_tilesquare.y );
//			square_z = 0;
		}
	
	//--------------------------
	// out of border
	if ( !XYTileValid(square_x, square_y) )
		{
			square_outofborder = 1;
			return;
		}	
	else
		{
			square_outofborder = 0;
		}
	
	//-----------------------------
	// if activeTileSquare is out of game border, these values show the last valid value, i.e. protected from having invalid values:
	
	square_abs = XYTileToAbsTile(square_x, square_y);				// = [square_x][square_y]
	
	// ###--------------------------------------------------------------------------
	
#ifndef MB_RELEASE	
	
	// calculate FPS (stabilized) - should be moved to a better place
	posinf_fps = 0.9*posinf_fps+0.1/time_frame;
	
	if ((TileArrayLength == 0) || (ClusterArrayLength == 0))
		{
			return;
		}
	
//	square_z = (var)Entity_GetZPos3D(&camera_tilesquare, GetMapZLevels());												// notokay
	square_z = (var)Vector_GetZPos3D(camera_tilesquare.x, GetMapZLevels());												// okay
//	if ( AbsTileValid(square_abs) )																									// okay
//		{						
//			square_z = 0;														// 0-1-2
//			
//			// find closest level based on midz and square distance comparisons
//			if (BuildingTile(square_abs))									// 1st or 2nd floor if building, i.e. 0 or 1
//				{					
//					var tempdiff = 10000;																								// init a huge value					
//					int i;
//					for (i=0; i<(GetMapZLevels()+1); i++)																			// i: 0,1,2
//						{
//							if (GetTileMidZ(square_abs, i) != (var)NO_Z_APPLIED)												// valid value - 0 should be changed to a negative value - done
//								{
//									if ( abs(camera_tilesquare.z - GetTileMidZ(square_abs, i)) <= tempdiff )			// = needed because if 0-1 is the same in case of bridge hole area - or a i-- loop needed!	
//										{
//											tempdiff = abs(camera_tilesquare.z - GetTileMidZ(square_abs, i));
//											square_z = i;									
//										}
//								}
//						}
//					camera_tilesquare.z = GetTileMidZ(square_abs, square_z);													// snap z
//				}
//		}
	//---------------------------------------------------------------------------------------
	
	//to display the value stored in the actual node
	square_value0 	= GetTileType(square_abs, 0);
	square_value1 	= GetTileType(square_abs, 1);
	square_value2 	= GetTileType(square_abs, 2);	
	
	square_min 		= GetTileMinZ(square_abs);
	square_mid0 	= GetTileMidZ(square_abs, 0);		
	square_mid1 	= GetTileMidZ(square_abs, 1);			
	square_mid2 	= GetTileMidZ(square_abs, 2);			
	square_max 		= GetTileMaxZ(square_abs);
	
	square_clear0	= GetTileUnitSize(square_abs, 0);
	square_clear1	= GetTileUnitSize(square_abs, 1);
	square_clear2	= GetTileUnitSize(square_abs, 2);
	square_clear3	= GetTileUnitSize(square_abs, 3);
	
	square_clearmask0	= GetTileUnitSizeMask(square_abs, 0);
	square_clearmask1	= GetTileUnitSizeMask(square_abs, 1);
	
	square_cost0	= GetTileCost(square_abs, 0);
	square_cost1	= GetTileCost(square_abs, 1);
	square_cost2	= GetTileCost(square_abs, 2);
	
	square_clus0	= GetTileClusterID(square_abs, 0);
	square_clus1	= GetTileClusterID(square_abs, 1);
	
	square_buil		= GetTileBuildingID(square_abs);
	
	//--------------------------------------------------------------------------------------
	
	PosInf_Show_ClusterData();
	
#endif
}


// set camera_tilesquare place and properties, and update in each frame
action		Camera_Square()
{  
	proc_mode = PROC_EARLY;
	        
	var temp;
	// give an entity name to square sprite eablig tracking x,y coordinates
//	camera_tilesquare = me;
	
	my.ENT_CATEGORY = (var)UIMODE_ACTIVETILE;
	set(me, PASSABLE | FLAG2 | ZNEAR);																							// passable to avoid c_tracing,  | ZNEAR
	my.flags2 |= UNTOUCHABLE;
	my.material = mtl_placementarea;
	
{}
#ifdef MB_RELEASE	
	set(me, INVISIBLE);
#endif	
	my.scale_x = GetMapTileSize() / MAP_DEFAULTGRIDSIZE / 2;
	my.scale_y = GetMapTileSize() / MAP_DEFAULTGRIDSIZE / 2;
//	// horizintal alignment
//	vec_set( my.pan , vector(270,90,0) ); 
	
	wait(3);
	
	while(UIMode<(var)UIMODE_NEWMAP)
		{								
			if ((window_focus) && (mouse_valid))
				{
					VECTOR* surfacepos_vec = NULL;
					
					if ((UIMode != (var)UIMODE_PLAYTEST02) && (UIMode!=(var)UIMODE_MAPINFO))
						{
							// editor mode
							surfacepos_vec = MousePos3D1OrZeroLevel();														// call c_tracing to get 3D position on terrain		
						}
					else
						{
							// rts test mode
							surfacepos_vec = MousePos3D2();																		// call c_tracing to get 3D position on any surface except FLAG2 and water
						}
					
					// move square only if mouse points within terrain - else it remains in previous position		
					if (surfacepos_vec)  																							// without this check it would point to Node (0,0) if mouse points out of terrain
						{
							vec_set(my.x, surfacepos_vec);									
							
//							Camera_SnapIt();  																						// snap to middle of tile
							vec_set(my.x, SnapCoordToTileMidCoordXY(&my.x));
							
							Camera_ShowPosition();								
						}		
				}
			wait(1);
		}
}


// create camera_tilesquare
void		Camera_Square_Init()
{
	collision_mode = 0; 
	camera_tilesquare = ent_create(camera_square2_str, nullvector, Camera_Square);	
	collision_mode = 1; 
}

//////////////////////////////////////////

//// set proper camera_tilesquare skin showing actual buildmode
//void		ChangeMode()
//{ 
//	if (UIMode == (var)UIMODE_MAPINFO)
//	{
//		ent_morph(camera_tilesquare, camera_square2_str);	// blue
//	}	
//	else if (UIMode == (var)UIMODE_STARTUP)
//	{
//		ent_morph(camera_tilesquare, camera_square3_str);	// red
//	}
//	else
//	{
//		ent_morph(camera_tilesquare, camera_square1_str);	// green
//	}	
//}


#endif