///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Water Shader by Wolfgang "BoH_Havoc" Reichardt (Shade-C 0.91 package), modified by sivan for MapBuilder editor
//
// Entity Textures:
//	Skin1 = Normalmap
//	Skin2 = Foam
//	Skin3 = Depthmap (not used)
//	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool AUTORELOAD;

//#define VERTEX_LIGHTS			// requires high water terrain vertex resolution, so a bit useless...
// or (never both)
#define PER_PIXEL_LIGHTS		// supports 8 lights, requires SM3.0 because of instruction quantity, thus modified fog for SM3.0

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// matrices, vectors, variables passed by the engine

float4x4 matWorldViewProj;
float4x4 matWorld;

float4x4 matView;			//***
float3 vecViewPos;

float4 vecSunPos;
float4 vecSunColor;
float3 vecSunDir;

float4 vecFog;				
float4 vecFogColor;	

float4 vecLight;
float4 vecColor;
float4 vecAmbient;
float4 vecDiffuse;
float4 vecSpecular;
float4 vecEmissive;
float  fPower;

float4 vecTime;
float mtlSkill1;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes

#include "sc_defines_MB.fx"
#include "sc_pointlight_MB.fx"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float scwater_waterdepth_var;														// max depth used for depthmap

float scwater_wavespeed_var 					= 7500;			// 6000;		// bump and foam texture position change, reverse ratio	1000..10000
float scwater_displacement_var 				= 64.0f; 		// 92.0f;	// of vertex height moves		0..320
float scwater_speed_var 						= 0.02f;			// 0.06f;	// of vertex height moves		0.000..0.100
float scwater_turbdensity_var 				= 0.25f;			// 0.27f;	// of vertex height moves and color?	0..1
					
float4 scwater_reflectcolor_var 				= {0.5, 0.6, 0.7, 0.8};			// 0..1 [4]					
float4 scwater_refractcolor_var 				= {0.6, 0.7, 0.8, 0.9};			// 0..1 [4]
float4 scwater_shorecolor_var					= {0.9, 1.0, 0.95, 1.0};

float scwater_reflectiondistance_var 		= 1.0;			// affects freshnel 0..10	(the higher the farer reflection start) squared distance
float scwater_globaltransparency_var 		= 0.25;			// general transparency 0..1, 1 transparent
float scwater_reflectionstrength_var 		= 0.50; 			// 0..1		over 1 it is uselessly strong
float scwater_refractionstrength_var 		= 0.25;			// 0..1		over 1 it is uselessly strong				//*** new

float scwater_foamtiling_var 					= 25;				// 1..50
float scwater_foamdistortion_var 			= 0.15;			// 0..0.5
float scwater_foamtranslation_var 			= 1.0;			// 0..10
float scwater_shorefoamscale_var 			= 0.25;			// 0..1		0 transparent at shore
float scwater_foamscale_var 					= 0.33;			// 0..1		0 transparent at deep water
float scwater_foamwave_deep_var				= 0.25;			// 0..1
float scwater_foamwave_shore_var				= 0.25;			// 0..1

float scwater_sunlightstrength_var 			= 2.0;			// brightness 0..10
float scwater_sunlightarea_var 				= 250;			// area size 10..1000	// reverse ratio

float scwater_bumpstrength_var				= 4.0;			// bump strength 0..10
float scwater_bumptexscale_var 				= 10.0;			// bump map size 0..50
float scwater_2ndwave_speed_var				= 1.33;			// 0.5..2.0
float scwater_2ndwave_scale_var				= 0.5;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

texture entSkin1; //normalmap
texture entSkin2; //foam
//texture entSkin3; //static depthmap (optional)

texture mtlSkin1;
texture mtlSkin2;
texture mtlSkin3;
texture mtlSkin4;

//-------------------------------------------------

sampler normalmapSampler = sampler_state
{
	Texture = <entSkin1>; 
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;   
	AddressU  = mirror;
	AddressV  = mirror;
};

sampler foamSampler = sampler_state
{
	Texture = <entSkin2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;   
	AddressU  = wrap;
	AddressV  = wrap;
};

//sampler waveSampler = sampler_state
//{
//	texture = <entSkin3>;
//	MinFilter = Linear;
//	MagFilter = Linear;
//	MipFilter = Linear;
//	AddressU  = clamp;
//	AddressV  = clamp;
//};

//-------------------------------------------------

sampler reflectionSampler = sampler_state
{
	texture = <mtlSkin1>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU  = clamp;
	AddressV  = clamp;
};

sampler refractionSampler = sampler_state
{
	texture = <mtlSkin2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU  = clamp;
	AddressV  = clamp;
};

sampler depthSampler = sampler_state
{
	texture = <mtlSkin3>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU  = clamp;
	AddressV  = clamp;
};

sampler fresnelSampler = sampler_state
{
	Texture = <mtlSkin4>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;   
	AddressU  = wrap;
	AddressV  = wrap;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define BSIZE 32
#define FULLSIZE 66
#define NOISEFRAC 0.03125

const float4 NTab[66] =
{
	-0.854611,-0.453029,0.25378,0,
	-0.84528,-0.456307,-0.278002,1,
	-0.427197,0.847095,-0.316122,2,
	0.670266,-0.496104,0.551928,3,
	-0.675674,-0.713842,0.184102,4,
	-0.0373602,-0.600265,0.798928,5,
	-0.939116,-0.119538,0.322135,6,
	0.818521,0.278224,0.502609,7,
	0.105335,-0.765291,0.635007,8,
	-0.634436,-0.298693,0.712933,9,
	-0.532414,-0.603311,-0.593761,10,
	0.411375,0.0976618,0.906219,11,
	0.798824,-0.416379,-0.434175,12,
	-0.691156,0.585681,-0.423415,13,
	0.612298,0.0777332,0.786797,14,
	0.264612,-0.262848,0.927842,15,
	-0.70809,0.0548396,-0.703989,16,
	0.933195,-0.294222,-0.206349,17,
	0.788936,-0.466718,-0.399692,18,
	-0.540183,-0.824413,0.168954,19,
	0.469322,-0.184125,0.863617,20,
	-0.84773,0.292229,-0.44267,21,
	0.450832,0.650314,-0.611427,22,
	0.906378,-0.247125,-0.342647,23,
	-0.995052,0.0271277,-0.0955848,24,
	-0.0252277,-0.778349,0.627325,25,
	0.991428,0.128623,0.0229457,26,
	-0.842581,-0.290688,0.453384,27,
	-0.662511,-0.500545,-0.557256,28,
	0.650245,-0.692099,-0.313338,29,
	0.636901,0.768918,-0.0558766,30,
	-0.437006,0.872104,-0.220138,31,
	-0.854611,-0.453029,0.25378,0,
	-0.84528,-0.456307,-0.278002,1,
	-0.427197,0.847095,-0.316122,2,
	0.670266,-0.496104,0.551928,3,
	-0.675674,-0.713842,0.184102,4,
	-0.0373602,-0.600265,0.798928,5,
	-0.939116,-0.119538,0.322135,6,
	0.818521,0.278224,0.502609,7,
	0.105335,-0.765291,0.635007,8,
	-0.634436,-0.298693,0.712933,9,
	-0.532414,-0.603311,-0.593761,10,
	0.411375,0.0976618,0.906219,11,
	0.798824,-0.416379,-0.434175,12,
	-0.691156,0.585681,-0.423415,13,
	0.612298,0.0777332,0.786797,14,
	0.264612,-0.262848,0.927842,15,
	-0.70809,0.0548396,-0.703989,16,
	0.933195,-0.294222,-0.206349,17,
	0.788936,-0.466718,-0.399692,18,
	-0.540183,-0.824413,0.168954,19,
	0.469322,-0.184125,0.863617,20,
	-0.84773,0.292229,-0.44267,21,
	0.450832,0.650314,-0.611427,22,
	0.906378,-0.247125,-0.342647,23,
	-0.995052,0.0271277,-0.0955848,24,
	-0.0252277,-0.778349,0.627325,25,
	0.991428,0.128623,0.0229457,26,
	-0.842581,-0.290688,0.453384,27,
	-0.662511,-0.500545,-0.557256,28,
	0.650245,-0.692099,-0.313338,29,
	0.636901,0.768918,-0.0558766,30,
	-0.437006,0.872104,-0.220138,31,
	-0.854611,-0.453029,0.25378,0,
	-0.84528,-0.456307,-0.278002,1
};

float4x4 NoiseMatrix = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float3 s_curve(float3 t) 
{ 
	return t*t*( float3(3,3,3) - float3(2,2,2)*t); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float noise(float3 v,
			const uniform float4 pg[FULLSIZE])
{
//	v = v + float3(10000.0f, 10000.0f, 10000.0f);   // hack to avoid negative numbers		// ### must be commented to avoid wave flickering
	
	float3 i = frac(v * NOISEFRAC) * BSIZE;   // index between 0 and BSIZE-1
	float3 f = frac(v);            // fractional position
	
	// lookup in permutation table
	float2 p;
	p.x = pg[ i[0]     ].w;
	p.y = pg[ i[0] + 1 ].w;
	p = p + i[1];
	
	float4 b;
	b.x = pg[ p[0] ].w;
	b.y = pg[ p[1] ].w;
	b.z = pg[ p[0] + 1 ].w;
	b.w = pg[ p[1] + 1 ].w;
	b = b + i[2];
	
	// compute dot products between gradients and vectors
	float4 r;
	r[0] = dot( pg[ b[0] ].xyz, f );
	r[1] = dot( pg[ b[1] ].xyz, f - float3(1.0f, 0.0f, 0.0f) );
	r[2] = dot( pg[ b[2] ].xyz, f - float3(0.0f, 1.0f, 0.0f) );
	r[3] = dot( pg[ b[3] ].xyz, f - float3(1.0f, 1.0f, 0.0f) );
	
	float4 r1;
	r1[0] = dot( pg[ b[0] + 1 ].xyz, f - float3(0.0f, 0.0f, 1.0f) );
	r1[1] = dot( pg[ b[1] + 1 ].xyz, f - float3(1.0f, 0.0f, 1.0f) );
	r1[2] = dot( pg[ b[2] + 1 ].xyz, f - float3(0.0f, 1.0f, 1.0f) );
	r1[3] = dot( pg[ b[3] + 1 ].xyz, f - float3(1.0f, 1.0f, 1.0f) );
	
	// interpolate
	f = s_curve(f);
	r = lerp( r, r1, f[2] );
	r = lerp( r.xyyy, r.zwww, f[1] );
	return lerp( r.x, r.y, f[0] );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void water_VS( 
					in float4 inPos				: POSITION, 	//input vertex position
					in float2 inTex				: TEXCOORD0, 	//input texcoords
					in float3 inNormal			: NORMAL,
					 
				   out float4 outPos				: POSITION, 	//ouput transformed position
				   out float4 outTex				: TEXCOORD0, 	//output texcoords
				   out float4 outProjectedTex	: TEXCOORD1, 	//output projected texcoords
				   out float3 outVertexPos		: TEXCOORD2,	//untrasnformed vertex position	
#ifdef PER_PIXEL_LIGHTS
					out float3 outWorldPos 		: TEXCOORD4,
					out float3 outWorldNormal	: TEXCOORD5,
#endif
				  	out float  outFog				: FOG,
				  	out float4 outLight			: COLOR0
				  ) 
{
	//----------------------------------------------------------------
	
	//animate vertex via noise |generate waves
	float4 noisePos 	= scwater_turbdensity_var * mul(inPos + (scwater_speed_var * vecTime.w), NoiseMatrix);
	float height 		= noise(noisePos.xyz, NTab);
	
	float4 newPos 		= inPos;
	newPos.y 			= inPos.y + height * scwater_displacement_var;				// position modified by waves
	outPos 				= mul(newPos, matWorldViewProj);
	
	//----------------------------------------------------------------
	
	//save the vertex information for further projected texture coordinate computation
	outProjectedTex	= outPos;
	
	//----------------------------------------------------------------
	
	//ouput texcoords
	outTex.xy				= inTex + (vecTime.w / scwater_wavespeed_var * 0.25f);	
	outTex.zw				= inTex + (vecTime.w / scwater_wavespeed_var * 0.25f * scwater_2ndwave_speed_var);	
	
	//----------------------------------------------------------------
	
	outVertexPos		= newPos.xyz;
	
	//----------------------------------------------------------------
	
	//Fog
	float4 PosWorld 	= mul(newPos, matWorld);		  
	
	outFog = vecFogColor.w																								// fog is applied
				* saturate( (distance(PosWorld, vecViewPos) - vecFog.x) * (vecFog.z) );					// saturate needed when fadeout distance is closer than clipping distance
	
	//----------------------------------------------------------------
	
	outLight = (vecAmbient * vecLight) + (vecEmissive * vecColor);						// ambient + emissive
	
	//----------------------------------------------------------------
{}   
#ifdef VERTEX_LIGHTS   
	float3 WorldNormal = normalize(mul(inNormal, (float3x3)matWorld));				// when no normalmap used - normalize because of entity scale!		
	
	for (int i=1; i<iLights; i++)  																// Add max 8 dynamic lights
		outLight += PointLightDiffuse(PosWorld, WorldNormal, i-1);						// dynamic lights
		
	outLight += 2.0 * PointLightDiffuse(PosWorld, WorldNormal, iLights-1);			// Sun
		
	outLight *= 2.0 * vecDiffuse;		
#endif
	
	//----------------------------------------------------------------
	
#ifdef PER_PIXEL_LIGHTS
	outWorldPos 		= PosWorld;	//mul(inPos, matWorld);									// needed for a couple of calculations... float4 for shadowmapping!
	outWorldNormal 	= normalize(mul(inNormal, (float3x3)matWorld));					// when no normalmap used - normalize because of entity scale!
#endif	
	
	//----------------------------------------------------------------
	
#ifndef VERTEX_LIGHTS
	#ifndef PER_PIXEL_LIGHTS
		float3 WorldNormal = normalize(mul(inNormal, (float3x3)matWorld));
		outLight += 4.0 * PointLightDiffuse(PosWorld, WorldNormal, iLights-1);		// Sun
	#endif
#endif
	
	//----------------------------------------------------------------
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float4 water_PS( 
						in float4 inTex				: TEXCOORD0, 		//input texture coords
						in float4 inProjectedTex	: TEXCOORD1, 		//projectedTexcoords, needed to perform reflections, refractions
						in float3 inVertexPos		: TEXCOORD2,		//original vertex position	
#ifdef PER_PIXEL_LIGHTS
						in float3 inWorldPos			: TEXCOORD4,		// only for per pixel dynamic lights
						in float3 inWorldNormal		: TEXCOORD5,		// only for per pixel dynamic lights
#endif					  
						in float4 inLight				: COLOR0,			// only for per vertex dynamic lights						
						in float  inFog				: FOG					// needed by SM3.0 fog		  
					 ):COLOR0 
{
	clip(mtlSkill1);
	
	//----------------------------------------------------------------
	// texture coordinates
	
	// old - better
	inProjectedTex.xy		/= inProjectedTex.w;
	inProjectedTex.xy		= inProjectedTex.xy * 0.5f * float2(1, -1) + 0.5f;
	// new
//	inProjectedTex.x 		= inProjectedTex.x / inProjectedTex.z / 2.0f + 0.5f;
//	inProjectedTex.y 		= -inProjectedTex.y / inProjectedTex.z / 2.0f + 0.5f;
   
   //----------------------------------------------------------------
   // normalmap
   
	float3 pixelNormal 	= tex2D(normalmapSampler, inTex.xy * scwater_bumptexscale_var); 			// do not normalize it !	
	pixelNormal.yz			= pixelNormal.zy;
	pixelNormal				= (pixelNormal.xyz - 0.5f) * 2;
	
	float3 pixelNormal2 	= tex2D(normalmapSampler, inTex.zw * scwater_bumptexscale_var * scwater_2ndwave_scale_var); 
	pixelNormal2.yz		= pixelNormal2.zy;
	pixelNormal2			= (pixelNormal2.xyz - 0.5f) * 2;
	
	pixelNormal 			= lerp(pixelNormal, pixelNormal2, 0.5f);
	
	// bump strength
	pixelNormal.xz			*=	scwater_bumpstrength_var;
	pixelNormal				= normalize(pixelNormal);
	
	//----------------------------------------------------------------
	// water depth changing with wave height dynamically
	
   float waterDepth 		= tex2D(depthSampler, inProjectedTex.xy).g;				// 0..1, where 0 is maxdepth, 1 is water level
   
//	float waveDepth		= inVertexPos.y / sc_waterDepth_var;						// -1..1
//	waterDepth 				-= waveDepth * 0.5;												// max depth should be bigger than wave size! - without *0.5 it can lead to shore transparency at wave bottoms...
//	waterDepth 				= saturate(waterDepth);											// 0 reflection+refraction, 1 shore - it is bad in middle of deep water surface
   
   //----------------------------------------------------------------
   // view textures
   
	float4 reflection		= tex2D(reflectionSampler, inProjectedTex.xy + scwater_reflectionstrength_var * pixelNormal.xz);
	float4 refraction		= tex2D(refractionSampler, inProjectedTex.xy - scwater_refractionstrength_var * pixelNormal.xz);
		
	//----------------------------------------------------------------
	// Sun light to reflect
	
	float3 camToSurface		= inVertexPos - vecViewPos;
	camToSurface				= normalize(-camToSurface);
	
	float3 relfectedVector	= normalize(reflect(-camToSurface, pixelNormal));        
	float3 surfaceToSun		= normalize(vecSunPos - inVertexPos);
	
	float3 sunlight 			= scwater_sunlightstrength_var * pow(saturate(dot(relfectedVector, surfaceToSun)), scwater_sunlightarea_var) * vecSunColor;
	
	//----------------------------------------------------------------
	// fresnel term
	
	float additionalReflection	= camToSurface.x * camToSurface.x + camToSurface.z * camToSurface.z;
   additionalReflection 		/= scwater_reflectiondistance_var;
   
	float dotProduct 				= dot(camToSurface, pixelNormal);
   dotProduct 						= saturate(dotProduct);
	
	float fresnel 					= tex1D(fresnelSampler, dotProduct);
	fresnel 							+= additionalReflection;
	fresnel 							= saturate(fresnel); 
   fresnel 							-= scwater_globaltransparency_var;    
   fresnel 							= saturate(fresnel);
	
	//----------------------------------------------------------------
	// reflection and refraction color
	
	float4 finalColor 	= lerp((refraction + 2.0 * scwater_refractcolor_var)/3.0, (reflection + 2.0 * scwater_reflectcolor_var)/3.0, fresnel); 
	
	//----------------------------------------------------------------
	// Sun reflection area
	
	finalColor.rgb 	+= saturate(sunlight.xyz);														//	Sunlight area				-pow(waterDepth,5) means less bright mear shore
	
	//-----------------------------------------------------------------
	// dynamic lights
{}	
#ifdef PER_PIXEL_LIGHTS	
	inWorldNormal = normalize(inWorldNormal);
	
	// per pixel dynamic lights
	float4 Lights = 0;				
	
	for (int i=1; i<iLights; i++)
		Lights += PointLightDiffuse(inWorldPos, inWorldNormal, i-1);
	
	Lights += 2.0 * PointLightDiffuse(inWorldPos, inWorldNormal, iLights-1);				// Sun
	
	Lights *= 2.0f * vecDiffuse;																
	
	// ambient + emissive + dynamic per pixel lights
	finalColor.rgb *= inLight.rgb + Lights.rgb;
#else	
	// ambient + emissive + dynamic per vertex lights
	finalColor.rgb *= inLight.rgb;	
#endif	
	
	//----------------------------------------------------------------
	// shore depth - moved here because of sun diffuse independency
	
	finalColor		= lerp( finalColor, saturate(scwater_shorecolor_var * refraction), waterDepth);		
	
	//----------------------------------------------------------------		
	// foam texture
	
	float2 foamTex		= inTex.xy * scwater_foamtiling_var + pixelNormal.xz * scwater_foamdistortion_var + float2(vecTime.w * scwater_foamtranslation_var * 0.001, vecTime.w * scwater_foamtranslation_var * 0.001);
	float4 foam    	= tex2D(foamSampler, foamTex);
	
	//----------------------------------------------------------------		
	// foam
		
	float waveHeight	= (inVertexPos.y/scwater_displacement_var + 1.0) * 0.5;												// 0..1 from -1..1, * strength of wave heigth effect 0..1 (not essential, can be set by other parameters like foam brightness, softness and strength)
	
	float waveHeightd = saturate(waveHeight - scwater_foamwave_deep_var);													// deep water wave height dependency
	float waveHeights = saturate(waveHeight - scwater_foamwave_shore_var);													// shore water wave heigth dependency
	
	finalColor.rgb 	+= (foam.xyz * foam.a * waveHeightd)	* (1.0 - waterDepth)	* scwater_foamscale_var;		// * scwater_reflectcolor_var;	// deep foam texture	 	
	finalColor.rgb 	+= (foam.xyz * foam.a * waveHeights)	* (waterDepth)			* scwater_shorefoamscale_var;	// * scwater_refractcolor_var;	// shore foam texture	-pow(waterDepth,5) means less foam mear shore
	
	finalColor.rgb 	=  saturate(finalColor.rgb); 
   
	//------------------------------------------------------------------
	// fog
	
	finalColor.rgb = lerp(finalColor.rgb, vecFogColor, inFog);		
	
	//------------------------------------------------------------------
	
	return finalColor;
};

////////////////////////////////////////////////////////////////////////////////////

technique water
{

	pass p0
	{
		CULLMODE 		= CCW;
		VertexShader	= compile vs_3_0 water_VS();	
		PixelShader 	= compile ps_3_0 water_PS();	
	}
}

//////////////////////////////////////////

technique fallback 
{ 
	pass 
	{
		
	} 
}
