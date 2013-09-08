#define NUM_SND_DIRS 144
struct sound
{
	int group; 
	string filename;

	sound( string s, int g ) : group( g ), filename( s ) {}
	sound( string s) : group( rand() % NUM_SND_DIRS ), filename( s ) {}
};

enum monster_types
{
	AGRUNT,
	APACHE,
	BARNACLE,
	BULLCHICKEN,
	CONTROLLER,
	GARG,
	BABYGARG,
	GONARCH,
	HEADCRAB,
	ZOMBIE,
	HOUNDEYE,
	ICHY,
	LEECH,
	ROACH,
	PITDRONE,
	KINGPIN,
	TOR,
	VOLTIGORE,
	GONOME,
	//OTIS,
	GRUNT,
	TURRET,
	NIH,
	SHOCKROACH,
	// The following can be edited 
	// through sentences.txt
	ROBOGRUNT,
	SHOCKTROOPER,
	BODYGUARD,
	ASLAVE,
	SCIENTIST,
	// HASSAULT,
	BARNEY,
	MONSTER_TYPES,
};

enum breakable_types
{
	GLASS,
	WOOD,
	METAL,
	FLESH,
	CONCRETE,
	TILE,
	COMPUTER, // metal and glass
	UNBREAKABLE, // no sounds
	ROCK,
	BREAKABLE_TYPES,
};

enum weapon_types
{
	PIPEWRENCH,
	CROWBAR,
	MINE,
	EGON,
	WEAPON_TYPES,
};

#define NUM_VOICE_DIRS 33
string voice_dirs[NUM_VOICE_DIRS] = 
{
	"aslave",		 
	"barney",		
	"scientist",  
	"bodyguard", 
	"shocktrooper",
	"otis",
	"fgrunt",
	"nihilanth",
	"vox",
	"x",
	"hgrunt",
	"holo",
	"gman",
	"fvox",

	"agrunt",		 // 26 files
	"apache",		 // 5 files
	"barnacle",	 // 8
	"bullchicken", // 26
	"controller",  // 16
	"garg",		 // 25
	"gonarch",	 // 22
	"headcrab",	 // 16
	"zombie",		 // 16
	"houndeye",	 // 25
	"ichy",		 // 19
	"scientist",   // 9 / 384
	"pitdrone", // 26
	"kingpin", // 19
	"tor", // 14
	"voltigore", // 25
	"gonome", // 15
	"squeek",
	"babygarg",
};

#define NUM_SQUEEK 6
string squeek[NUM_SQUEEK] =
{
	"sqk_blast1",
	"sqk_deploy1",
	"sqk_die1",
	"sqk_hunt1",
	"sqk_hunt2",
	"sqk_hunt3",
};

#define NUM_HORNET 6
string hornet[NUM_HORNET] =
{
	"ag_buzz1",
	"ag_buzz2",
	"ag_buzz3",
	"ag_hornethit1",
	"ag_hornethit2",
	"ag_hornethit3",
};

#define NUM_SPARKS 6
string spark[NUM_SPARKS] =
{
	"spark1",
	"spark2",
	"spark3",
	"spark4",
	"spark5",
	"spark6",
};

#define NUM_EGON 3
string egon[NUM_EGON] =
{
	"egon_off1",
	"egon_run3",
	"egon_windup2",
};

#define NUM_MINE 3
string mine[NUM_MINE] =
{
	"mine_activate",
	"mine_charge",
	"mine_deploy",
};

#define NUM_CROWBAR 6
string crowbar[NUM_CROWBAR] =
{
	"cbar_hit1",
	"cbar_hit2",
	"cbar_hitbod1",
	"cbar_hitbod2",
	"cbar_hitbod3",
	"cbar_miss1",
};

#define NUM_WRENCH 10
string wrench[NUM_WRENCH] =
{
	"pwrench_big_hitbod1",
	"pwrench_big_hitbod2",
	"pwrench_big_miss",
	"pwrench_hit1",
	"pwrench_hit2",
	"pwrench_hitbod1",
	"pwrench_hitbod2",
	"pwrench_hitbod3",
	"pwrench_miss1",
	"pwrench_miss2",
};

#define NUM_FANS 5
string fans[NUM_FANS] =
{
	"fan1",
	"fan2",
	"fan3",
	"fan4",
	"fan5",
};

#define NUM_ROT_BUTTONS 5
string rbuttons[NUM_ROT_BUTTONS] =
{
	"lever1",
	"lever2",
	"lever3",
	"lever4",
	"lever5",
};

#define NUM_BUTTONS 14
string buttons[NUM_BUTTONS] =
{
	"button1",
	"button2",
	"button3",
	"button4",
	"button5",
	"button6",
	"button7",
	"button8",
	"button9",
	"button10",
	"button11",
	"latchlocked1", // lock only
	"latchunlocked1", // unlock only
	"lightswitch2", // button only
};

#define NUM_DOOR_STOPS 10
string doorstops[NUM_DOOR_STOPS] =
{
	"doorstop1",
	"doorstop2",
	"doorstop3",
	"doorstop4",
	"doorstop5",
	"doorstop6",
	"doorstop7",
	"doorstop8",
	"doorstop9",
	"doorstop10",
};

#define NUM_DOOR_MOVES 10
string doormoves[NUM_DOOR_MOVES] =
{
	"doormove1",
	"doormove2",
	"doormove3",
	"doormove4",
	"doormove5",
	"doormove6",
	"doormove7",
	"doormove8",
	"doormove9",
	"doormove10",
};

#define NUM_TRACK_TRAINS 6
string tracktrains[NUM_TRACK_TRAINS] =
{
	"ttrain1",
	"ttrain2",
	"ttrain3",
	"ttrain4",
	"ttrain6",
	"ttrain7",
};

#define NUM_TRAIN_STOPS 8
string trainstops[NUM_TRAIN_STOPS] =
{
	"bigstop1",
	"bigstop2",
	"freightstop1",
	"heavystop2",
	"rackstop1",
	"railstop1",
	"squeekstop",
	"talkstop",
};

#define NUM_TRAIN_MOVES 13
string trainmoves[NUM_TRAIN_MOVES] =
{
	"bigmove1",
	"bigmove2",
	"elevmove1",
	"elevmove2",
	"elevmove3",
	"freightmove1",
	"freightmove2",
	"heavymove1",
	"rackmove1",
	"railmove1",
	"squeekmove1",
	"talkmove1",
	"talkmove2",
};



#define NUM_ITEMS 13
string items[NUM_ITEMS] =
{
	"9mmclip1",
	"flashlight1",
	"gunpickup2",
	"medcharge4",
	"medshot4",
	"medshot5",
	"medshotno1",
	"smallmedkit1",
	"smallmedkit2",
	"suitcharge1",
	"suitchargeno1",
	"suitchargeok1",
	"weapondrop1",
};

#define NUM_SPECIAL 4
string special[NUM_SPECIAL] =
{
	"!HEV_DEAD0",
	"!HEV_DEAD1",
	"speech/saveme1.wav",
	"speech/grenade1.wav",
	//"items/flashlight1.wav",
	// alien slave attacks
};


#define NUM_GLASS 6
string glass[NUM_GLASS] =
{
	"bustglass1",
	"bustglass2",
	"glass1",
	"glass2",
	"glass3",
	"glass4",
};

#define NUM_WOOD 5
string wood[NUM_WOOD] =
{
	"bustcrate1",
	"bustcrate2",
	"wood1",
	"wood2",
	"wood3",
};

#define NUM_METAL 5
string metal[NUM_METAL] =
{
	"bustmetal1",
	"bustmetal2",
	"metal1",
	//"metal2", in special
	"metal3",
	"metal4",
};

#define NUM_FLESH 8
string flesh[NUM_FLESH] =
{
	"bustflesh1",
	"bustflesh2",
	"flesh1",
	"flesh2",
	"flesh3",
	"flesh4",
	//"flesh5", // for spore
	"flesh6",
	"flesh7",
};

#define NUM_CONCRETE 5
string concrete[NUM_CONCRETE] =
{
	"bustconcrete1",
	"bustconcrete2",
	"concrete1",
	"concrete2",
	"concrete3",
};

#define NUM_TILE 1
string tile[NUM_TILE] =
{
	"bustceiling",
};

// 5 of 60
#define NUM_DEBRIS 5
string debris[NUM_DEBRIS] =
{
	"metal2", // crowbar drop
	"flesh5", // spore drop
	"pushbox1",
	"pushbox2",
	"pushbox3",
};

// 5 of 25 TODO: npc_step changeable?
#define NUM_COMMON 5
string common[NUM_COMMON] = 
{
	"bodydrop3",
	"npc_step1",
	"npc_step2",
	"npc_step3",
	"npc_step4",
};

// 9 of 384
#define NUM_SCIENTIST 14
string scientist[NUM_SCIENTIST] =
{
	"sci_die1",
	"sci_die2",
	"sci_die3",
	"sci_die4",
	"sci_pain1",
	"sci_pain2",
	"sci_pain3",
	"sci_pain4",
	"sci_pain5",
	"sci_pain6",
	"sci_pain7",
	"sci_pain8",
	"sci_pain9",
	"sci_pain10",
};

// 29 out of 178
// Otis also uses these sounds
#define NUM_BARNEY 6
string barney[NUM_BARNEY] = 
{
	"ba_die1",
	"ba_die2",
	"ba_die3",
	"ba_pain1",
	"ba_pain2",
	"ba_pain3",
};

// 5 out of 65
#define NUM_PLAYER 5
string player[NUM_PLAYER] =
{
	"pl_swim1",
	"pl_swim2",
	"pl_swim3",
	"pl_swim4",
	"sprayer",
};

// 17 out of 71
#define NUM_WEAPONS 27
string weapons[NUM_WEAPONS] =
{
	"rocket1",
	"grenade_hit1",
	"grenade_hit2",
	"grenade_hit3",
	"g_bounce2",
	"pwrench_big_hitbod1",
	"pwrench_big_hitbod2",
	"pwrench_big_miss",
	"pwrench_hit1",
	"pwrench_hit2",
	"pwrench_hitbod1",
	"pwrench_hitbod2",
	"pwrench_hitbod3",
	"pwrench_miss1",
	"pwrench_miss2",
	"egon_off1",
	"egon_run3",
	"egon_windup2",
	"mine_activate",
	"mine_charge",
	"mine_deploy",
	"cbar_hit1",
	"cbar_hit2",
	"cbar_hitbod1",
	"cbar_hitbod2",
	"cbar_hitbod3",
	"cbar_miss1",
};

#define NUM_AGRUNT 26
string agrunt[NUM_AGRUNT] =
{
	"ag_alert1",
	"ag_alert2",
	"ag_alert3",
	"ag_alert4",
	"ag_alert5",
	"ag_attack1",
	"ag_attack2",
	"ag_attack3",
	"ag_die1",
	"ag_die2",
	"ag_die3",
	"ag_die4",
	"ag_die5",
	"ag_fire1",
	"ag_fire2",
	"ag_fire3",
	"ag_idle1",
	"ag_idle2",
	"ag_idle3",
	"ag_idle4",
	"ag_idle5",
	"ag_pain1",
	"ag_pain2",
	"ag_pain3",
	"ag_pain4",
	"ag_pain5",
};

#define NUM_ASLAVE 18
string aslave[NUM_ASLAVE] =
{
	"aslave/slv_alert1",
	"aslave/slv_alert3",
	"aslave/slv_alert4",
	"aslave/slv_die1",
	"aslave/slv_die2",
	"aslave/slv_pain1",
	"aslave/slv_pain2",
	"aslave/slv_word1",
	"aslave/slv_word2",
	"aslave/slv_word3",
	"aslave/slv_word4",
	"aslave/slv_word5",
	"aslave/slv_word6",
	"aslave/slv_word7",
	"aslave/slv_word8",
	"debris/zap4", 
	"hassault/hw_shoot1",
	"weapons/electro4",
};

#define NUM_APACHE 5
string apache[NUM_APACHE] =
{
	"ap_rotor1",
	"ap_rotor2",
	"ap_rotor3",
	"ap_rotor4",
	"ap_whine1",
};

#define NUM_BARNACLE 8
string barnacle[NUM_BARNACLE] =
{
	"bcl_alert2",
	"bcl_bite3",
	"bcl_chew1",
	"bcl_chew2",
	"bcl_chew3",
	"bcl_die1",
	"bcl_die3",
	"bcl_tongue1",
};

#define NUM_BULLCHICKEN 26
string bullchicken[NUM_BULLCHICKEN] =
{
	"bc_acid1",
	"bc_acid2",
	"bc_attack1",
	"bc_attack2",
	"bc_attack3",
	"bc_attackgrowl",
	"bc_attackgrowl2",
	"bc_attackgrowl3",
	"bc_bite1",
	"bc_bite2",
	"bc_bite3",
	"bc_die1",
	"bc_die2",
	"bc_die3",
	"bc_idle1",
	"bc_idle2",
	"bc_idle3",
	"bc_idle4",
	"bc_idle5",
	"bc_pain1",
	"bc_pain2",
	"bc_pain3",
	"bc_pain4",
	"bc_spithit1",
	"bc_spithit2",
	"bc_spithit3",
};

#define NUM_CONTROLLER 16
string controller[NUM_CONTROLLER] =
{
	"con_alert1",
	"con_alert2",
	"con_alert3",
	"con_attack1",
	"con_attack2",
	"con_attack3",
	"con_die1",
	"con_die2",
	"con_idle1",
	"con_idle2",
	"con_idle3",
	"con_idle4",
	"con_idle5",
	"con_pain1",
	"con_pain2",
	"con_pain3",
};

#define NUM_GARG 25
string garg[NUM_GARG] =
{
	"gar_alert1",
	"gar_alert2",
	"gar_alert3",
	"gar_attack1",
	"gar_attack2",
	"gar_attack3",
	"gar_breathe1",
	"gar_breathe2",
	"gar_breathe3",
	"gar_die1",
	"gar_die2",
	"gar_flameoff1",
	"gar_flameon1",
	"gar_flamerun1",
	"gar_idle1",
	"gar_idle2",
	"gar_idle3",
	"gar_idle4",
	"gar_idle5",
	"gar_pain1",
	"gar_pain2",
	"gar_pain3",
	"gar_step1",
	"gar_step2",
	"gar_stomp1",
};

#define NUM_GONARCH 22
string gonarch[NUM_GONARCH] =
{
	"gon_alert1",
	"gon_alert2",
	"gon_alert3",
	"gon_attack1",
	"gon_attack2",
	"gon_attack3",
	"gon_birth1",
	"gon_birth2",
	"gon_birth3",
	"gon_childdie1",
	"gon_childdie2",
	"gon_childdie3",
	"gon_die1",
	"gon_pain2",
	"gon_pain4",
	"gon_pain5",
	"gon_sack1",
	"gon_sack2",
	"gon_sack3",
	"gon_step1",
	"gon_step2",
	"gon_step3",
};

#define NUM_HEADCRAB 16
string headcrab[NUM_HEADCRAB] =
{
	"hc_alert1",
	"hc_alert2",
	"hc_attack1",
	"hc_attack2",
	"hc_attack3",
	"hc_die1",
	"hc_die2",
	"hc_headbite",
	"hc_idle1",
	"hc_idle2",
	"hc_idle3",
	"hc_idle4",
	"hc_idle5",
	"hc_pain1",
	"hc_pain2",
	"hc_pain3",
};

#define NUM_ZOMBIE 16
string zombie[NUM_ZOMBIE] =
{
	"claw_miss1",
	"claw_miss2",
	"claw_strike1",
	"claw_strike2",
	"claw_strike3",
	"zo_alert10",
	"zo_alert20",
	"zo_alert30",
	"zo_attack1",
	"zo_attack2",
	"zo_idle1",
	"zo_idle2",
	"zo_idle3",
	"zo_idle4",
	"zo_pain1",
	"zo_pain2",
};

#define NUM_HOUNDEYE 25
string houndeye[NUM_HOUNDEYE] =
{
	"he_alert1",
	"he_alert2",
	"he_alert3",
	"he_attack1",
	"he_attack2",
	"he_attack3",
	"he_blast1",
	"he_blast2",
	"he_blast3",
	"he_die1",
	"he_die2",
	"he_die3",
	"he_hunt1",
	"he_hunt2",
	"he_hunt3",
	"he_hunt4",
	"he_idle1",
	"he_idle2",
	"he_idle3",
	"he_idle4",
	"he_pain1",
	"he_pain2",
	"he_pain3",
	"he_pain4",
	"he_pain5",
};

#define NUM_ICHY 19
string ichy[NUM_ICHY] =
{
	"ichy_alert1",
	"ichy_alert2",
	"ichy_alert3",
	"ichy_attack1",
	"ichy_attack2",
	"ichy_bite1",
	"ichy_bite2",
	"ichy_die1",
	"ichy_die2",
	"ichy_die3",
	"ichy_die4",
	"ichy_idle1",
	"ichy_idle2",
	"ichy_idle3",
	"ichy_idle4",
	"ichy_pain1",
	"ichy_pain2",
	"ichy_pain3",
	"ichy_pain5",
};

#define NUM_LEECH 5
string leech[NUM_LEECH] =
{
	"leech_alert1",
	"leech_alert2",
	"leech_bite1",
	"leech_bite2",
	"leech_bite3",
};

#define NUM_ROACH 3
string roach[NUM_ROACH] =
{
	"rch_die",
	"rch_smash",
	"rch_walk",
};

#define NUM_PITDRONE 26
string pitdrone[NUM_PITDRONE] =
{
	"pit_drone_alert1",
	"pit_drone_alert2",
	"pit_drone_alert3",
	"pit_drone_attack_spike1",
	"pit_drone_attack_spike2",
	"pit_drone_communicate1",
	"pit_drone_communicate2",
	"pit_drone_communicate3",
	"pit_drone_communicate4",
	"pit_drone_die1",
	"pit_drone_die2",
	"pit_drone_die3",
	"pit_drone_eat",
	"pit_drone_hunt1",
	"pit_drone_hunt2",
	"pit_drone_hunt3",
	"pit_drone_idle1",
	"pit_drone_idle2",
	"pit_drone_idle3",
	"pit_drone_melee_attack1",
	"pit_drone_melee_attack2",
	"pit_drone_pain1",
	"pit_drone_pain2",
	"pit_drone_pain3",
	"pit_drone_pain4",
	"pit_drone_run_on_grate",
};

#define NUM_BODYGUARD 45
string bodyguard[NUM_BODYGUARD] =
{
	"ashestoashes",
	"attack2",
	"babysitter",
	"bringiton",
	"die1",
	"die2",
	"die3",
	"getlost",
	"getshitoverwith",
	"happenedhere",
	"hellaimingat",
	"hellareyoudoing",
	"hellhappenedyou",
	"hello",
	"heyoverhere",
	"imbusy",
	"justafleshwound",
	"knockitoff",
	"laterfellas",
	"layeggs",
	"letsmove",
	"looklikeshit",
	"mondays",
	"notaninjury",
	"onassignment",
	"pain1",
	"pain2",
	"pain3",
	"palimworking",
	"pathetic",
	"primalroar",
	"pumpsomelead",
	"seeyalater",
	"shownupdrunk",
	"someonescoming",
	"stop1",
	"stop2",
	"stop3",
	"waitinrighthere",
	"watchyourfire",
	"whathelljoke",
	"whatsgoinon",
	"wounded1",
	"wounded2",
	"yougotit",
};

#define NUM_SHOCKROACH 10
string shockroach[NUM_SHOCKROACH] =
{
	"shock_angry",
	"shock_bite",
	"shock_die",
	"shock_flinch",
	"shock_idle1",
	"shock_idle2",
	"shock_idle3",
	"shock_jump1",
	"shock_jump2",
	"shock_walk",
};

#define NUM_SHOCKTROOPER 28
string shocktrooper[NUM_SHOCKTROOPER] =
{
	"blis",
	"dit",
	"dup",
	"ga",
	"hyu",
	"ka",
	"kiml",
	"kss",
	"ku",
	"kur",
	"kyur",
	"mub",
	"puh",
	"pur",
	"ras",
	"shock_fire",
	"shock_trooper_attack",
	"shock_trooper_die1",
	"shock_trooper_die2",
	"shock_trooper_die3",
	"shock_trooper_die4",
	"shock_trooper_pain1",
	"shock_trooper_pain2",
	"shock_trooper_pain3",
	"shock_trooper_pain4",
	"shock_trooper_pain5",
	"thirv",
	"wirt",
};

#define NUM_KINGPIN 19
string kingpin[NUM_KINGPIN] =
{
	"kingpin_alert1",
	"kingpin_alert2",
	"kingpin_alert3",
	"kingpin_death1",
	"kingpin_death2",
	"kingpin_idle1",
	"kingpin_idle2",
	"kingpin_idle3",
	"kingpin_move",
	"kingpin_moveslow",
	"kingpin_pain1",
	"kingpin_pain2",
	"kingpin_pain3",
	"kingpin_seeker1",
	"kingpin_seeker2",
	"kingpin_seeker3",
	"kingpin_seeker_amb",
	"port_suckin1",
	"port_suckout1",
};

#define NUM_TOR 14
string tor[NUM_TOR] =
{
	"tor-alerted",
	"tor-attack1",
	"tor-attack2",
	"tor-die",
	"tor-die2",
	"tor-foot",
	"tor-idle",
	"tor-idle2",
	"tor-idle3",
	"tor-pain",
	"tor-pain2",
	"tor-staff-discharge",
	"tor-summon",
	"tor-test1",
};

#define NUM_VOLTIGORE 25
string voltigore[NUM_VOLTIGORE] =
{
	"voltigore_alert1",
	"voltigore_alert2",
	"voltigore_alert3",
	"voltigore_attack_melee1",
	"voltigore_attack_melee2",
	"voltigore_attack_shock",
	"voltigore_communicate1",
	"voltigore_communicate2",
	"voltigore_communicate3",
	"voltigore_die1",
	"voltigore_die2",
	"voltigore_die3",
	"voltigore_eat",
	"voltigore_footstep1",
	"voltigore_footstep2",
	"voltigore_footstep3",
	"voltigore_idle1",
	"voltigore_idle2",
	"voltigore_idle3",
	"voltigore_pain1",
	"voltigore_pain2",
	"voltigore_pain3",
	"voltigore_pain4",
	"voltigore_run_grunt1",
	"voltigore_run_grunt2",
};

#define NUM_GONOME 15
string gonome[NUM_GONOME] =
{
	"gonome_death2",
	"gonome_death3",
	"gonome_death4",
	"gonome_eat",
	"gonome_idle1",
	"gonome_idle2",
	"gonome_idle3",
	"gonome_jumpattack",
	"gonome_melee1",
	"gonome_melee2",
	"gonome_pain1",
	"gonome_pain2",
	"gonome_pain3",
	"gonome_pain4",
	"gonome_run",
};

#define NUM_GRUNT 108 // ACK, TOO MANY
string grunt[NUM_GRUNT] =
{
	"alert",
	"allclear",
	"area",
	"ass",
	"babysitting",
	"backup",
	"bfeeling",
	"bogies",
	"busted", // ?
	"charge",
	"check",
	"checkrecon",
	"chicken",
	"clear",
	"command",
	"corners",
	"corporal",
	"corporal_01",
	"cover",
	"covering",
	"coverup",
	"critical", // ?
	"current",
	"damage",
	"death1",
	"death2",
	"death3",
	"death4",
	"death5",
	"death6",
	"disney",
	"dogs",
	"down", // ?
	"flank",
	"freaks",
	"friendly",
	"fubar",
	"fwound",
	"getsome",
	"go",
	"gotit",
	"grenade",
	"gr_pain1",
	"gr_pain2",
	"gr_pain3",
	"gr_pain4",
	"gr_pain5",
	"gr_pain6",
	"guard",
	"guardduty",
	"hear",
	"hearsome",
	"hell",
	"hellosir",
	"hellout",
	"hostiles",
	"incoming",
	"lost",
	"marines",
	"medic",
	"medical",
	"mission",
	"mister",
	"move",
	"moveup",
	"moving",
	"no",
	"notfail",
	"nothing",
	"now",
	"onepiece",
	"oneshot",
	"orders",
	"outof",
	"pain1",
	"pain2",
	"pain3",
	"pain4",
	"pain5",
	"pain6",
	"quiet",
	"rapidfire",
	"recon",
	"right",
	"roger",
	"scout",
	"secure",
	"seensquad",
	"short",
	"sir",
	"siryessir",
	"six",
	"sniper",
	"stayalert",
	"staydown", //?
	"suppressing",
	"sweep",
	"tag",
	"take",
	"takecover",
	"talking",
	"torch_light",
	"tosee",
	"wantsome",
	"watchfire",
	"watchit",
	"yes",
	"zone",
};

#define NUM_TURRET 13
string turret[NUM_TURRET] =
{
	"tu_active",
	"tu_active2",
	"tu_alert",
	"tu_deploy",
	"tu_die",
	"tu_die2",
	"tu_die3",
	"tu_fire1",
	"tu_ping",
	"tu_retract",
	"tu_search",
	"tu_spindown",
	"tu_spinup",
};

#define NUM_NIH 15
string nih[NUM_NIH] =
{
	"nil_alone",
	"nil_comes",
	"nil_deceive",
	"nil_die",
	"nil_done",
	"nil_freeman",
	"nil_last",
	"nil_man_notman",
	"nil_now_die",
	"nil_slaves",
	"nil_thelast",
	"nil_thetruth",
	"nil_thieves",
	"nil_win",
};

#define NUM_ROBOGRUNT 40
string robogrunt[NUM_ROBOGRUNT] =
{
	"danger",
	"bizwarn",
	"emergency",
	"evacuate",
	"dadeda",
	"acquired",
	"target",
	"personnel",
	"unauthorized",
	"alert",
	"buzzwarn",
	"grenade",
	"activated",
	"dirt",
	"die",
	"bag",
	"mister",
	"i",
	"will",
	"kit",
	"your ass",
	"ass",
	"accelerating",
	"engage",
	"status",
	"question",
	"status",
	"green",
	"is",
	"green",
	"search",
	"ing",
	"for",
	"hostile",
	"force",
	"sector",
	"report",
	"check",
	"communication",
	"relay",
};