
bool AUTORELOAD;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

////////////////////////////////////////////////////////////////////////////////////////////////
// matrices, vectors, variables passed by the engine

float4x4 matWorldViewProj;
float4x4 matWorld;
float4x4 matTexture;

//float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecEmissive;

////////////////////////////////////////////////////////////////////////////////////////////////
// skins/textures

texture entSkin1; 							// Colormap

// Color map sampler
sampler colorSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = WRAP;
	AddressV = WRAP;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// includes

#include "sc_defines_MB.fx"

////////////////////////////////////////////////////////////////////////////////////////////////
// custom matrices, vectors, variables

//////////////////////////////////////////////////////////////////////////////////////////////////
// struct VS

struct outSkyVS
{
	float4 Pos				: POSITION; 
	float4 Color			: COLOR0;    
	float2 Tex				: TEXCOORD0; 
#ifdef SKY_LIGHT	
	float3 WorldPos		: TEXCOORD1;
#endif
};

struct inSkyPS
{
	float4 Color			: COLOR0;    
	float2 Tex				: TEXCOORD0; 
#ifdef SKY_LIGHT	
	float3 WorldPos		: TEXCOORD1;
#endif	
};

////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shaders

outSkyVS SkyVS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0
	) 
{ 
	outSkyVS Out;
	
//	InPos.w = 1.0f;																	
	
	//----------------------------------------------------------------
	
	Out.Tex = mul(float3(InTex.x,InTex.y,1), matTexture).xy;						// get uv from texture transformation matrix
		
	//----------------------------------------------------------------
	
	Out.Pos = mul(InPos, matWorldViewProj); 						
	
	//----------------------------------------------------------------
	// lighting		
	
	Out.Color = (vecAmbient + vecEmissive) + vecColor;								// ambient + emissive + rgb			
	
   //-----------------------------------------------------------------
{}
#ifdef SKY_LIGHT	
	Out.WorldPos = mul(InPos, matWorld).xyz;
#endif	
   //-----------------------------------------------------------------
   
   return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

float4 SkyPS(inSkyPS In) : COLOR
{
	//----------------------------------------------------------
	// texture
	
	float4 Color = tex2D(colorSampler, In.Tex);	
	Color.a = 1;
	
	//----------------------------------------------------------
	
	Color.rgb *= In.Color.rgb;
	
	//----------------------------------------------------------
	// sky light
{}
#ifdef SKY_LIGHT	
	// density
//	float SkyLight = 1.0 - saturate( (In.WorldPos.y + 1000) / (5000 * sky_skylightheight_var) );
	// offset
	float SkyLight = 1.0 - saturate( (In.WorldPos.y + 2500 - 5000 * sky_skylightheight_var) / 5000 );
	
	Color.rgb = lerp(Color.rgb, sky_skylightcolor_var, SkyLight);
#endif
		
	//----------------------------------------------------------
	
	return Color;
}

////////////////////////////////////////////////////////////////////////////////////////////////

technique skysprite
{ 

   pass p0
   {
		VertexShader = compile vs_3_0 SkyVS();
		PixelShader  = compile ps_3_0 SkyPS();	
   }

}

//////////////////////////////////////////

technique fallback 
{ 
	pass 
	{ 	
		
	} 
}