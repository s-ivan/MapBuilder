
bool AUTORELOAD;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

#define PER_PIXEL_LIGHTS				
#define PER_PIXEL_LIGHTS_LOD

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
// includes

#include "sc_defines_MB.fx"
#include "sc_pointlight_MB.fx"
#include "sc_shadowmapping_MB.fx"		// scsm shadowmapping - variables, shadow samplers, vertex shader functions, pixel shader function

#ifdef HEIGHT_FOG
	#include "sc_heightfog_MB.fx"			// blend distance fog with height fog
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shaders in included files

#include "sc_vs_bones_MB.fx"				// vertex shader

////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader in included files

#include "sc_ps_bones_MB.fx"				// pixel shader

////////////////////////////////////////////////////////////////////////////////////////////////

technique bone_model
{ 
//	pass P0
//	{
////		ZFunc					= Greater;
//		ZEnable				= False;
//		
//		VertexShader = compile vs_3_0 BonesVSX();
//     	PixelShader  = compile ps_3_0 BonesPSX();
//	}
	
   pass P1 
   {
////		ZFunc 				= LessEqual;
//   	ZEnable 				= True;	
   	
//   	cullmode = none;	
   	
//   	ZWriteEnable 		= True;
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
	   
		VertexShader = compile vs_3_0 BonesVS1();
     	PixelShader  = compile ps_3_0 BonesPS1();
   }
	
}

technique bone_model_lod1
{ 
//	pass P0
//	{
////		ZFunc					= Greater;
//		ZEnable				= False;
//		
//		VertexShader = compile vs_3_0 BonesVSX();
//     	PixelShader  = compile ps_3_0 BonesPSX();
//	}
	
   pass P1 
   {
////		ZFunc 				= LessEqual;
//   	ZEnable 				= True;
   	
//   	cullmode = none;
   	
//   	ZWriteEnable 		= True;
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
	   
		VertexShader = compile vs_3_0 BonesVS1();
     	PixelShader  = compile ps_3_0 BonesPS1();
   }
	
}

technique bone_model_lod2
{ 

   pass P0 
   {
//   	cullmode = none;
   	
//   	ZWriteEnable 		= True;
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
	   
		VertexShader = compile vs_3_0 BonesVS2();
     	PixelShader  = compile ps_3_0 BonesPS2();
   }

}

technique bone_model_lod3
{ 

   pass P0 
   {
//   	cullmode = none;
   	
//   	ZWriteEnable 		= True;
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
   }

}

//////////////////////////////////////////

technique fallback 
{ 
	pass 
	{
//		cullmode = none;										
		
//		ZWriteEnable 		= True;
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
	} 
}