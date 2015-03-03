
////////////////////////////////////////////////////////////////////////////////////////////////
// defines

#define HEIGHT_FOG			// enables height fog for all sc_obj_ shaders, used when shadow_stencil == -1	
#define SKY_LIGHT				// fake sky light, basically a secondary distance fog
//#define DEBUG_TILE			// activates RTS visual debugging for sc_obj_ model, level, terrain shaders, requires DEBUG_PLAY02	
//#define SM_LOD				// use tex2D instead of tex2Dlod for shadowmap sampling

////////////////////////////////////////////////////////////////////////////////////////////////
// variables

//float scsm_brightness = 1.0;		// to match PSSM shaders (but slightly brighter than ffe!)
float scsm_brightness = 1.6;		// to match terrain shaders

#ifdef SKY_LIGHT
	float sky_skylightstart_var	= 10000;
	float sky_skylightend_var		= 12500;
	float sky_skylightheight_var	= 0.50;
	float3 sky_skylightcolor_var;
#endif