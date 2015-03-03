
#ifndef MBmapent_h
#define MBmapent_h

////////////////////////////////////////////////////////////////////////////////////////////////
// variables

var mapent_counter;													// number of mapents placed - nulled only at first init call !!!

////////////////////////////////////////////////////////////////////////////////////////////////
// functions

//------------------------------------------
// create

void		MapEnt_Create(	STRING* filename, 

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

void		MapEnt_CreateCollider(ENTITY* ent);																				// collider entity
//void		MapEnt_Created_Loop(ENTITY* ent);																			// fade in/out entities at a defined distance from camera, requires a shader with alpha blending enabled !!! or using no shader		

void		MapEnt_ReadObsFileData(		STRING* my_filename, 
												var* obs_defaultsize,
												var* obs_tilesize_x,
												var* obs_tilesize_y,
												var* obs_type,
												var* obs_collision,
												var* obs_shadow
											);	

//------------------------------------------
// shadows

void	MapEnt_SetShadow(ENTITY* ent, var mpe_lightmap, var mpe_decal, var mpe_shadow);

	void	MapEnt_SetNoshadow(ENTITY* ent);
	void	MapEnt_SetLightmap(ENTITY* ent);		
	void	MapEnt_SetSCSM(ENTITY* ent);
	void	MapEnt_SetPSSM(ENTITY* ent);
//	void	MapEnt_SetStencil(ENTITY* ent);
	void	MapEnt_SetSprite(ENTITY* ent, var mpe_decal);
		void	MapEnt_SetDecal(ENTITY* ent);
		void	MapEnt_SetEntDecal(ENTITY* ent);
	void	MapEnt_SetModel(ENTITY* ent);	

//------------------------------------------
// load/save

void 		MapEnt_Save_MapEnts_mem(char* file_name);
void		MapEnt_Load_MapEnts_mem(char* file_name, var import);

void 		MapEnt_CreateItem_FromFile(var imported, 
												STRING* my_filename, 
												var my_scale_x, my_scale_y, my_scale_z, 
												my_x, my_y, my_z, 
												my_pan, my_tilt, my_roll, 
												my_alpha, my_ambient, my_light, 
												my_red, my_green, my_blue, 
												my_unlit, my_overlay, my_material, 
												my_flag6, my_flag5);

//------------------------------------------
// removal

function MapEnt_RemoveObject(ENTITY* ent);																				// delete the given entity without checking
	void		MapEnt_RemoveChild(ENTITY* owner);																			// unique shadow sprite or model

//---
// not used by editor

int		MapEnt_RemoveArea(	
										int midtilex, 
										int midtiley, 
										int areasizex, 
										int areasizey
									);																									// remove all objects in the area, xy area were both building_tilesize i.e. placement area

//---
	
void		MapEnt_RemoveAll();																									// removes all the objects with the given type, nulls counter

//------------------------------------------


#endif