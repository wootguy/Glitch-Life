#include "StdAfx.h"
#include "model_util.h"
#include "ent_util.h"
#include "gsrand.h"
#include <set>

//
// Also kind of sprite util file
//

// request a completely random model (MODEL_TYPE_GENERIC), or a specific model type
string get_random_model(int request_model_type)
{
	int total_wep_models = NUM_MODEL_W + NUM_MODEL_V + NUM_MODEL_P;
	int w_start = 0;
	int v_start = w_start + NUM_MODEL_W;
	int p_start = v_start + NUM_MODEL_V;
	int r;

	if (request_model_type == MODEL_TYPE_GENERIC)
	{
		r = rand() % 3;
		if (r == 0) // weapon
		{
			r = rand() % total_wep_models;
			if (r >= p_start)
				return MODEL_P[r - p_start];
			else if (r >= v_start)
				return MODEL_V[r - v_start];
			else if (r >= w_start)
				return MODEL_W[r - w_start];
		}
		else if (r == 1)
			return MODEL_PROPS[rand() % NUM_MODEL_PROPS];
		else
		{
			r = rand() % 4;
			if (r) return MODEL_MONSTERS[rand() % NUM_MODEL_MONSTERS];
			return MODEL_PLAYERS[rand() % NUM_MODEL_PLAYERS]; 
		}
	}
	else if (request_model_type == MODEL_TYPE_PROP)
		return MODEL_PROPS[rand() % NUM_MODEL_PROPS];
	else if (request_model_type == MODEL_TYPE_MONSTER)
	{
		r = rand() % 4;
		if (r) return MODEL_MONSTERS[rand() % NUM_MODEL_MONSTERS];
		return MODEL_PLAYERS[rand() % NUM_MODEL_PLAYERS]; 
	}
	else if (request_model_type == MODEL_TYPE_P_WEAPON)
		return MODEL_P[rand() % NUM_MODEL_P];
	else if (request_model_type == MODEL_TYPE_W_WEAPON)
		return MODEL_W[rand() % NUM_MODEL_W];
	else if (request_model_type == MODEL_TYPE_V_WEAPON)
		return MODEL_V[rand() % NUM_MODEL_V];
	else
		print("Invalid model request: " + request_model_type);

	print("FAILED TO GET RANDOM MODEL: " + str(r));
	return "";
}

string get_random_sprite(int request_model_type)
{
	int total_sprites = NUM_STATIC_SPRITES + NUM_ANIMATED_SPRITES;
	int anim_start = 0;
	int static_start = NUM_ANIMATED_SPRITES;
	int r;

	if (request_model_type == SPRITE_TYPE_GENERIC)
	{
		r = rand() % total_sprites;
		if (r >= static_start)
			return STATIC_SPRITES[r - static_start];
		return ANIMATED_SPRITES[r];
	}
	else if (request_model_type == SPRITE_TYPE_ANIMATED)
		return ANIMATED_SPRITES[rand() % NUM_ANIMATED_SPRITES];
	else if (request_model_type == SPRITE_TYPE_STATIC)
		return STATIC_SPRITES[rand() % NUM_STATIC_SPRITES];
	else
		print("Invalid sprite request: " + request_model_type);

	print("FAILED TO GET RANDOM SPRITE: " + str(request_model_type));
	return "";
}

void init_random_monster_models()
{
	for (string_hashmap::iterator it = default_monster_models.begin(); it != default_monster_models.end(); ++it)
	{
		int r = rand() % NUM_MODEL_MONSTERS;
		if (mdlMode == MDL_TYPED)
			random_monster_models[it->first] = MODEL_MONSTERS[r];
		else
			random_monster_models[it->first] = get_random_model(MODEL_TYPE_GENERIC);
	}
}

int getModelType(string entity_name)
{
	if (entity_name.find("monster_furniture") == 0)
		return MODEL_TYPE_GENERIC;
	if (entity_name.find("weapon_") == 0)
		return MODEL_TYPE_WEAPON;
	if (entity_name.find("monster_") == 0)
		return MODEL_TYPE_MONSTER;
	return MODEL_TYPE_GENERIC;
}

bool parse_gmr_line(string line, string& a, string& b)
{
	// parse source
	int quote = line.find_first_of("\"");
	if (quote == string::npos)	return false;
	line = getSubStr(line, quote+1);
	quote = line.find_first_of("\"");
	if (quote == string::npos)	return false;
	a = getSubStr(line, 0, quote);
	line = getSubStr(line, quote+1);

	// parse replacement
	quote = line.find_first_of("\"");
	if (quote == string::npos)	return false;
	line = getSubStr(line, quote+1);
	quote = line.find_first_of("\"");
	if (quote == string::npos)	return false;
	b = getSubStr(line, 0, quote);

	return true;
}

// "models" actually means models+sprites+brush entities
int count_map_models(BSP * map, Entity** ents, string path, int& total_models, int& potential_additions,
					string& gmr_path, string_hashmap& ent_models)
{
	total_models = map->header.lump[LUMP_MODELS].nLength / BSP_MODEL_BYTES;
	total_models += 1; // BSP map model
	//println("BSP Models: " + str(map_models));

	total_models += default_gib_models.size(); // assume all gibs are precached

	for (uint i = 0; i < default_precache_models.size(); ++i)
		ent_models[default_precache_models[i]] = "default_precache_models";

	potential_additions = 0;
	int potential_door_gibs = 0;
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (ents[i] == NULL)
			break;

		string cname = toLowerCase(ents[i]->keyvalues["classname"]);


		if (cname.find("monstermaker") != string::npos || cname.find("env_xenmaker") != string::npos)
			cname = toLowerCase(ents[i]->keyvalues["monstertype"]);

		string custom_monster_model_key = "model";
		string ally_key = "is_player_ally";
		if (cname.find("squadmaker") != string::npos)
		{
			cname = toLowerCase(ents[i]->keyvalues["monstertype"]);
			custom_monster_model_key = "new_model";
			ally_key = "respawn_as_playerally";
		}

		if (cname.find("env_shooter") != string::npos)
		{
			if (ents[i]->keyvalues.find("shootmodel") != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues["shootmodel"])] = cname;
			potential_additions++;
		}

		if (matchStr(cname, "env_beam"))
		{
			if (ents[i]->keyvalues.find("texture") != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues["texture"])] = cname;
			potential_additions++;
		}
		if (matchStr(cname, "env_funnel"))
		{
			if (ents[i]->keyvalues.find("sprite") != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues["sprite"])] = cname;
			potential_additions++;
		}
		if (matchStr(cname, "env_laser"))
		{
			if (ents[i]->keyvalues.find("texture") != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues["texture"])] = cname;
			if (ents[i]->keyvalues.find("EndSprite") != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues["EndSprite"])] = cname;
			potential_additions+=2;
		}
		if (matchStr(cname, "item_recharge") || matchStr(cname, "item_healthcharger"))
		{
			if (ents[i]->keyvalues.find("model_juice") != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues["model_juice"])] = cname;
			potential_additions++;
			continue;
		}
			
		if (matchStr(cname, "func_breakable"))
			potential_additions++;

		// decided not to replace door gib models because you don't usually break them
		// and levels tend to have a lot of doors

		if (cname.find("monster_") == 0 || cname.find("ammo_") != string::npos 
			|| cname.find("item_") != string::npos || matchStr(cname, "cycler") || matchStr(cname, "cycler_weapon")
			|| matchStr(cname, "cycler_sprite") || matchStr(cname, "env_beverage") || matchStr(cname, "env_glow")
			|| matchStr(cname, "env_sprite") || matchStr(cname, "env_spritetrain") || matchStr(cname, "weaponbox"))
		{
			if (!matchStr(cname, "monster_cockroach") && !matchStr(cname, "monster_kingpin") && 
				!matchStr(cname, "monster_tentacle"))
				potential_additions++;
			
			string_hashmap defaults_hashmap = default_monster_models;
			bool is_friendly = ents[i]->keyvalues[ally_key] == "1";
			if (cname.find("item_") != string::npos)
				defaults_hashmap = default_item_models;
			if (cname.find("monster_") == 0 && is_friendly)
				defaults_hashmap = default_friendly_monster_models;

			if (ents[i]->keyvalues.find(custom_monster_model_key) != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues[custom_monster_model_key])] = cname;
			else if (cname.find("monster_") != string::npos || cname.find("item_") != string::npos)
			{
				string m = defaults_hashmap[getSubStr(cname, cname.find_first_of('_')+1)]; 
				if (m.length() > 0)
					ent_models[toLowerCase("models/" + m + ".mdl")] = cname;
				else if (is_friendly)
				{
					m = default_monster_models[getSubStr(cname, cname.find_first_of('_')+1)]; 
					if (m.length() > 0)
						ent_models[toLowerCase("models/" + m + ".mdl")] = cname;	
					else
						println("NO DEFAULT MODEL (ENEMY/ALLY) FOUND FOR: " + cname);
				}
				else
					println("NO DEFAULT MODEL FOUND FOR: " + cname);
			}
		}
		else if (cname.find("weapon_") != string::npos)
		{
			if (ents[i]->keyvalues.find("wpn_v_model") != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues["wpn_v_model"])] = cname;

			if (ents[i]->keyvalues.find("wpn_w_model") != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues["wpn_w_model"])] = cname;

			if (ents[i]->keyvalues.find("wpn_p_model") != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues["wpn_p_model"])] = cname;
		}
	}

	int count_before = total_models + ent_models.size();
	string gmr;
	// check for model replacement file
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (ents[i] == NULL)
			break;
		if (matchStr(ents[i]->keyvalues["classname"], "worldspawn"))
		{
			gmr = ents[i]->keyvalues["globalmodellist"];
			break;
		}
	}
	// cfg overrides map setting
	ifstream myfile (path + map->name + ".cfg");
	
	if (myfile.is_open())
	{
		while ( !myfile.eof() )
		{
			string line;
			getline (myfile,line);
			if (line.find("globalmodellist") != string::npos)
			{
				gmr = getSubStr(line, line.find("globalmodellist")+15);
				gmr = getSubStr(gmr, gmr.find_first_not_of(' '));
				int last_space = gmr.find_first_of(' ');
				if (last_space != string::npos)
					gmr = getSubStr(gmr, 0, last_space);
				break;
			}
		}
	}
	myfile.close();
	

	if (gmr.length() > 0)
	{
		string gmrpath = getSubStr(path, 0, path.find_last_of("\\/")); // get rid of "\maps\"
		gmrpath = getSubStr(gmrpath, 0, gmrpath.find_last_of("\\/"));
		gmrpath = relative_path_to_absolute(gmrpath + "/models/" + map->name, gmr);
		if (gmrpath[0] == '"')
			gmrpath = getSubStr(gmrpath, 1);
		if (gmrpath[gmrpath.size()-1] == '"')
			gmrpath = getSubStr(gmrpath, 0, gmrpath.size()-1);
		gmr_path = gmrpath;
	}

	potential_additions += default_weapon_models.size(); // w_ model replacements

	//println("Entity Models: " + str(ent_models.size()));
	total_models += ent_models.size();
	//println("POTENTIAL MODELS: " + str(map_models) + " + " + str(potential_additions) + " = " + str(map_models + potential_additions));

	return total_models;
}

string random_model_replace(string model)
{
	string replacement;
	if (model.find("sprites/") != string::npos)
		replacement = "sprites/" + get_random_sprite(SPRITE_TYPE_GENERIC) + ".spr";
	else
	{
		if (mdlMode == MDL_TYPED)
		{
			if (model.find("v_") != string::npos)
				replacement ="models/" +  get_random_model(MODEL_TYPE_V_WEAPON) + ".mdl";
			else if (model.find("p_") != string::npos)
				replacement = "models/" + get_random_model(MODEL_TYPE_P_WEAPON) + ".mdl"; 
			else if (model.find("w_") != string::npos)
				replacement = "models/" + get_random_model(MODEL_TYPE_W_WEAPON) + ".mdl"; 
			else
				replacement = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
		}
		else
		{
			replacement = get_random_model(MODEL_TYPE_GENERIC);
			if (matchStr(model, "models/w_satchel.mdl"))
				while (find(satchel_blacklist.begin(), satchel_blacklist.end(), replacement) != satchel_blacklist.end())
					replacement = get_random_model(MODEL_TYPE_GENERIC);	
			replacement = "models/" + replacement + ".mdl";	
		}
	}
	
	return replacement;
}

string get_random_replacement(string model, vector<string>& replaced, vector<string>& replace_models)
{
	string replacement;

	// ignoring special cases (nih is processed as ent model though)
	if (matchStr(model, "models/nihilanth.mdl") || matchStr(model, "models/controller.mdl") ||
		matchStr(model, "models/tentacle2.mdl") || matchStr(model, "models/nihilanth.mdl") ||
		matchStr(model, "models/kingpin.mdl") || model.find("apache") != string::npos ||
		matchStr(model, "models/sentry.mdl"))
		return ""; 

	if (model.find("sprites/") != string::npos)
		replacement = random_model_replace(model);	
	else
	{
		do
		{
			replacement = random_model_replace(model);	
			// using a replacement model that's been replaced in this file causes Host Precache error
		} while (find(replaced.begin(), replaced.end(), replacement) != replaced.end() ||
				 find(replace_models.begin(), replace_models.end(), replacement) != replace_models.end());
	}
	return replacement;
}

vector<string> writeGMR(string new_gmr_path, string old_gmr_path, string_hashmap& ent_models, int replace_level)
{
	vector<string> replace_models;
	vector<string> replaced;

	replace_models.insert(replace_models.end(), monster_sprites.begin(), monster_sprites.end());
	replace_models.insert(replace_models.end(), default_gib_models.begin(), default_gib_models.end());
	replace_models.insert(replace_models.end(), default_precache_models.begin(), default_precache_models.end());

	if (replace_level == 0) // load the old GMR file and replace models only if no new models are precached
	{
		if (old_gmr_path.length())
		{
			string_hashmap old_gmr;
			set<string> replacements;

			ifstream gmrfile(old_gmr_path);
			if (gmrfile.is_open())
			{
				while ( !gmrfile.eof() )
				{
					string line2;
					getline (gmrfile, line2);

					string a, b;
					if (parse_gmr_line(line2, a, b))
					{
						old_gmr[a] = b;
						replacements.insert(b);
					}
				}
			}
			else
				print("UNABLE TO OPEN GMR: " + old_gmr_path);

			if (replacements.size() < old_gmr.size())
			{
				// old gmr file was used to remove models
				int removes = 0;
				for (string_hashmap::iterator it = old_gmr.begin(); it != old_gmr.end(); ++it)
				{
					// don't replace models listed in old gmr
					vector<string>::iterator item = find(replace_models.begin(), replace_models.end(), it->first);
					if (item != replace_models.end())
					{
						replace_models.erase(item);
						removes++;
					}
					ent_models[it->first] = it->second;
				}

				int new_randomizations = 0;
				for (set<string>::iterator it = replacements.begin(); it != replacements.end(); ++it)
				{
					int uses = 0;
					string replaces;
					for (string_hashmap::iterator it2 = old_gmr.begin(); it2 != old_gmr.end(); ++it2)
					{
						if (matchStr(it2->second, *it))
						{
							replaces = it2->first;
							if (uses++ > 0)
								break;
						}
					}
					if (uses == 1)
					{
						string replacement = get_random_replacement(replaces, replaced, replace_models);
						if (!replacement.length())
							continue;
						ent_models[replaces] = replacement;
						new_randomizations++;
					}
				}
			}
		}
	}

	ofstream myfile;
	myfile.open(new_gmr_path);

	for (uint i = 0; i < replace_models.size(); i++)
	{
		if (matchStr(replace_models[i], "models/player.mdl"))
			continue; // replacing it causes a crash with certain weapons (looks for T model)
		string first = replace_models[i];
		if (find(dont_replace.begin(), dont_replace.end(), first) != dont_replace.end())
			continue;
		string second;
		do { second = get_random_replacement(first, replaced, replace_models); }
		while(!is_safe_model_replacement("", first, second));
		if (!second.length())
			continue;
		replaced.push_back(second);
		myfile << '\"' << first << "\" \"" << second << '\"' << '\n';
	}

	// entity replacements
	for (string_hashmap::iterator it = ent_models.begin(); it != ent_models.end(); ++it)
	{
		myfile << '\"' << it->first << "\" \"" << it->second << '\"' << '\n';
		replaced.push_back(it->first);
		replace_models.push_back(it->second);
	}
	myfile.close();

	if (replace_models.size() > 255)
		print("TOO MANY MODELS: " + str(replaced.size()));

	return replace_models;
}

bool replace_entity_model(Entity * ent, string model_key, int model_type, int& potential_additions)
{
	if (mdlMode == MDL_NONE || !potential_additions)
		return false;
	if (mdlMode == MDL_TYPED)
		ent->keyvalues[model_key] = "models/" + get_random_model(model_type) + ".mdl";
	else
		ent->keyvalues[model_key] = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
	potential_additions -= 1;
	return true;
}

bool replace_entity_sprite(Entity * ent, string model_key, int sprite_type, int& potential_additions)
{
	if (mdlMode == MDL_NONE || !potential_additions)
		return false;
	if (mdlMode == MDL_TYPED)
		ent->keyvalues[model_key] = "sprites/" + get_random_sprite(sprite_type) + ".spr";
	else
		ent->keyvalues[model_key] = "sprites/" + get_random_sprite(SPRITE_TYPE_GENERIC) + ".spr";
	potential_additions -= 1;
	return true;
}

bool is_safe_model_replacement(string classname, string model, string replacement)
{
	if (classname.find("monster_hwgrunt") == 0)
	{
		for (uint i = 0; i < hwgrunt_blacklist.size(); ++i)
			if (hwgrunt_blacklist[i].find(replacement) != string::npos)
				return false;
	}
	if (model.find("w_satchel") == 0)
	{
		for (uint i = 0; i < satchel_blacklist.size(); ++i)
			if (satchel_blacklist[i].find(replacement) != string::npos)
				return false;
	}
	if (model.find("models/doctor.mdl") == 0)
		return false; // problematic model
	return true;
}

void do_model_replacement(BSP * map, Entity** ents, string path)
{
	string old_gmr_path;
	string new_gmr_path = map->name + "_gmr.gsrand";
	string_hashmap ent_models;
	int total_models, potential_additions;
	count_map_models(map, ents, path, total_models, potential_additions, old_gmr_path, ent_models);

	int max_model_limit = MAX_MAP_MODELS - 4; // let's play it safe
	int replace_level = 2;
	if (total_models + potential_additions < max_model_limit)
	{
		replace_level = 2;
		print(str(total_models + potential_additions) + " models. ");
	}
	else if (total_models + (int)default_weapon_models.size() < max_model_limit)
	{
		replace_level = 1;
		print(str(total_models + potential_additions) + " models (GMR ONLY). ");
	}
	else
	{
		replace_level = 0;
		print(str(total_models) + " models (LIMITED GMR ONLY). ");
	}

	// these are always replaced by GMR
	if (replace_level <= 1)
	{
		vector<string> replace_models;
		vector<string> replaced;
		replace_models.insert(replace_models.end(), default_gib_models.begin(), default_gib_models.end());

		for (string_hashmap::iterator it = ent_models.begin(); it != ent_models.end(); ++it)
		{
			string cname = it->second;
			it->second = "";
			if (cname.find("env_shooter") != string::npos)
				it->second = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";

			else if (matchStr(cname, "env_beam"))
				it->second = "sprites/" + get_random_sprite(SPRITE_TYPE_GENERIC) + ".spr";
			else if (matchStr(cname, "env_funnel"))
				it->second = "sprites/" + get_random_sprite(SPRITE_TYPE_GENERIC) + ".spr";
			else if (matchStr(cname, "env_laser"))
			{
				it->second = "sprites/" + get_random_sprite(SPRITE_TYPE_GENERIC) + ".spr";
				it->second = "sprites/" + get_random_sprite(SPRITE_TYPE_GENERIC) + ".spr";
			}
			else if (matchStr(cname, "item_recharge") || matchStr(cname, "item_healthcharger"))
				it->second = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
			else if (matchStr(cname, "env_sprite") || matchStr(cname, "cycler_sprite") || matchStr(cname, "env_glow"))
			{
				// low chance of sprite becoming a model
				int r = rand() % 5;
				if (!r) it->second = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
				else    it->second = "sprites/" + get_random_sprite(SPRITE_TYPE_GENERIC) + ".spr";
			}
			else if (matchStr(cname, "env_spritetrain"))
			{
				int r = rand() % 2;
				if (!r) it->second = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
				else    it->second = "sprites/" + get_random_sprite(SPRITE_TYPE_GENERIC) + ".spr";
			}
			else if (cname.find("monster_") == 0 || cname.find("ammo_") != string::npos || cname.find("item_") != string::npos 
			    || matchStr(cname, "cycler") || matchStr(cname, "cycler_weapon") || cname.find("weapon_") == 0
				||  matchStr(cname, "env_beverage") || matchStr(cname, "weaponbox"))
			{
				if (matchStr(cname, "monster_kingpin") || matchStr(cname, "monster_tentacle"))
				{
					ent_models.erase(it--);
					continue; // replacing would cause a crash
				}
				else if (matchStr(cname, "monster_alien_controller"))
				{
					// crash for reasons unknown. Only using models that have been manually tested.
					int r = rand() % controller_whitelist.size();
					it->second = controller_whitelist[r];
				}
				else if (matchStr(cname, "monster_nihilanth"))
				{
					// crash for reasons unknown. Only using models that have been manually tested.
					int r = rand() % nih_whitelist.size();
					it->second = nih_whitelist[r];
				}
				else if (matchStr(cname, "monster_apache") || matchStr(cname, "monster_sentry"))
				{
					// crash if model has less than 2 bone controllers
					int r = rand() % NUM_APACHE_MODELS;
					it->second = "models/" + string(APACHE_MODELS[r]) + ".mdl";
				}
				else
				{
					int mtype = cname.find("monster_") == 0 ? MODEL_TYPE_MONSTER : MODEL_TYPE_GENERIC;
					do { it->second = "models/" + get_random_model(mtype) + ".mdl"; }
					while (!is_safe_model_replacement(cname, it->first, it->second));
				
					if (cname.find("monster_") == 0)
					{
						// game crashes if replacing monster_* model with w_ model
						// or just doesn't work if monster is spawned from squadmaker
						// p_ and v_ models get replaced in GMR so messes up custom displayname
						bool is_replaced_model = true;
					
						while (it->second.find("/w_") != string::npos ||
							   it->second.find("/p_") != string::npos ||
							   it->second.find("/v_") != string::npos ||
							   find(replace_models.begin(), replace_models.end(), it->second) != replace_models.end())
							it->second = "models/" + get_random_model(MODEL_TYPE_MONSTER) + ".mdl";
					}
				}
				// rename monster to "model_name monster_name"
				if (cname.find("monster_") == 0)
				{
					for (int i = 0; i < MAX_MAP_ENTITIES; i++)
					{
						if (ents[i] == NULL)
							break;

						string cname2 = ents[i]->keyvalues["classname"];
						if (cname2.find("monstermaker") != string::npos || cname2.find("env_xenmaker") != string::npos)
						{
							ents[i]->keyvalues.erase("model");
							cname2 = ents[i]->keyvalues["classname"] = "squadmaker"; // monstermakers can't do model replacement
						}

						if (cname.find("squadmaker") != string::npos)
						{
							ents[i]->keyvalues.erase("new_model"); // in case it doesn't get replaced
							cname2 = toLowerCase(ents[i]->keyvalues["monstertype"]);
						}

						if (cname2.find(cname) != 0)
							continue;

						string raw_model_name = it->second;
						raw_model_name = getSubStr(raw_model_name, 0, raw_model_name.length()-4); // strip .mdl
						raw_model_name = getSubStr(raw_model_name, raw_model_name.find_last_of("/\\")+1);
						if (raw_model_name.find("v_") == 0 || raw_model_name.find("p_") == 0)
							raw_model_name = getSubStr(raw_model_name, 2);
						string displayname = ents[i]->keyvalues["displayname"];
						if (!displayname.length())
							displayname = getSubStr(cname, string("monster_").length());

						ents[i]->keyvalues["displayname"] = raw_model_name + " " + displayname; 
					} 
				}
			}
			else
			{
				// not a recognized class
				ent_models.erase(it--);
				continue;
			}

			if (find(replaced.begin(), replaced.end(), it->second) != replaced.end() ||
				!is_safe_model_replacement(cname, it->first, it->second))
			{
				it->second = cname;
				it--; // do it again. We can't use a replacement model that's been replaced
			}
			else
				replaced.push_back(it->first);
		}
	}
	if (replace_level > 1)
	{
		vector<string> replace_models;
		replace_models.insert(replace_models.end(), default_precache_models.begin(), default_precache_models.end());
		replace_models.insert(replace_models.end(), default_gib_models.begin(), default_gib_models.end());

		ent_models.clear();

		string rand_roach = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
		string rand_nih = nih_whitelist[rand() % nih_whitelist.size()];

		for (int i = 0; i < MAX_MAP_ENTITIES; i++)
		{
			if (ents[i] == NULL)
				break;

			string cname = toLowerCase(ents[i]->keyvalues["classname"]);

			if (cname.find("monstermaker") != string::npos || cname.find("env_xenmaker") != string::npos)
			{
				ents[i]->keyvalues.erase("model");
				cname = ents[i]->keyvalues["classname"] = "squadmaker"; // monstermakers can't do model replacement
			}

			string custom_monster_model_key = "model";
			if (cname.find("squadmaker") != string::npos)
			{
				ents[i]->keyvalues.erase("new_model"); // in case it doesn't get replaced
				cname = toLowerCase(ents[i]->keyvalues["monstertype"]);
				custom_monster_model_key = "new_model";
			}

			if (matchStr(cname, "func_breakable"))
				replace_entity_model(ents[i], "gibmodel", MODEL_TYPE_GENERIC, potential_additions);

			if (cname.find("env_shooter") != string::npos)
				replace_entity_model(ents[i], "shootmodel", MODEL_TYPE_GENERIC, potential_additions);

			if (matchStr(cname, "env_beam"))
				replace_entity_sprite(ents[i], "texture", SPRITE_TYPE_GENERIC, potential_additions);
			if (matchStr(cname, "env_funnel"))
				replace_entity_sprite(ents[i], "sprite", SPRITE_TYPE_GENERIC, potential_additions);
			if (matchStr(cname, "env_laser"))
			{
				replace_entity_sprite(ents[i], "texture", SPRITE_TYPE_GENERIC, potential_additions);
				replace_entity_sprite(ents[i], "EndSprite", SPRITE_TYPE_GENERIC, potential_additions);
			}
			if (matchStr(cname, "item_recharge") || matchStr(cname, "item_healthcharger"))
			{
				replace_entity_model(ents[i], "model_juice", MODEL_TYPE_GENERIC, potential_additions);
				continue;
			}
			if (matchStr(cname, "env_sprite") || matchStr(cname, "cycler_sprite") || matchStr(cname, "env_glow"))
			{
				// low chance of sprite becoming a model
				int r = rand() % 5;
				if (!r) replace_entity_model(ents[i], "model", MODEL_TYPE_GENERIC, potential_additions);
				else    replace_entity_sprite(ents[i], "model", SPRITE_TYPE_GENERIC, potential_additions);
			}
			if (matchStr(cname, "env_spritetrain"))
			{
				int r = rand() % 2;
				if (!r) replace_entity_model(ents[i], "model", MODEL_TYPE_GENERIC, potential_additions);
				else    replace_entity_sprite(ents[i], "model", SPRITE_TYPE_GENERIC, potential_additions);
			}

			if (cname.find("monster_") == 0 || cname.find("ammo_") != string::npos || cname.find("item_") != string::npos 
			    || matchStr(cname, "cycler") || matchStr(cname, "cycler_weapon")
				||  matchStr(cname, "env_beverage") || matchStr(cname, "weaponbox"))
			{
				if (matchStr(cname, "monster_kingpin") || matchStr(cname, "monster_tentacle"))
				{
					ents[i]->keyvalues.erase(custom_monster_model_key);
					continue; // replacing would cause a crash
				}
				else if (matchStr(cname, "monster_nihilanth"))
				{
					ents[i]->keyvalues[custom_monster_model_key] = rand_nih;
					ent_models["models/" + default_monster_models["nihilanth"] + ".mdl"] = rand_nih;
				}
				else if (matchStr(cname, "monster_cockroach"))
				{
					ents[i]->keyvalues[custom_monster_model_key] = rand_roach;
					ent_models["models/" + default_monster_models["cockroach"] + ".mdl"] = rand_roach;
				}
				else if (matchStr(cname, "monster_alien_controller"))
				{
					int r = rand() % controller_whitelist.size();
					ents[i]->keyvalues[custom_monster_model_key] = controller_whitelist[r];
				}
				else if (matchStr(cname, "monster_apache") || matchStr(cname, "monster_sentry"))
				{
					// crash if model has less than 2 bone controllers
					int r = rand() % NUM_APACHE_MODELS;
					ents[i]->keyvalues[custom_monster_model_key] = "models/" + string(APACHE_MODELS[r]) + ".mdl";
				}
				else
				{
					int mtype = cname.find("monster_") == 0 ? MODEL_TYPE_MONSTER : MODEL_TYPE_GENERIC;
					do { replace_entity_model(ents[i], custom_monster_model_key, mtype, potential_additions); }
					while (!is_safe_model_replacement(cname, "", ents[i]->keyvalues[custom_monster_model_key]));
				}

				if (cname.find("monster_") == 0)
				{
					// game crashes if replacing monster_* model with w_ model
					// or just doesn't work if monster is spawned from squadmaker
					// p_ and v_ models get replaced in GMR so messes up custom displayname
					while (ents[i]->keyvalues[custom_monster_model_key].find("/w_") != string::npos ||
							ents[i]->keyvalues[custom_monster_model_key].find("/p_") != string::npos ||
							ents[i]->keyvalues[custom_monster_model_key].find("/v_") != string::npos ||
							find(replace_models.begin(), replace_models.end(), ents[i]->keyvalues[custom_monster_model_key]) != replace_models.end() ||
							!is_safe_model_replacement(cname, "", ents[i]->keyvalues[custom_monster_model_key]))
						replace_entity_model(ents[i], custom_monster_model_key, MODEL_TYPE_MONSTER, ++potential_additions);

					if (!ents[i]->keyvalues[custom_monster_model_key].length())
					{
						println("WHAT WHY: ");
						if (!replace_entity_model(ents[i], custom_monster_model_key, MODEL_TYPE_MONSTER, ++potential_additions))
							println("OH THATS WHY");
						println("DETAILS: " + cname + " " + ents[i]->keyvalues[custom_monster_model_key]);
					}
					// rename monster to "model_name monster_name"
					string raw_model_name = ents[i]->keyvalues[custom_monster_model_key];
					raw_model_name = getSubStr(raw_model_name, 0, raw_model_name.length()-4); // strip .mdl
					raw_model_name = getSubStr(raw_model_name, raw_model_name.find_last_of("/\\")+1);
					if (raw_model_name.find("v_") == 0 || raw_model_name.find("p_") == 0)
						raw_model_name = getSubStr(raw_model_name, 2);
					string displayname = ents[i]->keyvalues["displayname"];
					if (!displayname.length())
						displayname = getSubStr(cname, string("monster_").length());
						
					ents[i]->keyvalues["displayname"] = raw_model_name + " " + displayname; 
				}
			}

			if (potential_additions <= 0)
			{
				print("RAN OUT OF MODEL ADDITIONS");
				break;
			}
		} 
	}

	vector<string> model_replacements = writeGMR(path + new_gmr_path, old_gmr_path, ent_models, replace_level);

	// make super sure model replacements are precached
	for (uint i = 0; i < model_replacements.size(); i += 9)
	{
		Entity * ent = add_new_entity(ents, "custom_precache");
		if (!ent) break;
		for (uint k = i; k < i+9 && k < model_replacements.size(); ++k)
			ent->addKeyvalue("model_" + str((k-i)+1), model_replacements[k]);
	}
}

void genModelList()
{
	vector<string> allModels;
	string modelPath = "models/";
	vector<string> dirs = getAllSubdirs(modelPath);
	vector<string> monster;
	vector<string> props;
	vector<string> v_models;
	vector<string> p_models;
	vector<string> w_models;
	vector<string> apache_models;
	vector<string> player_models;

	for (uint i = 0; i < dirs.size(); i++)
	{
		vector<string> results = getDirFiles(dirs[i], "mdl");
		string cpath = getSubStr(dirs[i],7); // skip models/

		for (uint k = 0; k < results.size(); k++)
		{
			if (cpath.find("player/") == 0)
			{
				player_models.push_back(cpath + results[k].substr(0, results[k].find(".mdl")));
				continue;
			}
			string name = getSubStr(results[k],0,results[k].length()-4);

			// skip animation models
			string last_2_chars = getSubStr(name, name.length()-2);
			if (isNumber(last_2_chars)) // 357 gun doesn't use extra models (USUALLY)
			{
				bool is_anim_model = false;
				string find_model = getSubStr(name, 0, name.length()-2);
				for (uint x = 0; x < results.size(); x++)
				{
					string nameTemp = getSubStr(results[x],0,results[x].length()-4);
					if (matchStr(nameTemp, find_model))
						is_anim_model = true;
				}
				if (is_anim_model)
					continue;
			}

			if (name.length() > 0)
			{
				ifstream fin (dirs[i] + results[k], ios::binary);

				MDLHEADER mdlHead;
				fin.read((char*)&mdlHead, sizeof(MDLHEADER));

				if (mdlHead.numbonecontrollers >= 2)
					apache_models.push_back(cpath + results[k].substr(0, results[k].find(".mdl")));

				if (string(mdlHead.name).length() <= 0)
				{
					// println("found T model: " + name);
					fin.close();
					continue;
				}

				if (mdlHead.id == 1364411465)
				{
					//println("Found weird model: " + name);
					fin.close();
					continue;
				}

				if (name.length() > 2)
				{
					string prefix = getSubStr(name,0,2);
					if (matchStr(prefix, "v_"))
					{
						v_models.push_back(cpath + results[k].substr(0, results[k].find(".mdl")));
						continue;
					}
					else if (matchStr(prefix, "p_"))
					{
						p_models.push_back(cpath + results[k].substr(0, results[k].find(".mdl")));
						continue;
					}
					else if (matchStr(prefix, "w_"))
					{
						w_models.push_back(cpath + results[k].substr(0, results[k].find(".mdl")));
						continue;
					}
				}

				if (mdlHead.numseq == 1)
				{
					fin.seekg(mdlHead.seqindex);
					MDLSEQUENCE seq;
					fin.read((char*)&seq, sizeof(MDLSEQUENCE));
					string sub = getSubStr(string(seq.label),0,4);

					if (seq.numframes == 1 || mdlHead.numbones == 1 || matchStr(sub, "idle"))
					{
						//println("definite prop: " + name);
						props.push_back(cpath + results[k].substr(0, results[k].find(".mdl")));
						continue;
					}
					// TODO: Check the sequence for movement, don't just assume it's a prop
					//       by those
					/*
					else
					{
						println("possible prop: " + name + " idx: " + str(mdlHead.seqindex));
						println("first seq: " + string(seq.label) + ": " + str(seq.numframes));
					}
					*/
				}

				fin.close();

				monster.push_back(cpath + results[k].substr(0, results[k].find(".mdl")));

			}				
		}
	}

	println("#define NUM_APACHE_MODELS " + str(apache_models.size()));
	println("static const char * APACHE_MODELS[NUM_APACHE_MODELS] =");
	println("{");
	for (uint s = 0; s < apache_models.size(); s++)
		println("\t\"" + apache_models[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_V " + str(v_models.size()));
	println("static const char * MODEL_V[NUM_MODEL_V] =");
	println("{");
	for (uint s = 0; s < v_models.size(); s++)
		println("\t\"" + v_models[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_P " + str(p_models.size()));
	println("static const char * MODEL_P[NUM_MODEL_P] =");
	println("{");
	for (uint s = 0; s < p_models.size(); s++)
		println("\t\"" + p_models[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_W " + str(w_models.size()));
	println("static const char * MODEL_W[NUM_MODEL_W] =");
	println("{");
	for (uint s = 0; s < w_models.size(); s++)
		println("\t\"" + w_models[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_PROPS " + str(props.size()));
	println("static const char * MODEL_PROPS[NUM_MODEL_PROPS] =");
	println("{");
	for (uint s = 0; s < props.size(); s++)
		println("\t\"" + props[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_MONSTERS " + str(monster.size()));
	println("static const char * MODEL_MONSTERS[NUM_MODEL_MONSTERS] =");
	println("{");
	for (uint s = 0; s < monster.size(); s++)
		println("\t\"" + monster[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_PLAYERS " + str(player_models.size()));
	println("static const char * MODEL_PLAYERS[NUM_MODEL_PLAYERS] =");
	println("{");
	for (uint s = 0; s < player_models.size(); s++)
		println("\t\"" + player_models[s] + "\",");
	println("};\n");

	writeLog();
}

void genSpriteList()
{
	vector<string> allSprites;
	string spritePath = "sprites/";
	vector<string> dirs = getAllSubdirs(spritePath);
	vector<string> animated;
	vector<string> singleframe;

	for (uint i = 0; i < dirs.size(); i++)
	{
		vector<string> results = getDirFiles(dirs[i], "spr");
		string cpath = getSubStr(dirs[i],8); // skip sprites/
		for (uint k = 0; k < results.size(); k++)
		{
			string name = getSubStr(results[k],0,results[k].length()-4);
			if (name.length() > 0)
			{
				ifstream fin (dirs[i] + results[k], ios::binary);

				SPRHEADER sprHead;
				fin.read((char*)&sprHead, sizeof(SPRHEADER));
				fin.close();

				if (sprHead.frames > 1)
					animated.push_back(cpath + results[k].substr(0, results[k].find(".spr")));
				else
					singleframe.push_back(cpath + results[k].substr(0, results[k].find(".spr")));
			}				
		}
	}

	println("#define NUM_ANIMATED_SPRITES " + str(animated.size()));
	println("static const char * ANIMATED_SPRITES[NUM_ANIMATED_SPRITES] =");
	println("{");
	for (uint s = 0; s < animated.size(); s++)
		println("\t\"" + animated[s] + "\",");
	println("};\n");

	println("#define NUM_STATIC_SPRITES " + str(singleframe.size()));
	println("static const char * STATIC_SPRITES[NUM_STATIC_SPRITES] =");
	println("{");
	for (uint s = 0; s < singleframe.size(); s++)
		println("\t\"" + singleframe[s] + "\",");
	println("};\n");

	writeLog();
}
