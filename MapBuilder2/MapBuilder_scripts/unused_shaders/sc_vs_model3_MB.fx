////////////////////////////////////////////////////////////////////////////////////////////////
// 
// FLAT MODEL shader for exponential shadowmapping - MapBuilder2 - PIXEL V3
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
////////////////////////////////////////////////////////////////////////////////////////////////
// structs VS

struct outModelVS
{
	float4 Pos				: POSITION;
	float4 Color			: COLOR0;    
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
//	float3 SunDir			: TEXCOORD3;
#ifdef PER_PIXEL_LIGHTS
	float3 WorldPos		: TEXCOORD4;
#endif
	float4 Shadow			: TEXCOORD7;
   float  Fog 				: FOG;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader for pixel shading

outModelVS Model3VS
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
{}
#ifdef PER_PIXEL_LIGHTS	
  	Out.WorldPos = PosWorld.xyz;															// float3 !
#endif	
	//----------------------------------------------------------------
	
	Out.ViewDir = normalize(vecViewPos.xyz - PosWorld.xyz);
	
	//----------------------------------------------------------------
	
//	Out.SunDir = PosWorld.xyz - vecSunPos;
	
	//----------------------------------------------------------------
	
	Out.WorldNormal = normalize(mul(InNormal, (float3x3)matWorld));			// normalize because of entity scale!
	
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
	
	for (int i=1; i<iLights; i++)																				
		Lights += PointLightDiffuseSpecular(PosWorld, Out.WorldNormal, i-1, 1.0f, Out.ViewDir);
	
	Lights += 2.0f * PointLightDiffuseSpecular(PosWorld, Out.WorldNormal, iLights-1, 1.0f, Out.ViewDir);
	
	Lights *= 4.0;
	
	Out.Color += Lights;																		// + diffuse + specular
	
#endif				
	
	//-----------------------------------------------------------------
	
	return Out;
}
