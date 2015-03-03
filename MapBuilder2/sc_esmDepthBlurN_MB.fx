
bool AUTORELOAD;

//--------------------------------------------------------------------------

texture 	TargetMap;
//float scsm_blursize_flt = 0.00195;

//--------------------------------------------------------------------------

sampler depthSampler = sampler_state
{
	texture 		= <TargetMap>;
	magfilter 	= LINEAR;
	minfilter 	= LINEAR;
	mipfilter	= LINEAR;
};

//--------------------------------------------------------------------------

//float log_conv(float w0, float d1, float w1, float d2){
//	return (d1 + log(w0 + (w1 * exp(d2 - d1))));
//}

//--------------------------------------------------------------------------

float4 blurDepth(float2 Tex : TEXCOORD0):COLOR0
{	
	return tex2D(depthSampler,Tex);						// test - do nothing
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

