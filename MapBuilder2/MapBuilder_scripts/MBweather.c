
#ifndef MBweather_c
#define MBweather_c


///////////////////////////////////////////////////////////////////////////////////////////////

void 		Weather_Init_Rain()
{
	// rain settings
	weather_areasize						= 1250; 
	weather_particle_density			= 100;
	weather_velocity_x					= 10;
	weather_velocity_y					= 5;
	weather_randomness_x					= 10;
	weather_randomness_y					= 10;
	weather_falling_speed				= 30;
	weather_particle_size				= 0.5;
	
	weather_lightning_frequency		= 0.95;
	
	weather_snow							= 0;
	weather_mist							= 1;
	weather_wind							= 1;
	weather_lightning						= 1;
	
	Weather_Init();
}

void 		Weather_Init_Snow()
{	
	// snow settings
	weather_areasize						= 750; 
	weather_particle_density			= 150;
	weather_velocity_x					= 50;
	weather_velocity_y					= 10;
	weather_randomness_x					= 150;
	weather_randomness_y					= 150;
	weather_falling_speed				= 20;
	weather_particle_size				= 2;
	
	weather_lightning_frequency		= 1;
	
	weather_snow							= 1;
	weather_mist							= 0;
	weather_wind							= 1;
	weather_lightning						= 0;
	
	Weather_Init();
}

// title: Snow and rain
// uses: minimum_x, minimum_y, maximum_x, maximum_y, particle_density, randomness, falling_speed, particle_size, lightning_frequency, snow, mist, wind, lightning;
// desc: Snow & rain effect; includes mist and lightning
void 	Weather_Init()
{	
	if(weather_particle_size == (var)0)
		{ 
			weather_particle_size = 1; 
		} 							
	if(weather_lightning_frequency == (var)0)
		{ 
			weather_lightning_frequency = 0.95; 
		}
	if((weather_particle_density == (var)0))
		{
			return;
		}
	
	weather_ent = ent_create( NULL, camera.x, NULL);
	wait(1);
	
	weather_on = 1;
	
	if(!weather_snow) // raining?
	{
		ent_playloop(weather_ent, weather_rain_wav, 300);
	}
	if (weather_wind)
	{
		ent_playloop(weather_ent, weather_wind_wav, 200);
	}
	
	set(weather_ent, INVISIBLE);
	set(weather_ent, PASSABLE);
	
	VECTOR weather_generator;
	vec_fill(weather_generator,0);
	
	random_seed(0);
	
	while(weather_on)
		{
			vec_set( weather_ent.x , camera.x );																		// sound follows entity that follows camera
			
			int i;
			for (i = 0; i < 50; ++i)
				{
		//			vec_set( weather_generator , weather_ent.x );
					weather_generator.x = camera.x + /* cosv(camera.pan) * */ (0.5 * weather_areasize - random(weather_areasize));		
					weather_generator.y = camera.y + /* sinv(camera.pan) * */ (0.5 * weather_areasize - random(weather_areasize));
					weather_generator.z = camera.z + 320;
					
					// snow or rain
					effect(particle_init, weather_particle_density/50, weather_generator, nullvector);
				}
				
//			// lightning
//			if( (weather_lightning) && (random(1)>weather_lightning_frequency) )
//				{
//					vec_set(weather_line_start, weather_generator);
//					vec_set(weather_line_end, vector(weather_generator.x+random(weather_areasize)-weather_areasize/2, weather_generator.y+random(weather_areasize)-weather_areasize/2, weather_generator.z - 500));
//					
//					lightning_effect();
//					
////					snd_play(weather_thunder_wav, 100, 0);
//					
//					increase_brightness();
//				}
			
			wait(1);
		}
		
	ptr_remove(weather_ent);
	weather_ent = NULL;
}

///////////////////////////////////////////

function particle_init(PARTICLE* p)
{
	VECTOR temp;
	temp.x = weather_velocity_x + 0.5 * weather_randomness_x - random(weather_randomness_x);
	temp.y = weather_velocity_y + 0.5 * weather_randomness_y - random(weather_randomness_y);
	temp.z = - weather_falling_speed * 1.25 + random(weather_falling_speed * 0.5);
	vec_set(p.vel_x, temp);
	
	set(p, BRIGHT | MOVE);
	
	if(weather_snow)
		{
//			p.bmap = weather_snow_bmp;
			p.alpha = 60;			
		}
	else
		{
//			p.bmap = weather_drop_bmp;
			set(p, STREAK);
			p.alpha = 60;			
		}
	
	p.size = weather_particle_size;	
	p.event = fade_particles;
//	p.skill_a = handle(weather_ent); 					// store "you"
}

function fade_particles(PARTICLE* p)
{
	p.alpha -= 0.2 * time_step;
	
	if(p.alpha < 5)											// else , 0
		{
			p.lifespan = 0;
		}
////	else if(p.z < camera.z - 1000)						// else , 100 ***
////		{ 
////			p.lifespan = 0; 
////		}
//	else																					// if exact removal required !
//		{
//			VECTOR temp;
//			vec_set(temp,p.x);
//			temp.z += -p.vel_z + 15;												// above particle by speed distance			
//			
//			c_ignore(1,0);
//			c_trace( p.x, vector(p.x,p.y,p.z+p.vel_z), IGNORE_FLAG2 | IGNORE_PASSENTS | IGNORE_SPRITES | IGNORE_CONTENT );	// IGNORE_ME | IGNORE_YOU |  *** not set IGNORE_MAPS | IGNORE_MODELS | IGNORE_PASSABLE 
//			
//			if(HIT_TARGET) 															// the particle is inside a solid?
//				{
//					if (weather_mist)
//						{
//							if((random(1) > 0.50)) 									// limit mist quantity (0.9)
//								{
//									effect(rain_mist, 1, hit.x, nullvector); 	// generate mist (over hit surface)
//								}
//						}
//					p.lifespan = 0; 													// don't allow the particles to continue their movement if they have hit something solid
//				}
//		}		
}

/////////////////////////////////////////////

function rain_mist(PARTICLE* p)
{
	VECTOR temp;
	temp.x = random(2) - 1;
	temp.y = random(2) - 1;
	temp.z = random(1) + 0.5; // play with this value
	vec_set(p.vel_x, temp);
	
	p.bmap = weather_misty_bmp;
	p.size = 1;
	p.lifespan = 500;
	set(p, BRIGHT | MOVE);
	p.gravity = 0;
	p.alpha = 35;						// 30 ***
	p.event = fade_mist;
}

function fade_mist(PARTICLE* p)
{
	p.alpha -= 1.5 * time_step;
	p.size += 0.8 * time_step;
	if(p.alpha < 0) 
		{
			p.lifespan = 0;
		}
}

/////////////////////////////////////////

function lightning_effect()
{
	// define start and end positions of lightning strike:
	vec_set(weather_temporary, weather_line_start);
	vec_sub(weather_line_end, weather_line_start);
	weather_line_length = vec_length(weather_line_end);
	
	// create segments every 200 quants:
	weather_line_end.x = (weather_line_end.x * 100) / weather_line_length; 
	weather_line_end.y = (weather_line_end.y * 100) / weather_line_length;
	weather_line_end.z = (weather_line_end.z * 100) / weather_line_length;
	
	// loop:
	while(weather_line_length > 0)
		{
			vec_add(weather_line_start, weather_line_end);
			vec_set(weather_segment_start, weather_temporary);
			vec_set(weather_segment_end, weather_line_start);
			
			weather_segment_end.x += random(60) - 30; // displace the lightning segments (don't make the lightning look like a straight stroke)
			weather_segment_end.y += random(60) - 30;
			weather_segment_end.z += random(60) - 30;
			
			particle_segment();
			
			weather_line_length -= 100; // keep the same value here
		}
}

function lightning_fade(PARTICLE* p)
{
	p.lifespan = 0;
}

function lightning_particle(PARTICLE* p)
{
	VECTOR temp;
	temp.x = random(2) - 1;
	temp.y = random(2) - 1;
	temp.z = random(2) - 1;
	vec_set(p.vel_x, temp);
	
	p.bmap = weather_effect_bmp;
	p.size = 5;
	set(p, MOVE | BRIGHT | TRANSLUCENT);
	p.lifespan = 1;
	p.event = lightning_fade;
}

function particle_segment()
{
	// define start and end positions of lightning strike:
	vec_set(weather_temporary, weather_segment_end);
	vec_sub(weather_segment_end, weather_segment_start);
	weather_segment_length = vec_length(weather_segment_end);
	
	// create particles every 2. quant:
	weather_segment_end.x = (weather_segment_end.x * 2) / weather_segment_length; 
	weather_segment_end.y = (weather_segment_end.y * 2) / weather_segment_length;
	weather_segment_end.z = (weather_segment_end.z * 2) / weather_segment_length;
	
	// loop:
	while(weather_segment_length > 0)
		{
			effect(lightning_particle, 2, weather_segment_start.x, nullvector);	
			vec_add(weather_segment_start, weather_segment_end);
			weather_segment_length -= 2; // same value here
		}
}

function increase_brightness()
{
	var temp = camera.ambient;
	camera.ambient = 100;
	wait(3);
	camera.ambient = temp;
}


#endif