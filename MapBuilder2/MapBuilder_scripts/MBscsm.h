
#ifndef MBscsm_h
#define MBscsm_h


//#define DEBUG_DEPTHMAP

//--------------------------------------------------

BMAP* sc_map_shadowAlpha = "sc_shadowAlphaInv.bmp";	// a square bitmap fading black to white outwards,			sampled in obj shaders,		inverted to decrease the chance of black articafts out of shadow area
BMAP* sc_map_shadowDepthSun;									// used for blurring, maybe another stage needed,			sampled in obj shaders

//--------------------------------------------------

float sc_orthoMat[16];										// was var									// area on ground in front of camera -> mat_effect1
//float sc_orthoLightPos[16];								// was var, floatv -> (float)			// Sun position on 012, 3 maxdepth   -> mat_effect2 -> scsm_maxdepth
float sc_sunLightView[16];									// was var									// depth * matWorldView					 -> mat_effect3

//--------------------------------------------------
// events

function sc_mtl_shadowDepthSunEvent()
{
	mat_set(sc_sunLightView, matWorld);
	mat_multiply(sc_sunLightView, matView);						// matWorldView, but that is for shaders only
	
	mat_effect3 = sc_sunLightView;
	
	return 0;																// 1 to test area
}

//--------------------------------------------------

MATERIAL* sc_mtl_shadowBlur =
{
//	effect = "sc_esmDepthBlurN_MB.fx";						// no blur
	effect = "sc_esmDepthBlurB_MB.fx";						// box blur
//	effect = "sc_esmDepthBlurP_MB.fx";						// Poisson blur
}

MATERIAL* sc_mtl_shadowDepthSun =
{
	event = sc_mtl_shadowDepthSunEvent;
	effect = "sc_esmDepth_MB.fx";
}

//--------------------------------------------------

VIEW* blurDepthView = NULL;
VIEW* shadowDepthView = NULL;

////////////////////////////////////////////////////////////////////////////////////////////////
// parameters

var scsm_resolutions[4] = {512, 1024, 2048, 4096};

// main
var scsm_run					= 0;			// ok - run loop

// basic data
var scsm_resolution 			= 2048;		// 1024 ok - keep it low for good performance															// requires restart
var scsm_areasize	 			= 3000;		// ok - in quants																									// requires restart
var scsm_maxdepth 			= 300;		// ok - was = matEffect2[0].w = sc_orthoLightPos[3] , used	in depth and object shaders	// real-time
var scsm_sundistance 		= 3750;		// ok - to get proper sun position																			// calculated
var scsm_lod					= 0;			// -1 NOLOD set, 0-3 NOLOD not set, lod offset applied

// depth blur shader
var scsm_blurpixel 			= 1.71;		// ok - sc_mtl_shadowBlur.skill1 is vecSkill1 in blur shaders, now only for gui				// real-time
float scsm_blursize 			= 0.00195;	// ok - read by shaders, based on scsm_blurpixel and scsm_resolution								// calculated
var scsm_blurtype 			= 2;			// or use effect load : 0 no, 1 box, 2 poisson, 3 -													// effect_load

// exponential scale factors - object shaders
var scsm_esm						= 15;		// exponential shadowmapping constant																	// real-time
var scsm_esm_obj					= 15;		// exponential shadowmapping constant																	// real-time

//	// obsolete data - variance values - object shaders
//	var scsm_vsm_epsilon				= 1.5;		// terrain																										// real-time
//	float scsm_vsm_epsilon_f		= 0.0015;	// terrain																										// calculated
//	var scsm_vsm_epsilon_obj		= 3.0;		// objects																										// real-time
//	float scsm_vsm_epsilon_obj_f	= 0.0030;	// objects																										// calculated

// shadow darkness
var scsm_shadowalpha		 		= 0.55;																															// real-time
var scsm_lightmapalpha	 		= 0.55;																															// real-time

// experimantal
var scsm_shadowpixeloffset		= 0;
float scsm_shadowoffset_f		= 0;

// new adjustable border fade-out
var scsm_fadeout_start			= 0.75;

//////////////////////////////////////////////////////////////////
// functions

void Scsm_Start();													// init and loop
void Scsm_Close();													// close

	void		Scsm_SetBlurType();									// sets proper blur shader based on parameter value

//------------------
// load/save

	void		Scsm_Save_ssc(char* file_name);
	void		Scsm_Load_ssc(char* file_name);	

/////////////////////////////////////////////////////////////////

#endif