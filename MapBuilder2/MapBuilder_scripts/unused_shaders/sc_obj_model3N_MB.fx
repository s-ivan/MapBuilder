////////////////////////////////////////////////////////////////////////////////////////////////
// 
// NORMALMAPPING MODEL shader for exponential shadowmapping - MapBuilder2 - MAIN V3
// Copyright Iván Stummer (2014) All Rights Reserved
// 
////////////////////////////////////////////////////////////////////////////////////////////////
// 
// supports 8 dynamic lights (including Sun)
// diffuse and specular calculation for each light
// flat shading with per vertex (faster) or per pixel (nicer) calculations
// (compatible light strength with other sc_obj shaders)
//
////////////////////////////////////////////////////////////////////////////////////////////////

bool AUTORELOAD;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

#define		PER_PIXEL_LIGHTS		// dynamic lights are flat with vertex light, except SUn

#define		HEIGHT_FOG				// a vertical fog is blended to distance fog

//#define	DEBUG_TILE				// RTS related

////////////////////////////////////////////////////////////////////////////////////////////////
// matrices, vectors, variables passed by the engine

float4x4 matWorldViewProj;
float4x4 matWorld;

float4 vecFog;
float4 vecFogColor;	

float4 vecViewPos;
float4 vecViewDir;

float4 vecSunColor;	
float4 vecSunPos;
float3 vecSunDir;

float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;
float  fPower;

/////////////////////////////////////////////////////////////////////////////////////////////////
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

texture entSkin2; 							// Normalmap

// Normal map sampler
sampler NormalSampler = sampler_state	
{
	Texture = <entSkin2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// structs

struct outModelVS
{
	float4 Pos				: POSITION;
	float4 Color			: COLOR0;    
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
//	float3 SunDir			: TEXCOORD3;
	float3 WorldPos		: TEXCOORD4;
	float3 Tangent			: TEXCOORD5;
	float3 Binormal		: TEXCOORD6;
	float4 Shadow			: TEXCOORD7;
   float  Fog 				: FOG;
};

struct inModelPS 
{
	float4 Color			: COLOR0;  
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
//	float3 SunDir			: TEXCOORD3;
	float3 WorldPos		: TEXCOORD4;
	float3 Tangent			: TEXCOORD5;
	float3 Binormal		: TEXCOORD6;
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// includes

#include "sc_lighting_MB.fx"				// new ligthing functions

#include "sc_shadowmapping_MB.fx"		// scsm shadowmapping - variables, shadow samplers, vertex shader functions, pixel shader function

#ifdef HEIGHT_FOG
	#include "sc_heightfog.fx"				// blend distance fog with height fog
#endif

#ifdef DEBUG_TILE
	#include "rts_tiledarea_MB.fx"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

outModelVS ModelVS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0,
   float3 InTangent	: TEXCOORD2			// was float4 but that is bad with flat separated surfaces
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
	
	Out.ViewDir = normalize(vecViewPos.xyz - PosWorld.xyz);
	
	//----------------------------------------------------------------
	
//	Out.SunDir = normalize(PosWorld.xyz - vecSunPos);
	
	//----------------------------------------------------------------
	
	Out.Tangent		 = normalize( mul(InTangent.xyz, (float3x3)matWorld) );									// tangent (*bumpstrength)
	Out.Binormal	 = -abs(normalize( mul(cross(InTangent.xyz,InNormal), (float3x3)matWorld) ));		// binormal, without cross() * InTangent.w !!! (*bumpstrength)
   
	Out.WorldNormal = normalize(mul(InNormal, (float3x3)matWorld));											// normalize because of entity scale!
	
	//----------------------------------------------------------------
{}
#ifndef HEIGHT_FOG   
	Out.Fog = vecFogColor.w																					// fog is applied
				 * saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );	// saturate needed when fadeout distance is closer than clipping distance
#else
	Out.Fog = VS_heightfog(PosWorld.xyz);
#endif
	
	//----------------------------------------------------------------
	
	Out.Shadow = VS_shadowmapping(PosWorld);
	
	//----------------------------------------------------------------
	// lighting
	
	// final color
	Out.Color = (vecAmbient * vecLight) + (vecEmissive * vecColor);			// ambient + emissive
		
{}
#ifndef PER_PIXEL_LIGHTS
	
	float4 Lights = 0;
	
	// dynamic lights
	for (int i=1; i<iLights; i++)																				
		Lights += PointLightDiffuse(PosWorld, Out.WorldNormal, i-1);	
//		Lights += PointLightDiffuseSpecular(PosWorld, Out.WorldNormal, i-1, 1.0f, Out.ViewDir);
	
//	// Sun
//	Lights += SunLightDiffuseSpecular(PosWorld, Out.WorldNormal, iLights-1, 1.f, Out.ViewDir);		// always done in PS !
	
	Out.Color += Lights;																		// + diffuse + specular
	
#endif				
	
	//-----------------------------------------------------------------
	
	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

float4 ModelPS(inModelPS In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
   float Shadow = clamp( PS_shadowmapping(In.Shadow, scsm_esm_obj_var), 1-scsm_run_var, 1);
    
	//----------------------------------------------------------
   // normalize input vectors
   
//	float BumpStrength = 2.0f;
   
//	In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
//	In.WorldNormal = normalize(In.WorldNormal);
//	In.Tangent 		= normalize(In.Tangent) * BumpStrength;
//	In.Binormal 	= normalize(In.Binormal) * BumpStrength;
   
   //----------------------------------------------------------
   // normalmapping
   
   float3 BumpNormal = 2 * tex2D(NormalSampler, In.Tex).rgb - 1;															
   BumpNormal = (In.Tangent * BumpNormal.r + In.Binormal * BumpNormal.g + In.WorldNormal * BumpNormal.b);	// transform to world space 
   
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
   //----------------------------------------------------------
   // dynamic lighting = diffuse + specular
   
   float4 Lights = 0;
   
   // Sun
//	Lights += SunLightDiffuse(In.WorldPos,     BumpNormal, iLights-1);								// no specularity
	Lights += SunLightDiffuseSpecular(In.WorldPos, BumpNormal, iLights-1, Shadow, In.ViewDir);
{}
#ifdef PER_PIXEL_LIGHTS	
	
	// dynamic lights
	for (int i=1; i<iLights; i++)	
		Lights += PointLightDiffuse(In.WorldPos,     BumpNormal, i-1);												// slightly faster
//		Lights += PointLightDiffuseSpecular(In.WorldPos, BumpNormal, i-1, Shadow, In.ViewDir);				// slightly better														
	
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
	
#endif	
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (Lights + In.Color);																		// with per pixel dynamic lights, InColor contains only entity ambient and emissive
	
	//----------------------------------------------------------
	// add shadows
	
	Color.rgb *= Shadow;																						// dynamic shadows, add if loop runs
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);													// sm 3.0 fog support	
	
	//----------------------------------------------------------
	
	return Color;
}

////////////////////////////////////////////////////////////////////////////////////////////////

technique model
{ 

   pass p0
   {
//   	cullmode = none;									
   	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False;
		
		VertexShader = compile vs_3_0 ModelVS();
     	PixelShader  = compile ps_3_0 ModelPS();	
   }

}

technique fallback 
{ 
	pass 
	{
//		cullmode = none;										
		
		ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False; 	
	} 
}