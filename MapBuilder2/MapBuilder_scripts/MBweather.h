
#ifndef MBweather_h
#define MBweather_h


/////////////////////////////////////////////////////////////////////////////////////////////

// entry: "Snow flake" bitmap
// help: This bitmap is used for the snow flakes
BMAP* weather_snow_bmp 		= "graphics\\snow.dds";

// entry: "Rain drop" bitmap
// help: This bitmap is used for the rain drops
BMAP* weather_drop_bmp 		= "graphics\\drop.dds";

// entry: "Rain mist" bitmap
// help: This bitmap is used for the rain mist
BMAP* weather_misty_bmp 	= "graphics\\smoke.dds";

// entry: "Lightning" bitmap
// help: This bitmap is used for the lightning effect
BMAP* weather_effect_bmp 	= "graphics\\effect.dds";

/////////////////////////////////////////////////////////////////////////////////////////////

// entry: "Rain" sound
// help: This (looping) wave file is used as the sound for the rain
SOUND* weather_rain_wav 	= "graphics\\rain.wav";

// entry: "Wind" sound
// help: This (looping) wave file is used as the sound for the wind
SOUND* weather_wind_wav 	= "graphics\\wind.wav";

// entry: "Thunder" sound
// help: This wave file is used as the sound for the thunder
SOUND* weather_thunder_wav = "graphics\\thunder.wav";

//////////////////////////////////////////////////////////////////////////////////////////////

var weather_on								= 0;
var weather_areasize						= 1500; 
var weather_particle_density			= 150;
var weather_velocity_x					= 100;
var weather_velocity_y					= 100;
var weather_randomness_x				= 100;
var weather_randomness_y				= 100;
var weather_falling_speed				= 40;
var weather_particle_size				= 20;
var weather_lightning_frequency		= 0.95;

var weather_snow							= 0;
var weather_mist							= 1;
var weather_wind							= 1;
var weather_lightning					= 1;

ENTITY* weather_ent = NULL;

//////////////////////////////////////////////////////////////////////////////////////////////

var 		weather_temporary;
var 		weather_segment_length; 											// temporary length of the segment line 
VECTOR* 	weather_segment_start		= { x=0; y=0; z=0; } 			// starting point for the particle segment
VECTOR* 	weather_segment_end			= { x=0; y=0; z=0; }				// end of segment for the particle segment
var 		weather_line_length;									 				// temporary length of the particle line
VECTOR* 	weather_line_start			= { x=0; y=0; z=0; }				// the lightning effect starts here
VECTOR* 	weather_line_end				= { x=0; y=0; z=0; } 			// the lightning effect ends here

///////////////////////////////////////////////////////////////////////////////////////////////

void 		Weather_Init_Rain();
void 		Weather_Init_Snow();
void 		Weather_Init();

function particle_init(PARTICLE* p);
function fade_particles(PARTICLE* p);

function rain_mist(PARTICLE* p);
function fade_mist(PARTICLE* p);

function lightning_effect();
function lightning_fade(PARTICLE* p);
function lightning_particle(PARTICLE* p);

function particle_segment();
//function particle_lightning();
function increase_brightness();



#endif