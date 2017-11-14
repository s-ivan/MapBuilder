
#ifndef MBphysx_rts_c
#define MBphysx_rts_c


/////////////////////////////////////////////////////////////////////////////////////////////
// code


void		MBPhysX_Init()
{
	if(NxPhysicsSDK) return; 		// exit if runs
	
	NxPhysicsSDK = physX_load();	// load PhysX engine, get a pointer to it
	
	if(!NxPhysicsSDK) return;		// exit if fails
	
	NxScene = physX_run(0);			// get scene pointer
	
	pX_setsteprate(60,8,0);
	pX_setunit(1/40);
	
	// assign empty event functions if they are not set
	if(!on_exit) on_exit 				= on_exit_px0;
//	if(!on_level_load) on_level_load = on_level_load_px0;
	if(!on_ent_remove) on_ent_remove = on_ent_remove_px0;
	
	// store previous on_exit function
	on_exit_px 			= on_exit;
	on_exit 				= physX_close;
	
//	on_level_load_px 	= on_level_load;
//	on_level_load 		= physX_level_load;
	
	on_ent_remove_px 	= on_ent_remove;
	on_ent_remove 		= physX_ent_remove;
	
	pX_setgravity(vector(0,0,-9.81));	//?
	
	while(NxPhysicsSDK) 
		{
			proc_mode = PROC_EARLY;	// call physX_run before entity actions.
{}			
#ifdef FIXED_TIME
//			printf("FIXED_TIME");
			//This for-loop fixes the physX framerate to 60 when the frame rate is below 60 fps
			int loc_n = integer(time_step * 60 / 16) + 1;
			int loc_i = 0;
			if (freeze_mode <= 0)		
			for (; loc_i < loc_n; loc_i++)
				physX_run(0);
#else
			if (freeze_mode <= 0)		
				physX_run(time_frame/16);
#endif
			
			wait(1);
		}	
}


void		MBPhysX_Close()
{
	physX_destroy();
	NxScene 			= NULL;
	NxPhysicsSDK 	= NULL;
}


void		MBPhysX_UpdateEntities()
{
	if (!NxPhysicsSDK) return; 		// exit if not runs
	
	// collision groups - described in defines.h as
	// .group = 1		- water terrain, camera objects, collider entities (shrub, obstacle)
	// .group = 2		- terrain model
	// .group = 3		- obstacle model, map-entity
	// .group = 4		- ground terrain
	// .group = 30		- modify gizmo, rts unit
	pX_setgroupcollision ( 1, 1, 0);
	pX_setgroupcollision ( 2, 2, 0);
	pX_setgroupcollision ( 3, 3, 0);
	pX_setgroupcollision ( 4, 4, 0);
	pX_setgroupcollision ( 30, 30, 1);
	
	// set level blocks as polygon
	if (level_ent)
		pXent_settype(level_ent, PH_STATIC, PH_POLY);				
	
	// set entities
	for (you = ent_next(NULL); you; you = ent_next(you))
		{
			if (you.flags & PASSABLE) continue;							// increase for loop if entitiy is passable
			
			var type = ent_type(you);										// 0 invalid, 1 old block, 2-3 ABT-BPS block/mapent, 4 terrain, 5 model, 6 sprite
			
			if(type == 5 && (you.emask & DYNAMIC)) continue;		// dynamic models require special separate settings on their creation, like characters
			
			if(type < 2 && type > 5)  continue; 						// 0,1,6 not set, only: blocks, models, terrains
			
			if(type == 4)
				{
					pXent_settype(you, PH_STATIC, PH_TERRAIN);		// terrain
					
					// group			
					var temp_group = you.group;
					pXent_setgroup(you, temp_group);	
				}
			else
				{
					if (is(you, POLYGON))									// polygon collision entities including collider entitites of shrubs (and obstacles with decal shadows)
						{				
							pXent_settype(you, PH_STATIC, PH_POLY);	// polygon (models) - very expensive for e.g. tree models without simple collider models!!!	
							
							// group			
							var temp_group = you.group;
							pXent_setgroup(you, temp_group);	
						}
					else
						{
							// for tile area collision, set for a frame the DYNAMIC flag
							// scale_xy models (and mapents?) to tile area size stered by min_xy and max_xy,
							// keep scale_z, set to PH_BOX, then scale back
							// and reset DYNAMIC flag
							
							
						}												
				}												
		}
}








#endif