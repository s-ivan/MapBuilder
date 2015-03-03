
#ifndef MBmaterialeditor_c
#define MBmaterialeditor_c

//#include "MBmaterialeditor.h"
//#include "MBgui_common.h"	
//#include "MBuihandling.h"
//#include "MBmaterials.h"
//#include "MBscwater.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


void 	Material_Init()
{
//	UIMode = (var)UIMODE_MATERIAL;
	
	Material_LowPanel_Init();	
}


void 	Material_Close()
{
	if (material_matpanel!=NULL)
		{
			Material_MatPanel_Close();
		}	
	
	// low panel
	if (material_lowpanel)
		{
			ptr_remove(material_lowpanel);
			material_lowpanel = NULL;
		}
}


void	Material_LowPanel_Init()
{
	material_lowpanel = pan_create( panel_image2 , 11 );
	set( material_lowpanel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	material_lowpanel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	material_lowpanel.pos_y 	= ui_submenupanel_y;								// 256;
	material_lowpanel.red		= panel_font_r;
	material_lowpanel.green		= panel_font_g;
	material_lowpanel.blue		= panel_font_b;
	material_lowpanel.event		= ui_submenu_panelmove;
	
	//string 1 - title
	pan_setstring(material_lowpanel, 0,	15,10,	panel_font2,			str_create("MATERIAL SETTINGS") );
	
	//string 2-10 - built in materials
	pan_setstring(material_lowpanel, 0,	15,40,	panel_font2,		str_create("3DGS materials:") );
	pan_setstring(material_lowpanel, 0,	40,70,	panel_font,			str_create("terrain") );
	pan_setstring(material_lowpanel, 0,	40,90,	panel_font,			str_create("sprite") );	
	pan_setstring(material_lowpanel, 0,	40,110,	panel_font,			str_create("model") );			
	pan_setstring(material_lowpanel, 0,	40,130,	panel_font,			str_create("unlit") );	
	pan_setstring(material_lowpanel, 0,	40,150,	panel_font,			str_create("shadow") );
	pan_setstring(material_lowpanel, 0,	40,170,	panel_font,			str_create("flat") );
	pan_setstring(material_lowpanel, 0,	40,190,	panel_font,			str_create("shaded") );
	pan_setstring(material_lowpanel, 0,	40,210,	panel_font,			str_create("metal") );
	pan_setstring(material_lowpanel, 0,	40,230,	panel_font,			str_create("sky") );
	
	//string 11-. - entity category materials
	pan_setstring(material_lowpanel, 0,	15,260,	panel_font2,		str_create("MapBuilder materials:") );
	pan_setstring(material_lowpanel, 0,	40,290,	panel_font,			str_create("ground") );
	pan_setstring(material_lowpanel, 0,	40,310,	panel_font,			str_create("water") );	
	pan_setstring(material_lowpanel, 0,	40,330,	panel_font,			str_create("grass sprite") );			
	pan_setstring(material_lowpanel, 0,	40,350,	panel_font,			str_create("shrub sprite") );	
	pan_setstring(material_lowpanel, 0,	40,370,	panel_font,			str_create("grass model") );
//	pan_setstring(material_lowpanel, 0,	40,390,	panel_font,			str_create("shrub model") );		// not used
	pan_setstring(material_lowpanel, 0,	40,390,	panel_font,			str_create("obstacle model") );
	pan_setstring(material_lowpanel, 0,	40,410,	panel_font,			str_create("map entity") );
	pan_setstring(material_lowpanel, 0,	40,430,	panel_font,			str_create("terrain model") );
	pan_setstring(material_lowpanel, 0,	40,450,	panel_font,			str_create("tree model") );
	pan_setstring(material_lowpanel, 0,	40,470,	panel_font,			str_create("building model") );
//	pan_setstring(material_lowpanel, 0,	40,490,	panel_font,			str_create("unit model") );
	
	//button 1 - mat_terrain
	pan_setbutton(material_lowpanel, 0,	1, 	150,70,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL); 	
	//button 2 - mat_sprite
	pan_setbutton(material_lowpanel, 0,	1, 	150,90,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 3 - mat_model
	pan_setbutton(material_lowpanel, 0,	1, 	150,110,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 4 - mat_unlit
	pan_setbutton(material_lowpanel, 0,	1, 	150,130,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 5 - mat_shadow
	pan_setbutton(material_lowpanel, 0,	1, 	150,150,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 6 - mat_flat
	pan_setbutton(material_lowpanel, 0,	1, 	150,170,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 7 - mat_shaded
	pan_setbutton(material_lowpanel, 0,	1, 	150,190,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 8 - mat_metal
	pan_setbutton(material_lowpanel, 0,	1, 	150,210,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 9 - mat_sky
	pan_setbutton(material_lowpanel, 0,	1, 	150,230,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	
	//button 10 - ground
	pan_setbutton(material_lowpanel, 0,	1, 	150,290,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 11 - water
	pan_setbutton(material_lowpanel, 0,	1, 	150,310,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 12 - grass sprite
	pan_setbutton(material_lowpanel, 0,	1, 	150,330,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 13 - shrub sprite
	pan_setbutton(material_lowpanel, 0,	1, 	150,350,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 14 - grass model
	pan_setbutton(material_lowpanel, 0,	1, 	150,370,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
//	//button 15 - shrub model
//	pan_setbutton(material_lowpanel, 0,	1, 	150,390,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 15 - obstacle model
	pan_setbutton(material_lowpanel, 0,	1, 	150,390,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 16 - map entity wmb
	pan_setbutton(material_lowpanel, 0,	1, 	150,410,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 17 - terrain model
	pan_setbutton(material_lowpanel, 0,	1, 	150,430,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 18 - tree model
	pan_setbutton(material_lowpanel, 0,	1, 	150,450,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	//button 19 - building model
	pan_setbutton(material_lowpanel, 0,	1, 	150,470,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
//	//button 20 - unit model
//	pan_setbutton(material_lowpanel, 0,	1, 	150,490,		ss_set_on_bmp,ss_set_off_bmp,ss_set_over_bmp,ss_set_over_bmp,		Material_LowPanel_Select			,NULL,NULL);
	
	while((UIMode==(var)UIMODE_MATERIAL) || (UIMode==(var)UIMODE_FILEMENU2))
		{
			wait(1);
		}
		
	Material_Close();
}


void	Material_LowPanel_Select(button_number)
{
	wait(3);
	while (mouse_left)
		{
			wait(1);
		}
	
	material_select = button_number;
	
	if (button_number==(var)1)
		{
			Material_MatPanel_Init( mat_terrain , NULL );
		}
	else if (button_number==(var)2)
		{
			Material_MatPanel_Init( mat_sprite , NULL );
		}
	else if (button_number==(var)3)
		{
			Material_MatPanel_Init( mat_model , NULL );
		}
	else if (button_number==(var)4)
		{
			Material_MatPanel_Init( mat_unlit , NULL );
		}
	else if (button_number==(var)5)
		{
			Material_MatPanel_Init( mat_shadow , NULL );
		}
	else if (button_number==(var)6)
		{
			Material_MatPanel_Init( mat_flat , NULL );
		}
	else if (button_number==(var)7)
		{
			Material_MatPanel_Init( mat_shaded , NULL );
		}
	else if (button_number==(var)8)
		{
			Material_MatPanel_Init( mat_metal , NULL );
		}	
	else if (button_number==(var)9)
		{
			Material_MatPanel_Init( mat_sky , NULL );
		}
	//--------------------------------------------------	
	else if (button_number==(var)10)
		{
			if (terrain_entity)
				{
					if (terrain_entity.material == terrain_multi_mat)
						{
							Material_MatPanel_Init( terrain_multi_mat , terrain_single_mat );
						}
					else if (terrain_entity.material == terrain_single_mat)
						{
							Material_MatPanel_Init( terrain_single_mat , terrain_multi_mat );
						}
					else
						{
							Material_MatPanel_Init( terrain_entity.material , NULL );
						}
					
					if (str_stri(terrain_entity.material.effect, "auto"))
						{
							Material_TerrhmpAutoTexPanel_Init();
						}
				}
			else
				{
					Material_MatPanel_Init( terrain_single_mat , terrain_multi_mat );		// default
				}
		}
	else if (button_number==(var)11)
		{
			if (water_entity)
				{
					if (water_entity.material == water_multi_mat)
						{
							Material_MatPanel_Init( water_multi_mat , water_single_mat );
						}
					else if (water_entity.material == water_single_mat)
						{
							Material_MatPanel_Init( water_single_mat , water_multi_mat );
						}
					else
						{
							Material_MatPanel_Init( water_entity.material , NULL );
						}
										
					if (main_water > (var)0)
						{
							// shade-c water - water editor	
							Material_ScWaterPanel12_Init();
						}
					else if (main_water == (var)0)
						{
							// 3dgs mirror water - water editor
							Material_MWaterPanel_Init();
						}
				}
			else
				{
					Material_MatPanel_Init( water_multi_mat , water_single_mat );	// default
				}
		}
	else if (button_number==(var)12)
		{
			Material_MatPanel_Init( mtl_detail , NULL );
		}
	else if (button_number==(var)13)
		{
			Material_MatPanel_Init( mtl_shrub , NULL );
		}	
	else if (button_number==(var)14)
		{
			Material_MatPanel_Init( mtl_grass , NULL );
		}
	else if (button_number==(var)15)
		{
			Material_MatPanel_Init( mtl_obstacle , NULL );		// NULL was mtl_obstacle_transp
		}	
	else if (button_number==(var)16)
		{
			Material_MatPanel_Init( mtl_mapent , NULL );
		}
	else if (button_number==(var)17)
		{
			Material_MatPanel_Init( mtl_terrmdl , NULL );
		}
	else if (button_number==(var)18)
		{
			Material_MatPanel_Init( mtl_tree , NULL );
		}
	else if (button_number==(var)19)
		{
			Material_MatPanel_Init( mtl_building , NULL );
		}
//	else if (button_number==(var)20)
//		{
//			Material_MatPanel_Init( mtl_unit , NULL );
//		}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Material_MatPanel_Init(MATERIAL* material_primary, MATERIAL* material_secondary)
{
	// set pointers to primary material - and later in loop set secondary material to be equal, if extists
	material_amb_red 		= &material_primary.ambient_red;
	material_amb_green 	= &material_primary.ambient_green;
	material_amb_blue 	= &material_primary.ambient_blue;
	
	material_dif_red		= &material_primary.diffuse_red;
	material_dif_green	= &material_primary.diffuse_green;
	material_dif_blue		= &material_primary.diffuse_blue;
	
	material_emi_red		= &material_primary.emissive_red;
	material_emi_green	= &material_primary.emissive_green;
	material_emi_blue		= &material_primary.emissive_blue;
	
	material_spe_red		= &material_primary.specular_red;
	material_spe_green	= &material_primary.specular_green;
	material_spe_blue		= &material_primary.specular_blue;
	
	material_power			= &material_primary.power;
	material_albedo		= &material_primary.albedo;
	
	material_actual		= material_primary;
	
	// panel creation
	material_matpanel = pan_create( panel_image2 , 12 );
	set( material_matpanel , SHOW | SHADOW | OVERLAY );						// TRANSLUCENT | 
	material_matpanel.pos_x 	= ui_submenupanel_x;								// screen_size.x-256;
	material_matpanel.pos_y 	= ui_submenupanel_y;								// 256;
	material_matpanel.red		= panel_font_r;
	material_matpanel.green		= panel_font_g;
	material_matpanel.blue		= panel_font_b;
	material_matpanel.event		= ui_submenu_panelmove;
	
	// protection to allow outer call from object placement submenus
	if (material_lowpanel)
		{
			reset(material_lowpanel, SHOW);
		}
	
	//-----------------------------------------------------------------------------------------------------------------
	pan_setstring(material_matpanel, 0,	15,10,	panel_font2,			str_create("MATERIAL SETTINGS") );
	
	// ambient
	pan_setstring(material_matpanel, 0,	15,060,	panel_font2,				str_create("Ambient:") );
	pan_setdigits(material_matpanel, 0,	15,080,	"R: %0.f",				panel_font,	1, 			*material_amb_red);
	pan_setdigits(material_matpanel, 0,	15,100,	"G: %0.f",				panel_font,	1, 			*material_amb_green);
	pan_setdigits(material_matpanel, 0,	15,120,	"B: %0.f",				panel_font,	1, 			*material_amb_blue);
	pan_setslider(material_matpanel, 0,	72,082,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_amb_red); 
	pan_setslider(material_matpanel, 0,	72,102,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_amb_green); 
	pan_setslider(material_matpanel, 0,	72,122,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_amb_blue); 
	
	// diffuse
	pan_setstring(material_matpanel, 0,	15,140,	panel_font2,				str_create("Diffuse:") );
	pan_setdigits(material_matpanel, 0,	15,160,	"R: %0.f",				panel_font,	1, 			*material_dif_red);
	pan_setdigits(material_matpanel, 0,	15,180,	"G: %0.f",				panel_font,	1, 			*material_dif_green);
	pan_setdigits(material_matpanel, 0,	15,200,	"B: %0.f",				panel_font,	1, 			*material_dif_blue);
	pan_setslider(material_matpanel, 0,	72,162,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_dif_red); 
	pan_setslider(material_matpanel, 0,	72,182,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_dif_green); 
	pan_setslider(material_matpanel, 0,	72,202,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_dif_blue); 
	
	// emissive
	pan_setstring(material_matpanel, 0,	15,220,	panel_font2,				str_create("Emissive:") );
	pan_setdigits(material_matpanel, 0,	15,240,	"R: %0.f",				panel_font,	1, 			*material_emi_red);
	pan_setdigits(material_matpanel, 0,	15,260,	"G: %0.f",				panel_font,	1, 			*material_emi_green);
	pan_setdigits(material_matpanel, 0,	15,280,	"B: %0.f",				panel_font,	1, 			*material_emi_blue);
	pan_setslider(material_matpanel, 0,	72,242,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_emi_red); 
	pan_setslider(material_matpanel, 0,	72,262,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_emi_green); 
	pan_setslider(material_matpanel, 0,	72,282,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_emi_blue);
	
	// specular
	pan_setstring(material_matpanel, 0,	15,300,	panel_font2,				str_create("Specular:") );
	pan_setdigits(material_matpanel, 0,	15,320,	"R: %0.f",				panel_font,	1, 			*material_spe_red);
	pan_setdigits(material_matpanel, 0,	15,340,	"G: %0.f",				panel_font,	1, 			*material_spe_green);
	pan_setdigits(material_matpanel, 0,	15,360,	"B: %0.f",				panel_font,	1, 			*material_spe_blue);
	pan_setslider(material_matpanel, 0,	72,322,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_spe_red); 
	pan_setslider(material_matpanel, 0,	72,342,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_spe_green); 
	pan_setslider(material_matpanel, 0,	72,362,	horiz_slider_bmp,slider_knob_bmp,	0,255,	*material_spe_blue);
	
	// power
	pan_setstring(material_matpanel, 0,	15,380,	panel_font2,				str_create("Power:") );
	pan_setdigits(material_matpanel, 0,	15,400,	"P: %0.f",				panel_font,	1, 			*material_power);
	pan_setslider(material_matpanel, 0,	72,402,	horiz_slider_bmp,slider_knob_bmp,	0,10,		*material_power); 
	
	// albedo: influence of the sun light: -100 no sun, 100 default, 200 total sun
	pan_setstring(material_matpanel, 0,	15,420,	panel_font2,				str_create("Albedo:") );
	pan_setdigits(material_matpanel, 0,	15,440,	"A: %0.f",				panel_font,	1, 				*material_albedo);
	pan_setslider(material_matpanel, 0,	72,442,	horiz_slider_bmp,slider_knob_bmp,	-100,200,	*material_albedo); 
	
	// button 1-2: ok(set) , reset
	pan_setbutton(material_matpanel, 0,	1, 	050,480,		s_ok_on_bmp,s_ok_off_bmp,s_ok_over_bmp,s_ok_over_bmp,						Material_MatPanel_Close		,NULL,NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	150,480,		s_reset_on_bmp,s_reset_off_bmp,s_reset_over_bmp,s_reset_over_bmp,		Material_Reset					,NULL,NULL); 
	
	//panel1 - ambient
	material_colpan1 = pan_create( material_color1 , 13 );
	set( material_colpan1 , SHOW );					
	material_colpan1.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
	material_colpan1.pos_y = ui_submenupanel_y + 75;									// 256+075;
	bmap_fill( material_color1_bmp , material_primary.ambient_blue , 100 );
	material_colpan1.bmap = material_color1_bmp;
	material_colpan1.event = Material_ChangeColor;
	
	//panel2 - diffuse
	material_colpan2 = pan_create( material_color2 , 13 );
	set( material_colpan2 , SHOW );					
	material_colpan2.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
	material_colpan2.pos_y = ui_submenupanel_y + 155;									// 256+155;
	bmap_fill( material_color2_bmp , material_primary.diffuse_blue , 100 );
	material_colpan2.bmap = material_color2_bmp;
	material_colpan2.event = Material_ChangeColor;
	
	//panel3 - emissive
	material_colpan3 = pan_create( material_color3 , 13 );
	set( material_colpan3 , SHOW );					
	material_colpan3.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
	material_colpan3.pos_y = ui_submenupanel_y + 235;									// 256+235;
	bmap_fill( material_color3_bmp , material_primary.emissive_blue , 100 );
	material_colpan3.bmap = material_color3_bmp;
	material_colpan3.event = Material_ChangeColor;
	
	//panel4 - specular
	material_colpan4 = pan_create( material_color4 , 13 );
	set( material_colpan4 , SHOW );					
	material_colpan4.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
	material_colpan4.pos_y = ui_submenupanel_y + 315;									// 256+315;
	bmap_fill( material_color4_bmp , material_primary.specular_blue , 100 );
	material_colpan4.bmap = material_color4_bmp;
	material_colpan4.event = Material_ChangeColor;
	
	//button 3-30, fine setting arrows around sliders (at 72) at 60 and 180
	// ambient
	pan_setbutton(material_matpanel, 0,	1, 	060,080,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,080,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	060,100,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,100,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	060,120,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,120,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	// diffuse
	pan_setbutton(material_matpanel, 0,	1, 	060,160,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,160,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	060,180,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,180,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	060,200,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,200,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	// emissive
	pan_setbutton(material_matpanel, 0,	1, 	060,240,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,240,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	060,260,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,260,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	060,280,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,280,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	// specular
	pan_setbutton(material_matpanel, 0,	1, 	060,320,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,320,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	060,340,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,340,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	060,360,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,360,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	// power
	pan_setbutton(material_matpanel, 0,	1, 	060,400,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,400,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	// albedo
	pan_setbutton(material_matpanel, 0,	1, 	060,440,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				Material_MatPanel_Button, 		NULL, NULL); 
	pan_setbutton(material_matpanel, 0,	1, 	180,440,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		Material_MatPanel_Button, 		NULL, NULL); 
	
	// effect - string 22 23
	pan_setstring(material_matpanel, 0,	15,40,	panel_font2,		str_create("Fx:") );
	if (material_actual.effect != NULL)
		{
//			pan_setstring(material_matpanel, 0,	60,40,	panel_font,			str_create( material_primary.effect ) );
			pan_setstring(material_matpanel, 0,	40,40,	panel_font,			str_create( material_actual.effect ) );
		}
	else
		{
			pan_setstring(material_matpanel, 0,	40,40,	panel_font,			str_create( "none" ) );
		}
	pan_setbutton(material_matpanel, 0,	1, 	190,30,		ss_load_on_bmp,ss_load_off_bmp,ss_load_over_bmp,ss_load_over_bmp,				Material_Effect_Load			,NULL,NULL); 	
	pan_setbutton(material_matpanel, 0,	1, 	190,50,		ss_clear_on_bmp,ss_clear_off_bmp,ss_clear_over_bmp,ss_clear_over_bmp,		Material_Effect_Clear		,NULL,NULL); 	
	
	
	// loop
	while(material_matpanel)
		{
//			// move panels-------------------------------------------------------------- MOVED TO EVENT to eliminate lagging
//			if (material_lowpanel)
//				{
//					material_lowpanel.pos_x = ui_submenupanel_x;							
//					material_lowpanel.pos_y = ui_submenupanel_y;				
//				}
			
			if ( (material_colpan1) && (material_colpan2) && (material_colpan3) && (material_colpan4) )
				{
//					// move panels-------------------------------------------------------------- MOVED TO EVENT to eliminate lagging
//					material_colpan1.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
//					material_colpan1.pos_y = ui_submenupanel_y + 75;									// 256+075;
//					
//					material_colpan2.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
//					material_colpan2.pos_y = ui_submenupanel_y + 155;									// 256+155;
//					
//					material_colpan3.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
//					material_colpan3.pos_y = ui_submenupanel_y + 235;									// 256+235;
//					
//					material_colpan4.pos_x = ui_submenupanel_x + 205;									// screen_size.x-256+205;
//					material_colpan4.pos_y = ui_submenupanel_y + 315;									// 256+315;
					
					//set colours---------------------------------------------------------------
					bmap_fill( material_color1_bmp , material_primary.ambient_blue , 100 );
					material_colpan1.bmap = material_color1_bmp;
					
					bmap_fill( material_color2_bmp , material_primary.diffuse_blue , 100 );
					material_colpan2.bmap = material_color2_bmp;
					
					bmap_fill( material_color3_bmp , material_primary.emissive_blue , 100 );
					material_colpan3.bmap = material_color3_bmp;
					
					bmap_fill( material_color4_bmp , material_primary.specular_blue , 100 );
					material_colpan4.bmap = material_color4_bmp;
				}
			
			//set sub-materials--------------------------------------------------------
			if (material_secondary)
				{
					material_secondary.ambient_red 		= material_primary.ambient_red;
					material_secondary.ambient_green 	= material_primary.ambient_green;
					material_secondary.ambient_blue 		= material_primary.ambient_blue;
					
					material_secondary.diffuse_red 		= material_primary.diffuse_red;
					material_secondary.diffuse_green 	= material_primary.diffuse_green;
					material_secondary.diffuse_blue 		= material_primary.diffuse_blue;
					
					material_secondary.emissive_red 		= material_primary.emissive_red;
					material_secondary.emissive_green 	= material_primary.emissive_green;
					material_secondary.emissive_blue 	= material_primary.emissive_blue;
					
					material_secondary.specular_red 		= material_primary.specular_red;
					material_secondary.specular_green 	= material_primary.specular_green;
					material_secondary.specular_blue 	= material_primary.specular_blue;
					
					material_secondary.power 				= material_primary.power;
					material_secondary.albedo 				= material_primary.albedo;
				}						
			
			//---------------------------------------------------------------------------
					
			wait(1);
		}		
}


void		Material_MatPanel_Close()
{
	if (material_lowpanel)
		{
			set(material_lowpanel, SHOW);	
		}	
	
	if (material_matpanel)
		{
			ptr_remove(material_matpanel);
			material_matpanel = NULL;	
		}
	
	ptr_remove(material_colpan1);
	ptr_remove(material_colpan2);
	ptr_remove(material_colpan3);
	ptr_remove(material_colpan4);	
	material_colpan1 = NULL;
	material_colpan2 = NULL;
	material_colpan3 = NULL;
	material_colpan4 = NULL;		
	
	material_actual = NULL;
	
	//--------------------------------
	
	Material_ScWaterPanel12_Close();
	
	Material_MWaterPanel_Close();
	
	Material_TerrhmpAutoTexPanel_Close();
	
	//--------------------------------
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Material_MatPanel_Button(button_number)
{
	// ambient
	if (button_number==(var)3)
		{
			*material_amb_red = maxv( 0 , --*material_amb_red );
		}
	else if (button_number==(var)4)
		{
			*material_amb_red = minv( 255 , ++*material_amb_red );
		}
	else if (button_number==(var)5)
		{
			*material_amb_green = maxv( 0 , --*material_amb_green );
		}
	else if (button_number==(var)6)
		{
			*material_amb_green = minv( 255 , ++*material_amb_green );
		}
	else if (button_number==(var)7)
		{
			*material_amb_blue = maxv( 0 , --*material_amb_blue );
		}
	else if (button_number==(var)8)
		{
			*material_amb_blue = minv( 255 , ++*material_amb_blue );
		}
	// diffuse
	else if (button_number==(var)9)
		{
			*material_dif_red = maxv( 0 , --*material_dif_red );
		}
	else if (button_number==(var)10)
		{
			*material_dif_red = minv( 255 , ++*material_dif_red );
		}
	else if (button_number==(var)11)
		{
			*material_dif_green = maxv( 0 , --*material_dif_green );
		}
	else if (button_number==(var)12)
		{
			*material_dif_green = minv( 255 , ++*material_dif_green );
		}
	else if (button_number==(var)13)
		{
			*material_dif_blue = maxv( 0 , --*material_dif_blue );
		}
	else if (button_number==(var)14)
		{
			*material_dif_blue = minv( 255 , ++*material_dif_blue );
		} 
	// emissive
	else if (button_number==(var)15)
		{
			*material_emi_red = maxv( 0 , --*material_emi_red );
		}
	else if (button_number==(var)16)
		{
			*material_emi_red = minv( 255 , ++*material_emi_red );
		}
	else if (button_number==(var)17)
		{
			*material_emi_green = maxv( 0 , --*material_emi_green );
		}
	else if (button_number==(var)18)
		{
			*material_emi_green = minv( 255 , ++*material_emi_green );
		}
	else if (button_number==(var)19)
		{
			*material_emi_blue = maxv( 0 , --*material_emi_blue );
		}
	else if (button_number==(var)20)
		{
			*material_emi_blue = minv( 255 , ++*material_emi_blue );
		} 
	// specular
	else if (button_number==(var)21)
		{
			*material_spe_red = maxv( 0 , --*material_spe_red );
		}
	else if (button_number==(var)22)
		{
			*material_spe_red = minv( 255 , ++*material_spe_red );
		}
	else if (button_number==(var)23)
		{
			*material_spe_green = maxv( 0 , --*material_spe_green );
		}
	else if (button_number==(var)24)
		{
			*material_spe_green = minv( 255 , ++*material_spe_green );
		}
	else if (button_number==(var)25)
		{
			*material_spe_blue = maxv( 0 , --*material_spe_blue );
		}
	else if (button_number==(var)26)
		{
			*material_spe_blue = minv( 255 , ++*material_spe_blue );
		} 
	// power
	else if (button_number==(var)27)
		{
			*material_power = maxv( 0 , --*material_power );
		}
	else if (button_number==(var)28)
		{
			*material_power = minv( 10 , ++*material_power );
		}
	// albedo
	else if (button_number==(var)29)
		{
			*material_albedo = maxv( -100 , --*material_albedo );
		}
	else if (button_number==(var)30)
		{
			*material_albedo = minv( 200 , ++*material_albedo );
		}
}


void	Material_ChangeColor(PANEL* panel)
{	
	while (mouse_left)
		{
			wait(1);
		}
		
	// not important to be set by clinking	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Material_Reset()
{	
	wait(3);
	if (material_select == (var)1)							// terrain
		{			
			mat_terrain.ambient_red 	= 50;
			mat_terrain.ambient_green 	= 50;
			mat_terrain.ambient_blue 	= 50;
			
			mat_terrain.diffuse_red 	= 200;
			mat_terrain.diffuse_green 	= 200;
			mat_terrain.diffuse_blue 	= 200;
			
			mat_terrain.emissive_red 	= 0;
			mat_terrain.emissive_green = 0;
			mat_terrain.emissive_blue 	= 0;
			
			mat_terrain.specular_red 	= 0;
			mat_terrain.specular_green = 0;
			mat_terrain.specular_blue 	= 0;
			
			mat_terrain.power 			= 5;
			mat_terrain.albedo 			= 100;
		}
	else if (material_select == (var)2)						// sprite
		{			
			mat_sprite.ambient_red 		= 50;
			mat_sprite.ambient_green 	= 50;
			mat_sprite.ambient_blue 	= 50;
			
			mat_sprite.diffuse_red 		= 225;
			mat_sprite.diffuse_green 	= 225;
			mat_sprite.diffuse_blue 	= 225;
			
			mat_sprite.emissive_red 	= 0;
			mat_sprite.emissive_green 	= 0;
			mat_sprite.emissive_blue 	= 0;
			
			mat_sprite.specular_red 	= 0;
			mat_sprite.specular_green 	= 0;
			mat_sprite.specular_blue 	= 0;
			
			mat_sprite.power 				= 5;
			mat_sprite.albedo 			= 100;
		}
	else if (material_select == (var)3)						// model
		{			
			mat_model.ambient_red 		= 50;
			mat_model.ambient_green 	= 50;
			mat_model.ambient_blue 		= 50;
			
			mat_model.diffuse_red 		= 225;
			mat_model.diffuse_green 	= 225;
			mat_model.diffuse_blue 		= 225;
			
			mat_model.emissive_red 		= 0;
			mat_model.emissive_green 	= 0;
			mat_model.emissive_blue 	= 0;
			
			mat_model.specular_red 		= 0;
			mat_model.specular_green 	= 0;
			mat_model.specular_blue 	= 0;
			
			mat_model.power 				= 5;
			mat_model.albedo 				= 100;
		}
	else if (material_select == (var)4)						// unlit
		{			
			mat_unlit.ambient_red 		= 0;
			mat_unlit.ambient_green 	= 0;
			mat_unlit.ambient_blue 		= 0;
			
			mat_unlit.diffuse_red 		= 0;
			mat_unlit.diffuse_green 	= 0;
			mat_unlit.diffuse_blue 		= 0;
			
			mat_unlit.emissive_red 		= 128;
			mat_unlit.emissive_green 	= 128;
			mat_unlit.emissive_blue 	= 128;
			
			mat_unlit.specular_red 		= 0;
			mat_unlit.specular_green 	= 0;
			mat_unlit.specular_blue 	= 0;
			
			mat_unlit.power 				= 2;
			mat_unlit.albedo 				= 0;
		}
	else if (material_select == (var)5)						// shadow
		{			
			mat_shadow.ambient_red 		= 0;
			mat_shadow.ambient_green 	= 0;
			mat_shadow.ambient_blue 	= 0;
			
			mat_shadow.diffuse_red 		= 0;
			mat_shadow.diffuse_green 	= 0;
			mat_shadow.diffuse_blue 	= 0;
			
			mat_shadow.emissive_red 	= 0;
			mat_shadow.emissive_green 	= 0;
			mat_shadow.emissive_blue 	= 0;
			
			mat_shadow.specular_red 	= 0;
			mat_shadow.specular_green 	= 0;
			mat_shadow.specular_blue 	= 0;
			
			mat_shadow.power 				= 0;
			mat_shadow.albedo 			= 0;
		}
	else if (material_select == (var)6)						// flat
		{			
			mat_flat.ambient_red 		= 50;
			mat_flat.ambient_green 		= 50;
			mat_flat.ambient_blue 		= 50;
			
			mat_flat.diffuse_red 		= 225;
			mat_flat.diffuse_green 		= 225;
			mat_flat.diffuse_blue 		= 225;
			
			mat_flat.emissive_red 		= 0;
			mat_flat.emissive_green 	= 0;
			mat_flat.emissive_blue 		= 0;
			
			mat_flat.specular_red 		= 0;
			mat_flat.specular_green 	= 0;
			mat_flat.specular_blue 		= 0;
			
			mat_flat.power 				= 5;
			mat_flat.albedo 				= 100;
		}
	else if (material_select == (var)7)						// shaded
		{			
			mat_shaded.ambient_red 		= 50;
			mat_shaded.ambient_green 	= 50;
			mat_shaded.ambient_blue 	= 50;
			
			mat_shaded.diffuse_red 		= 225;
			mat_shaded.diffuse_green 	= 225;
			mat_shaded.diffuse_blue 	= 225;
			
			mat_shaded.emissive_red 	= 0;
			mat_shaded.emissive_green 	= 0;
			mat_shaded.emissive_blue 	= 0;
			
			mat_shaded.specular_red 	= 0;
			mat_shaded.specular_green 	= 0;
			mat_shaded.specular_blue 	= 0;
			
			mat_shaded.power 				= 5;
			mat_shaded.albedo 			= 100;
		}
	else if (material_select == (var)8)						// metal
		{			
			mat_metal.ambient_red 		= 50;
			mat_metal.ambient_green 	= 50;
			mat_metal.ambient_blue 		= 50;
			
			mat_metal.diffuse_red 		= 225;
			mat_metal.diffuse_green 	= 225;
			mat_metal.diffuse_blue 		= 225;
			
			mat_metal.emissive_red 		= 0;
			mat_metal.emissive_green 	= 0;
			mat_metal.emissive_blue 	= 0;
			
			mat_metal.specular_red 		= 255;
			mat_metal.specular_green 	= 255;
			mat_metal.specular_blue 	= 255;
			
			mat_metal.power 				= 10;
			mat_metal.albedo 				= 100;
		}
	else if (material_select == (var)9)						// sky
		{			
			mat_sky.ambient_red 			= 255;
			mat_sky.ambient_green 		= 255;
			mat_sky.ambient_blue 		= 255;
			
			mat_sky.diffuse_red 			= 0;
			mat_sky.diffuse_green 		= 0;
			mat_sky.diffuse_blue 		= 0;
			
			mat_sky.emissive_red 		= 0;
			mat_sky.emissive_green	 	= 0;
			mat_sky.emissive_blue 		= 0;
			
			mat_sky.specular_red 		= 0;
			mat_sky.specular_green 		= 0;
			mat_sky.specular_blue 		= 0;
			
			mat_sky.power 					= 2;
			mat_sky.albedo 				= 0;
		}
	//-------------------------------------------------------------
	else
		{
			STRING* temp_str = str_create("");							// needed for own material files		
			
			if (material_select == (var)10)								// terrain_multi_mat , terrain_single_mat
				{
					str_cpy( temp_str , hmp_foldername );		
					str_cat( temp_str , material_filename );
					Material_LoadMaterial(terrain_multi_mat,temp_str);
					Material_LoadMaterial(terrain_single_mat,temp_str);
				}
			else if (material_select == (var)11)						// water_multi_mat , water_single_mat
				{
					str_cpy( temp_str , hmpw_foldername );		
					str_cat( temp_str , material_filename );
					Material_LoadMaterial(water_multi_mat,temp_str);
					Material_LoadMaterial(water_single_mat,temp_str);
				}
			else if (material_select == (var)12)						// mtl_detail
				{
					str_cpy( temp_str , detail_foldername );		
					str_cat( temp_str , material_filename );
					Material_LoadMaterial(mtl_detail,temp_str);
				}	
			else if (material_select == (var)13)						// mtl_shrub
				{
					str_cpy( temp_str , shrub_foldername );		
					str_cat( temp_str , material_filename );
					Material_LoadMaterial(mtl_shrub,temp_str);
				}		
			else if (material_select == (var)14)						// mtl_grass
				{
					str_cpy( temp_str , grass_foldername );		
					str_cat( temp_str , material_filename );
					Material_LoadMaterial(mtl_grass,temp_str);
				}
//			else if (material_select == (var)15)						// mtl_bush
//				{
//					str_cpy( temp_str , bush_foldername );		
//					str_cat( temp_str , material_filename );
//					Material_LoadMaterial(mtl_bush,temp_str);
//				}	
			else if (material_select == (var)15)						// mtl_obstacle
				{
					str_cpy( temp_str , obstacle_foldername );		
					str_cat( temp_str , material_filename );
					Material_LoadMaterial(mtl_obstacle,temp_str);
//					Material_LoadMaterial(mtl_obstacle_transp,temp_str);
				}	
			else if (material_select == (var)16)						// mtl_mapent
				{
					str_cpy( temp_str , mapent_foldername );		
					str_cat( temp_str , material_filename );
					Material_LoadMaterial(mtl_mapent,temp_str);
				}
			else if (material_select == (var)17)						// mtl_terrmdl
				{
					str_cpy( temp_str , terrmdl_foldername );		
					str_cat( temp_str , material_filename );
					Material_LoadMaterial(mtl_terrmdl,temp_str);
				}
			else if (material_select == (var)18)						// mtl_tree
				{
					str_cpy( temp_str , tree_foldername );		
					str_cat( temp_str , material_filename );
					Material_LoadMaterial(mtl_tree,temp_str);
				}
			else if (material_select == (var)19)						// mtl_building
				{
					str_cpy( temp_str , building_foldername );		
					str_cat( temp_str , material_filename );
					Material_LoadMaterial(mtl_building,temp_str);
				}
//			else if (material_select == (var)20)						// mtl_unit
//				{
//					str_cpy( temp_str , unit_foldername );		
//					str_cat( temp_str , material_filename );
//					Material_LoadMaterial(mtl_unit,temp_str);
//				}
			
			ptr_remove(temp_str);
		}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Material_LoadMaterial(MATERIAL* mtl_to_load , STRING* mtl_filename)
{
	//--------------------------------------------------------
	var file_handle = file_open_read( mtl_filename );
	//--------------------------------------------------------
 	// read file if exists
	if (file_handle)
		{
			mtl_to_load.ambient_red 		= file_var_read(file_handle);
			mtl_to_load.ambient_green 		= file_var_read(file_handle);
			mtl_to_load.ambient_blue 		= file_var_read(file_handle);
			
			mtl_to_load.diffuse_red 		= file_var_read(file_handle);
			mtl_to_load.diffuse_green 		= file_var_read(file_handle);
			mtl_to_load.diffuse_blue 		= file_var_read(file_handle);
			
			mtl_to_load.specular_red 		= file_var_read(file_handle);
			mtl_to_load.specular_green 	= file_var_read(file_handle);
			mtl_to_load.specular_blue 		= file_var_read(file_handle);
			
			mtl_to_load.emissive_red 		= file_var_read(file_handle);
			mtl_to_load.emissive_green 	= file_var_read(file_handle);
			mtl_to_load.emissive_blue 		= file_var_read(file_handle);
			
			mtl_to_load.power 				= file_var_read(file_handle);
			mtl_to_load.albedo 				= file_var_read(file_handle);
			//--------------------------------------------------------
			file_close(file_handle);
		}		
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void		Material_Effect_Clear()
{
	wait(3);
	effect_load( material_actual, NULL);
	wait(3);
	if (material_actual.effect != NULL)
		{
			pan_setstring(material_matpanel, 23,	40,40,	panel_font,			str_create( material_actual.effect ) );
		}
	else
		{
			pan_setstring(material_matpanel, 23,	40,40,	panel_font,			str_create( "none" ) );
		}
}


void		Material_Effect_Load()
{
	wait(3);
	
	// call filehandlingclass
	FileMenu_LoadMaterialEffect();
}


void		Material_Load_Effect_fx(char* file_name)
{
	wait(3);
	
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	
	effect_load(material_actual, NULL);
	wait(3);
	
	effect_load(material_actual, temp_str);
	wait(3);
	
	if (material_actual.effect != NULL)
		{
			pan_setstring(material_matpanel, 23,	40,40,	panel_font,			str_create( material_actual.effect ) );
		}
	else
		{
			pan_setstring(material_matpanel, 23,	40,40,	panel_font,			str_create( "none" ) );
		}
		
	ptr_remove(temp_str);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#endif