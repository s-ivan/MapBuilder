
#ifndef MBterrmdl_c
#define MBterrmdl_c

////////////////////////////////////////////////////////////////////////////////////////////////
// code


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// create


void		TerrMdl_Create( 	STRING* filename, 
									
									var my_scale_x, my_scale_y, my_scale_z,
									my_x, my_y, my_z,
									my_pan, my_tilt, my_roll,
									
									my_ambient,
									my_light, my_red, my_green, my_blue,
									my_unlit, my_material, my_shadow)
{
	if (!filename) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, filename);
	
	//----------------------------------------------------------------------------
	
	ENTITY* ent = ent_create(temp_str, vector(my_x, my_y, my_z), NULL);						// changing my to ent resolved the strange ui loop stopping bug
	
//	while(!ent) {wait(1);}
	
			if (ent != NULL)										// check, or while/wait
				{
					//---------------------------------------------------------------------------------------------
					
					ent.scale_x		= my_scale_x;
					ent.scale_y		= my_scale_y;
					ent.scale_z		= my_scale_z;
					
					// salefy keep angles between -180...180 degree				
					ent.pan			= ang(my_pan);
					ent.tilt			= ang(my_tilt);
					ent.roll			= ang(my_roll);										
					
					//---------------------------------------------------------------------------------------------
					
					ent.alpha		= 100;	
					ent.ambient		= my_ambient;
					
					//---------------------------------------------------------------------------------------------
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
					
					//---------------------------------------------------------------------------------------------
					// general materials	
					
					ent.material 	= mtl_terrmdl;
										
					if (my_unlit)
						{
							set(   ent , UNLIT );					// keeps ambient settings
							reset( ent , LIGHT );
							ent.material 	= mat_unlit;
						}
					else if (my_material==1)	
						{
//							reset( ent , UNLIT );
							ent.material 	= mat_terrain;		
						}
					
					//---------------------------------------------------------------------------------------------
					// store data in skills
					
					// set tile pos
					ent.XTILE_POS = XCoordToTile(ent.x);
					ent.YTILE_POS = YCoordToTile(ent.y);
					ent.ZTILE_POS = 0;
					ent.ABSTILE_POS = XYTileToAbsTile(ent.XTILE_POS, ent.YTILE_POS);
					
					// set entity type and size
					ent.ENT_CATEGORY = (var)UIMODE_TERRMDL;
//					ent.ENT_CLIP     = 1;													//  clipping at 75%
//					ent.ENT_ANIM     = 0;	
//#ifndef MB_RELEASE	
//					ent.ENT_HANDLE   = handle(ent);										// used by the RTS building cluster creation system
//#endif
					ent.AREA_SIZEX   = 1; 
					ent.AREA_SIZEY   = 1; 
//					ent.ENT_TYPE 	 = obs_type; 										// store tile passability for pathfinder *** new ***
					
					// collision group
					ent.group = 2;
					
					//---------------------------------------------------------------------------------------------
					
					ent.eflags |= CLIP0;	// 75%										
					
					//---------------------------------------------------------------------------------------------
					// set collision	
					
					wait(1);	
					
					set(ent, POLYGON );
					
					c_setminmax(ent);
					
					//--------------------------------------------------------------------------------------------------------------	
					// set shadow type
	
					// include in lightmap, force ent_decal, shadow no/dynamic/sprite/model
					TerrMdl_SetShadow(ent, my_shadow);
//					wait_for(TerrMdl_SetShadow);															// needed because of decal and ent_decal shadows, to wait for completion before setting entity to static
					
					//---------------------------------------------------------------------------------------------
					
					terrmdl_counter++;	
					
					// not called anymore, so set static
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
					
					//---------------------------------------------------------------------------------------------	
				}
	
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// shadows


void	TerrMdl_SetShadow(ENTITY* ent, var terrmdl_shadow)
{
	if (terrmdl_shadow>0)
		{
			set(ent, SHADOW);
			set(ent , FLAG6);								// has shadow, to exclude from static lightmapping
			
			if (shadow_stencil==(var)8)
				{
					if (AlphaMaterialTest(ent))
						{
							set(ent, CAST);				// no self shadow on alpha material objects
						}
				}
		}	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// save/load


void		TerrMdl_Save_TerrMdls_tem(char* file_name)
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
	str_for_num( 			temp_str 	, terrmdl_counter );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
		
	// entity data - entity placed first will be written into file last
	you = ent_next(NULL); 									// get first entity
 	while (you) 												// repeat until there are no more entities
		{
			if ((you.ENT_CATEGORY == (var)UIMODE_TERRMDL) && (!is(you,FLAG8)))				// if terrain model, not part of wmb
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
					// material: terrain or category specific - okay
					if (you.material==mat_terrain)	
						{
							str_for_num( 	temp_str 	, 1 );
						}	
					else	// if (you.material==mtl_terrmdl)	
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
	
//	printf( "Terrain Model Map Saved: %s" , _chr(file_name) );
	
#ifndef MB_GAME
	// save panel
	reset( map_loadpanel , SHOW);
	reset( map_savetext1 , SHOW);
#endif	
	
	ptr_remove(temp_str);
}


///////////////////////////////////////////////////////////


void		TerrMdl_Load_TerrMdls_tem(char* file_name, var import)
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
	//mapent_counter += str_to_int( temp_str );		// be set during creation - any object creation could fail
		
	//------------------------------------------------------------------------
	// temporary variables storing properties of the currently read in entity
	STRING* my_filename = str_create("");
	var my_scale_x, my_scale_y, my_scale_z,
		my_x, my_y, my_z, 
		my_pan, my_tilt, my_roll,
		my_ambient, 
		my_light, my_red, my_green, my_blue, 
		my_unlit, my_material, my_shadow;
	var eof = 0;
	// while not eof read in entity data
	// after an entity read in, call creator function that includes importing, counting and setting everything
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
			
			// 1 model material, 0 obstacle material
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
			while (proc_status (TerrMdl_CreateItem_FromFile) > 2000)
				{
					wait(10);
				}
			// create entity
			TerrMdl_CreateItem_FromFile( import, my_filename, 
										my_scale_x, my_scale_y, my_scale_z,
										my_x, my_y, my_z,
										my_pan, my_tilt, my_roll,									
										my_ambient, 
										my_light, my_red, my_green, my_blue, 
										my_unlit, my_material, my_shadow);	
			
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
	//printf( "Terrain Model Map Loaded: %s" , _chr(file_name) );
		
#ifndef MB_GAME		
	// load panel
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
#endif
		
	ptr_remove(temp_str);
	ptr_remove(my_filename);
}


///////////////////////////////////////////////////////////


void TerrMdl_CreateItem_FromFile(	var imported, STRING* my_filename, 
												var my_scale_x, my_scale_y, my_scale_z,
												my_x, my_y, my_z,
												my_pan, my_tilt, my_roll, 
												my_ambient, 
												my_light, my_red, my_green, my_blue, 
												my_unlit, my_material, my_shadow)
{
//	my_filename = str_lwr( my_filename ); 		
	if (!my_filename) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str,terrmdl_foldername);
	str_cat(temp_str,my_filename);
	
	if ( file_exists(temp_str) )		// slow double check - not essential
		{
			//----------------------------------------------------------------------------
			// modify z if applied for another map
			
			if (imported)
				{			
					my_z = PosInfo3D1OrZeroLevel(my_x, my_y) ;  
				}	
			
			//----------------------------------------------------------------------------
			// create object
			
			TerrMdl_Create(	temp_str, 
									
									my_scale_x, my_scale_y, my_scale_z,		
									my_x, my_y, my_z, 
									my_pan, my_tilt, my_roll,
									
									my_ambient, 
									my_light, my_red, my_green, my_blue, 
									my_unlit, my_material, my_shadow);
//			wait_for(TerrMdl_Create);
			
			//----------------------------------------------------------------------------				
		}
	ptr_remove(temp_str);
}


////////////////////////////////////////////////////////////////////////////////////
// removal


function TerrMdl_RemoveObject(ENTITY* ent)
{
	if (ent == NULL)
		{
			return 0;
		}
	
	if (ent.ENT_CATEGORY != (var)UIMODE_TERRMDL)
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
	
	// remove the main entity
	ptr_remove(ent);		
	
	// decrease quantity
	terrmdl_counter--;
	
	return 1;
}


//-------------------------------------


int		TerrMdl_RemoveArea(int midtilex, int midtiley, int areasizex, int areasizey)
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
					if (you.ENT_CATEGORY == (var)UIMODE_TERRMDL)
						{
							// store next entity
							temp_ent = ent_next(you); 
													
							// remove its ent_decal shadow attached to terrain or terrain models
							Decal_Remove_Decal(you);
							wait_for(Decal_Remove_Decal);
							
							// remove the main entity
							ptr_remove(you);  										
							
							// decrease quantity
							terrmdl_counter--;
							
							removed = 1;
							counter++;	
						}						
					// unique shadows and collision models (they are in child and parent relations to the entity, but category is shifted)
					else if (you.ENT_CATEGORY == ((var)UIMODE_TERRMDL + (var)UIMODE_SHADOWSHIFT) )
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


void		TerrMdl_RemoveAll()
{
	Remove_EntCats( UIMODE_TERRMDL );
	wait_for(Remove_EntCats);
	
	terrmdl_counter = 0;
}


////////////////////////////////////////////////////////////////////////////////////


#endif