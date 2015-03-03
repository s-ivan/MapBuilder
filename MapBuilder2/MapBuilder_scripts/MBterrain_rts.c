
#ifndef MBterrain_rts_c
#define MBterrain_rts_c


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// terrain system


void		Terrain_StaticInit()
{	
sys_marker("tri");
	
	TerrTile_Init();
	wait_for(TerrTile_Init);
	
	TerrCluster_Init();
	wait_for(TerrCluster_Init);

sys_marker(NULL);
}


void		Terrain_StaticFree()
{
	TerrTile_Close();
	
	TerrCluster_Close();
}


/////////////////////////////////////////////////////////////////////
// tile and cluster - set to real values based on terrain hmp and mdl-s (safe with deleting previous data)


void		Terrain_StaticCreate()
{	
	//----------------------------------------------------
	
	if (!terrain_entity) return;
	
	if ((TileArrayLength > 0) || (ClusterArrayLength > 0)) return;		// no static processing if exists,
																							// results in problems after level load and new map processing,
																							// with debug message strings removed
	
	//----------------------------------------------------
	
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);	
	wait(3);	
	
	//----------------------------------------------------	
{}
#ifdef DEBUG_PLAY02
	str_cpy(terrain_progress_message, "Static Data");
	wait(3);		
#endif	
	Terrain_StaticInit();											// allocate data holders and set to defaults
	wait_for(Terrain_StaticInit);
#ifdef DEBUG_PLAY02
	str_cpy(terrain_progress_message, "Tile Data");
	wait(3);	
#endif
	TerrTile_SetTilesHeightTypeCost();							// c_trace ground surface, set tiles
	wait_for(TerrTile_SetTilesHeightTypeCost);
#ifdef DEBUG_PLAY02
	str_cpy(terrain_progress_message, "Cluster Data");
	wait(3);	
#endif
	TerrCluster_SetClusters();										// set and subdivide clusters
	wait_for(TerrCluster_SetClusters);	
#ifdef DEBUG_PLAY02
	str_cpy(terrain_progress_message, "Clearance Data");
	wait(3);	
#endif
	TerrTile_SetTilesClearance();									// create clearamce maps for each capability
	wait_for(TerrTile_SetTilesClearance);	
	
	//----------------------------------------------------
		
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);
	
	//----------------------------------------------------
}


/////////////////////////////////////////////////////////////////////
// building tile and cluster update


void		Terrain_SetBuildingTilesClusters(ENTITY* ent, STRING* bld_foldername)
{
//	//-----------------------------------------------------------------
//	// if building cluster(s) has already been set up - should set up terrain check too due to it
//	// clusters, and entity skill should be reset first to run this procedure again
//	if (ent.HASCLUSTER>0)
//		{
//			return;
//		}
		
	//-----------------------------------------------------------------

sys_marker("sm0");
	
	// get name from model or mapent name
	STRING* temp_str = str_create( "" );
	str_cpy(temp_str , bld_foldername);
	str_cat(temp_str , str_for_entfile(NULL , ent));
	str_trunc(temp_str,4);
	str_cat(temp_str,".bld");		
	
	// data from file
	int	building_floors			= 1;			// 1..2 by default, but should be flexibly extendable for higher floor numbers !!! ***
	int	building_custers_1		= 0;			// 0..3 - // should be flexible in floor number !!! ***
	int	building_custers_2		= 0;			// 0..3 - // should be flexible in floor number !!! ***
	
	// int* building_clusters_per_floor = NULL;
	char* building_cluster_type	= NULL;		// 1..6 array  , value: TERR_XXXX
	char* building_tile_cluster	= NULL;		// 1..256 array, value: 1..max cluster
	char* building_tile_type		= NULL;		// 1..256 array, value: 0 no access, 1 passable, 2 no change -> TERR_XXXX
	var* 	building_tile_height		= NULL;		// 1..256 array, value could be divided by 10 to get decimals - if type is no change, no height change!!!
	char* building_tile_clearance	= NULL;		// 1..256 array, value: 0,1,3,5 (but in practice 5=100)
	
	// new clusters' ID number
	short* building_cluster_id		= NULL;		// 1..6 array  , value: 0..ClusterArrayLength or 60000 if uninitialized		
	
	// intermediate data
	int	building_tiles				= (int)(ent.AREA_SIZEX) * (int)(ent.AREA_SIZEY);			// size of one floor, got from obw file
	int	building_clusters			= 0;																		// =0, must be calculated or set later (= building_custers_1 + building_custers_2)
	
	int	building_alltiles			= 0;																		// =0, must be calculated or set later (= floors * tiles)
	
	int 	i,j,k;
		
	//-----------------------------------------------------------------------

sys_marker("sm1");

	// read in building file data to temp structure	- tiles are read from bottom left corner, to right and to up, from level 0 to up
	var file_handle = file_open_read( temp_str );
	
	// read file if exists
	if (file_handle)
		{
			//---------------------------------------------------
			// level0 clusters
			file_str_read( file_handle , NULL );															// comment line
			
			file_str_read( file_handle , temp_str );
			building_custers_1 = str_to_int( temp_str );													// clusters on 1st floor
			
			//---------------------------------------------------
			// level1 clusters
			
			file_str_read( file_handle , NULL );															// comment line
			
			file_str_read( file_handle , temp_str );
			building_custers_2 = str_to_int( temp_str );													// clusters on 2nd floor
			
			//---------------------------------------------------
			// conversions
			
			building_clusters = building_custers_1 + building_custers_2;							// total number of building clusters
			
			building_floors = 0;
			if (building_custers_1>0) 
				{
					building_floors++;		// one times tiles per level/floor
				}
			if (building_custers_2>0) 
				{
					building_floors++;		// one times tiles per level/floor
				}
			building_alltiles = building_floors * building_tiles;
			
			// check
//			printf("building_clusters: %i" , building_clusters);
//			printf("building_alltiles: %i" , building_alltiles);
			
			//---------------------------------------------------
			// cluster types for each cluster

sys_marker("b01");			

			file_str_read( file_handle , NULL );															// comment line
			
			building_cluster_type = array_new( sizeof(char), NULL, building_clusters );		// type of the new clusters	
			
			for (i=0 ; i<building_clusters ; i++)
				{
					file_str_read( file_handle , temp_str );
					building_cluster_type[i] = (char)str_to_int( temp_str );		
					
//					// check
//					int temp_i = (int)building_cluster_type[i];
//					printf("building_cluster_type: %i" , temp_i);
				}
			
			//---------------------------------------------------
			// tile cluster numbers for each tile

sys_marker("b02");			

			file_str_read( file_handle , NULL );															// comment line
			
			building_tile_cluster = array_new( sizeof(char), NULL, building_alltiles );		// correspondance to building clusters
			
			for (i=0 ; i<(building_alltiles) ; i++)
				{
					file_str_read( file_handle , temp_str );
					building_tile_cluster[i] = (char)str_to_int( temp_str );
					
//					// check
//					int temp_i = (int)building_tile_cluster[i];
//					printf("building_tile_cluster: %i" , temp_i);	
				}
			
			//---------------------------------------------------
			// tile types for each tile

sys_marker("b03");			

			file_str_read( file_handle , NULL );															// comment line
			
			building_tile_type = array_new( sizeof(char), NULL, building_alltiles );			// tile types in each clusters
			
			for (i=0 ; i<building_alltiles ; i++)
				{
					file_str_read( file_handle , temp_str );
					building_tile_type[i] = (char)str_to_int( temp_str );
					
//					// check
//					int temp_i = (int)building_tile_type[i];
//					printf("building_tile_type: %i" , temp_i);
				}
			
			//---------------------------------------------------
			// tile heights for each tile

sys_marker("b04");

			file_str_read( file_handle , NULL );															// comment line
			
			building_tile_height = array_new( sizeof(var), NULL, building_alltiles );			// tile heights in each clusters
			
			for (i=0 ; i<building_alltiles ; i++)
				{
					file_str_read( file_handle , temp_str );
					building_tile_height[i] = str_to_num( temp_str );
					
//					// check
//					int temp_i = (int)building_tile_height[i];
//					printf("building_tile_height: %i" , temp_i);	
				}	
			
			//---------------------------------------------------
			// tile clearance masks for each tile
			
sys_marker("b05");			

			file_str_read( file_handle , NULL );
			
			building_tile_clearance = array_new( sizeof(char), NULL, building_alltiles );			// tile defaultclearance values in each clusters
			
			for (i=0 ; i<building_alltiles ; i++)
				{
					file_str_read( file_handle , temp_str );
					building_tile_clearance[i] = (char)str_to_int( temp_str );
					
//					// check
//					int temp_i = (int)building_tile_clearance[i];
//					printf("building_tile_clearance: %i" , temp_i);	
				}	
			
			//---------------------------------------------------
			
			file_close(file_handle);
			
			//---------------------------------------------------
		}	
	else		
		{
			//---------------------------------------------------
			// obstacle like entity - do not do anything
			if ( (ent.AREA_SIZEX < 5) && (ent.AREA_SIZEY < 5))
				{
					ent.ENT_TYPE = 0;			// obstacle
					return;	
				}						
			
			//---------------------------------------------------			
			// fill with fake data, based on mapent x-y tile size only
			// remains obstacle like map entity 
			// it should get a new cluster = CLUS_BUILDING4 33 , 
			// to be destroyable and result in better unit group avoidance path 
			// with not accessible tiles
			
//			printf("no building data");
			
			// get a fake height value
			VECTOR building_height_vec;
			vec_zero(building_height_vec);
			vec_for_max(building_height_vec,ent);
			var building_height = building_height_vec.z / (var)GetMapTileSize();
			
			building_floors    = 1;
			
			building_custers_1 = 1;
			building_custers_2 = 0;
			
			// conversions
			building_clusters = building_custers_1 + building_custers_2;						// total number of building clusters
			
			building_alltiles	= building_floors * building_tiles;									// total number of stored tile data

			//---------------------------------------------------
sys_marker("b06");
			
			// cluster types for each cluster
			building_cluster_type = array_new_default( sizeof(char), NULL, building_clusters, (char)CLUS_BUILDING4 );		// (0 or) 33 - no access building	
			
			// tile cluster numbers for each tile						
			building_tile_cluster = array_new_default( sizeof(char), NULL, building_alltiles, (char)1 );							// 1 - cluster 1	
			
			// tile types for each tile
			building_tile_type = array_new_default( sizeof(char), NULL, building_alltiles, (char)0 );								// 0 - no access
			
			// tile heights for each tile	
			building_tile_height = array_new_default( sizeof(var), NULL, building_alltiles, (var)0 );								// 0 or building_height(wrong!) : got from max vertex divided by tilesize, can affect arrow movement later...
			
			// tile clearance masks for level 1-2
			building_tile_clearance = array_new_default( sizeof(char), NULL, building_alltiles, (char)0 );						// 0 - no access
			
			//---------------------------------------------------		
		}	
	
	//-----------------------------------------------------------------------
	// editor related settings that differs in game
	
	// building	
	ent.ENT_TYPE = 2;				
	
	// very important !!!
	// to have unit collision with buildings, and c_trace detection on buildings, by avoiding IGNORE_FLAG2 exclusion
	// needs to be set again for the editor, and for terrain map abstraction, after demo ends!			
	reset(ent, FLAG2);
		
	//-----------------------------------------------------------------------
		
sys_marker("sm2");
	
	building_cluster_id = (short*)sys_malloc(building_clusters * sizeof(short));
	memset( building_cluster_id , (short)65535 , building_clusters * sizeof(short) );		// 65535 or 255, set to first byte value !
	
	//------------------------------------------------------------------------
	// add new clusters to map , or reserve invalid ones - and fill data from database read from file (or defaults created)
	
sys_marker("sm3");
	
	for (i=0 ; i<building_clusters ; i++)
		{
			// get a new cluster with default terrain cluster values
			int temp_int = (int)TileToCluster3D((int)ent.ABSTILE_POS,0);									// ### get ground cluster of entity middle
			
			int current_cluster_id = (int)TerrCluster_InitNewCluster( temp_int );						// get a new cluster id based on middle tile cluster id as parent	
			
			building_cluster_id[i] = (short)current_cluster_id;												// attach real cluster id to cluster number stored in file
			
//			// check
//			int temp_i = (int)building_cluster_id[i];
//			printf("building_cluster_id: %i" , temp_i);	
			
			// modify its building specific values
			SetClusterClusType(current_cluster_id, building_cluster_type[i]);
			SetClusterClusCost(current_cluster_id, COST_NORMAL);												// COST_FAST, COST_SLOW can be mod later to COST_BAD if needed
			SetClusterClusTilesX(current_cluster_id, (int)(ent.AREA_SIZEX));								// can be mod later after determining xy size
			SetClusterClusTilesY(current_cluster_id, (int)(ent.AREA_SIZEY));								// can be mod later after determining xy size
			SetClusterClusMidTileX(current_cluster_id, (int)(ent.XTILE_POS));								// can be mod later after determining xy size
			SetClusterClusMidTileY(current_cluster_id, (int)(ent.YTILE_POS));								// can be mod later after determining xy size
			
			// this level setting should be more flexible !
			if (
					(building_custers_1>0)
					&&
					(i<building_custers_1)
				)
				{
					SetClusterClusMidTileZ(current_cluster_id, 1);												// 0 ground, 1 for 1st floor, 2 for 2nd floor
				}
			else
				{
					SetClusterClusMidTileZ(current_cluster_id, 2);												// 0 ground, 1 for 1st floor, 2 for 2nd floor					
				}
			
			SetClusterClusBottleneck(current_cluster_id, 1);													// always bottleneck		
			
			SetClusterClusBuildingID(current_cluster_id, ent.ENT_HANDLE);									// adding a building id (=building entity handle) for fast data access	
			
		}	
	
	//---------------------------------------------------------------------------------	
	// rotate tile arrays and exchange custer xy sizes, if building entity is rotated - WIP !!!
	
	int direction = AngleToDirection(ent.pan);		
	
//	if (direction > 0)	// COMMENT OUT FOR TESTING 0 ROTATION
		{
			//----------------------------------------
			
			// !!! it is modified after each rotations !!!
			int areax;
			int areay;
			
			// does not change after rotations
			int arraylength = building_alltiles;
			
			int elementsize;
			
			//----------------------------------------
			
			areax = (int)ent.AREA_SIZEX;
			areay = (int)ent.AREA_SIZEY;	
			
			elementsize = sizeof(char);
				
//			building_tile_cluster 		= array_rotate_multi( building_tile_cluster, sizeof(char), &arraylength, direction, &areax, &areay, &defaultelement_char );	// XXX - void should be char
			building_tile_cluster 		= array_rotate_multi_char( building_tile_cluster, &arraylength, direction, &areax, &areay );												// okay
			
//			building_tile_cluster 		= array_rotate( building_tile_cluster, elementsize, NULL, direction, &areax, &areay );														// XXX - void should be char
//			building_tile_cluster 		= array_rotate_char( building_tile_cluster, NULL, direction, &areax, &areay );																// okay
			
			//---
			
			areax = (int)ent.AREA_SIZEX;
			areay = (int)ent.AREA_SIZEY;						
			
			elementsize = sizeof(char);
			
//			building_tile_type 			= array_rotate_multi( building_tile_type, sizeof(char), &arraylength, direction, &areax, &areay, &defaultelement_char );
			building_tile_type	 		= array_rotate_multi_char( building_tile_type, &arraylength, direction, &areax, &areay );
			
//			building_tile_type 			= array_rotate( building_tile_type, elementsize, NULL, direction, &areax, &areay );			
//			building_tile_type	 		= array_rotate_char( building_tile_type, NULL, direction, &areax, &areay );
			
			//---
			
			areax = (int)ent.AREA_SIZEX;
			areay = (int)ent.AREA_SIZEY;
			
			elementsize = sizeof(var);
				
//			building_tile_height 		= array_rotate_multi( building_tile_height, sizeof(var), &arraylength, direction, &areax, &areay, &defaultelement_var );
			building_tile_height 		= array_rotate_multi_var( building_tile_height, &arraylength, direction, &areax, &areay );
			
//			building_tile_height 		= array_rotate( building_tile_height, elementsize, NULL, direction, &areax, &areay );			
//			building_tile_height 		= array_rotate_var( building_tile_height, NULL, direction, &areax, &areay );
			
			//---
			
			areax = (int)ent.AREA_SIZEX;
			areay = (int)ent.AREA_SIZEY;
			
			elementsize = sizeof(char);
					
//			building_tile_clearance 	= array_rotate_multi( building_tile_clearance, sizeof(char), &arraylength, direction, &areax, &areay, &defaultelement_char );		
			building_tile_clearance		= array_rotate_multi_char( building_tile_clearance, &arraylength, direction, &areax, &areay );
			
//			building_tile_clearance 	= array_rotate( building_tile_clearance, elementsize, NULL, direction, &areax, &areay );					
//			building_tile_clearance		= array_rotate_char( building_tile_clearance, NULL, direction, &areax, &areay );
			
			//---
			// !!! now keep areax-y !!!
			//---
			
			//----------------------------------------
			// set entity area size due to actual rotation
			
			ent.AREA_SIZEX = (var)areax;
			ent.AREA_SIZEY = (var)areay;
			
			//----------------------------------------
		}
	
	//---------------------------------------------------------------------------------

sys_marker("sm4");		
	
//	// clearance generating functions use lower-left tile of entity area as reference
//	// thus here it is not needed	to deal with it at all - only on object placement and on clearance area definition
	
	//--------------------------------------------------------------------
	// area size can be an even number !!! only in case of buildings !!!
	// MODIFIED VERSION - TILE POSITION IS TO LEFT AND TO BOTTOM FROM OBJECT MIDDLE - FOR ANY ENTITY
	
	int halfareax = (int)integer(ent.AREA_SIZEX / 2);
	int halfareay = (int)integer(ent.AREA_SIZEY / 2);	
		
	//--------------------------------------------------------------------		
	// set tiles to cluster id , and to type with conversion (and cost), and midz = ent.z+(var)(int)(height*GetMapTileSize())
	
	int tempx_int = (int)ent.XTILE_POS - halfareax;
	int tempy_int = (int)ent.YTILE_POS - halfareay;

sys_marker(NULL);
	
	//--------------------------------------------------------------------	
	// cycle through each tile value read from file, their quantity is = area * newclusters
	
	int temp_counter_int = 0;	
	for (k=0 ; k<building_floors ; k++)																									// not all clusters, just all floors !!!
		{
			for (j=tempy_int ; j<(tempy_int+(int)ent.AREA_SIZEY) ; j++)
				{
					for (i=tempx_int ; i<(tempx_int+(int)ent.AREA_SIZEX) ; i++)
						{
							if ( XYTileValid(i, j) )																						// border protection
								{
sys_marker("sma");									
									int actual_tile 			= XYTileToAbsTile(i,j);													// abs tile pos
									int actual_file_pos		= temp_counter_int;														// the order as stored in local array after read from file
sys_marker("smb");									
									int actual_cluster_num 	= (int)building_tile_cluster[actual_file_pos];					// cluster number from file - not a real cluster ID
sys_marker("smc");
									int actual_cluster_id 	= (int)building_cluster_id[actual_cluster_num-1];				// real cluster ID got at creating new clusters in cluster array
sys_marker("smd");	// ***
									int actual_level 			= GetClusterClusMidTileZ(actual_cluster_id);						// level of cluster, was 0, now 1..2
sys_marker("sme");									
									// fake cluster middle tile setting to a member tile - to set middle surely fine - maybe not needed
									SetClusterClusMidTileX(actual_cluster_id, i);
									SetClusterClusMidTileY(actual_cluster_id, j);									
																		
//									if (building_tile_type[actual_file_pos] < (char)2)												// if not no-change - better to always assign it to a new cluster! ***
										{
											SetTileClusterID(actual_tile, actual_level-1, actual_cluster_id);					// only [0..1]
										}
sys_marker("smf");									
									if (building_tile_type[actual_file_pos] == (char)0)											// no acc
										{
											SetTileType(actual_tile, actual_level, TERR_BUILDING4);								// TERR_BUILDING4 is approx TERR_NOACC but detected for pathfinder as a special possibility for siege equipment usage
											SetTileCost(actual_tile, actual_level, COST_NOACC);									// indeed cost is used by pathfinder
										}
									else if (building_tile_type[actual_file_pos] == (char)1)																	// passable
										{
											// convert cluster type to tile type by -10
											SetTileType(actual_tile, actual_level, (int)building_cluster_type[actual_cluster_num-1] - 10);		// convert from cluster type
											SetTileCost(actual_tile, actual_level, COST_SLOW);																	// COST_FAST helps building moves, COST_SLOW - avoids	, COST_NORMAL balanced										
										}
									else //if (building_tile_type[actual_file_pos] == (char)2)									// ==2, no change (useful for 1st floor only), copy ground value
										{
											SetTileType(actual_tile, actual_level, GetTileType(actual_tile, 0));
											SetTileCost(actual_tile, actual_level, GetTileCost(actual_tile, 0));	
										}									
									SetTileBuildingID(actual_tile, ent.ENT_HANDLE);													// for all the 3 cases
sys_marker("smg");									
//									// if no change copy values from ground - useful for 1st floor only !!! - CAUSES ERRORS WHEN BUILDING IS RAISED UP FROM ITS DEFAULT PLACEMENT HEIGHT, AND UNDERGROUND PARTS APPEAR
//									if (building_tile_type[actual_file_pos] == (char)2)
//										{
//											SetTileMidZ(actual_tile, actual_level, GetTileMidZ(actual_tile, 0));
//										}
//									// should be equal or higher than terrain!
//									else
										{
											SetTileMidZ(actual_tile, actual_level, maxv( GetTileMidZ(actual_tile, 0) , (ent.z + building_tile_height[actual_file_pos] * (var)GetMapTileSize()) ));
										}		
sys_marker("smf");
									SetTileUnitSizeMask(actual_tile, actual_level-1, (int)building_tile_clearance[actual_file_pos]);								
sys_marker(NULL);									
								}	// end if				
							
							temp_counter_int++;		// actual array element read from file
							
						}	// end for i		
				}	// end for j
		}	// end for k

//sys_marker(NULL);	// ok
sys_marker("sm5");
	
	//-------------------------------------------------------------------------------
	// set all new xy cluster sizes and their middle tile - cluster type cost level and bottleneck is already set - size and middle was only temp from area size and position
	
	for (k=0 ; k<building_clusters ; k++)
		{
			short temp_s = building_cluster_id[k];
			TerrCluster_SetClusterXYSizeMiddle( temp_s );
			wait_for(TerrCluster_SetClusterXYSizeMiddle);
		} // ok
		
sys_marker(NULL);	
sys_marker("sm6");
	
	//-------------------------------------------------------------------------------
	// set neighbours and walkable neighbours
	
	for (k=0 ; k<building_clusters ; k++)
		{
			short temp_s = building_cluster_id[k];
			TerrCluster_SetClusterNeigWalkNeig( temp_s );
			wait_for(TerrCluster_SetClusterNeigWalkNeig);
		}	// ok

sys_marker(NULL);	
sys_marker("sm7");
	
	//-------------------------------------------------------------------------------
	// update neighbour clusters - they probably have new neighbours and walkable neighbours; and may be needed to be subdivided !
	
	for (k=0 ; k<building_clusters ; k++)
		{
			short temp_s = building_cluster_id[k];
			
			for (i=0 ; i<MAX_NEIGHBOURS ; i++)
				{
					short neig_clus_s = (short)GetClusterClusNeighbours(temp_s, i);
					
					if (neig_clus_s < (short)CLUS_NULL)
						{
							var subdivision_by_noacc = TerrCluster_DivideClusterByNoAccess(neig_clus_s);		// checks for not impassable tiles as start -
																																		// valid only for ground clusters! but it is okay and protected														
							
							if (!subdivision_by_noacc)
								{
									TerrCluster_SetClusterXYSizeMiddle(neig_clus_s);
									wait_for(TerrCluster_SetClusterXYSizeMiddle);
									
									TerrCluster_SetClusterNeigWalkNeig(neig_clus_s);
									wait_for(TerrCluster_SetClusterNeigWalkNeig);
								}	// ok
							else
								{
									// update neighbour clusters of neighbours as they should be updated to get new neighbours
									for (j=0 ; j<MAX_NEIGHBOURS ; j++)
										{
											short neigneig_clus_s = (short)GetClusterClusNeighbours(neig_clus_s, j);
											
											if (neigneig_clus_s < (short)CLUS_NULL)
												{
													TerrCluster_SetClusterNeigWalkNeig(neigneig_clus_s);
													wait_for(TerrCluster_SetClusterNeigWalkNeig);
												}											
										}							
								}
							
							// *** ### subdivision by concave to convex, based on building area corner tile positions, thus 4 cases
							// *** ### to avoid L U and O shaped clusters resulting in strange group movements around buildings
							var subdivision_by_shape = 0;
							
							//...
							
							//...
							
							//...
							
						}	// if <60000						
				}	// for i
		}	// for k

sys_marker(NULL);	
sys_marker("sm8");
	
	//-----------------------------------------------------------------
	
	ptr_remove(temp_str);
	
	sys_free(building_cluster_type);		
	sys_free(building_tile_cluster);
	sys_free(building_tile_type);
	sys_free(building_tile_height);
	sys_free(building_tile_clearance);
	sys_free(building_cluster_id);
	
//	// = NULL?

sys_marker(NULL);	
sys_marker("sm9");
	
	//--------------------------------------------------------------------
	
	ent.HASCLUSTER = 1;			// no need to do this cluster addition again 
										// except some rare cases when buildings are intersecting each other
										// or on partial destruction
	
	//--------------------------------------------------------------------
#ifdef BUILDING_DEBUG
	wait(1);
	ent.emask |= (DYNAMIC);		// needed for events
	wait(1);
	
	ent.emask |= ENABLE_SCAN | ENABLE_DETECT | ENABLE_TOUCH | ENABLE_RELEASE | ENABLE_CLICK ; 		// *** release
	ent.event = Terrain_BuildingEvent;																					// *** release
#endif
sys_marker(NULL);	
}


///////////////////////////////////////////////////////////////


function		Terrain_BuildingEvent()
{
	switch (event_type)
	  {
			case EVENT_TOUCH:
				set(my,TRANSLUCENT);
				my.alpha = 50;
				break;			
			case EVENT_RELEASE:
				reset(my,TRANSLUCENT);
				my.alpha = 100;
				break;
		}
}


///////////////////////////////////////////////////////////////


void		Terrain_ResetBuildingTilesClusters(ENTITY* ent)
{
	int i,j,k,m;
	
	var temp_buildingid =  ent.ENT_HANDLE;
	
	// then cycle through clusters and write into an array those cluster id-s which corresponds to this entity, let it be max 10
	k = BUILDING_MAXCLUSTERS;
//	short temp_cluster_id[BUILDING_MAXCLUSTERS];		// 6
	short* temp_cluster_id = (short*)sys_malloc(k * sizeof(short));
	memset( temp_cluster_id , (short)65535 , k * sizeof(short) );
	
	int temp_count = 0;
	for (i=0;i<ClusterArrayLength;i++)
		{
			if ((GetClusterClusBuildingID(i) == ent.ENT_HANDLE) && (temp_count<k))
				{
//					printf("Cluster found! %i",temp_count);
										
					temp_cluster_id[temp_count] = (short)i;
					temp_count++;
				}
		}
	
	// protection
	if (temp_count == 0)
		{
//			printf("No cluster found!");
			return;
		}
	if (temp_buildingid == (var)0)
		{
//			printf("No building ID!");
			return;
		}		
	
	//------------------------------------------------------------------------------------
	// area to be modified
	
	// MODIFIED VERSION - TILE POSITION IS TO LEFT AND TO BOTTOM FROM OBJECT MIDDLE - FOR ANY ENTITY
	int halfareax = (int)integer(ent.AREA_SIZEX / 2);
	int halfareay = (int)integer(ent.AREA_SIZEY / 2);	
	
	int tempx_int = (int)ent.XTILE_POS - halfareax;
	int tempy_int = (int)ent.YTILE_POS - halfareay;	
	
	short temp_1st_ground_clusid = 65535;
	int	temp_1st_groundclus_counter = 0;
	
	// set level 1-2 tiles/costs/midz to no access/invalid
	for (j=tempy_int ; j<(tempy_int+(int)(ent.AREA_SIZEY)) ; j++)
		{
			for (i=tempx_int ; i<(tempx_int+(int)(ent.AREA_SIZEX)) ; i++)
				{
					// map border check
					if ( XYTileValid(i, j) )										// border protection
						{
							m = XYTileToAbsTile( i , j );
							
							// is the tile assigned to the given building?
							if (GetTileBuildingID(m) == temp_buildingid)
								{
									// check
									if (temp_1st_groundclus_counter==0)
										{
//											printf("Building ID found %f", (double)temp_buildingid);
										}									
									
									// common
									SetTileBuildingID(m, (var)0);
									
									for (k=0;k<temp_count;k++)
										{
											if (temp_cluster_id[k] < (short)CLUS_NULL)			// protection of invalid clusters
												{											
													// level 1
													if (temp_cluster_id[k] == GetTileClusterID(m, 0))
														{
//															printf("Level-1 cluster ID found %i", (int)temp_cluster_id[k]);
															
//															SetTileClusterID(m, 0, GetTileClusterID(m, 0));						// remains the same !											
															SetTileType(m, 1, TERR_NOACCESS);
															SetTileCost(m, 1, COST_NOACC);
															SetTileMidZ(m, 1, (var)NO_Z_APPLIED);
															SetTileUnitSizeMask(m, 0, 0);
															
															// unify ground cluster tiles into first ground cluster found
															if (temp_1st_groundclus_counter == (int)0)
																{
//																	printf("Building ID found (1)!");
																	
																	temp_1st_ground_clusid = GetTileClusterID(m, 0);
																	temp_1st_groundclus_counter++;
																}
															else
																{
																	SetTileClusterID(m, 0, temp_1st_ground_clusid);
																}															
														}
														
													// level 2
													if (temp_cluster_id[k] == GetTileClusterID(m, 1))
														{
//															printf("Level-2 cluster ID found %i", (int)temp_cluster_id[k]);
															
															SetTileClusterID(m, 1, CLUS_NULL);										// was 65535
															SetTileType(m, 2, TERR_NOACCESS);
															SetTileCost(m, 2, COST_NOACC);
															SetTileMidZ(m, 2, (var)NO_Z_APPLIED);
															SetTileUnitSizeMask(m, 1, 0);
														}
												}
										}
								}
						}
				}
		}
	
	// set unnecessary clusters invalid, update clusters kept
	if (temp_1st_groundclus_counter>0)
		{
//			printf("One main cluster!");
			
			// keep only first ground cluster, set other clusters invalid
			for (k=0;k<temp_count;k++)
				{
					if (temp_cluster_id[k] < (short)CLUS_NULL)
						{
							if (temp_cluster_id[k] != temp_1st_ground_clusid)
								{					
									TerrCluster_SetClusterInvalid( temp_cluster_id[k] );
									wait_for(TerrCluster_SetClusterInvalid);
								}
						}
				}
			
			// reset to ground cluster ! very important !
			SetClusterClusMidTileZ(temp_1st_ground_clusid, 0);
			
			// set the only remaining level 0 cluster properties by update checks
			TerrCluster_SetClusterTypeCostHeight(temp_1st_ground_clusid);	
			wait_for(TerrCluster_SetClusterTypeCostHeight);
						
			TerrCluster_SetClusterXYSizeMiddle(temp_1st_ground_clusid);
			wait_for(TerrCluster_SetClusterXYSizeMiddle);
								
			TerrCluster_SetClusterNeigWalkNeig(temp_1st_ground_clusid);
			wait_for(TerrCluster_SetClusterNeigWalkNeig);
		}
	else			
		{
//			printf("No main cluster!");
			
			// only if something wrong !
			for (k=0;k<temp_count;k++)
				{
					short temp_id = temp_cluster_id[k];
					if (temp_id < (short)CLUS_NULL)
						{
							// set level 2 clusters invalid
							if (GetClusterClusMidTileZ(temp_id) == 2)
								{					
									TerrCluster_SetClusterInvalid(temp_id);
									wait_for(TerrCluster_SetClusterInvalid);
								}
							else
								{
									// set level 0 clusters properties by update checks
									TerrCluster_SetClusterTypeCostHeight(temp_id);		
									wait_for(TerrCluster_SetClusterTypeCostHeight);
											
									TerrCluster_SetClusterXYSizeMiddle(temp_id);	
									wait_for(TerrCluster_SetClusterXYSizeMiddle);	
												
									TerrCluster_SetClusterNeigWalkNeig(temp_id);
									wait_for(TerrCluster_SetClusterNeigWalkNeig);
								}
						}
				}
		}
	
//	// update neighbour clusters of neighbours as they should be updated to get new neighbours
//	for (j=0 ; j<10 ; j++)
//		{
//			short neigneig_clus_s = GetClusterClusNeighbours(neig_clus_s, j);
//			
//			if (neigneig_clus_s < 60000)
//				{
//					TerrCluster_SetClusterNeigWalkNeig(neigneig_clus_s);
//					wait_for(TerrCluster_SetClusterNeigWalkNeig);
//				}											
//		}	
	
	sys_free(temp_cluster_id);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Terrain_DynamicInit()
{
	//---------------------------------
	
	int i,j;
		
	terrain_map_levels		= GetMapZLevels();											// map levels i.e. building floors present in level
																										// currently it must be 2 i.e. BUILDING_LEVELS
																										// later can be limited if no buildings are in the actual level,
																										// to make faster pathfinding and clearance map updates
	
	//---------------------------------
	// allocate handle arrays for entity categories
	
//	terrain_shrub_handlearray	= (var*)sys_malloc( TileArrayLength * sizeof(var) );	
//	memset( terrain_shrub_handlearray , (var)0 , (TileArrayLength * sizeof(var)) ); 
	
	terrain_obstacle_handlearray	= (var*)sys_malloc( TileArrayLength * sizeof(var) );
	memset( terrain_obstacle_handlearray , (var)0 , (TileArrayLength * sizeof(var)) ); 
	
	terrain_building_handlearray	= (var*)sys_malloc( TileArrayLength * sizeof(var) );		
	memset( terrain_building_handlearray , (var)0 , (TileArrayLength * sizeof(var)) ); 
	
	//---------------------------------
	// allocate clearance arrays for entity categories
	
//	terrain_shrub_clearancearray	= (char*)sys_malloc( TileArrayLength * sizeof(char) );
//	memset( terrain_shrub_clearancearray , (char)CL_DEFAULT , (TileArrayLength * sizeof(char)) ); 
	
	terrain_obstacle_clearancearray	= (char*)sys_malloc( TileArrayLength * sizeof(char) );
	memset( terrain_obstacle_clearancearray , (char)CL_DEFAULT , (TileArrayLength * sizeof(char)) ); 	
	
	terrain_building_clearancearray	= (char**)sys_malloc( terrain_map_levels * sizeof(char*) );		
	for (j=0; j<terrain_map_levels; j++)
		{
			terrain_building_clearancearray[j] = (char*)sys_malloc( TileArrayLength * sizeof(char) );
			
			if (j==0)
				{
					memset( terrain_building_clearancearray[j] , (char)CL_DEFAULT , TileArrayLength * sizeof(char) ); 	// later summed to ground
				}
			else
				{
					memset( terrain_building_clearancearray[j] , (char)0 , TileArrayLength * sizeof(char) ); 				// summed only with units
				}
		}
	
	terrain_unit_clearancearray	= (char**)sys_malloc( terrain_map_levels * sizeof(char*) );
	for( i=0 ; i<terrain_map_levels ; i++ )
		{
			terrain_unit_clearancearray[i]	= (char*)sys_malloc( TileArrayLength * sizeof(char) );
			
			memset( terrain_unit_clearancearray[i] , (char)CL_DEFAULT , TileArrayLength * sizeof(char) ); 			
		}
		
	//---------------------------------
}


////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Terrain_DynamicFree()
{
	
sys_marker("DF0");

	//----------------------------------------	
	
	int i,j;
	
	proc_kill2(Terrain_AllClearanceMapUpdate, NULL);
	wait(1);
	
	//----------------------------------------	
	//----------------------------------------
	// shadow casting entity list
	
	if (terrain_shadowentity_array != NULL)
		{
			for (i=0;i<terrain_shadowentity_counter;i++)
				{
					set(terrain_shadowentity_array[i],SHADOW);
				}
			wait(1);
			
			for (i=0;i<terrain_shadowentity_counter;i++)
				{
					terrain_shadowentity_array[i].emask &= ~DYNAMIC;			
				}
			wait(1);
			
			for (i=0;i<terrain_shadowentity_counter;i++)
				{
					terrain_shadowentity_array[i] = NULL;
		//			ptr_remove(terrain_shadowentity_array[i]);						// no !!! it would delete them
				}
			
			for (i=0; i<terrain_shadowentity_counter; i++)
				{
					sys_free( terrain_shadowentity_array[i] );
				}
			sys_free(terrain_shadowentity_array);
			terrain_shadowentity_array = NULL;
			wait(1);
		}
	
	//----------------------------------------	
	//----------------------------------------
	// TILE MAPS
	
	//----------------------------------------	
	// shrub handle map
sys_marker("DFa");	
//	if (terrain_shrub_handlearray != NULL)
//		{
//			sys_free(terrain_shrub_handlearray);		
//			terrain_shrub_handlearray 		= NULL;
//			wait(1);
//		}
	
	//----------------------------------------	
	// obstacle handle map
sys_marker("DFb");	
	if (terrain_obstacle_handlearray != NULL)
		{
			sys_free(terrain_obstacle_handlearray);	
			terrain_obstacle_handlearray 	= NULL;
			wait(1);
		}
	
	//----------------------------------------	
	// building FLAG2 and ENT_TYPE reset

sys_marker("DF2");	

	// set entities to dynamic to be able to modify flag and skill
	you = ent_next(NULL); 		
	while (you) 						
		{
			if (( you.ENT_CATEGORY == (var)UIMODE_OBSTACLE ))
				{
					you.emask |= DYNAMIC;															
				}
			you = ent_next(you); 	
		}
	wait(1);
	
	// set again FLAG2 and reset ENT_TYPE to 0
	you = ent_next(NULL); 		
	while (you) 					
		{
			if ( AbsTileValid((int)you.ABSTILE_POS) )	
				{
					// restore for all entities
					you.AREA_SIZEX = you.ORIG_AREA_SIZEX;
					you.AREA_SIZEY = you.ORIG_AREA_SIZEY;
					
					if (( you.ENT_CATEGORY == (var)UIMODE_OBSTACLE ))
						{					
							if (you.ENT_TYPE == (var)2)
								{
									you.ENT_TYPE = 0;
								}
							else
								{
									set(you, FLAG2);										// keep obstacle building FLAG2 to have proper cluster and tile representation in MBposinfopanel.c
								}														
						}
				}
			you = ent_next(you); 
		}	
	wait(1);
	
	// set entities back to static for faster rendering
	you = ent_next(NULL); 		
	while (you) 					
		{
			if (( you.ENT_CATEGORY == (var)UIMODE_OBSTACLE ))
				{					
					you.emask &= ~DYNAMIC;										
				}
			you = ent_next(you); 	
		}	
	wait(1);
	
	//----------------------------------------	
	// building handle map
	
	if (terrain_building_handlearray != NULL)
		{
			sys_free(terrain_building_handlearray);					
			terrain_building_handlearray	= NULL;
			wait(1);
		}
	
	//----------------------------------------	
	//----------------------------------------	
	// shrub clearance map

sys_marker("DF3");	

//	if (terrain_shrub_clearancearray != NULL)
//		{
//			sys_free(terrain_shrub_clearancearray);	
//			terrain_shrub_clearancearray 		= NULL;
//			wait(1);
//		}
	
	//----------------------------------------	
	// obstacle clearance map
	
	if (terrain_obstacle_clearancearray != NULL)
		{
			sys_free(terrain_obstacle_clearancearray);	
			terrain_obstacle_clearancearray 	= NULL;	
			wait(1);
		}
	
	//----------------------------------------
	//	building clearance map
	
	if (terrain_building_clearancearray != NULL)
		{
			for (i=0; i<terrain_map_levels; i++)
				{
					sys_free( terrain_building_clearancearray[i] );
				}
			sys_free(terrain_building_clearancearray);
			terrain_building_clearancearray	= NULL;
			wait(1);
		}
	
	//----------------------------------------
	// summed clearance map	

sys_marker("DF4");
	
	if (terrain_summed_clearancearray != NULL)
		{
			for (i=0; i<4; i++)
				{			
					for (j=0; j<terrain_map_levels; j++)
					{
						sys_free( (terrain_summed_clearancearray[i])[j] );
					}
					sys_free( terrain_summed_clearancearray[i] );
				}
			sys_free(terrain_summed_clearancearray);
			terrain_summed_clearancearray	= NULL;
			wait(1);
		}
	
	//----------------------------------------
	// static clearance map	
	
	if (terrain_static_clearancearray != NULL)
		{
			for (i=0;i<4;i++)
				{			
					for (j=0; j<terrain_map_levels; j++)
						{
							sys_free( (terrain_static_clearancearray[i])[j] );
						}
					sys_free( terrain_static_clearancearray[i] );
				}
			sys_free(terrain_static_clearancearray);
			terrain_static_clearancearray	= NULL;
			wait(1);
		}
	
	//----------------------------------------
	//	unit clearance map
	
	if (terrain_unit_clearancearray != NULL)
		{
			for (i=0; i<terrain_map_levels; i++)
				{
					sys_free( terrain_unit_clearancearray[i] );
				}
			sys_free(terrain_unit_clearancearray);
			terrain_unit_clearancearray = NULL;
			wait(1);
		}
	
	//----------------------------------------	
	//----------------------------------------
	// CLUSTER MAPS

sys_marker("DF5");	
	
	//----------------------------------------
	// group map
	
	if (terrain_group_clusterarray != NULL)
		{
			sys_free(terrain_group_clusterarray);
			terrain_group_clusterarray = NULL;	
			wait(1);
		}
	
	//----------------------------------------
	
sys_marker(NULL);

}


////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Terrain_DynamicCreate()
{
	//----------------------------------------------------		
	
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);	
	wait(3);			
	
	//-----------------------------------------
	
	int i, j, k;
	var your_handle;
	int your_abstile_pos;
	
	//-----------------------------------------
	// allocate data holders
{}	
#ifdef DEBUG_PLAY02		
	str_cpy(terrain_progress_message, "Dynamic Data");
	wait(3);	
#endif
	
	Terrain_DynamicInit();
	wait_for(Terrain_DynamicInit);
	
	//-----------------------------------------
	// fill data holders, create corresponding clearance maps - slower than one common loop
	
	Terrain_DynamicBuildings();
	wait_for(Terrain_DynamicBuildings);
	
//	Terrain_DynamicShrubs();
//	wait_for(Terrain_DynamicShrubs);
	
	Terrain_DynamicObstacles();
	wait_for(Terrain_DynamicObstacles);
	
	Terrain_DynamicShadowEntities();
	wait_for(Terrain_DynamicShadowEntities);	
		
	//------------------------------------------------
	// create summed maps from entity maps by merging them
	
	Terrain_DynamicSummedMap();
	wait_for(Terrain_DynamicSummedMap);
	
	Terrain_DynamicStaticMap();
	wait_for(Terrain_DynamicStaticMap);
	
	Terrain_DynamicGroupMap();
	wait_for(Terrain_DynamicGroupMap);
	
	//------------------------------------------------------------------------------

#ifdef DEBUG_PLAY02		
	str_cpy(terrain_progress_message, "");
	str_cpy(terrcluster_progress_message, "");
	wait(3);
#endif
	
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);			
	
	//------------------------------------------------------------------------------
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// entity maps


void		Terrain_DynamicBuildings()
{
	var your_handle;
	int your_abstile_pos;
	
	//------------------------------------------------------------------------------
	// process static map modifications (inner floor tiles) and ground tiles around buildings
	
	you = ent_next(NULL); 																									// get first entity	
	while (you) 																												// repeat until there are no more entities
		{
			if ( AbsTileValid((int)you.ABSTILE_POS) )	
				{
					// store for all entities
					you.ORIG_AREA_SIZEX = you.AREA_SIZEX;
					you.ORIG_AREA_SIZEY = you.AREA_SIZEY;
					
					//------------------------------------------------------------------------------
					// read building data from file, set tiles and create clusters								
					
					// building if: larger than 5x5 tiles (obstacle or building), or has a .bld building info file (in practice for buildings only, but works for obstacles too)
							
					if ( you.ENT_CATEGORY == (var)UIMODE_BUILDING )													
						{	
							Terrain_SetBuildingTilesClusters(you, building_foldername);							// it checks and sets ENT_TYPE again if needed to real value
							wait_for(Terrain_SetBuildingTilesClusters);
						}
					else if ( you.ENT_CATEGORY == (var)UIMODE_OBSTACLE )											// large obstacles (>5x5) might require an own cluster for better pathfinding!!!
						{											
							Terrain_SetBuildingTilesClusters(you, obstacle_foldername);							// it checks and sets ENT_TYPE again if needed to real value
							wait_for(Terrain_SetBuildingTilesClusters);
							
							// moved inside setbuilding, only for ent.ENT_TYPE == 2; (building)
//							reset(you, FLAG2);																				// very important !!!
//																																	// to have unit collision with buildings, and c_trace detection on buildings, by avoiding IGNORE_FLAG2 exclusion
//																																	// needs to be set again for the editor, and for terrain map abstraction, after demo ends!
						}
					//------------------------------------------------------------------------------
					// create mapent and obstacle building entity handle map,
					// and clearance map around them, taking into building tile values on cluster border
					
					if (
							(( you.ENT_CATEGORY == (var)UIMODE_OBSTACLE ) && ( you.ENT_TYPE == (var)2 ))  // for building like obstacles - if 2 then it is building, if 1 then passable, if 0 impassable
							||
							(( you.ENT_CATEGORY == (var)UIMODE_BUILDING ) && ( you.ENT_TYPE == (var)2 )) 	// for building like buildings - if 2 then it is building, if 1 then passable, if 0 impassable
						)
						{
							your_handle 		= handle(you);																// from handle
							your_abstile_pos 	= (int)you.ABSTILE_POS;													// from skill	
																
							// set tile handle map
							terrain_building_handlearray[your_abstile_pos] = your_handle;
							
							// set tile area clearance of tiles around building tiles on all building levels
							Terrain_SetClearance_AroundBuilding( terrain_building_clearancearray , you );
							wait_for(Terrain_SetClearance_AroundBuilding);							
							
//							// no clipping
//							you.eflags &= ~CLIP0;
//							you.eflags &= ~CLIP1;
							
							// set clipping 75% because of sky cube image and modified lod distances
							you.eflags &= ~CLIP1;
							you.eflags |= CLIP0;	
						}
					//------------------------------------------------------------------------------								
				}
			
			you = ent_next(you); 																							// get next entity
		}
	
	//------------------------------------------------------------------------------
	// post process building clearance maps - add inner tiles already stored in static map to dynamic map containing surrounding ground tiles processed earlier
	
	// LIMITATION  buildings cannot overlap with other buildings or obstacles
	// BUG (?) : 	obstacles have no blocking effect on building levels - obstacle clearance map should be summed into it onto all levels
	//	OR :			high obstacles should have a building file, managing tiles and clusters on the levels reached by its height
	//					but not perfect as clearance maps are made in entity creation order and bad at overlappings
	
	int i,j;
	
	// building level 1 : minv with tilearray building clearance array [0]
	for( i=0 ; i<TileArrayLength ; i++ )
		{
			if (GetTileUnitSizeMask(i, 0) < 200)																	// by default it is 200, so it is not necessary to check for real building data
				{
					(terrain_building_clearancearray[0])[i] = (char)(int)minv( (var)GetTileUnitSizeMask(i, 0) , (var)(int)(terrain_building_clearancearray[0])[i] );
				}
		}
	
	// building level 2 or above: overwrite 0-s with tilearray building clearance mask [1 or above]
	for (j=1; j<terrain_map_levels; j++)
		{
			for( i=0 ; i<TileArrayLength ; i++ )
				{
					if (GetTileUnitSizeMask(i, j) < 200)															// by default it is 200, overwrite only if contains real building data
						{
							(terrain_building_clearancearray[j])[i] = (char)GetTileUnitSizeMask(i, j);
						}
				}
		}
	
	//------------------------------------------------------------------------------
}


//void		Terrain_DynamicShrubs()
//{
//	var your_handle;
//	int your_abstile_pos;
//	
//	you = ent_next(NULL); 																									// get first entity	
//	while (you) 																												// repeat until there are no more entities
//		{
//			if ( AbsTileValid((int)you.ABSTILE_POS) )	
//				{
//					// create shrub FLAG7 entity handle map and clearance map
//					if ( (you.ENT_CATEGORY == (var)UIMODE_SHRUB) && (is(you,FLAG7)) )							// FLAG7 : shrub collider entity
//						{
//							your_handle 		= handle(you);																// from handle
//							your_abstile_pos 	= (int)you.ABSTILE_POS;													// from skill	
//									
//							// set tile handle map
//							terrain_shrub_handlearray[your_abstile_pos] = your_handle;
//							
//							// set tile area clearance
//							Terrain_SetClearance( terrain_shrub_clearancearray , you );
//							wait_for(Terrain_SetClearance);
//						}
//				}
//			
//			you = ent_next(you); 																							// get next entity			
//		}
//}


void		Terrain_DynamicObstacles()
{
	var your_handle;
	int your_abstile_pos;
	
	you = ent_next(NULL); 																									// get first entity	
	while (you) 																												// repeat until there are no more entities
		{
			if ( AbsTileValid((int)you.ABSTILE_POS) )	
				{
					// create obstacle and mapent non-building entity handle map and clearance map
					if (
							(( you.ENT_CATEGORY == (var)UIMODE_OBSTACLE ) && ( you.ENT_TYPE == (var)0 ))  // obstacle and non-passable tile - obstacle type from its obs file containing its passability value : 0 == no access
							||
							(( you.ENT_CATEGORY == (var)UIMODE_BUILDING ) && ( you.ENT_TYPE == (var)0 )) 	// for obstacle like buildings - if 2 then it is building, if 1 then passable
						)
						{
							your_handle 		= handle(you);																// from handle
							your_abstile_pos 	= (int)you.ABSTILE_POS;													// from skill	
															
							// set tile handle map
							terrain_obstacle_handlearray[your_abstile_pos] = your_handle;
							
							// set tile area clearance
							Terrain_SetClearance( terrain_obstacle_clearancearray , you );
							wait_for(Terrain_SetClearance);								
						}
				}
			
			you = ent_next(you); 																							// get next entity			
		}
}


void		Terrain_DynamicShadowEntities()
{
	//------------------------------------------------------------------------------
	
	terrain_shadowentity_counter = 0;																					// required to allocate shadow entity list
	
	you = ent_next(NULL); 																									// get first entity	
	while (you) 																												// repeat until there are no more entities
		{
			// shadow casting entity quantity - required for memory allocation
			if (is(you,SHADOW))
				{
					if (
//							( you.ENT_CATEGORY == (var)UIMODE_SHRUB )
//							||
							( you.ENT_CATEGORY == (var)UIMODE_OBSTACLE )
							||
							( you.ENT_CATEGORY == (var)UIMODE_BUILDING )
							||
							( you.ENT_CATEGORY == (var)UIMODE_TERRMDL )
						)
						{
							terrain_shadowentity_counter++;
						}
				}				
			
			you = ent_next(you); 																							// get next entity			
		}
	
	//------------------------------------------------------------------------------
	// shadow entity list for switching them on/off runtime for better performance
		
	terrain_shadowentity_array = (ENTITY**)sys_malloc( terrain_shadowentity_counter * sizeof(ENTITY*) );	
	memset( terrain_shadowentity_array , (ENTITY*)0 , terrain_shadowentity_counter * sizeof(ENTITY*) ); 
	
	//------------------------------------------------------------------------------
	
	int i = 0;
	
	you = ent_next(NULL); 			// get first entity
	while (you) 						// repeat until there are no more entities
		{
			if (is(you,SHADOW))
				{
					if (
//							( you.ENT_CATEGORY == (var)UIMODE_SHRUB )
//							||
							( you.ENT_CATEGORY == (var)UIMODE_OBSTACLE )
							||
							( you.ENT_CATEGORY == (var)UIMODE_BUILDING )
							||
							( you.ENT_CATEGORY == (var)UIMODE_TERRMDL )
						)
						{
							terrain_shadowentity_array[i] = you;
							you.emask |= DYNAMIC;
							i++;
						}
				}
			you = ent_next(you); 	// get next entity
		}	
	
	//------------------------------------------------------------------------------
	// shadow clipping distance
			
	if (shadow_stencil == (var)8)
		{
			terrain_shadow_end = pssm_fadeoutend;
		}
	else if (shadow_stencil == (var)-1)
		{
			terrain_shadow_end = scsm_areasize * 2.5;
		}
	else
		{
			terrain_shadow_end = sky_loddistance3 * 2.5;
		}
	
	//------------------------------------------------------------------------------
}


void		Terrain_DynamicSummedMap()
{
	// create 4 terrain clearance map arrays, for each level
	int i,j,k;
	
	// dynamic allocation of clearance maps : capability, level, abstile
	terrain_summed_clearancearray = (char***)sys_malloc( 4 * sizeof(char**) );		
	
	// capability
	for (j=0; j<4; j++)
		{					
			terrain_summed_clearancearray[j] = (char**)sys_malloc( terrain_map_levels * sizeof(char*) );
			
			// level
			for (k=0; k<terrain_map_levels; k++)
				{
					(terrain_summed_clearancearray[j])[k] = (char*)sys_malloc( TileArrayLength * sizeof(char) );
					
					// fill with default values
					memset( (terrain_summed_clearancearray[j])[k] , (char)CL_DEFAULT , TileArrayLength * sizeof(char) ); 
					
					// create realistic results ready for controlled unit placement
					for( i=0 ; i<TileArrayLength ; i++ )
						{
							// ground
//							if (!(GetTileBuildingID(i) > (var)0))
							if (!BuildingTile(i))
								{
									// j : unit capability
									if (j == 0)
										{
											// (ground and buildings) and (shrubs and obstacles)
//											((terrain_summed_clearancearray[j])[k])[i] = (char)(int)minv( minv( (var)GetTileUnitSize(i, j) , (var)(int)(terrain_building_clearancearray[k])[i] ) , minv( (var)(int)terrain_shrub_clearancearray[i] , (var)(int)terrain_obstacle_clearancearray[i] ) );																						
											((terrain_summed_clearancearray[j])[k])[i] = (char)(int)minv( minv( (var)GetTileUnitSize(i, j) , (var)(int)(terrain_building_clearancearray[k])[i] ) ,  (var)(int)terrain_obstacle_clearancearray[i] );													
										}
									else if (j==1)
										{
											// (ground and buildings) and (shrubs and obstacles)
//											((terrain_summed_clearancearray[j])[k])[i] = (char)(int)minv( minv( (var)GetTileUnitSize(i, j) , (var)(int)(terrain_building_clearancearray[k])[i] ) , minv( (var)(int)terrain_shrub_clearancearray[i] , (var)(int)terrain_obstacle_clearancearray[i] ) );																						
											((terrain_summed_clearancearray[j])[k])[i] = (char)(int)minv( minv( (var)GetTileUnitSize(i, j) , (var)(int)(terrain_building_clearancearray[k])[i] ) , (var)(int)terrain_obstacle_clearancearray[i] );																																		
										}
									else if (j==2)
										{
											// (ground and buildings) and obstacles
											((terrain_summed_clearancearray[j])[k])[i] = (char)(int)minv( minv( (var)GetTileUnitSize(i, j) , (var)(int)(terrain_building_clearancearray[k])[i] ) , (var)(int)terrain_obstacle_clearancearray[i] );
										}
									else if (j==3)
										{
											// ground
											((terrain_summed_clearancearray[j])[k])[i] = (char)GetTileUnitSize(i, j);			
										}
								}
							// bulding floor
							else
								{
									// j : unit capability
									if (j < 3)
										{
											// buildings only
											((terrain_summed_clearancearray[j])[k])[i] = (terrain_building_clearancearray[k])[i];
										}
									else
										{
											// set it only once in game
											((terrain_summed_clearancearray[j])[k])[i] = (char)0;
										}
								}
							
						}																				
				}		
		}
}
	

void		Terrain_DynamicStaticMap()
{
	// create 4 terrain clearance map arrays, for each level
	int i,j,k;
	
	// dynamic allocation of clearance maps : capability, level, abstile
	terrain_static_clearancearray = (char***)sys_malloc( 4 * sizeof(char**) );		
	
	// capability
	for (j=0; j<4; j++)
		{					
			terrain_static_clearancearray[j] = (char**)sys_malloc( terrain_map_levels * sizeof(char*) );
			
			// level
			for (k=0; k<terrain_map_levels; k++)
				{
					(terrain_static_clearancearray[j])[k] = (char*)sys_malloc( TileArrayLength * sizeof(char) );
					
					// fill with default values
					memset( (terrain_static_clearancearray[j])[k] , (char)CL_DEFAULT , TileArrayLength * sizeof(char) ); 
					
					// create realistic results ready for controlled unit placement
					for( i=0 ; i<TileArrayLength ; i++ )
						{
							// ground
//							if (!(GetTileBuildingID(i) > (var)0))								// this line caused a bug - no clearance around buildings !!!
							if (!BuildingTile(i))
								{
									// j : unit capability
									if (j==0)
										{
											// (ground and buildings) and (shrubs and obstacles)
//											((terrain_static_clearancearray[j])[k])[i] = (char)(int)minv( minv( (var)GetTileUnitSize(i, j) , (var)(int)(terrain_building_clearancearray[k])[i] ) , minv( (var)(int)terrain_shrub_clearancearray[i] , (var)(int)terrain_obstacle_clearancearray[i] ) );	
											((terrain_static_clearancearray[j])[k])[i] = (char)(int)minv( minv( (var)GetTileUnitSize(i, j) , (var)(int)(terrain_building_clearancearray[k])[i] ) , (var)(int)terrain_obstacle_clearancearray[i] );																					
										}
									else if (j==1)
										{
											// (ground and buildings) and (shrubs and obstacles)
//											((terrain_static_clearancearray[j])[k])[i] = (char)(int)minv( minv( (var)GetTileUnitSize(i, j) , (var)(int)(terrain_building_clearancearray[k])[i] ) , minv( (var)(int)terrain_shrub_clearancearray[i] , (var)(int)terrain_obstacle_clearancearray[i] ) );																						
											((terrain_static_clearancearray[j])[k])[i] = (char)(int)minv( minv( (var)GetTileUnitSize(i, j) , (var)(int)(terrain_building_clearancearray[k])[i] ) , (var)(int)terrain_obstacle_clearancearray[i] );																																	
										}
									else if (j==2)
										{
											// (ground and buildings) and obstacles
											((terrain_static_clearancearray[j])[k])[i] = (char)(int)minv( minv( (var)GetTileUnitSize(i, j) , (var)(int)(terrain_building_clearancearray[k])[i] ) , (var)(int)terrain_obstacle_clearancearray[i] );
										}
									else if (j==3)
										{
											// ground
											((terrain_static_clearancearray[j])[k])[i] = (char)GetTileUnitSize(i, j);			
										}
								}
							// bulding floor
							else
								{
									// j : unit capability
									if (j<3)
										{
											// buildings only
											((terrain_static_clearancearray[j])[k])[i] = (terrain_building_clearancearray[k])[i];
										}
									else
										{
											// set it only once in game
											((terrain_static_clearancearray[j])[k])[i] = (char)0;
										}
								}
							
						}																				
				}		
		}
}


void		Terrain_DynamicGroupMap()
{
	//---------------------------------
	// shows how many groups are in a cluster - 0 if none - used by group pathfinder as a cluster cost penalty	
	
	terrain_group_clusterarray = (char*)sys_malloc( ClusterArrayLength * sizeof(char) );		
	memset( terrain_group_clusterarray , (char)0 , ClusterArrayLength * sizeof(char) ); 
	
	// update is in group update loop !
	//---------------------------------	
}


void		Terrain_DynamicGroupMapRecreate()
{
	
	if (terrain_group_clusterarray != NULL)
		{
			sys_free(terrain_group_clusterarray);
			terrain_group_clusterarray = NULL;	
		}
	
	Terrain_DynamicGroupMap();
	wait_for(Terrain_DynamicGroupMap);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// clearance map updates


void		Terrain_AllClearanceMapUpdate()
{
	//----------------------------------------------------
	// unit clearance map update - all levels together
	
	Terrain_UnitClearanceMapUpdate();
	wait_for(Terrain_UnitClearanceMapUpdate);
	
	//----------------------------------------------------
	// 4 capability maps
	
	// clearence map 1
	Terrain_SummedClearanceMapUpdate(0, 0, terrain_map_levels);
	wait_for(Terrain_SummedClearanceMapUpdate);
	
	// clearence map 2
	Terrain_SummedClearanceMapUpdate(1, 0, terrain_map_levels);
	wait_for(Terrain_SummedClearanceMapUpdate);
	
	// clearence map 3
	Terrain_SummedClearanceMapUpdate(2, 0, terrain_map_levels);
	wait_for(Terrain_SummedClearanceMapUpdate);								
	
	// clearence map 4
	Terrain_SummedClearanceMapUpdate(3, 0, terrain_map_levels);
	wait_for(Terrain_SummedClearanceMapUpdate);
	
	//----------------------------------------------------
//	// debug shader bmap									// does not work here without shader reset!!! required in each frame!
//	Play02_DebugMapUpdate();
//	wait_for(Play02_DebugMapUpdate);		
}


void		Terrain_SummedClearanceMapUpdate(int clearancemapnum, int fromlevel, int tolevel)
{	
	int i,j;
	
	// not necessary to clear !!! because all values are simply overwritten
	//memset( (terrain_summed_clearancearray[clearancemapnum])[j] , (char)CL_DEFAULT , TileArrayLength * sizeof(char) ); 	
	
	for( j=fromlevel ; j<tolevel ; j++ )
		{
			for( i=0 ; i<TileArrayLength ; i++ )
				{
					// non building
					if (!BuildingTile(i))
						{
							if (j==0)							// enough to process ground level
								{
									// unit + static , static needs to be updated only on change -> 1st change corresponding map (shrub/obstacle/building), then static
									// capability , j level , i tile ,
									((terrain_summed_clearancearray[clearancemapnum])[j])[i] = (char)(int)minv( (var)(int)(terrain_unit_clearancearray[0])[i] , ((terrain_static_clearancearray[clearancemapnum])[j])[i] );										
								}
						}
					// building
					else
						{
							if (clearancemapnum < 3)		// the same for ground units, water units not available
								{
									// units and level-j
									((terrain_summed_clearancearray[clearancemapnum])[j])[i] = (char)(int)minv( (var)(int)(terrain_unit_clearancearray[j])[i] , (var)(int)(terrain_building_clearancearray[j])[i] );								
								}
						}
				}			
		}
}


void		Terrain_UnitClearanceMapUpdate()
{
	int i;
	int totalunits = play02_groupsinarmy * play02_armies * play02_unitsingroup;
	
	// common unit pos data			
	for(i=0; i<terrain_map_levels; i++)
		{
			memset( terrain_unit_clearancearray[i] , (char)CL_DEFAULT , TileArrayLength * sizeof(char) ); 
		}
	
	for(i=0; i<(int)totalunits; i++)
		{				
			Terrain_SetClearance_Unit_Simple( terrain_unit_clearancearray , play02_unit_array[i].myentity );	
		}				
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
// clearance data creation


////////////////////////////////////////////////////////////////////////////////////////////////////////
// around shrubs, obstacles


void		Terrain_SetClearance( char* arraytoset , ENTITY* ent )
{
	//--------------------------------------------------------------------
	// area size can be even or odd	
	// MODIFIED VERSION - TILE POSITION IS TO LEFT AND TO BOTTOM FROM OBJECT MIDDLE - FOR ANY ENTITY
	
	int halfareax = (int)integer(ent.AREA_SIZEX / 2);
	int halfareay = (int)integer(ent.AREA_SIZEY / 2);	
		
	//--------------------------------------------------------------------
	
	int a = (int)ent.ABSTILE_POS - halfareax - halfareay * GetMapTilesX();			// lower-left tile of entity area
	a     = a - 2 - 2 * GetMapTilesX();														// lower-left tile of entity area +1 tile wide border
	
//	//--------------------------------------------------------------------
//	// area size can't be an even number !!!
//	
//	int a = ent.ABSTILE_POS - integer(ent.AREA_SIZEX/2) - (integer(ent.AREA_SIZEY/2)) * GetMapTilesX();		// lower-left tile of entity area
//	a		= a - 2 - 2 * GetMapTilesX();																								// lower-left tile of entity area with 0,1,3 clearance area included !!!
//	
	//--------------------------------------------------------------------
	
	int i,j;
	int temp;
	
	for (i=0 ; i<(ent.AREA_SIZEX +4) ; i++)
		{
			for (j=0 ; j<(ent.AREA_SIZEY +4) ; j++)
				{
					temp = a + i + j * GetMapTilesX();
					
					if ( AbsTileValid(temp) ) 																							// protection
						{
//							// 0 area - slower
//							if ( (i>1) && (i<(ent.AREA_SIZEX +2)) && (j>1) && (j<(ent.AREA_SIZEY +2)) )
//								{
//									arraytoset[temp] = (char)0;
//								}
							// 1 area border - else
							if (
									((i==1) && (j>0) && (j<(ent.AREA_SIZEY +3)))
									||
									((i==(ent.AREA_SIZEX +2)) && (j>0) && (j<(ent.AREA_SIZEY +3)))
									||
									((j==1) && (i>0) && (i<(ent.AREA_SIZEX +3)))
									||
									((j==(ent.AREA_SIZEY +2)) && (i>0) && (i<(ent.AREA_SIZEX +3)))
								)
								{
									arraytoset[temp] = (char)(int)minv( (var)1 , (var)(int)arraytoset[temp] );
								}
							// 3 area border
							else if ( 
											( (i==0) )
											||
											( (i==(ent.AREA_SIZEX +3)) )
											||
											( (j==0) )
											||
											( (j==(ent.AREA_SIZEY +3)) )
										)
								{
									arraytoset[temp] = (char)(int)minv( (var)3 , (var)(int)arraytoset[temp] );
								}
							// 0 area - faster
							else
								{
									arraytoset[temp] = (char)0;
								}
						}									
				}
		}	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// around buildings


void		Terrain_SetClearance_AroundBuilding( char** arraytoset , ENTITY* ent )
{	
	//--------------------------------------------------------------------
	// area size can be an even number !!! only in case of buildings !!!
	// MODIFIED VERSION - TILE POSITION IS TO LEFT AND TO BOTTOM FROM OBJECT MIDDLE - FOR ANY ENTITY
	
	int halfareax = (int)integer(ent.AREA_SIZEX / 2);
	int halfareay = (int)integer(ent.AREA_SIZEY / 2);
	
	//--------------------------------------------------------------------
	
	int a = (int)ent.ABSTILE_POS - halfareax - halfareay * GetMapTilesX();				// lower-left tile of entity area
	a     = a - 1 - GetMapTilesX();																	// lower-left tile of entity area +1 tile wide border
	
	//--------------------------------------------------------------------
	
	int temp, temp_n, tempx, tempy, tempflag;
	int i,j,k;
	
	for (i=0 ; i<(ent.AREA_SIZEX +2) ; i++)
		{
			for (j=0 ; j<(ent.AREA_SIZEY +2) ; j++)
				{
					// examined tile abstile
					temp = a + i + j * GetMapTilesX();									
					
					if ( AbsTileValid(temp) ) 	// protection
						{
							// set only lowest level of building clearance maps because it will be merged with ground clearance,
							// higher floors are handled separately !!! (terrain_building_clearancearray[level-1])[tile]
							if ( (arraytoset[0])[temp] > 1 )							
								{
									// check the 8 neighbouring tiles									
									// NEW
									for (k=0; k<8; k++)
										{
											temp_n = GetNeighbourofAbsTile(temp, k);
											
											if ( AbsTileValid(temp_n) )
												{													
													Terrain_SetClearance_NonBuilding_Tile( arraytoset[0], temp, temp_n );
												}
										}																
								}																			
						}
				}
		}
}


// sets clearance area of a non-building tile next to a building tile
void		Terrain_SetClearance_NonBuilding_Tile( char* arraytoset, var temp, var temp_n )
{
	int return_value = 0;																						// 0 = ground tile clearance should be kept, if later becomes higher it will be overwritten
	int i;
	
	if (BuildingTile(temp_n))																					// if neighbour temp_n is building
		{
			// if neighbour is a building tile: by default set to 1
			// estimating a blocking building tile next to it - later reset to 0 if there is a passable floor
			return_value = 1;
			
			for (i=0; i<terrain_map_levels; i++)															// passability of any building level (1 or 2) from neighbouring ground tile
				{
//					if (ClusterExists(GetTileClusterID(temp, i)))										// floor exists ###65535
					if (ClusterValid(GetTileClusterID(temp, i)))											// floor exists ###65535
						{
							// get tile height diff
							var height_diff = abs( GetTileMidZ(temp_n, i+1) - GetTileMidZ(temp, 0) );																										
							
							// if building tile too high or impassable, clearance should be set to 1
							// but use reverse logic: if height is okay and not impassable, do not modify clearance
							if (
									(height_diff <= (var)(MAX_WALK_Z_DIFF * GetMapTileSize()))
									&&
									(GetTileType(temp_n, i+1) % HEIGHT_OFFSET != TERR_BUILDING4)
								)
								{												
									// set to bld.clear.value+2
									int temp_int = GetTileUnitSizeMask(temp_n, i);						// can be 200, 100, or 0, 1, 3, 5
									
									if (temp_int >= CL_DEFAULT)
										temp_int = 98;
									
									if (temp_int==0)
										temp_int = -1;
									
									return_value = temp_int + 2;												// new - return the value to be set - then the minimum can be selected
									
									break;																			// no need to check higher levels if one found on a lower level
								}
						}
				}
		}
	
	if (return_value == 1)
		{
			Terrain_SetClearance_NonBuilding_Tile1( arraytoset, temp);
		}
	else if (return_value == 3)
		{
			Terrain_SetClearance_NonBuilding_Tile3( arraytoset, temp);
		}
	// else if 0 - no change
}


// sets clearance area of an impassable tile - not used
void		Terrain_SetClearance_NonBuilding_Tile0( char* arraytoset, int abstile )
{
	int temp;
	int a = abstile -2 -2*GetMapTilesX();			// lower-left tile of tile area - including 0,1,3 clearance values !!!	
	int i,j;
	for (i=0 ; i<(1 +4) ; i++)
		{
			for (j=0 ; j<(1 +4) ; j++)
			{
				temp = a + i + j * GetMapTilesX();
				
				if ( AbsTileValid(temp) ) // protection
				{
					if (!BuildingTile(temp))
					{
						// 0 area - faster
						if ((i==3) || (j==3))
						{
							arraytoset[temp] = (char)0;	// no minv needed :)
						}
						// 1 area border - else
						if ( ((i==1) && (j>0) && (j<(1 +3)))
						|| ((i==(1 +2)) && (j>0) && (j<(1 +3)))
						|| ((j==1) && (i>0) && (i<(1 +3)))
						|| ((j==(1 +2)) && (i>0) && (i<(1 +3)))
						)
						{
							arraytoset[temp] = (char)(int)minv( (var)1 , (var)(int)arraytoset[temp] );
						}
						// 3 area border
						else if ( 
						( (i==0) )
						|| ( (i==(1 +3)) )
						|| ( (j==0) )
						|| ( (j==(1 +3)) )
						)
						{
							arraytoset[temp] = (char)(int)minv( (var)3 , (var)(int)arraytoset[temp] );
						}									
					}
				}
				
			}
		}
}


// sets clearance area of passable tile having clearance value of 1
void		Terrain_SetClearance_NonBuilding_Tile1( char* arraytoset, int abstile )
{
	int temp;
	int a = abstile -1 -GetMapTilesX();			// lower-left tile of tile area - including 0,1,3 clearance values !!!	
	int i,j;
	for (i=0 ; i<(1 +2) ; i++)
		{
			for (j=0 ; j<(1 +2) ; j++)
				{
					temp = a + i + j * GetMapTilesX();
					
					if ( AbsTileValid(temp) ) // protection
						{
							if (!BuildingTile(temp))
								{
									// 1 area
									if ( (i==1) && (j==1) )
										{
											arraytoset[temp] = (char)(int)minv( (var)1 , (var)(int)arraytoset[temp] );
										}
									// 3 area border - else
									else
										{
											arraytoset[temp] = (char)(int)minv( (var)3 , (var)(int)arraytoset[temp] );
										}									
								}
						}
					
				}
		}
}


// sets clearance area of passable tile having clearance value of 3 
void		Terrain_SetClearance_NonBuilding_Tile3( char* arraytoset, int abstile )
{
	if (!BuildingTile(abstile))
		{
			arraytoset[abstile] = (char)(int)minv( (var)3 , (var)(int)arraytoset[abstile] );
		}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// around units


void		Terrain_SetClearance_Unit( char** arraytoset , ENTITY* ent )							// levels, tilearray   
{
	int temp;
	
	// area size always odd
	int halfareax = (int)integer(ent.AREA_SIZEX / 2);
	int halfareay = (int)integer(ent.AREA_SIZEY / 2);		
	
	// entity abs tile pos
	int pos = (int)ent.ABSTILE_POS;
	
	// lower left area corner
	int a = (int)ent.ABSTILE_POS - halfareax - 2 - halfareay * GetMapTilesX() - 2 * GetMapTilesX();	// lower-left tile of entity area - including 0,1,3 clearance values !!!	
	
	// entity z level
	int b = (int)ent.ZTILE_POS;									// 0-1-2 !!!
	
	// tile z pos (var) at entity place
	//	int midz = 0;
	int midz = GetTileMidZ(pos, b);						// b: 0-1-2
	
	
	// entity level c: 0-1
	int c = b;
	if (c > 0)	
		{
			c--;
		}
	
	// set area tiles
	int i,j,k;
	for (i=0 ; i<((int)ent.AREA_SIZEX +4) ; i++)
		{
			for (j=0 ; j<((int)ent.AREA_SIZEY +4) ; j++)
				{
					// abstile value converted from x-y
					temp = a + i + j * GetMapTilesX();					//  lower left abs tile plus for i-j values
					
					if ( AbsTileValid(temp) ) // protection
						{
							int m = 1;	
							int nexttile_mod = 0;		
							if (BuildingTile(temp))
								{
									m = terrain_map_levels;		// 2
									nexttile_mod = 1;
								}
							
							for (k=0; k<m; k++)					// 0-1
								{
									// valid cluster protection
//									if (ClusterExists(GetTileClusterID(temp, k)))
									if (ClusterValid(GetTileClusterID(temp, k)))
										{									
											// by default own area is 0
											var value_to_set = 0;
											
											// 1 area border - else
											if ( 		((i==1) && (j>0) && (j<(ent.AREA_SIZEY +3)))
													|| ((i==(ent.AREA_SIZEX +2)) && (j>0) && (j<(ent.AREA_SIZEY +3)))
													|| ((j==1) && (i>0) && (i<(ent.AREA_SIZEX +3)))
													|| ((j==(ent.AREA_SIZEY +2)) && (i>0) && (i<(ent.AREA_SIZEX +3)))
												)
												{							
													value_to_set = 1;
												}
											// 3 area border
											else if ( 
																( (i==0) )
															|| ( (i==(ent.AREA_SIZEX +3)) )
															|| ( (j==0) )
															|| ( (j==(ent.AREA_SIZEY +3)) )
														)
												{
													value_to_set = 3;
												}
											
											//											// test
											//											(arraytoset[k])[temp] = (char)(int)minv( value_to_set , (var)(int)(arraytoset[k])[temp] );
											//											(arraytoset[k])[temp] = 0;
											
											// height difference, not abs, so okay for different upward and downward move values
											var height_diff = GetTileMidZ(temp, k+nexttile_mod) - midz;																							
											
											// can be set only if within z walkability range i.e. 1 tile 
											// if any ladder
											if ( (GetTileType(temp, k+nexttile_mod) == TERR_SIEGE) || (GetTileType(pos, b) == TERR_SIEGE) )
												{
													if (
															( (height_diff>=(var)0) && (height_diff<=((var)MAX_WALK_Z_DIFF*3*GetMapTileSize())) )					// 1.25*3 climb up/down difference
															||
															( (height_diff<(var)0) && (height_diff>=(-(var)MAX_WALK_Z_DIFF*3*GetMapTileSize())) )
														)
														{
															(arraytoset[k])[temp] = (char)(int)minv( value_to_set , (var)(int)(arraytoset[k])[temp] );
														}
												}
											else				// non-ladder
												{
													if (
															( (height_diff>=(var)0) && (height_diff<=((var)MAX_WALK_Z_DIFF*(var)GetMapTileSize())) )			// 1.25 walk up difference
															||
															( (height_diff<(var)0) && (height_diff>=(-(var)MAX_WALK_Z_DIFF*1.5*(var)GetMapTileSize())) )	// 1.25*1.5 walk down difference
														)
														{
															(arraytoset[k])[temp] = (char)(int)minv( value_to_set , (var)(int)(arraytoset[k])[temp] );
														}
												}
										}						// valid cluster protection
								}								// k				
						}										// tile boundary protetcion
				}												// j
		}														// i	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// around units


void		Terrain_SetClearance_Unit_Simple( char** arraytoset , ENTITY* ent )
{
	int temp;
	
	// area size always odd
	int halfareax = (int)integer(ent.AREA_SIZEX / 2);
	int halfareay = (int)integer(ent.AREA_SIZEY / 2);	
	
	// entity abs tile pos
	int pos = (int)ent.ABSTILE_POS;
	
	// lower left area corner
	int a = (int)ent.ABSTILE_POS - halfareax - halfareay * GetMapTilesX();	// lower-left tile of entity area - without 1,3 clearance values !!!	
	
	// entity z level
	int b = (int)ent.ZTILE_POS;								// 0-1-2 !!!
	
	// tile z pos (var) at entity place
	int midz = GetTileMidZ(pos, b);					// b: 0-1-2
	
	
	// entity level c: 0-1
	int c = b;
	if (c > 0)	
		{
			c--;
		}
	
	// set area tiles
	int i,j,k;
	for (i=0 ; i<((int)ent.AREA_SIZEX) ; i++)
		{
			for (j=0 ; j<((int)ent.AREA_SIZEY) ; j++)
				{
					// abstile value converted from x-y
					temp = a + i + j * GetMapTilesX();					//  lower left abs tile plus for i-j values
					
					if ( AbsTileValid(temp) ) // protection
						{
							int m = 1;								// max levels
							int nexttile_mod = 0;		
							if (BuildingTile(temp))
								{
									m = terrain_map_levels;			// 2
									nexttile_mod = 1;
								}
							
							for (k=0; k<m; k++)					// 0-1
								{
									// valid cluster protection
//									if (ClusterExists(GetTileClusterID(temp, k)))
									if (ClusterValid(GetTileClusterID(temp, k)))
										{									
											// by default own area is 0
											var value_to_set = 0;
											
// removed--------------------------------------------------------------------
//											// 1 area border - else
//											if ( ((i==1) && (j>0) && (j<(ent.AREA_SIZEY +3)))
//													|| ((i==(ent.AREA_SIZEX +2)) && (j>0) && (j<(ent.AREA_SIZEY +3)))
//														|| ((j==1) && (i>0) && (i<(ent.AREA_SIZEX +3)))
//															|| ((j==(ent.AREA_SIZEY +2)) && (i>0) && (i<(ent.AREA_SIZEX +3)))
//												 )
//												{							
//													value_to_set = 1;
//												}
//											// 3 area border
//											else if ( 
//												( (i==0) )
//													|| ( (i==(ent.AREA_SIZEX +3)) )
//														|| ( (j==0) )
//															|| ( (j==(ent.AREA_SIZEY +3)) )
//												 )
//												{
//													value_to_set = 3;
//												}
//----------------------------------------------------------------------------
//
//											// test
//											(arraytoset[k])[temp] = (char)(int)minv( value_to_set , (var)(int)(arraytoset[k])[temp] );
//											(arraytoset[k])[temp] = 0;
											
											// height difference, not abs, so okay for different upward and downward move values
											var height_diff = GetTileMidZ(temp, k+nexttile_mod) - midz;
											
											// can be set only if within z walkability range i.e. 1 tile 
											// if any ladder
											if ( (GetTileType(temp, k+nexttile_mod) == TERR_SIEGE) || (GetTileType(pos, b) == TERR_SIEGE) )
												{
													if (
													( (height_diff >= (var)0) && (height_diff <= ((var)MAX_WALK_Z_DIFF*3*GetMapTileSize())) )		// 1.25*3 climb up/down difference
													||
													( (height_diff < (var)0) && (height_diff >= (-(var)MAX_WALK_Z_DIFF*3*GetMapTileSize())) )
													)
														{
															(arraytoset[k])[temp] = (char)(int)minv( value_to_set , (var)(int)(arraytoset[k])[temp] );
														}
												}
											else	// non-ladder
												{
													if (
													( (height_diff >= (var)0) && (height_diff <= ((var)MAX_WALK_Z_DIFF*(var)GetMapTileSize())) )		// 1.25 walk up difference
													||
													( (height_diff < (var)0) && (height_diff >= (-(var)MAX_WALK_Z_DIFF*1.5*(var)GetMapTileSize())) )	// 1.25*1.5 walk down difference
													)
														{
															(arraytoset[k])[temp] = (char)(int)minv( value_to_set , (var)(int)(arraytoset[k])[temp] );
														}
												}
										}	// valid cluster protection
								}	// k				
						}	// tile boundary protetcion
				}	// j
		}	// i
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Terrain_ProgressMessage()
{
	proc_kill(4);
	wait(1);
	
	if (terrain_progress_message == NULL)
		{
			terrain_progress_message = str_create("");
		}
	else
		{
			str_cpy(terrain_progress_message, "");
		}
	
	while (str_cmpi(terrain_progress_message, ""))
		{
			wait(1);
		}
	
	while (!str_cmpi(terrain_progress_message, ""))
		{
			draw_text(terrain_progress_message, 250, 450, vector(text_font_b, text_font_g, text_font_r));
			
			draw_text("ClusterArrayLength", 250, 475, vector(text_font_b, text_font_g, text_font_r));
			draw_text( str_for_int(NULL, ClusterArrayLength), 450, 475, vector(text_font_b, text_font_g, text_font_r));
			
			wait(1);
		}
}


void		Terrain_ProgressMessageFree()
{
	proc_kill2(Terrain_ProgressMessage, NULL);
	wait(1);
	if (terrain_progress_message != NULL)
		{
			ptr_remove(terrain_progress_message);
			terrain_progress_message = NULL;
		}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif