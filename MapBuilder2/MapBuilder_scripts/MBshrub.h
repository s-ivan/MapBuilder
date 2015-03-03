
#ifndef MBshrub_h
#define MBshrub_h

////////////////////////////////////////////////////
// defines

#define SHRUB_SIZE1			256.0
#define SHRUB_SIZE2 			512.0

////////////////////////////////////////////////////
// variables

var shrub_counter;												// number of shrubs placed - nulled only at first init call !!!

// shrub image size from file
var 		shrub_defaultsize	= SHRUB_SIZE1;					// image size 
STRING*	shrubsize_name = "shrub_dds\\_shrubsize.txt";

////////////////////////////////////////////////////////////////////////////////////////////////
// functions

//------------------------------------------
// create

void 		Shrub_Create(	STRING* filename, 
								var my_scale_x, my_scale_y, 
								my_x, my_y, my_z, 
								my_pan, my_tilt, 
								my_collider, my_alpha, my_ambient, 
								my_light, my_red, my_green, my_blue, 
								my_unlit, my_shadow, my_overlay, my_material,
								my_flag6, my_flag5, my_clip);

void		Shrub_CreateCollider(ENTITY* ent);

void		Shrub_SetDefaultSize();																			// sets default image size based on file value or to 256 high

//------------------------------------------
// shadows

void	Shrub_SetShadow(	ENTITY* ent, var shrub_lightmap, 
								var shrub_decal, var shrub_shadow, var shrub_firstshrub);		// include in lightmap, force ent_decal, shadow 0 no/1 dynamic/2 sprite/3 model

//------------------------------------------
// load/save

void 		Shrub_Save_Shrubs_shs(char* file_name);
void		Shrub_Load_Shrubs_shs(char* file_name, var import);

void 		Shrub_CreateItem_FromFile(	var imported, STRING* my_filename, 
												var my_scale_x, my_scale_y, 
												my_x, my_y, my_z, 
												my_pan, my_tilt, 
												my_collider, my_alpha, my_ambient, 
												my_light, my_red, my_green, my_blue, 
												my_unlit, my_shadow, my_overlay, my_material,
												my_flag6, my_flag5, my_clip);

//------------------------------------------
// removal

function Shrub_RemoveObject(ENTITY* ent);																// delete the given entity without checking
//	void		Shrub_RemoveChild(ENTITY* owner);														// unique shadow sprite or model - not used

//---

int		Shrub_RemoveArea(	
										int midtilex, 
										int midtiley, 
										int areasizex, 
										int areasizey
									);																				// remove all objects in the area, xy area were both building_tilesize i.e. placement area

//---

void		Shrub_RemoveAll();

//------------------------------------------


#endif