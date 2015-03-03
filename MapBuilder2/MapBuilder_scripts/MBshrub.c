#ifndef MBshrub_c
#define MBshrub_c


////////////////////////////////////////////////////////////////////////////////////////////////
// code

void 		Shrub_Create(	STRING* filename, 
								var my_scale_x, my_scale_y, 
								my_x, my_y, my_z, 
								my_pan, my_tilt, 
								my_collider, my_alpha, my_ambient, 
								my_light, my_red, my_green, my_blue, 
								my_unlit, my_shadow, my_overlay, my_material,
								my_flag6, my_flag5, my_clip)
{
	if (!filename) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, filename);
	
	//----------------------------------------------------------------------------
	
	collision_mode = 0;																						// no collision needed - even for lightmapping, because temp models are needed and used for c-tracing
	
	ENTITY* ent = ent_create(temp_str, vector(my_x, my_y, my_z), NULL);						// changing my to ent resolved the strange ui loop stopping bug
	
	collision_mode = 1;																						// needed for collider entity
	
//	while(!ent) {wait(1);}
	
			if (ent != NULL)										// check, or while/wait
				{
					//------------------------------------------------------------------------------------------------------------
					
					ent.scale_x		= my_scale_x;
					ent.scale_y		= my_scale_y;
					
					// salefy keep angles between -180...180 degree				
					ent.pan			= ang(my_pan);
					ent.tilt			= ang(my_tilt);
					ent.roll			= 0;									
					
					//------------------------------------------------------------------------------------------------------------
					
					if (my_collider)
						{			
							set(ent , FLAG7);						// this sprite is used for making handlearray on game init - not used anymore
						}
					
					set(ent, PASSABLE | FLAG2 | FLAG1);		
					ent.flags2 |= UNTOUCHABLE;
					
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
					
					ent.material 	= mtl_shrub;
					
					// set tree vertex anim shader parameters - tree material is set later
					ent.skill41 = floatv(50);						// speed x
					ent.skill42 = floatv(50);						// speed	y
					ent.skill43 = floatv(50);						//	oscillation	
//					ent.skill44 = floatv(ent.max_z/3);			// bending position
					ent.skill44 = floatv(0);						// bending position
					
					if (my_unlit)
						{
							set(   ent , UNLIT );					// keeps ambient settings
							reset( ent , LIGHT );
							ent.material 	= mat_unlit;
						}
					else if (my_material==1)	
						{
//							reset( ent , UNLIT );
							ent.material 	= mat_sprite;		
						}
					
					//---------------------------------------------------------------------------------------------
					// store data in skills
					
					// set tile pos
					ent.XTILE_POS = XCoordToTile(ent.x);
					ent.YTILE_POS = YCoordToTile(ent.y);
					ent.ZTILE_POS = 0;
					ent.ABSTILE_POS = XYTileToAbsTile(ent.XTILE_POS, ent.YTILE_POS);
					
					// set entity type and size
					ent.ENT_CATEGORY = (var)UIMODE_SHRUB;
//					ent.ENT_CLIP     = 1;													//  clipping at 75%
//					ent.ENT_ANIM     = 0;	

					ent.AREA_SIZEX   = 1; 					
					ent.AREA_SIZEY   = 1; 						
					ent.ENT_TYPE 	 = 0; 													// impassable	
					
//					// collision group - no collision!
//					ent.group = 3;
					
					//--------------------------------------------------------------------------
					// set appearance properties					
					
					if (my_clip==(var)2)
						{
							ent.eflags |= CLIP0;												// 75%
						}
					else if (my_clip==(var)1)
						{
							ent.eflags |= CLIP1; 											// 50%
						}
					else
						{
							ent.eflags |= CLIP0 | CLIP1;									// 25%
						}
					
					//--------------------------------------------------------------------------------------------------------------	
					// set collision			
					
					wait(1);																		// needed after scaling and before update collision size	
					
					c_setminmax(ent);
					
					if (my_collider)
						{
							Shrub_CreateCollider(ent);
						}
					
					//------------------------------------------------------------------------------------------------------------
					// shadow - no lightmap possible because no collision thus not c_trace-able
					
					var firstshrub		= (var)0;											// really used only by creator action
					
					if ((my_shadow>0) || (my_flag5>0))									// only 1st entities are set to cast shadow on creation, thus this value can be easily estimated
						{
							firstshrub	= (var)1;																
						}
					
					Shrub_SetShadow(ent, 1-my_flag6, my_flag5, my_shadow, firstshrub);	// flag6 is true when entity is excluded from lightmap casters
					wait_for(Shrub_SetShadow);
					
					//------------------------------------------------------------------------------------------------------------
					
					shrub_counter++;
						
					wait(1);																		// needed to ensure correct pan tilt roll and scale		
					
					ent.emask &= ~DYNAMIC;	
					
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
					
					//------------------------------------------------------------------------------------------------------------
				}
				
	ptr_remove(temp_str);
}


void		Shrub_CreateCollider(ENTITY* ent)
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
	//	obs_collider.pan = ent.pan;																				// ONLY IN FREE EDITION - IT SHOULD BE 0,90,180 OR 270 DEGREE ONLY !!!!!!!!!!!!!!!
	obs_collider.scale_x = GetMapTileSize()/16;																// 0.5*tilesize - 16 is the cube size
	obs_collider.scale_y = GetMapTileSize()/16;
	obs_collider.scale_z = GetMapTileSize()/16;																// does not matter, but better for debugging if small
	obs_collider.ENT_CATEGORY = (var)UIMODE_SHRUB + (var)UIMODE_SHADOWSHIFT;
	
	obs_collider.XTILE_POS = ent.XTILE_POS;
	obs_collider.YTILE_POS = ent.YTILE_POS;
	
	obs_collider.group = 1;	
	
	//-------------------------------------------------------
	// to manually set bounding box
	
	wait(1);	
	
	c_setminmax(obs_collider);
	obs_collider.eflags |= FAT | NARROW; 																		// set both flags to prevent automatic recalculation on scale changes			
//	obs_collider.min_x = -GetMapTileSize()/2;				// required ?
//	obs_collider.min_y = -GetMapTileSize()/2;				// required ?
//	obs_collider.max_x =  GetMapTileSize()/2;				// required ?
//	obs_collider.max_y =  GetMapTileSize()/2;				// required ?		
	obs_collider.min_z = -GetMapTileSize()/2;																	// required !
	obs_collider.max_z =  GetMapTileSize()/2;																	// required !	
	
	ent.parent = obs_collider;																						// in fact child, but used in this way for easy deletion
	
	//-------------------------------------------------------
	
	obs_collider = NULL;
	ptr_remove(obs_collider);	
}


void		Shrub_SetDefaultSize()
{
	// shrub imagesize determination: 256 (low quality) or 512 (high quality)
	STRING* temp_str = str_create("");
	var file_handle = file_open_read(shrubsize_name);
	if (file_handle!=0)
		{
			file_str_read( file_handle , temp_str );
			shrub_defaultsize = str_to_num( temp_str );
			file_close(file_handle);
		}
	else
		{
			shrub_defaultsize = SHRUB_SIZE1;
		}
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// shadows


void	Shrub_SetShadow(ENTITY* ent, var shrub_lightmap, var shrub_decal, var shrub_shadow, var shrub_firstshrub)
{
	if (shrub_lightmap==(var)0) 													// no lightmap i.e. dynamic shadow
		{
			set( ent , FLAG6 );														// has shadow, to exclude from static lightmapping
			
			if ((shadow_stencil==(var)-1) && (shrub_decal<1))				// !!!
				{
					if (shrub_shadow>(var)0)
						{
							set(ent, SHADOW);
						}
				}
			else if ((shadow_stencil==(var)8) && (shrub_decal<1))
				{
					if (shrub_shadow>(var)0)										// settings for all members of a shrub
						{
							set(ent, SHADOW | CAST);								// definitely has an alpha skin - surpress self shadows
						}			
				}
			else
				{
					// simple shadow cast by first member of a shrub consisting of a couple of sprites
					if ((shrub_shadow>(var)0) && (shrub_firstshrub==(var)1))
						{
							if (shrub_decal>0) 										// enforce decals
								{
									set( ent , FLAG5 );	
									
									if (shadow_stencil==(var)8)					
										{								
											set(ent, CAST);							// required by PSSM - no self shadow on alpha material objects
										}
								}
							
							if ((shadow_stencil > (var)-1) && (shadow_stencil < (var)8) && (!main_blur))
								{
									set(ent , SHADOW);				
									ent.shadow = ent_unitshadow_rot;				// not okay with stencil blur
									
									// for proper decal-shadow size
									wait(1);
								//	ent.y += 32;
									reset(ent, SHADOW);
									wait(2);
								//	ent.y -= 32;
									set(ent, SHADOW);
									wait(1);	
								}
							else
								{
									reset(ent, SHADOW);			
									Decal_Set_Decal(ent, bmap_for_entity(ent_unitshadow_rot,0));	
								}
						}					
					else																	// no shadow , no lightmap
						{
							reset(ent, SHADOW);
							
							if (shadow_stencil==(var)8)
								{									
									set(ent, CAST);									// no self shadow on alpha material objects
								}
						}	
				}		
		}
	else
		{			
			if (shadow_stencil==(var)8)
				{
					reset(ent, SHADOW);
					set(ent, CAST);													// no self shadow on alpha material objects - definitely has an alpha skin
				}
		}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// save/load


void		Shrub_Save_Shrubs_shs(char* file_name)
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
	str_for_num( 			temp_str 	, shrub_counter );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
		
	// entity data - entity placed first will be written into file last
	you = ent_next(NULL); 									// get first entity
 	while (you) 												// repeat until there are no more entities
		{
			if ((you.ENT_CATEGORY == (var)UIMODE_SHRUB) && (!is(you,FLAG8)))				// if shrub, not part of wmb
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
					// first piece that creates a collider cube
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
					// SHADOW | CAST
					if ( is(you,SHADOW) )
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
					// material: sprite or category specific - okay
					if (you.material==mat_sprite)	
						{
							str_for_num( 	temp_str 	, 1 );
						}		
					else	// if (you.material==mtl_shrub)	
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
					// CLIP0 CLIP1
					if ((you.eflags&CLIP1) && (you.eflags&CLIP0))
						{
							str_for_num( 	temp_str 	, 0 );	// 25%
						}
					else if ((you.eflags&CLIP1) && (!(you.eflags&CLIP0)))
						{
							str_for_num( 	temp_str 	, 1 );	// 50%
						}
					else
						{
							str_for_num( 	temp_str 	, 2 );	// 75%
						}					
					file_str_write( 		file_handle , temp_str );
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);	
					// for further development----------------------------
					str_for_num( 			temp_str 	, 0 );
					int i;
					for(i=0;i<2;i++)
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
	
//	printf( "Shrub Map Saved: %s" , _chr(file_name) );
	
#ifndef MB_GAME
	// save panel
	reset( map_loadpanel , SHOW);
	reset( map_savetext1 , SHOW);
#endif
	
	ptr_remove(temp_str);
}


////////////////////////////////////////////////////


void		Shrub_Load_Shrubs_shs(char* file_name, var import)
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
	// shrubs count check
	file_str_read( file_handle , temp_str );
	var ents_to_load = str_to_num( temp_str );
	if ( (num_entities+ents_to_load) > max_entities )
		{
			printf( "LOAD ERROR: \n Too many entities to be added: %0.d" , (double)ents_to_load );
			return;
			
		}	
	//shrub_counter += str_to_int( temp_str );		// set during creation - any object creation could fail	
	
	// temporary variables storing properties of the currently read in entity
	STRING* my_filename = str_create("");
	var my_scale_x, my_scale_y, 
		my_x, my_y, my_z, 
		my_pan, my_tilt, 
		my_collider, my_alpha, my_ambient, 
		my_light, my_red, my_green, my_blue, 
		my_unlit, my_shadow, my_overlay, my_material,
		my_flag6, my_flag5, my_clip;
	var eof = 0;
	// while not eof read in entity data
	// after an entity read in, call creator function that includes importing, counting and setting everything
	
	random_seed(0);
	Shrub_SetDefaultSize();
	wait_for(Shrub_SetDefaultSize);
	
	//----------------------------------------------------------------
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
			
			// collider entity creator
			file_str_read( file_handle , temp_str );
			my_collider = str_to_num( temp_str );			
			
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
			
			// SHADOW			
			file_str_read( file_handle , temp_str );
			my_shadow = str_to_num( temp_str );
			
			// OVERLAY			
			file_str_read( file_handle , temp_str );
			my_overlay = str_to_num( temp_str );
			
			// 1 sprite material, 0 detail material
			file_str_read( file_handle , temp_str );
			my_material = str_to_num( temp_str );
			
			// FLAG6
			file_str_read( file_handle , temp_str );
			my_flag6 = str_to_num( temp_str );
			
			// FLAG5
			file_str_read( file_handle , temp_str );
			my_flag5 = str_to_num( temp_str );	
			
			// clip
			file_str_read( file_handle , temp_str );
			my_clip = str_to_num( temp_str );	
			
			// for further development----------------------------
			int i;
			for(i=0;i<2;i++)
				{
					file_str_read( file_handle , temp_str );
				}
				
			// maximize entity creating function instances
			while (proc_status (Shrub_CreateItem_FromFile) > 2000)
				{
					wait(6);
				}
			// create entity
			Shrub_CreateItem_FromFile( import, my_filename, 
												my_scale_x, my_scale_y, 
												my_x, my_y, my_z, 
												my_pan, my_tilt, 
												my_collider, my_alpha, my_ambient, 
												my_light, my_red, my_green, my_blue, 
												my_unlit, my_shadow, my_overlay, my_material,
												my_flag6, my_flag5, my_clip );			
			
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
	//printf( "Shrub Map Loaded: %s" , _chr(file_name) );

#ifndef MB_GAME
	// load panel
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
#endif
	
	ptr_remove(temp_str);
	ptr_remove(my_filename);
}


////////////////////////////////////////////////////


void Shrub_CreateItem_FromFile(	var imported, STRING* my_filename, 
											var my_scale_x, my_scale_y, 
											my_x, my_y, my_z, 
											my_pan, my_tilt, 
											my_collider, my_alpha, my_ambient, 
											my_light, my_red, my_green, my_blue, 
											my_unlit, my_shadow, my_overlay, my_material,
											my_flag6, my_flag5, my_clip)
{	
//	my_filename = str_lwr( my_filename ); 		
	if (!my_filename) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, shrub_foldername);
	str_cat(temp_str, my_filename);
	
	if ( file_exists(temp_str) )		// slow double check - not essential
		{
			//----------------------------------------------------------------------------
			// modify z if applied for another map
			
			if (imported)
				{			
					my_z = PosInfo3D4OrZeroLevel(my_x, my_y) + GetMapTileSize()/2*(my_scale_x*shrub_defaultsize/GetMapTileSize());  // () = original scale
				}				
			
			//----------------------------------------------------------------------------
			// create object
			
			Shrub_Create(	temp_str, 
								my_scale_x, my_scale_y, 
								my_x, my_y, my_z, 
								my_pan, my_tilt, 
								my_collider, my_alpha, my_ambient, 
								my_light, my_red, my_green, my_blue, 
								my_unlit, my_shadow, my_overlay, my_material,
								my_flag6, my_flag5, my_clip);
//			wait_for(Grass_Create);
			
			//----------------------------------------------------------------------------
//			
//			me = ent_create( temp_str , vector(my_x,my_y,my_z) , NULL );
//			
//			if (me!=NULL)		// check
//				{									
//					my.pan			= my_pan;
//					my.tilt			= my_tilt;					
//					
//					my.scale_x		= my_scale_x;
//					my.scale_y		= my_scale_y;	
//					
//					set( me , PASSABLE );
//					c_setminmax(me);   								
//					//--------------------------------------------------------------------------
//					// non-loaded settings - mainly tile system related
//					
//					// set tile pos storing skills
//					my.XTILE_POS = XCoordToTile(my.x);
//					my.YTILE_POS = YCoordToTile(my.y);
//					my.ZTILE_POS = 0;
//					my.ABSTILE_POS = XYTileToAbsTile(my.XTILE_POS, my.YTILE_POS);
//											
//					// set entity type
//					my.ENT_CATEGORY = (var)UIMODE_SHRUB;
////					my.ENT_CLIP     = 4;					//  random clipping at 25+50+75%
////					my.ENT_ANIM     = 0;	
////					my.ENT_HANDLE   = handle(me);		// not needed to save - can be stored on creation but not necessary...
//					my.AREA_SIZEX   = 1;
//					my.AREA_SIZEY   = 1;										
//					
//					if (my_clip==(var)2)
//						{
//							my.eflags |= CLIP0;	// 75%
//						}
//					else if (my_clip==(var)1)
//						{
//							my.eflags |= CLIP1; // 50%
//						}
//					else
//						{
//							my.eflags |= CLIP0 | CLIP1;	// 25%
//						}
//					
//					//---------------------------------------------							
//					
//					set(me , FLAG2 | FLAG1);
//					my.flags2 |= UNTOUCHABLE;						
//					
//					my.alpha			= my_alpha;	
//					if (my_alpha<(var)98)
//						{
//							set( me , TRANSLUCENT );
//						}
//					if (my_overlay)
//						{
//							set( me , OVERLAY );
//							reset( me , TRANSLUCENT );
//							my.alpha			= 100;	
//						}
//					my.ambient		= my_ambient;
//					
//					//---------------------------------------------
//					// shadows
//					var firstshrub		= (var)0;											// really used only by creator action
//					if ((my_shadow>0) || (my_flag5>0))									// only 1st entities are set to cast shadow on creation, thus this value can be easily estimated
//						{
//							firstshrub	= (var)1;																
//						}
//					Shrub_SetShadow(my, 1-my_flag6, my_flag5, my_shadow, firstshrub);	// flag6 is true when entity is excluded from lightmap casters
//					wait_for(Shrub_SetShadow);
//					//--------------------------------------------
//					if (my_light) 
//						{
//							set(my,LIGHT);
//							my.lightrange 	= 0;
//							my.red     		= my_red;
//							my.green   		= my_green;
//							my.blue    		= my_blue;	
//						}
//						
//					if (my_unlit)
//						{
//							set(   me , UNLIT );
//							reset( me , LIGHT );
//							my.material 	= mat_unlit;
//						}
//					else if (my_material)	
//						{
//							reset( me , UNLIT );
//							my.material 	= mat_sprite;		
//						}
//					else													// if no premade material set
//						{
//							reset( me , UNLIT );
//							my.material 	= mtl_shrub;
//						}
//					// if animated, set defaults
//					my.skill41 = floatv(50);		// speed x			10
//					my.skill42 = floatv(50);		// speed	y			25
//					my.skill43 = floatv(50);		//	oscillation		35
//					//-------------------------------------------------------------------
//					// collider entity
//					
//					if (my_collider)
//						{			
//							set(my , FLAG7);											// this sprite is used for making handlearray on game init
//							
//							ENTITY* obs_collider = ent_create( "graphics\\camera_cube.mdl" , my.x , NULL);
//							
//							while (!obs_collider) {wait(1);	}
//							
//							set( obs_collider , INVISIBLE );
//							set( obs_collider , FLAG2 | FLAG1 | FLAG6 );
//							obs_collider.flags2 |= UNTOUCHABLE;
//							obs_collider.scale_x = GetMapTileSize()/16 ;
//							obs_collider.scale_y = GetMapTileSize()/16 ;
//							obs_collider.scale_z = GetMapTileSize()/16 ;
//							obs_collider.ENT_CATEGORY = (var)UIMODE_SHRUB + (var)UIMODE_SHADOWSHIFT;
//							obs_collider.XTILE_POS = XCoordToTile(my.x);
//							obs_collider.YTILE_POS = YCoordToTile(my.y);
//							// to manually set bounding box
//							wait(1);	
//				//			obs_collider.pan = my.pan;								// (should be after a wait) ONLY IN FREE EDITION - IT SHOULD BE 0,90,180 OR 270 DEGREE ONLY !!!!!!!!!!!!!!!							
//							c_setminmax(obs_collider);
//							obs_collider.eflags |= FAT | NARROW; 				// set both flags to prevent automatic recalculation on scale changes			
//							obs_collider.min_x = -GetMapTileSize()/2 ;
//							obs_collider.min_y = -GetMapTileSize()/2 ;
//							obs_collider.max_x =  GetMapTileSize()/2 ;
//							obs_collider.max_y =  GetMapTileSize()/2 ;				
//							obs_collider.min_z = -GetMapTileSize()/2;
//							obs_collider.max_z =  GetMapTileSize()/2;		
//							obs_collider.group = 1;
//							
//							obs_collider = NULL;
//							ptr_remove(obs_collider);															
//						}	
//					//------------------------------------------------------------------	
//					wait(1);	// needed because of pan and tilt - every shrubs has the same pan and tilt if made in the same frame														
////					if ((is(my,SHADOW)) && (shadow_stencil<8))
////						{
////							wait(1);
//////							me.z += 16;
////							reset(my,SHADOW);
////							wait(1);
//////							me.z -= 16;
////							set(my,SHADOW);
////							wait(1);
////						}
//					// not called anymore, so set static---------------------------------					
//					my.emask &= ~DYNAMIC;	
//					shrub_counter++;
//					
//				}
		}
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


function Shrub_RemoveObject(ENTITY* ent)
{
	if (ent == NULL)
		{
			return 0;
		}
	
	if (ent.ENT_CATEGORY != (var)UIMODE_SHRUB)
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
	
//	// remove unique shadow sprite or model
//	Shrub_RemoveChild(ent);
	
	// remove the main entity
	ptr_remove(ent);		
	
	// decrease quantity
	shrub_counter--;
	
	return 1;
}


//void		Shrub_RemoveChild(ENTITY* owner)
//{
//	you = ent_next(NULL); 
// 	while (you) 	
//		{
//			if ( (you.XTILE_POS == owner.XTILE_POS) && (you.YTILE_POS == owner.YTILE_POS) )
//				{
//					if (you.ENT_CATEGORY == ((var)UIMODE_SHRUB + (var)UIMODE_SHADOWSHIFT) )
//						{
//							if (you.parent == owner)
//								{
//									ent_remove(you);
//									
//									return;
//								}
//						}
//				}
//			
//			you = ent_next(you);
//		}
//}


//-------------------------------------


int		Shrub_RemoveArea(int midtilex, int midtiley, int areasizex, int areasizey)
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
					if (you.ENT_CATEGORY == (var)UIMODE_SHRUB)
						{
							// store next entity
							temp_ent = ent_next(you); 
													
							// remove its ent_decal shadow attached to terrain or terrain models
							Decal_Remove_Decal(you);
							wait_for(Decal_Remove_Decal);
							
							// remove the main entity
							ptr_remove(you);  										
							
							// decrease quantity
							shrub_counter--;
							
							removed = 1;
							counter++;	
						}						
					// unique shadows and collision models (they are in child and parent relations to the entity, but category is shifted)
					else if (you.ENT_CATEGORY == ((var)UIMODE_SHRUB + (var)UIMODE_SHADOWSHIFT) )
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


void		Shrub_RemoveAll()
{
	Remove_EntCats( UIMODE_SHRUB );
	wait_for(Remove_EntCats);
	
	shrub_counter = 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////



#endif