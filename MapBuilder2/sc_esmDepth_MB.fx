
bool AUTORELOAD;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

#define BONES // activate GPU bones (reduces the frame rate by 5%)

#ifdef BONES
	#include <bones>
#endif

////////////////////////////////////////////////////////////////////////////////////////////////
// matrices, vectors, variables passed by the engine

float4x4 matWorldViewProj;
float4x4 matWorldView;

float4x4 matEffect1;
//float4x4 matEffect2;

float4x4 matWorld;
//float4   vecSkill1;

//float d3d_alpharef_var;

////////////////////////////////////////////////////////////////////////////////////////////////
// custom matrices, vectors, variables

//float scsm_blursize_flt = 0.00195;

float scsm_maxdepth_var;

////////////////////////////////////////////////////////////////////////////////////////////////
// skins/textures/samplers

 Texture entSkin1;

sampler colorSampler = sampler_state 
{ 
	texture = <entSkin1>;
	magfilter = LINEAR;
	minfilter = LINEAR;
	mipfilter = LINEAR;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs

struct SMapVertexToPixel
{
	float4 Position	: POSITION;    
	float3 Position2D	: TEXCOORD0;
	float2 Tex			: TEXCOORD1;
};

struct SMapPixelToFrame
{
	float4 Color		: COLOR0; 
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

SMapVertexToPixel ShadowMapVertexShader( 
														float4 inPos : POSITION, 
														float2 InTex : TEXCOORD0
#ifdef BONES
														,
														int4 inBoneIndices	: BLENDINDICES,
														float4 inBoneWeights	: BLENDWEIGHT
#endif															
													)
{
	SMapVertexToPixel Output;// = (SMapVertexToPixel)0;
	
	//Output.Position = mul(inPos, matWorldViewProj);
	//Output.Position2D = Output.Position;
	
	//Parallel Projection
{}
#ifdef BONES	
	float4 bonePos 	= DoBones(inPos, inBoneIndices, inBoneWeights);
	Output.Position 	= mul(bonePos, mul(matWorldView, matEffect1));											// matEffect1 = sc_orthoMat
	Output.Position2D = mul(bonePos, matWorldView);
#else
	Output.Position 	= mul(inPos, mul(matWorldView, matEffect1));												// matEffect1 = sc_orthoMat
	Output.Position2D = mul(inPos, matWorldView);
#endif
	
	Output.Tex = InTex;
	
	return Output;    
}

////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shaders

// float DotProduct(float4 LightPos, float3 Pos3D, float3 Normal)
//{
//	float3 LightDir = normalize(LightPos - Pos3D);
//	return dot(LightDir, Normal);
//}
 

SMapPixelToFrame ShadowMapPixelShader(SMapVertexToPixel PSIn)
{
//	clip(tex2D(colorSampler, PSIn.Tex).a-d3d_alpharef_var/255.);				// the same as setting overlay flag

	//-----------
	
	SMapPixelToFrame Output;// = (SMapPixelToFrame)0;

//	Output.Color = 1;	
	Output.Color = 0;
	Output.Color.x = PSIn.Position2D.z / scsm_maxdepth_var;						
	
	Output.Color.w = tex2D(colorSampler, PSIn.Tex).a;
	
	return Output;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Techniques

technique ShadowMap
{
	pass Pass0
		{
//			alphablendenable = true;			// slower, bad shadows cast by alpha textures on bright surfaces
			
			alphatestenable = true;				// faster than alpha blending
			alphablendenable = false;			// false causes square shadows cast by alpha texture
			
			cullmode = none;						// do not cull back faces - better 32b texture shadows
			
			VertexShader = compile vs_3_0 ShadowMapVertexShader();
			PixelShader = compile ps_3_0 ShadowMapPixelShader();
		}
}