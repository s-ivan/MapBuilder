bool AUTORELOAD;

/////////////////////////////////////////////////////////////////////////

#define BONES 																			// activate GPU bones (reduces the frame rate by 5%)

#ifdef BONES
	#include <bones>
#endif

#include <pos>

/////////////////////////////////////////////////////////////////////////

float d3d_alpharef_var;
float4x4 matSplitViewProj;																// set up from the pssm script

/////////////////////////////////////////////////////////////////////////

texture entSkin1;
sampler sBaseTex = sampler_state { Texture = <entSkin1>; MipFilter = linear; };

/////////////////////////////////////////////////////////////////////////

void renderDepth_VS	(
							in float4 inPos: POSITION,
							in float2 inTex: TEXCOORD0,
#ifdef BONES
							in int4 inBoneIndices: BLENDINDICES,
							in float4 inBoneWeights: BLENDWEIGHT,
#endif	
							out float4 outPos: POSITION,
							out float4 PixelPos: TEXCOORD0,
							out float2 TexCoord: TEXCOORD1
							)
{
#ifdef BONES
  outPos = DoPos(DoBones(inPos,inBoneIndices,inBoneWeights));
#else  
  outPos = DoPos(inPos);
#endif  
  outPos = mul(outPos,matSplitViewProj);
  PixelPos = outPos;
  TexCoord = inTex;
}

/////////////////////////////////////////////////////////////////////////

float4 renderDepth_PS(float4 PixelPos: TEXCOORD0, float2 TexCoord: TEXCOORD1): COLOR
{
	clip(tex2D(sBaseTex,TexCoord).a-d3d_alpharef_var/255.);					// skip transparent parts of textures
	return PixelPos.z / PixelPos.w;
}

/////////////////////////////////////////////////////////////////////////

technique renderDepth
{
	pass p0
		{
			VertexShader = compile vs_3_0 renderDepth_VS();
			PixelShader = compile ps_3_0 renderDepth_PS();
		}
}
