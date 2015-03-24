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
int grapple_mode = GRAPPLE_HOOK;
int tex_embed_mode = EMBED_NORMAL;
string MAP_PREFIX = "gsrand_";
string random_seed;
int modelSafety = MODEL_SAFETY_TEX_LIMIT;
bool cheatNoclip = true;
bool cheatImpulse = true;
bool cheatGodmode = true;
int lightMode = 0;
int vertMode = 0;

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

string wadPath = "";
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
set<string> super_res_list;

list_hashmap monster_whitelists;
list_hashmap monster_blacklists;

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
				if (setting_name.find("grapple_mode")   == 0) grapple_mode = atoi(setting_value.c_str());
				if (setting_name.find("tex_embed_mode") == 0) tex_embed_mode = atoi(setting_value.c_str());
				if (setting_name.find("model_safety")   == 0) modelSafety = atoi(setting_value.c_str());
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

	// todo: randomize?
	for (int i = 0; i < NUM_SKILL_SETTINGS; i++)
	{
		string skill_setting = skill_settings[i];
	}

	for (int i = 1; i < 4; i++)
	{
		fout << "sk_plr_crowbar" << i << " \"9001\"" << "\n";
		fout << "sk_plr_wrench" << i << " \"9001\"" << "\n";
		fout << "sk_plr_grapple" << i << " \"9001\"" << "\n";
		fout << "sk_plr_secondarygauss" << i << " \"1000\"" << "\n";
		fout << "sk_plr_egon_narrow" << i << " \"9001\"" << "\n";
		fout << "sk_plr_egon_wide" << i << " \"9001\"" << "\n";
		fout << "sk_plr_HpMedic" << i << " \"100\"" << "\n";
		fout << "sk_nihilanth_zap" << i << " \"9001\"" << "\n";
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
				fout << text[i] << endl;
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
	if (texMode != TEX_NONE)
		fout << "    - You can use the barnacle weapon to latch on to walls and swing around like spiderman.\n";
	if (mdlMode != MDL_NONE)
		fout << "    - You can use the barnacle weapon to explode monsters that block your path.\n";
	if (entMode == ENT_SUPER)
		fout << "    - All doors are breakable. Certain walls and other objects are also breakable.\n";
	if (mdlMode != MDL_NONE)
		fout << "    - The 'Texture Overflow: MAX_GLTEXTURES' error is client-side. Just rejoin when this happens.\n";
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

	fout << "    Map Prefix: ";
	if (prefixMode == PREFIX_NONE)   fout << "No prefix\n";
	else							 fout << MAP_PREFIX + "\n";

	fout << "    grapple_mode: " << grapple_mode << endl;
	fout << "    gmr_only: " << (int)gmr_only << endl;
	fout << "    tex_embed_mode: " << tex_embed_mode << endl;
	if (random_seed.length())
		fout << "    random_seed: " << random_seed << endl;

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
				fout << endl;
		}
	}
	fout.close();

	return true;
}

// only load the texture data from a map
byte * loadTextureChunk(string mapname, int& lump_len)
{
	string filename = getWorkDir() + "maps/" + mapname + ".bsp";
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

vector<string> create_res_list(Entity ** ents)
{
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
			if (ents[i]->hasKey("gibmodel"))
				res_list.insert(ents[i]->keyvalues["gibmodel"]);	
		else if (matchStr(cname, "item_recharge") || matchStr(cname, "item_healthcharger"))
			if (ents[i]->hasKey("model_juice"))
				res_list.insert(ents[i]->keyvalues["model_juice"]);			
		

		else if (cname.find("monster_") == 0 || cname.find("ammo_") == 0
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
		while (name.length() > 1 && (name[0] =='/' || name[0] =='\\'))
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

		filtered.push_back(name);
		if (!fileExists(name))
		{
			if (name.find(".wad") != string::npos && name.find("gsrand") == 0)
				continue; // probably hasn't been generated yet
			println("WARNING: Missing resource - '" + name + "'"); 
			continue;
		}
		if (name.find(".mdl") != string::npos)
		{
			string m_name = getSubStr(name, 0, name.length()-4);
			string t_model = name + "T.mdl";
			if (fileExists(t_model))
				filtered.push_back(t_model);

			int id = 0;
			string a_model = m_name + "01.mdl";
			while (fileExists(a_model))
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
	fout.open (path + mapname + ".res", ios::out | ios::trunc);

	gsrand_res_identifier = "// This resource file was generated by w00tguy's randomizer.";
	fout << "// This resource file was generated by w00tguy's randomizer.\n";
	fout << "// Listed resources include sounds, models, sprites, wads, and skyboxes not included with SC 4.8.\n";
	fout << "// It also includes map cfgs, global sound/model replacement files, and this resource file.\n";
	fout << "// Dependencies for models are included if they exist (e.g. Tor.mdl + TorT.mdl + Tor01.mdl).\n\n";
	for (uint i = 0; i < res_files.size(); ++i)
		fout << res_files[i] << endl;
	
	fout.close();
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

int randomize_maps()
{
	parse_settings_file();

	string path = getWorkDir() + "maps/";
	vector<string> files = user_maps;

	int missing = 0;
	if (user_maps.empty())
		files = getDirFiles(getWorkDir() + "maps/","bsp");
	else
	{
		for (uint i = 0; i < files.size(); ++i)
		{
			if (!fileExists(path + files[i]))
			{
				files.erase(files.begin() + i--);
				missing++;
			}
		}
	}

	if (user_maps.empty())
		cout << "Found " << files.size() << " maps\n\n";
	else
		cout << "Found " << files.size() << " of " << (files.size() + missing) << " maps\n\n";

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

	vector<Wad> wads;
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
		for (uint i = 0; i < files.size(); ++i)
		{	
			string new_name = MAP_PREFIX + files[i];
			for (uint k = 0; k < files.size(); ++k)
				if (matchStr(new_name, files[k]))
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

	Entity *** entLists = new Entity**[files.size()];
	int idx = 0;

	system("cls");

	every_random_replacement.clear();
	super_res_list.clear();

	for (uint f = 0; f < files.size(); f++)
	{
		res_list.clear();
		string mapName = getSubStr(files[f], 0, files[f].length()-4);
		if (verbose)
		{
			println("\n        " + mapName);
			println("---------------------------");
		}

		string fpath = path + files[f];
		string prefix = prefixMode == PREFIX_NONE ? "" : MAP_PREFIX;

		if (!verbose)
			print(prefix + mapName + " --- ");

		BSP * map = loadBSP(mapName, true);
		if (map == NULL)
			continue;
		int numEnts;
		Entity** ents = getMapEnts(map, false, numEnts);

		if (prefixMode != PREFIX_NONE)
			map->name = MAP_PREFIX + map->name;

		if (texMode != TEX_NONE)
		{
			string wad_name = map->name;
			if (prefixMode == PREFIX_NONE)
				wad_name = "gsrand_" + map->name;
			int tex = makeMapWad(map, wad_name, wads);
			randomize_skybox(ents);
			if (!verbose)
				print(str(tex) + " textures. ");
		}	

		if (sndMode != SND_NONE)
			do_ent_sounds(ents, map->name);

		if (entMode != ENT_NONE)
		{
			if (!verbose)
				print(str(numEnts) + " ents. ");
			do_entity_randomization(ents, map->name);
			if (entMode == ENT_SUPER)
				createSKL(path, mapName);
		}
		add_gsrand_ents(ents);

		if (mdlMode != MDL_NONE)
			do_model_replacement(map, ents, path, mapName);

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
			
			if (!verbose)
				print(str(writables.size()) + " sounds.");
		}

		if (prefixMode != PREFIX_NONE)
			update_changelevels(ents, map->name);

		if (true)
		{
			corrupt_map_verts(map, ents);
			corrupt_map_lightmap(map);
		}

		if (texMode != TEX_NONE || sndMode != SND_NONE || entMode != ENT_NONE || mdlMode != MDL_NONE || prefixMode != PREFIX_NONE)
		{
			needsRipent(map, ents);
			ripent(map, ents, false);
		}

		createMOTD(path, mapName);

		vector<string> res_list = create_res_list(ents);
		create_res_file(res_list, ents, path, mapName);
		for (uint i = 0; i < res_list.size(); ++i)
			super_res_list.insert(toLowerCase(res_list[i]));
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
		
		println("");
	}

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

	if (numOverflow > 0)
		println("\n" + str(numOverflow) + " maps had too many sounds to replace.");

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
		<< "     Only the render styles of monsters will change.\n\n"
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
		<< "     replaced with random models of a random type\n"
		<< "     You might end up with a headcrab for a crowbar.\n\n"
		<< "Use similar types:\n"
		<< "     Weapons will be replaced with random weapon\n"
		<< "     models, props with prop models, etc. Monsters that\n"
		<< "     are used in scripted sequences aren't affected\n"
		<< "     since missing animations can stop map progression.\n\n"
		<< "You'll be equipped with a barnacle grapple in case a\n"
		<< "monster blocks your path due to missing animations\n"
		<< "(the grapple makes most monsters explode).\n\n"
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
		<< "1: Done\n"
		<< "0: Next Page\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1') return;
		if (choice == '0') break;
	}

	system("cls");
	cout << "\n                   Content modes\n"
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
		

		cout << "Options:\n";
		cout << "1: Texture Mode: ";
		if (texMode == TEX_MAP)            cout << "Write to map\n";
		else if (texMode == TEX_MASTERWAD) cout << "Write to a common WAD\n";
		else if (texMode == TEX_WADS)      cout << "Write WAD for each map\n";
		else if (texMode == TEX_NONE)      cout << "Do not change\n";

		cout << "2: Entity Mode : ";
		if (entMode == ENT_NONE)        cout << "Do not change\n";
		else if (entMode == ENT_SUPER)  cout << "Super random\n";
		else if (entMode == ENT_APPEAR) cout << "Appearances only\n";

		cout << "2: Corrupt Mode: ";
		if (corruptMode == CORRUPT_NONE)        cout << "Do not corrupt\n";
		else if (corruptMode == CORRUPT_SUPER)  cout << "Super random\n";
		else if (corruptMode == CORRUPT_CONFIG) cout << "Use config setting\n";

		cout << "3: Model Mode  : ";
		if (mdlMode == MDL_NONE)       cout << "Do not change\n";
		else if (mdlMode == MDL_CRAZY) cout << "Super random\n";
		else if (mdlMode == MDL_TYPED) cout << "Use similar types\n";

		cout << "4: Sound Mode  : ";
		if (sndMode == SND_NONE)       cout << "Do not change\n";
		else if (sndMode == SND_ALL)   cout << "Everything\n";
		else if (sndMode == SND_WORLD) cout << "World only\n";
		else if (sndMode == SND_MONST) cout << "Monsters only\n";

		cout << "5: Content Mode: ";
		if (contentMode == CONTENT_EVERYTHING)   cout << "Everything\n";
		else if (contentMode == CONTENT_DEFAULT) cout << "Default only\n";
		else if (contentMode == CONTENT_CUSTOM)  cout << "Non-default only\n";

		cout << "6: Map Prefix  : ";
		if (prefixMode == PREFIX_GSRAND)      cout << "gsrand_\n";
		else if (prefixMode == PREFIX_TIME)   cout << MAP_PREFIX << " (current time encoded in base36)\n";
		else if (prefixMode == PREFIX_CUSTOM) cout << "Custom\n";
		else if (prefixMode == PREFIX_NONE)   cout << "No prefix\n";

		cout << endl;

		cout << "7: Randomize!" << endl;
		cout << "8: Undo all changes\n" << endl;

		cout << "0: Help" << endl;

		char choice = _getch();
		if (choice == '7')
		{
			system("cls"); // WINDOWS ONLY
			initLists();
			if (randomize_maps() == 0)
			{
				println("\nDone.");

				if (fileExists("7za.exe"))
				{
					println("\nDo you want to pack the randomized maps into a 7zip archive?");
					println("\n1) No");
					println("2) Yes - Use fast compression          (-mx3)");
					println("3) Yes - Use normal compression        (-mx5)");

					int zip_action = 0;
					while(!zip_action)
					{
						choice = _getch();
						if (choice == '1') break;
						if (choice == '2') zip_action = 1;
						if (choice == '3') zip_action = 2;
						if (choice == '4') zip_action = 3;
					}
					
					if (zip_action)
					{
						println("\nPreparing file list...");
						string temp_dir = "gsrand_7zip_temp";
						ofstream fout;
						fout.open ("gsrand_7zip_list.txt", ios::out | ios::trunc);
						for (set<string>::iterator it = super_res_list.begin(); it != super_res_list.end(); ++it)
						{
							if (!fileExists(*it))
								println("Warning: Unable to find " + *it);
							else
							{
								string path = *it;
								std::replace( path.begin(), path.end(), '/', '\\'); // convert to windows slashes				
								fout << path << "\n";
							}
						}
						fout.close();
						remove("gsrand_output.zip");
						string level;
						if (zip_action == 1) level = "-mx3";
						if (zip_action == 2) level = "-mx5";
						string cmd = "7za.exe a " + level + " -mmt -tzip gsrand_output.zip @gsrand_7zip_list.txt";
						system(cmd.c_str());
						remove("gsrand_7zip_list.txt");
					}
				}

				cout << endl;
				writeLog();
				system("pause");
			}
		}
		else if (choice == '8')
		{
			system("cls"); // WINDOWS ONLY
			undoEverything();
			print("\nDone. ");
			writeLog();
			system("pause");
		}
		else if (choice == '1')
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
			if (++mdlMode >= MDL_MODES)
				mdlMode = 0;			
		}
		else if (choice == '4')
		{
			if (++sndMode >= SND_MODES)
				sndMode = 0;			
		}
		else if (choice == '5')
		{
			if (++contentMode >= CONTENT_MODES)
				contentMode = 0;			
		}
		else if (choice == '6')
		{
			if (++prefixMode >= PREFIX_MODES)
				prefixMode = 0;	
			if (prefixMode == PREFIX_TIME)
				MAP_PREFIX = get_date_base36() + "_";
			else
				MAP_PREFIX = "gsrand_";
		}
		else if (choice == '0')
		{
			printHelp();
		}
	}
		
	return 0;
}

