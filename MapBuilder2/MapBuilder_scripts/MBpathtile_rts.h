
#ifndef MBpathtile_rts_h
#define MBpathtile_rts_h

///////////////////////////////////////////////////////////////////
// defines

// moved to MBdefines_rts.h

///////////////////////////////////////////////////////////////
// variables

// A* pathfinder data holder struct for tiles
typedef struct													// main A* struct   
{
	//---
	
	int		startx;											// startx pos in world tile map coordinates
	int		starty;											// starty pos in world tile map coordinates
	int		startz;											// startz pos in world tile map coordinates converted to 0-1 level2
	
	int		targetx;											// targetx pos in world tile map coordinates
	int		targety;											// targety pos in world tile map coordinates
	int		targetz;											// targetz pos in world tile map coordinates converted 0-1 level2
	
	//---
	
	int		astarareaside;									// width and height
	int		astararraysize;								// width * height * levels
	
	int		astarminxtile;									// astar "window map" 0,0 tilex pos in world tile map coordinates
	int		astarminytile;									// astar "window map" 0,0 tiley pos in world tile map coordinates
	
	int		astarstartx;									// startx pos in astar tile map coordinates
	int		astarstarty;									// starty pos in astar tile map coordinates
	
	int		astartargetx;									// targetx pos in astar tile map coordinates
	int		astartargety;									// targety pos in astar tile map coordinates
	
	int		astarstartabs;									// start pos in astar tile map coordinates
	int		astartargetabs;								// target pos in astar tile map coordinates
	
	//---
	
	short		firstopen;										// pseudo linked list 1st element, start tile on main loop start
	short		lastopen;										// pseudo linked list last element, start tile on main loop start
	
	//---
	
	short*	f;													// for all length: [0..PLAY02_ASTARTILEMAX] , [0..PLAY02_ASTARTILEMAX] . [0..levels]
	short*	g;			
	short*	h;													// ### for all: plus a * for the one new dimension [0..1] in case of more levels map ())
	
	short*	parentabs;										// ### (((level)dimx)dimy)   maxarea is 255x255
	
	char*		oclist;											// n,o,c : 0,1,2
	
	char*		clearance;										// 0,1,3(,5),100 for ground , 0,1,3,5,7,9,11,13,15,17,100 for water
	
	char*		direction;										// VEHICLE : angle/45 directions : 0..7 needed for vehicle like movement to store direction to limit turning
																	// - can be used together with speed determination - i.e. apparent acceleration/deceleration on moving, probably in unit struct
																	// and useful for "group path" where it is needed for FORMATION offset recalculations
				
	short*	prevopen;										// previous open tiles are stored as a pseudo linked list		
	short*	nextopen;										// next open tiles are stored as a pseudo linked list
		
} PLAY02_TILEASTARSYS;			

PLAY02_TILEASTARSYS* pathtile2d_astararray = NULL;	// memory size: 11 bytes *(sizex)23*(sizey)23 *(units)1200 = 6.66 MB per level -> 13.3 MB 2lev -> 20 MB 3lev -> only 11,6 kB now !!!

int pathtile2d_astararraylength = 1;					// one struct can be used by all units as they are processes one by one, no pseudo parallel operations

///////////////////////////////////////////////////////////////////
// functions

//---------------------------------------------------------------------------------------------------------------------------------------
// init / close

void	PathTile2D_Init();
void	PathTile2D_Free();

//---------------------------------------------------------------------------------------------------------------------------------------
// conversion

// index = (x + y*xsize) + (xsize*ysize)*level;
// z = integer(index/(xsize*ysize));			
// x = index%xsize;		
// y = integer(index/xsize);

int	PathTIle2D_GetTileAbsAbs(int abstile, int ztile, int xsize);								// multilevel abstile from base level abstile and z level, 	area size needed			not used
int	PathTIle2D_GetTileAbs(int xtile, int ytile, int ztile, int xsize);						// multilevel abstile from x-y ile and z level, 				area size needed
int	PathTIle2D_GetTileX(int abstile, int xsize);														// x tile from multilevel abstile
int	PathTIle2D_GetTileY(int abstile, int xsize);														// y tile from multilevel abstile
int	PathTIle2D_GetTileZ(int abstile, int xsize);														// z tile from multilevel abstile, 0-1

void	PathTile2D_DebugDrawPoint(COLOR* col, VECTOR* coord);											// debug decal

//---------------------------------------------------------------------------------------------------------------------------------------
// pathfinder

// Simplified A* : Depth First (no G value used) - easy to extend to A*
// get path for the given unit + create path array - using an own small area clearance map 
int		PathTile2D_GetPath(
										PLAY02_UNITSTRUCT* unit_property,									// unit struct
										PLAY02_TILEASTARSYS* unit_astar,										// an A* struct to be used, normally only one is used for all the units, data is not kept after path search
										int unit_capability,														// capability 1-4, defines which static capability map to be used
										char** static_clearancearray, 										// static capability map to be used based on unit type
										int unit_size,																// x=y tile size 1,3,5... in practice larger than 1
										int maxpathlength,														// length of path array in unit struct
										int maxpathloop,															// search loop limit (the lower the better performance, but less quality)
										int debug_path,															// 0 no debug, 1 show basic debug info of selected unit
										int debug_area,															// 0 no debug, 1 show debug path (requires DEBUG_UNITPATH)
										int far_lod,																// do not calculate unit clearance as they won't be collided
										int turn_limit																// 0 no limit, 1 45°limit, 2 90° limit(, 3 135°limit)
										);																				// returns 0 on failure, 1 on success

//---------------------------------------------------------------------------------------------------------------------------------------
// sub functions

// initialization:

void		PathTile2D_LimitStartTargetTileToGameArea(
																		PLAY02_UNITSTRUCT* unit_property
																		);												// clamp start and target tiles to be within search area

void		PathTile2D_StoreStartTargetTilePos(
															PLAY02_UNITSTRUCT* unit_property, 
															PLAY02_TILEASTARSYS* unit_astar
															);															// set startxyz and targetxyz in world tile coordinates

void		PathTile2D_LimitTargetTileByDistance(
																PLAY02_UNITSTRUCT* unit_property, 
																PLAY02_TILEASTARSYS* unit_astar
																);														// clamp target tile xy to be within a certain distance without overwriting original target !

void		PathTile2D_SetAstarMinXYTile(
													PLAY02_TILEASTARSYS* unit_astar
													);																	// conversion from global tile map to astar "window" tile map

function	PathTile2D_SetAstarStartTargetTiles(
																PLAY02_TILEASTARSYS* unit_astar
																);	

void		PathTile2D_ResetParameters(
													PLAY02_UNITSTRUCT* unit_property,
													PLAY02_TILEASTARSYS* unit_astar,
													int turn_limit
													);																	// reset parameters and array to default values

void		PathTile2D_GenerateAstarClearanceMap(
																PLAY02_UNITSTRUCT* unit_property,
																PLAY02_TILEASTARSYS* unit_astar,
																char** static_clearancearray, 
																int unit_capability, 
																int far_lod
																);														// set 
	
	void		PathTile2D_SetClearanceOfEntity_Simple(	
																		char* arraytoset, 						// out array
																		int ent_num, 
																		int lowerleftx, 
																		int lowerlefty, 
																		int xyareasize
																		);												// sets clearance area of an entity, based on its x-y tile area size stored in skills
																														// simplified, i.e. only blocking tiles, no 1 and 3 border !	-	char* only !!!

void		PathTile2D_InitAstarStartTie(
													PLAY02_UNITSTRUCT* unit_property,
													PLAY02_TILEASTARSYS* unit_astar,
													int turn_limit
													);

// main loop:

function	PathTile2D_GetTileLowestF(
													PLAY02_TILEASTARSYS* unit_astar,
													int* tempa,
													int* tempb,
													int* tempc,
													int* tempcurrent
													);																	// choose the lowest F-valued neighbouring tile

void		PathTile2D_SetExaminedTile(
													PLAY02_TILEASTARSYS* unit_astar,
													int i,
													int tempa, 
													int tempb,
													int* tempx,
													int* tempy,
													var* multiplier1
													);																	// set neighbouring tile of best F-valued tile to check

function	PathTile2D_SetBuildingFlag(
													PLAY02_TILEASTARSYS* unit_astar,
													int tempa, 
													int tempb,
													int tempc,
													int tempx,
													int tempy,
													int* tempz
													);																	// ground or building check, building floor distance check

// main loop error handling:

void		PathTile2D_BestTargetApproximation(
															PLAY02_TILEASTARSYS* unit_astar,
															var* error1,
															var* found
															);															// if target not found, find best F-valued tile among open/closed list

function	PathTile2D_UnresolvableError(
													PLAY02_UNITSTRUCT* unit_property,
													var error1,
													int debug_path,
													int debug_area,
													int cycle_time
													);																	// unknown error
function	PathTile2D_PathNotFound(
												PLAY02_UNITSTRUCT* unit_property,
												var found,
												int debug_path,
												int debug_area,
												int cycle_time
												);																		// target is not found neither by main loop nor by target approximation

	void		PathTile2D_ResetPath(
												PLAY02_UNITSTRUCT* unit_property
												);																		// zero path length and step

// create path:

function	PathTile2D_CreatePath(
											PLAY02_UNITSTRUCT* unit_property,
											PLAY02_TILEASTARSYS* unit_astar,
											int maxpathlength,
											int debug_path,
											int debug_area,
											int* pathlength,
											int* counter
											);

// path error handling:

function	PathTile2D_TooShortPath(
												PLAY02_UNITSTRUCT* unit_property,
												int counter,
												int debug_path,
												int debug_area,
												int cycle_time
												);	

int		PathTile2D_LimitPathLength(
													PLAY02_UNITSTRUCT* unit_property,
													int maxpathlength,
													int counter,
													int debug_path,
													int debug_area,
													int cycle_time
													);

// finalize path:

void		PathTile2D_FinalizePath(
												PLAY02_UNITSTRUCT* unit_property, 
												int pathlength
												);																		// finalizing the path - setting correct order : 0th is start, last is target

// debugging:

void		PathTile2D_DebugUnitPath(
												PLAY02_UNITSTRUCT* unit_property, 
												int debug_path,
												int debug_area
												);																		// error around map edge ???

void		PathTile2D_DebugUnitArea(
												PLAY02_TILEASTARSYS* unit_astar, 
												int debug_path,
												int debug_area
												);

//---------------------------------------------------------------------------------------------------------------------------------------


#endif