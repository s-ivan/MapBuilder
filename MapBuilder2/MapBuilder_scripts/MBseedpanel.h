
#ifndef MBseedpanel_h
#define MBseedpanel_h


/////////////////////////////////////////////////////////////
// variables

// random placement of a given quantity over the selected area
var seed_max 		= 100;			// defines max quantity to be placed at a time, depends on entity type
var seed_step 		= 1;				// defines quantity increase by arrow button
var seed_quantity = 100;			// defines actual quantityí set

// full area coverage
var seed_distance = 320;			// average distance of entities to be placed, range: 16-640
var seed_rnddist	= 64;				// max +/- x/y deviance from position defined by seed_distance
var seed_limit		= 25000;			// (limit max entity quantity)

// limitations
var seed_minh		= -16;			// to not to place entities under water		 - 
var seed_maxh		= 640;			// to not to place entities on high mountains - 0 no limit
var seed_maxslope = 45;			// = 1-seed_maxslope : under the given normal vector y value no entities will be placed (no place on high slope)
var seed_minslope = 0;			// = 1-seed_minslope : over the given normal vector y value no entities will be placed (no place on flat)

// type of placement
var seed_random	= 0;				// select by check button
var seed_coverage	= 1;				// select by check button

// target area of placement
//var seed_full	= 1;
//var seed_game	= 0;
//var seed_border	= 0;
var seed_area 		= 0;				// 0 full area , 1 game area , 2 border area - controlled by 3 radio buttons

/////////////////////////////////////////////////////////////
// panels

PANEL*	seed_panel;

/////////////////////////////////////////////////////////////
// functions

void		Seed_Init();					// init and create panel
		
	void		Seed_ToggleRandom();
	void		Seed_ToggleCoverage();
	void		Seed_ToggleArea(var button_number);		
	
	void		Seed_DecQuantity();
	void		Seed_IncQuantity();
	void		Seed_DecDistance();
	void		Seed_IncDistance();
	void		Seed_DecRndDist();
	void		Seed_IncRndDist();
	void		Seed_DecLimit();	
	void		Seed_IncLimit();
	void		Seed_DecMinH();
	void		Seed_IncMinH();
	void		Seed_DecMaxH();
	void		Seed_IncMaxH();
	void		Seed_DecMinSlope();
	void		Seed_IncMinSlope();
	void		Seed_DecMaxSlope();
	void		Seed_IncMaxSlope();
	
	void		Seed_Close();					// close panel
	void		Seed_Place();				// calls outer placement functions
	
/////////////////////////////////////////////////////////////
// includes

//#include "MBdetailplace.c"
//#include "MBshrubplace.c"
//#include "MBgrassplace.c"
//#include "MBobstacleplace.c"


#endif