
////////////////////////////////////////////////////////////////////////////////////////////////
// skins/textures

texture entSkin1; 									// Colormap

// Color map sampler
sampler colorSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs

struct inBonesPS 
{
	float4 Color			: COLOR0; 
   float2 Tex				: TEXCOORD0; 
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
#ifdef PER_PIXEL_LIGHTS   
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
	float3 WorldPos		: TEXCOORD4;
#endif
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

struct inBonesPS2
{
	float4 Color			: COLOR0; 
   float2 Tex				: TEXCOORD0; 
#ifdef SKY_LIGHT
	float  SkyLight		: TEXCOORD3;
#endif
#ifdef PER_PIXEL_LIGHTS_LOD   
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
	float3 WorldPos		: TEXCOORD4;
#endif
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

//struct inBonesPSX
//{
//	float4 Color			: COLOR0; 
//	float2 Tex				: TEXCOORD0;
//	float  Fog 				: FOG;
//};

//////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

float4 BonesPS1(inBonesPS In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
   float Shadow = PS_shadowmapping_Fast(In.Shadow, scsm_esm_obj_var);
   
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
   //----------------------------------------------------------
   // dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	// per pixel dynamic lights
	float4 Lights = 0;
	
	for (int i=1; i<=iLights; i++)																		// add dynamic lights, and Sun (always the last one)
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
		
	Lights *= 1.0f * vecDiffuse;																
	
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (Lights + In.Color);																	// with per pixel dynamic lights, InColor contains only entity ambient fAmbient
#else
	Color.rgb *= (In.Color);																				// with per vertex dynamic lights
#endif

	//----------------------------------------------------------
	// add shadows
	
	Color.rgb *= Shadow;																						// dynamic shadows
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);												// sm 3.0 fog support	
	
	//----------------------------------------------------------
	
	return Color;
}


///////////////////////////////////////////////


float4 BonesPS2(inBonesPS In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
//	float Shadow = PS_shadowmapping_Fast(In.Shadow, scsm_esm_obj_var);
   
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
   //----------------------------------------------------------
   // dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS_LOD
	float4 Lights = 0;
	
	for (int i=1; i<=iLights; i++)																		// add dynamic lights, and Sun (always the last one)
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
		
	Lights *= 1.0f * vecDiffuse;																
	
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (Lights + In.Color);																	// with per pixel dynamic lights, InColor contains only entity ambient fAmbient
#else
	Color.rgb *= (In.Color);																				// with per vertex dynamic lights
#endif

	//----------------------------------------------------------
	// add shadows
	
//	Color.rgb *= Shadow;																						// dynamic shadows, add if loop runs
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);												// sm 3.0 fog support	
	
	//----------------------------------------------------------
	// add sky light	
{}
#ifdef SKY_LIGHT	
	Color.rgb = lerp(Color.rgb, sky_skylightcolor_var, In.SkyLight);		
#endif
	
	//----------------------------------------------------------
	
	return Color;
}


//float4 BonesPSX(inBonesPSX In): COLOR
//{
//	//----------------------------------------------------------
//	// color
//	
//	float4 Color = In.Color;
//	
//	//----------------------------------------------------------
//	// add fog
//	
//	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);												// sm 3.0 fog support	
//	
//	//----------------------------------------------------------
//	
//	return Color;
//}