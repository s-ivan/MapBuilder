
#ifndef MBterrwaterhmp_c
#define MBterrwaterhmp_c


///////////////////////////////////////////////////////////////////////////////////////////////////


void		TerrHmp_CreateWater(char* WaterFileName)
{
	if ( (!str_cmpi(WaterFileName,"-")) && (file_exists_onepath(WaterFileName)) )
		{			
			detail_size 		= GetMapWaterDetail();
			
			water_entity 		= ent_create( WaterFileName , nullvector, TerrHmp_WaterAction);						
			while(!water_entity) { wait(1); }
		}
}


void		TerrHmp_RemoveWater()
{
	//------------------------------------------------------------------------------------
	// water entity
	
	if (water_entity != NULL)  
		{
			if (main_water > (var)0)
				{
					// remove created skins in reverse order
					if (ent_getskin(water_entity,4))
						{
							bmap_purge(ent_getskin(water_entity,4));
							ptr_remove(ent_getskin(water_entity,4));
						}
					if (ent_getskin(water_entity,3))
						{
							bmap_purge(ent_getskin(water_entity,3));
							ptr_remove(ent_getskin(water_entity,3));
						}					
					if (ent_getskin(water_entity,2))
						{
							bmap_purge(ent_getskin(water_entity,2));
							ptr_remove(ent_getskin(water_entity,2));
						}
					if (ent_getskin(water_entity,1))
						{
							bmap_purge(ent_getskin(water_entity,1));
							ptr_remove(ent_getskin(water_entity,1));
						}				
				}
			
			ent_purge(water_entity);		
			wait(1);	
				
			ptr_remove(water_entity);		
		}
	
	water_entity		= NULL;												// here, because of wmb support !!!
	
	//------------------------------------------------------------------------------------
	// water vertex array
	
	if (terrhmp_watervertices != NULL)
		{
			sys_free(terrhmp_watervertices);
			terrhmp_watervertices = NULL;
		}
	
	//------------------------------------------------------------------------------------
}


///////////////////////////////////////////////////////////////////////////////////////////////////


action	TerrHmp_WaterAction()
{
sys_marker("wat");
	if (!water_entity) water_entity = me;
	
	my.group = 1;																								// for teredit_lightmap ignore in A7
	
	my.ENT_CATEGORY = (var)UIMODE_WATERTERR; 
	set(my , PASSABLE);
	
//	reset(my, SHADOW);	
//	reset(my, CAST);		
//	reset(my, TRANSLUCENT);
	
	// scale water to terrain---------	
	if (!map_wmb)																								// in A8 wmb terrain scaling causes rendering error if vertices are moving by getvertex setvertex
	{
		if (!terrain_lod)
			{
				vec_for_vertex(terrhmp_vwatermax, my, 1);												// vertex 1 = upper-left corner - crash on terrain lod >0
			}
		else
			{
//				vec_set(terrhmp_vwatermax, my.max_x);													// becomes 0 - crash on scaling
				
				CONTACT* c = ent_getvertex(my, NULL, 1);
				vec_set(terrhmp_vwatermax, vector((var)c.v.x, (var)c.v.z, (var)c.v.y));		// works				
			}
		
		if (vec_length(terrhmp_vwatermax) == (var)0)													// zero protection and default scale good for included water terrains
			vec_set(terrhmp_vwatermax, vector(1024,1024,0));
			
		if ((vec_length(terrhmp_vwatermax) > 0) && (vec_length(terrhmp_vgroundmax) > 0))
			{
				my.scale_x = abs(TerrHmp_GetGroundMaxX() / terrhmp_vwatermax.x); 
				my.scale_y = abs(TerrHmp_GetGroundMaxY() / terrhmp_vwatermax.y);
			}
		
		if (terrain_lod)	// it sets max size fine, but scaling must be done in 1st frame
			{
				wait(1);	
				c_updatehull(me,0);
				
				vec_set(terrhmp_vwatermax, my.max_x);
			}
		
		if (!terrain_entity)
			{
				wait(1);
				
				// set level entity (a pseudo entity) size - water size is initially result in wrongly large level_ent size
				vec_set( level_ent.max_x , my.max_x );
				vec_set( level_ent.min_x , my.min_x );
				
				level_ent.max_z += 1000;	
			}
	}
	
	//--------------------------------
	var wave = 1;	
	
	// set material - needed only for created entity when no mirror shader applied
	if (main_water<0)
		{
			my.alpha = 95;													// it does not really work if placed inside material definition...
			if (!map_wmb)													// auto set by wmb, resetting would result in black water
				{	
					if ( ent_status(me,8)>2 )
						{
							my.material = water_multi_mat;
							wave = 0;										// do not uv shift texture (because does not work)
						}
					else
						{						
							my.material = water_single_mat;
						}
				}
			else if ( ent_status(me,8)<3 )							// for wmb to set non mirror water tiling
				{
					// set matrix for detail tiling - needed when lightmap applied - have to be set here
					mat_identity(water_single_mat.matrix);
					water_single_mat.matrix11 = GetMapWaterDetail();
					water_single_mat.matrix22 = GetMapWaterDetail();	
				}
		}
	wait(1);	
	
	//--------------------------------
	// water vertex array	
	
	int j;
	var anim_allowed = 0;
	
	if ((main_water<1) || (main_engineversion<1))
		{
			j = ent_status(my,0); // number of vertices 
			
			if (j <= 33*33)
				{
					anim_allowed = 1;
					
					if (terrhmp_watervertices == NULL)
						{
							terrhmp_watervertices = (var*)sys_malloc( j * sizeof(var) );
						}
					
					// randomize surface
					random_seed(0); 	
					for (; j>0; j--)
						{
							terrhmp_watervertices[j-1] = random(360);	
						}
				}
		}
	
	//--------------------------------------------
	// shader
	if (main_engineversion>0)							// material and effect is already attached, crash if wmb is used in this way, when differs from original
		{
//			//-------------------------------------------------	
//			if (!map_wmb)									// auto set by wmb, resetting would result in black water
//				{	
//					wait(1);	
//					my.material = water_single_mat;
//					wait(1);	
//				}
			//-------------------------------------------------	
			if (main_water==(var)0)
				{								
					reset(my,TRANSLUCENT);			// does not really matter but ensures alpha tweaking - ordering issues with alpha materials - requires OVERLAY flag to be used for 32b textured entities
					my.alpha = 100;					
					
					Mwater_Init(my);					// results in strange individual decal shadow behaviour in A7 and A8.40	- do not set NOSHADOW for any mirror VIEW to avoid it
					
					my.red 	= 80;
					my.green = 90;						
					my.blue 	= 90;
				}
			else if (main_water > (var)0)
				{						
					Scwater_Init(my);												//*** start Shade-C 0.91 water
					wait_for(Scwater_Init);
				}
			
			wait(1);	
		}
	//-------------------------------------------------------------------
	if ((main_water<1) || (main_engineversion<1))						// texture or mirror water, or free/extra edition
		{	
			// basic water animation
			j = ent_status(my,0); 			// number of vertices again
			
			var i = 0;
			int k = 0;
			
			while(me)																// does not start with (UIMode<(var)UIMODE_NEWMAP) && because on init hmp is created probably earlier
				{	
					if ((main_engineversion<1) || (main_water<0))		// free/extra edition, or texture water
						{		
							// transparency on close camera, intranspareny from far to avoid transparent entity sorting problems
							TerrHmp_WaterTransparency();	// no needed with water shader
							//---------------------------------------------------------
							if (wave)
								{
									// waves by texture shift
									my.u -= 0.05*time_step;
									my.v -= 0.05*cosv(i)*time_step;												
								}			
							i += 1;
							i %= 360;	
						}						
					//----------------------------------------------------------
					// waves by moving vertices up and down - causes rendering error in A8 WMB files !!!
					// modify all vertices in each frame	
					if ((anim_allowed) && ((main_water<1) || (main_engineversion<1)))		// mirror water or texture water , or free/extra edition
						{
							for (k=0; k<j; k++) 
								{
									// waving is fine for A8 and for MB maps
									// in A7 if compiled by WED, waves are too big thus should be divided by scale, 
									// if compiled by MB they are okay, but because of division they appear very small
									
									CONTACT* c = ent_getvertex(my,NULL,k+1);    							// vertex number begins with 1 ! i.e. 1..1089, while array is 0..1088
									if (!map_wmb)		// in A8 wmb terrain scaling causes rendering error if vertices are moving by getvertex setvertex
									{				
										c.v.y = sinv(i + terrhmp_watervertices[k]) * GetMapTileSize()/4;			// waving  *amplitude
									}
									else
									{
										c.v.y = sinv(i + terrhmp_watervertices[k]) * GetMapTileSize()/4;			// waving  *amplitude					
									}
									ent_setvertex(my,c,k+1); 
								}
						}
					// i += waterspeed * time_step;	// to adjust speed if needed
					
			//		c_setminmax(my);		// mirror water checks for max vertex, but better to keep at 0
					
					//-----------------------------------------------------------
					wait(1);
				}
		}
	else
		{
			wait(1);	
			my.emask &= ~DYNAMIC; // because needed by wmb_save() by Emre
		}
sys_marker(NULL);	
}


void		TerrHmp_WaterTransparency()
{
	var screen_size_x = sys_metrics(0); // SM_CXSCREEN
	var screen_size_y = sys_metrics(1); // SM_CYSCREEN	
	VECTOR pos1, pos2;
	vec_set( pos1 , vector(screen_size_x/2,screen_size_y/2,0) );
	vec_for_screen (pos1, camera);	
	vec_set( pos2 , vector(screen_size_x/2,screen_size_y/2,20000) ); // should be a big value       		
	vec_for_screen (pos2, camera);
	// measure terrain distance from camera fixed to screen middle
	c_trace (pos1.x, pos2.x, IGNORE_SPRITES | IGNORE_MODELS | IGNORE_MAPS ); 
	if (HIT_TARGET)  	
	{
		VECTOR vtemp;
		var temp = vec_dist(pos1,hit.x);
		
		if ((temp<GetMapTileSize()*100) && (camera.z<GetMapTileSize()*15))			// look at short distance, and camera is at low height
		{							
			//							var temp2 = (100-GetMapTileSize()) + temp/100;						// 100 to 68 due to cam distance from terrain
			var temp2 = (100 - GetMapTileSize()*15/10) + camera.z/10 ;		// limits determined by cam height and transparency border height
			//							clamp(temp2,75,100);
			my.alpha = maxv(temp2,75);							
			set( my , TRANSLUCENT );
		}
		else
		{
			reset( my , TRANSLUCENT );
		}
	}
	else
	{
		reset( my , TRANSLUCENT );
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// get / set


VECTOR*	TerrHmp_GetWaterMax()
{
	return terrhmp_vwatermax;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


#endif