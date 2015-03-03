
#ifndef MBdecalshadows_h
#define MBdecalshadows_h

////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions

// decal shadow related
action	Decal_ShadowAction();
void		Decal_Update_DecalShadow(ENTITY* ent);
void		Decal_Assign_DecalShadow(ENTITY* ent, ENTITY* shad);

// ent_decal related
void		Decal_Set_Decal( ENTITY* model_ent , BMAP* shadow_bmp );
void		Decal_Remove_Decal(ENTITY* ent_owner);
	int		Decal_Remove_Decal_FromEnt(ENTITY* ent_attached, ENTITY* ent_owner);

// ent_decal for debugging
void		Decal_Debug(VECTOR* pos_vec, BMAP* decal_bmp, var life);

#endif