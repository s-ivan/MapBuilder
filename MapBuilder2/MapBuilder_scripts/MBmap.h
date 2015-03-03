
#ifndef MBmap_h
#define MBmap_h

//////////////////////////////////////////////////////////////////////
// defines


// standar map size definitions should be even number (in case of odd number tile coordinates are calculated wrongly)

#define MAP_TINYMAP					100				// for tutorials													(very fast)
#define MAP_SMALLMAP 				200				// 2 small armies on open field								(fast)
#define MAP_MIDMAP 					300				// 2 full armies on open field, or small fort siege	(medium)
#define MAP_BIGMAP 					400				// castle siege, or 3 armies on open field				(slower)
#define MAP_HUGEMAP 					500				// city siege, or 4 armies on open field					(slow)

#define MAP_DEFAULTGRIDSIZE 		32.0				
#define MAP_DEFAULTCLUSTERSIZE 	12					// in tiles: practically 8..16 - the smaller the faster search, but the more clusters : 12 is fine for ground, 8 is fine for buildings
#define MAP_MAXCLUSTERAREA 		256				// max 16*16=256 (used for visualization purposes)	

#define MAP_LEVEL_ENT_MINSIZEXY	5000.0			// a general minimum value
#define MAP_LEVEL_ENT_MINSIZEZ	1000.0			// a general minimum value
#define MAP_LEVEL_ENT_INCREMENT	1000.0			// additional value to max entity boundaries

//////////////////////////////////////////////////////////////////////
// variables 


//--------------------------------------------------------------------
// map data (save/load with .trr map)


STRING*	map_mapname 				= "";								// any text
int		map_maptilesx 				= 0;								// horizontal size in tiles
int		map_maptilesy 				= 0;								// vertical size in tiles
int		map_mapzlevels 			= 2;								// 0,1,2
int		map_tilesize 				= 32;								// 32
int		map_clustersize 			= 8;								// sub-area size 8..16 - the smaller the faster path search, but the more clusters
var		map_scalevalue 			= 1;								// mesh scaling, should be 1 for fast rendering, just for experiencing with tile size scaling

int		map_defaultterraintype	= 1;								// TERR_GRASS / TERR_SNOW / TERR_SAND
int		map_defaultmovecost		= 10;								// COST_NORMAL / COST_BAD / COST_BAD

STRING*	map_groundhmpname			= "";								// ground terrain file name - with path
STRING*	map_waterhmpname			= "";								// water terrain file name - with path
STRING*	map_levelwmbname			= "";								// wmb level file name - with path

STRING*	map_emptyname 				= "maps\\NewMap.trr";
																				// terrain texture related ones
var		map_detailsize_ground	= 32;								// terrain multitexture or detailmap tiling
var		map_detailsize_water		= 8;								// water multitexture or detailmap tiling
var		map_blendfactor_ground	= 0.67;							// terrain multitexture or detailmap blending with base colormap texture - the higher the stronger visibility of tiled textures or detailmap

var		map_terrain_chunk			= 32;

//--------------------------------------------------------------------
// map loading flags


var		map_wmb;															// 0 NULL level, 1 wmb level


// game only load_all customization (level detail can be decreased by not loading e.g. grass)
#ifdef MB_GAME
var		map_load_details			= 1;								// grass sprites
var		map_load_shrubs			= 1;								// shrub sprites
var		map_load_grass				= 1;								// grass models
var		map_load_obstacles		= 1;								// obstacle models
var		map_load_mapents			= 1;								// map entities
var		map_load_terrmdls			= 1;								// terrain models
var		map_load_buildings		= 1;								// building models

var		map_load_terrdeform		= 1;								// terrain vertices
var		map_load_sky				= 1;								// sky-sun-camera-lod-fog-shadow settings
var		map_load_materials		= 1;								// materials
var		map_load_watershaders	= 1;								// water shader settings
var		map_load_terrtext			= 1;								// terrain textures
var		map_load_terrlm			= 1;								// terrain lightmap
#endif


///////////////////////////////////////////
// entities

//// for rotating entities (units, small decorations)
//ENTITY* ent_unitshadow_rot = 
//	{ 
//		type = "shadows\\0dot_shadow.dds"; 
//	}
//// for entities having 0 pan always (large stationary units(buildings) and trees)
//ENTITY* ent_treeshadow_fix = 
//	{ 
//		type = "shadows\\0tree_shadow.dds"; 
//	}
//// shadow cast by another model
//ENTITY* ent_model_unitshadow = 
//	{ 
//		type = "shadows\\0cylinder_shadow.mdl"; 
//	}

// created entities - they are now stored in wmb too, but no other difference
ENTITY* ent_unitshadow_rot = NULL;
ENTITY* ent_treeshadow_fix = NULL;
ENTITY* ent_model_unitshadow = NULL;


//////////////////////////////////////////////////////////////////////
// functions		


void		Map_Init_NULL(char* file_name);															// fallback (if no wmb no hmp), set default a NULL level with shadow entities, calls shader init	
void		Map_Init_hmp(char* TerrainFileName, char* WaterFileName, var created);		// create a new Map from 1) a ground and a water terrain, 0) a .trr file containing terrain(s)
void		Map_Init_wmb(char* LevelFileName, var created);										// create a new Map from 1) a wmb level, 0) a .trr file containing a wmb

void 		Map_SaveMap_trr(char* file_name);
void 		Map_LoadMap_trr(char* file_name);
			
		void		Map_BuildMapData();																	// calls all editor related stuff		
		void		Map_DestroyMapData();																// stops and removes all used stuff

//--------------------------------------------------------------------

void		Map_CreateShadowEnts();																		// create the 3 shadow entities
void		Map_RemoveShadowEnts();

void		Map_NullEntCounters();

//--------------------------------------------------------------------

void		Map_Progressbar(var percent);
	void		Map_Progressbar_Hide();

#ifndef MB_GAME
void		Map_NexusCheck();
#endif

//////////////////////////////////////////////////////////////////////


#endif