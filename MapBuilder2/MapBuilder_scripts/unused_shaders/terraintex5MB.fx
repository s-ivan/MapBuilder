#include <lights>
#include <color>

bool AUTORELOAD;

/***************************************TWEAKABLES*****************************************************/

//#define PER_PIXEL_LIGHTS							// max 4 in PS2.a , 8 in PS3.0

float texShadowAlpha = 0.50f; 					//shadowalpha of shadowtexture		lightmap shadow darkness

float map_detailsize_ground_var = 40.0f;		// like detail_size
float map_blendfactor_ground_var	= 0.5f;		// tile blending strength over base colormap

/***************************************SHADER*CODE****************************************************/

texture entSkin1; 	// global colormap    rgb

texture entSkin2;
texture entSkin3;
texture entSkin4;

texture mtlSkin1;
texture mtlSkin2;
//texture mtlSkin3;
//texture mtlSkin4;

texture LightMap;		// global shadowmap rgb

/////////////////////////////////////////////////////////////////////

matrix matWorld;
matrix matWorldInv;
matrix matViewInv;
matrix matWorldViewProj;

float4x4 matEffect1;
float4x4 matEffect2;
float4x4 matEffect3;	// ***

float4 vecSunPos;
float4 vecSunColor;
//float4 vecLightPos[8];
float4 vecViewPos;
float4 vecFog;	

//$$$
float4 vecSunDir;
float4 vecViewDir;
//float4 vecLightColor[8];

// defined in <color>
//float4 vecColor;			// ent color rgba
//float4 vecAmbient;
//float4 vecDiffuse;
//float4 vecSpecular;
//float4 vecEmissive;
//float4 vecLight;			// ent color + prv
//float fPower;

//float fAmbient;
//float fAlbedo;

float3 vecFogColor;		//***

//float3x3 matTangent;		// $$$ create tangents in TEXCOORD2	

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

//sampler tileSampler6 = sampler_state 
//{ 
//   Texture = <mtlSkin3>; 
//   MinFilter = Linear;
//	MagFilter = Linear;
//	MipFilter = Linear;
//}; 

//sampler tileSampler7 = sampler_state 
//{ 
//   Texture = <mtlSkin4>; 
//   MinFilter = Linear;
//	MagFilter = Linear;
//	MipFilter = Linear;
//}; 

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
	out float3 OutWorldPos		: TEXCOORD6,			// needed only for ps per pixel dynamic lights
#endif			
   out float  OutFog 			: FOG						
   ) 					
{ 
	// Transform the vertex from object space to clip space-----------
   OutPos = mul(InPos, matWorldViewProj); 
   
	// Pass the texture coordinate to the pixel shader----------------
	OutTex = InTex;
   
   //----------------------------------------------------------------
   float4 PosWorld = mul(InPos, matWorld);															// needed for a couple of calculations... float4 for shadowmapping!
  	
{}  	
#ifdef PER_PIXEL_LIGHTS
  	OutWorldPos    = PosWorld;
#endif    	
	OutWorldNormal = normalize(mul(InNormal, matWorld));											// needed for diffuse and specular
	OutViewDir 		= vecViewPos - PosWorld;															// for specular
	
	//Fog-------------------------------------------------------------
	OutFog = saturate( 1 - (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );	// saturate needed when fadeout distance is closer than clipping distance
	
	//dynamic Lights--------------------------------------------------
	OutColor = DoAmbient();	//fAmbient; 																// Add entity ambient, sun light, and dynamic lights
{}	
#ifndef PER_PIXEL_LIGHTS	
	for (int i=1; i<=iLights; i++)  																		// Add max 8 dynamic lights
		OutColor += DoLight(PosWorld, OutWorldNormal, i-1);										// matTangent[2] or OutWorldNormal	
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
	float3 InWorldPos			: TEXCOORD6		// new - only for per pixel dynamic lights
#endif	
	) : COLOR0
{
	// color
	
	float4 color = tex2D(colorSampler, Tex);					// not essential to be sampled
		
	float2 Tile = frac( Tex * map_detailsize_ground_var );		// *** tiling
		
	//----------------------------------------------------------
	
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
	
//	float4 color6 = tex2D(tileSampler6, Tile);		
//	color6.a 	  = tex2D(tileSampler6, Tex).a;
	
//	float4 color7 = tex2D(tileSampler7, Tile);		
//	color7.a 	  = tex2D(tileSampler7, Tex).a;
	
	float summedweight = color2.a + color3.a + color4.a + color5.a;
	
	// in terraintexMB.fx the first color tilemap should have full cover, this is compensated now
	color1.a = saturate(1 - summedweight); 
	summedweight += color1.a;
	
   //----------------------------------------------------------
   
   InWorldNormal = normalize(InWorldNormal);
   
   // ambient
   float4 Ambient = vecAmbient * vecLight;																												
   
   // diffuse
   float4 Diffuse = vecDiffuse * dot(InWorldNormal, -vecSunDir) * vecSunColor * vecLight;					
   
	// specular
//	// Calculate the reflection vector:
//	float3 R = normalize(2 * dot(InWorldNormal, -vecSunDir) * InWorldNormal + vecSunDir);
//	// Calculate the speculate component:
//	InViewDir = normalize(InViewDir);
//	float4 Specular = pow(saturate(dot(R, InViewDir)), fPower) * vecSpecular * (vecLight + fAlbedo);									
	float3 Halfway   = saturate(dot(normalize(InViewDir - vecSunDir), InWorldNormal));
	float4 Specular = vecSpecular * pow( dot(InWorldNormal, Halfway) , fPower) * vecSunColor * vecLight;							
	
	// emissive
	float4 Emissive = vecEmissive;	
	
	//----------------------------------------------------------
	
	// texture
   float4 Color = 1;
   Color.rgb = lerp(color.rgb, clamp((color1.rgb*color1.a + color2.rgb*color2.a + color3.rgb*color3.a + color4.rgb*color4.a + color5.rgb*color5.a)/summedweight,0,1), map_blendfactor_ground_var);										
   //lerp(x,y,s) = x*(1-s) + y*s;	 0.67 means stronger tile blending
	
	//----------------------------------------------------------
{}
#ifdef PER_PIXEL_LIGHTS	
	// per pixel dynamic lights
	float4 Lights = 0;
	for (int i=1; i<=4; i++)
		Lights += DoPointLightPS(InWorldPos, InWorldNormal, i-1);
	Lights *= 2.0 * vecDiffuse;																		// enbrighten to match vertex light look
#endif	
		
	//----------------------------------------------------------
		
	// color
#ifdef PER_PIXEL_LIGHTS	
	Color.rgb *= (Ambient + Diffuse + Specular + Emissive + Lights + InColor);			// with per pixel dynamic lights, InColor contains only entity ambient fAmbient
#else
	Color.rgb *= (Ambient + Diffuse + Specular + Emissive + InColor);						// with per vertex dynamic lights
#endif
	
	//----------------------------------------------------------
	
	// lightmap = static shadowmap = 0.5 * lightmap
	float4 Lightmap = 2.0 * tex2D(LightMapSampler, Tex);										// *** 1.0 * for L3DT lightmap, 2.0 * for 3dgs lightmap		LmTex not okay, gets randomly? tiled
	Color.rgb *= clamp(Lightmap, texShadowAlpha, 1);											// ***			
	
	// 3dgs lightmap?
//	float4 Lightmap = tex2D(LightMapSampler, LmTex);											// ßßß
//	Lightmap = clamp(Lightmap, texShadowAlpha, 1);												// ßßß
//	Color.rgb *= 0.5 * float4(Diffuse+Lightmap*(Diffuse+Ambient.xyz),Ambient.w);		// ßßß  0.5 * if 3dgs original lightmap added, else no added brightness applied
	
	//----------------------------------------------------------
	
	return float4(saturate(Color.rgb),1);
}

//----------------------------------------------

float4 terrain_PS
	(
	float4 InColor		 		: COLOR0,			
	float2 Tex 					: TEXCOORD0,
	float3 InViewDir			: TEXCOORD2,
	float3 InWorldNormal		: TEXCOORD3
#ifdef PER_PIXEL_LIGHTS
	,
	float3 InWorldPos			: TEXCOORD6		// new - only for per pixel dynamic lights
#endif	
	) : COLOR0
{
	// color
	
	float4 color = tex2D(colorSampler, Tex);					// not essential to be sampled
		
	float2 Tile = frac( Tex * map_detailsize_ground_var );		// *** tiling
		
	//----------------------------------------------------------
	
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
	
//	float4 color6 = tex2D(tileSampler6, Tile);		
//	color6.a 	  = tex2D(tileSampler6, Tex).a;
	
//	float4 color7 = tex2D(tileSampler7, Tile);		
//	color7.a 	  = tex2D(tileSampler7, Tex).a;
	
	float summedweight = color2.a + color3.a + color4.a + color5.a;
	
	// in terraintexMB.fx the first color tilemap should have full cover, this is compensated now
	color1.a = saturate(1 - summedweight); 
	summedweight += color1.a;
	
   //----------------------------------------------------------
   
   InWorldNormal = normalize(InWorldNormal);
   
   // ambient
   float4 Ambient = vecAmbient * vecLight;																												
   
   // diffuse
   float4 Diffuse = vecDiffuse * dot(InWorldNormal, -vecSunDir) * vecSunColor * vecLight;					
   
	// specular
//	// Calculate the reflection vector:
//	float3 R = normalize(2 * dot(InWorldNormal, -vecSunDir) * InWorldNormal + vecSunDir);
//	// Calculate the speculate component:
//	InViewDir = normalize(InViewDir);
//	float4 Specular = pow(saturate(dot(R, InViewDir)), fPower) * vecSpecular * (vecLight + fAlbedo);									
	float3 Halfway   = saturate(dot(normalize(InViewDir - vecSunDir), InWorldNormal));
	float4 Specular = vecSpecular * pow( dot(InWorldNormal, Halfway) , fPower) * vecSunColor * vecLight;							
	
	// emissive
	float4 Emissive = vecEmissive;	
	
	//----------------------------------------------------------
	
	// texture
   float4 Color = 1;
   Color.rgb = lerp(color.rgb, clamp((color1.rgb*color1.a + color2.rgb*color2.a + color3.rgb*color3.a + color4.rgb*color4.a + color5.rgb*color5.a)/summedweight,0,1), map_blendfactor_ground_var);										
   //lerp(x,y,s) = x*(1-s) + y*s;	 0.67 means stronger tile blending
		
	//----------------------------------------------------------
{}
#ifdef PER_PIXEL_LIGHTS	
	// per pixel dynamic lights
	float4 Lights = 0;
	for (int i=1; i<=4; i++)
		Lights += DoPointLightPS(InWorldPos, InWorldNormal, i-1);
	Lights *= 2.0 * vecDiffuse;																		// enbrighten to match vertex light look
#endif	
		
	//----------------------------------------------------------
		
	// color
#ifdef PER_PIXEL_LIGHTS	
	Color.rgb *= (Ambient + Diffuse + Specular + Emissive + Lights + InColor);			// with per pixel dynamic lights, InColor contains only entity ambient fAmbient
#else
	Color.rgb *= (Ambient + Diffuse + Specular + Emissive + InColor);						// with per vertex dynamic lights
#endif
		
//	//----------------------------------------------------------
//	
//	// lightmap = static shadowmap = 0.5 * lightmap
//	float4 Lightmap = 2.0 * tex2D(LightMapSampler, Tex);											// *** 1.0 * for L3DT lightmap, 2.0 * for 3dgs lightmap		LmTex not okay, gets randomly? tiled
//	Color.rgb *= clamp(Lightmap, texShadowAlpha, 1);												// ***			
//	
//	// 3dgs lightmap?
////	float4 Lightmap = tex2D(LightMapSampler, LmTex);											// ßßß
////	Lightmap = clamp(Lightmap, texShadowAlpha, 1);												// ßßß
////	Color.rgb *= 0.5 * float4(Diffuse+Lightmap*(Diffuse+Ambient.xyz),Ambient.w);		// ßßß  0.5 * if 3dgs original lightmap added, else no added brightness applied
//	
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