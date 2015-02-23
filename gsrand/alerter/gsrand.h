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


bool verbose = false;
bool superRandom = false;
int texMode = 0;
int entMode = 0;
int sndMode = 0;
int mdlMode = 0;
bool usePrefix = false;

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
	TEX_WADS,
	TEX_MASTERWAD,
	TEX_MODES,
};

//string wadPath = "wad/";
string wadPath = "";

#define NUM_DEFAULT_WADS 48
std::string default_wads[NUM_DEFAULT_WADS] = 
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
	BSPTEXDATA * texdata;
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

struct MDLHEADER 
{
	int					id;
	int					version;

	char				name[64];
	int					length;

	vec3				eyeposition;	// ideal eye position
	vec3				min;			// ideal movement hull size
	vec3				max;			

	vec3				bbmin;			// clipping bounding box
	vec3				bbmax;		

	int					flags;

	int					numbones;			// bones
	int					boneindex;

	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;

	int					numhitboxes;			// complex bounding boxes
	int					hitboxindex;			

	int					numseq;				// animation sequences
	int					seqindex;

	int					numseqgroups;		// demand loaded sequences
	int					seqgroupindex;

	int					numtextures;		// raw textures
	int					textureindex;
	int					texturedataindex;

	int					numskinref;			// replaceable textures
	int					numskinfamilies;
	int					skinindex;

	int					numbodyparts;		
	int					bodypartindex;

	int					numattachments;		// queryable attachable points
	int					attachmentindex;

	int					soundtable;
	int					soundindex;
	int					soundgroups;
	int					soundgroupindex;

	int					numtransitions;		// animation node to animation node transition graph
	int					transitionindex;
};

struct MDLSEQUENCE
{
	char				label[32];	// sequence label

	float				fps;		// frames per second	
	int					flags;		// looping/non-looping flags

	int					activity;
	int					actweight;

	int					numevents;
	int					eventindex;

	int					numframes;	// number of frames per sequence

	int					numpivots;	// number of foot pivots
	int					pivotindex;

	int					motiontype;	
	int					motionbone;
	vec3				linearmovement;
	int					automoveposindex;
	int					automoveangleindex;

	vec3				bbmin;		// per sequence bounding box
	vec3				bbmax;		

	int					numblends;
	int					animindex;		// mstudioanim_t pointer relative to start of sequence group data
	// [blend][bone][X, Y, Z, XR, YR, ZR]

	int					blendtype[2];	// X, Y, Z, XR, YR, ZR
	float				blendstart[2];	// starting value
	float				blendend[2];	// ending value
	int					blendparent;

	int					seqgroup;		// sequence group for demand loading

	int					entrynode;		// transition node at entry
	int					exitnode;		// transition node at exit
	int					nodeflags;		// transition rules

	int					nextseq;		// auto advancing sequences
};

struct membuf : std::streambuf
{
	membuf(char* begin, int len) {
		this->setg(begin, begin, begin + len);
	}
};

const int num_exts = 16;
std::string exts[num_exts] = {"AIFF", "ASF", "DLS", "FLAC", "IT", "M3U", "MID", "MOD", 
						 "MP2",	 "MP3", "OGG", "S3M", "VAG", "WAV", "WMA", "XM"};

#define MAX_REPLACEMENTS 256
#define MAX_SENTENCES 2048

// will only be replaced with sounds from this group id

vector<string> masterWadTex;

int numOverflow = 0;

bool sparks;

bool weaps[WEAPON_TYPES];
int wsize[WEAPON_TYPES];
std::string * wlists[WEAPON_TYPES];

std::string * voice[NUM_VOICE_DIRS];
int vsize[NUM_VOICE_DIRS];

vector<std::string> ambients;
bool ftypes[NUM_FANS];

bool rbutts[NUM_ROT_BUTTONS];
bool butts[NUM_BUTTONS]; // lol butts..

bool dmove[NUM_DOOR_MOVES];
bool dstop[NUM_DOOR_STOPS];

bool ttrain[NUM_TRACK_TRAINS];
bool tmove[NUM_TRAIN_MOVES];
bool tstop[NUM_TRAIN_STOPS];

bool btypes[BREAKABLE_TYPES];
int bsize[BREAKABLE_TYPES];
std::string * blists[BREAKABLE_TYPES];

string mname[MONSTER_TYPES];
int msize[MONSTER_TYPES];
std::string * mlists[MONSTER_TYPES];
std::string mdirs[MONSTER_TYPES];

int monsters[MONSTER_TYPES];
int total_map_models = 0;

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

void getAllSounds();

void getAllVoices();

vector<sound> generateWritable(bool printInfo);

void writeGSR(std::string filename, vector<sound> writeList);

int updateCFG(std::string path, std::string mapname, bool addNotRemove, std::string wadstring);

Keyvalue extractKeyvalue(std::string line);

BSP * loadBSP(std::string mapname);

void loadLumpBackup(BSP * map, int lump, std::string suffix);

void saveLumpBackup(BSP * map, int lump, std::string suffix);

void ripent(BSP * map, Entity** entData);

bool needsRipent(Entity** ents);

Entity** getMapEnts(BSP * map, bool printInfo, int& numEnts);

void tallyMonsters(Entity** ents, string mapname);

std::string constructSentence();

void writeSentences(std::string mapName);

int checkBSPs();

void formatSentences();

void undoEverything();

void recurseSubdirs(std::string path, vector<std::string>& dirs);

vector<std::string> getAllSubdirs(std::string path);

void genSoundList();

vector<Wad> getWads(bool defaultOnly);

void writeWad(vector<string> wadTextures, vector<Wad> wads, string mapname);

BSPTEXDATA * genTexLump(vector<string> wadTextures, vector<Wad> wads, BSP * map);

int makeMapWad(BSP * map, vector<Wad>& wads);

// find out what kind of model the entitiy use
int getModelType(string entity_name);

string_hashmap random_monster_models;
string_hashmap random_weapon_models;

void init_random_monster_models();
