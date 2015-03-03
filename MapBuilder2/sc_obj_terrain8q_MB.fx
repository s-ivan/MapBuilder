
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

//texture entSkin2;	// global normalmap (optional specularmap on alpha)
texture entSkin3;		// rgba mask1
texture entSkin4;		// rgba mask2

texture mtlSkin1;		// horizontal quad tile colormap 1
//texture mtlSkin2;	// horizontal quad tile normalmap 1 (optional specularmap on alpha)
texture mtlSkin3;		// horizontal quad tile colormap 2
//texture mtlSkin4;	// horizontal quad tile normalmap 2 (optional specularmap on alpha)

texture LightMap;		// global shadowmap rgb

//----------------------------------------------

sampler colorSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//sampler globalNormSampler = sampler_state 
//{ 
//   Texture = <entSkin2>; 
//   MinFilter = Linear;
//	MagFilter = Linear;
//	MipFilter = Linear;
//}; 

//----------------------------------------------

sampler maskSampler1 = sampler_state 
{ 
   Texture = <entSkin3>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

sampler maskSampler2 = sampler_state 
{ 
   Texture = <entSkin4>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

//----------------------------------------------

sampler tileColorSampler1 = sampler_state 
{ 
   Texture = <mtlSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

//sampler normSampler1 = sampler_state 
//{ 
//   Texture = <mtlSkin2>; 
//   MinFilter = Linear;
//	MagFilter = Linear;
//	MipFilter = Linear;
//}; 

sampler tileColorSampler2 = sampler_state 
{ 
   Texture = <mtlSkin3>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

//sampler normSampler2 = sampler_state 
//{ 
//   Texture = <mtlSkin4>; 
//   MinFilter = Linear;
//	MagFilter = Linear;
//	MipFilter = Linear;
//}; 

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

/////////////////////////////////////////////////////////////////////
// pixel shaders

float4 TerrainA8LmPS(inTerrainPS In) : COLOR0
{
	//----------------------------------------------------------
   // shadowmapping
   
   float Shadow = clamp( PS_shadowmapping(In.Shadow, scsm_esm_var), 1-scsm_run_var, 1);
   
   //----------------------------------------------------------
   // normalize input vectors
   
//	In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal	= normalize(In.WorldNormal);
   
	//----------------------------------------------------------
	// color
	
	float4 color = tex2D(colorSampler, In.Tex);
	
	//----------------------------------------------------------
	// tiling
	
	float4 colorMask1 = tex2D(maskSampler1, In.Tex);
	float4 colorMask2 = tex2D(maskSampler2, In.Tex);
	
	float2 Tile = frac( In.Tex * map_detailsize_ground_var );									// *** tiling
	Tile.x      = frac(Tile.x)*(0.25-2*0.004)+0.004;												// keep y 1:1, iterate only x		,	or 0.004 to 0.002 ?	
	
	//----------------------------------------------------------
	// tile maps
	
	float4 color1 = tex2D(tileColorSampler1, Tile						) * colorMask1.r;		//    0   , 0
	float4 color2 = tex2D(tileColorSampler1, Tile+float2(0.25, 0.0)) * colorMask1.g;		// or 0.25, 0
	float4 color3 = tex2D(tileColorSampler1, Tile+float2(0.50, 0.0)) * colorMask1.b;		// or 0.5 , 0
	float4 color4 = tex2D(tileColorSampler1, Tile+float2(0.75, 0.0)) * colorMask1.a;		// or 0.75, 0
	
	float4 color5 = tex2D(tileColorSampler2, Tile						) * colorMask2.r;		//    0   , 0
	float4 color6 = tex2D(tileColorSampler2, Tile+float2(0.25, 0.0)) * colorMask2.g;		// or 0.25, 0
	float4 color7 = tex2D(tileColorSampler2, Tile+float2(0.50, 0.0)) * colorMask2.b;		// or 0.5 , 0
	float4 color8 = tex2D(tileColorSampler2, Tile+float2(0.75, 0.0)) * colorMask2.a;		// or 0.75, 0
	
	//----------------------------------------------------------
	// tile blending weight
	
	float summedweight = colorMask1.r + colorMask1.g + colorMask1.b + colorMask1.a + colorMask2.r + colorMask2.g + colorMask2.b + colorMask2.a;
	
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
	// texture
	
	// original
//	Color.rgb = lerp(color.rgb, clamp((color1.rgb*color1.a + color2.rgb*color2.a + color3.rgb*color3.a + color4.rgb*color4.a + color5.rgb*color5.a + color6.rgb*color6.a + color7.rgb*color7.a)/summedweight,0,1), map_blendfactor_ground_var);										
   
   // new - weights calculated previously
   float4 Color = lerp(color, saturate((color1 + color2 + color3 + color4 + color5 + color6 + color7 + color8)/summedweight), map_blendfactor_ground_var);			//lerp(x,y,s) = x*(1-s) + y*s;	 0.67 means stronger tile blending
	
	//----------------------------------------------------------
   // dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	
	float4 Lights = 0;
	
	for (int i=1; i<iLights; i++)
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
	
	Lights *= 2.0 * vecDiffuse;																
	
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

float4 TerrainA8PS(inTerrainPS In) : COLOR0
{
	//----------------------------------------------------------
   // shadowmapping
   
   float Shadow = clamp( PS_shadowmapping(In.Shadow, scsm_esm_var), 1-scsm_run_var, 1);
   
   //----------------------------------------------------------
   // normalize input vectors
   
//	In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal	= normalize(In.WorldNormal);
   
	//----------------------------------------------------------
	// color
	
	float4 color = tex2D(colorSampler, In.Tex);
	
	//----------------------------------------------------------
	// tiling
	
	float4 colorMask1 = tex2D(maskSampler1, In.Tex);
	float4 colorMask2 = tex2D(maskSampler2, In.Tex);
	
	float2 Tile = frac( In.Tex * map_detailsize_ground_var );									// *** tiling
	Tile.x      = frac(Tile.x)*(0.25-2*0.004)+0.004;												// keep y 1:1, iterate only x		,	or 0.004 to 0.002 ?	
	
	//----------------------------------------------------------
	// tile maps
	
	float4 color1 = tex2D(tileColorSampler1, Tile						) * colorMask1.r;		//    0   , 0
	float4 color2 = tex2D(tileColorSampler1, Tile+float2(0.25, 0.0)) * colorMask1.g;		// or 0.25, 0
	float4 color3 = tex2D(tileColorSampler1, Tile+float2(0.50, 0.0)) * colorMask1.b;		// or 0.5 , 0
	float4 color4 = tex2D(tileColorSampler1, Tile+float2(0.75, 0.0)) * colorMask1.a;		// or 0.75, 0
	
	float4 color5 = tex2D(tileColorSampler2, Tile						) * colorMask2.r;		//    0   , 0
	float4 color6 = tex2D(tileColorSampler2, Tile+float2(0.25, 0.0)) * colorMask2.g;		// or 0.25, 0
	float4 color7 = tex2D(tileColorSampler2, Tile+float2(0.50, 0.0)) * colorMask2.b;		// or 0.5 , 0
	float4 color8 = tex2D(tileColorSampler2, Tile+float2(0.75, 0.0)) * colorMask2.a;		// or 0.75, 0
	
	//----------------------------------------------------------
	// tile blending weight
	
	float summedweight = colorMask1.r + colorMask1.g + colorMask1.b + colorMask1.a + colorMask2.r + colorMask2.g + colorMask2.b + colorMask2.a;
	
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
	// texture
	
	// original
//	Color.rgb = lerp(color.rgb, clamp((color1.rgb*color1.a + color2.rgb*color2.a + color3.rgb*color3.a + color4.rgb*color4.a + color5.rgb*color5.a + color6.rgb*color6.a + color7.rgb*color7.a)/summedweight,0,1), map_blendfactor_ground_var);										
   
   // new - weights calculated previously
   float4 Color = lerp(color, saturate((color1 + color2 + color3 + color4 + color5 + color6 + color7 + color8)/summedweight), map_blendfactor_ground_var);			//lerp(x,y,s) = x*(1-s) + y*s;	 0.67 means stronger tile blending
	
	//----------------------------------------------------------
   // dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	
	float4 Lights = 0;
	
	for (int i=1; i<iLights; i++)
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
	
	Lights *= 2.0 * vecDiffuse;																
	
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
		PixelShader = compile ps_3_0 TerrainA8PS();
	}
}

technique terrain_lm
{
	pass p0
	{
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
	   
		VertexShader = compile vs_3_0 ModelVS();
		PixelShader = compile ps_3_0 TerrainA8LmPS();
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