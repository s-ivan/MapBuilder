////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Model PIXEL V2 shader for exponential shadowmapping - MapBuilder2
// Copyright Iván Stummer (2014) All Rights Reserved
// 
////////////////////////////////////////////////////////////////////////////////////////////////
// 
// supports 8 dynamic lights (including Sun)
// diffuse calculation for each light
// specular calculation only for Sun
// flat shading with per vertex (faster) or per pixel (nicer) calculations
// (compatible light strength with other sc_obj shaders)
//
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// skins/textures

texture entSkin1; 							// Colormap

// Color map sampler
sampler colorSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs PS

struct inModelPS 
{
	float4 Color			: COLOR0;  
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
   float3 SunDir			: TEXCOORD3;
#ifdef PER_PIXEL_LIGHTS
	float3 WorldPos		: TEXCOORD4;
#endif
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

float4 Model2PS(inModelPS In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
   float shadow1 = clamp( PS_shadowmapping(In.Shadow, scsm_esm_obj_var), 1-scsm_run_var, 1);
    
	//----------------------------------------------------------
   // normalize input vectors
   
   In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal = normalize(In.WorldNormal);
   
   //----------------------------------------------------------
   // Sun light
   
   float LightIntensity = dot(In.WorldNormal, -In.SunDir);																			// no saturate needed when normalized vectors used
   	
	// specular										
	float4 Specular = 0;
	if (LightIntensity * shadow1 > 0.50f)
		{
			float3 Halfway  = saturate(dot(normalize(In.ViewDir - In.SunDir), In.WorldNormal));			
			Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor * vecLight;					
			
//			// spec mapping
//			Specular = min( Specular , SpecularMap ); 																					// or Specular * SpecularMap
		}
	// specular - another calculation
//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - In.SunDir);												// R
//	float4 Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor * vecLight;		// R.V^n 												
	
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
   //----------------------------------------------------------
   // dynamic lights except Sun
{}
#ifdef PER_PIXEL_LIGHTS	
	
	float4 Lights = 0;
	
	for (int i=1; i<iLights; i++)																				// add max 8 dynamic lights, except the last which is the Sun
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
	
	Lights += 1.5f * PointLightDiffuse(In.WorldPos, In.WorldNormal, iLights-1);				// add Sun that increased significance that matches other shaders
	
	Lights *= 2.0 * vecDiffuse;																				// enbrighten to match vertex light look
	
	#ifdef DEBUG_TILE																
		if (play02_bmap_ready_var > 0)
			{
				if (play02_debugcluster_var < 1)
					{
						Color.rgb *= DebugTerrainClearance(In.WorldPos.xzy, In.WorldNormal);		// tile clearance data
					}
				else
					{
						Color.rgb *= DebugTerrainClusters(In.WorldPos.xzy, In.WorldNormal);		// cluster area pattern
					}
			}
		else
			{
				Color.rgb *= DebugTerrain (In.WorldPos.xz);												// tile pattern
			}
	#endif // DEBUG_TILE	
	
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (Specular + Lights + In.Color);															// with per pixel dynamic lights, InColor contains only entity ambient fAmbient
#else
	Color.rgb *= (Specular + In.Color);																		// with per vertex dynamic lights
#endif
	
	//----------------------------------------------------------
	// add shadows
	
	Color.rgb *= shadow1;																						// dynamic shadows, add if loop runs
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);													// sm 3.0 fog support	
	
	//----------------------------------------------------------
	
	return Color;
}
