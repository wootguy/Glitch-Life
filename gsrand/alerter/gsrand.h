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
	TEX_WADS,
	TEX_MASTERWAD,
	TEX_MODES,
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
extern bool superRandom;
extern int texMode;
extern int entMode;
extern int sndMode;
extern int mdlMode;
extern int prefixMode;
extern int contentMode;

//string wadPath = "wad/";
extern string wadPath;

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

#define MAX_REPLACEMENTS 256
#define MAX_SENTENCES 2048

// will only be replaced with sounds from this group id

extern vector<string> masterWadTex;

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

extern vector<string> user_sounds; // a list of every sound the program could find
extern vector<string> user_models; // a list of every model the program could find
extern vector<string> user_skies; // a list of every sky the program could find

extern bool barnacle_grapple_hook; // renames all textures to "xeno_grapple"
extern string MAP_PREFIX;

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

Keyvalue extractKeyvalue(std::string line);

BSP * loadBSP(std::string mapname);

void loadLumpBackup(BSP * map, int lump, std::string suffix);

void saveLumpBackup(BSP * map, int lump, std::string suffix);

int randomize_maps(); // where it all begins

void undoEverything();
