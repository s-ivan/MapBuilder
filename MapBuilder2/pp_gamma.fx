//////////////////////////////////////////////////
// Exposure compensation shader
//////////////////////////////////////////////////
//bool AUTORELOAD;

float4 vecTime;
float4 vecSkill1;
float4 vecViewPort;
texture mtlSkin1,mtlSkin2,TargetMap; 

sampler scene = sampler_state { Texture = <mtlSkin1>; };
sampler gamma = sampler_state { Texture = <mtlSkin2>; };
sampler source = sampler_state { Texture = <TargetMap>; };

static const float3 vecLuminance = { 0.3,0.4,0.3 };
static const float blurFactor16 = 1.5/16;
static const float blurFactor9 = 1.5/9;

static const float ds = 0.5/4;
static const float ds3 = ds/3;

static const float2 Offsets16[16] = { 
	{-ds,-ds},{-ds3,-ds},{ds3,-ds},{ds,-ds}, 
	{-ds,-ds3},{-ds3,-ds3},{ds3,-ds3},{ds,-ds3}, 
	{-ds, ds3},{-ds3, ds3},{ds3, ds3},{ds, ds3}, 
	{-ds, ds},{-ds3, ds},{ds3, ds},{ds, ds} 
};

static const float2 Offsets9[9] = { 
	{-ds,-ds},{0,-ds},{ds,-ds}, 
	{-ds,  0},{0,  0},{ds,  0}, 
	{-ds, ds},{0, ds},{ds, ds} 
};

float4 gammaStartPS(float2 Tex: TEXCOORD0): COLOR0 
{
	float luminance = 0.;
	for(int i=0; i<9; i++) {
		float4 Color = tex2D(scene,vecSkill1.xy*Tex.xy+Offsets9[i]);
		luminance += dot(Color,vecLuminance); 
	}
	float luminance1 = tex2D(gamma,float2(0.5,0.5)).b;		// g = delayed luminance
	return float4(luminance*blurFactor9,0,luminance1,1);
}

float4 gammaSamplePS(float2 Tex: TEXCOORD0): COLOR0 
{
	float4 Color = 0;
	for(int i=0; i<16; i++)
		Color += tex2D(source,4*Tex.xy+Offsets16[i]); //*vecViewPort.zw));
	return float4(Color.r*blurFactor16,Color.gb/16,1);
}

float4 gammaEndPS( float2 Tex: TEXCOORD0 ): COLOR0 
{
// add all pixels from the source map	
	float4 Color = 0;
	for(int i=0; i<16; i++)
		Color += tex2D(source,float2(0.5,0.5)+Offsets16[i]);
// exponential averaging
	float a = min(0.1,0.0001*vecSkill1.w/vecTime.x);	
	Color.b = a*Color.r + (1-a)*Color.b;
	return float4(Color.rgb/16,1);
}

technique gammaStart { pass { PixelShader = compile ps_2_0 gammaStartPS(); } }
technique gammaSample {	pass { PixelShader = compile ps_2_0 gammaSamplePS(); } }
technique gammaEnd { pass { PixelShader = compile ps_2_0 gammaEndPS(); } }

technique fallback { pass { } }
