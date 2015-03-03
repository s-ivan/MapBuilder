
//////////////////////////////////////////////////////////////////////////////////////////////////
// structs VS

struct outAnimTree1lVS
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

outAnimTree1lVS AnimTree1VS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0
	) 
{ 
   outAnimTree1lVS Out;
	
//	InPos.w = 1.0f;																			// to get proper matrix multiplication?
		
	//----------------------------------------------------------------
	
	Out.Tex = InTex;
	
	//---------------------------------------------------------------
	
	float4 PosWorld = mul(InPos, matWorld);
	
	//----------------------------------------------------------------     
	
	// move only upper part of tree - value is read from skill44
	if (InPos.y > vecSkill41.w ) 											
		{
			float force_x = 0.025;																								// was DoDefault(vecSkill41.x*(0.1/50),0.1); 
			float force_y = 0.025;																								// was DoDefault(vecSkill41.y*(0.1/50),0.1);
			
			float speed = sin( (vecTime.w + 0.2f * (PosWorld.x + PosWorld.y + PosWorld.z)) * 0.02f );		// was DoDefault(vecSkill41.z*(0.05/50),0.05)
				
			InPos.x += speed * force_x * InPos.y;																		
			InPos.z += speed * force_y * InPos.y;
			InPos.y -= 0.1 * abs(speed * (force_x + force_y)) * InPos.y;
		}
	
	//----------------------------------------------------------------
	
	Out.Pos = mul(InPos, matWorldViewProj); 
	
	//----------------------------------------------------------------
	
	PosWorld = mul(InPos, matWorld);														// recalculate due to modified position
{}
#ifdef PER_PIXEL_LIGHTS
  	Out.WorldPos    = PosWorld.xyz;
#endif 
	
	//----------------------------------------------------------------
	
	Out.ViewDir = vecViewPos - PosWorld;												// for flat specular - or = matViewInv[3].xyz - PosWorld;	= halfway vector
	
	//----------------------------------------------------------------
	
//	Out.SunDir = PosWorld.xyz - vecSunPos;
	
	//----------------------------------------------------------------
	
	Out.WorldNormal = normalize(mul(InNormal, (float3x3)matWorld));			// when no normalmap used - normalize because of entity scale!
	
	//----------------------------------------------------------------
	
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
	
	Out.Color = (vecAmbient * vecLight) + (vecEmissive * vecColor);			// ambient + emissive
		
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