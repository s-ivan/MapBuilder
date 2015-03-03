
#ifndef MBmaphelp_h
#define MBmaphelp_h

//////////////////////////////////////////////////////////////////////////////////////
// functions


// map 


//-----------------------------
// conversions

function XTileToCoord(int xtile);							// unprotected x tile to x coord - points to tile middle
function YTileToCoord(int ytile);							// unprotected x tile to x coord - points to tile middle

int 		XCoordToTile(var xcoord);							// unprotected x coord to x tile
int 		YCoordToTile(var ycoord);							// unprotected y coord to y tile

int		XYCoordToAbsTile(var xcoord, var ycoord);		// protected coord to abs tile

int		XYTileToAbsTile(int xtile, int ytile);			// protected xy tile to abs tile

int		AbsTileToXTile(int abstile);						// unprotected abs tile to x tile
int		AbsTileToYTile(int abstile);						// unprotected abs tile to y tile

function	AbsTileToXCoord(int abstile);						// unprotected abs tile to x coord
function	AbsTileToYCoord(int abstile);						// unprotected abs tile to y coord

//-----------------------------
// snapping

VECTOR*	SnapCoordToTileMidCoordXY(VECTOR* pos_vec);	// snap coord to tile xy middle			example:		vec_set(my.x, SnapCoordToTileMidCoordXY(&my.x));
VECTOR*	SnapCoordToTileCoordZ(VECTOR* pos_vec);		// snap coord to tile z						example:		vec_set(my.x, SnapCoordToTileCoordZ(&my.x));	

//-----------------------------
// protections

function AbsTileValid(int abstile);							// is abs tile within allowable range?
function XYTileValid(int xtile, int ytile);				// is x and y tile within allowable range?

//-----------------------------
// gets

STRING*	GetMapName();
int		GetMapTilesX();
int		GetMapTilesY();
int		GetMapZLevels();
int		GetMapTileSize();									
int		GetMapClusterSize();									// ***
function	GetMapScaleValue();
int		GetMapDefaultTerrainType();
int		GetMapDefaultMoveCost();

STRING*	GetMapGroundName();
STRING*	GetMapWaterName();
STRING*	GetMapLevelName();
function	GetMapGroundDetail();
function	GetMapWaterDetail();
function	GetMapGroundBlendFactor();

function	GetMapTerrainChunk();

// gets - calculated

int		GetMapTilesCount();

int		GetBaseMapClustersX();
int		GetBaseMapClustersY();
int		GetBaseMapClustersCount();

//-----------------------------
// sets

void		SetMapName(STRING* name);
void		SetMapTilesX(int xtiles);
void		SetMapTilesY(int ytiles);
void		SetMapZLevels(int levels);
void		SetMapTileSize(int tilesize);				
void		SetMapClusterSize(int clustersize);				// ***
void		SetMapScaleValue(var scalevalue);
void		SetMapDefaultTerrainType(int tiletype);
void		SetMapDefaultMoveCost(int movecost);

void		SetMapGroundName(STRING* name);
void		SetMapWaterName(STRING* name);
void		SetMapLevelName(STRING* name);
void		SetMapGroundDetail(var grounddetail);
void		SetMapWaterDetail(var waterdetail);
void		SetMapGroundBlendFactor(var groundblendfactor);

void		SetMapTerrainChunk(var chunksize);

// sets - calculated

void		SetMapTilesXY(int xytiles);
void		SetMapDefaults();										// set map_variables to defaults, size is based on terrian size

void		SetLevelEntSizeAuto();								// based on actual entities position and size
void		SetLevelEntSizeToMin();								// set level_ent size to default min if smaller - not needed due to new terrain min-max settings
void		SentLevelEntSizeToEnt(ENTITY* ent);

//////////////////////////////////////////////////////////////////////////////////////



#endif