
#ifndef MBterrmdl_h
#define MBterrmdl_h

////////////////////////////////////////////////////
// variables

var terrmdl_counter;												// number of terrmdls placed - nulled only at first init call !!!

//////////////////////////////////////////////////
// functions

//------------------------------------------
// create

void		TerrMdl_Create( 	STRING* filename, 

									var my_scale_x, my_scale_y, my_scale_z,
									my_x, my_y, my_z, 
									my_pan, my_tilt, my_roll,
									
									my_ambient, 
									my_light, my_red, my_green, my_blue, 
									my_unlit, my_material, my_shadow);

//------------------------------------------
// shadows

void	TerrMdl_SetShadow(ENTITY* ent, var terrmdl_shadow);

//------------------------------------------
// load/save

void 		TerrMdl_Save_TerrMdls_tem(char* file_name);
void 		TerrMdl_Load_TerrMdls_tem(char* file_name, var import);

void		TerrMdl_CreateItem_FromFile(	var imported, STRING* my_filename, 
													var my_scale_x, my_scale_y, my_scale_z,
													my_x, my_y, my_z, 
													my_pan, my_tilt, my_roll,
													my_ambient, 
													my_light, my_red, my_green, my_blue, 
													my_unlit, my_material, my_shadow);

//------------------------------------------
// removal

function TerrMdl_RemoveObject(ENTITY* ent);				// delete the given entity without checking

//---
// not used by editor

int		TerrMdl_RemoveArea(	
										int midtilex, 
										int midtiley, 
										int areasizex, 
										int areasizey
									);									// remove all objects in the area, xy area were both placement area size in tiles

//---

void		TerrMdl_RemoveAll();									// removes all the objects with the given type, nulls counter

//////////////////////////////////////////////////


#endif