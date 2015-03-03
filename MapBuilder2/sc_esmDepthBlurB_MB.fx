bool AUTORELOAD;

//--------------------------------------------------------------------------

texture 	TargetMap;
float scsm_blursize_flt = 0.00195;

//--------------------------------------------------------------------------

sampler depthSampler = sampler_state
{
	texture 		= <TargetMap>;
	magfilter 	= LINEAR;
	minfilter 	= LINEAR;
	mipfilter	= LINEAR;
};

//--------------------------------------------------------------------------

//#define SAMPLE_COUNT 3
//float2 Offsets[SAMPLE_COUNT] = 
//{
//	{1,0},
//	{0,1},
//	{1,1}
//};

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
	
	B = tex2D(depthSampler, Tex + Offsets[0] * scsm_blursize_flt).x;
	B2 = tex2D(depthSampler, Tex + Offsets[1] * scsm_blursize_flt).x;
	v = log_conv(w, B, w, B2);
	
	for(int i = 2; i < SAMPLE_COUNT; i++)
	{
		B = tex2D(depthSampler, Tex + Offsets[i] * scsm_blursize_flt).x;
		v = log_conv(1.0, v, w, B);
	}
	
	return float4(v,0,0,0);
	
//	return tex2D(depthSampler,Tex);		// test - do nothing
}

//--------------------------------------------------------------------------

technique tBlurDepth
{
	pass p0
	{
//		texture[0] = <TargetMap>;							// test
		PixelShader = compile ps_3_0 blurDepth();		
	}
}

//--------------------------------------------------------------------------

