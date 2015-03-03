
#ifndef MBterrwaterhmp_h
#define MBterrwaterhmp_h

/////////////////////////////////////////////////////////////////////////
// defines



//////////////////////////////////////////////////////////////////////////
// variables


VECTOR*	terrhmp_vwatermax			= { x = 5000; y = 5000; z =  0; }  
var*		terrhmp_watervertices	= NULL;
ENTITY*	water_entity 				= NULL;


/////////////////////////////////////////////////////////////////////////
// functions

//-----------------------------------------------------------------
// create

void		TerrHmp_CreateWater(char* WaterFileName);
void		TerrHmp_RemoveWater();

action 	TerrHmp_WaterAction();														// set terrain related variables and terrain material depending on shadow_stencil mode
	void		TerrHmp_WaterTransparency();											// adjust transparency for texture based water terrain - imperfect

//-----------------------------------------------------------------
// load/save

//		none, within water shader modules

//-----------------------------------------------------------------
// gets

	VECTOR*	TerrHmp_GetWaterMax();

/////////////////////////////////////////////////////////////////////////


#endif