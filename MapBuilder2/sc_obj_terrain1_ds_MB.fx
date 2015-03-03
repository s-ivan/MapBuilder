
bool AUTORELOAD;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

#define PER_PIXEL_LIGHTS						

////////////////////////////////////////////////////////////////////////////////////////////////
// matrices, vectors, variables passed by the engine

float4x4 matWorldViewProj;
float4x4 matWorld;

float4 vecFog;
float4 vecFogColor;	

float4 vecViewPos;
float4 vecViewDir;

float4 vecSunColor;
float4 vecSunDir;
float4 vecSunPos;

float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;
float  fPower;

////////////////////////////////////////////////////////////////////////////////////////////////
// custom matrices, vectors, variables

float map_detailsize_ground_var 	= 40.0f;			// like detail_size
float map_blendfactor_ground_var	= 0.5f;			// tile blending strength over base colormap

////////////////////////////////////////////////////////////////////////////////////////////////
// skins/textures/samplers

texture entSkin1; 	// global colormap

texture entSkin2;		// tile map1

texture LightMap;		// global shadowmap rgb

//----------------------------------------

sampler colorSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//----------------------------------------

sampler tileSampler1 = sampler_state 
{ 
   Texture = <entSkin2>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = mirror;
	AddressV = mirror;
}; 

//----------------------------------------------

sampler LightMapSampler = sampler_state 
{ 
   Texture = <LightMap>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// includes

#include "sc_defines_MB.fx"
#include "sc_pointlight_MB.fx"
#include "sc_shadowmapping_MB.fx"		// scsm shadowmapping - variables, shadow samplers, vertex shader functions, pixel shader function

#ifdef HEIGHT_FOG
	#include "sc_heightfog_MB.fx"			// blend distance fog with height fog
#endif

#ifdef DEBUG_TILE
	#include "rts_tiledarea_MB.fx"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs

struct inTerrainPS 
{
	float4 Color			: COLOR0;  
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
//	float3 SunDir			: TEXCOORD3;
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
#ifdef PER_PIXEL_LIGHTS
	float3 WorldPos		: TEXCOORD4;	
#endif
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

#include "sc_vs_model_MB.fx"				// vertex shader

////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shaders

// with lightmap
float4 TerrainLmPS(inTerrainPS In) : COLOR0
{
	//----------------------------------------------------------
   // shadowmapping
   
   float Shadow = clamp( PS_shadowmapping_lm(In.Shadow, scsm_esm_var, In.Tex, LightMapSampler), 1-scsm_run_var, 1);
   
   //----------------------------------------------------------
   // normalize input vectors
   
//	In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal = normalize(In.WorldNormal);
   
   //----------------------------------------------------------
   // Sun lighting
   
   float LightIntensity = saturate(dot(In.WorldNormal, -vecSunDir)) * Shadow;
      
   // diffuse   
   float4 Diffuse = 8.0 * vecDiffuse * LightIntensity * vecSunColor * vecLight;					
	
	// specular										
	float4 Specular = 0;
//	if (LightIntensity > 0.25f)
		{
			float3 Halfway  = saturate(dot(normalize(In.ViewDir - vecSunDir), In.WorldNormal));			
			Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor * vecLight;					
		}
	// specular - another calculation
//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - In.SunDir);													// R
//	float4 Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor * vecLight;			// R.V^n 												
	
	//----------------------------------------------------------
	// color
	
	float4 color = tex2D(colorSampler, In.Tex);
	
	//----------------------------------------------------------
	// tiling
	
	float2 Tile = frac( In.Tex * map_detailsize_ground_var );
	
	//----------------------------------------------------------
	// tile maps
	
	float4 color1 = tex2D(tileSampler1, Tile);	
//	color1.a 	  = tex2D(tileSampler1, In.Tex).a;													// not used
		
	//----------------------------------------------------------	
	// texture
	
   float4 Color = 1;
	
	Color.rgb = (color.rgb + color1.rgb) - 0.5;														// ffe compatible
	
	//----------------------------------------------------------
   // dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	
	float4 Lights = 0;
	
	for (int i=1; i<iLights; i++)
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
	
	Lights *= 2.0f * vecDiffuse;																
	
	#ifdef DEBUG_TILE																		
		Color.rgb *= DebugTerrain(In.WorldPos, In.WorldNormal);		
	#endif
	
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (Diffuse + Specular + Lights + In.Color);		
#else
	Color.rgb *= (Diffuse + Specular + In.Color);				
#endif
	
//	//----------------------------------------------------------	
//	// lightmap = static shadowmap = 0.5 * lightmap
//	
//	float4 Lightmap = 2.0 * tex2D(LightMapSampler, In.Tex);										// 1.0 * for L3DT lightmap, 2.0 * for 3dgs lightmap
//	Color.rgb *= clamp(Lightmap, scsm_lightmapalpha_var, 1);
//		
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);			
	
	//----------------------------------------------------------
	// add sky light	
{}
#ifdef SKY_LIGHT	
	Color.rgb = lerp(Color.rgb, sky_skylightcolor_var, In.SkyLight);		
#endif
	
	//----------------------------------------------------------
	
	return float4(Color.rgb,1);
}


// without lightmap
float4 TerrainPS(inTerrainPS In) : COLOR0
{
	//----------------------------------------------------------
   // shadowmapping
   
   float Shadow = clamp( PS_shadowmapping(In.Shadow, scsm_esm_var), 1-scsm_run_var, 1);
   
   //----------------------------------------------------------
   // normalize input vectors
   
//	In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal = normalize(In.WorldNormal);
   
   //----------------------------------------------------------
   // Sun lighting
   
   float LightIntensity = saturate(dot(In.WorldNormal, -vecSunDir)) * Shadow;
      
   // diffuse   
   float4 Diffuse = 8.0 * vecDiffuse * LightIntensity * vecSunColor * vecLight;																	
	
	// specular										
	float4 Specular = 0;
//	if (LightIntensity > 0.25f)
		{
			float3 Halfway  = saturate(dot(normalize(In.ViewDir - vecSunDir), In.WorldNormal));			
			Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor * vecLight;					
		}
	// specular - another calculation
//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - vecSunDir);													// R
//	float4 Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor * vecLight;			// R.V^n 													
	
	//----------------------------------------------------------
	// color
	
	float4 color = tex2D(colorSampler, In.Tex);
	
	//----------------------------------------------------------
	// tiling
	
	float2 Tile = frac( In.Tex * map_detailsize_ground_var );
	
	//----------------------------------------------------------
	// tile maps
	
	float4 color1 = tex2D(tileSampler1, Tile);	
//	color1.a 	  = tex2D(tileSampler1, In.Tex).a;													// not used
	
	//----------------------------------------------------------	
	// texture
	
   float4 Color = 1;
   
	Color.rgb = (color.rgb + color1.rgb) - 0.5;														// ffe compatible
	
	//----------------------------------------------------------
   // dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	
	float4 Lights = 0;
	
	for (int i=1; i<iLights; i++)
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
	
	Lights *= 2.0f * vecDiffuse;																
	
	#ifdef DEBUG_TILE			
		Color.rgb *= DebugTerrain(In.WorldPos, In.WorldNormal);		
	#endif	
	
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (Diffuse + Specular + Lights + In.Color);		
#else
	Color.rgb *= (Diffuse + Specular + In.Color);				
#endif
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);									
	
	//----------------------------------------------------------
	// add sky light	
{}
#ifdef SKY_LIGHT	
	Color.rgb = lerp(Color.rgb, sky_skylightcolor_var, In.SkyLight);		
#endif
	
	//----------------------------------------------------------
	
	return float4(Color.rgb,1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// _lodx and _lmx cannot be utilized

technique terrain
{
	pass p0
	{
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
		
		VertexShader = compile vs_3_0 ModelVS();
		PixelShader  = compile ps_3_0 TerrainPS();
	}
}

technique terrain_lm
{
	pass p0
	{		
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
		
		VertexShader = compile vs_3_0 ModelVS();
		PixelShader  = compile ps_3_0 TerrainLmPS();
	}
}

//////////////////////////////////////////

technique fallback 
{ 
	pass 
	{
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
	} 
}