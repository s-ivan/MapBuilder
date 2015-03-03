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

float log_conv(float w0, float d1, float w1, float d2){
	return (d1 + log(w0 + (w1 * exp(d2 - d1))));
}

//--------------------------------------------------------------------------

float4 blurDepth(float2 Tex : TEXCOORD0):COLOR0
{
	float w = 0.5;
	float B = tex2D(depthSampler, Tex).x;
	float v = log_conv(w, B, w, B);
	return float4(v,0,0,0);
	
//	return tex2D(depthSampler,Tex);						// test - do nothing
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

