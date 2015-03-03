#ifndef MBgrass_c
#define MBgrass_c

////////////////////////////////////////////////////////////////////////////////////////////////
// code


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// create


void		Grass_Create(	STRING* filename, 																		
									var my_scale_x, my_scale_y, my_scale_z,		
									my_x, my_y, my_z, 
									my_pan, my_tilt, my_roll,									
									my_alpha, my_ambient, 
									my_light, my_red, my_green, my_blue, 
									my_unlit, my_overlay, my_material,										
									my_shadow)
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
					
					// salefy keep angles between -180...180 degree				
					ent.pan			= ang(my_pan);
					ent.tilt			= ang(my_tilt);
					ent.roll			= ang(my_roll);									
					
					//------------------------------------------------------------------------------------------------------------
					
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
					
					ent.material 	= mtl_grass;
					
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
							ent.material 	= mat_model;		
						}
					
					//------------------------------------------------------------------------------------------------------------
					// set normals and lod materials
					
					Grass_SetNormals(ent);
					wait_for(Grass_SetNormals);
					
					Grass_CreateLODmodels(ent);
					wait_for(Grass_CreateLODmodels);	
					
					//---------------------------------------------------------------------------------------------
					// store data in skills
					
					// set tile pos
					ent.XTILE_POS = XCoordToTile(ent.x);
					ent.YTILE_POS = YCoordToTile(ent.y);
					ent.ZTILE_POS = 0;
					ent.ABSTILE_POS = XYTileToAbsTile(ent.XTILE_POS, ent.YTILE_POS);
					
					// set entity type and size
					ent.ENT_CATEGORY = (var)UIMODE_GRASS;
//					ent.ENT_CLIP     = 1;													//  clipping at 75%
//					ent.ENT_ANIM     = 0;	

					ent.AREA_SIZEX   = 1; 					
					ent.AREA_SIZEY   = 1; 						
					ent.ENT_TYPE 	 = 1; 													// passable	
					
//					// collision group - no collision!
//					ent.group = 3;
					
					//--------------------------------------------------------------------------
					// set appearance properties
					
//					str_for_entfile(temp_str, ent);										// already set
					
					if (str_stri(temp_str,"_0.") == (var)0)
						{
							ent.eflags |= CLIP0 | CLIP1;									// 25% if no lod
						}
					else
						{
							ent.eflags |= CLIP1; 											// 50% if has lod
						}	
					
					//------------------------------------------------------------------------------------------------------------
					// shadow - no lightmap possible because no collision thus not c_trace-able
					
					Grass_SetShadow(ent, my_shadow);
					wait_for(Grass_SetShadow);
					
					//------------------------------------------------------------------------------------------------------------
					
					grass_counter++;
						
					wait(1);																		// needed to ensure correct pan tilt roll and scale		
					
					ent.emask &= ~DYNAMIC;	
					
					//--------------------------------------------------------------------------------------------------------------						
					// safety nexus check - editor only!
					
#ifndef MB_GAME					
					Map_NexusCheck();
#endif					
					//-------------------------------------------------------------------------------------------------------------
					// level_ent
					
					if (!terrain_entity)
						{
							SentLevelEntSizeToEnt(ent);
						}
					
					//------------------------------------------------------------------------------------------------------------
				}
				
	ptr_remove(temp_str);
}									


void		Grass_SetNormals(ENTITY* ent)
{
//	// simple
//	
////	VECTOR min_vec;
////	vec_for_min(min_vec, ent);
////	VECTOR max_vec;
////	vec_for_max(max_vec, ent);
////	
////	VECTOR average_vec;													// DX y is height , 3dgs z is height
////	average_vec.x = 0;
////	average_vec.z = 0;
////	average_vec.y = abs(max_vec.z + min_vec.z)/2;	
//	
//	int i;
//	for (i=1; i<=(var)(ent_status(ent, 0)); i++)
//		{	
//			CONTACT* c = ent_getvertex(ent, NULL, i);   			// vertex number begins with 1 !			
//			
//			c.v.nx = (float)0; 											// or 0.7071 @ 2
//			c.v.nz = (float)0; 				
//			c.v.ny = (float)1; 											// upwards - uniform lighting
////			if ((var)c.v.y > average_vec.y)
////				{
////					c.v.ny = (float)1; 									// upwards
////				}
////			else
////				{
////					c.v.ny = (float)-1; 									// downwards
////				}
//			
//			ent_setvertex(ent, c, i);
//		}
//		
	//--------------------------------------------------------------------------------------------
	// complex
	
	// reset normals to MED default
//	LPD3DXMESH dx_mesh1;
	int i;
	for (i=0; i<(var)(ent_status(ent,9)); i++)
		{
			dx_mesh = ent_getmesh(ent, i, 0);															// ent , mesh subset , internal lod
			D3DXComputeNormals(dx_mesh,NULL);
			ent_setmesh(ent,dx_mesh,i,0);
			dx_mesh = NULL;			
		}
//	dx_mesh1 = ent_getmesh(NULL,0,0);																	// not needed
//	dx_mesh1->Release();
	//-----------------------------------------------------
	
	for (i=1; i<=(var)(ent_status(ent, 0)); i++)
		{	
			CONTACT* c = ent_getvertex(ent, NULL, i);   												// vertex number begins with 1 !			
			
			VECTOR nxnynz_vec;
			vec_set(nxnynz_vec,vector((var)c.v.nx, (var)c.v.ny, (var)c.v.nz));				// original normals
			vec_normalize(nxnynz_vec,1);
			
			VECTOR y_vec;
			vec_set(y_vec, vector(0,1,0));
			
			VECTOR temp_vec;
			var smoothing_factor = 0.05;																	// 0: (0,1,0) uniform	;		1: original
			vec_lerp(temp_vec, y_vec, nxnynz_vec, smoothing_factor);								// new normals iterated between normalized position vec and orig normal
			vec_normalize(temp_vec,1);
			
			c.v.nx = (float)temp_vec.x; 			
			c.v.ny = (float)temp_vec.y; 
			c.v.nz = (float)temp_vec.z; 																			
			
			ent_setvertex(ent, c, i);
		}
}


void		Grass_CreateLODmodels(ENTITY* ent)
{
	STRING* temp_grass_str = str_create("");
	int i;
	str_for_entfile(temp_grass_str,ent);					// get e.g. tree002_0.mdl
		
	if (str_stri(temp_grass_str,"_0.")>0)				// it is a lod0 model - does not work - case sensitive !!! - not essential
		{			
			str_trunc(temp_grass_str,5);	
			str_cat(temp_grass_str, "1.mdl");						
			you = ent_for_file(temp_grass_str);					// does first lod model exist?
			if (!you)													// check whether an entity already exists with this name or not - maybe faster
				{							
					// create invisible lod models if they do not already exist - okay - but too many entities																							
					for(i=1;i<4;i++)											// cycle through LOD stages 1 and 2 - really good, 3 is sprite based double sided crossed planes - a bit better if applied
						{
							str_trunc(temp_grass_str,5);					// remove 0.mdl from end, and 1.mdl in next cycle
							str_cat(temp_grass_str, str_for_num(NULL,(var)i));
							str_cat(temp_grass_str, ".mdl");
							
							you = ent_for_file(temp_grass_str);			// get pointer to lod model if exists
							if (!you)											// or check existance
								{
									// add path! 
									STRING* temp6_str = str_create("");
									str_cpy(temp6_str,grass_foldername);
									str_cat(temp6_str,temp_grass_str);
					
									if (file_exists(temp6_str))				
										{
	//										collision_mode = 0; 			// has no effect here...
											ent_create(temp6_str, vector(0,0,-GetMapTileSize()*100), Grass_CreateLOD);	// create a hidden LOD model
											wait_for(ent_create);
	//										collision_mode = 1; 
										}
									
									ptr_remove(temp6_str);
								}																		
						}
				}	// you
		}		
}


action		Grass_CreateLOD()
{
	my.alpha = 100;
	
	reset(my, SHADOW | TRANSLUCENT);							// ?
	set(my, PASSABLE | INVISIBLE | FLAG1 | FLAG2);		// no mirror, no decal shadows on it
	
	if (shadow_stencil==(var)8)
		{
			reset(my,CAST);
		}
	else
		{
			set(my,CAST);
		}
	
	my.flags2 |= UNTOUCHABLE;
	
	my.ENT_CATEGORY = (var)UIMODE_GRASS + (var)UIMODE_SHADOWSHIFT;
	
//	wait(1);
	
	Grass_SetNormals(my);
	wait_for(Grass_SetNormals);
	
	wait(1);
	my.emask &= ~DYNAMIC;										// set entity to static
}


void		Grass_SetDefaultSize()
{
	// grass imagesize determination: 256 (low quality) or 512 (high quality)
	STRING* temp_str = str_create("");
	var file_handle = file_open_read(grasssize_name);
	if (file_handle)
		{
			file_str_read( file_handle , temp_str );
			grass_defaultsize = str_to_num( temp_str );
			file_close(file_handle);
		}
	else
		{
			grass_defaultsize = GRASS_SIZE1;
		}
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// shadows


void	Grass_SetShadow(ENTITY* ent, var grass_shadow)
{
	if (grass_shadow)
		{
			set(ent , FLAG6);													// has shadow (but always excluded from lightmap)
			
			if ((shadow_stencil > (var)-1) && (shadow_stencil < (var)8))
				{
					if (!main_blur)
						{
							set(ent , SHADOW);				
							ent.shadow = ent_unitshadow_rot;				// not okay with stencil blur
						}
					else
						{
							reset(ent, SHADOW);			
							Decal_Set_Decal(ent, bmap_for_entity(ent_unitshadow_rot,0));	
						}
				}
			else
				{
					set( ent 		, SHADOW );
					
					if (shadow_stencil==(var)8)
						{
							set( ent 		, CAST );						// without it casted shadows look through grass models
						}
				}
		}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// save/load


void		Grass_Save_Grasses_grm(char* file_name)
{
{}
#ifndef MB_GAME	
	// save panel
	set( map_loadpanel , SHOW);
	set( map_savetext1 , SHOW);
	wait(1);
#endif
	
	var temp_var = 0;
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_write(temp_str);
	//---------------------------------------------
	// entity count
	str_for_num( 			temp_str 	, grass_counter );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
		
	// entity data - entity placed first will be written into file last
	you = ent_next(NULL); 									// get first entity
 	while (you) 												// repeat until there are no more entities
		{
			if ((you.ENT_CATEGORY == (var)UIMODE_GRASS) && (!is(you,FLAG8)))				// if grass, not part of wmb
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
					else	// if (you.material==mtl_grass)	
						{
							str_for_num( 	temp_str 	, 0 );
						}
					file_str_write( 		file_handle , temp_str );
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// SHADOW
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
					// for further development----------------------------
					str_for_num( 			temp_str 	, 0 );
					int i;
					for(i=0;i<4;i++)
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
	
//	printf( "Grass Map Saved: %s" , _chr(file_name) );
	
#ifndef MB_GAME	
	// save panel
	reset( map_loadpanel , SHOW);
	reset( map_savetext1 , SHOW);
#endif
		
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////


void		Grass_Load_Grasses_grm(char* file_name, var import)
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
	// details count check - only for safety
	file_str_read( file_handle , temp_str );
	var ents_to_load = str_to_num( temp_str );
	if ( (num_entities+ents_to_load) > (max_entities-1000) )					// keep 1000 entity slots for further game entities
		{
			printf( "LOAD ERROR: \n Too many entities to be added: %0.d" , (double)ents_to_load );
			return;
		}	
	//grass_counter += str_to_int( temp_str );  // set during creation - any object creation could fail
	
	// temporary variables storing properties of the currently read in entity
	STRING* my_filename = str_create("");
	var my_scale_x, my_scale_y, my_scale_z,
		my_pan, my_tilt, my_roll,
		my_x, my_y, my_z, 
		my_alpha, my_ambient, 
		my_light, my_red, my_green, my_blue, 
		my_unlit, my_overlay, my_material,
		my_shadow;
	var eof = 0;
	// while not eof read in entity data
	// after an entity read in, call creator function that includes importing, counting and setting everything
	collision_mode = 0; 
	random_seed(0);
	Grass_SetDefaultSize();
	wait_for(Grass_SetDefaultSize);
	//-----------------------------------------------------------------------------
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
			
			// pan
			file_str_read( file_handle , temp_str );
			my_pan = str_to_num( temp_str );
			
			// tilt
			file_str_read( file_handle , temp_str );
			my_tilt = str_to_num( temp_str );
			
			// roll
			file_str_read( file_handle , temp_str );
			my_roll = str_to_num( temp_str );
			
			// x
			file_str_read( file_handle , temp_str );
			my_x = str_to_num( temp_str );
			
			// y
			file_str_read( file_handle , temp_str );
			my_y = str_to_num( temp_str );
			
			// z
			file_str_read( file_handle , temp_str );
			my_z = str_to_num( temp_str );
			
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
			
			// 1 model material, 0 grass material
			file_str_read( file_handle , temp_str );
			my_material = str_to_num( temp_str );
			
			// SHADOW			
			file_str_read( file_handle , temp_str );
			my_shadow = str_to_num( temp_str );
			
			// for further development----------------------------
			int i;
			for(i=0;i<4;i++)
				{
					file_str_read( file_handle , temp_str );
				}
				
			// maximize entity creating function instances
			while (proc_status (Grass_CreateItem_FromFile) > 2000)
				{
					wait(6);
				}
			// create entity
			Grass_CreateItem_FromFile( import, my_filename, 
												my_scale_x, my_scale_y, my_scale_z,
												my_x, my_y, my_z, 
												my_pan, my_tilt, my_roll,												
												my_alpha, my_ambient, 
												my_light, my_red, my_green, my_blue, 
												my_unlit, my_overlay, my_material,
												my_shadow );
			
			ents_counter++;
			if (ents_counter%1000==(var)0)		// at every 500th entity loading
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
	collision_mode = 1; 
	
	//printf( "Grass Map Loaded: %s" , _chr(file_name) );
	
#ifndef MB_GAME	
	// load panel
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
#endif
		
	ptr_remove(temp_str);
	ptr_remove(my_filename);
}


//////////////////////////////////////////////////


void Grass_CreateItem_FromFile(	var imported, STRING* my_filename, 
											var my_scale_x, my_scale_y, my_scale_z,
											my_x, my_y, my_z, 
											my_pan, my_tilt, my_roll,
											my_alpha, my_ambient, 
											my_light, my_red, my_green, my_blue, 
											my_unlit, my_overlay, my_material,
											my_shadow)
{	
//	my_filename = str_lwr( my_filename ); 		
	if (!my_filename) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, grass_foldername);
	str_cat(temp_str, my_filename);
	
	if ( file_exists(temp_str) )		// slow double check - not essential
		{
			//----------------------------------------------------------------------------
			// modify z if applied for another map
			
			if (imported)
				{			
					my_z = PosInfo3D4OrZeroLevel(my_x, my_y) ;  
				}				
			
			//----------------------------------------------------------------------------
			// create object
			
			Grass_Create(	temp_str, 																		
								my_scale_x, my_scale_y, my_scale_z,		
								my_x, my_y, my_z, 
								my_pan, my_tilt, my_roll,									
								my_alpha, my_ambient, 
								my_light, my_red, my_green, my_blue, 
								my_unlit, my_overlay, my_material,										
								my_shadow);
//			wait_for(Grass_Create);
			
			//----------------------------------------------------------------------------
		}
	ptr_remove(temp_str);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


function Grass_RemoveObject(ENTITY* ent)
{
	if (ent == NULL)
		{
			return 0;
		}
	
	if (ent.ENT_CATEGORY != (var)UIMODE_GRASS)
		{
			return 0;
		}
	
	// remove decal attached to terrain or terrain models
	Decal_Remove_Decal(ent);
	wait_for(Decal_Remove_Decal);
	
//	// remove unique shadow sprite or model
//	Grass_RemoveChild(ent);
	
	// remove the main entity
	ptr_remove(ent);		
	
	// decrease quantity
	grass_counter--;
	
	return 1;
}


//void		Grass_RemoveChild(ENTITY* owner)
//{
//	you = ent_next(NULL); 
// 	while (you) 	
//		{
//			if ( (you.XTILE_POS == owner.XTILE_POS) && (you.YTILE_POS == owner.YTILE_POS) )
//				{
//					if (you.ENT_CATEGORY == ((var)UIMODE_GRASS + (var)UIMODE_SHADOWSHIFT) )
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


int		Grass_RemoveArea(int midtilex, int midtiley, int areasizex, int areasizey)
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
					if (you.ENT_CATEGORY == (var)UIMODE_GRASS)
						{
							// store next entity
							temp_ent = ent_next(you); 
													
							// remove its ent_decal shadow attached to terrain or terrain models
							Decal_Remove_Decal(you);
							wait_for(Decal_Remove_Decal);
							
							// remove the main entity
							ptr_remove(you);  										
							
							// decrease quantity
							grass_counter--;
							
							removed = 1;
							counter++;	
						}						
					// unique shadows and collision models (they are in child and parent relations to the entity, but category is shifted)
					else if (you.ENT_CATEGORY == ((var)UIMODE_GRASS + (var)UIMODE_SHADOWSHIFT) )
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


void		Grass_RemoveAll()
{
	Remove_EntCats( UIMODE_GRASS );
	wait_for(Remove_EntCats);
	
	grass_counter = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif