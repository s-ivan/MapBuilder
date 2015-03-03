///////////////////////////////
// system includes

#include <acknex.h>
#include <default.c>
#include <d3d9.h>			
#include <mtlFX.c>		// for terraintex.fx multiple detail skinning effect
////////////////////////////////////////////
// defines

#define ENT_CATEGORY				skill20	
#define UIMODE_GROUNDTERR		300

///////////////////////////////////
// variables 

char		map_tilesize 				= 32;							
var*		terrhmp_watervertices	= NULL;

/////////////////////////////////////
// entities

ENTITY*  terrain_entity;
ENTITY*	water_entity;

////////////////////////////////////
// materials

MATERIAL* terrain_multi_mat =
{
	effect = "terraintex.fx"; 
	ambient_blue  = 0;  		ambient_green  = 0;  	ambient_red  = 0;  
	diffuse_blue  = 175;  	diffuse_green  = 200;  	diffuse_red  = 200;  
	specular_blue = 50;  	specular_green = 60;  	specular_red = 60;  
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* terrain_single_mat =
{	
	ambient_blue  = 0;  		ambient_green  = 0;  	ambient_red  = 0;  
	diffuse_blue  = 175;  	diffuse_green  = 200;  	diffuse_red  = 200;  
	specular_blue = 50;  	specular_green = 60;  	specular_red = 60;  
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;
	power = 5;
	albedo = 100;
}

MATERIAL* water_multi_mat =
{	
	effect = "terraintex.fx"; 	
	ambient_blue  = 100;  	ambient_green  = 100;  	ambient_red  = 100;  // own brightness
	diffuse_blue  = 255;  	diffuse_green  = 255;  	diffuse_red  = 255;  // reflect sun light
	specular_blue = 175;  	specular_green = 150;  	specular_red = 150;  // towards sun
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;		// own light color
	power = 7;
	albedo = 100;
}

MATERIAL* water_single_mat =
{	
	ambient_blue  = 100;  	ambient_green  = 100;  	ambient_red  = 100;  // own brightness
	diffuse_blue  = 255;  	diffuse_green  = 255;  	diffuse_red  = 255;  // reflect sun light
	specular_blue = 175;  	specular_green = 150;  	specular_red = 150;  // towards sun
	emissive_blue = 0;  		emissive_green = 0;  	emissive_red = 0;		// own light color
	power = 7;
	albedo = 100;
}


///////////////////////////////////////////////
// code

// terrain action
action	TerrHmp_GroundAction()
{
   my.ENT_CATEGORY = UIMODE_GROUNDTERR;    					// to avoid deletion
	
	terrain_entity = me;
	
	if ( ent_status(me,8)>2 )
		{
			my.material = terrain_multi_mat;
		}
	else
		{
			my.material = terrain_single_mat;
		}	
//	vec_for_vertex(vScale, my, 1);								// vertex 1 = upper-left corners
	my.emask &= ~DYNAMIC;	
}

////////////////////////////////////////////////////////////////////////////////

function		TerrHmp_WaterTransparency()
{
	var screen_size_x = sys_metrics(0); // SM_CXSCREEN
	var screen_size_y = sys_metrics(1); // SM_CYSCREEN	
	VECTOR pos1, pos2;
	vec_set( pos1 , vector(screen_size_x/2,screen_size_y/2,0) );
	vec_for_screen (pos1, camera);	
	vec_set( pos2 , vector(screen_size_x/2,screen_size_y/2,20000) );        		
	vec_for_screen (pos2, camera);
	// measure terrain distance from camera fixed to screen middle
	c_trace (pos1.x, pos2.x, IGNORE_SPRITES | IGNORE_MODELS | IGNORE_MAPS ); 
	if (HIT_TARGET)  	
		{
					VECTOR vtemp;
					var temp = vec_dist(pos1,hit.x);
					
					if ((temp<map_tilesize*100) && (camera.z<map_tilesize*15))			// look at short distance, and camera is at low height
						{							
							var temp2 = (100 - map_tilesize*15/10) + camera.z/10 ;		// limits determined by cam height and transparency border height
							my.alpha = temp2;							
							set( my , TRANSLUCENT );
						}
					else
						{
							reset( my , TRANSLUCENT );
						}
		}
	else
		{
			reset( my , TRANSLUCENT );
		}
}


// water terrain settings
action	TerrHmp_WaterAction()
{		
	var wave = 1;
	// scale water to terrain--------- not okay in A8, so scale in WED exactly
//	VECTOR vWater;       
//	vec_zero(vWater);
//	vec_for_vertex(vWater, my, 1);								// vertex 1 = upper-left corner
//	my.scale_x = abs(vScale.x/vWater.x); 
//	my.scale_y = abs(vScale.y/vWater.y);
	//--------------------------------
	my.ENT_CATEGORY = UIMODE_GROUNDTERR;    					// to avoid deletion
	set( my , PASSABLE );	
	if ( ent_status(me,8)>2 )
		{
			my.material = water_multi_mat;
			wave = 0;
		}
	else
		{
			my.material = water_single_mat;
		}
	my.alpha = 85;														// it does not really work if placed inside material definition...
	//--------------------------------
	wait(10);	
sys_marker("wt0");	
	// water vertex array
	int j = ent_status(my,0); // number of vertices 
	terrhmp_watervertices = (var*)sys_malloc( j * sizeof(var) );
	// randomize surface
	random_seed(0);	
	// or randomize angle shift	 - okay
	for (; j>0; j--) 
		terrhmp_watervertices[j-1] = random(360);	
sys_marker(NULL);		
	wait(10);
	//-------------------------------------------------------------------
	// basic water animation
	int j = ent_status(my,0); 			// number of vertices again
	var i = 0;
	int k = 0;
	while(1)
		{	
			// transparency on close camera, intranspareny from far to avoid transparent entity sorting problems
			TerrHmp_WaterTransparency();
			//---------------------------------------------------------
			if (wave)
				{
					// waves by texture shift
					my.v -= 0.05*cosv(i)*time_step;
					my.u -= 0.05*time_step;			
				}			
				i += 1;
				i %= 360;							
			//----------------------------------------------------------
			// waves by moving vertices up and down - 0 terrain chunk value is needed !
			// modify all vertices in each frame		
			for (k=0; k<j; k++) 
				{
					CONTACT* c = ent_getvertex(my,NULL,k+1); 
					c.v.y = sinv(i + terrhmp_watervertices[k])  *map_tilesize/4;											// waving  *amplitude A8			
				}
			// i += waterspeed * time_step;	// to adjust speed if needed
	//		c_setminmax(my);
			//-----------------------------------------------------------
			wait(1);
		}
}


////////////////////////////////////////////////////////////////////


void main()
{
	
	// general video settings-------
	fps_max = 40; 					// limit the frame rate
	fps_min = 16; 					// avoid huge moves in case of very low frame rate
	video_mode = 8; 				// run in 1024x768 pixels
	video_depth = 32; 			// start the engine in 32 bit mode
	video_screen = 1; 			// start in full screen mode
	preload_mode = 3+4+8; 		// preload level entities, created and cloned entities, and precalculate their environment light
   tex_share = 1; 				// same entities share texture memoy	
	d3d_triplebuffer = 1; 		// activate Triple Buffering - more memory needed but higher frame rate achievable	
	d3d_entsort = 2;				// 2: sort transparent entities based on XY distance from camera instead of XYZ distance - or 6 for all the enities
	d3d_alpharef = 75;			// overlay transparency treshold level 0..255, default=127
	if (d3d_caps & 2) 			// good cards only 
		{
			d3d_anisotropy = 2; 	// 1..7 anisotropic filtering active - needed by anisotropic mipmapping
			d3d_mipmapping = 4;	// anisotropic mipmapping - much better terrain texture
		} 
	terrain_lod = 4;
	shadow_stencil = 1;
	//-------------------------------
	
	level_load("dry_level.wmb");	
	vec_set( camera.x , vector(0,0,1000) );
	wait(3);	
	ent_createlayer("scube02b_clearnoon+6.dds",SKY | CUBE | SHOW,1);
	
	def_move();

	while(1)
		{
//			draw_text( "level_ent min/max x y z", 50 , 125 , COLOR_WHITE );
//				
//			draw_text( str_for_num(NULL,level_ent->min_x) , 50 , 150 , COLOR_WHITE );
//			draw_text( str_for_num(NULL,level_ent->min_y) , 150 , 150 , COLOR_WHITE );
//			draw_text( str_for_num(NULL,level_ent->min_z) , 250 , 150 , COLOR_WHITE );
//			
//			draw_text( str_for_num(NULL,level_ent->max_x) , 50 , 175 , COLOR_WHITE );
//			draw_text( str_for_num(NULL,level_ent->max_y) , 150 , 175 , COLOR_WHITE );
//			draw_text( str_for_num(NULL,level_ent->max_z) , 250 , 175 , COLOR_WHITE );
//			
//			draw_text( "terrain_entity min/max x y z", 50 , 200 , COLOR_WHITE );
//			
//			draw_text( str_for_num(NULL,terrain_entity.min_x) , 50 , 225 , COLOR_WHITE );
//			draw_text( str_for_num(NULL,terrain_entity.min_y) , 150 , 225 , COLOR_WHITE );
//			draw_text( str_for_num(NULL,terrain_entity.min_z) , 250 , 225 , COLOR_WHITE );
//			
//			draw_text( str_for_num(NULL,terrain_entity.max_x) , 50 , 250 , COLOR_WHITE );
//			draw_text( str_for_num(NULL,terrain_entity.max_y) , 150 , 250 , COLOR_WHITE );
//			draw_text( str_for_num(NULL,terrain_entity.max_z) , 250 , 250 , COLOR_WHITE );
			
			wait(1);				
		}		
}
