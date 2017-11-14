
bool AUTORELOAD;

//bool REQUIRE_NORMAL;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

#define PER_PIXEL_LIGHTS			

//#define SPECULAR_MAP

////////////////////////////////////////////////////////////////////////////////////////////////
// matrices, vectors, variables passed by the engine

float4x4 matWorldViewProj;
float4x4 matWorld;

float3x3 matTangent;							// needed for normalmapping

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

////////////////////////////////////////////////////////////////////////////////////////////////
// custom matrices, vectors, variables

float sky_loddistance1_var					= 500.0f;		// lod1 distance
float sky_loddistance2_var					= 1000.0f;		// lod2 distance
float sky_loddistance3_var					= 2000.0f;		// lod3 distance

////////////////////////////////////////////////////////////////////////////////////////////////
// skins/textures

//texture entSkin1; 							// Colormap
//
//// Color map sampler
//sampler colorSampler = sampler_state 
//{ 
//   Texture = <entSkin1>; 
//   MinFilter = Linear;
//	MagFilter = Linear;
//	MipFilter = Linear;
//};

texture entSkin2; 							// Normalmap

// Normal map sampler
sampler NormalSampler = sampler_state	
{
	Texture = <entSkin2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs

struct outVS 
{
	float4 Color			: COLOR0; 
	float4 Pos				: POSITION; 
	float2 Tex				: TEXCOORD0; 
	float3 ViewDir			: TEXCOORD1;
	float3 WorldNormal	: TEXCOORD2;	
//	float3 SunDir			: TEXCOORD3;
#ifdef PER_PIXEL_LIGHTS	
	float3 WorldPos		: TEXCOORD4;
#endif	
	float3 Tangent			: TEXCOORD5;
	float3 Binormal		: TEXCOORD6;
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

//struct outVS2
//{
//	float4 Color			: COLOR0; 
//	float4 Pos				: POSITION; 
//	float2 Tex				: TEXCOORD0; 
//	float3 ViewDir			: TEXCOORD1;
//	float3 WorldNormal	: TEXCOORD2;	
////	float3 SunDir			: TEXCOORD3;
//	float3 WorldPos		: TEXCOORD4;	
//	float3 Tangent			: TEXCOORD5;
//	float3 Binormal		: TEXCOORD6;
//	float4 Shadow			: TEXCOORD7;
//	float  Fog 				: FOG;
//};

struct inPS 
{
	float4 Color			: COLOR0;  
	float2 Tex				: TEXCOORD0; 
	float3 ViewDir			: TEXCOORD1;
	float3 WorldNormal	: TEXCOORD2;		
//	float3 SunDir			: TEXCOORD3;
#ifdef PER_PIXEL_LIGHTS	
	float3 WorldPos		: TEXCOORD4;
#endif	
	float3 Tangent			: TEXCOORD5;
	float3 Binormal		: TEXCOORD6;
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

//struct inPS2
//{
//	float4 Color			: COLOR0;  
//	float2 Tex				: TEXCOORD0; 
//	float3 ViewDir			: TEXCOORD1;
//	float3 WorldNormal	: TEXCOORD2;		
////	float3 SunDir			: TEXCOORD3;
//	float3 WorldPos		: TEXCOORD4;	
//	float3 Tangent			: TEXCOORD5;
//	float3 Binormal		: TEXCOORD6;
//	float4 Shadow			: TEXCOORD7;
//	float  Fog 				: FOG;
//};

////////////////////////////////////////////////////////////////////////////////////////////////
// includes

#include "sc_defines_MB.fx"

#include "sc_pointlight_MB.fx"

#include "sc_shadowmapping_MB.fx"		// scsm shadowmapping - variables, shadow samplers, vertex shader functions, pixel shader function

#ifdef HEIGHT_FOG
	#include "sc_heightfog_MB.fx"			// blend distance fog with height fog
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

#include "sc_vs_model_MB.fx"				// vertex shader in included files

outVS ModelNVS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0,
   float3 InTangent	: TEXCOORD2			// was float4 but that is bad with flat separated surfaces
	) 
{ 
   outVS Out;
	
//	InPos.w = 1.0f;																					// to get proper matrix multiplication?
		
	//----------------------------------------------------------------
	
   Out.Tex = InTex;
   
   //---------------------------------------------------------------
	
	float4 PosWorld = mul(InPos, matWorld);											// needed for a couple of calculations... float4 for shadowmapping!
	
	//----------------------------------------------------------------
	
	Out.Pos = mul(InPos, matWorldViewProj); 
	
	//----------------------------------------------------------------
{}
#ifdef PER_PIXEL_LIGHTS
  	Out.WorldPos    = PosWorld.xyz;
#endif 
	
	//----------------------------------------------------------------
	
	Out.ViewDir = normalize(vecViewPos.xyz - PosWorld.xyz);									
			
	//----------------------------------------------------------------
	
	Out.Tangent		 = normalize( mul(InTangent.xyz, (float3x3)matWorld) );									// tangent (*bumpstrength)
	Out.Binormal	 = -abs(normalize( mul(cross(InTangent.xyz,InNormal), (float3x3)matWorld) ));		// binormal, without cross() * InTangent.w !!! (*bumpstrength)
   																																// - abs(binormal) is needed because of float3 tangent, and to have similarly correct vertical shading on all sides!
	Out.WorldNormal = normalize( mul(InNormal, (float3x3)matWorld) );											// normal
   
   //----------------------------------------------------------------
   
#ifndef HEIGHT_FOG   
	Out.Fog = vecFogColor.w																									// fog is applied
				 * saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );					// saturate needed when fadeout distance is closer than clipping distance
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
			
	for (int i=1; i<iLights; i++)  														// add (max 7) dynamic lights (except Sun)
		Lights += PointLightDiffuse(PosWorld, Out.WorldNormal, i-1);
	
	Lights *= vecDiffuse;
	
	Out.Color += Lights;
#endif			
	
   //-----------------------------------------------------------------
   
   return Out;
}

//outVS ModelNVS2
//	( 
//   float4 InPos		: POSITION, 
//   float3 InNormal	: NORMAL, 
//   float2 InTex		: TEXCOORD0,
//   float3 InTangent	: TEXCOORD2			// was float4 but that is bad with flat separated surfaces
//	) 
//{ 
//   outVS Out;
//	
////	InPos.w = 1.0f;																					// to get proper matrix multiplication?
//		
//	//----------------------------------------------------------------
//	
//   Out.Tex = InTex;
//   
//   //---------------------------------------------------------------
//	
//	float4 PosWorld = mul(InPos, matWorld);											// needed for a couple of calculations... float4 for shadowmapping!
//	
//	//----------------------------------------------------------------
//	
//	Out.Pos = mul(InPos, matWorldViewProj); 
//	
//	//----------------------------------------------------------------
//
//  	Out.WorldPos    = PosWorld.xyz;
//	
//	//----------------------------------------------------------------
//	
//	Out.ViewDir = normalize(vecViewPos.xyz - PosWorld.xyz);									
//			
//	//----------------------------------------------------------------
//	
//	Out.Tangent		 = normalize( mul(InTangent.xyz, (float3x3)matWorld) );									// tangent (*bumpstrength)
//	Out.Binormal	 = -abs(normalize( mul(cross(InTangent.xyz,InNormal), (float3x3)matWorld) ));		// binormal, without cross() * InTangent.w !!! (*bumpstrength)
//   																																// - abs(binormal) is needed because of float3 tangent, and to have similarly correct vertical shading on all sides!
//	Out.WorldNormal = normalize( mul(InNormal, (float3x3)matWorld) );											// normal
//   
//   //----------------------------------------------------------------
//{}   
//#ifndef HEIGHT_FOG   
//	Out.Fog = vecFogColor.w																									// fog is applied
//				 * saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );					// saturate needed when fadeout distance is closer than clipping distance
//#else
//	Out.Fog = VS_heightfog(PosWorld.xyz);
//#endif
//   
//   //----------------------------------------------------------------
//	
//	Out.Shadow = VS_shadowmapping(PosWorld);
//	
//	//----------------------------------------------------------------
//	// lighting
//	
//	// final color
//	Out.Color = (vecAmbient * vecLight) + (vecEmissive * vecColor);			// ambient + emissive
//		
//{}
//#ifndef PER_PIXEL_LIGHTS
//	float4 Lights = 0;	
//			
//	for (int i=1; i<iLights; i++)  														// add (max 7) dynamic lights (except Sun)
//		Lights += PointLightDiffuse(PosWorld, Out.WorldNormal, i-1);
//	
//	Lights *= vecDiffuse;
//	
//	Out.Color += Lights;
//#endif			
//	
//   //-----------------------------------------------------------------
//   
//   return Out;
//}

//////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

#include "sc_ps_model_MB.fx"				// pixel shader in included files

float4 ModelNPS(inPS In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
   float Shadow = clamp( PS_shadowmapping(In.Shadow, scsm_esm_obj_var), 1-scsm_run_var, 1);
   
   //----------------------------------------------------------
   // normalize input vectors
   
//	float BumpStrength = 2.0f;
   
   In.ViewDir 		= normalize(In.ViewDir);
//	In.Tangent 		= normalize(In.Tangent) * BumpStrength;
//	In.Binormal 	= normalize(In.Binormal) * BumpStrength;
//	In.WorldNormal = normalize(In.WorldNormal);
   
   //----------------------------------------------------------
   // normal mapping   
{}
#ifndef SPECULAR_MAP   
	float3 NormalMap 	= 2 * tex2D(NormalSampler, In.Tex).rgb - 1;																		// Read the normal from the normal map and convert from [0..1] to [-1..1] range         
   NormalMap 			= normalize(In.Tangent * NormalMap.r + In.Binormal * NormalMap.g + In.WorldNormal * NormalMap.b);			// transform to world space 
#else
	float4 BumpSpec   = tex2D(NormalSampler, In.Tex);	
	float SpecularMap = BumpSpec.a;	
	float3 NormalMap  = 2 * BumpSpec.rgb - 1;			
	NormalMap 			= normalize(In.Tangent * NormalMap.r + In.Binormal * NormalMap.g + In.WorldNormal * NormalMap.b);
#endif   
	//----------------------------------------------------------
   // Sun lighting
   
   float LightIntensity = saturate(dot(NormalMap, -vecSunDir));
   
   // diffuse   									
	float4 Diffuse = scsm_brightness * 5.0 * vecDiffuse * LightIntensity * vecSunColor * vecLight;									
	
	// specular
	float4 Specular = 0;
	if (LightIntensity * Shadow > 0.50f)																										// a bit higher limtit looks fine and more exact, than in flat shading (no speculars under shadows)
		{
			float3 Halfway  = saturate(dot(normalize(In.ViewDir - vecSunDir), NormalMap));
			
			Specular = vecSpecular * pow( dot(NormalMap, Halfway), fPower) * vecSunColor * vecLight;
{}			
#ifdef SPECULAR_MAP			
			// specular mapping
			Specular = Specular * SpecularMap;
#endif
		}
	// specular - another calculation
//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - vecSunDir);													// R
//	float4 Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor * vecLight;			// R.V^n 												
		
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
   //----------------------------------------------------------
   // dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	float4 Lights = 0;
	
	for (int i=1; i<iLights; i++)																			// add max 8 dynamic lights, ignore the last which is the Sun
		Lights += PointLightDiffuse(In.WorldPos, NormalMap, i-1);	
	
	Lights *= vecDiffuse;																		
	
	#ifdef DEBUG_TILE																
		Color.rgb *= DebugTerrain(In.WorldPos, In.WorldNormal);
	#endif 
	
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (Diffuse + Specular + Lights + In.Color);										// with per pixel dynamic lights, InColor contains only entity ambient fAmbient
#else
	Color.rgb *= (Diffuse + Specular + In.Color);													// with per vertex dynamic lights
#endif

	//----------------------------------------------------------
	// add shadows
	
	Color.rgb *= Shadow;																						// dynamic shadows, added to lightintensity
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);												// sm 3.0 fog support	
	
	//----------------------------------------------------------
	
	return Color;
}

//float4 ModelNPS2(inPS In): COLOR
//{
//	//----------------------------------------------------------
//	// shadowmapping
//   
//   float Shadow = clamp( PS_shadowmapping(In.Shadow, scsm_esm_obj_var), 1-scsm_run_var, 1);
//   
//   //----------------------------------------------------------
//   // normalize input vectors
//   
////	float BumpStrength = 2.0f;
//   
//   In.ViewDir 		= normalize(In.ViewDir);
////	In.Tangent 		= normalize(In.Tangent) * BumpStrength;
////	In.Binormal 	= normalize(In.Binormal) * BumpStrength;
////	In.WorldNormal = normalize(In.WorldNormal);
//   
//   //----------------------------------------------------------
//   // normal mapping
//   
//   // weighting value of normalmap between world and normalmap, based on distance, for smooth transience
////	float NormalWeight = saturate( (distance(In.WorldPos, vecViewPos.xyz) - sky_loddistance2_var) / (sky_loddistance3_var - sky_loddistance2_var) );  
//   float NormalWeight = saturate( (length(In.WorldPos - vecViewPos.xyz) - sky_loddistance2_var) / (sky_loddistance3_var - sky_loddistance2_var) );  
//   
//{}
//#ifndef SPECULAR_MAP   
//	float3 NormalMap 	= 2 * tex2D(NormalSampler, In.Tex).rgb - 1;																			// Read the normal from the normal map and convert from [0..1] to [-1..1] range   
//	NormalMap 			= lerp(NormalMap, float3(0, 0, 1.f), NormalWeight);   															// transition
//   NormalMap 			= normalize(In.Tangent * NormalMap.r + In.Binormal * NormalMap.g + In.WorldNormal * NormalMap.b);	// transform to world space 
//#else
//	float4 BumpSpec 	= tex2D(NormalSampler, In.Tex);	
//	float SpecularMap = BumpSpec.a;	
//	float3 NormalMap 	= 2 * BumpSpec.rgb - 1;	
//	NormalMap 			= lerp(NormalMap, float3(0, 0, 1.f), NormalWeight);	   														// transition	
//	NormalMap 			= normalize(In.Tangent * NormalMap.r + In.Binormal * NormalMap.g + In.WorldNormal * NormalMap.b);
//#endif   
//	//----------------------------------------------------------
//   // Sun lighting
//   
//   float LightIntensity = saturate(dot(NormalMap, -vecSunDir));
//   
//   // diffuse   									
//	float4 Diffuse = scsm_brightness * 5.0 * vecDiffuse * LightIntensity * vecSunColor * vecLight;									
//	
//	// specular
//	float4 Specular = 0;
//	if (LightIntensity * Shadow > 0.50f)																										// a bit higher limtit looks fine and more exact, than in flat shading (no speculars under shadows)
//		{
//			float3 Halfway  = saturate(dot(normalize(In.ViewDir - vecSunDir), NormalMap));
//			
//			Specular = vecSpecular * pow( dot(NormalMap, Halfway), fPower) * vecSunColor * vecLight;
//{}			
//#ifdef SPECULAR_MAP			
//			// specular mapping
//			Specular = Specular * SpecularMap;
//#endif
//		}
//	// specular - another calculation
////	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - vecSunDir);													// R
////	float4 Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor * vecLight;			// R.V^n 												
//		
//	//----------------------------------------------------------
//	// texture - keep its alpha afterwards
//	
//   float4 Color = tex2D(colorSampler, In.Tex);										
//   
//   //----------------------------------------------------------
//   // dynamic lights
//{}
//#ifdef PER_PIXEL_LIGHTS	
//	float4 Lights = 0;
//	
//	for (int i=1; i<iLights; i++)																			// add max 8 dynamic lights, ignore the last which is the Sun
//		Lights += PointLightDiffuse(In.WorldPos, NormalMap, i-1);	
//	
//	Lights *= vecDiffuse;																		
//	
//	#ifdef DEBUG_TILE																
//		Color.rgb *= DebugTerrain(In.WorldPos, In.WorldNormal);
//	#endif 
//	
//	//----------------------------------------------------------
//	// add lighting to color
//	
//	Color.rgb *= (Diffuse + Specular + Lights + In.Color);										// with per pixel dynamic lights, InColor contains only entity ambient fAmbient
//#else
//	Color.rgb *= (Diffuse + Specular + In.Color);													// with per vertex dynamic lights
//#endif
//
//	
////	Color.rgb = lerp(Color.rgb, float3(1,0,0), NormalWeight-0.5);
//	
//	//----------------------------------------------------------
//	// add shadows
//	
//	Color.rgb *= Shadow;																						// dynamic shadows, added to lightintensity
//	
//	//----------------------------------------------------------
//	// add fog
//	
//	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);												// sm 3.0 fog support	
//	
//	//----------------------------------------------------------
//	
//	return Color;
//}

////////////////////////////////////////////////////////////////////////////////////////////////

technique model
{ 

   pass p0
   {
//   	cullmode = none;									
   	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False;
   	
		VertexShader = compile vs_3_0 ModelNVS();
     	PixelShader  = compile ps_3_0 ModelNPS();	
   }

}

technique model_lod1
{ 

   pass p0
   {
//   	cullmode = none;									
   	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False;
   	
		VertexShader = compile vs_3_0 ModelNVS();
     	PixelShader  = compile ps_3_0 ModelNPS();	
   }

}

technique model_lod2
{ 

   pass p0
   {
//   	cullmode = none;									
   	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False;
   	
		VertexShader = compile vs_3_0 ModelNVS();
     	PixelShader  = compile ps_3_0 ModelNPS();	
   }

}

technique model_lod3
{ 

   pass p0
   {
//   	cullmode = none;									
   	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False;
   	
		VertexShader = compile vs_3_0 ModelVS2();
     	PixelShader  = compile ps_3_0 ModelPS2();	
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