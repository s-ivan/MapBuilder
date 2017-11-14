
bool AUTORELOAD;

// 8x8 grid-sprite animation shader

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

//#define PER_PIXEL_LIGHTS						
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

float4 vecSkill41;							// x frame, y width, z direction, w height

////////////////////////////////////////////////////////////////////////////////////////////////
// includes

//#include <vecskill>

#include "sc_defines_MB.fx"
#include "sc_pointlight_MB.fx"
#include "sc_shadowmapping_MB.fx"		// scsm shadowmapping - variables, shadow samplers, vertex shader functions, pixel shader function

#ifdef HEIGHT_FOG
	#include "sc_heightfog_MB.fx"			// blend distance fog with height fog
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs VS

struct outSpriteVS
{
	float4 Pos				: POSITION;
	float4 Color			: COLOR0;    
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
// float3 SunDir			: TEXCOORD3;
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
#ifdef PER_PIXEL_LIGHTS
	float3 WorldPos		: TEXCOORD4;
#endif
	float4 Shadow			: TEXCOORD7;
   float  Fog 				: FOG;
};

////////////////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs PS

struct inModelPS 
{
	float4 Color			: COLOR0;  
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
// float3 SunDir			: TEXCOORD3;
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
#ifdef PER_PIXEL_LIGHTS
	float3 WorldPos		: TEXCOORD4;
#endif
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader


outSpriteVS AnimSpriteVS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0
	) 
{ 
	outSpriteVS Out;
	
//	InPos.w = 1.0f;																												// to get proper matrix multiplication?
		
	//----------------------------------------------------------------
	
	Out.Tex = InTex;
	
	//---------------------------------------------------------------
	
	float4 PosWorld = mul(InPos, matWorld);
	
	//----------------------------------------------------------------
	
	Out.Pos = mul(InPos, matWorldViewProj); 
	
	//----------------------------------------------------------------
	
{}
#ifdef PER_PIXEL_LIGHTS
  	Out.WorldPos    = PosWorld.xyz;
#endif 
	
	//----------------------------------------------------------------
	
	Out.ViewDir = vecViewPos - PosWorld;												
	
	//----------------------------------------------------------------
	
//	Out.SunDir = PosWorld.xyz - vecSunPos;
	
	//----------------------------------------------------------------
	
	Out.WorldNormal = normalize(mul(normalize(float3(0.5f,0.5f,0.0f)), (float3x3)matWorld));				// vertical normals always! float3(0.0f,1.0f,0.0f)
	
	//----------------------------------------------------------------
	
#ifndef HEIGHT_FOG   
	Out.Fog = vecFogColor.w																										// fog is applied
				 * saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );						// saturate needed when fadeout distance is closer than clipping distance
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
	
	Out.Color = (vecAmbient + vecEmissive) + vecColor;								// ambient + emissive + rgb
		
{}
#ifndef PER_PIXEL_LIGHTS
	
	float4 Lights = 0;	
			
	for (int i=1; i<iLights; i++)  														// add dynamic lights, ignore Sun (always the last one)
		Lights += PointLightDiffuse(PosWorld, Out.WorldNormal, i-1);			
	
	Lights *= 2.0f * vecDiffuse;																
	
	Out.Color += Lights;
	
#endif				
	
   //-----------------------------------------------------------------
   
   return Out;
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader


float4 AnimSpritePS(inModelPS In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
   float Shadow = clamp( PS_shadowmapping(In.Shadow, scsm_esm_obj_var), 1-scsm_run_var, 1);
    
	//----------------------------------------------------------
   // normalize input vectors
   
//	In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal = normalize(In.WorldNormal);
   
   //----------------------------------------------------------
   // Sun light
   
   float LightIntensity = saturate(dot(In.WorldNormal, -vecSunDir));																// no saturate needed when normalized vectors used
      
   // diffuse   
   float4 Diffuse = scsm_brightness * 1.0 * vecDiffuse * LightIntensity * vecSunColor;						// vecSunDir is in view space thus not good here							
	
	// specular										
	float4 Specular = 0;
	if (LightIntensity > 0.25f)																												// 0.50 would be perfect with shadows, but it must be lowered to look fine with flat shading
		{
			float3 Halfway  = saturate(dot(normalize(In.ViewDir - vecSunDir), In.WorldNormal));	
					
			Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor;					
		}
	// specular - another calculation
//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - In.SunDir);												// R
//	float4 Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor;		// R.V^n 												
	
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
	float2 Tile = 0;
	Tile.x = frac(In.Tex.x * vecSkill41.y + floor(vecSkill41.x) * vecSkill41.y);				// x frame 0..7, 			y unit-width, 			frac for safety				frame is fine with floor(vecTime.w)
	Tile.y = frac(In.Tex.y * vecSkill41.w + vecSkill41.z * vecSkill41.w);						// z direction 0..7, 	w unit-height, 		frac for safety
	
   float4 Color = tex2D(colorSampler, Tile);										
   
   //----------------------------------------------------------
   // dynamic lights except Sun
{}
#ifdef PER_PIXEL_LIGHTS	
	float4 Lights = 0;
	
	for (int i=1; i<iLights; i++)																				// add max 8 dynamic lights, ignore the last which is the Sun
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
	
	Lights *= 2.0f * vecDiffuse;																
	
	#ifdef DEBUG_TILE																
		Color.rgb *= DebugTerrain(In.WorldPos, In.WorldNormal);
	#endif 
	
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (Diffuse + Specular + Lights + In.Color);											// with per pixel dynamic lights
#else
	Color.rgb *= (Diffuse + Specular + In.Color);														// with per vertex dynamic lights
#endif
	
	//----------------------------------------------------------
	// add shadows
	
//	Color.rgb *= lerp(scsm_shadowcolor.rgb, 1, Shadow);											// dynamic shadows
	Color.rgb *= Shadow;
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);													// sm 3.0 fog support	
	
	//----------------------------------------------------------
	// add sky light	
{}
#ifdef SKY_LIGHT	
	Color.rgb = lerp(Color.rgb, sky_skylightcolor_var, In.SkyLight);		
#endif
	
	//----------------------------------------------------------
	
	return Color;
}


////////////////////////////////////////////////////////////////////////////////////////////////

technique standsprite
{ 

   pass p0
   {
//   	cullmode = none;
   	
//   	ZEnable 				= True;	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= True;	
   	
		VertexShader = compile vs_3_0 AnimSpriteVS();
		PixelShader  = compile ps_3_0 AnimSpritePS();	
   }

}

//////////////////////////////////////////

technique fallback 
{ 
	pass 
	{ 
//		cullmode = none;
		
//   	ZEnable 				= True;	
		ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= True;		
	} 
}