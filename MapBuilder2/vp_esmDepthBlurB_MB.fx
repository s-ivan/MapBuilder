bool AUTORELOAD;

//--------------------------------------------------------------------------

texture 	TargetMap;

//--------------------------------------------------------------------------

// blurring distance
float pssm_blur_flt = 0.00195;

//// or replace (scsm_blursize_flt) with (vecViewPort.zw * scsm_blurpixel_var)
//float scsm_blurpixel_var = 1.25;
//float4 vecViewPort;

//// or
//float4 	vecSkill1;

//--------------------------------------------------------------------------

sampler depthSampler = sampler_state
{
	texture 		= <TargetMap>;
};

//--------------------------------------------------------------------------

#define SAMPLE_COUNT 5
float2 Offsets[SAMPLE_COUNT] = 
{
	{0,0},
	{1,0},
	{0,1},
	{-1,0},
	{0,-1}
};

//--------------------------------------------------------------------------

float log_conv(float w0, float d1, float w1, float d2){
	return (d1 + log(w0 + (w1 * exp(d2 - d1))));
}

//--------------------------------------------------------------------------

float4 blurDepth(float2 Tex : TEXCOORD0):COLOR0
{
	float v, B, B2;
	float w = (1.0/SAMPLE_COUNT);
	
	B = tex2D(depthSampler, Tex + Offsets[0] * pssm_blur_flt).x;
	B2 = tex2D(depthSampler, Tex + Offsets[1] * pssm_blur_flt).x;
	v = log_conv(w, B, w, B2);
	
	for(int i = 2; i < SAMPLE_COUNT; i++)
	{
		B = tex2D(depthSampler, Tex + Offsets[i] * pssm_blur_flt).x;
		v = log_conv(1.0, v, w, B);
	}
	
	return float4(v,0,0,0);
}

//--------------------------------------------------------------------------

technique tBlurDepth
{
	pass p0
	{
		PixelShader = compile ps_3_0 blurDepth();		
	}
}

//--------------------------------------------------------------------------

