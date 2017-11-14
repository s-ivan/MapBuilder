// bad

bool AUTORELOAD;

//bool REQUIRE_NORMAL;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

#define PER_PIXEL_LIGHTS			

//#define HEIGHT_FOG

////////////////////////////////////////////////////////////////////////////////////////////////
// matrices, vectors, variables passed by the engine

float4x4 matWorldViewProj;
float4x4 matWorld;

float3x3 matTangent;							// needed for normalmapping

float4 vecFog;
float3 vecFogColor;
	
//float fog_color_var;
//float map_fog_height_var;				// 250	(0..clip_far)
//float sky_hfogheightstrength_var;		// 0.25	(0..1)

float4 vecViewPos;
float4 vecViewDir;

float4 vecSunColor;	
float4 vecSunPos;
float3 vecSunDir;

float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;
float  fPower;

float4 vecTime;		

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
};

texture entSkin2; 							// Normalmap

// Normal map sampler
sampler NormalSampler = sampler_state	
{
	Texture = <entSkin2>;
//	MinFilter = None;
//	MagFilter = None;
//	MipFilter = None;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs

struct outVS 
{
	float4 Color			: COLOR0; 
	float4 Pos				: POSITION; 
	float2 Tex				: TEXCOORD0; 
	float3 ViewDir			: TEXCOORD1;	// in tangent space
	float3 WorldNormal	: TEXCOORD2;	
	float3 Tangent			: TEXCOORD3;
	float3 Binormal		: TEXCOORD4;
	float3 SunDir			: TEXCOORD5;	// in tangent space
#ifdef PER_PIXEL_LIGHTS	
	float3 WorldPos		: TEXCOORD6;
#endif	
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};


struct inPS 
{
	float4 Color			: COLOR0;  
	float2 Tex				: TEXCOORD0; 
	float3 ViewDir			: TEXCOORD1;	// in tangent space
	float3 WorldNormal	: TEXCOORD2;	
	float3 Tangent			: TEXCOORD3;
	float3 Binormal		: TEXCOORD4;
	float3 SunDir			: TEXCOORD5;	// in tangent space
#ifdef PER_PIXEL_LIGHTS	
	float3 WorldPos		: TEXCOORD6;
#endif	
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// includes

#include "sc_pointlight_MB.fx"

#include "sc_shadowmapping_MB.fx"		// scsm shadowmapping - variables, shadow samplers, vertex shader functions, pixel shader function

#ifdef HEIGHT_FOG
	#include "sc_heightfog.fx"				// blend distance fog with height fog
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

outVS ModelNVS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0,
   float3 InTangent	: TEXCOORD2			// was float4 but that is bad with flat separated surfaces
	) 
{ 
   outVS Out;
	
	InPos.w = 1.0f;	// to get proper matrix multiplication
	
	//----------------------------------------------------------------
	// Pass the texture coordinate to the pixel shader
	
   Out.Tex = InTex;
   
   //----------------------------------------------------------------
	// position in current view
	
	Out.Pos = mul(InPos, matWorldViewProj); 
	
   //---------------------------------------------------------------
   // position in world space
   
	float4 PosWorld = mul(InPos, matWorld);													// needed for a couple of calculations... float4 for shadowmapping!
	
//	float4 PosWorld = mul(float4(InPos.xyz,1), matWorld).xyz;							// or this?      
//	PosWorld.w = Out.Pos.z;
	
	//----------------------------------------------------------------
{}
#ifdef PER_PIXEL_LIGHTS
//	PosWorld = mul(InPos, matWorld);																// recalculate due to modified position
  	Out.WorldPos    = PosWorld.xyz;
#endif 
	
	//----------------------------------------------------------------
	// needed for normalmapping - compute 3x3 matrix to transform from world space to tangent space
		
	matTangent[0] = normalize( mul(InTangent.xyz, (float3x3)matWorld) );									// tangent
	matTangent[1] = -abs(normalize( mul(cross(InTangent.xyz,InNormal), (float3x3)matWorld) ));	// binormal, without cross() * InTangent.w !!!
																																// - abs(binormal) is needed because of float3 tangent, and to have similarly correct vertical shading on all sides!
	matTangent[2] = normalize( mul(InNormal, (float3x3)matWorld) );										// normal
	
	//----------------------------------------------------------------
	// needed for normalmapping
	
//	Out.ViewDir = normalize(vecViewPos.xyz - PosWorld.xyz);								// for flat specular - or = matViewInv[3].xyz - PosWorld;	= halfway vector		
	
	Out.ViewDir = normalize(mul(matTangent, vecViewPos - PosWorld));					// Calculate the view direction vector in tangent space
	
	//----------------------------------------------------------------
	// needed for normalmapping
	
	Out.SunDir = normalize(mul(matTangent, -vecSunDir));									// Calculate the light direction vector in tangent space
	
	//----------------------------------------------------------------
	// not needed for normalmapping
	
	Out.Tangent		 = normalize( mul(InTangent.xyz, (float3x3)matWorld) );										// tangent
	Out.Binormal	 = -abs(normalize( mul(cross(InTangent.xyz,InNormal), (float3x3)matWorld) ));			// binormal
	Out.WorldNormal = normalize( mul(InNormal, (float3x3)matWorld));												// normal
   
   //----------------------------------------------------------------
   
#ifndef HEIGHT_FOG   
	Out.Fog = saturate(fog_color_var)																					// fog is applied
				 * saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );					// saturate needed when fadeout distance is closer than clipping distance
#else
	Out.Fog = VS_heightfog(PosWorld.xyz);
#endif
   
   //----------------------------------------------------------------
	
	Out.Shadow = VS_shadowmapping(PosWorld);
	
	//----------------------------------------------------------------
	
   Out.Color = DoAmbient();																		// Add ambient and sun light
{}	
#ifndef PER_PIXEL_LIGHTS
	for (int i=1; i<=iLights; i++)  																// Add max 8 dynamic lights
//		Out.Color += PointLightDiffuse(PosWorld, Out.WorldNormal, i-1);				// matTangent[2] or OutWorldNormal
		Out.Color += PointLightDiffuse(PosWorld,   matTangent[2], i-1);				// matTangent[2] or OutWorldNormal
	Out.Color *= vecDiffuse;
#endif
   
   //-----------------------------------------------------------------
   
   return Out;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

float4 ModelNPS(inPS In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
   float shadow1 = PS_shadowmapping(In.Shadow, scsm_esm_obj_var);
   
   //----------------------------------------------------------
   // normalmapping
   
   In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
//	In.Tangent 		= normalize(In.Tangent);
//	In.Binormal 	= normalize(In.Binormal);
	In.WorldNormal = normalize(In.WorldNormal);
   
	float3 BumpNormal = 2 * tex2D(NormalSampler, In.Tex).rgb - 1;																		// Read the normal from the normal map and convert from [0..1] to [-1..1] range
//	BumpNormal = normalize(In.Tangent * BumpNormal.r + In.Binormal * BumpNormal.g + In.WorldNormal * BumpNormal.b);	// transform to world space
   BumpNormal = normalize(BumpNormal);
   
	//----------------------------------------------------------
   // lighting
   
   // ambient
   float4 Ambient = vecAmbient * vecLight;																															
   
   // diffuse   
//	float LightIntensity = saturate(dot(In.WorldNormal, -vecSunDir));
   float LightIntensity = saturate(dot(BumpNormal,  In.SunDir));												// saturate!
   
//	float4 Diffuse = 2.0 * vecDiffuse * LightIntensity * vecSunColor * vecLight;
	float4 Diffuse = 10.0 * vecDiffuse * (LightIntensity) * vecSunColor * vecLight;																	
	
	// specular
//	float3 Halfway  = saturate(dot(normalize( vecSunDir + In.ViewDir), In.WorldNormal));
	float3 Halfway  = saturate(dot(normalize( In.SunDir + In.ViewDir), BumpNormal));
	
//	float4 Specular = vecSpecular * pow( dot(In.WorldNormal, Halfway), fPower) * vecSunColor * vecLight;	
	float4 Specular = fPower*vecSpecular * pow( dot(BumpNormal, 		Halfway), 2.5f) * vecSunColor * vecLight;
	
//	// specular (bad, no 0 branching, thus spec visible where it should not be)
//	float3 R = normalize(2 * dot(BumpNormal, In.SunDir) * BumpNormal - In.SunDir); 
//	float4 Specular = vecSpecular * pow(saturate(dot(R, In.ViewDir)), 0.5f) * vecSunColor * vecLight * fPower; 
	
//	// specular (good)
//	float4 Specular = 0;
//	if (LightIntensity > 0)
//		{
//		//	float3 Halfway  = saturate(dot(normalize( vecSunDir + In.ViewDir), In.WorldNormal));
//			float3 Halfway  = saturate(dot(normalize(-In.SunDir + In.ViewDir), BumpNormal));
//			
//			float4 SpecularIntensity = 0.5f;
//		//	float4 Specular = vecSpecular * pow( dot(In.WorldNormal, Halfway), fPower) * vecSunColor * vecLight;	
//			Specular = vecSpecular * pow( dot(BumpNormal, 		Halfway), SpecularIntensity) * vecSunColor * vecLight * fPower;
//		}
	
//	// specular mapping (not too bad)
//	float4 Specular = 0;
//	if (LightIntensity > 0)
//		{
////			float4 SpecularIntensity = tex2D(SpecularSampler, In.Tex);
//			float4 SpecularIntensity = 0.5f;
//			
////			float3 reflection = normalize(2 * LightIntensity * BumpNormal + vecSunDir);
//			float3 reflection = normalize(2 * LightIntensity * BumpNormal - In.SunDir);
//			
//			Specular = vecSpecular * pow(saturate(dot(reflection, In.ViewDir)), SpecularIntensity) * vecSunColor * vecLight * fPower;
//		}
	
	// emissive
	float4 Emissive = vecEmissive;																		
	
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
   //----------------------------------------------------------
   // dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	// per pixel dynamic lights
	float4 Lights = 0;
	for (int i=1; i<=iLights; i++)
//		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);	
		Lights += PointLightDiffuse(In.WorldPos, BumpNormal, i-1);
	Lights *= 2.0 * vecDiffuse;																			// enbrighten to match vertex light look
#endif	
	
	//----------------------------------------------------------
	// add lighting to color
	
#ifdef PER_PIXEL_LIGHTS	
	Color.rgb *= (Ambient + Diffuse + Specular + Emissive + Lights + In.Color);			// with per pixel dynamic lights, InColor contains only entity ambient fAmbient
#else
	Color.rgb *= (Ambient + Diffuse + Specular + Emissive + In.Color);						// with per vertex dynamic lights
#endif

	//----------------------------------------------------------
	// add shadows
	
	Color.rgb *= clamp(shadow1, 1-scsm_run_var, 1);													// dynamic shadows, add if loop runs
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);												// sm 3.0 fog support	
	
	//----------------------------------------------------------
	
	return Color;
}

////////////////////////////////////////////////////////////////////////////////////////////////

technique model
{ 

   pass p0
   {
//   	cullmode = none;									
   	
   	ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False;
   	
		VertexShader = compile vs_3_0 ModelNVS();
     	PixelShader  = compile ps_3_0 ModelNPS();	
   }

}


technique fallback 
{ 
	pass 
	{
//		cullmode = none;										
		
		ZWriteEnable 		= True;		
		AlphaTestEnable 	= True;
	   AlphaBlendEnable 	= False; 	
	} 
}