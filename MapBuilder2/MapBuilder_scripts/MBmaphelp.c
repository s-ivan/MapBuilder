
#ifndef MBmaphelp_c
#define MBmaphelp_c


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code



//////////////////////////////////////////////////////////////
// coordinate conversions


function XTileToCoord(int xtile)
{
	return ((var)xtile * (var)GetMapTileSize() 	- (var)GetMapTilesX() * 0.5 * (var)GetMapTileSize()) 	+ (var)GetMapTileSize() * 0.5;		// coord calc from origin		- origo shift		+ tile half compensation
}


function YTileToCoord(int ytile)
{
	return ((var)ytile * (var)GetMapTileSize() 	- (var)GetMapTilesY() * 0.5 * (var)GetMapTileSize()) 	+ (var)GetMapTileSize() * 0.5;
}

//---------------------

int		XCoordToTile(var xcoord)
{
	return floor( (xcoord + (var)GetMapTilesX() * 0.5 * (var)GetMapTileSize()) / (var)GetMapTileSize() );
}


int		YCoordToTile(var ycoord)
{
	return floor( (ycoord + (var)GetMapTilesY() * 0.5 * (var)GetMapTileSize()) / (var)GetMapTileSize() );
}

//---------------------

int		XYCoordToAbsTile(var xcoord, var ycoord)
{
	int xtilepos = XCoordToTile(xcoord);
	int ytilepos = YCoordToTile(ycoord);
	
	return XYTileToAbsTile(xtilepos, ytilepos);	
}

//---------------------

int		XYTileToAbsTile(int xtile, int ytile)
{
	if (XYTileValid(xtile, ytile))
		{
			return xtile + ytile * GetMapTilesX();
		}
	else
		{
			return GetMapTilesCount();
		}
}

//---------------------

int		AbsTileToXTile(int abstile)
{
	return abstile % GetMapTilesX();
}


int		AbsTileToYTile(int abstile)
{
	return integer((var)abstile / (var)GetMapTilesX());	
}

//---------------------

function	AbsTileToXCoord(int abstile)
{
	return XTileToCoord( AbsTileToXTile(abstile) );
}


function	AbsTileToYCoord(int abstile)
{
	return YTileToCoord( AbsTileToYTile(abstile) );
}


//////////////////////////////////////////////////////////////
// snapping

VECTOR*	SnapCoordToTileMidCoordXY(VECTOR* pos_vec)
{
	//-------------------------
	
	if (pos_vec == NULL)	return nullvector;		// or NULL but that would require some safety checks
	
	//-------------------------
	
	VECTOR snapped_vec;
	vec_zero(snapped_vec);
	
	//-------------------------
	
	if (pos_vec.x < 0)
		{
			snapped_vec.x = GetMapTileSize() * (integer(pos_vec.x / GetMapTileSize())) - (GetMapTileSize() / 2);		
		}
	else if (pos_vec.x >= 0)
		{
			snapped_vec.x = GetMapTileSize() * (integer(pos_vec.x / GetMapTileSize())) + (GetMapTileSize() / 2);
		}       
	
	if (pos_vec.y < 0)
		{
			snapped_vec.y = GetMapTileSize() * (integer(pos_vec.y / GetMapTileSize())) - (GetMapTileSize() / 2);
		}
	else if (pos_vec.y >= 0)
		{
			snapped_vec.y = GetMapTileSize() * (integer(pos_vec.y / GetMapTileSize())) + (GetMapTileSize() / 2);
		}  
	
	snapped_vec.z = pos_vec.z;
	
	//-------------------------
	
	return &snapped_vec;
	
	//-------------------------
}


VECTOR*	SnapCoordToTileCoordZ(VECTOR* pos_vec)
{
	//-------------------------
	
	if (pos_vec == NULL)	return nullvector;		// or NULL but that would require some safety checks
	
	//-------------------------
	
	VECTOR snapped_vec;
	vec_set(snapped_vec, pos_vec);
	
	//-------------------------
	
	snapped_vec.z = integer(snapped_vec.z / GetMapTileSize()) * GetMapTileSize();	
	
	//-------------------------
	
	return &snapped_vec;
	
	//-------------------------
}


//////////////////////////////////////////////////////////////
// protections


function AbsTileValid(int abstile)
{
	if ( (abstile >= 0) && (abstile < GetMapTilesCount()) ) 
		{
			return 1;
		}
	else
		{
			return 0;
		}
}


function XYTileValid(int xtile, int ytile)
{
	if ( (xtile >= 0) && (ytile >= 0) && (xtile < GetMapTilesX()) && (ytile < GetMapTilesY()))
		{
			return 1;
		}
	else
		{
			return 0;
		}
}


//////////////////////////////////////////////////////////////
// gets

//---------------------------

STRING*	GetMapName()
{
	return map_mapname;
}

//---------------------------

int		GetMapTilesX()
{
	return map_maptilesx;
}


int		GetMapTilesY()
{
	return map_maptilesy;
}

//---------------------------

int		GetMapZLevels()
{
	return map_mapzlevels;
}


int		GetMapTileSize()
{
	return map_tilesize;
}


int		GetMapClusterSize()
{
	return map_clustersize;
}

//---------------------------

function	GetMapScaleValue()
{
	return map_scalevalue;
}

//---------------------------

int		GetMapDefaultTerrainType()
{
	return map_defaultterraintype;
}


int		GetMapDefaultMoveCost()
{
	return map_defaultmovecost;
}

//---------------------------

STRING*	GetMapGroundName()
{
	return map_groundhmpname;
}


STRING*	GetMapWaterName()
{
	return map_waterhmpname;
}


STRING*	GetMapLevelName()
{
	return map_levelwmbname;
}

//---------------------------

function	GetMapGroundDetail()
{
	return map_detailsize_ground;
}


function	GetMapWaterDetail()
{
	return map_detailsize_water;
}


function	GetMapGroundBlendFactor()
{
	return map_blendfactor_ground;
}


function	GetMapTerrainChunk()
{
	return map_terrain_chunk;
}

//////////////////////////////////////////////////////////////
// gets - calculated

//---------------------------

int		GetMapTilesCount()
{
	return GetMapTilesX() * GetMapTilesY();
}

//---------------------------

int		GetBaseMapClustersX()
{
	int clus_per_row = integer(GetMapTilesX() / GetMapClusterSize()) ;  			// e.g. = 500/10 = 50	
	if (GetMapTilesX() % GetMapClusterSize() > 0)
		{
			clus_per_row++;																	// results in smaller sized clusters on the east and north
		}																			
	return clus_per_row;		
}


int		GetBaseMapClustersY()
{
	int clus_per_column = integer(GetMapTilesY() / GetMapClusterSize()) ;  		// e.g. = 500/10 = 50	
	if (GetMapTilesY() % GetMapClusterSize() > 0)
		{
			clus_per_column++;																// results in smaller sized clusters on the east and north
		}																			
	return clus_per_column;	
}


int		GetBaseMapClustersCount()
{
	return GetBaseMapClustersX() * GetBaseMapClustersY();	
}


//////////////////////////////////////////////////////////////
// sets

//---------------------------

void		SetMapName(STRING* name)
{
	if (name)
		{
			str_cpy(map_mapname, name);
		}
}

//---------------------------

void		SetMapTilesX(int xtiles)
{
	xtiles			+= xtiles % 2;							// protection - keep it even !!! because of the compensation in XTileToCoord()
	map_maptilesx 	= xtiles;
}


void		SetMapTilesY(int ytiles)
{
	ytiles			+= ytiles % 2;							// protection - keep it even !!! because of the compensation in YTileToCoord()
	map_maptilesy 	= ytiles;
}

//---------------------------

void		SetMapZLevels(int levels)
{
	map_mapzlevels = levels;
}


void		SetMapTileSize(int tilesize)
{
	map_tilesize = tilesize;
}


void		SetMapClusterSize(int clustersize)
{
	map_clustersize = clustersize;
}

//---------------------------

void		SetMapScaleValue(var scalevalue)
{
	map_scalevalue = scalevalue;
}

//---------------------------

void		SetMapDefaultTerrainType(int tiletype)
{
	map_defaultterraintype = tiletype;
}


void		SetMapDefaultMoveCost(int movecost)
{
	map_defaultmovecost = movecost;
}

//---------------------------

void		SetMapGroundName(STRING* name)
{
	if (name)
		{
			str_cpy(map_groundhmpname, name);
		}
}


void		SetMapWaterName(STRING* name)
{
	if (name)
		{
			str_cpy(map_waterhmpname, name);
		}
}


void		SetMapLevelName(STRING* name)
{
	if (name)
		{
			str_cpy(map_levelwmbname, name);
		}
}

//---------------------------

void		SetMapGroundDetail(var grounddetail)
{
	map_detailsize_ground = grounddetail;
}


void		SetMapWaterDetail(var waterdetail)
{
	map_detailsize_water = waterdetail;
}


void		SetMapGroundBlendFactor(var groundblendfactor)
{
	map_blendfactor_ground = groundblendfactor;
}


void		SetMapTerrainChunk(var chunksize)
{
	map_terrain_chunk = chunksize;
}

//////////////////////////////////////////////////////////////
// sets - calculated


void		SetMapTilesXY(int xytiles)
{
	SetMapTilesX( xytiles );
	SetMapTilesY( xytiles );
}


void		SetMapDefaults()
{
	// minimum 3200
	// standard 20480
	
	//--------------------
	// x
	
	if (abs(TerrHmp_GetGroundMaxX())>=32768/2)
		{
			SetMapTilesX(MAP_HUGEMAP);
		}
	else if (abs(TerrHmp_GetGroundMaxX())>=28672/2)
		{
			SetMapTilesX(MAP_BIGMAP);
		} 
	else if (abs(TerrHmp_GetGroundMaxX())>=24576/2)
		{
			SetMapTilesX(MAP_MIDMAP);
		}
	else if (abs(TerrHmp_GetGroundMaxX())>=6400/2)
		{
			SetMapTilesX(MAP_SMALLMAP);
		} 
	else if (abs(TerrHmp_GetGroundMaxX())>=3200/2)
		{
			SetMapTilesX(MAP_TINYMAP);
		} 
	else
		{
			SetMapTilesX(MAP_TINYMAP);						
//			printf( "Ground terrain too small! Program exits." );
//			Main_Exit();
		}
	
	//--------------------
	// y
	
	if (abs(TerrHmp_GetGroundMaxY())>=32768/2)
		{
			SetMapTilesY(MAP_HUGEMAP);
		}
	else if (abs(TerrHmp_GetGroundMaxY())>=28672/2)
		{
			SetMapTilesY(MAP_BIGMAP);
		} 
	else if (abs(TerrHmp_GetGroundMaxY())>=24576/2)
		{
			SetMapTilesY(MAP_MIDMAP);
		}
	else if (abs(TerrHmp_GetGroundMaxY())>=6400/2)
		{
			SetMapTilesY(MAP_SMALLMAP);
		} 
	else if (abs(TerrHmp_GetGroundMaxY())>=3200/2)
		{
			SetMapTilesY(MAP_TINYMAP);
		} 
	else
		{
			SetMapTilesY(MAP_TINYMAP);						
//			printf( "Ground terrain too small! Program exits." );
//			Main_Exit();
		}
	
	//----------------------	
	
	map_tilesize 		= MAP_DEFAULTGRIDSIZE;									// 32 is default
	map_clustersize 	= MAP_DEFAULTCLUSTERSIZE;								// 8 is default
#ifndef MB_RELEASE	
	map_mapzlevels 	= BUILDING_LEVELS;										// 2, or map specific: open field	0 , siege 1-2
#else
	map_mapzlevels 	= 1;
#endif
	
	// calculated values
	map_scalevalue 	= GetMapTileSize() / MAP_DEFAULTGRIDSIZE;
	
#ifndef MB_RELEASE	
	map_defaultterraintype 	= TERR_GRASS ;
	map_defaultmovecost 		= COST_NORMAL ;
#else
	map_defaultterraintype 	= 1 ;
	map_defaultmovecost 		= 1 ;
#endif

	//----------------------
}

//---------------------------

void		SetLevelEntSizeAuto()
{
	SetLevelEntSizeToMin();
	
	you = ent_next(NULL);
	while (you)
		{
			SentLevelEntSizeToEnt(you);
			
			ent_next(you);
		}
}


void		SetLevelEntSizeToMin()
{
	level_ent.min_x = minv(level_ent.min_x, -MAP_LEVEL_ENT_MINSIZEXY);
	level_ent.min_y = minv(level_ent.min_y, -MAP_LEVEL_ENT_MINSIZEXY);
	level_ent.min_z = minv(level_ent.min_z, -MAP_LEVEL_ENT_MINSIZEZ);
	
	level_ent.max_x = maxv(level_ent.max_x, MAP_LEVEL_ENT_MINSIZEXY);
	level_ent.max_y = maxv(level_ent.max_y, MAP_LEVEL_ENT_MINSIZEXY);
	level_ent.max_z = maxv(level_ent.max_z, MAP_LEVEL_ENT_MINSIZEZ);
}


void		SentLevelEntSizeToEnt(ENTITY* ent)
{
	level_ent.min_x = minv(level_ent.min_x, ent.x + ent.min_x - MAP_LEVEL_ENT_INCREMENT);
	level_ent.min_y = minv(level_ent.min_y, ent.y + ent.min_y - MAP_LEVEL_ENT_INCREMENT);
	level_ent.min_z = minv(level_ent.min_z, ent.z + ent.min_z - MAP_LEVEL_ENT_INCREMENT);
	
	level_ent.max_x = maxv(level_ent.max_x, ent.x + ent.max_x + MAP_LEVEL_ENT_INCREMENT);
	level_ent.max_y = maxv(level_ent.max_y, ent.y + ent.max_y + MAP_LEVEL_ENT_INCREMENT);
	level_ent.max_z = maxv(level_ent.max_z, ent.z + ent.max_z + MAP_LEVEL_ENT_INCREMENT);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#endif