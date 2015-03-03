
#ifndef MBsky_h
#define MBsky_h

/////////////////////////////////////////////////////////
// defines

#define SKY_DEFAULTAMBIENT	 	25											// default ambient level of all entities

/////////////////////////////////////////////////////////
// variables

// sky entities
STRING* 	skyCubeName     = "skies\\scube02b_clearnoon+6.tga";						
STRING* 	skyCylinderName = "skies\\cyl001.dds";
STRING* 	skyClouds1Name  = "skies\\dome001_3dgs24b.dds";
STRING* 	skyClouds2Name  = "skies\\dome002_nice.dds";
STRING* 	skyModelName	 = "skies\\skybox.mdl";						// must have a (24b or 32b) tga skin

ENTITY* 	skyCube 			= NULL;
ENTITY* 	skyCylinder	 	= NULL;
ENTITY* 	skyClouds1 		= NULL;
ENTITY* 	skyClouds2 		= NULL;
ENTITY* 	skyModel 		= NULL;

STRING* 	skyModelSkin			= "";
STRING* 	skyDefaultModelSkin	= "cube002+6.tga";					// important for proper loading sky models when applying default skin

// sky parameters
var 		sky_domeorcube 			= 0;					// default: sky dome clipping distance is used even if no sky used

var 		sky_loddistance1 			=  500;				// LOD distances in quants
var 		sky_loddistance2			= 2000;
var 		sky_loddistance3			= 7000;

// height fog (used in sc shaders only)
var		sky_hfogminheight			= -100;				// height fog start level						[-500..1000]		-50
var		sky_hfogmaxheight			= 320;				// height fog height   							[0..3000]			320
var		sky_hfogheightstrength	= 0.25;				// height fog strength							[0..1]				0.25
var		sky_hfogheightblending	= 5.0;				// blending distance multiplier				[0..16]				5.0
var		sky_hfogheightdensity	= 0.5;				// height fog density 							[0..1]				1 = linear, <0 more dense
var		sky_hfogheightmin			= 0.20;				// min density inside height fog				[0..0.5]				0.20

// fake sky light (used in sc shaders only)
var		sky_skylightstart			= 8000;				// start distance
var		sky_skylightend			= 10500;				// 100% distance
var		sky_skylightcolor[3]		= { 0, 1, 1 };		// rgb color stored in 0..1 range, converted from sky_color
var		sky_skylightheight		= 0.50;				// percentage on sky image

////////////////////////////////////////////////////////
// functions

void	Sky_DefaultInit();									// sky system initialization to default settings
void	Sky_RemoveSkies();									// remove safely sky entitites

void 	Sky_Save_Sky_sky(char* file_name);				// save sky information including entities, lod, fog, sun, camera, pssm, scsm
void	Sky_Load_Sky_sky(char* file_name);				// load sky information including entities, lod, fog, sun, camera, pssm, scsm

////////////////////////////////////////////////////////
			
			
#endif