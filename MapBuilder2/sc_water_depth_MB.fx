
bool AUTORELOAD;

////////////////////////////////////////////////////////////////

matrix matWorldViewProj;
matrix matWorld;
float4 vecSkill5; 			// x waterplane height , y max waterdepth

////////////////////////////////////////////////////////////////

texture entSkin1;

sampler2D ColorSampler = sampler_state
{
	Texture = <entSkin1>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = WRAP;
	AddressV = WRAP;
};

////////////////////////////////////////////////////////////////

struct SMapVertexToPixel
{
	float4 Position   : POSITION;    
	float2 Tex 			: TEXCOORD1;
	float  PositionY 	: TEXCOORD2;
};

////////////////////////////////////////////////////////////////

SMapVertexToPixel depthmapVS( 
										float4 inPos : POSITION, 
										float2 inTex : TEXCOORD0
									 )
{
	SMapVertexToPixel Output = (SMapVertexToPixel)0;
	
	Output.Position = mul(inPos, matWorldViewProj);
	
	Output.Tex = inTex;
	
	// waterdepth
	float4 temp = mul(inPos,matWorld);
	Output.PositionY = temp.y;
	
	// - entity height
	Output.PositionY -= vecSkill5.x;
	//
	
	return Output;    
}

////////////////////////////////////////////////////////////////

float4 depthmapPS(SMapVertexToPixel PSIn) : COLOR0
{
	//waterdepth
	float pixelYDepth	= saturate(PSIn.PositionY / vecSkill5.y + 1);			// range -1..1, then 0..2, then cut to keep only 0..1 i.e. underwater maxdepth is 0, 0 is 1
	
	return float4(0, pixelYDepth, 0, 0);
}

////////////////////////////////////////////////////////////////

technique Depthmap
{
	pass Pass0
		{	
			VertexShader = compile vs_3_0 depthmapVS();
			PixelShader  = compile ps_3_0 depthmapPS();
		}
}
