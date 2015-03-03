////////////////////////////////////////////////////
// Poisson blur stencil shader
// Copyright (c) 2010 oP group
////////////////////////////////////////////////////
#include <poisson>

//float4 vecSkill1; // vecSkill1.x = blur factor		//¤¤¤ stencil shadow errors
float stencil_blur_value_var = 0;							//¤¤¤ works fine

texture StencilMap; // black/white stencil buffer image
sampler smpStencil = sampler_state { texture = <StencilMap>; }; 

float4 blurPoissonPS( float2 Tex : TEXCOORD0) : COLOR0 
{
//   return DoPoisson(smpStencil,Tex,vecSkill1.x);
   return DoPoisson(smpStencil, Tex, stencil_blur_value_var);
}

technique blur
{ 
   pass { PixelShader = compile ps_2_0 blurPoissonPS(); }
} 

technique fallback { pass { } }
