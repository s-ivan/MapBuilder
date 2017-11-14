////////////////////////////////////////////////////////////////////////////////////////////////
// 
// Model VERTEX LOD V1 shader for exponential shadowmapping - MapBuilder2
// Copyright Iván Stummer (2014) All Rights Reserved
// 
////////////////////////////////////////////////////////////////////////////////////////////////
// 
// lighting calculations are done differently in different lods (in VS or in PS)
// supports 8 dynamic lights (including Sun)
// diffuse calculation for each light
// additional diffuse calculation for Sun (useful only its normal mapping version)
// specular calculation only for Sun
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
   float3 SunDir			: TEXCOORD3;
#ifdef PER_PIXEL_LIGHTS
	float3 WorldPos		: TEXCOORD4;
#endif
	float4 Shadow			: TEXCOORD7;
   float  Fog 				: FOG;
};

struct outModelVS2
{
	float4 Pos				: POSITION; 
	float4 Color			: COLOR0;    
   float2 Tex				: TEXCOORD0; 
	float4 Shadow			: TEXCOORD7;
   float  Fog 				: FOG;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader for pixel shading

outModelVS ModelVS
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
	
	Out.ViewDir = vecViewPos.xyz - PosWorld.xyz;										// for flat specular - or = matViewInv[3].xyz - PosWorld;	= halfway vector
	
	//----------------------------------------------------------------
	
	Out.SunDir = PosWorld.xyz - vecSunPos;
	
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
	
	Out.Shadow = VS_shadowmapping(PosWorld);
	
	//----------------------------------------------------------------
	// lighting
	
	// final color
	Out.Color = (vecAmbient * vecLight) + (vecEmissive * vecColor);			// ambient + emissive
		
{}
#ifndef PER_PIXEL_LIGHTS
	// dynamic lights  (Sun is always the last one)
	float4 Lights = 0;	
			
	for (int i=1; i<iLights; i++)  														// Add max 8 dynamic lights
		Lights += PointLightDiffuse(PosWorld, Out.WorldNormal, i-1);			// matTangent[2] or OutWorldNormal
	
	Lights *= vecDiffuse * 2.0f;
	
	Out.Color += Lights;
#endif				
	
	//-----------------------------------------------------------------
	
	return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader for vertex shading

outModelVS2 ModelVS2
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0
	) 
{ 
	outModelVS2 Out;
	
//	InPos.w = 1.0f;																			// to get proper matrix multiplication?
	
	//----------------------------------------------------------------
	
	Out.Tex = InTex;
	
	//---------------------------------------------------------------
	
	float4 PosWorld = mul(InPos, matWorld);											// needed for a couple of calculations... float4 for shadowmapping!
	
	//----------------------------------------------------------------
	
	Out.Pos = mul(InPos, matWorldViewProj); 
	
	//----------------------------------------------------------------
	
//	Out.WorldPos = PosWorld.xyz;															// float3 !
	
	//----------------------------------------------------------------
	
	float3 ViewDir = vecViewPos.xyz - PosWorld.xyz;								
	
	//----------------------------------------------------------------
	
	float3 SunDir = normalize(PosWorld.xyz - vecSunPos);
	
	//----------------------------------------------------------------
	
	float3 WorldNormal = normalize(mul(InNormal, (float3x3)matWorld));		
	
	//----------------------------------------------------------------
{}   
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
	
//	// ambient
//   float4 Ambient = vecAmbient * vecLight;		
//	
//	// emissive
//	float4 Emissive = vecEmissive * vecColor;
	
	float LightIntensity = dot(WorldNormal, -SunDir);
	
	// diffuse
	float4 Diffuse = 2.0f * vecDiffuse * LightIntensity * vecSunColor * vecLight;
	
	// specular
	float4 Specular = 0;
	if (LightIntensity > 0.25f)
		{
			float3 Halfway  = saturate(dot(normalize(ViewDir - SunDir), WorldNormal));
			Specular = vecSpecular * pow( dot(WorldNormal, Halfway) , fPower) * vecSunColor * vecLight;	
		}
	
	// dynamic lights
	float4 Lights = 0;	
	for (int i=1; i<=iLights; i++)  															// Add max 8 dynamic lights
		Lights += PointLightDiffuse(PosWorld, WorldNormal, i-1);						// matTangent[2] or OutWorldNormal
	Lights *= 2.0f * vecDiffuse;
	
	// final color
	Out.Color = (vecAmbient * vecLight) + (vecEmissive * vecColor);				// ambient + emissive
	Out.Color += Diffuse + Specular + Lights;
	
   //-----------------------------------------------------------------
   
   return Out;
}