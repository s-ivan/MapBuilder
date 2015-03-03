
#ifndef MBdefines_h
#define MBdefines_h

///////////////////////////////////////////
// defines


//------------------------------
// flags usage in MapBuilder

// FLAG8 			- set for entities contained by an imported wmb 
// FLAG7 			- set for obstacle entities if they are trees, set for shrub collider entities
// FLAG6				- set for entities (obstacles and mapents) having any type of shadow - exclude from lightmap *** could be modified to include in lightmap
// FLAG5				- set for obstacle, mapent, (shrub) entities to force ent_decal shadows when pssm is used
// FLAG4				- 
// FLAG3 			- 
// FLAG2 			- set for surpressing nearby decal shadows
// FLAG1 			- set for entities to be hidden in water mirror

//------------------------------
// groups usage in MapBuilder

// .group = 1		- water terrain, camera objects, collider entities (shrub, obstacle)		- normally ignored by c-tracing
// .group = 2		- terrain model																			- often detected by c-tracing															- together with 4
// .group = 3		- obstacle model, map-entity, building model										- sometimes detected by c-tracing (obstacle models are set to FLAG2)		- together with 2, 4
// .group = 4		- ground terrain																			- always detected by c-tracing														- sometimes together with 2, 3
// .group = 30		- modify gizmo, rts unit																- only in special cases detected by c-tracing									- separately from others

//------------------------------
// entity related skills

// 1-20 are stored in $$m and wmb:

#define MINX				skill1		// bounding box
#define MINY				skill2
#define MINZ				skill3
#define MAXX				skill4
#define MAXY				skill5
#define MAXZ				skill6

#define MYALPHA			skill7		// WMB alpha

#define HASCLUSTER		skill8		// RTS only - for buildings only - showing they were processed or not, thus should be

//... here some shader skills will be in future (wind animation etc.)

#define ABSTILE_POS		skill14		// 10000..250000
#define XTILE_POS		 	skill15		// 0..500 x pos
#define YTILE_POS		 	skill16		// 0..500 y pos
#define ZTILE_POS		 	skill17		// 0..2   z pos - might be different later - i.e. snapping vertical building levels to whole tile heights

#define AREA_SIZEX		skill18		// area occupied by entity in tiles - x&y must be the same for units
#define AREA_SIZEY		skill19		// area occupied by entity in tiles - x&y can differ for buildings (, and ships )

#define ENT_CATEGORY		skill20		// differentiate between obstacle sprites/models, and other system entities. under 100 it is an deletable obstacle

// not stored in $$m and wmb:

#define ENT_TYPE			skill21		// tile passability - read from external file like from filename.obs to obs_type in case of obstacles

//------------------------------
// editor related

#define NO_Z_APPLIED		-1000.0		// RTS related - TileMidZ default - or something unrealistic like -64.646 etc.


///////////////////////////////////////////
// subfolders


STRING*	trr_foldername			= "maps\\";
STRING*	lev_foldername			= "levels\\";
STRING*	hmp_foldername			= "terrain_hmp\\";
STRING*	hmpw_foldername		= "water_hmp\\";
STRING*	map_foldername			= "maps\\";
STRING* 	sky_foldername			= "skies\\";

STRING* 	detail_foldername		= "grass_dds\\";	
STRING* 	shrub_foldername 		= "shrub_dds\\";
STRING* 	grass_foldername		= "grass_mdl\\";
STRING* 	obstacle_foldername 	= "obstacle_mdl\\";
STRING* 	mapent_foldername 	= "obstacle_wmb\\";
STRING*	terrmdl_foldername	= "terrain_mdl\\";
STRING* 	building_foldername 	= "building_mdl\\";
STRING*	tree_foldername		= "tree_mdl\\";			// currently used only to store mtl_tree material defaults

STRING*	textures_foldername	= "textures\\";
STRING* 	shadow_foldername 	= "shadows\\";				// subfolder name


///////////////////////////////////////////



#endif