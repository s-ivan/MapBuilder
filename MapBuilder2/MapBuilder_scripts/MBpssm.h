
#ifndef MBpssm_h
#define MBpssm_h

/////////////////////////////////////////////////////////////////
// defines

//#include <d3d9.h>

//#define PRAGMA_PATH "%EXE_DIR%\code";

#define fx_shadow0 	"vp_pssmMBdefault.fx"				// single-pass shadow shader PCF
#define fx_shadow1 	"vp_pssmMBcustom.fx"					// single-pass shadow shader PCF
#define fx_shadow2 	"vp_pssmMBdefaultESM.fx"			// single-pass shadow shader ESM
#define fx_shadow3 	"vp_pssmMBcustomESM.fx"				// single-pass shadow shader ESM

#define fx_depth01	"vp_depth.fx"							// depth rendering shader
#define fx_depth23	"vp_depthESM.fx"						// depth rendering shader for ESM

//#define DEBUG_PSSM												// comment this in for displaying the shadowmaps

/////////////////////////////////////////////////////////////////
// variables

var pssm_res 				= 1024;				// shadow map resolution
var pssm_numsplits		= 3;					// number of splits: 3 for small, 4 for large levels
var pssm_limit 			= 3;					// limit of splits to be really used for better performance

var pssm_splitweight 	= 0.5; 				// logarithmic / uniform ratio
var pssm_splitdist[5];							// split distances
var pssm_fadeoutstart 	= 1000;				// max shadow distance for fade out
var pssm_fadeoutend 		= 3000;				// max shadow distance for fade out for pssm_limit and for better fog appearance
var pssm_fadeout_perc 	= 0.75;				//
var pssm_max 				= 0.50;				// used for multiplying camera.clip_far by it, to compensate view and camera distance differencies, in original split distance calculations
var pssm_manual			= 1;					// 1 manual, 0 auto based on pssm_splitweight
var pssm_dist1 			= 1000;				// split distances set manually
var pssm_dist2 			= 2000;
var pssm_dist3 			= 4000;
var pssm_dist4 			= 8000;

var pssm_transparency 	= 0.6;				// shadow darkness
var pssm_shadowweight 	= 2.5;				// original value 2.5, max 5.5 is useful, distorts shadow appearance
var pssm_lod				= -1;					// -1 NOLOD offset 0; >=0 apply lod on shadows, 0 no offset, 1-2-3 offset by 1-2-3, but distance is calculated in depth view... not really nice
var pssm_splitmode		= 1;					// 0 default PSSM (PCF), 1 custom PSSM (PCF, better), 2 default PSESM, 3 custom PSESM (most preferred)

// PCF
var pssm_bias 				= 0.50;				// easier to tweak by a slider if var, not float
float pssm_fbias			= 0.0005;			// calculated

// ESM
var pssm_esm				= 1000;				// exponential shadowmapping (ESM) scale factor 1..2500
var pssm_blurtype			= 2;					// 0 no, 1 box, 2 poisson
var pssm_blurfactor		= 1.25;				// depthmap blur distance when ESM applied
float pssm_blur			= 0.004;				// calculated
var pssm_fogfactor		= 7.5;				// adjusts fog fade-out effect on shadows (alpha fade-out), around 7.5 fine

MATERIAL* pssm_mtlDepthBlur	= NULL;

/////////////////////////////////////////////////////////////////
// functions

void		Pssm_Init();
void		Pssm_Start(var numsplits);
void		Pssm_Close();
	
	void 		Pssm_ViewToSplitCustom(VIEW *frustview, var nearplane, var farplane, VIEW *split, float *matSplit);
	void 		Pssm_ViewCopy(VIEW* from,VIEW* to);
	float* 	Pssm_TexScale(float fSize);
	void		Pssm_Split(VIEW* view,var num,var weight);

//------------------
// load/save

	void		Pssm_Save_sps(char* file_name);
	void		Pssm_Load_sps(char* file_name);	

/////////////////////////////////////////////////////////////////


#endif