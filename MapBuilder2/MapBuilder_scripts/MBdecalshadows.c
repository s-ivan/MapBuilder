
#ifndef MBdecalshadows_c
#define MBdecalshadows_c

////////////////////////////////////////////////////////////////////////////////////////////////////////
// decal shadows

action	Decal_ShadowAction()
{
	set( my , INVISIBLE | PASSABLE | FLAG1 | FLAG2);
	my.ENT_CATEGORY = (var)UIMODE_SHADOWENT;
	my.XTILE_POS = 0;
	my.YTILE_POS = 0;
}

void		Decal_Update_DecalShadow(ENTITY* ent)
{
	ent.emask |= (DYNAMIC);	
	wait(1);
	
	reset(ent,SHADOW);
	wait(1);
	
	set(ent,SHADOW);
	
	wait(1);
	ent.emask &= ~DYNAMIC;
}

void		Decal_Assign_DecalShadow(ENTITY* ent, ENTITY* shad)
{
	ent.emask |= (DYNAMIC);	
	wait(1);
	
	reset(ent,SHADOW);
	wait(1);
	
	set(ent,SHADOW);
	ent.shadow = shad;
	
	wait(1);
	ent.emask &= ~DYNAMIC;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ent_decal shadows

// get the bitmap of shadow entity and create a decal particle as the shadow of the model entity onto the terrain surface
void		Decal_Set_Decal( ENTITY* model_ent , BMAP* shadow_bmp )
{
	wait(3);		// because of model scaling
	
	me = model_ent;
	
	c_trace(vector(my.x,my.y,my.z+GetMapTileSize()*10),vector(my.x,my.y,my.z-GetMapTileSize()*40), IGNORE_ME | IGNORE_CONTENT | IGNORE_PASSABLE | IGNORE_FLAG2 | IGNORE_SPRITES | SCAN_TEXTURE );	// | SCAN_TEXTURE needed !!!
	
	if (HIT_TARGET)
		{
			// model
			PARTICLE* shadow_p;
			if (ent_type(me)!=(var)6)
				{
					shadow_p = ent_decal(you,shadow_bmp,(my.max_x-my.min_x)*2,my.pan-your.pan);									// correct - due to manual
//					shadow_p = ent_decal(you,shadow_bmp,vec_length(vec_diff(NULL, my.max_x, me.min_x)),my.pan-your.pan);	// also fine
				}
			else
				{
					var spritesizex = my.scale_x * bmap_width( bmap_for_entity(me, 0) );
//					var spritesizey = my.scale_y * bmap_height( bmap_for_entity(me, 0) );
//					shadow_p = ent_decal(you,shadow_bmp,(abs(spritesizex)+abs(spritesizey)),my.pan-your.pan);	
					shadow_p = ent_decal(you,shadow_bmp,abs(spritesizex),my.pan-your.pan);											// correct
				}
			shadow_p.material = mtl_decalshadow; 																								// assign another material when contact material is not suitable for decals like terraintex.fx
			
			// set darkness, no mirror, no decal shadow detection (but NOPARTICLE set in water mirror parameters)
			set(shadow_p, TRANSLUCENT | FLAG1 | FLAG2);
			shadow_p.alpha = mat_shadow.alpha;						
			
			// info needed for deletion - done by setting lifespan to 0 after using ent_getdecal in cycles for the terrain and all terrain models !!!
//			shadow_p.skill_a = (var)UIMODE_OBSTACLE + (var)UIMODE_SHADOWSHIFT;		// ENT_CATEGORY
//			shadow_p.skill_b = my.XTILE_POS;														// XTILE_POS
//			shadow_p.skill_c = my.YTILE_POS;														// YTILE_POS
			shadow_p.skill_d = handle(my);														// ENTITY handle of shadow owner object - creator is to which it is attached - needed for deletion
																											// me = ptr_for_handle(shadow_p.skill_d);
//			shadow_p.vel_x = hit.x;																	// skill_x = me.x hit.x ent.x causes error on engine close
//			shadow_p.vel_y = hit.y;
//			shadow_p.vel_z = hit.z;																									
////			vec_set(shadow_p.skill_x, my.x);													// world position - error - skill_x/y/z used internally, must not be used !!!!
			
			// free up pointer
			shadow_p = NULL;
			ptr_remove(shadow_p);
		}
}

int		Decal_Remove_Decal_FromEnt(ENTITY* ent_attached, ENTITY* ent_owner)
{
	int i;	
	int j;
	// shadow on terrain
	var mesh_count = ent_status(ent_attached,16);
	
	for(j=0;j<mesh_count;j++)
		{
			i = 1;
			while(ent_getdecal(ent_attached , j , i))
				{
					PARTICLE* p = ent_getdecal(ent_attached , j , i);
					if (p.skill_d==(var)handle(ent_owner))
						{
							p.lifespan = 0;	// kill particle
							return(1);
						}
					i++;
				}
		}
		
	return(0);
}

void		Decal_Remove_Decal(ENTITY* ent_owner)
{
	if (terrain_entity != NULL)
		{
			// shadow on terrain
			if (Decal_Remove_Decal_FromEnt(terrain_entity, ent_owner) > 0)
				{
					return;
				}
		}
	
	// shadow on a terrain model - you is used in obstacle_remove !!!
	// me results in bad entity placement onto terrain entities after deletion some obstacles !!!
	// so another temp entity pointer is needed
	ENTITY* ent_temp = ent_next(NULL); // get first entity
 	while (ent_temp) // repeat until there are no more entities
		{ 
			if (ent_temp.ENT_CATEGORY == (var)UIMODE_TERRMDL)
				{
					if (Decal_Remove_Decal_FromEnt(ent_temp, ent_owner)>0)
						{
							return;
						}
				}
				
			ent_temp = ent_next(ent_temp); // get next entity
		}	
}

//typedef struct PARTICLE {																		// clipped at 50% of clip_far
//	C_LINK link;
//	var	lifespan;	// time until removal in ticks; set to 0 for removing 		********
//	var	x,y,z;		// world or skin position
//	var	vel_x,vel_y,vel_z;	// speed or beam vector										// can be used
//	var	size;		// in quants
//	BMAP	*bmap;		// particle image, or NULL
//	EVENT	event;		// particle function, executed every frame
//	long	flags;		
//	var	skill_x,skill_y,skill_z; // used for decals internally						******** error if modified
//	var	gravity;
//	var	angle;		
//	var	alpha;		// opacity 0..100
//	ENTITY	*creator;	// Entity that created the particle effect, or NULL
//	var	blue,green,red;
//	var	skill[4];	// more general purpose skills										******** skill_a,b,c,d
//   void*   d3dmesh;    // decal mesh
//   MATERIAL* material; // decal material
//} PARTICLE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void		Decal_Debug(VECTOR* pos_vec , BMAP* decal_bmp, var life)
{
	VECTOR from_vec, to_vec;
	
	vec_set(from_vec, pos_vec);
	from_vec.z += GetMapTileSize() * 2;
	
	vec_set(to_vec, pos_vec);
	to_vec.z -= GetMapTileSize();
	
	// ignore units
	c_ignore(30,0);
	
	c_trace(from_vec, to_vec, IGNORE_CONTENT | IGNORE_PASSABLE | IGNORE_FLAG2 | IGNORE_SPRITES | SCAN_TEXTURE);
	
	if (HIT_TARGET)
		{
			PARTICLE* decal_p;
			
			if ((ent_type(hit.entity) > (var)0) && (ent_type(hit.entity) < (var)6))
				{
					decal_p = ent_decal(hit.entity, decal_bmp, 32, 0);									
				}
			decal_p.material = mtl_sprite; 																								// assign another material when contact material is not suitable for decals like terraintex.fx
			
			// set darkness, no mirror, no decal shadow detection (but NOPARTICLE set in water mirror parameters)
			set(decal_p, TRANSLUCENT | FLAG1 | FLAG2);
//			decal_p.alpha = 75;	//mat_shadow.alpha;		// less visible				
			
			decal_p.lifespan = life;
			
			// free up pointer
			decal_p = NULL;
			ptr_remove(decal_p);
		}
}


#endif