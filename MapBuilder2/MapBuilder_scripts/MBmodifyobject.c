//----------------------------------------------------------------
// POINTERS
//
// int a, *p;
// a = 123;
// p = &a;
// str_for_int(value_str, *p);	//=123
//
// *p = 456;
// str_for_int(value_str, a);		//=456
//-------------------------
// int a[5];
// int *p, i;
// for cycle:	a[i] = i;
//
// p = a;									// assign array to pointer
// for (i = 0; i < 5; i++)
// 	{
//	   str_for_int(temp_str, *(p + i));
//	   str_cat(value_str, temp_str);
//	   str_cat(value_str, " ");
// 	}   
//--------------------------
//
// declarations (variable, pointer to variable, pointer to pointer to variable...): int int, *pint, **ppint, ***pppint
//
// *pi - dereference, i.e. get value from memory address pointed to by this pointer, like i = *pi;
// &i - reference, i.e. get memory address of the variable, thus transfer it to a pointer, like pi = &i;
//
//----------------------------------------------------------------
//
//function somefunction(var* localvariable)		// pointer definition
//{
//	localvariable = 5;
//}
//...
//othervariable = 10;
//somefunction(&othervariable);		// send reference of variable
//
//----------------------------------------------------------------


// to do:
//
// snap-xy and snap-z											- ok
// bounding box adjustments, auto after scale			- ok		// continuous update because of sliders
// obstacle material settings for tree material too	- ok
// material change for lods and for all instances		- ok		// + alpha , UNLIT, TRANSLUCENT, OVERLAY
// ambient for all instances option							- ok
// scale for all instances option							- ok
// lightmap for all instances option						- ok
// LIGHT+rgb for all instances option						- ok
// clear all similar instances								- ok
// proper decal shadows update by lightmap for all 	- ok
//
// issues:
//
// collider entity does not move/rotate/scale together with entity (requires move/rotate/scale parent entity) - move rotate ok, scale should be tile size dependent
// ent_decal shadows don't move on entity move, and not updated on OK
// ent_decal shadows are duplicated when lightmap switched on/off, or lightmap for all applied (requires flag check or something in ModifyObject_UpdateShadows)


#ifndef MBmodifyobject_c
#define MBmodifyobject_c



///////////////////////////////////////////////
// code



///////////////////////////////////////////////
// closing

// generates closing
void		ModifyObject_CloseOK()
{
	wait(3);
	
	UIMode = (var)UIMODE_NULL;	// arbitrary temp setting needed because of ui function if statements, no real initialization
	
}

// closing after modifying a selected entity
void		ModifyObject_CloseMod()
{
	you = ptr_for_handle( handle(mod_handle) );
//	you = mod_modified_ent;
	
	// protection
	if (!you)
		{
//			UIMode = (var)UIMODE_NULL;
//			ModifyObject_ResetValues();
			return;
		}
	
	// mainly required by entities having ent_decal shadows
	ModifyObject_UpdateShadows(you, mod_lightmap);
	wait_for(ModifyObject_UpdateShadows);
	
	// set material for lod entities and other instances
	if (mod_origmaterial != you.material)
		{
			ModifyObject_SetMaterialForAll(you);
			wait_for(ModifyObject_SetMaterialForAll);
		}
	mod_origmaterial = NULL;
	
	if (mod_amball)
		{
			ModifyObject_SetAmbientForAll(you);
			wait_for(ModifyObject_SetAmbientForAll);
		}
	
	if (mod_scaleall)
		{
			ModifyObject_SetScaleForAll(you);
			wait_for(ModifyObject_SetScaleForAll);
		}
	
	if (mod_rgball)
		{
			ModifyObject_SetRGBForAll(you);
			wait_for(ModifyObject_SetRGBForAll);
		}
	
	if (mod_lmall)
		{
			ModifyObject_SetLightmapForAll(you);
			wait_for(ModifyObject_SetLightmapForAll);
		}
	
	// set tile position if x-y have changed
	you.XTILE_POS = XCoordToTile(you.x);
	you.YTILE_POS = YCoordToTile(you.y);
	you.ABSTILE_POS = XYTileToAbsTile(you.XTILE_POS, you.YTILE_POS);
		
	// set back to static
	you.emask &= ~DYNAMIC;	
	
	ModifyObject_ResetValues();
}


// removing modified entity and all similar entities
void		ModifyObject_ClearAll()
{
	wait(3);
	
	if (!mod_modified_ent) return;
	
	proc_kill2(ModifyObject_Select, NULL);		
	
	STRING* temp_str = str_create("");
	str_for_entfile(temp_str, mod_modified_ent);
	
//	var k;
//	do
//		{							
//			k = 0;							
//			k = ModifyObject_RemoveEntity(temp_str);										
//		}
//	while(k>0);
	
	ModifyObject_RemoveEntities(temp_str);
	wait_for(ModifyObject_RemoveEntities);
	
	UIMode = (var)UIMODE_NULL;	// arbitrary temp setting needed because of ui function if statements, no real initialization
	
	ModifyObject_ResetValues();
	
	ptr_remove(temp_str);
}


void		ModifyObject_RemoveEntities(STRING* filename)
{
	ENTITY* ent = ent_next(NULL); // get first entity
	ENTITY* temp_ent = NULL;
	
 	while (ent) // repeat until there are no more entities
		{ 
			if (str_cmpi(filename, str_for_entfile(NULL, ent)))
				{
					// decrease corresponding entity counter
					if (mod_origmenu == (var)UIMODE_OBSTACLE)
						{
							obstacle_counter--;
						}
					else if (mod_origmenu == (var)UIMODE_MAPENT)
						{
							mapent_counter--;
						}
					else if (mod_origmenu == (var)UIMODE_GRASS)
						{
							grass_counter--;
						}
					else if (mod_origmenu == (var)UIMODE_TERRMDL)
						{
							terrmdl_counter--;
						}
					else if (mod_origmenu == (var)UIMODE_BUILDING)
						{
							building_counter--;
						}
						
					// ent_decal shadows obstacle or mapent or building (or shrub)
					if ((mod_origmenu == (var) UIMODE_OBSTACLE) || (mod_origmenu == (var) UIMODE_MAPENT) || (mod_origmenu == (var) UIMODE_BUILDING))
						{
							// remove decal attached to terrain or terrain models
							Decal_Remove_Decal(ent);
							wait_for(Decal_Remove_Decal);
						}
					
					// collider entities
					if ((mod_origmenu == (var) UIMODE_OBSTACLE) || (mod_origmenu == (var) UIMODE_BUILDING))
						{
							if (ent.parent)
								{
									ptr_remove(ent.parent); 
								}
						}
					
					// store next entity
					temp_ent = ent_next(ent);
					
					// remove entity	
					ptr_remove(ent);  
					
					// update entities for while loop
					ent = NULL;
					ent = temp_ent;
 					temp_ent = NULL;	
				}									
			else
				{
	 				ent = ent_next(ent); 
	 				temp_ent = NULL;
 				}
 			
		}	
}


void		ModifyObject_ResetValues()
{	
	// remove gizmos
	if (mod_gizmo_x_ent) ptr_remove(mod_gizmo_x_ent);
	mod_gizmo_x_ent = NULL;
	
	if (mod_gizmo_y_ent) ptr_remove(mod_gizmo_y_ent);
	mod_gizmo_y_ent = NULL;
	
	if (mod_gizmo_z_ent) ptr_remove(mod_gizmo_z_ent);
	mod_gizmo_z_ent = NULL;
	
	// reset entity
	mod_modified_ent = NULL;
	
	// NULL var pointers 
	mod_handle = 0;
		
	mod_x = NULL;
	mod_y = NULL;
	mod_z = NULL;
	
	mod_pan  = NULL;
	mod_tilt = NULL;
	mod_roll = NULL;

	mod_r = NULL;
	mod_g = NULL;
	mod_b = NULL;
	
	mod_alpha   = NULL;
	mod_ambient = NULL;
	
	mod_scale_x	= NULL;
	mod_scale_y	= NULL;
	mod_scale_z	= NULL;
	
	mod_ent_cat = 0;
	
	vec_fill(mod_orig_bbmin, 0);
	vec_fill(mod_orig_bbmax, 0);
	vec_fill(mod_orig_scale, 0);
	
	// close upper panel
	if (mod_panel) ptr_remove(mod_panel);	
	mod_panel = NULL;
	
	// not internal closing to previous menu, but calling another main menu 
	// (by pressing the ok button it sets UIMode to UIMODE_FILEMENU state to exit the loop, and to start this function)
	// outer call sets new menu state by main ui loop at "main menu init calls	"
	if (UIMode != (var)UIMODE_NULL)
		{
			//mod_origmenu = UIMode;
			return;
		}
	
	// call previous or new
	if (mod_origmenu == (var)UIMODE_OBSTACLE)
		{
			UI_ObstacleMode();	
		}
	else if (mod_origmenu == (var)UIMODE_MAPENT)
		{
			UI_MapEntMode();	
		}	
	else if (mod_origmenu == (var)UIMODE_GRASS)
		{
			UI_GrassMode();	
		}	
	else if (mod_origmenu == (var)UIMODE_TERRMDL)
		{
			UI_TerrMdlMode();	
		}
	else if (mod_origmenu == (var)UIMODE_BUILDING)
		{
			UI_BuildingMode();	
		}
}


/////////////////////////////////////////////////////////////////
// initialization

void		ModifyObject_Select(ENTITY* ent)
{
	//--------------------------------------------------------
	// setting the entity to be modified
	
	// called by cloning	
	if (ent)
		{			
			you = ent;
		}
	// called by clicking
	else
		{
			you = NULL;
			
			// case A - detect by tile position (non-colliding entities)
			if (mod_origmenu==(var)UIMODE_GRASS)
				{	
					you = ModifyObject_SelectByTile();
				}	
//			// case A+B - detect by boundign box or if passable by collider entity's tile position
//			else if (mod_origmenu==(var)UIMODE_OBSTACLE)			
//				{
//					you = ModifyObject_SelectByMixed();
//				}	
			// case B - detect by bounding box - ignoring collider entities
			else
				{
					you = ModifyObject_SelectByClick();	
				}
		}
	
	if (you == NULL)
		{
//			ModifyObject_CloseMod();		// not needed, nothing is set up, no uimode is changed !!!
			return;
		}
	
	//--------------------------------------------------------
	// menu initializations
	
	UIMode = (var)UIMODE_MODIFYOBJ2;		// *** to disable placement menu	
	
	ModifyObject_SetSelected();
	wait_for(ModifyObject_SetSelected);	
	
	ModifyObject_Panel_Init2();		
	wait_for(ModifyObject_Panel_Init2);
	
	mod_gizmo_x_ent = ent_create("graphics\\gizmo_arrow_x.mdl" , you.x , ModifyObject_Gizmo);
	mod_gizmo_y_ent = ent_create("graphics\\gizmo_arrow_y.mdl" , you.x , ModifyObject_Gizmo);
	mod_gizmo_z_ent = ent_create("graphics\\gizmo_arrow_z.mdl" , you.x , ModifyObject_Gizmo);
	
	//--------------------------------------------------------
	// menu and object update
	
	while(UIMode == (var)UIMODE_MODIFYOBJ2)
		{
			//------------------			
			// gizmo
			
			if (
					(proc_status(ModifyObject_Gizmo_Move) == (var)0)
					&&
					(proc_status(ModifyObject_Gizmo_Rotate) == (var)0)
					&&
					(proc_status(ModifyObject_Gizmo_Scale) == (var)0)
				)		
				{
					// gizmo position
					vec_set(mod_gizmo_x_ent.x, you.x);
					vec_set(mod_gizmo_y_ent.x, you.x);
					vec_set(mod_gizmo_z_ent.x, you.x);
					
					// gizmo angles
					if (mod_type > (var)0)
						{							
							vec_set(mod_gizmo_x_ent.pan, you.pan);
							vec_set(mod_gizmo_y_ent.pan, you.pan);
							vec_set(mod_gizmo_z_ent.pan, you.pan);
						}
					else
						{
							vec_set(mod_gizmo_x_ent.pan, nullvector);
							vec_set(mod_gizmo_y_ent.pan, nullvector);
							vec_set(mod_gizmo_z_ent.pan, nullvector);
						}
				}
			
			//------------------
			// object
			
			// object scale
			ModifyObject_Update_Scale(you);
			wait_for(ModifyObject_Update_Scale);
			
			// object snap xy or z if checked
			ModifyObject_Update_SnapXYZ(you);					
			wait_for(ModifyObject_Update_SnapXYZ);						
			
			// object tile pos (not all entity types has area xy parameters stored)
			ModifyObject_Update_TilePos(you);
			wait_for(ModifyObject_Update_TilePos);	
			
			// object bbox drawing
			ModifyObject_BBox(you);
			wait_for(ModifyObject_BBox);
			
			// object collider entity modification, if exists
			ModifyObject_Update_Collider(you);
			wait_for(ModifyObject_Update_Collider);
			
			// object debug info
			ModifyObject_Update_Info(you);
			wait_for(ModifyObject_Update_Info);
			
			//------------------
			
			wait(1);
			
			//------------------
		}
	
	//--------------------------------------------------------
	// closing
	
	ModifyObject_CloseMod();
	
	//--------------------------------------------------------
}


ENTITY*	ModifyObject_SelectByTile()
{
	you = NULL;
	
	var temp = 0;
	
	// check for entity in the actual tile
	you = ent_next(NULL); // get first entity
 	
 	while (you) // repeat until there are no more entities
		{ 
			if ( (you.XTILE_POS==square_x) && (you.YTILE_POS==square_y) )
				{
					if (you.ENT_CATEGORY == mod_origmenu)
						{
							temp = 1;
							break;
						}
				}
			you = ent_next(you); // get next entity
		}			
	// exit if no valid entity	
	if (!temp)
		{
			return NULL;
		}
	
	return you;
}

	
ENTITY*	ModifyObject_SelectByClick()
{
	you = NULL;
	
	VECTOR pos1, pos2;
	
	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,15000) );     		
	vec_for_screen (pos2, camera);
	
	// ignore group 1 - i,e, collider entities - in A7 it does not work with c_trace !!!
	c_ignore(1,0);
//	me = camera_cameraobject;		// its group value is 1 just like collider entities - ignorable by push - A7
	
	// rays an ellipsoid that collides with polygonal shape of models
	c_trace (pos1.x, pos2.x, IGNORE_SPRITES | IGNORE_WORLD | USE_POLYGON ); 		//	IGNORE_ME | IGNORE_PUSH | 			| USE_BOX
	
	if (you)
		{
//			you = hit.entity;	// not needed, set by ctrace
			
			if (you.ENT_CATEGORY != mod_origmenu)
				{
//					printf("different category A %0.f", (double)(you.ENT_CATEGORY));					
					you = NULL;																			// important because withoput it it returns an other category entitiy!!!
					
					if (mouse_ent)																		// double check because of wmb passable entities - terrain is hit by c_trace, but mouse_ent can be okay
						{
							you = mouse_ent;															// because wmb passable entitites have no collision at all...							
							
							if (you.ENT_CATEGORY != mod_origmenu)
								{
									you = NULL;									
//									printf("different category B %0.f", (double)(you.ENT_CATEGORY));
									return NULL;
								}
						}
				}
		}
	else
		{
			if (mouse_ent)																				// double check...
				{
					you = mouse_ent;																	// because of wmb passable entitites, but it's still not really fine... maybe they have no collision at all...							
					
					if (you.ENT_CATEGORY != mod_origmenu)
						{
							you = NULL;							
//							printf("different category C %0.f", (double)(you.ENT_CATEGORY));
							return NULL;
						}
				}
			else
				{
//					printf("nothing found");
					return NULL;
				}
		}
	
	return you;
}


// not used !!!
ENTITY*	ModifyObject_SelectByMixed()
{
	you = NULL;
	
	VECTOR pos1, pos2;
	
	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,50000) ); // should be a big value       		
	vec_for_screen (pos2, camera);
	
	c_ignore(1,0);
	c_trace (pos1.x, pos2.x, IGNORE_PUSH ); 		// ignore collider owners
	
	if (HIT_TARGET)
		{
			you = hit.entity;
			// exit if neighter obstacle nor collider
			if ((you.ENT_CATEGORY != mod_origmenu) && (you.ENT_CATEGORY != mod_origmenu+(var)UIMODE_SHADOWSHIFT))
				{
					UIMode = (var)UIMODE_MODIFYOBJ;
					return NULL;
				}
			// collider - look for owner at the same tile pos
			if (you.ENT_CATEGORY == mod_origmenu+(var)UIMODE_SHADOWSHIFT)	
				{
					var temp = 0;
					// check for entity in the actual tile
					you = ent_next(NULL); // get first entity
				 	while (you) // repeat until there are no more entities
						{ 
							if ( (you.XTILE_POS==square_x) && (you.YTILE_POS==square_y) )
								{
									if (you.ENT_CATEGORY == mod_origmenu)
										{
											temp = 1;
											break;
										}
								}
							you = ent_next(you); // get next entity
						}			
					// exit if no valid entity	
					if (!temp)
						{
							UIMode = (var)UIMODE_MODIFYOBJ;
							return NULL;
						}	
				}
		}
	else
		{
			return NULL;
		}
	
	return you;
}


void	ModifyObject_SetSelected()
{
	mod_modified_ent = you;
	
	// go on if there is a valid selection		
	mod_handle = handle(you);	
	
	//---------------------------------------
	// basic properties
	
	mod_x = &you.x;
	mod_y = &you.y;
	mod_z = &you.z;
	
	mod_pan  = &you.pan;
	mod_tilt = &you.tilt;
	mod_roll = &you.roll;
	
	mod_scale_x = &you.scale_x;
	mod_scale_y = &you.scale_y;
	mod_scale_z = &you.scale_z;
	
	if (is(you,LIGHT))
		{
			mod_light = 1;
		}
	else
		{
			mod_light = 0;
		}		
	mod_r = &you.red;
	mod_g = &you.green;
	mod_b = &you.blue;
	
	//----------------------------------------
	// material properties
	
	mod_origmaterial = you.material;
	
	if (is(you,UNLIT))
		{
			mod_unlit = 1;
		}
	else
		{
			mod_unlit = 0;
		}				
	
	if (mod_origmenu==(var)UIMODE_OBSTACLE)
		{
			if (you.material==mat_model)
				{
					mod_matmodel = 1;
				}
			else
				{
					mod_matmodel = 0;
				}
			
			if (you.material==mtl_tree)
				{
					mod_mtltree = 1;
//					set( my , FLAG7 );
				}
			else
				{
					mod_mtltree = 0;
//					reset( my , FLAG7 );
				}	
		}
	else if (mod_origmenu==(var)UIMODE_MAPENT)
		{
			if (you.material==mat_shaded)
				{
					mod_matmodel = 1;
				}
			else
				{
					mod_matmodel = 0;
				}	
		}	
	else if (mod_origmenu==(var)UIMODE_GRASS)
		{
			if (you.material==mat_model)
				{
					mod_matmodel = 1;
				}
			else
				{
					mod_matmodel = 0;
				}	
		}			
	else if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			if (you.material==mat_terrain)
				{
					mod_matmodel = 1;
				}
			else
				{
					mod_matmodel = 0;
				}	
		}		
	else if (mod_origmenu==(var)UIMODE_BUILDING)
		{
			if (you.material==mat_model)
				{
					mod_matmodel = 1;
				}
			else
				{
					mod_matmodel = 0;
				}
			
//			if (you.material==mtl_tree)
//				{
//					mod_mtltree = 1;
////					set( my , FLAG7 );
//				}
//			else
//				{
//					mod_mtltree = 0;
////					reset( my , FLAG7 );
//				}	
		}
	
	//--------------------------------------------
	// transparency properties
	
	if (is(you,TRANSLUCENT))
		{
			mod_transp = 1;
		}
	else
		{
			mod_transp = 0;
		}	
		
	if (is(you,OVERLAY))
		{
			mod_overl = 1;
		}
	else
		{
			mod_overl = 0;
		}	
	
	//----------------------------------------------
	// other properties
	
	if (is(you,FLAG6))
		{
			mod_lightmap = 0;		// excluded from lightmap
		}
	else
		{
			mod_lightmap = 1;
		}				
		
	mod_alpha 	= &you.alpha;
	mod_ambient = &you.ambient;	
	
	mod_ent_cat = you.ENT_CATEGORY;
	
	vec_set(mod_orig_bbmin, you.min_x);
	vec_set(mod_orig_bbmax, you.max_x);
	vec_set(mod_orig_scale, you.scale_x);
	
	//----------------------------------------------
	
	// reset to dynamic if static
	you.emask |= (DYNAMIC);
	
	// pop it to see actual selection well
	you.z += 16;
	wait(2);
	you.z -= 16;
	wait(1);	
}

//////////////////////////////////////
// bounding box of selected entity

void		ModifyObject_BBox(ENTITY* ent)
{
	VECTOR vMin,vMax;  	
	
	VECTOR vTemp;
	
	VECTOR vTemp1,vTemp2;
	COLOR cTemp;
	vec_set(cTemp,vector(255,200,100));
	
			//----------------------------------------------------------------------
			vec_set(vMin, ent.min_x);    
//			vec_rotate(vMin,ent.pan);
//			vec_add(vMin,ent.x);
			
			vec_set(vMax, ent.max_x);
//			vec_rotate(vMax,ent.pan);
//			vec_add(vMax,ent.x);
			
			//----------------------------------------------------------------------
			// rotating vectors to get proper d3d lines
			
			// 2 intermediate lines are needed to avoid e.g. blue-green color fade when object placement cube is applied			
			draw_line3d(nullvector,NULL,100);
			draw_line3d(vector(1,1,1),cTemp,0);
			draw_line3d(nullvector,NULL,100);
			
			vec_set(vTemp,vector(vMin.x,vMin.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,NULL,100);
			
			vec_set(vTemp,vector(vMin.x,vMax.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMax.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMin.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMin.x,vMin.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMin.x,vMin.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMin.x,vMax.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMax.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMin.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMin.x,vMin.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMin.x,vMax.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,NULL,100); 
			
			vec_set(vTemp,vector(vMin.x,vMax.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMax.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,NULL,100); 
			
			vec_set(vTemp,vector(vMax.x,vMax.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
			
			vec_set(vTemp,vector(vMax.x,vMin.y,vMin.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,NULL,100); 
			
			vec_set(vTemp,vector(vMax.x,vMin.y,vMax.z));
			vec_rotate(vTemp,ent.pan);
			vec_add(vTemp,ent.x);
			draw_line3d(vTemp,cTemp,100); 
}

//////////////////////////////////////
// panel initialization

void	ModifyObject_Panel_Init2()
{		
	mod_panel = pan_create( panel_image2 , 12 );
	set( mod_panel , SHOW | SHADOW | OVERLAY );					
	mod_panel.pos_x 	= ui_submenupanel_x;
	mod_panel.pos_y 	= ui_submenupanel_y;
	mod_panel.red		= panel_font_r;
	mod_panel.green	= panel_font_g;
	mod_panel.blue		= panel_font_b;
	mod_panel.event	= ui_submenu_panelmove;
	
	// string 1
	pan_setstring(mod_panel, 0,	15,10,	panel_font2,			str_create("MODIFY SELECTED OBJECT MODE") );
			
	// button 1 - Ok
	pan_setbutton(mod_panel, 0,	1, 	45,485,		s_ok_on_bmp,s_ok_off_bmp,s_ok_over_bmp,s_ok_over_bmp,							ModifyObject_CloseOK			,NULL,NULL); 	// *** 45 or 100
	
	//--------------------------------------------------------------------------------
	// basic settings
	
	// x-y-z	
	pan_setdigits(mod_panel, 0,	15,50,	"X: %0.f",				panel_font,	1, 			*mod_x);
	pan_setdigits(mod_panel, 0,	15,70,	"Y: %0.f",				panel_font,	1, 			*mod_y);
	pan_setdigits(mod_panel, 0,	15,90,	"Z: %0.f",				panel_font,	1, 			*mod_z);
	
	// sliders 1-2-3
	var min_offset;
	var max_offset;
	
	if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			min_offset = *mod_x - GetMapTileSize()*10;
			max_offset = *mod_x + GetMapTileSize()*10;
		}
	else
		{
			min_offset = *mod_x - GetMapTileSize()/2;
			max_offset = *mod_x + GetMapTileSize()/2;
		}
		mod_minoffset_x = min_offset;
		mod_maxoffset_x = max_offset;
	pan_setslider(mod_panel, 0,	90+12,52,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_x); 
	
	if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			min_offset = *mod_y - GetMapTileSize()*10;
			max_offset = *mod_y + GetMapTileSize()*10;
		}
	else
		{
			min_offset = *mod_y - GetMapTileSize()/2;
			max_offset = *mod_y + GetMapTileSize()/2;
		}
		mod_minoffset_y = min_offset;
		mod_maxoffset_y = max_offset;
	pan_setslider(mod_panel, 0,	90+12,72,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_y); 
	
	if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			min_offset = *mod_z - GetMapTileSize()*10;
			max_offset = *mod_z + GetMapTileSize()*10;
		}
	else
		{
			min_offset = *mod_z - GetMapTileSize()/2;
			max_offset = *mod_z + GetMapTileSize()/2;
		}
		mod_minoffset_z = min_offset;
		mod_maxoffset_z = max_offset;
	pan_setslider(mod_panel, 0,	90+12,92,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_z); 
	
	// pan-tilt-roll , sliders 4-5-6
	min_offset = -180;
	max_offset = 180;
		mod_minoffset_ptr = min_offset;
		mod_maxoffset_ptr = max_offset;
	pan_setdigits(mod_panel, 0,	15,130,	"Pan: %0.f",				panel_font,	1, 			*mod_pan);
	pan_setdigits(mod_panel, 0,	15,150,	"Tilt: %0.f",				panel_font,	1, 			*mod_tilt);
	pan_setdigits(mod_panel, 0,	15,170,	"Roll: %0.f",				panel_font,	1, 			*mod_roll);
	pan_setslider(mod_panel, 0,	90+12,132,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_pan); 
	pan_setslider(mod_panel, 0,	90+12,152,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_tilt); 
	pan_setslider(mod_panel, 0,	90+12,172,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_roll); 
	
	// alpha-ambient , sliders 7-8		
	pan_setdigits(mod_panel, 0,	15,280,	"Alpha: %0.f",				panel_font,	1, 			*mod_alpha);	
	pan_setslider(mod_panel, 0,	90+12,282,	horiz_slider_bmp,slider_knob_bmp,	0,100,		*mod_alpha); 
	pan_setdigits(mod_panel, 0,	15,300,	"Ambient: %0.f",			panel_font,	1, 			*mod_ambient);	
	pan_setslider(mod_panel, 0,	90+12,302,	horiz_slider_bmp,slider_knob_bmp,	0,100,		*mod_ambient); 
	
	// scale-x-y-z , sliders 9-10-11
	if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			min_offset = 0.1;
			max_offset = 5;
		}
	else
		{
			min_offset = 0.75;
			max_offset = 1.25;
		}
	mod_minoffset_sc = min_offset;
	mod_maxoffset_sc = max_offset;
	pan_setdigits(mod_panel, 0,	15,210,	"Scale X: %0.2f",			panel_font,	1, 			*mod_scale_x);
	pan_setslider(mod_panel, 0,	90+12,212,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,	*mod_scale_x); 
	pan_setdigits(mod_panel, 0,	15,230,	"Scale Y: %0.2f",			panel_font,	1, 			*mod_scale_y);
	pan_setslider(mod_panel, 0,	90+12,232,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,	*mod_scale_y); 
	pan_setdigits(mod_panel, 0,	15,250,	"Scale Z: %0.2f",			panel_font,	1, 			*mod_scale_z);
	pan_setslider(mod_panel, 0,	90+12,252,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,	*mod_scale_z); 
	
	// Light RGB  , sliders 12-13-14
	// button 2
	pan_setstring(mod_panel, 0,	40,320,	panel_font,			str_create("set LIGHT to adjust RGB values:") );	
	pan_setbutton(mod_panel, 0,	2, 	15,320,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,					ModifyObject_ToggleLight	,NULL,NULL); 
	button_state( mod_panel , 2 , mod_light ); 		// 1 ON, 0 OFF	
	
	pan_setdigits(mod_panel, 0,	15,340,	"R: %0.f",				panel_font,	1, 			*mod_r);
	pan_setdigits(mod_panel, 0,	15,360,	"G: %0.f",				panel_font,	1, 			*mod_g);
	pan_setdigits(mod_panel, 0,	15,380,	"B: %0.f",				panel_font,	1, 			*mod_b);
	pan_setslider(mod_panel, 0,	90+12,342,	horiz_slider_bmp,slider_knob_bmp,	0,255,		*mod_r); 
	pan_setslider(mod_panel, 0,	90+12,362,	horiz_slider_bmp,slider_knob_bmp,	0,255,		*mod_g); 
	pan_setslider(mod_panel, 0,	90+12,382,	horiz_slider_bmp,slider_knob_bmp,	0,255,		*mod_b); 
	
	//--------------------------------------------------------------------------------
	// material settings
	
	// unlit-model-translucent-overlay
	// button 3
	pan_setstring(mod_panel, 0,	40,400,	panel_font,			str_create("set UNLIT") );	
	pan_setbutton(mod_panel, 0,	2, 	15,400,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,					ModifyObject_ToggleUnlit	,NULL,NULL); 
	button_state( mod_panel , 3 , mod_unlit ); 		// 1 ON, 0 OFF	
	// button 4
	if ( (mod_origmenu==(var)UIMODE_OBSTACLE) || (mod_origmenu==(var)UIMODE_GRASS) || (mod_origmenu==(var)UIMODE_BUILDING))
		{
			pan_setstring(mod_panel, 0,	40,420,	panel_font,			str_create("set mat_model") );
		}	
	else if (mod_origmenu==(var)UIMODE_MAPENT)
		{
			pan_setstring(mod_panel, 0,	40,420,	panel_font,			str_create("set mat_shaded") );
		}
	else if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			pan_setstring(mod_panel, 0,	40,420,	panel_font,			str_create("set mat_terrain") );
		}
	pan_setbutton(mod_panel, 0,	2, 	15,420,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,					ModifyObject_ToggleModel	,NULL,NULL); 
	button_state( mod_panel , 4 , mod_matmodel ); 		// 1 ON, 0 OFF	
	
	//--------------------------------------------------------------------------------
	// transparency settings
	
	// button 5
	if (mod_origmenu==(var)UIMODE_OBSTACLE)
		{
			pan_setstring(mod_panel, 0,	145,400,	panel_font,			str_create("set TRANSLUCENT") );		// y 440
			pan_setbutton(mod_panel, 0,	2, 	125,400,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,			ModifyObject_ToggleTransp	,NULL,NULL); 
			button_state( mod_panel , 5 , mod_transp ); 		// 1 ON, 0 OFF	
		}
	else		// hide it
		{
			pan_setstring(mod_panel, 0,	255,400,	panel_font,			str_create(" ") );	
			pan_setbutton(mod_panel, 0,	2, 	255,400,		fileselect_off_bmp,fileselect_off_bmp,NULL,NULL,					NULL	,NULL,NULL); 
		}
	
	// button 6
	pan_setstring(mod_panel, 0,	40,440,	panel_font,			str_create("set OVERLAY") );	
	pan_setbutton(mod_panel, 0,	2, 	15,440,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,					ModifyObject_ToggleOverl	,NULL,NULL); 
	button_state( mod_panel , 6 , mod_overl ); 		// 1 ON, 0 OFF	
	
	//--------------------------------------------------------------------------------
	// button 7,8	9,10	11,12				x y z
	pan_setbutton(mod_panel, 0,	1, 	90,50,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,50,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	90,70,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,70,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	90,90,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,90,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	// button 13,14	15,16		17,18		pan tilt roll
	pan_setbutton(mod_panel, 0,	1, 	90,130,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,130,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	90,150,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,150,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	90,170,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,170,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	// button 19,20	21,22					alpha ambient
	pan_setbutton(mod_panel, 0,	1, 	90,280,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,280,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	90,300,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,300,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	// button 23,24	25,26		27,28		scale_xyz
	pan_setbutton(mod_panel, 0,	1, 	90,210,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,210,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	90,230,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,230,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	90,250,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,250,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	// button 29,30	31,32		33,34		r g b
	pan_setbutton(mod_panel, 0,	1, 	90,340,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,340,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	90,360,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,360,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	90,380,		ss_arrow_left_on_bmp, ss_arrow_left_off_bmp, ss_arrow_left_on_bmp, ss_arrow_left_off_bmp,				ModifyObject_Button, 		NULL, NULL); 
	pan_setbutton(mod_panel, 0,	1, 	210,380,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_Button, 		NULL, NULL); 
	
	//--------------------------------------------------------------------------------
	// button 35 - jump to next entity in same tile by ModifyObject_NextEntity
	if (mod_origmenu==(var)UIMODE_GRASS)
		{
			pan_setstring(mod_panel, 0,	35,460,	panel_font,			str_create("Next Entity") );
			pan_setbutton(mod_panel, 0,	1, 	15,462,		ss_arrow_right_on_bmp, ss_arrow_right_off_bmp, ss_arrow_right_on_bmp, ss_arrow_right_off_bmp,		ModifyObject_NextEntity, 		NULL, NULL); 	
		}
	else		// hide it
		{
			pan_setstring(mod_panel, 0,	255,30,	panel_font,			str_create("") );	
			pan_setbutton(mod_panel, 0,	1, 	255,32,		fileselect_off_bmp,fileselect_off_bmp,NULL,NULL,					NULL	,NULL,NULL); 
		}
	
	//--------------------------------------------------------------------------------
	// button 36 - lightmap
	if ((mod_origmenu==(var)UIMODE_OBSTACLE) || (mod_origmenu==(var)UIMODE_MAPENT) || (mod_origmenu==(var)UIMODE_TERRMDL) || (mod_origmenu==(var)UIMODE_BUILDING))
		{
			pan_setstring(mod_panel, 0,	40,460,	panel_font,			str_create("lightmap") );	
			pan_setbutton(mod_panel, 0,	2, 	15,460,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,							ModifyObject_ToggleLightm	,NULL,NULL); 
			button_state( mod_panel , 36 , mod_lightmap ); 		// 1 ON, 0 OFF	
		}
	else if (mod_origmenu == (var)UIMODE_GRASS)
		{
			pan_setstring(mod_panel, 0,	145,400,	panel_font,			str_create("no shadow") );	
			pan_setbutton(mod_panel, 0,	2, 	125,400,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,							ModifyObject_ToggleLightm	,NULL,NULL); 
			button_state( mod_panel , 36 , mod_lightmap ); 		// 1 ON, 0 OFF	
		}
	else		// hide it
		{
			pan_setstring(mod_panel, 0,	255,460,	panel_font,			str_create("") );	
			pan_setbutton(mod_panel, 0,	2, 	255,460,		fileselect_off_bmp,fileselect_off_bmp,NULL,NULL,					NULL	,NULL,NULL); 
		}
	
	//--------------------------------------------------------------------------------
	// toggle mod_type
	// button 37-38-39
	
//	pan_setstring(mod_panel, 0,	223,35,	panel_font,			str_create("Mode") );	
	pan_setstring(mod_panel, 0,	40,33,	panel_font,			str_create("Move") );	
	pan_setstring(mod_panel, 0,	40,113,	panel_font,			str_create("Rotate") );	
	pan_setstring(mod_panel, 0,	40,193,	panel_font,			str_create("Scale") );	
	
	pan_setbutton(mod_panel, 0,	4, 	15,33,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		ModifyObject_ToggleMove,NULL,NULL); 		// 228,72,
	if (mod_type==(var)0) button_state( mod_panel , 37 , ON ); 		// 1 ON, 0 OFF	
	
	pan_setbutton(mod_panel, 0,	4, 	15,113,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		ModifyObject_ToggleRotate,NULL,NULL); 		// 228,142,
	if (mod_type==(var)1) button_state( mod_panel , 38 , ON ); 		// 1 ON, 0 OFF	
	
	pan_setbutton(mod_panel, 0,	4, 	15,193,		radiobtn_on_bmp,radiobtn_off_bmp,radiobtn_offover_bmp,radiobtn_onover_bmp,		ModifyObject_ToggleScale,NULL,NULL); 		// 228,212,
	if (mod_type==(var)2) button_state( mod_panel , 39 , ON ); 		// 1 ON, 0 OFF	
	
	//--------------------------------------------------------------------------------
	// snap	 
	
	// button 40
	pan_setstring(mod_panel, 0,	110,33,	panel_font,			str_create("Snap XY") );
	pan_setbutton(mod_panel, 0,	2, 	90,33,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		ModifyObject_ToggleSnapXY	,NULL,NULL); 
	button_state( mod_panel , 40 , mod_snapxy ); 		// 1 ON, 0 OFF	
	
	// button 41
	pan_setstring(mod_panel, 0,	180,33,	panel_font,			str_create("Snap Z") );
	pan_setbutton(mod_panel, 0,	2, 	160,33,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		ModifyObject_ToggleSnapZ	,NULL,NULL); 
	button_state( mod_panel , 41 , mod_snapz ); 			// 1 ON, 0 OFF	
	
	//--------------------------------------------------------------------------------
	// material settings cont.
	
	// toggle change ambient for all instances
	// button 42
	pan_setstring(mod_panel, 0,	145,440,	panel_font,			str_create("Amb All") );
	pan_setbutton(mod_panel, 0,	2, 	125,440,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		ModifyObject_ToggleAmbAll	,NULL,NULL); 
	button_state( mod_panel , 42 , mod_amball ); 		// 1 ON, 0 OFF	
	
	// toggle change scale for all instances
	// button 43
	pan_setstring(mod_panel, 0,	145,460,	panel_font,			str_create("Scale All") );		
	pan_setbutton(mod_panel, 0,	2, 	125,460,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		ModifyObject_ToggleScaleAll	,NULL,NULL); 
	button_state( mod_panel , 43 , mod_scaleall ); 		// 1 ON, 0 OFF	
	
	// toggle change light+rgb for all instances
	// button 44
	pan_setstring(mod_panel, 0,	210,440,	panel_font,			str_create("RGB All") );		
	pan_setbutton(mod_panel, 0,	2, 	190,440,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		ModifyObject_ToggleRGBAll	,NULL,NULL); 
	button_state( mod_panel , 44 , mod_rgball ); 		// 1 ON, 0 OFF	
	
	// toggle change lightmap for all instances
	if (mod_origmenu != (var)UIMODE_GRASS)
		{
			// button 45
			pan_setstring(mod_panel, 0,	210,460,	panel_font,			str_create("Lm All") );				
		}
	else
		{
			// button 45
			pan_setstring(mod_panel, 0,	210,460,	panel_font,			str_create("NS All") );				
		}
	pan_setbutton(mod_panel, 0,	2, 	190,460,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		ModifyObject_ToggleLmAll	,NULL,NULL); 
	button_state( mod_panel , 45 , mod_lmall ); 		// 1 ON, 0 OFF	
	
	// mtl_tree
	if (mod_origmenu == (var)UIMODE_OBSTACLE)
		{
			// button 46
			pan_setstring(mod_panel, 0,	145,420,	panel_font,			str_create("set mtl_tree") );	
			pan_setbutton(mod_panel, 0,	2, 	125,420,		checkbox_on_bmp,checkbox_off_bmp,checkbox_onover_bmp,checkbox_offover_bmp,		ModifyObject_ToggleTree	,NULL,NULL); 
			button_state( mod_panel , 46 , mod_mtltree ); 		// 1 ON, 0 OFF	
		}
		
	//-------------------------------------------------------------------------------
	// button 47 - clear all
	pan_setbutton(mod_panel, 0,	1, 	160,485,		m_clearall_on_bmp,m_clearall_off_bmp,m_clearall_over_bmp,m_clearall_over_bmp,			ModifyObject_ClearAll	,NULL,NULL);
	
}


void		ModifyObject_Button(button_number)
{
	wait(3);
	
	//	*mod_x;
	if (button_number==(var)7)
		{
			*mod_x = maxv( mod_minoffset_x , --*mod_x );
		}
	else if (button_number==(var)8)
		{
			*mod_x = minv( mod_maxoffset_x , ++*mod_x );
		}
	//	*mod_y;
	else if (button_number==(var)9)
		{
			*mod_y = maxv( mod_minoffset_y , --*mod_y );
		}
	else if (button_number==(var)10)
		{
			*mod_y = minv( mod_maxoffset_y , ++*mod_y );
		}
	//	*mod_z;
	else if (button_number==(var)11)
		{
			*mod_z = maxv( mod_minoffset_z , --*mod_z );
		}
	else if (button_number==(var)12)
		{
			*mod_z = minv( mod_maxoffset_z , ++*mod_z );
		}
	//	*mod_pan;
	else if ((button_number==(var)13) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_pan = maxv( mod_minoffset_ptr , --*mod_pan );
		}
	else if ((button_number==(var)14) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_pan = minv( mod_maxoffset_ptr , ++*mod_pan );
		}
	//	*mod_tilt;
	else if ((button_number==(var)15) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_tilt = maxv( mod_minoffset_ptr , --*mod_tilt );
		}
	else if ((button_number==(var)16) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_tilt = minv( mod_maxoffset_ptr , ++*mod_tilt );
		}
	//	*mod_roll;
	else if ((button_number==(var)17) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_roll = maxv( mod_minoffset_ptr , --*mod_roll );
		}
	else if ((button_number==(var)18) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_roll = minv( mod_maxoffset_ptr , ++*mod_roll );
		}
	//	*mod_alpha;
	else if (button_number==(var)19)
		{
			*mod_alpha = maxv( 0 , --*mod_alpha );
		}
	else if (button_number==(var)20)
		{
			*mod_alpha = minv( 100 , ++*mod_alpha );
		}
	//	*mod_ambient;
	else if (button_number==(var)21)
		{
			*mod_ambient = maxv( 0 , --*mod_ambient );
		}
	else if (button_number==(var)22)
		{
			*mod_ambient = minv( 100 , ++*mod_ambient );
		}
	//	*mod_scale_x;
	else if ((button_number==(var)23) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_scale_x -= 0.01;
			*mod_scale_x = maxv( mod_minoffset_sc , *mod_scale_x );
		}
	else if ((button_number==(var)24) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_scale_x += 0.01;
			*mod_scale_x = minv( mod_maxoffset_sc , *mod_scale_x );
		}
	//	*mod_scale_y;
	else if ((button_number==(var)25) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_scale_y -= 0.01;
			*mod_scale_y = maxv( mod_minoffset_sc , *mod_scale_y );
		}
	else if ((button_number==(var)26) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_scale_y += 0.01;
			*mod_scale_y = minv( mod_maxoffset_sc , *mod_scale_y );
		}
	//	*mod_scale_z;
	else if ((button_number==(var)27) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_scale_z -= 0.01;
			*mod_scale_z = maxv( mod_minoffset_sc , *mod_scale_z );
		}
	else if ((button_number==(var)28) && (mod_modified_ent.ENT_TYPE != (var)2))
		{
			*mod_scale_z += 0.01;
			*mod_scale_z = minv( mod_maxoffset_sc , *mod_scale_z );
		}
	//	*mod_r;
	else if (button_number==(var)29)
		{
			*mod_r = maxv( 0 , --*mod_r );
		}
	else if (button_number==(var)30)
		{
			*mod_r = minv( 255 , ++*mod_r );
		}
	//	*mod_g;
	else if (button_number==(var)31)
		{
			*mod_g = maxv( 0 , --*mod_g );
		}
	else if (button_number==(var)32)
		{
			*mod_g = minv( 255 , ++*mod_g );
		}
	//	*mod_b;
	else if (button_number==(var)33)
		{
			*mod_b = maxv( 0 , --*mod_b );
		}
	else if (button_number==(var)34)
		{
			*mod_b = minv( 255 , ++*mod_b );
		}	
}

//-------------------------------------------------------------------

void		ModifyObject_ToggleLight()
{
	wait(3);
	mod_light = 1 - mod_light;
	
	you = ptr_for_handle( handle(mod_handle) );
	toggle( you , LIGHT );
	
	if (mod_light)
		{
			reset( you , UNLIT );
			mod_unlit = 0;
			if (you.material == mat_unlit)
				{
					if (mod_origmenu==(var)UIMODE_OBSTACLE)
						{
							you.material = mtl_obstacle;
							
							int i;
							for(i=0;i<ent_status(you,8);i++)
								{
									ent_mtlset(you, mtl_obstacle, i+1);		// to assign effect too correctly
								}
						}
					else if (mod_origmenu==(var)UIMODE_MAPENT)
						{
							you.material = mtl_mapent;
							
							int i;
							for(i=0;i<ent_status(you,8);i++)
								{
									ent_mtlset(you, mtl_mapent, i+1);
								}
						}
					else if (mod_origmenu==(var)UIMODE_GRASS)
						{
							you.material = mtl_grass;
							
							int i;
							for(i=0;i<ent_status(you,8);i++)
								{
									ent_mtlset(you, mtl_grass, i+1);			// to assign effect correctly too
								}
						}
					else if (mod_origmenu==(var)UIMODE_TERRMDL)
						{
							you.material = mtl_terrmdl;
							
							int i;
							for(i=0;i<ent_status(you,8);i++)
								{
									ent_mtlset(you, mtl_terrmdl, i+1);		// to assign effect correctly too
								}
						}
					else if (mod_origmenu==(var)UIMODE_BUILDING)
						{
							you.material = mtl_building;
							
							int i;
							for(i=0;i<ent_status(you,8);i++)
								{
									ent_mtlset(you, mtl_building, i+1);		// to assign effect correctly too
								}
						}
					else
						{
							
						}
				}
		}
	else 
		{
			
		}
		
	ModifyObject_UpdateButtons();
}

//---------------------------------------------------------------

void		ModifyObject_ToggleUnlit()
{
	wait(3);
	mod_unlit = 1 - mod_unlit;
	
	you = ptr_for_handle( handle(mod_handle) );
	toggle( you , UNLIT );
	
	// reset tree anyway for obstacles
	if (mod_origmenu==(var)UIMODE_OBSTACLE)
		{
			reset( you , FLAG7 );
		}
		
	if (mod_unlit)
		{	
			reset( you , LIGHT );
			mod_light 		= 0;
			mod_matmodel 	= 0;	
			mod_mtltree 	= 0;						
			
			you.material = mat_unlit;
			
			int i;
			for(i=0;i<ent_status(you,8);i++)
				{
					ent_mtlset(you, mat_unlit, i+1);					// to assign effect too correctly
				}
		}
	else
		{
			if (mod_origmenu==(var)UIMODE_OBSTACLE)
				{
					you.material = mtl_obstacle;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_obstacle, i+1);		// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_MAPENT)
				{
					you.material = mtl_mapent;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_mapent, i+1);		// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_GRASS)
				{
					you.material = mtl_grass;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_grass, i+1);			// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_TERRMDL)
				{
					you.material = mtl_terrmdl;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_terrmdl, i+1);		// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_BUILDING)
				{
					you.material = mtl_building;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_building, i+1);		// to assign effect too correctly
						}
				}
			else
				{
					
				}
		}
		
	ModifyObject_UpdateButtons();
}


void		ModifyObject_ToggleModel()
{
	wait(3);
	mod_matmodel = 1 - mod_matmodel;
	
	you = ptr_for_handle( handle(mod_handle) );
	
	// reset tree anyway for obstacles
	if (mod_origmenu==(var)UIMODE_OBSTACLE)
		{
			reset( you , FLAG7 );
		}
	
	if (mod_matmodel)
		{
			reset( you , UNLIT );
			mod_unlit 	= 0;			
			mod_mtltree = 0;
			
			if ( (mod_origmenu==(var)UIMODE_OBSTACLE) || (mod_origmenu==(var)UIMODE_GRASS) || (mod_origmenu==(var)UIMODE_BUILDING) )
				{
					you.material = mat_model;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mat_model, i+1);			// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_MAPENT)
				{
					you.material = mat_shaded;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mat_shaded, i+1);		// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_TERRMDL)
				{
					you.material = mat_terrain;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mat_terrain, i+1);		// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_BUILDING)
				{
					you.material = mtl_building;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_building, i+1);		// to assign effect too correctly
						}
				}
			else
				{
					
				}
		}
	else
		{
			if (mod_origmenu==(var)UIMODE_OBSTACLE)
				{
					you.material = mtl_obstacle;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_obstacle, i+1);		// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_MAPENT)
				{
					you.material = mtl_mapent;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_mapent, i+1);		// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_GRASS)
				{
					you.material = mtl_grass;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_grass, i+1);			// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_TERRMDL)
				{
					you.material = mtl_terrmdl;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_terrmdl, i+1);		// to assign effect too correctly
						}
				}
			else if (mod_origmenu==(var)UIMODE_BUILDING)
				{
					you.material = mtl_building;
					
					int i;
					for(i=0;i<ent_status(you,8);i++)
						{
							ent_mtlset(you, mtl_building, i+1);		// to assign effect too correctly
						}
				}
			else
				{
					
				}
		}
		
	ModifyObject_UpdateButtons();
}


void		ModifyObject_ToggleTree()							// only obstacle models
{
	wait(3);
	mod_mtltree = 1 - mod_mtltree;
	
	you = ptr_for_handle( handle(mod_handle) );
	
	if (mod_mtltree)
		{
			set( you , FLAG7 );
			
			reset( you , UNLIT );
			mod_unlit 		= 0;			
			mod_matmodel 	= 0;
			
			Obstacle_SetTreeMaterial(you);
			reset( you , TRANSLUCENT );
			mod_transp = 0;
			mod_overl  = 1;
			
//			you.material = mtl_tree;
//			
//			int i;
//			for(i=0;i<ent_status(you,8);i++)
//				{
//					ent_mtlset(you, mod_mtltree, i+1);		// to assign effect too correctly
//				}
		}
	else
		{
			you.material = mtl_obstacle;
			
			reset( you , FLAG7 );
			
			int i;
			for(i=0;i<ent_status(you,8);i++)
				{
					ent_mtlset(you, mtl_obstacle, i+1);		// to assign effect too correctly
				}
		}
	
	ModifyObject_UpdateButtons();
}

//----------------------------------------------------------------

void		ModifyObject_ToggleTransp()						// only obstacle models
{
	wait(3);
	mod_transp = 1 - mod_transp;
	
	you = ptr_for_handle( handle(mod_handle) );
	
	if (mod_transp)
		{
			set( you , TRANSLUCENT );	
		}
	else
		{
			reset( you , TRANSLUCENT );	
		}
		
	ModifyObject_UpdateButtons();
}


void		ModifyObject_ToggleOverl()
{
	wait(3);
	mod_overl = 1 - mod_overl;
	
	you = ptr_for_handle( handle(mod_handle) );
	
	if (mod_overl)
		{
			set( you , OVERLAY );			
		}
	else
		{
			reset( you , OVERLAY );	
		}
	
	ModifyObject_UpdateButtons();
}

//----------------------------------------------------------------

void		ModifyObject_ToggleLightm()
{
	wait(3);
	mod_lightmap = 1 - mod_lightmap;
	
	ENTITY* ent = ptr_for_handle(handle(mod_handle));	// or simply : mod_modified_ent  (this is added later)
	
//	you = ptr_for_handle( handle(mod_handle) );
	
	ModifyObject_UpdateShadows(ent, mod_lightmap);
	wait_for(ModifyObject_UpdateShadows);
	
	//-----------------------------------------------------------
	
	ModifyObject_UpdateButtons();
}


void		ModifyObject_UpdateShadows(ENTITY* ent, var modified_lightmap)
{
	// always try to remove ent_decal shadow attached to terrain and terrain models - it will be placed again with new settings
//	if (is(ent,FLAG5))											// noo!!! it is valid only for enforced ent_decal shadows
		{
			Decal_Remove_Decal(ent);
			wait_for(Decal_Remove_Decal);
			wait(1);
		}
	
	if (ent.ENT_CATEGORY == (var)UIMODE_GRASS)	
		{
			reset(ent, SHADOW);	
			reset(ent, CAST);
			reset(ent, FLAG6);
			
			Grass_SetShadow(ent, 1-modified_lightmap);
		}
//	else if (mod_origmenu==(var)UIMODE_TERRMDL)					// okay
	else if (ent.ENT_CATEGORY == (var)UIMODE_TERRMDL)	
		{
			reset(ent, SHADOW);	
			reset(ent, CAST);
			reset(ent, FLAG6);
			
			TerrMdl_SetShadow(ent, 1-modified_lightmap);			
			
//			if (modified_lightmap)								// inverse flag setting !!!
//				{
//					reset(ent, FLAG6);							// include into lightmap
//					reset(ent, SHADOW);			
//				}
//			else
//				{
//					set(ent, FLAG6);								// exclude from lightmap
//					set(ent, SHADOW);
//					
//					if (shadow_stencil==(var)8)
//						{
//							if (AlphaMaterialTest(ent)>0)
//								{
//									set(ent, CAST);				// no self shadow on alpha material objects
//								}
//						}
//				}
		}
//	else if (mod_origmenu==(var)UIMODE_OBSTACLE)
	else if (ent.ENT_CATEGORY == (var)UIMODE_OBSTACLE)	
		{
			// setting from info file
			var shadow_type 	= ModifyObject_ReturnShadow(ent);			
			var isdecal 		= is(ent,FLAG5);
			var istree 			= is(ent,FLAG7);
			
			// this function is for creation time and does not resets flags always!
			// sets SHADOW, CAST, FLAG5, FLAG6, so they should be reset!
			reset(ent, SHADOW);
			reset(ent, CAST);
			reset(ent, FLAG5);
			reset(ent, FLAG6);		
			Obstacle_SetShadow(ent, modified_lightmap, isdecal, shadow_type, istree);	// entity, lightmap, decal, shadow, tree
			wait_for(Obstacle_SetShadow);
		}	
//	else if (mod_origmenu==(var)UIMODE_MAPENT)
	else if (ent.ENT_CATEGORY == (var)UIMODE_MAPENT)	
		{
			var shadow_type 	= ModifyObject_ReturnShadow(ent);			
			var isdecal 		= is(ent,FLAG5);
			
			// this function is for creation time and does not resets flags always!
			// sets SHADOW, CAST, FLAG5, FLAG6, so they should be reset!
			reset(ent, SHADOW);
			reset(ent, CAST);
			reset(ent, FLAG5);
			reset(ent, FLAG6);
			MapEnt_SetShadow(ent, modified_lightmap, isdecal, shadow_type);
			wait_for(MapEnt_SetShadow);
		}	
	else if (ent.ENT_CATEGORY == (var)UIMODE_SHRUB)												// non-modifiable entity category, only outer call to set lightmap property!!!
		{
			var isshadow	 	= (var)1;//is(ent, SHADOW);										// assume some kind of shadow, or it can't work...								
			var isdecal 		= is(ent, FLAG5);
			var firstshrub		= (var)0;																// really used only by creator action
			if ((isshadow>0) || (isdecal>0))
				{
					firstshrub	= (var)1;																
				}
			
			// this function is for creation time and does not resets flags always!
			// sets SHADOW, CAST, FLAG5, FLAG6, so they should be reset!
			reset(ent, SHADOW);
			reset(ent, CAST);
			reset(ent, FLAG5);
			reset(ent, FLAG6);
			Shrub_SetShadow(ent, modified_lightmap, isdecal, isshadow, firstshrub);
			wait_for(Shrub_SetShadow);
		}
	else if (ent.ENT_CATEGORY == (var)UIMODE_BUILDING)	
		{
			// setting from info file
			var shadow_type 	= ModifyObject_ReturnShadow(ent);			
			var isdecal 		= is(ent,FLAG5);
			
			// this function is for creation time and does not resets flags always!
			// sets SHADOW, CAST, FLAG5, FLAG6, so they should be reset!
			reset(ent, SHADOW);
			reset(ent, CAST);
			reset(ent, FLAG5);
			reset(ent, FLAG6);		
			Building_SetShadow(ent, modified_lightmap, isdecal, shadow_type);	// entity, lightmap, decal, shadow
			wait_for(Building_SetShadow);
		}	
}

//---------------------------------------------------------------

void		ModifyObject_ToggleSnapXY()
{
	wait(3);
	mod_snapxy = 1 - mod_snapxy;
}

void		ModifyObject_ToggleSnapZ()
{
	wait(3);
	mod_snapz = 1 - mod_snapz;
}

//---------------------------------------------------------------

void		ModifyObject_ToggleAmbAll()
{
	wait(3);
	mod_amball = 1 - mod_amball;
}

void		ModifyObject_ToggleScaleAll()
{
	wait(3);
	mod_scaleall = 1 - mod_scaleall;
}

void		ModifyObject_ToggleRGBAll()
{
	wait(3);
	mod_rgball = 1 - mod_rgball;
}

void		ModifyObject_ToggleLmAll()
{
	wait(3);
	mod_lmall = 1 - mod_lmall;
}

//////////////////////////////////////

function		ModifyObject_ReturnShadow(ENTITY* ent)
{
	STRING* my_filename = str_create(""); 
	str_for_entfile(my_filename, ent); 		
	
	//-----------------------------------------------------------------------
	var obs_defaultsize		= (var)MAP_DEFAULTGRIDSIZE;
	var obs_tilesize_x		= 1;
	var obs_tilesize_y		= 1;
	var obs_type				= 0;								// 0 no access, 1 passable 
	var obs_collision			= 2;								// 0 passable, 1 polygon, 2 tile area
	var obs_shadow				= 1;								// 0 nothing, 1 dynamic, 2 sprite, 3 model
	
	STRING* temp_str0 = str_create("");
	//-----------------------------------------------------------------------
	// get filename from model name
	if (mod_origmenu==(var)UIMODE_OBSTACLE) 
		{
			str_cpy( temp_str0 , obstacle_foldername );
			str_cat( temp_str0 , my_filename );
			str_trunc( temp_str0 , 4 ); 
			str_cat( temp_str0 , ".obs" );	
		}
	else if (mod_origmenu==(var)UIMODE_MAPENT) 
		{
			str_cpy( temp_str0 , mapent_foldername );
			str_cat( temp_str0 , my_filename );
			str_trunc( temp_str0 , 4 ); 
			str_cat( temp_str0 , ".obs" );	
		}
	else if (mod_origmenu==(var)UIMODE_BUILDING) 
		{
			str_cpy( temp_str0 , building_foldername );
			str_cat( temp_str0 , my_filename );
			str_trunc( temp_str0 , 4 ); 
			str_cat( temp_str0 , ".obs" );	
		}
	//-----------------------------------------------------------------------
	var file_handle = file_open_read( temp_str0 );
	//-----------------------------------------------------------------------
	// read file if exists
	if (file_handle)
		{
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			obs_defaultsize = str_to_num( temp_str0 );
			
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			obs_tilesize_x = str_to_num( temp_str0 );
			
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			obs_tilesize_y = str_to_num( temp_str0 );
			
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			obs_shadow = str_to_num( temp_str0 );
			
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			obs_type = str_to_num( temp_str0 );
			
				file_str_read( file_handle , temp_str0 );		// comment line
			file_str_read( file_handle , temp_str0 );
			obs_collision = str_to_num( temp_str0 );
			
			file_close(file_handle);	
			
//			printf("obsw file read fine");
		}
	ptr_remove(temp_str0);
	//-----------------------------------------------------------------------------	
	return (obs_shadow);
}

//////////////////////////////////////


void		ModifyObject_UpdateButtons()
{
	button_state( mod_panel , 2 , mod_light ); 
	button_state( mod_panel , 3 , mod_unlit );
	button_state( mod_panel , 4 , mod_matmodel ); 
	button_state( mod_panel , 5 , mod_transp ); 	
	button_state( mod_panel , 6 , mod_overl ); 	
	button_state( mod_panel , 36 , mod_lightmap ); 	
	if (mod_origmenu==(var)UIMODE_OBSTACLE)
		{
			button_state( mod_panel , 40 , mod_mtltree ); 	
		}
}

//////////////////////////////////////

void		ModifyObject_Update_Scale(ENTITY* temp_ent)
{
	// scale bounding box (there might be special settings due to decal shadow size, not proportional with the entity size)
	temp_ent.min_x = mod_orig_bbmin.x * (temp_ent.scale_x / mod_orig_scale.x);
	temp_ent.min_y = mod_orig_bbmin.y * (temp_ent.scale_y / mod_orig_scale.y);
	temp_ent.min_z = mod_orig_bbmin.z * (temp_ent.scale_z / mod_orig_scale.z);
	
	temp_ent.max_x = mod_orig_bbmax.x * (temp_ent.scale_x / mod_orig_scale.x);
	temp_ent.max_y = mod_orig_bbmax.y * (temp_ent.scale_y / mod_orig_scale.y);
	temp_ent.max_z = mod_orig_bbmax.z * (temp_ent.scale_z / mod_orig_scale.z);	
}

//////////////////////////////////////

void	ModifyObject_Update_SnapXYZ(ENTITY* ent)
{		
	//snap to tile middle
	if (mod_snapxy) 
		{									
			int tempx_int;
			int tempy_int;
			
			tempx_int = XCoordToTile(ent.x);						
			tempy_int = YCoordToTile(ent.y);			
			
			// modify snapping due to object size - check mod_origmenu or ent.ENT_CATEGORY
			if ( (ent.ENT_CATEGORY == (var)UIMODE_OBSTACLE) || (ent.ENT_CATEGORY == (var)UIMODE_MAPENT) || (ent.ENT_CATEGORY == (var)UIMODE_BUILDING) )
				{
					if ((int)(ent.AREA_SIZEX)%2 == 0)
						{
							ent.x = integer(ent.x / GetMapTileSize()) * GetMapTileSize();	// to tile edge
						}
					else
						{
							ent.x = XTileToCoord(tempx_int);											// to tile middle
						}
					
					if ((int)(ent.AREA_SIZEY)%2 == 0)
						{
							ent.y = integer(ent.y / GetMapTileSize()) * GetMapTileSize();	// to tile edge
						}
					else
						{
							ent.y = YTileToCoord(tempy_int);											// to tile middle
						}
				}
			else
				{
					ent.x = XTileToCoord(tempx_int);													// to tile middle
					ent.y = YTileToCoord(tempy_int);													// to tile middle
				}
		}
	
	// snap to tilesize downwards
	if (mod_snapz) 
		{
			ent.z = integer(ent.z / GetMapTileSize()) * GetMapTileSize();					// to tile edge
		}	
}

//////////////////////////////////////

void		ModifyObject_Update_TilePos(ENTITY* ent)
{
	if ( (ent.ENT_CATEGORY == (var)UIMODE_OBSTACLE) || (ent.ENT_CATEGORY == (var)UIMODE_MAPENT) || (ent.ENT_CATEGORY == (var)UIMODE_BUILDING) )
		{
			if ((int)(ent.AREA_SIZEX)%2 == 0)
				{
					ent.XTILE_POS = XCoordToTile(ent.x - GetMapTileSize()/2);							
				}
			else
				{
					ent.XTILE_POS = XCoordToTile(ent.x);
				}
			
			if ((int)(you.AREA_SIZEY)%2 == 0)
				{
					ent.YTILE_POS = YCoordToTile(ent.y - GetMapTileSize()/2);							
				}
			else
				{
					ent.YTILE_POS = YCoordToTile(ent.y);
				}
		}
	else
		{
			ent.XTILE_POS = XCoordToTile(ent.x);
			ent.YTILE_POS = YCoordToTile(ent.y);
		}
	
	ent.ABSTILE_POS = XYTileToAbsTile(ent.XTILE_POS, ent.YTILE_POS);
}

//////////////////////////////////////

void		ModifyObject_Update_Collider(ENTITY* ent)
{
	if (ent.parent)
		{
			// pos
			vec_set(ent.parent.x, ent.x);
			// tile pos
			ent.parent.XTILE_POS = XCoordToTile(ent.parent.x);
			ent.parent.YTILE_POS = YCoordToTile(ent.parent.y);
			
			// rot
			if (!is(ent, FLAG7))													// if tree, stay axis aligned
				{
					vec_set(ent.parent.pan, ent.pan);
				}
			
			// scale requires to store original values (and take into account tile size ?)
			
			// ...
			
		}
}

//////////////////////////////////////

void		ModifyObject_Update_Info(ENTITY* ent)
{
	COLOR colvec;
	vec_set(colvec, vector(50,50,50));
	
	var originx = 10;
	var originy = 625;
	
	draw_text( str_for_entfile(NULL, ent), 							originx, 		originy , colvec );
	
	draw_text( "Tile Position X-Y-Z-Abs:", 							originx, 		originy+25 , colvec );
	draw_text( str_for_int(NULL, ent.XTILE_POS) , 					originx+250 , 	originy+25 , colvec );
	draw_text( str_for_int(NULL, ent.YTILE_POS) , 					originx+300 , 	originy+25 , colvec );
	draw_text( str_for_int(NULL, ent.ZTILE_POS) , 					originx+350 , 	originy+25 , colvec );
	draw_text( str_for_int(NULL, ent.ABSTILE_POS) , 				originx+400 , 	originy+25 , colvec );
	
	draw_text( "Area Size X-Y:", 											originx, 		originy+45 , colvec );
	draw_text( str_for_int(NULL, ent.AREA_SIZEX) , 					originx+250 , 	originy+45 , colvec );
	draw_text( str_for_int(NULL, ent.AREA_SIZEY) , 					originx+300 , 	originy+45 , colvec );
	
	draw_text( "Angle / Direction:",										originx, 		originy+65 , colvec );
	draw_text( str_for_int(NULL, (int)ent.pan) , 					originx+250 , 	originy+65 , colvec );
	draw_text( str_for_int(NULL, AngleToDirection(ent.pan)) , 	originx+300 , 	originy+65 , colvec );
}

//////////////////////////////////////

void	ModifyObject_NextEntity()
{
	// set you
	you = ptr_for_handle( handle(mod_handle) );
	
	if (you == NULL) return;
	
	var orig_handle = mod_handle;
	
	// store original tile position for searching next entity in the same tile
	var tempx = you.XTILE_POS;
	var tempy = you.YTILE_POS;
	
	//close you-------------------------------------------
	you.z += 16;
	wait(2);
	you.z -= 16;
	wait(1);
	
	// set tile position, x-y may have changed
	you.XTILE_POS = XCoordToTile(you.x);
	you.YTILE_POS = YCoordToTile(you.y);
	you.ABSTILE_POS = XYTileToAbsTile(you.XTILE_POS, you.YTILE_POS);
		
	// set back to static
	you.emask &= ~DYNAMIC;	
		
	// NULL var pointers 
	mod_handle = 0;
		
	mod_x = NULL;
	mod_y = NULL;
	mod_z = NULL;
	
	mod_pan  = NULL;
	mod_tilt = NULL;
	mod_roll = NULL;

	mod_r = NULL;
	mod_g = NULL;
	mod_b = NULL;
	
	mod_alpha   = NULL;
	mod_ambient = NULL;
	
	mod_scale_x	= NULL;
	mod_scale_y	= NULL;
	mod_scale_z	= NULL;
	
	mod_ent_cat = 0;
	//---------------------------------------------	
	sys_marker("N00");
//	you = ent_next( ptr_for_handle( handle(mod_handle) ) );
//	mod_handle = 0;
	you = ent_next(you);
	sys_marker(NULL);
	sys_marker("N01");
	var temp = 0;
	while (you) // repeat until there are no more entities
		{ 
			if ( (you.XTILE_POS==tempx) && (you.YTILE_POS==tempy) )
				{
					if (you.ENT_CATEGORY == mod_origmenu)
						{
							temp = 1;
							break;
						}
				}
			you = ent_next(you); // get next entity
		}	
	sys_marker(NULL);	
	//---------------------------------------------------------
	sys_marker("N02");	
	// if not found restart from first entity
	if (!temp)
		{
			you = ent_next(NULL); // get first entity
		 	while (you) // repeat until there are no more entities
				{ 
					if ( (you.XTILE_POS==tempx) && (you.YTILE_POS==tempy) )
						{
							if (you.ENT_CATEGORY == mod_origmenu)
								{
									temp = 1;
									break;
								}
						}
					you = ent_next(you); // get next entity
				}	
		}	
	sys_marker(NULL);			
	//---------------------------------------------------------------------
	// reset if no valid entity
	if (!temp)
		{
			you = ptr_for_handle( handle(orig_handle) );
					
			return;
		}	
	//---------------------------------------------------------------------
	sys_marker("N03");
	ModifyObject_SetSelected();
	sys_marker(NULL);
}

//////////////////////////////////////
// set material

void		ModifyObject_SetMaterialForAll(ENTITY* ent)
{
	//---------------------------------------------------------------------------------------
	// set lods to similar
	
	STRING* temp_str = str_create("");
	int i;
	str_for_entfile(temp_str,ent);											// get file name
	
	if (str_stri(temp_str,"_0.")>0)											// is it a lod0 entity? - case sensitive !!!
		{			
			for(i=1;i<4;i++)														// cycle through LOD stages 1 2 3 
				{
					str_trunc(temp_str,5);										// remove 0.mdl/0.wmb from end
					str_cat(temp_str, str_for_num(NULL,(var)i));			// add 1-2-3
					
					if (mod_origmenu==(var)UIMODE_MAPENT)					// add .extension
						{
							str_cat(temp_str, ".wmb");	
						}
					else
						{
							str_cat(temp_str, ".mdl");						
						}
					
					ENTITY* temp_ent = ent_for_file(temp_str);			// get pointer to lod model
					if (temp_ent)													// check existance
						{
							ModifyObject_ChangeMaterial(ent, temp_ent);	// copy material
							wait_for(ModifyObject_ChangeMaterial);							
						}		
					temp_ent = NULL;
					ptr_remove(temp_ent);																
				}
		}	
	
	//---------------------------------------------------------------------------------------
	// set other instances to similar
	
	str_for_entfile(temp_str, ent);																		// get file name again
	ENTITY* temp_ent = ent_next(NULL); 																	// get first entity
	while(temp_ent)
		{
			if (str_cmpi(temp_str, str_for_entfile(NULL,temp_ent)))								// if having the same name
				{
					ModifyObject_ChangeMaterial(ent, temp_ent);										// copy material
						wait_for(ModifyObject_ChangeMaterial);	
				}
			
			temp_ent = ent_next(temp_ent); 																// get next entity
		}
	temp_ent = NULL;
	ptr_remove(temp_ent);	
	ptr_remove(temp_str);
}


// copy material from ent to temp_ent
void		ModifyObject_ChangeMaterial(ENTITY* ent, ENTITY* temp_ent)	
{	
//	temp_ent.ambient = ent.ambient;									// not required by lods		// MOVED
	temp_ent.alpha   = ent.alpha;										// not required by lods
	
	if (ent.material == mtl_tree)
		{
			reset(temp_ent, UNLIT);
			reset(temp_ent, TRANSLUCENT);
			Obstacle_SetTreeMaterial(temp_ent);						// special material assignments						
		}
	else
		{
			temp_ent.material = ent.material;						// assign entity material
			
			if (is(ent, UNLIT))											// required?
				{
					set(temp_ent, UNLIT);
				}
			else
				{
					reset(temp_ent, UNLIT);
				}
			
			if (is(ent, TRANSLUCENT))									// required?
				{
					set(temp_ent, TRANSLUCENT);
				}
			else
				{
					reset(temp_ent, TRANSLUCENT);
				}
			
			if (is(ent, OVERLAY))										// required?
				{
					set(temp_ent, OVERLAY);
				}
			else
				{
					reset(temp_ent, OVERLAY);
				}
			
			int j;
			for(j=0;j<ent_status(temp_ent,8);j++)
				{
					ent_mtlset(temp_ent, ent.material, j+1);		// assign skin material for correct effect assignment
				}
		}
}


void		ModifyObject_SetAmbientForAll(ENTITY* ent)
{
	STRING* temp_str = str_create("");
	str_for_entfile(temp_str, ent);																		// get file name again
	ENTITY* temp_ent = ent_next(NULL); 																	// get first entity
	while(temp_ent)
		{
			if (str_cmpi(temp_str, str_for_entfile(NULL,temp_ent)))								// if having the same name
				{
					temp_ent.ambient = ent.ambient;								
				}
			
			temp_ent = ent_next(temp_ent); 																// get next entity
		}
	temp_ent = NULL;
	ptr_remove(temp_ent);	
	ptr_remove(temp_str);
}

void		ModifyObject_SetScaleForAll(ENTITY* ent)
{
	STRING* temp_str = str_create("");
	str_for_entfile(temp_str, ent);																		// get file name again
	ENTITY* temp_ent = ent_next(NULL); 																	// get first entity
	while(temp_ent)
		{
			if (str_cmpi(temp_str, str_for_entfile(NULL,temp_ent)))								// if having the same name
				{
					vec_set(temp_ent.scale_x, ent.scale_x);
				}
			
			temp_ent = ent_next(temp_ent); 																// get next entity
		}
	temp_ent = NULL;
	ptr_remove(temp_ent);	
	ptr_remove(temp_str);
}

void		ModifyObject_SetRGBForAll(ENTITY* ent)
{
	STRING* temp_str = str_create("");
	str_for_entfile(temp_str, ent);																		// get file name again
	ENTITY* temp_ent = ent_next(NULL); 																	// get first entity
	while(temp_ent)
		{
			if (str_cmpi(temp_str, str_for_entfile(NULL,temp_ent)))								// if having the same name
				{
					if (is(ent, LIGHT))								
						{
							set(temp_ent, LIGHT);		
							vec_set(temp_ent.blue, ent.blue);
						}
					else
						{
							reset(temp_ent, LIGHT);			
						}
				}
			
			temp_ent = ent_next(temp_ent); 																// get next entity
		}
	temp_ent = NULL;
	ptr_remove(temp_ent);	
	ptr_remove(temp_str);
}


void		ModifyObject_SetLightmapForAll(ENTITY* ent)
{
	STRING* temp_str = str_create("");
	str_for_entfile(temp_str, ent);																		// get file name again
	ENTITY* temp_ent = ent_next(NULL); 																	// get first entity
	while(temp_ent)
		{
			if (str_cmpi(temp_str, str_for_entfile(NULL,temp_ent)))								// if having the same name
				{
					if (ent != temp_ent)
						{
							ModifyObject_UpdateShadows_ForAll(temp_ent);				
							// no wait_for is essential 										
						}
				}						
			
			temp_ent = ent_next(temp_ent); 																// get next entity
		}
	temp_ent = NULL;
	ptr_remove(temp_ent);	
	ptr_remove(temp_str);
}


void		ModifyObject_UpdateShadows_ForAll(ENTITY* temp_ent)
{
	// set to dynamic
	temp_ent.emask |= DYNAMIC;	
	wait(1);
	
	ModifyObject_UpdateShadows(temp_ent, mod_lightmap);
	wait_for(ModifyObject_UpdateShadows);
	
	// set back to static
	temp_ent.emask &= ~DYNAMIC;	
}

/////////////////////////////////////////////////////////////////////////////////////////////
// gizmo related


//////////////////////////////////////
// mode selection - panel calls

void		ModifyObject_ToggleMove()
{
	if (mod_type == (var)0)		return;
	
	mod_type = 0;
}


void		ModifyObject_ToggleRotate()
{
	if (mod_type == (var)1)		return;
	
	if (mod_modified_ent.ENT_TYPE == (var)2)	return;
	
	mod_type = 1;
}


void		ModifyObject_ToggleScale()
{
	if (mod_type == (var)2)		return;
	
	if (mod_modified_ent.ENT_TYPE == (var)2)	return;
	
	mod_type = 2;
}


//////////////////////////////////////
// gizmo usage detection

void		ModifyObject_Click()
{
	VECTOR pos1, pos2;
			
	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,5000) );     		
	vec_for_screen (pos2, camera);
	
	// ignore group 1 - collider entities, camera helper objects, water terrain
	// ignore group 2 - terrain models
	// ignore group 3 - obstacle models, map entities, building models
	// ignore group 4 - ground terrain
	c_ignore(1,2,3,4,0);				// grass models has no collision, thus no exclusion needed
	me = camera_cameraobject;		// its group value is 1 just like collider entities - ignorable by push - A7
		
	// rays an ellipsoid that collides with polygonal shape of models
	c_trace (pos1.x, pos2.x, IGNORE_SPRITES | USE_POLYGON | IGNORE_ME | USE_BOX); 	// (grass and shrub sprites cannot be modified) 	IGNORE_ME | IGNORE_PUSH | USE_BOX (box requires a me entity to be set)
			
	if (HIT_TARGET)
		{
			you = hit.entity;
			
			if (you.group != 30)
				{
					return;
				}
			
			if (mod_type == (var)0)	
				{
					if (proc_status(ModifyObject_Gizmo_Move) == (var)0)
						{
							ModifyObject_Gizmo_Move(you);			
						}
				}
			else if (mod_type == (var)1)	
				{
					if (proc_status(ModifyObject_Gizmo_Rotate) == (var)0)
						{
							ModifyObject_Gizmo_Rotate(you);			
						}
				}
			else if (mod_type == (var)2)	
				{
					if (proc_status(ModifyObject_Gizmo_Scale) == (var)0)
						{
							ModifyObject_Gizmo_Scale(you);			
						}
				}
		}
	else
		{
			return;
		}
}


//////////////////////////////////////
// initial gizmo parameters

//void		ModifyObject_Gizmo_Event()
//{	
//	if (event_type == EVENT_CLICK)
//		{
//			if (proc_status(ModifyObject_Gizmo_Move) < (var)1)
//				{
//					ModifyObject_Gizmo_Move(my);
//				}
//		}
//}

action	ModifyObject_Gizmo()
{
	set(my, PASSABLE | FLAG2 | FLAG1 | CAST | ZNEAR | UNLIT | POLYGON);
	my.material = mat_unlit;
	my.group = 30;
//	my.emask |= (ENABLE_TOUCH | ENABLE_CLICK);
//	my.event = ModifyObject_Gizmo_Event;
}

//////////////////////////////////////
// move by gizmo

void		ModifyObject_Gizmo_Move(ENTITY* ent)
{
	while(mouse_left)
		{
			if (ent == mod_gizmo_x_ent)
				{
					ent.x += (mickey.x + mickey.y)/2;
					
					vec_set(mod_gizmo_y_ent.x, ent.x);
					vec_set(mod_gizmo_z_ent.x, ent.x);
				}
			else if (ent == mod_gizmo_y_ent)
				{
					ent.y += (mickey.x + mickey.y)/2;
					
					vec_set(mod_gizmo_x_ent.x, ent.x);
					vec_set(mod_gizmo_z_ent.x, ent.x);
				}
			else if (ent == mod_gizmo_z_ent)
				{
					ent.z -= mickey.y;
					
					vec_set(mod_gizmo_x_ent.x, ent.x);
					vec_set(mod_gizmo_y_ent.x, ent.x);
				}					
			
			ENTITY* temp_ent = ptr_for_handle( handle(mod_handle) );
			
			// position
			vec_set(temp_ent.x, ent.x);			
			
			temp_ent = NULL;
			
			wait(1);
		}
		
	ModifyObject_Update_PanelXYZ();
}


// panel move parameter updates afetr gizmo manipulation
void		ModifyObject_Update_PanelXYZ()
{
	// sliders 1-2-3
	var min_offset;
	var max_offset;
	
	if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			min_offset = *mod_x - GetMapTileSize()*10;
			max_offset = *mod_x + GetMapTileSize()*10;
		}
	else
		{
			min_offset = *mod_x - GetMapTileSize()/2;
			max_offset = *mod_x + GetMapTileSize()/2;
		}
		mod_minoffset_x = min_offset;
		mod_maxoffset_x = max_offset;
	pan_setslider(mod_panel, 1,	90+12,52,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_x); 
	
	if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			min_offset = *mod_y - GetMapTileSize()*10;
			max_offset = *mod_y + GetMapTileSize()*10;
		}
	else
		{
			min_offset = *mod_y - GetMapTileSize()/2;
			max_offset = *mod_y + GetMapTileSize()/2;
		}
		mod_minoffset_y = min_offset;
		mod_maxoffset_y = max_offset;
	pan_setslider(mod_panel, 2,	90+12,72,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_y); 
	
	if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			min_offset = *mod_z - GetMapTileSize()*10;
			max_offset = *mod_z + GetMapTileSize()*10;
		}
	else
		{
			min_offset = *mod_z - GetMapTileSize()/2;
			max_offset = *mod_z + GetMapTileSize()/2;
		}
		mod_minoffset_z = min_offset;
		mod_maxoffset_z = max_offset;
	pan_setslider(mod_panel, 3,	90+12,92,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_z); 
}

//////////////////////////////////////
// rotate by gizmo    

void		ModifyObject_Gizmo_Rotate(ENTITY* ent)
{
	if (mod_modified_ent.ENT_TYPE == (var)2)	return;
	
	while(mouse_left)
		{
			if (ent == mod_gizmo_x_ent)
				{
					ent.roll += (mickey.x + mickey.y)/2;
					
					vec_set(mod_gizmo_y_ent.roll, ent.roll);
					vec_set(mod_gizmo_z_ent.roll, ent.roll);
				}
			else if (ent == mod_gizmo_y_ent)
				{
					ent.tilt += (mickey.x + mickey.y)/2;
					
					vec_set(mod_gizmo_x_ent.tilt, ent.tilt);
					vec_set(mod_gizmo_z_ent.tilt, ent.tilt);
				}
			else if (ent == mod_gizmo_z_ent)
				{
					ent.pan += (mickey.x + mickey.y)/2;
					
					vec_set(mod_gizmo_x_ent.pan, ent.pan);
					vec_set(mod_gizmo_y_ent.pan, ent.pan);
				}					
						
			ENTITY* temp_ent = ptr_for_handle( handle(mod_handle) );			
			
			// angle
			vec_set(temp_ent.pan, ent.pan);
			
			// -180..180
			*mod_pan  = ang(*mod_pan);
			*mod_tilt = ang(*mod_tilt);
			*mod_roll = ang(*mod_roll);
			
			temp_ent = NULL;
			
			wait(1);
		}
		
	ModifyObject_Update_PanelPTR();
}

void		ModifyObject_Update_PanelPTR()
{
	// sliders 4-5-6
	var min_offset = -180;
	var max_offset = 180;
	mod_minoffset_ptr = min_offset;
	mod_maxoffset_ptr = max_offset;
	pan_setslider(mod_panel, 4,	90+12,132,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_pan); 
	pan_setslider(mod_panel, 5,	90+12,152,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_tilt); 
	pan_setslider(mod_panel, 6,	90+12,172,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,		*mod_roll); 
}

//////////////////////////////////////
// scale by gizmo       		

void		ModifyObject_Gizmo_Scale(ENTITY* ent)  
{
	if (mod_modified_ent.ENT_TYPE == (var)2)	return;
	
	while(mouse_left)
		{
			ENTITY* temp_ent = ptr_for_handle( handle(mod_handle) );
			
			if (ent == mod_gizmo_x_ent)
				{
					temp_ent.scale_x += (mickey.x - mickey.y)/16;
				}
			else if (ent == mod_gizmo_y_ent)
				{
					temp_ent.scale_y += (mickey.x - mickey.y)/16;
				}
			else if (ent == mod_gizmo_z_ent)
				{
					temp_ent.scale_z += (mickey.x - mickey.y)/16;
				}								
			
			// updated in mod main loop
//			ModifyObject_Update_Scale(temp_ent);
//			wait_for(ModifyObject_Update_Scale);
			
			temp_ent = NULL;
			
			wait(1);
		}
		
	ModifyObject_Update_PanelSSS();	
}


void		ModifyObject_Update_PanelSSS()
{
	// sliders 9-10-11
	var min_offset;
	var max_offset;
	
	if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			min_offset = *mod_scale_x*0.1;
			max_offset = *mod_scale_x*5;
		}
	else
		{
			min_offset = *mod_scale_x*0.75;
			max_offset = *mod_scale_x*1.25;
		}
	mod_minoffset_sc = min_offset;
	mod_maxoffset_sc = max_offset;
	pan_setslider(mod_panel, 9,	90+12,212,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,	*mod_scale_x); 
	
	if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			min_offset = *mod_scale_y*0.1;
			max_offset = *mod_scale_y*5;
		}
	else
		{
			min_offset = *mod_scale_y*0.75;
			max_offset = *mod_scale_y*1.25;
		}
	mod_minoffset_sc = min_offset;
	mod_maxoffset_sc = max_offset;
	pan_setslider(mod_panel, 10,	90+12,232,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,	*mod_scale_y); 
	
	if (mod_origmenu==(var)UIMODE_TERRMDL)
		{
			min_offset = *mod_scale_z*0.1;
			max_offset = *mod_scale_z*5;
		}
	else
		{
			min_offset = *mod_scale_z*0.75;
			max_offset = *mod_scale_z*1.25;
		}
	mod_minoffset_sc = min_offset;
	mod_maxoffset_sc = max_offset;
	pan_setslider(mod_panel, 11,	90+12,252,	horiz_slider_bmp,slider_knob_bmp,	min_offset,max_offset,	*mod_scale_z); 	
}

//////////////////////////////////////



#endif