
//////////////////////////////////////////////////////////////////////////////////////////////////
// structs

struct outBonesVS 
{
	float4 Color			: COLOR0; 
   float4 Pos				: POSITION; 
   float2 Tex				: TEXCOORD0;
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
#ifdef PER_PIXEL_LIGHTS    
   float3 ViewDir			: TEXCOORD1;			//-
   float3 WorldNormal	: TEXCOORD2;
	float3 WorldPos		: TEXCOORD4;			// needed only for ps per pixel dynamic lights
#endif
	float4 Shadow			: TEXCOORD7;			//-
   float  Fog 				: FOG;
};

struct outBonesVS2
{
	float4 Color			: COLOR0; 
   float4 Pos				: POSITION; 
   float2 Tex				: TEXCOORD0;
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
#ifdef PER_PIXEL_LIGHTS_LOD
   float3 ViewDir			: TEXCOORD1;			//-
   float3 WorldNormal	: TEXCOORD2;
	float3 WorldPos		: TEXCOORD4;			// needed only for ps per pixel dynamic lights
#endif
	float4 Shadow			: TEXCOORD7;			//-
   float  Fog 				: FOG;
};

//struct outBonesVSX
//{
//	float4 Color			: COLOR0; 
//   float4 Pos				: POSITION; 
//   float2 Tex				: TEXCOORD0;
//   float  Fog 				: FOG;
//};

////////////////////////////////////////////////////////////////////////////////////////////
// bone animation from default.fx with lowered bone number

float4x3 matBones[36];
int		iWeights;

float4 DoBones(float4 Pos, int4 BoneIndices, float4 BoneWeights, int Weights)
{
	if(Weights == 0) return Pos;
	
	float3 OutPos = 0;
	for(int i=0; i<Weights; i++)
		OutPos += mul(Pos.xzyw, matBones[BoneIndices[i]]) * BoneWeights[i];
		
	return float4(OutPos.xzy, 1.0);
}

// only rotation and translation => inv(transpose(matWorld)) == matWorld
float3 DoBones(float3 Normal, int4 BoneIndices, float4 BoneWeights, int Weights)
{
	if(Weights == 0) return Normal;
	
	float3 OutNormal = 0;
	for(int i=0; i<Weights; i++)
		OutNormal += mul(Normal.xzy, (float3x3)matBones[BoneIndices[i]]) * BoneWeights[i];
		
	return normalize(OutNormal.xzy);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

outBonesVS BonesVS1
	( 
   float4 InPos			: POSITION, 
   float3 InNormal		: NORMAL, 
   float2 InTex			: TEXCOORD0,
   
   int4 inBoneIndices	: BLENDINDICES,
	float4 inBoneWeights	: BLENDWEIGHT
	) 
{ 
   outBonesVS Out;
	
	//----------------------------------------------------------------
   
   Out.Tex = InTex;
   
	//----------------------------------------------------------------
	 
	float4 bonePos 	= DoBones(InPos, inBoneIndices, inBoneWeights, iWeights);
	
	//----------------------------------------------------------------
   
   float4 PosWorld	= mul(bonePos, matWorld);																
	
	//----------------------------------------------------------------
	
	Out.Pos 				= mul(bonePos, matWorldViewProj); 							    
   
	//----------------------------------------------------------------	
{}
#ifdef PER_PIXEL_LIGHTS
  	Out.WorldPos    = PosWorld;
	
	//----------------------------------------------------------------
	
	Out.ViewDir = vecViewPos - PosWorld;																
		
	//----------------------------------------------------------------
	
	Out.WorldNormal = normalize(mul(DoBones(InNormal, inBoneIndices, inBoneWeights, iWeights), matWorld));	
#else
	float3 WorldNormal = normalize(mul(DoBones(InNormal, inBoneIndices, inBoneWeights, iWeights), matWorld));	
#endif 
	
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
	
	Out.Color = (vecAmbient + vecEmissive) + vecColor;								// ambient + emissive + rgb
		
{}
#ifndef PER_PIXEL_LIGHTS
	
	float4 Lights = 0;	
			
	for (int i=1; i<=iLights; i++)  														// add dynamic lights, and Sun (always the last one)
		Lights += PointLightDiffuse(PosWorld, WorldNormal, i-1);			
	
	Lights *= 2.0f * vecDiffuse;																
	
	Out.Color += Lights;
	
#endif				
	
   //-----------------------------------------------------------------
   
   return Out;
}


outBonesVS BonesVS2
	( 
   float4 InPos			: POSITION, 
   float3 InNormal		: NORMAL, 
   float2 InTex			: TEXCOORD0,
   
   int4 inBoneIndices	: BLENDINDICES,
	float4 inBoneWeights	: BLENDWEIGHT
	) 
{ 
   outBonesVS2 Out;
	
	//----------------------------------------------------------------
   
   Out.Tex = InTex;
   
	//----------------------------------------------------------------
	 
	float4 bonePos 	= DoBones(InPos, inBoneIndices, inBoneWeights, min(1, iWeights));
	
   //----------------------------------------------------------------
   
   float4 PosWorld	= mul(bonePos, matWorld);															
	
	//----------------------------------------------------------------
	
	Out.Pos 				= mul(bonePos, matWorldViewProj); 							    
   
	//----------------------------------------------------------------	
{}
#ifdef PER_PIXEL_LIGHTS_LOD
  	Out.WorldPos    = PosWorld;
	
	//----------------------------------------------------------------
	
	Out.ViewDir = vecViewPos - PosWorld;																
		
	//----------------------------------------------------------------
	
	Out.WorldNormal = normalize(mul(DoBones(InNormal, inBoneIndices, inBoneWeights, min(1, iWeights)), matWorld));
#else
	float3 WorldNormal = normalize(mul(DoBones(InNormal, inBoneIndices, inBoneWeights, min(1, iWeights)), matWorld));
#endif 
	
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
   
   Out.Shadow = 0;
   
	//----------------------------------------------------------------
	// lighting
	
	Out.Color = (vecAmbient + vecEmissive) + vecColor;								// ambient + emissive + rgb
		
{}
#ifndef PER_PIXEL_LIGHTS_LOD
	
	float4 Lights = 0;	
			
	for (int i=1; i<=iLights; i++)  														// add dynamic lights, and Sun (always the last one)
		Lights += PointLightDiffuse(PosWorld, WorldNormal, i-1);			
	
	Lights *= 2.0f * vecDiffuse;																
	
	Out.Color += Lights;
	
#endif				
	
   //-----------------------------------------------------------------
   
   return Out;
}


//////////////////////////////////////////////////////////////////////
//
//
//outBonesVSX BonesVSX
//	( 
//   float4 InPos			: POSITION, 
//   float3 InNormal		: NORMAL, 
//   float2 InTex			: TEXCOORD0,
//   
//   int4 inBoneIndices	: BLENDINDICES,
//	float4 inBoneWeights	: BLENDWEIGHT
//	) 
//{
//	outBonesVSX Out;
//	
//	//----------------------------------------------------------------
//   
//   Out.Tex = InTex;
//   
//	//----------------------------------------------------------------
//	 
//	float4 bonePos 	= DoBones(InPos, inBoneIndices, inBoneWeights, min(1, iWeights));
//	Out.Pos 				= mul(bonePos, matWorldViewProj); 							    
//   
//   //----------------------------------------------------------------
//   
//   float4 PosWorld 	= mul(bonePos, matWorld);															
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
//	//----------------------------------------------------------------
//	
//   Out.Color = float4(1,0,0,1);
//   
//   //-----------------------------------------------------------------
//   
//   return Out;
//}
//