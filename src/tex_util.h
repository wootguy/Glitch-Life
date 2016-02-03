#include <vector>
#include "Wad.h"
#include "Entity.h"

struct BSP;
struct BSPTEXDATA;

using namespace std;

vector<Wad> getWads();

void gen_default_wad_list();

void create_tex_embed_wad(vector<Wad>& wads);

void get_all_skies();

void writeWad(vector<string>& wadTextures, vector<Wad>& wads, string mapname);

BSPTEXDATA * genTexLump(vector<string> wadTextures, vector<Wad>& wads, BSP * map);

int makeMapWad(BSP * map, string map_name, vector<Wad>& wads);

vector<string> unEmbedAllTextures(BSP * map, int& grapple_id, int& global_id);

void embedAllTextures(BSP * map, Entity ** ents);

#define NUM_DEFAULT_WADS 57
static const char * default_wads[NUM_DEFAULT_WADS] =
{
	"barney",
	"bmt",
	"CHANGELEVEL",
	"coopcris",
	"cs_bdog",
	"deathmission",
	"decals",
	"decay",
	"descrcl",
	"dmc",
	"extension",
	"gmantowers",
	"halflife",
	"halflifemerged",
	"jailbreak2",
	"jp_dev",
	"jp_security",
	"kk1",
	"kk2",
	"last",
	"lcd_display",
	"liquids",
	"megamonstermassacre",
	"mistral",
	"mommamesa",
	"neilm2",
	"neilm4",
	"neilm5",
	"neilm_sc",
	"nightmare",
	"nih_fort",
	"nih_hex_interface",
	"nih_silowad",
	"nw",
	"nw_xeno",
	"op4ctf",
	"Opfor",
	"osprey",
	"quarter",
	"sandstone",
	"scopfor",
	"scrpg2",
	"sc_mazing",
	"sc_tetris",
	"sectore",
	"spraypaint",
	"stadium",
	"stadium4",
	"storage",
	"submerged",
	"svencoop1",
	"svencoop2",
	"svencoop_training",
	"tfc",
	"tfc2",
	"xeno",
	"zhlt",
};

#define NUM_SKIES 43
static const char * SKIES[NUM_SKIES] =
{
	"2desert",
	"ac_",
	"alien1",
	"alien2",
	"alien3",
	"arcn",
	"black",
	"carnival",
	"city",
	"cliff",
	"coliseum",
	"desert",
	"desnoon",
	"dfcliff",
	"doom1",
	"dusk",
	"dustbowl",
	"fodrian",
	"forest512_",
	"gmcity",
	"grassy",
	"hplanet",
	"morning",
	"neb1",
	"neb2b",
	"neb6",
	"neb7",
	"night",
	"parallax-errorlf256_",
	"sandstone",
	"sky_blu_",
	"space",
	"tetris",
	"theyh2",
	"theyh3",
	"thn",
	"toon",
	"tornsky",
	"twildes",
	"vreality_sky",
	"xen10",
	"xen8",
	"xen9",
};