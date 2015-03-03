#ifndef MBdetail_h
#define MBdetail_h

////////////////////////////////////////////////////
// defines

#define DETAIL_SIZE1			256.0
#define DETAIL_SIZE2 		512.0

////////////////////////////////////////////////////
// variables

var detail_counter;													// number of details placed - nulled only at first init call !!!

// detail image size from file
var 		detail_defaultsize	= DETAIL_SIZE1;				// image size - could be determined for each sprite 
STRING*	detailsize_name = "grass_dds\\_detailsize.txt";

////////////////////////////////////////////////////////////////////////////////////////////////
// functions

//------------------------------------------
// create

void Detail_Create( STRING* filename, 
							var my_scale, 
							my_x, my_y, my_z, 
							my_alpha, my_ambient, 
							my_light, my_red, my_green, my_blue, 
							my_unlit, my_overlay, my_material);

	void		Detail_SetDefaultSize();							// sets default image size based on file value or to 256 high
	
//------------------------------------------
// shadows

void	Detail_SetShadow(ENTITY* ent);

//------------------------------------------
// load/save

void 		Detail_Save_Details_grs(char* file_name);
void		Detail_Load_Details_grs(char* file_name, var import);

void Detail_CreateItem_FromFile(	var imported, STRING* my_filename, 
											var my_scale, 
											my_x, my_y, my_z, 
											my_alpha, my_ambient, 
											my_light, my_red, my_green, my_blue, 
											my_unlit, my_overlay, my_material);

//------------------------------------------
// removal

function Detail_RemoveObject(ENTITY* ent);															// delete the given entity without checking

//---

int		Detail_RemoveArea(	
										int midtilex, 
										int midtiley, 
										int areasizex, 
										int areasizey
									);																				// remove all objects in the area, xy area were both building_tilesize i.e. placement area

//---

void		Detail_RemoveAll();

//------------------------------------------


#endif