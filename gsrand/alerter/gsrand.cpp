// alerter.cpp : Defines the entry point for the console application.
//
#include "gsrand.h"
#include <sys/stat.h>
#include <conio.h>
#include <iomanip>
#include <algorithm>
#include "sound_util.h"
#include "model_util.h"
#include "tex_util.h"
#include "ent_util.h"
#include "bsp_util.h"

bool verbose = false;
bool superRandom = false;
int texMode = 0;
int entMode = 0;
int sndMode = 0;
int mdlMode = 0;
int corruptMode = 0;
int prefixMode = PREFIX_GSRAND;
int contentMode = 0;

string gsrand_motd_identifier;
string gsrand_res_identifier;
string gsrand_skl_identifier;

bool gmr_only = false;
bool printRejects = false;
bool bypassHlsp = true;
int grapple_mode = GRAPPLE_HOOK;
int tex_embed_mode = EMBED_NORMAL;
string MAP_PREFIX = "gsrand_";
string random_seed;
int modelSafety = MODEL_SAFETY_TEX_LIMIT;
bool cheatNoclip = false;
bool cheatImpulse = false;
bool cheatGodmode = false;
bool singleplayer = false;
int bspModelSwap = 0;
int sndEffects = 0;
int lightMode = 0;
int vertMode = 0;
int ctexMode = 0;
float vertScaleX = 0.5f;
float vertScaleY = 0.5f;
float vertScaleZ = 0.5f;
float vertDistort = 4;
int wepSkillMode = 1;
int monSkillMode = 1;
int sentenceMode = 0;

int numOverflow = 0;
bool sparks;
bool weaps[WEAPON_TYPES];
int wsize[WEAPON_TYPES];
const char ** wlists[WEAPON_TYPES];
string * voice[NUM_VOICE_DIRS];
int vsize[NUM_VOICE_DIRS];
bool ftypes[NUM_FANS];
bool rbutts[NUM_ROT_BUTTONS];
bool butts[NUM_BUTTONS];
bool dmove[NUM_DOOR_MOVES];
bool dstop[NUM_DOOR_STOPS];
bool ttrain[NUM_TRACK_TRAINS];
bool tmove[NUM_TRAIN_MOVES];
bool tstop[NUM_TRAIN_STOPS];
bool btypes[BREAKABLE_TYPES];
int bsize[BREAKABLE_TYPES];
const char ** blists[BREAKABLE_TYPES];
string mname[MONSTER_TYPES];
int msize[MONSTER_TYPES];
const char ** mlists[MONSTER_TYPES];
string mdirs[MONSTER_TYPES];
int monsters[MONSTER_TYPES];
int total_map_models = 0;

vector<string> user_maps;
vector<string> user_sounds;
vector<string> user_sound_dirs;
list_hashmap user_voices;
vector<string> user_models;
vector<string> user_skies;
vector<string> user_animated_sprites;
vector<string> user_sprites;
vector<string> user_monster_models;
vector<string> user_prop_models;
vector<string> user_v_models;
vector<string> user_p_models;
vector<string> user_w_models;
vector<string> user_apache_models;
vector<string> user_player_models;
string_hashmap every_random_replacement;
int total_model_count = 0;

vector<string> masterWadTex;
vector<string> ambients;

string_hashmap random_monster_models;
string_hashmap random_weapon_models;

string_hashmap default_w_weapon_models;
string_hashmap default_p_weapon_models;
string_hashmap default_v_weapon_models;

// model vars
string_hashmap default_monster_models;
string_hashmap default_friendly_monster_models;
string_hashmap default_weapon_models;
string_hashmap default_ammo_models;
string_hashmap default_item_models;
vector<string> default_gib_models;
vector<string> default_precache_models; // models that are precached by default
vector<string> gmr_replace_only; // models only replacable through GMR
vector<string> weapon_blacklist;
vector<string> hwgrunt_blacklist;
vector<string> nih_whitelist; // w_ and player models are ok but not in this list
vector<string> controller_whitelist; // w_ and player models also ok
vector<string> monster_sprites;
vector<string> dont_replace; // shouldn't be replaced
vector<string> weapon_types;
height_hashmap default_monster_heights;

vector<string> default_content;
set<string> res_list;
set<string> res_sentence_list;
set<string> super_res_list;

list_hashmap monster_whitelists;
list_hashmap monster_blacklists;

string_hashmap filename_cache;

DateTime generation_date;

void readConfigFile()
{
	if (!fileExists("gsrand.cfg"))
	{
		println("No config file found");
		return;
	}
}

void initLists()
{
	init_default_model_lists();

	wlists[WEP_PIPEWRENCH] = wrench;
	wlists[WEP_PIPEWRENCH] = crowbar;
	wlists[WEP_PIPEWRENCH] = mine;
	wlists[WEP_PIPEWRENCH] = egon;

	wsize[WEP_PIPEWRENCH] = NUM_WRENCH;
	wsize[WEP_PIPEWRENCH] = NUM_CROWBAR;
	wsize[WEP_PIPEWRENCH] = NUM_MINE;
	wsize[WEP_PIPEWRENCH] = NUM_EGON;

	blists[GLASS] = glass;
	blists[WOOD] = wood;
	blists[METAL] = metal;
	blists[FLESH] = flesh;
	blists[CONCRETE] = concrete;
	blists[TILE] = tile;
	//blists[COMPUTER] = computer;
	//blists[UNBREAKABLE] = glass;
	//blists[ROCK] = concrete;
	
	bsize[GLASS] = NUM_GLASS;
	bsize[WOOD] = NUM_WOOD;
	bsize[METAL] = NUM_METAL;
	bsize[FLESH] = NUM_FLESH;
	bsize[CONCRETE] = NUM_CONCRETE;
	bsize[TILE] = NUM_TILE;

	mlists[AGRUNT] = agrunt;
	mlists[APACHE] = apache;
	mlists[BARNACLE] = barnacle;
	mlists[BULLCHICKEN] = bullchicken;
	mlists[CONTROLLER] = controller;
	mlists[GARG] = garg;
	mlists[BABYGARG] = garg;
	mlists[GONARCH] = gonarch;
	mlists[HEADCRAB] = headcrab;
	mlists[ZOMBIE] = zombie;
	mlists[HOUNDEYE] = houndeye;
	mlists[ICHY] = ichy;
	mlists[LEECH] = leech;
	mlists[ROACH] = roach;
	mlists[PITDRONE] = pitdrone;
	mlists[KINGPIN] = kingpin;
	mlists[TOR] = tor;
	mlists[VOLTIGORE] = voltigore;
	mlists[GONOME] = gonome;
	//mlists[OTIS] = otis;
	mlists[GRUNT] = grunt;
	mlists[TURRET] = turret;
	mlists[NIH] = nih;
	mlists[ROBOGRUNT] = robogrunt;
	mlists[SHOCKROACH] = shockroach;
	mlists[SHOCKTROOPER] = shocktrooper;
	mlists[BODYGUARD] = bodyguard;
	mlists[ASLAVE] = aslave;
	mlists[SCIENTIST] = scientist;
	//mlists[HASSAULT] = hassault;
	mlists[BARNEY] = barney;

	mdirs[AGRUNT] = "agrunt";
	mdirs[APACHE] = "apache";
	mdirs[BARNACLE] = "barnacle";
	mdirs[BULLCHICKEN] = "bullchicken";
	mdirs[CONTROLLER] = "controller";
	mdirs[GARG] = "garg";
	mdirs[BABYGARG] = "babygarg";
	mdirs[GONARCH] = "gonarch";
	mdirs[HEADCRAB] = "headcrab";
	mdirs[ZOMBIE] = "zombie";
	mdirs[HOUNDEYE] = "houndeye";
	mdirs[ICHY] = "ichy";
	mdirs[LEECH] = "leech";
	mdirs[ROACH] = "roach";
	mdirs[PITDRONE] = "pitdrone";
	mdirs[KINGPIN] = "kingpin";
	mdirs[TOR] = "tor";
	mdirs[VOLTIGORE] = "voltigore";
	mdirs[GONOME] = "gonome";
	//mdirs[OTIS] = "otis";
	mdirs[GRUNT] = "fgrunt";
	mdirs[TURRET] = "turret";
	mdirs[NIH] = "nihilanth";
	mdirs[ROBOGRUNT] = "vox";
	mdirs[SHOCKROACH] = "shockroach";
	mdirs[SHOCKTROOPER] = "shocktrooper";
	mdirs[BODYGUARD] = "bodyguard";
	mdirs[ASLAVE] = "aslave";
	mdirs[SCIENTIST] = "scientist";
	//mdirs[HASSAULT] = "hassault";
	mdirs[BARNEY] = "barney";

	msize[AGRUNT] = NUM_AGRUNT;
	msize[APACHE] = NUM_APACHE;
	msize[BARNACLE] = NUM_BARNACLE;
	msize[BULLCHICKEN] = NUM_BULLCHICKEN;
	msize[CONTROLLER] = NUM_CONTROLLER;
	msize[GARG] = NUM_GARG;
	msize[BABYGARG] = NUM_GARG;
	msize[GONARCH] = NUM_GONARCH;
	msize[HEADCRAB] = NUM_HEADCRAB;
	msize[ZOMBIE] = NUM_ZOMBIE;
	msize[HOUNDEYE] = NUM_HOUNDEYE;
	msize[ICHY] = NUM_ICHY;
	msize[LEECH] = NUM_LEECH;
	msize[ROACH] = NUM_ROACH;
	msize[PITDRONE] = NUM_PITDRONE;
	msize[KINGPIN] = NUM_KINGPIN;
	msize[TOR] = NUM_TOR;
	msize[VOLTIGORE] = NUM_VOLTIGORE;
	msize[GONOME] = NUM_GONOME;
	//msize[OTIS] = NUM_OTIS;
	msize[GRUNT] = NUM_GRUNT;
	msize[TURRET] = NUM_TURRET;
	msize[NIH] = NUM_NIH;
	msize[ROBOGRUNT] = NUM_ROBOGRUNT;
	msize[SHOCKROACH] = NUM_SHOCKROACH;
	msize[SHOCKTROOPER] = NUM_SHOCKTROOPER;
	msize[BODYGUARD] = NUM_BODYGUARD;
	msize[ASLAVE] = NUM_ASLAVE;
	msize[SCIENTIST] = NUM_SCIENTIST;
	//msize[HASSAULT] = hassault;
	msize[BARNEY] = NUM_BARNEY;
}

void filter_default_content(vector<string>& unfiltered, const char ** default_list, int num_default)
{
	if (contentMode != CONTENT_EVERYTHING)
	{
		vector<string> filtered;
		for (uint i = 0, sz = unfiltered.size(); i < sz; ++i)
		{
			bool match = false;
			for (int k = 0; k < num_default; k++)
			{
				if (matchStr(default_list[k], unfiltered[i]))
				{
					match = true;
					break;
				}
			}
			if (match && contentMode == CONTENT_DEFAULT ||
				!match && contentMode == CONTENT_CUSTOM)
				filtered.push_back(unfiltered[i]);		
		}
		unfiltered = filtered;
	}	
}

string get_weapon_name(int id)
{
	switch(id)
	{
		case WEP_PIPEWRENCH: return "weapon_pipewrench";
		case WEP_CROWBAR: return "weapon_crowbar";
		case WEP_MINE: return "weapon_tripmine";
		case WEP_357: return "weapon_357";
		case WEP_9MMAR: return "weapon_9mmAR";
		case WEP_9MMHANDGUN: return "weapon_9mmhandgun";
		case WEP_CROSSBOW: return "weapon_crossbow";
		case WEP_EAGLE: return "weapon_eagle";
		case WEP_GAUSS: return "weapon_gauss";
		case WEP_GRAPPLE: return "weapon_grapple";
		case WEP_GRENADE: return "weapon_grenade";
		case WEP_HORNET: return "weapon_hornetgun";
		case WEP_M16: return "weapon_m16";
		case WEP_SAW: return "weapon_m249";
		case WEP_MEDKIT: return "weapon_medkit";
		case WEP_MINIGUN: return "weapon_minigun";
		case WEP_RPG: return "weapon_rpg";
		case WEP_SATCHEL: return "weapon_satchel";
		case WEP_SHOTGUN: return "weapon_shotgun";
		case WEP_SNARK: return "weapon_snark";
		case WEP_SNIPER: return "weapon_sniperrifle";
		case WEP_SPORE: return "weapon_sporelauncher";
		case WEP_UZI: return "weapon_uzi";
		case WEP_UZIAKIMBO: return "weapon_uziakimbo";
	}
	return "";
}

void init_default_content()
{
	initMasterList();
	
	for (uint d = 0; d < NUM_MASTER_DIRS; ++d)
		for (uint s = 0; s < masterSize[d]; ++s)
			default_content.push_back(string("sound/") + masterDirs[d] + string("/") + masterList[d][s]);
	default_content.push_back("sound/thunder.wav"); // the only default sound not in a folder

	for (uint s = 0; s < NUM_MODEL_V; ++s)
		default_content.push_back(string("models/") + MODEL_V[s] + string(".mdl"));
	for (uint s = 0; s < NUM_MODEL_P; ++s)
		default_content.push_back(string("models/") + MODEL_P[s] + string(".mdl"));
	for (uint s = 0; s < NUM_MODEL_W; ++s)
		default_content.push_back(string("models/") + MODEL_W[s] + string(".mdl"));
	for (uint s = 0; s < NUM_MODEL_PROPS; ++s)
		default_content.push_back(string("models/") + MODEL_PROPS[s] + string(".mdl"));
	for (uint s = 0; s < NUM_MODEL_MONSTERS; ++s)
		default_content.push_back(string("models/") + MODEL_MONSTERS[s] + string(".mdl"));
	for (uint s = 0; s < NUM_MODEL_PLAYERS; ++s)
		default_content.push_back(string("models/") + MODEL_PLAYERS[s] + string(".mdl"));

	for (uint i = 0; i < NUM_ANIMATED_SPRITES; ++i)
		default_content.push_back(string("sprites/") + ANIMATED_SPRITES[i] + string(".spr"));
	for (uint i = 0; i < NUM_STATIC_SPRITES; ++i)
		default_content.push_back(string("sprites/") + STATIC_SPRITES[i] + string(".spr"));

	for (uint i = 0; i < NUM_DEFAULT_WADS; ++i)
		default_content.push_back(string(default_wads[i]) + ".wad");
	default_content.push_back("cached.wad");
	default_content.push_back("gfx.wad");
	default_content.push_back("decals.wad");
	default_content.push_back("tempdecal.wad");

	for (uint i = 0; i < NUM_SKIES; ++i)
	{
		default_content.push_back(string("gfx/env/") + SKIES[i] + string("ft.tga"));
		default_content.push_back(string("gfx/env/") + SKIES[i] + string("bk.tga"));
		default_content.push_back(string("gfx/env/") + SKIES[i] + string("up.tga"));
		default_content.push_back(string("gfx/env/") + SKIES[i] + string("dn.tga"));
		default_content.push_back(string("gfx/env/") + SKIES[i] + string("lf.tga"));
		default_content.push_back(string("gfx/env/") + SKIES[i] + string("rt.tga"));
	}
}

enum parse_modes
{
	PARSE_SETTINGS,
	PARSE_MAPS,
	PARSE_MODES
};

void parse_settings_file()
{
	user_maps.clear();
	random_seed = "";
	grapple_mode = GRAPPLE_HOOK;
	gmr_only = false;
	vertMode = lightMode = ctexMode = 0;

	ifstream myfile(getWorkDir() + "gsrand_config.txt");
	if (myfile.is_open())
	{
		int parse_mode = PARSE_SETTINGS;
		while ( !myfile.eof() )
		{
			string line;
			getline (myfile,line);
			int comments = line.find("//");
			if (comments != string::npos)
			{
				if (comments > 0)
					line = getSubStr(line, 0, comments);
				else
					continue;
			}
			line = trimSpaces(line);

			int begin_header = line.find("[");
			if (begin_header != string::npos)
			{
				int end_header = line.find("]");
				if (end_header != string::npos)
				{
					
					string name = trimSpaces(getSubStr(line, begin_header+1, end_header));
					if (matchStr("settings", name))
						parse_mode = PARSE_SETTINGS;
					else if (matchStr("maps", name))
						parse_mode = PARSE_MAPS;
					else
						println("gsrand_settings.txt - Unknown category: '" + name + "'");
				}
				continue;
			}

			if (parse_mode == PARSE_SETTINGS)
			{
				int equal = line.find_first_of("=");
				if (equal == string::npos || equal == line.size()-1)
					continue;
				string setting_name = trimSpaces(getSubStr(line, 0, equal));
				string setting_value = trimSpaces(getSubStr(line, equal+1));

				if (setting_name.find("enable_noclip")  == 0) cheatNoclip = atoi(setting_value.c_str()) != 0;
				if (setting_name.find("enable_impulse101") == 0) cheatImpulse = atoi(setting_value.c_str()) != 0;
				if (setting_name.find("enable_godmode") == 0) cheatGodmode = atoi(setting_value.c_str()) != 0;
				if (setting_name.find("gmr_only")       == 0) gmr_only = atoi(setting_value.c_str()) != 0;
				if (setting_name.find("print_rejects")  == 0) printRejects = atoi(setting_value.c_str()) != 0;
				if (setting_name.find("singleplayer")  == 0) singleplayer = atoi(setting_value.c_str()) != 0;
				if (setting_name.find("bypass_hlsp_bullshit_damnit_who_thought_that_was_a_good_idea")  == 0) 
					bypassHlsp = atoi(setting_value.c_str()) != 0;
				if (setting_name.find("grapple_mode")   == 0) grapple_mode = atoi(setting_value.c_str());
				if (setting_name.find("tex_embed_mode") == 0) tex_embed_mode = atoi(setting_value.c_str());
				if (setting_name.find("model_safety")   == 0) modelSafety = atoi(setting_value.c_str());
				if (setting_name.find("sentence_mode")   == 0) sentenceMode = atoi(setting_value.c_str());
				if (setting_name.find("random_sound_effects") == 0) sndEffects = atoi(setting_value.c_str());
				if (setting_name.find("random_solid_ent_models") == 0) bspModelSwap = atoi(setting_value.c_str());
				if (setting_name.find("weapon_skill_mode") == 0) wepSkillMode = atoi(setting_value.c_str());
				if (setting_name.find("monster_skill_mode") == 0) monSkillMode = atoi(setting_value.c_str());
				if (setting_name.find("lightmap_corruption") == 0)
				{
					lightMode = 0;
					if (setting_value.find("shift") != string::npos) lightMode = LIGHT_SHIFTED;
					else if (setting_value.find("disco") != string::npos) lightMode = LIGHT_DISCO;
					else if (setting_value.find("dark")  != string::npos) lightMode = LIGHT_DARK;
					else if (corruptMode == CORRUPT_CONFIG)
						println("Invalid value for lightmap_corruption: '" + setting_value + "'");
				}
				if (setting_name.find("texture_corruption") == 0)
				{
					ctexMode = 0;
					if (setting_value.find("flat")  != string::npos) ctexMode = CTEX_FLAT_COLOR;
					else if (setting_value.find("shift") != string::npos) ctexMode = CTEX_MISALIGN;
					else if (setting_value.find("white") != string::npos) ctexMode = CTEX_WHITE;
					else if (setting_value.find("grey") != string::npos) ctexMode = CTEX_GREY;
					else if (setting_value.find("bw") != string::npos) ctexMode = CTEX_BW;
					else if (setting_value.find("contrast") != string::npos) ctexMode = CTEX_CONTRAST;
					else if (setting_value.find("invert") != string::npos) ctexMode = CTEX_INVERT;
					else if (setting_value.find("random") != string::npos) ctexMode = CTEX_RANDOM;
					else if (corruptMode == CORRUPT_CONFIG)
						println("Invalid value for texture_corruption: '" + setting_value + "'");
				}
				else if (setting_name.find("vertex_corruption_scale") == 0)
				{
					vector<string> values = splitString(setting_value, " ");
					if (values.size() >= 3)
					{
						vertScaleX = atof(values[0].c_str());
						vertScaleY = atof(values[1].c_str());
						vertScaleZ = atof(values[2].c_str());
					}
					else if (values.size() > 0)
						vertScaleX = vertScaleY = vertScaleZ = atof(values[0].c_str());
					else if (corruptMode == CORRUPT_CONFIG)
						println("Invalid value for vertex_corruption_scale: '" + setting_value + "'");
				}
				else if (setting_name.find("vertex_corruption_blend") == 0) 
					vertDistort = atoi(setting_value.c_str());
				else if (setting_name.find("vertex_corruption") == 0)
				{
					vertMode = 0;
					if (setting_value.find("flip")  != string::npos) vertMode |= VERT_FLIP;
					if (setting_value.find("scale") != string::npos) vertMode |= VERT_SCALE;
					if (setting_value.find("blend") != string::npos) vertMode |= VERT_DISTORT;
				}
				if (setting_name.find("random_seed") == 0)
				{
					random_seed = setting_value;
					uint seed = 0x12345678;
					for (uint i = 0; i < setting_value.size(); ++i)
						seed = seed*101 + setting_value[i];
					srand(seed);
				}
			}
			else if (parse_mode == PARSE_MAPS)
			{
				if (line.length() < 1)
					continue;
				if (matchStr(line, "*hlsp"))
				{
					for (int i = 0; i < NUM_HLSP_MAPS; ++i)
						user_maps.push_back(hlsp_maps[i] + string(".bsp"));
				}
				else if (matchStr(line, "*op4"))
				{
					for (int i = 0; i < NUM_OP4_MAPS; ++i)
						user_maps.push_back(op4_maps[i] + string(".bsp"));
				}
				else if (matchStr(line, "*bs"))
				{
					for (int i = 0; i < NUM_BLUESHIFT_MAPS; ++i)
						user_maps.push_back(blueshift_maps[i] + string(".bsp"));
				}
				else if (matchStr(line, "*they"))
				{
					for (int i = 0; i < NUM_HUNGER_MAPS; ++i)
						user_maps.push_back(hunger_maps[i] + string(".bsp"));
				}
				else if (matchStr(line, "*esc"))
				{
					for (int i = 0; i < NUM_ESCAPE_MAPS; ++i)
						user_maps.push_back(escape_maps[i] + string(".bsp"));
				}
				else
					user_maps.push_back(line + string(".bsp"));

			}
			
		}
		myfile.close();
	}
	else
		println("WARNING: gsrand_config.txt is missing.");
}

int restore_map_txt_file(string path, string mapname, string ext)
{
	vector<string> text;
	string line;

	ifstream myfile (path + mapname + ext + ".bak");
	if (myfile.is_open())
	{
		while ( !myfile.eof() )
		{
			getline (myfile,line);
			text.push_back(line);
		}
		myfile.close();
	}
	else
	{
		ifstream myfile (path + mapname + ext); // check if existing one is from gsrand
		if (myfile.is_open())
		{
			bool gsrand_file = false;
			while ( !myfile.eof() )
			{
				getline (myfile,line);
				if ( (gsrand_motd_identifier.length() > 0 && line.find(gsrand_motd_identifier) != string::npos) || 
				     (gsrand_res_identifier.length() > 0 && line.find(gsrand_res_identifier) != string::npos) ||
					 (gsrand_skl_identifier.length() > 0 && line.find(gsrand_skl_identifier) != string::npos))
				{
					gsrand_file = true;
					break;
				}
			}
			myfile.close();
			if (gsrand_file) // probably safe to delete it. probably :>
			{
				remove( string(path + mapname + ext).c_str() );
				return -1;
			}
		}
		return 0;
	}
	

	ofstream fout;

	if (text.size() > 0)
	{
		fout.open (path + mapname + ext, ios::out | ios::trunc);
		for (uint i = 0; i < text.size()-1; i++)
			fout << text[i] << endl;
		fout << text[text.size()-1];
		fout.close();
	}
	
	remove( string(path + mapname + ext + ".bak").c_str() );

	return 1;
}

bool createSKL(string path, string mapname)
{
	vector<string> text;
	string line;

	ifstream myfile (path + mapname + "_skl.cfg");
	if (myfile.is_open())
	{
		while ( !myfile.eof() )
		{
			getline (myfile,line);
			text.push_back(line);
		}
	}
	myfile.close();

	ofstream fout;

	// create a backup of the current file
	if (prefixMode == PREFIX_NONE)
	{
		if (text.size() > 0)
		{
			fout.open (path + mapname + "_skl.cfg.bak", ios::out | ios::trunc);
			for (uint i = 0; i < text.size()-1; i++)
				fout << text[i] << endl;
			fout << text[text.size()-1];
			fout.close(); 
		}
	}

	if (prefixMode != PREFIX_NONE)
		mapname = MAP_PREFIX + mapname;

	res_list.insert("maps/" + mapname + "_skl.cfg");
	fout.open (path + mapname + "_skl.cfg", ios::out | ios::trunc);

	gsrand_skl_identifier = "// This skill file was generated by w00tguy's map randomizer";
	fout << gsrand_skl_identifier << "\n\n";

	if (wepSkillMode == SKILL_DEFAULT && monSkillMode == SKILL_DEFAULT && text.size())
		for (uint i = 0; i < text.size(); i++)
			fout << text[i] << "\n";

	if (wepSkillMode == SKILL_SOME_SUPER)
	{
		for (int i = 1; i < 4; i++)
		{
			fout << "sk_plr_crowbar" << i << " \"9001\"" << "\n";
			fout << "sk_plr_wrench" << i << " \"9001\"" << "\n";
			fout << "sk_plr_grapple" << i << " \"9001\"" << "\n";
			fout << "sk_plr_HpMedic" << i << " \"100\"" << "\n";
		}
	}
	if (wepSkillMode == SKILL_ALL_SUPER)
	{
		for (int k = 0; k < NUM_PLR_SKILL_SETTINGS; k++)
		{
			for (int i = 1; i < 4; i++)
			{
				string dmg = "9001";
				if (matchStr(plr_skill_settings[k], "sk_plr_secondarygauss"))
					dmg = "1000";
				fout << plr_skill_settings[k] << i << " \"" + dmg + "\"" << "\n";
			}
		}
		for (int k = 0; k < NUM_PLR_EXPLOSIVE_SKILL_SETTINGS; k++)
			for (int i = 1; i < 4; i++)
				fout << plr_explosive_skill_settings[k] << i << " \"300\"" << "\n";
	}
	if (monSkillMode == SKILL_SOME_SUPER)
	{
		for (int k = 0; k < NUM_MON_DODGE_SKILL_SETTINGS; k++)
			for (int i = 1; i < 4; i++)
				fout << mon_dodge_skill_settings[k] << i << " \"9001\"" << "\n";
	}
	if (monSkillMode == SKILL_ALL_SUPER)
	{
		for (int k = 0; k < NUM_MON_SKILL_SETTINGS; k++)
			for (int i = 1; i < 4; i++)
				fout << mon_skill_settings[k] << i << " \"9001\"" << "\n";
	}
	
	fout.close();

	return true;
}

bool createCFG(string path, string mapname)
{
	vector<string> text;
	string line;

	ifstream myfile (path + mapname + ".cfg");
	if (myfile.is_open())
	{
		while ( !myfile.eof() )
		{
			getline (myfile,line);
			if (mdlMode == MDL_NONE && line.find("globalmodellist") != string::npos)
			{
				string gmr = getSubStr(line, line.find("globalmodellist")+15);
				gmr = getSubStr(gmr, gmr.find_first_not_of(' '));
				int last_space = gmr.find_first_of(' ');
				if (last_space != string::npos)
					gmr = getSubStr(gmr, 0, last_space);
				gmr = relative_path_to_absolute("models/" + mapname, gmr);
				res_list.insert(gmr);
			}
			if (sndMode == MDL_NONE && line.find("globalsoundlist") != string::npos)
			{
				string gmr = getSubStr(line, line.find("globalsoundlist")+15);
				gmr = getSubStr(gmr, gmr.find_first_not_of(' '));
				int last_space = gmr.find_first_of(' ');
				if (last_space != string::npos)
					gmr = getSubStr(gmr, 0, last_space);
				gmr = relative_path_to_absolute("models/" + mapname, gmr);
				res_list.insert(gmr);
			}
			text.push_back(line);
		}
	}
	myfile.close();

	ofstream fout;

	// create a backup of the current cfg
	if (prefixMode == PREFIX_NONE)
	{
		fout.open (path + mapname + ".cfg.bak", ios::out | ios::trunc);
		if (text.size() > 0)
		{
			for (uint i = 0; i < text.size()-1; i++)
				fout << text[i] << endl;
			fout << text[text.size()-1];
		}
		fout.close(); 
	}

	if (prefixMode != PREFIX_NONE)
		mapname = MAP_PREFIX + mapname;

	res_list.insert("maps/" + mapname + ".cfg");
	fout.open (path + mapname + ".cfg", ios::out | ios::trunc);
	if (text.size() > 0)
	{
		bool has_grapple = false;
		bool has_longjump = false;
		for (uint i = 0; i < text.size(); i++)
		{
			if (text[i][0] == '#') // comment
				continue;
			if (mdlMode != MDL_NONE && text[i].find("globalmodellist") != string::npos)
				continue; // can cause a crash if there are more than one of these
			if (sndMode != SND_NONE && text[i].find("globalsoundlist") != string::npos)
				continue; // can cause a crash if there are more than one of these
			if (prefixMode != PREFIX_NONE && text[i].find("nextmap") != string::npos)
			{
				string old_map = getSubStr(text[i], text[i].find("nextmap") + string("nextmap").length());
				if (old_map.length() > 1)
				{
					old_map = getSubStr(old_map, old_map.find_first_not_of(" \t"));
					fout << "nextmap " + MAP_PREFIX + old_map; 
				}
			}
			else if (prefixMode != PREFIX_NONE && text[i].find("nomaptrans") != string::npos)
			{
				string old_map = getSubStr(text[i], text[i].find("nomaptrans") + string("nomaptrans").length());
				if (old_map.length() > 1)
				{
					old_map = getSubStr(old_map, old_map.find_first_not_of(" \t"));
					fout << "nomaptrans " + MAP_PREFIX + old_map; 
				}
			}
			else
			{
				if (text[i].find("weapon_grapple") != string::npos)
					has_grapple = true;
				if (text[i].find("item_longjump") != string::npos)
					has_longjump = true;
				fout << text[i];
			}
			if (i != text.size()-1)
				fout << endl;
		}

		if (!has_grapple && (mdlMode != MDL_NONE || entMode == ENT_SUPER) && grapple_mode != GRAPPLE_DISABLE)
			fout << "\nweapon_grapple\n";
		if (entMode == ENT_SUPER)
		{
			fout << "\nmp_disablegaussjump 0\n";
			fout << "mp_dropweapons 1\n";
			fout << "mp_flashlight 1\n";
			fout << "mp_allowmonsterinfo 1\n";
			if (!has_longjump)
				fout << "item_longjump\n";
		}
	}
	

	fout << "\n## GSRAND BEGIN\n";
	if (sndMode != SND_NONE)
		fout << "globalsoundlist ../../maps/" << mapname << ".gsrand\n" <<
		"sentence_file maps/" << mapname << "_sentences.gsrand\n";
	if (mdlMode != MDL_NONE)
		fout << "globalmodellist ../../maps/" << mapname << "_gmr.gsrand\n";
	fout.close();

	return true;
}

bool createMOTD(string path, string mapname)
{
	vector<string> text;
	string line;

	ifstream myfile (path + mapname + "_motd.txt");
	if (myfile.is_open())
	{
		while ( !myfile.eof() )
		{
			getline (myfile,line);
			text.push_back(line);
		}
	}
	myfile.close();

	ofstream fout;

	// create a backup of the current file
	if (prefixMode == PREFIX_NONE)
	{
		if (text.size() > 0)
		{
			fout.open (path + mapname + "_motd.txt.bak", ios::out | ios::trunc);
			for (uint i = 0; i < text.size()-1; i++)
				fout << text[i] << "\n";
			fout << text[text.size()-1];
			fout.close(); 
		}
	}

	if (prefixMode != PREFIX_NONE)
		mapname = MAP_PREFIX + mapname;

	res_list.insert("maps/" + mapname + "_motd.txt");
	fout.open (path + mapname + "_motd.txt", ios::out | ios::trunc);

	gsrand_motd_identifier = "      This map was generated by w00tguy's map randomizer";
	fout << "===============================================\n";
	fout << "      This map was generated by w00tguy's map randomizer\n";
	fout << "===============================================\n\n";
	fout << "Helpful Info:\n";
	if (sndMode != SND_NONE)
	{
		fout << "    - Type this into the console (press the '~' key to open the console):\n";
		fout << "            bind J cl_stopsound\n";
		fout << "      You can now press the 'J' key to stop all active sounds (trust me, you will need this)\n"; 
	}
	if (mdlMode == MDL_CRAZY)
		fout << "    - If weapon models block your vision, you can type \"r_drawviewmodel 0\" in console to hide them.\n";
	if (cheatGodmode)
		fout << "    - Press the USE and DUCK keys together to activate godmode.\n";
	if (cheatNoclip)
		fout << "    - Press the USE and JUMP keys together to activate noclip.\n";
	if (cheatImpulse)
		fout << "    - Press the USE and RELOAD keys together for all weapons and ammo.\n";
	if (grapple_mode == GRAPPLE_HOOK_ALWAYS || entMode == ENT_SUPER && grapple_mode == GRAPPLE_HOOK)
		fout << "    - You can use the barnacle weapon to latch on to walls and swing around like spiderman.\n";
	fout << "\n";

	fout << "Date generated: " << generation_date.str() << "\n\n";

	fout << "Map generation settings:\n";
	fout << "    Texture Mode: ";
	if (texMode == TEX_MAP)            fout << "Write to map\n";
	else if (texMode == TEX_MASTERWAD) fout << "Write to a common WAD\n";
	else if (texMode == TEX_WADS)      fout << "Write WAD for each map\n";
	else if (texMode == TEX_NONE)      fout << "Do not change\n";

	fout << "    Entity Mode: ";
	if (entMode == ENT_NONE)        fout << "Do not change\n";
	else if (entMode == ENT_SUPER)  fout << "Super random\n";
	else if (entMode == ENT_APPEAR) fout << "Appearances only\n";

	fout << "    Corrupt Mode : ";
	if (corruptMode == CORRUPT_NONE)        fout << "Do not corrupt\n";
	else if (corruptMode == CORRUPT_SUPER)  fout << "Super random\n";
	else if (corruptMode == CORRUPT_CONFIG) fout << "Use config setting\n";

	fout << "    Model Mode: ";
	if (mdlMode == MDL_NONE)       fout << "Do not change\n";
	else if (mdlMode == MDL_CRAZY) fout << "Super random\n";
	else if (mdlMode == MDL_TYPED) fout << "Use similar types\n";

	fout << "    Sound Mode: ";
	if (sndMode == SND_NONE)       fout << "Do not change\n";
	else if (sndMode == SND_ALL)   fout << "Everything\n";
	else if (sndMode == SND_WORLD) fout << "World only\n";
	else if (sndMode == SND_MONST) fout << "Monsters only\n";

	fout << "    Content Mode: ";
	if (contentMode == CONTENT_EVERYTHING)   fout << "Everything\n";
	else if (contentMode == CONTENT_DEFAULT) fout << "Default only\n";
	else if (contentMode == CONTENT_CUSTOM)  fout << "Non-default only\n";

	if (vertMode != 0)
	{
		fout << "    Geometry corruption:";
		if (vertMode & VERT_FLIP) fout << " Flip ";
		if (vertMode & VERT_SCALE) fout << " Scale (" << vertScaleX << "x) ";
		if (vertMode & VERT_DISTORT) fout << " Blend (" << vertDistort << " units) "; 
		fout << "\n";
	}
	if (lightMode != 0)
	{
		fout << "    Lightmap corruption: ";
		if (lightMode == LIGHT_SHIFTED) fout << "Shift\n";
		if (lightMode == LIGHT_DISCO) fout << "Disco\n";
		if (lightMode == LIGHT_DARK) fout << "Dark\n";
	}
	if (ctexMode != 0)
	{
		fout << "    Texture corruption: ";
		if (ctexMode == CTEX_FLAT_COLOR) fout << "Flat\n";
		if (ctexMode == CTEX_MISALIGN) fout << "Shift\n";
		if (ctexMode == CTEX_WHITE) fout << "White\n";
		if (ctexMode == CTEX_GREY) fout << "Greyscale\n";
		if (ctexMode == CTEX_BW) fout << "Black & white\n";
		if (ctexMode == CTEX_CONTRAST) fout << "High Contrast\n";
		if (ctexMode == CTEX_INVERT) fout << "Invert colors\n";
		if (ctexMode == CTEX_RANDOM) fout << "Random\n";
	}

	fout << "    weapon_skill_mode: " << wepSkillMode << "\n";
	fout << "    monster_skill_mode: " << monSkillMode << "\n";
	fout << "    grapple_mode: " << grapple_mode << "\n";
	fout << "    gmr_only: " << (int)gmr_only << "\n";
	fout << "    tex_embed_mode: " << tex_embed_mode << "\n";
	fout << "    model_safety: " << modelSafety << "\n";
	fout << "    singleplayer: " << (int)singleplayer << "\n";
	if (singleplayer == 0)
		fout << "    sentence_mode: " << (int)sentenceMode << "\n";
	fout << "    random_solid_ent_models: " << bspModelSwap << "\n";
	fout << "    random_sound_effects: " << sndEffects << "\n";

	if (random_seed.length())
		fout << "    random_seed: " << random_seed << "\n";

	if (text.size() > 0)
	{
		fout << "\n";
		fout << "===============================================\n";
		fout << "                The original map MOTD starts here\n";
		fout << "===============================================\n\n";
		for (uint i = 0; i < text.size(); i++)
		{
			fout << text[i];
			if (i != text.size()-1)
				fout << "\n";
		}
	}
	fout.close();

	return true;
}

// only load the texture data from a map
byte * loadTextureChunk(string filename, int& lump_len)
{
	if (!fileExists(filename))
	{
		err("file does not exist: " + filename);
		return NULL;
	}

	// Read all BSP Data
	ifstream fin (filename, ios::binary);

	BSP map;
	fin.read((char*)&map.header.nVersion, sizeof(int));
	for (int i = 0; i < HEADER_LUMPS; i++)
		fin.read((char*)&map.header.lump[i], sizeof(BSPLUMP));
		
	byte * tex_lump = new byte[map.header.lump[LUMP_TEXTURES].nLength];
	fin.seekg(map.header.lump[LUMP_TEXTURES].nOffset);
	if (fin.eof())
		println("FAILED TO READ LUMP " + str(LUMP_TEXTURES));
	else
		fin.read((char*)tex_lump, map.header.lump[LUMP_TEXTURES].nLength);
	fin.close();

	lump_len = map.header.lump[LUMP_TEXTURES].nLength;
	return tex_lump;
}

BSP * loadBSP(string mapname, bool loadAll)
{
	string filename = getWorkDir() + "maps/" + mapname + ".bsp";
	if (!fileExists(filename))
	{
		err("file does not exist: " + filename);
		return NULL;
	}

	// Read all BSP Data
	ifstream fin (filename, ios::binary);

	BSP * map = new BSP;
	map->name = mapname;
	fin.read((char*)&map->header.nVersion, sizeof(int));

	for (int i = 0; i < HEADER_LUMPS; i++)
	{
		fin.read((char*)&map->header.lump[i], sizeof(BSPLUMP));
	}
	map->lumps = new byte*[HEADER_LUMPS];

	for (int i = 0; i < HEADER_LUMPS; i++)
	{
		//println("reading lump at " + str(map->header.lump[i].nOffset) + " LENGTH: " + str(map->header.lump[i].nLength));
		
		if (loadAll || entMode != ENT_NONE || texMode != TEX_NONE || sndMode != SND_NONE || i == LUMP_ENTITIES)
		{
			map->lumps[i] = new byte[map->header.lump[i].nLength];
			fin.seekg(map->header.lump[i].nOffset);
			if (fin.eof())
				println("FAILED TO READ LUMP " + str(i));
			else
				fin.read((char*)map->lumps[i], map->header.lump[i].nLength);
		}
		else
			map->lumps[i] = new byte[1];
	}	
	fin.close();

	return map;
}

void loadLumpBackup(BSP * map, int lump, string suffix)
{
	string filename = getWorkDir() + "maps/" + map->name + suffix;
	if (!fileExists(filename))
	{
		//println("Ent backup file does not exist: " + filename);
		return;
	}
	struct stat results;

	if (stat(filename.c_str(), &results) != 0)
	{
		println("error getting file size of ent backup file");
		return;
	}

	int eSize = results.st_size;

	ifstream fin (filename, ios::binary);
	delete [] map->lumps[lump];
	map->lumps[lump] = new byte[eSize];
	map->header.lump[lump].nLength = eSize;
	fin.read((char*)map->lumps[lump], eSize);
	fin.close();
	//println("loaded " + str(eSize) + " from backup file");
}

void saveLumpBackup(BSP * map, int lump, string suffix)
{
	string filename = getWorkDir() + "maps/" + map->name + suffix;
	if (fileExists(filename))
	{
		//println("Entity backup file already exists.");
		return;
	}
	ofstream fout(filename, ios::out | ios::binary | ios::trunc);
	fout.write((char*)map->lumps[lump], map->header.lump[lump].nLength); 
	fout.close();
}

vector<string> create_res_list(Entity ** ents, string mapname)
{
	// append to res_list
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (ents[i] == NULL)
			break;
		string cname = toLowerCase(ents[i]->keyvalues["classname"]);

		if (matchStr(cname, "worldspawn"))
		{
			if (ents[i]->hasKey("wad"))
			{
				vector<string> wads = splitString(ents[i]->keyvalues["wad"], ";");
				for (uint k = 0; k < wads.size(); ++k)
				{
					int dirfind = wads[k].find_last_of("\\/");
					if (dirfind) wads[k] = getSubStr(wads[k], dirfind+1);
					res_list.insert(wads[k]);
				}
			}
			if (ents[i]->hasKey("skyname"))
			{
				res_list.insert("gfx/env/" + ents[i]->keyvalues["skyname"] + "ft.tga");
				res_list.insert("gfx/env/" + ents[i]->keyvalues["skyname"] + "bk.tga");
				res_list.insert("gfx/env/" + ents[i]->keyvalues["skyname"] + "up.tga");
				res_list.insert("gfx/env/" + ents[i]->keyvalues["skyname"] + "dn.tga");
				res_list.insert("gfx/env/" + ents[i]->keyvalues["skyname"] + "lf.tga");
				res_list.insert("gfx/env/" + ents[i]->keyvalues["skyname"] + "rt.tga");
			}
			if (ents[i]->hasKey("globalmodellist") && ents[i]->keyvalues["globalmodellist"].length())
			{
				string gmr = ents[i]->keyvalues["globalmodellist"];
				string gmrpath = relative_path_to_absolute("models/" + mapname, gmr);
				res_list.insert(gmrpath);
			}
			if (ents[i]->hasKey("globalsoundlist") && ents[i]->keyvalues["globalmodellist"].length())
			{
				string gsr = ents[i]->keyvalues["globalsoundlist"];
				string gsrpath = relative_path_to_absolute("sound/" + mapname, gsr);
				res_list.insert(gsrpath);
			}
		}

		if (cname.find("monster_") == 0 || cname.find("weapon_") == 0 || matchStr(cname, "squadmaker"))
		{
			if (ents[i]->hasKey("soundlist") && ents[i]->keyvalues["soundlist"].length())
			{
				string gsr = ents[i]->keyvalues["soundlist"];
				string gsrpath = relative_path_to_absolute("sound/" + mapname, gsr);
				res_list.insert(gsrpath);
			}
		}

		//
		// find custom entity sounds
		//
		if (matchStr(cname, "ambient_generic") || matchStr(cname, "func_rotating"))
		{
			if (ents[i]->hasKey("message") && ents[i]->keyvalues["message"].find(".") != string::npos)
			{
				string msg = ents[i]->keyvalues["message"];
				if (msg[0] == '*' || msg[0] == '/')
					msg = getSubStr(msg, 1);
				msg = relative_path_to_absolute("sound", msg);
				res_list.insert(msg);
			}
		}
		else if (matchStr(cname, "custom_precache"))
			for (string_hashmap::iterator it = ents[i]->keyvalues.begin(); it != ents[i]->keyvalues.end(); ++it)
				if (it->first.find("model_") == 0 || it->first.find("sound_") == 0)
					res_list.insert(it->second);
		// rest of the sounds set in global replacement function		

		//
		// find custom entity models
		//

		if (cname.find("monstermaker") != string::npos || cname.find("env_xenmaker") != string::npos)
			cname = toLowerCase(ents[i]->keyvalues["monstertype"]);

		string custom_monster_model_key = "model";
		if (cname.find("squadmaker") != string::npos)
		{
			cname = toLowerCase(ents[i]->keyvalues["monstertype"]);
			custom_monster_model_key = "new_model";
		}

		if (matchStr(cname, "env_shooter") && ents[i]->hasKey("shootmodel"))
			res_list.insert(ents[i]->keyvalues["shootmodel"]);
		else if (matchStr(cname, "env_beam") && ents[i]->hasKey("texture"))
			res_list.insert(ents[i]->keyvalues["texture"]);
		else if (matchStr(cname, "env_funnel") && ents[i]->hasKey("sprite"))
			res_list.insert(ents[i]->keyvalues["sprite"]);
		else if (matchStr(cname, "env_laser"))
		{
			if (ents[i]->hasKey("texture"))
				res_list.insert(ents[i]->keyvalues["texture"]);
			if (ents[i]->hasKey("EndSprite"))
				res_list.insert(ents[i]->keyvalues["EndSprite"]);
		}
		else if (cname.find("func_tank") == 0)
		{
			if (ents[i]->hasKey("spritesmoke"))
				res_list.insert(ents[i]->keyvalues["spritesmoke"]);
			if (ents[i]->hasKey("spriteflash"))
				res_list.insert(ents[i]->keyvalues["spriteflash"]);
		}
		else if (matchStr(cname, "func_breakable") || matchStr(cname, "func_door") || matchStr(cname, "func_door_rotating") ||
				 matchStr(cname, "momentary_door") || matchStr(cname, "func_pushable"))
		{
			if (ents[i]->hasKey("gibmodel"))
				res_list.insert(ents[i]->keyvalues["gibmodel"]);
		}	
		else if (matchStr(cname, "item_recharge") || matchStr(cname, "item_healthcharger"))
		{
			if (ents[i]->hasKey("model_juice"))
				res_list.insert(ents[i]->keyvalues["model_juice"]);		
		}	
		

		if (cname.find("monster_") == 0 || cname.find("ammo_") == 0
			|| cname.find("item_") == 0 || cname.find("cycler") == 0 || matchStr(cname, "env_beverage") 
			|| matchStr(cname, "env_glow") || matchStr(cname, "env_sprite") || matchStr(cname, "env_spritetrain") 
			|| matchStr(cname, "weaponbox") || matchStr(cname, "trigger_changemodel"))
		{			
			if (ents[i]->hasKey(custom_monster_model_key))
				res_list.insert(ents[i]->keyvalues[custom_monster_model_key]);
		}
		else if (cname.find("weapon_") == 0 || matchStr(cname, "world_items"))
		{
			if (ents[i]->hasKey("wpn_v_model"))
				res_list.insert(ents[i]->keyvalues["wpn_v_model"]);
			if (ents[i]->hasKey("wpn_w_model"))
				res_list.insert(ents[i]->keyvalues["wpn_w_model"]);
			if (ents[i]->hasKey("wpn_p_model"))
				res_list.insert(ents[i]->keyvalues["wpn_p_model"]);
		}
	}

	vector<string> filtered;
	for (set<string>::iterator it = res_list.begin(); it != res_list.end(); ++it)
	{
		string name = *it;
		while (name.length() > 1 && (name[0] =='/' || name[0] =='\\' || name[0] == ' ' || name[0] == '\t'))
			name = getSubStr(*it, 1);
		if (name.length() <= 1)
			continue;

		// filter default content out of res list
		bool is_default = false;
		for (uint i = 0; i < default_content.size(); ++i)
		{
			if (matchStr(name, default_content[i]))
			{
				is_default = true;
				break;
			}
		}
		if (is_default)
			continue;

		if (!fileExists(name) && !fileExists("../valve/" + name) && !fileExists("../svencoop_downloads/" + name))
		{
			if (name.find(".wad") != string::npos && name.find("gsrand") == 0)
				continue; // probably hasn't been generated yet
			println("WARNING: Missing resource - '" + name + "'"); 
			continue;
		}
		filtered.push_back(name);
		if (name.find(".mdl") != string::npos)
		{
			string m_name = getSubStr(name, 0, name.length()-4);
			
			string t_model = m_name + "T.mdl";
			if (fileExists(t_model) || fileExists("../valve/" + t_model) || fileExists("../svencoop_downloads/" + t_model))
				filtered.push_back(t_model);

			int id = 0;
			string a_model = m_name + "01.mdl";
			while (fileExists(a_model) || fileExists("../valve/" + a_model) || fileExists("../svencoop_downloads/" + a_model))
			{
				filtered.push_back(a_model);
				a_model = m_name;
				if (++id < 10)
					a_model += "0";
				a_model += string(str(id)) + ".mdl";
			}
		}
	}
	return filtered;
}

void create_res_file(vector<string>& res_files, Entity ** ents, string path, string mapname)
{
	vector<string> text;
	string line;

	ifstream myfile (path + mapname + ".res");
	if (myfile.is_open())
	{
		while ( !myfile.eof() )
		{
			getline (myfile,line);
			text.push_back(line);
		}
	}
	myfile.close();

	ofstream fout;

	// create a backup of the current res file
	if (prefixMode == PREFIX_NONE)
	{
		if (text.size() > 0)
		{
			fout.open (path + mapname + ".res.bak", ios::out | ios::trunc);
			for (uint i = 0; i < text.size()-1; i++)
				fout << text[i] << endl;
			fout << text[text.size()-1];
			fout.close(); 
		}
	}

	if (prefixMode != PREFIX_NONE)
		mapname = MAP_PREFIX + mapname;

	res_files.push_back("maps/" + mapname + ".res");

	if (!singleplayer)
	{
		// make sure we have the correct case for these files
		set<string> case_corrected_files;
		for (uint i = 0; i < res_files.size(); i++)
		{
			if (filename_cache.find(res_files[i]) != filename_cache.end())
				case_corrected_files.insert(filename_cache[res_files[i]]);
			else
			{
				string corrected;
				if (fileExists(res_files[i]))	
					corrected = getFilename(res_files[i]);
				else if (fileExists("../svencoop_downloads/" + res_files[i]))
				{
					corrected = getFilename("../svencoop_downloads/" + res_files[i]);
					corrected = getSubStr(corrected, string("../svencoop_downloads/").length());
				}
				filename_cache[res_files[i]] = corrected;
				case_corrected_files.insert(corrected);
			}
		}

		res_files.clear();
		res_files.reserve(case_corrected_files.size());
		for (set<string>::iterator it = case_corrected_files.begin(); it != case_corrected_files.end(); it++)
			res_files.push_back(*it);

		fout.open (path + mapname + ".res", ios::out | ios::trunc);

		gsrand_res_identifier = "// This resource file was generated by w00tguy's randomizer.";
		fout << "// This resource file was generated by w00tguy's randomizer.\n";
		fout << "// Listed resources include sounds, models, sprites, wads, and skyboxes not included with SC 4.8.\n";
		fout << "// Dependencies for models are included if they exist (e.g. Tor.mdl + TorT.mdl + Tor01.mdl).\n\n";
		int written = 0;
		int toomany = 0;
		bool onlysounds = true;
		for (uint i = 0; i < res_files.size(); ++i)
		{
			if (res_files[i].find(".gsrand") != string::npos || res_files[i].find(".cfg") != string::npos || 
				res_files[i].find(".txt") != string::npos || res_files[i].find(".res") != string::npos)
				continue; // clients don't need these
			written++;
			if (written > 506)
				toomany++;
			else
				fout << res_files[i] << endl;
		}
		if (!toomany)
		{
			for (set<string>::iterator it = res_sentence_list.begin(); it != res_sentence_list.end(); it++)
			{
				written++;
				if (written > 506)
					toomany++;
				else
					fout << *it << endl;
			}
		}
		else
			onlysounds = false;
		if (toomany)
		{
			if (!onlysounds)
				println("DANGER: " + str(toomany) + " resources could not fit in map RES file (potential crashes)");
			else
				println("WARNING: " + str(toomany) + " sounds could not fit in map RES file");
		}
		fout.close();
	}
}

string get_date_base36()
{
	int now_code = atoi(DateTime::now().compact_str().c_str());

	string b36;

	while (now_code)
	{
		b36 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[now_code % 36] + b36;
		now_code /= 36;
	}
	return b36;
}

vector<Wad> wads;
vector<string> maps;
void find_user_content()
{
	string path = getWorkDir() + "maps/";
	maps = user_maps;

	int missing = 0;
	if (user_maps.empty())
		maps = getDirFiles(getWorkDir() + "maps/","bsp");
	else
	{
		for (uint i = 0; i < maps.size(); ++i)
		{
			if (!fileExists(path + maps[i]))
			{
				maps.erase(maps.begin() + i--);
				missing++;
			}
		}
	}

	if (user_maps.empty())
		cout << "Found " << maps.size() << " maps\n\n";
	else
		cout << "Found " << maps.size() << " of " << (maps.size() + missing) << " maps\n\n";

	if (contentMode == CONTENT_EVERYTHING) println("Finding content...");
	if (contentMode == CONTENT_DEFAULT) println("Finding default content...");
	if (contentMode == CONTENT_CUSTOM) println("Finding non-default content...");

	numOverflow = 0;
	int sounds = 0;

	if (sndMode != SND_NONE)
	{
		getAllSounds();
		getAllVoices();
		println("Found " + str(user_sounds.size()) + " sounds");
	}

	if (mdlMode != MDL_NONE)
	{
		get_all_models();
		total_model_count = user_monster_models.size() + user_prop_models.size() + user_v_models.size() +
						  user_p_models.size() + user_w_models.size() + user_apache_models.size() + 
						  user_player_models.size();
		init_random_monster_models();
		println("Found " + str(total_model_count) + " models");	

		get_all_sprites();
		println("Found " + str(user_sprites.size() + user_animated_sprites.size()) + " sprites");
	}

	if (texMode != TEX_NONE)
	{
		wads = getWads();
		if (tex_embed_mode != EMBED_DISABLE)
			create_tex_embed_wad(wads);
		if (tex_embed_mode == EMBED_ONLY)
		{
			vector<Wad> new_wads;
			new_wads.push_back(wads[wads.size()-1]);
			wads.clear();
			wads = new_wads;
		}
		else
			println("Found " + str(wads.size()) + " wads");
		
		get_all_skies();
		println("Found " + str(user_skies.size()) + " skyboxes");
	}	

	// print warnings
	if (mdlMode != MDL_NONE)
	{
		if (user_monster_models.empty()) println("Warning: Couldn't find any monster models");
		if (user_prop_models.empty())    println("Warning: Couldn't find any prop models");
		if (user_player_models.empty())  println("Warning: Couldn't find any player models");
		if (user_v_models.empty())		 println("Warning: Couldn't find any v_weapon models");
		if (user_p_models.empty())		 println("Warning: Couldn't find any p_weapon models");
		if (user_w_models.empty())		 println("Warning: Couldn't find any w_weapon models");
		if (user_apache_models.empty())  println("Warning: Couldn't find any potential apache/sentry models");

		if (user_animated_sprites.empty()) println("Warning: Couldn't find any animated sprites");
		if (user_sprites.empty())		   println("Warning: Couldn't find any single-frame sprites");
	}

	// get map prefix
	if (prefixMode == PREFIX_CUSTOM)
	{
		print("\nEnter your custom map prefix: ");
		cin >> MAP_PREFIX;
		vector<string> conflicts;
		for (uint i = 0; i < maps.size(); ++i)
		{	
			string new_name = MAP_PREFIX + maps[i];
			for (uint k = 0; k < maps.size(); ++k)
				if (matchStr(new_name, maps[k]))
					conflicts.push_back(getSubStr(new_name, 0, new_name.size()-4));
		}
		if (conflicts.size())
		{
			print("DANGER! The following maps will be overwritten if you use this prefix:\n\n");
			for (uint i = 0; i < conflicts.size(); ++i)
			{
				print(conflicts[i]);
				if (i != conflicts.size()-1)
					print(", ");
			}
			print("\n");
		}
	}

	if (printRejects)
		writeLog();
}

int randomize_maps()
{
	parse_settings_file();

	string path = "maps/";
	maps.clear();
	wads.clear();
	find_user_content();

	if (prefixMode == PREFIX_NONE)
		cout << "\nDANGER: Closing the program before this finishes will likely corrupt a map!\n";
	cout << "\nThe randomizer is ready.\n\n";
	cout << "1: Abort\n";
	cout << "0: Continue\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1') return 1;
		if (choice == '0') break;
	} 

	if (prefixMode == PREFIX_TIME)
		MAP_PREFIX = get_date_base36() + "_";
	generation_date = DateTime::now();

	Entity *** entLists = new Entity**[maps.size()];
	int idx = 0;

	system("cls");

	every_random_replacement.clear();
	super_res_list.clear();
	filename_cache.clear();

	//////////////////////////////
	// BEGIN THE RANDOMIZATION! //
	//////////////////////////////

	for (uint f = 0; f < maps.size(); f++)
	{
		res_list.clear();
		res_sentence_list.clear();
		string mapName = getSubStr(maps[f], 0, maps[f].length()-4);

		string fpath = path + maps[f];
		string prefix = prefixMode == PREFIX_NONE ? "" : MAP_PREFIX;

		print(prefix + mapName + "\t");

		BSP * map = loadBSP(mapName, true);
		if (map == NULL)
			continue;
		int numEnts;
		Entity** ents = getMapEnts(map, false, numEnts);

		if (prefixMode != PREFIX_NONE)
			map->name = MAP_PREFIX + map->name;

		if (corruptMode != CORRUPT_NONE)
		{
			if (corruptMode == CORRUPT_SUPER)
			{
				vertMode = lightMode = ctexMode = 0;
				if (rand() % 4 == 0) vertMode |= VERT_FLIP;
				if (rand() % 4 == 0) vertMode |= VERT_SCALE; 
				if (rand() % 3 == 0) vertMode |= VERT_DISTORT;
				if (rand() % 4 == 0) 
				{
					if (rand() % 10)
					{
						lightMode = LIGHT_DISCO;
						if (rand() % 4) lightMode = LIGHT_SHIFTED;
					}
					else
						lightMode = LIGHT_DARK;
				}
				if (rand() % 3 == 0) ctexMode = rand() % CTEX_MODES;
				vertScaleX = (float)(150 - (rand() % 126)) * 0.01f;
				vertScaleY = vertScaleZ = vertScaleX;
				if (rand() % 2) vertDistort = 4;
				else			vertDistort = 2;
				if (vertMode & VERT_SCALE) vertDistort *= vertScaleX;

				if (!singleplayer) vertMode &= ~VERT_DISTORT; // unstable on dedicated servers
			}
		}

		bool will_embed = texMode == TEX_MAP && (corruptMode == CORRUPT_NONE || ctexMode != CTEX_WHITE && ctexMode != CTEX_FLAT_COLOR);
		bool should_hook = grapple_mode == GRAPPLE_HOOK_ALWAYS || ((entMode == ENT_SUPER || corruptMode != CORRUPT_NONE) && grapple_mode == GRAPPLE_HOOK);
		bool tex_corrupt = corruptMode != CORRUPT_NONE && ctexMode >= CTEX_WHITE;
		if ((should_hook && !will_embed) || tex_corrupt)
			embedAllTextures(map, ents); // we need to rename everything to xeno_grapple!

		if (texMode != TEX_NONE && (corruptMode == CORRUPT_NONE || (ctexMode != CTEX_WHITE && ctexMode != CTEX_FLAT_COLOR)))
		{
			string wad_name = map->name;
			if (prefixMode == PREFIX_NONE)
				wad_name = "gsrand_" + map->name;
			int tex = makeMapWad(map, wad_name, wads);
			randomize_skybox(ents);
			//print(str(tex) + " tex. ");
			print("TEX...");
		}	

		if (corruptMode != CORRUPT_NONE)
		{
			corrupt_map_verts(map, ents);
			corrupt_map_lightmap(map);
			corrupt_map_textures(map, ents); 
			//print("corrupted. ");
			print("BSP...");
		}
		if (bspModelSwap == 2 || (corruptMode != MDL_NONE && bspModelSwap == 1))
			randomize_bsp_models(map, ents);
		
		if (sndMode != SND_NONE)
			do_ent_sounds(ents, map->name);

		if (entMode != ENT_NONE)
		{
			//print(str(numEnts) + " ents. ");
			do_entity_randomization(ents, map->name);
			print("ENT...");
		}

		createSKL(path, mapName);
		add_gsrand_ents(ents);

		if (mdlMode != MDL_NONE)
		{
			do_model_replacement(map, ents, path, mapName);
			print("MDL...");
		}
		

		if (sndMode != SND_NONE || prefixMode != PREFIX_NONE || mdlMode != MDL_NONE)
			createCFG(path, mapName);

		if (sndMode != SND_NONE)
		{
			vector<sound> writables;
			if (sndMode != SND_MONST)
			{
				writables = getReplacableSounds(verbose);
				string gsrName = path + map->name + ".gsrand";
				res_list.insert("maps/" + map->name + ".gsrand");
				writeGSR(gsrName, writables);
			}
			if (sndMode != SND_WORLD)
			{
				writeMonsterSoundLists(map->name);
				writeSentences(map->name);
			}

			//print(str(writables.size()) + " sounds.");
			print("SND...");
		}
		
		if (prefixMode != PREFIX_NONE)
			update_changelevels(ents, map->name);

		if (texMode != TEX_NONE || sndMode != SND_NONE || entMode != ENT_NONE || mdlMode != MDL_NONE || prefixMode != PREFIX_NONE)
		{
			needsRipent(map, ents);
			ripent(map, ents, false);
		}

		createMOTD(path, mapName);

		vector<string> res_files = create_res_list(ents, mapName); // appends to res_list
		create_res_file(res_files, ents, path, mapName);
		if (!singleplayer)
			print("RES...");

		for (uint i = 0; i < res_files.size(); ++i)
		{
			if (!singleplayer)
				super_res_list.insert(res_files[i]);
			else // we don't care about case since we're not making res files
				super_res_list.insert(toLowerCase(res_files[i])); 
		}
		super_res_list.insert("maps/" + map->name + ".bsp");

		for (int i = 0; i < HEADER_LUMPS; i++)
			delete [] map->lumps[i];
		delete [] map->lumps;
		delete map;

		// this is faster than deleting each entity (deleted after all maps randomized)
		for (int i = 0; i < numEnts; i++)
		{
			if (ents[i] != NULL)
				ents[i]->keyvalues.clear();
		}
		
		println("Done");
	}	

	/////////////
	// CLEANUP //
	/////////////

	for (int i = 0; i < idx; i++)
	{
		if (entLists[i] != NULL)
		{
			for (int k = 0; k < MAX_MAP_ENTITIES; k++)
			{
				if (entLists[i][k] != NULL)
					delete entLists[i][k];
				else
					break;
			}
			delete [] entLists[i];
		}
	}
	delete [] entLists;

	if (texMode == TEX_MASTERWAD)
		writeWad(masterWadTex, wads, "gsrand");

	return 0;
}

void undoEverything()
{
	string path = getWorkDir() + "maps/";

	int numUpdated = 0;
	int numRemoved = 0;
	vector<string> files = getDirFiles(path,"entbackup");
	vector<string> files2 = getDirFiles(path,"texbackup");
	for (uint i = 0; i < files2.size(); i++)
		files.push_back(files2[i]);
	vector<string> restored;

	// restore entity data
	for (uint f = 0; f < files.size(); f++)
	{
		string mapname = getSubStr(files[f],0,files[f].length()-10);

		bool repeat = false;
		for (uint i = 0; i < restored.size(); i++)
		{
			if (matchStr(restored[i],mapname))
			{
				repeat = true;
				break;
			}
		}
		if (!repeat)
		{
			BSP * map = loadBSP(mapname, true);
			if (map == NULL)
				println("Found entbackup file but no map: " + mapname + ".bsp");
			else 
			{
				ripent(map, NULL, true);
				numUpdated++;
				for (int i = 0; i < HEADER_LUMPS; i++)
					delete [] map->lumps[i];
				delete [] map->lumps;
				delete map;
			}
			restored.push_back(mapname);
			println("Restored " + mapname + ".bsp");
		}
		remove( string(path + files[f]).c_str() );
	}

	if (numRemoved > 0)
		println("Restored " + str(numUpdated) + " BSPs");

	// delete BSPs with gsrand_ prefix
	files = getDirFiles(path,"bsp");
	for (uint f = 0; f < files.size(); f++)
	{
		if (files[f].length() < 7)
			continue;
		string mapPrefix = getSubStr(files[f],0,7);
		if (matchStr(mapPrefix,"gsrand_"))
		{
			remove( string(path + files[f]).c_str() );
			numRemoved++;
		}
	}

	if (numRemoved > 0)
		println("Deleted " + str(numRemoved) + " BSPs");

	files = getDirFiles(path,"gsrand");

	for (uint f = 0; f < files.size(); f++)
		remove( string(path + files[f]).c_str() );

	if (files.size() > 0)
		println("Deleted " + str(files.size()) + " GSRAND files");

	numUpdated = 0;
	numRemoved = 0;
	files = getDirFiles(path,"cfg");
	for (uint f = 0; f < files.size(); f++)
	{
		string prefix = "";
		if (files[f].length() > 7)
		{
			prefix = getSubStr(files[f],0,7);
			if (matchStr(prefix,"gsrand_"))
			{
				remove( string(path + files[f]).c_str() );
				numRemoved++;
				continue;
			}
		}

		files[f] = getSubStr(files[f],0,files[f].length()-4);
		int cfg_ret = restore_map_txt_file(path, files[f], ".cfg");
		int res_ret = restore_map_txt_file(path, files[f], ".res");
		int motd_ret = restore_map_txt_file(path, files[f], "_motd.txt");
		int skl_ret = restore_map_txt_file(path, files[f], "_skl.cfg");
		if (cfg_ret > 0) numUpdated++;
		else if (cfg_ret < 0) numRemoved++;
		if (res_ret > 0) numUpdated++;
		else if (res_ret < 0) numRemoved++;
		if (motd_ret > 0) numUpdated++;
		else if (motd_ret < 0) numRemoved++;
		if (skl_ret > 0) numUpdated++;
		else if (skl_ret < 0) numRemoved++;
	}
	if (numUpdated > 0)
		println("Restored " + str(numUpdated) + " CFG/RES/MOTD files.");

	files = getDirFiles(path,"res");
	vector<string> motd_files = getDirFiles(path,"txt");
	files.insert(files.end(), motd_files.begin(), motd_files.end());
	for (uint f = 0; f < files.size(); f++)
	{
		string prefix = "";
		if (files[f].length() > 7)
		{
			prefix = getSubStr(files[f],0,7);
			if (matchStr(prefix,"gsrand_"))
			{
				remove( string(path + files[f]).c_str() );
				numRemoved++;
				continue;
			}
		}
	}
	if (numRemoved > 0)
		println("Deleted " + str(numRemoved) + " CFG/RES/MOTD files.");

	numRemoved = 0;
	files = getDirFiles(getWorkDir(),"wad");
	for (uint f = 0; f < files.size(); f++)
	{
		if (files[f].length() < 8)
			continue;
		string prefix = getSubStr(files[f],0,6);
		if (matchStrCase(prefix,"gsrand"))
		{
			numRemoved++;
			remove( string(getWorkDir() + files[f]).c_str() );
		}
	}

	if (numRemoved > 0)
		println("Deleted " + str(numRemoved) + " WADs.");
}

void printHelp()
{
	system("cls");
	cout << "\n                    Texture modes\n"
		 << "-------------------------------------------------------\n"
		 << "Write to map:\n"
		 << "     Textures are randomized and written into the BSP.\n"
		 << "     This mode is useful for playing with friends since\n" 
		 << "     only the BSP files need to be shared. This is the\n"
		 << "     only mode that can replace all map textures.\n\n"
		 << "Write WAD for each map:\n"
		 << "     Textures are written into WADs instead of the BSPs.\n"
		 << "     If used without a Map Prefix, players can join your\n"
		 << "     server without downloading any files. However, they\n"
		 << "     will see the normal map textures unless you provide\n"
		 << "     them your BSPs and WADs. Textures stored in the BSP\n"
		 << "     cannot be replaced.\n\n"
		 << "Write to a common WAD:\n"
		 << "     Like above, except all maps use a single WAD. Maps\n"
		 << "     will appear less random, but this is the most space\n"
		 << "     efficient texture replacement mode.\n\n"
		 << "1: Done\n"
		 << "0: Next Page\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1') return;
		if (choice == '0') break;
	}

	system("cls");
	cout << "\n                    Entity modes\n"
		<< "-------------------------------------------------------\n"
		<< "Super random:\n"
		<< "     All monsters have randomized render modes and tilts.\n"
		<< "     Moving objects change direction, speed, etc. The \n"
		<< "     randomizer will try its best to avoid making the map\n"
		<< "     impossible to complete. CVARs will also be set in\n"
		<< "     this mode.\n\n"
		<< "Appearance only:\n"
		<< "     No moving objects will be affected in this mode.\n"
		<< "     Only the appearances of entities will change.\n\n"
		<< "\n\n\n\n\n"
		<< "WARNING: Entity randomization can cause crashes or\n"
		<< "         prevent map completion on rare occasions.\n\n"
		<< "1: Done\n"
		<< "0: Next Page\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1') return;
		if (choice == '0') break;
	}

	system("cls");
	cout << "\n                  Corruption modes\n"
		<< "-------------------------------------------------------\n"
		<< "Super random:\n"
		<< "     Map geometry will be randomly flipped, scaled, and\n"
		<< "     distorted at random. Lightmaps and texture colors\n"
		<< "     will be randomly shifted, inverted, barfed on, etc.\n\n"
		<< "Use config settings:\n"
		<< "     All maps will use the same corruption settings.\n"
		<< "     Use this if you want to play an entire map series\n"
		<< "     upside-down or something. Specify corruption\n"
		<< "     settings in gsrand_config.txt\n\n"
		<< "\n\n\n\n\n"
		<< "WARNING: Map geometry randomization is buggy and may prevent\n"
		<< "         you from finishing a map without cheats.\n\n"
		<< "1: Done\n"
		<< "0: Next Page\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1') return;
		if (choice == '0') break;
	}

	system("cls");
	cout << "\n                     Model modes\n"
		<< "-------------------------------------------------------\n"
		<< "Super random:\n"
		<< "     All weapons, props, and monster models will be\n"
		<< "     replaced with random models of a random type.\n"
		<< "     You might end up with a headcrab for a crowbar.\n\n"
		<< "Use similar types:\n"
		<< "     Weapons will be replaced with random weapon\n"
		<< "     models, monsters with monster models, etc.\n\n"
		<< "You'll be equipped with a barnacle grapple in case a\n"
		<< "monster blocks your path due to missing animations\n"
		<< "(the grapple makes most monsters explode).\n\n"
		<< "\n\n\n"
		<< "WARNING: Model replacement is buggy and will cause crashes. \n"
		<< "         Check the 'model_safety' setting in gsrand_config.txt \n\n"
		<< "1: Done\n"
		<< "0: Next Page\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1') return;
		if (choice == '0') break;
	}

	system("cls");
	cout << "\n                     Sound modes\n"
		<< "-------------------------------------------------------\n"
		<< "Everything:\n"
		<< "     All world and monster sounds will be randomized.\n"
		<< "     This includes custom sentences for monsters that\n"
		<< "     speak (scientist, otis, etc.).\n\n"
		<< "World only:\n"
		<< "     Only world sounds will be randomized.\n"
		<< "     (weapons, buttons, doors, platforms, etc.)\n\n"
		<< "Monsters only:\n"
		<< "     Only monster sounds will be randomized.\n"
		<< "     Includes custom sentences.\n\n"
		<< "\n\n\n\n\n\n"
		<< "1: Done\n"
		<< "0: Next Page\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1') return;
		if (choice == '0') break;
	}

	system("cls");
	cout << "\n                    Content modes\n"
		<< "-------------------------------------------------------\n"
		<< "Everything:\n"
		<< "     The program will use any content it can find. Use\n"
		<< "     this option when the program is outside of your \n"
		<< "     svencoop folder for complete control over what\n"
		<< "     content gets used.\n\n"
		<< "Default only:\n"
		<< "     The program will only use the content that comes\n"
		<< "     included with Sven Co-op 4.8. Players won't have\n"
		<< "     to download any extra content when joining your\n"
		<< "     server (apart from randomized maps and WADs).\n\n"
		<< "Non-default only:\n"
		<< "     The program will only use content NOT included with\n"
		<< "     Sven Co-op 4.8. Any downloaded content and content\n"
		<< "     you've created will be used instead.\n\n"
		<< "\n\n"
		<< "1: Done\n"
		<< "0: Next Page\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1') return;
		if (choice == '0') break;
	}

	system("cls");
	cout << "\n                      Map prefix\n"
		<< "-------------------------------------------------------\n"
		<< "No Prefix:\n"
		<< "     Your maps will be randomized in place. Select\n"
		<< "     \"Undo all changes\" to restore your maps to normal.\n\n"
		<< "gsrand_:\n"
		<< "     New maps will be created with the gsrand_ prefix.\n"
		<< "     Select \"Undo all changes\" to delete these maps.\n\n"
		<< "XXXXXX_ (current time encoded in base36):\n"
		<< "     New maps will be created with a unique prefix to\n"
		<< "     avoid filename conflicts. This prefix is generated\n"
		<< "     using the current system time (YY/MM/DD HH:mm).\n"
		<< "     \"Undo all changes\" will not delete these maps.\n\n"
		<< "Custom:\n"
		<< "     You will be prompted to type your own map prefix.\n"
		<< "     \"Undo all changes\" will not delete these maps.\n\n"
		<< "\n"
		<< "1: Done\n"
		<< "0: Next Page\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1') return;
		if (choice == '0') break;
	}
	
}

int main(int argc, char* argv[])
{
 	srand ( (uint)time(NULL) );
	init_default_content();
	//srand (1337);
	//genSoundList();
	//get_all_models();
	//genSpriteList();
	//get_all_skies();
	//return 0;
	
	//getAllSounds();
	//return 0;

	while (true)
	{
		system("cls"); // WINDOWS ONLY
		cout << std::setw(80) << right << "version 2\n";
		cout << "Welcome to w00tguy's map randomizer!\n\n";

		cout << "Options:\n\n";
		cout << " 1) Texture Mode : ";
		if (texMode == TEX_MAP)            cout << "Write to map\n";
		else if (texMode == TEX_MASTERWAD) cout << "Write to a common WAD\n";
		else if (texMode == TEX_WADS)      cout << "Write WAD for each map\n";
		else if (texMode == TEX_NONE)      cout << "Do not change\n";

		cout << " 2) Entity  Mode : ";
		if (entMode == ENT_NONE)        cout << "Do not change\n";
		else if (entMode == ENT_SUPER)  cout << "Super random\n";
		else if (entMode == ENT_APPEAR) cout << "Appearances only\n";

		cout << " 3) Corrupt Mode : ";
		if (corruptMode == CORRUPT_NONE)        cout << "Do not corrupt\n";
		else if (corruptMode == CORRUPT_SUPER)  cout << "Super random\n";
		else if (corruptMode == CORRUPT_CONFIG) cout << "Use config settings\n";

		cout << " 4) Model   Mode : ";
		if (mdlMode == MDL_NONE)       cout << "Do not change\n";
		else if (mdlMode == MDL_CRAZY) cout << "Super random\n";
		else if (mdlMode == MDL_TYPED) cout << "Use similar types\n";

		cout << " 5) Sound   Mode : ";
		if (sndMode == SND_NONE)       cout << "Do not change\n";
		else if (sndMode == SND_ALL)   cout << "Everything\n";
		else if (sndMode == SND_WORLD) cout << "World only\n";
		else if (sndMode == SND_MONST) cout << "Monsters only\n";

		cout << " 6) Content Mode : ";
		if (contentMode == CONTENT_EVERYTHING)   cout << "Everything\n";
		else if (contentMode == CONTENT_DEFAULT) cout << "Default only\n";
		else if (contentMode == CONTENT_CUSTOM)  cout << "Non-default only\n";

		cout << " 7) Map Prefix   : ";
		if (prefixMode == PREFIX_GSRAND)      cout << "gsrand_\n";
		else if (prefixMode == PREFIX_TIME)   cout << MAP_PREFIX << " (current time encoded in base36)\n";
		else if (prefixMode == PREFIX_CUSTOM) cout << "Custom\n";
		else if (prefixMode == PREFIX_NONE)   cout << "No prefix (DANGEROUS)\n";

		cout << "\n";

		cout << "\nCommands:\n\n";
		cout << " 8) Randomize!\n";
		cout << " 9) Undo all changes\n";

		cout << "\n\nPress 0 for explanations on each option\n";

		char choice = _getch();
		
		if (choice == '1')
		{
			if (++texMode >= TEX_MODES)
				texMode = 0;			
		}
		else if (choice == '2')
		{
			if (++entMode >= ENT_MODES)
				entMode = 0;			
		}
		else if (choice == '3')
		{
			if (++corruptMode >= CORRUPT_MODES)
				corruptMode = 0;			
		}
		else if (choice == '4')
		{
			if (++mdlMode >= MDL_MODES)
				mdlMode = 0;			
		}
		else if (choice == '5')
		{
			if (++sndMode >= SND_MODES)
				sndMode = 0;			
		}
		else if (choice == '6')
		{
			if (++contentMode >= CONTENT_MODES)
				contentMode = 0;			
		}
		else if (choice == '7')
		{
			if (++prefixMode >= PREFIX_MODES)
				prefixMode = 0;	
			if (prefixMode == PREFIX_TIME)
				MAP_PREFIX = get_date_base36() + "_";
			else
				MAP_PREFIX = "gsrand_";
		}
		else if (choice == '8')
		{
			system("cls"); // WINDOWS ONLY
			initLists();

			bool should_pause = true;
			if (randomize_maps() == 0)
			{
				println("\nAll done!");

				println("\nWhat do you want to do with the randomized maps?");
				println("\n0) Nothing");
				println("1) Copy them into a folder");
				bool have_7zip = fileExists("7za.exe");
				if (have_7zip)
				{
					println("2) 7zip them with fast compression   (-mx1)");
					println("3) 7zip them with normal compression (-mx5)");
					//println("3) Yes - Don't use any compression     (-mx0)");
					if (dirExists("..\\svencoop_downloads"))
						println("\nNote: Content from 'svencoop_downloads' may be copied to the current folder");
				}

				int zip_action = 0;
				while(!zip_action)
				{
					choice = _getch();
					if (choice == '0') 
					{
						should_pause = false;
						break;
					}
					if (choice == '1') zip_action = 1;
					if (have_7zip && choice == '2') zip_action = 2;
					if (have_7zip && choice == '3') zip_action = 3;
					//if (choice == '3') zip_action = 3;
				}
					
				if (zip_action != 0)
				{
					println("\n\nBrace yourself. There are " + str(super_res_list.size()) + " files to process.\n");
					system("pause");
				}

				if (zip_action > 1) // compress to zip
				{
					println("\nPreparing file list...");
					string temp_dir = "gsrand_7zip_temp";
					ofstream fout;
					fout.open ("gsrand_7zip_list.txt", ios::out | ios::trunc);
					for (set<string>::iterator it = super_res_list.begin(); it != super_res_list.end(); ++it)
					{
						if (!fileExists(*it) && !fileExists("../valve/" + *it) && !fileExists("../svencoop_downloads/" + *it))
							println("Warning: Unable to find " + *it);
						else
						{									
							string path = *it;
							if (!fileExists(*it))
							{
								string old_path = path;
								if (fileExists("../valve/" + *it))
									old_path = "../valve/" + path;
								if (fileExists("../svencoop_downloads/" + *it))
									old_path = "../svencoop_downloads/" + path;

								if (!matchStr(old_path, path))
								{
									string old_path_win = old_path;
									string path_win = path;
									replace( old_path_win.begin(), old_path_win.end(), '/', '\\');
									replace( path_win.begin(), path_win.end(), '/', '\\');
									if (path_win.find("\\") != string::npos)
									{
										string dir = getSubStr(path_win, 0, path_win.find_last_of("\\"));
										string cmd = "mkdir \"" + dir + "\" 2> nul"; 
										system(cmd.c_str());
									}

									string cmd = "copy /Y \"" + old_path_win + "\" \"" + path_win + "\" > nul";
									system(cmd.c_str());
									println("Copying " + old_path_win);
								}
							}

							std::replace( path.begin(), path.end(), '/', '\\'); // convert to windows slashes				
							fout << path << "\n";
						}
					}
					fout.close();
					string output_prefix = "gsrand_output_";
					if (zip_action == 2) output_prefix += "mx1";
					if (zip_action == 3) output_prefix += "mx5";
					//if (zip_action == 3) output_prefix += "mx0";
					string output_name = output_prefix + ".zip";
					for (int i = 2; i < 100; i++)
					{
						if (fileExists(output_name))
							output_name = output_prefix + "_" + str(i) + ".zip";
						else
							break;
					}
					if (fileExists(output_name))
						remove(output_name.c_str());
					string level;
					if (zip_action == 2) level = "-mx1";
					if (zip_action == 3) level = "-mx5";
					//if (zip_action == 3) level = "-mx0";
					string cmd = "7za.exe a " + level + " -mmt -tzip " + output_name + " @gsrand_7zip_list.txt";
					system(cmd.c_str());
					remove("gsrand_7zip_list.txt");
					if (fileExists(output_name))
					{
						println("\nThe archive was named '" + output_name + "' and is located here:");
						println(getWorkDir() + output_name);
					}
				}
				else if (zip_action == 1) // just copy
				{
					string output_prefix = "gsrand_output";
					string output_name = output_prefix;
					for (int i = 2; i < 100; i++)
					{
						if (dirExists(output_name))
							output_name = output_prefix + "_" + str(i);
						else
							break;
					}

					string cmd = "mkdir \"" + output_name + "\" 2> nul"; 
					system(cmd.c_str());

					for (set<string>::iterator it = super_res_list.begin(); it != super_res_list.end(); ++it)
					{
						if (!fileExists(*it) && !fileExists("../valve/" + *it) && !fileExists("../svencoop_downloads/" + *it))
							println("Warning: Unable to find " + *it);
						else
						{									
							string path = *it;
							std::replace( path.begin(), path.end(), '/', '\\');
							string old_path = path;
							if (!fileExists(*it))
							{
								if (fileExists("..\\valve\\" + *it))
									old_path = "..\\valve\\" + path;
								if (fileExists("..\\svencoop_downloads\\" + *it))
									old_path = "..\\svencoop_downloads\\" + path;
							}

							int dir = path.find_last_of("\\");
							if (dir != string::npos)
							{
								string mdir = getSubStr(path, 0, dir);
								string cmd = "mkdir \"" + output_name + "\\" + mdir + "\" 2> nul"; 
								system(cmd.c_str());
							}

							string cmd = "copy /Y \"" + old_path + "\" \"" + output_name + "\\" + path + "\" > nul";
							system(cmd.c_str());
							//println(cmd);
							println("Copying " + old_path);
						}
					}

					if (dirExists(output_name))
					{
						println("\nThe folder was named '" + output_name + "' and is located here:");
						println(getWorkDir() + output_name);
					}
				}

				cout << endl;
				writeLog();
				
				if (should_pause) 
				{
					println("Let's all take a moment to praise the program for its accomplishments.\n");
					system("pause");
				}
			}
		}
		else if (choice == '9')
		{
			system("cls"); // WINDOWS ONLY
			undoEverything();
			print("\nDone. ");
			writeLog();
			system("pause");
		}
		else if (choice == '0')
		{
			printHelp();
		}
	}
		
	return 0;
}

