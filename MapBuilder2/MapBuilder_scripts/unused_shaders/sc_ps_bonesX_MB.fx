
//////////////////////////////////////////////////////////////////////////////////////////////////
// pixel shader

float4 ModelPS(inPS In): COLOR
{
	//----------------------------------------------------------
	// shadowmapping
   
   float shadow1 = PS_shadowmapping_Fast(In.Shadow, scsm_esm_obj_var);
    
	//----------------------------------------------------------
   // lighting
   
   In.ViewDir 		= normalize(In.ViewDir);
   In.WorldNormal = normalize(In.WorldNormal);
   
   // ambient
   float4 Ambient = vecAmbient * vecLight;																															
   
   // diffuse   
   float4 Diffuse = 2.0 * vecDiffuse * dot(In.WorldNormal, -In.SunDir) * vecSunColor * vecLight;									
	
//	// specular
//	float3 Halfway  = saturate(dot(normalize(-In.SunDir + In.ViewDir), In.WorldNormal));
//	float4 Specular = vecSpecular * pow( dot(In.WorldNormal, Halfway) , fPower) * vecSunColor * vecLight;										
//	
//	// emissive
//	float4 Emissive = vecEmissive;																		
	
	//----------------------------------------------------------
	// texture - keep its alpha afterwards
	
   float4 Color = tex2D(colorSampler, In.Tex);										
   
   //----------------------------------------------------------
   // dynamic lights
{}
#ifdef PER_PIXEL_LIGHTS	
	// per pixel dynamic lights
	float4 Lights = 0;
	for (int i=1; i<=iLights; i++)
		Lights += PointLightDiffuse(In.WorldPos, In.WorldNormal, i-1);
	Lights *= 2.0 * vecDiffuse;																			// enbrighten to match vertex light look
	
	#ifdef DEBUG_TILE																
		if (play02_bmap_ready_var > 0)
			{
				if (play02_debugcluster_var < 1)
					{
						Color.rgb *= DebugTerrainClearance(In.WorldPos.xzy, In.WorldNormal);	// tile clearance data
					}
				else
					{
						Color.rgb *= DebugTerrainClusters(In.WorldPos.xzy, In.WorldNormal);	// cluster area pattern
					}
			}
		else
			{
				Color.rgb *= DebugTerrain (In.WorldPos.xz);											// tile pattern
			}
	#endif // DEBUG_TILE
	
#endif	
	
	//----------------------------------------------------------
	// add lighting to color
	
#ifdef PER_PIXEL_LIGHTS	
	Color.rgb *= (Ambient + Diffuse + /*Specular + Emissive +*/ Lights + In.Color);			// with per pixel dynamic lights, InColor contains only entity ambient fAmbient
#else
	Color.rgb *= (Ambient + Diffuse + /*Specular + Emissive +*/ In.Color);						// with per vertex dynamic lights
#endif

	//----------------------------------------------------------
	// add shadows
	
	Color.rgb *= clamp(shadow1, 1-scsm_run_var, 1);													// dynamic shadows, add if loop runs
	
	//----------------------------------------------------------
	// add fog
	
	Color.rgb = lerp(Color.rgb, vecFogColor, In.Fog);												// sm 3.0 fog support	
	
	//----------------------------------------------------------
	
	return Color;
}