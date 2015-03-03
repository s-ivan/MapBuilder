
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
		      
			VertexShader = compile vs_2_0 animTree_VS2(); 
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