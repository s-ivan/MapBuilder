////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MapBuilder Editor - Startup Menu
//
////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef mapbuilder_c
#define mapbuilder_c

////////////////////////////////////////////////////////////////////////////////////////////////////////
// system includes


#include <acknex.h>											// A8 game engine

#include <d3d9.h>												// to access DirectX functions
#include <windows.h>											// to access GetTickCount for precision timer
#include <strio.c>											// Win API I/O functions - needed by file handling functions used in wmb compilation

//#include <default.c>										// modified version is used		
//#include <level.c>											// splash screen and terrain functions - replaced with own functions in teredit.c - nearly similar
#include <mtlFX.c>											// used by engine default object shaders
//#include <mtlView.c> 										// customized in MBMtlView.c
//#include <shadows.c>										// pssm - modified version used: MBshadows.c
#include <keys.c>												// needed by key_hit


////////////////////////////////////////////////////////////////////////////////////////////////////////
// variables - <trr>-s to be renamed

#define 	MAINMENU_MAXPERSET 			8
#define	MAINMENU_MAXFILES				600

BMAP*		mainmenu_slider_bmap			= NULL;			// scaleable image to adjust its height to the number of items in the file list displayed - not essential in main menu
var		mainmenu_actual_first_item	= 0;				// 0..max-1 , shows the id of the 1st filename displayed
var		mainmenu_all_items			= 0;				// number of file names stored by the TEXT object in its strings
var		mainmenu_last_item			= 0;				// the last item that can be filemenu_actual_first_item, to hide empty file slots
var		mainmenu_itemslider			= 0;				// actual position of the scrollbar slider, separated from filemenu_actual_first_item, but updated in each frame

STRING**	mainmenu_filenames			= NULL;			// file names displayed
TEXT*		mainmenu_files_txt;								// store all filenames of a foder in its strings
var		mainmenu_select 				= 1;				// 1st button i.e. 1st file is selected

STRING*	mainmenu_hmp_filename		= NULL;			// store ground and water terrain filenames
STRING*	mainmenu_hmpw_filename		= NULL;

int		mainmenu_whattoload 			= 0;				// determines what map to load by the common file menu
																	// 0 full map, 1 terrain only, 2 wmb, 3-4 create from ground and water terrain

////////////////////////////////////////////////////
// strings


STRING*	main_currentres 	= "_resolution.txt";		// file name of actual settings data
STRING*	main_prevres 		= "_prevres.txt";			// file name of last valid settings data
STRING*	main_defaultres 	= "_defres.txt";			// file name of default settings data


////////////////////////////////////////////////////
// panels


PANEL* 	mainmenu_panel;									// contains a few main menu buttons to decide what map to load
PANEL* 	mainmenu_backpanel;								// holds a fullscreen background image
PANEL* 	mainmenu_shadecpanel;							// required by shade-c license

PANEL* 	mainmenu_settings_panel;						// a larger menu background

PANEL* 	mainmenu_load_panel;								// file loading manu background - to be kept only


////////////////////////////////////////////////////
// texts


TEXT* mainmenu_titletext =
{
	layer = 10;
	
	pos_x = 512;
	pos_y = 75;
	
//	red = 250;
//	green = 240;
//	blue = 150;
	
	red = 75;
	green = 75;
	blue = 75;
	
	font = "Arial#50b"; 
	string ( "MapBuilder 2" );	
	
	flags = CENTER_X | CENTER_Y | SHADOW;	
}


TEXT* mainmenu_copytext =
{
	layer = 10;
	
	pos_x = 512;
	pos_y = 650;
	
//	red = 250;
//	green = 240;
//	blue = 150;
	
	red = 75;
	green = 75;
	blue = 75;
	
	font = "Arial#20b"; 
	string ( "Copyright 2011-2017 Ivan Stummer" , 
				"www.mapbuilder.uw.hu" , 
				"All rights reserved" , 
				"" , 
				"Version 2.53.0" );
	
	flags = CENTER_X | CENTER_Y | SHADOW;	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions


void	main();
	
	void	MainMenu_Panel_Init();
		void	MainMenu_Close();
		void	MainMenu_Exit();
		
		//----------------------------------------------------------
			
		void	Main_RefreshPanels();
		
		void	Main_Settings_Panel_Init();
			void	Main_Settings_Panel_Close();
			void	Main_Settings_Button(button_number);
			void	Main_Apply_Settings();
			void	Main_Cancel_Settings();
			void	Main_Reset_Settings();
				void	Main_Read_Settings(STRING* filename);
				void	Main_Write_Settings(STRING* filename);				
				
		//----------------------------------------------------------
		
		void	MainMenu_Map();
		void	MainMenu_Trr();		
		void	MainMenu_Wmb();
		void	MainMenu_Hmp();
		void	MainMenu_HmpW();
		void	MainMenu_Null();
		
		void	MainMenu_Load_Init();
			void	MainMenu_Load_Close();
			void	MainMenu_Load_Load();
			
			void	MainMenu_Load_IncSet();
			void	MainMenu_Load_DecSet();
			void	MainMenu_Load_ChangeSet();
			void	MainMenu_Load_Select(button_number);
		
		//----------------------------------------------------------	

////////////////////////////////////////////////////////////////////////////////////////////////////////
// includes


// MapBuilder2 project includes

#define PRAGMA_PATH "MapBuilder_scripts"
#define PRAGMA_PATH "RTS_scripts"				// RTS game scripts excluded from free editor

#include "MapBuilder2.h"		
#include "MapBuilder2.c"		


////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


void main()
{
sys_marker("in0");	

	UIMode = (var)UIMODE_INIT;
	
//	compatibility = 13;						// nexus and memory in kB instead of MB - makes free texture memory wrongly as a totally invalid minus value ! engine bug
	
	// fade in---------------------
		
	if ((sys_winversion >= 6) && (video_screen==2))
		video_alpha = 0;
		
	// show all errors-------------
//	warn_level = 0;
	
//	screen_size.x = 1024;
//	screen_size.y = 768;
//	video_aspect = 1.333;					// or 1.6 or 1.777		to force aspect ratio
	
	// set window title
//	video_window(NULL,NULL,1,"MapBuilder");
	vec_set(screen_color,vector(1,1,1));
	vec_set(sky_color,vector(1,1,1));		
	
	// free up memory on exit-------
	on_exit 		= Exit_Event;				// error message cancel button - it is run one frame before shut down
	on_close 	= Main_Exit;				// close icon of title bar in windowed mode
//	on_resize 	= ScreenSize_Event;		// only if resizable window !
//	on_maximize = ScreenSize_Event;		// only if resizable window !
//	on_minimize = ScreenSize_Event;		// not needed at all
	
	// avoid 3dgs calls-------------
	on_enter	= Key_Do_Nothing;			// disable resize
	on_esc	= Key_Do_Nothing;			// disable exit
	on_tab	= Key_Do_Nothing;			// disable console
	on_f1		= Key_Do_Nothing;
	on_f2		= Key_Do_Nothing;
	on_f3		= Key_Do_Nothing;
	on_f4		= Key_Do_Nothing;
	on_f5		= Key_Do_Nothing;
	on_f6		= Key_Do_Nothing;
	on_f7		= Key_Do_Nothing;
	on_f8		= Key_Do_Nothing;
	on_f9		= Key_Do_Nothing;
	on_f10	= Key_Do_Nothing;
	on_f11	= Key_Do_Nothing;			// disable debug screen
	on_f12	= Key_Do_Nothing;
	on_0		= Key_Do_Nothing;			// disable free move
		
//	if (sys_metrics(43)<3)
//		{
//			printf("MapBuilder requires 3 mouse buttons!");
//		}
//	if (sys_metrics(75)<1)
//		{
//			printf("MapBuilder requires a mouse wheel for zooming!");
//		}
		
	// keep desktop resolution - always use this
//	video_set(sys_metrics(0), sys_metrics(1), 0, 1);
//	video_set(1024, 768, 0, 1);
	// window settings
//	video_window(NULL,NULL,4+16+32+64+128,"MapBuilder2");		// resizable-border +title +close, +minimize(, +maximize) - stops with DX texture errors!
	video_window(NULL,NULL,2+16+32+64,"MapBuilder2");			// fix-border +title (default), +close, +minimize
//	video_window(NULL,NULL,1,NULL);									// no border - okay for ATI card usage in windowed mode too
	
//	wait(1);
	
	//----------------------------------------------------------------------------------------
	
	// get available resolutions
	VideoRes_GetScreenRes();
	wait_for(VideoRes_GetScreenRes);
	
	// load last settings to current
	Main_Read_Settings(main_currentres);
	wait_for(Main_Read_Settings);
	
	// update main resolution array index
	int i=0;
	for (i=0; i<30; i++)
		{
			if ((videores_table_x[i]==(var)screen_size.x) && (videores_table_y[i]==(var)screen_size.y))
				{
					main_resolution = i;
					break;
				}	
		}
		
	// store settings to previous - for cancel - requires main_resolution to be set properly
	Main_Write_Settings(main_prevres);
	wait_for(Main_Write_Settings);
	
	//-----------------------------------------------------------------------------------------------	
	
	// set pro edition only stuff
	if ((main_engineversion == (var)2) && (edition == (var)4))
		{
			d3d_instancing = 1; 							// activate sprite instancing - Pro
			terrain_lod 	= main_terrain_lod; 		// activate terraincache and lod - Pro		
																// terrain scaling problems - solved	
																// high slope terrain requires lower lod value to avoid gaps
																// >1 okay with shadow_stencil -1, 0, 1
																// >1 not okay with shadow_stencil 8		(together with shadow lod i.e. pssm_lod)
		}
	else
		{
			d3d_instancing = 0;
			terrain_lod 	= 0;
		}	
	
	terrain_chunk	= main_terrain_chunk;			// default value
	
	//-----------------------------------------------------------------------------------------------	
	
//	stencil_blur(1); 						// not now, activate blurred shadows, included in mtlView.c, look better, but not okay with decal shadows !!!
	mat_shadow.alpha = 40;
	shadow_threshold = 0;				// dynamic shadows switch on/off lightmap brightness level 0..255

	// to enable close model placement but terrain models are ignored as shadow casting surfaces
	shadow_mode |= IGNORE_FLAG2 ;
//	shadow_mode |= IGNORE_MODELS;
//	shadow_mode &= ~IGNORE_PASSABLE;
//	shadow_mode &= ~IGNORE_PASSENTS;
	
	move_friction = 0.5;					// 0.5 okay - too low friction results in bouncing off, thus a bit higher is needed for proper collision counting
	move_min_z = 0.4;
												// !!!
	max_entities  = 75000;				// 300000, 200000 results in a surpressed nexus overflow on level load in case of max nexus 200, 100000 not but can easily lead to nexus overflow on design time
												// depends on max nexus set by MapBuilder.wmp/wmb or in case of running from SED by -nx ...
												// 100000 requires nexus approx. 300, 50000 requires nexus approx 150
	
	max_particles = 100000;				// 500000, 100000 decal shadows use particles! - using many effect particles results in bad performance
	
	clip_particles = 0.25;
	d3d_near_decals = 0.999;			// 1 : to not to leave decals to cover entities, disable ZNEAR, decal shadow flickering
	
//	particle_mode 	= 4;					// force decals to use entiy mesh 4 , or own mesh 8 - the best to use auto
	decal_clip		= 89;					// max angle of difference between decal normal and surface normal 0..89
	
	map_subents = 1;						// place wmb map entity sub-enitites too
	
	if (d3d_texlimit>4096)
		d3d_texlimit = 4096;				// limit max texture size
	
	tree_range = 1024;					// default 1000
	time_smooth = 0.9;					// time_step is average of last x frames, 0.9 = 10 frames, 0.666 = 3 frames , 0.5 = 2 frames 
	
//	max_bones = 1;							// only one bone i.e. no bone weighting, 1..4, default 4 (for CPU bone anim)
	
	//-------------------------------
//	mip_levels 	= 8; 	// 0..10 use up to 10 mipmaps, default 4 - good only for dds, tga can store only 4 but created runtime if more than 4
//
//	mip_shaded 	= 4; 	// let shaded block textures appear sharper in the distance
//	mip_flat 	= 4;	// let model textures appear sharper in the distance
//	mip_sprites = 4; 	// mipmapping for sprites 
	//-------------------------------
	
	level_load("mapbuilder.wmb");	
//	wait(3);		
		
	mouse_pointer = 2;  			// arrow on window borders
	mouse_map = mousearrow;		// in fullscreen mode it must be used
	mouse_mode = 1;
//	enable_mouse = 2;				// panel events can handle right clicks too	
	
//	draw_textmode("Arial",0,15,100);	// possible only after level load - after the first frame
	draw_textmode("Arial",1,20,100);	// better for debugging
	
	// startup menu------------------------
	
	UIMode = (var)UIMODE_STARTUP;
	
	MainMenu_Panel_Init();
	wait_for(MainMenu_Panel_Init);
	
	// refresh non created and main menu panel positions due to resolution
	Main_RefreshPanels();
	wait_for(Main_RefreshPanels);
	
	//--------------------------------------
	// fade in screen
		
	if ((sys_winversion >= 6) && (video_screen==2))
     for (video_alpha = 0; video_alpha < 100; video_alpha += 15*time_step)
       wait(1);	
	
	video_alpha = 100;	

	//--------------------------------------
	// level independent editor loop - mouse pointer update, screenshot making, 3dgs debug info panel, and custom debugging texts	
sys_marker("in1");
	
	var show_debug = 0;
	
//	fps_max = 0; // no limit test
	
	while(1)	
		{	
			//--------------------------------------------------
			// mouse pos update
			
			vec_set(mouse_pos, mouse_cursor);
			
			//--------------------------------------------------
			// make screenshot any time
			
			if ((key_f) && (key_alt) && (!key_ctrl) && (!key_shift))
				{
					while (key_f) {wait (1);}
					str_cpy(save_dir,"screenshots");
					file_for_screen( "shot.jpg" , ui_screencounter ); 	
					ui_screencounter++;
					str_cpy(save_dir,"");										// can cause problems if not reset...
				}
			
			//--------------------------------------------------
			// debug panel
			
			if (key_f11) 			
				{
					while (key_f11) {wait (1);}	
					if (show_debug)
						{
							reset(def_debug_pan,SHOW);
						}
					show_debug = 1 - show_debug;													
//					def_debug();	// sometimes fails !!!					
				}
			
			if (show_debug) 
				{
					set(def_debug_pan,SHOW);
					
					def_dfps = 0.9*def_dfps+0.1/time_frame;
					def_dtps = 0.9*def_dtps+0.1*num_vistriangles/time_frame;
					
					def_dent	= time_entities + time_physics;				// ent		4-6
					def_dtlv = time_level+time_sky	+time_portals;		// geo		0
					// time_effects												// par		0
					// time_draw													// drw		0
					// time_panels													// pan		0.5
					def_dtcs = time_client+time_server;						// srv		1			sound and movie + entity updates and multiplayer communication (high ref resultis in high srv too!)
					def_dtrf = time_update+time_idle;						// ref		4-5		screen refresh + idling to fps_max
					def_dtac = time_actions+time_pfuncs;					// fnc		8-13
					
//					uu1 = time_client;											// uu1		0
//					uu2 = time_server;											// uu2		1-2		entity update
					
					def_dsnd = num_mediastreams + num_sounds;
					
					def_cang.pan = cycle(camera->pan,0,360); 
					def_cang.tilt = cycle(camera->tilt,-180,180); 
					def_cang.roll = cycle(camera->roll,-180,180); 
					
//					def_box(def_debug_pan->pos_x-3,def_debug_pan->pos_y-2,
//	                    def_debug_pan->pos_x+314,def_debug_pan->pos_y+103,
//	                    vector(255,255,255));
					
					//if(pX_stats)
					//	num_bodies = pX_stats(1);
				}
			
			//--------------------------------------------------
			// other debug data for testing
			
//			if (level_ent)
//				{
//					DEBUG_VAR(level_ent.min_x, 400);
//					DEBUG_VAR(level_ent.min_y, 425);
//					DEBUG_VAR(level_ent.min_z, 450);
//					
//					DEBUG_VAR(level_ent.max_x, 500);
//					DEBUG_VAR(level_ent.max_y, 525);
//					DEBUG_VAR(level_ent.max_z, 550);
//				}
			
//			DEBUG_VAR(d3d_antialias,600);
//			DEBUG_VAR(shadow_stencil,600);
//			DEBUG_VAR(fixv(mtl_shadow.skill1),633);
//			DEBUG_VAR(stencil_blur_value,667);
			
//			DEBUG_VAR(terrhmp_vwatermax.x, 400);
//			DEBUG_VAR(terrhmp_vwatermax.y, 425);
//			DEBUG_VAR(terrhmp_vwatermax.z, 450);
//			
//			DEBUG_VAR(terrhmp_vgroundmax.x, 500);
//			DEBUG_VAR(terrhmp_vgroundmax.y, 525);
//			DEBUG_VAR(terrhmp_vgroundmax.z, 550);
			
//			DEBUG_VAR(scsm_resolution, 300);
//			DEBUG_VAR(warn_level, 325);
			
//			if (terrain_entity)
//				{
//					draw_text(str_for_entfile(NULL, terrain_entity), 300, 300, COLOR_WHITE);
//					draw_text(str_for_entname(NULL, terrain_entity), 500, 300, COLOR_WHITE);
//				}
//			if (water_entity)
//				{
//					draw_text(str_for_entfile(NULL, water_entity), 300, 325, COLOR_WHITE);
//					draw_text(str_for_entname(NULL, water_entity), 500, 325, COLOR_WHITE);
//				}
			//--------------------------------------------------
			
			wait(1);
			
			//--------------------------------------------------
		}	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// menu panel


void MainMenu_Panel_Init()
{
sys_marker("mpi");	
	
	//----------------------------------------------------
	// create background panel
	
	mainmenu_backpanel = pan_create( panel_image0 , 9 );
	set( mainmenu_backpanel, SHOW | SHADOW | OVERLAY );
//	mainmenu_backpanel.scale_x = screen_size.x / mainmenu_backpanel.size_x;
//	mainmenu_backpanel.scale_y = screen_size.y / mainmenu_backpanel.size_y;
	mainmenu_backpanel.size_x = screen_size.x;
	mainmenu_backpanel.size_y = screen_size.y;
	
	//----------------------------------------------------
	// create shadec logo panel
	
	mainmenu_shadecpanel = pan_create( panel_image6 , 10 );
	set( mainmenu_shadecpanel, SHOW | SHADOW | OVERLAY );
	mainmenu_shadecpanel.pos_x = screen_size.x - 200;
	mainmenu_shadecpanel.pos_y = screen_size.y - 60;
	
	//----------------------------------------------------
	// create menu panel
	
	mainmenu_panel = pan_create( panel_image , 10 );
	set( mainmenu_panel, SHOW | SHADOW | OVERLAY );
	mainmenu_panel.pos_x = screen_size.x/2 - 128;
	mainmenu_panel.pos_y = screen_size.y/2 - 128;	
	mainmenu_panel.red		= panel_font_r;
	mainmenu_panel.green		= panel_font_g;
	mainmenu_panel.blue		= panel_font_b;
	
	//----------------------------------------------------
	
	pan_setbutton(mainmenu_panel, 0,	1, 	24,24,		newmap_on_bmp, newmap_off_bmp, newmap_over_bmp, newmap_over_bmp,															MainMenu_Null,			NULL, NULL); 	
	pan_setbutton(mainmenu_panel, 0,	1, 	24,54,		loadmap_on_bmp, loadmap_off_bmp, loadmap_over_bmp, loadmap_over_bmp,														MainMenu_Map, 			NULL, NULL); 	
	pan_setbutton(mainmenu_panel, 0,	1, 	24,84,		loadterrain_on_bmp, loadterrain_off_bmp, loadterrain_over_bmp, loadterrain_over_bmp,								MainMenu_Trr, 			NULL, NULL); 		
	pan_setbutton(mainmenu_panel, 0,	1, 	24,114,		loadlevel_on_bmp, loadlevel_off_bmp, loadlevel_over_bmp, loadlevel_over_bmp,											MainMenu_Wmb, 			NULL, NULL); 
//	pan_setbutton(mainmenu_panel, 0,	1, 	24,144,		constructterrain_on_bmp, constructterrain_off_bmp, constructterrain_over_bmp, constructterrain_over_bmp,		MainMenu_Hmp, 			NULL, NULL); 	
	
	pan_setbutton(mainmenu_panel, 0,	1, 	24,144,		settings_on_bmp, settings_off_bmp, settings_over_bmp, settings_over_bmp,												Main_Settings_Panel_Init, 	NULL, NULL); 
	pan_setbutton(mainmenu_panel, 0,	1, 	24,212,		exit_on_bmp, exit_off_bmp, exit_over_bmp, exit_over_bmp,																		MainMenu_Exit,	 		NULL, NULL); 	
	
	//----------------------------------------------------
}


void MainMenu_Close()
{
sys_marker("mmc");
	
//	// wait to cover black screen during loading map - not needed any more, load panel+text is shown on each type of level load
//	wait(4);
	
	if (mainmenu_panel)
		{
			ptr_remove(mainmenu_panel);
			mainmenu_panel = NULL;
		}
	
	if (mainmenu_backpanel)
		{
			ptr_remove(mainmenu_backpanel);	
			mainmenu_backpanel = NULL;
		}
	
	if (mainmenu_shadecpanel)
		{
			ptr_remove(mainmenu_shadecpanel);
			mainmenu_shadecpanel = NULL;
		}
	
	Main_Settings_Panel_Close();
	
	reset(mainmenu_titletext , SHOW);
	reset(mainmenu_copytext , SHOW);
}


void MainMenu_Exit()
{
	wait(3);
	
	Main_Exit();	
}


/////////////////////////////////////////////////////


void	Main_RefreshPanels()
{
	//----------------------------------------------------------------
	// position non-created texts
	
	mainmenu_titletext.pos_x 	= screen_size.x / 2;
	mainmenu_copytext.pos_x 	= screen_size.x / 2;
	mainmenu_copytext.pos_y 	= screen_size.y - 100;
	
	//----------------------------------------------------------------
	// set non-created panel scales
	
	map_loadpanel.scale_x 		= screen_size.x / map_loadpanel.size_x;
	map_loadpanel.scale_y 		= screen_size.y / map_loadpanel.size_y;
	
	//----------------------------------------------------------------
	// set created panel scales
	
	if (mainmenu_backpanel!=NULL)
		{
			// null to be able to change			
			mainmenu_backpanel.bmap = NULL;						
			
			// select bmap accorning to screen aspect ratio
			if (screen_size.x/screen_size.y > (var)1.6)
				{
					mainmenu_backpanel.bmap = bmap_create("panels\\screen169.jpg");
				}
			else if (screen_size.x/screen_size.y < (var)1.6)
				{
					mainmenu_backpanel.bmap = bmap_create("panels\\screen43.jpg");
				}
			else
				{
					mainmenu_backpanel.bmap = bmap_create("panels\\screen1610.jpg");
				}
			
			// change panels size to bmap (can be smaller/bigger than screen)
			mainmenu_backpanel.size_x = bmap_width(mainmenu_backpanel.bmap);
			mainmenu_backpanel.size_y = bmap_height(mainmenu_backpanel.bmap);
			
			// scale panel to screen
			mainmenu_backpanel.scale_x = screen_size.x / mainmenu_backpanel.size_x;
			mainmenu_backpanel.scale_y = screen_size.y / mainmenu_backpanel.size_y;
		}
	
	//--------------------------------------------------------
	// set created editor menu panel position parameters (movable panels)
	
	// submenu default position
	ui_submenupanel_x		= screen_size.x-256;
	ui_submenupanel_y		= 0;
	
	// submenu small subpanel
	ui_submenupanel2_x	= screen_size.x-256;
	ui_submenupanel2_y	= 512;
	
	// submenu big subpanel
	ui_submenupanel3_x	= screen_size.x-512;
	ui_submenupanel3_y	= 0;
	
	//--------------------------------------------------------
	// set created panel positions
		
	if (mainmenu_panel!=NULL)										
		{
			mainmenu_panel.pos_x 			= screen_size.x/2 - 128;
			mainmenu_panel.pos_y 			= screen_size.y/2 - 128;
		}
	if (mainmenu_settings_panel!=NULL)
		{
			mainmenu_settings_panel.pos_x = screen_size.x/2 - 128;
			mainmenu_settings_panel.pos_y = screen_size.y/2 - 256;
		}
	
	//--------------------------------------------------------
	// set non-created panel positions
	
	mainmenu_shadecpanel.pos_x = screen_size.x - 200;
	mainmenu_shadecpanel.pos_y = screen_size.y - 60;
	
	ui_mainmenu_panel.pos_y 	= screen_size.y - 50;
	
	def_debug_pan.bmap			= bmap_createblack(280, 100, 24);
	def_debug_pan.pos_x			= 0;
	def_debug_pan.pos_y			= 0;
	
#ifndef MB_RELEASE
	tile_data_panel.pos_x 		= screen_size.x - 256;
	tile_data_panel.pos_y 		= 0;
	
	cluster_data_panel.pos_x 	= screen_size.x - 256;
	cluster_data_panel.pos_y 	= 256;
#endif
	
	//--------------------------------------------------------
	
	set(mainmenu_titletext,SHOW);
	set(mainmenu_copytext,SHOW);
	
	//--------------------------------------------------------
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////


void	Main_Settings_Panel_Init()
{
sys_marker("ms0");	

	wait(3);
	
	// update main resolution array index
	main_resolution = 0;
	int i=0;
	for (i=0; i<30; i++)
		{
			if ((videores_table_x[i]==(var)screen_size.x) && (videores_table_y[i]==(var)screen_size.y))
				{
					main_resolution = i;
					break;
				}	
		}
	
//	// store settings to previous - requires main_resolution to be set properly
//	Main_Write_Settings(main_prevres);
//	wait_for(Main_Write_Settings);
	
	// create panel--------------
	mainmenu_settings_panel = pan_create( panel_image2 , 11 );
	set( mainmenu_settings_panel, SHADOW | OVERLAY | SHOW );
	mainmenu_settings_panel.pos_x = screen_size.x/2 - 128;
	mainmenu_settings_panel.pos_y = screen_size.y/2 - 256;
	mainmenu_settings_panel.red		= panel_font_r;
	mainmenu_settings_panel.green		= panel_font_g;
	mainmenu_settings_panel.blue		= panel_font_b;
	
	//---------------------------
	
	//string 1 - title
	pan_setstring(mainmenu_settings_panel, 0,	15,10,	panel_font2,							str_create("SETTINGS") );
	
	// button 1 , ok -> Apply x45
	pan_setbutton(mainmenu_settings_panel, 0,	1, 		25,485,		s_ok_on_bmp,s_ok_off_bmp,s_ok_over_bmp,s_ok_over_bmp,															Main_Apply_Settings		,NULL,NULL); 
	
	//------------------------------------
	// resolution x - 2 arrows
	// string 2 3 4 5
	pan_setstring(mainmenu_settings_panel, 0,	15,40,	panel_font,			str_create("Resolution:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,40,							"%0.f", 	panel_font, 	1, 	videores_table_x[main_resolution]);
	pan_setstring(mainmenu_settings_panel, 0,	175,40,	panel_font,			str_create("x") );
	pan_setdigits(mainmenu_settings_panel, 0,	185,40,							"%0.f", 	panel_font, 	1, 	videores_table_y[main_resolution]);
	// button 2 3
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,40,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	220,40,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																								
	
	// main_video_screen - 2 arrows 1-2
	// string 6 7
	pan_setstring(mainmenu_settings_panel, 0,	15,60,	panel_font,			str_create("Screen mode:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,60,							"%0.f", 	panel_font, 	1, 	main_video_screen);
	// button 4 5
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,60,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,60,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	//------------------------------------
	// fps_max - slider 20-500
	// string 8	
	pan_setdigits(mainmenu_settings_panel, 0,	15,90,	"Fps max: %0.f",					panel_font,	1, 		fps_max);
	// slider 1 - 20..500
	pan_setslider(mainmenu_settings_panel, 0,	120+12,92,	horiz_slider_bmp,slider_knob_bmp,	20,500,		fps_max); 
	// button 6 7
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,90,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	240,90,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 
	
	// fps_min - slider 1-50
	// string 9
	pan_setdigits(mainmenu_settings_panel, 0,	15,110,	"Fps min: %0.f",					panel_font,	1, 		fps_min);
	// slider 2 - 1-50
	pan_setslider(mainmenu_settings_panel, 0,	120+12,112,	horiz_slider_bmp,slider_knob_bmp,	1,50,			fps_min); 
	// button 8 9
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,110,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	240,110,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 
	
	//------------------------------------
	// preload_mode - 2 arrows - 1 2 3 7 15
	// string 10 11
	pan_setstring(mainmenu_settings_panel, 0,	15,140,	panel_font,			str_create("Preload mode:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,140,							"%0.f", 	panel_font, 	1, 	preload_mode);
	// button 10 11
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,140,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	170,140,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	// tex_share - 2 arrows - 0 1
	// string 12 13
	pan_setstring(mainmenu_settings_panel, 0,	15,160,	panel_font,			str_create("Tex share:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,160,							"%0.f", 	panel_font, 	1, 	tex_share);
	// button 12 13
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	// d3d_triplebuffer - 2 arrows - 0 1
	// string 14 15
	pan_setstring(mainmenu_settings_panel, 0,	15,180,	panel_font,			str_create("Triple buffer:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,180,							"%0.f", 	panel_font, 	1, 	d3d_triplebuffer);
	// button 14 15
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,180,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,180,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	// d3d_entsort - 2 arrows - 1 2 5 6
	// string 16 17
	pan_setstring(mainmenu_settings_panel, 0,	15,200,	panel_font,			str_create("Ent sort:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,200,							"%0.f", 	panel_font, 	1, 	d3d_entsort);
	// button 16 17
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	//------------------------------------
	// d3d_alpharef - slider 0-255
	// string 18
	pan_setdigits(mainmenu_settings_panel, 0,	15,230,	"Alpha ref: %0.f",					panel_font,	1, 		d3d_alpharef);
	// slider 3 - 0-255
	pan_setslider(mainmenu_settings_panel, 0,	120+12,232,	horiz_slider_bmp,slider_knob_bmp,	0,255,			d3d_alpharef); 
	// button 18 19
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,230,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	240,230,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 
	
	// d3d_anisotropy - 2 arrows - 0-7
	// string 19 20
	pan_setstring(mainmenu_settings_panel, 0,	15,250,	panel_font,			str_create("Anisontrophy:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,250,							"%0.f", 	panel_font, 	1, 	d3d_anisotropy);
	// button 20 21
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,250,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,250,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	// d3d_mipmapping - 2 arrows - 0 1 3 4
	// string 21 22
	pan_setstring(mainmenu_settings_panel, 0,	15,270,	panel_font,			str_create("Mipmapping:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,270,							"%0.f", 	panel_font, 	1, 	d3d_mipmapping);
	// button 22 23
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,270,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,270,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	// d3d_antialias - 2 arrows - 0 1 4 9
	// string 23 24
	pan_setstring(mainmenu_settings_panel, 0,	15,290,	panel_font,			str_create("Antialias:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,290,							"%0.f", 	panel_font, 	1, 	d3d_antialias);
	// button 24 25
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,290,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,290,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	//------------------------------------
	// main_engineversion - 2 arrows - 0 1
	// string 25 26
	pan_setstring(mainmenu_settings_panel, 0,	15,320,	panel_font,			str_create("Engine edition:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,320,							"%0.f", 	panel_font, 	1, 	main_engineversion);
	// button 26 27
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,320,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,320,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	//------------------------------------
	// shadow_stencil - 2 arrows - -1 0 1 8
	// string 27 28
	pan_setstring(mainmenu_settings_panel, 0,	15,360,	panel_font,			str_create("Shadow stencil:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,360,							"%0.f", 	panel_font, 	1, 	shadow_stencil);
	// button 28 29
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,360,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,360,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	// shadow_lod - 2 arrows - 0 1 2 3
	// string 29 30
	pan_setstring(mainmenu_settings_panel, 0,	15,380,	panel_font,			str_create("Shadow lod:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,380,							"%0.f", 	panel_font, 	1, 	shadow_lod);
	// button 30 31
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,380,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,380,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	//------------------------------------
	// main_hdr - 2 arrows - 0 1
	// string 31 32
	pan_setstring(mainmenu_settings_panel, 0,	15,400,	panel_font,			str_create("Bloom1/Bloom2/HDR:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,400,							"%0.f", 	panel_font, 	1, 	main_hdr);
	// button 32 33
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,400,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,400,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	// main_blur - slider - 0 or 0.1-4 - under 0.1 should be rounded to 0 !!!
	// string 33
	pan_setdigits(mainmenu_settings_panel, 0,	15,420,	"Blur: %0.1f",					panel_font,	1, 		main_blur);
	// slider 4 - 0.1-4
	pan_setslider(mainmenu_settings_panel, 0,	120+12,422,	horiz_slider_bmp,slider_knob_bmp,	0,4,		main_blur); 
	// button 34 35
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,420,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	240,420,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 
	
	// main_camera_mode - 2 arrows - 0 1 2 3
	// string 34 35
	pan_setstring(mainmenu_settings_panel, 0,	15,440,	panel_font,			str_create("Camera mode:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,440,							"%0.f", 	panel_font, 	1, 	main_camera_mode);
	// button 36 37
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,440,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,440,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	//------------------------------------
	// button 38 - cancel to previous
	pan_setbutton(mainmenu_settings_panel, 0,	1, 		100,485,		s_cancel_on_bmp,s_cancel_off_bmp,s_cancel_over_bmp,s_cancel_over_bmp,									Main_Cancel_Settings		,NULL,NULL); 
	
	// button 39 - reset to defaults
	pan_setbutton(mainmenu_settings_panel, 0,	1, 		175,485,		s_reset_on_bmp,s_reset_off_bmp,s_reset_over_bmp,s_reset_over_bmp,											Main_Reset_Settings		,NULL,NULL); 
	
	//------------------------------------
	// main_water -1 0 1 2 3 4
	// string 36 37
	pan_setstring(mainmenu_settings_panel, 0,	15,460,	panel_font,			str_create("Water:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,460,							"%0.f", 	panel_font, 	1, 	main_water);
	// button 40 41
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,460,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,460,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
		
	//------------------------------------
	// main_terrain_lod 0 1 2 3 4
	// string 38 39
	pan_setstring(mainmenu_settings_panel, 0,	15,340,	panel_font,			str_create("Terrain LOD:") );
	pan_setdigits(mainmenu_settings_panel, 0,	145,340,							"%0.f", 	panel_font, 	1, 	main_terrain_lod);
	// button 42 43
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	120,340,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Main_Settings_Button, 	NULL, NULL); 
	pan_setbutton(mainmenu_settings_panel, 0,	1, 	165,340,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Main_Settings_Button, 	NULL, NULL); 																							
	
	//------------------------------------
}


void	Main_Settings_Panel_Close()
{
	if (mainmenu_settings_panel)
		{
			ptr_remove(mainmenu_settings_panel);
			mainmenu_settings_panel = NULL;
		}
}


void		Main_Settings_Button(button_number)
{
	wait(3);
	
	// resolution
	if (button_number==(var)2)
		{
			if ((main_resolution-1)>=(var)0)
				{
					int temp_int = main_resolution-1;
					if ((videores_table_x[temp_int] >= main_minresx) && (videores_table_y[temp_int] >= main_minresy))
						{
							main_resolution--;
							pan_setdigits(mainmenu_settings_panel, 3,	145,40,							"%0.f", 	panel_font, 	1, 	videores_table_x[main_resolution]);
							pan_setdigits(mainmenu_settings_panel, 5,	185,40,							"%0.f", 	panel_font, 	1, 	videores_table_y[main_resolution]);
						}
				}
		}
	else if (button_number==(var)3)
		{
			if ((main_resolution+1)<=(var)29)
				{
					int temp_int = main_resolution+1;
					if ((videores_table_x[temp_int] >= main_minresx) && (videores_table_y[temp_int] >= main_minresy))
						{
							main_resolution++;
							pan_setdigits(mainmenu_settings_panel, 3,	145,40,							"%0.f", 	panel_font, 	1, 	videores_table_x[main_resolution]);
							pan_setdigits(mainmenu_settings_panel, 5,	185,40,							"%0.f", 	panel_font, 	1, 	videores_table_y[main_resolution]);
						}
				}
		}
	//main_video_screen------------------
	else if (button_number==(var)4)
		{
			main_video_screen--;
			main_video_screen = maxv( 1 , main_video_screen );
		}
	else if (button_number==(var)5)
		{
			main_video_screen++;
			main_video_screen = minv( 2 , main_video_screen );
		}
	//fps_max----------------------------
	else if (button_number==(var)6)
		{
			fps_max--;
			fps_max = maxv( 20 , fps_max );
		}
	else if (button_number==(var)7)
		{
			fps_max++;
			fps_max = minv( 250 , fps_max );
		}
	//fps_min----------------------------
	else if (button_number==(var)8)
		{
			fps_min--;
			fps_min = maxv( 1 , fps_min );
		}
	else if (button_number==(var)9)
		{
			fps_min++;
			fps_min = minv( 50 , fps_min );
		}
	//preload_mode----------------------- 1 2 3 7 15
	else if (button_number==(var)10)
		{
			if (preload_mode==(var)15)
				{
					preload_mode = 7;
				}
			else if (preload_mode==(var)7)
				{
					preload_mode = 3;
				}
			else
				{
					preload_mode--;			
				}
			preload_mode = maxv( 1 , preload_mode );
		}
	else if (button_number==(var)11)
		{
			if (preload_mode==(var)7)
				{
					preload_mode = 15;
				}
			else if (preload_mode==(var)3)
				{
					preload_mode = 7;
				}
			else
				{
					preload_mode++;		
				}			
			preload_mode = minv( 15 , preload_mode );
		}
	// tex_share--------------------------------------
	else if (button_number==(var)12)
		{
			tex_share--;
			tex_share = maxv( 0 , tex_share );
		}
	else if (button_number==(var)13)
		{
			tex_share++;
			tex_share = minv( 1 , tex_share );
		}
	// d3d_triplebuffer--------------------------------
	else if (button_number==(var)14)
		{
			d3d_triplebuffer--;
			d3d_triplebuffer = maxv( 0 , d3d_triplebuffer );
		}
	else if (button_number==(var)15)
		{
			d3d_triplebuffer++;
			d3d_triplebuffer = minv( 1 , d3d_triplebuffer );
		}
	// d3d_entsort------------------------------------- 1 2 5 6
	else if (button_number==(var)16)
		{
			if (d3d_entsort==(var)5)
				{
					d3d_entsort = 2;
				}
			else
				{
					d3d_entsort--;			
				}
			d3d_entsort = maxv( 1 , d3d_entsort );
		}
	else if (button_number==(var)17)
		{
			if (d3d_entsort==(var)2)
				{
					d3d_entsort = 5;
				}
			else
				{
					d3d_entsort++;		
				}			
			d3d_entsort = minv( 6 , d3d_entsort );
		}
	// d3d_alpharef-----------------------------------
	else if (button_number==(var)18)
		{
			d3d_alpharef--;
			d3d_alpharef = maxv( 0 , d3d_alpharef );
		}
	else if (button_number==(var)19)
		{
			d3d_alpharef++;
			d3d_alpharef = minv( 255 , d3d_alpharef );
		}
	// d3d_anisotropy----------------------------------
	else if (button_number==(var)20)
		{
			d3d_anisotropy--;
			d3d_anisotropy = maxv( 0 , d3d_anisotropy );
		}
	else if (button_number==(var)21)
		{
			d3d_anisotropy++;
			d3d_anisotropy = minv( 7 , d3d_anisotropy );
		}
	// d3d_mipmapping--------------------------------- 0 1 3 4
	else if (button_number==(var)22)
		{
			if (d3d_mipmapping==(var)3)
				{
					d3d_mipmapping = 1;
				}
			else
				{
					d3d_mipmapping--;			
				}
			d3d_mipmapping = maxv( 0 , d3d_mipmapping );
		}
	else if (button_number==(var)23)
		{
			if (d3d_mipmapping==(var)1)
				{
					d3d_mipmapping = 3;
				}
			else
				{
					d3d_mipmapping++;		
				}			
			d3d_mipmapping = minv( 4 , d3d_mipmapping );
		}
	// d3d_antialias------------------------------ 0 1 4 9
	else if (button_number==(var)24)
		{
			if (d3d_antialias==(var)9)
				{
					d3d_antialias = 4;
				}
			else if (d3d_antialias==(var)4)
				{
					d3d_antialias = 1;
				}
			else
				{
					d3d_antialias--;			
				}
			d3d_antialias = maxv( 0 , d3d_antialias );
		}
	else if (button_number==(var)25)
		{
			if (d3d_antialias==(var)4)
				{
					d3d_antialias = 9;
				}
			else if (d3d_antialias==(var)1)
				{
					d3d_antialias = 4;
				}
			else
				{
					d3d_antialias++;		
				}			
			d3d_antialias = minv( 9 , d3d_antialias );
		}
	// main_engineversion--------------------------------
	else if (button_number==(var)26)
		{
			main_engineversion--;
			main_engineversion = maxv( 0 , main_engineversion );
			
			if (main_engineversion<1)	shadow_stencil = clamp(shadow_stencil, 0, 1);
		}
	else if (button_number==(var)27)
		{
			main_engineversion++;
			main_engineversion = minv( 2 , main_engineversion );
		}
	// shadow_stencil------------------------------------- -1 0 1 8
	else if (button_number==(var)28)
		{
			if (shadow_stencil==(var)8)
				{
					shadow_stencil = 1;
				}
			else
				{
					shadow_stencil--;			
				}
			shadow_stencil = maxv( -1 , shadow_stencil );
			
			if (main_engineversion<1)	shadow_stencil = clamp(shadow_stencil, 0, 1);
		}
	else if (button_number==(var)29)
		{
			if (shadow_stencil==(var)1)
				{
					shadow_stencil = 8;
				}
			else
				{
					shadow_stencil++;		
				}			
			shadow_stencil = minv( 8 , shadow_stencil );
			
			if (main_engineversion<1)	shadow_stencil = clamp(shadow_stencil, 0, 1);
		}
	// shadow_lod--------------------------------------------
	else if (button_number==(var)30)
		{
			shadow_lod--;
			shadow_lod = maxv( 0 , shadow_lod );
		}
	else if (button_number==(var)31)
		{
			shadow_lod++;
			shadow_lod = minv( 3 , shadow_lod );
		}
	// main_hdr----------------------------------------------
	else if (button_number==(var)32)
		{
			main_hdr--;
			main_hdr = maxv( 0 , main_hdr );
		}
	else if (button_number==(var)33)
		{
			main_hdr++;
			main_hdr = minv( 3 , main_hdr );
		}
	// main_blur------------------------------------------------ 0.1 4
	else if (button_number==(var)34)
		{
			main_blur -= 0.1;
			main_blur = maxv( 0 , main_blur );
		}
	else if (button_number==(var)35)
		{
			main_blur += 0.1;
			main_blur = minv( 4 , main_blur );
		}
	// main_camera_mode------------------------------------------ 0 1 2 3
	else if (button_number==(var)36)
		{
			main_camera_mode--;
			main_camera_mode = maxv( 0 , main_camera_mode );
		}
	else if (button_number==(var)37)
		{
			main_camera_mode++;
			main_camera_mode = minv( 1 , main_camera_mode );
		}
	// main_water----------------------------------------------- -1 0 1 2 3 4
	else if (button_number==(var)40)
		{
			main_water = maxv( -1 , --main_water );
		}
	else if (button_number==(var)41)
		{
			main_water = minv( 4 , ++main_water );
		}
	// main_terrain_lod----------------------------------------------- 0 1 2 3 4
	else if (button_number==(var)42)
		{
			main_terrain_lod = maxv( 0 , --main_terrain_lod );
		}
	else if (button_number==(var)43)
		{
			main_terrain_lod = minv( 4 , ++main_terrain_lod );
		}
}


/////////////////////////////////////////////////////


void	Main_Apply_Settings()
{
	wait(3);
	
	// rounding down because of slider
	if (main_blur<0.1)	main_blur = 0;
	
	// write actual settings to file
	Main_Write_Settings(main_currentres);
	wait_for(Main_Write_Settings);
	
	// store settings to previous - requires main_resolution to be set properly
	Main_Write_Settings(main_prevres);
	wait_for(Main_Write_Settings);
	
	// read and set actual settings from file - and set 
	Main_Read_Settings(main_currentres);
	wait_for(Main_Read_Settings);
	
	// close settings panel
	Main_Settings_Panel_Close();
	wait_for(Main_Settings_Panel_Close);
	
	// refresh non-created texts and panels
	Main_RefreshPanels();
	wait_for(Main_RefreshPanels);
}


void	Main_Cancel_Settings()
{
	wait(3);
	
	// rounding down because of slider
	if (main_blur<0.1)	main_blur = 0;
	
	// load back previously stored settings - and set
	Main_Read_Settings(main_prevres);
	wait_for(Main_Read_Settings);
	
	// write previous settings to actual settings file
	Main_Write_Settings(main_currentres);
	wait_for(Main_Write_Settings);
	
	// close settings panel
	Main_Settings_Panel_Close();
	wait_for(Main_Settings_Panel_Close);
	
	// refresh non-created texts and panels
	Main_RefreshPanels();
	wait_for(Main_RefreshPanels);
}


void	Main_Reset_Settings()
{
	wait(3);
	
	// rounding down because of slider
	if (main_blur<0.1)	main_blur = 0;
	
	// load back default settings stored
	Main_Read_Settings(main_defaultres);
	wait_for(Main_Read_Settings);
	
	// write default settings to actual settings file
	Main_Write_Settings(main_currentres);
	wait_for(Main_Write_Settings);
	
	// close settings panel - disables cancel after reset but it's okay
	Main_Settings_Panel_Close();
	wait_for(Main_Settings_Panel_Close);
	
	// refresh non-created texts and panels
	Main_RefreshPanels();
	wait_for(Main_RefreshPanels);
	
	// reopen settings panel to see defaults
	Main_Settings_Panel_Init();
}


/////////////////////////////////////////////////////


void	Main_Read_Settings(STRING* filename)
{
	// video settings
	STRING* temp_str = str_create("");
	var file_handle = file_open_read(filename);
	if (file_handle)
		{
			//---------------------------------------------------------------------
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
			var screen_x = maxv( main_minresx , str_to_num( temp_str ) );
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
			var screen_y = maxv( main_minresy , str_to_num( temp_str ) );
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
			var screen_type = minv( 2 , maxv( 1 , str_to_num( temp_str ) ) );			
			
			if (!video_set(screen_x,screen_y,0,screen_type))
				{
					video_set(sys_metrics(0), sys_metrics(1), 0, 1);
				}
			
//			// set shade-c shadowmapping resolution higher than current screen resolution to avoid rendering errors - later can be decreased, this initial setting is a must!
//			int i;
//			for (i=0; i<4; i++)
//				{
////					if (scsm_resolutions[i] >= sys_metrics(0))
//					if (scsm_resolutions[i] >= screen_size.x)
//						{
//							scsm_resolution = scsm_resolutions[i];
//							break;
//						}
//				}
			
			// to avoid engine freeze, because of warning messages in fullscreen mode
			if (video_screen == 1)
				{
					warn_level = 0;		// no warnings
				}
			else
				{
					warn_level = 1;		// 1 basic warnings, 2 textures/functions, 3 missing files, 4-6 more
												// but over 1 it is useless, too many invalid file name warnings if they contain path
				}
			
			//--------------------------------------------------------------------
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				fps_max = str_to_num( temp_str );
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				fps_min = str_to_num( temp_str );
				
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				preload_mode = str_to_num( temp_str );
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				tex_share = str_to_num( temp_str );
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				d3d_triplebuffer = str_to_num( temp_str );
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				d3d_entsort = str_to_num( temp_str );
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				d3d_alpharef = str_to_num( temp_str );
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				d3d_anisotropy = str_to_num( temp_str );
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				d3d_mipmapping = str_to_num( temp_str );
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				d3d_antialias = str_to_num( temp_str );		// better not to use
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				main_engineversion = str_to_num( temp_str );
			
			// limit features by actual compiler edition
			if ((edition == (var)1) || (edition == (var)2))
				{
					main_engineversion = 0;
				}
			else if (edition == (var)3)
				{
					main_engineversion = maxv(main_engineversion, 1);
				}			
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
			if (main_engineversion>0)
				{
					shadow_stencil = str_to_num( temp_str );
				}
			else
				{
					shadow_stencil = clamp(str_to_num( temp_str ), 0, 1);
				}
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				shadow_lod = str_to_num( temp_str );
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
			if (main_engineversion>0)
				{
					main_hdr = str_to_num( temp_str );
				}
			else
				{
					main_hdr = 0;
				}				
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				main_blur = str_to_num( temp_str );
			
			if (
					(main_engineversion>0)
					&&
					(main_blur>0)
					&&
					(shadow_stencil>0)
//					&&
//					(shadow_stencil<8)
				)
				{
					// do not run now - not okay with antialias !!! - but okay if both antialias and hdr set
//					stencil_blur(main_blur); 
					
					if (!main_hdr)
						{
							d3d_antialias = 0;
						}
				}
			else
				{
					main_blur = 0;
				}
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				main_camera_mode = str_to_num( temp_str );			
			
			file_str_read( file_handle , NULL );
			file_str_read( file_handle , temp_str );
				main_water = str_to_num( temp_str );	
			
			if (main_engineversion<1)
				{
					main_water = -1;
				}
			
			//-------------------------
			// update 234m5
						
			var eof = file_str_read( file_handle , NULL );
			
			if (eof >= (var)0)
				{
					file_str_read( file_handle , temp_str );
						var temp_lod = str_to_num( temp_str );	
					
					if ((main_engineversion == (var)2))// && (edition == (var)4))
						{					
							main_terrain_lod	= temp_lod;																			
						}
					else
						{
							main_terrain_lod 	= 0;
						}	
				}	
			// else keep default
			
			// set pro edition only features: sprite instancing and terrain lod
			if ((main_engineversion == (var)2))// && (edition == (var)4))
				{
					d3d_instancing = 1; 							// activate sprite instancing - Pro
					terrain_lod 	= main_terrain_lod; 		// activate terraincache and lod - Pro		
																		// terrain scaling problems - solved	
																		// high slope terrain requires lower lod value to avoid gaps
																		// >1 okay with shadow_stencil -1, 0, 1
																		// >1 not okay with shadow_stencil 8		(together with shadow lod i.e. pssm_lod)
				}
			else
				{
					d3d_instancing = 0;
					terrain_lod 	= 0;
				}						
			
			//-------------------------
			
			file_close(file_handle);
		}
	else
		{
//			printf("File not found!");
			
//			video_mode = 8;					// default is 7 but it is too small for menu panels
//			video_screen = 1;					// 1 fullscreen	
			// or get desktop resolution and use fullscreen mode
			video_set(sys_metrics(0), sys_metrics(1), 0, 1);	
			
			fps_max 				= 40; 		// limit the frame rate - slows it down a bit...
			fps_min 				= 20; 		// avoid huge moves in case of very low frame rate	
			
			preload_mode 		= 3+4;		// 3+4+8; preload level entities, created and cloned entities, and precalculate their environment light
   		tex_share 			= 1; 			// same entities share texture memoy		
   		
   		d3d_triplebuffer 	= 1; 			// activate Triple Buffering - more memory needed but higher frame rate achievable	
			d3d_entsort 		= 2;			// 2: sort transparent entities based on XY distance from camera instead of XYZ distance - or 6 for all the enities
			d3d_alpharef 		= 100;		// overlay transparency treshold level 0..255, 75 was okay, default=127, TreeIT max 125, TreeMagikG3 150 but tree dependent
		//	if (d3d_caps & 2) 				// good cards only 
			d3d_anisotropy 	= 4; 			// 2, 1..7 anisotropic filtering active - needed by anisotropic mipmapping
			d3d_mipmapping 	= 4;			// anisotropic mipmapping - much better terrain texture - 3 or 4
			d3d_antialias 		= 0; 			// activate scene antialiasing 0 1 4 9
			
			main_engineversion = 2;			// 1 commercial, 2 pro edition
			
			if ((main_engineversion == (var)2))				// && (edition == (var)4))
				{
					d3d_instancing = 1; 							// activate sprite instancing - Pro
					terrain_lod 	= main_terrain_lod; 		// activate terraincache and lod - Pro		
																		// terrain scaling problems - solved	
																		// high slope terrain requires lower lod value to avoid gaps
																		// >1 okay with shadow_stencil -1, 0, 1
																		// >1 not okay with shadow_stencil 8		(together with shadow lod i.e. pssm_lod)
				}
			else
				{
					d3d_instancing = 0;
					terrain_lod 	= 0;
				}
			
			shadow_stencil 	= 8;			// -1, 0, 1, (2,) 8
			shadow_lod 			= 0;			// shifts stencil shadows by x lod stage(s) and clips shadows if actual lod+shift is over 3
			
			main_water			= 4;			// -1..4
			main_hdr 			= 0;			// 0 off, 1 bloom, 2 bloom-blur, 3 hdr
			main_blur 			= 0;			// no blur			
			main_camera_mode	= 0;			// all ent_decals
						
		}
	ptr_remove(temp_str);
	
	main_video_screen = video_screen;			
	
	// set shade-c shadowmapping resolution higher than current screen resolution to avoid rendering errors - later can be decreased, this initial setting is a must!
	int i;
	for (i=0; i<4; i++)
		{
//			if (scsm_resolutions[i] >= sys_metrics(0))
			if (scsm_resolutions[i] >= screen_size.x)
				{
					scsm_resolution = scsm_resolutions[i];
					break;
				}
		}
}


void	Main_Write_Settings(STRING* filename)
{
	// video settings
	STRING* temp_str = str_create("");
	var file_handle = file_open_write(filename);
	if (file_handle)
		{
			//----------------------------------------------------------------
			file_str_write( 		file_handle , "// resolution x" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, videores_table_x[main_resolution] );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
		
			file_str_write( 		file_handle , "// resolution y" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, videores_table_y[main_resolution] );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// video_screen: 1 full / 2 window" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, main_video_screen );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			//--------------------------------------------------------------
			file_str_write( 		file_handle , "// fps_max" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, fps_max );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// fps_min" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, fps_min );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// preload_mode: 1 3 7 15" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, preload_mode );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// tex_share: 0 off / 1 on" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, tex_share );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// d3d_triplebuffer: 0 off / 1 on" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, d3d_triplebuffer );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_str_write( 		file_handle , "// d3d_entsort: 1 2 5 6" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, d3d_entsort );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_str_write( 		file_handle , "// d3d_alpharef: 0_255" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, d3d_alpharef );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// d3d_anisotropy: 0_7" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, d3d_anisotropy );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// d3d_mipmapping: 0 1 3 4" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, d3d_mipmapping );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
				
			file_str_write( 		file_handle , "// d3d_antialias: 0 1 4 9" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, d3d_antialias );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// engine edition: 0 free/extra / 1 comm/pro" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, main_engineversion );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// shadow_stencil: -1 shade-c / 0 decal / 1 declal and stencil / 8 pssm" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, shadow_stencil );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// shadow_lod: 0 off / 1 2 3 shift" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, shadow_lod );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// hdr: 0 off / 1 on" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, main_hdr );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// blur: 0 off / 01-4 on" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, main_blur );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			
			file_str_write( 		file_handle , "// decal mode: 0 all ent_decal / 1 shrub decal shadow / 2 obstacle decal shadow / 3 mapentity decal shadow" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, main_camera_mode );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);			
			
			file_str_write( 		file_handle , "// water: -1 texture water / 0 default mirror water / 1-2 basic shade-c water / 3-4 shade-c water with dynamic depth" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, main_water );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);	
			
			file_str_write( 		file_handle , "// terrain lod: 0 no lod / 1 terrain cache (Pro) / 2-4 use terrain lod stages 1-3 (Pro)" );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);
			str_for_num( 			temp_str 	, main_terrain_lod );
			file_str_write( 		file_handle , temp_str );
				file_asc_write(	file_handle , 13); 
				file_asc_write(	file_handle , 10);	
				
			file_close(file_handle);
		}
	ptr_remove(temp_str);	
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////


void	MainMenu_Map()
{
	mainmenu_whattoload = 0;
	
	MainMenu_Load_Init();
}


void	MainMenu_Trr()
{
	mainmenu_whattoload = 1;
	
	MainMenu_Load_Init();
}


void	MainMenu_Wmb()
{
	mainmenu_whattoload = 2;
	
	MainMenu_Load_Init();
}


void	MainMenu_Hmp()
{
	mainmenu_whattoload = 3;		// switch to 4 to water hmp selection, after ground hmp selection done
	
	MainMenu_Load_Init();
}


void	MainMenu_HmpW()
{
	mainmenu_whattoload = 4;
	
	MainMenu_Load_Init();
}


void	MainMenu_Null()
{
	wait(3);
	
	// close main menu panel
	MainMenu_Close();
	
	// load an empty level immediately
	Map_Init_NULL(map_emptyname);		
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////


void MainMenu_Load_Init()
{
sys_marker("L01");

	wait(3);
	
	int i;	
	
	//----------------------------------------------------------------
	// displayed filename string array initialization
	
	int arraysize = MAINMENU_MAXPERSET;
	mainmenu_filenames = (STRING**)sys_malloc( arraysize * sizeof(STRING*) );
		
	for(i=0; i<arraysize; i++)
		{
			mainmenu_filenames[i] = str_create("");
		}
	
	//-----------------------------------------------------------------
	// determine how many .trr files are in the folder
	
sys_marker("L02");	
	
	mainmenu_files_txt = txt_create( (var)MAINMENU_MAXFILES , (var)0 );
	for(i=0; i<MAINMENU_MAXFILES; i++)
		{
			str_cpy( (mainmenu_files_txt->pstring)[i] , "" );
		}	
	
	if (mainmenu_whattoload == 0)
		{
			i = txt_for_dir(mainmenu_files_txt, filemenu_terrains_filter); 					// folder + filter
		}
	else if (mainmenu_whattoload == 1)
		{
			i = txt_for_dir(mainmenu_files_txt, filemenu_terrains_filter); 					// folder + filter
		}
	else if (mainmenu_whattoload == 2)
		{
			i = txt_for_dir(mainmenu_files_txt, filemenu_wmb_filter); 							// folder + filter
		}
	else if (mainmenu_whattoload == 3)
		{
			i = txt_for_dir(mainmenu_files_txt, filemenu_hmp_filter); 							// folder + filter
			
			if (mainmenu_hmp_filename == NULL)
				{
					mainmenu_hmp_filename = str_create("");
				}
			else
				{
					str_cpy(mainmenu_hmp_filename, "");
				}
		}
	else if (mainmenu_whattoload == 4)
		{
			i = txt_for_dir(mainmenu_files_txt, filemenu_hmpw_filter); 						// folder + filter
			
			if (mainmenu_hmpw_filename == NULL)
				{
					mainmenu_hmpw_filename = str_create("");
				}
			else
				{
					str_cpy(mainmenu_hmpw_filename, "");
				}
		}
	
//	i = 0;
//	while (str_len((mainmenu_files_txt->pstring)[i]) > 0)										// or i = txt_for_dir(filemenu_files_txt, filemenu_filter); 
//		{
//			i++;
//		}
	
	//-----------------------------------------------------------------
	// parameters for one by one file name scrolling
	
	mainmenu_select					= 1;															// selected item - must be reset
	
	mainmenu_actual_first_item	= 0;	
	mainmenu_all_items				= i;
	mainmenu_last_item				= maxv(0, mainmenu_all_items - (var)MAINMENU_MAXPERSET);
	mainmenu_itemslider				= 0;
	
	//------------------------------------------------------------------
	// copy first 8 filenames to string array
	
sys_marker("L03");
		
	for(i=0; i<MAINMENU_MAXPERSET; i++)
		{
			str_cpy( mainmenu_filenames[i] , "" );													
			str_cat( mainmenu_filenames[i] , (mainmenu_files_txt->pstring)[i] );			// filename with extension
			
			if ( str_cmpi( mainmenu_filenames[i] , "" ) )										
				{
					str_cpy( mainmenu_filenames[i] , "-" );
				}
		}
	
	//----------------------------------------------------------------
	// create panel
	
sys_marker("L04");
		
	mainmenu_load_panel = pan_create( panel_image , 11 );
	set( mainmenu_load_panel, SHADOW | OVERLAY | SHOW );
	mainmenu_load_panel.pos_x = screen_size.x/2 - 128;
	mainmenu_load_panel.pos_y = screen_size.y/2 - 128;
	mainmenu_load_panel.red			= panel_font_r;
	mainmenu_load_panel.green		= panel_font_g;
	mainmenu_load_panel.blue		= panel_font_b;
	
	//------------------------------------------------------------------
	// panel items
	
sys_marker("L05");
		
	// 8 filename strings to display in menu
	for(i=0; i<MAINMENU_MAXPERSET; i++)
		{
			pan_setstring(mainmenu_load_panel, 0,	25,24+2+i*25,	panel_font,			str_create( mainmenu_filenames[i] ) );
		}
			
	// 8 radio buttons under filenames, buttons 1-8
	for(i=0; i<MAINMENU_MAXPERSET; i++)
		{
			pan_setbutton(mainmenu_load_panel, 0,	4, 	10,24+i*25,		fileselect_on_bmp, fileselect_off_bmp, fileselect_over_bmp, fileselect_over_bmp,		MainMenu_Load_Select,		NULL, NULL); 
		}
	// set first file button
	button_state( mainmenu_load_panel , 1 , ON );
	
	// Cancel button - button 9
	pan_setbutton(mainmenu_load_panel, 0,	1, 	128+24,224,		s_cancel_on_bmp, s_cancel_off_bmp, s_cancel_over_bmp, s_cancel_over_bmp,						MainMenu_Load_Close, 		NULL, NULL); 

sys_marker("L06");
	
	// title
	if (mainmenu_whattoload == 0)
		{
			pan_setstring(mainmenu_load_panel, 0,	( mainmenu_load_panel.size_x/2-str_width("LOAD MAP [.trr]",panel_font2)/2),3,	panel_font2,				str_create("LOAD MAP [.trr]") );
		}
	else if (mainmenu_whattoload == 1)
		{
			pan_setstring(mainmenu_load_panel, 0,	( mainmenu_load_panel.size_x/2-str_width("LOAD TERRAIN [.trr]",panel_font2)/2),3,	panel_font2,		str_create("LOAD TERRAIN [.trr]") );
		}
	else if (mainmenu_whattoload == 2)
		{
			pan_setstring(mainmenu_load_panel, 0,	( mainmenu_load_panel.size_x/2-str_width("LOAD LEVEL [.wmb]",panel_font2)/2),3,	panel_font2,			str_create("LOAD LEVEL [.wmb]") );
		}
	else if (mainmenu_whattoload == 3)
		{
			pan_setstring(mainmenu_load_panel, 0,	( mainmenu_load_panel.size_x/2-str_width("ADD GROUND [.hmp]",panel_font2)/2) ,3,	panel_font2,			str_create("ADD GROUND [.hmp]") );
		}
	else if (mainmenu_whattoload == 4)
		{
			pan_setstring(mainmenu_load_panel, 0,	( mainmenu_load_panel.size_x/2-str_width("ADD WATER [.hmp]",panel_font2)/2),3,	panel_font2,			str_create("ADD WATER [.hmp]") );
		}
	
sys_marker("L07");
	
	// scrollbar arrows
	pan_setbutton(mainmenu_load_panel, 0,	1, 	226,24,		s_arrow_up_on_bmp, s_arrow_up_off_bmp, s_arrow_up_on_bmp, s_arrow_up_off_bmp,						MainMenu_Load_DecSet, 		NULL, NULL); 
	pan_setbutton(mainmenu_load_panel, 0,	1, 	226,192,		s_arrow_down_on_bmp, s_arrow_down_off_bmp, s_arrow_down_on_bmp, s_arrow_down_off_bmp,			MainMenu_Load_IncSet, 		NULL, NULL); 	

sys_marker("L08");	

	// scrollbar slider 		
	if (mainmenu_last_item >= (var)1)
		{
			// scale slider bar image
			mainmenu_slider_bmap = bmap_createblack(20, 25*(var)MAINMENU_MAXPERSET-75, 32);			
			wait(1);
			bmap_blitpart(mainmenu_slider_bmap, vert_bar_bmp, NULL, vector(20, 25*(var)MAINMENU_MAXPERSET-75, 0), NULL, NULL);
			wait(1);
			
			// item based stepping
			pan_setslider(mainmenu_load_panel, 0,	228,54,				mainmenu_slider_bmap,vert_bar_knob_bmp,		0,mainmenu_last_item,	mainmenu_itemslider);
//			pan_setslider(mainmenu_load_panel, 0,	228,54,				vert_bar_bmp,vert_bar_knob_bmp,			0,mainmenu_last_item,	mainmenu_itemslider);			// original unscaled for 8 items
		}
	
sys_marker("L09");	

	// Ok button - button 12
	pan_setbutton(mainmenu_load_panel, 0,	1, 	48,224,		s_ok_on_bmp, s_ok_off_bmp, s_ok_over_bmp, s_ok_over_bmp,				MainMenu_Load_Load, 		NULL, NULL); 	 
	
	//--------------------------------------------------------------------
	
sys_marker("L10");
	
	while( mainmenu_load_panel )	
		{			
			// file names displayed
			if (mainmenu_last_item >= (var)1)
				{
					if ( (mainmenu_itemslider > mainmenu_actual_first_item + 0.5) || (mainmenu_itemslider < mainmenu_actual_first_item - 0.5 ) )
						{					
							mainmenu_actual_first_item = integer(mainmenu_itemslider + 0.5);
							mainmenu_actual_first_item = clamp(mainmenu_actual_first_item, 0, mainmenu_last_item);
							
							MainMenu_Load_ChangeSet();	
						}
				}
			
			wait(1);
		}
	
	//--------------------------------------------------------------------
	
//	ptr_remove(temp_str);

sys_marker(NULL);	
}


////////////////////////////////////////////////////


void MainMenu_Load_Close()
{
	if (proc_status(MainMenu_Load_Close) > 0) return;
	
	wait(3);
	
	//-------------------
	// remove load panel
	
	ptr_remove(mainmenu_load_panel);
	mainmenu_load_panel = NULL;
	
	//-------------------
	// remove text
	
	int i;
	for (i = 0; i < mainmenu_files_txt->strings; i++)
		{
			if ((mainmenu_files_txt->pstring)[i]) 
				{
					ptr_remove((mainmenu_files_txt->pstring)[i]);
				}
		}
	
	ptr_remove(mainmenu_files_txt);
	mainmenu_files_txt = NULL;	
	
	//-------------------
	// remove string array
	
	for(i=0; i<MAINMENU_MAXPERSET; i++)
		{
			ptr_remove(mainmenu_filenames[i]);
		}
		
	sys_free(mainmenu_filenames);
	mainmenu_filenames = NULL;
	
	//-------------------
	// remove scroll bar image
	
	if (mainmenu_slider_bmap != NULL)
		{
			ptr_remove(mainmenu_slider_bmap);
			mainmenu_slider_bmap = NULL;
		}
	
	//-------------------
	// on water hmp close - create a terrain without water
	
	if (mainmenu_whattoload == 4)
		{
			// close main menu
			MainMenu_Close();
			
			// on close or on load, initialize terrain and map data:
			Map_Init_hmp( _chr(mainmenu_hmp_filename) , _chr(mainmenu_hmpw_filename) , (var)1);																			
			wait_for(Map_Init_hmp);
			
			ptr_remove(mainmenu_hmp_filename);
			mainmenu_hmp_filename = NULL;
			
			ptr_remove(mainmenu_hmpw_filename);						
			mainmenu_hmpw_filename = NULL;
		}
	
	//-------------------
}


void MainMenu_Load_Load()
{
	if (proc_status(MainMenu_Load_Load) > 0) return;
	
	wait(3);
	
	//-----------------------------------------------------------------
	
	var button_number = mainmenu_select;
	if ( str_cmpi(mainmenu_filenames[button_number-1],"-") )
		{
			return;
		}	
		
	//-----------------------------------------------------------------
	// assemble file name, foldername + filename
	
	STRING* temp_str = str_create("");
	
	if (mainmenu_whattoload == 0)
		{
			str_cpy( temp_str , map_foldername );
		}
	else if (mainmenu_whattoload == 1)
		{
			str_cpy( temp_str , trr_foldername );
		}
	else if (mainmenu_whattoload == 2)
		{
			str_cpy( temp_str , lev_foldername );
		}
	else if (mainmenu_whattoload == 3)
		{
			str_cpy( temp_str , hmp_foldername );
		}
	else if (mainmenu_whattoload == 4)
		{
			str_cpy( temp_str , hmpw_foldername );
		}			
	
	str_cat( temp_str , mainmenu_filenames[button_number-1] );
	
	if (!file_exists(temp_str))
		{
			return;
		}	
		
	//-----------------------------------------------------------------
	// hide non-created panels
	
	if (mainmenu_whattoload != 3)
		{
			reset( mainmenu_load_panel, SHOW );
			reset( mainmenu_backpanel, SHOW );
			reset( mainmenu_shadecpanel, SHOW );	
			reset( mainmenu_panel, SHOW );
			reset( mainmenu_titletext, SHOW);
			reset( mainmenu_copytext, SHOW);
			
			wait(3);
		}
	
	//-----------------------------------------------------------------
	
	if (mainmenu_whattoload == 0)
		{
			FileMenu_Load_All( temp_str );				
			
			// close load menu panel
			MainMenu_Load_Close();
		}
	else if (mainmenu_whattoload == 1)
		{
			// load trr			
			Map_LoadMap_trr( temp_str );			
			
			// close load menu panel
			MainMenu_Load_Close();
		}
	else if (mainmenu_whattoload == 2)
		{
			// load wmb
			Map_Init_wmb( temp_str , 1 );											// 1 to create new
			
			// close load menu panel
			MainMenu_Load_Close();
		}
	else if (mainmenu_whattoload == 3)
		{
			// store ground hmp filename
			str_cpy( mainmenu_hmp_filename , temp_str );
			
			// close load menu panel
			MainMenu_Load_Close();
			wait_for(MainMenu_Load_Close);										// needed, because menu is re-initialized immediately by MainMenu_HmpW
			
			// open load menu panel again in water hmp mode
			MainMenu_HmpW();
		}
	else if (mainmenu_whattoload == 4)
		{
			// store water hmp filename
			str_cpy( mainmenu_hmpw_filename , temp_str );
			
			// loading is handled in MainMenu_Load_Close() !!!
			
			// close load menu panel
			MainMenu_Load_Close();
		}		
	
	//-----------------------------------------------------------------
	// close main menu panel
	
	if (mainmenu_whattoload != 4)													// it is case 3, but it was set to 4 by MainMenu_HmpW() just before it !!!
		{
			MainMenu_Close();
		}
	
	//-----------------------------------------------------------------
		
	wait(3);
	ptr_remove(temp_str);
	
	//-----------------------------------------------------------------
}


////////////////////////////////////////////////////


void	MainMenu_Load_IncSet()
{
	// item based stepping
	mainmenu_itemslider = clamp(mainmenu_itemslider+1, 0, mainmenu_last_item);					// update is done in panel while loop		
}


void	MainMenu_Load_DecSet()
{
	// item based stepping
	mainmenu_itemslider = clamp(mainmenu_itemslider-1, 0, mainmenu_last_item);					// update is done in panel while loop
}


void	MainMenu_Load_ChangeSet()			//(button_number)
{
	// cannot change if only 1 set exists
	if (mainmenu_last_item < (var)1)
		{
			return;
		}	
	
	// item based stepping
	int k = mainmenu_actual_first_item;
	int i;  
	
	// refresh strings
	for(i=0; i<MAINMENU_MAXPERSET; i++)
		{
			str_cpy( mainmenu_filenames[i] , "" );												
			str_cat( mainmenu_filenames[i] , (mainmenu_files_txt->pstring)[k+i] );			// filename with extension
			if ( str_cmpi(mainmenu_filenames[i],"") )												
				{
					str_cpy( mainmenu_filenames[i] , "-" );
				}
		}	
		
	// refresh 8 filename panel strings to display
	for(i=0; i<MAINMENU_MAXPERSET; i++)
		{
			pan_setstring(mainmenu_load_panel, i+1,	25,24+2+i*25,	panel_font,			str_create( mainmenu_filenames[i] ) );
		}	
}


////////////////////////////////////////////////////


void	MainMenu_Load_Select(button_number)	
{
	mainmenu_select = button_number;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif