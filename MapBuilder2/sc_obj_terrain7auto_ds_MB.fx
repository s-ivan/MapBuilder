
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

float4 vecTime;	

////////////////////////////////////////////////////////////////////////////////////////////////
// custom matrices, vectors, variables

float map_detailsize_ground_var 	= 40.0f;				// like detail_size
float map_blendfactor_ground_var	= 0.5f;				// tile blending strength over base colormap

//--------------------------------------------

float basecolor_tiling_var		= 1.0f;					// tiling of base colormap

float grass_height_border_var = 512.0f;				// WorldPosition.y value
float grass_height_width_var 	= 256.0f;				// height transition width

float shore_height_width_var 	= 16.0f;					// -WorldPosition.y (start of underwater) and WorldPosition.y (start of grass1) values as borders

float cliff_slope_min_var 		= 67.0f;					// 0..90 min slope in degrees
float cliff_slope_width_var	= 5.0f;					// 1..15 degree of transiency

//float caustics_height_mid	= -256.0f;				// WorldPosition.y value
//float caustics_width			= 128.0f;				// WorldPosition.y 3/- borders measured from mid

////////////////////////////////////////////////////////////////////////////////////////////////
// skins/textures/samplers

texture entSkin1; 	// global colormap														ok

texture entSkin2;		// tile map1		grass1 - height max								ok
texture entSkin3;		// tile map2		grass2 - height min								ok
texture entSkin4;		// tile map3		cliff - slope min									ok

texture mtlSkin1;		// tile map4		shore - height min max							ok
texture mtlSkin2;		// tile map5		underwater - heigth max							ok
texture mtlSkin3;		// tile map6		snow/path - alpha mask weigthing				ok
texture mtlSkin4;		// tile map7		snow/path - alpha mask weigthing				ok

texture LightMap;		// global shadowmap rgb													ok

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
// structs VS

struct outModelVS
{
	float4 Pos				: POSITION;
	float4 Color			: COLOR0;    
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
//	float3 SunDir			: TEXCOORD3;
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
	float3 WorldPos		: TEXCOORD4;
	float4 Shadow			: TEXCOORD7;
   float  Fog 				: FOG;
};

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
	float3 WorldPos		: TEXCOORD4;
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

outModelVS ModelVS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0
	) 
{ 
	outModelVS Out;
	
//	InPos.w = 1.0f;																			// to get proper matrix multiplication?
	
	//----------------------------------------------------------------
	
	Out.Tex = InTex;
	
	//---------------------------------------------------------------
	
	float4 PosWorld = mul(InPos, matWorld);											// needed for a couple of calculations... float4 for shadowmapping!
	
	//----------------------------------------------------------------
	
	Out.Pos = mul(InPos, matWorldViewProj); 
	
	//----------------------------------------------------------------
	
  	Out.WorldPos = PosWorld.xyz;															// float3 !
	
	//----------------------------------------------------------------
	
	Out.ViewDir = vecViewPos.xyz - PosWorld.xyz;										// for flat specular - or = matViewInv[3].xyz - PosWorld;	= halfway vector
	
	//----------------------------------------------------------------
	
//	Out.SunDir = PosWorld.xyz - vecSunPos;
	
	//----------------------------------------------------------------
	
	Out.WorldNormal = normalize(mul(InNormal, (float3x3)matWorld));			// when no normalmap used - normalize because of entity scale!
	
	//----------------------------------------------------------------
{}
#ifndef HEIGHT_FOG   
	Out.Fog = vecFogColor.w																									// fog is applied
				 * saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );					// saturate needed when fadeout distance is closer than clipping distance
#else
	Out.Fog = VS_heightfog(PosWorld.xyz);
#endif
	
	//----------------------------------------------------------------
	
#ifdef SKY_LIGHT
	Out.SkyLight = saturate( (distance(PosWorld, vecViewPos) - sky_skylightstart_var) / (sky_skylightend_var - sky_skylightstart_var) );
#endif
	
	//----------------------------------------------------------------
	
	Out.Shadow = VS_shadowmapping(PosWorld);
	
	//----------------------------------------------------------------
	// lighting
	
	// final color	
	Out.Color = (vecAmbient + vecEmissive) + vecColor;								// ambient + emissive + rgb
		
{}
#ifndef PER_PIXEL_LIGHTS
	
	float4 Lights = 0;	
			
	for (int i=1; i<iLights; i++)  														// add dynamic lights, ignore Sun (always the last one)
		Lights += PointLightDiffuse(PosWorld, Out.WorldNormal, i-1);			
	
	Lights *= 2.0f * vecDiffuse;																
	
	// final color	
	Out.Color += Lights;
	
#endif				
	
	//-----------------------------------------------------------------
	
	return Out;
}

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
	
	float4 color = tex2D(colorSampler, frac(In.Tex * basecolor_tiling_var));
	
	//----------------------------------------------------------
	// tiling
	
	float2 Tile = frac( In.Tex * map_detailsize_ground_var );
	
	//----------------------------------------------------------
	// tile maps
	
	float4 color1 = tex2D(tileSampler1, Tile);	
//	color1.a 	  = tex2D(tileSampler1, In.Tex).a;	// calculated
		
	float4 color2 = tex2D(tileSampler2, Tile);	
//	color2.a 	  = tex2D(tileSampler2, In.Tex).a;
		
	float4 color3 = tex2D(tileSampler3, Tile);
//	color3.a 	  = tex2D(tileSampler3, In.Tex).a;
		
	float4 color4 = tex2D(tileSampler4, Tile);		
//	color4.a 	  = tex2D(tileSampler4, In.Tex).a;
	
	float4 color5 = tex2D(tileSampler5, Tile);		
//	color5.a 	  = tex2D(tileSampler5, In.Tex).a;
	
//	float4 color6 = tex2D(tileSampler6, frac( Tile * (sin(vecTime.w * 0.1f) * 0.5f + 1.0f) ));
	float4 color6 = tex2D(tileSampler6, Tile);					
	color6.a 	  = tex2D(tileSampler6, In.Tex).a;
	
	float4 color7 = tex2D(tileSampler7, Tile);		
	color7.a 	  = tex2D(tileSampler7, In.Tex).a;
	
	//----------------------------------------------------------
	// auto calculate blending weights instead of reading from alpha		
	
	// height dependent
	
	// grass 2 - fade in based on height
	color2.a		= saturate( (In.WorldPos.y - grass_height_border_var) / (grass_height_width_var) );
	
	// grass 1 - inverse of grass2
	color1.a		= 1.0f - color2.a;
	
	// shore
	color4.a		= 1.0f - saturate( abs( (In.WorldPos.y) / (shore_height_width_var) ) );
	
	// limit all the previous ones
	color1.a		= min(color1.a, 1.0f - color4.a);
	color2.a		= min(color2.a, 1.0f - color4.a);
	
	// underwater
	color5.a		= 1.0f - saturate( (In.WorldPos.y + shore_height_width_var) / (shore_height_width_var) );
	
	// limit grass1
	color1.a		= min(color1.a, 1.0f - color5.a);
	
//	// caustics			
//	color6.a		= 1.0f - saturate( ( - In.WorldPos.y + caustics_height_mid + caustics_width) / (caustics_height_mid + caustics_width) );
	
	// slope dependent
		
	// cliff
	float slope = 1.57 - acos( dot(In.WorldNormal, float3(0.0f,1.0f,0.0f)) );												// 90 - alpha
	color3.a 	= 1.0 - saturate( (slope - (1.57 - cliff_slope_min_var * 0.0085)) / (cliff_slope_width_var * 0.017) );		
//	float slope = dot(In.WorldNormal, float3(0.0f,1.0f,0.0f));	
//	color3.a 	= 1.0f - saturate( (slope - (1.0 - cliff_slope_min_var / 360.0)) / (cliff_slope_width_var * 0.017) );
		
	// limit all the over-water ones
	color1.a		= min(color1.a, 1.0f - color3.a);
	color2.a		= min(color2.a, 1.0f - color3.a);
	color4.a		= min(color4.a, 1.0f - color3.a);
	
	// painted (color6-7)
	
	// limit all the previous ones
	color1.a		= min(color1.a, 1.0f - saturate(color6.a + color7.a));
	color2.a		= min(color2.a, 1.0f - saturate(color6.a + color7.a));
	color3.a		= min(color3.a, 1.0f - saturate(color6.a + color7.a));
	color4.a		= min(color4.a, 1.0f - saturate(color6.a + color7.a));
	color5.a		= min(color5.a, 1.0f - saturate(color6.a + color7.a));
	
//	// sample
//	Out.SkyLight = saturate( (distance(PosWorld, vecViewPos) - sky_skylightstart_var) / (sky_skylightend_var - sky_skylightstart_var) );
	
	float summedweight = color1.a + color2.a + color3.a + color4.a + color5.a + color6.a + color7.a;
	
//	//----------------------------------------------------------
//	// tile blending weight
//	
//	float summedweight = color2.a + color3.a + color4.a + color5.a + color6.a + color7.a;
//	
//	// the first color tilemap should have full cover, this is compensated now
//	color1.a = saturate(1 - summedweight); 
//	
//	summedweight += color1.a;
//	
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
//	// lightmap = static shadowmap = 0.5 * lightmap
//	
//	float4 Lightmap = 2.0 * tex2D(LightMapSampler, In.Tex);										// 1.0 * for L3DT lightmap, 2.0 * for 3dgs lightmap
//	Color.rgb *= clamp(Lightmap, scsm_lightmapalpha_var, 1);
//	
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
	
	float4 color = tex2D(colorSampler, frac(In.Tex * basecolor_tiling_var));
	
	//----------------------------------------------------------
	// tiling
	
	float2 Tile = frac( In.Tex * map_detailsize_ground_var );
	
	//----------------------------------------------------------
	// tile maps
	
	float4 color1 = tex2D(tileSampler1, Tile);	
//	color1.a 	  = tex2D(tileSampler1, In.Tex).a;	// calculated
		
	float4 color2 = tex2D(tileSampler2, Tile);	
//	color2.a 	  = tex2D(tileSampler2, In.Tex).a;
		
	float4 color3 = tex2D(tileSampler3, Tile);
//	color3.a 	  = tex2D(tileSampler3, In.Tex).a;
		
	float4 color4 = tex2D(tileSampler4, Tile);		
//	color4.a 	  = tex2D(tileSampler4, In.Tex).a;
	
	float4 color5 = tex2D(tileSampler5, Tile);		
//	color5.a 	  = tex2D(tileSampler5, In.Tex).a;
	
//	float4 color6 = tex2D(tileSampler6, frac( Tile * (sin(vecTime.w * 0.1f) * 0.5f + 1.0f) ));
	float4 color6 = tex2D(tileSampler6, Tile);					
	color6.a 	  = tex2D(tileSampler6, In.Tex).a;
	
	float4 color7 = tex2D(tileSampler7, Tile);		
	color7.a 	  = tex2D(tileSampler7, In.Tex).a;
	
	//----------------------------------------------------------
	// auto calculate blending weights instead of reading from alpha		
	
	// height dependent
	
	// grass 2 - fade in based on height
	color2.a		= saturate( (In.WorldPos.y - grass_height_border_var) / (grass_height_width_var) );
	
	// grass 1 - inverse of grass2
	color1.a		= 1.0f - color2.a;
	
	// shore
	color4.a		= 1.0f - saturate( abs( (In.WorldPos.y) / (shore_height_width_var) ) );
	
	// limit all the previous ones
	color1.a		= min(color1.a, 1.0f - color4.a);
	color2.a		= min(color2.a, 1.0f - color4.a);
	
	// underwater
	color5.a		= 1.0f - saturate( (In.WorldPos.y + shore_height_width_var) / (shore_height_width_var) );
	
	// limit grass1
	color1.a		= min(color1.a, 1.0f - color5.a);
	
//	// caustics			
//	color6.a		= 1.0f - saturate( ( - In.WorldPos.y + caustics_height_mid + caustics_width) / (caustics_height_mid + caustics_width) );
	
	// slope dependent
		
	// cliff
	float slope = 1.57 - acos( dot(In.WorldNormal, float3(0.0f,1.0f,0.0f)) );												// 90 - alpha
	color3.a 	= 1.0 - saturate( (slope - (1.57 - cliff_slope_min_var * 0.0085)) / (cliff_slope_width_var * 0.017) );		
//	float slope = dot(In.WorldNormal, float3(0.0f,1.0f,0.0f));	
//	color3.a 	= 1.0f - saturate( (slope - (1.0 - cliff_slope_min_var / 360.0)) / (cliff_slope_width_var * 0.017) );
		
	// limit all the over-water ones
	color1.a		= min(color1.a, 1.0f - color3.a);
	color2.a		= min(color2.a, 1.0f - color3.a);
	color4.a		= min(color4.a, 1.0f - color3.a);
	
	// painted (color6-7)
	
	// limit all the previous ones
	color1.a		= min(color1.a, 1.0f - saturate(color6.a + color7.a));
	color2.a		= min(color2.a, 1.0f - saturate(color6.a + color7.a));
	color3.a		= min(color3.a, 1.0f - saturate(color6.a + color7.a));
	color4.a		= min(color4.a, 1.0f - saturate(color6.a + color7.a));
	color5.a		= min(color5.a, 1.0f - saturate(color6.a + color7.a));
	
//	// sample
//	Out.SkyLight = saturate( (distance(PosWorld, vecViewPos) - sky_skylightstart_var) / (sky_skylightend_var - sky_skylightstart_var) );
	
	float summedweight = color1.a + color2.a + color3.a + color4.a + color5.a + color6.a + color7.a;
	
//	//----------------------------------------------------------
//	// tile blending weight
//	
//	float summedweight = color2.a + color3.a + color4.a + color5.a + color6.a + color7.a;
//	
//	// the first color tilemap should have full cover, this is compensated now
//	color1.a = saturate(1 - summedweight); 
//	
//	summedweight += color1.a;
//	
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