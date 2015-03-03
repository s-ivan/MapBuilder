
#ifndef MBskyeditor_h
#define MBskyeditor_h

/////////////////////////////////////////////////////////
// defines

#define SKY_MAXPERSET 			8
#define SKY_MAXSET 				10

/////////////////////////////////////////////////////////
// variables

// sky editor main menu---------------------------

PANEL*	sky_lowpanel;			// main panel
PANEL*	sky_colpan1;			// sky
PANEL*	sky_colpan2;			// sun
PANEL*	sky_colpan3;			// fog1
PANEL*	sky_colpan4;			// fog2
PANEL*	sky_colpan5;			// fog3
PANEL*	sky_colpan6;			// fog4
PANEL*	sky_colchangepanel;	// change any color of the 6 above
PANEL*	sky_heightfogpanel;	// haight fog parameters
PANEL*	sky_skypanel;			// change sky entities

BMAP*		panel_color1_bmp			= "panels\\color.bmp";
STRING* 	panel_color1 				= "bmap = panel_color1_bmp;";
BMAP*		panel_color2_bmp			= "panels\\color.bmp";
STRING* 	panel_color2 				= "bmap = panel_color2_bmp;";
BMAP*		panel_color3_bmp			= "panels\\color.bmp";
STRING* 	panel_color3 				= "bmap = panel_color3_bmp;";
BMAP*		panel_color4_bmp			= "panels\\color.bmp";
STRING* 	panel_color4 				= "bmap = panel_color4_bmp;";
BMAP*		panel_color5_bmp			= "panels\\color.bmp";
STRING* 	panel_color5 				= "bmap = panel_color5_bmp;";
BMAP*		panel_color6_bmp			= "panels\\color.bmp";
STRING* 	panel_color6 				= "bmap = panel_color6_bmp;";

// var*s
var *sky_skycurve;
var *sky_skyclip;
var *sky_sunpan;
var *sky_suntilt;
var *sky_sunlight;
var *sky_camclipnear;
var *sky_camclipfar;
var *sky_camamb;
var *sky_camarc;
var *sky_lod1;
var *sky_lod2;
var *sky_lod3;
var *sky_fogstart;
var *sky_fogend;
var *sky_fogcol;
var *sky_shadran;
var *sky_shadalpha;
var *sky_r;
var *sky_g;
var *sky_b;

var *sky_fog_minheight;
var *sky_fog_maxheight;
var *sky_fog_heightstrength;
var *sky_fog_heightblending;
var *sky_fog_heightdensity;
var *sky_fog_heightmin;

var *sky_skylight_start;
var *sky_skylight_end;
var *sky_skylight_height;

// sky entity settings submenu---------------------------

// sky sets
var 		max_sky_items	= SKY_MAXPERSET;
var 		max_sky_sets 	= SKY_MAXSET;					// to avoid trying to use invalid sets
var 		sky_set;												// 10 different obstacle sets can be created/used with 8 items in each
//STRING* 	sky_foldername 	= "skies\\";			// subfolder name
STRING**	sky_filenames = NULL;							// str_create("") is needed as initialization, before str_cpy() !!! - should be created
TEXT* 	skies_txt;
var 		sky_switch_type[SKY_MAXPERSET];				// what to place from the actual set: items 1-8
var 		sky_selected		= 0;							// 0..7

// var*s
var *sky_skyu;
var *sky_skyv;
var *sky_skyscale_x;
var *sky_skyalpha;
var *sky_skypan;
var *sky_skytilt;
var *sky_skyz;

// vars
var sky_skylayer;

////////////////////////////////////////////////////////
// functions


void 	Sky_Init();														// sky editor start
void 	Sky_Close();													// sky editor close

	void	Sky_Panel_Init();
	
		void	Sky_Button(button_number);
		
		void	Sky_DecFog();
		void	Sky_IncFog();
		
		void	Sky_ChangeColor(PANEL* panel);
			void		Sky_ChangeColorClose();
			void		Sky_ChangeColor_Button(button_number);
			void		Sky_SunLight_Button(button_number);
			void		Sky_SkyLight_Button(button_number);
			void		Sky_HeightFog_Button(button_number);			
			
		void		Sky_RemoveModel();
		void		Sky_RemoveCube();
		void		Sky_RemoveCyl();
		void		Sky_RemoveDome1();
		void		Sky_RemoveDome2();
		
		void		Sky_SetSky(var callbutton);
			void		Sky_SetSkyClose();
			void		Sky_SetSky_ModelButton(button_number);
			void		Sky_SetSky_CubeButton(button_number);
			void		Sky_SetSky_CylinderButton(button_number);
			void		Sky_SetSky_CloudsButton(button_number);
		
			void		Sky_ToggleItem(button_number);
			void		Sky_IncSet();
			void		Sky_DecSet();
			void		Sky_UpdateSetNames();

			void		Sky_ChangeModel();
			void		Sky_ChangeCube();
			void		Sky_ChangeCylinder();
			void		Sky_ChangeDome1();
			void		Sky_ChangeDome2();

////////////////////////////////////////////////////////

		
#endif