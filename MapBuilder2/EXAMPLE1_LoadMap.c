//////////////////////////
// game startup scripts //
//////////////////////////

////////////////////////////////////////
// system includes

#include <acknex.h>		// gamestudio main include file
#include <default.c>		// modified version is used in editor only, required to show statistics and simple camera movements
#include <d3d9.h>			// required for calling DirectX functions from lite-c
//#include <level.c>		// splash screen and terrain functions - replaced with own functions in teredit.c - nearly similar
#include <mtlFX.c>		// used by engine default object shaders
#include <windows.h>		// to access GetTickCount for precision timer
#include <strio.c>		// Win API I/O functions - string handling, (win file dialogs) - needed by functions used in wmb compilation
//#include <shadows.c>	// pssm - modified version used: MBshadows.c
//#include <mtlView.c> 	// contains the stencil_blur() function (not okay with decal shadows, only with ent_decals), and mtl_hdr - which is customized in MBMtlView.c
#include <keys.c>			// needed by key_hit

////////////////////////////////////////
// MapBuilder2 project include

#define MB_GAME			// very important! this allows to call game specific functions only instead of editor functions!

#define PRAGMA_PATH "MapBuilder_scripts"
#include "MapBuilder2.h"		
#include "MapBuilder2.c"

// please note there is a similar named .wdl file containing some very imprtant settings
// namely paths to subfolder resources required by loading wmb levels
// NULL levels can be loaded without making a similar wdl file
// it is exactly the same as MapBuilder.wdl but should have the same name as your main .c file

////////////////////////////////////////
// functions

void	main();																		// startup
	void	MB_InitialSettings();												// MB initial settings
	void	MB_Load_FullMap(STRING* mapfilename);							// MB loader function example
	void	MB_SetReset_Shadowmapping(var* pssm_state, var* temp_scsm_depth, var* temp_stencil_blur, var* temp_window);		// set/reset shadowmapping to avoid bad shadows after alt+tab
	
////////////////////////////////////////

// your game script include - see in other example projects and tutorials
//#include "your_game_script.c"
//#include "your_camera_script.c"

////////////////////////////////////////
// code

void main()
{
	// window settings
	video_window(NULL,NULL,16+32+64,"MapBuilder2-Example-1");		// border+title (default), +close, +minimize
	
	//-------------------------------------------------------------------------
	// set initial parameters similarly as in MapBuilder editor
	
	MB_InitialSettings();
	wait_for(MB_InitialSettings);		// needed because it loads an empty level for setting nexus high enough
												// which is not essential for smaller levels where the default 40MB nexus is enough
	
	//-------------------------------------------------------------------------
	// put here game startup menu for selecting level or settings...
	
	// ...
	
	//------------------------------------------------------------------------
	// hide loading screen by showing here a panel (and text) - now only a text appears - customize it!
	
	set(map_loadtext1,SHOW);
	
	//-------------------------------------------------------------------------
	// customiize what to load:
	// by default all existing files of a full map are loaded,
	// but if a value is set to 0, then it will be skipped
	
//	map_load_details			= 0;								// grass sprites
//	map_load_shrubs			= 0;								// shrub sprites
//	map_load_grass				= 0;								// grass models
//	map_load_obstacles		= 0;								// obstacle models
//	map_load_mapents			= 0;								// map entities
//	map_load_terrmdls			= 0;								// terrain models
//	map_load_terrdeform		= 0;								// terrain vertices
//	map_load_sky				= 0;								// sky-sun-camera-lod-fog-shadow settings
//	map_load_materials		= 0;								// materials
//	map_load_watershaders	= 0;								// water shader settings
//	map_load_terrtext			= 0;								// terrain textures
//	map_load_terrlm			= 0;								// terrain lightmap
	
	//-------------------------------------------------------------------------
	// level loading:
	// it creates a NULL level and creates a ground terrain, and optionally a water terrain,
	// or alternatively loads a wmb level containing arbitrary entities,
	// afterwards it loads entities using ent_create from a couple of entity map files if exist,
	// then it loads/sets materials, sky-Sun-camera-lod-fog-shadow parameters, shadowmapping, and finally water shaders
	
	// 1st type
	// a full map consisting of some desert vegetation and building entities, using replaced terrain textures, plus material and sky settings
	MB_Load_FullMap("dry_land.trr");
//	// or
//	// a full map of ground and water terrains, a few buildings, lot of trees, and grass covered middle area
//	MB_Load_FullMap("mapbuilder2_level.trr");
	
//	// 2nd type
//	// a full map consisting of a wmb level and some additional map-entities stored separately, plus metarial and sky settings
//	MB_Load_FullMap( "dry_level.trr" );
	
//	// 3rd type
//	// a single wmb level exported from MapBuilder2.3 having an additional info file containing shadow and fog information
//	// but ordinary wmb levels also can be loaded in this way
////	MB_Load_FullMap( "test.wmb" );
//	if ((edition > 2) || (edition < 0))
//		{
//			MB_Load_FullMap( "mapbuilder2_level_scsm.wmb" );
//		}
//	else
//		{
//			MB_Load_FullMap( "mapbuilder2_level_free.wmb" );
//		}
		
	// wait until everything is loaded 
	wait_for(MB_Load_FullMap);
	
	//-------------------------------------------------------------------------
	// start your camera and game system scripts
	
	vec_set(camera.x,vector(0,0,2000));
	vec_set(camera.pan,vector(230,-20,0));
	
	// free camera moving by calling 3DGS default.c, the same as pressing button '0'
	def_move();								
	
	//--------------------------------------------------------------------------
	// hide here the loading panel - now only hides the text - start the game
	wait(3);
	reset(map_loadtext1,SHOW);	
	
//	// test menu - in windowed mode only
//	long hMenu = CreateMenu();
//	long hSubMenu = CreateMenu();
//	InsertMenu(hSubMenu,1,MF_BYPOSITION|MF_STRING,1,"Function1");
//	InsertMenu(hSubMenu,2,MF_BYPOSITION|MF_STRING,2,"Function2");
//	InsertMenu(hMenu,0,MF_BYPOSITION|MF_STRING|MF_POPUP,hSubMenu,"Submenu");
//	SetMenu(hWnd,hMenu);
	
	//--------------------------------------------------------------------------
	// ui loop
	
	var screencounter = 0;
	
	var pssm_state = 0;														// required to enable switching on/off
	var temp_scsm_depth = 0;												// required to enable switching on/off
	var temp_stencil_blur = 0;												// required to enable switching on/off
	var temp_window = 1;														// required to switch shadowmapping on/off when window gets active/inactive
	
	while(1)	
		{
			// required to switch shadowmapping on/off when window gets active/inactive to avoid shadow artifacts
			MB_SetReset_Shadowmapping(&pssm_state, &temp_scsm_depth, &temp_stencil_blur, &temp_window);
			
			if (key_k)
				{
					while (key_k) {wait (1);}
					str_cpy(save_dir,"screenshots");
					file_for_screen( "shot.jpg" , screencounter ); 	
					screencounter++;
				}
			
//			vec_set(mouse_pos,mouse_cursor);	
					
			wait(1);
		}		
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////

void MB_InitialSettings()
{
	UIMode = (var)UIMODE_INIT;				// to not to display loading bar of the first dummy NULL level used for nexus setting
	
	// avoid 3dgs calls of default.c
//	on_esc = Key_Do_Nothing;
	on_tab = Key_Do_Nothing;
	on_f1  = Key_Do_Nothing;
	on_f2  = Key_Do_Nothing;
	on_f3  = Key_Do_Nothing;
	on_f4  = Key_Do_Nothing;
	on_f5  = Key_Do_Nothing;
	on_f6  = Key_Do_Nothing;
	on_f7  = Key_Do_Nothing;
	on_f8  = Key_Do_Nothing;
	on_f9  = Key_Do_Nothing;
	on_f10 = Key_Do_Nothing;
//	on_f11 = Key_Do_Nothing;
	on_f12 = Key_Do_Nothing;
	on_0   = Key_Do_Nothing;
	
	compatibility = 13;					// show different kinds of memory sizes as A7 in kB instead of MB for better precision
	
	on_exit = Exit_Event;				// error message cancel button - it is run one frame before shut down
	on_close = Main_Exit;				// close icon of title bar in windowed mode
	
	max_entities  = 100000;				// 200000 results in a surpressed nexus overflow on level load, 100000 not but leads to nexus overflow on design time
	max_particles = 100000;				// 100000 decal shadows use particles
	clip_particles = 0.25;
	d3d_near_decals = 0.999;			// 1 : to not to leave decals to cover entities, disable ZNEAR, decal shadow flickering
	
	map_subents = 1;						// place wmb map entity sub-enitites too
	
	if (d3d_texlimit>4096)
		d3d_texlimit = 4096;				// limit max texture size
	
	time_smooth = 0.9;					// time_step is average of last x frames, 0.9 = 10 frames, 0.666 = 3 frames , 0.5 = 2 frames 
	
	fps_max = 50; 					// limit the frame rate - slows it down a bit...
	fps_min = 16; 					// avoid huge moves in case of very low frame rate
	
	// resolution
//	video_set(sys_metrics(0), sys_metrics(1), 0, 1);	// keep desktop resolution, fullscreen
	video_set(800,600,0,2);
		
	preload_mode 	= 3+4; 		// preload level entities, created and cloned entities, and precalculate their environment light
   tex_share 		= 1; 			// same entities share texture memoy	
	
	d3d_triplebuffer 	= 1; 		// activate Triple Buffering - more memory needed but higher frame rate achievable	
	d3d_entsort 		= 2;		// 2: sort transparent entities based on XY distance from camera instead of XYZ distance - or 6 for all the enities
	d3d_alpharef 		= 75;		// overlay transparency treshold level 0..255, default=127
	d3d_anisotropy 	= 2; 		// 2, 1..7 anisotropic filtering active - needed by anisotropic mipmapping
	d3d_mipmapping 	= 4;		// anisotropic mipmapping - much better terrain texture
		
	shadow_lod 			= 1;				// dynamic shadow created from 2nd LOD stage of a models (if it has LOD stages)
	mat_shadow.alpha 	= 40;				// shadow transparency
	shadow_threshold 	= 25;				// minimum brightess needed for creating dynamic shadows - i.e. avoid night shadows
	
	move_friction 		= 0;				// gliding friction of c_move
	move_min_z 			= 0.4;			// max step size of player
	
	//-------------------------------------------------------------------------
	// edition
	
	if (edition == (var)3)
		{
			main_engineversion = 1;
		}
	else if ((edition == (var)4) || (edition == (var)-1))
		{
			main_engineversion = 2;
		}
	else
		{
			main_engineversion = 0;
		}
	
	//-------------------------------------------------------------------------
	// shadow mode
	
// -1 (exponential shadowmapping - commercial/pro) - most recommended
	// 0 (decal shadows - free/extra/commercial/pro)
	// 1 (decal+stencil shadows - free/extra/commercial/pro) - recommended below commercial
	// 8 (pssm - free/extra/commercial/pro)
	
	// commercial, pro, or trial edition only
	if ((edition > 2) || (edition < 0))
		{
			shadow_stencil = -1;
			//shadow_stencil = 8;	// pssm + ent_decal + lightmap
			//shadow_stencil = 1;	// stencil + decal + ent_decal + lightmap
			//shadow_stencil = 0;	// decal + ent_decal + lightmap
		}
	else
		{
			shadow_stencil = 1;	// stencil + decal + lightmap
			//shadow_stencil = 0;	// decal + lightmap
		}
	
	//-------------------------------------------------------------------------
	// water mode
	
	// commercial, pro, or trial edition only
	if ((edition > 2) || (edition < 0))
		{
			main_water = 4;			// best water settings
		}
	else
		{
			main_water = -1;			// texture water
		}
	
	//-------------------------------------------------------------------------
	// pp effects
	
	if ((edition > 2) || (edition < 0))
		{
			main_blur					= 1.5;		// blur stencil or pssm shadows
			main_hdr						= 2;			// set pseudo bloom-blur post-processing effect
		}
	else
		{
			main_blur					= 0;			// no blur 
			main_hdr						= 0;			// no post-processing effect
		}
	
	//-------------------------------------------------------------------------
	// set max nexus to 200 by an empty level - or comment it out if the default 40 is enough
	
	level_load("mapbuilder.wmb");	
	wait(1);
	
	//-------------------------------------------------------------------------	
	
	UIMode = (var)UIMODE_STARTUP;		// user interface state - you can customize it
	
}


//////////////////////////////////////////////////////////////////////////////


void MB_Load_FullMap(STRING* mapfilename)
{
	// set to 1 to display progress bar, to 0 to hide
	filemenu_progressbar = 1; 
	
	//-------------------------------------------------------------------------
	// assemble level file name to load: from foldername and .trr filename
	
	// maps folder name "\\maps" read from included MB scripts
	STRING* temp_str = str_create("");
	str_cpy( temp_str , map_foldername );
	
	// map name read from function parameter
	str_cat( temp_str , mapfilename );
	
	//-------------------------------------------------------------------------
	// define map type: .trr (MapBuilder map) or .wmb (GameStudio level)
	
	STRING* temp2_str = str_create("");
	
	str_cpy(temp2_str, mapfilename);
	str_clip(temp2_str, str_len(temp2_str)-4);	// result is ".trr" or ".wmb"
	
	var mb_map = 1;	// the map is a .trr map
	if (str_cmpi(temp2_str, ".wmb"))
		{
			mb_map = 0; // the map is a .wmb level
			
			// change folder name
			str_cpy(temp_str, lev_foldername);
			
			// level name read from function parameter
			str_cat( temp_str , mapfilename );
		}
	
	//----------------------------------------------------------------------
	// load level created in MapBuilder , using MapBuilder's functions
	
	// required by decal shadows to enable close model placement by ignoring each other except terrain and terrain models
	shadow_mode |= IGNORE_FLAG2 ;
	
	//-----------------------------------------------------------------------------------------------
	
	if (mb_map)
		{
			// load full map - a [.trr] file and other map files having the same name, but different extensions
			FileMenu_Load_All( temp_str );		
			wait_for(FileMenu_Load_All);
		}
	else
		{
			// load a [.wmb] level with default/current material and sky settings
			FileMenu_Load_wmb( temp_str );
			wait_for(FileMenu_Load_wmb);
		}
	
	// reset values to default to enable easier settings on next map load
	map_load_details			= 1;								// grass sprites
	map_load_shrubs			= 1;								// shrub sprites
	map_load_grass				= 1;								// grass models
	map_load_obstacles		= 1;								// obstacle models
	map_load_mapents			= 1;								// map entities
	map_load_terrmdls			= 1;								// terrain models
	map_load_buildings		= 1;								// building models
	
	map_load_terrdeform		= 1;								// terrain vertices
	map_load_sky				= 1;								// sky-sun-camera-lod-fog-shadow settings
	map_load_materials		= 1;								// materials
	map_load_watershaders	= 1;								// water shader settings
	map_load_terrtext			= 1;								// terrain textures
	map_load_terrlm			= 1;								// terrain lightmap
		
	//-------------------------------------------------------------------------------------
	
	// for proper in-game decal shadows reset FLAG2
	shadow_mode &= ~IGNORE_FLAG2 ;
	wait(1);			
	
	//-------------------------------------------------------------------------------------
	
	ptr_remove(temp_str);
	ptr_remove(temp2_str);
}


//////////////////////////////////////////////////////////////////////////////////////////


void	MB_SetReset_Shadowmapping(var* pssm_state, var* temp_scsm_depth, var* temp_stencil_blur, var* temp_window)
{
	if ((!window_focus))// || (!mouse_valid))
		{
			if (*temp_window)	// was focused, //or was not left in previous frame
				{
					*temp_window = 0;
					
					// toggle shadowmapping-------------------------------------------------
					if (shadow_stencil==(var)8)
						{
							if (pssm_numsplits > (var)0)
								{
									*pssm_state = pssm_numsplits;
									
									Pssm_Close();
									wait_for(Pssm_Close);	
								}
						}	
					else if (shadow_stencil==(var)-1)
						{
							if (scsm_run == (var)1)
								{
									*temp_scsm_depth 	= scsm_maxdepth;
									
									Scsm_Close();
									wait_for(Scsm_Close);												
								}
						}
				}
		}
	else
		{
			if (!(*temp_window))		// was not focused, //or was left in previous frame
				{
					*temp_window = 1;
					
					// toggle shadowmapping-------------------------------------------------
					if (shadow_stencil==(var)8)
						{
							if (pssm_numsplits == (var)0)
								{
									pssm_numsplits = *pssm_state;
									wait(3);	
									Pssm_Start(*pssm_state);
									wait(3);	
								}
						}	
					else if (shadow_stencil==(var)-1)
						{
							if (scsm_run == (var)0)
								{
									Scsm_Start();									// auto calculates depth that suits new resolution, but can be modified afterwards
									wait(15);
									if (*temp_scsm_depth>0)
										scsm_maxdepth 	= *temp_scsm_depth;
								}
						}
				}
		}	
}