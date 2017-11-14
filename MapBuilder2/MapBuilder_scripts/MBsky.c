
#ifndef MBsky_c
#define MBsky_c

////////////////////////////////////////////////////////
// sky system


void		Sky_DefaultInit()
{
sys_marker("ski");	
	// sky layers 	------------------------------------------------------
	// char* filenames are needed - and terr file should be in exe dir
	sky_domeorcube = 0;	
	
	//	if (skyModelName)				// must have a 32b TGA internal skin - fog on sky - bigger clipping range is needed to not to cut off sky model
	//		{
		//			sky_domeorcube = 2;
		//			skyModel = ent_createlayer( skyModelName , SKY | SCENE | SHOW , 0 );	
		//			var actualmapsize = abs(TerrHmp_GetGroundMaxX())*2;
		//			vec_scale( skyModel.scale_x , actualmapsize/1000 );
		//			skyModel.z = 1024;	//512+128  +128;
		//			set(skyModel,TRANSLUCENT);
		//			skyModel.alpha = 100;
		//			str_cpy(skyModelSkin,skyDefaultModelSkin);
		//			skyModel.material = mat_sky;
		//			wait(3);
	//		}
	
	// fucks transparent water uv map in A8 - A8.30.5 corrected !!!
	if (skyCubeName)				// must be 32b TGA or DDS DTX3 - no fog but transparency can simulate it - skybox model is better maybe...
		if (!skyCube)	
			{
				sky_domeorcube = 1;
				skyCube = ent_createlayer( skyCubeName , SKY | CUBE | SHOW , 0 );	
				skyCube.z = -16;
				set(skyCube,TRANSLUCENT);
				skyCube.alpha = 90;
				skyCube.material = mat_sky;
				wait(3);
			}
	
	//	if (skyCylinderName)
	//			{
		//				sky_domeorcube = 1;
		//				skyCylinder = ent_createlayer( skyCylinderName , SKY | CYLINDER | SHOW , 1 );	
		//				skyCylinder.scale_x = 0.15;		// 1 = put once around horizont
		//				skyCylinder.pan = 0;					// can be rotated
		//				skyCylinder.tilt = -17.5;			// vertical position 
		//				set(skyCylinder,TRANSLUCENT);
		//				skyCylinder.alpha = 45;
		//				skyCylinder.material = mat_sky;
		//				wait(3);
	//	 		}	
	
	//	if (skyClouds1Name)
	//		{	
		//			skyClouds1 = ent_createlayer( skyClouds1Name , SKY | DOME | SHOW , 2 );
		//			skyClouds1.scale_x = 2;		
		//			skyClouds1.u = 0.5;
		//			skyClouds1.v = 0.7;
		//			set(skyClouds1,TRANSLUCENT);
		//			skyClouds1.alpha = 90;	
		//			skyClouds1.material = mat_sky;
		//			wait(3);	
	//		}
	
	//	if (skyClouds2Name)
	//		{
		//			skyClouds2 = ent_createlayer( skyClouds2Name , SKY | DOME | SHOW , 3 );	
		//			skyClouds2.scale_x = 1.5;	
		//			skyClouds2.u = 2;
		//			skyClouds2.v = 3;
		//			set(skyClouds2,TRANSLUCENT);
		//			skyClouds2.alpha = 95;	
		//			skyClouds2.material = mat_sky;
		//			wait(3);
	//		}	
	
	// sky entity dependent settings-------------------------------------------
	
	if (!map_wmb)
		{
			// sun color, direction, height
			vec_set(sun_color,vector(110,125,145));
		//	vec_fill(ambient_color , (var)SKY_DEFAULTAMBIENT*2.55);		// 64,64,64 = camera.ambient = 25;		
			sun_angle.pan = 315;
			sun_angle.tilt = 50;									// 45..60°
		}
	
	// sky color	   
	vec_set(sky_color,vector(244,207,181));		// = fogcolor2 , light: 255,200,175 , darker: 225,175,125
	sky_curve = 1.5;										// 1.5...2 is okay
	sky_clip = 17.5;										// min.15, but 20 is okay always
	sky_blend = 1;											// fade out around bottom edge of sky      
	
	// sky light color
	sky_skylightcolor[0] = sky_color.red / 255;
	sky_skylightcolor[1] = sky_color.green / 255;
	sky_skylightcolor[2] = sky_color.blue / 255;
	
	if (!map_wmb)
		{
			// fog settings : 1 sunrise, 2 daytime, 3 sunset, 4 night
			vec_set(d3d_fogcolor1,vector(103,096,101));		// light: 255,225,200 , light blue: 225,200,175
			vec_set(d3d_fogcolor2,vector(244,207,181));
			vec_set(d3d_fogcolor3,vector(083,060,064));
			vec_set(d3d_fogcolor4,vector(049,037,036));
			// 5 is black needed for night darkness, 0 means no fog
			fog_color = 2; 								// from wmb info file
		}
	
	// camera  arc	
	camera.arc = 60;													// 60..90 hasznos,de a 120 is érdekes még.... - de a sky görbületen állítani kell ha növeljük...
	if (!map_wmb)
		{
			camera.ambient = (var)SKY_DEFAULTAMBIENT;			// better if lower a bit than 50.. e.g. 25
		}
	
	// clipping distance
	camera.clip_near  = 16;											// close clipping to avoid terrain clipping of uneven terrains
	if (sky_domeorcube == (var)0)									// dome
	{
		camera.clip_far   = 15000;									// *0.75 - 5500 min. for sky dome only
		camera.fog_start 	= sky_loddistance1; 					// 0.6
		camera.fog_end 	= camera.clip_far*0.71;	 			// 1-1.1
	}
	else if (sky_domeorcube == (var)1)							// cylinder or cube
	{
		camera.clip_far   = 15000;									// *1 - distance between corner and middle - 7500 needed if sky cylinder used 
		camera.fog_start 	= sky_loddistance1; 					// 0.6
		camera.fog_end 	= camera.clip_far*0.71;	 			// 1-1.1
	} 
	else //if (sky_domeorcube == (var)2)						// cube model
	{
//		camera.clip_far   = vec_length (terrhmp_vgroundmin)*2 +10;	// *2 +10 - 15000-30000. scale to a bit higher than the diagonal distance of the actual map,    																		
		camera.clip_far   = 25000;	
		camera.fog_start 	= sky_loddistance1; 					// 6000.
		camera.fog_end 	= camera.clip_far*0.75;	 			// 10000.
	}  
	
	//--------------------------------------------------------------------------
	// lod settings - distances are given in quants, but should be given relatively to cameracip far, in percentage, which can be different
	var temp1 = sky_loddistance1*100;
	var temp2 = sky_loddistance2*100;
	var temp3 = sky_loddistance3*100;
	vec_set(d3d_lodfactor,vector( temp1/camera.clip_far , temp2/camera.clip_far , temp3/camera.clip_far )); 		// 10,20,50. % values of view.clip_far, default: 12.5 , 25 , 50
	
	clip_particles = d3d_lodfactor.y;

	// dynamic shadows lod settings - should be given relatively to camera clip far, in percentage
	shadow_range = 100;	//	temp3/camera.clip_far;	// unfortunately lod0 dependent
	
sys_marker(NULL);	
}



void	Sky_RemoveSkies()
{
	// sky layer entities
	
	if (skyClouds1!=NULL)  
	{
		ptr_remove(skyClouds1);
		skyClouds1		= NULL;
	}
	if (skyClouds2!=NULL)  
	{
		ptr_remove(skyClouds2);
		skyClouds2		= NULL;
	}
	if (skyCube!=NULL)     
	{
		ptr_remove(skyCube);
		skyCube			= NULL;
	}
	if (skyCylinder!=NULL) 
	{
		ptr_remove(skyCylinder);
		skyCylinder		= NULL;
	}
	if (skyModel!=NULL)     
	{
		ptr_remove(skyModel);
		skyModel			= NULL;
	}
	//	printf( "destroy: sky done" );		
}


////////////////////////////////////////////////////////////////////


void 		Sky_Save_Sky_sky(char* file_name)
{
{}
#ifndef MB_GAME	
	// save panel
	set( map_loadpanel , SHOW);
	set( map_savetext1 , SHOW);
	wait(1);
#endif	
	
	var temp_var = 0;
	STRING* temp_str = str_create("");
	str_cpy(temp_str,file_name);
	//---------------------------------------------
	var file_handle = file_open_write(temp_str);
	//---------------------------------------------
	if (file_handle)
		{
			//------------------	
			// sky model
			if (skyModel)
				{
					// model name
					str_for_entfile( 		temp_str 	, skyModel );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// alternative skin name					
					file_str_write( 		file_handle , skyModelSkin );			// stores without path
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					// model scale_x - better if  auto calculated on load - in the other case it is map size specific
					file_var_write( file_handle , skyModel.scale_x );					
					// model z
					file_var_write( file_handle , skyModel.z );	
					// model alpha
					file_var_write( file_handle , skyModel.alpha );	
				}
			else
				{
					file_str_write( 		file_handle , "-" );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					file_str_write( 		file_handle , "-" );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
				}
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
			//------------------	
			// sky cube
			if (skyCube)
				{
					// cube name
					str_for_entfile( 		temp_str 	, skyCube );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);					
					// cube z
					file_var_write( file_handle , skyCube.z );	
					// cube alpha
					file_var_write( file_handle , skyCube.alpha );	
				}
			else
				{
					file_str_write( 		file_handle , "-" );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
				}
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
					
			//------------------	
			// sky cylinder
			if (skyCylinder)
				{
					// cylinder name
					str_for_entfile( 		temp_str 	, skyCylinder );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);					
					// cylinder scale_x
					file_var_write( file_handle , skyCylinder.scale_x );
					// cylinder pan	
					file_var_write( file_handle , skyCylinder.pan );	
					// cylinder tilt
					file_var_write( file_handle , skyCylinder.tilt );	
					// cylinder alpha
					file_var_write( file_handle , skyCylinder.alpha );	
				}
			else
				{
					file_str_write( 		file_handle , "-" );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
				}
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
			
			//------------------	
			// sky dome-1
			if (skyClouds1)
				{
					// dome-1 name
					str_for_entfile( 		temp_str 	, skyClouds1 );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);					
					// dome-1 scale_x
					file_var_write( file_handle , skyClouds1.scale_x );
					// dome-1 u	
					file_var_write( file_handle , skyClouds1.u );	
					// dome-1 v
					file_var_write( file_handle , skyClouds1.v );	
					// dome-1 alpha
					file_var_write( file_handle , skyClouds1.alpha );	
				}
			else
				{
					file_str_write( 		file_handle , "-" );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
				}
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
			
			//------------------
			// sky dome-2
			if (skyClouds2)
				{
					// dome-1 name
					str_for_entfile( 		temp_str 	, skyClouds2 );
					file_str_write( 		file_handle , temp_str );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);					
					// dome-1 scale_x
					file_var_write( file_handle , skyClouds2.scale_x );
					// dome-1 u	
					file_var_write( file_handle , skyClouds2.u );	
					// dome-1 v
					file_var_write( file_handle , skyClouds2.v );	
					// dome-1 alpha
					file_var_write( file_handle , skyClouds2.alpha );	
				}
			else
				{
					file_str_write( 		file_handle , "-" );			
						file_asc_write(	file_handle , 13); 
						file_asc_write(	file_handle , 10);
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
					file_var_write( file_handle , 0 );
				}
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
			
			//------------------	
			// sun_color.red
			file_var_write( file_handle , sun_color.red );
			// sun_color.green
			file_var_write( file_handle , sun_color.green );
			// sun_color.blue
			file_var_write( file_handle , sun_color.blue );
			// sun_angle.pan
			file_var_write( file_handle , sun_angle.pan );
			// sun_angle.tilt
			file_var_write( file_handle , sun_angle.tilt );
			// sun_light
			file_var_write( file_handle , sun_light );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
			//------------------	
			// sky_color.red
			file_var_write( file_handle , sky_color.red );
			// sky_color.green
			file_var_write( file_handle , sky_color.green );
			// sky_color.blue
			file_var_write( file_handle , sky_color.blue );
			// sky_curve
			file_var_write( file_handle , sky_curve );
			// sky_clip
			file_var_write( file_handle , sky_clip );
			// sky_blend			
			file_var_write( file_handle , sky_blend );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
			//------------------	
			// d3d_fogcolor1.red
			file_var_write( file_handle , d3d_fogcolor1.red );
			// d3d_fogcolor1.green
			file_var_write( file_handle , d3d_fogcolor1.green );
			// d3d_fogcolor1.blue
			file_var_write( file_handle , d3d_fogcolor1.blue );
			// d3d_fogcolor2.red
			file_var_write( file_handle , d3d_fogcolor2.red );
			// d3d_fogcolor2.green
			file_var_write( file_handle , d3d_fogcolor2.green );
			// d3d_fogcolor2.blue
			file_var_write( file_handle , d3d_fogcolor2.blue );
			// d3d_fogcolor3.red
			file_var_write( file_handle , d3d_fogcolor3.red );
			// d3d_fogcolor3.green
			file_var_write( file_handle , d3d_fogcolor3.green );
			// d3d_fogcolor3.blue
			file_var_write( file_handle , d3d_fogcolor3.blue );
			// d3d_fogcolor4.red
			file_var_write( file_handle , d3d_fogcolor4.red );
			// d3d_fogcolor4.green
			file_var_write( file_handle , d3d_fogcolor4.green );
			// d3d_fogcolor4.blue
			file_var_write( file_handle , d3d_fogcolor4.blue );
			// fog_color
			file_var_write( file_handle , fog_color );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
			//------------------		
			// camera.arc
			file_var_write( file_handle , camera.arc );
			// camera.ambient
			file_var_write( file_handle , camera.ambient );
			// camera.clip_near
			file_var_write( file_handle , camera.clip_near );
			// camera.clip_far
			file_var_write( file_handle , camera.clip_far );
			// camera.fog_start
			file_var_write( file_handle , camera.fog_start );
			// camera.fog_end 
			file_var_write( file_handle , camera.fog_end );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
			//------------------	
			// d3d_lodfactor 1 -> sky_loddistance1
			file_var_write( file_handle , sky_loddistance1 );
			// d3d_lodfactor 2 -> sky_loddistance2
			file_var_write( file_handle , sky_loddistance2 );
			// d3d_lodfactor 3 -> sky_loddistance3
			file_var_write( file_handle , sky_loddistance3 );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);	
			//------------------	
			// shadow_range
			file_var_write( file_handle , shadow_range );
			// mat_shadow.alpha
			file_var_write( file_handle , mat_shadow.alpha );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
//			////////////////////////////////////////////////////////////////////// TO BE REMOVED
//			//------------------
//			// pssm_res
//			file_var_write( file_handle , pssm_res );
//			// pssm_splitweight
//			file_var_write( file_handle , pssm_splitweight );
//			// pssm_transparency
//			file_var_write( file_handle , pssm_transparency );
//			// pssm_fbias * 1000
//			file_var_write( file_handle , pssm_bias );				// instead of pssm_fbias*1000, thus compatible with old values
//			// pssm_limit
//			if (pssm_limit > (var)0)
//				{
//					file_var_write( file_handle , pssm_limit );
//				}
//			else
//				{
//					pssm_limit = pssm_numsplits;
//					pssm_limit = clamp(pssm_limit, 1, 4);
//					file_var_write( file_handle , pssm_limit );		// default value - not essential
//				}
//			// pssm_numsplits
//			if (pssm_numsplits > (var)0)
//				{
//					file_var_write( file_handle , pssm_numsplits );
//				}
//			else
//				{
//					pssm_numsplits = pssm_limit;							// probably was set just before
//					file_var_write( file_handle , pssm_numsplits );	// default value - required when saving from scsm?
//				}
//			// pssm_max
//			file_var_write( file_handle , pssm_max );
//			// pssm_fadeout_perc
//			file_var_write( file_handle , pssm_fadeout_perc );
//			// pssm_manual
//			file_var_write( file_handle , pssm_manual );
//			// pssm_dist1
//			file_var_write( file_handle , pssm_dist1 );
//			// pssm_dist2
//			file_var_write( file_handle , pssm_dist2 );
//			// pssm_dist3
//			file_var_write( file_handle , pssm_dist3 );
//			// pssm_dist4
//			file_var_write( file_handle , pssm_dist4 );
//			// pssm_shadowweight
//			file_var_write( file_handle , pssm_shadowweight );
//			// pssm_lod
//			file_var_write( file_handle , pssm_lod+1 );						// because of backward compatibility of loading (-1 applied)
//			// pssm_splitmode
//			file_var_write( file_handle , pssm_splitmode );
//			
//			file_asc_write(	file_handle , 13); 
//			file_asc_write(	file_handle , 10);	
//			
//			////////////////////////////////////////////////////////////////////// TO BE REMOVED
//			//-----------------
//			// scsm_maxdepth
//			file_var_write( file_handle , scsm_maxdepth );
//			// scsm_blurpixel
//			file_var_write( file_handle , scsm_blurpixel );
//			// scsm_blurtype
//			file_var_write( file_handle , scsm_blurtype );
//			// scsm_vsm_epsilon
//			file_var_write( file_handle , 0 );									// scsm_vsm_epsilon - obsolete
//			// scsm_vsm_epsilon_obj
//			file_var_write( file_handle , 0 );									// scsm_vsm_epsilon_obj - obsolete
//			// scsm_shadowalpha
//			file_var_write( file_handle , scsm_shadowalpha );
//			// scsm_lightmapalpha
//			file_var_write( file_handle , scsm_lightmapalpha );
//			// scsm_resolution
//			file_var_write( file_handle , scsm_resolution );
//			// scsm_areasize
//			file_var_write( file_handle , scsm_areasize );
//			//
//			// scsm_esm
//			file_var_write( file_handle , scsm_esm );							// new
//			// scsm_esm_obj
//			file_var_write( file_handle , scsm_esm_obj );					// new
//			// scsm_shadowpixeloffset
//			file_var_write( file_handle , scsm_shadowpixeloffset );		// new
//			
//			file_asc_write(	file_handle , 13); 
//			file_asc_write(	file_handle , 10);	
//			
//			////////////////////////////////////////////////////////////////////// TO BE REMOVED
//			//-----------------
//			// pssm_esm
//			file_var_write( file_handle , pssm_esm );							// new
//			// pssm_blurtype
//			file_var_write( file_handle , pssm_blurtype );					// new
//			// pssm_blurfactor
//			file_var_write( file_handle , pssm_blurfactor );				// new
//			// pssm_fogfactor
//			file_var_write( file_handle , pssm_fogfactor );					// new
//			
//			file_asc_write(	file_handle , 13); 
//			file_asc_write(	file_handle , 10);
//			
//			////////////////////////////////////////////////////////////////////// TO BE REMOVED
//			//----------------
//			// scsm_lod
//			file_var_write( file_handle , scsm_lod+1 );						// new new		- saved as pssm_lod because of eof check , thus default value is -1 even if file reading because of eof results in 0
//			
//			file_asc_write(	file_handle , 13); 
//			file_asc_write(	file_handle , 10);
//			
			////////////////////////////////////////////////////////////////////// TO BE KEPT
			//-----------------
			// version check
			
			file_var_write( file_handle , 2.48 );								// version control (version at the time of the introduction of the new format)
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
			//-----------------
			// height fog
			file_var_write( file_handle , 1 );									// version control (must be 1)
			
			file_var_write( file_handle , sky_hfogminheight );
			file_var_write( file_handle , sky_hfogmaxheight );
			file_var_write( file_handle , sky_hfogheightstrength );
			file_var_write( file_handle , sky_hfogheightblending );
			file_var_write( file_handle , sky_hfogheightdensity );
			file_var_write( file_handle , sky_hfogheightmin );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
			////////////////////////////////////////////////////////////////////// TO BE KEPT
			//-----------------
			// sky light
			file_var_write( file_handle , 1 );									// version control (must be 1)
			
			file_var_write( file_handle , sky_skylightstart );
			file_var_write( file_handle , sky_skylightend );
			file_var_write( file_handle , sky_skylightheight );
			
			file_asc_write(	file_handle , 13); 
			file_asc_write(	file_handle , 10);
			
//			////////////////////////////////////////////////////////////////////// TO BE REMOVED
//			//-----------------
//			// scsm fade-out
//			
//			file_var_write( file_handle , scsm_fadeout_start );
//			
//			file_asc_write(	file_handle , 13); 
//			file_asc_write(	file_handle , 10);
//			
//			//-----------------
			//////////////////////////////////////////////////////////////////////
			
			file_close(file_handle);
		}
	
//	printf( "Sky Settings Saved: %s" , _chr(file_name) );
	
#ifndef MB_GAME	
	// save panel
	reset( map_loadpanel , SHOW);
	reset( map_savetext1 , SHOW);
#endif	
	
	ptr_remove(temp_str);
}


////////////////////////////////////////////////////////////////////


void		Sky_Load_Sky_sky(char* file_name)
{
{}
#ifndef MB_GAME
	// load panel
	set( map_loadpanel , SHOW);
	set( map_loadtext1 , SHOW);
	wait(3);
#endif	// MB_GAME
		
	STRING* temp_str = str_create("");
	STRING* temp_str2 = str_create("");
	str_cpy(temp_str,file_name);
	var temp;
	
	var shadowmapping_loaded = 0;																		// for backward compatibility
	
	//---------------------------------------------		TO BE REMOVED
	// pssm runs or not
//	var 	pssm_state 					= 0;
	// original pssm parameters
	var 	temp_pssm_res 				= pssm_res;
	var 	temp_pssm_splitweight 	= pssm_splitweight;
	var 	temp_pssm_transparency 	= pssm_transparency;
	var	temp_pssm_bias 			= pssm_bias;
	var 	temp_pssm_limit 			= pssm_limit;
	var 	temp_pssm_numsplits 		= pssm_numsplits;
	// new pssm parameters
	var	temp_pssm_max 				= pssm_max;
	var	temp_pssm_fadeout_perc 	= pssm_fadeout_perc;
	var	temp_pssm_manual 			= pssm_manual;
	var	temp_pssm_dist1 			= pssm_dist1;
	var	temp_pssm_dist2 			= pssm_dist2;
	var	temp_pssm_dist3 			= pssm_dist3;
	var	temp_pssm_dist4 			= pssm_dist4;
	var	temp_pssm_shadowweight 	= pssm_shadowweight;
	var	temp_pssm_lod 				= pssm_lod;
	var	temp_pssm_splitmode 		= pssm_splitmode;
	// new ESM parameters
	var	temp_pssm_esm		 		= pssm_esm;
	var	temp_pssm_blurtype 		= pssm_blurtype;
	var	temp_pssm_blurfactor		= pssm_blurfactor;
	var	temp_pssm_fogfactor 		= pssm_fogfactor;
	
	//---------------------------------------------		TO BE REMOVED
	// shade-c shadowmapping parameters
	var 	temp_scsm_maxdepth			= scsm_maxdepth;
	var 	temp_scsm_resolution 		= scsm_resolution;
	var 	temp_scsm_areasize			= scsm_areasize;
	//
	var 	temp_scsm_blurpixel			= scsm_blurpixel;
	var 	temp_scsm_blurtype			= scsm_blurtype;
	var 	temp_scsm_vsm_epsilon		= 0;									// scsm_vsm_epsilon - obsolete
	var 	temp_scsm_vsm_epsilon_obj	= 0;									// scsm_vsm_epsilon_obj - obsolete
	var 	temp_scsm_shadowalpha		= scsm_shadowalpha;
	var 	temp_scsm_lightmapalpha		= scsm_lightmapalpha;
	
	var 	temp_scsm_esm					= scsm_esm;							// new
	var 	temp_scsm_esm_obj				= scsm_esm_obj;					// new
	var 	temp_scsm_shadowpixeloffset= scsm_shadowpixeloffset;		// new
	
	var 	temp_scsm_lod					= scsm_lod;							// new new
	
	//---------------------------------------------
	var file_handle = file_open_read(temp_str);	
	//---------------------------------------------
	if (file_handle)
		{			
			//------------------
			// sky entities:			
			sky_domeorcube = 0;
			
			// always remove model or it does not work properly..... but on skin change it is okay....
			ptr_remove(skyModel);
			skyModel = NULL;
			
			//------------------	
			// model name
			file_str_read( file_handle , temp_str );
			// alternative skin name - stored without path, or "-" if default sky model is used
			file_str_read( file_handle , skyModelSkin );	
			// is not needed - delete if exists
			if ( str_cmpi(temp_str,"-") )
				{
					if (skyModel)
						{
							ptr_remove(skyModel);
							skyModel = NULL;
						}
					// model scale_x
					temp = file_var_read(file_handle); 
					// model z
					temp = file_var_read(file_handle); 
					// model alpha
					temp = file_var_read(file_handle); 
				}
			// if needed - change skin if exists, or create a model (and change skin if needed) if does not exist
			else
				{					
					sky_domeorcube = 2;		
					if (!skyModel)
						{								
							str_cpy(temp_str2,sky_foldername);
							str_cat(temp_str2,temp_str);
							skyModel = ent_createlayer( temp_str2 , SKY | SCENE | SHOW , 0 );	
							set(skyModel,TRANSLUCENT);
							skyModel.material = mat_sky;
						}		
					// model scale_x - better to autogenerate based on actual ground terrain sizes, thus it is not map size specific
					var temp = file_var_read(file_handle); // skyModel.scale_x = 					
					var actualmapsize = abs(TerrHmp_GetGroundMaxX()) * 2;
					vec_scale( skyModel.scale_x , actualmapsize / 1000 );
					skyModel.scale_y = skyModel.scale_x; 				// needed when removed and recreated only
					skyModel.scale_z = skyModel.scale_x; 				// needed when removed and recreated only
					// model z
					skyModel.z = file_var_read(file_handle);  
					// model alpha
					skyModel.alpha = file_var_read(file_handle); 													
					if ( !str_cmpi( skyModelSkin , "-" ) )				// if there is a model it must exist - default or custom skin
						{				
							str_cpy(temp_str2,sky_foldername);
							str_cat(temp_str2,skyModelSkin);				
							BMAP* temp_bmp = bmap_create( temp_str2 );
							ptr_remove( ent_getskin( skyModel , 1 ) );
							ent_setskin( skyModel , temp_bmp , 1);	
						}				
					
					wait(3);
				}						
			//------------------	
			// cube name
			file_str_read( file_handle , temp_str );
			// is not needed - delete if exists
			if ( str_cmpi(temp_str,"-") )
				{
					if (skyCube)
						{
							ptr_remove(skyCube);
							skyCube = NULL;
						}
					// cube z
					temp = file_var_read(file_handle); 
					// cube alpha
					temp = file_var_read(file_handle); 
				}
			// if needed - create a cube
			else
				{
					sky_domeorcube = 1;
					str_cpy(temp_str2,sky_foldername);
					str_cat(temp_str2,temp_str);
					if (!skyCube)
						{															
							skyCube = ent_createlayer( temp_str2 , SKY | CUBE | SHOW , 0 );	
							set(skyCube,TRANSLUCENT);
							skyCube.material = mat_sky;
						}
					else
						{
							ent_morph( skyCube , temp_str2 );	
						}
					// cube z
					skyCube.z = file_var_read(file_handle); 
					// cube alpha
					skyCube.alpha = file_var_read(file_handle); 
					
					wait(1);
				}			
			//------------------	
			// cylinder name
			file_str_read( file_handle , temp_str );
			// is not needed - delete if exists
			if ( str_cmpi(temp_str,"-") )
				{
					if (skyCylinder)
						{
							ptr_remove(skyCylinder);
							skyCylinder = NULL;
						}
					// cylinder scale_x
					temp = file_var_read(file_handle); 
					// cylinder pan
					temp = file_var_read(file_handle); 
					// cylinder tilt
					temp = file_var_read(file_handle); 
					// cylinder alpha
					temp = file_var_read(file_handle); 
				}
			// if needed - create a cube
			else
				{
					sky_domeorcube = 1;
					str_cpy(temp_str2,sky_foldername);
					str_cat(temp_str2,temp_str);
					if (!skyCylinder)
						{															
							skyCylinder = ent_createlayer( temp_str2 , SKY | CYLINDER | SHOW , 1 );	
							set(skyCylinder,TRANSLUCENT);
							skyCylinder.material = mat_sky;
						}
					else
						{
							ent_morph( skyCylinder , temp_str2 );	
						}
					// cylinder scale_x
					skyCylinder.scale_x = file_var_read(file_handle); 
					// cylinder pan
					skyCylinder.pan = file_var_read(file_handle); 
					// cylinder tilt
					skyCylinder.tilt = file_var_read(file_handle); 
					// cylinder alpha
					skyCylinder.alpha = file_var_read(file_handle); 
					
					wait(1);
				}				
			//------------------	
			// dome-1 name
			file_str_read( file_handle , temp_str );
			// is not needed - delete if exists
			if ( str_cmpi(temp_str,"-") )
				{
					if (skyClouds1)
						{
							ptr_remove(skyClouds1);
							skyClouds1 = NULL;
						}
					// dome-1 scale_x
					temp = file_var_read(file_handle); 
					// dome-1 u
					temp = file_var_read(file_handle); 
					// dome-1 v
					temp = file_var_read(file_handle); 
					// dome-1 alpha
					temp = file_var_read(file_handle); 
				}
			// if needed - create a dome
			else
				{
					str_cpy(temp_str2,sky_foldername);
					str_cat(temp_str2,temp_str);
					if (!skyClouds1)
						{															
							skyClouds1 = ent_createlayer( temp_str2 , SKY | DOME | SHOW , 2 );	
							set(skyClouds1,TRANSLUCENT);
							skyClouds1.material = mat_sky;
						}
					else
						{
							ent_morph( skyClouds1 , temp_str2 );	
						}
					// dome-1 scale_x
					skyClouds1.scale_x = file_var_read(file_handle); 
					// dome-1 u
					skyClouds1.u = file_var_read(file_handle); 
					// dome-1 v
					skyClouds1.v = file_var_read(file_handle); 
					// dome-1 alpha
					skyClouds1.alpha = file_var_read(file_handle); 
					
					wait(1);
				}			
			//------------------
			// dome-2 name
			file_str_read( file_handle , temp_str );
			// is not needed - delete if exists
			if ( str_cmpi(temp_str,"-") )
				{
					if (skyClouds2)
						{
							ptr_remove(skyClouds2);
							skyClouds2 = NULL;
						}
					// dome-2 scale_x
					temp = file_var_read(file_handle); 
					// dome-2 u
					temp = file_var_read(file_handle); 
					// dome-2 v
					temp = file_var_read(file_handle); 
					// dome-2 alpha
					temp = file_var_read(file_handle); 
				}
			// if needed - create a dome
			else
				{
					str_cpy(temp_str2,sky_foldername);
					str_cat(temp_str2,temp_str);
					if (!skyClouds2)
						{															
							skyClouds2 = ent_createlayer( temp_str2 , SKY | DOME | SHOW , 3 );	
							set(skyClouds2,TRANSLUCENT);
							skyClouds2.material = mat_sky;
						}
					else
						{
							ent_morph( skyClouds2 , temp_str2 );	
						}
					// dome-2 scale_x
					skyClouds2.scale_x = file_var_read(file_handle); 
					// dome-2 u
					skyClouds2.u = file_var_read(file_handle); 
					// dome-2 v
					skyClouds2.v = file_var_read(file_handle); 
					// dome-2 alpha
					skyClouds2.alpha = file_var_read(file_handle); 
					
					wait(1);
				}		
			//------------------	
			sun_color.red = file_var_read(file_handle); 
			sun_color.green = file_var_read(file_handle); 			
			sun_color.blue = file_var_read(file_handle); 			
			sun_angle.pan = file_var_read(file_handle); 			
			sun_angle.tilt = file_var_read(file_handle); 
			sun_light = file_var_read(file_handle); 
			
			//------------------	
			sky_color.red = file_var_read(file_handle); 			
			sky_color.green = file_var_read(file_handle); 			
			sky_color.blue = file_var_read(file_handle); 
			
			// sky light color
			sky_skylightcolor[0] = sky_color.red / 255;
			sky_skylightcolor[1] = sky_color.green / 255;
			sky_skylightcolor[2] = sky_color.blue / 255;
						
			sky_curve = file_var_read(file_handle); 			
			sky_clip = file_var_read(file_handle); 			
			sky_blend = file_var_read(file_handle); 		
			
			//------------------	
			d3d_fogcolor1.red = file_var_read(file_handle); 			
			d3d_fogcolor1.green = file_var_read(file_handle); 			
			d3d_fogcolor1.blue = file_var_read(file_handle); 			
			d3d_fogcolor2.red = file_var_read(file_handle); 			
			d3d_fogcolor2.green = file_var_read(file_handle); 			
			d3d_fogcolor2.blue = file_var_read(file_handle); 			
			d3d_fogcolor3.red = file_var_read(file_handle); 			
			d3d_fogcolor3.green = file_var_read(file_handle); 			
			d3d_fogcolor3.blue = file_var_read(file_handle); 			
			d3d_fogcolor4.red = file_var_read(file_handle); 			
			d3d_fogcolor4.green = file_var_read(file_handle); 			
			d3d_fogcolor4.blue = file_var_read(file_handle); 			
			fog_color = file_var_read(file_handle); 
			
			//------------------		
			camera.arc = file_var_read(file_handle); 			
			camera.ambient = file_var_read(file_handle); 			
			camera.clip_near = file_var_read(file_handle); 			
			camera.clip_far = file_var_read(file_handle); 	
			if (skyModel)
				{
					camera.clip_far = vec_length(terrhmp_vgroundmin)*2 +10;					// auto set for applying sky for larger terrains 
				}					
			camera.fog_start = file_var_read(file_handle); 			
			camera.fog_end  = file_var_read(file_handle); 
			
			//------------------	
			// d3d_lodfactor 1 2 3
			sky_loddistance1 = file_var_read(file_handle); 
			sky_loddistance2 = file_var_read(file_handle); 
			sky_loddistance3 = file_var_read(file_handle); 
			var temp1 = sky_loddistance1*100;
		   var temp2 = sky_loddistance2*100;
		   var temp3 = sky_loddistance3*100;
		   vec_set(d3d_lodfactor,vector( temp1/camera.clip_far , temp2/camera.clip_far , temp3/camera.clip_far )); 		// 10,20,50. % values of view.clip_far, default: 12.5 , 25 , 50
   		clip_particles = d3d_lodfactor.y;
			//------------------	
			shadow_range = file_var_read(file_handle);
			mat_shadow.alpha = file_var_read(file_handle); 
			
//			printf("basic sky settings okay");
			
			//------------------	
			// vertical fog, sky light (supported when shadow_stencil == -1)						
			
			temp 									= file_var_read(file_handle);								// must be 2.48, later can be increased according to version number but should be kept under 512
			if (temp == (var)2.48)
				{
					//------------------
					// vertical fog
					
					temp 							= file_var_read(file_handle);								// 1, no check needed					
					sky_hfogminheight			= file_var_read(file_handle);	
					sky_hfogmaxheight			= file_var_read(file_handle);	
					sky_hfogheightstrength	= file_var_read(file_handle);	
					sky_hfogheightblending	= file_var_read(file_handle);	
					sky_hfogheightdensity 	= file_var_read(file_handle);	
					sky_hfogheightmin			= file_var_read(file_handle);	
					
					//------------------
					// sky light
					
					temp 							= file_var_read(file_handle);								// 1, no check needed					
					sky_skylightstart			= file_var_read(file_handle);	
					sky_skylightend			= file_var_read(file_handle);	
					sky_skylightheight		= file_var_read(file_handle);
				}
			//////////////////////////////////////////////////////////////////////		OLD VERSION FOR KEEPING COMPATIBILITY
			//------------------
			// pssm settings
			
//			temp_pssm_res 						= file_var_read(file_handle);
			else if (temp >= (var)512)																			// not eof, in case of old files needed, and to not to crash pssm
				{
					shadowmapping_loaded = 1;
					
					temp_pssm_res 				= temp;
					temp_pssm_splitweight 	= file_var_read(file_handle);
					temp_pssm_transparency	= file_var_read(file_handle);
					temp_pssm_bias 			= file_var_read(file_handle);
					temp_pssm_limit 			= file_var_read(file_handle);
					temp_pssm_numsplits 		= file_var_read(file_handle);
				
					// new parameters
					temp_pssm_max 						= file_var_read(file_handle);
					if (temp_pssm_max > (var)0)																// not eof, in case of old files needed, and to not to crash pssm
						{
							temp_pssm_fadeout_perc 	= file_var_read(file_handle);
							temp_pssm_manual 			= file_var_read(file_handle);
							temp_pssm_dist1 			= file_var_read(file_handle);
							temp_pssm_dist2 			= file_var_read(file_handle);
							temp_pssm_dist3 			= file_var_read(file_handle);
							temp_pssm_dist4 			= file_var_read(file_handle);
							temp_pssm_shadowweight 	= file_var_read(file_handle);
							temp_pssm_lod 				= file_var_read(file_handle)-1;					// -1 because of backward compatibility
							temp_pssm_splitmode		= file_var_read(file_handle);
						
							//------------------
							// new scsm settings
							
							temp_scsm_maxdepth			= file_var_read(file_handle);
							if (temp_scsm_maxdepth > (var)0)													// not eof, in case of old files needed, and to not to crash scsm
								{					
									temp_scsm_blurpixel			= file_var_read(file_handle);
									temp_scsm_blurtype			= file_var_read(file_handle);
									temp_scsm_vsm_epsilon		= file_var_read(file_handle);			// not used, obsolete vsm
									temp_scsm_vsm_epsilon_obj	= file_var_read(file_handle);			// not used, obsolete vsm
									temp_scsm_shadowalpha		= file_var_read(file_handle);
									temp_scsm_lightmapalpha		= file_var_read(file_handle);
									
									temp_scsm_resolution			= file_var_read(file_handle);
									temp_scsm_areasize			= file_var_read(file_handle);
									
									//--------------------------------
									// new esm settings
									
									temp_scsm_esm					= file_var_read(file_handle);			// new esm
									if (temp_scsm_esm > (var)0)												// not eof, in case of old files needed, and to not to crash scsm
										{
											temp_scsm_esm_obj				= file_var_read(file_handle);	// new esm
											temp_scsm_shadowpixeloffset= file_var_read(file_handle);	// new esm
											
											//--------------------------------
											// new pssm settings
											
											temp_pssm_esm					= file_var_read(file_handle);		// new esm
											if (temp_pssm_esm > (var)0)											// not eof, in case of old files needed, and to not to crash scsm
												{
													temp_pssm_blurtype	= file_var_read(file_handle);		// new esm
													temp_pssm_blurfactor	= file_var_read(file_handle);		// new esm
													temp_pssm_fogfactor	= file_var_read(file_handle);		// new esm
													
													temp_scsm_lod			= file_var_read(file_handle)-1;	// new lod, no eof check because can be 0, btu it's fine, used as pssm_lod, shifted by 1
													
													
													//////////////////////////////////////////////////////////////////////		TO BE KEPT
													
													/// here some protection nneded to handle file format change !!!! (after resaving all maps in new format)
													
													//--------------------------------
													// new heigth fog parameters
													var temp_fog 					= file_var_read(file_handle);		// 1 if exists (additional value)
													if (temp_fog > (var)0)													// not eof, in case of old files needed, and to not to crash scsm
													{												
														sky_hfogminheight			= file_var_read(file_handle);	
														sky_hfogmaxheight			= file_var_read(file_handle);	
														sky_hfogheightstrength	= file_var_read(file_handle);	
														sky_hfogheightblending	= file_var_read(file_handle);	
														sky_hfogheightdensity 	= file_var_read(file_handle);	
														sky_hfogheightmin			= file_var_read(file_handle);	
														
														//--------------------------------
														// new sky light settings
														var temp_skylight 			= file_var_read(file_handle);		// 1 if exists (additional value)
														if (temp_skylight > (var)0)											// not eof, in case of old files needed, and to not to crash scsm
															{
																sky_skylightstart		= file_var_read(file_handle);	
																sky_skylightend		= file_var_read(file_handle);	
																sky_skylightheight	= file_var_read(file_handle);
																
																//////////////////////////////////////////////////////////////////////	KEPT UNTIL THIS
																//--------------------------------
																// new scsm settings
																scsm_fadeout_start	= file_var_read(file_handle);
															}
													}																					
												}
										}
								}
						}
				}
			//------------------
			//////////////////////////////////////////////////////////////////////
			
			file_close(file_handle);		
		}	
	
//	printf("pssm scsm loading okay");
	
	//----------------------------------------------
	// restart actual shadowmapping with new settings								TO BE REMOVED
	
	if (shadowmapping_loaded)
		{
			if (shadow_stencil == (var)8)
				{
					if (temp_pssm_res > 0)															// protection because of file format changes				
						{
							wait(3);	
							
							// stop to be able to write data - stopped earlier
							Pssm_Close();
							wait_for(Pssm_Close);	
							
							pssm_res 				= temp_pssm_res;
							pssm_splitweight 		= temp_pssm_splitweight;
							pssm_transparency		= temp_pssm_transparency;
							pssm_bias 				= temp_pssm_bias;
							pssm_limit				= temp_pssm_limit;
							pssm_numsplits 		= temp_pssm_numsplits;
							
							if (temp_pssm_max > 0)													// protection because of file format changes		
								{
									pssm_max				= temp_pssm_max;
									pssm_fadeout_perc = temp_pssm_fadeout_perc;
									pssm_manual 		= temp_pssm_manual;
									pssm_dist1 			= temp_pssm_dist1;
									pssm_dist2 			= temp_pssm_dist2;
									pssm_dist3 			= temp_pssm_dist3;
									pssm_dist4 			= temp_pssm_dist4;
									pssm_shadowweight = temp_pssm_shadowweight;
									pssm_lod 			= temp_pssm_lod;
									pssm_splitmode		= temp_pssm_splitmode;
									
									if (temp_pssm_esm > 0)											// protection because of file format changes		
										{
											pssm_esm				= temp_pssm_esm;
											pssm_blurtype		= temp_pssm_blurtype;
											pssm_blurfactor	= temp_pssm_blurfactor;
											pssm_fogfactor		= temp_pssm_fogfactor;
										}
								}
							
							wait(3);
							
							Pssm_Start(temp_pssm_numsplits);
							
		//					printf("pssm okay");
							
							// also set scsm parameters not set before, for keeping data enabling correct saving/loading in different shadow modes
							if (temp_scsm_maxdepth > 0)	
								{
									scsm_maxdepth			= temp_scsm_maxdepth;
									
									scsm_blurpixel			= temp_scsm_blurpixel;
									scsm_blurtype			= temp_scsm_blurtype;
		//							scsm_vsm_epsilon		= temp_scsm_vsm_epsilon;			// obsolete vsm
		//							scsm_vsm_epsilon_obj	= temp_scsm_vsm_epsilon_obj;		// obsolete vsm
									scsm_shadowalpha		= temp_scsm_shadowalpha;
									scsm_lightmapalpha	= temp_scsm_lightmapalpha;
									
									scsm_resolution 		= temp_scsm_resolution;					
									scsm_areasize			= temp_scsm_areasize;	
									
									if (temp_scsm_esm > 0)											// new esm
										{
											scsm_esm						= temp_scsm_esm;
											scsm_esm_obj				= temp_scsm_esm_obj;
											
											scsm_shadowpixeloffset	= temp_scsm_shadowpixeloffset;
										}
								}
							
		//					printf("scsm okay");
						}						
				}	
			else if (shadow_stencil == (var)-1)			// with file version protection
				{
					if (temp_scsm_maxdepth > 0)
						{
							Scsm_Close();
							wait_for(Scsm_Close);
							wait(3);
							
//							printf("scsm stopped");
							
							scsm_maxdepth			= temp_scsm_maxdepth;
									
							scsm_blurpixel			= temp_scsm_blurpixel;
							scsm_blurtype			= temp_scsm_blurtype;
		//					scsm_vsm_epsilon		= temp_scsm_vsm_epsilon;					// obsolete vsm
		//					scsm_vsm_epsilon_obj	= temp_scsm_vsm_epsilon_obj;				// obsolete vsm
							scsm_shadowalpha		= temp_scsm_shadowalpha;
							scsm_lightmapalpha	= temp_scsm_lightmapalpha;
							
							scsm_resolution 		= temp_scsm_resolution;					
							scsm_areasize			= temp_scsm_areasize;	
							
							if (temp_scsm_esm > 0)													// new esm, modify defautl values if data exist
								{
									scsm_esm						= temp_scsm_esm;
									scsm_esm_obj				= temp_scsm_esm_obj;
									
									scsm_shadowpixeloffset	= temp_scsm_shadowpixeloffset;
								}					
							
							wait(1);
							
//							printf("scsm values set");
							
							Scsm_Start();																// auto calculates depth that suits new resolution, but can be modified afterwards
							
							wait(15);
							
							scsm_maxdepth 			= temp_scsm_maxdepth;						// needs to be set again after starting shadowmapping
							
//							printf("scsm okay");
						}
					else
						{
//							printf("scsm error");
						}
					
					// also set pssm parameters, for keeping data enabling correct saving/loading in different shadow modes
					if (temp_pssm_res > 0)															// protection because of file format changes				
						{
							pssm_res 				= temp_pssm_res;
							pssm_splitweight 		= temp_pssm_splitweight;
							pssm_transparency		= temp_pssm_transparency;
							pssm_bias 				= temp_pssm_bias;
							pssm_limit				= temp_pssm_limit;
							pssm_numsplits 		= temp_pssm_numsplits;
							
							if (temp_pssm_max > 0)													// protection because of file format changes		
								{
									pssm_max				= temp_pssm_max;
									pssm_fadeout_perc = temp_pssm_fadeout_perc;
									pssm_manual 		= temp_pssm_manual;
									pssm_dist1 			= temp_pssm_dist1;
									pssm_dist2 			= temp_pssm_dist2;
									pssm_dist3 			= temp_pssm_dist3;
									pssm_dist4 			= temp_pssm_dist4;
									pssm_shadowweight = temp_pssm_shadowweight;
									pssm_lod 			= temp_pssm_lod;
									pssm_splitmode		= temp_pssm_splitmode;
									
									if (temp_pssm_esm > 0)											// protection because of file format changes		
										{
											pssm_esm				= temp_pssm_esm;
											pssm_blurtype		= temp_pssm_blurtype;
											pssm_blurfactor	= temp_pssm_blurfactor;
											pssm_fogfactor		= temp_pssm_fogfactor;
										}
								}
							
		//					printf("pssm okay");
						}
					else
						{
		//					printf("pssm error");
						}
				}
		}
	//----------------------------------------------
	
//	printf( "Sky Settings Loaded: %s" , _chr(file_name) );
	
#ifndef MB_GAME	
	// load panel
	reset( map_loadpanel , SHOW);
	reset( map_loadtext1 , SHOW);	
#endif	// MB_GAME
		
	ptr_remove(temp_str);
	ptr_remove(temp_str2);
}


////////////////////////////////////////////////////////////////////


#endif