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
   float4 Color = tex2D( g_samSrcColor, Tex.xy);
   
//	return Color; // test for orig
   
   float4 colorrgba = Color; 
   float colorsum  = Color.r + Color.g + Color.b;
   
   //--------------------------------------------------------
   // blur - the brighter the more blur
	
	//																		* pixel size		* blur distance	* brightness factor
	Color += tex2D( g_samSrcColor, Tex.xy + float2( 0, 1) * vecViewPort.zw 	* vecSkill1.x 		* (colorsum / 3));
	Color += tex2D( g_samSrcColor, Tex.xy + float2( 0,-1) * vecViewPort.zw 	* vecSkill1.x 		* (colorsum / 3));
	Color += tex2D( g_samSrcColor, Tex.xy + float2( 1, 0) * vecViewPort.zw 	* vecSkill1.x 		* (colorsum / 3));
	Color += tex2D( g_samSrcColor, Tex.xy + float2(-1, 0) * vecViewPort.zw 	* vecSkill1.x 		* (colorsum / 3));
	
	// /5 to keep brightness
	Color *= 0.2;	
	
	//--------------------------------------------------------
	// bloom
	
	// brighten pixel if over lower limit
//	float4 ColorMod = Color 	* max(colorsum, vecSkill1.y) / 3 	* vecSkill1.z;	
	float4 ColorMod = Color 	* max(colorrgba, vecSkill1.y) / 3 	* vecSkill1.z;	
	
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