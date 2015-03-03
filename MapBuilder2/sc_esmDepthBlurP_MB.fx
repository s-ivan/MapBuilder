bool AUTORELOAD;

//--------------------------------------------------------------------------

texture TargetMap;

// blurring distance
float scsm_blursize_flt = 0.00195;
//// or replace (scsm_blursize_flt) with (vecViewPort.zw * scsm_blurpixel_var)
//float scsm_blurpixel_var = 1.25;
//float4 vecViewPort;

//--------------------------------------------------------------------------

sampler depthSampler = sampler_state 
{
	texture 		= <TargetMap>;
	magfilter 	= LINEAR;
	minfilter 	= LINEAR;
	mipfilter	= LINEAR;
};

//--------------------------------------------------------------------------

#define NUM_TAPS 12

static const float2 fTaps_Poisson[NUM_TAPS] = 
{
	{-.326,-.406},
	{-.840,-.074},
	{-.696, .457},
	{-.203, .621},
	{ .962,-.195},
	{ .473,-.480},
	{ .519, .767},
	{ .185,-.893},
	{ .507, .064},
	{ .896, .412},
	{-.322,-.933},
	{-.792,-.598}
};

//--------------------------------------------------------------------------

float log_conv(float w0, float d1, float w1, float d2){
	return (d1 + log(w0 + (w1 * exp(d2 - d1))));
}

//--------------------------------------------------------------------------

float4 PoissonPS (float2 texCoord: TEXCOORD0) : COLOR0
{
	float v, B, B2;
	float w = (1.0/NUM_TAPS);
	
	B = tex2D(depthSampler, texCoord + fTaps_Poisson[0] * scsm_blursize_flt).x;
	B2 = tex2D(depthSampler, texCoord + fTaps_Poisson[1] * scsm_blursize_flt).x;
	v = log_conv(w, B, w, B2);
	
	for(int i = 2; i < NUM_TAPS; i++)
	{
		B = tex2D(depthSampler, texCoord + fTaps_Poisson[i] * scsm_blursize_flt).x;
		v = log_conv(1.0, v, w, B);
	}
	
	return float4(v,0,0,0);
	
//	return tex2D(depthSampler,Tex);		// test - do nothing	
}

//--------------------------------------------------------------------------

technique techPoisson
{
	pass p0
	{
		PixelShader = compile ps_3_0 PoissonPS();
	}
}

//--------------------------------------------------------------------------
