
#ifndef MBterrain_rts_h
#define MBterrain_rts_h


//////////////////////////////////////////////////////////////////////////////////////////
// defines

#define BUILDING_USECSV															// OK - only .csv files should be made for all buildings!

//#define BUILDING_DEBUG														// to run building entity events (in-game selection)
#define BUILDING_MAXCLUSTERS		6

//////////////////////////////////////////////////////////////////////////
// variables

//////////////////////////////////////////////////////////////
// stationary and dynamic terrain data holders (arrays)
// handle arrays are needed for making later custom small area clearance maps, not used for full map sized clearance maps

int		terrain_map_levels		 				= BUILDING_LEVELS;	// ### levels used by the game level, 0 no buildings, 1 one level blds, 2 two level blds
																						// can be lower than in static terrain data for faster operation, but cannot be higher!
																						// a bit useless, better to use map_levels ?

// handle arrays: 																// stationary, only for fast looping through

//var*	terrain_shrub_handlearray 				= NULL;					// stationary: colliding shrub entities XXX
var*		terrain_obstacle_handlearray 			= NULL;					// stationary: obstacle or mapent simple objects
var*		terrain_building_handlearray		 	= NULL;					// stationary: mapent or obstacle buildings objects ###

// clearance arrays:

//char*	terrain_shrub_clearancearray 			= NULL;					// stationary: ground level only XXX
char*		terrain_obstacle_clearancearray 		= NULL;					// stationary: ground level only - but in theory it could easily be multi-level

char**	terrain_building_clearancearray		= NULL;					// stationary: [2] 0. for ground/level1 , 1. for level2
char** 	terrain_unit_clearancearray			= NULL; 					// dynamic :### plus [0..1] if 2 level map >>> ((level)tile)

char*** 	terrain_static_clearancearray	 		= NULL; 					// stationary: 4 maps WITHOUT UNITS, one for each capability, ### plus [0..1] if 2 level map >>> (((capability)level2)tile)
char*** 	terrain_summed_clearancearray	 		= NULL; 					// dynamic: only for debugging: static map + units map, one for each capability, ### plus [0..1] if 2 level map >>> (((capability)level)tile)

// other game arrays:

char*		terrain_group_clusterarray				= NULL;					// shows whether there is no or one or two or any group(s) in the given cluster - faster than handling ClusterArray

// other non-game related:

ENTITY** terrain_shadowentity_array 			= NULL;					// to switch on/off SHADOW flag based on camera distance for faster rendering
var		terrain_shadowentity_counter 			= 0;						// number of shadow entities = array length
var		terrain_shadow_end						= 5000;					// disnance of SHADOW flag switch off (sky clipping happens at 15000/sqrt(2))

STRING*	terrain_progress_message				= NULL;					// for tracking terrain system processing state and length

/////////////////////////////////////////////////////////////////////////
// functions

	// RTS MAP INITIALIZATION
	
// base map - default tiles and clusters
void		Terrain_StaticInit();																									// allocate static terrain system (tile and cluster system) and set to defaults
	void 		Terrain_StaticFree();																								// remove and free static terrain system data		
	
// static map - height and slope tile analysis, cluster subdivision
	void		Terrain_StaticCreate();																								// terrain analysis - measure tile height and slope, then set tile and cluster properties to real values
	
	//------------------------

// statironary map initialization + units clearance map (dynamic!) (what is not used only for debugging, can moved elsewhere)	
	void		Terrain_DynamicInit();																								// allocate dynamic terrain system (tile and cluster system) and set to defaults
		void		Terrain_DynamicFree();																							// remove and free dynamic terrain system data		
	
// stationary map creation + groups map (dynamic!) + summed maps (static=stationary, summed=dynamic for debugging with units)
		void		Terrain_DynamicCreate();																						// create dynamic terrain system
	
	//------------------------
	//------------------------

// stationary map - modifying static map (on startup and runtime as needed)	
	void		Terrain_SetBuildingTilesClusters(ENTITY* ent, STRING* bld_foldername);								// after placing an entity, or on game initialization called by a cycle going through all building entities
		function		Terrain_BuildingEvent();																					// for debugging: set building transparent on touch and reset on release (setting material to UNLIT would be fine)	
	void		Terrain_ResetBuildingTilesClusters(ENTITY* ent);															// reset terrain tiles and clusters just before deleting the entity !
	
	//------------------------
	
// stationary map
			void		Terrain_DynamicBuildings();																				// generate clearance map around and within buildings -> cluster odifications affect TileArray !!!
//			void		Terrain_DynamicShrubs();
			void		Terrain_DynamicObstacles();																				// generate clearance map around and within obstacles -> cluster odifications affect TileArray !!!
			
			void		Terrain_DynamicShadowEntities();																			// collect entities casting shadow in an array

// stationary map
			void		Terrain_DynamicStaticMap();																				// create terrain_static_clearancearray
// stationary map
			void		Terrain_DynamicSummedMap();																				// create terrain_summed_clearancearray (similar to static, units clearance map is added only on update)
				
// dynamic map
			void		Terrain_DynamicGroupMap();																					// group quantity in each clusters (limited to 2-3), must be done after building cluster creation !!!
			void		Terrain_DynamicGroupMapRecreate();																		// required after cluster array length change !!! runtime only !
				
	//------------------------
	// clearance map updates for debugging only
	
// dynamic map for debugging (only on startup only for debugging, within 1 frame)
	void		Terrain_AllClearanceMapUpdate();																					// update all clearance maps - unit cl map and 4 static cl maps - only on startup for debugging
	
// dynamic map for debugging (same as above but: 1st frame: unit map, 2nd-5th frame: summed: 4 capability maps)
		void		Terrain_SummedClearanceMapUpdate(int clearancemapnum, int fromlevel, int tolevel);			// update summed clearance maps - only for debug maps - unit+static for the given capability and levels
		void		Terrain_UnitClearanceMapUpdate();																			// update unit clearance map - only for debug maps
		
	//------------------------
	// clearance map helpers
		
			void		Terrain_SetClearance( char* arraytoset , ENTITY* ent );											// clearance for shrubs and obstacles
			
			void		Terrain_SetClearance_AroundBuilding( char** arraytoset , ENTITY* ent );						// clearance for buildings
				void		Terrain_SetClearance_NonBuilding_Tile( char* arraytoset, var temp, var temp_n );		// helper of Terrain_SetClearance_AroundBuilding - sets clearance area of a non-building tile next to a building tile
					void		Terrain_SetClearance_NonBuilding_Tile0( char* arraytoset , int abstile );			// not used - helper of Terrain_SetClearance_NonBuilding_Tile - set tile to 0
					void		Terrain_SetClearance_NonBuilding_Tile1( char* arraytoset , int abstile );			// helper of Terrain_SetClearance_NonBuilding_Tile - set tile to 1
					void		Terrain_SetClearance_NonBuilding_Tile3( char* arraytoset , int abstile );			// helper of Terrain_SetClearance_NonBuilding_Tile - set tile to 3
			
			void		Terrain_SetClearance_Unit( char** arraytoset , ENTITY* ent );									// not used - clearance for units - exact
			void		Terrain_SetClearance_Unit_Simple( char** arraytoset , ENTITY* ent );							// clearance for units - with 0 or 100 values only - faster!
		
	//------------------------
	
	void		Terrain_ProgressMessage();																							// displays a string containing info about the state of the map decomposition process
		void		Terrain_ProgressMessageFree();
	
/////////////////////////////////////////////////////////////////////////



#endif