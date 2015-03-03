
bool AUTORELOAD;

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <lights>
#include <texture>
#include <color>
#include <vecskill>

///////////////////////////////////////////////////////////////////////////////////////////////////

float4 vecTime;

///////////////////////////////////////////////////////////////////////////////////////////////////

struct vsOut
{
	float4 Pos		:	POSITION;
	float  Fog		:	FOG;		
	float4 Ambient	:	COLOR;
	float2 Tex		:	TEXCOORD0;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// helpers

float4 SmoothCurve( float4 x ) 
{  
  return x * x *( 3.0 - 2.0 * x );  
}  

float4 TriangleWave( float4 x )
{  
  return abs( frac( x + 0.5 ) * 2.0 - 1.0 );  
}
  
float4 SmoothTriangleWave( float4 x )
{  
  return SmoothCurve( TriangleWave( x ) );  
}  

///////////////////////////////////////////////////////////////////////////////////////////////////

vsOut animGrass_VS1(
						   in float4 inPos		: 	POSITION, 
						   in float3 inNormal	:	NORMAL,
						   in float2 inTex		: 	TEXCOORD0
						)
{
	vsOut Out;
	
	//-------------------------------------------------------------------------------------
   
	Out.Tex	= DoTexture(inTex);	
	
	//-------------------------------------------------------------------------------------
	
	float3 P = DoPos(inPos);
	float3 N = DoNormal(inNormal);
	
	//---------------------------------------------------------------   
   
   // fine-oscillate 
	// Phases (object, vertex, branch)  
   float fBranchPhase = inPos.x; //1;
   float fDetailPhase = inPos.y;	//1;
      
   float fObjPhase = dot(P.xyz, 1);  
	fBranchPhase += fObjPhase;  
	
	float fVtxPhase = dot(inPos.xyz, fDetailPhase + fBranchPhase);  
	
	// x is used for edges; y is used for branches  
   float2 vWavesIn = vecTime.w + float2(fVtxPhase, fBranchPhase );  
   
	// 1.975, 0.793, 0.375, 0.193 are good frequencies  
	float fSpeed = 0.10;
	float fDetailFreq = 0.10;
   float4 vWaves = (frac( vWavesIn.xxyy * float4(1.975, 0.793, 0.375, 0.193) ) * 2.0 - 1.0 ) * fSpeed * fDetailFreq;
   
	vWaves = SmoothTriangleWave( vWaves );  
	
	float2 vWavesSum = vWaves.xz + vWaves.yw;  
	
	// Edge (xy) and branch bending (z)  
	float fEdgeAtten = 0.25;
	float fDetailAmp = 5;
	float fBranchAtten = 1.0;
	float fBranchAmp = 5;
	inPos.xyz += vWavesSum.xxy * float3(fEdgeAtten * fDetailAmp * inNormal.xy, fBranchAtten * fBranchAmp);  		
   
	//-------------------------------------------------------------------------------------
   
	float force_x = 0.20;	//DoDefault(vecSkill41.x * (0.1 / 50), 0.1); 
	float force_y = 0.20;	//DoDefault(vecSkill41.y * (0.1 / 50), 0.1);
	
	float speed = sin((vecTime.w + 0.2 * (P.x + P.y + P.z)) * 0.025 );	// * DoDefault(vecSkill41.z * (0.05 / 50), 0.05));
	
	// * (1 - Out.Tex.y) to move only upper vertices
	inPos.x += speed * force_x * inPos.y * (1 - Out.Tex.y);
	inPos.z += speed * force_y * inPos.y * (1 - Out.Tex.y);
	inPos.y -= 0.1 * abs(speed * (force_x + force_y)) * inPos.y * (1 - Out.Tex.y);
	
	//-------------------------------------------------------------------------------------
	
	Out.Pos = DoTransform(inPos);
   
   //-------------------------------------------------------------------------------------
   
   Out.Fog	= DoFog(inPos);
   
   //-------------------------------------------------------------------------------------
   
   Out.Ambient = DoAmbient();	
	
	for (int i=1; i<=iLights; i++)  
		Out.Ambient.xyz += DoLight(P, N, i-1);		
	
	Out.Ambient.xyz *= vecDiffuse;
	
	//-------------------------------------------------------------------------------------
	
	return Out;		
}

///////////////////////////////////////////////////////////////////////////////////////////////////

vsOut animGrass_VS2(
						   in float4 inPos		: 	POSITION, 
						   in float3 inNormal	:	NORMAL,
						   in float2 inTex		: 	TEXCOORD0
						)
{
	vsOut Out;
	
	//-------------------------------------------------------------------------------------
   
	Out.Tex	= DoTexture(inTex);	
	
	//-------------------------------------------------------------------------------------
	
	float3 P = DoPos(inPos);
	float3 N = DoNormal(inNormal);
	
	//-------------------------------------------------------------------------------------
   
	float force_x = 0.20;	//DoDefault(vecSkill41.x * (0.1 / 50), 0.1); 
	float force_y = 0.20;	//DoDefault(vecSkill41.y * (0.1 / 50), 0.1);
	
	float speed = sin((vecTime.w + 0.2 * (P.x + P.y + P.z)) * 0.025 );	// * DoDefault(vecSkill41.z * (0.05 / 50), 0.05));
	
	// * (1 - Out.Tex.y) to move only upper vertices
	inPos.x += speed * force_x * inPos.y * (1 - Out.Tex.y);
	inPos.z += speed * force_y * inPos.y * (1 - Out.Tex.y);
	inPos.y -= 0.1 * abs(speed * (force_x + force_y)) * inPos.y * (1 - Out.Tex.y);
	
	//-------------------------------------------------------------------------------------
	
	Out.Pos = DoTransform(inPos);
   
   //-------------------------------------------------------------------------------------
   
   Out.Fog	= DoFog(inPos);
   
   //-------------------------------------------------------------------------------------
   
   Out.Ambient = DoAmbient();	
	
	for (int i=1; i<=iLights; i++)  
		Out.Ambient.xyz += DoLight(P, N, i-1);		
	
	Out.Ambient.xyz *= vecDiffuse;	
	
	//-------------------------------------------------------------------------------------
	
	return Out;		
}

///////////////////////////////////////////////////////////////////////////////////////////////////

vsOut animGrass_VS3(
						   in float4 inPos		: 	POSITION, 
						   in float3 inNormal	:	NORMAL,
						   in float2 inTex		: 	TEXCOORD0
						)
{
	vsOut Out;
	
	//-------------------------------------------------------------------------------------
   
	Out.Tex	= DoTexture(inTex);	
	
	//-------------------------------------------------------------------------------------
	
	float3 P = DoPos(inPos);
	float3 N = DoNormal(inNormal);
	
	//-------------------------------------------------------------------------------------
	
	Out.Pos = DoTransform(inPos);
   
   //-------------------------------------------------------------------------------------
   
   Out.Fog	= DoFog(inPos);
   
   //-------------------------------------------------------------------------------------
   
   Out.Ambient = DoAmbient();	
	
	for (int i=1; i<=iLights; i++)  
		Out.Ambient.xyz += DoLight(P, N, i-1);		
	
	Out.Ambient.xyz *= vecDiffuse;
	
	//-------------------------------------------------------------------------------------
	
	return Out;		
}

///////////////////////////////////////////////////////////////////////////////////////////////////

technique animGrass
{
	pass 
		{
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;	
		    
			VertexShader = compile vs_2_0 animGrass_VS1(); 
		}
}

technique animGrass_lod1
{
	pass 
		{ 
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;
		   
		   VertexShader = compile vs_2_0 animGrass_VS3(); 
		}
}

technique animGrass_lod2
{
	pass 
		{
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;
		   
		   VertexShader = compile vs_2_0 animGrass_VS3(); 
		}
}

technique animGrass_lod3 
{ 
	pass 
		{
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;
		   
		   VertexShader = compile vs_2_0 animGrass_VS3(); 
		} 
}

///////////////////////////////////////////////////////////////////////////////////////////////////

technique fallback 
{ 
	pass 
		{ 
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;	
		} 
}