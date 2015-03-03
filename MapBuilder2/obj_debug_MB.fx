
bool AUTORELOAD;

////////////////////////////////////////////////////////////////////////////////////////////////
// defines

////////////////////////////////////////////////////////////////////////////////////////////////
// matrices, vectors, variables passed by the engine

float4x4 matWorldViewProj;
//float4x4 matWorldInv;
//float4x4 matViewInv;
float4x4 matWorld;

//float4 vecFog;
//float3 vecFogColor;	
//float fog_color_var;

//float4 vecViewPos;
//float4 vecViewDir;

//float4 vecLightPos[8];
//float4 vecLightColor[8];

//float4 vecSunColor;
//float4 vecSunDir;	

//int iLights;

////////////////////////////////////////////////////////////////////////////////////////////////
// custom matrices, vectors, variables

//float scsm_lightmapalpha_var		= 0.50f; 		//shadowalpha of shadowtexture		lightmap shadow darkness
//
//float map_detailsize_ground_var 	= 40.0f;			// like detail_size
//float map_blendfactor_ground_var	= 0.5f;			// tile blending strength over base colormap

//////////////////////////////////////////////////////////////////////////////////////////////////
// structs

struct outVS 
{
	float4 Color			: COLOR0; 
   float4 Pos				: POSITION; 
   float2 Tex				: TEXCOORD0; 
//	float3 ViewDir			: TEXCOORD2;
   float3 WorldNormal	: TEXCOORD3;
	float3 WorldPos		: TEXCOORD6;	// needed only for ps per pixel dynamic lights
//	float4 Shadow			: TEXCOORD7;
//   float  Fog 				: FOG;
};


struct inPS 
{
	float4 Color			: COLOR0;  
   float2 Tex				: TEXCOORD0; 
//	float3 ViewDir			: TEXCOORD2;
   float3 WorldNormal	: TEXCOORD3;
	float3 WorldPos		: TEXCOORD6;	// needed only for ps per pixel dynamic lights
//	float4 Shadow			: TEXCOORD7;
//	float  Fog 				: FOG;
};

////////////////////////////////////////////////////////////////////////////////////////////////
// includes
	
	#include <color>
	#include <lights>
	
//	#include "sc_shadowmapping_MB.fx"		// scsm shadowmapping - variables, shadow samplers, vertex shader functions, pixel shader function
	
	#include "rts_tiledarea_MB.fx"

//////////////////////////////////////////////////////////////////////////////////////////////////
// vertex shader

outVS DebugVS
	( 
   float4 InPos		: POSITION, 
   float3 InNormal	: NORMAL, 
   float2 InTex		: TEXCOORD0
	) 
{ 
   outVS Out;
	
	//----------------------------------------------------------------
	
	// Pass the texture coordinate to the pixel shader
   Out.Tex = InTex;
   
   //---------------------------------------------------------------
   
   float4 PosWorld = mul(InPos, matWorld);											// needed for a couple of calculations... float4 for shadowmapping!
   
   //----------------------------------------------------------------
	
	Out.Pos = mul(InPos, matWorldViewProj); 
	
	//----------------------------------------------------------------

//	Out.WorldPos = mul(InPos, matWorld).xyz;											// recalculate due to modified position
	Out.WorldPos = PosWorld.xyz;
	
	//----------------------------------------------------------------
	
//	Out.ViewDir = vecViewPos - PosWorld;												// for flat specular - or = matViewInv[3].xyz - PosWorld;	= halfway vector
	
	//----------------------------------------------------------------
	
	Out.WorldNormal = normalize(mul(InNormal, (float3x3)matWorld));			// when no normalmap used - normalize because of entity scale!
   
   //----------------------------------------------------------------
   
//	Out.Fog = saturate(fog_color_var)
//				 * saturate( (distance(PosWorld, vecViewPos) - vecFog.x)
//				 * (vecFog.z) );																// saturate needed when fadeout distance is closer than clipping distance
   
   //----------------------------------------------------------------
	
//	Out.Shadow = VS_shadowmapping(PosWorld);
	
	//----------------------------------------------------------------
	
   Out.Color = DoAmbient();																// Add ambient and sun light

	for (int i=1; i<=iLights; i++)  														// Add max 8 dynamic lights
		Out.Color += DoLight(PosWorld, Out.WorldNormal, i-1)*vecDiffuse;		// matTangent[2] or OutWorldNormal
   
   //-----------------------------------------------------------------
   
   return Out;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

// without lightmap
float4 DebugPS(inPS In) : COLOR0
{
	float3 Color = In.Color * 0.75;
   
	if (play02_debugcluster_var < 1)
		{
			Color.rgb *= DebugTerrainClearance(In.WorldPos.xzy, In.WorldNormal);			// tile clearance data
		}
	else
		{
			Color.rgb *= DebugTerrainClusters(In.WorldPos.xzy, In.WorldNormal);			// cluster area pattern
		}
	
	//----------------------------------------------------------
	
	return float4(Color.rgb,1);
}

////////////////////////////////////////////////////////////////////////////////////////////////

technique terrain
{
	pass p0
	{
//		cullmode = none;
		
		alphaTestEnable 	= false;
		alphaBlendEnable 	= false;
		
		VertexShader = compile vs_3_0 DebugVS();
		PixelShader  = compile ps_3_0 DebugPS();
	}
}
