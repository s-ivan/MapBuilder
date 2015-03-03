
bool AUTORELOAD;

///////////////////////////////////////////////////////////////////////////////////////////////////

float4 vecTime;

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

vsOut animTree_VS1(
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
   
   // fine-oscillate only outer parts
	if ((abs(inPos.x) > 32) && (abs(inPos.z) > 32))
   {
   	// Phases (object, vertex, branch)  
	   float fBranchPhase = inPos.x; //1;
	   float fDetailPhase = inPos.y;	//1;
	      
	   float fObjPhase = dot(P.xyz, 1);  
		fBranchPhase += fObjPhase;  
		
		float fVtxPhase = dot(inPos.xyz, fDetailPhase + fBranchPhase);  
		
		// x is used for edges; y is used for branches  
	   float2 vWavesIn = vecTime.w + float2(fVtxPhase, fBranchPhase );  
	   
		// 1.975, 0.793, 0.375, 0.193 are good frequencies  
		float fSpeed = 0.15;
		float fDetailFreq = 0.15;
	   float4 vWaves = (frac( vWavesIn.xxyy * float4(1.975, 0.793, 0.375, 0.193) ) * 2.0 - 1.0 ) * fSpeed * fDetailFreq;
	   
		vWaves = SmoothTriangleWave( vWaves );  
		
		float2 vWavesSum = vWaves.xz + vWaves.yw;  
		
		// Edge (xy) and branch bending (z)  
		float fEdgeAtten = 0.25;
		float fDetailAmp = 10;
		float fBranchAtten = 5.0;
		float fBranchAmp = 10;
		inPos.xyz += vWavesSum.xxy * float3(fEdgeAtten * fDetailAmp * inNormal.xy, fBranchAtten * fBranchAmp);  		
	}
   
	//-------------------------------------------------------------------------------------
	
	// move only upper part of tree - value is read from skill44
	if (inPos.y > vecSkill41.w ) 
		{
			float force_x = 0.025;	//DoDefault(vecSkill41.x*(0.1/50),0.1); 
			float force_y = 0.025;	//DoDefault(vecSkill41.y*(0.1/50),0.1);
			
			float speed = sin( (vecTime.w + 0.2 * (P.x + P.y + P.z)) * 0.020);	//DoDefault(vecSkill41.z*(0.05/50),0.05)
				
			inPos.x += speed * force_x * inPos.y;
			inPos.z += speed * force_y * inPos.y;
			inPos.y -= 0.1 * abs(speed * (force_x + force_y)) * inPos.y;
		}
	
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

vsOut animTree_VS2(
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
	
	// move only upper part of tree - value is read from skill44
	if (inPos.y > vecSkill41.w ) 
		{
			float force_x = 0.025;	//DoDefault(vecSkill41.x*(0.1/50),0.1); 
			float force_y = 0.025;	//DoDefault(vecSkill41.y*(0.1/50),0.1);
			
			float speed = sin( (vecTime.w + 0.2 * (P.x + P.y + P.z)) * 0.020);	//DoDefault(vecSkill41.z*(0.05/50),0.05)
				
			inPos.x += speed * force_x * inPos.y;
			inPos.z += speed * force_y * inPos.y;
			inPos.y -= 0.1 * abs(speed * (force_x + force_y)) * inPos.y;
		}
	
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

vsOut animTree_VS3(
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

technique animTree
{
	pass 
		{
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;		
		      
			VertexShader = compile vs_2_0 animTree_VS1(); 
		}
}

technique animTree_lod1
{
	pass 
		{
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;	
		   	   
		   VertexShader = compile vs_2_0 animTree_VS2(); 
		}
}

technique animTree_lod2
{
	pass 
		{
			ZWriteEnable 		= True;			
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;	
		   
		   VertexShader = compile vs_2_0 animTree_VS3(); 
		}
}

technique animTree_lod3 
{ 
	pass 
		{
			ZWriteEnable 		= True;			
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;	
		   
		   VertexShader = compile vs_2_0 animTree_VS3(); 
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