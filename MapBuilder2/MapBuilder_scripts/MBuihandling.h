
#ifndef MBuihandling_h
#define MBuihandling_h


/////////////////////////////////////////////////////
// variables


//var water_anim = 0;

var ui_screencounter = 0;
var ui_wireframe = 0;

// coordinates of 

STRING*	ui_guipos 	= "_guipos.txt";

// 256x512 submenu panels : file,filefile sky,skysky, material,materialmaterial, settings, teredit,he,tx,lm, all the placements, modify
var ui_submenupanel_x;
var ui_submenupanel_y;

// 256x256 sub-submenu panels : @sky all the colors, obstacle/mapent edit,
var ui_submenupanel2_x;
var ui_submenupanel2_y;

// 256x512 sub-submenu panels : @sky pssm (pssm2 is linked to it), scsm, scwater 1? 2, mwater 1
var ui_submenupanel3_x;
var ui_submenupanel3_y;

var ui_drawd3dgrid = 1;


/////////////////////////////////////////////////////
// panels 


PANEL* ui_mainmenu_panel =
{
	flags = SHADOW | OVERLAY;		// | TRANSLUCENT
//	alpha = 75;	
	
	pos_x = 0; 
	pos_y = 718;		// 736;		// was 32 high now 64, buttons at 6 from top, start at 3, shift by 41
	
//	bmap = panel_image3_bmp;		//"graypanel768x256.dds";		// transparent if not set
	layer = 10;	
	
//	button(003,6,		m_grass_s_on_bmp, 	m_grass_s_off_bmp, 	m_grass_s_over_bmp, 		UI_DetailMode	 ,NULL,NULL);
//	button(072,6,		m_shrub_s_on_bmp, 	m_shrub_s_off_bmp,	m_shrub_s_over_bmp, 		UI_ShrubMode	 ,NULL,NULL);
//	
//	button(142,6,		m_grass_m_on_bmp, 	m_grass_m_off_bmp, 	m_grass_m_over_bmp, 		UI_GrassMode	 ,NULL,NULL);
//	button(212,6,		m_obstacle_on_bmp, 	m_obstacle_off_bmp, 	m_obstacle_over_bmp, 	UI_ObstacleMode ,NULL,NULL);
//	button(282,6,		m_mapent_on_bmp, 		m_mapent_off_bmp, 	m_mapent_over_bmp, 		UI_MapEntMode 	 ,NULL,NULL);
//	button(352,6,		m_terrain_on_bmp, 	m_terrain_off_bmp, 	m_terrain_over_bmp, 		UI_TerrMdlMode  ,NULL,NULL);
//	button(422,6,		m_obstacle_on_bmp, 	m_obstacle_off_bmp, 	m_obstacle_over_bmp, 	UI_BuildingMode ,NULL,NULL);
//	
//	button(492,6,		m_material_on_bmp, 	m_material_off_bmp, 	m_material_over_bmp, 	UI_MaterialMode ,NULL,NULL);	
//	button(562,6,		m_sky_on_bmp,	 		m_sky_off_bmp, 		m_sky_over_bmp, 			UI_SkyMode		 ,NULL,NULL);
//	button(632,6,		m_settings_on_bmp, 	m_settings_off_bmp, 	m_settings_over_bmp, 	UI_SettingsMode ,NULL,NULL);	
//	button(702,6,		m_edit_terr_on_bmp, 	m_edit_terr_off_bmp, m_edit_terr_over_bmp, 	UI_TerEditMode	 ,NULL,NULL);
//	
//	button(772,6,		m_file_on_bmp, 		m_file_off_bmp, 		m_file_over_bmp, 			UI_FileMode		 ,NULL,NULL);
	
	button(  5,25,		m_ground_on_bmp, 		m_ground_off_bmp, 	m_ground_over_bmp, 		UI_GroundMode	 ,NULL,NULL);		// F1
	button(  5, 0,		m_water_on_bmp, 		m_water_off_bmp, 		m_water_over_bmp, 		UI_WaterMode	 ,NULL,NULL);		// +SHIFT
	
	button( 75,25,		m_terrain_on_bmp, 	m_terrain_off_bmp, 	m_terrain_over_bmp, 		UI_TerrMdlMode  ,NULL,NULL);		// F2
	button( 75, 0,		m_edit_terr_on_bmp, 	m_edit_terr_off_bmp, m_edit_terr_over_bmp, 	UI_TerEditMode	 ,NULL,NULL);		// +SHIFT
	
	button(145,25,		m_grass_m_on_bmp, 	m_grass_m_off_bmp, 	m_grass_m_over_bmp, 		UI_GrassMode	 ,NULL,NULL);		// F3
	button(145, 0,		m_grass_s_on_bmp, 	m_grass_s_off_bmp, 	m_grass_s_over_bmp, 		UI_DetailMode	 ,NULL,NULL);		// +SHIFT
	
	button(215,25,		m_obstacle_on_bmp, 	m_obstacle_off_bmp, 	m_obstacle_over_bmp, 	UI_ObstacleMode ,NULL,NULL);		// F4
	button(215, 0,		m_shrub_s_on_bmp, 	m_shrub_s_off_bmp,	m_shrub_s_over_bmp, 		UI_ShrubMode	 ,NULL,NULL);		// +SHIFT
	
	button(285,25,		m_building_on_bmp, 	m_building_off_bmp, 	m_building_over_bmp, 	UI_BuildingMode ,NULL,NULL);		// F5					// new image needed !!!
	button(285, 0,		m_mapent_on_bmp, 		m_mapent_off_bmp, 	m_mapent_over_bmp, 		UI_MapEntMode 	 ,NULL,NULL);		// +SHIFT
	
	button(355,25,		m_sky_on_bmp,	 		m_sky_off_bmp, 		m_sky_over_bmp, 			UI_SkyMode		 ,NULL,NULL);		// F6
	button(355, 0,		m_material_on_bmp, 	m_material_off_bmp, 	m_material_over_bmp, 	UI_MaterialMode ,NULL,NULL);		// +SHIFT
	
	button(425,25,		m_file_on_bmp, 		m_file_off_bmp, 		m_file_over_bmp, 			UI_FileMode		 ,NULL,NULL);		// F12
	button(425, 0,		m_settings_on_bmp, 	m_settings_off_bmp, 	m_settings_over_bmp, 	UI_SettingsMode ,NULL,NULL);		// +SHIFT
}


//PANEL* ui_mainside_panel =											// not used anymore
//{
//	flags = SHADOW | OVERLAY;		// | TRANSLUCENT
////	alpha = 75;	
//	pos_x = 1024; 
//	pos_y = 736;
//	bmap = panel_image3_bmp;	
//	layer = 9;	
//}


//PANEL* ui_subbottom_panel =											// not used anymore
//{
//	flags = SHADOW | OVERLAY;		// | TRANSLUCENT
////	alpha = 75;	
//	pos_x = 768; 
//	pos_y = 768;
//	bmap = panel_image4_bmp;	
//	layer = 9;	
//}


//-------------------------------------------------------------------------
// general info texts

TEXT* ui_helptext =
{
	layer = 15;
	
	red = 75;
	green = 75;
	blue = 75;
	
	flags = SHADOW;	
	
	pos_x = 50;
	pos_y = 100;	//125;
	
	font = "Courier#12b"; 
	string (	
				"HELP" ,
				"" ,
				"CAMERA" ,
				"" ,
				"Toggle Free/RTS Camera    Ctrl + K" ,
				"Move/Rotate/Height Free:  arrow keys, +CTRL, Home/End" ,				
				"Move/Rotate/Height RTS:   arrow keys, PgUp/PgDn, Home/End" ,
				"Free Look:                hold middle mouse button (or right +Ctrl)" ,
				"Zoom:                     mouse wheel" ,
				"Fast Move:                +Shift" , 
				"" ,
				"Screenshot:               Alf + F" , 
				"" ,
				"MAIN MENU" ,
				"" ,
				"Place Ground Terrain:     F1" ,
				"Place Water Terrain:      Shift + F1" ,
				"Place Terrain Models:     F2" ,
				"Edit Terrain:             Shift + F2" ,
				"Place Grass Models:       F3" ,
				"Place Grass Sprites:      Shift + F3" ,
				"Place Obstacle Models:    F4" ,
				"Place Shrub Sprites:      Shift + F4" ,
				"Place Building Models:    F5" ,				
				"Place WMB Map Entities:   Shift + F5" ,
				"Sky Settings:             F6" ,
				"Material Settings:        Shift + F6" ,
				"File Menu:                F12" ,
				"Map Settings:             Shift + F12" ,
				"" ,
				"3DGS Statistics Panel:    F11" ,
				"" ,
				"TEST" ,
				"" ,
				"3rd Person Test Mode:     T" ,	
				"Control Test Character:   WSAD, run: +Shift, crouch: +Ctrl, shoot: E, jump: Space" ,		
				"Rain:                     1" ,
				"Snow:                     2" ,
				"Stop Weather Effects:     0" ,		
				"" ,
				"GENERAL" ,
				"" ,
				"Toggle Grid               G"
				"Toggle Collision:         Ctrl + Tab" ,
				"Toggle Wireframe:         Shift + Tab" ,
				"Toggle Shadowmapping:     Ctrl + Q" ,
				"" ,
				"Quit:                     Ctrl + Alt + End" ,							
				"Toggle This Help:         Esc"
				);	
}

// detail , shrub
TEXT* ui_helptext_place0 =
{
	layer = 15;
	
	red = 75;
	green = 75;
	blue = 75;
	
	flags = SHADOW;
	
	pos_x = 50;
	pos_y = 100;
	
	font = "Courier#12b"; 
	string (	
				"HELP" ,
				"" ,
				"PLACEMENT" ,
				"" ,
				"Place Entities:           left click" ,
				"Continuous Placement:     hold down left button" ,
				"Deletion:                 right click" ,
				"" ,
				"Placement Area Size:      N/M" ,
				"Next Entity Set:          B" ,				
				"Select/Deselect Item:     1..8" ,
				"" ,
				"GENERAL" ,
				"" ,
				"Toggle Grid               G"
				"Toggle Collision:         Ctrl + Tab" ,
				"Toggle Wireframe:         Shift + Tab" ,
				"Toggle Shadowmapping:     Ctrl + Q" ,
				"" ,
				"Quit:                     Ctrl + Alt + End" ,							
				"Toggle This Help:         Esc"
			);
}

// grass model , obstacle
TEXT* ui_helptext_place1 =
{
	layer = 15;
	
	red = 75;
	green = 75;
	blue = 75;
	
	flags = SHADOW;
	
	pos_x = 50;
	pos_y = 100;
	
	font = "Courier#12b"; 
	string (	
				"HELP" ,
				"" ,
				"PLACEMENT" ,
				"" ,
				"Place Entities:           left click" ,
				"Continuous Placement:     hold down left button" ,
				"Deletion:                 right click" ,
				"Modify:                   Ctrl + left click" ,
				"Clone:                    Shift + left click" ,
				"" ,
				"Placement Area Size:      N/M" ,
				"Next Entity Set:          B" ,
				"Rotate Preview:           X/C or Ctrl/Alt + Mouse wheel" ,
				"Select/Deselect Item:     1..8" ,
				"" ,
				"GENERAL" ,
				"" ,
				"Toggle Grid               G"
				"Toggle Collision:         Ctrl + Tab" ,
				"Toggle Wireframe:         Shift + Tab" ,
				"Toggle Shadowmapping:     Ctrl + Q" ,
				"" ,
				"Quit:                     Ctrl + Alt + End" ,							
				"Toggle This Help:         Esc"
			);
}

// mapent , terrmdl , building
TEXT* ui_helptext_place2 =
{
	layer = 15;
	
	red = 75;
	green = 75;
	blue = 75;
	
	flags = SHADOW;
	
	pos_x = 50;
	pos_y = 100;
	
	font = "Courier#12b"; 
	string (	
				"HELP" ,
				"" ,
				"PLACEMENT" ,
				"" ,
				"Place Entities:           left click" ,
				"Deletion:                 right click" ,
				"Modify:                   Ctrl + left click" ,
				"Clone:                    Shift + left click" ,
				"" ,
				"Next Entity Set:          B" ,
				"Rotate Preview:           X/C or Ctrl/Alt + Mouse wheel" ,
				"Select/Deselect Item:     1..8" ,
				"" ,
				"GENERAL" ,
				"" ,
				"Toggle Grid               G"
				"Toggle Collision:         Ctrl + Tab" ,
				"Toggle Wireframe:         Shift + Tab" ,
				"Toggle Shadowmapping:     Ctrl + Q" ,
				"" ,
				"Quit:                     Ctrl + Alt + End" ,							
				"Toggle This Help:         Esc"
			);
}

TEXT* ui_helptext_teredit =
{
	layer = 15;
	
	red = 75;
	green = 75;
	blue = 75;
	
	flags = SHADOW;
	
	pos_x = 50;
	pos_y = 100;
	
	font = "Courier#12b"; 
	string (	
				"HELP" ,
				"" ,			
				"TERRAIN" ,
				"" ,
				"Editing Area Size:        N/M" ,
				"" ,
				"HEIGHT EDITING" ,
				"" ,
				"Raise or Set height:      left click" ,
				"Lower or Smooth surface:  right click" ,
				"" ,
				"Undo:                     Ctrl + Z" ,
				"Redo:                     Ctrl + Y" ,
				"Reset:                    Ctrl + R" ,
				"" ,
				"Update surface:           U" ,
				"Update entities:          Ctrl + U" ,
				"" ,
				"TEXTURE EDITING" ,
				"" ,
				"Draw on selected skin:    left click" ,
				"Clear on selected skin:   right click" ,
				"" ,
				"GENERAL" ,
				"" ,
				"Toggle Grid               G"
				"Toggle Collision:         Ctrl + Tab" ,
				"Toggle Wireframe:         Shift + Tab" ,
				"Toggle Shadowmapping:     Ctrl + Q" ,
				"" ,
				"Quit:                     Ctrl + Alt + End" ,							
				"Toggle This Help:         Esc"
			);
}


// title text------------------------------
TEXT* ui_nexustext =
{
	layer = 21;
	
	pos_x = 512;
	pos_y = 100;
	
	red = 200;
	green = 75;
	blue = 75;
	
	font = "Arial#40b"; 
	string (	"Nexus too high" ,
				"Please save and reload your map!"
				);	
	flags = CENTER_X | CENTER_Y | SHADOW;	
}

//////////////////////////////////////
// functions

void 		UI_Init();

void		UI_GroundMode();
void		UI_WaterMode();

void 		UI_DetailMode();
void 		UI_ShrubMode();
void		UI_GrassMode();
void		UI_ObstacleMode();
void		UI_MapEntMode();
void		UI_TerrMdlMode();
void		UI_BuildingMode();

void		UI_SkyMode();
void		UI_MaterialMode();
void		UI_SettingsMode();
#ifndef MB_RELEASE
void		UI_MapInfoMode();
#endif
void		UI_TerEditMode();
void		UI_FileMode();

// events------
void		ui_panelmove(PANEL* panel);				// does not stores position
void		ui_submenu_panelmove(PANEL* panel);		// stores submenu position
void		ui_small_panelmove(PANEL* panel);		// stores small sub-sub panel positions
void		ui_various_panelmove(PANEL* panel);		// stores large sub-sub panel positions
//-------------

void		UI_GuiPosInfo_Read();
void		UI_GuiPosInfo_Write();

void		UI_StartWireFrame();
void		UI_StartD3DLoop();
void 		UI_D3DLoop(ENTITY* ent);

void		UI_DrawD3DGrid();

//////////////////////////////////////
// includes

//#include "MBsky.c"														
//#include "MBfilehandling.c"
//#include "MBmodifyobject.c"
//#include "MBplayertest01.c"
//#include "MBsettings.c"
//#include "MBplayertest02.c"		//RTS -



#endif