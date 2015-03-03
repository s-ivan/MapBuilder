
bool AUTORELOAD;

/////////////////////////////////////////////////////////////////////////////

#define FRESNEL_TRANSPARENCY

/////////////////////////////////////////////////////////////////////////////

#include <transform>
#include <fog>
#include <pos>
#include <normal>
//#include <vecskill>		// use global variables instead of skills

/////////////////////////////////////////////////////////////////////////////

float4 vecTime;
float4 vecSunColor;	
float4 vecColor;	
float fAlbedo;
float fAlpha;

/////////////////////////////////////////////////////////////////////////////

texture entSkin1;
texture mtlSkin1;
texture mtlSkin2;

//	my.skill41 = floatv(10);		// 10 speed x
//	my.skill42 = floatv(25);		// 25 speed	y
//	my.skill43 = floatv(25);		//	25 ripple - waves
//	my.skill44 = floatv(75);		// 100 scale

float mwater_speedx_var						= 10.0;
float mwater_speedy_var						= 25.0;
float mwater_ripple_var						= 50.0;
float mwater_scale_var						= 100.0;

/////////////////////////////////////////////////////////////////////////////

sampler sBumpTex = sampler_state
	{
		Texture = <mtlSkin1>;
		MipFilter = Linear;
		MinFilter = Linear;
		MagFilter = Linear;
		AddressU = Wrap;
		Addressv = Wrap;
	};

sampler sMirrorTex = sampler_state
	{
		Texture = <mtlSkin2>;
		MipFilter = Linear;
		MinFilter = Linear;
		MagFilter = Linear;
		AddressU = Clamp;
		Addressv = Clamp;
	};

/////////////////////////////////////////////////////////////////////////////

struct out_mirror // Output to the pixelshader fragment
	{
		float4 Pos	: POSITION;
		float  Fog	: FOG;
		float4 Color: COLOR0;
		float2 Tex0	: TEXCOORD0;
		float3 Tex1	: TEXCOORD1;
		float3 Albedo: TEXCOORD2;
	};

/////////////////////////////////////////////////////////////////////////////

out_mirror vs_water_mirror
									(
										in float4 inPos		: POSITION,
										in float3 inNormal	: NORMAL,
										in float4 inTex0		: TEXCOORD0
									)
{
	out_mirror Out;
	
	Out.Pos = DoTransform(inPos); 										// transform to screen coordinates
	Out.Fog = DoFog(inPos);

	// bump and reflection coordinates
   float2 Speed;
   Speed.x  = (mwater_speedx_var * 0.000002);
   Speed.y  = (mwater_speedy_var * 0.000002);
   
	Out.Tex0 = (inTex0 + Speed*vecTime.w) * (mwater_scale_var*0.05);
	Out.Tex1 = Out.Pos.xyw;

	// color and transparency
	Out.Albedo.x = 0;
	Out.Albedo.y = 0;
	Out.Albedo.z = (mwater_ripple_var * 0.001);						// ripple
	
	Out.Color    = float4(vecColor.xyz + vecSunColor.xyz, fAlpha);
{}
#ifdef FRESNEL_TRANSPARENCY
	float4 P = DoPos(inPos); 												// vector world position
	float3 vecToView = normalize(vecViewPos-P); 						// direction towards camera
	float3 N = DoNormal(inNormal); 										// normal world orientation
	Out.Color.a = 0.67 + fAlpha * (1.0 - dot(vecToView,N));
#endif
		
	return Out;
}
	
/////////////////////////////////////////////////////////////////////////////

float4 ps_water_mirror(out_mirror In): COLOR
	{

		float4 Bump  = tex2D(sBumpTex,In.Tex0)*2-1;
		
		float2 Coord = 0.5 * (1.0 + In.Tex1.xy/In.Tex1.z) + Bump.xy * In.Albedo.z;
		
		float4 ColorMod 	= In.Color;
		ColorMod.a 			= 1;
		
		return tex2D(sMirrorTex,Coord) * In.Color * ColorMod; // ***
	}

//////////////////////////////////////////////////////////////////

technique water_mirror
	{
		pass one
		{
			AlphaBlendEnable 	= True;
		   
			VertexShader	= compile vs_2_0 vs_water_mirror();
			PixelShader		= compile ps_2_0 ps_water_mirror();			
		}		
	}

technique fallback { pass one { } }
