#ifndef MBmwater_c
#define MBmwater_c


//////////////////////////////////////////////////////////////////////
// code


void	 Mwater_Init(ENTITY* ent)
{
	//------------------------------------------------------------------	
	
	my = ent;
	
	//------------------------------------------------------------------	
	
	my.material = mtl_mirrorWaterMB;											
	
	//------------------------------------------------------------------	
	
	if (mw_bmap_water_uv)
		{
			ptr_remove(mw_bmap_water_uv);												// *** correct mirror shift on multiple level loads
			mw_bmap_water_uv = NULL;
		}
	
//	mw_bmap_water_uv = bmap_to_uv(bmap_create("water.dds"));					// orig - results in mirror offset
//	mw_bmap_water_uv = bmap_to_normals(bmap_create("water.dds"), 50);		// *** - no mirror offset error !
   mw_bmap_water_uv = bmap_to_normals(bmap_for_entity(my,1), 50);			// *** use 1st skin, okay

   my.material.skin1 = mw_bmap_water_uv;
   my.material.flags |= TRANSLUCENT;
	
	//------------------------------------------------------------------	
	
	if (mw_bmap_mirrortarget) 
		{ 
	//		my.material.skin2 = mw_bmap_mirrortarget;
	//		return; 	
			ptr_remove(mw_bmap_mirrortarget);										// *** correct mirror shift on multiple level loads
			mw_bmap_mirrortarget = NULL;
		}
	
	//------------------------------------------------------------------	
	
	mw_bmap_mirrortarget = bmap_createblack(256,256,888);
	my.material.skin2 = mw_bmap_mirrortarget;
	mw_view_mirror.bmap = mw_bmap_mirrortarget;
	mw_view_mirror.size_x = bmap_width(mw_view_mirror.bmap);
	mw_view_mirror.size_y = bmap_height(mw_view_mirror.bmap);

	vec_set(mw_view_mirror.pnormal_x,vector(0,0,1.0));							// reflect upwards
	
	set(mw_view_mirror,NOCULL|PORTALCLIP);											// *** NOSHADOW|  - to get proper decal shadows - engine bug to be fixed
	
#ifdef ENGINE_BUG_DECAL_A840	
	if ((shadow_stencil < (var)0 ) || (shadow_stencil > (var)2))			// *** because of an engine bug below A8.45.1
#endif	
 		{
 			set(mw_view_mirror, NOSHADOW);											// ***			needed by pssm, not needed by decal/stencil shadows 
 		}
	
	set(mw_view_mirror,NOFLAG1);														// suppress all entities with flag1 set

	set(mw_view_mirror,NOPARTICLE);													// requird by PORTALCLIP
	set(mw_view_mirror,NOSHADER);														// requird by PORTALCLIP
	
	//------------------------------------------------------------------	
		
	while ((mw_bmap_mirrortarget) && (me))											// ¤¤¤
	{
		proc_mode = PROC_LATE;															// camera must be moved before
		
		if (!is(my,CLIPPED)) 
			{
				// place the mirror plane at the topmost entity polygon
				vec_for_max(mw_view_mirror.portal_x,my);							// without taking into account scale
//				vec_mul(view_mirror.portal_x, my.scale_x);						// *** multiply with scale
				vec_add(mw_view_mirror.portal_x,my.x);								// offset by position
//				view_mirror.portal_z = my.z;											// ***to avoid errors because of vertex movements
				mwater_mirrorvisible = 1;
			}
		
		if (mwater_mirrorvisible) 
			{ 
				set(mw_view_mirror,SHOW);
				mw_view_mirror.genius 		= camera.genius;
				mw_view_mirror.aspect 		= (screen_size.x / screen_size.y) * camera.aspect;		// screen aspect, independent of render target
				mw_view_mirror.arc    		= camera.arc;
				mw_view_mirror.fog_start 	= camera.fog_start;
				mw_view_mirror.fog_end   	= camera.fog_end;	
				mw_view_mirror.clip_far		= camera.clip_far * 0.5;
				mw_view_mirror.clip_near 	= camera.clip_near * 2;
				mw_view_mirror.x 	 		 	= camera.x;
				mw_view_mirror.y 	   		= camera.y;
				mw_view_mirror.z 	   		= 2 * mw_view_mirror.portal_z - camera.z;					// move the camera at its mirror position
				mw_view_mirror.pan    		= camera.pan;
				mw_view_mirror.tilt   		= -camera.tilt;													// flip the vertical camera angle	
				mw_view_mirror.roll   		= -camera.roll;
			}
		else 
			{ 
				// switch rendering off when all mirror objects are outside the frustum
				reset(mw_view_mirror, SHOW); 
			}
		
		// reset
		mwater_mirrorvisible = 0;

#ifdef MIRROR_DEBUG
		if (key_f11) 
			{	
				// debugging - make mirror visible onscreen
				mw_view_mirror.bmap = NULL;
			} 
		else 
			{
				mw_view_mirror.bmap = mw_bmap_mirrortarget;
			}	
#endif
		wait(1);
	}
	
	//------------------------------------------------------------------	
	
	// ¤¤¤
	ptr_remove(mw_bmap_mirrortarget);							// *** correct mirror shift on multiple level loads
	mw_bmap_mirrortarget = NULL;
	
	//------------------------------------------------------------------	
}


////////////////////////////////////////////////////////////////////////////////////////


void		Mwater_Save_MWater_smw(char* file_name)
{
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_write(temp_str);
	//---------------------------------------------
	if (file_handle)
		{
			file_var_write( file_handle , water_entity.red );
			file_var_write( file_handle , water_entity.green );
			file_var_write( file_handle , water_entity.blue );
			file_var_write( file_handle , water_entity.alpha );
			
			file_var_write( file_handle , mwater_speedx );
			file_var_write( file_handle , mwater_speedy );
			file_var_write( file_handle , mwater_ripple );
			file_var_write( file_handle , mwater_scale );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
			//------------------------------
			
			file_close(file_handle);
		}
	//----------------------------------------------
		
	ptr_remove(temp_str);
}


void		Mwater_Load_MWater_smw(char* file_name)
{
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_read(temp_str);
	//---------------------------------------------
	if (file_handle)
		{
			water_entity.red 			= file_var_read(file_handle); 
			water_entity.green 		= file_var_read(file_handle); 
			water_entity.blue 		= file_var_read(file_handle); 
			water_entity.alpha 		= file_var_read(file_handle); 
			
			mwater_speedx 	= file_var_read(file_handle); 
			mwater_speedy 	= file_var_read(file_handle); 
			mwater_ripple 	= file_var_read(file_handle); 
			mwater_scale 	= file_var_read(file_handle); 
			
			//------------------------------------
			
			file_close(file_handle);
		}
	//----------------------------------------------	
	
	if (water_entity.alpha > (var)99)
		{
			reset(water_entity, TRANSLUCENT);	
		}
	else
		{
			set(water_entity, TRANSLUCENT);	
		}
	
	//----------------------------------------------
	
	ptr_remove(temp_str);
}


////////////////////////////////////////////////////////////////////////////////////////


#endif