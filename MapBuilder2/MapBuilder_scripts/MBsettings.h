
#ifndef MBsettings_h
#define MBsettings_h


/////////////////////////////////////////////////////////

//--------------------------------------
// map adjustable parameters

var settings_tilesize;

var settings_maptilesx;
var settings_maptilesy;

var settings_mapzlevels;
var settings_clustersize;			

//--------------------------------------
// map informative parameters

var settings_scalevalue;

var settings_defaultterraintype;
var settings_defaultmovecost;

var settings_mapclusters;

//--------------------------------------
// map helper parameters

var settings_maxtilesx;
var settings_maxtilesy;
var settings_mintilesxy		= 100;

//--------------------------------------
// terrain adjustable parameters

var settings_blendfactor_ground;
var settings_ground_detail;
var settings_water_detail;				// only for texture based water

var settings_terrain_chunk;

//--------------------------------------

PANEL*	settings_panel;

///////////////////////////////////////////////////////////////////////////
// functions

void	Settings_Init();
void	Settings_Close();

	void	Settings_Panel_Init();
		void	Settings_Panel_Button(button_number);
		void	Settings_Ok();
		void	Settings_LightmapAll(button_number);		// set / reset all entities' static lightmap shadows


#endif