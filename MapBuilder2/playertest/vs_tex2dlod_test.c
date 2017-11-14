#include <acknex.h>
#include <default.c>

MATERIAL* mat1 = 
{
	effect="
	const float4x4 matWorldViewProj; 
	
	texture entSkin1;
	
	sampler ColorMapSampler = sampler_state 
	{ 
	   Texture = <entSkin1>; 
	}; 
	
	struct VS_OUTPUT
	{
		float4 position : POSITION0;
		float4 color : COLOR0;
	};
	
	VS_OUTPUT DiffuseVS( 
							   in float4 InPos: POSITION, 
							   in float2 InTex: TEXCOORD0
							 )
	{ 
		VS_OUTPUT output;
		output.position=InPos;
	   output.position = mul(output.position, matWorldViewProj); 
	   output.color=tex2Dlod(ColorMapSampler,float4(InTex.xy,0.0f,0.0f));
	   return output;
	} 
	 
	float4 DiffusePS(in float4 col : COLOR0) : COLOR0
	{
		return col;
	}
	
	// Technique: 
	technique DiffuseTechnique 
	{ 
	   pass P0 
	   {
	   	shademode=flat;
	      VertexShader = compile vs_3_0 DiffuseVS(); 
	      PixelShader = compile ps_3_0 DiffusePS();
	   } 
	} 	
	";
}

void main()
{
	error(user_name);
	
	level_load(NULL);
	wait(1);
	
	camera.x=150;
	camera.pan=180;
        //Choose a model you like here =)
	you=ent_create("pguard3_w.mdl",nullvector,NULL);
	you.material=mat1;
}
