
#ifndef MBmaterialeditor_h
#define MBmaterialeditor_h

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// variables

// control panel
PANEL*	material_lowpanel;
PANEL*	material_matpanel;

PANEL*	material_colpan1;		// ambient
PANEL*	material_colpan2;		// diffuse
PANEL*	material_colpan3;		// emissive
PANEL*	material_colpan4;		// specular

BMAP*		material_color1_bmp			= "panels\\color2.bmp";
STRING* 	material_color1 				= "bmap = material_color1_bmp;";
BMAP*		material_color2_bmp			= "panels\\color2.bmp";
STRING* 	material_color2 				= "bmap = material_color2_bmp;";
BMAP*		material_color3_bmp			= "panels\\color2.bmp";
STRING* 	material_color3 				= "bmap = material_color3_bmp;";
BMAP*		material_color4_bmp			= "panels\\color2.bmp";
STRING* 	material_color4 				= "bmap = material_color4_bmp;";

var		material_select;
STRING*	material_filename	= "_material.mtl";

// var*-s
var*	material_amb_red;
var*	material_amb_green;
var*	material_amb_blue;
var*	material_dif_red;
var*	material_dif_green;
var*	material_dif_blue;
var*	material_emi_red;
var*	material_emi_green;
var*	material_emi_blue;
var*	material_spe_red;
var*	material_spe_green;
var*	material_spe_blue;
var*	material_power;
var*	material_albedo;

MATERIAL*	material_actual;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions

//----------------------------------------------------------------
// main menu

void 	Material_Init();
void 	Material_Close();
	
	void	Material_LowPanel_Init();
	void	Material_LowPanel_Select(button_number);

//----------------------------------------------------------------
// material editor of the selected material

	void	Material_MatPanel_Init(MATERIAL* material_primary, MATERIAL* material_secondary);
	void 	Material_MatPanel_Close();
	void		Material_MatPanel_Button(button_number);
	void		Material_ChangeColor(PANEL* panel);
	void		Material_Reset();
	
		void		Material_LoadMaterial(MATERIAL* mtl_to_load , STRING* mtl_filename);				// load default settings from a text file in entity category subfolder
		void		Material_Effect_Clear();
		void		Material_Effect_Load();
			void		Material_Load_Effect_fx(char* file_name);

//----------------------------------------------------------------


#endif