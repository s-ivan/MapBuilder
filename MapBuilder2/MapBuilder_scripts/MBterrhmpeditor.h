
#ifndef MBterrhmpeditor_h
#define MBterrhmpeditor_h

////////////////////////////////////////////////////
// defines

#define TERRGROUND_MAXPERSET 	8
#define TERRGROUND_MAXSET 		32

////////////////////////////////////////////////////
// variables

//var terrground_ready			= 0;									// ready for placement to avoid crash - needed only when preview or special database file is used

var terrground_switch_type[TERRGROUND_MAXPERSET];				// what to place from the actual set: items 1-8
var terrground_selected		= 0;										// 0..7

// terrmdl sets
var 		max_terrground_items	= TERRGROUND_MAXPERSET;
var 		max_terrground_sets 	= TERRGROUND_MAXSET;				// to avoid trying to use invalid sets
var 		terrground_set			= 1;									// TERRGROUND_MAXSET different sets can be created/used with 8 items in each
STRING**	terrground_filenames = NULL;								// str_create("") is needed as initialization, before str_cpy() !!! [TERRGROUND_MAXPERSET]
TEXT* 	terrgrounds_txt;

// control panel
PANEL*  	terrground_panel;

//////////////////////////////////////////////////
// functions

//------------------------------------------
// editor initialization

void 		TerrGround_Init();								// create tool objects
	void		TerrGround_Close();							// remove all tool objects

//------------------------------------------
// editor menu

void		TerrGround_Panel_Init();						// shows all the necessary data
	void		TerrGround_Panel_Toggle();					// hide, unhide
	
	void		TerrGround_Panel_Button(button_number);// called by slider value fine setting buttons - common function
	
	void		TerrGround_DecSet();							// jump to previous set
	void		TerrGround_IncSet();							// jump to next set
//	void		TerrGround_NextSet();						// jump to next set by hotkey
	void		TerrGround_UpdateSetNames();				// update names displayed on panel
	
	void		TerrGround_Flag_Toggle(button_number);	// switch on/off the 8 item within the active set
	void		TerrGround_Flag_Switch(var setplusone);// the same by hotkey
	
	void		TerrGround_MaterialSetup();				// calls the outer material setup panel - materialsetupclass

//------------------------------------------
// editor placement

void	 	TerrGround_CreateObject();						// on left-click							- place a terrhmp

//------------------------------------------
// editor deletion

void		TerrGround_DeleteAll();							// remove all objects

////////////////////////////////////////////////


#endif