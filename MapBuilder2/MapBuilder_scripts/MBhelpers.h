#ifndef MBhelpers_h
#define MBhelpers_h


///////////////////////////////////////////
// functions

//----------------------
//----------------------
// ray-tracing

function PosInfo3D0(var tempx, var tempy);								// detect evrything

function PosInfo3D1(var tempx, var tempy);								// detect terrain (and blocks)											- object placement onto terrain (for terrainmdl)
																						// ignore:
																						// 1:(water-terrain + camera-objects + collider-entities),
																						// 2:(terrain models),
																						// 3:(obstacles + mapents + buildings),
																						// 30:(gizmos + RTS-units)
																						// sprites, models, mapents, passable
																						
function PosInfo3D2(var tempx, var tempy);								// detect terrain and walkable object surface (and blocks)		- object placement onto terrainhmp, terrainmdl, buildingmdl, mapent
																						// ignore:
																						// 1:(water-terrain + camera-objects + collider-entities),
																						// 30:(gizmos + RTS-units)
																						// sprites, flag2, passable
																						
function PosInfo3D3(var tempx, var tempy);								// detect water and ground terrain (and blocks)						- camera movement over ground and water terrain
																						// ignore:
																						// 2:(terrain models),
																						// 3:(obstacles + mapents + buildings),
																						// 30:(gizmos + RTS-units)
																						// sprites, models															(models needed for excluding collider models)
																						
function PosInfo3D4(var tempx, var tempy);								// detect terrain and terrain models (and blocks)					- object placement onto terrainhmp, terrainmdl (for any object)
																						// ignore:
																						// 1:(water-terrain + camera-objects + collider-entities),
																						// 3:(obstacles + mapents + buildings),
																						// 30:(gizmos + RTS-units)
																						// sprites, flag2, passable
																						
																						// (blocks can be excluded by IGNORE_WORLD but it would ignore terrains too - or add a group value to level_enbt and add it to c_ignore?)
//----------------------

function PosInfo3D1OrZeroLevel(var tempx, var tempy);					// PosInfo3D1 or PosInfoOnZeroLevel
function PosInfo3D2OrZeroLevel(var tempx, var tempy);					// PosInfo3D2 or PosInfoOnZeroLevel
function PosInfo3D3OrZeroLevel(var tempx, var tempy);					// PosInfo3D3 or PosInfoOnZeroLevel
function PosInfo3D4OrZeroLevel(var tempx, var tempy);					// PosInfo3D4 or PosInfoOnZeroLevel

//----------------------

VECTOR*	MousePos3D0();															// get mouse position on everything 									- (for object deletion)
VECTOR*	MousePos3D1();															// get mouse position on terrain 										- (camera tile-square in editor mode)
VECTOR*	MousePos3D2();															// get mouse position on walkable surface 							- ignore water terrain, obstacles (camera tile-square in rts game mode)
VECTOR*	MousePos3D3();															// get mouse position on water and ground terrain 					- ignore terrain models, obstacles+mapents, units (for camera movement - not used)
VECTOR*	MousePos3D4();															// get mouse position on terrain and terrain models surface 	- ignore obstacles+mapents, units

//----------------------					

VECTOR*	MousePos3D0OrZeroLevel();											// MousePos3D0 or MousePosOnZeroLevel
VECTOR*	MousePos3D1OrZeroLevel();											// MousePos3D1 or MousePosOnZeroLevel
VECTOR*	MousePos3D2OrZeroLevel();											// MousePos3D2 or MousePosOnZeroLevel
VECTOR*	MousePos3D3OrZeroLevel();											// MousePos3D3 or MousePosOnZeroLevel
VECTOR*	MousePos3D4OrZeroLevel();											// MousePos3D4 or MousePosOnZeroLevel

//----------------------

function		PosInfoOnZeroLevel(var tempx, var tempy);					// when no terrain: get z pos in zero level, which is always zero, but additionally sets normal vector to point upwards
VECTOR*		MousePosOnZeroLevel();											// when no terrain: get camera view line - zero plane intersection or 0,0,0

	function Line_Plane_Intersection(VECTOR* RESULT, VECTOR* line_p0, VECTOR* line_p1, VECTOR* plane_p0, VECTOR* plane_N);		// returns line-plane intersection point

//----------------------

ENTITY*	MouseInfo3D(VECTOR* _vector);										// mouse trace for entity

//----------------------
//----------------------
// file

function file_exists_onepath(STRING* temp_str);
function file_remove_onepath(STRING* temp_str);

//----------------------

void 		File_Write_Asc(var file_handle, STRING* temp_str);
STRING* 	File_Read_Asc(var file_handle, STRING* temp_str);

//----------------------
//----------------------
// entity

function	AlphaMaterialTest(ENTITY* ent);
function	AlphaMaterialNumber(ENTITY* ent);

//----------------------

VECTOR*  vec_for_ent_ext(VECTOR* _target, ENTITY* _ent, int _vertexNumber);

//----------------------

void 		Update_BBox(ENTITY* ent);

//----------------------

void		Remove_MapEntities();						// not used - all entities - auto done on loading a new level, which is much faster
	function	Remove_MapEntity();

void		Remove_EntCats(int category);				// one category (was used only on game test mode closing, now compatible with every categories)
	int	Remove_EntCat(int category);

//----------------------
//----------------------

#endif