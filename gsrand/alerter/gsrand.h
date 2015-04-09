#pragma once

#include "Globals.h"
#include "Util.h"
#include <iostream>
#include <windows.h>
#include <fstream>
#include <time.h>
#include "soundlists.h"
#include "Entity.h"
#include "sentences.h"
#include "masterList.h"
#include "modelList.h"
#include "spriteList.h"
#include "Wad.h"

enum mdl_modes
{
	MDL_NONE,
	MDL_CRAZY,
	MDL_TYPED,
	MDL_MODES,
};

enum snd_modes
{
	SND_NONE,
	SND_ALL,
	SND_WORLD,
	SND_MONST,
	SND_MODES,
};

enum ent_modes
{
	ENT_NONE,
	ENT_SUPER,
	ENT_APPEAR,
	ENT_MODES,
};

enum tex_modes
{
	TEX_NONE,
	TEX_MAP,
	TEX_MASTERWAD,
	TEX_MODES,
};

enum corrupt_modes
{
	CORRUPT_NONE,
	CORRUPT_SUPER,
	CORRUPT_CONFIG,
	CORRUPT_MODES,
};

enum vert_modes { // can be combined
	VERT_FLIP = 1,
	VERT_SCALE = 2,
	VERT_DISTORT = 4,
};

enum lightmap_modes {
	LIGHT_SHIFTED = 1,
	LIGHT_DISCO,
	LIGHT_DARK,
	LIGHT_MODES,
};

enum ctex_modes {
	CTEX_FLAT_COLOR = 1, // scale up so big only a single pixel colors a face
	CTEX_MISALIGN,   // a decent way to torture mappers
	CTEX_WHITE,      // disable all textures (lightmap mode)
	CTEX_GREY,		 // convert all textures to greyscale
	CTEX_BW,         // like that one black and white map
	CTEX_CONTRAST,   // 3-tone colors
	CTEX_INVERT,	 // invert colors
	CTEX_RANDOM,     // randomized palette
	CTEX_MODES,
};

enum prefix_modes
{
	PREFIX_NONE,
	PREFIX_GSRAND,
	PREFIX_TIME,
	PREFIX_CUSTOM,
	PREFIX_MODES,
};

enum content_modes
{
	CONTENT_EVERYTHING, // anything the program can find
	CONTENT_DEFAULT, // default SC 4.8 content only
	CONTENT_CUSTOM, // only non-default SC 4.8 content
	CONTENT_MODES,
};

extern bool verbose;
extern int texMode;
extern int entMode;
extern int sndMode;
extern int mdlMode;
extern int corruptMode;
extern int prefixMode;
extern int contentMode;

#define MAX_MAP_TEXTURES     512
#define MAX_MAP_ENTITIES     8192		// increased from 1024
#define MAX_MAP_MODELS       512 // Really more like 511

#define LUMP_ENTITIES      0
#define LUMP_PLANES        1
#define LUMP_TEXTURES      2
#define LUMP_VERTICES      3
#define LUMP_VISIBILITY    4
#define LUMP_NODES         5
#define LUMP_TEXINFO       6
#define LUMP_FACES         7
#define LUMP_LIGHTING      8
#define LUMP_CLIPNODES     9
#define LUMP_LEAVES       10
#define LUMP_MARKSURFACES 11
#define LUMP_EDGES        12
#define LUMP_SURFEDGES    13
#define LUMP_MODELS       14
#define HEADER_LUMPS      15

struct BSPLUMP
{
	int nOffset; // File offset to data
	int nLength; // Length of data
};

struct BSPHEADER
{
	int nVersion;           // Must be 30 for a valid HL BSP file
	BSPLUMP lump[HEADER_LUMPS]; // Stores the directory of lumps
};

struct BSPTEXDATA
{
	int numTex;
	int * offset;
	int * len;
	WADTEX ** tex;
};

#define BSP_MODEL_BYTES 64 // size of a BSP model in bytes

struct BSP
{
	std::string name;
	BSPHEADER header;
	byte ** lumps;
	//BSPTEXDATA * texdata;
};

enum sprite_formats
{
	SPR_NORMAL,
	SPR_ADDITIVE,
	SPR_INDEXALPHA,
	SPR_ALPHATEST
};

struct SPRHEADER
{
	int ident;			  // should always be "IDSP"
	int version;	  // should be 2
	int mode;		  // see sprite_modes enum
	int format;	  // see sprite_formats enum
	float radius;
	int width;
	int height;
	int frames;
	float beamLength;
	int syncType;    // 0 = synchronized, 1 = random
	//short paletteSz; // always 256??
};

struct membuf : std::streambuf
{
	membuf(char* begin, int len) {
		this->setg(begin, begin, begin + len);
	}
};

#define MAX_REPLACEMENTS 256
#define MAX_SENTENCES 2048

// will only be replaced with sounds from this group id

extern vector<string> masterWadTex;
extern height_hashmap masterWadCorruptions; // matches a texture name to a corruption type
extern string masterWadName;

extern int numOverflow;

extern bool sparks;

extern bool weaps[WEAPON_TYPES];
extern int wsize[WEAPON_TYPES];
extern const char ** wlists[WEAPON_TYPES];

extern string * voice[NUM_VOICE_DIRS];
extern int vsize[NUM_VOICE_DIRS];

extern vector<string> ambients;
extern bool ftypes[NUM_FANS];

extern bool rbutts[NUM_ROT_BUTTONS];
extern bool butts[NUM_BUTTONS]; // lol butts..

extern bool dmove[NUM_DOOR_MOVES];
extern bool dstop[NUM_DOOR_STOPS];

extern bool ttrain[NUM_TRACK_TRAINS];
extern bool tmove[NUM_TRAIN_MOVES];
extern bool tstop[NUM_TRAIN_STOPS];

extern bool btypes[BREAKABLE_TYPES];
extern int bsize[BREAKABLE_TYPES];
extern const char ** blists[BREAKABLE_TYPES];

extern string mname[MONSTER_TYPES];
extern int msize[MONSTER_TYPES];
extern const char ** mlists[MONSTER_TYPES];
extern string mdirs[MONSTER_TYPES];

extern int monsters[MONSTER_TYPES];
extern int total_map_models;

extern string_hashmap random_monster_models;
extern string_hashmap random_weapon_models;

typedef tr1::unordered_map< string, vector<string> > list_hashmap;

// lists of everything the program could find
extern vector<string> user_sounds; 
extern vector<string> user_sound_dirs; // for sentences
extern list_hashmap user_voices;
extern vector<string> user_animated_sprites;
extern vector<string> user_sprites;
extern vector<string> user_skies; 
extern vector<string> user_monster_models;
extern vector<string> user_prop_models;
extern vector<string> user_v_models;
extern vector<string> user_p_models;
extern vector<string> user_w_models;
extern vector<string> user_apache_models;
extern vector<string> user_player_models;
extern string_hashmap every_random_replacement; // every entity type gets a single model replacement

extern set<string> res_list;
extern set<string> res_sentence_list; // lower priority
extern set<string> super_res_list; // every file needed to play the randomized maps

extern int total_model_count;

extern string MAP_PREFIX;

enum grapple_modes
{
	GRAPPLE_DISABLE,
	GRAPPLE_NORMAL, // no wall hooking
	GRAPPLE_HOOK, // all walls can be grappled
	GRAPPLE_HOOK_ALWAYS, // no matter what settings are use
};

enum skill_modes
{
	SKILL_DEFAULT,
	SKILL_SOME_SUPER,
	SKILL_ALL_SUPER,
};

enum tex_embed_modes
{
	EMBED_DISABLE,
	EMBED_NORMAL,
	EMBED_ONLY, // only embedded textures will be used	
};

enum model_safety_modes
{
	MODEL_SAFETY_NONE,
	MODEL_SAFETY_TEX_LIMIT,
	MODEL_SAFETY_GLOBAL_ONLY,
};

extern bool gmr_only;
extern int grapple_mode;
extern int modelSafety;
extern bool superRandom;
extern bool printRejects;
extern bool cheatNoclip;
extern bool cheatImpulse;
extern bool cheatGodmode;
extern bool singleplayer;
extern bool bypassHlsp;
extern int bspModelSwap;
extern int sndEffects;
extern int lightMode;
extern int vertMode;
extern int ctexMode;
extern float vertDistort;
extern float vertScaleX, vertScaleY, vertScaleZ;
extern int wepSkillMode, monSkillMode;
extern int sentenceMode;
extern int earRapeMode;

enum model_types
{
	MODEL_TYPE_MONSTER,
	MODEL_TYPE_WEAPON,
	MODEL_TYPE_GENERIC, // also means "I don't know"
	MODEL_TYPE_PROP,
	MODEL_TYPE_P_WEAPON,
	MODEL_TYPE_V_WEAPON,
	MODEL_TYPE_W_WEAPON,
};

enum sprite_types
{
	SPRITE_TYPE_STATIC,
	SPRITE_TYPE_ANIMATED,
	SPRITE_TYPE_GENERIC
};

void filter_default_content(vector<string>& unfiltered, const char ** default_list, int num_default);

Keyvalue extractKeyvalue(std::string line);

byte * loadTextureChunk(string mapname, int& lump_len);

BSP * loadBSP(std::string mapname, bool loadAll);

void loadLumpBackup(BSP * map, int lump, std::string suffix);

void saveLumpBackup(BSP * map, int lump, std::string suffix);

int randomize_maps(); // where it all begins

void undoEverything();

#define NUM_ESCAPE_MAPS 12
static const char * escape_maps[NUM_ESCAPE_MAPS] =
{
	"escape_series_1a",
	"escape_series_1b",
	"escape_series_2a",
	"escape_series_2b",
	"escape_series_2c",
	"escape_series_2d",
	"escape_series_2e",
	"escape_series_3a",
	"escape_series_3b",
	"escape_series_3c",
	"escape_series_3d",
	"escape_series_3e",
};

#define NUM_HUNGER_MAPS 30
static const char * hunger_maps[NUM_HUNGER_MAPS] =
{
	"-They_Hunger_Ep1",
	"they3a",
	"they3b",
	"they4",
	"they5",
	"they6",
	"they7",
	"they9",
	"they10",
	"they11",
	"they12",
	"they13",
	"they14",
	"they15",
	"they16",
	"they17",
	"they18",
	"they19",
	"they20",
	"they21",
	"they21b",
	"they22",
	"they22b",
	"they23",
	"they24",
	"they25",
	"they26",
	"they27",
	"they28",
	"they29",
};

#define NUM_BLUESHIFT_MAPS 31
static const char * blueshift_maps[NUM_BLUESHIFT_MAPS] =
{
	"-blueshift",
	"ba_canal1",
	"ba_canal1b",
	"ba_canal2",
	"ba_canal3",
	"ba_elevator",
	"ba_maint",
	"ba_outro",
	"ba_power1",
	"ba_power2",
	"ba_security1",
	"ba_security2",
	"ba_teleport1",
	"ba_teleport2",
	"ba_tram2",
	"ba_tram3",
	"ba_xen1",
	"ba_xen2",
	"ba_xen3",
	"ba_xen4",
	"ba_xen5",
	"ba_xen6",
	"ba_yard1",
	"ba_yard2",
	"ba_yard3",
	"ba_yard3a",
	"ba_yard3b",
	"ba_yard4",
	"ba_yard4a",
	"ba_yard5",
	"ba_yard5a",
};

#define NUM_OP4_MAPS 35
static const char * op4_maps[NUM_OP4_MAPS] =
{
	"of0a0",
	"of1a1",
	"of1a2",
	"of1a3",
	"of1a4",
	"of1a4b",
	"of1a5",
	"of1a5b",
	"of1a6",
	"of2a1",
	"of2a1b",
	"of2a2",
	"of2a4",
	"of2a5",
	"of2a6",
	"of3a1",
	"of3a2",
	"of3a4",
	"of3a5",
	"of3a6",
	"of4a1",
	"of4a2",
	"of4a3",
	"of4a4",
	"of4a5",
	"of5a1",
	"of5a2",
	"of5a3",
	"of5a4",
	"of6a1",
	"of6a2",
	"of6a3",
	"of6a4",
	"of6a4b",
	"of6a5",
};

#define NUM_HLSP_MAPS 100
static const char * hlsp_maps[NUM_HLSP_MAPS] =
{
	"c0a0",
	"c0a0a",
	"c0a0b",
	"c0a0c",
	"c0a0d",
	"c0a0e",
	"c1a0",
	"c1a0a",
	"c1a0b",
	"c1a0c",
	"c1a0d",
	"c1a0e",
	"c1a1",
	"c1a1a",
	"c1a1b",
	"c1a1c",
	"c1a1d",
	"c1a1f",
	"c1a2",
	"c1a2a",
	"c1a2b",
	"c1a2c",
	"c1a2d",
	"c1a2e",
	"c1a3",
	"c1a3a",
	"c1a3b",
	"c1a3c",
	"c1a3d",
	"c1a3e",
	"c1a4",
	"c1a4b",
	"c1a4d",
	"c1a4e",
	"c1a4f",
	"c1a4g",
	"c1a4i",
	"c1a4j",
	"c1a4k",
	"c1a4z",
	"c2a1",
	"c2a1a",
	"c2a1b",
	"c2a2",
	"c2a2a",
	"c2a2b1",
	"c2a2b2",
	"c2a2c",
	"c2a2d",
	"c2a2e",
	"c2a2f",
	"c2a2g",
	"c2a2h",
	"c2a3",
	"c2a3a",
	"c2a3b",
	"c2a3c",
	"c2a3d",
	"c2a3e",
	"c2a4",
	"c2a4a",
	"c2a4b",
	"c2a4c",
	"c2a4d",
	"c2a4e",
	"c2a4f",
	"c2a4g",
	"c2a4h",
	"c2a5",
	"c2a5a",
	"c2a5b",
	"c2a5c",
	"c2a5d",
	"c2a5e",
	"c2a5f",
	"c2a5g",
	"c2a5w",
	"c2a5x",
	"c3a1",
	"c3a1a",
	"c3a1b",
	"c3a2",
	"c3a2a",
	"c3a2b",
	"c3a2c",
	"c3a2d",
	"c3a2e",
	"c3a2f",
	"c4a1",
	"c4a1a",
	"c4a1b",
	"c4a1c",
	"c4a1d",
	"c4a1e",
	"c4a1f",
	"c4a2",
	"c4a2a",
	"c4a2b",
	"c4a3",
	"c5a1",
};

#define NUM_PLR_EXPLOSIVE_SKILL_SETTINGS 7
static const char * plr_explosive_skill_settings[NUM_PLR_EXPLOSIVE_SKILL_SETTINGS] =
{
	"sk_plr_9mmAR_grenade",
	"sk_plr_rpg",
	"sk_plr_hand_grenade",
	"sk_plr_satchel",
	"sk_plr_tripmine",
	"sk_plr_spore",
	"sk_shockroach_dmg_xpl_splash",
};

#define NUM_PLR_SKILL_SETTINGS 20
static const char * plr_skill_settings[NUM_PLR_SKILL_SETTINGS] =
{
	"sk_plr_crowbar",
	"sk_plr_9mm_bullet",
	"sk_plr_357_bullet",
	"sk_plr_9mmAR_bullet",
	"sk_plr_buckshot",
	"sk_plr_xbow_bolt_monster",
	"sk_plr_gauss",
	"sk_plr_egon_narrow",
	"sk_plr_egon_wide",
	"sk_plr_HpMedic",
	"sk_plr_wrench",
	"sk_plr_grapple",
	"sk_plr_uzi",
	"sk_556_bullet",
	"sk_plr_secondarygauss",
	"sk_hornet_pdmg",
	"sk_plr_762_bullet",
	"sk_plr_shockrifle",
	"sk_plr_shockrifle_beam",
	"sk_shockroach_dmg_xpl_touch",
};

#define NUM_MON_DODGE_SKILL_SETTINGS 56
static const char * mon_dodge_skill_settings[NUM_MON_DODGE_SKILL_SETTINGS] =
{
	"sk_agrunt_dmg_punch",
	"sk_bullsquid_dmg_bite",
	"sk_bullsquid_dmg_whip",
	"sk_bullsquid_dmg_spit",
	"sk_bigmomma_dmg_slash",
	"sk_bigmomma_dmg_blast",
	"sk_bigmomma_radius_blast",
	"sk_gargantua_dmg_slash",
	"sk_gargantua_dmg_fire",
	"sk_gargantua_dmg_stomp",
	"sk_headcrab_dmg_bite",
	"sk_hgrunt_kick",
	"sk_houndeye_dmg_blast",
	"sk_islave_dmg_claw",
	"sk_islave_dmg_clawrake",
	"sk_islave_dmg_zap",
	"sk_ichthyosaur_shake",
	"sk_leech_dmg_bite",
	"sk_controller_dmgzap",
	"sk_controller_speedball",
	"sk_controller_dmgball",
	"sk_nihilanth_zap",
	"sk_snark_dmg_bite",
	"sk_snark_dmg_pop",
	"sk_zombie_dmg_one_slash",
	"sk_zombie_dmg_both_slash",
	"sk_babygargantua_dmg_slash",
	"sk_babygargantua_dmg_fire",
	"sk_babygargantua_dmg_stomp",
	"sk_rgrunt_explode",
	"sk_massassin_sniper",
	"sk_zombie_barney_dmg_one_slash",
	"sk_zombie_barney_dmg_both_slash",
	"sk_barnacle_bite",
	"sk_zombie_soldier_dmg_one_slash",
	"sk_zombie_soldier_dmg_both_slash",
	"sk_gonome_dmg_one_slash",
	"sk_gonome_dmg_guts",
	"sk_gonome_dmg_one_bite",
	"sk_pitdrone_dmg_bite",
	"sk_pitdrone_dmg_whip",
	"sk_pitdrone_dmg_spit",
	"sk_shocktrooper_kick",
	"sk_shocktrooper_maxcharge",
	"sk_tor_punch",
	"sk_tor_sonicblast",
	"sk_voltigore_dmg_punch",
	"sk_voltigore_dmg_beam",
	"sk_voltigore_dmg_explode",
	"sk_tentacle",
	"sk_stukabat",
	"sk_kingpin_tele_blast",
	"sk_kingpin_plasma_blast",
	"sk_kingpin_melee",
	"sk_kingpin_telefrag",
	"sk_hornet_dmg",
};

#define NUM_MON_SKILL_SETTINGS 66
static const char * mon_skill_settings[NUM_MON_SKILL_SETTINGS] =
{
	"sk_agrunt_dmg_punch",
	"sk_bullsquid_dmg_bite",
	"sk_bullsquid_dmg_whip",
	"sk_bullsquid_dmg_spit",
	"sk_bigmomma_dmg_slash",
	"sk_bigmomma_dmg_blast",
	"sk_bigmomma_radius_blast",
	"sk_gargantua_dmg_slash",
	"sk_gargantua_dmg_fire",
	"sk_gargantua_dmg_stomp",
	"sk_headcrab_dmg_bite",
	"sk_hgrunt_kick",
	"sk_hgrunt_pellets",
	"sk_hgrunt_gspeed",
	"sk_houndeye_dmg_blast",
	"sk_islave_dmg_claw",
	"sk_islave_dmg_clawrake",
	"sk_islave_dmg_zap",
	"sk_ichthyosaur_shake",
	"sk_leech_dmg_bite",
	"sk_controller_dmgzap",
	"sk_controller_speedball",
	"sk_controller_dmgball",
	"sk_nihilanth_zap",
	"sk_snark_dmg_bite",
	"sk_snark_dmg_pop",
	"sk_zombie_dmg_one_slash",
	"sk_zombie_dmg_both_slash",
	"sk_grunt_buckshot",
	"sk_babygargantua_dmg_slash",
	"sk_babygargantua_dmg_fire",
	"sk_babygargantua_dmg_stomp",
	"sk_hwgrunt_minipellets",
	"sk_rgrunt_explode",
	"sk_massassin_sniper",
	"sk_otis_bullet",
	"sk_zombie_barney_dmg_one_slash",
	"sk_zombie_barney_dmg_both_slash",
	"sk_barnacle_bite",
	"sk_zombie_soldier_dmg_one_slash",
	"sk_zombie_soldier_dmg_both_slash",
	"sk_gonome_dmg_one_slash",
	"sk_gonome_dmg_guts",
	"sk_gonome_dmg_one_bite",
	"sk_pitdrone_dmg_bite",
	"sk_pitdrone_dmg_whip",
	"sk_pitdrone_dmg_spit",
	"sk_shocktrooper_kick",
	"sk_shocktrooper_maxcharge",
	"sk_tor_punch",
	"sk_tor_energybeam",
	"sk_tor_sonicblast",
	"sk_voltigore_dmg_punch",
	"sk_voltigore_dmg_beam",
	"sk_voltigore_dmg_explode",
	"sk_tentacle",
	"sk_stukabat",
	"sk_kingpin_lightning",
	"sk_kingpin_tele_blast",
	"sk_kingpin_plasma_blast",
	"sk_kingpin_melee",
	"sk_kingpin_telefrag",
	"sk_12mm_bullet",
	"sk_9mmAR_bullet",
	"sk_9mm_bullet",
	"sk_hornet_dmg",
};