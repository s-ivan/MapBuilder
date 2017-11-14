
#ifndef MBpathtile_rts_c
#define MBpathtile_rts_c


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// init / close


void	PathTile2D_Init()
{
sys_marker("PT0");
	
	// allocate one astar struct - enough for military units
	pathtile2d_astararray = (PLAY02_TILEASTARSYS*)sys_malloc( pathtile2d_astararraylength * sizeof(PLAY02_TILEASTARSYS) );
		
	int i;
	for(i=0; i<pathtile2d_astararraylength; ++i)
		{
			// astar parameters
			
			pathtile2d_astararray[i].astarareaside	= integer(GetMapClusterSize() * AREASIZE_MULTIPLIER);
			if (pathtile2d_astararray[i].astarareaside % 2 > 0)
				{
					++pathtile2d_astararray[i].astarareaside;
				}
			
			pathtile2d_astararray[i].astararraysize	= pathtile2d_astararray[i].astarareaside * pathtile2d_astararray[i].astarareaside * terrain_map_levels;
			
			pathtile2d_astararray[i].astarstartx		= 0;
			pathtile2d_astararray[i].astarstarty		= 0;
			
			pathtile2d_astararray[i].astartargetx		= 0;
			pathtile2d_astararray[i].astartargety		= 0;
			
			pathtile2d_astararray[i].astarstartabs		= 0;
			pathtile2d_astararray[i].astartargetabs	= 0;
			
			pathtile2d_astararray[i].firstopen			= (short)65535;
			pathtile2d_astararray[i].lastopen			= (short)65535;
			
			// astar arrays
			
			int m = pathtile2d_astararray[i].astararraysize;
			
			pathtile2d_astararray[i].f				= (short*)array_new( sizeof(short), NULL, m, 255 );			// 65535 to avoid possible smallest F issues when loop is interrupted
			pathtile2d_astararray[i].g				= (short*)array_new( sizeof(short), NULL, m, 0 );
			pathtile2d_astararray[i].h				= (short*)array_new( sizeof(short), NULL, m, 0 );
			pathtile2d_astararray[i].parentabs	= (short*)array_new( sizeof(short), NULL, m, 255 );			// (char)255 to be (short)65535
			
			pathtile2d_astararray[i].oclist		= (char*)array_new( sizeof(char), NULL, m, 0 );
			pathtile2d_astararray[i].clearance	= (char*)array_new( sizeof(char), NULL, m, 0 );
			pathtile2d_astararray[i].direction	= (char*)array_new( sizeof(char), NULL, m, 255 );
			
			pathtile2d_astararray[i].prevopen	= (short*)array_new( sizeof(short), NULL, m, 255 );
			pathtile2d_astararray[i].nextopen	= (short*)array_new( sizeof(short), NULL, m, 255 );					
		}
sys_marker(NULL);
}


void	PathTile2D_Free()
{
sys_marker("PT1");
	int i;	
	for (i=0; i<pathtile2d_astararraylength; ++i)
		{			
			array_destroy( pathtile2d_astararray[i].f );
			array_destroy( pathtile2d_astararray[i].g );
			array_destroy( pathtile2d_astararray[i].h );
			array_destroy( pathtile2d_astararray[i].parentabs );
			array_destroy( pathtile2d_astararray[i].oclist );
			array_destroy( pathtile2d_astararray[i].clearance );
			array_destroy( pathtile2d_astararray[i].direction );
			array_destroy( pathtile2d_astararray[i].prevopen );
			array_destroy( pathtile2d_astararray[i].nextopen );
			
//			if (i%50==0) {wait(1);}	
		}
sys_marker("PT2");
	
	array_destroy( pathtile2d_astararray );

sys_marker(NULL);
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// conversion


int	PathTIle2D_GetTileAbsAbs(int abstile, int ztile, int xsize)
{
	int ysize = xsize;
	
	return( abstile + (xsize*ysize)*ztile );							// multilevel abstile
}


int	PathTIle2D_GetTileAbs(int xtile, int ytile, int ztile, int xsize)		// ztile is 0-1
{
	int ysize = xsize;
	
	return( (xtile + ytile*xsize) + (xsize*ysize)*ztile );		// multilevel abstile
}


int	PathTIle2D_GetTileX(int abstile, int xsize)
{	
	int ysize = xsize;	
	
	if (abstile > xsize*ysize)												// multilevel check
		{
			abstile -= xsize*ysize;			
		}
	
	int xvalue = abstile%xsize;
	
	return (xvalue);
//	return( abstile%xsize );												// 1 level						
}


int	PathTIle2D_GetTileY(int abstile, int xsize)
{
	int ysize = xsize;	
	
	if (abstile > xsize*ysize)												// multilevel check
		{
			abstile -= xsize*ysize;			
		}	
	int yvalue = integer(abstile/xsize);
	
	return (yvalue);		
//	return( integer(abstile/xsize) );									// 1 level	
}


// z = integer(index/(xsize*ysize));
int	PathTIle2D_GetTileZ(int abstile, int xsize)
{
	int ysize = xsize;
	
	return( integer(abstile/(xsize*ysize)) );							// if abstile is multilevel, 0-1
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// debugging


void	PathTile2D_DebugDrawPoint(COLOR* col, VECTOR* coord)
{
	int counter = time_frame * 16 * 2;	// 2 sec in theory
	
	// to hold data over frames
	VECTOR vec, color;
	vec_set(vec, coord);
	vec_set(color, col);
	
	while(counter > 0)
		{
			draw_point3d(vec, color, 100, 5);	
						
			wait(1);
			
			counter--;
		}
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// pathfinder


int		PathTile2D_GetPath( PLAY02_UNITSTRUCT* unit_property, 
										PLAY02_TILEASTARSYS* unit_astar, 
										int unit_capability, 
										char** static_clearancearray,
										int unit_size, 
										int maxpathlength, 
										int maxpathloop, 
										int debug_path,
										int debug_area,
										int far_lod,
										int turn_limit
									 	)
{
	//-----------------------------------------------------------------------------------------
sys_marker("gpv");	
	// for statistics to see what method was used (tile path search)
	play02_pathcounter++;																									
	
	//-----------------------------------------------------------------------------------------
	// protections
	
	if (unit_property == NULL)				return 0;
	if (unit_astar == NULL)					return 0;
	if (static_clearancearray == NULL)	return 0;
	
	//-----------------------------------------------------------------------------------------
	// tilearray game area border checking is needed to avoid script crash !
			
	PathTile2D_LimitStartTargetTileToGameArea(unit_property);
	
	//-----------------------------------------------------------------------------------------
	// store start and target tile positions (map position) - set it because it might change because of new user interactions
	
	PathTile2D_StoreStartTargetTilePos(unit_property, unit_astar);
	
	//-----------------------------------------------------------------------------------------
	// limiting start target distance - to be within search area - to avoid too long loops caused by non-accessible targets - can lead to pathfinding to wrong targets!!!
	// modifies only stored start and target, not the one stored in unit's struct, to enable the unit to get a path again to that far target
	
	PathTile2D_LimitTargetTileByDistance(unit_property, unit_astar);
	
	//-----------------------------------------------------------------------------------------
	// tile conversion needed from full map to astar array map !!!	
	
	// min xy tile of astar area in world tile coordinates
	PathTile2D_SetAstarMinXYTile(unit_astar);
	
	// set multilevel abs tiles
	if (!PathTile2D_SetAstarStartTargetTiles(unit_astar))
		{
			return 0;
		}
	
	//-----------------------------------------------------------------------------------------
	// reset parameters and arrays
	
	PathTile2D_ResetParameters(unit_property, unit_astar, turn_limit);
	
	//-----------------------------------------------------------------------------------------
	// set clearance data within examined area, except own position, based on entity and clearance arrays
	
	PathTile2D_GenerateAstarClearanceMap(unit_property, unit_astar, static_clearancearray, unit_capability, far_lod);
	
	//-----------------------------------------------------------------------------------------
	// set start tile parameters
	
	PathTile2D_InitAstarStartTie(unit_property, unit_astar, turn_limit);

	//-----------------------------------------------------------------------------------------
	// start settings
	
	var error1 = 0 ; 												// no error
	var found = 0 ;												// target not found  
	int cycle_time = 1; 											// for loop limiting
	
	// loop iterators
	int i;
	int j;
	int k;
	int m;
	
	//-----------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------
	// the main loop 
	
	while ((found < 1) && (error1 < 1))
		{
			//------------------------------------------------------
			
			cycle_time++;
			
			//------------------------------------------------------
			// get a tile whose neighbouring tiles will be examined
			
			// xyz tile having the lowest F value
			int tempa = -1;		
			int tempb = -1;
			int tempc = -1;
			
			// abs tile having the lowest F value
			int tempcurrent = -1;	
			
			// find best unprocessed tile (lowest f) - break loop if not found
			if (!PathTile2D_GetTileLowestF(unit_astar, &tempa, &tempb, &tempc, &tempcurrent))
				{
					error1 = 1;
					break;		// the while loop
				}
			
			//------------------------------------------------------
			// the smallest element goes to closed list from open list
			
			// now tile [tempc][tempa][tempb] and [tempcurrent] holds the best position,
			// and its neighbours will be checked now
			
			(unit_astar->oclist)[tempcurrent] = 2;
			
			//------------------------------------------------------
			// looking around, i.e. checking the 8 possible neighbours
			// with diagonal travel penalty
sys_marker("gpb");
			
			var multiplier1;      										// multiply by: 1 if straight, 1.4 if diagonal route
			
			i = 0;															// direction
			
			while ((i < 8) && (!found) && (!error1))				// directions loop: 1-4 straight <5, 5-8 diagonal <9		- mod to angle/45 directions
				{
					//------------------------------------------------------
					// neighbouring tile to be examined - mod to angle/45 directions
					
					int tempx;
					int tempy;
					int tempz;
					
					PathTile2D_SetExaminedTile(unit_astar, i, tempa, tempb, &tempx, &tempy, &multiplier1);
					
					// now tempa-b-c stores the original tile, and tempx-y the neighbour examined actually
					
					// tempz can be set after building check by PathTile2D_SetBuildingFlag()
					tempz = 0;
					
					//------------------------------------------------------
					// turn limit (should be reworked using true A*, introducing additional movement cost as turn penalty)
					
					var turn_allowed = 1;
					
					if (turn_limit > 0)
						{
							if (DirectionDifference( (int)(unit_astar->direction)[tempcurrent], i ) > turn_limit)		// 0 no limit, 1 45°limit, 2 90° limit (, 3 135°limit)
								{
									turn_allowed = 0;
								}
						}
					
					//------------------------------------------------------	
					// protection
					
					if (turn_allowed)
					if (tempx >= 0) 
					if (tempy >= 0) 
					if (tempx < unit_astar->astarareaside) 
					if (tempy < unit_astar->astarareaside) 
					if (GetMapTilesX() > (tempx + unit_astar->astarminxtile)) 
					if (GetMapTilesY() > (tempy + unit_astar->astarminytile))						
						{
							//------------------------------------------------------
							// building check
							
							// neighbour can be a buildng tile, if yes, check height difference, if accessible set to 1, else 0
							// if not building, simply set to 1
							// (should be 1 to be tile passable)
							var buildingflag = PathTile2D_SetBuildingFlag(unit_astar, tempa, tempb, tempc, tempx, tempy, &tempz);																		
							
							//------------------------------------------------------	
							// just examined tile in astar abstile
							
							int tempnext = PathTIle2D_GetTileAbs(tempx, tempy, tempz, unit_astar->astarareaside);
							
							//------------------------------------------------------	
							
							if ( (unit_astar->clearance)[ tempnext ] >= (char)unit_size ) 		// ### clearance value bigger than or equal as own size >>> OWN MAP VERSION USES HERE (unit_astar->clearance)[tempnext] instead of (unit_clearancearray[tempz])[temp] <<<
							if	( (unit_astar->oclist)[ tempnext ]<(char)1 )							// oclist value was 2 , but not okay if an already open list tile is overwritten, because it is definitely results in a longer path	
							if	( buildingflag )																// ### not building, or not inaccessible building tile																				
								{
									
//									// A* first line -----------------------------------------------------------------------------------
//									if ( ((unit_astar->oclist)[tempnext] == (char)1 )	 		  // if tile is already on open list	
//										{
//											// check whether now G is smaller than previously i.e. now the path is shorter to here
//											// TileArray stores move cost terrain properties
//											if ( ( (unit_astar->g)[tempcurrent] + GetTileCost(tempnext, tempz)*multiplier1 ) < (unit_astar->g)[tempnext] )
//												{
//													// giving new values to F G and "parent" in this tile 
//													
//													// G = parent value + (current terrain penalty * direction multiplier1) + height penalty
//													(unit_astar->g)[tempnext] = (unit_astar->g)[tempa])[tempb] + (GetTileCost(temp, ?)*multiplier1) ;
//													
////												// depth first
////												(unit_astar->g)[tempnext] = 0;		
//													
//													// F: H value must exist because of previous examination is done
//													(unit_astar->f)[tempnext] = (unit_astar->g)[tempnext] + (unit_astar->h)[tempnext]; 
//													
//													(unit_astar->parentabs)[tempnext] = tempcurrent;
//												}										
//										}
//									else	// if tile is not on open list yet		
//									// A* last line -------------------------------------------------------------------------------------
										
										{
											//---------------------------------
											// put tile onto open list
											
											(unit_astar->oclist)[ tempnext ] = 1;
											
											//---------------------------------
											// G - cost of path completed
											
											// if A* 
//											(unit_astar->g)[ tempnext ] = (unit_astar->g)[ tempcurrent ] + (GetTileCost(tempnext_in_world_coord, tempz) * multiplier1;
											
											// 0 if Depth First 
											(unit_astar->g)[ tempnext ] = 0;
											
											//---------------------------------									
											// H - heuristic function - cost of estimated distance to target
											
											// DIAGONAL SHORTCUT
											(unit_astar->h)[tempnext] = (short)(int)integer( ( minv( abs(tempx - unit_astar->astartargetx) , abs(tempy - unit_astar->astartargety) ) * 1.4
																											 + abs( abs(tempx - unit_astar->astartargetx) - abs(tempy - unit_astar->astartargety) )
																											 ) * GetMapDefaultMoveCost() );
											
//											// EUCLIDIAN
//											(unit_astar->h)[tempnext] = (short)integer( sqrt(pow(abs(tempx - unit_astar->astartargetx),2) + pow(abs(tempy - unit_astar->astartargety),2)) * GetMapDefaultMoveCost() );	
											
//											// MANHATTAN
//											(unit_astar->h)[tempnext] = (short)integer( ( abs(tempx - unit_astar->astartargetx) + abs(tempy - unit_astar->astartargety) ) * GetMapDefaultMoveCost() );
											
//											// horizontal modification
//											(unit_astar->h)[tempnext] += (short)((int)abs(tempz - unit_astar->targetz) * GetMapClusterSize() * GetMapDefaultMoveCost());
											
											//---------------------------------
											// F = G + H
											
											(unit_astar->f)[tempnext] = (unit_astar->g)[tempnext] + (unit_astar->h)[ tempnext ];
											
											//---------------------------------
											// parent i.e. previous step tile
											
											(unit_astar->parentabs)[tempnext] = (short)tempcurrent;
											
											//---------------------------------
											// direction
											
											(unit_astar->direction)[tempnext] = (char)i;
											
											//---------------------------------
											// update pseudo linked list data
{}
#ifdef ASTAR_OPEN_LINKED_TILE																						
											// new tile's next tile is NULL
											(unit_astar->nextopen)[tempnext] = (short)65535;
											
											// new tiles's previous tile is the actual last open tile
											(unit_astar->prevopen)[tempnext] = unit_astar->lastopen;
											
											// update last open tile's next tile to be the new tile
											if (unit_astar->lastopen != (short)65535)																											
												{
													(unit_astar->nextopen)[(int)unit_astar->lastopen] = (short)tempnext;
												}											
											
											// set last open tile now to be the new tile
											unit_astar->lastopen = (short)tempnext;
											
											// if first open tile is NULL, set new tile as first open tile
											if (unit_astar->firstopen == (short)65535)							// != ??? why not == ? - it was THE LLL STUCK BUG !!!																																							
												{
													unit_astar->firstopen = (short)tempnext;
												}		
#endif
											//---------------------------------
										}
								}	
						}
					
					i++;			// increment direction
				}
			
			//------------------------------------------------------
			// stop loop if needed
sys_marker("gpc");			
			
			// if target gets on open or closed list, then target is found
			if ( (unit_astar->oclist)[unit_astar->astartargetabs] > (char)0) 
				{
					found = 1;
					break;
				}
			
			// to avoid infinite loop, exit while loop
			if ( cycle_time >= maxpathloop ) 
				{
					error1 = 1;
					break;
				}	
			
			//------------------------------------------------------	
		}
	
	//-----------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------
	// main loop error handling
	
	//----------------------------------------------------------------------------
	// if no target found by main loop, try to find best possible target tile
	
	PathTile2D_BestTargetApproximation(unit_astar, &error1, &found);
	
	//----------------------------------------------------------------------------
	//  failure messages on unhandled errors
sys_marker("gpe");	

	if (PathTile2D_UnresolvableError(unit_property, error1, debug_path, debug_area, cycle_time))
		{
			return 0;
		}
	
	if (PathTile2D_PathNotFound(unit_property, found, debug_path, debug_area, cycle_time))
		{
			return 0;
		}
	
	//-----------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------
	// create path
	
	int pathlength = 0;
	int counter = 0;
	
	if (PathTile2D_CreatePath(unit_property, unit_astar, maxpathlength, debug_path, debug_area, &pathlength, &counter))
		{
			return 0;
		}
	
	//----------------------------------------------------------------------------
	// path error handling
sys_marker("gpm");
		
	if (PathTile2D_TooShortPath(unit_property, counter, debug_path, debug_area, cycle_time))
		{
			return 0;
		}
	
	pathlength = PathTile2D_LimitPathLength(unit_property, maxpathlength, counter, debug_path, debug_area, cycle_time);
	
	//----------------------------------------------------------------------------
	
	PathTile2D_FinalizePath(unit_property, pathlength);
	
	//-----------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------
	// debugging
	
	PathTile2D_DebugUnitPath(unit_property, debug_path, debug_area);	
	PathTile2D_DebugUnitArea(unit_astar, debug_path, debug_area);
	
	//-----------------------------------------------------------------------------------------
	//-----------------------------------------------------------------------------------------
	
	return 1;
}


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// helpers


void		PathTile2D_LimitStartTargetTileToGameArea(PLAY02_UNITSTRUCT* unit_property)
{
sys_marker("gp0");
	
	unit_property->mystarttilex = (short)(int)clamp((var)(int)unit_property->mystarttilex, 0, (var)GetMapTilesX()-1);
	unit_property->mystarttiley = (short)(int)clamp((var)(int)unit_property->mystarttiley, 0, (var)GetMapTilesY()-1);
	
	unit_property->mytargettilex = (short)(int)clamp((var)(int)unit_property->mytargettilex, 0, (var)GetMapTilesX()-1);
	unit_property->mytargettiley = (short)(int)clamp((var)(int)unit_property->mytargettiley, 0, (var)GetMapTilesY()-1);
}


void		PathTile2D_StoreStartTargetTilePos(PLAY02_UNITSTRUCT* unit_property, PLAY02_TILEASTARSYS* unit_astar)
{
sys_marker("gps");	
	
	unit_astar->startx	= (int)unit_property->mystarttilex;				// from short
	unit_astar->starty	= (int)unit_property->mystarttiley;				// from short
	unit_astar->startz	= (int)unit_property->mystarttilez;				// from char
	// not now !
//	if (unit_astar->startz > 0)
//		{
//			unit_astar->startz--;													// convert 0-1-2 to 0-1 !!!
//		}
	
	unit_astar->targetx	= (int)unit_property->mytargettilex;			// from short
	unit_astar->targety	= (int)unit_property->mytargettiley;			// from short	
	unit_astar->targetz	= (int)unit_property->mytargettilez;			// from char
	// not now !
//	if (unit_astar->targetz > 0)
//		{
//			unit_astar->targetz--;													// convert 0-1-2 to 0-1 !!!
//		}
}


void		PathTile2D_LimitTargetTileByDistance(PLAY02_UNITSTRUCT* unit_property, PLAY02_TILEASTARSYS* unit_astar)
{
sys_marker("gpr");	
	
	var modified = 0;
	
	// x distance
	if (unit_astar->startx - unit_astar->targetx >= unit_astar->astarareaside)									// start > target
		{
			unit_astar->targetx = unit_astar->startx - unit_astar->astarareaside + 1;
			modified = 1;
		}
	else if (unit_astar->targetx - unit_astar->startx >= unit_astar->astarareaside)							// start < target
		{
			unit_astar->targetx = unit_astar->startx + unit_astar->astarareaside - 1;
			modified = 1;
		}
	
	// y distance
	if (unit_astar->starty - unit_astar->targety >= unit_astar->astarareaside)									// start > target
		{
			unit_astar->targety = unit_astar->starty - unit_astar->astarareaside + 1;
			modified = 1;
		}
	else if (unit_astar->targety - unit_astar->starty >= unit_astar->astarareaside)							// start < target
		{
			unit_astar->targety = unit_astar->starty + unit_astar->astarareaside - 1;
			modified = 1;
		}
	
	// get valid z
	if (modified)
		{
			// the unit is probably separated from its group !!!
			// problem solving on group level needed !!!
			
			if (!ValidZPos3D(XYTileToAbsTile(unit_astar->targetx, unit_astar->targety), unit_astar->targetz))		// here targetz is wrong !!! was converted to level2 !!!
				{
					// search from top floor to ground
					int i;
					for(i=terrain_map_levels; i>=0; --i)																	// 0-1-2
						{
							if (ValidZPos3D(XYTileToAbsTile(unit_astar->targetx, unit_astar->targety), i))
								{
									unit_astar->targetz = i;																	// 0-1-2
								}
						}					
				}
		}
}


void		PathTile2D_SetAstarMinXYTile(PLAY02_TILEASTARSYS* unit_astar)
{
sys_marker("gpt");	
	//-----------------------------------------------------------------------------------------
	// lower left corner of astar map in full tile map coordinates
	
	if (unit_astar->startx < unit_astar->targetx)
		{
			unit_astar->astarminxtile = unit_astar->startx - integer( ( unit_astar->astarareaside - abs(unit_astar->targetx - unit_astar->startx) ) / 2 );				//  (areawidth - start_target_xdistance ) / 2
		}
	else
		{	
			unit_astar->astarminxtile = unit_astar->targetx - integer( ( unit_astar->astarareaside - abs(unit_astar->startx - unit_astar->targetx) ) / 2 );					
		}
	
	if (unit_astar->starty < unit_astar->targety)
		{
			unit_astar->astarminytile = unit_astar->starty - integer( ( unit_astar->astarareaside - abs(unit_astar->targety - unit_astar->starty) ) / 2 );
		}
	else
		{
			unit_astar->astarminytile = unit_astar->targety - integer( ( unit_astar->astarareaside - abs(unit_astar->starty - unit_astar->targety) ) / 2 );
		}	
	
	//-----------------------------------------------------------------------------------------
	// safety check 
	
	if (unit_astar->astarminxtile < 0) 
		{
			unit_astar->astarminxtile = 0;
		}
	else if (unit_astar->astarminxtile >= GetMapTilesX() - unit_astar->astarareaside) 
		{
			unit_astar->astarminxtile = GetMapTilesX() - unit_astar->astarareaside - 1;
		}
		
	if (unit_astar->astarminytile < 0) 
		{
			unit_astar->astarminytile = 0;
		}
	else if (unit_astar->astarminytile >= GetMapTilesY() - unit_astar->astarareaside) 
		{
			unit_astar->astarminytile = GetMapTilesY() - unit_astar->astarareaside - 1;
		}
	
	//-----------------------------------------------------------------------------------------
}

	
function		PathTile2D_SetAstarStartTargetTiles(PLAY02_TILEASTARSYS* unit_astar)
{
	//-----------------------------------------------------------------------------------------
	// start and target tiles in astar map coordinates
	
	unit_astar->astarstartx = unit_astar->startx - unit_astar->astarminxtile;
	unit_astar->astarstarty = unit_astar->starty - unit_astar->astarminytile;
	if (unit_astar->startz > 0)
		{
			unit_astar->startz--;													// convert 0-1-2 to 0-1 !!!
		}

	unit_astar->astartargetx = unit_astar->targetx - unit_astar->astarminxtile;
	unit_astar->astartargety = unit_astar->targety - unit_astar->astarminytile;
	if (unit_astar->targetz > 0)
		{
			unit_astar->targetz--;													// convert 0-1-2 to 0-1 !!!
		}	
	
	//-----------------------------------------------------------------------------------------
	
	unit_astar->astarstartabs  = PathTIle2D_GetTileAbs(unit_astar->astarstartx,  unit_astar->astarstarty,  unit_astar->startz,  unit_astar->astarareaside);
	unit_astar->astartargetabs = PathTIle2D_GetTileAbs(unit_astar->astartargetx, unit_astar->astartargety, unit_astar->targetz, unit_astar->astarareaside);
	
	if (
			(unit_astar->astarstartabs < 0)
			||
			(unit_astar->astarstartabs >= unit_astar->astararraysize)
			||
			(unit_astar->astartargetabs < 0)
			||
			(unit_astar->astartargetabs >= unit_astar->astararraysize)
		)
		{
			return 0;
		}
	
	return 1;
	
	//-----------------------------------------------------------------------------------------
}


void		PathTile2D_ResetParameters(PLAY02_UNITSTRUCT* unit_property, PLAY02_TILEASTARSYS* unit_astar, int turn_limit)
{
	//-----------------------------------------------------------------------------------------
	
sys_marker("gp1");	
		
	// unit array nulling - not needed, overwritten
//	memset( unit_property->mypathtilex , (short)65535 , (int)maxpathlength * sizeof(short) ); 			// memset okay only for one byte!!!
//	memset( unit_property->mypathtiley , (short)65535 , (int)maxpathlength * sizeof(short) ); 			// so 255 sets 65535 for a short
//	memset( unit_property->mypathtilez , (char)255 , (int)maxpathlength * sizeof(char) ); 				// ###
	
	unit_property->mypathtilestep   = 0;
	unit_property->mypathtilelength = 0;		
	
	//-----------------------------------------------------------------------------------------
	// a-star array nulling		

sys_marker("gp2");
		
	array_fillbytes( unit_astar->f,				sizeof(short),		unit_astar->astararraysize, (char)255 );				// to avoid possible smallest F issues when loop is interrupted
	array_fillbytes( unit_astar->g,				sizeof(short),		unit_astar->astararraysize, (char)0 );
	array_fillbytes( unit_astar->h,				sizeof(short),		unit_astar->astararraysize, (char)0 );
	array_fillbytes( unit_astar->parentabs,	sizeof(short),		unit_astar->astararraysize, (char)255 );
	array_fillbytes( unit_astar->oclist,		sizeof(char),		unit_astar->astararraysize, (char)0 );
	array_fillbytes( unit_astar->clearance,	sizeof(char),		unit_astar->astararraysize, (char)CL_DEFAULT );
	
	if (turn_limit > 0)
		{
			array_fillbytes( unit_astar->direction, 	sizeof(char),		unit_astar->astararraysize, (char)255 );
		}
	
	// LLL
#ifdef ASTAR_OPEN_LINKED_TILE

	array_fillbytes( unit_astar->prevopen, 	sizeof(short),		unit_astar->astararraysize, (char)255 );
	array_fillbytes( unit_astar->nextopen, 	sizeof(short),		unit_astar->astararraysize, (char)255 );
	
#endif
	
	//-----------------------------------------------------------------------------------------
}


void		PathTile2D_GenerateAstarClearanceMap(PLAY02_UNITSTRUCT* unit_property, PLAY02_TILEASTARSYS* unit_astar, char** static_clearancearray, int unit_capability, int far_lod)
{
	//-----------------------------------------------------------------------------------------
	// set clearance data within examined area, except own position, based on entity and clearance arrays
sys_marker("gp3");	
	
	// 1) units map - calculate only for close units
	
	int mygroupnum = (int)unit_property->myentity.UNIT_GROUP;
	
	if (far_lod < 1)																																		// units are far from camera, or it is set for group related pathfindings where units should be ignored
		{
			int k;
			
			for(k=0; k<rtsunitarraylength; k++)
				{
					if	((rtsunitarray[k]).myentity != NULL )																				// if myentity exists - for safetyx and was used to exclude group pseudounitstruct entity, which is now used as a flag following the boss
					if ((rtsunitarray[k]).myentity.XTILE_POS >= unit_astar->astarminxtile)										// within pathfinding "window" area
					if	((rtsunitarray[k]).myentity.YTILE_POS >= unit_astar->astarminytile)
					if	((rtsunitarray[k]).myentity.XTILE_POS < unit_astar->astarminxtile + unit_astar->astarareaside)
					if	((rtsunitarray[k]).myentity.YTILE_POS < unit_astar->astarminytile + unit_astar->astarareaside)	
					if	((rtsunitarray[k]).myentity != unit_property->myentity )														// exclude unit_property->myentity or my entity, especially required if my area is larger than 1 !
					if	(
							((rtsunitarray[k]).myentity.UNIT_GROUP != unit_property->myentity.UNIT_GROUP )						// TEST ok - don not sample own group members - okay, faster, more collision on close view				
							||																																	// or
							(rtsgrouparray[mygroupnum].ourunitpositioning % 10 > 0)															// sample if group formation reposition done
						)
//					if	((rtsunitarray[k]).mystate != (char)STATE_MOVE )																	// TEST X - do not sample moving units - faster but a bit ugly movement with a lot of collisions on close view, and can result in many overlapping units if moving in far view wothout collision									
						{
							PathTile2D_SetClearanceOfEntity_Simple(unit_astar->clearance, k, unit_astar->astarminxtile, unit_astar->astarminytile, unit_astar->astarareaside);
						}
				}
		}
	
//	printf("ownmap 03");
	
sys_marker("gp4");

	// 2) add static clearance map to units map	
	
	int i;
	int j;
	int k;
	
	int ownmap_tilecounter = 0;
	
	for( k=0 ; k<terrain_map_levels ; k++ )																							// Z		0-1
		{
			for(j=0; j<unit_astar->astarareaside; j++)																				// Y
				{
					for(i=0; i<unit_astar->astarareaside; i++)																		// X
						{
							// temp to store tilearray absolute tile value converted from xytile
							int temp = XYTileToAbsTile( unit_astar->astarminxtile + i , unit_astar->astarminytile + j );												
							
//							// test
//							(unit_astar->clearance)[ownmap_tilecounter] = (char)(int)minv( (var)(int)(unit_astar->clearance)[ownmap_tilecounter], (var)(int)(static_clearancearray[k])[temp] );	
							
							// non building
							if (!BuildingTile(temp))
								{
									if (k==0)							// enough to process ground level
										{																																	
											(unit_astar->clearance)[ownmap_tilecounter] = (char)(int)minv( (var)(int)(unit_astar->clearance)[ownmap_tilecounter], (var)(int)(static_clearancearray[k])[temp] );																						
										}
								}
							// building
							else
								{
									if (unit_capability < 3)		// the same for ground units, water units not available
										{
											(unit_astar->clearance)[ownmap_tilecounter] = (char)(int)minv( (var)(int)(unit_astar->clearance)[ownmap_tilecounter], (var)(int)(static_clearancearray[k])[temp] );								
										}
								}	
								
							++ownmap_tilecounter;
						}
				}
		}
	
//	printf("ownmap 04");
	
	//-----------------------------------------------------------------------------------------
}


void		PathTile2D_SetClearanceOfEntity_Simple(char* arraytoset , int ent_num, int lowerleftx , int lowerlefty , int xyareasize)
{
sys_marker(NULL);		
sys_marker("ss0");
	
	//----------------
	
	ENTITY* ent = (rtsunitarray[ent_num]).myentity;
	
	//----------------
	
	// entity abs tile pos
	int pos = ent.ABSTILE_POS;
	
	// lower left area corner
	int a = ent.ABSTILE_POS - integer(ent.AREA_SIZEX/2) - (integer(ent.AREA_SIZEY/2)) * GetMapTilesX();		// lower-left tile of entity area - without 1,3 clearance values !!!	
	
	// entity z level
	int b = ent.ZTILE_POS;								// 0-1-2 !!!
	
	// tile z pos (var) at entity place
	int midz = GetTileMidZ(pos, b);					// b: 0-1-2
	
	// entity level c: 0-1
	int c = b;
	if (c > 0)	
		{
			c--;
		}
	
	//----------------
	// set area tiles

sys_marker(NULL);		
sys_marker("ss1");
	
	int i;
	int j;
	int k;
	
	for (i=0 ; i<(ent.AREA_SIZEX) ; i++)
		{
			for (j=0 ; j<(ent.AREA_SIZEY) ; j++)
				{
					// abstile value converted from x-y
					int temp = a + i + j * GetMapTilesX();					//  lower left abs tile plus for i-j values										
					
					//----------------
					// convert abstile to xy tile
					
					int tempx = AbsTileToXTile(temp);
					int tempy = AbsTileToYTile(temp);
					
					//----------------
					
					// protection to astar area "window" tile coordinates (larger units can go out of it)
					if (tempx >= lowerleftx)
					if (tempx < lowerleftx + xyareasize) 
					if	(tempy >= lowerlefty)
					if (tempy < lowerlefty + xyareasize) 									
					// protection - replaced by "window" protection, which is already protected ?
//					if ( AbsTileValid(temp) ) 		
						{
							//-----------------------
							// calculate level
							
							int m = 1;								// max levels
							int nexttile_mod = 0;				// midz modifier +1 in buildings
							
							if (BuildingTile(temp))
								{
									m = terrain_map_levels;			// 2
									nexttile_mod = 1;
								}
							
							//-----------------------
							for (k=0; k<m; k++)					// 0-1
								{
									// valid cluster protection
//									if ( ClusterExists(GetTileClusterID(temp, k)) )
									if ( ClusterValid(GetTileClusterID(temp, k)) )
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
											
											// height difference, not abs, so okay for different upward and downward move values
											var height_diff = GetTileMidZ(temp, k + nexttile_mod) - midz;
																						
											// convert temp and k to astar area "window" tile coordinates tempx,tempy											
											int w_tempx = tempx - lowerleftx;
											int w_tempy = tempy - lowerlefty;											
											int w_temp  = PathTIle2D_GetTileAbs(w_tempx, w_tempy, k, xyareasize);																																	

//											// old
//											(arraytoset[k])[temp] = (char)(int)minv( value_to_set , (var)(int)(arraytoset[k])[temp] );
//											(arraytoset[k])[temp] = 0;											
											
//											// new - test - OK
//											arraytoset[w_temp] = (char)0;
											
											// new
											// can be set only if within z walkability range i.e. 1 tile (to not to affect not neighbouring tiles on other floors)
											// if any ladder
											if ( (GetTileType(temp, k+nexttile_mod) == TERR_SIEGE) || (GetTileType(pos, b) == TERR_SIEGE) )
												{
													if (
													( (height_diff >= (var)0) && (height_diff <= ((var)MAX_WALK_Z_DIFF*3*GetMapTileSize())) )		// 1.25*3 climb up/down difference
													||
													( (height_diff < (var)0) && (height_diff >= (-(var)MAX_WALK_Z_DIFF*3*GetMapTileSize())) )
													)
														{
//															(arraytoset[k])[temp] = (char)(int)minv( value_to_set , (var)(int)(arraytoset[k])[temp] );
															
															// use modified temp containing now astar area "window" tile coordinates															
//															arraytoset[w_temp] = (char)(int)minv( value_to_set , (var)(int)arraytoset[w_temp] );
															// simplified
															arraytoset[w_temp] = (char)0;														
														}
												}
											else	// non-ladder
												{
													if (
													( (height_diff >= (var)0) && (height_diff <= ((var)MAX_WALK_Z_DIFF * GetMapTileSize())) )			// 1.25 walk up difference
													||
													( (height_diff < (var)0) && (height_diff >= (-(var)MAX_WALK_Z_DIFF * 1.5 * GetMapTileSize())) )	// 1.25*1.5 walk down difference
													)
														{
//															(arraytoset[k])[temp] = (char)(int)minv( value_to_set , (var)(int)(arraytoset[k])[temp] );
															
															// use modified temp containing now astar area "window" tile coordinates															
//															arraytoset[w_temp] = (char)(int)minv( value_to_set , (var)(int)arraytoset[w_temp] );
															// simplified
															arraytoset[w_temp] = (char)0;																
														}
												}
											
										}	// valid cluster protection
								}	// k				
						}	// tile boundary protetcion
				}	// j
		}	// i

sys_marker(NULL);	

}


void		PathTile2D_InitAstarStartTie(PLAY02_UNITSTRUCT* unit_property, PLAY02_TILEASTARSYS* unit_astar, int turn_limit)
{
	//-----------------------------------------------------------------------------------------
sys_marker("gpr");
	
	// put start on open list - more exactly the whole unit area - but parents also shoul be set - can be done by a brushfire loop
//	(((unit_astar->oclist)[startz])[astarstartx])[astarstarty] = 1;			// convert to avoid out of boundary error
	(unit_astar->oclist)[unit_astar->astarstartabs] = (char)1;
	
	//-----------------------------------------------------------------------------------------
	// heuristic function - distance estimation:
	// cluster middle tiles distance - multiplied by average cost (weight)

sys_marker("gps");
	
//	// MANHATTAN value = ( x-distance + y-distance ) * weight (e.g. an average cost value)
//	(unit_astar->h)[unit_astar->astarstartabs] = (short)integer( ( abs(unit_astar->astarstartx - unit_astar->astartargetx)
//																					 + abs(unit_astar->astarstarty - unit_astar->astartargety) )
//																					 * GetMapDefaultMoveCost() );
	
//	// EUCLIDIAN value = sqrt( sqr(x-distance) + sqr(y-distance) ) 
//	(unit_astar->h)[unit_astar->astarstartabs] = (short)integer( sqrt(pow(abs(unit_astar->astarstartx - unit_astar->astartargetx), 2)
//																						 + pow(abs(unit_astar->astarstarty - unit_astar->astartargety), 2))
//																						  * GetMapDefaultMoveCost() );	
	
	// DIAGONAL SHORTCUT value : ### diagonal distance * 1.4 until fx or fy reached + the remainig orthogonal distance
	(unit_astar->h)[unit_astar->astarstartabs] = (short)(int)integer( (
																							 minv( abs(unit_astar->astarstartx - unit_astar->astartargetx) , abs(unit_astar->astarstarty - unit_astar->astartargety) )*1.4 
																							+ abs( abs(unit_astar->astarstartx - unit_astar->astartargetx) - abs(unit_astar->astarstarty - unit_astar->astartargety) )
																							) * GetMapDefaultMoveCost() );	
	
	// DIJKTSRA: h=0
//	unit_astar->h[unit_astar->astarstartx][unit_astar->astarstarty] = 0;
	
//	// horizontal modification: + or * ?
//	(unit_astar->h)[unit_astar->astarstartabs] += (short)((int)abs(unit_astar->startz - unit_astar->targetz) * GetMapClusterSize() * GetMapDefaultMoveCost());
	
	//-----------------------------------------------------------------------------------------

sys_marker("gpt");
	
	// cost of path completed (in case of depth first it remains 0)
//	(((unit_astar->g)[startz])[astarstartx])[astarstarty] = 0;
	(unit_astar->g)[unit_astar->astarstartabs] = (short)0;
	
	//-----------------------------------------------------------------------------------------
	
	// weighting function
//	(((unit_astar->f)[startz])[astarstartx])[astarstarty] = (((unit_astar->h)[startz])[astarstartx])[astarstarty] + (((unit_astar->g)[startz])[astarstartx])[astarstarty];
	(unit_astar->f)[unit_astar->astarstartabs] = (unit_astar->h)[unit_astar->astarstartabs] + (unit_astar->g)[unit_astar->astarstartabs];
	
	//-----------------------------------------------------------------------------------------
	
sys_marker("gp6");

	if (turn_limit > 0)
		{
			(unit_astar->direction)[unit_astar->astarstartabs] = (char)AngleToDirection( unit_property->myentity.pan );
		}
	
	//-----------------------------------------------------------------------------------------
	// first and last tiles of the pseudo linked list, storing processed tiles
	// (it was used in failure handling, but there it caused targeting errors)

//#ifdef ASTAR_OPEN_LINKED_TILE	
	unit_astar->firstopen = (short)unit_astar->astarstartabs;
	unit_astar->lastopen  = (short)unit_astar->astarstartabs;
//#endif	

	//-----------------------------------------------------------------------------------------
}												


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main loop


function	PathTile2D_GetTileLowestF(PLAY02_TILEASTARSYS* unit_astar, int* tempa, int* tempb, int* tempc, int* tempcurrent)
{
	// find best unprocessed tile (lowest f)	
sys_marker("gp7");						
	
	// big value needed for finding the tile having the smallest F value, converted to short so max 65535
	int a = 65535;
	
	// b is >0 if there exists an open list element
	int b = 0;
	
{}			
#ifdef ASTAR_OPEN_LINKED_TILE	
			//------------------------------------------------------		
			// set currenttile from firsttile
			// set searchtile from currenttile.nexttile
			short currenttile = unit_astar->firstopen;
			short searchtile = unit_astar->firstopen;
			
			while(searchtile != (short)65535)										// while a tile found having a default value
				{
					b++;																		// any openlist tile found
//					draw_text( "|" , 10+b , 700 , vector(100,100,100) );
				
sys_marker("gp8");
									
					if ( (unit_astar->f)[ (int)searchtile ] < (short)a )					
						{							
							// update a
							a = (unit_astar->f)[ (int)searchtile ];
							
							// tempa-b stores this array position
							*tempc = PathTIle2D_GetTileZ( (int)searchtile, unit_astar->astarareaside);
							*tempa = PathTIle2D_GetTileX( (int)searchtile, unit_astar->astarareaside);
							*tempb = PathTIle2D_GetTileY( (int)searchtile, unit_astar->astarareaside);
							
							// abs tile
							*tempcurrent = (int)searchtile;
							
							// store to current, search will change					
							currenttile = searchtile;
						}
				
sys_marker("gp9");
					
					// get next open tile to check its cost, set searchtile to it (and avoiding overwriting data by a temporary variable)
					// can be a NULL tile i.e. 255,255,255, then exit loop
					short temptile = searchtile;
					
					searchtile = (unit_astar->nextopen)[ (int)temptile ];
						
				}
			//------------------------------------------------------
#else			
			//------------------------------------------------------
			// replaced by LLL :
			
			// scan all the tiles within unit array - it could be speeded up by using linked list nodes for open tiles			
			int k;
			for(k=0; k<unit_astar->astararraysize; k++)		
				{
					// if on open list
					if ( (unit_astar->oclist)[k] == (char)1 ) 			// 0 non-examined, 1 on open list, 2 on closed list	
						{
							b++;
					
							// looking for open list element having the smallest value
							if ( (unit_astar->f)[k] < (short)a )								// was <= but uselessly
								{
									a = (unit_astar->f)[k];
									// tempa-b stores this array position
									*tempc = PathTIle2D_GetTileZ( (int)k, unit_astar->astarareaside);
									*tempa = PathTIle2D_GetTileX( (int)k, unit_astar->astarareaside);
									*tempb = PathTIle2D_GetTileY( (int)k, unit_astar->astarareaside);
									
									// abs tile
									*tempcurrent = k;
								}	
						}
				}
			//------------------------------------------------------
#endif
	
	//------------------------------------------------------
	// error: if no open list element, or if no smallest F value tile determined
	
	if ((b == (int)0) || (*tempcurrent==(int)-1)) 
		{
			return 0;
		}
	
	//------------------------------------------------------
	// set pseudo linked list 1st and last elements

#ifdef ASTAR_OPEN_LINKED_TILE		
sys_marker("gpa");

	// previous and next open tiles of current open tile, that was just moved to closed list, so they should be linked					
	short prevtile = (unit_astar->prevopen)[ *tempcurrent ];
	short nexttile = (unit_astar->nextopen)[ *tempcurrent ];
	
	// current tile has a previous tile
	if (prevtile != (short)65535)																											
		{
			// copy the current tile's next tile (can be invalid) and set as the previous open tile's next tile, because current tile is now out of open tiles
			(unit_astar->nextopen)[ (int)prevtile ] = nexttile;
		}
	// current tile does not have a previous tile
	else																											
		{
			// copy the current tile's next tile to first open tile - can be NULL
			unit_astar->firstopen = nexttile;
		}
	
	// current tile has a next tile
	if (nexttile != (short)65535)																											
		{
			// copy the current tile's previous tile (can be invalid) and set as the next open tile's previous tile, because current tile is now out of open tiles
			(unit_astar->prevopen)[ (int)nexttile ] = prevtile;
		}
	// current tile does not have a next tile
	else																											
		{
			// copy the current tile's previous tile to last open tile
			unit_astar->lastopen = prevtile;
		}	
#endif
	
	//------------------------------------------------------
	
	return 1;
}


void		PathTile2D_SetExaminedTile(PLAY02_TILEASTARSYS* unit_astar, int i, int tempa, int tempb, int* tempx, int* tempy, var* multiplier1)
{
	// S
	if (i == (int)6)
	{
		*tempx = tempa ;
		*tempy = tempb -1;
		*multiplier1 = 1;
	}
	// E
	else if (i == (int)0)
	{
		*tempx = tempa +1;
		*tempy = tempb ;
		*multiplier1 = 1;
	}
	// N
	else if (i == (int)2)
	{
		*tempx = tempa ;
		*tempy = tempb +1;
		*multiplier1 = 1;
	}
	// W
	else if (i == (int)4)
	{
		*tempx = tempa -1;
		*tempy = tempb ;
		*multiplier1 = 1;
	}			
	// SW
	else if (i == (int)5)
	{
		*tempx = tempa -1;
		*tempy = tempb -1;
		*multiplier1 = 1.4;
	}
	// SE
	else if (i == (int)7)
	{
		*tempx = tempa +1;
		*tempy = tempb -1;
		*multiplier1 = 1.4;
	}
	// NE
	else if (i == (int)1)
	{
		*tempx = tempa +1;
		*tempy = tempb +1;
		*multiplier1 = 1.4;
	}
	// NW
	else if (i == (int)3)
	{
		*tempx = tempa -1;
		*tempy = tempb +1;
		*multiplier1 = 1.4;
	}		
}


function		PathTile2D_SetBuildingFlag(PLAY02_TILEASTARSYS* unit_astar, int tempa, int tempb, int tempc, int tempx, int tempy, int* tempz)
{
	//------------------------------------------------------
	
	// just examined tile converted to world abstile
	int temp = XYTileToAbsTile( tempx + unit_astar->astarminxtile , tempy + unit_astar->astarminytile );
	
	// current tile converted to world abstile
	int temp_prev = XYTileToAbsTile( tempa + unit_astar->astarminxtile , tempb + unit_astar->astarminytile );
								
	//------------------------------------------------------
	// building check
	
	// additional values to tile.midz in case of buildings, when valid cluster exists
	int currtile_mod = 0;
	int nexttile_mod = 0;
	
	// actual tile :
	if (BuildingTile(temp_prev))		
		{
			currtile_mod = 1; 																	// level modifier - if building, add 1 to midz
		}
	
	// neighbour tile :
	if (BuildingTile(temp)) 																	// && ClusterExists(GetTileClusterID(temp, k))	
		{
			nexttile_mod = 1;																		// level modifier - if building, add 1 to midz
		}	
		
	// if any of the 2 tiles is building - requires later check which is building, which is not
	if (
			(nexttile_mod > 0) 
			|| 
			(currtile_mod > 0)
		)
		{
			int k;										
			// 0 or 1 level, is 1 2 midz if building
			for (k=0; k<terrain_map_levels; ++k)																												// passability of any building level (1 or 2) from neighbouring ground tile
				{													
					// neighbour is a valid level, double check?
					if (GetTileMidZ(temp, k+nexttile_mod) != (var)NO_Z_APPLIED)
						{
							// height difference, not abs, so okay for different upward and downward move values
							var height_diff = GetTileMidZ(temp, k+nexttile_mod) - GetTileMidZ(temp_prev, tempc+currtile_mod);														
								
							// if any ladder
							if ( (GetTileType(temp, k+nexttile_mod) == TERR_SIEGE) || (GetTileType(temp_prev, tempc+currtile_mod) == TERR_SIEGE) )
								{
									if (
											( (height_diff>=(var)0) && (height_diff<=((var)MAX_WALK_Z_DIFF*3*GetMapTileSize())) )				// 1.25*3*tile climb difference
											||
											( (height_diff<(var)0) && (height_diff>=(-(var)MAX_WALK_Z_DIFF*3*GetMapTileSize())) )
										)
										{
											*tempz = k;																													// if heigh diff okay, set neighbour tile z to loop value
											return 1;
										}
								}
							else	// non-ladder
								{
									if (
											( (height_diff>=(var)0) && (height_diff<=((var)MAX_WALK_Z_DIFF * GetMapTileSize())) )				// 1.25*tile = normal walk difference
											||
											( (height_diff<(var)0) && (height_diff>=(-(var)MAX_WALK_Z_DIFF*1.5 * GetMapTileSize())) )			// 1,25*1.5*tile normal walk difference
										)
										{
											*tempz = k;																													// if heigh diff okay, set neighbour tile z to loop value
											return 1;
										}
								}
						}
					
				}		// for k									
		}
	else	// ### if none of the 2 tiles is building
		{
			return 1;
		}	
	
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main loop error handling


void		PathTile2D_BestTargetApproximation(PLAY02_TILEASTARSYS* unit_astar, var* error1, var* found)
{
	// error handling by target approximation - essential !!!
	
sys_marker("gpd");
	
	if ((*error1 > 0) || (*found < 1))										// finding a tile closest to target and setting as target  NOT OPTIMIZED FOR LLL !!!
		{
			// big value needed for finding the tile having the smallest F value, converted to a short so max 65535
			int a = 65535;
			
			// b is >0 if there exists an open list element
			int b = 0;
			
			// tempa and tempb show the tile having the smallest F value
			int tempa = -1;		
			int tempb = -1;
			int tempc = -1;
			
			int tempcurrent = -1;												// %%%%						
			
			//LLL---------------------------------------------------------------------
			// unfortunately LLL works badly here (examines only open list) - resulting in far targets and ugly, separating unit moves
//{}			
//#ifdef ASTAR_OPEN_LINKED_TILE			
//			
//			// set searchtile from currenttile.nexttile
//			short searchtile = unit_astar->firstopen;
//			
//			while(searchtile < (short)65535)								// while a tile found having a default value
//				{
//					b++;	// any openlist tile found
////				draw_text( "|" , 10+b , 700 , vector(100,100,100) );
//									
//					if ( (unit_astar->f)[ (int)searchtile ] < (short)a )					
//						{							
//							// update a
//							a = (unit_astar->f)[ (int)searchtile ];
//							
//							// tempabc stores astar tile sxy
//							tempc = PathTIle2D_GetTileZ( (int)searchtile, unit_astar->astarareaside);
//							tempa = PathTIle2D_GetTileX( (int)searchtile, unit_astar->astarareaside);
//							tempb = PathTIle2D_GetTileY( (int)searchtile, unit_astar->astarareaside);
//							// astar abs tile
//							tempcurrent = (int)searchtile;
//						}
//					
//					// get next open tile to check its cost, set searchtile to it (and avoiding overwriting data by a temporary variable)
//					// can be a NULL tile i.e. 255,255,255, then exit loop
//					short temptile = searchtile;
//					
//					searchtile = (unit_astar->nextopen)[ (int)temptile ];						
//				}
//#else			
			// replaced by LLL :
			// slower but runs only once, more stable than LLL !!!
			int k;
			for(k=0; k<unit_astar->astararraysize; ++k)		
				{
					// if on open or closed list
					if ( (unit_astar->oclist)[k] > (char)0 ) 												// 0 non-examined, 1 on open list, 2 on close3d list	
						{
							b++;
					
							// looking for open list element having the smallest F value
							if ( (unit_astar->f)[k] < (short)a )							
								{
									a = (unit_astar->f)[k];
									// tempabc stores this array position
									tempc = PathTIle2D_GetTileZ( (int)k, unit_astar->astarareaside);
									tempa = PathTIle2D_GetTileX( (int)k, unit_astar->astarareaside);
									tempb = PathTIle2D_GetTileY( (int)k, unit_astar->astarareaside);
									// abs tile
									tempcurrent = k;
								}	
						}
				}
//#endif			
			// now tile [tempa][tempb] holds the best position
			
			// new target found
			if (b > 0)
			if (tempa > -1)
			if (tempb > -1)
				{
					unit_astar->astartargetx = tempa;									// overwrite original astar target
					unit_astar->astartargety = tempb;
					
					unit_astar->targetx = tempa + unit_astar->astarminxtile;		// overwrite original astar target
					unit_astar->targety = tempb + unit_astar->astarminytile;	
					unit_astar->targetz = tempc;											// ### 0-1									
					
					// NO !!! BETTER IF THE ORIGINAL TARGET IS KEPT
					// MUCH BETTER ERROR HANDLING: BETTER COHESION, GOT NO STUCK !!!!
					//
//					// overwrite original target too (???) can cause a unit got stuck - for arrival chack path-length check is used
//					unit_property->mytargettilex = (short)targetx;
//					unit_property->mytargettiley = (short)targety;
////					unit_property->mytargettilez = (char)targetz;		// wrong z conversion! - 0-1 too 0-1-2 conversion requires tilebuildingid check!!!
//					
//					int abstile = XYTileToAbsTile(targetx,targety);
//					if (BuildingTile(abstile))
//						{
//							unit_property->mytargettilez = (char)(targetz + 1);
//						}
//					else
//						{
//							unit_property->mytargettilez = (char)targetz;
//						}
					
					*error1 = 0;
					*found = 1;
				}
		}
}


function	PathTile2D_UnresolvableError(PLAY02_UNITSTRUCT* unit_property, var error1, int debug_path, int debug_area, int cycle_time)
{
	// error and not found) - message
	if (error1 > 0) 					// should be || for common error handling and finding a cluster closest to target and set as target !
		{
{}
#ifdef DEBUG_UNITPATH			
			if (debug_path > 0)
				{
					draw_text( "unresolvable tile path error !" , 400 , 450 , vector(0,0,255) );
					// cycles
					draw_text( "cycles:" , 400 , 475 , vector(0,0,255) );
					draw_text( str_for_int(NULL, cycle_time) , 500 , 475 , vector(0,0,255) );
				}
#endif
			PathTile2D_ResetPath(unit_property);
			return 1;
		}
	
	return 0;	
}


function	PathTile2D_PathNotFound(PLAY02_UNITSTRUCT* unit_property, var found, int debug_path, int debug_area, int cycle_time)
{
	// not found - message 
	if (found < 1)
		{
{}
#ifdef DEBUG_UNITPATH					
			if (debug_path > 0)
				{
					draw_text( "tile path not found !" , 400 , 450 , vector(0,255,255) );
					// cycles
					draw_text( "cycles:" , 400 , 475 , vector(0,255,255) );
					draw_text( str_for_int(NULL, cycle_time) , 500 , 475 , vector(0,255,255) );
				}
#endif
			PathTile2D_ResetPath(unit_property);
			return 1;	
		}
	
	return 0;
}


void		PathTile2D_ResetPath(PLAY02_UNITSTRUCT* unit_property)
{
	unit_property->mypathtilestep 	= (char)0;
	unit_property->mypathtilelength 	= (char)0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// create path


function		PathTile2D_CreatePath(PLAY02_UNITSTRUCT* unit_property, PLAY02_TILEASTARSYS* unit_astar, int maxpathlength, int debug_path, int debug_area, int* pathlength, int* counter)
{
sys_marker("gpf");	
	
	var failure = 0;
	
	int counterm;
	
	// temporarily the first element of path-array is the target node		
	(unit_property->mypathtilex)[*counter] = (short)unit_astar->targetx;												// stored as int
	(unit_property->mypathtiley)[*counter] = (short)unit_astar->targety; 
	(unit_property->mypathtilez)[*counter] = (char)unit_astar->targetz; 												// level2 0-1
	
sys_marker("gpg");

	// looking for start node backward from target 
	while (
				((unit_property->mypathtilex)[*counter] != (short)unit_astar->startx) 
				|| 
				((unit_property->mypathtiley)[*counter] != (short)unit_astar->starty)
				||
				((unit_property->mypathtilez)[*counter] != (char)unit_astar->startz)									// level2 0-1
			) 
		{
sys_marker("gph");	
					
			// counter now points at the current element
			*counter++;
			counterm = *counter - 1;					
			
			if (*counter > (maxpathlength-1))
				{
{}
#ifdef DEBUG_UNITPATH
					if (debug_path > 0)
						{
							draw_text( "tile path length exceeded !" , 400 , 450 , vector(255,255,255) );
						}
#endif					
					failure = 1;
					break;																												// ??? it can cause a BUG
				}
			
			// pointing at the previous elment's position
			int tempx = (unit_property->mypathtilex)[counterm] ;
			int tempy = (unit_property->mypathtiley)[counterm] ;
			int tempz = (unit_property->mypathtilez)[counterm] ;															// ### later, when astar struct okay			
			
sys_marker("gpi");
	
			// astar array is relative array to minx miny tile calculated in the beginning 
			int tempa = tempx - unit_astar->astarminxtile;
			int tempb = tempy - unit_astar->astarminytile;
			int tempc = tempz;
			
//			if (tempc>0) tempc--;																									// convert 0-1-2 to 0-1 ?????? it is already in 0-1 system
			
			int tempcurrent = PathTIle2D_GetTileAbs(tempa, tempb, tempc, unit_astar->astarareaside);			// in astar abs tile coord						
{}
#ifdef DEBUG_UNITPATH						
			if (debug_path > 0)
				{
					// safety check
					if ((tempcurrent >= unit_astar->astararraysize) || (tempcurrent < 0))
						{
							draw_text( "bad tile parent !" , 400 , 450 , vector(255,255,255) );
							failure = 1;
							break;																										// ??? it can cause a BUG
						}
				 }
#else
			// safety check
			if ((tempcurrent >= unit_astar->astararraysize) || (tempcurrent < 0))
				{
					failure = 1;
					break;																												// ??? it can cause a BUG
				}
#endif			
			
sys_marker("gpj");
			if ( 
					((unit_astar->parentabs)[tempcurrent] >= (short)0)
					&&																														// was ||
					((unit_astar->parentabs)[tempcurrent] < (short)unit_astar->astararraysize)   					// RARE E1513 ERROR - if not checked properly !
				)
				{
sys_marker("gpk");
						
					// the new element is the parent of the previous element, convert xy from local to global map					
					(unit_property->mypathtilex)[*counter] = PathTIle2D_GetTileX( (unit_astar->parentabs)[ tempcurrent ], unit_astar->astarareaside ) + unit_astar->astarminxtile;
					(unit_property->mypathtiley)[*counter] = PathTIle2D_GetTileY( (unit_astar->parentabs)[ tempcurrent ], unit_astar->astarareaside ) + unit_astar->astarminytile;
					(unit_property->mypathtilez)[*counter] = PathTIle2D_GetTileZ( (unit_astar->parentabs)[ tempcurrent ], unit_astar->astarareaside ) ;										// 0-1
					
					// set continuously to minimize error consequences
					*pathlength 							= *counter +1;	
					unit_property->mypathtilestep 	= 1;																		// 0 is not good for periodic path updates, it shows the next tile to achieve, the previous is the current !!!
					unit_property->mypathtilelength 	= *pathlength;								
				}				
			else
				{
{}
#ifdef DEBUG_UNITPATH
				if (debug_path > 0)
					{	
						draw_text( "parent exceeds tile array length !" , 400 , 450 , vector(255,255,255) );
					}
#endif					
					failure = 1;
					break;																												// ??? it can cause a BUG
				}	
		}
	
	if (failure)
		{
			PathTile2D_ResetPath(unit_property);
			
			return 1;
		}
	
	return 0;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// path error handling


function	PathTile2D_TooShortPath(PLAY02_UNITSTRUCT* unit_property, int counter, int debug_path, int debug_area, int cycle_time)
{
	// unresolvable error - too short path		
	
	if (counter < 1)	
		{
{}
#ifdef DEBUG_UNITPATH			
			if (debug_path > 0)
				{
					draw_text( "too short tile path!" , 400 , 450 , vector(255,255,0) );
					// cycles
					draw_text( "cycles:" , 400 , 475 , vector(255,255,0) );
					draw_text( str_for_int(NULL, cycle_time) , 500 , 475 , vector(255,255,0) );
				}
#endif				
			PathTile2D_ResetPath(unit_property);
			return 1;
		}
	
	return 0;
}


int		PathTile2D_LimitPathLength(PLAY02_UNITSTRUCT* unit_property, int maxpathlength, int counter, int debug_path, int debug_area, int cycle_time)
{
	// error - too long path - probably never can happen
	
	if (counter >= (maxpathlength - 1))
		{
{}
#ifdef DEBUG_UNITPATH			
			if (debug_path > 0)
				{
					draw_text( "too long tile path!" , 400 , 450 , vector(255,255,0) );
					// cycles
					draw_text( "cycles:" , 400 , 475 , vector(255,255,0) );
					draw_text( str_for_int(NULL, cycle_time) , 500 , 475 , vector(255,255,0) );
				}
#endif					
//			pathlength = maxpathlength;
			unit_property->mypathtilelength 	= maxpathlength;
		}
	
	return unit_property->mypathtilelength;		// remains the same as pathlength, if path is not too long
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// finalize path


void		PathTile2D_FinalizePath(PLAY02_UNITSTRUCT* unit_property, int pathlength)
{
	// finalizing the path - setting correct order : 0th is start, last is target
sys_marker("gpo");
	
	int i;
	for(i=0; i<integer(pathlength/2); ++i)               
		{
			int pathlength_minus1_minusi = pathlength - 1 - i;
			
			// last element to temp
			short tempx = (unit_property->mypathtilex)[pathlength_minus1_minusi] ;
			short tempy = (unit_property->mypathtiley)[pathlength_minus1_minusi] ;
			char tempz = (unit_property->mypathtilez)[pathlength_minus1_minusi] ;
			
			// first element to last
			(unit_property->mypathtilex)[pathlength_minus1_minusi] = (unit_property->mypathtilex)[i];
			(unit_property->mypathtiley)[pathlength_minus1_minusi] = (unit_property->mypathtiley)[i];
			(unit_property->mypathtilez)[pathlength_minus1_minusi] = (unit_property->mypathtilez)[i];
			
			// temp element to first
			(unit_property->mypathtilex)[i] = tempx;
			(unit_property->mypathtiley)[i] = tempy;	
			(unit_property->mypathtilez)[i] = tempz;						
		}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// debugging


void		PathTile2D_DebugUnitPath(PLAY02_UNITSTRUCT* unit_property, int debug_path, int debug_area)
{
{}
#ifdef DEBUG_UNITPATH
sys_marker("gpp");																
	
	// representing the path of selected unit or selected group's pseudounit
		
	if (debug_path == 0)	return;
		
	// draw lines from start to target : 0 to i 
	VECTOR vTemp;
	COLOR cTemp;	

	vec_set(cTemp, vector(100,255,100));									// green
	
	var random_offset = random(4);
	
//	draw_text( "found" , 400 , 450 , vector(0,255,0) );				// not needed, only errors are displayed
	
	// draw path
	
	// intermediate lines - not needed in this case
	draw_line3d(nullvector,NULL,100);
	draw_line3d(vector(1,1,1),cTemp,0);
	draw_line3d(nullvector,NULL,100);
	
	int j;
	for(j=0; j<unit_property->mypathtilelength; ++j)
		{
			//----------------------------------------
			
			// given: x,y tile -> abstile -> maxtileheight
			int ax = (int)(unit_property->mypathtilex)[j];
			int ay = (int)(unit_property->mypathtiley)[j];
			int az = (int)(unit_property->mypathtilez)[j];										// ### should be this, 0..1
			
			// path x&y tile to coord
			int tempx = XTileToCoord(ax);
			int tempy = YTileToCoord(ay);
			
			// x&y tile to absolute tile
			int tempb = XYTileToAbsTile(ax, ay);
			
			if (BuildingTile(tempb))
				{	
					az++;																						// for drawing a 3 level system is used, for search a 2 level
				}
			
			int tempz = GetTileMidZ(tempb, az) + GetMapTileSize() + random_offset; 		// ### should be something like this - if az correct!!!
			
			//----------------------------------------
			// path by lines of all units in selected group
			
			vec_set(vTemp,vector(tempx,tempy,tempz));												// set to tile position
			
			if (j == (int)0)																				// from nullvector to target cluster middle
				{
					draw_line3d(vTemp,NULL,100);														// move pen to position
				}
			else
				{
					draw_line3d(vTemp,cTemp,100); 													// draw line
				}
			
			//----------------------------------------
			// path by dots of selected unit
			
			if (debug_area > 0)
				{
					tempz = GetTileMidZ(tempb, az) + 7.5;											// set above oclist dots
					vec_set(vTemp, vector(tempx,tempy,tempz));
					PathTile2D_DebugDrawPoint(COLOR_WHITE, vTemp);
				}
			
			//----------------------------------------	
		}						

#endif
}


void		PathTile2D_DebugUnitArea(PLAY02_TILEASTARSYS* unit_astar, int debug_path, int debug_area)
{
{}
#ifdef DEBUG_UNITAREA	
sys_marker("gpq");
	
	// should pass the data array and show within a while loop until updated again...
	
	if (debug_area > 0)																									// only of selected unit !!!
		{
			int k;
			for(k = 0; k < unit_astar->astararraysize; ++k)														// 0-1 : building 2nd floor is not fine !!! but bug is somewhere earlier, this part is fine
				{
					// tempa-b-c stores the x-y-z position in astar area tile coordinates					
//					int ztile = 0;
					int ztile = PathTIle2D_GetTileZ( (int)k, unit_astar->astarareaside);
					int xtile = PathTIle2D_GetTileX( (int)k, unit_astar->astarareaside);
					int ytile = PathTIle2D_GetTileY( (int)k, unit_astar->astarareaside);			
					
					// convert to world tile coordinates
					xtile += unit_astar->astarminxtile;
					ytile += unit_astar->astarminytile;
					int abstile = XYTileToAbsTile(xtile, ytile);
					
					int level_modifier = 0;
					if (BuildingTile(abstile)) 
						{
							level_modifier = 1;
						}
					
					// world coordinates
					VECTOR coord;
					vec_zero(coord);
					coord.x = XTileToCoord(xtile);
					coord.y = YTileToCoord(ytile);
					coord.z = GetTileMidZ(abstile, ztile + level_modifier) + 2.5;							// set nearly to ground
						
#ifdef DEBUG_OWNMAP
	
					if ( (unit_astar->clearance)[k] == (char)0 ) 												// 0 impassable, 1 , 3 ...	
						{
							PathTile2D_DebugDrawPoint(COLOR_RED, coord);									
						}
					else 
					if ( (unit_astar->clearance)[k] == (char)1 ) 												// 0 impassable, 1 , 3 ...	
						{
							PathTile2D_DebugDrawPoint(COLOR_BLUE, coord);									
						}
					else 
					if ( (unit_astar->clearance)[k] == (char)3 ) 										
						{
							PathTile2D_DebugDrawPoint(COLOR_GREEN, coord);
						}
					else
						{
							PathTile2D_DebugDrawPoint(COLOR_BLACK, coord);										// too much dots - slow - but shows whole area fine ***
						}		
	
#else			
		
					if ( (unit_astar->oclist)[k] == (char)1 ) 													// 0 non-examined, 1 on open list, 2 on closed list	
						{
							PathTile2D_DebugDrawPoint(COLOR_BLUE, coord);									
						}
					else 
					if ( (unit_astar->oclist)[k] == (char)2 ) 												
						{
							PathTile2D_DebugDrawPoint(COLOR_RED, coord);
						}
					else
						{
							PathTile2D_DebugDrawPoint(COLOR_BLACK, coord);										// too much dots - slow - but shows whole area fine ***
						}	

#endif			
				}
		}
#endif
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#endif								