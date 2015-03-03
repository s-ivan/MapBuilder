#ifndef MBdetail_c
#define MBdetail_c


////////////////////////////////////////////////////////////////////////////////////////////////
// code


void Detail_Create( STRING* filename, 
							var my_scale, 
							my_x, my_y, my_z, 
							my_alpha, my_ambient, 
							my_light, my_red, my_green, my_blue, 
							my_unlit, my_overlay, my_material)
{
	if (!filename) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, filename);
	
	//----------------------------------------------------------------------------
	
//	collision_mode = 0;																						// no collision needed - even for lightmapping, because temp models are needed and used for c-tracing
	
	ENTITY* ent = ent_create(temp_str, vector(my_x, my_y, my_z), NULL);						// changing my to ent resolved the strange ui loop stopping bug
	
//	collision_mode = 1;																						// needed for collider entity
	
//	while(!ent) {wait(1);}
	
			if (ent != NULL)										// check, or while/wait
				{
					//------------------------------------------------------------------------------------------------------------
					
					ent.scale_x		= my_scale;
					ent.scale_y		= my_scale;
					
					// pan tilt roll is always 0 to face to camera
					
					//------------------------------------------------------------------------------------------------------------
					
					set(ent, PASSABLE | FLAG2 | FLAG1 | FLAG6);		
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
					
					ent.material 	= mtl_detail;
					
					// set tree vertex anim shader parameters - tree material is set later
					ent.skill41 = floatv(50);												// speed x
					ent.skill42 = floatv(50);												// speed	y
					ent.skill43 = floatv(50);												//	oscillation	
//					ent.skill44 = floatv(ent.max_z/3);									// bending position
					ent.skill44 = floatv(0);												// bending position
					
					if (my_unlit)
						{
							set(   ent , UNLIT );											// keeps ambient settings
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
					ent.ENT_CATEGORY = (var)UIMODE_DETAIL;
//					ent.ENT_CLIP     = 1;													//  clipping at 75%
//					ent.ENT_ANIM     = 0;	

					ent.AREA_SIZEX   = 1; 					
					ent.AREA_SIZEY   = 1; 						
					ent.ENT_TYPE 	 = 0; 													// impassable	
					
//					// collision group - no collision!
//					ent.group = 3;
					
					//--------------------------------------------------------------------------
					// set appearance properties
					
					var dist = random(10);
					
					if (dist < 9)
						{
							ent.eflags |= CLIP0 | CLIP1;									// 25%
						}	
					else
						{
							ent.eflags |= CLIP1; 											// 50%
						}
					
					//------------------------------------------------------------------------------------------------------------
					// shadow
					
					Detail_SetShadow(ent);
					
					//------------------------------------------------------------------------------------------------------------
					
					detail_counter++;
						
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
					
					//-------------------------------------------------------------------------------------------------------------
				}
	ptr_remove(temp_str);
}


void		Detail_SetDefaultSize()
{
	// detail imagesize determination: 256 (low quality) or 512 (high quality)
	STRING* temp_str = str_create("");
	var file_handle = file_open_read(detailsize_name);
	if (file_handle)
		{
			file_str_read( file_handle , temp_str );
			detail_defaultsize = str_to_num( temp_str );
			file_close(file_handle);
		}
	else
		{
			detail_defaultsize = DETAIL_SIZE1;
		}
	ptr_remove(temp_str);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// shadows


void	Detail_SetShadow(ENTITY* ent)
{
	if (shadow_stencil == (var)8)
		{
			set(ent, CAST);
		}	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// save/load


void		Detail_Save_Details_grs(char* file_name)
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
	str_for_num( 			temp_str 	, detail_counter );
	file_str_write( 		file_handle , temp_str );
		file_asc_write(	file_handle , 13); 
		file_asc_write(	file_handle , 10);
		
	// entity data - entity placed first will be written into file last
	you = ent_next(NULL); 									// get first entity
 	while (you) 												// repeat until there are no more entities
		{
			if ((you.ENT_CATEGORY == (var)UIMODE_DETAIL) && (!is(you,FLAG8)))				// if detail / grass , not part of wmb
				{
					// name
					str_for_entfile( 		temp_str 	, you );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// scale_X
					str_for_num( 			temp_str 	, you.scale_x );
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
					// material: sprite or category specific - okay
					if (you.material==mat_sprite)	
						{
							str_for_num( 	temp_str 	, 1 );
						}		
					else	// if (you.material==mtl_detail)	
						{
							str_for_num( 	temp_str 	, 0 );
						}
					file_str_write( 		file_handle , temp_str );
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					
					// for further development----------------------------
					str_for_num( 			temp_str 	, 0 );
					int i;
					for(i=0;i<5;i++)
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


////////////////////////////////////////////////////


void		Detail_Load_Details_grs(char* file_name, var import)
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
	//detail_counter += str_to_int( temp_str );  // set during creation - any object creation could fail
	
	// temporary variables storing properties of the currently read in entity
	STRING* my_filename = str_create("");
	var my_scale_x, my_x, my_y, my_z, 
		my_alpha, my_ambient, 
		my_light, my_red, my_green, my_blue, 
		my_unlit, my_overlay, my_material;
	var eof = 0;
	// while not eof read in entity data
	// after an entity read in, call creator function that includes importing, counting and setting everything
	collision_mode = 0; 
	
	random_seed(0);
	Detail_SetDefaultSize();
	wait_for(Detail_SetDefaultSize);
	
	//--------------------------------------------------------------------
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
			
			// 1 sprite material, 0 detail material
			file_str_read( file_handle , temp_str );
			my_material = str_to_num( temp_str );
			
			// for further development----------------------------
			int i;
			for(i=0;i<5;i++)
				{
					file_str_read( file_handle , temp_str );
				}	
						
			// maximize entity creating function instances
			while (proc_status (Detail_CreateItem_FromFile) > 2000)
				{
					wait(6);
				}
			// create entity
			Detail_CreateItem_FromFile( 	import, my_filename, 
													my_scale_x, 
													my_x, my_y, my_z, 
													my_alpha, my_ambient, 
													my_light, my_red, my_green, my_blue, 
													my_unlit, my_overlay, my_material );
			
			ents_counter++;
			if (ents_counter%1000==(var)0)		// at every 1000th entity loading
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
	
	//printf( "Detail Map Loaded: %s" , _chr(file_name) );

#ifndef MB_GAME
	// load panel
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
#endif
		
	ptr_remove(temp_str);
	ptr_remove(my_filename);
}


////////////////////////////////////////////////////


void Detail_CreateItem_FromFile(	var imported, STRING* my_filename, 
											var my_scale, 
											my_x, my_y, my_z, 
											my_alpha, my_ambient, 
											my_light, my_red, my_green, my_blue, 
											my_unlit, my_overlay, my_material)
{
	//	my_filename = str_lwr( my_filename ); 		
	if (!my_filename) return;
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str, detail_foldername);
	str_cat(temp_str, my_filename);
	
	if ( file_exists(temp_str) )		// slow double check - not essential
		{
			//----------------------------------------------------------------------------
			// modify z if applied for another map
			
			if (imported)
				{			
					my_z = PosInfo3D4OrZeroLevel(my_x, my_y) + GetMapTileSize()/2*(my_scale*detail_defaultsize/GetMapTileSize());  // () = original scale
				}				
			
			//----------------------------------------------------------------------------
			// create object
			
			Detail_Create( temp_str, 
								my_scale, 
								my_x, my_y, my_z, 
								my_alpha, my_ambient, 
								my_light, my_red, my_green, my_blue, 
								my_unlit, my_overlay, my_material);
//			wait_for(Detail_Create);
			
			//----------------------------------------------------------------------------
		}
	ptr_remove(temp_str);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////


function Detail_RemoveObject(ENTITY* ent)
{
	if (ent == NULL)
		{
			return 0;
		}
	
	if (ent.ENT_CATEGORY != (var)UIMODE_DETAIL)
		{
			return 0;
		}
	
	// remove the main entity
	ptr_remove(ent);		
	
	// decrease quantity
	detail_counter--;
	
	return 1;
}


//-------------------------------------


int		Detail_RemoveArea(int midtilex, int midtiley, int areasizex, int areasizey)
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
					if (you.ENT_CATEGORY == (var)UIMODE_DETAIL)
						{
							// store next entity
							temp_ent = ent_next(you); 
							
							// remove the main entity
							ptr_remove(you);  										
							
							// decrease quantity
							detail_counter--;
							
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


void		Detail_RemoveAll()
{
	Remove_EntCats( UIMODE_DETAIL );
	wait_for(Remove_EntCats);
	
	detail_counter = 0;
}


////////////////////////////////////////////////////////////////////////////////////////////////


#endif