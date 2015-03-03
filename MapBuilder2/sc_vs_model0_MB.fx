
//////////////////////////////////////////////////////////////////////////////////////////////////
// structs VS

struct outModelVS
{
	float4 Pos				: POSITION;
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
//	float4 Shadow			: TEXCOORD7;
   float  Fog 				: FOG;
};

struct outModelVS2
{
	float4 Pos				: POSITION; 
	float4 Color			: COLOR0;    
   float2 Tex				: TEXCOORD0; 
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
#ifdef PER_PIXEL_LIGHTS_LOD
	float3 WorldNormal	: TEXCOORD2;
	float3 WorldPos		: TEXCOORD4;
#endif    
//	float4 Shadow			: TEXCOORD7;
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
	
//	Out.Shadow = VS_shadowmapping(PosWorld);
	
	//----------------------------------------------------------------
	// lighting
	
	// final color	
	Out.Color = (vecAmbient * vecLight) + (vecEmissive * vecColor);			// ambient + emissive
		
{}
#ifndef PER_PIXEL_LIGHTS
	
	float4 Lights = 0;	
			
	for (int i=1; i<iLights; i++)  														// add dynamic lights, ignore Sun (always the last one)
		Lights += PointLightDiffuse(PosWorld, Out.WorldNormal, i-1);			
	
	Lights *= 2.0f * vecDiffuse;																
	
	// final color	
	Out.Color += Lights;
	
#endif				
	
	//-----------------------------------------------------------------
	
	return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader for vertex shading - only for lod3

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
{}
#ifdef PER_PIXEL_LIGHTS_LOD 	
	Out.WorldPos = PosWorld.xyz;															// float3 !
#endif	
	//----------------------------------------------------------------
	
	float3 ViewDir = vecViewPos.xyz - PosWorld.xyz;								
	
	//----------------------------------------------------------------
	
//	float3 SunDir = normalize(PosWorld.xyz - vecSunPos);
	
	//----------------------------------------------------------------
	
	float3 WorldNormal = normalize(mul(InNormal, (float3x3)matWorld));		
{}
#ifdef PER_PIXEL_LIGHTS_LOD 
	Out.WorldNormal = WorldNormal;
#endif
		
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
	
//	Out.Shadow = VS_shadowmapping(PosWorld);
	
	//----------------------------------------------------------------
	// lighting		
	
	Out.Color = (vecAmbient * vecLight) + (vecEmissive * vecColor);				// ambient + emissive
	
	// Sun lighting 
	
	float LightIntensity = saturate(dot(WorldNormal, -vecSunDir));
	
	// diffuse
	float4 Diffuse = scsm_brightness * 5.0 * vecDiffuse * LightIntensity * vecSunColor * vecLight;
	
	// specular
	float4 Specular = 0;
	if (LightIntensity > 0.33f)
		{
			float3 Halfway  = saturate(dot(normalize(ViewDir - vecSunDir), WorldNormal));
			Specular = vecSpecular * pow( dot(WorldNormal, Halfway) , fPower) * vecSunColor * vecLight;	
		}
	
	// final color	
	Out.Color += Diffuse + Specular;	
	
	// dynamic lights	
{}
#ifndef PER_PIXEL_LIGHTS_LOD
		
	float4 Lights = 0;	
		
	for (int i=1; i<iLights; i++)  															// Add max 8 dynamic lights, except Sun
		Lights += PointLightDiffuse(PosWorld, WorldNormal, i-1);					
	
	Lights *= 2.0f * vecDiffuse;																
	
	// final color
	Out.Color += Lights;
	
#endif			
	
   //-----------------------------------------------------------------
   
   return Out;
}