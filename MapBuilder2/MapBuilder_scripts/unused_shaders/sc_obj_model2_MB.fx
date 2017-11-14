////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Model MAIN V2 shader for exponential shadowmapping - MapBuilder2
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

bool AUTORELOAD;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

#define PER_PIXEL_LIGHTS		// Sun light is always per pixel

#define HEIGHT_FOG				// a vertical fog is blended to distance fog

//#define DEBUG_TILE				// RTS related

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
// includes

#include "sc_pointlight_MB.fx"

#include "sc_shadowmapping_MB.fx"		// scsm shadowmapping - variables, shadow samplers, vertex shader functions, pixel shader function

#ifdef HEIGHT_FOG
	#include "sc_heightfog.fx"				// blend distance fog with height fog
#endif

#ifdef DEBUG_TILE
	#include "rts_tiledarea_MB.fx"
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

#include "sc_vs_model2_MB.fx"				// vertex shader in included files

////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

#include "sc_ps_model2_MB.fx"				// pixel shader in included files

////////////////////////////////////////////////////////////////////////////////////////////////

technique model
{ 

   pass p0
   {
//   	cullmode = none;									
   	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False;
		
		VertexShader = compile vs_3_0 Model2VS();
     	PixelShader  = compile ps_3_0 Model2PS();	
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