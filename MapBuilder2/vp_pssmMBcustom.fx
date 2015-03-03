bool AUTORELOAD;

/////////////////////////////////////////////////////////////////////////

#define BONES 										// activate GPU bones (reduces the frame rate by 5%)

#ifdef BONES
	#include <bones>
#endif

#include <pos>

//#define PSSM_DEBUG								// use different coloured shadows for each split: red, green, blue, black

/////////////////////////////////////////////////////////////////////////

float4x4 matViewProj;
float4x4 matView;
float4x4 matTex[4]; 															// set up from the pssm script
float4 vecFog;	

#define PCFSAMPLES_NEAR	7													// 5 -> 7 higher values for smoother shadows (slower) 
#define PCFSAMPLES_MID	5													// *** 3 -> 5
#define PCFSAMPLES_FAR	3													// 3

//#define FAST																	// a bit ugly look, but faster - sharper faster - too many errors on terrain slopes ***
//#define OWM_BIAS															// use different pcf and bias for each entity - slow pssm initialization ***
																					// results in no shadows on entities cast by other entities !!! except terrain

/////////////////////////////////////////////////////////////////////////

float pssm_fbias_flt 			= 0.0005f;								
float pssm_res_var 				= 1024;
float pssm_transparency_var 	= 0.7;

float pssm_limit_var;
float pssm_fadeoutstart_var;	
float pssm_fadeoutend_var;		

float pssm_shadowweight_var;

float pssm_fogfactor_var		= 5.0;									// fog approximation required by SM3.0

//float d3d_alpharef_var;

/////////////////////////////////////////////////////////////////////////

#ifdef OWM_BIAS
	float4  vecSkill57;														// ***	w : entity's own bias offset ; x,y,z : entity's own PSFSAMPLES_NEAR/MID/FAR
#endif

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
	MinFilter = point; MagFilter = point; MipFilter = none;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};

sampler sDepth2 = sampler_state {
	texture = <mtlSkin2>;
	MinFilter = point; MagFilter = point; MipFilter = none;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};

sampler sDepth3 = sampler_state {
	texture = <mtlSkin3>;
	MinFilter = point; MagFilter = point; MipFilter = none;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};

sampler sDepth4 = sampler_state {
	texture = <mtlSkin4>;
	MinFilter = point; MagFilter = point; MipFilter = none;
	AddressU = Border; AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};

/////////////////////////////////////////////////////////////////////////

// PCF soft-shadowing
float DoPCF(sampler2D sMap,float4 vShadowTexCoord,int iSqrtSamples,float fBias)
{
	float fShadowTerm = 0.0f;  
	float fRadius = (iSqrtSamples - 1.0f) / 2;

	for (float y = -fRadius; y <= fRadius; y++)
	{
		for (float x = -fRadius; x <= fRadius; x++)
		{
			float2 vOffset = float2(x,y)/pssm_res_var;
//			float fDepth = tex2D(sMap, vShadowTexCoord.xy + vOffset).x;
			float fDepth = tex2Dlod(sMap, float4(vShadowTexCoord.xy + vOffset, 0.0f, 0.0f)).x;			// ***
			float fSample = (vShadowTexCoord.z < fDepth + fBias);

			// Edge tap smoothing 	
			float xWeight = 1, yWeight = 1;
			
			if (x == -fRadius) 
				xWeight = 1 - frac(vShadowTexCoord.x * pssm_res_var);
			else if (x == fRadius)
				xWeight = frac(vShadowTexCoord.x * pssm_res_var);
			
	
			if (y == -fRadius)
				yWeight = 1 - frac(vShadowTexCoord.y * pssm_res_var);
			else if (y == fRadius)
				yWeight = frac(vShadowTexCoord.y * pssm_res_var);
	
			fShadowTerm += fSample * xWeight * yWeight;
		}											
	}		
	
	return fShadowTerm / (iSqrtSamples * iSqrtSamples);
}

/////////////////////////////////////////////////////////////////////////

// Calculates the shadow occlusion using bilinear PCF
float DoFastPCF(sampler2D sMap,float4 vShadowTexCoord,float fBias)
 //float fLightDepth, float2 vTexCoord)
{
    float fShadowTerm = 0.0f;

    // transform to texel space
    float2 vShadowMapCoord = pssm_res_var * vShadowTexCoord.xy;
    
    // Determine the lerp amounts           
    float2 vLerps = frac(vShadowMapCoord);

    // read in bilerp stamp, doing the shadow checks
    float fSamples[4];
    
    fSamples[0] = (tex2Dlod(sMap,float4(vShadowTexCoord.xy,0,0)).x + fBias < vShadowTexCoord.z) ? 0.0f: 1.0f;  
    fSamples[1] = (tex2Dlod(sMap,float4(vShadowTexCoord.xy + float2(1.0/pssm_res_var,0),0,0)).x + fBias < vShadowTexCoord.z) ? 0.0f: 1.0f;  
    fSamples[2] = (tex2Dlod(sMap,float4(vShadowTexCoord.xy + float2(0,1.0/pssm_res_var),0,0)).x + fBias < vShadowTexCoord.z) ? 0.0f: 1.0f;  
    fSamples[3] = (tex2Dlod(sMap,float4(vShadowTexCoord.xy + float2(1.0/pssm_res_var,1.0/pssm_res_var),0,0)).x + fBias < vShadowTexCoord.z) ? 0.0f: 1.0f;  
    
    // lerp between the shadow values to calculate our light amount
    fShadowTerm = lerp( lerp( fSamples[0], fSamples[1], vLerps.x ),
                              lerp( fSamples[2], fSamples[3], vLerps.x ),
                              vLerps.y );                              
                                
    return fShadowTerm;                                
}

/////////////////////////////////////////////////////////////////////////

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
									float4 TexCoord[4] 		: TEXCOORD,										// 0..3
									float2 inTex				: TEXCOORD4,
									float  fDistance			: TEXCOORD5,
									float  inFog				: FOG
  								) : COLOR
{  
  // to clip away shadows from transparent surfaces	
  float fShadow;
{}
#ifdef PSSM_DEBUG	
	float3 fDebug = 0;												// shadow color for debugging
#endif
	  
	float2 shifted[4];
	shifted[0] = TexCoord[0].xy*2.0-1.0;
	shifted[1] = TexCoord[1].xy*2.0-1.0;
	shifted[2] = TexCoord[2].xy*2.0-1.0;
	shifted[3] = TexCoord[3].xy*2.0-1.0;
	
	float4 inSplit = float4(dot(shifted[0], shifted[0]), dot(shifted[1], shifted[1]), dot(shifted[2], shifted[2]), dot(shifted[3], shifted[3]));
	inSplit = (inSplit < 1.0);
	
{}  
#ifdef FAST
	if(inSplit.x > 0.5)
		fShadow = DoFastPCF(sDepth1,TexCoord[0],pssm_fbias_flt);
	else if(inSplit.y > 0.5)
		fShadow = DoFastPCF(sDepth2,TexCoord[1],2*pssm_fbias_flt);
	else if(inSplit.z > 0.5)
		fShadow = DoFastPCF(sDepth3,TexCoord[2],4*pssm_fbias_flt);
	else
		fShadow = DoFastPCF(sDepth4,TexCoord[3],8*pssm_fbias_flt);
#else

	#ifdef OWM_BIAS																						// use entity specific pcf and bias values - slow initialization
		if		(inSplit.x > 0.5)							
			fShadow = DoPCF(sDepth1, TexCoord[0], vecSkill57.x, (pssm_fbias_flt));						
		else if(inSplit.y > 0.5)
			fShadow = DoPCF(sDepth2, TexCoord[1], vecSkill57.y, (pssm_fbias_flt + vecSkill57.w));		
		else if(inSplit.z > 0.5)
			fShadow = DoPCF(sDepth3, TexCoord[2], vecSkill57.y, (pssm_fbias_flt + vecSkill57.w));		
		else
			fShadow = DoPCF(sDepth4, TexCoord[3], vecSkill57.z, (pssm_fbias_flt + vecSkill57.w));		
	#else 	 
		if		(inSplit.x > 0.5)
			{
				fShadow = DoPCF(sDepth1, TexCoord[0], PCFSAMPLES_NEAR, 1*pssm_fbias_flt);		// PCFSAMPLES_NEAR 1*pssm_fbias_flt
				{}
				#ifdef PSSM_DEBUG					
					fDebug.r = 0.5;																					// shadow color for debugging
				#endif				
			}
		else if(inSplit.y > 0.5)
			{
				fShadow = DoPCF(sDepth2, TexCoord[1], PCFSAMPLES_MID, 2*pssm_fbias_flt);		// PCFSAMPLES_FAR 2*
				{}
				#ifdef PSSM_DEBUG					
					fDebug.g = 0.5;																					// shadow color for debugging
				#endif				
			}
		else if(inSplit.z > 0.5)
			{
				fShadow = DoPCF(sDepth3, TexCoord[2], PCFSAMPLES_FAR, 4*pssm_fbias_flt);		// PCFSAMPLES_FAR 4*
				{}
				#ifdef PSSM_DEBUG					
					fDebug.b = 0.5;																					// shadow color for debugging
				#endif
			}
		else
			{
				fShadow = DoPCF(sDepth4, TexCoord[3], PCFSAMPLES_FAR, 8*pssm_fbias_flt);		// PCFSAMPLES_FAR 8*
			}
	#endif
 	  
#endif

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
	return float4(fDebug, clamp(1-pssm_shadowweight_var*fShadow, 0, alpha));		// with debug colours
#else
	return float4(0, 0, 0, clamp(1-pssm_shadowweight_var*fShadow, 0, alpha));		// *** was 2.5, max 5.5, experimental value
#endif	
	
}

/////////////////////////////////////////////////////////////////////////

technique renderShadows
{
	pass p0
		{
			ZWriteEnable = True;
			AlphaBlendEnable = False;
			
			VertexShader = compile vs_3_0 renderShadows_VS();
			PixelShader = compile ps_3_0 renderShadows_PS();
		}
}

