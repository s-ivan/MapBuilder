
#ifndef MBscsmeditor_h
#define MBscsmeditor_h

/////////////////////////////////////////////////////////
// variables

// var*s 
var *sky_scsm_maxdepth;
var *sky_scsm_blurpixel;
var *sky_scsm_blurtype;
var *sky_scsm_vsm_epsilon;
var *sky_scsm_vsm_epsilon_obj;
var *sky_scsm_shadowalpha;
var *sky_scsm_lightmapalpha;
var *sky_scsm_shadowpixeloffset;
var *sky_scsm_fadeout_start;

// vars
var sky_scsm_resolution;
var sky_scsm_areasize;
var sky_scsm_lod;

/////////////////////////////////////////////////////////
// panels

PANEL*	sky_scsmpanel;			// scsm shadow settings

/////////////////////////////////////////////////////////
// functions

void		Sky_ScsmPanel_Init();
	void		Sky_ScsmPanel_Close();

	void		Sky_Scsm_Button(button_number);
	void		Sky_Scsm_Apply();
//	void		Sky_Scsm_SetBlurType();

/////////////////////////////////////////////////////////


#endif