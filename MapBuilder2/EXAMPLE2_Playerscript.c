/////////////////////////////////////////////////////////
// subscript of Example2 - it cannot run alone
/////////////////////////////////////////////////////////
// based on Lite-C Workshop24  
// simple shooter tutorial
// written by George Pirvu
// extended by Ivan Stummer
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// defines


#define STATE     skill1
#define ANIMATION skill2
#define CREATOR   skill3


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// variables


var winner;										// winning condition
var screencounter = 0;						// to make series of screenshots

var play01_camdist	= 0;					// camera distance
var play01_campan		= 0;					// camera angle

var play01_crawlspeed		= 2.5;		// +ctrl
var play01_walkspeed			= 5;			// wasd
var play01_runspeed			= 12.5;		// +shuift
var play01_jumpspeed			= 20;			// space
var play01_gravity_force	= 4.0;		// fall


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// public functions


void		PlayerTest01_Init(var my_x, my_y, char* player_filename, char* enemy_filename, var enemy_quant);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// private functions


function camera_follow(ENTITY* ent);

action 	spell_fly_green();
action 	spell_fly_red();

function wizard_hit();

action 	wizard_red();
action 	wizard_green();


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// code


void		PlayerTest01_Init(var my_x, my_y, char* player_filename, char* enemy_filename, var enemy_quant)
{
	//------------------------------------------------------------------------------------------------------------
	
	// set game state, needed for game loops
	UIMode=(var)UIMODE_PLAYTEST01;
	
	winner = 0;
	STRING* temp_str = str_create("");
	
	//------------------------------------------------------------------------------------------------------------
	
	// player
	str_cpy( temp_str , "playertest\\" );
	str_cat( temp_str , player_filename );	
	player = ent_create( temp_str , vector(my_x,my_y,3000) , wizard_red );
	
	//------------------------------------------------------------------------------------------------------------
	
	// enemy
	str_cpy( temp_str , "playertest\\" );
	str_cat( temp_str , enemy_filename );
	var i;
	random_seed(0);
	for(i=0;i<enemy_quant;i++)
		{
			ent_create( temp_str , vector( my_x+random(500)-1000 , my_y+random(500)-1000 , 3000) , wizard_green );
		}		
	
	//------------------------------------------------------------------------------------------------------------
	
	// set mouse
	mouse_map = mousearrow;			// in fullscreen mode it must be used
	mouse_mode = 1;
	//enable_mouse = 2;				// panel events can handle right clicks too
	
	//------------------------------------------------------------------------------------------------------------
	
	var pssm_state = 0;														// required to enable switching on/off
	var temp_scsm_depth = 0;												// required to enable switching on/off
	var temp_stencil_blur = 0;												// required to enable switching on/off
	var temp_window = 1;														// required to switch shadowmapping on/off when window gets active/inactive
	
	COLOR info_col;
	vec_set(info_col, vector(200,200,200));
	
	COLOR win_col;
	vec_set(win_col, vector(100,100,200));
	
	COLOR lost_col;
	vec_set(lost_col, vector(100,200,100));
	
	COLOR draw_col;
	vec_set(draw_col, vector(200,100,100));
	
	// game loop
	while(UIMode==(var)UIMODE_PLAYTEST01) 
		{
			//--------------------------------------------------------
			// required to switch shadowmapping on/off when window gets active/inactive to avoid shadow artifacts
			
			MB_SetReset_Shadowmapping(&pssm_state, &temp_scsm_depth, &temp_stencil_blur, &temp_window);
			
			//--------------------------------------------------------
			// cheating
			
			if (key_c)
				{
					while(key_c)	{wait(1);}
					winner = enemy_quant;
				}
			
			//--------------------------------------------------------
			// check winning conditions
			
			if (winner == (var)enemy_quant) 
				{
					draw_text("Well done! Red wins...",10,200,win_col);
					draw_text("Press ENTER to go to next level!",10,225,win_col);
				}
			else if (winner < 255) 
				{			
					draw_text("Enemy killed: ",10,200,info_col);
					str_for_num(temp_str,winner);
					draw_text(temp_str,200,200,info_col);				
				}
			else if (winner == (var)255) 
				{
					draw_text("Ouch!! Green wins...",10,200,lost_col);
					draw_text("Press ENTER to play again!",10,225,lost_col);
				}
			else // mutual kill
				{
					draw_text("Press ENTER to play again!",10,200,draw_col);
				}
			
			//--------------------------------------------------------
			// info text
			
			draw_text("Move/Turn:",10,300,info_col);
			draw_text("W-A-S-D",100,300,info_col);
			
			draw_text("Run:",10,325,info_col);
			draw_text("+SHIFT",100,325,info_col);
			
			draw_text("Restart:",10,350,info_col);
			draw_text("ENTER",100,350,info_col);
			
			draw_text("Cheat:",10,375,info_col);
			draw_text("C",100,375,info_col);
			
			draw_text("Screenshot:",10,400,info_col);
			draw_text("F",100,400,info_col);
			
			//--------------------------------------------------------
			// close this level
			
			if (key_enter) break;	
			
			//--------------------------------------------------------
			// make screenshot any time
			
			if (key_f)
				{
					while (key_f) {wait (1);}
					str_cpy(save_dir,"screenshots");
					file_for_screen( "shot.jpg" , screencounter ); 	
					screencounter++;
				}
					
			vec_set(mouse_pos,mouse_cursor);		
			
			//--------------------------------------------------------
			// loop end
			
			wait(1);
			
			//--------------------------------------------------------
		}
	
	//------------------------------------------------------------------------------------------------------------
	
	// player won, go to next level, else repeat the same
	if (winner == (var)enemy_quant) 									// comment this line to always go to next level
		{
			main_counter++;
		}
	
	//------------------------------------------------------------------------------------------------------------
	
	// stop local loops
	UIMode = (var)UIMODE_NEWMAP;
	mouse_mode=0;
	wait(3);
	
//	ent_remove(player);
//	player = NULL;
	
	//------------------------------------------------------------------------------------------------------------
	
	// restart
	Game01_Init();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


function camera_follow(ENTITY* ent)
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


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


action spell_fly_green()
{
	my.ambient = 100;  							// bright
	my.lightrange = 300; 						// activate dynamic light
	vec_set(my.blue,vector(50,150,50)); 	// green color
	set(me,FLAG1,FLAG2,BRIGHT);				// additive blending
	
	// set events enable hit detection, and to be scanned
	my.emask |= ENABLE_IMPACT | ENABLE_SCAN;   
	
	// set my bounding box to my real size
	c_setminmax(me);   		
	// face same direction as player
	my.pan = your.pan; 		
	my.tilt = your.tilt;		 
	
	my.material = mtl_placementarea;			// no shader attached to get it bright even if shadow_stencil == -1
//	my.material = mat_sprite;					// no shader attached to get it bright even if shadow_stencil == -1
	
	// set group to make the spell non-colliding with another spell
	my.group = 30;   
	
	// store the spell creator (you) in a skill
	// so that the detecting entity knows its enemy	  
	my.CREATOR = you;  
	
	// initial state
	my.STATE = 1;
	
	while(UIMode==(var)UIMODE_PLAYTEST01)
		{
			// state 1: flying ---------------------------------------
			if (my.STATE == 1) 
				{
					my.roll += 20*time_step; 
					c_ignore(30,0);					
					c_move(me,vector(40*time_step,0,0),NULL,IGNORE_YOU | IGNORE_PASSABLE );
					if (HIT_TARGET)  		// collided? 
						my.STATE = 2; 		// go to next state
				}
	
			// state 2: exploding -------------------------------------  
			else if (my.STATE == 2) 
				{
					//set(me,ZNEAR);  									// render in front of close objects
					my.pan = random(360);
					my.tilt = random(180)-90;
					my.roll = random(360);
					my.lightrange *= 1+0.5*time_step; 				// increase light range
					vec_scale(my.scale_x,1+0.5*time_step); 		// inflate size
					if (my.scale_x > 10) 
						{ 														// explosion finished? 
							ent_remove(me);
							return;
						}
				} 
			//---------------------------------------------------------
			
			wait(1);	
		}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


action spell_fly_red()
{
	my.ambient = 50;  								// medium bright
	my.lightrange = 300; 							// activate dynamic light
	vec_set(my.blue,vector(50,50,150)); 		// red color
	set(me,FLAG1,FLAG2,BRIGHT);					// additive blending
	
	// set events enable hit detection, and to be scanned
	my.emask |= ENABLE_IMPACT | ENABLE_SCAN;   
	
	c_setminmax(me);   								// set my bounding box to my real size
	
	my.material = mtl_placementarea;				// no shader attached to get it bright even if shadow_stencil == -1
//	my.material = mat_sprite;						// no shader attached to get it bright even if shadow_stencil == -1
	
	// set group to make the spell non-colliding with another spell
	my.group = 30; 
	
	// store the spell creator (you) in a skill
	// so that the detecting entity knows its enemy	  
	my.CREATOR = you;  
	
	// initial state
	my.STATE = 1;
			
	//------------------------------------------------------------
	// determine aim by mouse click on level surface or model
	VECTOR pos1, pos2;
	vec_set( pos1 , vector(mouse_pos.x,mouse_pos.y,0) );
	vec_for_screen (pos1, camera);
	vec_set( pos2 , vector(mouse_pos.x,mouse_pos.y,50000) ); // should be a big value       		
	vec_for_screen (pos2, camera);
	c_ignore(30,0);
	c_trace (pos1, pos2, IGNORE_ME | IGNORE_YOU | IGNORE_SPRITES | IGNORE_PASSABLE | IGNORE_CONTENT ); 
	if (HIT_TARGET) 						// if valid click, set target direction
		{					
			VECTOR vDirection;
			vec_diff(vDirection,hit.x,my.x);
			vec_to_angle(my.pan,vDirection);
		}
	else
		{
			my.pan = your.pan; 			// just go in the same direction as player stands
			my.tilt = your.tilt;
		}
	//----------------------------------------------------------
			
	while(UIMode==(var)UIMODE_PLAYTEST01)
		{
			// state 1: flying ------------------------------------------------
			if (my.STATE == 1) 
				{
					my.roll += 20*time_step; 
					c_ignore(30,0);
					c_move(me,vector(40*time_step,0,0),NULL,IGNORE_YOU | IGNORE_PASSABLE );
					if (HIT_TARGET)  		// collided? 
						my.STATE = 2;  	// go to next state
				}	
			// state 2: exploding ---------------------------------------------  
			else if (my.STATE == 2) 
				{
					//set(me,ZNEAR);  								// render in front of close objects
					my.pan = random(360);
					my.tilt = random(180)-90;
					my.roll = random(360);
					my.lightrange *= 1+0.5*time_step; 			// increase light range
					vec_scale(my.scale_x,1+0.5*time_step); 	// inflate size
					if (my.scale_x > 10) 
						{ 													// explosion finished? 
							ent_remove(me);
							return;
						}
				} 	
			//----------------------------------------------------------------
			
			wait(1);	
		}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// hit event
function wizard_hit()
{
	if (event_type==EVENT_IMPACT)
		{
			my.ANIMATION = 0;
			my.STATE = 4; 					// 4 = dead
		}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


action wizard_red()
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
	effect_load(playermat, "sc_obj_model_MB.fx");					// sc_obj_modelN_MB is the correct normal mapping shader okay with scsm or without
	
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
		// update min vertex because of crawling
		vec_for_min(vFeet,me);
		vec_mul(vFeet,my.scale_x);
		
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
						(polygon_hit > (var)GetMapTileSize() * 0.71)						// polygone detection HIT_TARGET and it is far enough
						||
						(polygon_hit <= (var)0)														// no polygon detection HIT_TARGET
					)
				)
				{
					c_move(me, vector(distance,0,0), nullvector, IGNORE_PASSABLE);		// | USE_POLYGON	| GLIDE
				}
			
			//-----------------------------------------------------------		
			// animate the entity    
			
//			my.ANIMATION += 2*distance;
//			ent_animate(me,"walk",my.ANIMATION,ANM_CYCLE);
			
			var anim = sign(distance);					// forward or backward move
			
//			ent_animate(me,NULL,0,0); 					// reset all bones
			
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
					ent_animate(me, "stand", my.ANIMATION, ANM_CYCLE);	// lstand
					my.ANIMATION %= 100;
				}
			
//			ent_animate(me, "wpose", my.ANIMATION, ANM_ADD);			// bone anim soldier weapon aiming pose	
			
			//-----------------------------------------------------------	
			// key e pressed -> go to state 2
			
			if ((mouse_left) && ((window_focus) && (mouse_valid)))			
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
			my.ANIMATION += 8*time_step;
			ent_animate(me,"attack",my.ANIMATION,0);
			
			if (my.ANIMATION > 100) 
				{ 
					// create the spell and go to state 3
					ent_create("spell.mdl",vector(my.x,my.y,my.z+20), spell_fly_red);
					
//					VECTOR vGunEnd;
//					vec_for_vertex(vGunEnd,my,5035);
//					ent_create("spell.mdl",vGunEnd, spell_fly_red);
					
					my.ANIMATION = 0;
					my.STATE = 3;
				}
		}  
		
		//-----------------------------------------------------------
		//-----------------------------------------------------------
		// state 3: waiting for key release 
		if (my.STATE == 3) 
		{
			if (!mouse_left) 
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
			winner = 255; // other wizard won
			my.ANIMATION += 3*time_step; 
			ent_animate(me,"death",my.ANIMATION,0);
			if (my.ANIMATION > 70)
				return;
		}		
		
		//-----------------------------------------------------------
		//-----------------------------------------------------------
//		// create ent_decal as shadow
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
		
		camera_follow(me);			// own camera
		
		wait(1);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// AI
action wizard_green()
{
	my.event = wizard_hit;
	my.emask |= ENABLE_IMPACT;   
	my.ENT_CATEGORY = (var)UIMODE_PLAYTEST01;
   ENTITY* enemy = ent_create(NULL,NULL,NULL); // spell target entity - more actions are running in parallel, so "= NULL" is not okay
	
	// put onto ground
	VECTOR vFeet;
	vec_for_min(vFeet,me);
	vec_mul(vFeet,my.scale_x);
	
	// needed for proper placement
	set(my,FLAG2);
	
	var pos_found = 0;	
	while(!pos_found)
		{
			// adjust entity to the ground height, using a downwards trace
			c_trace( my.x , vector(my.x,my.y,my.z-5000) , IGNORE_ME | IGNORE_PASSABLE );
			
			// to avoid placement onto shrubs, obstacles, and other wizards, and underwater
			if ( (is(you,FLAG2)) || (hit.z < 0) )	
				{
					my.x += random(250) -500;
					my.y += random(250) -500;
					my.z = 3000;					
				}
			else
				{
					pos_found = 1;
				}
  		}
  	my.z = hit.z - vFeet.z; // always place player's feet on the ground
  	
	// bounding box 
 	wait(1);
	my.eflags |= FAT | NARROW; 
 	vec_set(my.min_x,vector(-12,-12,-10)); 
 	vec_set(my.max_x,vector(12,12,24));
	wait(1);	
	
	set(my, SHADOW );  // FLAG2
//	my.shadow = ent_unitshadow_rot; 		// decal shadow
	
	my.STATE = 1;
	
	while (UIMode==(var)UIMODE_PLAYTEST01)
		{  
			// state 1: wait until wizard or spell comes close enough ------------------
			if(my.STATE == 1) 
				{
					// detect all ENABLE_SCAN entities within 750 quants 			
					c_scan(my.x,my.pan,vector(360,0,750),SCAN_ENTS | SCAN_LIMIT | IGNORE_ME);
					
					if (you) 
						{  
							enemy = your.CREATOR;
								
							if (enemy == player)		// red wizard or flying spell of red wizard detected? 
								{
									my.STATE = 2; 
								}
						}
					
				}		
			// state 2: turn towards enemy, cast spells ---------------------------------
			else if (my.STATE == 2) 
				{
					// get the angle to the enemy			
					VECTOR vDirection;
					ANGLE vTargetAngle;
					vec_diff(vDirection,enemy.x,my.x);
					vec_to_angle(vTargetAngle,vDirection);
					// vAngle is now the angle to the enemy.		
					// Turn right or left depending on the difference
					// between the current and the target pan angle		
					my.pan += time_step * sign( ang(vTargetAngle.pan) - ang(my.pan) );
					my.tilt += time_step * sign( ang(vTargetAngle.tilt) - ang(my.tilt) );
											
					my.ANIMATION += 3*time_step;
					ent_animate(me,"attack",my.ANIMATION,ANM_CYCLE);
		
					if (my.ANIMATION > 100) 									// create the spell
						{ 
							ent_create("spell.mdl",vector(my.x,my.y,my.z+10),spell_fly_green);
							my.ANIMATION -= 100;    							// continue the cycle from the beginning
						}
		
					if (enemy.STATE == 4) 										// enemy dead?
						my.STATE = 3;
				}  			
			// state 3: killed the enemy => dance of joy --------------------------
			else if(my.STATE == 3) 
				{
					my.ANIMATION += 8*time_step; 
					ent_animate(me,"attack",my.ANIMATION,ANM_CYCLE);
				}			
			// state 4: death animation -------------------------------------------
			else if(my.STATE == 4) 
				{			
					my.ANIMATION += 3*time_step; 
					ent_animate(me,"death",my.ANIMATION,0); 	// animate the entity
					if (my.ANIMATION > 70) 
						{
							winner++; 									// red wizard (player) killed one more enemy							
							set(me,PASSABLE);							// or POLYGON, enable shooting/moving through the dead wizard
							return;
						}
				}
			// --------------------------------------------------------------------	
			
			wait(1);
		}
}