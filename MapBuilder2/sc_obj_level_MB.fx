
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
float fPower;

////////////////////////////////////////////////////////////////////////////////////////////////
// skins/textures

texture entSkin1; 							// Colormap
//texture entSkin2; 							// lightmap usually looks bad with dynamic shadows

// Color map sampler
sampler colorSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

// Lightmap sampler							// lightmap usually looks bad with dynamic shadows
//sampler shadowSampler = sampler_state 
//{ 
//   Texture = <entSkin2>; 
//   MinFilter = Linear;
//	MagFilter = Linear;
//	MipFilter = Linear;
//};

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
// structs

struct outLevelVS 
{
	float4 Color			: COLOR0; 
   float4 Pos				: POSITION; 
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
//	float2 ShadowTex		: TEXCOORD6; 			// lightmap usually looks bad with dynamic shadows
   float  Fog 				: FOG;
};


struct inLevelPS 
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
//	float2 ShadowTex		: TEXCOORD6; 			// lightmap usually looks bad with dynamic shadows
	float  Fog 				: FOG;
};

////////////////////////////////////////////////////////////////////////////////////////////////

// Vertex Shader
outLevelVS LevelVS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
//	float2 InShadow	: TEXCOORD1,														// lightmap usually looks bad with dynamic shadows
   float2 InTex		: TEXCOORD0
	) 
{ 
   outLevelVS Out;
   
//	InPos.w = 1.0f;																			// to get proper matrix multiplication?
   
   //----------------------------------------------------------------
   
	// Pass the texture coordinate to the pixel shader
   Out.Tex = InTex;
   
   // Pass the lightmap coordinate to the pixel shader
//	Out.ShadowTex = InShadow;																// lightmap usually looks bad with dynamic shadows
   
   //----------------------------------------------------------------
   
   float4 PosWorld = mul(InPos, matWorld);											// needed for a couple of calculations... float4 for shadowmapping!
   
   //----------------------------------------------------------------
	
	Out.Pos = mul(InPos, matWorldViewProj); 
	
   //----------------------------------------------------------------
{}
#ifdef PER_PIXEL_LIGHTS	
  	Out.WorldPos = PosWorld.xyz;															// float3 !
#endif
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
	
	Out.Color = vecAmbient + vecEmissive + vecColor;								// ambient + emissive + rgb
		
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

////////////////////////////////////////////////////////////////////////////////////////////////

float4 LevelPS(inLevelPS In): COLOR
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
   // lighting
   
   float LightIntensity = saturate(dot(In.WorldNormal, -vecSunDir));
      
   // diffuse   
   float4 Diffuse = scsm_brightness * 1.0 * vecDiffuse * LightIntensity * vecSunColor;			// 4 instead of 8 to match model, but not okay alone, there are problems with ambient/emissive engine behaviour!!!										
	
	// specular										
	float4 Specular = 0;
	if (LightIntensity > 0.25f)
		{
			float3 Halfway  = saturate(dot(normalize(In.ViewDir - vecSunDir), In.WorldNormal));			
			Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor;					
		}
	// specular - another calculation
//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - In.SunDir);													// R
//	float4 Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor;			// R.V^n 
	
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
   //----------------------------------------------------------
   // dynamic lights
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
	
	Color.rgb *= Shadow;																							// dynamic shadows
	
	//----------------------------------------------------------
	// add lightmap															
	
//	Color.rgb *= clamp(tex2D(shadowSampler, In.ShadowTex).rgb, scsm_lightmapalpha_var, 1);	// lightmap usually looks bad with dynamic shadows
	
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
	
	return Color;
}

////////////////////////////////////////////////////////////////////////////////////////////////

technique t01
{ 
   pass P0 
   {
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
		
		VertexShader = compile vs_3_0 LevelVS();
     	PixelShader  = compile ps_3_0 LevelPS();
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