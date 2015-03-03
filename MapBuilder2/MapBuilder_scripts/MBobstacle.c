
#ifndef MBobstacle_c
#define MBobstacle_c

////////////////////////////////////////////////////////////////////////////////////////////////
// code


////////////////////////////////////////////////////////////////////////////////////
// create


void		Obstacle_Create(	STRING* filename, 
									var obs_defaultsize,
									obs_tilesize_x, obs_tilesize_y,
									obs_type, obs_collision, obs_shadow,									
									var my_scale_x, my_scale_y, my_scale_z,		
									my_x, my_y, my_z, 
									my_pan, my_tilt, my_roll,									
									my_alpha, my_ambient, 
									my_light, my_red, my_green, my_blue, 
									my_unlit, my_overlay, my_material,										
									my_flag7, my_flag6, my_flag5)
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
			
//			if (obs_type != (var)2)						// not building - removed, can be rotated
				{
					// salefy keep angles between -180...180 degree				
					ent.pan			= ang(my_pan);
					ent.tilt			= ang(my_tilt);
					ent.roll			= ang(my_roll);
				}										
			
			//------------------------------------------------------------------------------------------------------------
			
			set(ent , FLAG2);								// FLAG2 should be reset for buildings afterwards !!!				
			
			//------------------------------------------------------------------------------------------------------------
			// tree detection
			
			var istree = 0;
			if (my_flag7)
				{
					istree = 1;
					set(ent , FLAG7);
				}
				
			//------------------------------------------------------------------------------------------------------------
			
			ent.alpha   	= 100;
			ent.ambient		= my_ambient;		
	
			if (my_alpha < (var)98)						// my_translucent could be used	
				{
					set( ent , TRANSLUCENT );
//					reset( ent , OVERLAY );
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
			
			ent.material 	= mtl_obstacle;
			
			if (istree)
				{
					// set tree vertex anim shader parameters - tree material is set later
					ent.skill41 = floatv(10);						// speed x
					ent.skill42 = floatv(25);						// speed	y
					ent.skill43 = floatv(35);						//	oscillation	
					ent.skill44 = floatv(ent.max_z/3);			// bending position
				}
			else if (my_unlit)
				{
					set(   ent , UNLIT );					// keeps ambient settings
					reset( ent , LIGHT );
					ent.material 	= mat_unlit;
				}
			else if (my_material==1)	
				{
//					reset( ent , UNLIT );
					ent.material 	= mat_model;		
				}
//			else if (my_material==2)	
//				{
//					ent.material 	= mtl_obstacle_transp;		
//				}
			
			//--------------------------------------------------------------------------------------------------------------					
			// set tree material and normals - needed one frame later than mtl_obstacle?
			
			if (istree)
				{																
					Obstacle_SetTreeMaterial(ent);					// set material(s) and normals 
					wait_for(Obstacle_SetTreeMaterial);
					
					Obstacle_CreateLODmodels(ent);
					wait_for(Obstacle_CreateLODmodels);						
				}	
			
			//---------------------------------------------------------------------------------------------
			// store data in skills
			
			// set tile pos
			ent.XTILE_POS = XCoordToTile(ent.x);
			ent.YTILE_POS = YCoordToTile(ent.y);
			ent.ZTILE_POS = 0;
			ent.ABSTILE_POS = XYTileToAbsTile(ent.XTILE_POS, ent.YTILE_POS);
			
			// set entity type and size
			ent.ENT_CATEGORY = (var)UIMODE_OBSTACLE;
//			ent.ENT_CLIP     = 1;													//  clipping at 75%
//			ent.ENT_ANIM     = 0;	
#ifndef MB_RELEASE	
				ent.ENT_HANDLE   = handle(ent);										// used by the RTS building cluster creation system
#endif
			ent.AREA_SIZEX   = obs_tilesize_x; 									// * (my_scale_x*obs_defaultsize/GetMapTileSize());	// should take into account scaling - only refining scaling is allowed
			ent.AREA_SIZEY   = obs_tilesize_y; 									// * (my_scale_x*obs_defaultsize/GetMapTileSize());	// should take into account scaling - only refining scaling is allowed
			ent.ENT_TYPE 	 = obs_type; 											// store tile passability for pathfinder *** new ***
			
			// collision group
			ent.group = 3;
			
			//------------------------------------------------------------------------------------------------------------
			// set clipping
			
//			STRING* temp5_str = str_create("");
//			str_for_entfile(temp5_str,me);
//			if (str_stri(temp5_str,"_0.")==(var)0)
//				{
//					if ( (ent.AREA_SIZEX < 5) && (ent.AREA_SIZEY < 5) )
						{
							ent.eflags |= CLIP0;	// 75%
						}
//				}
//			ptr_remove(temp5_str);	
			
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
			else if (((shadow_stencil>(var)-1) && (shadow_stencil<(var)8)) || (my_flag5>0))	//(obs_collision==(var)2)		// flag5 means obstacle_decal
				{
					// to set bounding box to extends because of min_z and max_z 
					// or before bounding box vec_scale is used	
					set(ent, PASSABLE);							
					
					Obstacle_CreateCollider(ent);
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
			Obstacle_SetShadow(ent, (1-my_flag6), my_flag5, obs_shadow, my_flag7);
			wait_for(Obstacle_SetShadow);														// needed because of decal and ent_decal shadows, to wait for completion before setting entity to static
			
			//--------------------------------------------------------------------------------------------------------------	
			
			obstacle_counter++;
			
			ent.emask &= ~DYNAMIC;																	// set to static for faster rendering
			
//			Obstacle_Created_Loop(ent);															// it can result in too many running functions !!! and entity cannot be static
			
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
//			Terrain_SetBuildingTilesClusters(ent, building_foldername);					// done for all buildings on game init
//	#endif
//#endif
			//---------------------------------------------------------------------------------------------------
		}
	
	ptr_remove(temp_str);
}


void		Obstacle_CreateCollider(ENTITY* ent)
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
	obs_collider.ENT_CATEGORY = (var)UIMODE_OBSTACLE + (var)UIMODE_SHADOWSHIFT;
	
	obs_collider.XTILE_POS = ent.XTILE_POS;
	obs_collider.YTILE_POS = ent.YTILE_POS;
	
	obs_collider.group = 1;	
	
	//-------------------------------------------------------
	// to manually set bounding box
	
	wait(1);	
	
	if (!is(ent, FLAG7)) 
		{ 
			obs_collider.pan = ent.pan; 																			// (should be after a wait) ONLY IN FREE EDITION - IT SHOULD BE 0,90,180 OR 270 DEGREE ONLY !!!!!!!!!!!!!!!	
		}
	
	c_setminmax(obs_collider);
	obs_collider.eflags |= FAT | NARROW; 																		// set both flags to prevent automatic recalculation on scale changes			
	//	obs_collider.min_x = -GetMapTileSize()/2 * obstacle_tilesize_x * obstacle_scale;				// required ?
	//	obs_collider.min_y = -GetMapTileSize()/2 * obstacle_tilesize_y * obstacle_scale;				// required ?
	//	obs_collider.max_x =  GetMapTileSize()/2 * obstacle_tilesize_x * obstacle_scale;				// required ?
	//	obs_collider.max_y =  GetMapTileSize()/2 * obstacle_tilesize_y * obstacle_scale;				// required ?		
	obs_collider.min_z =  0;																						// required !
	obs_collider.max_z =  ent.max_z;																				// required !	
	
	ent.parent = obs_collider;																						// in fact child, but used in this way for easy deletion
	
	//-------------------------------------------------------
	
	obs_collider = NULL;
	ptr_remove(obs_collider);	
}


//void		Obstacle_Created_Loop(ENTITY* ent)
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


void		Obstacle_ReadObsFileData(	STRING* my_filename, 
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
	str_cpy( temp_str0 , obstacle_foldername );
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


/////////////////////////////////////////////////////
// tree related functions


void		Obstacle_CreateLODmodels(ENTITY* ent)
{
	STRING* temp_tree_str = str_create("");
	int i;
	str_for_entfile(temp_tree_str,ent);					// get e.g. tree002_0.mdl
	
	if (str_stri(temp_tree_str,"_0.")>0)				// it is a lod0 model - does not work - case sensitive !!! - not essential
		{			
			str_trunc(temp_tree_str,5);	
			str_cat(temp_tree_str, "1.mdl");						
			you = ent_for_file(temp_tree_str);					// does first lod model exist?
			if (!you)													// check whether an entity already exists with this name or not - maybe faster
				{							
					// create invisible lod models if they do not already exist - okay - but too many entities																							
					for(i=1;i<4;i++)											// cycle through LOD stages 1 and 2 - really good, 3 is sprite based double sided crossed planes - a bit better if applied
						{
							str_trunc(temp_tree_str,5);					// remove 0.mdl from end, and 1.mdl in next cycle
							str_cat(temp_tree_str, str_for_num(NULL,(var)i));
							str_cat(temp_tree_str, ".mdl");
							
							you = ent_for_file(temp_tree_str);			// get pointer to lod model if exists
							if (!you)											// or check existance
								{
									// add path! 
									STRING* temp6_str = str_create("");
									str_cpy(temp6_str,obstacle_foldername);
									str_cat(temp6_str,temp_tree_str);
									
									if (file_exists(temp6_str))				
										{
											//										collision_mode = 0; 			// has no effect here...
											ent_create(temp6_str, vector(0,0,-GetMapTileSize()*100), Obstacle_CreateTreeLOD);	// create a hidden LOD model
											wait_for(ent_create);
											//										collision_mode = 1; 
										}
									
									ptr_remove(temp6_str);
								}																		
						}
				}	// you
		}	
}


action	Obstacle_CreateTreeLOD()
{
	my.alpha = 100;
	reset(my, SHADOW | TRANSLUCENT);												// ?
	set(my, PASSABLE | INVISIBLE | FLAG1 | FLAG2 | FLAG6 | FLAG7);		// no mirror, no static lightmap, no decal shadows on it, tree
	my.flags2 |= UNTOUCHABLE;
	if (shadow_stencil==(var)8)
		{
			reset(my,CAST);
		}
	else
		{
			set(my,CAST);
		}
	my.ENT_CATEGORY = (var)UIMODE_OBSTACLE + (var)UIMODE_SHADOWSHIFT;
	
	wait(1);
	
	Obstacle_SetTreeMaterial(my);							// set material(s) and normals
	wait_for(Obstacle_SetTreeMaterial);
	
	wait(1);
	my.emask &= ~DYNAMIC;									// set entity to static
}


void		Obstacle_SetTreeMaterial(ENTITY* ent)
{
//	if (shadow_stencil > (var)-1)
		{
			set(ent,OVERLAY);											// for proper transparency, and sorting with water terrain
		}
	if (shadow_stencil == (var)8)
		{
			set(ent,CAST);												// for proper shadows, if not set pssm shadows can be seen through alpha texture objects - engine bug
		}
//	ent.material 	= mtl_obstacle;								// to get material not only when another obstacle is present - set in create action
//	wait(1);
	ent.material 	= mtl_tree;										// to get vertex animation for the full model
	// override material settings for leaf skin only
	int i;
	for(i=0;i<ent_status(ent,8);i++)
		{
			BMAP* skin_bmap = ent_getskin(ent,i+1);			    
			if (skin_bmap.flags&BMPF_ARGB)						// alpha = leaf skin
				{
					ent_mtlset(ent, mtl_tree, i+1);				// to override .mdl contained material
					Obstacle_SetTreeNormals(ent,i+1);			// normals show out from entity middle, with some randomization, resets bounding box to extends !
				}
			else															// non-alpha = bark skin
				{
					if (shadow_stencil > (var)-1)
						{
							ent_mtlset(ent, mtl_obstacle, i+1);	// to override .mdl contained material and tree material - keeps mtl_tree effect !!! i.e. vertex animation
						}
					else
						{
							ent_mtlset(ent, mtl_tree, i+1);		// to override .mdl contained material - required for full tree animation
						}
				}
			skin_bmap = NULL;
			bmap_remove(skin_bmap);		
		}
}


void		Obstacle_SetTreeNormals(ENTITY* tree_ent, int skin_num)		
{	
	// reset normals to MED default
	//	LPD3DXMESH dx_mesh1;
	int i;
	for (i=0; i<(var)(ent_status(tree_ent,9)); i++)
		{
			dx_mesh = ent_getmesh(tree_ent,i,0);						// ent , mesh subset , lod
			D3DXComputeNormals(dx_mesh,NULL);
			ent_setmesh(tree_ent,dx_mesh,i,0);
			dx_mesh = NULL;			
		}
//	dx_mesh1 = ent_getmesh(NULL,0,0);									// not needed
//	dx_mesh1->Release();
	//-----------------------------------------------------
	// set normals between spherical and normalized
	
	VECTOR min_vec;
	vec_for_min(min_vec,tree_ent);
	VECTOR max_vec;
	vec_for_max(max_vec,tree_ent);
	
	VECTOR average_vec;						// DX y is height , 3dgs z is height
	average_vec.x = 0;
	average_vec.z = 0;
	average_vec.y = abs(max_vec.z + min_vec.z)/2;		
	
	// set normals pointing out of model center, but only for second skin
	int i;
	for (i=1; i<=(var)(ent_status(tree_ent,0)); i++)
		{	
			CONTACT* c = ent_getvertex(tree_ent,NULL,i);   			// vertex number begins with 1 !			
			
			if (
				(	(skin_num ==1)	&&	(c.v.u1>=(float)0) && (c.v.v1>=(float)0)	)
				||	
				(	(skin_num ==2)	&&	(c.v.u2>=(float)0) && (c.v.v2>=(float)0)	)
				)
				{	
					VECTOR xyz_vec;
					// a
					vec_diff(xyz_vec, vector((var)c.v.x, (var)c.v.y, (var)c.v.z), average_vec);	// from model mid to vertex position
					// b
					//					vec_set(xyz_vec,vector((var)c.v.x, (var)c.v.y, (var)c.v.z));						// position from 0,0,0
					//					xyz_vec.y -= abs(max_vec.z + min_vec.z)/2;												// adjust by model middle
					vec_normalize(xyz_vec,1);
					
					VECTOR nxnynz_vec;
					vec_set(nxnynz_vec,vector((var)c.v.nx, (var)c.v.ny, (var)c.v.nz));				// original normals
					vec_normalize(nxnynz_vec,1);
					
					VECTOR temp_vec;
					var smoothing_factor = 0.40;																	// 0.60 okay for TreeIT , 0.50 okay for TreeMagikG3 - but anim tree modifies it, so it's better to decrease it a bit !!!
					if (ent_status(tree_ent,8)==(var)1) 
					{
						smoothing_factor = 0;																	// no iteration for 1 skin i.e. lod3 models, use purely spherical normals
					}
					vec_lerp(temp_vec, xyz_vec, nxnynz_vec, smoothing_factor);							// new normals iterated between normalized position vec and orig normal
					// (1-f)*v1 + f*v2					
					//					temp_vec.x = xyz_vec.x*(1-smoothing_factor) + nxnynz_vec.x*smoothing_factor;
					//					temp_vec.y = xyz_vec.y*(1-smoothing_factor) + nxnynz_vec.y*smoothing_factor;
					//					temp_vec.z = xyz_vec.z*(1-smoothing_factor) + nxnynz_vec.z*smoothing_factor;					
					
					//					vec_set(temp_vec,xyz_vec);																		// no iteration - a bit flat effect
					vec_normalize(temp_vec,1);
					
					c.v.nx = (float)temp_vec.x; 			
					c.v.ny = (float)temp_vec.y; 
					c.v.nz = (float)temp_vec.z; 	
					
					ent_setvertex(tree_ent,c,i);
				}			
		}
	
//	// not needed - set later in Obstacle_Create()
//	// manually set bounding box because of decal shadow size - required because the above manipulations set boundign box to entity extends!
//	if (((shadow_stencil>(var)-1) && (shadow_stencil<(var)8)) || (is(tree_ent, FLAG5)))			// !!!
//		{
//			tree_ent.eflags |= FAT | NARROW; 													// set both flags to prevent automatic recalculation on scale changes			
//			tree_ent.min_z = 0;
//			tree_ent.min_x = - maxv( abs(tree_ent.min_x) , tree_ent.max_z/2 );	
//			tree_ent.max_x = - tree_ent.min_x;
//			tree_ent.min_y = - maxv( abs(tree_ent.min_y) , tree_ent.max_z/2 );	
//			tree_ent.max_y = - tree_ent.min_y;
//		}
//	else	// -1 or 8
//		{
//			tree_ent.eflags |= FAT | NARROW; 
//			tree_ent.min_z = 0;
//			tree_ent.min_x = - GetMapTileSize()/2 * tree_ent.AREA_SIZEX;
//			tree_ent.max_x = - tree_ent.min_x;
//			tree_ent.min_y = - GetMapTileSize()/2 * tree_ent.AREA_SIZEY;
//			tree_ent.max_y = - tree_ent.min_y;
//		}
}


////////////////////////////////////////////////////////////////////////////////////////////////
// shadows

void	Obstacle_SetNoshadow(ENTITY* ent)
{
	set(ent, FLAG6);										// exclude from lightmap
	reset(ent, SHADOW);
	reset(ent, FLAG5);									// no entdecal enforced
	
	if (shadow_stencil==(var)8)
		{
			if (AlphaMaterialTest(ent)>0)
				{
					set(ent, CAST);						// no self shadow on alpha material objects
				}
		}
}


void	Obstacle_SetLightmap(ENTITY* ent)
{
	reset(ent, FLAG6);									// include in lightmap
	reset(ent, FLAG5);									// no entdecal enforced
	
	if (shadow_stencil==(var)-1)
		{
			set(ent, SHADOW | CAST);					// no shadow cast, only self shadows
		}
	else if (shadow_stencil==(var)8)
		{
			reset(ent, SHADOW);
			
			if (AlphaMaterialTest(ent)>0)
				{					
					set(ent, CAST);						// no self shadow on alpha material objects
				}
		}
	else
		{
			reset(ent, SHADOW);
		}
}


void	Obstacle_SetSCSM(ENTITY* ent)
{
	set(ent , FLAG6);										// exclude from lightmap
	set(ent, SHADOW);
	reset(ent, FLAG5);									// no entdecal enforced
}


void	Obstacle_SetPSSM(ENTITY* ent)
{
	set(ent, FLAG6);										// exclude from lightmap
	set(ent, SHADOW);
	reset(ent, FLAG5);									// no entdecal enforced
	
	if ((AlphaMaterialTest(ent)>0) || (is(ent,OVERLAY)))
		{
			set(ent, CAST);								// no self shadow on alpha material objects
		}
}


void	Obstacle_SetStencil(ENTITY* ent)
{
	set(ent, FLAG6);										// exclude from lightmap
	set(ent, SHADOW);	
	reset(ent, FLAG5);									// no entdecal enforced
	
	if (AlphaMaterialTest(ent)>0)
		{
			set(ent, CAST);									// no self shadow possible on alpha material objects
		}
}


void	Obstacle_SetSprite(ENTITY* ent, var obs_decal, var obs_tree)		// called only in case of shadow_stencil 0 and 1
{	
	if ((!main_blur) && (obs_decal < 1))											// no stencil blur applied, no entdecal enforced
		{
			Obstacle_SetDecal(ent, obs_tree);										// much slower than decal shadows
			wait_for(Obstacle_SetDecal);
		}
	else
		{
			if (obs_decal)
				{
					set(ent, FLAG5);														// enforced decal
				}			
			Obstacle_SetEntDecal(ent, obs_tree);
			wait_for(Obstacle_SetEntDecal);
		}	
}


void	Obstacle_SetDecal(ENTITY* ent, var obs_tree)
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
			temp_ent.ENT_CATEGORY = (var)UIMODE_OBSTACLE + (var)UIMODE_SHADOWSHIFT;
			temp_ent.XTILE_POS = XCoordToTile(ent.x);
			temp_ent.YTILE_POS = YCoordToTile(ent.y);
			
			ent.shadow = temp_ent;					
			
			temp_ent = NULL;	
			ptr_remove(temp_ent);
		}
	else																											// use common shadow images
		{
			if (obs_tree)
				{
					ent.shadow = ent_treeshadow_fix; 		
				}
			else
				{
					ent.shadow = ent_unitshadow_rot; 	
				}
		}
	
	wait(1);
//	ent.y += 32;
	reset(ent, SHADOW);
	wait(2);
//	ent.y -= 32;
	set(ent, SHADOW);
	wait(1);
}


void	Obstacle_SetEntDecal(ENTITY* ent, var obs_tree)
{
	wait(1);													// because bounding box size dependency !!!
	
	set(ent , FLAG6);										// exclude from lightmap
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
			temp_ent.ENT_CATEGORY = (var)UIMODE_OBSTACLE + (var)UIMODE_SHADOWSHIFT;
			temp_ent.XTILE_POS = XCoordToTile(ent.x);
			temp_ent.YTILE_POS = YCoordToTile(ent.y);
			
			Decal_Set_Decal(ent, bmap_for_entity(temp_ent,0));			
			
			temp_ent = NULL;	
			ptr_remove(temp_ent);
		}
	else																											// use common shadow images
		{
			if (obs_tree)
				{
					Decal_Set_Decal(ent, bmap_for_entity(ent_treeshadow_fix,0));
				}
			else
				{
					Decal_Set_Decal(ent, bmap_for_entity(ent_unitshadow_rot,0));
				}
		}
}


void	Obstacle_SetModel(ENTITY* ent)
{
	set(ent , FLAG6);										// exclude from lightmap
	set(ent, SHADOW | CAST);							// | CAST : no self shadow
	reset(ent, FLAG5);									// no entdecal enforced
	
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
			temp_ent.ENT_CATEGORY = (var)UIMODE_OBSTACLE + (var)UIMODE_SHADOWSHIFT;
			temp_ent.XTILE_POS = XCoordToTile(ent.x);
			temp_ent.YTILE_POS = YCoordToTile(ent.y);
			
			ent.shadow = temp_ent;	
			
			temp_ent = NULL;	
			ptr_remove(temp_ent);			
		}
	else
		{			
			ent.shadow = ent_model_unitshadow;			// shadow model - a simle symmetric model casting shadow intead of the actual model - does not rotates yet so must be symmetric
		}
}

////////////////////////////////////////////////////////////////////////////

// include in lightmap, force ent_decal, shadow 0 no/1 dynamic/2 sprite/3 model
void	Obstacle_SetShadow(ENTITY* ent, var obs_lightmap, var obs_decal, var obs_shadow, var obs_tree)
{	
	if (obs_shadow == (var)0)												// no shadow
		{
			Obstacle_SetNoshadow(ent);
			wait_for(Obstacle_SetNoshadow);
		}
	else if (obs_lightmap == (var)1)										// static lightmap shadow
		{
			Obstacle_SetLightmap(ent);
			wait_for(Obstacle_SetLightmap);
		} 
	else																			// dynamic shadow
		{						
			if (shadow_stencil == (var)-1)										// sc esm shadowmapping
				{
					if (obs_decal<1)
						{
							Obstacle_SetSCSM(ent);
							wait_for(Obstacle_SetSCSM);
						}
					else																			// enforce entdecal shadow
						{
							set(ent, FLAG5);
							Obstacle_SetEntDecal(ent, obs_tree);
							wait_for(Obstacle_SetEntDecal);
						}
				}
			else if (shadow_stencil == (var)8)									// pssm shadowmapping
				{
					if (obs_decal<1)
						{
							Obstacle_SetPSSM(ent);
							wait_for(Obstacle_SetPSSM);
						}
					else																			// enforce entdecal shadow
						{
							set(ent, FLAG5);
							Obstacle_SetEntDecal(ent, obs_tree);
							wait_for(Obstacle_SetEntDecal);
						}	
				}
			else
				{					
					if (obs_shadow==(var)1)											// stencil shadow	
						{
							if (obs_decal<1)
								{
									Obstacle_SetStencil(ent);
									wait_for(Obstacle_SetStencil);
								}
							else																	// enforce sprite shadow (decal or entdecal)
								{
									Obstacle_SetSprite(ent, obs_decal, obs_tree);
									wait_for(Obstacle_SetSprite);
								}
							
						}
					else if (obs_shadow==(var)2)									// sprite shadow (decal or entdecal)
						{
							Obstacle_SetSprite(ent, obs_decal, obs_tree);
							wait_for(Obstacle_SetSprite);
						}
					else if (obs_shadow==(var)3)									// model shadow
						{
							if (obs_decal<1)
								{
									Obstacle_SetModel(ent);
									wait_for(Obstacle_SetModel);
								}
							else																	// enforce sprite shadow (decal or entdecal)
								{									
									Obstacle_SetSprite(ent, obs_decal, obs_tree);
									wait_for(Obstacle_SetSprite);
								}
						}					
					else																	// protetction (no shadow)
						{
							Obstacle_SetNoshadow(ent);
							wait_for(Obstacle_SetNoshadow);
						}					
				}
		}	
}


////////////////////////////////////////////////////////////////////////////////////
// save/load


void		Obstacle_Save_Obstacles_obm(char* file_name)
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
	str_for_num( 			temp_str 	, obstacle_counter );
	file_str_write( 		file_handle , temp_str );
	file_asc_write(	file_handle , 13); 
	file_asc_write(	file_handle , 10);
	
	// entity data - entity placed first will be written into file last
	you = ent_next(NULL); 									// get first entity
	while (you) 												// repeat until there are no more entities
		{
			if ((you.ENT_CATEGORY == (var)UIMODE_OBSTACLE) && (!is(you,FLAG8)))				// if obstacle, not part of wmb
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
//					else	if (you.material==mtl_obstacle_transp)	
//						{
//							str_for_num( 	temp_str 	, 2 );
//						}	
					else	// if (you.material==mtl_obstacle)	
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
	
//	printf( "Obstacle Map Saved: %s" , _chr(file_name) );

#ifndef MB_GAME
	// save panel
	reset( map_loadpanel , SHOW);
	reset( map_savetext1 , SHOW);
#endif	
	
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////


void		Obstacle_Load_Obstacles_obm(char* file_name, var import)
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
	// obstacles count check
	file_str_read( file_handle , temp_str );
	var ents_to_load = str_to_num( temp_str );
	if ( (num_entities+ents_to_load) > max_entities )
		{
			printf( "LOAD ERROR: \n Too many entities to be added: %0.d" , (double)ents_to_load );
			return;
		}	
	//obstacle_counter += str_to_int( temp_str );		// set during creation - any object creation could fail
	
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
			
			// 1 model material, 0 obstacle material
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
			while (proc_status (Obstacle_CreateItem_FromFile) > 2000)
				{
					wait(10);
				}
			// create entity
			Obstacle_CreateItem_FromFile( import, my_filename, 
			my_scale_x, my_scale_y, my_scale_z,
			my_x, my_y, my_z, 
			my_pan, my_tilt, my_roll,
			my_alpha, my_ambient, 
			my_light, my_red, my_green, my_blue, 
			my_unlit, my_overlay, my_material,
			my_flag7, my_flag6, my_flag5 );	
			
			
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
	//printf( "Obstacle Map Loaded: %s" , _chr(file_name) );
	
#ifndef MB_GAME	
	// load panel
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
#endif
	
	ptr_remove(temp_str);
	ptr_remove(my_filename);
}


//////////////////////////////////////////////////


void		Obstacle_CreateItem_FromFile( var imported, STRING* my_filename, 
													var my_scale_x, my_scale_y, my_scale_z,		
													my_x, my_y, my_z, 
													my_pan, my_tilt, my_roll,
													my_alpha, my_ambient, 
													my_light, my_red, my_green, my_blue, 
													my_unlit, my_overlay, my_material,										
													my_flag7, my_flag6, my_flag5)
{
//	my_filename = str_lwr( my_filename ); 		
	if (!my_filename) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, obstacle_foldername);
	str_cat(temp_str, my_filename);

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
			
			Obstacle_ReadObsFileData(my_filename, &obs_defaultsize, &obs_tilesize_x, &obs_tilesize_y, &obs_type, &obs_collision, &obs_shadow);
			
			//----------------------------------------------------------------------------
			// modify z if applied for another map
			
			if (imported)
				{			
					my_z = PosInfo3D4OrZeroLevel(my_x, my_y) ;  
				}	
			
			//----------------------------------------------------------------------------
			// create object
			
			Obstacle_Create(	temp_str, 
									obs_defaultsize,
									obs_tilesize_x, obs_tilesize_y,
									obs_type, obs_collision, obs_shadow,
									my_scale_x, my_scale_y, my_scale_z,		
									my_x, my_y, my_z, 
									my_pan, my_tilt, my_roll,
									my_alpha, my_ambient, 
									my_light, my_red, my_green, my_blue, 
									my_unlit, my_overlay, my_material,										
									my_flag7, my_flag6, my_flag5);
//			wait_for(Obstacle_Create);
			
			//----------------------------------------------------------------------------
		}
	
//	wait(1);
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


function Obstacle_RemoveObject(ENTITY* ent)
{
	if (ent == NULL)
		{
			return 0;
		}
	
	if (ent.ENT_CATEGORY != (var)UIMODE_OBSTACLE)
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
	Obstacle_RemoveChild(ent);
	
	// remove the main entity
	ptr_remove(ent);		
	
	// decrease quantity
	obstacle_counter--;
	
	return 1;
}


void		Obstacle_RemoveChild(ENTITY* owner)
{
	you = ent_next(NULL); 
 	while (you) 	
		{
			if ( (you.XTILE_POS == owner.XTILE_POS) && (you.YTILE_POS == owner.YTILE_POS) )
				{
					if (you.ENT_CATEGORY == ((var)UIMODE_OBSTACLE + (var)UIMODE_SHADOWSHIFT) )
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


int		Obstacle_RemoveArea(int midtilex, int midtiley, int areasizex, int areasizey)
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
					if (you.ENT_CATEGORY == (var)UIMODE_OBSTACLE)
						{
							// store next entity
							temp_ent = ent_next(you); 
													
							// remove its ent_decal shadow attached to terrain or terrain models
							Decal_Remove_Decal(you);
							wait_for(Decal_Remove_Decal);
							
							// remove the main entity
							ptr_remove(you);  										
							
							// decrease quantity
							obstacle_counter--;
							
							removed = 1;
							counter++;	
						}						
					// unique shadows and collision models (they are in child and parent relations to the entity, but category is shifted)
					else if (you.ENT_CATEGORY == ((var)UIMODE_OBSTACLE + (var)UIMODE_SHADOWSHIFT) )
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


void		Obstacle_RemoveAll()
{
	Remove_EntCats( UIMODE_OBSTACLE );
	wait_for(Remove_EntCats);
	
	obstacle_counter = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif