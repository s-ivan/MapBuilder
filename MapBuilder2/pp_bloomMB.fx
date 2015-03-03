bool AUTORELOAD;

///////////////////////////

float4 vecViewPort;
float4 vecSkill1;

///////////////////////////

Texture TargetMap;

sampler2D g_samSrcColor = sampler_state { texture = <TargetMap>; MipFilter = Linear;	};

///////////////////////////

float4 postprocessing_bloomblur( float2 Tex : TEXCOORD0 ) : COLOR0 
{
	float div = 1;
	
   float4 Color = tex2D( g_samSrcColor, Tex.xy);
   
	float4 colorrgba = Color;   
   float colorsum   = Color.r + Color.g + Color.b;
	
	//--------------------------------------------------------
	// bloom
	
	// brighten pixel if over lower limit
//	float4 ColorMod = Color * max(colorsum, vecSkill1.y) / 3 * vecSkill1.z;	
	float4 ColorMod = Color * max(colorrgba, vecSkill1.y) / 3 * vecSkill1.z;	
	
	// add overall brightness
	Color *= vecSkill1.w;
	
	return (Color + ColorMod);
}

///////////////////////////

technique PostProcess 
{
	pass p1 
	{
//		AlphaBlendEnable = false;
		VertexShader = null;
		
		PixelShader = compile ps_3_0 postprocessing_bloomblur();
	}
}