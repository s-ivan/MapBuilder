
#ifndef MBmwater_h
#define MBmwater_h

//////////////////////////////////////////////////////////////////////
// defines

//#define MIRROR_DEBUG

//////////////////////////////////////////////////////////////////////
// variables

var mwater_speedx				= 10;
var mwater_speedy				= 25;
var mwater_ripple				= 50;
var mwater_scale				= 100;

var mwater_mirrorvisible 	= 0;

//////////////////////////////////////////////////////////////////////
// views

#ifdef MIRROR_DEBUG
	VIEW* mw_view_mirror = { layer = 1; }
#else
	VIEW* mw_view_mirror = { layer = -1; }  // render mirror view before camera view
#endif

//////////////////////////////////////////////////////////////////////
// bmaps

BMAP* 	mw_bmap_mirrortarget 	= NULL;
BMAP*		mw_bmap_water_uv 			= NULL;

//////////////////////////////////////////////////////////////////////
// materials

MATERIAL* mtl_mirrorWaterMB =
{
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 200;  	diffuse_green  = 200;  	diffuse_red  = 200;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
	effect = "mirrorWaterMB.fx";
}

//////////////////////////////////////////////////////////////////////
// functions

void		Mwater_Init(ENTITY* ent);

//------------------
// load/save

	void		Mwater_Save_MWater_smw(char* file_name);
	void		Mwater_Load_MWater_smw(char* file_name);	
	
//////////////////////////////////////////////////////////////////////



#endif