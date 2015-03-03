//////////////////////////////////////////////////
// HDR emulation by bloom effect
//////////////////////////////////////////////////
//bool AUTORELOAD;
#define EXPOSURE_COMPENSATION

#include <poisson>

texture TargetMap,mtlSkin1,mtlSkin2; 
float4 vecSkill1;

sampler scene = sampler_state { Texture = <mtlSkin1>; AddressU = Border; AddressV = Border;	};
sampler gamma = sampler_state { Texture = <mtlSkin2>; AddressU = Border; AddressV = Border;	};
sampler source = sampler_state { Texture = <TargetMap>; AddressU = Border; AddressV = Border;	};

static const float3 vecLuminance = { 0.3,0.4,0.3 };
static const float blurFactor = 1.5;
static const float blurWeights[13] =
{
	0.002216*blurFactor,
	0.008764*blurFactor,
	0.026995*blurFactor,
	0.064759*blurFactor,
	0.120985*blurFactor,
	0.176033*blurFactor,
	0.199471*blurFactor,
	0.176033*blurFactor,
	0.120985*blurFactor,
	0.064759*blurFactor,
	0.026995*blurFactor,
	0.008764*blurFactor,
	0.002216*blurFactor
};

//////////////////////////////////////////////////////

static const float width1 = 5, width2 = 6, width3 = 10; // filter widths

// poisson filter, increase bright parts, remove dark parts
float4 filterStartPS(float2 Tex: TEXCOORD0): COLOR0 
{
   float4 Color = DoPoisson(source,Tex*4,width1);	
// enhance bright parts -> multiply by luminance  
   Color.rgb *= dot(Color,vecLuminance);
// clip away below threshold   
   float threshold = vecSkill1.y*0.5/50;
   float factor = vecSkill1.x*2./50;
   Color.rgb = factor*max(Color-threshold,0.);
   return Color; 
}

float4 filterEndPS(float2 Tex: TEXCOORD0): COLOR0
{

	float4 Color = tex2D(scene,Tex/4);
	Color += DoPoisson(source,Tex/4,width3); // add bloom

#ifdef EXPOSURE_COMPENSATION
	float luminance = tex2D(gamma,float2(0.5,0.5)).b;
	Color.rgb *= 1 - min(0.6,vecSkill1.z*0.02*luminance); 
#endif	
	return Color;	
}

// horizontal and vertical filtering
float4 filterHPS(float2 Tex: TEXCOORD0): COLOR0
{
	float4 Color = 0.;
	float filterStep = width2*vecViewPort.w;
	for(int i = -6; i <= 6; i++)
		Color += tex2D(source,Tex+float2(i*filterStep,0.))*blurWeights[i+6];
	return Color;
}

float4 filterVPS(float2 Tex: TEXCOORD0): COLOR0
{
	float4 Color = 0.;
	float filterStep = width2*vecViewPort.w;
	for(int i = -6; i <= 6; i++)
		Color += tex2D(source,Tex+float2(0,i*filterStep))*blurWeights[i+6];
	return Color;
}

technique filterStart { pass { PixelShader = compile ps_2_0 filterStartPS(); } }
technique filterEnd { pass { PixelShader = compile ps_2_0 filterEndPS(); } }
technique filterH { pass { PixelShader = compile ps_2_0 filterHPS(); } }
technique filterV { pass { PixelShader = compile ps_2_0 filterVPS(); } }
