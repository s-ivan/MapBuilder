
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs PS

struct inSpritePS 
{
	float4 Color			: COLOR0;  
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
   float3 SunDir			: TEXCOORD3;
#ifdef PER_PIXEL_LIGHTS
	float3 WorldPos		: TEXCOORD4;
#endif
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

//------------------------------------------------------------------------------------------------

struct inSpritePS2
{
	float4 Color			: COLOR0;  
   float2 Tex				: TEXCOORD0; 
	float4 Shadow			: TEXCOORD7;
	float  Fog 				: FOG;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

float4 SpritePS(inSpritePS In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
   float shadow1 = PS_shadowmapping(In.Shadow, scsm_esm_obj_var);
    
	//----------------------------------------------------------
   // normalize input vectors
   
   In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal = normalize(In.WorldNormal);
   
   //----------------------------------------------------------
   // lighting
   
   float LightIntensity = saturate(dot(In.WorldNormal, -In.SunDir));
      
   // diffuse   
   float4 Diffuse = 8.0 * vecDiffuse * LightIntensity * vecSunColor * vecLight;													// vecSunDir is in view space thus not good here							
	
	// specular										
	float4 Specular = 0;
	if (LightIntensity > 0.25f)
		{
			float3 Halfway  = saturate(dot(normalize(In.ViewDir - In.SunDir), In.WorldNormal));			
			Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor * vecLight;					
			
//			// spec mapping
//			Specular = min( Specular , SpecularMap ); 																						// or Specular * SpecularMap
		}
	// specular - another calculation
//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - In.SunDir);													// R
//	float4 Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor * vecLight;			// R.V^n 
	
	// got from vertex shader
//	// ambient
//	float4 Ambient = vecAmbient * vecLight;																															
//	
//	// emissive
//	float4 Emissive = vecEmissive * vecColor;													
	
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
   //----------------------------------------------------------
   // dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	// per pixel dynamic lights
	float4 Lights = 0;
	for (int i=1; i<iLights; i++)
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
	Lights *= 2.0 * vecDiffuse;																				// enbrighten to match vertex light look
#endif	
	
	//----------------------------------------------------------
	// add lighting to color
	
#ifdef PER_PIXEL_LIGHTS	
	Color.rgb *= (/*Ambient + Emissive +*/ Diffuse + Specular + Lights + In.Color);			// with per pixel dynamic lights, InColor contains only entity ambient fAmbient
#else
	Color.rgb *= (/*Ambient + Emissive +*/ Diffuse + Specular + (In.Color));					// with per vertex dynamic lights - saturate needed only for sprites? no!
#endif
	
	//----------------------------------------------------------
	// add shadows
	
	Color.rgb *= clamp(shadow1, 1-scsm_run_var, 1);														// dynamic shadows, add if loop runs
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);													// sm 3.0 fog support	
	
	//----------------------------------------------------------
	
	return Color;
}


//------------------------------------------------------------------------------------------------


float4 SpritePS2(inSpritePS2 In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
   float shadow1 = PS_shadowmapping(In.Shadow, scsm_esm_obj_var);
    
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
	//----------------------------------------------------------
	// add lighting to color
	
	Color.rgb *= (/*Ambient + Diffuse + Specular + Emissive +*/ In.Color);						// with per vertex dynamic lights

	//----------------------------------------------------------
	// add shadows
	
	Color.rgb *= clamp(shadow1, 1-scsm_run_var, 1);														// dynamic shadows, add if loop runs
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);													// sm 3.0 fog support	
	
	//----------------------------------------------------------
	
	return Color;
}