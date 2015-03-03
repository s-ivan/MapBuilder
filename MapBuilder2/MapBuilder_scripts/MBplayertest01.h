
#ifndef MBplayertest01_h
#define MBplayertest01_h


/////////////////////////////////////////////////

//#define PLAY01_DEBUG

#define STATE     skill1
#define ANIMATION skill2
#define CREATOR   skill3

/////////////////////////////////////////////////

var play01_winner 	= 0;

var play01_camdist	= 0;
var play01_campan		= 0;

var play01_crawlspeed		= 2.5;		// +ctrl
var play01_walkspeed			= 5;			// wasd
var play01_runspeed			= 12.5;		// +shuift
var play01_jumpspeed			= 20;			// space
var play01_gravity_force	= 4.0;


ENTITY* player01;

MATERIAL* playermat 	= NULL;

/////////////////////////////////////////////////

void		PlayarTest01_Init();
void		PlayarTest01_Close();

action 	PlayarTest01_Spell_Fly();

action 	PlayarTest01_Wizard_Walk();
	void		PlayarTest01_Camera_Follow(ENTITY* ent);
	function PlayarTest01_Wizard_Hit();
	
action 	PlayarTest01_Wizard_Stand();

#endif