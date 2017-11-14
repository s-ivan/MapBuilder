
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
	float4 Shadow			: TEXCOORD7;
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
	
	// final color	
	Out.Color = (vecAmbient + vecEmissive) + vecColor;								// ambient + emissive + rgb
		
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
	
	Out.Shadow = VS_shadowmapping(PosWorld);
	
	//----------------------------------------------------------------
	// lighting		
	
	Out.Color = (vecAmbient + vecEmissive) + vecColor;								// ambient + emissive + rgb
	
	// Sun lighting 
	
	float LightIntensity = saturate(dot(WorldNormal, -vecSunDir));
	
	// diffuse
	float4 Diffuse = scsm_brightness * 1.0 * vecDiffuse * LightIntensity * vecSunColor ;
	
	// specular
	float4 Specular = 0;
	if (LightIntensity > 0.33f)
		{
			float3 Halfway  = saturate(dot(normalize(ViewDir - vecSunDir), WorldNormal));
			Specular = vecSpecular * pow( dot(WorldNormal, Halfway) , fPower) * vecSunColor;	
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader for vertex shading - only for lod3 - billboard

//float4x4 matViewProj;
float4 vecSkill1;
//float4 vecSkill45;

outModelVS2 ModelVS2B
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0
	) 
{ 
	outModelVS2 Out;
	
//	InPos.w = 1.0f;																			// to get proper matrix multiplication?
	
	//----------------------------------------------------------------
	// billboard
	
	// no entity rotation supported
	InPos.xz = float2 ( InPos.x * vecSkill1.y - InPos.z * vecSkill1.x, InPos.x * vecSkill1.x + InPos.z * vecSkill1.y );
	
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
	
//	float3 WorldNormal = normalize(mul(InNormal, (float3x3)matWorld));											// original vertex normal is not relevant now, pointing from initial non-rotated vertex !
	float3 WorldNormal = normalize(mul(normalize(float3(0.0f, 1.0f, 0.0f)), (float3x3)matWorld));		// vertical normals always! float3(0.0f,1.0f,0.0f)
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
	
	Out.Shadow = VS_shadowmapping(PosWorld);
	
	//----------------------------------------------------------------
	// lighting		
	
	Out.Color = (vecAmbient + vecEmissive) + vecColor;								// ambient + emissive + rgb
	
	// Sun lighting 
	
	float LightIntensity = saturate(dot(WorldNormal, -vecSunDir));
	
	// diffuse
	float4 Diffuse = scsm_brightness * 1.0 * vecDiffuse * LightIntensity * vecSunColor;
	
	// specular
	float4 Specular = 0;
	if (LightIntensity > 0.33f)
		{
			float3 Halfway  = saturate(dot(normalize(ViewDir - vecSunDir), WorldNormal));
			Specular = vecSpecular * pow( dot(WorldNormal, Halfway) , fPower) * vecSunColor;	
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











float4x4 matWorldView;
float4x4 matView;
float4x4 matProj;

outModelVS2 ModelVS3B
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
	// billboard
	
	
	float width = 32;
	float height = 64;
	
	
	float3 rightVector = normalize(cross(ViewDir, InNormal));
	
	// Calculate the position of this billboard vertex.
	float3 position = InPos;
	
	// Offset to the left or right.
	position += rightVector * (InTex.x - 0.5) * width;
	
	// Offset upward if we are one of the top two vertices.
	position += InNormal * (1 - InTex.y) * height;
	
//	// overwrite
//	Out.Pos = mul(float4(position, 1), matWorldViewProj);
	
	// Apply the camera transform.
	float4 viewPosition = mul(float4(position, 1), matWorld);
	
	viewPosition = mul(viewPosition, matView);
	
	// overwrite
	Out.Pos = mul(viewPosition, matProj);
	
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
	
	Out.Color = (vecAmbient + vecEmissive) + vecColor;								// ambient + emissive + rgb
	
	// Sun lighting 
	
	float LightIntensity = saturate(dot(WorldNormal, -vecSunDir));
	
	// diffuse
	float4 Diffuse = scsm_brightness * 1.0 * vecDiffuse * LightIntensity * vecSunColor;
	
	// specular
	float4 Specular = 0;
	if (LightIntensity > 0.33f)
		{
			float3 Halfway  = saturate(dot(normalize(ViewDir - vecSunDir), WorldNormal));
			Specular = vecSpecular * pow( dot(WorldNormal, Halfway) , fPower) * vecSunColor;	
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