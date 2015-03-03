
bool AUTORELOAD;

/////////////////////////////////////////////////////////////////////

#define PER_PIXEL_LIGHTS							// max 4 in PS2.a , 8 in PS3.0

float texShadowAlpha = 0.50f; 					//shadowalpha of shadowtexture		lightmap shadow darkness

float map_detailsize_ground_var = 40.0f;		// like detail_size
float map_blendfactor_ground_var	= 0.5f;		// tile blending strength over base colormap

#include <lights>

#include <color>

/////////////////////////////////////////////////////////////////////

texture entSkin1; 	// global colormap    rgb

texture entSkin2;		// tile colormaps 1-7
texture entSkin3;
texture entSkin4;

texture mtlSkin1;
texture mtlSkin2;
texture mtlSkin3;
texture mtlSkin4;

texture LightMap;		// global shadowmap rgb

/////////////////////////////////////////////////////////////////////

float4x4 matWorld;
float4x4 matWorldViewProj;

float4 vecSunPos;
float4 vecSunColor;
float4 vecSunDir;

float4 vecViewDir;
float4 vecViewPos;

float4 vecFog;	
float3 vecFogColor;		

// defined in <lights>
//float4 vecLightPos[8];
//float4 vecLightColor[8];
float4 vecLightDir[8];

// defined in <color>
//float4 vecColor;
//float4 vecAmbient;
//float4 vecDiffuse;
//float4 vecSpecular;
//float4 vecEmissive;
//float4 vecLight;
//float fPower;

/////////////////////////////////////////////////////////////////////

sampler colorSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//----------------------------------------

sampler tileSampler1 = sampler_state 
{ 
   Texture = <entSkin2>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

sampler tileSampler2 = sampler_state 
{ 
   Texture = <entSkin3>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

sampler tileSampler3 = sampler_state 
{ 
   Texture = <entSkin4>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

sampler tileSampler4 = sampler_state 
{ 
   Texture = <mtlSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

sampler tileSampler5 = sampler_state 
{ 
   Texture = <mtlSkin2>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

sampler tileSampler6 = sampler_state 
{ 
   Texture = <mtlSkin3>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

sampler tileSampler7 = sampler_state 
{ 
   Texture = <mtlSkin4>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

//----------------------------------------------

sampler LightMapSampler = sampler_state 
{ 
   Texture = <LightMap>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
}; 

/////////////////////////////////////////////////////////////////////

#ifdef PER_PIXEL_LIGHTS

float4 DoPointLightPS(float3 P, float3 N, int i) 
{
	if (length(vecLightDir[i]) > 0) return float4(0,0,0,0);				// exclude directional lights like Sun
	
	float3 D = (float3)vecLightPos[i]-P;
	float NdotL = dot(N, normalize(D));
	float4 Color = vecLightColor[i] * NdotL;

	float fac = 0.f;
	if (NdotL >= 0.f && vecLightPos[i].w > 0.f) {
		float LD = length(D)/vecLightPos[i].w;
		if (LD < 1.f)
			fac = 1.f - LD;
	}
	return Color * fac;
}

#endif

/////////////////////////////////////////////////////////////////////

void terrain_VS
	( 
   in float4 InPos		: POSITION, 
   in float3 InNormal	: NORMAL, 
   in float2 InTex		: TEXCOORD0,
   
	out float4 OutColor 			: COLOR0,   			
   out float4 OutPos				: POSITION, 			
   out float2 OutTex				: TEXCOORD0, 			
   out float3 OutViewDir		: TEXCOORD2,			
	out float3 OutWorldNormal	: TEXCOORD3,	
#ifdef PER_PIXEL_LIGHTS
	out float3 OutWorldPos		: TEXCOORD6,		
#endif			
   out float  OutFog 			: FOG						
   ) 					
{ 
	// Transform the vertex from object space to clip space-----------
   OutPos = mul(InPos, matWorldViewProj); 
   
	// Pass the texture coordinate to the pixel shader----------------
	OutTex = InTex;
   
   //----------------------------------------------------------------
   float4 PosWorld = mul(InPos, matWorld);															
  	
{}  	
#ifdef PER_PIXEL_LIGHTS
  	OutWorldPos    = PosWorld;																				// for pp dynamic lights
#endif    	
	OutWorldNormal = normalize(mul(InNormal, matWorld));											// for diffuse and specular
	OutViewDir 		= vecViewPos - PosWorld;															// for specular
	
	//Fog-------------------------------------------------------------
	OutFog = saturate( 1 - (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );	// saturate needed when fadeout distance is closer than clipping distance
	
	//dynamic Lights--------------------------------------------------
	OutColor = DoAmbient();																					// ambient + emissive
{}	
#ifndef PER_PIXEL_LIGHTS	
	for (int i=1; i<iLights; i++)  																		// Add max 8 dynamic lights, except Sun
		OutColor += DoLight(PosWorld, OutWorldNormal, i-1);									
	OutColor *= vecDiffuse;
#endif		
}

/////////////////////////////////////////////////////////////////////

float4 terrain_lm_PS
	(
	float4 InColor		 		: COLOR0,			
	float2 Tex 					: TEXCOORD0,
	float3 InViewDir			: TEXCOORD2,
	float3 InWorldNormal		: TEXCOORD3
#ifdef PER_PIXEL_LIGHTS
	,
	float3 InWorldPos			: TEXCOORD6	
#endif	
	) : COLOR0
{
	//----------------------------------------------------------
	// colormap
	
	float4 color = tex2D(colorSampler, Tex);			
		
	float2 Tile = frac( Tex * map_detailsize_ground_var );		
		
	//----------------------------------------------------------
	// tile colormaps
	
	float4 color1 = tex2D(tileSampler1, Tile);	
//	color1.a 	  = tex2D(tileSampler1, Tex).a;
		
	float4 color2 = tex2D(tileSampler2, Tile);	
	color2.a 	  = tex2D(tileSampler2, Tex).a;
		
	float4 color3 = tex2D(tileSampler3, Tile);
	color3.a 	  = tex2D(tileSampler3, Tex).a;
		
	float4 color4 = tex2D(tileSampler4, Tile);		
	color4.a 	  = tex2D(tileSampler4, Tex).a;
	
	float4 color5 = tex2D(tileSampler5, Tile);		
	color5.a 	  = tex2D(tileSampler5, Tex).a;
	
	float4 color6 = tex2D(tileSampler6, Tile);		
	color6.a 	  = tex2D(tileSampler6, Tex).a;
	
	float4 color7 = tex2D(tileSampler7, Tile);		
	color7.a 	  = tex2D(tileSampler7, Tex).a;
	
	float summedweight = color2.a + color3.a + color4.a + color5.a + color6.a + color7.a;
	
	// in terraintexMB.fx the first color tilemap should have full cover, this is compensated now
	color1.a = saturate(1 - summedweight); 
	summedweight += color1.a;
	
   //----------------------------------------------------------
   // Sun lighting
   
   InViewDir	  = normalize(InViewDir);
   InWorldNormal = normalize(InWorldNormal);
   
//	// ambient
//	float4 Ambient = vecAmbient * vecLight;																												
   
   // diffuse
   float4 Diffuse = 8.0 * vecDiffuse * saturate(dot(InWorldNormal, -vecSunDir)) * vecSunColor * vecLight;					
   
	// specular									
	float3 Halfway   = saturate(dot(normalize(InViewDir - vecSunDir), InWorldNormal));
	float4 Specular = vecSpecular * pow( dot(InWorldNormal, Halfway) , fPower) * vecSunColor * vecLight;							
	
//	// emissive
//	float4 Emissive = vecEmissive;	
	
	//----------------------------------------------------------	
	// texture
	
   float4 Color = 1;
   Color.rgb = lerp(color.rgb, clamp((color1.rgb*color1.a + color2.rgb*color2.a + color3.rgb*color3.a + color4.rgb*color4.a + color5.rgb*color5.a + color6.rgb*color6.a + color7.rgb*color7.a)/summedweight,0,1), map_blendfactor_ground_var);										
   //lerp(x,y,s) = x*(1-s) + y*s;	 0.67 means stronger tile blending
	
	//----------------------------------------------------------
	// dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	// per pixel dynamic lights
	float4 Lights = 0;
	for (int i=1; i<=4; i++)
		Lights += DoPointLightPS(InWorldPos, InWorldNormal, i-1);
	Lights *= vecDiffuse;																	
#endif	
		
	//----------------------------------------------------------		
	// color
	
#ifdef PER_PIXEL_LIGHTS	
	Color.rgb *= (Diffuse + Specular + Lights + InColor);			// with per pixel dynamic lights, InColor contains only entity ambient + emissive
#else
	Color.rgb *= (Diffuse + Specular + InColor);						// with per vertex dynamic lights
#endif
	
	//----------------------------------------------------------	
	// lightmap = static shadowmap = 0.5 * lightmap
	
	float4 Lightmap = 2.0 * tex2D(LightMapSampler, Tex);			// 1.0 * for L3DT lightmap, 2.0 * for 3dgs lightmap
	Color.rgb *= clamp(Lightmap, texShadowAlpha, 1);												
		
	//----------------------------------------------------------
	
	return float4(saturate(Color.rgb),1);
}

/////////////////////////////////////////////////////////////////////

float4 terrain_PS
	(
	float4 InColor		 		: COLOR0,			
	float2 Tex 					: TEXCOORD0,
	float3 InViewDir			: TEXCOORD2,
	float3 InWorldNormal		: TEXCOORD3
#ifdef PER_PIXEL_LIGHTS
	,
	float3 InWorldPos			: TEXCOORD6		
#endif	
	) : COLOR0
{
	//----------------------------------------------------------
	// colormap
	
	float4 color = tex2D(colorSampler, Tex);				
		
	float2 Tile = frac( Tex * map_detailsize_ground_var );	
		
	//----------------------------------------------------------
	// tile colormaps
	
	float4 color1 = tex2D(tileSampler1, Tile);	
//	color1.a 	  = tex2D(tileSampler1, Tex).a;
		
	float4 color2 = tex2D(tileSampler2, Tile);	
	color2.a 	  = tex2D(tileSampler2, Tex).a;
		
	float4 color3 = tex2D(tileSampler3, Tile);
	color3.a 	  = tex2D(tileSampler3, Tex).a;
		
	float4 color4 = tex2D(tileSampler4, Tile);		
	color4.a 	  = tex2D(tileSampler4, Tex).a;
	
	float4 color5 = tex2D(tileSampler5, Tile);		
	color5.a 	  = tex2D(tileSampler5, Tex).a;
	
	float4 color6 = tex2D(tileSampler6, Tile);		
	color6.a 	  = tex2D(tileSampler6, Tex).a;
	
	float4 color7 = tex2D(tileSampler7, Tile);		
	color7.a 	  = tex2D(tileSampler7, Tex).a;
	
	float summedweight = color2.a + color3.a + color4.a + color5.a + color6.a + color7.a;
	
	// in terraintexMB.fx the first color tilemap should have full cover, this is compensated now
	color1.a = saturate(1 - summedweight); 
	summedweight += color1.a;
	
   //----------------------------------------------------------
   // Sun lighting
   
   InViewDir	  = normalize(InViewDir);
   InWorldNormal = normalize(InWorldNormal);
   
//	// ambient
//	float4 Ambient = vecAmbient * vecLight;																												
   
   // diffuse
   float4 Diffuse = 8.0 * vecDiffuse * saturate(dot(InWorldNormal, -vecSunDir)) * vecSunColor * vecLight;					
   
	// specular									
	float3 Halfway   = saturate(dot(normalize(InViewDir - vecSunDir), InWorldNormal));
	float4 Specular = vecSpecular * pow( dot(InWorldNormal, Halfway) , fPower) * vecSunColor * vecLight;							
	
//	// emissive
//	float4 Emissive = vecEmissive;	
	
	//----------------------------------------------------------	
	// texture
	
   float4 Color = 1;
   Color.rgb = lerp(color.rgb, clamp((color1.rgb*color1.a + color2.rgb*color2.a + color3.rgb*color3.a + color4.rgb*color4.a + color5.rgb*color5.a + color6.rgb*color6.a + color7.rgb*color7.a)/summedweight,0,1), map_blendfactor_ground_var);										
   //lerp(x,y,s) = x*(1-s) + y*s;	 0.67 means stronger tile blending
	
	//----------------------------------------------------------
	// dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	// per pixel dynamic lights
	float4 Lights = 0;
	for (int i=1; i<=4; i++)
		Lights += DoPointLightPS(InWorldPos, InWorldNormal, i-1);
	Lights *= vecDiffuse;																	
#endif	
		
	//----------------------------------------------------------		
	// color
	
#ifdef PER_PIXEL_LIGHTS	
	Color.rgb *= (Diffuse + Specular + Lights + InColor);			// with per pixel dynamic lights, InColor contains only entity ambient + emissive
#else
	Color.rgb *= (Diffuse + Specular + InColor);						// with per vertex dynamic lights
#endif
	
	//----------------------------------------------------------
	
	return float4(saturate(Color.rgb),1);
}

/////////////////////////////////////////////////////////////////////

technique terrain
{
	pass p0
	{
		VertexShader = compile vs_2_0 terrain_VS();
		PixelShader  = compile ps_2_a terrain_PS();
	}
}

technique terrain_lm
{
	pass p0
	{
		VertexShader = compile vs_2_0 terrain_VS();
		PixelShader  = compile ps_2_a terrain_lm_PS();
	}
}