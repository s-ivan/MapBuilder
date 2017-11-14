// a useless shader done for experimental purposes

bool AUTORELOAD;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

#define PER_PIXEL_LIGHTS						

#define HEIGHT_FOG

//#define DEBUG_TILE

////////////////////////////////////////////////////////////////////////////////////////////////
// matrices, vectors, variables passed by the engine

float4x4 matWorldViewProj;
float4x4 matWorld;

float4 vecFog;
float4 vecFogColor;	

float4 vecViewPos;
float4 vecViewDir;

float4 vecSunColor;
float4 vecSunDir;
float4 vecSunPos;

float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;
float  fPower;

////////////////////////////////////////////////////////////////////////////////////////////////
// custom matrices, vectors, variables

float scsm_lightmapalpha_var		= 0.50f; 		//shadowalpha of shadowtexture		lightmap shadow darkness

float map_detailsize_ground_var 	= 40.0f;			// like detail_size
float map_blendfactor_ground_var	= 0.5f;			// tile blending strength over base colormap

float sky_loddistance1_var			= 500.0f;		// lod1 distance
float sky_loddistance2_var			= 1000.0f;		// lod2 distance
float sky_loddistance3_var			= 2000.0f;		// lod3 distance

////////////////////////////////////////////////////////////////////////////////////////////////
// skins/textures/samplers

texture entSkin1; 	// global colormap

texture entSkin2;		// tile map1

texture LightMap;		// global shadowmap rgb

//----------------------------------------

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
	AddressU = mirror;
	AddressV = mirror;
}; 

//----------------------------------------------

sampler LightMapSampler = sampler_state 
{ 
   Texture = <LightMap>; 
   MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs

struct outTerrainVS
{
	float4 Pos				: POSITION;
	float4 Color			: COLOR0;    
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
   float3 SunDir			: TEXCOORD3;
	float3 WorldPos		: TEXCOORD4;
	float4 Shadow			: TEXCOORD7;
	float	 ViewDistance	: TEXCOORD8;
	float4 Specular		: TEXCOORD9;
   float  Fog 				: FOG;
};

struct inTerrainPS 
{
	float4 Color			: COLOR0;  
   float2 Tex				: TEXCOORD0; 
   float3 ViewDir			: TEXCOORD1;
   float3 WorldNormal	: TEXCOORD2;
   float3 SunDir			: TEXCOORD3;
	float3 WorldPos		: TEXCOORD4;	
	float4 Shadow			: TEXCOORD7;
	float	 ViewDistance	: TEXCOORD8;
	float4 Specular		: TEXCOORD9;
	float  Fog 				: FOG;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// includes

#include "sc_pointlight_MB.fx"

#include "sc_shadowmapping_MB.fx"		// scsm shadowmapping - variables, shadow samplers, vertex shader functions, pixel shader function

#ifdef HEIGHT_FOG
	#include "sc_heightfog.fx"				// blend distance fog with height fog
#endif

#ifdef DEBUG_TILE
	#include "rts_tiledarea_MB.fx"
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

outTerrainVS TerrainVS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0
	) 
{ 
	outTerrainVS Out;
	
//	InPos.w = 1.0f;																			// to get proper matrix multiplication?
	
	//----------------------------------------------------------------
	
	Out.Tex = InTex;
	
	//---------------------------------------------------------------
	
	float4 PosWorld = mul(InPos, matWorld);											// needed for a couple of calculations... float4 for shadowmapping!
	
	//----------------------------------------------------------------
	
	Out.Pos = mul(InPos, matWorldViewProj); 
	
	//----------------------------------------------------------------
	
  	Out.WorldPos = PosWorld.xyz;															// float3 !
  		
	//----------------------------------------------------------------
	
	Out.ViewDir = normalize(vecViewPos.xyz - PosWorld.xyz);						// for flat specular - or = matViewInv[3].xyz - PosWorld;	= halfway vector
	
	//----------------------------------------------------------------
	
	Out.SunDir = normalize(PosWorld.xyz - vecSunPos);
	
	//----------------------------------------------------------------
	
	Out.WorldNormal = normalize(mul(InNormal, (float3x3)matWorld));			// when no normalmap used - normalize because of entity scale!
	
	//----------------------------------------------------------------
	
//	Out.ViewDistance = distance(PosWorld, vecViewPos);
	Out.ViewDistance = length(vecViewPos.xyz - PosWorld.xyz);
	
	//----------------------------------------------------------------
{}
#ifndef HEIGHT_FOG   
	Out.Fog = vecFogColor.w																	// fog is applied
				 * saturate( (Out.ViewDistance - vecFog.x) * (vecFog.z) );		// saturate needed when fadeout distance is closer than clipping distance
#else
	Out.Fog = VS_heightfog(PosWorld.xyz);
#endif
	
	//----------------------------------------------------------------
	
	Out.Shadow = VS_shadowmapping(PosWorld);
	
	//----------------------------------------------------------------
	// lighting
	
	Out.Specular = 0;
	
	if (Out.ViewDistance < sky_loddistance3_var*2.0f)
		{
			// dynamic lights
			float4 Lights = 0;	
{}	
#ifndef PER_PIXEL_LIGHTS		
			for (int i=1; i<=iLights; i++)  														// Add max 8 dynamic lights
				Lights += PointLightDiffuse(PosWorld, Out.WorldNormal, i-1);			// matTangent[2] or OutWorldNormal
			Lights *= vecDiffuse * 2.5f;
#endif
			
			// final color
			Out.Color = (vecAmbient * vecLight) + (vecEmissive * vecColor);			// ambient + emissive
			Out.Color += Lights;
		}
	else
		{
			float LightIntensity = dot(Out.WorldNormal, -Out.SunDir);
						
			// specular
			Out.Specular = 0;
			if (LightIntensity > 0.25f)
				{
					float3 Halfway  = saturate(dot(normalize(Out.ViewDir - Out.SunDir), Out.WorldNormal));
					Out.Specular = vecSpecular * pow( dot(Out.WorldNormal, Halfway) , fPower) * vecSunColor * vecLight;	
				}
			
			// dynamic lights + diffuse
			float4 Lights = 0;	
			for (int i=1; i<=iLights; i++)  														
				Lights += PointLightDiffuse(PosWorld, Out.WorldNormal, i-1);					
			Lights *= 2.5f * vecDiffuse;
			
			// final color
			Out.Color = (vecAmbient * vecLight) + (vecEmissive * vecColor);				// ambient + emissive
			Out.Color += Lights;
		}
	
	//-----------------------------------------------------------------
	
	return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shaders

// with lightmap
float4 Terrain1LmPS(inTerrainPS In) : COLOR0
{
	//----------------------------------------------------------
	// color
	
	float4 color = tex2D(colorSampler, In.Tex);
	
	//----------------------------------------------------------
	// tiling
	
	float2 Tile = frac( In.Tex * map_detailsize_ground_var );
	
	//----------------------------------------------------------
	// tile maps
	
	float4 color1 = tex2D(tileSampler1, Tile);	
	
	//----------------------------------------------------------	
	// texture
	
   float4 Color = 1;
   
	Color.rgb = (color.rgb + color1.rgb) - 0.5;														// ffe compatible
	
	//----------------------------------------------------------
   // shadowmapping
   
   float shadow1 = PS_shadowmapping(In.Shadow, scsm_esm_var);
   
   //----------------------------------------------------------
   // normalize input vectors
   
   In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal = normalize(In.WorldNormal);
   
   //----------------------------------------------------------
   // lighting
   
	float4 Specular = 0;
   float4 Lights = 0;
   
   if (In.ViewDistance < (sky_loddistance3_var*2.0f + 2000.0f))
		{
		   float LightIntensity = saturate(dot(In.WorldNormal, -In.SunDir));
		      
			// specular										
			if (LightIntensity > 0.25f)
				{
					float3 Halfway  = saturate(dot(normalize(In.ViewDir - In.SunDir), In.WorldNormal));			
					Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor * vecLight;					
					
		//			// spec mapping
		//			Specular = min( Specular , SpecularMap ); 																						// or Specular * SpecularMap
				}
			// specular - another calculation
		//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - In.SunDir);													// R
		//	Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor * vecLight;			// R.V^n 
								
			//----------------------------------------------------------
			// dynamic lights						
{}
#ifdef PER_PIXEL_LIGHTS	

			{
			// per pixel dynamic lights			
			for (int i=1; i<=iLights; i++)
				Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
			Lights *= 2.5 * vecDiffuse;																			// enbrighten to match vertex light look
			}
	#ifdef DEBUG_TILE																
				if (play02_bmap_ready_var > 0)
					{
						if (play02_debugcluster_var < 1)
							{
								Color.rgb *= DebugTerrainClearance(In.WorldPos.xzy, In.WorldNormal);			// tile clearance data
							}
						else
							{
								Color.rgb *= DebugTerrainClusters(In.WorldPos.xzy, In.WorldNormal);			// cluster area pattern
							}
					}
				else
					{
						Color.rgb *= DebugTerrain (In.WorldPos.xz);											// tile pattern
					}
	#endif // DEBUG_TILE	
#endif	
			
			// transition problem
			if (In.ViewDistance >= sky_loddistance3_var*2.0f)
				{
					float ratio = saturate( (In.ViewDistance - sky_loddistance3_var*2.0f) / 2000.0f );
					Specular = lerp(Specular, In.Specular, ratio);
					Lights = lerp(Lights, In.Color, ratio);
				}	
																												
		}												
	else
		{
			Specular = In.Specular;
			Lights = In.Color;
		}
	
	//----------------------------------------------------------
	// add lighting to color

{}
#ifdef PER_PIXEL_LIGHTS	
	Color.rgb *= (Specular + Lights);					
#else
	Color.rgb *= (Specular + In.Color);	
#endif	
	
	//----------------------------------------------------------
	// add shadows
	
	Color.rgb *= clamp(shadow1, 1-scsm_run_var, 1);													// dynamic shadows, add if loop runs
	
	//----------------------------------------------------------	
	// lightmap = static shadowmap = 0.5 * lightmap
	
	float4 Lightmap = 2.0 * tex2D(LightMapSampler, In.Tex);										// 1.0 * for L3DT lightmap, 2.0 * for 3dgs lightmap
	Color.rgb *= clamp(Lightmap, scsm_lightmapalpha_var, 1);
		
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);												// sm 3.0 fog support	
	
	//----------------------------------------------------------
	
	return float4(Color.rgb,1);
}


// without lightmap
float4 Terrain1PS(inTerrainPS In) : COLOR0
{
	//----------------------------------------------------------
	// color
	
	float4 color = tex2D(colorSampler, In.Tex);
	
	//----------------------------------------------------------
	// tiling
	
	float2 Tile = frac( In.Tex * map_detailsize_ground_var );
	
	//----------------------------------------------------------
	// tile maps
	
	float4 color1 = tex2D(tileSampler1, Tile);	
	
	//----------------------------------------------------------	
	// texture
	
   float4 Color = 1;
   
	Color.rgb = (color.rgb + color1.rgb) - 0.5;														// ffe compatible
	
	//----------------------------------------------------------
   // shadowmapping
   
   float shadow1 = PS_shadowmapping(In.Shadow, scsm_esm_var);
   
   //----------------------------------------------------------
   // normalize input vectors
   
   In.SunDir 		= normalize(In.SunDir);
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal = normalize(In.WorldNormal);
   
   //----------------------------------------------------------
   // lighting
   
	float4 Specular = 0;
   float4 Lights = 0;
   
   if (In.ViewDistance < (sky_loddistance3_var*2.0f + 2000.0f))
		{
		   float LightIntensity = saturate(dot(In.WorldNormal, -In.SunDir));
		   			
			// specular										
			if (LightIntensity > 0.25f)
				{
					float3 Halfway  = saturate(dot(normalize(In.ViewDir - In.SunDir), In.WorldNormal));			
					Specular = vecSpecular * pow( dot(In.WorldNormal, 	Halfway), fPower) * vecSunColor * vecLight;					
					
		//			// spec mapping
		//			Specular = min( Specular , SpecularMap ); 																						// or Specular * SpecularMap
				}
			// specular - another calculation
		//	float3 Reflect = normalize(2 * LightIntensity * In.WorldNormal - In.SunDir);													// R
		//	Specular = vecSpecular * pow(saturate(dot(Reflect, In.ViewDir)), fPower) * vecSunColor * vecLight;			// R.V^n 
						
			//----------------------------------------------------------
			// dynamic lights						
{}
#ifdef PER_PIXEL_LIGHTS	

			{
			// per pixel dynamic lights			
			for (int i=1; i<=iLights; i++)
				Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
			Lights *= 2.5 * vecDiffuse;																			// enbrighten to match vertex light look
			}
	#ifdef DEBUG_TILE																
				if (play02_bmap_ready_var > 0)
					{
						if (play02_debugcluster_var < 1)
							{
								Color.rgb *= DebugTerrainClearance(In.WorldPos.xzy, In.WorldNormal);			// tile clearance data
							}
						else
							{
								Color.rgb *= DebugTerrainClusters(In.WorldPos.xzy, In.WorldNormal);			// cluster area pattern
							}
					}
				else
					{
						Color.rgb *= DebugTerrain (In.WorldPos.xz);											// tile pattern
					}
	#endif // DEBUG_TILE	
#endif	
			
			// transition problem
			if (In.ViewDistance >= sky_loddistance3_var*2.0f)
				{
					float ratio = saturate( (In.ViewDistance - sky_loddistance3_var*2.0f) / 2000.0f );
					Specular = lerp(Specular, In.Specular, ratio);
					Lights = lerp(Lights, In.Color, ratio);
				}	
																												
		}												
	else
		{
			Specular = In.Specular;
			Lights = In.Color;
		}

	//----------------------------------------------------------
	// add lighting to color

{}
#ifdef PER_PIXEL_LIGHTS	
	Color.rgb *= ( Specular + Lights);					
#else
	Color.rgb *= ( Specular + In.Color);	
#endif	
	
	//----------------------------------------------------------
	// add shadows
	
	Color.rgb *= clamp(shadow1, 1-scsm_run_var, 1);													// dynamic shadows, add if loop runs
	
	//----------------------------------------------------------	
//	// lightmap = static shadowmap = 0.5 * lightmap
//	
//	float4 Lightmap = 2.0 * tex2D(LightMapSampler, In.Tex);										// 1.0 * for L3DT lightmap, 2.0 * for 3dgs lightmap
//	Color.rgb *= clamp(Lightmap, scsm_lightmapalpha_var, 1);
//		
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);												// sm 3.0 fog support	
	
	//----------------------------------------------------------
	
	return float4(Color.rgb,1);
}

////////////////////////////////////////////////////////////////////////////////////////////////
// no _lodx and _lmx cannot be utilized

technique terrain
{
	pass p0
	{
//		cullmode = none;
		
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
		
		VertexShader = compile vs_3_0 TerrainVS();
		PixelShader  = compile ps_3_0 Terrain1PS();
	}
}

//-------------------------------------------------------

technique terrain_lm
{
	pass p0
	{
//		cullmode = none;
		
		AlphaTestEnable 	= False;
	   AlphaBlendEnable 	= False;
		
		VertexShader = compile vs_3_0 TerrainVS();
		PixelShader  = compile ps_3_0 Terrain1LmPS();
	}
}