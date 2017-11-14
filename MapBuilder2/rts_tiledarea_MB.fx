////////////////////////////////////////////////////////
// variables got from RTS

float play02_map_tilesize_var;
float play02_map_maptilesx_var;
float play02_map_maptilesy_var;
float play02_bmap_ready_var;
float play02_debugcluster_var;

////////////////////////////////////////////////////////
// clearance texture data got from RTS

texture play02_tiledatabmap_bmap;										// R = level 0 clearance / 255 (0..255)
																					// G = level 1 clearance / 255 (0..255)
																					// B = level 0 height / 25500 (0..255)
																					// A = level 1 height / 10000 (0..100)

sampler TileDataSampler1 = sampler_state
{
	Texture = <play02_tiledatabmap_bmap>;
	MinFilter = None;															// very important !!!
	MagFilter = None;															// very important !!!
	MipFilter = None;   														// very important !!!
};

//// or : requires SetTexture in each frame!
//
//texture DebugMap;															// or : sent by SetTexture - same update behaviour - error on buildings! seems to be a .b and .a precision error
//
//sampler TileDataSampler1 = sampler_state
//{
//	Texture = <DebugMap>;
//	MinFilter = None;															// very important !!!
//	MagFilter = None;															// very important !!!
//	MipFilter = None;   														// very important !!!
//};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// helpers


//float DebugCoordToTile(float xycoord)
//{	
//	float xytile = floor( (xycoord + play02_map_maptiles_var * 0.5f * play02_map_tilesize_var) / play02_map_tilesize_var );
//	
//	return( xytile );
//}

float DebugCoordToTileX(float xcoord)
{	
	float xytile = floor( (xcoord + play02_map_maptilesx_var * 0.5f * play02_map_tilesize_var) / play02_map_tilesize_var );
	
	return( xytile );
}

float DebugCoordToTileY(float ycoord)
{	
	float xytile = floor( (ycoord + play02_map_maptilesy_var * 0.5f * play02_map_tilesize_var) / play02_map_tilesize_var );
	
	return( xytile );
}


float2 DebugWorldToTile(float2 WorldXZ)
{
//	float2 WorldXZ = In.WorldPos.xz;
	
	float2 tileXY = float2(0.0f, 0.0f); 
	
	tileXY.x = DebugCoordToTileX(WorldXZ.x);
	tileXY.y = DebugCoordToTileY(WorldXZ.y); 
	
	return tileXY;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// tile pattern


float3 DebugTerrainTiles(float2 WorldXZ)
{
	// full are with double rows/columns at 0,-0
//		float2 WorldXZ = In.WorldPos.xz;	
//		float valuex = int(In.WorldPos.x / play02_map_tilesize_var);				// tile x, 2 0 rows in middle
//		float valuez = int(In.WorldPos.z / play02_map_tilesize_var);				// tile z, 2 0 columns in middle
//		float fracx	= frac(abs(valuex)/2);													//  * 2 : 0 or 1
//		float fracz	= frac(abs(valuez)/2);													//  * 2 : 0 or 1
//		float finalcol = (fracx+fracz)/4 + 0.75;
//		Color.rgb *= float3(finalcol, finalcol, finalcol);		
	
	// game area only
//	float2 WorldXZ = In.WorldPos.xz;	
		
	float2 tileXY = DebugWorldToTile(WorldXZ);	
	
	float fracx	= frac(abs(tileXY.x - play02_map_tilesize_var)/2);
	float fracz	= frac(abs(tileXY.y - play02_map_tilesize_var)/2);
	
	float finalcol = (fracx+fracz)/4 + 0.75;		
	
	if (
			(tileXY.x >= 0)
			&&
			(tileXY.y >= 0)
			&&
			(tileXY.x < play02_map_maptilesx_var)
			&&
			(tileXY.y < play02_map_maptilesy_var)
		)
		{
			return( float3(finalcol, finalcol, finalcol) );			// inside game area : squared pattern
		}
	else 
		{
			return( float3(1, 1, 1) );										// outside game area : no change
		}	
	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// tile clearance value with tile pattern


float3 DebugTerrainClearance(float3 WorldXZY, float3 WorldNormal)
{
//	float3 WorldXZY = In.WorldPos.xzy;
	
	float2 tileXY = DebugWorldToTile(WorldXZY.xy);	
	
	// get abstile from xytiles
//	float tileABS = tileXY.x + tileXY.y * play02_map_tilesize_var;
	
	//----------------------------------------------------------------------------------------------
	// clearance data
	
	float3 finalcolRGB = float3(1, 1, 1);
	
	// set colour based on height and clearance - game area only
	if (
			(tileXY.x >= 0)
			&&
			(tileXY.y >= 0)
			&&
			(tileXY.x < play02_map_maptilesx_var)
			&&
			(tileXY.y < play02_map_maptilesy_var)
		)
		{
			
			//-------------------------------------------------------------------------
			// read data from 1d or 2d texture
			
//			float datapos = tileABS / 512 / 512;
//			float4 tiledata = tex1D(TileDataSampler1, datapos);
			
			float2 datapos;
			datapos.x = tileXY.x / 512.0f;
			datapos.y = tileXY.y / 512.0f;
			float4 tiledata = tex2D(TileDataSampler1, datapos);
			
//			finalcolRGB = tiledata.rgb;	// test
			
			//-------------------------------------------------------------------------
			
			// determine height boundaries for level 0 and level 1
			float lowerbound0 = (tiledata.b * 255.0f * 10.0f) - play02_map_tilesize_var * 0.5f;													// 0..1 to 0..255*10
			float upperbound0 = (tiledata.b * 255.0f * 10.0f) + play02_map_tilesize_var * 0.5f;													// 0..1 to 0..255*10
			
			float lowerbound1 = (tiledata.a * 100.0f * 25.5f) - play02_map_tilesize_var * 0.5f;													// 0..1 to 0..100*25.5
			float upperbound1 = (tiledata.a * 100.0f * 25.5f) + play02_map_tilesize_var * 0.5f;													// 0..1 to 0..100*25.5
			
			// determine clearance values for level 0 and level 1
			float clearance0 = tiledata.r * 255.0f;													// 0..1 to 0..255
			float clearance1 = tiledata.g * 255.0f;													// 0..1 to 0..255
			
			//--------------------------------------------------------------------------
			
			if (
//					(WorldNormal.y > 0.5)
//					&&
					(
//						(clearance1 > 250) 																	// no building in tile, paint full entity height
//						|| 
						((WorldXZY.z >= lowerbound0) && (WorldXZY.z <= upperbound0))
					)
				)
				{
					if (clearance0 == 0.0f)
						{
							finalcolRGB = float3(1.0, 0.5, 0.5);	// red
						}
					else
					if (clearance0 == 1.0f)
						{
							finalcolRGB = float3(0.5, 0.5, 1.0);	// blue
						}
					else
					if (clearance0 == 3.0f)
						{
							finalcolRGB = float3(0.5, 1.0, 0.5);	// green
						}
					/*
					else
					if (clearance0 < 5.5f)
						{
							finalcolRGB = float3(0.5, 1.0, 1.0);	// cyan - not used
						}
					else
						{
							finalcolRGB = float3(1.0, 1.0, 0.5);	// yellow - not needed
						}
					*/
				}
			else
			if (
//					(WorldNormal.y > 0.5f)
//					&&
//					(clearance1 < 250)
//					&&
					(WorldXZY.z >= lowerbound1) && (WorldXZY.z <= upperbound1)
				)
				{
					if (clearance1 == 0.0f)
						{
							finalcolRGB = float3(1.0, 0.5, 1.0);	// lred -> purple
						}
					else
					if (clearance1 == 1.0f)
						{
							finalcolRGB = float3(0.5, 1.0, 1.0);	// lblue -> cyan
						}
					else
					if (clearance1 == 3.0f)
						{
							finalcolRGB = float3(1.0, 1.0, 0.5);	// lgreen -> yellow
						}
				}
		
		}	// in game area
	
//	 return finalcolRGB;	// test
	
	//---------------------------------------------------------------------
	// tile pattern
	
	float3 tilecolRGB = float3(1, 1, 1);
	
	float fracx	= frac(abs(tileXY.x - play02_map_tilesize_var)/2);
	float fracz	= frac(abs(tileXY.y - play02_map_tilesize_var)/2);
	
	float tilecol = (fracx+fracz)/4 + 0.75;		
	
	if (
			(tileXY.x >= 0.0f)
			&&
			(tileXY.y >= 0.0f)
			&&
			(tileXY.x < play02_map_maptilesx_var)
			&&
			(tileXY.y < play02_map_maptilesy_var)
		)
		{
			tilecolRGB = float3(tilecol, tilecol, tilecol);				// inside game area : squared pattern
		}
	else 
		{
			tilecolRGB = float3(0.5, 0.5, 0.5);								// outside game area : grey
		}	
	
	//---------------------------------------------------------------------
	
	return (finalcolRGB * tilecolRGB);
	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cluster pattern with tile pattern


float3 DebugTerrainClusters(float3 WorldXZY, float3 WorldNormal)
{
//	float3 WorldXZY = In.WorldPos.xzy;
	
	float2 tileXY = DebugWorldToTile(WorldXZY.xy);	
	
	// get abstile from xytiles
//	float tileABS = tileXY.x + tileXY.y * play02_map_tilesize_var;
	
	//----------------------------------------------------------------------------------------------
	// clearance data
	
	float3 finalcolRGB = float3(1, 1, 1);
	
	// set colour based on height and clearance - game area only
	if (
			(tileXY.x >= 0)
			&&
			(tileXY.y >= 0)
			&&
			(tileXY.x < play02_map_maptilesx_var)
			&&
			(tileXY.y < play02_map_maptilesy_var)
		)
		{
			//-------------------------------------------------------------------------
			// read data from 1d or 2d texture
			
//			float datapos = tileABS / 512 / 512;
//			float4 tiledata = tex1D(TileDataSampler1, datapos);
			
			float2 datapos;
			datapos.x = tileXY.x / 512.0f;
			datapos.y = tileXY.y / 512.0f;
			float4 tiledata = tex2D(TileDataSampler1, datapos);
			
//			finalcolRGB = tiledata.rgb;	// test
			
			//-------------------------------------------------------------------------
			
			// determine height boundaries for level 0 and level 1
			float lowerbound0 = (tiledata.b * 255.0f * 10.0f) - play02_map_tilesize_var * 0.5f;													// 0..1 to 0..255*10
			float upperbound0 = (tiledata.b * 255.0f * 10.0f) + play02_map_tilesize_var * 0.5f;													// 0..1 to 0..255*10
			
			float lowerbound1 = (tiledata.a * 100.0f * 25.5f) - play02_map_tilesize_var * 0.5f;													// 0..1 to 0..100*25.5
			float upperbound1 = (tiledata.a * 100.0f * 25.5f) + play02_map_tilesize_var * 0.5f;													// 0..1 to 0..100*25.5
			
			// determine cluster tile values for level 0 and level 1
			float cluster0 = tiledata.r * 255.0f;																					// 0..1
			float cluster1 = tiledata.g * 255.0f;																					// 0..1
			
			//--------------------------------------------------------------------------
			
			float cluster = 100;
			
			if (
//					(WorldNormal.y > 0.5)
//					&&
					(
//						(clearance1 > 250) 																															// no building in tile, paint full entity height
//						|| 
						((WorldXZY.z >= lowerbound0) && (WorldXZY.z <= upperbound0))
					)
				)
				{
					cluster = cluster0;
					
//					float clustercol = cluster0 * 0.5f + 0.5f;
//					
//					finalcolRGB = float3(clustercol, 1.0f, clustercol);	
				}
			else
			if (
//					(WorldNormal.y > 0.5f)
//					&&
//					(clearance1 < 250)
//					&&
					(WorldXZY.z >= lowerbound1) && (WorldXZY.z <= upperbound1)
				)
				{
					cluster = cluster1;
					
//					float clustercol = cluster1 * 0.5f + 0.5f;
//					
//					finalcolRGB = float3(clustercol, clustercol, 1.0f);	
				}
			
			if (cluster != 100)
				{
					if (cluster == 0.0f)
						{
							finalcolRGB = float3(1.0, 0.75, 0.75);		// lred
						}
					else
					if (cluster == 1.0f)
						{
							finalcolRGB = float3(0.75, 0.75, 1.0);		// lblue
						}
					else
					if (cluster == 2.0f)
						{
							finalcolRGB = float3(0.75, 1.0, 0.75);		// lgreen
						}
					else
					if (cluster == 3.0f)
						{
							finalcolRGB = float3(1.0, 0.75, 1.0);		// lpurple
						}
					else
					if (cluster == 4.0f)
						{
							finalcolRGB = float3(0.75, 1.0, 1.0);		// lcyan
						}
					else
					if (cluster == 5.0f)
						{
							finalcolRGB = float3(1.0, 1.0, 0.75);		// lyellow
						}
					else
					if (cluster == 6.0f)
						{
							finalcolRGB = float3(0.75, 0.75, 0.75);	// lgrey
						}
				}
				
		}	// in game area
	
//	 return finalcolRGB;	// test
	
	//---------------------------------------------------------------------
	// tile pattern
	
	float3 tilecolRGB = float3(1, 1, 1);			
	
	if (
			(tileXY.x >= 0.0f)
			&&
			(tileXY.y >= 0.0f)
			&&
			(tileXY.x < play02_map_maptilesx_var)
			&&
			(tileXY.y < play02_map_maptilesy_var)
		)
		{
			float fracx	= frac(abs(tileXY.x - play02_map_tilesize_var)/2);
			float fracz	= frac(abs(tileXY.y - play02_map_tilesize_var)/2);
			
			float tilecol = (fracx+fracz)/4 + 0.75;	
			
			tilecolRGB = float3(tilecol, tilecol, tilecol);				// inside game area : squared pattern
		}
	else 
		{
			tilecolRGB = float3(0.5, 0.5, 0.5);								// outside game area : grey
		}	
	
	//---------------------------------------------------------------------
	
	return (finalcolRGB * tilecolRGB);
	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// main function


float3 DebugTerrain(float3 WorldXYZ, float3 WorldNormal)
{
	float3 Color = 1;
	
	if (play02_bmap_ready_var > 0)
		{
			if (play02_debugcluster_var < 1)
				{
					Color.rgb *= DebugTerrainClearance(WorldXYZ.xzy, WorldNormal);			// tile clearance data
				}
			else
				{
					Color.rgb *= DebugTerrainClusters(WorldXYZ.xzy, WorldNormal);			// cluster area pattern
				}
		}
	else
		{
			Color.rgb *= DebugTerrainTiles(WorldXYZ.xz);											// tile pattern
		}	
	
	return Color;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////