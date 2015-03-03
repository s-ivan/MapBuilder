
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

///////////////////////////////////////////////////////////////////////////////////////////////////

vsOut animGrass_VS(
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
	
	float force_x = 0.15;	//DoDefault(vecSkill41.x*(0.1/50),0.1); 
	float force_y = 0.15;	//DoDefault(vecSkill41.y*(0.1/50),0.1);
	
	float speed = sin((vecTime.w + 0.2 * (P.x + P.y + P.z)) * 0.030);	// * DoDefault(vecSkill41.z*(0.05/50),0.05));
	
	// * (1 - Out.Tex.y) to move only upper vertices
	inPos.x += speed * force_x * inPos.y * (1 - Out.Tex.y);
	inPos.z += speed * force_y * inPos.y * (1 - Out.Tex.y);
	inPos.y -= 0.1 * abs(speed * (force_x + force_y)) * inPos.y * (1 - Out.Tex.y);
		
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
		    
			VertexShader = compile vs_2_0 animGrass_VS(); 
		}
}

technique animGrass_lod1
{
	pass 
		{ 
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;
		   
		   VertexShader = compile vs_2_0 animGrass_VS2(); 
		}
}

technique animGrass_lod2
{
	pass 
		{
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;
		}
}

technique animGrass_lod3 
{ 
	pass 
		{
			ZWriteEnable 		= True;		
			AlphaTestEnable 	= True;
		   AlphaBlendEnable 	= False;
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