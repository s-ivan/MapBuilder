#ifndef MBgrass_h
#define MBgrass_h

////////////////////////////////////////////////////
// defines

#define GRASS_SIZE1			32.0
#define GRASS_SIZE2 			64.0

////////////////////////////////////////////////////////////////////////////////////////////////
// variables

var grass_counter;																				// number of grasses placed - nulled only at first init call !!!
				
// grass image size from file
var 		grass_defaultsize	= GRASS_SIZE1;													// image size - could be determined for each sprite 
STRING*	grasssize_name = "grass_mdl\\_grasssize.txt";

////////////////////////////////////////////////////////////////////////////////////////////////
// functions

//------------------------------------------
// create

void		Grass_Create(	STRING* filename, 																		
								var my_scale_x, my_scale_y, my_scale_z,		
								my_x, my_y, my_z, 
								my_pan, my_tilt, my_roll,									
								my_alpha, my_ambient, 
								my_light, my_red, my_green, my_blue, 
								my_unlit, my_overlay, my_material,										
								my_shadow);															// general grass entity creation

void		Grass_SetNormals(ENTITY* ent);													// nearly vertical normals for better lighting

void		Grass_CreateLODmodels(ENTITY* ent);												// if has lod, create if does not exist
	action		Grass_CreateLOD();															// one lod model, to have correct normals

void		Grass_SetDefaultSize();																// sets default image size based on file value or to 256 high

//------------------------------------------
// shadows

void		Grass_SetShadow(ENTITY* ent, var grass_shadow);

//------------------------------------------
// load/save

void 		Grass_Save_Grasses_grm(char* file_name);
void 		Grass_Load_Grasses_grm(char* file_name, var import);

void 		Grass_CreateItem_FromFile( var imported, STRING* my_filename, 
												var my_scale_x, my_scale_y, my_scale_z,
												my_x, my_y, my_z,
												my_pan, my_tilt, my_roll,												 
												my_alpha, my_ambient, 
												my_light, my_red, my_green, my_blue, 
												my_unlit, my_overlay, my_material,
												my_shadow);

//------------------------------------------
// removal

function Grass_RemoveObject(ENTITY* ent);																					// delete the given entity without checking
//	void		Grass_RemoveChild(ENTITY* owner);																			// unique shadow sprite or model - not used

//---

int		Grass_RemoveArea(	
										int midtilex, 
										int midtiley, 
										int areasizex, 
										int areasizey
									);																									// remove all objects in the area, xy area were both building_tilesize i.e. placement area

//---

void		Grass_RemoveAll();

//------------------------------------------


#endif