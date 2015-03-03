bool AUTORELOAD;

/////////////////////////////////////////////////////////////////////////

#define BONES 													// activate GPU bones (reduces the frame rate by 5%)

#ifdef BONES
	#include <bones>
#endif

#include <pos>

//#define PSSM_DEBUG											// use different coloured shadows for each split: red, green, blue, black

/////////////////////////////////////////////////////////////////////////

float4x4 matViewProj;
float4x4 matView;
float4x4 matTex[4]; 											// set up from the pssm script
float4 vecFog;	

/////////////////////////////////////////////////////////////////////////

float pssm_transparency_var 	= 0.7;

float pssm_limit_var;
float pssm_fadeoutstart_var;	
float pssm_fadeoutend_var;		

float pssm_esm_var				= 7.5;					// expoanential shadowmapping scale factor
float pssm_fogfactor_var		= 5.0;					// fog approximation required by SM3.0

//float d3d_alpharef_var;

/////////////////////////////////////////////////////////////////////////

texture entSkin1;

texture mtlSkin1;
texture mtlSkin2;
texture mtlSkin3;
texture mtlSkin4;

sampler sBaseTex = sampler_state { 
	Texture = <entSkin1>; 
	MipFilter = linear; 
};

sampler sDepth1 = sampler_state {
	texture = <mtlSkin1>;
	MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};

sampler sDepth2 = sampler_state {
	texture = <mtlSkin2>;
	MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};

sampler sDepth3 = sampler_state {
	texture = <mtlSkin3>;
	MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};

sampler sDepth4 = sampler_state {
	texture = <mtlSkin4>;
	MinFilter = LINEAR; MagFilter = LINEAR; MipFilter = LINEAR;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};

/////////////////////////////////////////////////////////////////////
// exponential shadowmapping

float ESM(sampler2D sMap, float4 fShadowTexCoord)
	{
		float fDepth = tex2Dlod(sMap, float4(fShadowTexCoord.xy,0,0)).x;	
		float fDistance = fShadowTexCoord.z / fShadowTexCoord.w;
//		float fShadow = fDistance - fDepth;
//		
//		if ( fShadow > 0)
//	   	fShadow = exp( pssm_esm_var * (fDepth - fDistance) );	
//	   else
//	   	fShadow = 1;
		
		float fShadow = exp( pssm_esm_var * (fDepth - fDistance) );				
		
		return fShadow;
	}

/////////////////////////////////////////////////////////////////////

void renderShadows_VS(
								in float4  inPos				: POSITION,
								in float2  inTex				: TEXCOORD0,
#ifdef BONES
								in int4    inBoneIndices	: BLENDINDICES,
								in float4  inBoneWeights	: BLENDWEIGHT,
#endif	
								out float4 outPos				: POSITION,
								out float4 TexCoord[4]		: TEXCOORD,										// 0..3
								out float2 outTex				: TEXCOORD4,
								out float  fDistance			: TEXCOORD5,
								out float  outFog 			: FOG 
  							)
{
	// calculate world position
{}	
#ifdef BONES
	float4 PosWorld = DoPos(DoBones(inPos,inBoneIndices,inBoneWeights));
#else  
	float4 PosWorld = DoPos(inPos);																			// mul(inPos,matWorld)
#endif  
	
	outPos = mul(PosWorld,matViewProj);
	
	// store view space position
	fDistance = mul(PosWorld, matView).z;
	
	outTex = inTex;
	
	// coordinates for shadow maps
	for(int i=0; i<pssm_limit_var; i++)																		//  *** pssm_numsplits_var
		TexCoord[i] = mul(PosWorld,matTex[i]);
	
	// Fog
	outFog = saturate( 1 - (fDistance - vecFog.x) * (vecFog.z) / pssm_fogfactor_var );		// saturate needed when fadeout distance is closer than clipping distance
   
}

/////////////////////////////////////////////////////////////////////////

float4 renderShadows_PS	(
									float4 TexCoord[4] 		: TEXCOORD,		// 0..3
									float2 inTex				: TEXCOORD4,
									float  fDistance			: TEXCOORD5,
									float  inFog				: FOG
  								) : COLOR
{  
	float2 shifted[4];
	shifted[0] = TexCoord[0].xy*2.0-1.0;
	shifted[1] = TexCoord[1].xy*2.0-1.0;
	shifted[2] = TexCoord[2].xy*2.0-1.0;
	shifted[3] = TexCoord[3].xy*2.0-1.0;
	
	float4 inSplit = float4(dot(shifted[0], shifted[0]), dot(shifted[1], shifted[1]), dot(shifted[2], shifted[2]), dot(shifted[3], shifted[3]));
	inSplit = (inSplit < 1.0);
	
	// to clip away shadows from transparent surfaces	
	float fShadow;
{}
#ifdef PSSM_DEBUG	
	float3 fDebug = 0;																									// shadow color for debugging
#endif
	
	if(inSplit.x > 0.5)
	{
		fShadow = ESM(sDepth1, TexCoord[0]);
		{}
		#ifdef PSSM_DEBUG
			fDebug.r = 0.5;																								// shadow color for debugging
		#endif		
	}
	else if(inSplit.y > 0.5)
	{
		fShadow = ESM(sDepth2, TexCoord[1]);
		{}
		#ifdef PSSM_DEBUG
			fDebug.g = 0.5;																								// shadow color for debugging
		#endif	
	}
	else if(inSplit.z > 0.5)
	{	
		fShadow = ESM(sDepth3, TexCoord[2]);
		{}
		#ifdef PSSM_DEBUG
			fDebug.b = 0.5;																								// shadow color for debugging
		#endif	
	}
	else
	{
		fShadow = ESM(sDepth4, TexCoord[3]);
	}
	
	// FINALIZE
		
	// set shadow darkness
//	float alpha = tex2D(sBaseTex,inTex.xy).a * pssm_transparency_var;
	float alpha = tex2Dlod(sBaseTex, float4(inTex.xy,0.0f,0.0f)).a * pssm_transparency_var;		// ***
	
	// enable 32b skin self shadows - by Slin and oliver2s
	// but clips shadows when pssm_transparency_var is set under d3d_alpharef_var value!
	clip(alpha-10/255.f);	
	
	// min max fade out 
	alpha *= ( 1 - saturate( (fDistance-pssm_fadeoutstart_var) / (pssm_fadeoutend_var-pssm_fadeoutstart_var) ) );
	
	// apply fog approximation
	alpha *= inFog;

#ifdef PSSM_DEBUG		
	return float4(fDebug, clamp(1-fShadow, 0, alpha));															// with debug colours
#else
	return float4(0, 0, 0, clamp(1-fShadow, 0, alpha));
#endif	
}

/////////////////////////////////////////////////////////////////////////

technique renderShadows
{
	pass p0
		{
			ZWriteEnable = True;																							// to get shadows on alpha decals/entities
			AlphaBlendEnable = False;																					// to get shadows on alpha decals/entities
			
			VertexShader = compile vs_3_0 renderShadows_VS();
			PixelShader = compile ps_3_0 renderShadows_PS();						
		}
}

