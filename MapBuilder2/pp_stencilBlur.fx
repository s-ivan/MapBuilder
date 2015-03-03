texture TargetMap;
texture StencilMap;

sampler smpSource = sampler_state { texture = <TargetMap>; };
sampler smpStencil = sampler_state { texture = <StencilMap>; };

float4 vecViewPort; // contains viewport pixel size in zw components

float4 blurPS( float2 Tex : TEXCOORD0) : COLOR0 
{
	float4 Color = tex2D(smpSource,Tex.xy); // sample previous render target
	float4 Stencil = 0;
	const float fDist = 2.5;
	for (float i=-fDist; i<=fDist; i+=fDist) // average stencil shadow from 9 samples
	for (float j=-fDist; j<=fDist; j+=fDist)
	Stencil += tex2D(smpStencil,Tex.xy+float2(i*vecViewPort.z,j*vecViewPort.w));
	Color.rgb -= 0.15*Stencil.a; // blend stencil alpha over previous render target
	return Color;
}

technique blur
{ 
	pass one
	{ 
		PixelShader = compile ps_2_0 blurPS(); 
	}
} 

technique fallback { pass one { } }