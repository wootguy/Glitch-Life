#include "model_util.h"
#include "ent_util.h"
#include "gsrand.h"
#include <set>
#include "studio.h"

//
// Also kind of sprite util file
//


// request a completely random model (MODEL_TYPE_GENERIC), or a specific model type
string get_random_model(int request_model_type)
{
	int total_wep_models = user_v_models.size() + user_p_models.size() + user_w_models.size();
	int w_start = 0;
	int v_start = w_start + user_w_models.size();
	int p_start = v_start + user_v_models.size();
	int r;

	if (request_model_type == MODEL_TYPE_GENERIC)
	{
		r = rand() % 3;
		if (r == 0 && total_wep_models > 0) // weapon
		{
			r = rand() % total_wep_models;
			if (r >= p_start && user_p_models.size())
				return user_p_models[r - p_start];
			else if (r >= v_start && user_v_models.size())
				return user_v_models[r - v_start];
			else if (r >= w_start && user_w_models.size())
				return user_w_models[r - w_start];
		}
		else if (r == 1 && user_prop_models.size())
			return user_prop_models[rand() % user_prop_models.size()];
		else
		{
			r = rand() % 10;
			if ((r || !user_player_models.size()) && user_monster_models.size()) 
				return user_monster_models[rand() % user_monster_models.size()];
			if (user_player_models.size())
				return user_player_models[rand() % user_player_models.size()]; 
		}
	}
	else if (request_model_type == MODEL_TYPE_PROP && user_prop_models.size())
		return user_prop_models[rand() % user_prop_models.size()];
	else if (request_model_type == MODEL_TYPE_MONSTER)
	{
		r = rand() % 10;
		if ((r || !user_player_models.size()) && user_monster_models.size()) 
			return user_monster_models[rand() % user_monster_models.size()];
		if (user_player_models.size())
			return user_player_models[rand() % user_player_models.size()]; 
	}
	else if (request_model_type == MODEL_TYPE_P_WEAPON && user_p_models.size())
		return user_p_models[rand() % user_p_models.size()];
	else if (request_model_type == MODEL_TYPE_W_WEAPON && user_w_models.size())
		return user_w_models[rand() % user_w_models.size()];
	else if (request_model_type == MODEL_TYPE_V_WEAPON && user_v_models.size())
		return user_v_models[rand() % user_v_models.size()];

	return "not_precached";
}

string get_random_sprite(int request_model_type)
{
	int total_sprites = user_sprites.size() + user_animated_sprites.size();
	int anim_start = 0;
	int static_start = user_animated_sprites.size();
	int r;

	if (total_sprites > 0)
	{
		if (request_model_type == SPRITE_TYPE_GENERIC)
		{
			r = rand() % total_sprites;
			if (r >= static_start && user_sprites.size())
				return user_sprites[r - static_start];
			return user_animated_sprites[r];
		}
		else if (request_model_type == SPRITE_TYPE_ANIMATED && user_animated_sprites.size())
			return user_animated_sprites[rand() % user_animated_sprites.size()];
		else if (request_model_type == SPRITE_TYPE_STATIC && user_sprites.size())
			return user_sprites[rand() % user_sprites.size()]; 
	}

	return "tile";
}

void filter_default_model_content(vector<string>& unfiltered)
{
	if (contentMode != CONTENT_EVERYTHING)
	{
		vector<string> filtered;
		for (uint i = 0, sz = unfiltered.size(); i < sz; ++i)
		{
			bool match = false;
			for (int k = 0; k < NUM_MODEL_MONSTERS; k++)
			{
				if (matchStr(MODEL_MONSTERS[k], unfiltered[i]))
				{
					match = true;
					break;
				}
			}
			for (int k = 0; k < NUM_MODEL_PROPS && !match; k++)
			{
				if (matchStr(MODEL_PROPS[k], unfiltered[i]))
				{
					match = true;
					break;
				}
			}
			for (int k = 0; k < NUM_MODEL_V && !match; k++)
			{
				if (matchStr(MODEL_V[k], unfiltered[i]))
				{
					match = true;
					break;
				}
			}
			for (int k = 0; k < NUM_MODEL_P && !match; k++)
			{
				if (matchStr(MODEL_P[k], unfiltered[i]))
				{
					match = true;
					break;
				}
			}
			for (int k = 0; k < NUM_MODEL_W && !match; k++)
			{
				if (matchStr(MODEL_W[k], unfiltered[i]))
				{
					match = true;
					break;
				}
			}
			for (int k = 0; k < NUM_MODEL_PLAYERS && !match; k++)
			{
				if (matchStr(MODEL_PLAYERS[k], unfiltered[i]))
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

void add_to_black_list(string model, string special_name)
{
	for (list_hashmap::iterator it = monster_blacklists.begin(); it != monster_blacklists.end(); ++it)
	{
		if (find(it->second.begin(), it->second.end(), special_name) != it->second.end())
		{
			it->second.push_back("models/" + model + ".mdl");
		}
	}
}

void find_all_models(string modelPath)
{
	user_monster_models.clear();
	user_prop_models.clear();
	user_player_models.clear();
	user_v_models.clear();
	user_p_models.clear();
	user_w_models.clear();
	user_apache_models.clear(); 

	vector<string> dirs = getAllSubdirs(modelPath);

	for (uint i = 0; i < dirs.size(); i++)
	{
		vector<string> results = getDirFiles(dirs[i], "mdl");

		string cpath = "";
		int dir_find = dirs[i].find("models/");
		if (dir_find != string::npos && dirs[i].length() > dir_find + string("models/").length())
			cpath = getSubStr(dirs[i], dir_find + string("models/").length()); // skip models/

		for (uint k = 0; k < results.size(); k++)
		{
			int end_name = results[k].length() - 4; // skip .mdl
			if (cpath.find("player/") == 0)
			{
				user_player_models.push_back(cpath + results[k].substr(0, end_name));
				continue;
			}
			string name = getSubStr(results[k],0,results[k].length()-4);

			// skip animation models
			if (name.length() > 2)
			{
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
			}


			if (name.length() > 0)
			{
				ifstream fin (dirs[i] + results[k], ios::binary);

				studiohdr_t mdlHead;
				fin.read((char*)&mdlHead, sizeof(studiohdr_t));

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

				if (mdlHead.numbonecontrollers >= 2)
				{
					if (mdlHead.numbonecontrollers == 2 && mdlHead.numseq == 1)
					{
						fin.seekg(mdlHead.seqindex);
						mstudioseqdesc_t seq;
						fin.read((char*)&seq, sizeof(mstudioseqdesc_t));

						if (seq.numframes > 1 && string(seq.label).find("idle") == 0)
							add_to_black_list(cpath + results[k].substr(0, end_name), "*apache");
					}
					user_apache_models.push_back(cpath + results[k].substr(0, end_name));
				}

				int numTextures = mdlHead.numtextures;
				if (numTextures == 0)
				{
					string t_path = modelPath + cpath + results[k].substr(0, end_name) + "t.mdl";
					if (!fileExists(t_path))
					{
						if (printRejects)
							println("Missing T model: " + cpath + results[k]);
						continue;
					}
					ifstream fint (t_path, ios::binary);
					studiohdr_t mdlHeadt;
					fint.read((char*)&mdlHeadt, sizeof(studiohdr_t));
					fint.close();
					numTextures = mdlHeadt.numtextures;
				}

				if (modelSafety == MODEL_SAFETY_TEX_LIMIT && numTextures > 16)
				{
					if (printRejects)
						println("More than 16 textures: " + cpath + results[k]);
					continue;
				}

				if (name.length() > 2)
				{
					string prefix = getSubStr(name,0,2);
					if (matchStr(prefix, "v_"))
					{
						user_v_models.push_back(cpath + results[k].substr(0, end_name));
						continue;
					}
					else if (matchStr(prefix, "p_"))
					{
						user_p_models.push_back(cpath + results[k].substr(0, end_name));
						continue;
					}
					else if (matchStr(prefix, "w_"))
					{
						user_w_models.push_back(cpath + results[k].substr(0, end_name));
						continue;
					}
				}

				if (mdlHead.numseq == 1)
				{
					fin.seekg(mdlHead.seqindex);
					mstudioseqdesc_t seq;
					fin.read((char*)&seq, sizeof(mstudioseqdesc_t));

					if (seq.numframes == 1 || mdlHead.numbones == 1 || string(seq.label).find("idle") != string::npos)
					{
						user_prop_models.push_back(cpath + results[k].substr(0, end_name));
						continue;
					}
					// TODO: Check the sequence for movement, don't just assume it's a monster
				}

				
				// figure out what kind of monster it is
				int headcrab_matches = 0;
				int babygarg_matches = 0; // sadly there's no way to tell the difference between a garg and babygarg model
				int pitdrone_matches = 0;
				int bigmom_matches = 0;
				int bullsquid_matches = 0;
				int houndeye_matches = 0;
				int friendly_matches = 0;
				int stukabat_matches = 0;
				for (int i = 0; i < mdlHead.numseq; ++i)
				{
					fin.seekg(mdlHead.seqindex + i*sizeof(mstudioseqdesc_t));
					mstudioseqdesc_t seq;
					fin.read((char*)&seq, sizeof(mstudioseqdesc_t));
					if (matchStr(string(seq.label), "jump") || matchStr(string(seq.label), "jump_variation1") ||
						matchStr(string(seq.label), "jump_variation2") || matchStr(string(seq.label), "yaw_adjustment"))
						headcrab_matches++;
					if (matchStr(string(seq.label), "shootflames1") || matchStr(string(seq.label), "shootflames2") ||
						matchStr(string(seq.label), "stomp"))
						babygarg_matches++;
					if (matchStr(string(seq.label), "gallop") || matchStr(string(seq.label), "range") ||
						matchStr(string(seq.label), "reload"))
						pitdrone_matches++;
					if (matchStr(string(seq.label), "mortar") || matchStr(string(seq.label), "defend") ||
						matchStr(string(seq.label), "spawn"))
						bigmom_matches++;
					if (matchStr(string(seq.label), "suprisedhop") || matchStr(string(seq.label), "whip") ||
						matchStr(string(seq.label), "bite"))
						bullsquid_matches++;
					if (matchStr(string(seq.label), "die_crumple") || matchStr(string(seq.label), "walk_limp") ||
						matchStr(string(seq.label), "sleeptostand"))
						houndeye_matches++;
					if (matchStr(string(seq.label), "doublewhip") || matchStr(string(seq.label), "vomit") ||
						matchStr(string(seq.label), "smallflinch2"))
						friendly_matches++;
					if (matchStr(string(seq.label), "Land_ceiling") || matchStr(string(seq.label), "Attack_claw") ||
						matchStr(string(seq.label), "Hover"))
						stukabat_matches++;
				}
				if (headcrab_matches  >= 4) add_to_black_list(cpath + results[k].substr(0, end_name), "*headcrab");
				if (babygarg_matches  >= 3) add_to_black_list(cpath + results[k].substr(0, end_name), "*garg");
				if (pitdrone_matches  >= 3) add_to_black_list(cpath + results[k].substr(0, end_name), "*pitdrone");
				if (bigmom_matches    >= 3) add_to_black_list(cpath + results[k].substr(0, end_name), "*bigmomma");
				if (bullsquid_matches >= 3) add_to_black_list(cpath + results[k].substr(0, end_name), "*bullsquid");
				if (houndeye_matches  >= 3) add_to_black_list(cpath + results[k].substr(0, end_name), "*houndeye");
				if (friendly_matches  >= 3) add_to_black_list(cpath + results[k].substr(0, end_name), "*friendly");
				if (stukabat_matches  >= 3) add_to_black_list(cpath + results[k].substr(0, end_name), "*stukabat");

				if (fileExists("models/" + cpath + results[k].substr(0, end_name) + "t.mdl"))
					add_to_black_list(cpath + results[k].substr(0, end_name), "*tmodel");

				fin.close();

				user_monster_models.push_back(cpath + results[k].substr(0, end_name));
			}				
		}
	}
}

void get_all_models()
{	
	parse_model_lists(); // get all monster whitelists and blacklists

	find_all_models("../valve/models/");
	vector<string> temp_monster_models = user_monster_models;
	vector<string> temp_prop_models = user_prop_models;
	vector<string> temp_player_models = user_player_models;
	vector<string> temp_v_models = user_v_models;
	vector<string> temp_p_models = user_p_models;
	vector<string> temp_w_models = user_w_models;
	vector<string> temp_apache_models = user_apache_models; 
	find_all_models("models/");

	// combine half-life and sven-coop models
	insert_unique(temp_monster_models, user_monster_models);
	insert_unique(temp_prop_models, user_prop_models);
	insert_unique(temp_player_models, user_player_models);
	insert_unique(temp_v_models, user_v_models);
	insert_unique(temp_p_models, user_p_models);
	insert_unique(temp_w_models, user_w_models);
	insert_unique(temp_apache_models, user_apache_models);

	filter_default_model_content(user_monster_models);
	filter_default_model_content(user_prop_models);
	filter_default_model_content(user_v_models);
	filter_default_model_content(user_p_models);
	filter_default_model_content(user_w_models);
	filter_default_model_content(user_apache_models);
	filter_default_model_content(user_player_models);	

	vector<string> all_user_models;
	for (uint i = 0; i < user_monster_models.size(); ++i)
		all_user_models.push_back("models/" + user_monster_models[i] + ".mdl");
	for (uint i = 0; i < user_prop_models.size(); ++i)
		all_user_models.push_back("models/" + user_prop_models[i] + ".mdl");
	for (uint i = 0; i < user_v_models.size(); ++i)
		all_user_models.push_back("models/" + user_v_models[i] + ".mdl");
	for (uint i = 0; i < user_p_models.size(); ++i)
		all_user_models.push_back("models/" + user_p_models[i] + ".mdl");
	for (uint i = 0; i < user_w_models.size(); ++i)
		all_user_models.push_back("models/" + user_w_models[i] + ".mdl");
	for (uint i = 0; i < user_player_models.size(); ++i)
		all_user_models.push_back("models/" + user_player_models[i] + ".mdl");

	// i'll be using this model to indicate replacement errors
	vector<string>::iterator it = find(user_prop_models.begin(), user_prop_models.end(), "not_precached");
	if (it != user_prop_models.end())
		user_prop_models.erase(it);

	for (list_hashmap::iterator it = monster_whitelists.begin(); it != monster_whitelists.end(); ++it)
	{
		for (uint i = 0; i < it->second.size(); ++i)
			if (find(all_user_models.begin(), all_user_models.end(), it->second[i]) == all_user_models.end())
				it->second.erase(it->second.begin() + i--); // model doesn't exist in user's models
	}
	for (list_hashmap::iterator it = monster_blacklists.begin(); it != monster_blacklists.end(); ++it)
	{
		for (uint i = 0; i < it->second.size(); ++i)
			if (find(all_user_models.begin(), all_user_models.end(), it->second[i]) == all_user_models.end())
				it->second.erase(it->second.begin() + i--); // model doesn't exist in user's models
	}

	if (false)
	{
		vector<string> some_user_models;
		for (uint i = 0; i < user_monster_models.size(); ++i)
			some_user_models.push_back("models/" + user_monster_models[i] + ".mdl");
		for (uint i = 0; i < user_prop_models.size(); ++i)
			some_user_models.push_back("models/" + user_prop_models[i] + ".mdl");
		for (uint i = 0; i < some_user_models.size(); ++i)
		{
			println(some_user_models[i]);
		}
		writeLog();
		return;
	}

	if (false)
	{
		println("#define NUM_APACHE_MODELS " + str(user_apache_models.size()));
		println("static const char * APACHE_MODELS[NUM_APACHE_MODELS] =");
		println("{");
		for (uint s = 0; s < user_apache_models.size(); s++)
			println("\t\"" + user_apache_models[s] + "\",");
		println("};\n");

		println("#define NUM_MODEL_V " + str(user_v_models.size()));
		println("static const char * MODEL_V[NUM_MODEL_V] =");
		println("{");
		for (uint s = 0; s < user_v_models.size(); s++)
			println("\t\"" + user_v_models[s] + "\",");
		println("};\n");

		println("#define NUM_MODEL_P " + str(user_p_models.size()));
		println("static const char * MODEL_P[NUM_MODEL_P] =");
		println("{");
		for (uint s = 0; s < user_p_models.size(); s++)
			println("\t\"" + user_p_models[s] + "\",");
		println("};\n");

		println("#define NUM_MODEL_W " + str(user_w_models.size()));
		println("static const char * MODEL_W[NUM_MODEL_W] =");
		println("{");
		for (uint s = 0; s < user_w_models.size(); s++)
			println("\t\"" + user_w_models[s] + "\",");
		println("};\n");

		println("#define NUM_MODEL_PROPS " + str(user_prop_models.size()));
		println("static const char * MODEL_PROPS[NUM_MODEL_PROPS] =");
		println("{");
		for (uint s = 0; s < user_prop_models.size(); s++)
			println("\t\"" + user_prop_models[s] + "\",");
		println("};\n");

		println("#define NUM_MODEL_MONSTERS " + str(user_monster_models.size()));
		println("static const char * MODEL_MONSTERS[NUM_MODEL_MONSTERS] =");
		println("{");
		for (uint s = 0; s < user_monster_models.size(); s++)
			println("\t\"" + user_monster_models[s] + "\",");
		println("};\n");

		println("#define NUM_MODEL_PLAYERS " + str(user_player_models.size()));
		println("static const char * MODEL_PLAYERS[NUM_MODEL_PLAYERS] =");
		println("{");
		for (uint s = 0; s < user_player_models.size(); s++)
			println("\t\"" + user_player_models[s] + "\",");
		println("};\n");

		writeLog(); 
	}

}

void find_all_sprites(string spritePath)
{
	user_sprites.clear();
	user_animated_sprites.clear();

	vector<string> dirs = getAllSubdirs(spritePath);

	for (uint i = 0; i < dirs.size(); i++)
	{
		vector<string> results = getDirFiles(dirs[i], "spr");
		string cpath = "";
		int dir_find = dirs[i].find("sprites/");
		if (dir_find != string::npos && dirs[i].length() > dir_find + string("sprites/").length())
			cpath = getSubStr(dirs[i], dir_find + string("sprites/").length()); // skip sprites/
		for (uint k = 0; k < results.size(); k++)
		{
			string name = getSubStr(results[k],0,results[k].length()-4);
			if (name.length() > 0)
			{
				ifstream fin (dirs[i] + results[k], ios::binary);

				int end_name = results[k].length() - 4; // skip .spr
				SPRHEADER sprHead;
				fin.read((char*)&sprHead, sizeof(SPRHEADER));
				fin.close();

				// Normal and Additive sprites have dimension restrictions (was news to me, too :>)
				if (sprHead.format < SPR_INDEXALPHA && (sprHead.width * sprHead.height) % 4)
				{
					if (printRejects)
						println("Normal/Additive sprite with pixels not divisible by 4: " + cpath + results[k]);
					continue; // Bad sprite! Will cause GL Upload error if we try to use it
				}

				if (modelSafety > 0 && sprHead.frames > 16) 
				{
					if (printRejects)
						println("More than 16 frames: " + cpath + results[k]);
					continue; // skip sprites with lots of frames to prevent MAX_GLTEXTURES error
				}

				if (sprHead.frames > 1)
					user_animated_sprites.push_back(cpath + results[k].substr(0, end_name));
				else
					user_sprites.push_back(cpath + results[k].substr(0, end_name));
			}				
		}
	}
}

void get_all_sprites()
{
	find_all_sprites("../valve/sprites/");
	vector<string> temp_sprites = user_sprites;
	vector<string> temp_animated_sprites = user_animated_sprites;
	find_all_sprites("sprites/");
	
	// combine half-life and sven-coop sprites
	insert_unique(temp_sprites, user_sprites);
	insert_unique(temp_animated_sprites, user_animated_sprites);

	// i'll be using this sprite to indicate replacement errors
	vector<string>::iterator it = find(user_sprites.begin(), user_sprites.end(), "tile");
	if (it != user_sprites.end())
		user_sprites.erase(it);

	filter_default_content(user_animated_sprites, ANIMATED_SPRITES, NUM_ANIMATED_SPRITES);
	filter_default_content(user_sprites, STATIC_SPRITES, NUM_STATIC_SPRITES);

	if (false)
	{
		println("#define NUM_ANIMATED_SPRITES " + str(user_animated_sprites.size()));
		println("static const char * ANIMATED_SPRITES[NUM_ANIMATED_SPRITES] =");
		println("{");
		for (uint s = 0; s < user_animated_sprites.size(); s++)
			println("\t\"" + user_animated_sprites[s] + "\",");
		println("};\n");

		println("#define NUM_STATIC_SPRITES " + str(user_sprites.size()));
		println("static const char * STATIC_SPRITES[NUM_STATIC_SPRITES] =");
		println("{");
		for (uint s = 0; s < user_sprites.size(); s++)
			println("\t\"" + user_sprites[s] + "\",");
		println("};\n");

		writeLog(); 
	}

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
					string& gmr_path, string_hashmap& ent_models, string original_map_name)
{
	total_models = map->header.lump[LUMP_MODELS].nLength / BSP_MODEL_BYTES;
	total_models += 1; // BSP map model
	//println("BSP Models: " + str(map_models));

	for (uint i = 0; i < NUM_DEFAULT_PRECACHE; ++i)
		ent_models[DEFAULT_PRECACHE[i]] = "default_precache_models";
	total_models += default_weapon_models.size()*2; // p_ and v_ models

	if (entMode == ENT_SUPER)
	{
		// lots of stuff becomes breakable
		for (uint i = 0; i < default_gib_models.size(); ++i)
			ent_models[default_gib_models[i]] = "func_breakable";				
	}

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
			else
				ent_models["sprites/flare6.spr"] = cname;
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
		if (cname.find("func_tank") == 0)
		{
			if (ents[i]->hasKey("spritesmoke"))
				ent_models[toLowerCase(ents[i]->keyvalues["spritesmoke"])] = cname;
			if (ents[i]->hasKey("spriteflash"))
				ent_models[toLowerCase(ents[i]->keyvalues["spriteflash"])] = cname;
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
		{
			if (ents[i]->keyvalues.find("gibmodel") != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues["gibmodel"])] = cname;
			else
			{
				int material = atoi(ents[i]->keyvalues["material"].c_str());
				if (material < 0 || material > 8) material = 1;
				if (material == 7) material = 0; // unbreakable glass precaches glass
				if (material == 8) material = 7; // since we skip unbreakable glass
				ent_models[default_gib_models[material]] = cname;
			}
				
			potential_additions++;
		}

		// decided not to replace door gib models because you don't usually break them
		// and levels tend to have a lot of doors

		if (cname.find("monster_") == 0 || cname.find("ammo_") != string::npos 
			|| cname.find("item_") != string::npos || cname.find("cycler") == 0
			|| matchStr(cname, "env_beverage") || matchStr(cname, "env_glow")
			|| matchStr(cname, "env_sprite") || matchStr(cname, "env_spritetrain") || matchStr(cname, "weaponbox"))
		{
			if (!matchStr(cname, "monster_cockroach") && !matchStr(cname, "monster_kingpin") && 
				!matchStr(cname, "monster_tentacle") && !matchStr(cname, "monster_nihilanth"))
				potential_additions++;
			
			string_hashmap * defaults_hashmap = &default_monster_models;
			bool is_friendly = ents[i]->keyvalues[ally_key] == "1";
			if (cname.find("item_") != string::npos)
				defaults_hashmap = &default_item_models;
			if (cname.find("monster_") == 0 && is_friendly)
				defaults_hashmap = &default_friendly_monster_models;

			if (ents[i]->keyvalues.find(custom_monster_model_key) != ents[i]->keyvalues.end())
				ent_models[toLowerCase(ents[i]->keyvalues[custom_monster_model_key])] = cname;
			else if (cname.find("monster_") != string::npos || cname.find("item_") != string::npos)
			{
				string m = (*defaults_hashmap)[getSubStr(cname, cname.find_first_of('_')+1)]; 
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
			else if (matchStr(cname, "env_beverage"))
				ent_models["models/can.mdl"] = cname;
		}
		else if (cname.find("weapon_") != string::npos)
		{
			if (ents[i]->keyvalues.find("wpn_v_model") != ents[i]->keyvalues.end())
				ents[i]->keyvalues["wpn_v_model"] = "";

			if (ents[i]->keyvalues.find("wpn_w_model") != ents[i]->keyvalues.end())
				ents[i]->keyvalues["wpn_w_model"] = "";

			if (ents[i]->keyvalues.find("wpn_p_model") != ents[i]->keyvalues.end())
				ents[i]->keyvalues["wpn_p_model"] = "";
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
	ifstream myfile (path + original_map_name + ".cfg");
	
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

string random_model_replace(string model, string_hashmap& ent_models)
{
	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY)
	{
		model = toLowerCase(model);
		if (every_random_replacement.find(model) != every_random_replacement.end())
			return every_random_replacement[model]; 
	}

	string replacement;
	bool go_again;
	if (model.find("sprites/") != string::npos)
	{
		do { replacement = "sprites/" + get_random_sprite(SPRITE_TYPE_GENERIC) + ".spr"; }
		while(replacement.find(" ") != string::npos);
	}
	else
	{
		int tries = 0;
		do
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
				replacement = "models/" + replacement + ".mdl";	
			}
			go_again = false;
			for (string_hashmap::iterator it = ent_models.begin(); it != ent_models.end(); ++it)
			{
				if (it->first.find(replacement) != string::npos)
				{
					go_again = true;
					break;
				}
			}
			if (tries++ > 100)
			{
				println("Failed to find safe model replacement for: " + model);
				return "";
			}
		} while(replacement.find(" ") != string::npos || go_again || !is_safe_model_replacement("", model, replacement));
	}
	
	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY && model.length())
		every_random_replacement[model] = replacement; 

	return replacement;
}

string get_random_replacement(string model, vector<string>& replaced, vector<string>& replace_models, string_hashmap& ent_models)
{
	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY)
	{
		model = toLowerCase(model);
		if (every_random_replacement.find(model) != every_random_replacement.end())
			return every_random_replacement[model]; 
	}

	string replacement;

	// ignoring special cases (nih is processed as ent model though)
	if (matchStr(model, "models/nihilanth.mdl") || matchStr(model, "models/controller.mdl") ||
		matchStr(model, "models/tentacle2.mdl") || matchStr(model, "models/nihilanth.mdl") ||
		matchStr(model, "models/kingpin.mdl") || model.find("apache") != string::npos ||
		matchStr(model, "models/sentry.mdl"))
		return ""; 

	if (model.find("sprites/") != string::npos)
	{
		do
		{
			replacement = random_model_replace(model, string_hashmap());
		} while(replacement.find_first_of(" ") != string::npos);
	}
	else
	{
		int tries = 0;
		bool go_again;
		do
		{
			replacement = random_model_replace(model, ent_models);	

			if (replacement.length() == 0)
				break;

			go_again = false;
			for (string_hashmap::iterator it = ent_models.begin(); it != ent_models.end(); ++it)
			{
				if (it->first.find(replacement) != string::npos)
				{
					go_again = true;
					break;
				}
			}

			if (tries++ > 100)
			{
				println("Failed to find safe model replacement for: " + model);
				break;
			}

			// using a replacement model that's been replaced in this file can cause Host Precache error
		} while (find(replaced.begin(), replaced.end(), replacement) != replaced.end() ||
				 find(replace_models.begin(), replace_models.end(), replacement) != replace_models.end() ||
				 go_again);
			
	}

	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY && model.length())
		every_random_replacement[model] = replacement; 

	return replacement;
}

string get_random_model_no_space_safe(int type, string replacing, string_hashmap ent_models)
{
	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY)
	{
		replacing = toLowerCase(replacing);
		if (every_random_replacement.find(replacing) != every_random_replacement.end())
			return every_random_replacement[replacing]; 
	}

	string replacement;
	bool go_again;
	int tries = 0;
	do 
	{ 
		replacement = get_random_model(type); 
		go_again = false;
		for (string_hashmap::iterator it = ent_models.begin(); it != ent_models.end(); ++it)
		{
			if (it->first.find(replacement) != string::npos)
			{
				go_again = true;
				break;
			}
		}
		if (tries++ > 100)
			break;
	}
	while(replacement.find(" ") != string::npos || go_again);

	if (tries > 100)
		println("Failed to find safe random model for: " + replacing);

	replacement = "models/" + replacement + ".mdl";

	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY)
		every_random_replacement[replacing] = replacement;

	return replacement;
}

set<string> writeGMR(string new_gmr_path, string old_gmr_path, string_hashmap& ent_models, int replace_level)
{
	vector<string> replace_models;
	vector<string> replaced;

	replace_models.insert(replace_models.end(), monster_sprites.begin(), monster_sprites.end());
	for (int i = 0; i < NUM_DEFAULT_PRECACHE; i++)
		replace_models.push_back(DEFAULT_PRECACHE[i]);
	if (replace_level == 2)
		replace_models.insert(replace_models.end(), default_gib_models.begin(), default_gib_models.end());

	for (string_hashmap::iterator it = default_weapon_models.begin(); it != default_weapon_models.end(); ++it)
	{
		string model = random_model_replace("models/v_" + it->second + ".mdl", ent_models);
				
		int findwep = model.find("v_");
		if (findwep == string::npos) model.find("p_");
		if (findwep == string::npos) model.find("w_");
		if (findwep != string::npos)
		{
			string dir = getSubStr(model, 0, findwep);
			string name = getSubStr(model, findwep+2); // strip .mdl and anything before name
			string v_wep = get_random_model_no_space_safe(MODEL_TYPE_V_WEAPON, "models/v_" + default_v_weapon_models[it->first] + ".mdl", ent_models);
			string p_wep = get_random_model_no_space_safe(MODEL_TYPE_P_WEAPON, "models/p_" + default_p_weapon_models[it->first] + ".mdl", ent_models);
			string w_wep = get_random_model_no_space_safe(MODEL_TYPE_W_WEAPON, "models/w_" + default_w_weapon_models[it->first] + ".mdl", ent_models);
			if (fileExists(dir + "v_" + name))
				v_wep = dir + "v_" + name;
			else
			{
				for (uint i = 0; i < user_v_models.size(); ++i)
					if ((user_v_models[i] + ".mdl").find("v_" + name) != string::npos)
						v_wep = "models/" + user_v_models[i] + ".mdl";
			}
			if (fileExists(dir + "p_" + name))
				p_wep = dir + "p_" + name;
			else
			{
				for (uint i = 0; i < user_p_models.size(); ++i)
					if ((user_p_models[i] + ".mdl").find("p_" + name) != string::npos)
						p_wep = "models/" + user_p_models[i] + ".mdl"; 
			}
			if (fileExists(dir + "w_" + name))
				w_wep = dir + "w_" + name;
			else
			{
				for (uint i = 0; i < user_w_models.size(); ++i)
					if ((user_w_models[i] + ".mdl").find("w_" + name) != string::npos)
						w_wep = "models/" + user_w_models[i] + ".mdl"; 
			}
				
			ent_models["models/v_" + default_v_weapon_models[it->first] + ".mdl"] = v_wep;
			ent_models["models/p_" + default_p_weapon_models[it->first] + ".mdl"] = p_wep;
			ent_models["models/w_" + default_w_weapon_models[it->first] + ".mdl"] = w_wep;
		}
		else
		{
			// use the same model for all views so it at least kind of makes sense
			ent_models["models/v_" + default_v_weapon_models[it->first] + ".mdl"] = model;
			ent_models["models/p_" + default_p_weapon_models[it->first] + ".mdl"] = model;
			ent_models["models/w_" + default_w_weapon_models[it->first] + ".mdl"] = model;
		}

	}

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
						string replacement = get_random_replacement(replaces, replaced, replace_models, ent_models);
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

	set<string> models_used_as_replacements;
	for (uint i = 0; i < replace_models.size(); i++)
	{
		if (matchStr(replace_models[i], "models/player.mdl"))
			continue; // replacing it causes a crash with certain weapons (looks for T model)
		string first = replace_models[i];
		if (replace_level == 0 && first.find("w_") != string::npos)
			continue; // don't take a chance to create more models
		if (find(dont_replace.begin(), dont_replace.end(), first) != dont_replace.end())
			continue;
		string second;
		second = get_random_replacement(first, replaced, replace_models, ent_models);
		if (!second.length())
			continue;
		replaced.push_back(first);
		models_used_as_replacements.insert(second);
		myfile << '\"' << first << "\" \"" << second << '\"' << '\n';
		if (second.find("models/zombie_nights/zombie.mdl") == 0)
			println("REPLACE_MODELS USED A BAWAN");
	}

	// entity replacements
	for (string_hashmap::iterator it = ent_models.begin(); it != ent_models.end(); ++it)
	{
		if (!it->second.length())
			continue;
		myfile << '\"' << it->first << "\" \"" << it->second << '\"' << '\n';
		replaced.push_back(it->first);
		models_used_as_replacements.insert(it->second);
		if (it->second.find("models/zombie_nights/zombie.mdl") == 0)
			println("ENT_MODELS USED A BAWAN");
	}
	myfile.close();

	if (replaced.size() > 255)
		print("TOO MANY MODELS: " + str(replaced.size()) + ". ");

	return models_used_as_replacements;
}

string replace_entity_model(Entity * ent, string model_key, int model_type, int& potential_additions)
{
	string model = "";
	string ally_key = "is_player_ally";
	string cname = toLowerCase(ent->keyvalues["classname"]);
	if (cname.find("squadmaker") != string::npos)
	{
		cname = toLowerCase(ent->keyvalues["monstertype"]);
		ally_key = "respawn_as_playerally";
	}
	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY && false)
	{
		if (ent->keyvalues.find(model_key) != ent->keyvalues.end() && ent->keyvalues[model_key].length())
			model = ent->keyvalues[model_key];
		else if (cname.find("monster_") == 0)
		{
			string short_cname = getSubStr(cname, string("monster_").length());
			if (default_monster_models.find(short_cname) != default_monster_models.end())
				model = default_monster_models[short_cname];
			if (ent->keyvalues.find(ally_key) != ent->keyvalues.end() && ent->keyvalues[ally_key].find("1") == 0)
				if (default_friendly_monster_models.find(short_cname) != default_friendly_monster_models.end())
					model = default_friendly_monster_models[short_cname];
			if (model.length())
				model = "models/" + model + ".mdl";
		}
		else if (cname.find("item_") == 0)
		{
			string short_cname = getSubStr(cname, string("item_").length());
			if (default_item_models.find(short_cname) != default_item_models.end())
				model = "models/" + default_item_models[short_cname] + ".mdl";
		}
		else if (cname.find("ammo_") == 0)
		{
			string short_cname = getSubStr(cname, string("ammo_").length());
			if (default_ammo_models.find(short_cname) != default_ammo_models.end())
				model = "models/" + default_ammo_models[short_cname] + ".mdl";
		}
		else if (cname.find("weaponbox") == 0)
		{
			model = "models/w_weaponbox.mdl";
		}
		else if (cname.find("func_breakable") == 0)
		{
			int material = atoi(ent->keyvalues["material"].c_str());
			if (material < 0 || material > 8) material = 1;
			if (material == 7) material = 0; // unbreakable glass precaches glass
			if (material == 8) material = 7; // since we skip unbreakable glass
			model = default_gib_models[material];
		}
		else if (cname.find("env_beverage") == 0)
		{
			model = "models/can.mdl";
		}
		if (model.length() && every_random_replacement.find(model) != every_random_replacement.end())
		{
			ent->keyvalues[model_key] = every_random_replacement[model];
			return ent->keyvalues[model_key];
		}
		else if (!model.length())
			println("No model for this? " + cname);
	}

	int tries = 0;
	do
	{
		if (mdlMode == MDL_NONE || !potential_additions)
			return false;
		if (mdlMode == MDL_TYPED)
			ent->keyvalues[model_key] = "models/" + get_random_model(model_type) + ".mdl";
		else
			ent->keyvalues[model_key] = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
		if (tries++ > 100)
		{
			println("Failed to find safe model replacement for " + cname);
			ent->keyvalues[model_key] = "";
			return "";
		}
	} while(!is_safe_model_replacement(cname, "", ent->keyvalues[model_key])); // run it through the blacklists
	potential_additions -= 1;
	
	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY && model.length())
		every_random_replacement[model] = ent->keyvalues[model_key];

	return ent->keyvalues[model_key];
}

string replace_entity_sprite(Entity * ent, string model_key, int sprite_type, int& potential_additions)
{
	string model = "";
	string cname = ent->keyvalues["classname"];
	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY)
	{
		if (ent->keyvalues.find(model_key) != ent->keyvalues.end() && ent->keyvalues[model_key].length())
			model = ent->keyvalues[model_key];
		if (model.length() && every_random_replacement.find(model) != every_random_replacement.end())
		{
			ent->keyvalues[model_key] = every_random_replacement[model];
			return ent->keyvalues[model_key];
		}
		else if (cname.find("env_funnel") == 0)
		{
			model = "sprites/flare6.spr";
		}
		else if (false && !model.length())
			println("No sprite for this? " + ent->keyvalues["classname"]);
	}

	if (mdlMode == MDL_NONE || !potential_additions)
		return false;
	if (mdlMode == MDL_TYPED)
		ent->keyvalues[model_key] = "sprites/" + get_random_sprite(sprite_type) + ".spr";
	else
		ent->keyvalues[model_key] = "sprites/" + get_random_sprite(SPRITE_TYPE_GENERIC) + ".spr";
	potential_additions -= 1;

	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY && model.length())
		every_random_replacement[model] = ent->keyvalues[model_key];

	return ent->keyvalues[model_key];
}

bool is_safe_model_replacement(string classname, string model, string replacement)
{
	if (monster_blacklists.find(classname) != monster_blacklists.end())
	{
		vector<string>& blacklist = monster_blacklists[classname];
		for (uint i = 0; i < blacklist.size(); ++i)
			if (matchStr(blacklist[i], replacement))
				return false;
	}
	if (classname.find("monster_") == 0)
	{
		if (monster_blacklists.find("monsters") != monster_blacklists.end())
		{
			vector<string>& blacklist = monster_blacklists["monsters"];
			for (uint i = 0; i < blacklist.size(); ++i)
				if (matchStr(blacklist[i], replacement))
					return false;
		}

		// game crashes if replacing monster_* model with w_ model
		// or just doesn't work if monster is spawned from squadmaker
		// p_ and v_ models get replaced in GMR so messes up custom displayname
		if (replacement.find("/w_") != string::npos ||
			replacement.find("/p_") != string::npos ||
			replacement.find("/v_") != string::npos)
				return false;
	}
	if (model.find("w_") != string::npos || model.find("p_") != string::npos || model.find("v_") != string::npos)
	{
		if (monster_blacklists.find("weapons") != monster_blacklists.end())
		{
			vector<string>& wep_blacklist = monster_blacklists["weapons"];
			for (uint i = 0; i < wep_blacklist.size(); ++i)
				if (matchStr(wep_blacklist[i], replacement))
					return false;
		}
	}
	if (monster_blacklists.find("anything") != monster_blacklists.end())
	{
		vector<string>& blacklist = monster_blacklists["anything"];
		for (uint i = 0; i < blacklist.size(); ++i)
			if (matchStr(blacklist[i], replacement))
				return false;
	}
	if (replacement.find("models/zombie_nights/zombie.mdl") == 0)
		println("HOW CAN YOU THINK THATS SAFE??");

	return true;
}

string get_random_model_safe(int type, string replacing, string_hashmap ent_models, string cname)
{
	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY)
	{
		replacing = toLowerCase(replacing);
		if (every_random_replacement.find(replacing) != every_random_replacement.end())
			return every_random_replacement[replacing]; 
	}

	string model;
	bool go_again = false;
	int tries = 0;
	do
	{
		model = "models/" + get_random_model(type) + ".mdl";

		// make sure the replacement model isn't in the list of models to be replaced.
		// this causes a no precache error for certain models (TODO: Which models? I know w_grenade and w_uzi do it)
		go_again = false;
		for (string_hashmap::iterator it = ent_models.begin(); it != ent_models.end(); ++it)
		{
			if (it->first.find(model) != string::npos)
			{
				go_again = true;
				break;
			}
		}
	} while ((go_again || !is_safe_model_replacement(cname, replacing, model) || model.find_first_of(" ") != string::npos) && tries++ < 100);
	if (tries >= 100)
		println("WARNING: Failed to find a random model that isn't in the list of models to be replaced");

	if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY)
		every_random_replacement[replacing] = model;

	return model;
}

string get_random_sprite_safe(int type, string replacing)
{
	if (modelSafety != MODEL_SAFETY_GLOBAL_ONLY)
		return "sprites/" + get_random_sprite(type) + ".spr";

	replacing = toLowerCase(replacing);
	if (every_random_replacement.find(replacing) != every_random_replacement.end())
		return every_random_replacement[replacing];
	string replacement = "sprites/" + get_random_sprite(type) + ".spr";
	every_random_replacement[replacing] = replacement;
	return replacement;
}

void do_model_replacement(BSP * map, Entity** ents, string path, string original_map_name)
{
	string old_gmr_path;
	string new_gmr_path = map->name + "_gmr.gsrand";
	string_hashmap ent_models;
	int total_models, potential_additions;
	count_map_models(map, ents, path, total_models, potential_additions, old_gmr_path, ent_models, original_map_name);

	int max_model_limit = MAX_MAP_MODELS - 4; // let's play it safe
	int replace_level = 2;
	if (total_models + potential_additions < max_model_limit)
	{
		replace_level = 2;
		if (gmr_only)
		{
			replace_level = 1;
			print(str(total_models + potential_additions) + " models (GMR). ");
		}
		else
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
		for (string_hashmap::iterator it = ent_models.begin(); it != ent_models.end(); ++it)
		{
			string cname = it->second;
			it->second = "";
			if (cname.find("env_shooter") == 0)
				it->second = get_random_model_safe(MODEL_TYPE_GENERIC, it->first, ent_models, cname);
			else if (cname.find("func_breakable") == 0)
				it->second = get_random_model_safe(MODEL_TYPE_GENERIC, it->first, ent_models, cname);
			else if (matchStr(cname, "env_beam"))
				it->second = get_random_sprite_safe(SPRITE_TYPE_GENERIC, it->first);
			else if (matchStr(cname, "env_funnel"))
				it->second = get_random_sprite_safe(SPRITE_TYPE_GENERIC, it->first);
			else if (matchStr(cname, "env_laser"))
				it->second = get_random_sprite_safe(SPRITE_TYPE_GENERIC, it->first);
			else if (cname.find("func_tank") == 0)
				it->second = get_random_sprite_safe(SPRITE_TYPE_GENERIC, it->first);
			else if (matchStr(cname, "item_recharge") || matchStr(cname, "item_healthcharger"))
				it->second = get_random_model_safe(MODEL_TYPE_GENERIC, it->first, ent_models, cname);
			else if (matchStr(cname, "env_sprite") || matchStr(cname, "cycler_sprite") || matchStr(cname, "env_glow"))
			{
				// low chance of sprite becoming a model
				int r = rand() % 5;
				if (!r) it->second = get_random_model_safe(MODEL_TYPE_GENERIC, it->first, ent_models, cname);
				else    it->second = get_random_sprite_safe(SPRITE_TYPE_GENERIC, it->first);
			}
			else if (matchStr(cname, "env_spritetrain"))
			{
				int r = rand() % 2;
				if (!r) it->second = get_random_model_safe(MODEL_TYPE_GENERIC, it->first, ent_models, cname);
				else    it->second = get_random_sprite_safe(SPRITE_TYPE_GENERIC, it->first);
			}
			else if (cname.find("monster_") == 0 || cname.find("ammo_") == 0 || cname.find("item_") == 0 
			    || cname.find("cycler") == 0 || cname.find("weapon_") == 0
				||  matchStr(cname, "env_beverage") || matchStr(cname, "weaponbox"))
			{
				if (matchStr(cname, "monster_kingpin") || matchStr(cname, "monster_tentacle"))
				{
					ent_models.erase(it--);
					continue; // replacing would cause a crash
				}
				else if (matchStr(cname, "monster_apache") || matchStr(cname, "monster_sentry"))
				{
					// crash if model has less than 2 bone controllers
					int tries = 0;
					do {
						if (user_apache_models.size())
							it->second = "models/" + user_apache_models[rand() % user_apache_models.size()] + ".mdl";
						else
							it->second = "models/" + string(APACHE_MODELS[rand() % NUM_APACHE_MODELS]) + ".mdl";
						if (tries++ > 100)
						{
							println("Failed to find safe model replacement for " + cname);
							it->second = "";
							break;
						}
					} while(!is_safe_model_replacement(cname, it->first, it->second));
				}
				else if (monster_whitelists.find(cname) != monster_whitelists.end())
				{
					vector<string> whitelist = monster_whitelists[cname];
					if (whitelist.size() > 0)
						it->second = whitelist[rand() % whitelist.size()];
					// otherwise don't replace since the user has none of the models on the whitelist
				}
				else
				{
					int mtype = cname.find("monster_") == 0 ? MODEL_TYPE_MONSTER : MODEL_TYPE_GENERIC;
					it->second = get_random_model_safe(mtype, it->first, ent_models, cname);
				}
				
				if (cname.find("monster_") == 0 && it->second.length() > 4)
				{
					string raw_model_name = it->second;
					raw_model_name = getSubStr(raw_model_name, 0, raw_model_name.length()-4); // strip .mdl
					raw_model_name = getSubStr(raw_model_name, raw_model_name.find_last_of("/\\")+1);
					if (raw_model_name.find("v_") == 0 || raw_model_name.find("p_") == 0)
						raw_model_name = getSubStr(raw_model_name, 2);

					bool is_player_model = it->second.find("player/") != string::npos || 
					                       it->second.find("barnacle") != string::npos;

					// find all monsters that will be using this replacement
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

						if (is_player_model) 
						{   
							// player model origins arn't at the feet, so we need to offset the monster to compensate
							int height = 36;
							if (default_monster_heights.find(cname) != default_monster_heights.end())
								height = default_monster_heights[cname] - 36;
							ents[i]->keyvalues["minhullsize"] = "-16 -16 -36";
							ents[i]->keyvalues["maxhullsize"] = "16 16 " + str(height);
						}

						// rename monster to "model_name monster_name"
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
			if (!it->second.length())
			{
				println("Couldn't replace model for " + cname);
				ent_models.erase(it--);
				continue;
			}
		}
	}
	if (replace_level > 1)
	{
		vector<string> replace_models;
		for (int i = 0; i < NUM_DEFAULT_PRECACHE; i++)
			replace_models.push_back(DEFAULT_PRECACHE[i]);
		replace_models.insert(replace_models.end(), default_gib_models.begin(), default_gib_models.end());

		ent_models.clear();

		string rand_roach, rand_nih;
		rand_roach = "models/roach.mdl";
		rand_nih = "models/nihilanth.mdl";
		if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY && 
			every_random_replacement.find(rand_nih) != every_random_replacement.end())
		{
			rand_nih = every_random_replacement[rand_nih];
		}
		else
		{
			int tries = 0;
			do 
			{
				if (monster_whitelists.find("monster_nihilanth") != monster_whitelists.end())
				{
					vector<string> whitelist = monster_whitelists["monster_nihilanth"];
					if (whitelist.size() == 0)
						break;
					rand_nih = whitelist[rand() % whitelist.size()];
				}
				else
					rand_nih = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
				if (tries++ > 100)
				{
					println("Failed to find safe model replacement for monster_nihilanth" );
					rand_nih = "";
					break;
				}
			} while(!is_safe_model_replacement("monster_nihilanth", "", rand_nih));
			if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY)
				every_random_replacement["models/nihilanth.mdl"] = rand_nih;
		}

		if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY && 
			every_random_replacement.find(rand_nih) != every_random_replacement.end())
		{
			rand_roach = every_random_replacement[rand_roach];
		}
		else
		{
			int tries = 0;
			do 
			{
				if (monster_whitelists.find("monster_cockroach") != monster_whitelists.end())
				{
					vector<string> whitelist = monster_whitelists["monster_cockroach"];
					if (whitelist.size() == 0)
						break;
					rand_roach = whitelist[rand() % whitelist.size()];
				}
				else
					rand_roach = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
				if (tries++ > 100)
				{
					println("Failed to find safe model replacement for monster_roach" );
					rand_roach = "";
					break;
				}
			} while(!is_safe_model_replacement("monster_cockroach", "", rand_roach));
			if (modelSafety == MODEL_SAFETY_GLOBAL_ONLY)
				every_random_replacement["models/roach.mdl"] = rand_roach;
		}


		for (int i = 0; i < MAX_MAP_ENTITIES; i++)
		{
			if (ents[i] == NULL)
				break;

			string cname = toLowerCase(ents[i]->keyvalues["classname"]);
			string new_model = "";

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
				new_model = replace_entity_model(ents[i], "gibmodel", MODEL_TYPE_GENERIC, potential_additions);
			else if (cname.find("env_shooter") != string::npos)
				new_model = replace_entity_model(ents[i], "shootmodel", MODEL_TYPE_GENERIC, potential_additions);
			else if (matchStr(cname, "env_beam"))
				new_model = replace_entity_sprite(ents[i], "texture", SPRITE_TYPE_GENERIC, potential_additions);
			else if (matchStr(cname, "env_funnel"))
				new_model = replace_entity_sprite(ents[i], "sprite", SPRITE_TYPE_GENERIC, potential_additions);
			else if (matchStr(cname, "env_laser"))
			{
				new_model = replace_entity_sprite(ents[i], "texture", SPRITE_TYPE_GENERIC, potential_additions);
				new_model = replace_entity_sprite(ents[i], "EndSprite", SPRITE_TYPE_GENERIC, potential_additions);
			}
			else if (cname.find("func_tank") == 0)
			{
				new_model = replace_entity_sprite(ents[i], "spritesmoke", SPRITE_TYPE_GENERIC, potential_additions);
				new_model = replace_entity_sprite(ents[i], "spriteflash", SPRITE_TYPE_GENERIC, potential_additions);
			}
			else if (matchStr(cname, "item_recharge") || matchStr(cname, "item_healthcharger"))
			{
				new_model = replace_entity_model(ents[i], "model_juice", MODEL_TYPE_GENERIC, potential_additions);
				continue;
			}
			else if (matchStr(cname, "env_sprite") || matchStr(cname, "cycler_sprite") || matchStr(cname, "env_glow"))
			{
				// low chance of sprite becoming a model
				int r = rand() % 5;
				if (!r) new_model = replace_entity_model(ents[i], "model", MODEL_TYPE_GENERIC, potential_additions);
				else    new_model = replace_entity_sprite(ents[i], "model", SPRITE_TYPE_GENERIC, potential_additions);
			}
			else if (matchStr(cname, "env_spritetrain"))
			{
				int r = rand() % 2;
				if (!r) new_model = replace_entity_model(ents[i], "model", MODEL_TYPE_GENERIC, potential_additions);
				else    new_model = replace_entity_sprite(ents[i], "model", SPRITE_TYPE_GENERIC, potential_additions);
			}
			else if (cname.find("monster_") == 0 || cname.find("ammo_") == 0 || cname.find("item_") == 0
			    || cname.find("cycler") == 0 ||  matchStr(cname, "env_beverage") || matchStr(cname, "weaponbox"))
			{
				if (matchStr(cname, "monster_kingpin") || matchStr(cname, "monster_tentacle"))
				{
					ents[i]->keyvalues.erase(custom_monster_model_key);
					continue; // replacing would cause a crash
				}
				else if (matchStr(cname, "monster_nihilanth") && rand_nih.length()) // GMR replace only
				{
					ents[i]->keyvalues[custom_monster_model_key] = rand_nih;
					ent_models["models/" + default_monster_models["nihilanth"] + ".mdl"] = rand_nih;
				}
				else if (matchStr(cname, "monster_cockroach") && rand_roach.length()) // GMR replace only
				{
					ents[i]->keyvalues[custom_monster_model_key] = rand_roach;
					ent_models["models/" + default_monster_models["cockroach"] + ".mdl"] = rand_roach;
				}
				else if (matchStr(cname, "monster_apache") || matchStr(cname, "monster_sentry"))
				{
					// crash if model has less than 2 bone controllers
					int tries = 0;
					do {
						if (user_apache_models.size())
						{
							int r = rand() % user_apache_models.size();
							ents[i]->keyvalues[custom_monster_model_key] = "models/" + user_apache_models[r] + ".mdl";
						}
						else
						{
							int r = rand() % NUM_APACHE_MODELS;
							ents[i]->keyvalues[custom_monster_model_key] = "models/" + string(APACHE_MODELS[r]) + ".mdl";
						}
						if (tries++ > 100)
						{
							println("Failed to find safe model replacement for " + cname);
							ents[i]->keyvalues[custom_monster_model_key] = "";
							break;
						}
					} while(!is_safe_model_replacement(cname, "", ents[i]->keyvalues[custom_monster_model_key]));
				}
				else if (monster_whitelists.find(cname) != monster_whitelists.end())
				{
					vector<string> whitelist = monster_whitelists[cname];
					if (whitelist.size() > 0)
						ents[i]->keyvalues[custom_monster_model_key] = whitelist[rand() % whitelist.size()];					
					// otherwise don't replace since the user has none of the models on the whitelist
				}
				else
				{
					int mtype = cname.find("monster_") == 0 ? MODEL_TYPE_MONSTER : MODEL_TYPE_GENERIC;
					new_model = replace_entity_model(ents[i], custom_monster_model_key, mtype, potential_additions);
				}

				if (cname.find("monster_") == 0 && ents[i]->keyvalues[custom_monster_model_key].length() > 4)
				{
					if (ents[i]->keyvalues[custom_monster_model_key].find("player/") != string::npos || 
						ents[i]->keyvalues[custom_monster_model_key].find("barnacle") != string::npos) 
					{   
						// player model origins arn't at the feet, so we need to offset the monster to compensate
						int height = 36;
						if (default_monster_heights.find(cname) != default_monster_heights.end())
							height = default_monster_heights[cname] - 36;
						ents[i]->keyvalues["minhullsize"] = "-16 -16 -36";
						ents[i]->keyvalues["maxhullsize"] = "16 16 " + str(height);
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
	
	res_list.insert("maps/" + new_gmr_path);
	
	set<string> model_replacements = writeGMR(path + new_gmr_path, old_gmr_path, ent_models, replace_level);
	
	// make super sure model replacements are precached
	// this will also make these models visible to the RES generator
	int i = 0;
	for (set<string>::iterator it = model_replacements.begin(); it != model_replacements.end(); )
	{
		res_list.insert(*it);
		if (false)
		{
			Entity * ent = add_new_entity(ents, "custom_precache");
			if (!ent) break;
			for (int k = 0; it != model_replacements.end() && k < 9; k++)
			{
				if (it->length() < 4)
				{
					it++; k--;
					continue;
				}
				string ext = getSubStr(*it, it->length()-3);
				if (!matchStr(ext, "mdl")) // don't precache sprites (not needed)
				{
					it++; k--;
					continue;
				}
				ent->addKeyvalue("model_" + str(k+1), *(it++));
			} 
		}
		else
			it++;
		i++;
	}
}