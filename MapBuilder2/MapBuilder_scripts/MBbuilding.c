
#ifndef MBbuilding_c
#define MBbuilding_c

////////////////////////////////////////////////////////////////////////////////////////////////
// code


////////////////////////////////////////////////////////////////////////////////////
// create


void		Building_Create(	STRING* filename, 
									var obs_defaultsize,
									obs_tilesize_x, obs_tilesize_y,
									obs_type, obs_collision, obs_shadow,
									var my_scale_x, my_scale_y, my_scale_z,		
									my_x, my_y, my_z, 
									my_pan, my_tilt, my_roll,
									my_alpha, my_ambient, 
									my_light, my_red, my_green, my_blue, 
									my_unlit, my_overlay, my_material,										
									my_flag6, my_flag5)
{
	if (!filename) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, filename);
	
	//----------------------------------------------------------------------------
	
	ENTITY* ent = ent_create(temp_str, vector(my_x, my_y, my_z), NULL);						// changing my to ent resolved the strange ui loop stopping bug
	
//	while(!ent) {wait(1);}
	
			if (ent != NULL)										// check, or while/wait
				{
					//------------------------------------------------------------------------------------------------------------
					
					ent.scale_x		= my_scale_x;
					ent.scale_y		= my_scale_y;
					ent.scale_z		= my_scale_z;
					
//					if (obs_type != (var)2)						// not building - removed, can be rotated
						{
							// salefy keep angles between -180...180 degree				
							ent.pan			= ang(my_pan);
							ent.tilt			= ang(my_tilt);
							ent.roll			= ang(my_roll);
						}										
					
					//------------------------------------------------------------------------------------------------------------
					
//					set(ent , FLAG2);								// FLAG2 should be reset for buildings afterwards !!!				
										
					//------------------------------------------------------------------------------------------------------------
					
					ent.alpha   	= 100;
					ent.ambient		= my_ambient;		

					if (my_alpha < (var)98)						// my_translucent could be used	
						{
							set( ent , TRANSLUCENT );
//							reset( ent , OVERLAY );
							ent.alpha  = my_alpha;				
						}
					
					if (my_overlay)
						{
							set( ent , OVERLAY );
							reset( ent , TRANSLUCENT );
							ent.alpha	= 100;
						}	
							
					//------------------------------------------------------------------------------------------------------------
					// entity specific color
					
					if (my_light)
						{
							set(   ent , LIGHT );
							reset( ent , UNLIT );
							ent.lightrange 	= 0;
							ent.red     		= my_red;
							ent.green   		= my_green;
							ent.blue    		= my_blue;			
						}	
					
					//------------------------------------------------------------------------------------------------------------
					// general materials	
					
					ent.material 	= mtl_building;
										
					if (my_unlit)
						{
							set(   ent , UNLIT );					// keeps ambient settings
							reset( ent , LIGHT );
							ent.material 	= mat_unlit;
						}
					else if (my_material==1)	
						{
//							reset( ent , UNLIT );
							ent.material 	= mat_model;		
						}
					
					//---------------------------------------------------------------------------------------------
					// store data in skills
					
					// set tile pos
					ent.XTILE_POS = XCoordToTile(ent.x);
					ent.YTILE_POS = YCoordToTile(ent.y);
					ent.ZTILE_POS = 0;
					ent.ABSTILE_POS = XYTileToAbsTile(ent.XTILE_POS, ent.YTILE_POS);
					
					// set entity type and size
					ent.ENT_CATEGORY = (var)UIMODE_BUILDING;
//					ent.ENT_CLIP     = 1;													//  clipping at 75%
//					ent.ENT_ANIM     = 0;	
#ifndef MB_RELEASE	
					ent.ENT_HANDLE   = handle(ent);										// used by the RTS building cluster creation system
#endif
					ent.AREA_SIZEX   = obs_tilesize_x; 								// * (my_scale_x*obs_defaultsize/GetMapTileSize());	// should take into account scaling - only refining scaling is allowed
					ent.AREA_SIZEY   = obs_tilesize_y; 								// * (my_scale_x*obs_defaultsize/GetMapTileSize());	// should take into account scaling - only refining scaling is allowed
					ent.ENT_TYPE 	 = obs_type; 										// store tile passability for pathfinder *** new ***
					
					// collision group
					ent.group = 3;
					
					//------------------------------------------------------------------------------------------------------------
					// set clipping
					
//					STRING* temp5_str = str_create("");
//					str_for_entfile(temp5_str,me);
//					if (str_stri(temp5_str,"_0.")==(var)0)
//						{
//							if ( (my.AREA_SIZEX < 5) && (my.AREA_SIZEY < 5) )
								{
									ent.eflags |= CLIP0;	// 75%
								}
//						}
//					ptr_remove(temp5_str);	
					
					//--------------------------------------------------------------------------------------------------------------	
					// set collision			
							
					wait(1);									// needed after scaling and before update collision size	
					
					c_setminmax(ent);
					
					if (obs_collision==(var)0)
						{
							set(ent, PASSABLE);
						}
					else if (obs_collision==(var)1)
						{
							set(ent, POLYGON);
						}
					else if (((shadow_stencil>(var)-1) && (shadow_stencil<(var)8)) || (my_flag5>0))	//(obs_collision==(var)2)		// flag5 means building_decal
						{
							// to set bounding box to extends because of min_z and max_z 
							// or before bounding box vec_scale is used	
							set(ent, PASSABLE);							
							
							Building_CreateCollider(ent);
						}									
					
					//--------------------------------------------------------------------------------------------------------------	
					// manually set bounding box because of decal shadow size
					
					if (((shadow_stencil>(var)-1) && (shadow_stencil<(var)8)) || (my_flag5>0))
						{
							ent.eflags |= FAT | NARROW; 													// set both flags to prevent automatic recalculation on scale changes			
							ent.min_z = 0;
							ent.min_x = - maxv( abs(ent.min_x) , ent.max_z/2 );	
							ent.max_x = - ent.min_x;
							ent.min_y = - maxv( abs(ent.min_y) , ent.max_z/2 );	
							ent.max_y = - ent.min_y;
						}
					else		// -1 or 8
						{
							ent.eflags |= FAT | NARROW; 
							ent.min_z = 0;
							ent.min_x = - GetMapTileSize()/2 * obs_tilesize_x;						// * (my_scale_x*obs_defaultsize/GetMapTileSize());
							ent.max_x = - ent.min_x;
							ent.min_y = - GetMapTileSize()/2 * obs_tilesize_y;						// * (my_scale_x*obs_defaultsize/GetMapTileSize());
							ent.max_y = - ent.min_y;
						}
					
					//--------------------------------------------------------------------------------------------------------------	
					// set shadow type
	
					// include in lightmap, force ent_decal, shadow no/dynamic/sprite/model
					Building_SetShadow(ent, (1-my_flag6), my_flag5, obs_shadow);
					wait_for(Building_SetShadow);															// needed because of decal and ent_decal shadows, to wait for completion before setting entity to static
					
					//--------------------------------------------------------------------------------------------------------------	
					
					building_counter++;
					
					ent.emask &= ~DYNAMIC;																	// set to static for faster rendering
							
//					Building_Created_Loop(ent);															// it can result in too many running functions !!! and entity cannot be static
					
					//--------------------------------------------------------------------------------------------------------------						
					// safety nexus check - editor only!
					
#ifndef MB_GAME					
					Map_NexusCheck();
#endif
					//---------------------------------------------------------------------------------------------------
					// level_ent
					
					if (!terrain_entity)
						{
							SentLevelEntSizeToEnt(ent);
						}
					
					//---------------------------------------------------------------------------------------------------
					// manage RTS map system - rts only!
					
//#ifndef MB_RELEASE					
//	#ifdef MAPENT_ADD_BUILDING_CLUSTERS
//					Terrain_SetBuildingTilesClusters(ent, building_foldername);					// done for all buildings on game init
//	#endif
//#endif
					//---------------------------------------------------------------------------------------------------
				}
	
	ptr_remove(temp_str);
}


void		Building_CreateCollider(ENTITY* ent)
{	
	// create a 16x16x16 quant cube
	ENTITY* obs_collider = ent_create( "graphics\\camera_cube.mdl" , ent.x , NULL);				// ent.x was additional_vec
	while (!obs_collider) { wait(1); }
	
	//-------------------------------------------------------
	set( obs_collider , INVISIBLE );
	// to test comment INVISIBLE line, and uncomment these lines:
//	set( obs_collider , TRANSLUCENT );
//	obs_collider.alpha = 50;						
	
	set(obs_collider , FLAG2 | FLAG1 | FLAG6);
	obs_collider.flags2 |= UNTOUCHABLE;
//	obs_collider.pan = ent.pan;																					// ONLY IN FREE EDITION - IT SHOULD BE 0,90,180 OR 270 DEGREE ONLY !!!!!!!!!!!!!!!
	obs_collider.scale_x = GetMapTileSize()/16 * ent.AREA_SIZEX;										// 0.5*tilesize - 16 is the cube size
	obs_collider.scale_y = GetMapTileSize()/16 * ent.AREA_SIZEY;
	obs_collider.scale_z = GetMapTileSize()/16 ;																// does not matter, but better for debugging if small
	obs_collider.ENT_CATEGORY = (var)UIMODE_BUILDING + (var)UIMODE_SHADOWSHIFT;
	
	obs_collider.XTILE_POS = ent.XTILE_POS;
	obs_collider.YTILE_POS = ent.YTILE_POS;
	
	obs_collider.group = 1;	
	
	//-------------------------------------------------------
	// to manually set bounding box
	
	wait(1);	
	
	c_setminmax(obs_collider);
	obs_collider.eflags |= FAT | NARROW; 																		// set both flags to prevent automatic recalculation on scale changes			
//	obs_collider.min_x = -GetMapTileSize()/2 * building_tilesize_x * building_scale;				// required ?
//	obs_collider.min_y = -GetMapTileSize()/2 * building_tilesize_y * building_scale;				// required ?
//	obs_collider.max_x =  GetMapTileSize()/2 * building_tilesize_x * building_scale;				// required ?
//	obs_collider.max_y =  GetMapTileSize()/2 * building_tilesize_y * building_scale;				// required ?			
	obs_collider.min_z =  0;																						// required !
	obs_collider.max_z =  ent.max_z;																				// required !	
	
	ent.parent = obs_collider;																						// in fact child, but used in this way for easy deletion
	
	//-------------------------------------------------------
	
	obs_collider = NULL;
	ptr_remove(obs_collider);	
}


//void		Building_Created_Loop(ENTITY* ent)
//{
//	ent.skill1 = sky_loddistance2;
//	while(1)
//		{
//			if (vec_dist (ent.x, camera.x) < ent.skill1) 									// the camera has come close to this entity?
//				{
//					reset(ent,INVISIBLE); 														// ent.invisible = off; // then make it visible (however, alpha = 0 at this point)
//					while (ent.alpha < 100) 														// run this loop until the entity becomes opaque again
//					{
//						ent.alpha = minv(100, ent.alpha + 15 * time_step); 				// increase ent.alpha (15 = speed) and limit it to 100
//						wait (1);
//					}
//					reset(ent,TRANSLUCENT);														// ent.transparent = off; // get rid of some nasty artifacts when the entity is completely visible
//				}
//			else 																						// the player has moved away from this entity?
//				{
//					set(ent,TRANSLUCENT); 														// ent.transparent = on; // then set the "transparent" flag again
//					while (ent.alpha > 0) 														// run this loop until the entity becomes practically invisible (alpha = 0)
//					{
//						ent.alpha = maxv(0, ent.alpha - 15 * time_step); 					// decrease ent.alpha (15 = speed) and limit it to 0
//						wait (1);
//					}
//					set(ent,INVISIBLE); 															// ent.invisible = on; // now hide the entity for good, increasing the frame rate
//				}
//			wait(1);
//		}
//}


void		Building_ReadObsFileData(	STRING* my_filename, 
												var* obs_defaultsize,
												var* obs_tilesize_x,
												var* obs_tilesize_y,
												var* obs_type,
												var* obs_collision,
												var* obs_shadow
											)
{
	STRING* temp_str0 = str_create("");
	//-----------------------------------------------------------------------
	// get filename from model name
	str_cpy( temp_str0 , building_foldername );
	str_cat( temp_str0 , my_filename );
	str_trunc( temp_str0 , 4 ); 
	str_cat( temp_str0 , ".obs" );	
	//-----------------------------------------------------------------------
	var file_handle = file_open_read( temp_str0 );
	//-----------------------------------------------------------------------	
	if (file_handle)
		{
			// read file if exists
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			*obs_defaultsize = str_to_num( temp_str0 );
			
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			*obs_tilesize_x = str_to_num( temp_str0 );
			
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			*obs_tilesize_y = str_to_num( temp_str0 );
			
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			*obs_shadow = str_to_num( temp_str0 );
			
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			*obs_type = str_to_num( temp_str0 );
			
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			*obs_collision = str_to_num( temp_str0 );
			
			file_close(file_handle);	
		}
	else
		{
			// defaults
			*obs_defaultsize		= (var)MAP_DEFAULTGRIDSIZE;
			*obs_tilesize_x		= 1;
			*obs_tilesize_y		= 1;
			*obs_type				= 0;								// (means passability) 0 no access, 1 default (passable), 2 building, (3 forest)
			*obs_collision			= 2;								// 0 passable, 1 polygonal (for real buildings), 2 tile area (cheaper, for blocking objects)
			*obs_shadow				= 2;								// 0 nothing, 1 dynamic, 2 sprite, 3 model (2-3 switches to 1 in case of shadowmapping)
		}
	//-----------------------------------------------------------------------
	ptr_remove(temp_str0);
}


////////////////////////////////////////////////////////////////////////////////////
// shadows


void	Building_SetShadow(ENTITY* ent, var build_lightmap, var build_decal, var build_shadow)
{	
	if (build_shadow == (var)0)																			// no shadow
		{
			Building_SetNoshadow(ent);
			wait_for(Building_SetNoshadow);
		}
	else if (build_lightmap == (var)1)																	// static lightmap shadow
		{
			Building_SetLightmap(ent);
			wait_for(Building_SetLightmap);
		} 
	else																											// dynamic shadow
		{						
			if (shadow_stencil == (var)-1)																// sc esm shadowmapping
				{
					if (build_decal<1)
						{
							Building_SetSCSM(ent);
							wait_for(Building_SetSCSM);
						}
					else																							// enforce entdecal shadow
						{
							set(ent, FLAG5);
							Building_SetEntDecal(ent);
							wait_for(Building_SetEntDecal);
						}
				}
			else if (shadow_stencil == (var)8)															// pssm shadowmapping
				{
					if (build_decal<1)
						{
							Building_SetPSSM(ent);
							wait_for(Building_SetPSSM);
						}
					else																							// enforce entdecal shadow
						{
							set(ent, FLAG5);
							Building_SetEntDecal(ent);
							wait_for(Building_SetEntDecal);
						}	
				}
			else
				{					
					if (build_shadow==(var)1)																// stencil shadow	
						{
							if (build_decal<1)
								{
									Building_SetStencil(ent);
									wait_for(Building_SetStencil);
								}
							else																					// enforce sprite shadow (decal or entdecal)
								{
									Building_SetSprite(ent, build_decal);
									wait_for(Building_SetSprite);
								}
								
						}
					else if (build_shadow==(var)2)														// sprite shadow (decal or entdecal)
						{
							Building_SetSprite(ent, build_decal);
							wait_for(Building_SetSprite);
						}
					else if (build_shadow==(var)3)														// model shadow
						{
							if (build_decal<1)
								{
									Building_SetModel(ent);
									wait_for(Building_SetModel);
								}
							else																					// enforce sprite shadow (decal or entdecal)
								{									
									Building_SetSprite(ent, build_decal);
									wait_for(Building_SetSprite);
								}
						}					
					else																							// protetction (no shadow)
						{
							Building_SetNoshadow(ent);
							wait_for(Building_SetNoshadow);
						}					
				}
		}	
}


//-----------------------------------------------------------------


void	Building_SetNoshadow(ENTITY* ent)
{
	set(ent, FLAG6);																							// exclude from lightmap
	reset(ent, SHADOW);
	reset(ent, FLAG5);																						// no entdecal enforced
	
	if (shadow_stencil==(var)8)
		{
			if (AlphaMaterialTest(ent)>0)
				{
					set(ent, CAST);																			// no self shadow on alpha material objects
				}
		}
}


void	Building_SetLightmap(ENTITY* ent)
{
	reset(ent, FLAG6);																						// include in lightmap
	reset(ent, FLAG5);																						// no entdecal enforced
	
	if (shadow_stencil==(var)-1)
		{
			set(ent, SHADOW | CAST);																		// no shadow cast, only self shadows
		}
	else if (shadow_stencil==(var)8)
		{
			reset(ent, SHADOW);
			
			if (AlphaMaterialTest(ent)>0)
				{					
					set(ent, CAST);																			// no self shadow on alpha material objects
				}
		}
	else
		{
			reset(ent, SHADOW);
		}
}


void	Building_SetSCSM(ENTITY* ent)
{
	set(ent , FLAG6);																							// exclude from lightmap
	set(ent, SHADOW);
	reset(ent, FLAG5);																						// no entdecal enforced
}


void	Building_SetPSSM(ENTITY* ent)
{
	set(ent, FLAG6);																							// exclude from lightmap
	set(ent, SHADOW);
	reset(ent, FLAG5);																						// no entdecal enforced
	
	if ((AlphaMaterialTest(ent)>0) || (is(ent,OVERLAY)))
		{
			set(ent, CAST);																					// no self shadow on alpha material objects
		}
}


void	Building_SetStencil(ENTITY* ent)
{
	set(ent, FLAG6);																							// exclude from lightmap
	set(ent, SHADOW);	
	reset(ent, FLAG5);																						// no entdecal enforced
	
	if (AlphaMaterialTest(ent)>0)
		{
			set(ent, CAST);																					// no self shadow possible on alpha material objects
		}
}


void	Building_SetSprite(ENTITY* ent, var build_decal)										// called only in case of shadow_stencil 0 and 1
{	
	if ((!main_blur) && (build_decal<1))																// no stencil blur applied, no entdecal enforced
		{
			Building_SetDecal(ent);
			wait_for(Building_SetDecal);
		}
	else
		{
			if (build_decal)
				{
					set(ent, FLAG5);																			// enforced decal
				}			
			Building_SetEntDecal(ent);
			wait_for(Building_SetEntDecal);
		}	
}


void	Building_SetDecal(ENTITY* ent)
{
	wait(1);																										// because bounding box size dependency !!!
	
	set(ent, FLAG6);																							// exclude from lightmap
	set(ent, SHADOW);
	
	STRING* temp_str = str_create("");																	// check for own .dds shadow sprite in shadow folder	
	str_cpy(temp_str, shadow_foldername);
	str_cat(temp_str, str_for_entfile(NULL, ent));
	str_trunc(temp_str, 4);
	str_cat(temp_str, "_shadow.dds");
		
	if ( file_exists(temp_str) )																			// use own shadow image
		{			
			ENTITY* temp_ent = ent_create(temp_str, nullvector, NULL);							// entities must be created to be saved into wmb
			while (!temp_ent) {wait(1);}
			
			set(temp_ent, INVISIBLE | PASSABLE | FLAG1 | FLAG2);
			temp_ent.flags2 |= UNTOUCHABLE;
			temp_ent.ENT_CATEGORY = (var)UIMODE_BUILDING + (var)UIMODE_SHADOWSHIFT;
			temp_ent.XTILE_POS = XCoordToTile(ent.x);
			temp_ent.YTILE_POS = YCoordToTile(ent.y);
			temp_ent.parent = ent;																			// ent's parent can be the collider entity
			
			ent.shadow = temp_ent;					
			
			temp_ent = NULL;	
			ptr_remove(temp_ent);
		}
	else																											// use common shadow images
		{
			ent.shadow = ent_unitshadow_rot; 	
		}
		
	wait(1);
//	ent.y += 32;
	reset(ent, SHADOW);
	wait(2);
//	ent.y -= 32;
	set(ent, SHADOW);
	wait(1);
}


void	Building_SetEntDecal(ENTITY* ent)
{
	wait(1);																										// because bounding box size dependency !!!
	
	set(ent , FLAG6);																							// exclude from lightmap
	reset(ent, SHADOW);
	
	STRING* temp_str = str_create("");																	// check for own .dds shadow sprite in shadow folder	
	str_cpy(temp_str, shadow_foldername);
	str_cat(temp_str, str_for_entfile(NULL, ent));
	str_trunc(temp_str, 4);
	str_cat(temp_str, "_shadow.dds");
		
	if ( file_exists(temp_str) )																			// use own shadow image
		{			
			ENTITY* temp_ent = ent_create( temp_str , nullvector , NULL);						// entities must be created to be saved into wmb
			while (!temp_ent) {wait(1);}
			
			set(temp_ent, INVISIBLE | PASSABLE | FLAG1 | FLAG2);
			temp_ent.flags2 |= UNTOUCHABLE;
			temp_ent.ENT_CATEGORY = (var)UIMODE_BUILDING + (var)UIMODE_SHADOWSHIFT;
			temp_ent.XTILE_POS = XCoordToTile(ent.x);
			temp_ent.YTILE_POS = YCoordToTile(ent.y);
			temp_ent.parent = ent;																			// ent's parent can be the collider entity
			
			Decal_Set_Decal(ent, bmap_for_entity(temp_ent,0));			
			
			temp_ent = NULL;	
			ptr_remove(temp_ent);
		}
	else																											// use common shadow images
		{
			Decal_Set_Decal(ent, bmap_for_entity(ent_unitshadow_rot,0));
		}
}


void	Building_SetModel(ENTITY* ent)
{
	set(ent , FLAG6);																							// exclude from lightmap
	set(ent, SHADOW | CAST);																				// | CAST : no self shadow
	reset(ent, FLAG5);																						// no entdecal enforced
	
	// check for own .dds shadow sprite in shadow folder
	STRING* temp3_str = str_create("");
	str_for_entfile(temp3_str, ent);
	
	STRING* temp4_str = str_create("");
	str_cpy(temp4_str, shadow_foldername);
	str_cat(temp4_str, temp3_str);
	str_trunc(temp4_str,4);
	str_cat(temp4_str,"_shadow.mdl");
	
	// use own shadow model
	if ( file_exists(temp4_str) )
		{
			ENTITY* temp_ent = ent_create( temp4_str , nullvector , NULL);
			while (!temp_ent) {wait(1);}
			
			set(temp_ent, INVISIBLE | PASSABLE | FLAG1 | FLAG2);
			temp_ent.flags2 |= UNTOUCHABLE;
			temp_ent.ENT_CATEGORY = (var)UIMODE_BUILDING + (var)UIMODE_SHADOWSHIFT;
			temp_ent.XTILE_POS = XCoordToTile(ent.x);
			temp_ent.YTILE_POS = YCoordToTile(ent.y);
			temp_ent.parent = ent;																			// ent's parent can be the collider entity
					
			ent.shadow = temp_ent;	
			
			temp_ent = NULL;	
			ptr_remove(temp_ent);			
		}
	else
		{			
			ent.shadow = ent_model_unitshadow;															// shadow model - a simle symmetric model casting shadow intead of the actual model - does not rotates yet so must be symmetric
		}
}


////////////////////////////////////////////////////////////////////////////////////
// save/load


void		Building_Save_Buildings_blm(char* file_name)
{
{}
#ifndef MB_GAME	
	// save panel
	set( map_loadpanel , SHOW);
	set( map_savetext1 , SHOW);
	wait(1);
#endif	
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_write(temp_str);
	//---------------------------------------------
	// entity count
	str_for_num( 			temp_str 	, building_counter );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
		
	// entity data - entity placed first will be written into file last
	you = ent_next(NULL); 									// get first entity
 	while (you) 												// repeat until there are no more entities
		{
			if ((you.ENT_CATEGORY == (var)UIMODE_BUILDING) && (!is(you,FLAG8)))				// if building, not part of wmb
				{
					// name
					str_for_entfile( 		temp_str 	, you );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// scale_x
					str_for_num( 			temp_str 	, you.scale_x );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// scale_y
					str_for_num( 			temp_str 	, you.scale_y );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// scale_z
					str_for_num( 			temp_str 	, you.scale_z );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// x
					str_for_num( 			temp_str 	, you.x );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// y
					str_for_num( 			temp_str 	, you.y );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// z
					str_for_num( 			temp_str 	, you.z );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);	
					// pan
					str_for_num( 			temp_str 	, you.pan );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// tilt
					str_for_num( 			temp_str 	, you.tilt );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// roll
					str_for_num( 			temp_str 	, you.roll );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// alpha
					str_for_num( 			temp_str 	, you.alpha );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);				
					// ambient
					str_for_num( 			temp_str 	, you.ambient );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// LIGHT
					if ( is(you,LIGHT) )
						{
							str_for_num( 	temp_str 	, 1 );							
						}
					else
						{
							str_for_num( 	temp_str 	, 0 );
						}
					file_str_write( 		file_handle , temp_str );
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// red
					str_for_num( 			temp_str 	, you.red );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// green
					str_for_num( 			temp_str 	, you.green );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// blue
					str_for_num( 			temp_str 	, you.blue );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// UNLIT
					if ( is(you,UNLIT) )
						{
							str_for_num( 	temp_str 	, 1 );							
						}
					else
						{
							str_for_num( 	temp_str 	, 0 );
						}
					file_str_write( 		file_handle , temp_str );
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);	
					// OVERLAY
					if ( is(you,OVERLAY) )
						{
							str_for_num( 	temp_str 	, 1 );							
						}
					else
						{
							str_for_num( 	temp_str 	, 0 );
						}
					file_str_write( 		file_handle , temp_str );
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);	
					// material: model or category specific - okay
					if (you.material==mat_model)	
						{
							str_for_num( 	temp_str 	, 1 );
						}	
//					else	if (you.material==mtl_building_transp)	
//						{
//							str_for_num( 	temp_str 	, 2 );
//						}	
					else	// if (you.material==mtl_building)	
						{
							str_for_num( 	temp_str 	, 0 );
						}
					file_str_write( 		file_handle , temp_str );
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);	
					// FLAG7
					if ( is(you,FLAG7) )
						{
							str_for_num( 	temp_str 	, 1 );							
						}
					else
						{
							str_for_num( 	temp_str 	, 0 );
						}
					file_str_write( 		file_handle , temp_str );
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);	
					// FLAG6
					if ( is(you,FLAG6) )
						{
							str_for_num( 	temp_str 	, 1 );							
						}
					else
						{
							str_for_num( 	temp_str 	, 0 );
						}
					file_str_write( 		file_handle , temp_str );
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);	
					// FLAG5
					if ( is(you,FLAG5) )
						{
							str_for_num( 	temp_str 	, 1 );							
						}
					else
						{
							str_for_num( 	temp_str 	, 0 );
						}
					file_str_write( 		file_handle , temp_str );
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);	
					// for further development----------------------------
					str_for_num( 			temp_str 	, 0 );
					int i;
					for(i=0;i<3;i++)
						{
							file_str_write( 		file_handle , temp_str );			
								file_asc_write(	file_handle , 13); 
								file_asc_write(	file_handle , 10);
						}									
				}
			you = ent_next(you); 							// get next entity
		}
	//----------------------------------------------
	file_close(file_handle);
	
//	printf( "Building Map Saved: %s" , _chr(file_name) );
	
#ifndef MB_GAME
	// save panel
	reset( map_loadpanel , SHOW);
	reset( map_savetext1 , SHOW);
#endif	
	
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////


void		Building_Load_Buildings_blm(char* file_name, var import)
{
{}
#ifndef MB_GAME	
	// load panel
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);
	wait(3);
#endif
		
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	var temp;
	//---------------------------------------------
	var file_handle = file_open_read(temp_str);	
	//---------------------------------------------
	// buildings count check
	file_str_read( file_handle , temp_str );
	var ents_to_load = str_to_num( temp_str );
	if ( (num_entities+ents_to_load) > max_entities )
		{
			printf( "LOAD ERROR: \n Too many entities to be added: %0.d" , (double)ents_to_load );
			return;
		}	
	//building_counter += str_to_int( temp_str );		// set during creation - any object creation could fail
	
	//------------------------------------------------------------------------
	// temporary variables storing properties of the currently read in entity
	STRING* my_filename = str_create("");
	var my_scale_x, my_scale_y, my_scale_z,
		my_x, my_y, my_z, 
		my_pan, my_tilt, my_roll,
		my_alpha, my_ambient, 
		my_light, my_red, my_green, my_blue, 
		my_unlit, my_overlay, my_material,
		my_flag7, my_flag6, my_flag5;
	var eof = 0;
	// while not eof read in entity data
	// after an entity read in, call creator function that includes importing, counting and setting everything
	random_seed(0);
	//-----------------------------------------------------------------------
	var ents_counter = 0;
	while(1)		// or for loop based on count value read in
		{
			// name
			eof = file_str_read( file_handle , temp_str );
			if (eof==(var)-1)
				{
					break;
				}
			str_cpy( my_filename , temp_str );
			
			// scale_x
			file_str_read( file_handle , temp_str );
			my_scale_x = str_to_num( temp_str );
			
			// scale_y
			file_str_read( file_handle , temp_str );
			my_scale_y = str_to_num( temp_str );
			
			// scale_z
			file_str_read( file_handle , temp_str );
			my_scale_z = str_to_num( temp_str );
			
			// x
			file_str_read( file_handle , temp_str );
			my_x = str_to_num( temp_str );
			
			// y
			file_str_read( file_handle , temp_str );
			my_y = str_to_num( temp_str );
			
			// z
			file_str_read( file_handle , temp_str );
			my_z = str_to_num( temp_str );
			
			// pan
			file_str_read( file_handle , temp_str );
			my_pan = str_to_num( temp_str );
			
			// tilt
			file_str_read( file_handle , temp_str );
			my_tilt = str_to_num( temp_str );
			
			// roll
			file_str_read( file_handle , temp_str );
			my_roll = str_to_num( temp_str );
			
			// alpha
			file_str_read( file_handle , temp_str );
			my_alpha = str_to_num( temp_str );
			
			// ambient
			file_str_read( file_handle , temp_str );
			my_ambient = str_to_num( temp_str );
			
			// LIGHT
			file_str_read( file_handle , temp_str );
			my_light = str_to_num( temp_str );
			
			// red
			file_str_read( file_handle , temp_str );
			my_red = str_to_num( temp_str );
			
			// green
			file_str_read( file_handle , temp_str );
			my_green = str_to_num( temp_str );
			
			// blue
			file_str_read( file_handle , temp_str );
			my_blue = str_to_num( temp_str );
			
			// UNLIT			
			file_str_read( file_handle , temp_str );
			my_unlit = str_to_num( temp_str );
			
			// OVERLAY			
			file_str_read( file_handle , temp_str );
			my_overlay = str_to_num( temp_str );
			
			// 1 model material, 0 building material
			file_str_read( file_handle , temp_str );
			my_material = str_to_num( temp_str );
			
			// FLAG7
			file_str_read( file_handle , temp_str );
			my_flag7 = str_to_num( temp_str );			
			
			// FLAG6
			file_str_read( file_handle , temp_str );
			my_flag6 = str_to_num( temp_str );	
			
			// FLAG5
			file_str_read( file_handle , temp_str );
			my_flag5 = str_to_num( temp_str );	
			
			// for further development----------------------------
			int i;
			for(i=0;i<3;i++)
				{
					file_str_read( file_handle , temp_str );
				}
				
			// maximize entity creating function instances
			while (proc_status (Building_CreateItem_FromFile) > 2000)
				{
					wait(10);
				}
			// create entity
			Building_CreateItem_FromFile( import, my_filename, 
										my_scale_x, my_scale_y, my_scale_z,
										my_x, my_y, my_z, 
										my_pan, my_tilt, my_roll,
										my_alpha, my_ambient, 
										my_light, my_red, my_green, my_blue, 
										my_unlit, my_overlay, my_material,
										my_flag6, my_flag5 );	
										
			
			ents_counter++;
			if (ents_counter%100==(var)0)		// at every 100th entity loading
				{
{}
#ifndef MB_GAME					
					Map_Progressbar(ents_counter/ents_to_load*100);
					wait_for(Map_Progressbar);
#endif
					wait(1);
				}
		}
#ifndef MB_GAME	
	reset(map_loadbar,SHOW);  
#endif	
	//----------------------------------------------
	file_close(file_handle);
	//----------------------------------------------	
	//printf( "Building Map Loaded: %s" , _chr(file_name) );
	
#ifndef MB_GAME	
	// load panel
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
#endif
		
	ptr_remove(temp_str);
	ptr_remove(my_filename);
}


//////////////////////////////////////////////////


void		Building_CreateItem_FromFile( var imported, STRING* my_filename, 
													var my_scale_x, my_scale_y, my_scale_z,		
													my_x, my_y, my_z, 
													my_pan, my_tilt, my_roll,
													my_alpha, my_ambient, 
													my_light, my_red, my_green, my_blue, 
													my_unlit, my_overlay, my_material,										
													my_flag6, my_flag5)
{
//	my_filename = str_lwr( my_filename ); 		
	if (!my_filename) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str,building_foldername);
	str_cat(temp_str,my_filename);

	if ( file_exists(temp_str) )		// slow double check - not essential
		{			
			//-----------------------------------------------------------------------
			// set building properties from .obs file 
			// defaultsize for scaling, x/y tile area for tile management, type (passability), collision shape, shadow
			
			var obs_defaultsize;
			var obs_tilesize_x;
			var obs_tilesize_y;
			var obs_type;
			var obs_collision;
			var obs_shadow;
			
			Building_ReadObsFileData(my_filename, &obs_defaultsize, &obs_tilesize_x, &obs_tilesize_y, &obs_type, &obs_collision, &obs_shadow);
			
			//----------------------------------------------------------------------------
			// modify z if applied for another map
			
			if (imported)
				{			
					my_z = PosInfo3D4OrZeroLevel(my_x, my_y) ;  
				}	
			
			//----------------------------------------------------------------------------
			// create object
			
			Building_Create(	temp_str, 
									obs_defaultsize,
									obs_tilesize_x, obs_tilesize_y,
									obs_type, obs_collision, obs_shadow,
									my_scale_x, my_scale_y, my_scale_z,		
									my_x, my_y, my_z, 
									my_pan, my_tilt, my_roll,
									my_alpha, my_ambient, 
									my_light, my_red, my_green, my_blue, 
									my_unlit, my_overlay, my_material,										
									my_flag6, my_flag5);
//			wait_for(Building_Create);
			
			//----------------------------------------------------------------------------
		}
	
//	wait(1);
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// object removal


function Building_RemoveObject(ENTITY* ent)
{
	if (ent == NULL)
		{
			return 0;
		}
	
	if (ent.ENT_CATEGORY != (var)UIMODE_BUILDING)
		{
			return 0;
		}
	
	// remove collider entity
	if (ent.parent != NULL)
		{
			ptr_remove(ent.parent);
		}
	
	// remove decal attached to terrain or terrain models
	Decal_Remove_Decal(ent);
	wait_for(Decal_Remove_Decal);
	
	// remove unique shadow sprite or model
	Building_RemoveChild(ent);
	
	// remove the main entity
	ptr_remove(ent);		
	
	// decrease quantity
	building_counter--;
	
	return 1;
}


void		Building_RemoveChild(ENTITY* owner)
{
	you = ent_next(NULL); 
 	while (you) 	
		{
			if ( (you.XTILE_POS == owner.XTILE_POS) && (you.YTILE_POS == owner.YTILE_POS) )
				{
					if (you.ENT_CATEGORY == ((var)UIMODE_BUILDING + (var)UIMODE_SHADOWSHIFT) )
						{
							if (you.parent == owner)
								{
									ent_remove(you);
									
									return;
								}
						}
				}
			
			you = ent_next(you);
		}
}


//-------------------------------------


int		Building_RemoveArea(int midtilex, int midtiley, int areasizex, int areasizey)
{
	int i,j;
	var k = 1;
	
	// determine tiles to be cleared
	// minx : xtile - integer((building_tilesize-1)/2)
	var tempx = midtilex - integer(areasizex/2);
	var tempy = midtiley - integer(areasizey/2);
	
	ENTITY* temp_ent = NULL;
	var removed = 0;
	int counter = 0;
	
	you = ent_next(NULL); 
 	while (you) 	
		{
			temp_ent = NULL;
			removed = 0;
			
			if ( (you.XTILE_POS >= tempx) && (you.XTILE_POS < tempx + areasizex) && (you.YTILE_POS >= tempy) && (you.YTILE_POS < tempy + areasizey) )		// here could be an area check -> would be much faster
				{
					if (you.ENT_CATEGORY == (var)UIMODE_BUILDING)
						{
							// store next entity
							temp_ent = ent_next(you); 
													
							// remove its ent_decal shadow attached to terrain or terrain models
							Decal_Remove_Decal(you);
							wait_for(Decal_Remove_Decal);
							
							// remove the main entity
							ptr_remove(you);  										
							
							// decrease quantity
							building_counter--;
							
							removed = 1;
							counter++;	
						}						
					// unique shadows and collision models (they are in child and parent relations to the entity, but category is shifted)
					else if (you.ENT_CATEGORY == ((var)UIMODE_BUILDING + (var)UIMODE_SHADOWSHIFT) )
						{
							// store next entity
							temp_ent = ent_next(you); 
							
							// remove the entity
							ptr_remove(you);  										
							
							removed = 1;
							counter++;		
						}						
				}
				
			if (removed)
				{
					you = temp_ent; 
				}
			else
				{
	 				you = ent_next(you); 
 				}
		}	
	
	return counter;
}


//-------------------------------------


void		Building_RemoveAll()
{
	Remove_EntCats( UIMODE_BUILDING );
	wait_for(Remove_EntCats);
	
	building_counter = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////



#endif