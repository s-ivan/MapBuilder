
#ifndef MBmaphelp2_h
#define MBmaphelp2_h

///////////////////////////////////////////////////////////////////////
// functions

// lookup arrays could fasten neighbour checks !!!

//------------
// angle and direction conversions - mod to angle/45 directions

function	DirectionToAngle(int direction);										// 0..7 angle/45 directions to degrees
int		AngleToDirection(var angle);											// degrees to angle/45 directions

int		DirectionDifference(int reference, int compared);				// resulting direction of substraction of two direction vectors or angles 	given in direction	r-c
int		DirectionAddition(int reference, int compared);					// resulting direction of addition of two direction vectors or angles 		given in direction	r+c

function	DirectionDiagonal(int direction);									// returns true if diagonal direction
function DirectionCardinal(int direction);									// returns true if cardinal directio

function AngleRoundToDirection(var angle);									// rounds an angle to point in the closest of the 8 directions

int		DirectionRotateLeft(int direction);
int		DirectionRotateRight(int direction);

//------------
// gets calculated - neighbours - mod to angle/45 directions

int		GetNeighbourofAbsTile(int abstile, int direction);											// returns abs tile towards given direction with border check
int		GetNeighbourofAbsTileByDist(int abstile, int direction, int distance);				// returns abs tile towards given direction with border check in given tile distance

int		GetNeighbourofXYTile(int xtile, int ytile, int direction);								// returns abs tile towards given direction with border check
int		GetNeighbourofXYTileByDist(int xtile, int ytile, int direction, int distance);	// returns abs tile towards given direction with border check in given tile distance

void		GetXYTileOffset(int* xtile, int* ytile, int direction, int distance);				// returns xy offset values (as pointers) due to direction and (tile) distance

//------------

#endif