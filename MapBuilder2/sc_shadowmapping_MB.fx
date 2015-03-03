////////////////////////////////////////////////////////////////////////////////////////////////
// (c) Ivan Stummer - based on Shade-C by BohHavok
// MapBuilder 2
// v.1.0
////////////////////////////////////////////////////////////////////////////////////////////////
// usage:
// it should be included in shaders using scsm
// (shade-c 0.91 based exponential shadowmapping)
// in original object shader it is needed to have:
// include:								#include "sc_shadowmapping_MB.fx"
// 1 vertex shader call:			Out.Shadow = VS_shadowmapping(PosWorld);
// 1 pixel shader call:				float Shadow = PS_shadowmapping(In.Shadow);
////////////////////////////////////////////////////////////////////////////////////////////////
// custom matrices, vectors, variables

float scsm_esm_var 					= 5.00f;			// exponential shadowmapping constant
float scsm_esm_obj_var 				= 5.00f;			// exponential shadowmapping constant

float scsm_shadowalpha_var			= 0.50f; 		// range: 0-1 | 0 = pure black shadow | 1 = no shadow at all
float scsm_lightmapalpha_var		= 0.50f; 		//shadowalpha of shadowtexture		lightmap shadow darkness
float scsm_run_var					= 1.0f;			// shadowmapping running 1, stopped 0

float scsm_shadowoffset_f_flt		= 0.0f;

float scsm_maxdepth_var 			= 450.0f;

float scsm_fadeout_start_var 		= 0.75f;			// fade out in shadowmapping texture uv percentage - instead of original texture based solution

float4 scsm_shadowcolor				= {0.0, 0.1, 0.3, 1.0};			// a little color tint - requires "Color.rgb *= lerp(scsm_shadowcolor.rgb, 1, Shadow);" instead of "Color.rgb *= Shadow;" in PS add shadows section

float4x4 matEffect1;
float4x4 matEffect3;

////////////////////////////////////////////////////////////////////////////////////////////////
// skins/textures

// ShadowDepth map sampler
texture sc_map_shadowDepthSun_bmap;

sampler ShadowMapSampler = sampler_state
{
	Texture = <sc_map_shadowDepthSun_bmap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//// Shadowmap border fade-out image - was used in original shader
//Texture sc_map_shadowAlpha_bmap;
//
//sampler ShadowAlphaSampler = sampler_state
//{
//	texture = <sc_map_shadowAlpha_bmap>;
//	magfilter = LINEAR;
//	minfilter = LINEAR;
//	mipfilter = LINEAR;
//	AddressU = CLAMP;
//	AddressV = CLAMP;
//};

////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader calculations

float4	VS_shadowmapping(float4 PosWorld)
{
	float4 ShadowMapSamplingPos 	= mul(PosWorld, mul(matEffect3, matEffect1));
	float4 RealDistance 				= mul(PosWorld, matEffect3).z / scsm_maxdepth_var;
	
	float4 OutShadow;	// -1,1
	
//	// original
//	OutShadow.x = ShadowMapSamplingPos.x;
//	OutShadow.y = ShadowMapSamplingPos.y;
//	OutShadow.z = ShadowMapSamplingPos.w;
//	OutShadow.w = RealDistance.x;
	
	// new -1..1 to 0..1
	OutShadow.x = ShadowMapSamplingPos.x / ShadowMapSamplingPos.w / 2.0f + 0.5f;
	OutShadow.y = - ShadowMapSamplingPos.y / ShadowMapSamplingPos.w / 2.0f + 0.5f - scsm_shadowoffset_f_flt;
	OutShadow.z = 0;
	OutShadow.w = RealDistance.x;	
	
	return OutShadow;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader calculations

////---------------------------------------------------------------------------------------------
//// texture based border fade-out
//
//float		PS_shadowmapping_Old(float4 InShadow, float scsm_esm)
//{
//	float2 ProjectedTexCoords;
//   ProjectedTexCoords[0] =  InShadow.x/InShadow.z/2.0f +0.5f;
//   ProjectedTexCoords[1] = -InShadow.y/InShadow.z/2.0f +0.5f - scsm_shadowoffset_f_flt;			// apply a little offset if shadow root fades out
//
////	float shadowDepth = tex2D(ShadowMapSampler, ProjectedTexCoords).x;						
//	float shadowDepth = tex2Dlod(ShadowMapSampler, float4(ProjectedTexCoords.xy, 0.0f, 0.0f)).x;
//	
////   // ESM																													// original calculations
////   float Shadow = In.Shadow.w - shadowDepth;
////   if (Shadow > 0)
////   	Shadow = exp( scsm_esm * (shadowDepth - In.Shadow.w) );												// rearranged to be faster
//////    Shadow = exp( scsm_esm * shadowDepth) * exp( -scsm_esm_var * In.Shadow.w) ;				// the original
////   else
////   	Shadow = 1;
//  	
//  	// ESM
//  	float Shadow = exp( scsm_esm * (shadowDepth - InShadow.w) );											// new simplified, faster, requires depthmap bmap format 14
//  	
//   // alpha fade-out (depending on image border fade out)
////	float fade = tex2D(ShadowAlphaSampler, ProjectedTexCoords);
//   float fade = tex2Dlod(ShadowAlphaSampler, float4(ProjectedTexCoords.xy, 0.0f, 0.0f));
//   
//   Shadow *= fade;						
//   Shadow += 1-fade;																									// without it dark border out of shadow area - cool for debugging
//   Shadow = clamp(Shadow, scsm_shadowalpha_var, 1);															// shadow alpha
//   
//   return Shadow;
// }
 
 //---------------------------------------------------------------------------------------------
 // no border fade-out
 
float		PS_shadowmapping_Fast(float4 InShadow, float scsm_esm)											// black out of shadowmapping area so requires switching off SHADOW flag
{
	float2 ProjectedTexCoords = InShadow.xy;
//	ProjectedTexCoords[0] =  InShadow.x / InShadow.z / 2.0f + 0.5f;
//	ProjectedTexCoords[1] = -InShadow.y / InShadow.z / 2.0f + 0.5f - scsm_shadowoffset_f_flt;		// apply a little offset if shadow root fades out
	
	float shadowDepth = tex2Dlod(ShadowMapSampler, float4(ProjectedTexCoords.xy, 0.0f, 0.0f)).x;
  	
  	// ESM
  	float Shadow = exp( scsm_esm * (shadowDepth - InShadow.w) );											// new simplified, fastee, requires depthmap bmap format 14
  	
//  	if (ProjectedTexCoords[0] <= 0 || ProjectedTexCoords[0] >= 1 || ProjectedTexCoords[1] <= 0 || ProjectedTexCoords[1] >= 1)
//  		Shadow = 1.0f;
  	
   Shadow = clamp(Shadow, scsm_shadowalpha_var, 1);															// shadow alpha
   
   return Shadow;
 }
 
  //---------------------------------------------------------------------------------------------
  // adjustable border fade-out
  
float		PS_shadowmapping(float4 InShadow, float scsm_esm)
{
	float2 ProjectedTexCoords = InShadow.xy;
//	ProjectedTexCoords[0] =  InShadow.x / InShadow.z / 2.0f + 0.5f;
//	ProjectedTexCoords[1] = -InShadow.y / InShadow.z / 2.0f + 0.5f - scsm_shadowoffset_f_flt;		// apply a little offset if shadow root fades out
	
{}
#ifdef SM_LOD
	float shadowDepth = tex2D(ShadowMapSampler, ProjectedTexCoords).x;						
#else
	float shadowDepth = tex2Dlod(ShadowMapSampler, float4(ProjectedTexCoords.xy, 0.0f, 0.0f)).x;
#endif
	
  	// ESM
  	float Shadow = exp( scsm_esm * (shadowDepth - InShadow.w) );											// new simplified, faster, requires depthmap bmap format 14
  	   
   // shadow darkness
   Shadow = clamp(Shadow, scsm_shadowalpha_var, 1);
   
   // fade out at borders
   float fadeout_value = 2.0f * max( abs(saturate(ProjectedTexCoords.x) - 0.5f) , abs(saturate(ProjectedTexCoords.y) - 0.5f) );
   fadeout_value = saturate(fadeout_value - scsm_fadeout_start_var) / (1.0f - scsm_fadeout_start_var);
   
   // final shadow, similarly to fog
   Shadow = lerp(Shadow, 1, fadeout_value);
   
   return Shadow;
 }
 
 //---------------------------------------------------------------------------------------------
 // adjustable border fade-out, with transition into static lightmap
  
 float		PS_shadowmapping_lm(float4 InShadow, float scsm_esm, float2 InTex, sampler LightMapSampler)
{
	float2 ProjectedTexCoords = InShadow.xy;
//	ProjectedTexCoords[0] =  InShadow.x / InShadow.z / 2.0f + 0.5f;
//	ProjectedTexCoords[1] = -InShadow.y / InShadow.z / 2.0f + 0.5f - scsm_shadowoffset_f_flt;		// apply a little offset if shadow root fades out
	
{}
#ifdef SM_LOD
	float shadowDepth = tex2D(ShadowMapSampler, ProjectedTexCoords).x;						
#else
	float shadowDepth = tex2Dlod(ShadowMapSampler, float4(ProjectedTexCoords.xy, 0.0f, 0.0f)).x;
#endif
	  	
  	// ESM
  	float Shadow = exp( scsm_esm * (shadowDepth - InShadow.w) );											// new simplified, faster, requires depthmap bmap format 14
  	   
   // shadow darkness
   Shadow = clamp(Shadow, scsm_shadowalpha_var, 1);
   
   // fade out at borders
   float fadeout_value = 2.0f * max( abs(saturate(ProjectedTexCoords.x) - 0.5f) , abs(saturate(ProjectedTexCoords.y) - 0.5f) );
   fadeout_value = saturate(fadeout_value - scsm_fadeout_start_var) / (1.0f - scsm_fadeout_start_var);
   
   // final shadow, similarly to fog
   Shadow = lerp(Shadow, 1, fadeout_value);
   
   // static lightmap
   float4 Lightmap = 2.0 * tex2D(LightMapSampler, InTex);										// 1.0 * for L3DT lightmap, 2.0 * for 3dgs lightmap
	Lightmap = clamp(Lightmap, scsm_lightmapalpha_var, 1);
	Lightmap = lerp(Lightmap, 1, 1-fadeout_value);
	
	// out	
   return Shadow * Lightmap;
 }