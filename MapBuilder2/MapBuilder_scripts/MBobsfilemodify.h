
#ifndef MBobsfilemodify_h
#define MBobsfilemodify_h


//////////////////////////////////////////
// variables

var		obsw_tilesize;				// 32..
var		obsw_areax;					// 1,3,5,7,9 - extended up to 19 !
var		obsw_areay;					// 1,3,5,7,9 - extended up to 19 !
var		obsw_shadow;				// 0 no, 1 stencil, 2 sprite, 3 model
var		obsw_passability;			// for tile/cluster system:    0 no acc,  1 passable, 3 forest-slow
var		obsw_collision;			// for basic collision system: 0 no coll, 1 polygon,  2 tile area and object height
STRING*	obsw_filename = "";		// full obs or obw filename with path

PANEL*	obsw_panel;
var		obsw_origmenu;

//////////////////////////////////////////
// functions

void		ObswModify_Init(var tsize, var tilex, var tiley, var ttype, var tshadow, var tcoll, char* tfilename);
void		ObswModify_Close();
void		ObswModify_OK();
	void		ObswModify_Panel_Init();
		void		ObswModify_Panel_Button(var button_number);
		
/////////////////////////////////////////
// includes


#endif