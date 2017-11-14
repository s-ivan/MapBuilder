
#ifndef MBmaphelp2_c
#define MBmaphelp2_c


///////////////////////////////////////////////////////////////////////
// code


///////////////////////////////////////////
// group angle(degree)-direction(0-7) conversions

function DirectionToAngle(int direction)
{
	return (var)(direction * 45);														// 0,45,90,135,180,225,270,315
}


int AngleToDirection(var angle)
{	
	angle	= ang(angle);																	// shifts to -180...+180° range
	angle	+= 360 + 45/2;																	// be more than 360°, create +/-45°/2 range
	angle	%= 360;																			// 0..360
	
	return integer(angle / 45);														// 0..7
}


int	DirectionDifference(int reference, int compared)
{
	var difference = DirectionToAngle(reference) - DirectionToAngle(compared);
	
	return AngleToDirection(difference);
	
//	//or
//	return (8 - abs(groupolddirection - groupnewdirection));		// wrong !!!
}


int	DirectionAddition(int reference, int compared)
{
	var addition = DirectionToAngle(reference) + DirectionToAngle(compared);
	
	return AngleToDirection(addition);
	
//	return (reference + compared) % 8;
}


function	DirectionDiagonal(int direction)
{
	return (var)(direction % 2);
}


function DirectionCardinal(int direction)
{
	return (var)(1 - direction % 2);
}


function AngleRoundToDirection(var angle)
{
	return (var)(AngleToDirection(angle) * 45);														// or simply return integer(ang(diff_ang.pan)/45)*45; but it has a wrong rounding
}


int	DirectionRotateLeft(int direction)
{
	return ((direction + 1) % 8);
}


int	DirectionRotateRight(int direction)
{
	return ((direction + 7) % 8);
}


//////////////////////////////////////////////////////////////
// gets - calculated - neighbours


int		GetNeighbourofAbsTile(int abstile, int direction)
{	
	if (direction == 0)									// E
		{
			abstile += 1;
		}
	else if (direction == 1)							// NE
		{
			abstile += GetMapTilesX();
			abstile += 1;
		}
	else if (direction == 2)							// N
		{
			abstile += GetMapTilesX();
		}
	else if (direction == 3)							// NW
		{
			abstile += GetMapTilesX();
			abstile -= 1;
		}
	else if (direction == 4)							// W
		{
			abstile -= 1;
		}
	else if (direction == 5)							// SW
		{
			abstile -= GetMapTilesX();
			abstile -= 1;
		}
	else if (direction == 6)							// S
		{
			abstile -= GetMapTilesX();
		}
	else if (direction == 7)							// SE
		{
			abstile -= GetMapTilesX();
			abstile += 1;
		}
	
	if (AbsTileValid(abstile))							// protection
		{
			return abstile;								// valid
		}
	else
		{
			return GetMapTilesCount();					// invalid
		}
}


int		GetNeighbourofAbsTileByDist(int abstile, int direction, int distance)
{	
	if (direction == 0)									// E
		{
			abstile += distance;
		}
	else if (direction == 1)							// NE
		{
			abstile += GetMapTilesX() * distance;
			abstile += distance;
		}
	else if (direction == 2)							// N
		{
			abstile += GetMapTilesX() * distance;
		}
	else if (direction == 3)							// NW
		{
			abstile += GetMapTilesX() * distance;
			abstile -= distance;
		}
	else if (direction == 4)							// W
		{
			abstile -= distance;
		}
	else if (direction == 5)							// SW
		{
			abstile -= GetMapTilesX() * distance;
			abstile -= distance;
		}
	else if (direction == 6)							// S
		{
			abstile -= GetMapTilesX() * distance;
		}
	else if (direction == 7)							// SE
		{
			abstile -= GetMapTilesX() * distance;
			abstile += distance;
		}
	
	if (AbsTileValid(abstile))							// protection
		{
			return abstile;								// valid
		}
	else
		{
			return GetMapTilesCount();					// invalid
		}
}


int		GetNeighbourofXYTile(int xtile, int ytile, int direction)
{
	if (direction == 0)									// E
		{
			++xtile;
		}
	else if (direction == 1)							// NE
		{
			++ytile;
			++xtile;
		}
	else if (direction == 2)							// N
		{
			++ytile;
		}
	else if (direction == 3)							// NW
		{
			++ytile;
			--xtile;
		}
	else if (direction == 4)							// W
		{
			--xtile;
		}
	else if (direction == 5)							// SW
		{
			--ytile;
			--xtile;
		}
	else if (direction == 6)							// S
		{
			--ytile;
		}
	else if (direction == 7)							// SE
		{
			--ytile;
			++xtile;
		}
	
	int return_tile = XYTileToAbsTile(xtile, ytile);
	
	if (AbsTileValid(return_tile))					// protection
		{
			return return_tile;							// valid
		}
	else
		{
			return GetMapTilesCount();					// invalid
		}
}


int		GetNeighbourofXYTileByDist(int xtile, int ytile, int direction, int distance)
{		
	if (direction == 0)									// E
		{
			xtile += distance;
		}
	else if (direction == 1)							// NE
		{
			ytile += distance;
			xtile += distance;
		}
	else if (direction == 2)							// N
		{
			ytile += distance;
		}
	else if (direction == 3)							// NW
		{
			ytile += distance;
			xtile -= distance;
		}
	else if (direction == 4)							// W
		{
			xtile -= distance;
		}
	else if (direction == 5)							// SW
		{
			ytile -= distance;
			xtile -= distance;
		}
	else if (direction == 6)							// S
		{
			ytile -= distance;
		}
	else if (direction == 7)							// SE
		{
			ytile -= distance;
			xtile += distance;
		}
	
	int return_tile = XYTileToAbsTile(xtile, ytile);
	
	if (AbsTileValid(return_tile))					// protection
		{
			return return_tile;							// valid
		}
	else
		{
			return GetMapTilesCount();					// invalid
		}
}


void		GetXYTileOffset(int* xtile, int* ytile, int direction, int distance)
{
	if (direction == 0)									// E
		{
			*xtile = distance;
			*ytile = 0;
		}
	else if (direction == 1)							// NE
		{
			*xtile = distance;
			*ytile = distance;			
		}
	else if (direction == 2)							// N
		{
			*xtile = 0;
			*ytile = distance;			
		}
	else if (direction == 3)							// NW
		{
			*xtile = -distance;
			*ytile = distance;			
		}
	else if (direction == 4)							// W
		{
			*xtile = -distance;
			*ytile = 0;
		}
	else if (direction == 5)							// SW
		{
			*xtile = -distance;
			*ytile = -distance;			
		}
	else if (direction == 6)							// S
		{
			*xtile = 0;
			*ytile = -distance;			
		}
	else if (direction == 7)							// SE
		{
			*xtile = distance;
			*ytile = -distance;			
		}
}


//////////////////////////////////////////////


#endif