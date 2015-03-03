////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Model PIXEL V1 shader for exponential shadowmapping - MapBuilder2
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
// float3 SunDir			: TEXCOORD3;
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
#ifdef PER_PIXEL_LIGHTS
	float3 WorldPos		: TEXCOORD4;
#endif
//	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

//------------------------------------------------------------------------------------------------

struct inModelPS2
{
	float4 Color			: COLOR0;  
   float2 Tex				: TEXCOORD0; 
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
#ifdef PER_PIXEL_LIGHTS_LOD
	float3 WorldNormal	: TEXCOORD2;
	float3 WorldPos		: TEXCOORD4;
#endif   
//	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

float4 ModelPS(inModelPS In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
//   float Shadow = clamp( PS_shadowmapping(In.Shadow, scsm_esm_obj_var), 1-scsm_run_var, 1);
    
	//----------------------------------------------------------
   // normalize input vectors
   
//	In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal = normalize(In.WorldNormal);
   
   //----------------------------------------------------------
   // Sun light
   
   float LightIntensity = saturate(dot(In.WorldNormal, -vecSunDir));																// no saturate needed when normalized vectors used
      
   // diffuse   
   float4 Diffuse = scsm_brightness * 5.0 * vecDiffuse * LightIntensity * vecSunColor * vecLight;						// vecSunDir is in view space thus not good here							
	
	// specular										
	float4 Specular = 0;
	if (LightIntensity > 0.25f)																												// 0.50 would be perfect with shadows, but it must be lowered to look fine with flat shading
		{
			float3 Halfway  = saturate(dot(normalize(In.ViewDir - vecSunDir), In.WorldNormal));	
					
			Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor * vecLight;					
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
	
	for (int i=1; i<iLights; i++)																				// add max 8 dynamic lights, ignore the last which is the Sun
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
	
	Lights *= 2.0f * vecDiffuse;																
	
	#ifdef DEBUG_TILE																
		Color.rgb *= DebugTerrain(In.WorldPos, In.WorldNormal);
	#endif 
	
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (Diffuse + Specular + Lights + In.Color);											// with per pixel dynamic lights
#else
	Color.rgb *= (Diffuse + Specular + In.Color);														// with per vertex dynamic lights
#endif
	
	//----------------------------------------------------------
	// add shadows
	
//	Color.rgb *= Shadow;																							// dynamic shadows
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);													// sm 3.0 fog support	
	
	//----------------------------------------------------------
	// add sky light	
{}
#ifdef SKY_LIGHT	
	Color.rgb = lerp(Color.rgb, sky_skylightcolor_var, In.SkyLight);		
#endif
	
	//----------------------------------------------------------
	
	return Color;
}


//------------------------------------------------------------------------------------------------
// pixel shader for vertex shading - only for lod3

float4 ModelPS2(inModelPS2 In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
//	float Shadow = clamp( PS_shadowmapping(In.Shadow, scsm_esm_obj_var), 1-scsm_run_var, 1);		// with loop run check
//   
//   In.Color *= Shadow;
   
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
   //----------------------------------------------------------
   // dynamic lights except Sun
{}
#ifdef PER_PIXEL_LIGHTS_LOD
	float4 Lights = 0;
	
	for (int i=1; i<iLights; i++)																				// add max 8 dynamic lights, ignore the last which is the Sun
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
	
	Lights *= 2.0f * vecDiffuse;																
	
	#ifdef DEBUG_TILE																
		Color.rgb *= DebugTerrain(In.WorldPos, In.WorldNormal);
	#endif 
	
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (In.Color + Lights);
#else	
	Color.rgb *= (In.Color);																					// with per vertex dynamic lights
#endif
	
	//----------------------------------------------------------
	// add shadows
	
//	Color.rgb *= Shadow;																							// dynamic shadows
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);													// sm 3.0 fog support	
	
	//----------------------------------------------------------
	// add sky light	
{}
#ifdef SKY_LIGHT	
	Color.rgb = lerp(Color.rgb, sky_skylightcolor_var, In.SkyLight);		
#endif
	
	//----------------------------------------------------------
	
	return Color;
}