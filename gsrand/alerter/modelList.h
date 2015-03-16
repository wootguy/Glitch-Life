#pragma once
#include <string>
#include <set>
#include <unordered_map>
#include <fstream>
#include "Util.h"

using namespace std;

typedef tr1::unordered_map< string, vector<string> > list_hashmap;
typedef tr1::unordered_map< string, string > string_hashmap;
typedef tr1::unordered_map< string, int > height_hashmap;

extern string_hashmap default_monster_models;
extern string_hashmap default_friendly_monster_models;
extern string_hashmap default_weapon_models;
extern string_hashmap default_w_weapon_models;
extern string_hashmap default_p_weapon_models;
extern string_hashmap default_v_weapon_models;
extern string_hashmap default_ammo_models;
extern string_hashmap default_item_models;
extern vector<string> default_gib_models;

extern vector<string> gmr_replace_only; // models only replacable through GMR

extern list_hashmap monster_whitelists; // set of models that are tested to be working for the monster
extern list_hashmap monster_blacklists; // set of models that would crash the game if they were used as replacements


extern vector<string> monster_sprites;
extern vector<string> dont_replace; // shouldn't be replaced
extern vector<string> weapon_types;
extern height_hashmap default_monster_heights;

// to regenerate:
// run "mcache" command
// copy output to tet file. Edit with NP++
// find and replace regex ".+: " with "default_precache_models.push_back\(""
// find and replace regex "\(.*$" with ""\);"
// select all text, right click, set to lowercase
// remove the entries that aren't actually models (e.g. "(!P)" or "maps/mapname.bsp")
// remove doctor.mdl (seems to be a broken model in HL?)

static void init_default_monster_heights()
{
	default_monster_heights["monster_headcrab"] = 24;
	default_monster_heights["monster_chumtoad"] = 12;
	default_monster_heights["monster_houndeye"] = 36;
	default_monster_heights["monster_bullchicken"] = 64;
	default_monster_heights["monster_alien_grunt"] = 64;
	default_monster_heights["monster_ichthyosaur"] = 64;
	default_monster_heights["monster_stukabat"] = 24;
}

static void parse_model_lists()
{
	ifstream myfile(getWorkDir() + "gsrand_models.txt");
	if (myfile.is_open())
	{
		vector<string> current_targets; // which classnames the models apply to
		list_hashmap * current_list = &monster_blacklists; // whether we're in a whitelist or blacklist mode

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
					if (name.find("WHITELISTS_BEGIN") != string::npos)
						current_list = &monster_whitelists;
					else if (name.find("BLACKLISTS_BEGIN") != string::npos)
						current_list = &monster_blacklists;
					else
					{
						current_targets.clear();
						int and = name.find("&");
						if (and != string::npos)
							current_targets = splitString(name, "&");
						else
							current_targets.push_back(name);
					}
				}
				continue;
			}

			if (line.find("models") == 0 || line.find("*") == 0)
				for (uint i = 0; i < current_targets.size(); ++i)
					(*current_list)[current_targets[i]].push_back(line);
			
		}
		myfile.close();
	}
	else
		println("WARNING: gsrand_models.txt is missing. I hope you know what you're doing D:");
}

static void init_default_model_lists()
{
	default_gib_models.clear();
	gmr_replace_only.clear();
	default_monster_models.clear();
	default_friendly_monster_models.clear();
	default_weapon_models.clear();
	default_ammo_models.clear();
	default_item_models.clear();
	weapon_types.clear();
	monster_sprites.clear();

	monster_sprites.push_back("sprites/mommaspit.spr"); // momma spit
	monster_sprites.push_back("sprites/mommaspout.spr"); // momma spit splash
	monster_sprites.push_back("sprites/bigspit.spr"); // bullsquid barf
	monster_sprites.push_back("sprites/tinyspit.spr"); // bullsquid barf splash
	monster_sprites.push_back("sprites/xspark4.spr"); // controller orb

	dont_replace.push_back("sprites/bloodspray.spr");
	dont_replace.push_back("sprites/blood.spr");

	default_gib_models.push_back("models/glassgibs.mdl");
	default_gib_models.push_back("models/woodgibs.mdl");
	default_gib_models.push_back("models/metalplategibs.mdl");
	default_gib_models.push_back("models/fleshgibs.mdl");
	default_gib_models.push_back("models/cindergibs.mdl");
	default_gib_models.push_back("models/ceilinggibs.mdl");
	default_gib_models.push_back("models/computergibs.mdl");
	default_gib_models.push_back("models/rockgibs.mdl");

	// models that can only be replaced with GMR
	gmr_replace_only.push_back("models/roach.mdl");

	default_monster_models["alien_babyvoltigore"] = "baby_voltigore";
	default_monster_models["alien_controller"]    = "controller";
	default_monster_models["alien_grunt"]         = "agrunt";
	default_monster_models["alien_slave"]         = "islave";
	default_monster_models["alien_tor"]           = "Tor";
	default_monster_models["alien_voltigore"]     = "voltigore";
	default_monster_models["apache"]              = "apache";
	default_monster_models["assassin_repel"]      = "massn";
	default_monster_models["babycrab"]			  = "baby_headcrab";
	default_monster_models["babygarg"]            = "babygarg";
	default_monster_models["barnacle"]            = "barnacle";
	default_monster_models["barney"]              = "barney";
	default_monster_models["barney_dead"]         = "barney"; 
	default_monster_models["bigmomma"]            = "big_mom";
	default_monster_models["blkop_osprey"]        = "blkop_osprey";
	default_monster_models["bloater"]             = "floater";
	default_monster_models["bodyguard"]           = "bgman";
	default_monster_models["bullchicken"]         = "bullsquid";
	default_monster_models["chumtoad"]            = "chumtoad";
	default_monster_models["cleansuit_scientist"] = "cleansuit_scientist";
	default_monster_models["cockroach"]           = "roach";
	default_monster_models["flyer_flock"]         = "boid";
	default_monster_models["gargantua"]           = "garg";
	default_monster_models["gman"]                = "gman";
	default_monster_models["gonome"]              = "gonome";
	default_monster_models["grunt_ally_repel"]    = "hgrunt";
	default_monster_models["grunt_repel"]         = "hgrunt";
	default_monster_models["handgrenade"]         = "w_grenade";
	default_monster_models["headcrab"]            = "headcrab";
	default_monster_models["hevsuit_dead"]        = "wew"; // ??????????????????????????????
	default_monster_models["hgrunt_dead"]         = "hgrunt";
	default_monster_models["houndeye"]            = "houndeye";
	default_monster_models["human_assassin"]      = "massn";
	default_monster_models["female_assassin"]     = "hassassin";
	default_monster_models["human_grunt"]         = "hgrunt";
	default_monster_models["human_grunt_ally"]    = "hgrunt_opforf";
	default_monster_models["human_grunt_ally_dead"] = "hgruntf";
	default_monster_models["human_grunt_dead"]    = "hgrunt";
	default_monster_models["human_medic"]         = "hgrunt_medic";
	default_monster_models["human_medic_ally"]    = "hgrunt_medicf";
	default_monster_models["medic_ally_repel"]    = "hgrunt_medicf";
	default_monster_models["human_medic_ally_repel"] = "hgrunt_medicf";
	default_monster_models["human_torch"]         = "hgrunt_torch";
	default_monster_models["human_torch_ally"]    = "hgrunt_torchf";
	default_monster_models["hwgrunt"]             = "hwgrunt";
	default_monster_models["hwgrunt_repel"]       = "hwgrunt";
	default_monster_models["ichthyosaur"]          = "icky";
	default_monster_models["kingpin"]             = "kingpin";
	default_monster_models["leech"]               = "leech";
	default_monster_models["male_assassin"]       = "massn";
	default_monster_models["miniturret"]          = "miniturret";
	default_monster_models["nihilanth"]           = "nihilanth";
	default_monster_models["osprey"]              = "osprey";
	default_monster_models["otis"]                = "otis";
	default_monster_models["otis_dead"]           = "otis";
	default_monster_models["pitdrone"]            = "pit_drone";
	default_monster_models["rat"]                 = "bigrat";
	default_monster_models["robogrunt"]           = "rgrunt";
	default_monster_models["robogrunt_repel"]     = "rgrunt";
	default_monster_models["satchel"]             = "w_satchel";
	default_monster_models["scientist"]           = "scientist";
	default_monster_models["scientist_dead"]      = "scientist";
	default_monster_models["sentry"]              = "sentry";
	default_monster_models["shockroach"]          = "w_shock_rifle";
	default_monster_models["shocktrooper"]        = "strooper";
	default_monster_models["sitting_scientist"]   = "scientist";
	default_monster_models["snark"]               = "w_squeak";
	default_monster_models["sqknest"]             = "w_sqknest";
	default_monster_models["stukabat"]            = "stukabat";
	default_monster_models["tentacle"]            = "tentacle2";
	default_monster_models["torch_ally_repel"]    = "hgrunt_torchf";
	default_monster_models["tripmine"]            = "wow"; // ?????????????????
	default_monster_models["turret"]              = "turret";
	default_monster_models["zombie"]              = "zombie";
	default_monster_models["zombie_barney"]       = "zombie_barney";
	default_monster_models["zombie_soldier"]      = "zombie_soldier";
	default_monster_models["zombie_grunt"]        = "zombie_soldier";
	default_monster_models["rosenberg"]           = "wheelchair_sci";
	default_monster_models["cleansuit_scientist_dead"] = "cleansuit_scientist";
	default_monster_models["alien_slave_dead"]    = "islave";
	default_monster_models["houndeye_dead"]       = "houndeye_dead";
	default_monster_models["blkop_apache"]        = "blkop_apache";
	default_monster_models["geneworm"]            = "geneworm";

	default_friendly_monster_models["apache"]              = "apachef";
	default_friendly_monster_models["assassin_repel"]      = "massnf";
	default_friendly_monster_models["babygarg"]            = "babygargf";
	default_friendly_monster_models["barney"]              = "barnabus";
	default_friendly_monster_models["barney_dead"]         = "barnabus";
	default_friendly_monster_models["grunt_ally_repel"]    = "hgruntf";
	default_friendly_monster_models["grunt_repel"]         = "hgruntf";
	default_friendly_monster_models["hgrunt_dead"]         = "hgruntf";
	default_friendly_monster_models["human_assassin"]      = "hassassinf";
	default_friendly_monster_models["human_grunt"]         = "hgruntf";
	default_friendly_monster_models["human_grunt_ally"]    = "hgrunt_opfor";
	default_friendly_monster_models["human_grunt_ally_dead"] = "hgrunt";
	default_friendly_monster_models["human_medic_ally"]    = "hgrunt_medic";
	default_friendly_monster_models["human_torch_ally"]    = "hgrunt_torch";
	default_friendly_monster_models["hwgrunt"]             = "hwgruntf";
	default_friendly_monster_models["hwgrunt_repel"]       = "hwgruntf";
	default_friendly_monster_models["male_assassin"]       = "massnf";
	default_friendly_monster_models["otis"]                = "otisf";
	default_friendly_monster_models["otis_dead"]           = "otisf";
	default_friendly_monster_models["robogrunt"]           = "rgruntf";
	default_friendly_monster_models["robogrunt_repel"]     = "rgruntf";
	default_friendly_monster_models["torch_ally_repel"]    = "hgrunt_torch";
	default_friendly_monster_models["human_grunt_dead"]    = "hgruntf";
	default_friendly_monster_models["human_medic"]         = "hgrunt_medicf";
	default_friendly_monster_models["human_torch"]         = "hgrunt_torchf";
	default_friendly_monster_models["female_assassin"]     = "hassassinf";
	default_friendly_monster_models["medic_ally_repel"]    = "hgrunt_medic";
	default_friendly_monster_models["human_medic_ally_repel"] = "hgrunt_medic";

	default_weapon_models["357"]           = "357";
	default_weapon_models["9mmar"]         = "9mmar";
	default_weapon_models["9mmhandgun"]    = "9mmhandgun";
	default_weapon_models["crossbow"]      = "crossbow";
	default_weapon_models["crowbar"]       = "crowbar";
	default_weapon_models["eagle"]         = "desert_eagle";
	default_weapon_models["egon"]          = "egon";
	default_weapon_models["gauss"]         = "gauss";
	default_weapon_models["grapple"]       = "bgrap";
	default_weapon_models["handgrenade"]   = "grenade";
	default_weapon_models["hornetgun"]     = "hgun";
	default_weapon_models["m16"]           = "m16";
	default_weapon_models["m249"]          = "saw";
	default_weapon_models["medkit"]        = "medkit";
	default_weapon_models["minigun"]       = "minigun";
	default_weapon_models["pipewrench"]    = "pipe_wrench";
	default_weapon_models["rpg"]           = "rpg";
	default_weapon_models["satchel"]       = "satchel";
	default_weapon_models["shotgun"]       = "shotgun";
	default_weapon_models["snark"]         = "squeak";
	default_weapon_models["sniperrifle"]   = "m40a1";
	default_weapon_models["sporelauncher"] = "spore_launcher";
	default_weapon_models["tripmine"]      = "tripmine";
	default_weapon_models["uzi"]           = "uzi";
	default_weapon_models["uziakimbo"]     = "2uzis";
	default_weapon_models["uzi_gold"]      = "uzi_gold"; 
	default_weapon_models["uziakimbo_gold"]= "2uzis_gold";
	default_weapon_models["satchel_radio"] = "satchel_radio";

	for (string_hashmap::iterator it = default_weapon_models.begin(); it != default_weapon_models.end(); ++it)
	{
		default_w_weapon_models[it->first] = it->second;
		default_p_weapon_models[it->first] = it->second;
		default_v_weapon_models[it->first] = it->second;
	}
	default_v_weapon_models["m16"] = "m16a2";
	default_p_weapon_models["9mmar"] = "9mmAR";
	default_v_weapon_models["uziakimbo"] = "uzi";
	default_v_weapon_models["uzi_gold"] = "uzi";
	default_v_weapon_models["uziakimbo_gold"] = "uzi";

	default_ammo_models["357"]        = "357ammo"; // maybe ammobox?
	default_ammo_models["556"]        = "saw_clip";
	default_ammo_models["762"]        = "m40a1clip";
	default_ammo_models["argrenades"] = "argrenade";
	default_ammo_models["mp5grenades"] = "argrenade";
	default_ammo_models["9mmar"]      = "9mmarclip";
	default_ammo_models["9mmbox"]     = "chainammo";
	default_ammo_models["9mmclip"]    = "9mmarclip";
	default_ammo_models["mp5clip"]    = "9mmarclip";
	default_ammo_models["9mmarclip"]  = "9mmarclip";
	default_ammo_models["glockclip"]  = "9mmarclip";
	default_ammo_models["buckshot"]   = "shotbox";
	default_ammo_models["crossbow"]   = "crossbow_clip";
	default_ammo_models["gaussclip"]  = "gaussammo";
	default_ammo_models["rpgclip"]    = "rpgammo";
	default_ammo_models["spore"]      = "spore"; // ????
	default_ammo_models["sporeclip"]  = "spore_ammo"; // ????

	// TODO: ACtually find the default models. None of these are right
	default_item_models["airtank"] = "airtank";
	default_item_models["antidote"] = "antidote";
	default_item_models["armorvest"] = "armorvest";
	default_item_models["battery"] = "battery";
	default_item_models["healthcharger"] = "healthcharger";
	default_item_models["healthkit"] = "healthkit";
	default_item_models["helmet"] = "helmet";
	default_item_models["longjump"] = "longjump";
	default_item_models["recharge"] = "recharge";
	default_item_models["security"] = "security";
	default_item_models["suit"] = "suit";

	for (string_hashmap::iterator it = default_weapon_models.begin(); it != default_weapon_models.end(); ++it)
		weapon_types.push_back(it->first);

	init_default_monster_heights();
}

// weapons are precached too but we handle those differently
#define NUM_DEFAULT_PRECACHE 42
static const char * DEFAULT_PRECACHE[NUM_DEFAULT_PRECACHE] =
{
	"models/shotgunshell.mdl",
	"models/shell.mdl",
	"models/saw_shell.mdl",
	"models/grenade.mdl",
	"models/shock_effect.mdl",
	"models/spore.mdl",
	"models/spore_ammo.mdl",
	"models/rpgrocket.mdl",
	"models/crossbow_bolt.mdl",
	"models/cretegibs.mdl",
	"models/chumtoad.mdl",
	"models/hornet.mdl",
	"models/player.mdl",
	"models/hgibs.mdl",
	"models/agibs.mdl",
	"models/p_rpg_r.mdl",

	"sprites/tongue.spr",
	"sprites/blueflare1.spr",
	"sprites/redflare2.spr",
	"sprites/glow01.spr",
	"sprites/spore_exp_01.spr",
	"sprites/spore_exp_c_01.spr",
	"sprites/pitdronespit.spr",
	"sprites/hotglow.spr",
	"sprites/smoke.spr",
	"sprites/laserdot.spr",
	"sprites/streak.spr",
	"sprites/xbeam1.spr",
	"sprites/XSpark1.spr",
	"sprites/muz1.spr",
	"sprites/laserbeam.spr",
	"sprites/zerogxplode.spr",
	"sprites/WXplo1.spr",
	"sprites/steam1.spr",
	"sprites/bubble.spr",
	"sprites/bloodspray.spr",
	"sprites/blood.spr",
	"sprites/wep_smoke_01.spr",
	"sprites/explode1.spr",
	"sprites/saveme.spr",
	"sprites/grenade.spr",
	"sprites/muzzleflash_saw.spr",
};

#define NUM_APACHE_MODELS 124
static const char * APACHE_MODELS[NUM_APACHE_MODELS] =
{
	"apache",
	"apachef",
	"babygarg",
	"babygargf",
	"baby_voltigore",
	"barnabus",
	"barney",
	"bgman",
	"blkop_apache",
	"blkop_osprey",
	"cleansuit_scientist",
	"drill",
	"garg",
	"gman",
	"hgrunt",
	"hgruntf",
	"hgrunt_medic",
	"hgrunt_medicf",
	"hgrunt_opfor",
	"hgrunt_opforf",
	"hgrunt_sniper",
	"hgrunt_torch",
	"hgrunt_torchf",
	"highdef_otis",
	"icky",
	"intro_commander",
	"intro_medic",
	"intro_regular",
	"intro_saw",
	"intro_torch",
	"massn",
	"massnf",
	"miniturret",
	"mini_apache",
	"mini_osprey",
	"mortar",
	"osprey",
	"osprey2",
	"otis",
	"otisf",
	"pit_worm_up",
	"player",
	"recruit",
	"scientist",
	"scientist2",
	"sentry",
	"turret",
	"voltigore",
	"wheelchair_sci",
	"assaultmesa2/biggarg",
	"bmt/barney/barney",
	"bshift/barney",
	"bshift/civ_coat_scientist",
	"bshift/civ_paper_scientist",
	"bshift/civ_scientist",
	"bshift/console_civ_scientist",
	"bshift/dead_barney",
	"bshift/gman",
	"bshift/gordon_scientist",
	"bshift/intro_barney",
	"bshift/intro_otis",
	"bshift/mini_apache",
	"bshift/otis",
	"bshift/scientist",
	"bshift/scientist_cower",
	"bshift/wrangler",
	"deadsimpleneo/babygarg",
	"deadsimpleneo/barney",
	"deadsimpleneo/barneyillusion",
	"deadsimpleneo/fastmedic",
	"deadsimpleneo/sentry",
	"demonprey/voltigore_elite",
	"escape/garg",
	"escape/icky",
	"gmantowers/svenapache",
	"greysnake/hgrunt_opfor",
	"greysnake/massn",
	"hammerhead/civilian",
	"hammerhead/civilian3",
	"hunger/apache2",
	"hunger/civ",
	"hunger/diablo",
	"hunger/franklin2",
	"hunger/huey_apache",
	"hunger/hungerbarney",
	"hunger/hungerotis",
	"hunger/hungerzork",
	"hunger/nurse",
	"hunger/pilot",
	"hunger/scientist",
	"hunger/sheriff",
	"hunger/zombiebull",
	"infiltrate/david",
	"it_has_leaks/wizard",
	"opfor/gman",
	"opfor/scientist",
	"richard_boderman/superrobo",
	"rngstuff/ld_barnabus",
	"sandstone/engineer",
	"sandstone/ofhgrunte",
	"sandstone/ofhgruntf",
	"sandstone/ofmedicf",
	"sandstone/rpggrunte",
	"sandstone/rpggruntf",
	"sc_activist/Barney",
	"sc_activist/garg",
	"sc_activist/hgrunt",
	"sc_activist/hgruntf",
	"sc_activist/swat",
	"sc_mazing/babygarg",
	"sc_psyko/redbabygarg",
	"sc_royals/crazybarney",
	"sc_tetris/megaicky",
	"sc_tetris/panther",
	"sectore/gordon",
	"sectore/panther",
	"sectore/player/cryokeen/cryokeen",
	"spaceviking/cbarney",
	"svencoop2/sc2sci",
	"svencooprpg2/hydra",
	"turr/igotcrabs",
	"valve_hd/barney",
	"valve_hd/gman",
	"vger/demonotis",
};

#define NUM_MODEL_V 124
static const char * MODEL_V[NUM_MODEL_V] =
{
	"v_357",
	"v_9mmar",
	"v_9mmhandgun",
	"v_bgrap",
	"v_bgrap_tonguetip",
	"v_chub",
	"v_crossbow",
	"v_crowbar",
	"v_desert_eagle",
	"v_egon",
	"v_gauss",
	"v_grenade",
	"v_hgun",
	"v_m16a2",
	"v_m40a1",
	"v_medkit",
	"v_minigun",
	"v_pipe_wrench",
	"v_rpg",
	"v_satchel",
	"v_satchel_radio",
	"v_saw",
	"v_shock",
	"v_shotgun",
	"v_spore_launcher",
	"v_squeak",
	"v_tripmine",
	"v_uzi",
	"adamr/weapons/gold357/v_gold357",
	"adamr/weapons/gold357/v_gold357_op4",
	"afrikakorps/weapons/v_357",
	"afrikakorps/weapons/v_9mmhandgun",
	"afrikakorps/weapons/v_amerk",
	"afrikakorps/weapons/v_grenade",
	"afrikakorps/weapons/v_m40a1",
	"afrikakorps/weapons/v_rpg",
	"afrikakorps/weapons/v_tommy",
	"auspices/v_shotgun",
	"bmt/crowbar/v_crowbar",
	"bshift/v_357",
	"bshift/v_9mmar",
	"bshift/v_9mmhandgun",
	"bshift/v_crossbow",
	"bshift/v_crowbar",
	"bshift/v_egon",
	"bshift/v_gauss",
	"bshift/v_grenade",
	"bshift/v_hgun",
	"bshift/v_m16a2",
	"bshift/v_rpg",
	"bshift/v_satchel",
	"bshift/v_satchel_radio",
	"bshift/v_shotgun",
	"bshift/v_squeak",
	"bshift/v_tripmine",
	"escape/v_crowbar",
	"hunger/v_ap9",
	"hunger/v_hunger357",
	"hunger/v_hunger9mmar",
	"hunger/v_hunger9mmhandgun",
	"hunger/v_hungercrossbow",
	"hunger/v_hungercrowbar",
	"hunger/v_hungergrenade",
	"hunger/v_hungersatchel",
	"hunger/v_hungersatchel_radio",
	"hunger/v_hungershotgun",
	"hunger/v_shovel",
	"hunger/v_syringe",
	"hunger/v_taurus",
	"hunger/v_tnt",
	"mmm/v_ar",
	"mmm/v_as",
	"mommamesa/v_shotgun",
	"opfor/v_357",
	"opfor/v_9mmar",
	"opfor/v_9mmhandgun",
	"opfor/v_crossbow",
	"opfor/v_crowbar",
	"opfor/v_desert_eagle",
	"opfor/v_displacer",
	"opfor/v_egon",
	"opfor/v_gauss",
	"opfor/v_grenade",
	"opfor/v_knife",
	"opfor/v_m16a2",
	"opfor/v_m40a1",
	"opfor/v_medkit",
	"opfor/v_minigun",
	"opfor/v_penguin",
	"opfor/v_pipe_wrench",
	"opfor/v_rpg",
	"opfor/v_satchel",
	"opfor/v_satchel_radio",
	"opfor/v_saw",
	"opfor/v_shotgun",
	"opfor/v_spore_launcher",
	"opfor/v_squeak",
	"opfor/v_tripmine",
	"opfor/v_uzi",
	"rngweapons/v_rngmace",
	"rngweapons/kyper_kuutio/v_electrogun",
	"rngweapons/kyper_kuutio/v_exoshotgun",
	"rngweapons/kyper_kuutio/v_gausspistol",
	"rngweapons/kyper_kuutio/v_kyperbow",
	"rngweapons/kyper_kuutio/v_microgun",
	"rngweapons/kyper_kuutio/v_mininuke",
	"rngweapons/kyper_kuutio/v_omegacannon",
	"rngweapons/kyper_kuutio/v_sporegun",
	"spaceviking/v_crowbar",
	"svencooprpg2/v_2hb2",
	"svencooprpg2/v_flintlock",
	"svencooprpg2/v_hb1",
	"svencooprpg2/v_hb2",
	"svencooprpg2/v_hb3",
	"svencooprpg2/v_heal",
	"svencooprpg2/v_longbow",
	"svencooprpg2/v_sarticulus",
	"svencooprpg2/v_spell1",
	"svencooprpg2/v_spell2",
	"svencooprpg2/v_spell3",
	"svencooprpg2/v_sword",
	"svencooprpg2/v_warhammer",
	"svencooprpg2/v_xbow",
	"vger/v_357",
};

#define NUM_MODEL_P 81
static const char * MODEL_P[NUM_MODEL_P] =
{
	"p_2uzis",
	"p_2uzis_gold",
	"p_357",
	"p_9mmAR",
	"p_9mmhandgun",
	"p_bgrap",
	"p_crossbow",
	"p_crowbar",
	"p_desert_eagle",
	"p_egon",
	"p_gauss",
	"p_glock",
	"p_grenade",
	"p_hgun",
	"p_m16",
	"p_m40a1",
	"p_medkit",
	"p_minigunidle",
	"p_minigunspin",
	"p_mp5",
	"p_pipe_wrench",
	"p_rpg",
	"p_rpg_r",
	"p_satchel",
	"p_satchel_radio",
	"p_saw",
	"p_shock",
	"p_shotgun",
	"p_spore_launcher",
	"p_squeak",
	"p_tripmine",
	"p_uzi",
	"p_uzi_gold",
	"adamr/weapons/gold357/p_gold357",
	"afrikakorps/weapons/p_357",
	"afrikakorps/weapons/p_9mmhandgun",
	"afrikakorps/weapons/p_amerk",
	"afrikakorps/weapons/p_grenade",
	"afrikakorps/weapons/p_m40a1",
	"afrikakorps/weapons/P_rpg",
	"afrikakorps/weapons/p_tommy",
	"bmt/crowbar/p_crowbar",
	"escape/p_crowbar",
	"hunger/p_ap9",
	"hunger/p_hunger9mmar",
	"hunger/p_hunger9mmhandgun",
	"hunger/p_hungercrowbar",
	"hunger/p_hungershotgun",
	"hunger/p_shovel",
	"hunger/p_syringe",
	"hunger/p_taurus",
	"hunger/p_tnt",
	"mmm/p_ar",
	"opfor/p_displacer",
	"opfor/p_knife",
	"opfor/p_penguin",
	"rngweapons/p_rngmace",
	"rngweapons/kyper_kuutio/pee/p_electrogun",
	"rngweapons/kyper_kuutio/pee/p_exoshotgun",
	"rngweapons/kyper_kuutio/pee/p_gausspistol",
	"rngweapons/kyper_kuutio/pee/p_kyperbow",
	"rngweapons/kyper_kuutio/pee/p_microgun",
	"rngweapons/kyper_kuutio/pee/p_mininuke",
	"rngweapons/kyper_kuutio/pee/p_mininuke_r",
	"rngweapons/kyper_kuutio/pee/p_omegacannon",
	"rngweapons/kyper_kuutio/pee/p_sporegun",
	"spaceviking/p_crowbar",
	"svencooprpg2/p_2hb2",
	"svencooprpg2/p_flintlock",
	"svencooprpg2/p_hb1",
	"svencooprpg2/p_hb2",
	"svencooprpg2/p_hb3",
	"svencooprpg2/p_heal",
	"svencooprpg2/p_longbow",
	"svencooprpg2/p_sarticulus",
	"svencooprpg2/p_spell2",
	"svencooprpg2/p_spell3",
	"svencooprpg2/p_sword",
	"svencooprpg2/p_warhammer",
	"svencooprpg2/p_xbow",
	"vger/p_357",
};

#define NUM_MODEL_W 107
static const char * MODEL_W[NUM_MODEL_W] =
{
	"w_2uzis",
	"w_2uzis_gold",
	"w_357",
	"w_357ammo",
	"w_357ammobox",
	"w_9mmar",
	"w_9mmarclip",
	"w_9mmclip",
	"w_9mmhandgun",
	"w_adrenaline",
	"w_antidote",
	"w_argrenade",
	"w_battery",
	"w_bgrap",
	"w_chainammo",
	"w_crossbow",
	"w_crossbow_clip",
	"w_crowbar",
	"w_desert_eagle",
	"w_egon",
	"w_fgrenade",
	"w_flare",
	"w_flashlight",
	"w_gauss",
	"w_gaussammo",
	"w_grenade",
	"w_hgun",
	"w_isotopebox",
	"w_longjump",
	"w_m16",
	"w_m40a1",
	"w_m40a1clip",
	"w_medkit",
	"w_minigun",
	"w_mp5",
	"w_oxygen",
	"w_pipe_wrench",
	"w_pmedkit",
	"w_rad",
	"w_rpg",
	"w_rpgammo",
	"w_satchel",
	"w_saw",
	"w_saw_clip",
	"w_security",
	"w_shock",
	"w_shock_rifle",
	"w_shotbox",
	"w_shotgun",
	"w_shotshell",
	"w_silencer",
	"w_spore_launcher",
	"w_sqknest",
	"w_squeak",
	"w_suit",
	"w_svencoop_pickup",
	"w_uzi",
	"w_uzi_gold",
	"w_weaponbox",
	"adamr/weapons/gold357/w_gold357",
	"afrikakorps/weapons/w_357",
	"afrikakorps/weapons/w_9mmhandgun",
	"afrikakorps/weapons/w_amerk",
	"afrikakorps/weapons/w_grenade",
	"afrikakorps/weapons/w_m40a1",
	"afrikakorps/weapons/w_rpg",
	"afrikakorps/weapons/w_tommy",
	"bmt/crowbar/w_crowbar",
	"bmt/eagle/w_357ammobox",
	"bshift/w_flashlight",
	"escape/w_crowbar",
	"hunger/w_ap9",
	"hunger/w_hunger9mmar",
	"hunger/w_hunger9mmarclip",
	"hunger/w_hunger9mmhandgun",
	"hunger/w_hungercrowbar",
	"hunger/w_hungershotgun",
	"hunger/w_shovel",
	"hunger/w_syringebox",
	"hunger/w_taurus",
	"hunger/w_tnt",
	"mommamesa/w_blue_suit",
	"opfor/w_accelerator",
	"opfor/w_backpack",
	"opfor/w_displacer",
	"opfor/w_health",
	"opfor/w_icon",
	"opfor/w_jumppack",
	"opfor/w_knife",
	"opfor/w_penguin",
	"opfor/w_penguinnest",
	"opfor/w_porthev",
	"rngweapons/w_rngmace",
	"rngweapons/kyper_kuutio/wee/w_electrogun",
	"rngweapons/kyper_kuutio/wee/w_exoshotgun",
	"rngweapons/kyper_kuutio/wee/w_gausspistol",
	"rngweapons/kyper_kuutio/wee/w_kyperbow",
	"rngweapons/kyper_kuutio/wee/w_microgun",
	"rngweapons/kyper_kuutio/wee/w_mininuke",
	"rngweapons/kyper_kuutio/wee/w_mininuke_ammo",
	"rngweapons/kyper_kuutio/wee/w_omegacannon",
	"rngweapons/kyper_kuutio/wee/w_sporegun",
	"spaceviking/w_crowbar",
	"svencooprpg2/w_sword",
	"svencooprpg2/w_warhammer",
	"vger/w_357",
	"vhe-models/w_tripmine_hammer",
};

#define NUM_MODEL_PROPS 239
static const char * MODEL_PROPS[NUM_MODEL_PROPS] =
{
	"abone_template1",
	"agibs",
	"apache",
	"apache2",
	"apachef",
	"base",
	"big_momgibs",
	"big_rock",
	"bleachbones",
	"blkop_apache",
	"blkop_bodygibs",
	"blkop_enginegibs",
	"blkop_tailgibs",
	"bm_leg",
	"bm_sack",
	"bm_shell",
	"boid",
	"bonegibs",
	"bookgibs",
	"bskull_template1",
	"cactusgibs",
	"can",
	"catwalkgibs",
	"ceilinggibs",
	"chromegibs",
	"cindergibs",
	"cindergibs_green",
	"cindergibs_yellow",
	"civ_stand",
	"computergibs",
	"concretegibs",
	"concrete_gibs",
	"cretegibs",
	"crossbow_bolt",
	"crystal",
	"egg",
	"fleshgibs",
	"floater",
	"fungus(large)",
	"fungus(small)",
	"fungus",
	"garbagegibs",
	"gib_b_bone",
	"gib_b_gib",
	"gib_hgrunt",
	"gib_legbone",
	"gib_lung",
	"gib_skull",
	"glassgibs",
	"grenade",
	"hgibs",
	"hornet",
	"hvr",
	"in_teleport",
	"islave_gibs1",
	"kosobar",
	"mbarrel",
	"mechgibs",
	"med_crategibs",
	"metalgibs",
	"metalplategibs",
	"metalplategibs_dark",
	"metalplategibs_green",
	"mil_crate",
	"mil_crategibs",
	"mil_stand",
	"mini_apache",
	"mini_osprey_bodygibs",
	"mini_osprey_enginegibs",
	"mini_osprey_tailgibs",
	"mortarshell",
	"not_precached",
	"nuke_button",
	"nuke_case",
	"nuke_timer",
	"office_gibs",
	"osprey_bodygibs",
	"osprey_enginegibs",
	"osprey_tailgibs",
	"out_teleport",
	"pelvis",
	"pit_drone_gibs",
	"pit_drone_spike",
	"pit_worm_gibs",
	"plant_01",
	"plant_cactus",
	"plant_palm",
	"qmeal",
	"recruit_push",
	"ribcage",
	"riblet1",
	"rip",
	"rockgibs",
	"rpgrocket",
	"sandbags",
	"saw_link",
	"saw_shell",
	"shell",
	"shock_effect",
	"shotgunshell",
	"shrapnel",
	"snapbug",
	"snow_tree",
	"spit",
	"spore",
	"stealth",
	"stickygib",
	"stickygibpink",
	"strooper_gibs",
	"suit2",
	"tech_crategibs",
	"testsphere",
	"tool_box",
	"tree_palm",
	"uplant1",
	"uplant3",
	"ventgibs",
	"vgibs",
	"webgibs",
	"wetfloorsign_gibs",
	"woodgibs",
	"xen_rockgibs",
	"xen_rockgib_big",
	"xen_rockgib_med",
	"xen_rockgib_small",
	"zombiegibs1",
	"adamr/backpack_sven",
	"adamr/ceiling_lamp",
	"adamr/hevcharge_wall",
	"adamr/key",
	"adamr/keycard_l1",
	"adamr/keycard_l2",
	"adamr/keycard_l3",
	"adamr/medkit_wall",
	"adamr/vase",
	"afrikakorps/misc/gascan",
	"afrikakorps/misc/hk_neuvillecar",
	"afrikakorps/misc/hk_p4j_yellowhull",
	"afrikakorps/misc/hk_panzer4j_greyfull",
	"afrikakorps/misc/hk_sandbags",
	"afrikakorps/misc/hk_sherman_damaged2",
	"afrikakorps/misc/oiltank",
	"afrikakorps/misc/palm1A",
	"afrikakorps/misc/palm1B",
	"afrikakorps/misc/palm2",
	"bshift/mini_apache",
	"bshift/qmeal",
	"bshift/xen_rockgibs",
	"bshift/xen_rockgib_big",
	"bshift/xen_rockgib_med",
	"bshift/xen_rockgib_small",
	"crystal/plant1",
	"crystal/plant2",
	"crystal/plant3",
	"ginsmodels/gins_clock",
	"ginsmodels/gins_substation",
	"ginsmodels/gins_trashcan",
	"ginsmodels/lag_radio",
	"gmantowers/svenapache",
	"hammerhead/arc_fern",
	"hammerhead/big_rock",
	"hammerhead/shell",
	"hammerhead/tree1",
	"hammerhead/xen_tree5",
	"hammerhead/furniture/pissoir",
	"hammerhead/furniture/tub",
	"hammerhead/furniture/washbowl",
	"hunger/apache2",
	"hunger/cwhead",
	"hunger/einarhead",
	"hunger/huey_apache",
	"hunger/magnushead",
	"hunger/neilhead",
	"hunger/paulhead",
	"hunger/vegitation/arc_bush",
	"hunger/vegitation/arc_fern",
	"hunger/vegitation/arc_flower",
	"hunger/vegitation/arc_xer_tree1",
	"hunger/vegitation/arc_xer_tree2",
	"hunger/vegitation/bush1",
	"hunger/vegitation/bush2",
	"hunger/vegitation/fern1",
	"hunger/vegitation/fern2",
	"hunger/vegitation/shrub1",
	"hunger/vegitation/tree1",
	"hunger/vegitation/tree2",
	"hunger/vegitation/uplant1",
	"hunger/vegitation/uplant1_2x",
	"hunger/vegitation/uplant3",
	"hunger/vegitation/zalec_tree1",
	"infiltrate/bags",
	"mommamesa/mantaray",
	"puchi/spportal/ginsengavenger/elecswitch",
	"puchi/spportal/ginsengavenger/gins_backwall",
	"puchi/spportal/ginsengavenger/gins_bucket",
	"puchi/spportal/ginsengavenger/gins_butts",
	"puchi/spportal/ginsengavenger/gins_cactus",
	"puchi/spportal/ginsengavenger/gins_crateccals",
	"puchi/spportal/ginsengavenger/gins_creteytrashcan",
	"puchi/spportal/ginsengavenger/gins_deskchair",
	"puchi/spportal/ginsengavenger/gins_flatscrn",
	"puchi/spportal/ginsengavenger/gins_leafy",
	"puchi/spportal/ginsengavenger/gins_lunch_sack",
	"puchi/spportal/ginsengavenger/gins_mthrinlawstong",
	"puchi/spportal/ginsengavenger/gins_papersmags",
	"puchi/spportal/ginsengavenger/gins_shelves1",
	"puchi/spportal/ginsengavenger/gins_sink",
	"puchi/spportal/ginsengavenger/gins_toilet",
	"puchi/spportal/ginsengavenger/gins_transformer",
	"puchi/spportal/ginsengavenger/gins_trashcan",
	"puchi/spportal/ginsengavenger/gins_urinal",
	"revil/Furnature/sink",
	"revil/Furnature/toilet",
	"richard_boderman/bulb",
	"richard_boderman/palm1",
	"richard_boderman/palm2",
	"richard_boderman/palm3",
	"richard_boderman/plant",
	"richard_boderman/rockgibs",
	"rngstuff/ae_pipesystem",
	"rngstuff/ae_pumper",
	"rngweapons/kyper_kuutio/wee/mininuke_rocket",
	"rngweapons/kyper_kuutio/wee/sporewhite",
	"sandstone/armor",
	"sandstone/backpack",
	"sandstone/dome",
	"sandstone/dome2",
	"sandstone/fern2",
	"sandstone/stall",
	"sc_psyko/arc_fern",
	"sc_robination/dead_tree02",
	"sc_robination/dead_tree08",
	"sc_royals/arc_fern",
	"sc_royals/palm1a",
	"sc_royals/palm1b",
	"sc_royals/palm2",
	"sc_royals/rockgibsgold",
	"toonrun/spleen",
	"vhe-models/trigger_camera",
};

#define NUM_MODEL_MONSTERS 360
static const char * MODEL_MONSTERS[NUM_MODEL_MONSTERS] =
{
	"aflock",
	"agrunt",
	"agruntf",
	"archer",
	"babygarg",
	"babygargf",
	"baby_headcrab",
	"baby_strooper",
	"baby_voltigore",
	"ball",
	"barnabus",
	"barnacle",
	"barney",
	"barney_helmet",
	"barney_vest",
	"base_flag",
	"bgman",
	"bigrat",
	"big_mom",
	"blkop_dead_osprey",
	"blkop_osprey",
	"breather",
	"bullsquid",
	"chair",
	"chubby",
	"chumtoad",
	"cleansuit_scientist",
	"construction",
	"controller",
	"crashed_osprey",
	"deadhaz",
	"dead_osprey",
	"doctor",
	"drill",
	"filecabinet",
	"flag",
	"forklift",
	"friendly",
	"garg",
	"gasbag",
	"geneworm",
	"gman",
	"gonome",
	"gordon",
	"hair",
	"hassassin",
	"hassassinf",
	"hassault",
	"headcrab",
	"hgrunt",
	"hgruntf",
	"hgrunt_medic",
	"hgrunt_medicf",
	"hgrunt_opfor",
	"hgrunt_opforf",
	"hgrunt_sniper",
	"hgrunt_torch",
	"hgrunt_torchf",
	"highdef_otis",
	"holo",
	"houndeye",
	"houndeye_dead",
	"hwgrunt",
	"hwgruntf",
	"icky",
	"intro_commander",
	"intro_medic",
	"intro_regular",
	"intro_saw",
	"intro_torch",
	"islave",
	"islavef",
	"kingheadcrab",
	"kingpin",
	"leech",
	"light",
	"loader",
	"massn",
	"massnf",
	"miniturret",
	"mini_osprey",
	"mortar",
	"nihilanth",
	"osprey",
	"osprey2",
	"otis",
	"otisf",
	"pipe_bubbles",
	"pit_drone",
	"pit_worm_up",
	"player",
	"protozoa",
	"recruit",
	"rengine",
	"rgrunt",
	"rgruntf",
	"roach",
	"santashelper",
	"sat_globe",
	"scientist",
	"scientist2",
	"scigun",
	"scp_9mmar",
	"scp_crossbow",
	"scp_m40a1",
	"sentry",
	"skeleton",
	"sphere",
	"spiggot",
	"spore_ammo",
	"sprite",
	"stick",
	"stretcher",
	"strooper",
	"stukabat",
	"teleporter_blue_sprites",
	"teleporter_orange_rings",
	"tentacle2",
	"Tor",
	"Torf",
	"tree",
	"turret",
	"uplant2",
	"voltigore",
	"wheelchair",
	"wheelchair_sci",
	"zombie",
	"zombie_barney",
	"zombie_soldier",
	"adamr/securitycam",
	"afrikakorps/enemy/hgrunt",
	"afrikakorps/enemy/massn",
	"assaultmesa2/biggarg",
	"bmt/aqua/bullsquid",
	"bmt/barney/barney",
	"bmt/crowbar/df",
	"bmt/icky/icky",
	"bmt/zombie/slime",
	"bshift/ball",
	"bshift/barney",
	"bshift/barney_helmet",
	"bshift/barney_vest",
	"bshift/base_flag",
	"bshift/ba_holo",
	"bshift/chair",
	"bshift/chumtoad",
	"bshift/civ_coat_scientist",
	"bshift/civ_paper_scientist",
	"bshift/civ_scientist",
	"bshift/console_civ_scientist",
	"bshift/dead_barney",
	"bshift/gman",
	"bshift/gordon_scientist",
	"bshift/gordon_suit",
	"bshift/gun_mag",
	"bshift/hgrunt",
	"bshift/holo",
	"bshift/houndeye",
	"bshift/intro_barney",
	"bshift/intro_otis",
	"bshift/otis",
	"bshift/outro_crowbar",
	"bshift/scientist",
	"bshift/scientist_cower",
	"bshift/tool_box_sm",
	"bshift/wrangler",
	"bshift/zombie",
	"danger_of_collapse/superhoundeye",
	"deadsimpleneo/babygarg",
	"deadsimpleneo/barney",
	"deadsimpleneo/barneyillusion",
	"deadsimpleneo/fastmedic",
	"deadsimpleneo/sentry",
	"deadsimpleneo/Tor",
	"deadsimpleneo/Tor_melee",
	"demonprey/voltigore_elite",
	"escape/garg",
	"escape/icky",
	"escape_series/fast_gonome/gonome",
	"escape_series/fast_zombie/zombie",
	"gmantowers/piranha",
	"greysnake/hgrunt_opfor",
	"greysnake/massn",
	"hammerhead/chubby",
	"hammerhead/civilian",
	"hammerhead/civilian3",
	"hammerhead/furniture/toilette",
	"hunger/babykelly",
	"hunger/bullsquid",
	"hunger/chicken",
	"hunger/civ",
	"hunger/diablo",
	"hunger/franklin2",
	"hunger/heart",
	"hunger/hgrunt",
	"hunger/hungerbarney",
	"hunger/hungercrab",
	"hunger/hungergonome",
	"hunger/hungerhound",
	"hunger/hungerotis",
	"hunger/hungerslave",
	"hunger/hungerzombie",
	"hunger/hungerzork",
	"hunger/lpzombie",
	"hunger/megasquid",
	"hunger/nurse",
	"hunger/nursezombie",
	"hunger/pilot",
	"hunger/scientist",
	"hunger/sheriff",
	"hunger/thehand",
	"hunger/zgrunt",
	"hunger/zombie",
	"hunger/zombie2",
	"hunger/zombie3",
	"hunger/zombiebull",
	"hunger/zombierat",
	"hunger/vegitation/ouitz_tree1",
	"infiltrate/david",
	"it_has_leaks/rotfish",
	"it_has_leaks/wizard",
	"jumpers/bigroach",
	"mmm/bullgiant",
	"mmm/controllergiant",
	"mmm/crabgiant",
	"mmm/giant",
	"mmm/houndgiant",
	"mmm/slavegiant",
	"mommamesa/bman_2",
	"mommamesa/giantroach",
	"mommamesa/hevgrunt",
	"mommamesa/prisoner",
	"nih/chubby",
	"opfor/gman",
	"opfor/islave",
	"opfor/scientist",
	"opfor/stretcher",
	"puchi/mountainbase/dead_headcrab",
	"puchi/spportal/chubby",
	"puchi/spportal/construction",
	"puchi/spportal/piranha",
	"richard_boderman/boderman",
	"richard_boderman/Hassassin",
	"richard_boderman/superrobo",
	"rngstuff/kyper_assistant",
	"rngstuff/ld_barnabus",
	"rngstuff/mr_zombie3",
	"sandstone/engineer",
	"sandstone/hwgrunte",
	"sandstone/massne",
	"sandstone/ofhgrunte",
	"sandstone/ofhgruntf",
	"sandstone/ofmedicf",
	"sandstone/rpggrunte",
	"sandstone/rpggruntf",
	"sc_activist/Barney",
	"sc_activist/bullsquid",
	"sc_activist/chumcrab",
	"sc_activist/contrller",
	"sc_activist/garg",
	"sc_activist/hgrunt",
	"sc_activist/hgruntf",
	"sc_activist/houndeye",
	"sc_activist/icky",
	"sc_activist/robohwgrunt",
	"sc_activist/swat",
	"sc_mazing/babygarg",
	"sc_mazing/blob",
	"sc_mazing/pit_drone",
	"sc_mazing/scarab",
	"sc_mazing/zombie",
	"sc_psyko/blueslime",
	"sc_psyko/greenhoundeye",
	"sc_psyko/lightgreenslime",
	"sc_psyko/orangeslime",
	"sc_psyko/pigdrone",
	"sc_psyko/pinkslime",
	"sc_psyko/redbabygarg",
	"sc_psyko/redslime",
	"sc_psyko/tormaster",
	"sc_psyko/turquoiseslave",
	"sc_psyko/turquoisetrooper",
	"sc_psyko/yellowcontroller",
	"sc_psyko/yellowslave",
	"sc_psyko/yellowslime",
	"sc_royals/anubis",
	"sc_royals/crazybarney",
	"sc_royals/headcrabp",
	"sc_royals/houndeyep",
	"sc_royals/islavep",
	"sc_royals/mummy",
	"sc_royals/pharaoh",
	"sc_royals/Torp",
	"sc_tetris/big_big_mom",
	"sc_tetris/blocker",
	"sc_tetris/block_fassn",
	"sc_tetris/block_guard",
	"sc_tetris/block_hwgrunt",
	"sc_tetris/block_leech",
	"sc_tetris/block_massn",
	"sc_tetris/block_ninja",
	"sc_tetris/controllerboss",
	"sc_tetris/controllerguardian",
	"sc_tetris/megaicky",
	"sc_tetris/mushroom_red_large",
	"sc_tetris/mushroom_red_small",
	"sc_tetris/panther",
	"sc_tetris/rgrunt",
	"sc_tetris/shockboss",
	"sectore/cryogrunt",
	"sectore/darkdrone3",
	"sectore/freeman",
	"sectore/glassdrone",
	"sectore/gordon",
	"sectore/panther",
	"sectore/player/cryokeen/cryokeen",
	"sp/gonome2",
	"sp/green_headc",
	"spaceviking/cbarney",
	"spaceviking/robobug2",
	"spaceviking/spirate",
	"svencoop2/angelchubb",
	"svencoop2/sc2grunt",
	"svencoop2/sc2sci",
	"svencooprpg2/bogcreature",
	"svencooprpg2/bogplant",
	"svencooprpg2/cattail",
	"svencooprpg2/dragon",
	"svencooprpg2/dwarf",
	"svencooprpg2/ghost1",
	"svencooprpg2/grylion",
	"svencooprpg2/heretic",
	"svencooprpg2/hydra",
	"svencooprpg2/mage",
	"svencooprpg2/monster",
	"svencooprpg2/mushroom",
	"svencooprpg2/mushroom2",
	"svencooprpg2/priest",
	"svencooprpg2/rat",
	"svencooprpg2/sheep",
	"svencooprpg2/zombie",
	"toonrun/chubbina",
	"toonrun/hgrunt",
	"turr/betty",
	"turr/igotcrabs",
	"turretfortress/friendly",
	"turretfortress/houndeye",
	"turretfortress/islave",
	"turretfortress/kfc",
	"turretfortress/kingpin",
	"turretfortress/zombie",
	"valve_hd/barney",
	"valve_hd/gman",
	"valve_hd/zombie",
	"vger/demonotis",
	"vger/gvrobo",
	"vger/pvrobo",
	"vger/rvrobo",
	"vger/vgergrunt",
	"vhe-models/spore_ammo_hammer",
};

#define NUM_MODEL_PLAYERS 60
static const char * MODEL_PLAYERS[NUM_MODEL_PLAYERS] =
{
	"player/aswat/aswat",
	"player/barney/barney",
	"player/barniel/barniel",
	"player/beret/beret",
	"player/betagordon/betagordon",
	"player/boris/boris",
	"player/cannibal/cannibal",
	"player/cl_suit/cl_suit",
	"player/colette/colette",
	"player/dm_barney/dm_barney",
	"player/dm_gina/dm_gina",
	"player/dm_gordon/dm_gordon",
	"player/dm_helmet/dm_helmet",
	"player/etac/etac",
	"player/fassn/fassn",
	"player/gina/gina",
	"player/gman/gman",
	"player/gordon/gordon",
	"player/grunt/grunt",
	"player/helmet/helmet",
	"player/hevbarney/hevbarney",
	"player/hevbarney2/hevbarney2",
	"player/hevscientist/hevscientist",
	"player/hevscientist2/hevscientist2",
	"player/hevscientist3/hevscientist3",
	"player/hevscientist4/hevscientist4",
	"player/hevscientist5/hevscientist5",
	"player/hgrunt/Hgrunt",
	"player/junctionpunch/junctionpunch",
	"player/kate/kate",
	"player/madscientist/madscientist",
	"player/mandalorian/mandalorian",
	"player/massn/massn",
	"player/obi09/obi09",
	"player/otis/otis",
	"player/punisher/punisher",
	"player/recon/recon",
	"player/rgrunt/rgrunt",
	"player/robo/robo",
	"player/scientist/scientist",
	"player/selene/selene",
	"player/shephard/shephard",
	"player/sieni/sieni",
	"player/th_cl_suit/th_cl_suit",
	"player/th_dave/th_dave",
	"player/th_einar/th_einar",
	"player/th_gangster/th_gangster",
	"player/th_jack/th_jack",
	"player/th_neil/th_neil",
	"player/th_nurse/th_nurse",
	"player/th_nypdcop/th_nypdcop",
	"player/th_orderly/th_orderly",
	"player/th_patient/th_patient",
	"player/th_paul/th_paul",
	"player/th_worker/th_worker",
	"player/tower/tower",
	"player/trinityrage/trinityrage",
	"player/unm_barney/unm_barney",
	"player/uscm/uscm",
	"player/zombie/zombie",
};