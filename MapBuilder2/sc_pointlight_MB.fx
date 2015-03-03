
///////////////////////////////////////////////////////////////////////////////////////////
// varialbles

float4 vecLightPos[8];
float4 vecLightColor[8];
//float4 vecLightDir[8];																				// normalized vector, only for directional lights, otherwise vecLightDir.w == 0

int iLights;

///////////////////////////////////////////////////////////////////////////////////////////
// original approach (no vecDiffuse and diffuse light weighting applied inside the functions)

// diffuse - original version
float4 PointLightDiffuse(float3 WorldPos, float3 WorldNormal, int i) 
{
	float3 LightDir 		= vecLightPos[i].xyz - WorldPos;
	float LightIntensity	= saturate(dot(WorldNormal, normalize(LightDir)));				
	float4 Color 			= vecLightColor[i] * LightIntensity;

	float LightFactor		= 0.f;
	
//	if ((LightIntensity >= 0.f) && (vecLightPos[i].w > 0.f)) 								// vecLightPos.w (range) check can be neglected because of iLights?
//	if (LightIntensity >= 0.f) 																		// alway because of saturate
		{
			float LightAttenuation = length(LightDir) / vecLightPos[i].w;
			
			if (LightAttenuation < 1.f)
				{
					LightFactor = 1.f - LightAttenuation;
				}
		}
	
	return Color * LightFactor;
}

///////////////////////////////////////////////////////////////////////////////////////////
