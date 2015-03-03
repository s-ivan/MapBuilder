
#ifndef MBmaterials_h
#define MBmaterials_h

////////////////////////////////////
// materials

//	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;  	// own brightness
//	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;  // reflect sun light (terrains should have approx half than models)
//	specular_blue = 75;  	specular_green = 50;  	specular_red = 50;  	// towards sun
//	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;		// own light color
//	power = 5;																				// sharpness and intensity of specular
//	albedo = 100;																			// sun light response
	
MATERIAL* terrain_multi_mat =
{
	effect = "terraintexMB.fx"; 
	flags = PASS_SOLID; 																	// needed because of alpha mask blending
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 200;  	diffuse_green  = 200;  	diffuse_red  = 200;
	specular_blue = 0; 	 	specular_green = 0;  	specular_red = 0;  
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* terrain_single_mat =
{
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 200;  	diffuse_green  = 200;  	diffuse_red  = 200;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;  
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* water_multi_mat =
{	
	effect = "terraintex.fx"; 	
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 200;  	diffuse_green  = 200;  	diffuse_red  = 200;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* water_single_mat =
{
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 200;  	diffuse_green  = 200;  	diffuse_red  = 200;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* mtl_detail =
{
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* mtl_shrub =
{
	effect= "obj_standTree_MB.fx";
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* mtl_grass =
{	
	effect= "obj_standTree_MB.fx";
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* mtl_obstacle =
{
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

//MATERIAL* mtl_obstacle_transp =
//{
//	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;  
//	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;  
//	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;  
//	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
//	power = 2;
//	albedo = 100;
//	flags = TRANSLUCENT;																	// not used any more, engine bug is fixed
//}

MATERIAL* mtl_mapent =
{
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* mtl_terrmdl =
{
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 200;  	diffuse_green  = 200;  	diffuse_red  = 200;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* mtl_building =
{
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

//MATERIAL* mtl_building_transp =
//{
//	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;  
//	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;  
//	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;  
//	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
//	power = 2;
//	albedo = 100;
//	flags = TRANSLUCENT;																	// not used any more, engine bug is fixed
//}

// wth shader
MATERIAL* mtl_tree =
{
	effect= "obj_standTree_MB.fx";
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* mtl_decalshadow =															// not needed to be modifyable
{	
	ambient_blue  = 0;  		ambient_green  = 0;  	ambient_red  = 0;  
	diffuse_blue  = 0;  		diffuse_green  = 0;  	diffuse_red  = 0;  
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;  
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 0;
	albedo = 0;
}

MATERIAL* mtl_placementarea =															// not needed to be modifyable
{	
	ambient_blue  = 255;  	ambient_green  = 255;  	ambient_red  = 255;  // auto-tweaked runtime - ?
	diffuse_blue  = 0; 	 	diffuse_green  = 0;  	diffuse_red  = 0;  
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;  
	emissive_blue = 100;  	emissive_green = 100;  	emissive_red = 100;
	power = 0;
	albedo = 100;
}

//----------------------

// with shader - with alpha
MATERIAL* mtl_unit_ca_nh_s =
{
	effect = "specBumpMBa.fx";			// okay, maybe faster													- requires: color+alpha , normal+_ 		 , _+specular		// but okay with normal+height
//	effect = "specParallaxMBa.fx";	// okay but split in model where uv map not continuous 		- requires: color+alpha , normal+height , _+specular
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

// with shader
MATERIAL* mtl_unit_c_nh_s =
{
	effect = "specBumpMB.fx";			// okay, maybe faster													- requires: color , normal+_ 		 , _+specular				// but okay with normal+height
//	effect = "specParallaxMB.fx";		// okay but split in model where uv map not continuous 		- requires: color , normal+height , _+specular
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

// non-shader with alpha
MATERIAL* mtl_unit_nsa =
{
	effect =								
	  "										
	    technique alpha_set
	    {
	      pass p0
	      {
	        zWriteEnable = true;
	        alphaTestEnable = true;
	        alphaBlendEnable = false;
	      }
	    }
	  ";	
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

// non-shader
MATERIAL* mtl_unit_ns =
{
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* mtl_unit1_n =
{
	effect = "bump.fx";
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* mtl_bonesanim =
{
	effect= "obj_bones_MB.fx";
	ambient_blue  = 50;  	ambient_green  = 50;  	ambient_red  = 50;
	diffuse_blue  = 225;  	diffuse_green  = 225;  	diffuse_red  = 225;
	specular_blue = 0;  		specular_green = 0;  	specular_red = 0;
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

//MATERIAL* mtl_wireframe_mb =
//{
//	ambient_red = 255; 		ambient_green = 20; 		ambient_blue = 20;
//	
//	effect = "technique wireframe { pass one { Fillmode = Wireframe; } }";
//}


//////////////////////////////////////////////////////////////////////
// functions


void		Materials_ShadersInit();
void		Materials_ShadersRemove();

void		Materials_Load_Materials_mat(char* file_name);
	void		Materials_Load_Materials_Any(MATERIAL* mat_to_save , var file_handle);
	void		Materials_Load_Materials_Pair(MATERIAL* material_primary, MATERIAL* material_secondary);		// not used any more
	void		Materials_Load_Shader_Any(MATERIAL* material_actual, var file_handle);

void 		Materials_Save_Materials_mat(char* file_name);
	void		Materials_Save_Materials_Any(MATERIAL* mat_to_save , var file_handle);
	void		Materials_Save_Shader_Any(MATERIAL* material_actual , var file_handle);


//////////////////////////////////////////////////////////////////////


#endif