////////////////////////////////////////////////////////////////////////
// main wdl:
//		Created by WED.
////////////////////////////////////////////////////////////////////////

WINDOW WINSTART
{
	MODE STANDARD;  	// always required  
	COMMAND "-nwnd"; 	// no start window
}

////////////////////////////////////////////////////////////////////////////
// The PATH keyword gives directories where template files can be found.
path "..\\..\\..\\..\\..\\..\\program files (x86)\\gstudio8\\code";
path "..\\..\\..\\..\\..\\..\\program files (x86)\\gstudio8\\templates\\images";
path "..\\levels";

////////////////////////////////////////////////////////////////////////////
// Manually added folders.
// Required to open wmb levels that are put to "levels" folder after compilation, but their resources are placed in different folders.

PATH "graphics";
PATH "shadows";

PATH "terrain_mdl";
PATH "building_mdl";
PATH "obstacle_mdl";
PATH "grass_mdl";

PATH "obstacle_wmb";

PATH "grass_dds";
PATH "shrub_dds";

PATH "terrain_hmp";
PATH "water_hmp";

/////////////////////////////////////////////////////////////////
// Filename of the starting level.

string level_str = <MapBuilder.wmb>; // give file names in angular brackets

