#include <vector>
#include "Wad.h"
#include "Entity.h"

struct BSP;
struct BSPTEXDATA;

using namespace std;

vector<Wad> getWads();

void create_tex_embed_wad(vector<Wad>& wads);

void get_all_skies();

void writeWad(vector<string> wadTextures, vector<Wad> wads, string mapname);

BSPTEXDATA * genTexLump(vector<string> wadTextures, vector<Wad> wads, BSP * map);

int makeMapWad(BSP * map, string map_name, vector<Wad>& wads);

void embedAllTextures(BSP * map, Entity ** ents);

#define NUM_DEFAULT_WADS 48
static const char * default_wads[NUM_DEFAULT_WADS] = 
{
	"afrikakorps",
	"afrikakorps2",
	"auspices",
	"barney",
	"bmt",
	//"cached",
	"CHANGELEVEL",
	"coopcris",
	"cs_bdog",
	"deathmission",
	//"decals",
	"decay",
	"escape_series",
	"extension",
	"fonts",
	//"gfx",
	"gmantowers",
	"halflife",
	"hammerhead",
	"jailbreak2",
	"kk1",
	"kk2",
	"last",
	"liquids",
	"megamonstermassacre",
	"mistral",
	"neilm2",
	"neilm4",
	"neilm_sc",
	"nightmare",
	"nw",
	"nw_xeno",
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
	"storage",
	"submerged",
	"svencoop1",
	"svencoop2",
	//"tempdecal",
	"vger",
	"xeno",
	"zero",
	"zhlt",
};

#define NUM_SKIES 44
static const char * SKIES[NUM_SKIES] =
{
	"ac_",
	"arcn",
	"carnival",
	"coliseum",
	"desnoon",
	"dfcliff",
	"doom1",
	"dustbowl",
	"fodrian",
	"forest512_",
	"gmcity",
	"grassy",
	"hplanet",
	"hydramanti",
	"parallax-errorlf256_",
	"sandstone",
	"sky_blu_",
	"tetris",
	"theyh2",
	"theyh3",
	"thn",
	"toon",
	"tornsky",
	"twildes",
	"vreality_sky",
	"2desert",
	"alien1",
	"alien2",
	"alien3",
	"black",
	"city",
	"cliff",
	"desert",
	"dusk",
	"morning",
	"neb1",
	"neb2b",
	"neb6",
	"neb7",
	"night",
	"space",
	"xen10",
	"xen8",
	"xen9",
};