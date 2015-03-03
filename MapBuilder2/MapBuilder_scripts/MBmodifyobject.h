
#ifndef MBmodifyobject_h
#define MBmodifyobject_h

// parameters to be modified
var	*mod_x;
var	*mod_y;
var	*mod_z;

var	*mod_pan;
var	*mod_tilt;
var	*mod_roll;

var	mod_light;			
var	*mod_r;
var	*mod_g;
var	*mod_b;

var	mod_unlit;			
var	mod_matmodel;		
var	mod_mtltree;		

var	mod_transp;										// error in wmb - they become transparent on selection !
var	mod_overl;										// error in wmb - they become transparent on selection !

var	mod_lightmap;									// 1 lightmap, 0 dynamic shadows

var	*mod_alpha;										// error in wmb - they become transparent on selection !
var	*mod_ambient;

var	mod_amball		= 0;							// do changes for all similar entity instances on pressing OK
var	mod_scaleall	= 0;							// ambient, scale, LIGHT+rgb, lightmap/shadow
var	mod_rgball		= 0;
var	mod_lmall		= 0;

var	*mod_scale_x;
var	*mod_scale_y;
var	*mod_scale_z;

var	mod_ent_cat;									// entity category - also defined by mod_origmenu
var	mod_handle;										// handle to pointer to the actual entity

// needed by menu
var	mod_origmenu;									// defines entity category to modify
var	mod_selection;									// bool - 0 nothing, 1 something selected

var	mod_type = 0;									// 0 move, 1 rotate, 2 scale

// needed by fine setting buttons
var	mod_minoffset_x;								// x y z
var	mod_maxoffset_x;
var	mod_minoffset_y;
var	mod_maxoffset_y;
var	mod_minoffset_z;
var	mod_maxoffset_z;
var	mod_minoffset_ptr;							// pan tilt roll
var	mod_maxoffset_ptr;
var	mod_minoffset_sc;								// scale
var	mod_maxoffset_sc;

var	mod_snapxy	= 0;								// use xy and/or z snapping on move
var	mod_snapz	= 0;

VECTOR* mod_orig_bbmin = { x=0; y=0; z=0; }
VECTOR* mod_orig_bbmax = { x=0; y=0; z=0; }
VECTOR* mod_orig_scale = { x=0; y=0; z=0; }

MATERIAL*	mod_origmaterial = NULL;			// *** store original to check changes to make it for lod models and other instances to avoid problems

// the modified model
ENTITY*	mod_modified_ent = NULL;

// gizmo models
ENTITY*	mod_gizmo_x_ent = NULL;
ENTITY*	mod_gizmo_y_ent = NULL;
ENTITY*	mod_gizmo_z_ent = NULL;

// control panel
PANEL*	mod_lowpanel;
PANEL*	mod_panel;

////////////////////////////////////////////////
// functions

void		ModifyObject_CloseOK();										// sets a temp UIMODE to end loop in ModifyObject_Select and to start ModifyObject_CloseMod
	void		ModifyObject_CloseMod();								// modifies entity and similar entity properties as needed
void		ModifyObject_ClearAll();									// remove all similar entities including the selected one, close menu
	void		ModifyObject_RemoveEntities(STRING* filename);	// does the erasing job, including ent_decal shadows and collider entities
		void		ModifyObject_ResetValues();						// reset mod parameters to default, called by both ModifyObject_CloseMod and ModifyObject_ClearAll
		
void		ModifyObject_Select(ENTITY* ent);
	
	ENTITY*	ModifyObject_SelectByTile();							// select by area - non-collision entities (grass)
	ENTITY*	ModifyObject_SelectByClick();							// select by c-trace or mouse_ent - entities with collision (obstacle, mapent, terrmdl) - but ignoring collider entities
	ENTITY*	ModifyObject_SelectByMixed();							// not used, not finished !
	
	void	ModifyObject_SetSelected();								// object initialization
	void		ModifyObject_BBox(ENTITY* ent);						// draws bounding box of selected entity 
	
	void		ModifyObject_Panel_Init2();							// new panel over initial one		
		
		void		ModifyObject_NextEntity();
		
		void		ModifyObject_Button(button_number);
		
		void		ModifyObject_ToggleLight();
		void		ModifyObject_ToggleUnlit();
		void		ModifyObject_ToggleModel();
		void		ModifyObject_ToggleTree();
		void		ModifyObject_ToggleTransp();
		void		ModifyObject_ToggleOverl();
		void		ModifyObject_ToggleLightm();
			void		ModifyObject_UpdateShadows(ENTITY* ent, var modified_lightmap);		// lightmap parameter enables outer calls
		
		void		ModifyObject_ToggleSnapXY();
		void		ModifyObject_ToggleSnapZ();
		
		void		ModifyObject_ToggleAmbAll();
		void		ModifyObject_ToggleScaleAll();
		void		ModifyObject_ToggleRGBAll();
		void		ModifyObject_ToggleLmAll();
			void		ModifyObject_UpdateShadows_ForAll(ENTITY* temp_ent);
		
			function		ModifyObject_ReturnShadow(ENTITY* ent);
			void		ModifyObject_UpdateButtons();
			void		ModifyObject_Update_Scale(ENTITY* temp_ent);
			void		ModifyObject_Update_SnapXYZ(ENTITY* ent);										// do snapping if options checked
			void		ModifyObject_Update_TilePos(ENTITY* ent);										// update tile positions (x,y,abs) stored in skills
			void		ModifyObject_Update_Collider(ENTITY* ent);
			void		ModifyObject_Update_Info(ENTITY* ent);
			
		void		ModifyObject_SetMaterialForAll(ENTITY* ent);
			void		ModifyObject_ChangeMaterial(ENTITY* ent, ENTITY* temp_ent);
		void		ModifyObject_SetAmbientForAll(ENTITY* ent);
		void		ModifyObject_SetScaleForAll(ENTITY* ent);
		void		ModifyObject_SetRGBForAll(ENTITY* ent);
		void		ModifyObject_SetLightmapForAll(ENTITY* ent);
		
		void		ModifyObject_ToggleMove();
		void		ModifyObject_ToggleRotate();
		void		ModifyObject_ToggleScale();				
		
	void		ModifyObject_Click();										// detects and starts gizmo based entity manipulations
		action	ModifyObject_Gizmo();									// gizmo settings		
		void		ModifyObject_Gizmo_Move(ENTITY* ent);				// move entity by gizmo			
			void		ModifyObject_Update_PanelXYZ();					// refresh panel values after entity manipulation done by the help of gizmo
		void		ModifyObject_Gizmo_Rotate(ENTITY* ent);			// rotate entity by gizmo
			void		ModifyObject_Update_PanelPTR();					// refresh panel values after entity manipulation done by the help of gizmo
		void		ModifyObject_Gizmo_Scale(ENTITY* ent);				// move entity by gizmo			
			void		ModifyObject_Update_PanelSSS();					// refresh panel values after entity manipulation done by the help of gizmo
			
///////////////////////////////////////////////////
// includes



#endif