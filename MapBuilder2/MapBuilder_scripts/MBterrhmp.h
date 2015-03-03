
#ifndef MBterrhmp_h
#define MBterrhmp_h

/////////////////////////////////////////////////////////////////////////
// defines



//////////////////////////////////////////////////////////////////////////
// variables

//------------------------------------
// basic data

VECTOR* 	terrhmp_vgroundmax		= { x = 5000; y = 5000; z =  1000; }  
VECTOR* 	terrhmp_vgroundmin		= { x =-5000; y =-5000; z = -1000; }  

ENTITY*  terrain_entity 			= NULL;

//------------------------------------
// auto texturing parameters

var basecolor_tiling			= 1.0;					// tiling of base colormap

var grass_height_border 	= 500.0;					// WorldPosition.y value
var grass_height_width 		= 200.0;					// height transition width

var shore_height_width 		= 20.0;					// -WorldPosition.y (start of underwater) and WorldPosition.y (start of grass1) values as borders

var cliff_slope_min 			= 67.0;					// 0..90 min slope in degrees
var cliff_slope_width		= 4.0;					// 1..15 degree of transiency

//var caustics_height_mid	= -256.0;				// WorldPosition.y value
//var caustics_width			= 128.0;					// WorldPosition.y 3/- borders measured from mid

//------------------------------------
// vertex deformation

typedef struct 
{
	var 			original;								// when starting editing
//	var 			actual;									// shows always the actual vertex z values
	var			undo[10];								// 10 other vertex arrays to undo modifications
} VERTEXSYS;												// 48 bytes : max 500x500= 12.000.000 B (11.7MB)

VERTEXSYS* teredit_vertexarray = NULL;				// fix size sys_malloc/sys_free

/////////////////////////////////////////////////////////////////////////
// functions

//-----------------------------------------------------------------
// create

void		TerrHmp_CreateGround(char* TerrainFileName);
void		TerrHmp_RemoveGround();

action 	TerrHmp_GroundAction();														// set terrain related variables and terrain material depending on shadow_stencil mode

//-----------------------------------------------------------------
// load/save

// terrain vertices
void		TerrHmp_Save_Vertices_trv(char* file_name);
void		TerrHmp_Load_Vertices_trv(char* file_name);										

// lightmap
void		TerrHmp_Save_LightMap_tga(char* file_name);
void		TerrHmp_Load_LightMap_tga(char* file_name);										

// multitexturing textures
void		TerrHmp_Save_Skins_tga(char* file_name);										// saves a set of terrain skins
void		TerrHmp_Load_Skins_tga(char* file_name);										// loads a set of terrain skins

	void		TerrHmp_Tx_StoreSkins(char* file_name, var mode);						// saves terrain skins as .dds or .tga into the desired subfolder
	void		TerrHmp_Tx_DeleteSkinFiles(char* file_name, var mode);				// deletes terrain skins saved as .dds or .tga into the desired subfolder (good for removing temporary files)
	
	void		TerrHmp_Tx_LoadSkin(STRING* temp_str, var skin_num);					// loads stored bitmap by bmap_load and copies it into skin pixel by pixel - required because of wrong bmap_create tga handling - if bmap_save used it is flipped, and works only with bmap_load
	void		TerrHmp_Tx_LoadSkin_KeepAlpha(STRING* temp_str, var skin_num);		// same as above without touching texture alpha pixels
		void		TerrHmp_Tx_UpdateMaterialSkins();										// get material skins 1-4 from actual entity skins 5-8 - for terrain shaders using material skins got from entitiy skins

// single texture + detail texture
void		TerrHmp_Save_Skin24_tga(char* file_name);
void		TerrHmp_Load_Skin24_tga(char* file_name);

void		TerrHmp_Save_DetailMap24_dds(char* file_name);
void		TerrHmp_Load_DetailMap24_dds(char* file_name);						

// autotexturing parameters
	
	void	TerrHmp_Save_AutoTex_sat(char* file_name);
	void	TerrHmp_Load_AutoTex_sat(char* file_name);
	
//-----------------------------------------------------------------
// gets

	VECTOR*	TerrHmp_GetGroundMin();
	VECTOR*	TerrHmp_GetGroundMax();
	
	function	TerrHmp_GetGroundMinX();
	function	TerrHmp_GetGroundMinY();
	function	TerrHmp_GetGroundMinZ();
	
	function	TerrHmp_GetGroundMaxX();
	function	TerrHmp_GetGroundMaxY();
	function	TerrHmp_GetGroundMaxZ();

//-----------------------------------------------------------------
// sets
	
	void	TerrHmp_SetGroundMinToDefault();
	void	TerrHmp_SetGroundMaxToDefault();

//-----------------------------------------------------------------
// vertex normals

	void		TerrHmp_FixNormals();																			// to get proper gouraud shading, ctrace normals for all vertices - requires a wait before, if vertices were modified previously!
	void		TerrHmp_FixNormals_Area(var upleft_vert, var width_vert, var height_vert);		// same as above but for a distinct area of vertices

/////////////////////////////////////////////////////////////////////////


#endif