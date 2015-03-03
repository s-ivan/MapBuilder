////////////////////////////////////////////////////////////////////////////////////////////////
// (c) Ivan Stummer
// MapBuilder 2
// v.1.0
////////////////////////////////////////////////////////////////////////////////////////////////
// custom matrices, vectors, variables

float sky_hfogminheight_var;				// -50	(-1000..1000)
float sky_hfogmaxheight_var;				// 250	(0..clip_far)
float sky_hfogheightstrength_var;		// 0.75	(0..1)
float sky_hfogheightblending_var;		// 4.0f	(0..32)
float sky_hfogheightdensity_var;			// 0.5	(0.001..1)		decreases fade-out linearity
float sky_hfogheightmin_var;				// 0.25	(0..0.5)

////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader calculations


float VS_heightfog(float3 PosWorld)
{
	// basic linear distance fog - saturate needed when fadeout distance is closer than clipping distance
	float distfog = saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );
      
	// height fog, default: 0.75 fog strength, 250 fog height
	float heightfog = ( (sky_hfogmaxheight_var - PosWorld.y) / (sky_hfogheightdensity_var * (sky_hfogmaxheight_var - sky_hfogminheight_var)) );	// saturate removed
   
	float blendfog = saturate( max(distfog, sky_hfogheightstrength_var * heightfog) );
//	float blendfog = saturate(distfog + heightfog);
   
   //---------------------------------------------
   
	// test outputs
//	return distfog;	
//	return heightfog;
//	return blendfog;
	
	// blend base distance fog with blendfog to not to allow height fog to cover close areas, apply 2nd strength value
	return vecFogColor.w * lerp(distfog, blendfog, saturate(distfog * sky_hfogheightblending_var + sky_hfogheightmin_var));
}


/////////////////////////////////////////////////////////


float VS_heightfog_Water(float3 PosWorld)
{
	// basic linear distance fog - saturate needed when fadeout distance is closer than clipping distance
	float distfog = saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );
      
	// height fog, default: 0.75 fog strength, 250 fog height
	float heightfog = ( (sky_hfogmaxheight_var - PosWorld.y) / (sky_hfogheightdensity_var * (sky_hfogmaxheight_var - sky_hfogminheight_var)) );	// saturate removed
   
	float blendfog = saturate( max(distfog, sky_hfogheightstrength_var * heightfog) );
//	float blendfog = saturate(distfog + heightfog);
   
   //---------------------------------------------
   
	// test outputs
//	return distfog;	
//	return heightfog;
//	return blendfog;
	
	// blend base distance fog with blendfog to not to allow height fog to cover close areas, apply 2nd strength value
	return vecFogColor.w * lerp(distfog, blendfog, saturate(distfog * sky_hfogheightblending_var + sky_hfogheightmin_var/2.0f));
}


/////////////////////////////////////////////////////////


float VS_heightfog_SM2(float3 PosWorld)
{
	// basic linear distance fog - saturate needed when fadeout distance is closer than clipping distance
	float distfog = saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );
      
	// height fog, default: 0.75 fog strength, 250 fog height
	float heightfog = ( (sky_hfogmaxheight_var - PosWorld.y) / (sky_hfogheightdensity_var * (sky_hfogmaxheight_var - sky_hfogminheight_var)) );	// saturate removed
   
	float blendfog = saturate( max(distfog, sky_hfogheightstrength_var * heightfog) );
//	float blendfog = saturate(distfog + heightfog);
   
   //---------------------------------------------
   
	// test outputs
//	return distfog;	
//	return heightfog;
//	return blendfog;
	
	// blend base distance fog with blendfog to not to allow height fog to cover close areas, apply 2nd strength value
	return lerp(distfog, blendfog, saturate(distfog * sky_hfogheightblending_var + sky_hfogheightmin_var));
}


////////////////////////////////////////////////////////////////////////////////////////////////