
#ifndef MBbuilding_h
#define MBbuilding_h

////////////////////////////////////////////////////////////////////////////////////////////////
// variables

var building_counter;																											// number of buildings placed - nulled on level init

////////////////////////////////////////////////////////////////////////////////////////////////
// functions

//------------------------------------------
// create

void		Building_Create(	STRING* filename, 

									var obs_defaultsize,
									obs_tilesize_x, obs_tilesize_y,
									obs_type, obs_collision, obs_shadow,
									
									var my_scale_x, my_scale_y, my_scale_z,		
									my_x, my_y, my_z, 
									my_pan, my_tilt, my_roll,
									
									my_alpha, my_ambient, 
									my_light, my_red, my_green, my_blue, 
									my_unlit, my_overlay, my_material,										
									my_flag6, my_flag5);																			// general building entity creation

void		Building_CreateCollider(ENTITY* ent);																			// collider entity
//void		Building_Created_Loop(ENTITY* ent);																			// fade in/out entities at a defined distance from camera, requires a shader with alpha blending enabled !!! or using no shader								

void		Building_ReadObsFileData(	STRING* my_filename, 
												var* obs_defaultsize,
												var* obs_tilesize_x,
												var* obs_tilesize_y,
												var* obs_type,
												var* obs_collision,
												var* obs_shadow
											);																							// read object data from .obs file created by editor

//------------------------------------------
// shadows

void	Building_SetShadow(ENTITY* ent, var build_lightmap, var build_decal, var build_shadow);

	void	Building_SetNoshadow(ENTITY* ent);
	void	Building_SetLightmap(ENTITY* ent);		
	void	Building_SetSCSM(ENTITY* ent);
	void	Building_SetPSSM(ENTITY* ent);
	void	Building_SetStencil(ENTITY* ent);
	void	Building_SetSprite(ENTITY* ent, var build_decal);
		void	Building_SetDecal(ENTITY* ent);
		void	Building_SetEntDecal(ENTITY* ent);
	void	Building_SetModel(ENTITY* ent);	

//------------------------------------------
// load/save

void 		Building_Save_Buildings_blm(char* file_name);																// save all building entities in a map file
void		Building_Load_Buildings_blm(char* file_name, var import);												// load building entities from a map file

	void 		Building_CreateItem_FromFile(var imported, STRING* my_filename, 
														var my_scale_x, my_scale_y, my_scale_z, 
														my_x, my_y, my_z, 
														my_pan, my_tilt, my_roll, 
														my_alpha, my_ambient, 
														my_light, my_red, my_green, my_blue, 
														my_unlit, my_overlay, my_material, 
														my_flag6, my_flag5);														// preliminary operations for creating an entity from a map file (calls Building_Create)

//------------------------------------------
// removal

function Building_RemoveObject(ENTITY* ent);																				// delete the given entity without checking
	void		Building_RemoveChild(ENTITY* owner);																		// unique shadow sprite or model

//---
// not used by editor

int		Building_RemoveArea(	
										int midtilex, 
										int midtiley, 
										int areasizex, 
										int areasizey
									);																									// remove all objects in the area, xy area were both building_tilesize i.e. placement area

//---
	
void		Building_RemoveAll();																								// removes all the objects with the given type, nulls counter
		
//------------------------------------------

#endif