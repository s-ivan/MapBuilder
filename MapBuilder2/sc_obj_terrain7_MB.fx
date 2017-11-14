
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

//float4 vecLight;
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
texture entSkin3;		// tile map2
texture entSkin4;		// tile map3

texture mtlSkin1;		// tile map4
texture mtlSkin2;		// tile map5
texture mtlSkin3;		// tile map6
texture mtlSkin4;		// tile map7

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

sampler tileSampler2 = sampler_state 
{ 
   Texture = <entSkin3>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = mirror;
	AddressV = mirror;
}; 

sampler tileSampler3 = sampler_state 
{ 
   Texture = <entSkin4>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = mirror;
	AddressV = mirror;
}; 

sampler tileSampler4 = sampler_state 
{ 
   Texture = <mtlSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = mirror;
	AddressV = mirror;
}; 

sampler tileSampler5 = sampler_state 
{ 
   Texture = <mtlSkin2>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = mirror;
	AddressV = mirror;
}; 

sampler tileSampler6 = sampler_state 
{ 
   Texture = <mtlSkin3>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = mirror;
	AddressV = mirror;
}; 

sampler tileSampler7 = sampler_state 
{ 
   Texture = <mtlSkin4>; 
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
// structs PS

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
   
   float Shadow = clamp( PS_shadowmapping(In.Shadow, scsm_esm_var), 1-scsm_run_var, 1);
   
   //----------------------------------------------------------
   // normalize input vectors
   
//	In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal = normalize(In.WorldNormal);
   
   //----------------------------------------------------------
   // Sun lighting
   
   float LightIntensity = saturate(dot(In.WorldNormal, -vecSunDir));
      
   // diffuse   
   float4 Diffuse = 1.0 * vecDiffuse * LightIntensity * vecSunColor;																		
	
	// specular										
	float4 Specular = 0;
//	if (LightIntensity > 0.25f)
		{
			float3 Halfway  = saturate(dot(normalize(In.ViewDir - vecSunDir), In.WorldNormal));			
			Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor;					
		}
	// specular - another calculation
//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - In.SunDir);													// R
//	float4 Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor;			// R.V^n 													
	
	//----------------------------------------------------------
	// color
	
	float4 color = tex2D(colorSampler, In.Tex);
	
	//----------------------------------------------------------
	// tiling
	
	float2 Tile = frac( In.Tex * map_detailsize_ground_var );
	
	//----------------------------------------------------------
	// tile maps
	
	float4 color1 = tex2D(tileSampler1, Tile);	
//	color1.a 	  = tex2D(tileSampler1, In.Tex).a;	// calculated
		
	float4 color2 = tex2D(tileSampler2, Tile);	
	color2.a 	  = tex2D(tileSampler2, In.Tex).a;
		
	float4 color3 = tex2D(tileSampler3, Tile);
	color3.a 	  = tex2D(tileSampler3, In.Tex).a;
		
	float4 color4 = tex2D(tileSampler4, Tile);		
	color4.a 	  = tex2D(tileSampler4, In.Tex).a;
	
	float4 color5 = tex2D(tileSampler5, Tile);		
	color5.a 	  = tex2D(tileSampler5, In.Tex).a;
	
	float4 color6 = tex2D(tileSampler6, Tile);		
	color6.a 	  = tex2D(tileSampler6, In.Tex).a;
	
	float4 color7 = tex2D(tileSampler7, Tile);		
	color7.a 	  = tex2D(tileSampler7, In.Tex).a;
	
	//----------------------------------------------------------
	// tile blending weight
	
	float summedweight = color2.a + color3.a + color4.a + color5.a + color6.a + color7.a;
	
	// the first color tilemap should have full cover, this is compensated now
	color1.a = saturate(1 - summedweight); 
	
	summedweight += color1.a;
	
	//----------------------------------------------------------	
	// texture
	
   float4 Color = 1;
   Color.rgb = lerp(color.rgb, 
   									saturate(
														(
															color1.rgb * color1.a
														 + color2.rgb * color2.a
														 + color3.rgb * color3.a 
														 + color4.rgb * color4.a 
														 + color5.rgb * color5.a 
														 + color6.rgb * color6.a 
														 + color7.rgb * color7.a
														) 
														/ summedweight
													), 
										map_blendfactor_ground_var
						 );																						//lerp(x,y,s) = x*(1-s) + y*s;	 0.67 means stronger tile blending
	
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
	// lightmap = static shadowmap = 0.5 * lightmap
	
	float4 Lightmap = 2.0 * tex2D(LightMapSampler, In.Tex);										// 1.0 * for L3DT lightmap, 2.0 * for 3dgs lightmap
	Color.rgb *= clamp(Lightmap, scsm_lightmapalpha_var, 1);
	
	//----------------------------------------------------------
	// add shadows
	
//	Color.rgb *= lerp(scsm_shadowcolor.rgb, 1, Shadow);											// dynamic shadows
	Color.rgb *= Shadow;
	
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
	
	return float4(Color.rgb, 1);
}

////////////////////////////////////////////////////////////////////////////////////////////////

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
   // lighting
   
   float LightIntensity = saturate(dot(In.WorldNormal, -vecSunDir));
      
   // diffuse   
   float4 Diffuse = 1.0 * vecDiffuse * LightIntensity * vecSunColor;																	
	
	// specular										
	float4 Specular = 0;
//	if (LightIntensity > 0.25f)
		{
			float3 Halfway  = saturate(dot(normalize(In.ViewDir - vecSunDir), In.WorldNormal));			
			Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor;			
		}
	// specular - another calculation
//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - In.SunDir);													// R
//	float4 Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor;			// R.V^n 												
	
	//----------------------------------------------------------
	// color
	
	float4 color = tex2D(colorSampler, In.Tex);
	
	//----------------------------------------------------------
	// tiling
	
	float2 Tile = frac( In.Tex * map_detailsize_ground_var );
	
	//----------------------------------------------------------
	// tile maps
	
	float4 color1 = tex2D(tileSampler1, Tile);	
//	color1.a 	  = tex2D(tileSampler1, In.Tex).a;	// calculated
		
	float4 color2 = tex2D(tileSampler2, Tile);	
	color2.a 	  = tex2D(tileSampler2, In.Tex).a;
		
	float4 color3 = tex2D(tileSampler3, Tile);
	color3.a 	  = tex2D(tileSampler3, In.Tex).a;
		
	float4 color4 = tex2D(tileSampler4, Tile);		
	color4.a 	  = tex2D(tileSampler4, In.Tex).a;
	
	float4 color5 = tex2D(tileSampler5, Tile);		
	color5.a 	  = tex2D(tileSampler5, In.Tex).a;
	
	float4 color6 = tex2D(tileSampler6, Tile);		
	color6.a 	  = tex2D(tileSampler6, In.Tex).a;
	
	float4 color7 = tex2D(tileSampler7, Tile);		
	color7.a 	  = tex2D(tileSampler7, In.Tex).a;
	
	//----------------------------------------------------------
	// tile blending weight
	
	float summedweight = color2.a + color3.a + color4.a + color5.a + color6.a + color7.a;
	
	// the first color tilemap should have full cover, this is compensated now
	color1.a = saturate(1 - summedweight); 
	
	summedweight += color1.a;
	
	//----------------------------------------------------------	
	// texture
	
   float4 Color = 1;
   Color.rgb = lerp(color.rgb, 
   									saturate(
														(
															color1.rgb * color1.a
														 + color2.rgb * color2.a
														 + color3.rgb * color3.a 
														 + color4.rgb * color4.a 
														 + color5.rgb * color5.a 
														 + color6.rgb * color6.a 
														 + color7.rgb * color7.a
														) 
														/ summedweight
													), 
										map_blendfactor_ground_var
						 );																						//lerp(x,y,s) = x*(1-s) + y*s;	 0.67 means stronger tile blending
			
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
	// add shadows
	
//	Color.rgb *= lerp(scsm_shadowcolor.rgb, 1, Shadow);											// dynamic shadows
	Color.rgb *= Shadow;
	
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
	
	return float4(Color.rgb, 1);
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