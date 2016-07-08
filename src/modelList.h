#pragma once
#include <string>
#include <set>
#include <unordered_map>
#include <fstream>
#include "Util.h"
#include "default_model_list.h"

typedef std::unordered_map< string, vector<string> > list_hashmap;
typedef std::unordered_map< string, string > string_hashmap;
typedef std::unordered_map< string, int > height_hashmap;

extern string_hashmap default_monster_models;
extern string_hashmap default_friendly_monster_models;
extern string_hashmap default_weapon_models;
extern string_hashmap default_w_weapon_models;
extern string_hashmap default_p_weapon_models;
extern string_hashmap default_v_weapon_models;
extern string_hashmap default_ammo_models;
extern string_hashmap default_item_models;
extern string_hashmap default_xen_models;
extern vector<string> default_gib_models;

extern vector<string> gmr_replace_only; // models only replacable through GMR

extern list_hashmap monster_whitelists; // set of models that are tested to be working for the monster
extern list_hashmap monster_blacklists; // set of models that would crash the game if they were used as replacements


extern vector<string> monster_sprites;
extern vector<string> dont_replace; // shouldn't be replaced
extern vector<string> weapon_types;
extern height_hashmap default_monster_heights;

static void init_default_monster_heights()
{
	default_monster_heights["monster_headcrab"] = 24;
	default_monster_heights["monster_chumtoad"] = 12;
	default_monster_heights["monster_houndeye"] = 36;
	default_monster_heights["monster_bullchicken"] = 64;
	default_monster_heights["monster_alien_grunt"] = 64;
	default_monster_heights["monster_ichthyosaur"] = 64;
	default_monster_heights["monster_tentacle"] = 64;
	default_monster_heights["monster_stukabat"] = 24;

	default_monster_heights["monster_nihilanth"] = 384;
	default_monster_heights["monster_gargantua"] = 128; // ?
	default_monster_heights["monster_babygarg"] = 128; // ?
	default_monster_heights["monster_alien_voltigore"] = 90;
	default_monster_heights["monster_bigmomma"] = 190;
}

static void parse_model_lists()
{
	ifstream myfile("gsrand_models.txt");
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
						int ampersand = name.find("&");
						if (ampersand != string::npos)
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
	default_xen_models.clear();

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
	default_weapon_models["displacer"]	   = "displacer";

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

	// TODO: Actually find the default models. None of these are right. Well maybe they are idk.
	default_item_models["airtank"] = "airtank";
	default_item_models["antidote"] = "antidote";
	default_item_models["armorvest"] = "armorvest";
	default_item_models["battery"] = "w_battery";
	default_item_models["healthcharger"] = "healthcharger";
	default_item_models["healthkit"] = "w_medkit"; // or is it w_pmedkit?
	default_item_models["helmet"] = "helmet";
	default_item_models["longjump"] = "w_longjump";
	default_item_models["recharge"] = "recharge";
	default_item_models["security"] = "security";
	default_item_models["suit"] = "w_suit";

	default_xen_models["hair"] = "hair";
	default_xen_models["plantlight"] = "light";
	default_xen_models["spore_large"] = "fungus(large)";
	default_xen_models["spore_medium"] = "fungus";
	default_xen_models["spore_small"] = "fungus(small)";
	default_xen_models["tree"] = "tree";

	for (string_hashmap::iterator it = default_weapon_models.begin(); it != default_weapon_models.end(); ++it)
		weapon_types.push_back(it->first);

	init_default_monster_heights();
}

// weapons are precached too but we handle those differently
#define NUM_DEFAULT_PRECACHE 44
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
	"sprites/exit1.spr",
	"sprites/disp_ring.spr",
};
