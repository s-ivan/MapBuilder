
//////////////////////////////////////////////////////////////////////////////////////////////////
// structs VS

struct outAnimGrass2VS
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
// helpers

float4 SmoothCurve( float4 x ) 
{  
  return x * x *( 3.0 - 2.0 * x );  
}  

float4 TriangleWave( float4 x )
{  
  return abs( frac( x + 0.5 ) * 2.0 - 1.0 );  
}
  
float4 SmoothTriangleWave( float4 x )
{  
  return SmoothCurve( TriangleWave( x ) );  
}  

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

outAnimGrass2VS AnimGrass2VS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0
	) 
{ 
   outAnimGrass2VS Out;
	
//	InPos.w = 1.0f;																			// to get proper matrix multiplication?
		
	//----------------------------------------------------------------
	
	Out.Tex = InTex;
	
	//---------------------------------------------------------------
	
	float4 PosWorld = mul(InPos, matWorld);
	
	//----------------------------------------------------------------     
   
   // fine-oscillate 
	// Phases (object, vertex, branch)  
   float fBranchPhase = InPos.x; //1;
   float fDetailPhase = InPos.y;	//1;
      
   float fObjPhase = dot(PosWorld.xyz, 1);  
	fBranchPhase += fObjPhase;  
	
	float fVtxPhase = dot(InPos.xyz, fDetailPhase + fBranchPhase);  
	
	// x is used for edges; y is used for branches  
   float2 vWavesIn = vecTime.w + float2(fVtxPhase, fBranchPhase );  
   
	// 1.975, 0.793, 0.375, 0.193 are good frequencies  
	float fSpeed = 0.10;
	float fDetailFreq = 0.20;
   float4 vWaves = (frac( vWavesIn.xxyy * float4(1.975, 0.793, 0.375, 0.193) ) * 2.0 - 1.0 ) * fSpeed * fDetailFreq;
   
	vWaves = SmoothTriangleWave( vWaves );  
	
	float2 vWavesSum = vWaves.xz + vWaves.yw;  
	
	// Edge (xy) and branch bending (z)  
	float fEdgeAtten = 0.1;
	float fDetailAmp = 10;
	float fBranchAtten = 3.0;
	float fBranchAmp = 10;
	InPos.xyz += vWavesSum.xxy * float3(fEdgeAtten * fDetailAmp * InNormal.xy, fBranchAtten * fBranchAmp);  
	
   //----------------------------------------------------------------   
	
	float force_x = 0.10;	//DoDefault(vecSkill41.x*(0.1/50),0.1); 
	float force_y = 0.10;	//DoDefault(vecSkill41.y*(0.1/50),0.1);
	
	float speed = sin( (vecTime.w+0.2*(PosWorld.x+PosWorld.y+PosWorld.z)) * 0.03 );	//DoDefault(vecSkill41.z*(0.05/50),0.05)
	
	// * (1 - Out.Tex.y) to move only upper vertices
	InPos.x += speed * force_x * InPos.y * (1 - Out.Tex.y);
	InPos.z += speed * force_y * InPos.y * (1 - Out.Tex.y);
	InPos.y -= 0.1*abs(speed*(force_x+force_y)) * InPos.y * (1 - Out.Tex.y);
	
	//----------------------------------------------------------------
	
	Out.Pos = mul(InPos, matWorldViewProj); 
	
	//----------------------------------------------------------------
	
	PosWorld = mul(InPos, matWorld);														// recalculate due to modified position
{}
#ifdef PER_PIXEL_LIGHTS	
  	Out.WorldPos    = PosWorld.xyz;
#endif 
	
	//----------------------------------------------------------------
	
	Out.ViewDir = vecViewPos - PosWorld;												// for flat specular - or = matViewInv[3].xyz - PosWorld;	= for halfway vector
	
	//----------------------------------------------------------------
	
//	Out.SunDir = PosWorld.xyz - vecSunPos;
	
	//----------------------------------------------------------------
	
	Out.WorldNormal = normalize(mul(InNormal, (float3x3)matWorld));		
   
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