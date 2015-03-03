
bool AUTORELOAD;

///////////////////////////////////////////////////////////

#include <transform>
//#include <bones>
#include <fog>
#include <pos>
#include <normal>
#include <lights>
#include <texture>
#include <color>

//---------------------------------

#include "sc_defines_MB.fx"

#ifdef HEIGHT_FOG
	float4 vecFogColor;						// fog_color_var>0 or vecFogColor.w==1 if fog is on
	
	#include "sc_heightfog_MB.fx"			// blend distance fog with height fog
#endif

float shadow_stencil_var;

///////////////////////////////////////////////////////////
// rfom <bones> section of default.fx with lowered bones quantity

float4x3 matBones[36];		// was 72 in <bones> - 36 is enough but much faster!
int iWeights;


float4 DoBones(float4 Pos, int4 BoneIndices, float4 BoneWeights, int Weights)
{
	if(Weights == 0) return Pos;
	
	float3 OutPos = 0;
	for(int i=0; i<Weights; i++)
		OutPos += mul(Pos.xzyw,matBones[BoneIndices[i]])*BoneWeights[i];
		
	return float4(OutPos.xzy,1.0);
}

// only rotation and translation => inv(transpose(matWorld)) == matWorld
float3 DoBones(float3 Normal, int4 BoneIndices, float4 BoneWeights, int Weights)
{
	if(Weights == 0) return Normal;
	
	float3 OutNormal = 0;
	for(int i=0; i<Weights; i++)
		OutNormal += mul(Normal.xzy,(float3x3)matBones[BoneIndices[i]])*BoneWeights[i];
		
	return normalize(OutNormal.xzy);
}

///////////////////////////////////////////////////////////

struct vsOut
{
	float4 Pos		:	POSITION;
	float  Fog		:	FOG;		
	float4 Ambient	:	COLOR;
	float2 Tex		:	TEXCOORD0;
};

///////////////////////////////////////////////////////////

vsOut bones_VS1(
					   in float4 inPos			: 	POSITION, 
					   in float3 inNormal		:	NORMAL,
					   in float2 inTex			: 	TEXCOORD0,
					   
					   in int4   inBoneIndices	:	BLENDINDICES,
					   in float4 inBoneWeights	:	BLENDWEIGHT
				   )
{
	vsOut Out;
	
	float4 Pos = DoBones(inPos, inBoneIndices, inBoneWeights, iWeights);
	
	Out.Pos	= DoTransform(Pos); 
	Out.Tex	= DoTexture(inTex);
	Out.Fog	= 0;//DoFog(inPos);											// not used if no pixel shader
	
	float3 P = DoPos(Pos);
	float3 N = DoNormal(DoBones(inNormal, inBoneIndices, inBoneWeights, iWeights));
	
	Out.Ambient = DoAmbient();	
		
	for (int i=1; i<=iLights; i++)  										// Add max 8 dynamic lights
		Out.Ambient.rgb += DoLight(P,N,i-1);				
	
	Out.Ambient.rgb *= vecDiffuse;										// *0.5
	
{}
#ifdef HEIGHT_FOG  	 
//	if (shadow_stencil_var < 0)
		{
			float temp	= VS_heightfog_SM2(P.xyz);
			Out.Ambient.rgb = lerp(Out.Ambient.rgb, vecFogColor.rgb, temp);
		}
#endif				
		
	return Out;		
}


vsOut bones_VS2(
					   in float4 inPos			: 	POSITION, 
					   in float3 inNormal		:	NORMAL,
					   in float2 inTex			: 	TEXCOORD0,
					   
					   in int4   inBoneIndices	:	BLENDINDICES,
					   in float4 inBoneWeights	:	BLENDWEIGHT
				   )
{
	vsOut Out;
	
	float4 Pos = DoBones(inPos, inBoneIndices, inBoneWeights, min(1, iWeights));
	
	Out.Pos	= DoTransform(Pos); 
	Out.Tex	= DoTexture(inTex);
	Out.Fog	= 0;//DoFog(inPos);											// not used if no pixel shader
	
	float3 P = DoPos(Pos);
	float3 N = DoNormal(DoBones(inNormal, inBoneIndices, inBoneWeights, min(1, iWeights)));
//	float3 N = DoNormal(inNormal);										// not exact here and looks bad...
	
	Out.Ambient = DoAmbient();	
		
	for (int i=1; i<=iLights; i++)  										// Add max 8 dynamic lights
		Out.Ambient.rgb += DoLight(P,N,i-1);				
	
	Out.Ambient.rgb *= vecDiffuse;										// *0.5
	
{}
#ifdef HEIGHT_FOG  	 
//	if (shadow_stencil_var < 0)
		{
			float temp	= VS_heightfog_SM2(P.xyz);
			Out.Ambient.rgb = lerp(Out.Ambient.rgb, vecFogColor.rgb, temp);
		}
#endif				
		
	return Out;		
}


///////////////////////////////////////////////////////////

technique bones
{
	pass 
		{
			VertexShader = compile vs_2_0 bones_VS1();
		}
}

technique bones_lod1
{
	pass 
		{
			VertexShader = compile vs_2_0 bones_VS2();
		}
}

technique bones_lod2
{
	pass 
		{
			VertexShader = compile vs_2_0 bones_VS2();
		}
}

technique bones_lod3
{
	pass 
		{
			VertexShader = compile vs_2_0 bones_VS2();
		}
}

///////////////////////////////////////////////////////////

technique fallback 
{
	pass { } 
}

///////////////////////////////////////////////////////////