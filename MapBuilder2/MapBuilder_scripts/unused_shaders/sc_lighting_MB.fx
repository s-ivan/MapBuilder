
///////////////////////////////////////////////////////////////////////////////////////////
// varialbles

float4 vecLightPos[8];
float4 vecLightColor[8];
//float4 vecLightDir[8];			// normalized vector, only for directional lights, otherwise nullvector (.w == 0)

int iLights;

///////////////////////////////////////////////////////////////////////////////////////////
// a different approach (vecDiffuse and diffuse light weighting applied inside the functions)

////////////////////////////////
// diffuse

float4 PointLightDiffuse(float3 WorldPos, float3 WorldNormal, int i) 
{
	float4 LightColor			= float4(vecLightColor[i].rgb, 1.0f);
	
	float  LightRange			= vecLightPos[i].w;
	float3 LightDirection 	= WorldPos.xyz - vecLightPos[i].xyz;
	float  LightDistance		= length(LightDirection);
	LightDirection 			= normalize(LightDirection);
	
	float LightIntensity		= saturate(dot(WorldNormal, -LightDirection));						// input WorldNormal should be normalized
	float4 Diffuse				= 8.0 * vecDiffuse * LightIntensity * LightColor;
	
//	if ((LightIntensity >= 0.f) && (LightRange > 0.f)) 										// LightRange check can be neglected because of iLights?
//	if (LightIntensity >= 0.f) 
		{
			// diffuse
			
			float LightAttenuation = LightDistance / LightRange;
			
			float LightFactor		= 0.0f;			
			if (LightAttenuation < 1.0f)
				{
					LightFactor = 1.0f - LightAttenuation;
				}
				
			Diffuse *= LightFactor;						
		}
	
	return ( Diffuse * vecLight );
}

////////////////////////////////
// Sun diffuse

float4 SunLightDiffuse(float3 WorldPos, float3 WorldNormal, int i) 
{
	float4 LightColor			= vecSunColor;//float4(vecLightColor[i].rgb, 1.0f);
	
	float  LightRange			= vecLightPos[i].w;
	float3 LightDirection 	= vecSunDir;//WorldPos.xyz - vecLightPos[i].xyz;			
//	float  LightDistance		= length(LightDirection);
//	LightDirection 			= normalize(LightDirection);
	
	float LightIntensity		= saturate(dot(WorldNormal, -LightDirection));							// input WorldNormal should be normalized
	float4 Diffuse				= 8.0f * vecDiffuse * LightIntensity * LightColor;

	return ( Diffuse * vecLight );
}

////////////////////////////////
// diffuse + specular

float4 PointLightDiffuseSpecular(float3 WorldPos, float3 WorldNormal, int i, float Shadow, float3 ViewDir) 
{
	float4 LightColor			= float4(vecLightColor[i].rgb, 1.0f);
	
	float  LightRange			= vecLightPos[i].w;
	float3 LightDirection 	= WorldPos.xyz - vecLightPos[i].xyz;
	float  LightDistance		= length(LightDirection);
	LightDirection 			= normalize(LightDirection);
	
	float LightIntensity		= saturate(dot(WorldNormal, -LightDirection));						// input WorldNormal should be normalized
	float4 Diffuse				= 8.0f * vecDiffuse * LightIntensity * LightColor;

	float4 Specular			= 0.f;
	
//	if ((LightIntensity >= 0.f) && (LightRange > 0.f)) 												// LightRange check can be neglected because of iLights?
//	if (LightIntensity >= 0.f) 
		{
			// diffuse
			
			float LightAttenuation = LightDistance / LightRange;
			
			float LightFactor		= 0.f;			
			if (LightAttenuation < 1.f)
				{
					LightFactor = 1.f - LightAttenuation;
				}
				
			Diffuse *= LightFactor;
			
			// specular
			
			if (LightIntensity * Shadow > 0.50f)																// no specularity in dark areas
				{
					float3 Halfway  = saturate(dot(normalize(ViewDir - LightDirection), WorldNormal));		
					Specular = vecSpecular * pow( dot(WorldNormal, Halfway), fPower) * LightColor;
//					// spec mapping
//					Specular = min( Specular , SpecularMap ); 												// or Specular * SpecularMap					
				}
		}
	
	return ( (Diffuse + Specular) * vecLight );
}

////////////////////////////////
// Sun diffuse + specular

float4 SunLightDiffuseSpecular(float3 WorldPos, float3 WorldNormal, int i, float Shadow, float3 ViewDir) 
{
	float4 LightColor			= vecSunColor;//float4(vecLightColor[i].rgb, 1.0f);
	
	float  LightRange			= vecLightPos[i].w;
	float3 LightDirection 	= vecSunDir;//WorldPos.xyz - vecLightPos[i].xyz;			
//	float  LightDistance		= length(LightDirection);
//	LightDirection 			= normalize(LightDirection);
	
	float LightIntensity		= saturate(dot(WorldNormal, -LightDirection));							// input WorldNormal should be normalized
	float4 Diffuse				= 8.0f * vecDiffuse * LightIntensity * LightColor;

	float4 Specular			= 0.f;
		
	// specular
	
	if (LightIntensity * Shadow > 0.50f)																		// no Sun specularity in dark areas
		{
			float3 Halfway  	= saturate(dot(normalize(ViewDir - LightDirection), WorldNormal));		
			Specular 			= vecSpecular * pow( dot(WorldNormal, Halfway), fPower) * LightColor;
//			// spec mapping
//			Specular 			= min( Specular , SpecularMap ); 											// or Specular * SpecularMap					
		}
	
	return ( (Diffuse + Specular) * vecLight );
}
