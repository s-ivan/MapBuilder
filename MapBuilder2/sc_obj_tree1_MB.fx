
bool AUTORELOAD;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

#define PER_PIXEL_LIGHTS						
//#define PER_PIXEL_LIGHTS_LOD

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
float4 vecSkill41;

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

////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shaders in included files

#include "sc_vs_animtree1_MB.fx"			// vertex shader
#include "sc_vs_model_MB.fx"				// vertex shader

////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader in included files

#include "sc_ps_model_MB.fx"				// pixel shader

////////////////////////////////////////////////////////////////////////////////////////////////

technique animtree1
{ 

   pass p0
   {
//   	cullmode = none;									
   	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False;
   	
		VertexShader = compile vs_3_0 AnimTree1VS();
     	PixelShader  = compile ps_3_0 ModelPS();	
   }

}

technique animtree1_lod1
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

technique animtree1_lod2
{ 

   pass p0
   {
//   	cullmode = none;										
   	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False;
   	
		VertexShader = compile vs_3_0 ModelVS2B();
     	PixelShader  = compile ps_3_0 ModelPS2();
   }

}

technique animtree1_lod3
{ 

   pass p0
   {
//   	cullmode = none;										
   	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False;
   	
//		VertexShader = compile vs_3_0 ModelVS2();
//		PixelShader  = compile ps_3_0 ModelPS2();
   }

}

//////////////////////////////////////////

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