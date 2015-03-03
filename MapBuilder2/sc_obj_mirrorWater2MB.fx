
bool AUTORELOAD;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//#define FRESNEL_TRANSPARENCY

#define SUNLIGHT

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4x4 matWorldViewProj;
float4x4 matWorld;

float3 vecViewPos;

float4 vecSunPos;
float4 vecSunColor;
float3 vecSunDir;

float4 vecFog;				
float4 vecFogColor;	

float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;
float  fPower;

float fAlpha;

float4 vecTime;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes

#include "sc_defines_MB.fx"
#include "sc_pointlight_MB.fx"

#ifdef SHADOWMAPPING
	#include "sc_shadowmapping_MB.fx"	// scsm shadowmapping - variables, shadow samplers, vertex shader functions, pixel shader function
#endif

#ifdef HEIGHT_FOG
	#include "sc_heightfog_MB.fx"			// blend distance fog with height fog
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

texture entSkin1;
texture mtlSkin1;
texture mtlSkin2;

//	my.skill41 = floatv(10);		// 10 speed x
//	my.skill42 = floatv(25);		// 25 speed	y
//	my.skill43 = floatv(25);		//	25 ripple - waves
//	my.skill44 = floatv(75);		// 100 scale

float mwater_speedx_var				= 10.0;
float mwater_speedy_var				= 25.0;
float mwater_ripple_var				= 50.0;
float mwater_scale_var				= 100.0;

float material_mwater_scale2		= 0.5;
float material_mwater_speed2		= 2.0;

float material_mwater_sunlightarea 		= 250.0;
float material_mwater_sunlightstrength	= 1.0;

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

struct VS_out_mirror
{
	float4 Pos			: POSITION;
	float  Fog			: FOG;
	float4 Color		: COLOR0;
	float4 Tex01		: TEXCOORD0;
	float3 TexM			: TEXCOORD1;
	float  Ripple		: TEXCOORD2;
	float4 WorldPos	: TEXCOORD4;
#ifdef SKY_LIGHT
	float  SkyLight	: TEXCOORD7;
#endif		
};

struct PS_in_mirror
{
	float  Fog			: FOG;
	float4 Color		: COLOR0;
	float4 Tex01		: TEXCOORD0;
	float3 TexM			: TEXCOORD1;
	float  Ripple		: TEXCOORD2;
	float4 WorldPos	: TEXCOORD4;
#ifdef SKY_LIGHT
	float  SkyLight	: TEXCOORD7;
#endif				
};

/////////////////////////////////////////////////////////////////////////////

VS_out_mirror vs_water_mirror(
										in float4 inPos		: POSITION,
										in float3 inNormal	: NORMAL,
										in float4 inTex		: TEXCOORD0
									)
{
	VS_out_mirror Out;
	
	//----------------------------------------------------------------
	
	Out.Pos = mul(inPos, matWorldViewProj);
	
	//----------------------------------------------------------------
	
	//Fog
	float4 PosWorld 	= mul(inPos, matWorld);		
{}
#ifndef HEIGHT_FOG   
	Out.Fog = vecFogColor.w																								// fog is applied
				* saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );					// saturate needed when fadeout distance is closer than clipping distance
#else
	Out.Fog = VS_heightfog_Water(PosWorld.xyz);
#endif
	
	//----------------------------------------------------------------
	
	Out.WorldPos = PosWorld;
	
	//----------------------------------------------------------------
	
#ifdef SKY_LIGHT
	Out.SkyLight = saturate( (distance(PosWorld, vecViewPos) - sky_skylightstart_var) / (sky_skylightend_var - sky_skylightstart_var) );
#endif
	
	//----------------------------------------------------------------
	// bump and reflection coordinates
   
   float2 Speed;
   Speed.x  = (mwater_speedx_var * 0.000002);
   Speed.y  = (mwater_speedy_var * 0.000002);
   
   Out.Tex01 = 0;
   
	Out.Tex01.xy = (inTex + Speed * vecTime.w) * (mwater_scale_var * 0.05);
	Out.Tex01.zw = (inTex + Speed * vecTime.w * material_mwater_speed2) * (mwater_scale_var * 0.05) * material_mwater_scale2;
	
	Out.TexM = Out.Pos.xyw;
	
	//----------------------------------------------------------------
	// bump scale
	
	Out.Ripple = (mwater_ripple_var * 0.001);
	
	//----------------------------------------------------------------
	// color and transparency
	
	Out.Color   = float4(vecColor.xyz + vecSunColor.xyz, fAlpha);
{}
#ifdef FRESNEL_TRANSPARENCY	
	float3 vecToView   = normalize(vecViewPos - PosWorld); 													// direction towards camera	
	float3 WorldNormal = normalize(mul(inNormal, (float3x3)matWorld));
	
	Out.Color.a = 0.67 + fAlpha * (1.0 - dot(vecToView, WorldNormal));
#endif
	
	//----------------------------------------------------------------
		
	return Out;
}
	
/////////////////////////////////////////////////////////////////////////////

float4 ps_water_mirror(PS_in_mirror In) : COLOR
{
	//------------------------------------------------------------------
	// normals
	
	float4 Bump  = tex2D(sBumpTex, In.Tex01.xy) * 2.0 - 1.0;
	float4 Bump2 = tex2D(sBumpTex, In.Tex01.zw) * 2.0 - 1.0;
	
	Bump			 = lerp(Bump, Bump2, 0.5f);
	
	//------------------------------------------------------------------
	// mirror sampling
	
	float2 Coord = 0.5 * (1.0 + In.TexM.xy/In.TexM.z) + Bump.xy * In.Ripple;
	
	float4 finalColor = tex2D(sMirrorTex, Coord) * In.Color * float4(In.Color.rgb, 1.0); 
	
	//----------------------------------------------------------------
	// Sun light to reflect
{}	
#ifdef SUNLIGHT	
	float3 camToSurface		= In.WorldPos - vecViewPos;
	camToSurface				= normalize(-camToSurface);
	
	float3 relfectedVector	= normalize(reflect(-camToSurface, Bump));        
	float3 surfaceToSun		= normalize(vecSunPos - In.WorldPos);
	
	float3 sunlight 			= material_mwater_sunlightstrength * pow(saturate(dot(relfectedVector, surfaceToSun)), material_mwater_sunlightarea) * vecSunColor;
	
	finalColor.rgb 			+= saturate(sunlight.xyz);		
#endif
		
	//------------------------------------------------------------------
	// fog
	
	finalColor.rgb = lerp(finalColor.rgb, vecFogColor, In.Fog);		
	
	//------------------------------------------------------------------
	// add sky light	

#ifdef SKY_LIGHT	
	finalColor.rgb = lerp(finalColor.rgb, sky_skylightcolor_var, In.SkyLight);		
#endif
	
	//------------------------------------------------------------------
	
	return finalColor;
}

//////////////////////////////////////////////////////////////////

technique water_mirror
	{
		pass one
		{
			AlphaBlendEnable 	= True;
		   
			VertexShader = compile vs_3_0 vs_water_mirror();
			PixelShader  = compile ps_3_0 ps_water_mirror();			
		}		
	}

technique fallback { pass one { } }
