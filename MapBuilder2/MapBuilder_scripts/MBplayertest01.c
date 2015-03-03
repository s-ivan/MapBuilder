
#ifndef MBplayertest01_c
#define MBplayertest01_c


/////////////////////////////////////////////////

void		PlayarTest01_Init()
{
	shadow_mode &= ~IGNORE_FLAG2 ;
	wait(3);
	
	Camera_Close();	// editor camera stop
	
//	player01 = ent_create( "playertest\\pguard3_w.mdl" , vector(square_coord_x,square_coord_y,square_coord_z+GetMapTileSize()*3) , PlayarTest01_Wizard_Walk );
	player01 = ent_create( "playertest\\marine.mdl" , vector(square_coord_x, square_coord_y, square_coord_z + 32.0) , PlayarTest01_Wizard_Walk );
	
	
	while(UIMode==(var)UIMODE_PLAYTEST01)
		{
			if ((window_focus) && (mouse_valid))
				{
					if (key_0)
						{
							// stop any effect
							weather_on = 0;
						}
					else if (key_1)
						{
							while(key_1)	{wait(1);}
							Weather_Init_Rain();
						}
					else if (key_2)
						{
							while(key_2)	{wait(1);}
							Weather_Init_Snow();
						}
				}
			
			wait(1);
		}
	PlayarTest01_Close();
}


void		PlayarTest01_Close()
{
	// stop any effect
	weather_on = 0;
	
	wait(1);
	
	Camera_Init();		// editor camera start		
	
	ptr_remove(player01);
	player01 = NULL;
		
	ptr_remove(playermat);
	playermat = NULL;
	
	shadow_mode |= IGNORE_FLAG2 ;
}

////////////////////////////////////////////////////////////////////////////

action PlayarTest01_Spell_Fly()
{
	my.ambient = 50;  						// medium bright
	my.lightrange = 50;						// activate light
	vec_set(my.blue,vector(0, 0, 255)); // light color
	vec_scale(my.scale_x, 0.25);
	set(my, BRIGHT);   						// additive blending

	c_setminmax(me);   						// set my bounding box to my real size
	my.pan = your.pan; 						// face same direction as player

// set FLAG2 to make the spell detectable by c_scan,
// and store the spell creator (you) in a skill
// so that the detecting entity knows its enemy	
	set(my, FLAG2);     							
	
	my.material = mtl_placementarea;		// no shader attached to get it bright even if shadow_stencil == -1
	
//	if ((shadow_stencil >= (var)0) && (main_water == (var)2))
//		{
//			set(my, FLAG1);     				//XXX do not render it in mirror views, also required by shade-c 0.91 water depth 
//		}
	
	my.CREATOR = you;  
	
//	var myspeed = 40;
	var myspeed = 200;
	
	my.STATE = 1;
	while(UIMode==(var)UIMODE_PLAYTEST01)
	{
// state 1: flying ///////////////////////////////////////////  
		if (my.STATE == 1) 
		{
			my.roll += 20*time_step; 
			c_move(me,vector(myspeed*time_step,0,0),NULL,IGNORE_YOU | IGNORE_PASSABLE);
			if (HIT_TARGET)  // collided? 
				my.STATE = 2;  // go to next state
		}

// state 2: exploding ////////////////////////////////////////  
		if (my.STATE == 2) 
		{
			set(me,ZNEAR);  // render in front of close objects
			my.pan = random(360);
			my.tilt = random(180)-90;
			my.roll = random(360);
			my.lightrange *= 1+0.5*time_step; // increase light range
			vec_scale(my.scale_x,1+0.5*time_step); // inflate size
			if (my.scale_x > 5) { // explosion finished? 
				ent_remove(me);
				return;
			}
		} 

		wait(1);	
	}
}

//////////////////////////////////////////////////////////////////////////////////

// hit event
function PlayarTest01_Wizard_Hit()
{
	my.ANIMATION = 0;
	my.STATE = 4; // 4 = dead
}

//////////////////////////////////////////////////////////////////////////////////

void		PlayarTest01_Camera_Follow(ENTITY* ent)
{
	if ((window_focus) && (mouse_valid))
		{
			if (key_home) play01_camdist -= 10*time_step*(1 + key_shift);					// inc/dec camera distance from player
			if (key_end)  play01_camdist += 10*time_step*(1 + key_shift);					
			play01_camdist = clamp(play01_camdist, -100, 500);
			
			if (key_pgup) play01_campan  += 5*time_step*(1 + key_shift);					// cw/ccw camera rotation around player
			if (key_pgdn) play01_campan  -= 5*time_step*(1 + key_shift);
		}
	
	VECTOR oldcamera;
	VECTOR newcamera;
	
	vec_set(oldcamera, camera.x);
	
	vec_set(newcamera,vector(-150-play01_camdist,0,40));  								// camera position relative to the player      			
	vec_rotate(newcamera, ent.pan); 															// rotate the camera position with the player	vector(play01_campan,0,0)
	vec_rotate(newcamera, vector(play01_campan,0,0));			
	vec_add(newcamera,ent.x);      															// add player position
	
	vec_lerp(camera.x, oldcamera, newcamera, 0.85);										// smoothen a bit
	
	vec_set(camera.pan,vector(ent.pan+play01_campan,-8,0)); 							// look in player direction, slighty down      
}

//////////////////////////////////////////////////////////////////////////////////

action PlayarTest01_Wizard_Walk()
{
	//---------------------------------
	// events
	
	my.event = PlayarTest01_Wizard_Hit;
	my.emask |= ENABLE_IMPACT;   
 	
//	wait(3);
//	printf("player init");
 	
 	//--------------------------------- 	
	// set z always by c_tracing
	
	VECTOR vFeet;
	vec_for_min(vFeet,me);
	vec_mul(vFeet,my.scale_x);
	
	c_trace(vector(my.x, my.y, 5000.0), vector(my.x, my.y, -1000.0), IGNORE_ME | IGNORE_CONTENT | IGNORE_PASSABLE | USE_POLYGON | USE_BOX);		// | USE_BOX for smoother ground detection
	if (HIT_TARGET)
		{
			my.z = hit.z - vFeet.z;
		}
	
	//---------------------------------
	// boundary box

 	wait(1);
	my.eflags |= FAT | NARROW; 				// set both flags to prevent automatic recalculation on scale changes
	vec_set(my.min_x,vector(-8,-8, -3)); 	// better step if xy are lower and z is higher - but worse small wall detection
	vec_set(my.max_x,vector(16, 8, 32));	// better step if x is lower
//	vec_set(my.max_x,vector(25,8,30));		// no wall penetration
	wait(1);	
	
	//---------------------------------
	// material settings
	
	my.ambient = 12;	// 0..100, 10 = 25.5 in 0..255
	
//	my.material = mtl_model;
	
	// normalmapped
	playermat = mtl_create();
	vec_set(playermat.ambient_blue, mat_model.ambient_blue);
	vec_set(playermat.diffuse_blue, mat_model.diffuse_blue);
	vec_set(playermat.specular_blue, mat_model.specular_blue);
	vec_set(playermat.emissive_blue, mat_model.emissive_blue);
	playermat.power = mat_model.power;
	playermat.albedo = mat_model.albedo;
	effect_load(playermat, "sc_obj_modelN_MB.fx");					// the correct normal mapping shader okay with scsm or without
	
	my.material = playermat;
	
	//---------------------------------
	// flags
	
// set FLAG2 to make the wizard detectable by c_scan, 
// and store the wizard pointer in its own CREATOR skill 
// so that the detecting entity knows its enemy	
	set(my, FLAG2 | SHADOW);
		
   my.CREATOR = me;         
	my.STATE = 1;
	
//	printf("player ready");
	
	//---------------------------------
	
	var distance 			= 0;
	var jump_in_progress = 0;
	var actual_gravity	= 0;	
	
	while (UIMode==(var)UIMODE_PLAYTEST01)
	{
		//-----------------------------------------------------------	
		// state 1: walking 
		if (my.STATE == 1)
		{
			//-----------------------------------------------------------
			
			my.ANIMATION %= 100;
			
			//-----------------------------------------------------------
			// rotate the entity with the arrow keys     
			
			if ((window_focus) && (mouse_valid))
				{
					my.pan += (key_a-key_d)*5*time_step;
					
//					// or rotate with collision to avoid going through polygon thin walls at mesh borders
//					var rot_angle = (key_a-key_d)*5*time_step;
//					c_rotate(me, vector(rot_angle, 0, 0), IGNORE_PASSABLE);		
				}
			
			//-----------------------------------------------------------
			// move the entity forward/backward with the arrow keys
			
			if (jump_in_progress == (var)0)										// keep previous distance during jump
				{
					distance	= 0;
						
					if ((window_focus) && (mouse_valid))
						{
							distance = (key_w-key_s) * time_step;														
							
							if ((key_shift) && (distance > 0))					// only forward
								{
									distance *= play01_runspeed;
								}
							else if ((key_ctrl) && (distance != (var)0))		// moving
								{
									distance *= play01_crawlspeed;																		
								}
							else
								{
									distance *= play01_walkspeed;
								}
							
							if (key_ctrl)
								{
									// smaller bbox
									vec_set(my.min_x,vector(-8,-8, -12)); 
									vec_set(my.max_x,vector(16, 8, 20));	
								}
							else
								{
									// standard bbox
									vec_set(my.min_x,vector(-8,-8, -3)); 	
									vec_set(my.max_x,vector(16, 8, 32));	
								}
						}
				}
			
			// simple move
//			c_move(me, vector(distance,0,0), NULL, GLIDE | IGNORE_PASSABLE );
			
			//-----------------------------------------------------------
			// prevent under water movement
			
			VECTOR temp_vec;
			vec_set(temp_vec, vector(distance, 0, 0));
			vec_rotate(temp_vec, my.pan);
			vec_add(temp_vec, my.x);
			
			VECTOR temp2_vec;
			vec_set(temp2_vec, vector(temp_vec.x, temp_vec.y, temp_vec.z - 1000.0));
			
			c_trace(temp_vec, temp2_vec, IGNORE_ME | IGNORE_PASSABLE | USE_POLYGON);		// | USE_BOX for smoother ground detection
			
			var water_hit = -1000;
			if (HIT_TARGET)
				{
					water_hit = hit.z;
				}
			
#ifdef PLAY01_DEBUG			
			// debug - ok
			draw_line3d(temp_vec, NULL, 100);
			draw_line3d(temp2_vec, COLOR_WHITE, 100);
#endif			
			//-----------------------------------------------------------
			// allow no bad collision with small polygon models
			
			vec_set(temp_vec, vector(my.max_x*sign(distance),0,0));		// *sign(distance) needed to check forward or backward move
			vec_rotate(temp_vec, my.pan);
			vec_add(temp_vec, my.x);
			temp_vec.z += my.min_z;
			
//			vec_set(temp2_vec, vector(my.x, my.y, my.z+my.min_z));
			vec_set(temp2_vec, vector(my.min_x*sign(distance),0,0));		// *sign(distance) needed to check forward or backward move
			vec_rotate(temp2_vec, my.pan);
			vec_add(temp2_vec, my.x);
			temp2_vec.z += my.min_z;
			
			var polygon_hit = c_trace(temp2_vec, temp_vec, IGNORE_ME | IGNORE_PASSABLE | USE_POLYGON);				
			
#ifdef PLAY01_DEBUG				
			// debug - ok
			draw_line3d(temp_vec, NULL, 100);
			draw_line3d(temp2_vec, COLOR_RED, 100);
#endif			
			//-----------------------------------------------------------
			// gravity						
			
			var move_z	= 0;			
			
			// ground
			var grounddist = c_trace(my.x, vector(my.x, my.y, my.z-10000), IGNORE_ME | IGNORE_PASSABLE);
			
			// acceleration - if ground is lower or jumping
			if((grounddist + vFeet.z > 0) || (actual_gravity < 0))
				{					
					actual_gravity += play01_gravity_force * time_step;
					
					// apply acceleration
					move_z -= minv(actual_gravity * time_step, grounddist + vFeet.z);
				}
			else
				{
					actual_gravity = 0;
					
					move_z = (hit.z - vFeet.z) - my.z;
//					move_z -= grounddist + vFeet.z;
				}
			
			// jump by acceleration
			if ((window_focus) && (mouse_valid))
				{
					if(key_space)					
						{							
							// allow jump
							if(grounddist + vFeet.z - minv(actual_gravity * time_step, grounddist + vFeet.z) == (var)0)
								{
									actual_gravity = -play01_jumpspeed;
									jump_in_progress = 1;
								}
						}
				}
			
			my.z += move_z;
			
			//-----------------------------------------------------------
			// do move if allowed
			
			if (
					(water_hit > -GetMapTileSize()) 
					&& 
					(
						(polygon_hit > (var)GetMapTileSize() * 0.71)							// polygone detection HIT_TARGET and it is far enough
						||
						(polygon_hit <= (var)0)														// no polygon detection HIT_TARGET
					)
				)
				{
					c_move(me, vector(distance,0,0), nullvector, IGNORE_PASSABLE);		// | USE_POLYGON	| GLIDE
				}
						
			//-----------------------------------------------------------	
			// adjust entity to the ground height, using a downwards trace
			
//			c_trace(my.x, vector(my.x, my.y, my.z-1000), IGNORE_ME | IGNORE_PASSABLE);
//			my.z = hit.z - vFeet.z; 																				// always place player's feet on the ground
			
//			// smoother on-stairs movement, slower (but still ugly) falling - some underground placement errors... XXX
//			c_trace(vector(my.x, my.y, my.z), vector(my.x, my.y, my.z - 1000.0), IGNORE_ME | IGNORE_PASSABLE);		// | USE_BOX for smoother ground detection
//			if (HIT_TARGET)
//				{
//					my.z = smooth(hit.z - vFeet.z, 0.50);
//				}
			
			//-----------------------------------------------------------		
			// animate the entity    
			
//			my.ANIMATION += 2*distance;
//			ent_animate(me,"walk",my.ANIMATION,ANM_CYCLE);
			
			var anim = sign(distance);					// forward or backward move
			
			ent_animate(me,NULL,0,0); 					// reset all bones
			
			if (jump_in_progress)												// jump
				{
					my.ANIMATION += 7*time_step;
					ent_animate(me, "jump", my.ANIMATION, NULL);		
					if (my.ANIMATION >= (var)100)
						{
							jump_in_progress = 0;
							my.ANIMATION %= 100;
						}					
				}
			else if ((key_shift) && (anim))									// run
				{
					my.ANIMATION += 10*time_step*anim;
					ent_animate(me, "run", my.ANIMATION, ANM_CYCLE);
					my.ANIMATION %= 100;
				}
			else if ((key_ctrl) && (anim))									// crawl, move
				{
					my.ANIMATION += 4*time_step*anim;
					ent_animate(me, "crawl", my.ANIMATION, ANM_CYCLE);
					my.ANIMATION %= 100;
				}
			else if ((key_ctrl) && (!anim))									// crawl, no move
				{
					ent_animate(me, "crawl", my.ANIMATION, NULL);
				}
			else if (anim)															// walk
				{
					my.ANIMATION += 10*time_step*anim;
					ent_animate(me, "walk", my.ANIMATION, ANM_CYCLE);
					my.ANIMATION %= 100;
				}
			else																		// no move, no jump, no crawl
				{
					my.ANIMATION += 3*time_step;
					ent_animate(me, "lstand", my.ANIMATION, ANM_CYCLE);
					my.ANIMATION %= 100;
				}
			
			ent_animate(me, "wpose", my.ANIMATION, ANM_ADD);			
			
			//-----------------------------------------------------------	
			// key e pressed -> go to state 2
			
			if ((key_e) && ((window_focus) && (mouse_valid)))			
				{ 
					my.ANIMATION = 0;
					my.STATE = 2;
				}
			
			//-----------------------------------------------------------
		}
		
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		// state 2: casting a spell 
		if (my.STATE == 2) 
		{
//			my.ANIMATION += 8*time_step;
//			ent_animate(me,"attack",my.ANIMATION,0);
//			
//			if (my.ANIMATION > 100) 
				{ // create the spell and go to state 3
	//				ent_create("spell.mdl",vector(my.x,my.y,my.z+20), PlayarTest01_Spell_Fly);
					
					VECTOR vGunEnd;
					vec_for_vertex(vGunEnd,my,5035);
					ent_create("spell.mdl",vGunEnd, PlayarTest01_Spell_Fly);
					
					my.ANIMATION = 0;
					my.STATE = 3;
				}
		}  
		
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		// state 3: waiting for key release 
		if (my.STATE == 3) 
		{
			if (!key_e) 
				{
//					my.ANIMATION += 3*time_step;
//					ent_animate(me,NULL,0,0); 	
//					ent_animate(me,"lstand",my.ANIMATION,0);
//					ent_animate(me,"reload",my.ANIMATION,ANM_ADD);
//					
//	//				my.ANIMATION += 8*time_step;
//	//				ent_animate(me,"reload",my.ANIMATION,0);
//					
//					if (my.ANIMATION > 100) 
						my.STATE = 1; 											// when key was released, back to state 1
				}
//			my.STATE = 1; 														// fast shooting test
		}
		
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		// state 4: death animation 
		if (my.STATE == 4) 
		{
			play01_winner = 2; // other wizard won
			my.ANIMATION += 3*time_step; 
			ent_animate(me,"death",my.ANIMATION,0);
			if (my.ANIMATION > 70)
				return;
		}		
		
		//-----------------------------------------------------------
		//-----------------------------------------------------------
//		// create decal as shadow
//		c_trace(my.x,vector(my.x,my.y,my.z-100) , IGNORE_ME | IGNORE_CONTENT | IGNORE_PASSABLE | IGNORE_FLAG2  | IGNORE_SPRITES | SCAN_TEXTURE );		
//		if (HIT_TARGET)
//			{
//				PARTICLE* shadow_p = ent_decal(you,shadow_bmp,(abs(me.max_x)+abs(me.max_y)+abs(me.max_z)),0);			// A7
//				shadow_p.material = mtl_decalshadow; 																								// assign another material when contact material is not suitable for decals like terraintex.fx
//				set(shadow_p,TRANSLUCENT);
//				shadow_p.alpha = 50;						
//				shadow_p.lifespan = 0.01;				
//			}
		
		//-----------------------------------------------------------
		
		PlayarTest01_Camera_Follow(me);			// own camera
		
		wait(1);
	}
}

/////////////////////////////////////////////////////////////////////////////////////

action PlayarTest01_Wizard_Stand()
{
	my.event = PlayarTest01_Wizard_Hit;
	my.emask |= ENABLE_IMPACT;   

  ENTITY* enemy = NULL; // spell target
	
	set(my,FLAG2 | SHADOW);  
	my.shadow = ent_unitshadow_rot; 
	
	my.STATE = 1;
	while (UIMode==(var)UIMODE_PLAYTEST01)
	{  
// state 1: wait until wizard or spell comes close enough ////// 
		if(my.STATE == 1) 
		{
// detect all FLAG2 entities within 750 quants 			
			c_scan(my.x,my.pan,vector(360,0,750),SCAN_ENTS | SCAN_FLAG2 | IGNORE_ME);
			if (you) {  // red wizard or flying spell detected?
				my.STATE = 2; 
				enemy = your.CREATOR;
			}
		}
	
// state 2: turn towards enemy, cast spells /////////////////// 
		if (my.STATE == 2) 
		{
// get the angle to the enemy			
			VECTOR vDirection;
			ANGLE vTargetAngle;
			vec_diff(vDirection,enemy.x,my.x);
			vec_to_angle(vTargetAngle,vDirection);
// vAngle is now the angle to the enemy.		
// Turn right or left depending on the difference
// between the current and the target pan angle			
			my.pan += time_step * sign(ang(vTargetAngle.pan - my.pan));
				
			my.ANIMATION += 3*time_step;
			ent_animate(me,"attack",my.ANIMATION,ANM_CYCLE);

			if (my.ANIMATION > 100) { // create the spell
				ent_create("spell.mdl",vector(my.x,my.y,my.z+10), PlayarTest01_Spell_Fly);
				my.ANIMATION -= 100;    // continue the cycle from the beginning
			}

			if (enemy.STATE == 4) // enemy dead?
				my.STATE = 3;
		}  
		
// state 3: killed the enemy => dance of joy //////////////////// 
		if(my.STATE == 3) 
		{
			my.ANIMATION += 8*time_step; 
			ent_animate(me,"attack",my.ANIMATION,ANM_CYCLE);
		}
		
// state 4: death animation //////////////////////////////////// 
		if(my.STATE == 4) 
		{
			play01_winner = 1; // other wizard (red) won
			my.ANIMATION += 3*time_step; 
			ent_animate(me,"death",my.ANIMATION,0); // animate the entity
			if (my.ANIMATION > 70) 
				return;
		}

		wait(1);
	}
}


#endif