
#ifndef MBterrwatereditor_h
#define MBterrwatereditor_h

////////////////////////////////////////////////////
// defines

#define TERRWATER_MAXPERSET 	8
#define TERRWATER_MAXSET 		32

////////////////////////////////////////////////////
// variables

//var terrwater_ready			= 0;									// ready for placement to avoid crash - needed only when preview or special database file is used

var terrwater_switch_type[TERRWATER_MAXPERSET];				// what to place from the actual set: items 1-8
var terrwater_selected		= 0;										// 0..7

// terrmdl sets
var 		max_terrwater_items	= TERRWATER_MAXPERSET;
var 		max_terrwater_sets 	= TERRWATER_MAXSET;				// to avoid trying to use invalid sets
var 		terrwater_set			= 1;									// TERRWATER_MAXSET different sets can be created/used with 8 items in each
STRING**	terrwater_filenames = NULL;								// str_create("") is needed as initialization, before str_cpy() !!! [TERRWATER_MAXPERSET]
TEXT* 	terrwaters_txt;

// control panel
PANEL*  	terrwater_panel;

//////////////////////////////////////////////////
// functions

//------------------------------------------
// editor initialization

void 		TerrWater_Init();								// create tool objects
	void		TerrWater_Close();							// remove all tool objects

//------------------------------------------
// editor menu

void		TerrWater_Panel_Init();						// shows all the necessary data
	void		TerrWater_Panel_Toggle();					// hide, unhide
	
	void		TerrWater_Panel_Button(button_number);// called by slider value fine setting buttons - common function
	
	void		TerrWater_DecSet();							// jump to previous set
	void		TerrWater_IncSet();							// jump to next set
//	void		TerrWater_NextSet();						// jump to next set by hotkey
	void		TerrWater_UpdateSetNames();				// update names displayed on panel
	
	void		TerrWater_Flag_Toggle(button_number);	// switch on/off the 8 item within the active set
	void		TerrWater_Flag_Switch(var setplusone);// the same by hotkey
	
	void		TerrWater_MaterialSetup();				// calls the outer material setup panel - materialsetupclass

//------------------------------------------
// editor placement

void	 	TerrWater_CreateObject();						// on left-click							- place a terrhmp

//------------------------------------------
// editor deletion

void		TerrWater_DeleteAll();							// remove all objects

////////////////////////////////////////////////


#endif