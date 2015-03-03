
#ifndef MBpssmeditor_h
#define MBpssmeditor_h

/////////////////////////////////////////////////////////
// variables

// var*s 
var *sky_pssm_transp;
var *sky_pssm_res;
var *sky_pssm_splitw;
var *sky_pssm_fadeout_perc;

var *sky_pssm_bias;
var *sky_pssm_limit;

var *sky_pssm_max;
var *sky_pssm_manual;
var *sky_pssm_dist1;
var *sky_pssm_dist2;
var *sky_pssm_dist3;
var *sky_pssm_dist4;
var *sky_pssm_shadowweight;
var *sky_pssm_lod;
var *sky_pssm_splitmode;

var *sky_pssm_esm;			
var *sky_pssm_blurfactor;			
var *sky_pssm_blurtype;				
var *sky_pssm_fogfactor;			

// vars
var sky_pssm_numsplits;			// Pssm_Start needs to be set to 0, wait, and then to this value
var sky_pssm_auto;

/////////////////////////////////////////////////////////
// panels

PANEL*	sky_pssmpanel;			// pssm shadow settings for real-time parameters
PANEL*	sky_pssmpanel2;		// pssm shadow settings for static parameters

/////////////////////////////////////////////////////////
// functions

void		Sky_PssmPanel_Init();
	void		Sky_PssmPanel_Close();
	
	void		Sky_Pssm_Button(button_number);
	void		Sky_Pssm_Button2(button_number);
	void		Sky_Pssm_Apply();
	void		Sky_Pssm_ToggleManual();
	void		Sky_Pssm_SetBlurType();	

/////////////////////////////////////////////////////////


#endif