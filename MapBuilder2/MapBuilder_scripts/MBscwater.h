
#ifndef MBscwater_h
#define MBscwater_h


//////////////////////////////////////////////////////////////
// variables

// reflection
var scwater_reflectcolor[4]		= {0.23, 0.32, 0.53, 1.0};			// 0 - 1					float4
var scwater_reflectiondistance 	= 5.2;									// 1 - 10
var scwater_reflectionstrength 	= 0.04;									// 0 - 0.5

// refraction
var scwater_refractcolor[4]		= {0.10, 0.22, 0.37, 1.0};			// 0 - 1					float4
var scwater_refractionstrength 	= 0.04;									// 0 - 1

// shore
var scwater_shorecolor[4]			= {1.0, 1.0, 1.0, 0.1};				// 0 - 1					float4

// global
var scwater_globaltransparency 	= 0.50;									// 0 - 1

// wave
var scwater_wavespeed				= 17500;									// 5000 - 30000
var scwater_displacement 			= 12;										// 0 - 320
var scwater_speed						= 0.025;									// 0.000 - 0.100
var scwater_turbdensity				= 0.50;									// 0 - 1

// foam
var scwater_foamwave_deep			= 0.0;									// 0 - 1
var scwater_foamwave_shore			= 0.0;									// 0 - 1
var scwater_foamtiling 				= 27;										// 1 - 50
var scwater_foamdistortion 		= 0.010;									// 0 - 0.50
var scwater_foamtranslation 		= 0.857;									// 0 - 10
var scwater_shorefoamscale 		= 0.60;									// 0 - 1
var scwater_foamscale 				= 0.15;									// 0 - 1

// sun
var scwater_sunlightstrength 		= 10.0;									// 0 - 10
var scwater_sunlightarea 			= 921;									// 10 - 1000

// ripples
var scwater_bumpstrength 			= 32.0;									// 0 - 50
var scwater_bumptexscale 			= 35.0;									// 0 - 100
var scwater_2ndwave_speed			= 2.00;									// -2 - 2
var scwater_2ndwave_scale			= 0.75;									// 0 - 5

//////////////////////////////////////////////////////////////
// views

// depth--------------------------------------------------------

var scwater_waterdepth = 16;					// max shore depth

BMAP* sc_map_depth = NULL;

MATERIAL* sc_mtl_depth =
{
//	event = sc_mtlEvent_depth;					// not needed when FLAG1 set for water and NOFLAG1 applied for depth view
	flags = ENABLE_RENDER;
	effect = "sc_water_depth_MB.fx";			// the _SM3 version is not okay with tree animation - it can be seen unmoved in refraction part
}

VIEW* sc_view_depth = 
{
	material = sc_mtl_depth; 
	layer = -2;								
}

// reflection----------------------------------------------------

BMAP* sc_map_reflect = NULL;

VIEW* sc_view_reflect = 
{ 
	layer = -11;
}

// refraction----------------------------------------------------

BMAP* sc_map_refract = NULL;

VIEW* sc_view_refract = 
{ 
	layer = -10;
}

//////////////////////////////////////////////////////////////
// textures

STRING* sc_map_nm_name 		= "water_hmp\\water1_skin1_na.tga";
STRING* sc_map_foam_name 	= "water_hmp\\waterFoamTex2.tga";
STRING* sc_map_depth_name 	= "water_hmp\\water_d.tga";

//STRING* sc_map_fres_name 	= "water_hmp\\sc_waterFresnel.bmp";
//BMAP* sc_bmap_waterFresnel = NULL;
BMAP* sc_bmap_waterFresnel = "water_hmp\\sc_waterFresnel.bmp";

//////////////////////////////////////////////////////////////
// event

function sc_mtlEvent_water()
{
//	mtl.skill1 = floatv(-1.0);
	
	if(render_view == camera)
	{
		return 0;									// render the entity
//		mtl.skill1 = floatv(1.0);
	}
	
	return 1;
}

//////////////////////////////////////////////////////////////
// material

MATERIAL* sc_mtl_water =
{
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 200;  	diffuse_green  = 200;  	diffuse_red  = 200;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
	
	skin4 = sc_bmap_waterFresnel;				// can't be a created bitmap if skin is defined here, use bmap_create with ent_setskin
	flags = ENABLE_RENDER | TANGENT;			// | TRANSLUCENT; not needed		ENABLE_RENDER | 

	event = sc_mtlEvent_water;					// not needed, FLAG1 does the job, but can ignore it from refraction view too
}

//////////////////////////////////////////////////////////////
// functions

void Scwater_Init(ENTITY* ent);
	void Scwater_Close();
	
	void Scwater_Depth();
	void Scwater_Reflection();
	void Scwater_Refraction();

	void Scwater_ViewsLoop(ENTITY* ent);

//------------------
// load/save

	void		Scwater_Save_ScWater_scw(char* file_name);
	void		Scwater_Load_ScWater_scw(char* file_name);
	
	void		Scwater_Load_ScWaterNm_tga(char* file_name);
	void		Scwater_Load_ScWaterFoam_tga(char* file_name);		

//////////////////////////////////////////////////////////////


#endif