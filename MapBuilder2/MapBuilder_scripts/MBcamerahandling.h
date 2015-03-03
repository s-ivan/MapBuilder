
#ifndef MBcamerahandling_h
#define MBcamerahandling_h

///////////////////////////////////////////////////////////////////////
// defines

#define CAMZ		2000			// cam.obj. initial zoom
#define CAMH		64				// cam.obj. initial height
#define CAMA		270			// camera initial angle

#define CAMZDWN	128			// camera limit
#define CAMZUP		3072			// camera limit
#define CAMMSP		32				// camera move speed

///////////////////////////////////////////////////////////////////////
// variables

//---------------------------------------------------------
// RTS camera object

var camera_zoomspeed 		= 2;
var camera_rotatespeed 		= 8;
var camera_movespeed 		= 32;

var camera_zoomdownlimit 	= 64;
var camera_zoomuplimit 		= 3072;

var camera_object_height;												// height of camera object - should be constant but over surface, or constantly over surface
var camera_object_minz		= 32;
var camera_object_maxz		= 1024;

var camera_object_maxx 		= 0;										// set later based on actual map size
var camera_object_minx 		= 0;										// set later based on actual map size
var camera_object_maxy 		= 0;										// set later based on actual map size
var camera_object_miny 		= 0;										// set later based on actual map size

var camera_height;														// camera height 
var camera_angle;			 												// rotation of camera around camera object: 270 = watch from South i.e. watch to North

//---------------------------------------------------------
// active tile square object

var square_coord_x 		= 0;
var square_coord_y 		= 0;
var square_coord_z 		= 0;											// square entity coordinates
var square_x 				= 0;
var square_y 				= 0;											// square entity tile positions
var square_z 				= 0;															
var square_value0			= 0;											// local value of Tiles
var square_value1			= 0;
var square_value2			= 0;
var square_abs				= 0;											// square entity tilearray position

var square_min				= 0;
var square_mid0			= 0;
var square_mid1			= 0;
var square_mid2			= 0;
var square_max				= 0;

var square_clear0 		= 0;
var square_clear1 		= 0;
var square_clear2 		= 0;
var square_clear3 		= 0;
var square_clearmask0 	= 0;
var square_clearmask1 	= 0;

var square_cost0			= 0;
var square_cost1			= 0;
var square_cost2			= 0;
var square_clus0			= 0;
var square_clus1			= 0;
var square_buil			= 0;

var square_outofborder 	= 0;

//---------------------------------------------------------
// path- and camera_tilesquare sprite/model height offset values

var camera_roadlevel 		= 0.2;
var camera_pathlevel 		= 0.5;
var camera_squarelevel	 	= 1;

//---------------------------------------------------------
STRING* camera_object_str = "graphics\\camera_cube.mdl";  	// any simple model

STRING* camera_square1_str = "graphics\\tile1.mdl";      	// green - build square
STRING* camera_square2_str = "graphics\\tile2.mdl";			// blue - position square 
STRING* camera_square3_str = "graphics\\tile3.mdl";			// red - wait square 

//---------------------------------------------------------
ENTITY* camera_cameraobject	= NULL;								// followed by the camera
ENTITY* camera_tilesquare		= NULL;								// the square entity on terrain surface showing the tile where mouse is pointing at

////////////////////////////////////////////////////////////////////////////////////
// functions


void		Camera_SetDefaultPos();										// temp camera pos during level loading

// initialization
void		Camera_Init();
	void		Camera_Close();
	void		Camera_ChangeMode();
	
// simple camnera
	void		Camera_Init_Simple();
		void		Camera_Simple_Close();
	
// RTS camera object
	void		Camera_Init_RTS();
		action 	Camera_Object_RTS();   
			void		Camera_Object_TestLimits();
		void		Camera_Object_Close();

// camera_tilesquare
	void		Camera_Square_Init();
		action 	Camera_Square();		
		
			void		Camera_SnapIt();
		
			void		Camera_ShowPosition();	// does important updates and unit conversions
		void	Camera_Square_Close();

// mode changes
//void		ChangeMode();


/////////////////////////////////////////////////////////////////////////////



#endif