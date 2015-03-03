#ifndef MBhelpers_c
#define MBhelpers_c


///////////////////////////////////////////////////////////////////////////////////////////
// terrain level detection


function PosInfo3D0(var tempx, var tempy)
{
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(tempx, tempy, TerrHmp_GetGroundMaxZ() + GetMapTileSize()) );       		
	vec_set( pos2 , vector(tempx, tempy, TerrHmp_GetGroundMinZ() - GetMapTileSize()) );
	
	c_trace (pos1, pos2, IGNORE_CONTENT); 
	
	if (HIT_TARGET) 
		{
			return(hit.z);							// measured Z
		}
	else
		{
			return((var)NO_Z_APPLIED);			// default Z
		}
}


function PosInfo3D1(var tempx, var tempy)
{
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(tempx, tempy, TerrHmp_GetGroundMaxZ() + GetMapTileSize()) );       		
	vec_set( pos2 , vector(tempx, tempy, TerrHmp_GetGroundMinZ() - GetMapTileSize()) );
	
	// ignore 1:(water-terrain + camera-objects + collider-entities), 2:(terrain models), 3:(obstacles + mapents + buildings), 30:(gizmos + RTS-units)
	c_ignore(1,2,3,30,0);
	c_trace (pos1, pos2, IGNORE_SPRITES | IGNORE_MODELS | IGNORE_MAPS | IGNORE_PASSABLE | IGNORE_CONTENT); 
	
	if (HIT_TARGET) 
		{
			return(hit.z);							// measured Z
		}
	else
		{
			return((var)NO_Z_APPLIED);			// default Z
		}
}


function PosInfo3D2(var tempx, var tempy)
{
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(tempx, tempy, TerrHmp_GetGroundMaxZ() + GetMapTileSize()) );        		
	vec_set( pos2 , vector(tempx, tempy, TerrHmp_GetGroundMinZ() - GetMapTileSize()) );
	
	// ignore 1:(water-terrain + camera-objects + collider-entities), 30:(gizmos + RTS-units)
	c_ignore(1,30,0);
	c_trace (pos1, pos2, IGNORE_SPRITES | IGNORE_FLAG2 | IGNORE_PASSABLE | IGNORE_CONTENT);    
	
	if (HIT_TARGET) 
		{
			return(hit.z);							// measured Z
		}
	else
		{
			return((var)NO_Z_APPLIED);			// default Z
		}
}


function PosInfo3D3(var tempx, var tempy)
{
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(tempx, tempy, TerrHmp_GetGroundMaxZ() + GetMapTileSize()) );          		
	vec_set( pos2 , vector(tempx, tempy, TerrHmp_GetGroundMinZ() - GetMapTileSize()) );
	
	// ignore 2:(terrain models), 3:(obstacles + mapents + buildings), 30:(gizmos + RTS-units)
	c_ignore(2,3,30,0);
	c_trace (pos1, pos2, IGNORE_SPRITES | IGNORE_MODELS | IGNORE_CONTENT);  	
	
	if (HIT_TARGET) 
		{
			return(hit.z);							// measured Z
		}
	else
		{
			return((var)NO_Z_APPLIED);			// default Z
		}
}


function PosInfo3D4(var tempx, var tempy)
{
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(tempx, tempy, TerrHmp_GetGroundMaxZ() + GetMapTileSize()) );         		
	vec_set( pos2 , vector(tempx, tempy, TerrHmp_GetGroundMinZ() - GetMapTileSize()) );
	
	// ignore 1:(water-terrain + camera-objects + collider-entities), 3:(obstacles + mapents + buildings), 30:(gizmos + RTS-units)
	c_ignore(1,3,30,0);
	c_trace (pos1, pos2, IGNORE_SPRITES | IGNORE_FLAG2 | IGNORE_PASSABLE | IGNORE_CONTENT);   
	
	if (HIT_TARGET) 
		{
			return(hit.z);							// measured Z
		}
	else
		{
			return((var)NO_Z_APPLIED);			// default Z
		}
}


///////////////////////////////////////////////////////////////////////////////////////////


function PosInfo3D1OrZeroLevel(var tempx, var tempy)
{
	var temp = PosInfo3D1(tempx, tempy);
	
	if (temp != (var)NO_Z_APPLIED)
		{
			return temp;
		}
	else
		{
			return PosInfoOnZeroLevel(tempx, tempy);
		}
}


function PosInfo3D2OrZeroLevel(tempx, tempy)
{
	var temp = PosInfo3D2(tempx, tempy);
	
	if (temp != (var)NO_Z_APPLIED)
		{
			return temp;
		}
	else
		{
			return PosInfoOnZeroLevel(tempx, tempy);
		}
}


function PosInfo3D3OrZeroLevel(tempx, tempy)
{
	var temp = PosInfo3D3(tempx, tempy);
	
	if (temp != (var)NO_Z_APPLIED)
		{
			return temp;
		}
	else
		{
			return PosInfoOnZeroLevel(tempx, tempy);
		}
}


function PosInfo3D4OrZeroLevel(tempx, tempy)
{
	var temp = PosInfo3D4(tempx, tempy);
	
	if (temp != (var)NO_Z_APPLIED)
		{
			return temp;
		}
	else
		{
			return PosInfoOnZeroLevel(tempx, tempy);
		}
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


VECTOR*		MousePos3D0()
{
sys_marker("mp0");	
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,15000) );     		
	vec_for_screen (pos2, camera);
	
	// detect everything
	c_trace (pos1, pos2, IGNORE_CONTENT);  
	
	if (HIT_TARGET)
		{
			vec_set(pos1, hit.x);
			return &pos1;
		}
	else
		{
			return NULL;
		} 
}


VECTOR*	MousePos3D1()
{
sys_marker("mp1");
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,15000) );     		
	vec_for_screen (pos2, camera);
	
	// ignore 1:(water-terrain + camera-objects + collider-entities), 2:(terrain models), 3:(obstacles + mapents + buildings), 30:(gizmos + RTS-units)
	c_ignore(1,2,3,30,0);
	c_trace (pos1, pos2, IGNORE_SPRITES | IGNORE_MODELS | IGNORE_MAPS | IGNORE_PASSABLE | IGNORE_CONTENT); 
	
	if (HIT_TARGET)
		{
			vec_set(pos1, hit.x);
			return &pos1;
		}
	else
		{
			return NULL;
		}
}


VECTOR*		MousePos3D2()
{
sys_marker("mp2");
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,15000) );     		
	vec_for_screen (pos2, camera);
	
	// ignore 1:(water-terrain + camera-objects + collider-entities), 30:(gizmos + RTS-units)
	c_ignore(1,30,0);
	c_trace (pos1, pos2, IGNORE_SPRITES | IGNORE_FLAG2 | IGNORE_PASSABLE | IGNORE_CONTENT);    
	
	if (HIT_TARGET)
		{
			vec_set(pos1, hit.x);
			return &pos1;
		}
	else
		{
			return NULL;
		} 
}


VECTOR*		MousePos3D3()
{
sys_marker("mp3");	
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,15000) );     		
	vec_for_screen (pos2, camera);
	
	// ignore 2:(terrain models), 3:(obstacles + mapents + buildings), 30:(gizmos + RTS-units)
	c_ignore(2,3,30,0);
	c_trace (pos1, pos2, IGNORE_SPRITES | IGNORE_MODELS | IGNORE_CONTENT);  	
	
	if (HIT_TARGET)
		{
			vec_set(pos1, hit.x);
			return &pos1;
		}
	else
		{
			return NULL;
		} 
}


VECTOR*		MousePos3D4()
{
sys_marker("mp4");	
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,15000) );     		
	vec_for_screen (pos2, camera);
	
	// ignore 1:(water-terrain + camera-objects + collider-entities), 3:(obstacles + mapents + buildings), 30:(gizmos + RTS-units)
	c_ignore(1,3,30,0);
	c_trace (pos1, pos2, IGNORE_SPRITES | IGNORE_FLAG2 | IGNORE_PASSABLE | IGNORE_CONTENT);   
	
	if (HIT_TARGET)
		{
			vec_set(pos1, hit.x);
			return &pos1;
		}
	else
		{
			return NULL;
		} 
}


///////////////////////////////////////////////////////////////////////////////////////////


VECTOR*		MousePos3D0OrZeroLevel()
{
	VECTOR* temp_vec = MousePos3D0();
	
	if (temp_vec != NULL)
		{
			return temp_vec;
		}
	else
		{
			return MousePosOnZeroLevel();
		}
}


VECTOR*		MousePos3D1OrZeroLevel()
{
	VECTOR* temp_vec = MousePos3D1();
	
	if (temp_vec != NULL)
		{
			return temp_vec;
		}
	else
		{
			return MousePosOnZeroLevel();
		}
}


VECTOR*		MousePos3D2OrZeroLevel()
{
	VECTOR* temp_vec = MousePos3D2();
	
	if (temp_vec != NULL)
		{
			return temp_vec;
		}
	else
		{
			return MousePosOnZeroLevel();
		}
}


VECTOR*		MousePos3D3OrZeroLevel()
{
	VECTOR* temp_vec = MousePos3D3();
	
	if (temp_vec != NULL)
		{
			return temp_vec;
		}
	else
		{
			return MousePosOnZeroLevel();
		}
}


VECTOR*		MousePos3D4OrZeroLevel()
{
	VECTOR* temp_vec = MousePos3D4();
	
	if (temp_vec != NULL)
		{
			return temp_vec;
		}
	else
		{
			return MousePosOnZeroLevel();
		}
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


function PosInfoOnZeroLevel(var tempx, var tempy)
{
//	VECTOR pos1, pos2;
//	
//	vec_set( pos1 , vector(tempx, tempy, TerrHmp_GetGroundMinZ() + GetMapTileSize()) );
//	vec_set( pos2 , vector(tempx, tempy, TerrHmp_GetGroundMaxZ() - GetMapTileSize()) );        		
//	
//	VECTOR position, planepos, planenormal;
//	
//	vec_zero( position );
//	vec_set( planepos , vector(0,0,0) );
//	vec_set( planenormal , vector(0,0,1) );
//	
//	Line_Plane_Intersection( &position, &pos1, &pos2, &planepos, &planenormal );
//	
//	vec_set(normal, planenormal);
//	
//	return position.z;
	
	//----------------------------
	// the simple solution
	
	vec_set(normal, vector(0,0,1));
	
	return 0;
}


VECTOR*		MousePosOnZeroLevel()
{
	VECTOR pos1, pos2;
	
	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,15000) ); // should be a big value       		
	vec_for_screen (pos2, camera);
	
	VECTOR position, planepos, planenormal;
	
	vec_zero( position );
	vec_set( planepos , vector(0,0,0) );
	vec_set( planenormal , vector(0,0,1) );
	
	Line_Plane_Intersection( &position, &pos1, &pos2, &planepos, &planenormal );
	
	return &position;
}


function Line_Plane_Intersection(VECTOR* RESULT, VECTOR* line_p0, VECTOR* line_p1, VECTOR* plane_p0, VECTOR* plane_N)
{
	if(vec_dist(line_p0,line_p1)==0||vec_dist(nullvector,plane_N)==0)
	{
		return(0);
	}
	VECTOR line_indeps;
	VECTOR line_coefs;
	vec_set(line_indeps,line_p0);
	vec_set(line_coefs,line_p1);
	vec_sub(line_coefs,line_p0);
	if((plane_N.x*line_coefs.x)+(plane_N.y*line_coefs.y)+(plane_N.z*line_coefs.z)==0)
	{
		return(0);
	}
	var plane_indep;
	VECTOR plane_coefs;
	plane_indep=-(plane_N.x*plane_p0.x)-(plane_N.y*plane_p0.y)-(plane_N.z*plane_p0.z);
	vec_set(plane_coefs,plane_N);
	vec_mul(line_indeps,plane_coefs);
	vec_mul(line_coefs,plane_coefs);
	var T;
	T=(-plane_indep-(line_indeps.x+line_indeps.y+line_indeps.z))/(line_coefs.x+line_coefs.y+line_coefs.z);
	RESULT.x=line_p0.x+T*(line_p1.x-line_p0.x);
	RESULT.y=line_p0.y+T*(line_p1.y-line_p0.y);
	RESULT.z=line_p0.z+T*(line_p1.z-line_p0.z);
	return(1);
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


ENTITY*	MouseInfo3D(VECTOR* _vector)
{
	VECTOR pos1, pos2;

	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,15000) ); // should be a big value       		
	vec_for_screen (pos2, camera);

	c_trace (pos1, pos2, IGNORE_ME | IGNORE_CONTENT | IGNORE_PASSABLE);
	
	if (HIT_TARGET)
		{
			vec_set(_vector, hit.x);	
			
			return hit.entity;			// you or hit.entity;
		}
	else
		{
			return NULL;
		}
}


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


function file_exists_onepath(STRING* temp_str)
{
	long temp_long = file_date(temp_str);		
	
	if (temp_long!=(long)0)
		{
			return 1;
		}
	else
		{
			return 0;
		}
}

function file_remove_onepath(STRING* temp_str)
{
	if (file_exists_onepath(temp_str))
		{
			remove( _chr(temp_str) );	
			return 1;
		}
	else
		{
			return 0;
		}	
}

///////////////////////////////////////////////////////////////////////////////////////////
// file read write an ascii line - or WriteLine / ReadLine

void		File_Write_Asc(var file_handle, STRING* temp_str)
{
	if (temp_str == NULL)														// needed protection
		{
			file_str_write( file_handle , "0" );							// can result in error on reading if nothing put here... 0 is used because it can be a number stored
		}
	else
		{
			file_str_write( file_handle , temp_str );
		}
	file_asc_write(	file_handle , 13); 
	file_asc_write(	file_handle , 10);
}

STRING*	File_Read_Asc(var file_handle, STRING* temp_str)			// both returns and sets the input string
{
//	if (temp_str == NULL)														// unneded protection
//		{
//			file_str_read( file_handle , NULL );							// = skip line
//			return NULL;	
//		}
//	else
		{
			file_str_read( file_handle , temp_str );			
			return temp_str;
		}		
}

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
// alpha material detection

function	AlphaMaterialTest(ENTITY* ent)
{
	int i;
	var return_value = 0;
	for(i=0;i<(int)ent_status(ent,8);i++)
		{
			BMAP* skin_bmap = ent_getskin(ent,i+1);			    
		   if (skin_bmap.flags&BMPF_ARGB)
		   	{
		   		return_value = 1;
		   		skin_bmap = NULL;
	   			bmap_remove(skin_bmap);
		   		break;
	   		}
	   	skin_bmap = NULL;
	   	bmap_remove(skin_bmap);
		}
	return(return_value);	// 0 no alpha skins, 1 has an alpha skin
}

function	AlphaMaterialNumber(ENTITY* ent)
{
	int i;
	var return_value = 0;
	for(i=0;i<(int)ent_status(ent,8);i++)
		{
			BMAP* skin_bmap = ent_getskin(ent,i+1);			    
		   if (skin_bmap.flags&BMPF_ARGB)
		   	{
		   		return_value = i+1;
		   		skin_bmap = NULL;
	   			bmap_remove(skin_bmap);
		   		break;
	   		}
	   	skin_bmap = NULL;
	   	bmap_remove(skin_bmap);
		}
	return(return_value);	// 0 no alpha skins, >1 number of 1st alpha skin found
}

///////////////////////////////////////////////////////////////////////////////////////////
// snippet by PadMalcom
// get world position of a vertex of an entity

VECTOR* vec_for_ent_ext(VECTOR* _target, ENTITY* _ent, int _vertexNumber) 
{
	CONTACT* c = ent_getvertex(_ent, NULL, _vertexNumber);
	
	if (c != NULL) 
		{
			vec_set(_target, vector(c->v->x, c->v->z, c->v->y));
			vec_mul(_target, vector(_ent.scale_x, _ent.scale_y, _ent.scale_z));
			vec_rotate(_target, _ent.pan);
			vec_add(_target, _ent.x);
			
			return _target;
		} 
	else
		{
			return NULL;
		}
}

///////////////////////////////////////////////////////////////////////////////////////////
// bounding box

void Update_BBox(ENTITY* temp_ent)
{
	wait(1);
	
//	c_setminmax(temp_ent);
	temp_ent.eflags |= FAT | NARROW; 												
	
	if (temp_ent.MINX != (var)0)	temp_ent.min_x = temp_ent.MINX;				// skill1
	if (temp_ent.MINY != (var)0)	temp_ent.min_y = temp_ent.MINY;
	if (temp_ent.MINZ != (var)0)	temp_ent.min_z = temp_ent.MINZ;
	if (temp_ent.MAXX != (var)0)	temp_ent.max_x = temp_ent.MAXX;
	if (temp_ent.MAXY != (var)0)	temp_ent.max_y = temp_ent.MAXY;
	if (temp_ent.MAXZ != (var)0)	temp_ent.max_z = temp_ent.MAXZ;				// skill6
}

///////////////////////////////////////////////////////////////////////////////////////////////
// removing entities without corresponding entity pointers: details, shrubs, obstacles,trees 


void		Remove_MapEntities()
{
	var k;
	do
		{							
			k = 0;							
			k = Remove_MapEntity();							
		}
	while(k>0);
	
	detail_counter 	= 0;
	shrub_counter 		= 0;
	grass_counter 		= 0;
	obstacle_counter 	= 0;
	mapent_counter 	= 0;
	terrmdl_counter 	= 0;
	building_counter 	= 0;
	// other counter nullings
	//...
}


function		Remove_MapEntity()
{
	you = ent_next(NULL);
 	while (you)
		{ 
			if ((you.ENT_CATEGORY < 2*(var)UIMODE_SHADOWSHIFT) && (you.ENT_CATEGORY > 0))
				{
					// remove its ent_decal shadow attached to terrain or terrain models
					Decal_Remove_Decal(you);
					wait_for(Decal_Remove_Decal);
					
					// remove entity, unique shadow entity, collider entity
					ptr_remove(you);  			
					
					return(1);		
				}				
			
	 		you = ent_next(you);
		}	
	return(0);
}


//-----------------------------------------------------------------------
// remove temp or any entities corresponding to a given category - counter nulling must be done manually


void		Remove_EntCats(int category)
{
	int k;
	do
		{							
			k = 0;							
			k = Remove_EntCat(category);						
		}
	while(k>0);
}


int		Remove_EntCat(int category)
{
	you = ent_next(NULL);
 	while (you)
		{
			if (you.ENT_CATEGORY == (var)category)
				{
					// remove decal attached to terrain or terrain models
					Decal_Remove_Decal(you);
					wait_for(Decal_Remove_Decal);
					
					// remove entity	
					ptr_remove(you); 
					
					// decrease counter... but you can do it after calling Remove_EntCats() manually
					
					return 1;		
				}							
			// unique shadows and collider models
			else if (you.ENT_CATEGORY == ((var)category + (var)UIMODE_SHADOWSHIFT) )
				{
					// remove entity	
					ptr_remove(you);  	
							
					return 1;
				}
			
	 		you = ent_next(you);
		}
	
	return 0;
}


//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////


#endif