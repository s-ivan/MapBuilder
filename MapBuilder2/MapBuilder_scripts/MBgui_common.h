#ifndef MBgui_common_h
#define MBgui_common_h

///////////////////////////////////////////////////////////////////////////
// panels


//---------------------------------------------
// static commonly used panels


// a simple progress-bar
PANEL* map_loadbar = 
{ 
	layer = 100; 
}


// a transparent full screen covering panel covering everything to block GUI operations and representing inactive state
PANEL* map_loadpanel =
{
	flags = SHADOW | TRANSLUCENT;
	alpha = 75;	
	pos_x = 0; 
	pos_y = 0;
	size_x = 1024;
	size_y = 768;
	bmap = panel_image5_bmp;		
	layer = 20;		
}


//---------------------------------------------
// common panel components - used for dynamic panels


BMAP* 	mousearrow 					= "graphics\\mousearrow2.tga";

FONT*		panel_font					= "Arial#12b";
FONT*		panel_font2					= "Arial#14b";


var		text_font_r					= 225;
var		text_font_g					= 225;
var		text_font_b					= 200;

var		panel_font_r				= 200;
var		panel_font_g				= 200;
var		panel_font_b				= 200;


BMAP*		panel_image0_bmp			= "panels\\screen.jpg";					// 1280x768 to be usable in both aspect ratio without too much distortion
BMAP*		panel_image1_bmp			= "panels\\gpanel256x256.tga";
BMAP*		panel_image2_bmp			= "panels\\gpanel256x512.tga";
BMAP*		panel_image3_bmp			= "panels\\gpanel1024x32dark.tga";
BMAP*		panel_image4_bmp			= "panels\\gpanel256x512dark.tga";

BMAP*		panel_image5_bmp			= "panels\\loadpanel256.dds";
BMAP*		panel_image6_bmp			= "panels\\shadeclogo2.jpg";


// needed for pan_create
STRING* 	panel_image0 				= "bmap = panel_image0_bmp;";
STRING* 	panel_image 				= "bmap = panel_image1_bmp;";			//"bmap = graypanel256.dds;";  !! cannot contain subfolder...
STRING* 	panel_image2 				= "bmap = panel_image2_bmp;";			//graypanel256x512.dds;";
STRING* 	panel_image3 				= "bmap = panel_image3_bmp;";

STRING* 	panel_image6 				= "bmap = panel_image6_bmp;";


//-------------------------------------------------------------------
// panel button images


// slider: 106x16
BMAP*		horiz_slider_bmp 			= "panels\\hslider.tga";
// slider knob: 6x10
BMAP*		slider_knob_bmp 			= "panels\\hsliderknob.tga";

// slider: 20x128
BMAP*		vert_bar_bmp 				= "panels\\vert_bar.tga";
// slider knob: 20x20
BMAP*		vert_bar_knob_bmp 		= "panels\\vert_bar_knob.tga";

// checkbox: 16x16
BMAP* 	checkbox_on_bmp 			= "panels\\checkbox_on.bmp";
BMAP* 	checkbox_off_bmp 			= "panels\\checkbox_off.bmp";
BMAP* 	checkbox_onover_bmp 		= "panels\\checkbox_onover.bmp";
BMAP* 	checkbox_offover_bmp		= "panels\\checkbox_offover.bmp";

// radiobutton: 16x16
BMAP* 	radiobtn_on_bmp			= "panels\\radiobtn_on.bmp";
BMAP* 	radiobtn_off_bmp			= "panels\\radiobtn_off.bmp";
BMAP* 	radiobtn_onover_bmp		= "panels\\radiobtn_onover.bmp";
BMAP* 	radiobtn_offover_bmp		= "panels\\radiobtn_offover.bmp";

// general arrows: 11x15
BMAP* 	ss_arrow_left_on_bmp		= "panels\\ss_arrow_left_on.bmp";
BMAP* 	ss_arrow_left_off_bmp	= "panels\\ss_arrow_left_off.bmp";

BMAP* 	ss_arrow_right_on_bmp	= "panels\\ss_arrow_right_on.bmp";
BMAP* 	ss_arrow_right_off_bmp	= "panels\\ss_arrow_right_off.bmp";

BMAP* 	select_thumb_bmp			= "panels\\select_thumb.bmp";


// start menu buttons: 200x25, font:10 bold---------------------------------------------


BMAP* 	newmap_on_bmp				= "panels\\xl_newmap_on.bmp";
BMAP* 	newmap_off_bmp				= "panels\\xl_newmap_off.bmp";
BMAP* 	newmap_over_bmp			= "panels\\xl_newmap_over.bmp";

BMAP* 	loadmap_on_bmp				= "panels\\xl_loadmap_on.bmp";
BMAP* 	loadmap_off_bmp			= "panels\\xl_loadmap_off.bmp";
BMAP* 	loadmap_over_bmp			= "panels\\xl_loadmap_over.bmp";

BMAP* 	loadterrain_on_bmp		= "panels\\xl_loadterrain_on.bmp";
BMAP* 	loadterrain_off_bmp		= "panels\\xl_loadterrain_off.bmp";
BMAP* 	loadterrain_over_bmp		= "panels\\xl_loadterrain_over.bmp";

BMAP* 	constructterrain_on_bmp		= "panels\\xl_constructterrain_on.bmp";
BMAP* 	constructterrain_off_bmp	= "panels\\xl_constructterrain_off.bmp";
BMAP* 	constructterrain_over_bmp	= "panels\\xl_constructterrain_over.bmp";

BMAP* 	loadlevel_on_bmp			= "panels\\xl_loadlevel_on.bmp";
BMAP* 	loadlevel_off_bmp			= "panels\\xl_loadlevel_off.bmp";
BMAP* 	loadlevel_over_bmp		= "panels\\xl_loadlevel_over.bmp";

//BMAP* 	createterrain_on_bmp		= "panels\\xl_createterrain_on.bmp";
//BMAP* 	createterrain_off_bmp	= "panels\\xl_createterrain_off.bmp";
//BMAP* 	createterrain_over_bmp	= "panels\\xl_createterrain_over.bmp";

BMAP* 	settings_on_bmp			= "panels\\xl_settings_on.bmp";
BMAP* 	settings_off_bmp			= "panels\\xl_settings_off.bmp";
BMAP* 	settings_over_bmp			= "panels\\xl_settings_over.bmp";

BMAP* 	exit_on_bmp					= "panels\\xl_exit_on.bmp";
BMAP* 	exit_off_bmp				= "panels\\xl_exit_off.bmp";
BMAP* 	exit_over_bmp				= "panels\\xl_exit_over.bmp";


// general file handling menus: 200x20, no font--------------------------------------


BMAP* 	fileselect_on_bmp			= "panels\\fileselect_on.tga";
BMAP* 	fileselect_over_bmp		= "panels\\fileselect_over.tga";
BMAP* 	fileselect_off_bmp		= "panels\\fileselect_off.tga";


// larger arrows, should be replaced by small ones: 20x20 -------------------------------


BMAP* 	s_arrow_up_on_bmp			= "panels\\s_arrow_up_on.bmp";
BMAP* 	s_arrow_up_off_bmp		= "panels\\s_arrow_up_off.bmp";

BMAP* 	s_arrow_down_on_bmp		= "panels\\s_arrow_down_on.bmp";
BMAP* 	s_arrow_down_off_bmp		= "panels\\s_arrow_down_off.bmp";


// submenus - smaller buttons: 50x20 font: 8 bold-------------------------------


BMAP* 	s_load_on_bmp				= "panels\\s_load_on.bmp";
BMAP* 	s_load_off_bmp				= "panels\\s_load_off.bmp";
BMAP* 	s_load_over_bmp			= "panels\\s_load_over.bmp";

BMAP* 	s_save_on_bmp				= "panels\\s_save_on.bmp";
BMAP* 	s_save_off_bmp				= "panels\\s_save_off.bmp";
BMAP* 	s_save_over_bmp			= "panels\\s_save_over.bmp";

BMAP* 	s_ok_on_bmp					= "panels\\s_ok_on.bmp";
BMAP* 	s_ok_off_bmp				= "panels\\s_ok_off.bmp";
BMAP* 	s_ok_over_bmp				= "panels\\s_ok_over.bmp";

BMAP* 	s_cancel_on_bmp			= "panels\\s_cancel_on.bmp";
BMAP* 	s_cancel_off_bmp			= "panels\\s_cancel_off.bmp";
BMAP* 	s_cancel_over_bmp			= "panels\\s_cancel_over.bmp";

BMAP* 	s_reset_on_bmp				= "panels\\s_reset_on.bmp";
BMAP* 	s_reset_off_bmp			= "panels\\s_reset_off.bmp";
BMAP* 	s_reset_over_bmp			= "panels\\s_reset_over.bmp";

BMAP* 	s_undo_on_bmp				= "panels\\s_undo_on.bmp";
BMAP* 	s_undo_off_bmp				= "panels\\s_undo_off.bmp";
BMAP* 	s_undo_over_bmp			= "panels\\s_undo_over.bmp";

BMAP* 	s_redo_on_bmp				= "panels\\s_redo_on.bmp";
BMAP* 	s_redo_off_bmp				= "panels\\s_redo_off.bmp";
BMAP* 	s_redo_over_bmp			= "panels\\s_redo_over.bmp";


// sky and materials panel buttons: 50x15, font:8 bold----------------------


BMAP* 	ss_set_on_bmp				= "panels\\ss_set_on.bmp";
BMAP* 	ss_set_off_bmp				= "panels\\ss_set_off.bmp";
BMAP* 	ss_set_over_bmp			= "panels\\ss_set_over.bmp";

BMAP* 	ss_del_on_bmp				= "panels\\ss_del_on.bmp";
BMAP* 	ss_del_off_bmp				= "panels\\ss_del_off.bmp";
BMAP* 	ss_del_over_bmp			= "panels\\ss_del_over.bmp";

BMAP* 	ss_clear_on_bmp			= "panels\\ss_clear_on.bmp";
BMAP* 	ss_clear_off_bmp			= "panels\\ss_clear_off.bmp";
BMAP* 	ss_clear_over_bmp			= "panels\\ss_clear_over.bmp";

BMAP* 	ss_add_on_bmp				= "panels\\ss_add_on.bmp";
BMAP* 	ss_add_off_bmp				= "panels\\ss_add_off.bmp";
BMAP* 	ss_add_over_bmp			= "panels\\ss_add_over.bmp";

BMAP* 	ss_change_on_bmp			= "panels\\ss_change_on.bmp";
BMAP* 	ss_change_off_bmp			= "panels\\ss_change_off.bmp";
BMAP* 	ss_change_over_bmp		= "panels\\ss_change_over.bmp";

//BMAP* 	ss_material_on_bmp		= "panels\\ss_material_on.bmp";				// removed from placement submenus
//BMAP* 	ss_material_off_bmp		= "panels\\ss_material_off.bmp";
//BMAP* 	ss_material_over_bmp		= "panels\\ss_material_over.bmp";

BMAP* 	ss_edit_on_bmp				= "panels\\ss_edit_on.bmp";
BMAP* 	ss_edit_off_bmp			= "panels\\ss_edit_off.bmp";
BMAP* 	ss_edit_over_bmp			= "panels\\ss_edit_over.bmp";

BMAP* 	ss_apply_on_bmp			= "panels\\ss_apply_on.bmp";
BMAP* 	ss_apply_off_bmp			= "panels\\ss_apply_off.bmp";
BMAP* 	ss_apply_over_bmp			= "panels\\ss_apply_over.bmp";

BMAP* 	ss_raise_on_bmp			= "panels\\ss_raise_on.bmp";
BMAP* 	ss_raise_off_bmp			= "panels\\ss_raise_off.bmp";
BMAP* 	ss_raise_over_bmp			= "panels\\ss_raise_over.bmp";

BMAP* 	ss_lower_on_bmp			= "panels\\ss_lower_on.bmp";
BMAP* 	ss_lower_off_bmp			= "panels\\ss_lower_off.bmp";
BMAP* 	ss_lower_over_bmp			= "panels\\ss_lower_over.bmp";

BMAP* 	ss_save_on_bmp				= "panels\\ss_save_on.bmp";
BMAP* 	ss_save_off_bmp			= "panels\\ss_save_off.bmp";
BMAP* 	ss_save_over_bmp			= "panels\\ss_save_over.bmp";

BMAP* 	ss_load_on_bmp				= "panels\\ss_load_on.bmp";
BMAP* 	ss_load_off_bmp			= "panels\\ss_load_off.bmp";
BMAP* 	ss_load_over_bmp			= "panels\\ss_load_over.bmp";

BMAP* 	ss_store_on_bmp			= "panels\\ss_store_on.bmp";
BMAP* 	ss_store_off_bmp			= "panels\\ss_store_off.bmp";
BMAP* 	ss_store_over_bmp			= "panels\\ss_store_over.bmp";

BMAP* 	ss_undo_on_bmp				= "panels\\ss_undo_on.bmp";
BMAP* 	ss_undo_off_bmp			= "panels\\ss_undo_off.bmp";
BMAP* 	ss_undo_over_bmp			= "panels\\ss_undo_over.bmp";

BMAP* 	ss_redo_on_bmp				= "panels\\ss_redo_on.bmp";
BMAP* 	ss_redo_off_bmp			= "panels\\ss_redo_off.bmp";
BMAP* 	ss_redo_over_bmp			= "panels\\ss_redo_over.bmp";

BMAP* 	ss_reset_on_bmp			= "panels\\ss_reset_on.bmp";
BMAP* 	ss_reset_off_bmp			= "panels\\ss_reset_off.bmp";
BMAP* 	ss_reset_over_bmp			= "panels\\ss_reset_over.bmp";

BMAP* 	ss_restore_on_bmp			= "panels\\ss_restore_on.bmp";
BMAP* 	ss_restore_off_bmp		= "panels\\ss_restore_off.bmp";
BMAP* 	ss_restore_over_bmp		= "panels\\ss_restore_over.bmp";

BMAP* 	ss_update_on_bmp			= "panels\\ss_update_on.bmp";
BMAP* 	ss_update_off_bmp			= "panels\\ss_update_off.bmp";
BMAP* 	ss_update_over_bmp		= "panels\\ss_update_over.bmp";


// main menu buttons: 60x20, font: 8 bold--------------------------------------


BMAP* 	m_grass_s_on_bmp			= "panels\\m_grass_s_on.bmp";
BMAP* 	m_grass_s_off_bmp			= "panels\\m_grass_s_off.bmp";
BMAP* 	m_grass_s_over_bmp		= "panels\\m_grass_s_over.bmp";

BMAP* 	m_shrub_s_on_bmp			= "panels\\m_shrub_s_on.bmp";
BMAP* 	m_shrub_s_off_bmp			= "panels\\m_shrub_s_off.bmp";
BMAP* 	m_shrub_s_over_bmp		= "panels\\m_shrub_s_over.bmp";

BMAP* 	m_grass_m_on_bmp			= "panels\\m_grass_m_on.bmp";
BMAP* 	m_grass_m_off_bmp			= "panels\\m_grass_m_off.bmp";
BMAP* 	m_grass_m_over_bmp		= "panels\\m_grass_m_over.bmp";

BMAP* 	m_obstacle_on_bmp			= "panels\\m_obstacle_on.bmp";
BMAP* 	m_obstacle_off_bmp		= "panels\\m_obstacle_off.bmp";
BMAP* 	m_obstacle_over_bmp		= "panels\\m_obstacle_over.bmp";

BMAP* 	m_mapent_on_bmp			= "panels\\m_mapent_on.bmp";
BMAP* 	m_mapent_off_bmp			= "panels\\m_mapent_off.bmp";
BMAP* 	m_mapent_over_bmp			= "panels\\m_mapent_over.bmp";

BMAP* 	m_terrain_on_bmp			= "panels\\m_terrain_m_on.bmp";
BMAP* 	m_terrain_off_bmp			= "panels\\m_terrain_m_off.bmp";
BMAP* 	m_terrain_over_bmp		= "panels\\m_terrain_m_over.bmp";

BMAP* 	m_building_on_bmp			= "panels\\m_building_on.bmp";
BMAP* 	m_building_off_bmp		= "panels\\m_building_off.bmp";
BMAP* 	m_building_over_bmp		= "panels\\m_building_over.bmp";

BMAP* 	m_ground_on_bmp			= "panels\\m_ground_on.bmp";
BMAP* 	m_ground_off_bmp			= "panels\\m_ground_off.bmp";
BMAP* 	m_ground_over_bmp			= "panels\\m_ground_over.bmp";

BMAP* 	m_water_on_bmp				= "panels\\m_water_on.bmp";
BMAP* 	m_water_off_bmp			= "panels\\m_water_off.bmp";
BMAP* 	m_water_over_bmp			= "panels\\m_water_over.bmp";

BMAP* 	m_sky_on_bmp				= "panels\\m_sky_on.bmp";
BMAP* 	m_sky_off_bmp				= "panels\\m_sky_off.bmp";
BMAP* 	m_sky_over_bmp				= "panels\\m_sky_over.bmp";

BMAP* 	m_material_on_bmp			= "panels\\m_material_on.bmp";
BMAP* 	m_material_off_bmp		= "panels\\m_material_off.bmp";
BMAP* 	m_material_over_bmp		= "panels\\m_material_over.bmp";

BMAP* 	m_settings_on_bmp			= "panels\\m_settings_on.bmp";
BMAP* 	m_settings_off_bmp		= "panels\\m_settings_off.bmp";
BMAP* 	m_settings_over_bmp		= "panels\\m_settings_over.bmp";

BMAP* 	m_edit_terr_on_bmp		= "panels\\m_edit_terr_on.bmp";
BMAP* 	m_edit_terr_off_bmp		= "panels\\m_edit_terr_off.bmp";
BMAP* 	m_edit_terr_over_bmp		= "panels\\m_edit_terr_over.bmp";

BMAP* 	m_mapinfo_on_bmp			= "panels\\m_mapinfo_on.bmp";
BMAP* 	m_mapinfo_off_bmp			= "panels\\m_mapinfo_off.bmp";
BMAP* 	m_mapinfo_over_bmp		= "panels\\m_mapinfo_over.bmp";

BMAP* 	m_file_on_bmp				= "panels\\m_file_on.bmp";
BMAP* 	m_file_off_bmp				= "panels\\m_file_off.bmp";
BMAP* 	m_file_over_bmp			= "panels\\m_file_over.bmp";

//BMAP* 	m_exit_on_bmp				= "panels\\m_exit_on.bmp";
//BMAP* 	m_exit_off_bmp				= "panels\\m_exit_off.bmp";


// side panel larger-----------------------------------------------------------


BMAP* 	m_clearall_on_bmp			= "panels\\m_clearall_on.bmp";
BMAP* 	m_clearall_off_bmp		= "panels\\m_clearall_off.bmp";
BMAP* 	m_clearall_over_bmp		= "panels\\m_clearall_over.bmp";

//BMAP* 	m_modify_on_bmp			= "panels\\m_modify_on.bmp";				// removed from placement submenus
//BMAP* 	m_modify_off_bmp			= "panels\\m_modify_off.bmp";
//BMAP* 	m_modify_over_bmp			= "panels\\m_modify_over.bmp";

BMAP* 	m_seed_on_bmp				= "panels\\m_seed_on.bmp";
BMAP* 	m_seed_off_bmp				= "panels\\m_seed_off.bmp";
BMAP* 	m_seed_over_bmp			= "panels\\m_seed_over.bmp";

BMAP* 	m_apply_on_bmp				= "panels\\m_apply_on.bmp";
BMAP* 	m_apply_off_bmp			= "panels\\m_apply_off.bmp";
BMAP* 	m_apply_over_bmp			= "panels\\m_apply_over.bmp";


///////////////////////////////////////////////////////////////////////////
// texts


TEXT* map_loadtext1 =
{
	layer = 21;
	pos_x = 384;
	pos_y = 384;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#40b"; 
	string ("Loading...Please wait!");
	flags = CENTER_X | CENTER_Y | SHADOW;	
}

TEXT* map_savetext1 =
{
	layer = 21;
	pos_x = 384;
	pos_y = 384;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#40b"; 
	string ("Saving...Please wait!");
	flags = CENTER_X | CENTER_Y | SHADOW;	
}

TEXT* map_updatetext1 =
{
	layer = 21;
	pos_x = 384;
	pos_y = 384;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#40b"; 
	string ("Updating...Please wait!");
	flags = CENTER_X | CENTER_Y | SHADOW;	
}

TEXT* map_loadmap_text1 =
{
	layer = 21;
	pos_x = 250;
	pos_y = 425;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#16b"; 
	flags = SHADOW;	
	string ("Creating tile and cluster system...");	
}

TEXT* map_loadmap_text2 =
{
	layer = 21;
	pos_x = 250;
	pos_y = 450;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#16b"; 
	flags = SHADOW;
	string ("Evaluating terrain height and slope...");	
}

TEXT* map_loadmap_text3 =
{
	layer = 21;
	pos_x = 250;
	pos_y = 475;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#16b"; 
	flags = SHADOW;
	string ("Updating tile and cluster settings...");	
}

TEXT* map_loadmap_text3skip =
{
	layer = 21;
	pos_x = 250;
	pos_y = 475;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#16b"; 
	flags = SHADOW;
	string ("Updating tile and cluster settings...SKIPPED");	
}
TEXT* map_loadmap_text4 =
{
	layer = 21;
	pos_x = 250;
	pos_y = 500;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#16b"; 
	flags = SHADOW;
	string ("Creating basic unit clearance maps for pathfinding...");	
}

TEXT* map_loadmap_text4skip =
{
	layer = 21;
	pos_x = 250;
	pos_y = 500;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#16b"; 
	flags = SHADOW;
	string ("Creating basic unit clearance maps for pathfinding...SKIPPED");	
}

TEXT* map_loadmap_text5 =
{
	layer = 21;
	pos_x = 250;
	pos_y = 525;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#16b"; 
	flags = SHADOW;
	string ("Initializing camera system...");	
}

TEXT* map_loadmap_text6 =
{
	layer = 21;
	pos_x = 250;
	pos_y = 550;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#16b"; 
	flags = SHADOW;
	string ("Starting UI...");	
}

TEXT* map_loadmap_text7 =
{
	layer = 21;
	pos_x = 250;
	pos_y = 425;
	red = 225;
	green = 225;
	blue = 200;
	font = "Arial#16b"; 
	flags = SHADOW;	
	string ("string that should be modified runtime through pstring[0]");	
}


///////////////////////////////////////////////////////////////////////////


#endif