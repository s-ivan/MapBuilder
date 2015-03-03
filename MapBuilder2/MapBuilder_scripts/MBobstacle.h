
#ifndef MBobstacle_h
#define MBobstacle_h

////////////////////////////////////////////////////////////////////////////////////////////////
// variables

var obstacle_counter;											// number of obstacles placed - nulled only at first init call !!!

////////////////////////////////////////////////////////////////////////////////////////////////
// functions

void		Obstacle_Create(	STRING* filename, 
									var obs_defaultsize,
									obs_tilesize_x, obs_tilesize_y,
									obs_type, obs_collision, obs_shadow,									
									var my_scale_x, my_scale_y, my_scale_z,		
									my_x, my_y, my_z, 
									my_pan, my_tilt, my_roll,									
									my_alpha, my_ambient, 
									my_light, my_red, my_green, my_blue, 
									my_unlit, my_overlay, my_material,										
									my_flag7, my_flag6, my_flag5);															// general obstacle entity creation

void		Obstacle_CreateCollider(ENTITY* ent);																			// collider entity
//void		Obstacle_Created_Loop(ENTITY* ent);																				// fade in/out entities at a defined distance from camera, requires a shader with alpha blending enabled !!! or using no shader								

void		Obstacle_ReadObsFileData(	STRING* my_filename, 
												var* obs_defaultsize,
												var* obs_tilesize_x,
												var* obs_tilesize_y,
												var* obs_type,
												var* obs_collision,
												var* obs_shadow
											);																							// read object data from .obs file created by editor

//------------------------------------------
// tree related

	void		Obstacle_CreateLODmodels(ENTITY* ent);										// check for lod models and create if does not exist, only for trees! because of special material and normals
	action	Obstacle_CreateTreeLOD();														// create lod models for trees to have proper material and normals set
		void		Obstacle_SetTreeMaterial(ENTITY* ent);									// set mtl_tree for tree leaf skin 
			void		Obstacle_SetTreeNormals(ENTITY* tree_ent, int skin_num);		// set leaf normals properly	

//------------------------------------------
// shadows

void	Obstacle_SetShadow(ENTITY* ent, var obs_lightmap, var obs_decal, var obs_shadow, var obs_tree);

	void	Obstacle_SetNoshadow(ENTITY* ent);
	void	Obstacle_SetLightmap(ENTITY* ent);		
	void	Obstacle_SetSCSM(ENTITY* ent);
	void	Obstacle_SetPSSM(ENTITY* ent);
	void	Obstacle_SetStencil(ENTITY* ent);
	void	Obstacle_SetSprite(ENTITY* ent, var obs_decal, var obs_tree);
		void	Obstacle_SetDecal(ENTITY* ent, var obs_tree);
		void	Obstacle_SetEntDecal(ENTITY* ent, var obs_tree);
	void	Obstacle_SetModel(ENTITY* ent);	

//------------------------------------------
// load/save

void 		Obstacle_Save_Obstacles_obm(char* file_name);
void		Obstacle_Load_Obstacles_obm(char* file_name, var import);

	void 		Obstacle_CreateItem_FromFile(var imported, STRING* my_filename, 
														var my_scale_x, my_scale_y, my_scale_z, 
														my_x, my_y, my_z, 
														my_pan, my_tilt, my_roll, 
														my_alpha, my_ambient, 
														my_light, my_red, my_green, my_blue, 
														my_unlit, my_overlay, my_material, 
														my_flag7, my_flag6, my_flag5);
												
//------------------------------------------
// removal

function Obstacle_RemoveObject(ENTITY* ent);																				// delete the given entity without checking
	void		Obstacle_RemoveChild(ENTITY* owner);																		// unique shadow sprite or model

//---

int		Obstacle_RemoveArea(	
										int midtilex, 
										int midtiley, 
										int areasizex, 
										int areasizey
									);																									// remove all objects in the area, xy area were both building_tilesize i.e. placement area

//---

void		Obstacle_RemoveAll();

//------------------------------------------


#endif