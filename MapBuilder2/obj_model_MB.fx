
bool AUTORELOAD;

///////////////////////////////////////////////////////////////////////////////////////////////////

#include <transform>
#include <fog>
#include <pos>
#include <normal>
#include <lights>
#include <texture>
#include <color>
//#include <vecskill>

///////////////////////////////////////////////////////////////////////////////////////////////////

//float4 vecTime;

///////////////////////////////////////////////////////////////////////////////////////////////////

struct vsOut
{
	float4 Pos		:	POSITION;
	float  Fog		:	FOG;		
	float4 Ambient	:	COLOR;
	float2 Tex		:	TEXCOORD0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

vsOut Model_VS(
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
	
	for (int i=1; i<iLights; i++)  
		Out.Ambient.xyz += DoLight(P, N, i-1);		
	
	Out.Ambient.xyz += 0.75 * DoLight(P, N, iLights-1);												// Sun
	
	Out.Ambient.xyz *= vecDiffuse;
	
   //-------------------------------------------------------------------------------------   
   
	return Out;		
}

///////////////////////////////////////////////////////////////////////////////////////////////////

technique model
{
	pass 
		{
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;
		    
			VertexShader = compile vs_2_0 Model_VS(); 
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