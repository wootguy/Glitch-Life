// alerter.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "gsrand.h"
#include <sys/stat.h>
#include <conio.h>

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
	init_black_lists();
	init_random_monster_models();

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
			return MODEL_MONSTERS[rand() % NUM_MODEL_MONSTERS]; 
	}
	else if (request_model_type == MODEL_TYPE_PROP)
		return MODEL_PROPS[rand() % NUM_MODEL_PROPS];
	else if (request_model_type == MODEL_TYPE_MONSTER)
		return MODEL_MONSTERS[rand() % NUM_MODEL_MONSTERS];
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

void getAllSounds()
{
	initMasterList();
	/*
	vector<string> allSounds;
	string soundPath = "D:/Steam/steamapps/common/Half-Life/svencoop/sound/";

	for (int i = 0; i < NUM_SND_DIRS; i++)
	{
		vector<string> snds;
		for (int e = 0; e < num_exts; e++)
		{
			vector<string> files = getDirFiles(soundPath + dirs[i] + '/',exts[e]);
			for (int f = 0; f < files.size(); f++)
				snds.push_back(dirs[i] + '/' + files[f]);			
		}
		misc[i] = &snds[0];
		miscSize[i] = snds.size();
	}
	*/
}

void getAllVoices()
{
	for (int i = 0; i < NUM_VOICE_DIRS; i++)
	{
		bool found = false;
		for (int k = 0; k < NUM_MASTER_DIRS; k++)
		{
			if (matchStr(voice_dirs[i], masterDirs[k]))
			{
				voice[i] = new string[masterSize[k]];
				for (int s = 0; s < masterSize[k]; s++)
					voice[i][s] = getSubStr(masterList[k][s], 0, masterList[k][s].length()-4);
				vsize[i] = masterSize[k];
				found = true;
				break;
			}
		}
		if (!found)
			println("couldn't find " + voice_dirs[i]);
	}
}

vector<sound> generateWritable(bool printInfo)
{
	vector<sound> writable;
	int group = rand() % NUM_SND_DIRS;
	writable.reserve(256);
	
	for (int i = 0; i < NUM_SQUEEK; i++) 
		writable.push_back(sound("squeek/" + squeek[i] + ".wav"));

	for (int i = 0; i < NUM_HORNET; i++) 
		writable.push_back(sound("hornet/" + hornet[i] + ".wav"));

	for (int i = 0; i < NUM_COMMON; i++)
		writable.push_back(sound("common/" + common[i] + ".wav"));

	for (int i = 0; i < NUM_PLAYER; i++)
		writable.push_back(sound("player/" + player[i] + ".wav"));

	for (int i = 0; i < NUM_WEAPONS; i++)
		writable.push_back(sound("weapons/" + weapons[i] + ".wav"));

	for (int i = 0; i < NUM_ITEMS; i++)
		writable.push_back(sound("items/" + items[i] + ".wav"));

	for (int i = 0; i < NUM_SPECIAL; i++)
		writable.push_back(sound(special[i]));

	for (int i = 0; i < NUM_MASTER_buttons; i++)
		writable.push_back(sound("buttons/" + MASTER_buttons[i]));

	//for (int i = 0; i < NUM_MASTER_plats; i++)
		//writable.push_back(sound("plats/" + MASTER_plats[i] + ".wav"));

	//for (int i = 0; i < NUM_MASTER_doors; i++)
		//writable.push_back(sound("doors/" + MASTER_doors[i] + ".wav"));

	for (int i = 0; i < NUM_MASTER_debris; i++)
		writable.push_back(sound("debris/" + MASTER_debris[i]));

	if (printInfo) println("Wrote global sounds: " + str(writable.size()));

	/*
	for (int i = 0; i < WEAPON_TYPES; i++)
	{
		if (weaps[i])
		{
			group = rand() % NUM_SND_DIRS;
			for (int k = 0; k < wsize[i]; k++)
				writable.push_back(sound("weapons/" + wlists[i][k] + ".wav", group));
		}
	}

	if (printInfo) println("Wrote weapon sounds: " + str(writable.size()));
	*/

	/*
	if (sparks)
	{
		group = rand() % NUM_SND_DIRS;
		for (int i = 0; i < NUM_SPARKS; i++)
			writable.push_back(sound("buttons/" + spark[i] + ".wav", group));
	}
		
	if (printInfo) println("Wrote spark sounds: " + str(writable.size()));
	*/
	/*
	for (int i = 0; i < NUM_FANS; i++)
	{
		if (ftypes[i])
			writable.push_back("fans/" + fans[i] + ".wav");
	}
	
	if (printInfo) println("Wrote fan sounds: " + str(writable.size()));
	*/

	/*
	for (int i = 0; i < NUM_BUTTONS; i++)
	{
		if (butts[i])
			writable.push_back(sound("buttons/" + buttons[i] + ".wav"));
	}

	for (int i = 0; i < NUM_ROT_BUTTONS; i++)
	{
		if (rbutts[i])
			writable.push_back(sound("buttons/" + rbuttons[i] + ".wav"));
	}

	if (printInfo) println("Wrote button/lock sounds: " + str(writable.size()));

	*/

	

	for (int i = 0; i < NUM_TRAIN_MOVES; i++)
	{
		//if (tmove[i])
			writable.push_back(sound("plats/" + trainmoves[i] + ".wav"));
	}
	for (int i = 0; i < NUM_TRAIN_STOPS; i++)
	{
		//if (tstop[i])
			writable.push_back(sound("plats/" + trainstops[i] + ".wav"));
	}
	
	/*
	for (int i = 0; i < NUM_TRACK_TRAINS; i++)
	{
		if (ttrain[i])
			writable.push_back(sound("plats/" + tracktrains[i] + ".wav"));
	}
	
	if (printInfo) println("Wrote train sounds: " + str(writable.size()));
	*/

	
	for (int i = 0; i < NUM_DOOR_MOVES; i++)
	{
		if (dmove[i])
			writable.push_back(sound("doors/" + doormoves[i] + ".wav"));
	}
	for (int i = 0; i < NUM_DOOR_STOPS; i++)
	{
		if (dstop[i])
			writable.push_back(sound("doors/" + doorstops[i] + ".wav"));
	}

	if (printInfo) println("Wrote door sounds: " + str(writable.size()));
	

	/*
	// breakables
	if (btypes[COMPUTER])
	{
		group = rand() % NUM_SND_DIRS;
		for (int k = 0; k < NUM_GLASS; k++)
			writable.push_back(sound("debris/" + blists[GLASS][k] + ".wav",group));
		for (int k = 0; k < NUM_METAL; k++)
			writable.push_back(sound("debris/" + blists[METAL][k] + ".wav",group));
		btypes[GLASS] = btypes[METAL] = false;
	}
	if (btypes[ROCK])
	{
		group = rand() % NUM_SND_DIRS;
		for (int k = 0; k < NUM_CONCRETE; k++)
			writable.push_back(sound("debris/" + blists[CONCRETE][k] + ".wav", group));
		btypes[CONCRETE] = false;
	}
	for (int i = 0; i < COMPUTER; i++)
	{
		if (btypes[i])
		{
			group = rand() % NUM_SND_DIRS;
			for (int k = 0; k < bsize[i]; k++)
				writable.push_back(sound("debris/" + blists[i][k] + ".wav",group));
		}
	}
	
	if (printInfo) println("Wrote material sounds: " + str(writable.size()));
	*/

	for (int i = 0; i < ambients.size(); i++)
		writable.push_back(sound(ambients[i]));
	
	if (printInfo) println("Wrote ambient sounds: " + str(writable.size()));

	if (writable.size() > MAX_REPLACEMENTS)
		numOverflow++;

	return writable;
}

void writeMonsterLists(string mapname)
{
	int monsterWrites = 0;
	for (int i = 0; i < MONSTER_TYPES; i++)
	{
		if (monsters[i] > 0)
		{
			int group = rand() % NUM_SND_DIRS;
			string filename = "maps/" + mapname + "_" + mdirs[i] + ".gsrand";
			ofstream myfile;
			myfile.open(filename);

			for (int k = 0; k < msize[i]; k++)
			{
				int randDir = group;
				int randSnd = rand() % masterSize[randDir];
				if (matchStr(masterList[randDir][randSnd], mlists[i][k]))
				{
					i--;
					continue;
				}
				myfile << '\"';
				if (i != ASLAVE)
					myfile << mdirs[i] << '/';
				myfile << mlists[i][k] << ".wav\" \"" << masterDirs[randDir] + '/' + masterList[randDir][randSnd] << '\"' << '\n';
			}
			myfile.close();

			//println("Wrote " + mdirs[i] + ": " + str(msize[i]));
			monsters[i] = 0; // so we don't pick it again
			monsterWrites++;
		}
	}
}

void writeGSR(string filename, vector<sound> writeList)
{
	ofstream myfile;
	myfile.open (filename);

	for (int i = 0; i < writeList.size(); i++)
	{
		int randDir = writeList[i].group;
		int randSnd = rand() % masterSize[randDir];
		if (matchStr(masterList[randDir][randSnd], writeList[i].filename))
		{
			i--;
			continue;
		}
		
		myfile << '\"' << writeList[i].filename << "\" \"" << masterDirs[randDir] + '/' + masterList[randDir][randSnd] << '\"' << '\n';
	}
	myfile.close();

	//println("wrote " + str(writeList.size()) + " sound replacements");
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

int get_weapon_id(string weapon_name)
{
	string lower = toLowerCase(weapon_name);

	if (lower.find("weapon_pipewrench") != string::npos) return WEP_PIPEWRENCH;
	if (lower.find("weapon_crowbar") != string::npos)    return WEP_CROWBAR;
	if (lower.find("weapon_tripmine") != string::npos)   return WEP_MINE;
	if (lower.find("weapon_357") != string::npos)        return WEP_357;
	if (lower.find("weapon_9mmar") != string::npos)      return WEP_9MMAR;
	if (lower.find("weapon_9mmhandgun") != string::npos) return WEP_9MMHANDGUN;
	if (lower.find("weapon_crossbow") != string::npos)   return WEP_CROSSBOW;
	if (lower.find("weapon_eagle") != string::npos)      return WEP_EAGLE;
	if (lower.find("weapon_gauss") != string::npos)      return WEP_GAUSS;
	if (lower.find("weapon_grapple") != string::npos)    return WEP_GRAPPLE;
	if (lower.find("weapon_grenade") != string::npos)    return WEP_GRENADE;
	if (lower.find("weapon_hornetgun") != string::npos)  return WEP_HORNET;
	if (lower.find("weapon_m16") != string::npos)        return WEP_M16;
	if (lower.find("weapon_m249") != string::npos)       return WEP_SAW;
	if (lower.find("weapon_medkit") != string::npos)     return WEP_MEDKIT;
	if (lower.find("weapon_minigun") != string::npos)    return WEP_MINIGUN;
	if (lower.find("weapon_rpg") != string::npos)        return WEP_RPG;
	if (lower.find("weapon_satchel") != string::npos)    return WEP_SATCHEL;
	if (lower.find("weapon_shotgun") != string::npos)    return WEP_SHOTGUN;
	if (lower.find("weapon_snark") != string::npos)      return WEP_SNARK;
	if (lower.find("weapon_sniperrifle") != string::npos)   return WEP_SNIPER;
	if (lower.find("weapon_sporelauncher") != string::npos) return WEP_SPORE;
	if (lower.find("weapon_uzi") != string::npos)			return WEP_UZI;
	if (lower.find("weapon_uziakimbo") != string::npos)     return WEP_UZIAKIMBO;
		
	return -1;
}

int updateCFG(string path, string mapname, bool addNotRemove)
{
	vector<string> text;
	string line;

	if (!addNotRemove && !fileExists(path + mapname + ".cfg"))
		return 1;

	ifstream myfile (path + mapname + ".cfg");
	bool empty = true;
	bool wasEdited = false;
	if (myfile.is_open())
	{
		while ( !myfile.eof() )
		{
			getline (myfile,line);
			if (addNotRemove) // we're probably getting ready to write sound replacements
			{ 
				int wep_id = get_weapon_id(line);
				if (wep_id != -1)
					weaps[wep_id] = true;
			}
			if (matchStr(line,"## GSRAND BEGIN"))
			{
				wasEdited = true;
				break;
			}
			if (empty && line.length() > 0)
				empty = false;
			text.push_back(line);
		}
	}
	myfile.close();

	if (!addNotRemove)
	{
		if (empty)
		{
			//println("removed " + path + mapname + ".cfg !");
			remove( string(path + mapname + ".cfg").c_str() );
			return 2;
		}
		if (!wasEdited)
			return 1;
	}

	ofstream fout;

	if (usePrefix && addNotRemove)
		mapname = "gsrand_" + mapname;

	fout.open (path + mapname + ".cfg", ios::out | ios::trunc);
	if (text.size() > 0)
	{
		for (int i = 0; i < text.size()-1; i++)
			fout << text[i] << endl;
		fout << text[text.size()-1];
	}
	
	if (addNotRemove)
	{
		fout << "\n## GSRAND BEGIN\n";
		if (sndMode != SND_NONE)
			fout << "globalsoundlist ../../maps/" << mapname << ".gsrand\n" <<
			"sentence_file maps/" << mapname << "_sentences.gsrand";
		if (mdlMode != MDL_NONE)
			fout << "globalmodellist ../../maps/" << mapname << "_gmr.gsrand\n";
	}
	fout.close();

	return 0;
	
	//println("Wrote to " + mapname + ".cfg");
}

Keyvalue extractKeyvalue(string line)
{
	int begin = -1;
	int end = -1;

	string key, value;
	key = "";
	value = "";
	int comment = 0;

	for (int i = 0; i < line.length(); i++)
	{
		if (line[i] == '/')
		{
			if (++comment >= 2)
			{
				key = value = "";
				break;
			}
		}
		else
			comment = 0;
		if (line[i] == '"')
		{
			if (begin == -1)
				begin = i + 1;
			else
			{
				end = i;
				if (key.length() == 0)
				{
					key = getSubStr(line,begin,end);
					begin = end = -1;
				}
				else
				{
					value = getSubStr(line,begin,end);
					break;
				}
			}
		}
	}
	return Keyvalue(key, value);
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

void ripent(BSP * map, Entity** entData, bool restore)
{
	if (restore)
	{
		loadLumpBackup(map, LUMP_ENTITIES, ".entbackup");
		loadLumpBackup(map, LUMP_TEXTURES, ".texbackup");
	}
	else
	{
		if (!usePrefix)
			saveLumpBackup(map, LUMP_ENTITIES, ".entbackup");			
	}

	int oldLen = map->header.lump[LUMP_ENTITIES].nLength;
	// calculate size of entity data
	if (!restore)
	{
		int eSize = 1; // 10
		for (int i = 0; i < MAX_MAP_ENTITIES; i++)
		{
			if (entData[i] == NULL)
				break;
			eSize += 4; // "{ \n } \n"
			
			hashmap hmap = entData[i]->keyvalues;
			for (hashmap::iterator it = hmap.begin(); it != hmap.end(); ++it)
				eSize += 6 + it->first.length() + it->second.length();
			
		}
		map->header.lump[LUMP_ENTITIES].nLength = eSize;
	}

	// calculate lump offsets for BSP data (needed even without edits)
	int offset = sizeof(BSPHEADER);
	int lumpOrder[HEADER_LUMPS];
	int lumpOffsets[HEADER_LUMPS];
	for (int loops = 0; loops < HEADER_LUMPS; loops++)
	{
		int minOffset = 0x7FFFFFFF;
		int minLump = -1;
		for (int i = 0; i < HEADER_LUMPS; i++)
		{
			if (map->header.lump[i].nOffset < minOffset)
			{
				minOffset = map->header.lump[i].nOffset;
				minLump = i;
			}
		}	
		if (minLump != -1)
		{
			lumpOffsets[loops] = offset;
			lumpOrder[loops] = minLump;
			offset += map->header.lump[minLump].nLength;
			map->header.lump[minLump].nOffset = 0x7FFFFFFF;
		}
		else
			println("UH OH. We're missing some lumps");
	}

	for (int i = 0; i < HEADER_LUMPS; i++)
		map->header.lump[lumpOrder[i]].nOffset = lumpOffsets[i];

	
	string filename = getWorkDir() + "maps/" + map->name + ".bsp";

	ofstream fout(filename, ios::out | ios::binary | ios::trunc);
	fout.write((char*)&map->header, sizeof(BSPHEADER));

	for (int i = 0; i < HEADER_LUMPS; i++)
	{
		int idx = lumpOrder[i];
		//println("Wrote Lump: " + str(idx) + " Size: " + str(map->header.lump[idx].nLength));
		if (!restore && idx == LUMP_ENTITIES)
		{
			string chunk = "";
			for (int k = 0; k < MAX_MAP_ENTITIES; k++)
			{
				if (entData[k] == NULL)
					break;
				chunk += "{\n";
				hashmap hmap = entData[k]->keyvalues;
				for (hashmap::iterator it = hmap.begin(); it != hmap.end(); ++it)
				{
					if (matchStr(it->first, "classname"))
						continue;
					chunk += "\"" + it->first + "\" \"" + it->second + "\"\n";
				}
				chunk += "\"classname\" \"" + hmap["classname"] + "\"\n";
				chunk += "}\n";
			}
			chunk += '\0';
			fout.write(chunk.c_str(), chunk.length());
		}
		else if (!restore && idx == LUMP_TEXTURES && texMode == TEX_MAP && map->texdata != NULL)
		{
			int numTex = map->texdata->numTex;
			fout.write((char*)&numTex, sizeof(int));
			fout.write((char*)map->texdata->offset, sizeof(int)*numTex);
				
			for (int i = 0; i < numTex; i++)
			{
				fout.write((char*)map->texdata->tex[i], sizeof(BSPMIPTEX));
				fout.write((char*)map->texdata->tex[i]->data, map->texdata->len[i]);
				delete [] map->texdata->tex[i]->data;
				delete map->texdata->tex[i];
			}
			delete [] map->texdata->tex;
			delete [] map->texdata->len;
			delete [] map->texdata->offset;
			delete map->texdata;
		}
		else
		{
			fout.write((char*)map->lumps[idx], map->header.lump[idx].nLength);
		}
	}

	fout.close();
}

bool needsRipent(BSP * map, Entity** ents)
{
	if (ents[0] == NULL)
	{
		println("No entities in map!");
		return false;
	}
	if (!matchStr(ents[0]->keyvalues["classname"], "worldspawn"))
	{
		println("ERROR: First entity is NOT worldspawn!");
		return false;
	}

	string newWadString = "";
	if (texMode == TEX_MASTERWAD)
		newWadString = "gsrand";
	else if (!usePrefix)
		newWadString = "gsrand_" + map->name;
	else 
		newWadString = map->name;

	newWadString += ".wad;zhlt.wad";

	if (texMode != TEX_MAP && texMode != TEX_NONE)
	{
		if (!matchStr(ents[0]->keyvalues["wad"], newWadString))
		{
			ents[0]->keyvalues["wad"] = newWadString;
			return true;
		}
	}

	return false;
}

Entity ** getMapEnts(BSP * map, bool printInfo, int& numEnts)
{
	Entity ** ents = new Entity*[MAX_MAP_ENTITIES];
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
		ents[i] = NULL;
	int idx = 0;


	membuf sbuf((char*)map->lumps[LUMP_ENTITIES], map->header.lump[LUMP_ENTITIES].nLength);
	istream in(&sbuf);

	int lineNum = 0;
	int lastBracket = -1;
	Entity* ent = NULL;

	string line = "";
	while (!in.eof())
	{
		lineNum++;
		getline (in,line);
		if (line.length() < 1 || line[0] == '\n')
			continue;

		if (line[0] == '{')
		{
			if (lastBracket == 0 && printInfo)
			{
				warn(str(lineNum) + ": Unexpected '{'");
				continue;
			}
			lastBracket = 0;

			if (printInfo)
				println(str(lineNum) + ": Beginning new entity");
			if (ent != NULL)
				delete ent;
			ent = new Entity();
		}
		else if (line[0] == '}')
		{
			if (lastBracket == 1 && printInfo)
				warn(str(lineNum) + ": Unexpected '}'");
			lastBracket = 1;

			if (ent == NULL)
				continue;

			if (printInfo)
				println(str(lineNum) + ": End of entity. " + str(ent->keyvalues.size()) + " keys. Class: " + ent->keyvalues["classname"]);
			//ents.push_back(ent);
			ents[idx++] = ent;
			ent = NULL;
		}
		else if (lastBracket == 0 && ent != NULL) // currently defining an entity
		{
			Keyvalue k = extractKeyvalue(line);
			if (k.key.length() == 0 || k.value.length() == 0)
			{
				// invalidate this entity
				if (printInfo) warn("invalid key.");
				//lastBracket = 1; 
				//ent = NULL;
			}
			else
			{
				ent->addKeyvalue(k);
				if (printInfo)
					println(str(lineNum) + ": set key '" + k.key + "' to '" + k.value + "'");
			}
		}
		else if (printInfo)
			println("unhandled line " + str(lineNum) + ": " + line);
	}	
	//println("got " + str(ents.size()) + " entities");
	if (ent != NULL)
		delete ent;

	if (entMode == ENT_SUPER)
	{
		ent = new Entity();
		ent->addKeyvalue("classname", "trigger_setcvar");
		ent->addKeyvalue("m_iszCVarToChange", "mp_respawndelay");
		ent->addKeyvalue("message", "1");
		ent->addKeyvalue("targetname", "gsrand_cvars");
		ents[idx++] = ent;

		ent = new Entity();
		ent->addKeyvalue("classname", "trigger_setcvar");
		ent->addKeyvalue("m_iszCVarToChange", "sv_maxspeed");
		ent->addKeyvalue("message", "350");
		ent->addKeyvalue("targetname", "gsrand_cvars");
		ents[idx++] = ent;

		ent = new Entity();
		ent->addKeyvalue("classname", "trigger_setcvar");
		ent->addKeyvalue("m_iszCVarToChange", "mp_banana");
		ent->addKeyvalue("message", "1");
		ent->addKeyvalue("targetname", "gsrand_cvars");
		ents[idx++] = ent;

		ent = new Entity();
		ent->addKeyvalue("classname", "trigger_setcvar");
		ent->addKeyvalue("m_iszCVarToChange", "mp_disablegaussjump");
		ent->addKeyvalue("message", "0");
		ent->addKeyvalue("targetname", "gsrand_cvars");
		ents[idx++] = ent;

		ent = new Entity();
		ent->addKeyvalue("classname", "trigger_setcvar");
		ent->addKeyvalue("m_iszCVarToChange", "mp_disable_autoclimb");
		ent->addKeyvalue("message", "0");
		ent->addKeyvalue("targetname", "gsrand_cvars");
		ents[idx++] = ent;

		ent = new Entity();
		ent->addKeyvalue("classname", "trigger_setcvar");
		ent->addKeyvalue("m_iszCVarToChange", "mp_npckill");
		ent->addKeyvalue("message", "1");
		ent->addKeyvalue("targetname", "gsrand_cvars");
		ents[idx++] = ent;

		ent = new Entity();
		ent->addKeyvalue("classname", "trigger_setcvar");
		ent->addKeyvalue("m_iszCVarToChange", "sk_plr_crowbar#");
		ent->addKeyvalue("message", "9001");
		ent->addKeyvalue("targetname", "gsrand_cvars");
		ents[idx++] = ent;

		ent = new Entity();
		ent->addKeyvalue("classname", "trigger_auto");
		ent->addKeyvalue("delay", "2");
		ent->addKeyvalue("triggerstate", "1");
		ent->addKeyvalue("target", "gsrand_cvars");
		ents[idx++] = ent;
	}

	numEnts = idx;
	return ents;
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

void tallyMonsters(Entity** ents, string mapname)
{
	ambients.clear();

	sparks = false;

	for (int i = 0; i < WEAPON_TYPES; i++)
		weaps[i] = false;

	for (int i = 0; i < NUM_FANS; i++)
		ftypes[i] = false;

	for (int i = 0; i < NUM_ROT_BUTTONS; i++)
		rbutts[i] = false;

	for (int i = 0; i < NUM_BUTTONS; i++)
		butts[i] = false;

	for (int i = 0; i < NUM_DOOR_STOPS; i++)
		dstop[i] = false;

	for (int i = 0; i < NUM_DOOR_MOVES; i++)
		dmove[i] = false;

	for (int i = 0; i < NUM_TRACK_TRAINS; i++)
		ttrain[i] = false;

	for (int i = 0; i < NUM_TRAIN_STOPS; i++)
		tstop[i] = false;

	for (int i = 0; i < NUM_TRAIN_MOVES; i++)
		tmove[i] = false;

	for (int i = 0; i < BREAKABLE_TYPES; i++)
		btypes[i] = false;

	string monsterModels[MONSTER_TYPES];
	
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (ents[i] == NULL) 
			break;
		string cname = ents[i]->keyvalues["classname"];

		// map-specific changes
		if (entMode != ENT_NONE)
			if (matchStr(cname, "trigger_once") && matchStr(ents[i]->keyvalues["master"], "alldead_ms"))
				ents[i]->keyvalues.erase("master"); // bypass "kill all monsters" bullshit
		if (matchStr(cname,"trigger_hurt") && matchStr(ents[i]->keyvalues["targetname"],"electro_hurt")) 
			ents[i]->keyvalues["spawnflags"] = str( atoi(ents[i]->keyvalues["spawnflags"].c_str()) | 32 ); // c2a1 crash
		if (matchStr(cname, "func_door_rotating") && matchStr(ents[i]->keyvalues["targetname"],"c3a2d_petals"))
			ents[i]->keyvalues["classname"] = cname = "func_breakable"; // anything these doors do crashes (except tiny moves)

		if (matchStr(cname, "ambient_generic"))
		{
			string snd = ents[i]->keyvalues["message"];
			bool repeat = false;
			for (int k = 0; k < ambients.size(); k++)
			{
				if (matchStr(snd,ambients[k]))
				{
					repeat = true;
					break;
				}
			}
			if (repeat)
				continue;
			ambients.push_back(snd);
			continue;
		}

		if (matchStr(cname,"func_breakable"))
		{
			string matstr = ents[i]->keyvalues["material"];
			int material = atoi(matstr.c_str());
			if (material >= 0 && material < BREAKABLE_TYPES)
				btypes[material] = true;
			if (entMode == ENT_SUPER)
			{
				int randWeap = rand() % 3;
				if (randWeap == 0)		randWeap = 1;
				else if (randWeap == 1)	randWeap = 19;
				else					randWeap = 20;
				ents[i]->keyvalues["instantbreak"] = "1";
				ents[i]->keyvalues["weapon"] = to_string((_Longlong)randWeap);
				// disable "only trigger" and "immune to clients" breakables (can stop map progression)
				ents[i]->keyvalues["spawnflags"] = str((atoi(ents[i]->keyvalues["spawnflags"].c_str()) & ~65) | 32);
			}
			continue;
		}

		if (matchStr(cname,"path_corner") || matchStr(cname,"path_track"))
		{
			if (entMode == ENT_SUPER)
			{
				int randSpeed = pow(2.0,(rand()%5+7));
				int randSpeed2 = pow(2.0,rand()%14);
				ents[i]->keyvalues["speed"] = to_string((_Longlong)randSpeed);
				//ents[i]->keyvalues["yaw_speed"] = to_string((_Longlong)randSpeed);
			}
		}

		if (matchStr(cname,"func_train") || matchStr(cname,"func_tracktrain"))
		{
			if (entMode == ENT_SUPER)
			{
				int randSpeed = pow(2.0,(rand()%5+7));
				int randRot1 = pow(2.0,rand()%10);
				int randRot2 = pow(2.0,rand()%10);
				int randRot3 = pow(2.0,rand()%10);
				ents[i]->keyvalues["speed"] = to_string((_Longlong)randSpeed);
			}
		}

		if (matchStr(cname,"func_train"))
		{
			string movestr = ents[i]->keyvalues["movesnd"];
			int movesnd = atoi(movestr.c_str()) - 1;
			if (movesnd >= 0 && movesnd < NUM_TRAIN_MOVES)
				tmove[movesnd] = true;

			string stopstr = ents[i]->keyvalues["stopsnd"];
			int stopsnd = atoi(stopstr.c_str()) - 1;
			if (stopsnd >= 0 && stopsnd < NUM_TRAIN_STOPS)
				tstop[stopsnd] = true;

			continue;
		}

		if (matchStr(cname,"func_wall"))
		{
			if (entMode == ENT_SUPER)
			{
				int randMat = rand() % 8;
				if (randMat == 7)
					randMat = 8;
				ents[i]->keyvalues["classname"] = "func_breakable";
				ents[i]->keyvalues["material"] = to_string((_Longlong)randMat);
				ents[i]->keyvalues["health"] = "100";
				ents[i]->keyvalues["displayname"] = "derp i'm a berk";

				int flags = 40; // repariable, show HUD info
				ents[i]->keyvalues["spawnflags"] = to_string((_Longlong)flags);
			}
		} 

		if (matchStr(cname,"player_loadsaved"))
		{
			if (entMode == ENT_SUPER)
				ents[i]->keyvalues["targetname"] = "NO RESTARTS ALLOWED YO";
		}

		if (matchStr(cname,"trigger_changelevel"))
		{
			if (usePrefix)
				ents[i]->keyvalues["map"] = "gsrand_" + ents[i]->keyvalues["map"];
			if (entMode == ENT_SUPER) 
			{
				if (matchStr(mapname,ents[i]->keyvalues["map"])) // no restarts!
					ents[i]->keyvalues["map"] = "Sanic Hodgepodge";
			}
		}

		if (matchStr(cname,"func_pushable"))
		{
			if (entMode == ENT_SUPER)
			{
				int friction = 1;
				ents[i]->keyvalues["friction"] = to_string((_Longlong)friction);
			}
		}

		if (matchStr(cname,"func_platrot"))
		{
			if (entMode == ENT_SUPER)
			{
				int rotation = pow(2.0,rand()%7+7);
				ents[i]->keyvalues["rotation"] = to_string((_Longlong)rotation);
			}
		}

		if (matchStr(cname,"func_conveyor") || matchStr(cname,"func_plat") || matchStr(cname,"func_platrot"))
		{
			if (entMode == ENT_SUPER)
			{
				int randSpeed = pow(2.0,rand()%5+7);
				ents[i]->keyvalues["speed"] = to_string((_Longlong)randSpeed);
			}
		}

		if (matchStr(cname,"func_water"))
		{
			if (entMode == ENT_SUPER)
			{
				int randHeight = pow(2.0,rand()%6);
				ents[i]->keyvalues["WaveHeight"] = to_string((_Longlong)randHeight);
			}
		}

		if (matchStr(cname,"func_rotating") || matchStr(cname,"func_rot_button"))
		{
			if (entMode == ENT_SUPER)
			{
				int randSpeed = pow(2.0,rand()%8+4);
				ents[i]->keyvalues["speed"] = to_string((_Longlong)randSpeed);

				int flags = atoi(ents[i]->keyvalues["spawnflags"].c_str());
				flags &= ~4;
				flags &= ~8;
				flags += (rand()%2)*4 + (rand()%2)*8;
				ents[i]->keyvalues["spawnflags"] = to_string((_Longlong)flags);
			}
		}

		if (matchStr(cname,"momentary_rot_button"))
		{
			if (entMode == ENT_SUPER)
			{
				int randSpeed = pow(2.0,rand()%6+4);
				ents[i]->keyvalues["speed"] = to_string((_Longlong)randSpeed);

				int flags = atoi(ents[i]->keyvalues["spawnflags"].c_str());
				flags &= ~64;
				flags &= ~128;
				flags += (rand()%2)*64 + (rand()%2)*128;
				ents[i]->keyvalues["spawnflags"] = to_string((_Longlong)flags);
			}
		}

		if (matchStr(cname,"func_door_rotating"))
		{
			if (entMode == ENT_SUPER)
			{
				int flags = atoi(ents[i]->keyvalues["spawnflags"].c_str());
				flags &= ~64;
				flags &= ~128;
				flags &= ~2;
				flags += (rand()%2)*64 + (rand()%2)*128 + (rand()%2)*2;
				ents[i]->keyvalues["spawnflags"] = to_string((_Longlong)flags);

				int randDist = 360*(rand()%10) + (int)atof(ents[i]->keyvalues["distance"].c_str());
				ents[i]->keyvalues["distance"] = to_string((_Longlong)randDist);
			}
		}

		if (matchStr(cname,"func_door"))
		{
			if (entMode == ENT_SUPER)
			{
				int randLip = -pow(2.0,rand()%5+6);
				string oldAngle = ents[i]->keyvalues["angle"];
				if (atoi(oldAngle.c_str()) >= 0) // breaking elevators can stop progression
				{
					int dir = rand() % 2;
					string angle;
					if (dir == 0)
						angle = to_string((_Longlong)(rand()%360));
					else if (dir == 1)
						angle = to_string((_Longlong)(rand()%2)-2);
					ents[i]->keyvalues["angle"] = angle;
					if (!matchStr(oldAngle, angle))
						ents[i]->keyvalues["lip"] = to_string((_Longlong)randLip);
				}
			}
		}

		if (matchStr(cname,"func_door") || matchStr(cname,"func_door_rotating") || matchStr(cname,"momentary_door"))
		{
			if (matchStr(mapname, "c4a1a")) // there's so many doors it causes a crash somehow
				continue;
			string movestr = ents[i]->keyvalues["movesnd"];
			int movesnd = atoi(movestr.c_str()) - 1;
			if (movesnd >= 0 && movesnd < NUM_DOOR_MOVES)
				dmove[movesnd] = true;

			string stopstr = ents[i]->keyvalues["stopsnd"];
			int stopsnd = atoi(stopstr.c_str()) - 1;
			if (stopsnd >= 0 && stopsnd < NUM_DOOR_STOPS)
				dstop[stopsnd] = true;

			string lockstr = ents[i]->keyvalues["locked_sound"];
			int locksnd = atoi(lockstr.c_str()) - 1;
			if (locksnd >= 0 && locksnd < NUM_BUTTONS)
				butts[locksnd] = true;

			string unlockstr = ents[i]->keyvalues["unlocked_sound"];
			int unlocksnd = atoi(unlockstr.c_str()) - 1;
			if (unlocksnd >= 0 && unlocksnd < NUM_BUTTONS)
				butts[unlocksnd] = true;

			if (entMode == ENT_SUPER)
			{
				int randSpeed = pow(2.0,(rand()%8+4));
				int randMat = rand() % 8;
				if (randMat == 7)
					randMat = 8;
				ents[i]->keyvalues["speed"] = to_string((_Longlong)randSpeed);
				ents[i]->keyvalues["breakable"] = "1";
				ents[i]->keyvalues["material"] = to_string((_Longlong)randMat);
				ents[i]->keyvalues["dmg"] = "9999";
				ents[i]->keyvalues["health"] = "100";
				ents[i]->keyvalues["displayname"] = "possible murdoor";
				ents[i]->keyvalues["showhudinfo"] = "1";
				ents[i]->keyvalues["repairable"] = "1";
			}
			continue;
		}

		if (matchStr(cname,"func_rot_button"))
		{
			string pushstr = ents[i]->keyvalues["sounds"];
			int pushsnd = atoi(pushstr.c_str()) - 22;
			if (pushsnd >= 0 && pushsnd < NUM_ROT_BUTTONS)
				rbutts[pushsnd] = true;
			continue;
		}

		if (matchStr(cname,"func_button"))
		{
			string pushstr = ents[i]->keyvalues["sounds"];
			int pushsnd = atoi(pushstr.c_str()) - 1;
			if (pushsnd >= 0 && pushsnd < NUM_BUTTONS)
				butts[pushsnd-1] = true;

			string lockstr = ents[i]->keyvalues["locked_sound"];
			int locksnd = atoi(lockstr.c_str()) - 1;
			if (locksnd >= 0 && locksnd < NUM_BUTTONS)
				butts[locksnd] = true;

			string unlockstr = ents[i]->keyvalues["unlocked_sound"];
			int unlocksnd = atoi(unlockstr.c_str()) - 1;
			if (unlocksnd >= 0 && unlocksnd < NUM_BUTTONS)
				butts[unlocksnd] = true;

			string flagsStr = ents[i]->keyvalues["spawnflags"];
			int flags = atoi(flagsStr.c_str()) - 1;
			if (flags & 64)
				sparks = true;

			if (entMode == ENT_SUPER)
			{
				if (atoi(ents[i]->keyvalues["wait"].c_str()) >= 0)
					ents[i]->keyvalues["wait"] = "1";
			}

			continue;
		}

		if (matchStr(cname, "env_spark"))
			sparks = true;

		
		/* TODO: unable to replace?

		if (matchStr(cname,"func_rotating"))
		{
		string movestr = ents[i]->keyvalues["sounds"];
		int movesnd = atoi(movestr.c_str()) - 22;
		if (movesnd >= 0 && movesnd < NUM_FANS)
		ftypes[movesnd] = true;
		continue;
		}

		if (matchStr(cname,"func_tracktrain"))
		{
			string movestr = ents[i]->keyvalues["sounds"];
			int movesnd = atoi(movestr.c_str());
			if (movesnd > 0)
				ttrain[movesnd-1] = true;
			continue;
		}
		*/
		/*
		if (matchStr(cname, "weaponbox") || matchStr(cname, "game_player_equip"))
		{
			for (int i = 0; i < WEAPON_TYPES; ++i)
				if (ents[i]->keyvalues[get_weapon_name(i)] && .length() > 0)	
					weaps[i] = true;
		}
		*/
		string child = "";

		if (matchStr(cname, "monstermaker"))
		{
			if (entMode == ENT_SUPER)
				ents[i]->keyvalues["classname"] = cname = "squadmaker"; // just make a squadmaker so we have more options
		}

		if (matchStr(cname, "squadmaker"))
		{
			if (entMode == ENT_SUPER)
			{
				if (rand() % 3) ents[i]->keyvalues["change_rendermode"] = "1";
				ents[i]->keyvalues["notsolid"] = "1";
			}
			child = ents[i]->keyvalues["monstertype"];
		}		

		int wep_id = get_weapon_id(cname);
		if (wep_id == -1)
			wep_id = get_weapon_id(child);
		if (wep_id != -1)
			weaps[wep_id] = wep_id;

		if (child.length() > 0)
			cname = child;

		if (cname.length() < 9) continue; // too short for MONSTER_X

		string prefix = getSubStr(cname,0,8);

		if (matchStr(prefix,"monster_"))
		{
			string mtype = getSubStr(cname,8);

			int matchIdx = -1;

			if (matchStr(mtype,"alien_babyvoltigore"))		matchIdx = VOLTIGORE;
			else if (matchStr(mtype,"alien_controller"))	matchIdx = CONTROLLER;
			else if (matchStr(mtype,"alien_grunt"))			matchIdx = AGRUNT;
			else if (matchStr(mtype,"alien_slave"))			matchIdx = ASLAVE;
			else if (matchStr(mtype,"alien_tor"))			matchIdx = TOR;
			else if (matchStr(mtype,"alien_voltigore"))		matchIdx = VOLTIGORE;
			else if (matchStr(mtype,"apache"))				matchIdx = APACHE;
			// else if (matchStr(mtype,"assassin_repel"))	matchIdx = GRUNT;
			else if (matchStr(mtype,"babycrab"))			matchIdx = HEADCRAB;
			else if (matchStr(mtype,"babygarg"))			matchIdx = BABYGARG;
			else if (matchStr(mtype,"barnacle"))			matchIdx = BARNACLE;
			else if (matchStr(mtype,"barney"))				matchIdx = BARNEY;
			else if (matchStr(mtype,"bigmomma"))			matchIdx = GONARCH;
			else if (matchStr(mtype,"blkop_osprey"))		matchIdx = APACHE;
			// else if (matchStr(mtype,"bloater"))			matchIdx = BLOATER;
			//else if (matchStr(mtype,"bodyguard"))			matchIdx = BODYGUARD;
			else if (matchStr(mtype,"bullchicken"))			matchIdx = BULLCHICKEN;
			//else if (matchStr(mtype,"cleansuit_scientist")) matchIdx = SCIENTIST;
			else if (matchStr(mtype,"cockroach"))			matchIdx = ROACH;
			// else if (matchStr(mtype,"flyer_flock"))		matchIdx = ???;
			// else if (matchStr(mtype,"furniture"))		matchIdx = ???;
			else if (matchStr(mtype,"gargantua"))			matchIdx = GARG;
			// else if (matchStr(mtype,"generic"))			matchIdx = ???;
			// else if (matchStr(mtype,"gman"))				matchIdx = GMAN;
			else if (matchStr(mtype,"gonome"))				matchIdx = GONOME;
			//else if (matchStr(mtype,"grunt_ally_repel"))	matchIdx = GRUNT;
			//else if (matchStr(mtype,"GRUNT_REPEL"))			matchIdx = GRUNT;
			// else if (matchStr(mtype,"handgrenade"))		matchIdx = ???;
			else if (matchStr(mtype,"headcrab"))			matchIdx = HEADCRAB;
			else if (matchStr(mtype,"houndeye"))			matchIdx = HOUNDEYE;
			// else if (matchStr(mtype,"human_assassin"))	matchIdx = GRUNT;
			//else if (matchStr(mtype,"human_grunt"))			matchIdx = GRUNT;
			//else if (matchStr(mtype,"human_grunt_ally"))	matchIdx = GRUNT;
			//else if (matchStr(mtype,"human_medic_ally"))	matchIdx = GRUNT;
			//else if (matchStr(mtype,"human_torch_ally"))	matchIdx = GRUNT;
			// else if (matchStr(mtype,"human_hwgrunt"))	matchIdx = HASSAULT;
			else if (matchStr(mtype,"ichthyosaur"))			matchIdx = ICHY;
			else if (matchStr(mtype,"kingpin"))				matchIdx = KINGPIN;
			else if (matchStr(mtype,"leech"))				matchIdx = LEECH;
			// else if (matchStr(mtype,"male_assassin"))	matchIdx = GRUNT;
			//else if (matchStr(mtype,"medic_ally_repel"))	matchIdx = GRUNT;
			else if (matchStr(mtype,"nihilanth"))			matchIdx = NIH;
			else if (matchStr(mtype,"osprey"))				matchIdx = APACHE;
			else if (matchStr(mtype,"otis"))				matchIdx = BARNEY;
			else if (matchStr(mtype,"pitdrone"))			matchIdx = PITDRONE;
			// else if (matchStr(mtype,"rat"))				matchIdx = ???;
			//else if (matchStr(mtype,"robogrunt"))			matchIdx = ROBOGRUNT;
			//else if (matchStr(mtype,"robogrunt_repel"))		matchIdx = ROBOGRUNT;
			// else if (matchStr(mtype,"satchel"))			matchIdx = ???;
			else if (matchStr(mtype,"scientist"))			matchIdx = SCIENTIST;
			//else if (matchStr(mtype,"sentry"))				matchIdx = TURRET;
			else if (matchStr(mtype,"shocktrooper"))		matchIdx = SHOCKTROOPER;
			// else if (matchStr(mtype,"snark"))			matchIdx = SNARK;
			// else if (matchStr(mtype,"sqnest"))			matchIdx = SNARK;
			// else if (matchStr(mtype,"stukabat"))			matchIdx = STUKABAT;
			// else if (matchStr(mtype,"tentacle"))			matchIdx = TENTACLE; // TODO: has sounds?
			//else if (matchStr(mtype,"torch_ally_repel"))	matchIdx = GRUNT;
			// else if (matchStr(mtype,"tripmine"))			matchIdx = ???;
			//else if (matchStr(mtype,"turret"))			matchIdx = TURRET;
			//else if (matchStr(mtype,"miniturret"))		matchIdx = TURRET;
			else if (matchStr(mtype,"zombie"))				matchIdx = ZOMBIE;
			else if (matchStr(mtype,"zombie_barney"))		matchIdx = ZOMBIE;
			else if (matchStr(mtype,"zombie_soldier"))		matchIdx = ZOMBIE;

			if (matchIdx >= 0)
			{
				monsters[matchIdx]++;
				ents[i]->keyvalues["soundlist"] = "../../maps/" + mapname + "_" + mdirs[matchIdx] + ".gsrand";
			}

			if (entMode == ENT_SUPER || entMode == ENT_APPEAR)
			{
				// random render effects
				int fxint = rand()%6;
				if (fxint == 0)
					fxint = 1;
				else if (fxint == 1)
					fxint = 2;
				else if (fxint == 2)
					fxint = 15;
				else if (fxint == 3)
					fxint = 16;
				else if (fxint == 4)
					fxint = 18;
				else if (fxint == 5)
					fxint = 19;
				ents[i]->keyvalues["renderfx"] = to_string((_Longlong)fxint);
				ents[i]->keyvalues["renderamt"] = to_string((_Longlong)(rand()%2)*155 + 100);
				ents[i]->keyvalues["rendercolor"] = to_string((_Longlong)rand()%256) + " " + 
					to_string((_Longlong)rand()%256) + " " + 
					to_string((_Longlong)rand()%256);

				int rstyle = rand() % 3;
				if (rstyle == 0)
				{
					ents[i]->keyvalues["rendermode"] = "0";
					ents[i]->keyvalues["renderfx"] = "19";
					fxint = 19;
				}
				else if (rstyle == 1)
					ents[i]->keyvalues["rendermode"] = "2";
				else
					ents[i]->keyvalues["rendermode"] = "5";

				if (fxint == 19)
					ents[i]->keyvalues["renderamt"] = to_string((_Longlong)(rand()%2)*254 + 1);

				// 1/3 chance of rotating monster
				if (rand() % 3 == 0)
				{
					int rot1 = rand() % 180 - 90;
					int rot2 = rand() % 180 - 90;
					int dir = rand() % 4;
					string angles = "0 0 0";
					if (dir == 0)
						angles = to_string((_Longlong)rot1) + " 0 0";
					else if (dir == 1)
						angles = to_string((_Longlong)rot1) + " 0 " + to_string((_Longlong)rot2);
					else if (dir == 2)
						angles = "0 0 " + to_string((_Longlong)rot1);
					ents[i]->keyvalues["angles"] = angles;
				}

				// 1/5 chance of making monster an ally
				if (rand() % 5 == 0 && matchIdx != SCIENTIST && entMode == ENT_SUPER)
				{
					int ally = 0;
					if (matchStr(ents[i]->keyvalues["classname"],"squadmaker") || 
						matchStr(ents[i]->keyvalues["classname"],"monstermaker"))
						ents[i]->keyvalues["respawn_as_playerally"] = "1";
					else
						ents[i]->keyvalues["is_player_ally"] = "1";
				}
			}

		}
	}
}

string constructSentence()
{
	int randVoice = rand() % NUM_VOICE_DIRS;
	string sentence = voice_dirs[randVoice] + '/';
		
	for (int i = 0, words = (rand() % 5) + 1; i < words; i++)
	{
		if (vsize[randVoice] > 0)
		{
			int r = rand() % vsize[randVoice];
			sentence += voice[randVoice][r] + ' ';
		}
	}
	return sentence;
}

void writeSentences(string mapName)
{
	string file2 = getWorkDir() + "maps/" + mapName + "_sentences.gsrand";
	ofstream fout;
	fout.open (file2, ios::out | ios::trunc);
	for (int i = 0; i < NUM_SENTENCES; i++)
		fout << sentences[i] << " " << constructSentence() << endl;

	fout.close();

	//println("Wrote " + mapName + "_sentences.gsrand");
}

#include <set>

// no slashes allowed at the end of start_dir
string relative_path_to_absolute(string start_dir, string path)
{
	int up = path.find("../");
	if (up == string::npos)
		return start_dir + "/" + path;
	
	while (up != string::npos)
	{
		int up_dir = start_dir.find_last_of("\\/");
		if (up_dir == string::npos)
		{
			print("Could not convert '" + path + "' to absolute path using root dir: " + start_dir); 
			return start_dir + "/" + path;
		}
		if (up > 0) // some crazy person went back down a directory before going up
		{
			start_dir += getSubStr(path, 0, up-1);
			path = getSubStr(path, up);
			up = 0;
		}
		start_dir = getSubStr(start_dir, 0, up_dir);
		path = getSubStr(path, 3);
		up = path.find("../");
	}
	
	return start_dir + "/" + path;	
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

void writeGMR(string new_gmr_path, string old_gmr_path, string_hashmap& ent_models, int replace_level)
{
	vector<string> replace_models;
	vector<string> replaced;
	if (replace_level == 2) // replace default precache models + weapon models (entity models replaced at map-level)
	{
		replace_models.insert(replace_models.end(), default_gib_models.begin(), default_gib_models.end());
		replace_models.insert(replace_models.end(), monster_sprites.begin(), monster_sprites.end());
		replace_models.insert(replace_models.end(), default_precache_models.begin(), default_precache_models.end());
	}
	else if (replace_level == 1) //  replace default precache models + weapon models + entity models
	{
		replace_models.insert(replace_models.end(), monster_sprites.begin(), monster_sprites.end());
		replace_models.insert(replace_models.end(), default_gib_models.begin(), default_gib_models.end());
	}
	else if (replace_level == 0) // load the old GMR file and replace models only if no new models are precached
	{
		replace_models.insert(replace_models.end(), monster_sprites.begin(), monster_sprites.end());
		replace_models.insert(replace_models.end(), default_gib_models.begin(), default_gib_models.end());
		/*
		for (string_hashmap::iterator it = ent_models.begin(); it != ent_models.end(); ++it)
		{
			if (it->first.find("/w_")) // would create an additional model
				ent_models.erase(it--);
		}
		*/
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

	for (int i = 0; i < replace_models.size(); i++)
	{
		if (matchStr(replace_models[i], "models/player.mdl"))
			continue; // replacing it causes a crash with certain weapons (looks for T model)
		string first = replace_models[i];
		if (find(dont_replace.begin(), dont_replace.end(), first) != dont_replace.end())
			continue;
		string second = get_random_replacement(first, replaced, replace_models);
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
	}
	myfile.close();

	if (replace_models.size() > 255)
		print("TOO MANY MODELS: " + str(replaced.size()));
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
		print(str(total_models + potential_additions) + " models");
	}
	else if (total_models + default_weapon_models.size() < max_model_limit)
	{
		replace_level = 1;
		print(str(total_models + potential_additions) + " models GMR-ONLY");
	}
	else
	{
		replace_level = 0;
		print(str(total_models) + " models PARTIAL-GMR-ONLY");
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
			else if (cname.find("weapon_") == 0)
			{
				int r = rand() % 2;
				it->second = random_model_replace(it->first);
			}
			else if (cname.find("monster_") == 0 || cname.find("ammo_") != string::npos || cname.find("item_") != string::npos 
			    || matchStr(cname, "cycler") || matchStr(cname, "cycler_weapon")
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
					it->second = "models/" + APACHE_MODELS[r] + ".mdl";
				}
				else
				{
					it->second = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
				
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
							it->second = "models/" + get_random_model(MODEL_TYPE_GENERIC) + ".mdl";
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

			if (find(replaced.begin(), replaced.end(), it->second) != replaced.end())
				it--; // do it again. We can't use a replacement model that's been replaced
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
					ents[i]->keyvalues[custom_monster_model_key] = "models/" + APACHE_MODELS[r] + ".mdl";
				}
				else
					replace_entity_model(ents[i], custom_monster_model_key, MODEL_TYPE_GENERIC, potential_additions);

				if (cname.find("monster_") == 0)
				{
					// game crashes if replacing monster_* model with w_ model
					// or just doesn't work if monster is spawned from squadmaker
					// p_ and v_ models get replaced in GMR so messes up custom displayname
					bool is_replaced_model = true;
					
					while (ents[i]->keyvalues[custom_monster_model_key].find("/w_") != string::npos ||
							ents[i]->keyvalues[custom_monster_model_key].find("/p_") != string::npos ||
							ents[i]->keyvalues[custom_monster_model_key].find("/v_") != string::npos ||
							find(replace_models.begin(), replace_models.end(), ents[i]->keyvalues[custom_monster_model_key]) != replace_models.end())
						replace_entity_model(ents[i], custom_monster_model_key, MODEL_TYPE_GENERIC, ++potential_additions);

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

	writeGMR(path + new_gmr_path, old_gmr_path, ent_models, replace_level);
}

// the "main" function
int checkBSPs()
{
	string path = getWorkDir() + "maps/";
	vector<string> files = getDirFiles(getWorkDir() + "maps/","bsp");

	cout << "Found " << files.size() << " maps\n";

	vector<Wad> wads;
	if (texMode == TEX_MAP || texMode == TEX_WADS)
	{
		wads = getWads(false);
		println("Found " + str(wads.size()) + " wads");
	}

	numOverflow = 0;
	getAllSounds();
	getAllVoices();

	int sounds = 0;
	if (sndMode != SND_NONE)
	{
		int voices = 0;
		for (int i = 0; i < NUM_MASTER_DIRS; ++i)
			sounds += masterSize[i];
		for (int i = 0; i < NUM_VOICE_DIRS; ++i)
			voices += vsize[i];
		println("Using default SC 4.8 sounds (" + str(sounds) + " sounds)");
	}


	cout << "\nWARNING:\nClosing the application before this finishes may cause file corruption.\n\n";
	cout << "1: Abort\n";
	cout << "0: Continue\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1')
			return 1;
		if (choice == '0')
			break;
	}

	Entity *** entLists = new Entity**[files.size()];
	int idx = 0;

	system("cls");

	for (int f = 0; f < files.size(); f++)
	{
		string mapName = getSubStr(files[f], 0, files[f].length()-4);
		if (verbose)
		{
			println("\n        " + mapName);
			println("---------------------------");
		}

		string fpath = path + files[f];

		if (!verbose)
			print(mapName + " --- ");

		BSP * map = loadBSP(mapName, true);
		if (usePrefix)
			map->name = "gsrand_" + map->name;

		if (texMode != TEX_NONE)
		{
			int tex = makeMapWad(map, wads);

			if (!verbose)
				print(str(tex) + " textures. ");
		}

		int numEnts;
		Entity** ents = getMapEnts(map, false, numEnts);
		
		if (mdlMode != MDL_NONE)
		{
			do_model_replacement(map, ents, path);
		}


		if (sndMode != SND_NONE || entMode != ENT_NONE)
		{
			if (!verbose && entMode != ENT_NONE)
				print(str(numEnts) + " ents. ");
			tallyMonsters(ents, map->name);
		}

		if (sndMode != SND_NONE || usePrefix || mdlMode != MDL_NONE)
			updateCFG(path, mapName, true);

		if (sndMode != SND_NONE)
		{
			vector<sound> writables;
			if (sndMode != SND_MONST)
			{
				writables = generateWritable(verbose);
				string gsrName = path + map->name + ".gsrand";
				writeGSR(gsrName, writables);
			}
			if (sndMode != SND_WORLD)
			{
				writeMonsterLists(map->name);
				writeSentences(map->name);
			}
			
			if (!verbose)
				print(str(writables.size()) + " sounds.");
		}

		if (texMode != TEX_NONE || sndMode != SND_NONE || entMode != ENT_NONE || mdlMode != MDL_NONE)
		{
			needsRipent(map, ents);
			ripent(map, ents, false);
		}

		for (int i = 0; i < HEADER_LUMPS; i++)
			delete [] map->lumps[i];
		delete [] map->lumps;
		delete map;

		// this is faster than deleting each entity
		for (int i = 0; i < numEnts; i++)
		{
			if (ents[i] != NULL)
				ents[i]->keyvalues.clear();
		}
		entLists[idx++] = ents;
		
		/*
		for (int i = 0; i < numEnts; i++)
		{
			if (ents[i] != NULL)
				delete ents[i];
		}
		delete [] ents;
		*/
		
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
	{
		println("\nPreparing to write master WAD");
		vector<Wad> wads = getWads(false);
		println("Found " + str(wads.size()) + " wads");
		writeWad(masterWadTex, wads, "gsrand");
	}
	return 0;
}

void formatSentences()
{
	string file = getWorkDir() + "maps/sentences.txt";
	vector<string> text;
	string line;
	ifstream myfile (file);
	if (myfile.is_open())
	{
		while ( !myfile.eof() )
		{
			getline (myfile,line);
			if (line.length() > 0 && isLetter(line[0]))
			{
				int end = line.length();
				for (int i = 0; i < line.length(); i++)
				{
					if (line[i] == ' ' || line[i] == '\t')
					{
						end = i;
						break;
					}
				}
				line = getSubStr(line, 0, end);

				bool repeat = false;
				for (int i = 0; i < text.size(); i++)
				{
					if (matchStr(text[i], line))
					{
						repeat = true;
						break;
					}
				}
				if (!repeat)
					text.push_back(line);
			}
		}
	}
	myfile.close();

	string file2 = getWorkDir() + "maps/sentences2.txt";
	ofstream fout;
	println(file2);
	fout.open (file2, ios::out | ios::trunc);
	for (int i = 0; i < text.size(); i++)
	{
		fout << "\"" <<  text[i] << "\"," << endl;
	}

	fout.close();
}

void undoEverything()
{
	string path = getWorkDir() + "maps/";

	int numUpdated = 0;
	int numRemoved = 0;
	vector<string> files = getDirFiles(path,"entbackup");
	vector<string> files2 = getDirFiles(path,"texbackup");
	for (int i = 0; i < files2.size(); i++)
		files.push_back(files2[i]);
	vector<string> restored;
	for (int f = 0; f < files.size(); f++)
	{
		string mapname = getSubStr(files[f],0,files[f].length()-10);

		bool repeat = false;
		for (int i = 0; i < restored.size(); i++)
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

	files = getDirFiles(path,"bsp");
	for (int f = 0; f < files.size(); f++)
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

	for (int f = 0; f < files.size(); f++)
		remove( string(path + files[f]).c_str() );

	if (files.size() > 0)
		println("Deleted " + str(files.size()) + " GSRAND files");

	numUpdated = 0;
	numRemoved = 0;
	files = getDirFiles(path,"cfg");
	for (int f = 0; f < files.size(); f++)
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
		int action = updateCFG(path, files[f], false);
		if (action == 0)
			numUpdated++;
		else if (action == 2)
			numRemoved++;
	}
	if (numUpdated > 0)
		println("Restored " + str(numUpdated) + " CFGs.");
	if (numRemoved > 0)
		println("Deleted " + str(numRemoved) + " CFGs.");

	numRemoved = 0;
	files = getDirFiles(getWorkDir(),"wad");
	for (int f = 0; f < files.size(); f++)
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

void recurseSubdirs(string path, vector<string>& dirs)
{
	dirs.push_back(path);
	vector<string> files = getSubdirs(path);
	for (int i = 0; i < files.size(); i++)
		recurseSubdirs(path + files[i] + '/', dirs);
}

vector<string> getAllSubdirs(string path)
{
	vector<string> dirs;
	recurseSubdirs(path, dirs);
	return dirs;
}

void genSoundList()
{
	vector<string> allSounds;
	string soundPath = "sound/";
	vector<string> dirs = getAllSubdirs(soundPath);
	vector<string> validDirs;
	vector<string> codeDirs;
	vector<int> dirSizes;
	vector<string> all;
	all.reserve(5000);

	for (int i = 0; i < dirs.size(); i++)
	{
		string prefix = "";
		string vdir = "";
		if (dirs[i].length() > 6)
		{
			prefix = getSubStr(dirs[i], 6); // skip sound/
			vdir = getSubStr(prefix,0,prefix.length()-1);
		}
		//println(dirs[i]);
		bool validDir = false;
		vector<string> sounds;
		for (int e = 0; e < num_exts; e++)
		{
			vector<string> results = getDirFiles(dirs[i], exts[e]);
			if (results.size() > 0)
			{
				if (!validDir)
				{
					//println("\"" + vdir + "\",");
					validDirs.push_back(vdir);
					validDir = true;
				}
				
				for (int k = 0; k < results.size(); k++)
				{
					all.push_back(prefix + results[k]);
					sounds.push_back(results[k]);
					//println(prefix + results[k]);
				}
			}
		}
		if (sounds.size() > 0)
		{
			string formatted = vdir;
			for (int c = 0; c < formatted.length(); c++)
			{
				if (formatted[c] == '/')
					formatted[c] = '_';
			}
			if (formatted.length() == 0)
				formatted = "sound";
			formatted = "MASTER_" + formatted;
			codeDirs.push_back(formatted);
			dirSizes.push_back(sounds.size());
			string num_cnt = "NUM_" + formatted; 
			println("#define " + num_cnt + " " + str(sounds.size()));
			println("string " + formatted + "[" + num_cnt + "] =\n{");
			for (int s = 0; s < sounds.size(); s++)
			{
				println("\t\"" + sounds[s] + "\",");
			}
			println("};\n");
		}	
	}

	println("#define NUM_MASTER_DIRS " + str(validDirs.size()));
	println("string * masterList[NUM_MASTER_DIRS];");
	println("int      masterSize[NUM_MASTER_DIRS];");
	println("string   masterDirs[NUM_MASTER_DIRS];");
	println("void initMasterList()\n{");
	for (int i = 0; i < validDirs.size(); i++)
	{
		println("\tmasterList[" + str(i) + "] = " + codeDirs[i] + ';');
	}
	println("");
	for (int i = 0; i < validDirs.size(); i++)
	{
		println("\tmasterSize[" + str(i) + "] = " + str(dirSizes[i]) + ';');
	}
	println("");
	for (int i = 0; i < validDirs.size(); i++)
	{
		println("\tmasterDirs[" + str(i) + "] = \"" + validDirs[i] + "\";");
	}
	println("}\n");

	println("found " + str(all.size()) + " files in " + str(dirs.size()) + " dirs");
	println("Valid dirs: " + str(validDirs.size()));
	writeLog();
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

	for (int i = 0; i < dirs.size(); i++)
	{
		vector<string> results = getDirFiles(dirs[i], "mdl");
		string cpath = getSubStr(dirs[i],7); // skip models/

		for (int k = 0; k < results.size(); k++)
		{
			string name = getSubStr(results[k],0,results[k].length()-4);

			// skip animation models
			string last_2_chars = getSubStr(name, name.length()-2);
			if (isNumber(last_2_chars)) // 357 gun doesn't use extra models (USUALLY)
			{
				bool is_anim_model = false;
				string find_model = getSubStr(name, 0, name.length()-2);
				for (int x = 0; x < results.size(); x++)
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
	println("string APACHE_MODELS[NUM_APACHE_MODELS] =");
	println("{");
	for (int s = 0; s < apache_models.size(); s++)
		println("\t\"" + apache_models[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_V " + str(v_models.size()));
	println("string MODEL_V[NUM_MODEL_V] =");
	println("{");
	for (int s = 0; s < v_models.size(); s++)
		println("\t\"" + v_models[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_P " + str(p_models.size()));
	println("string MODEL_P[NUM_MODEL_P] =");
	println("{");
	for (int s = 0; s < p_models.size(); s++)
		println("\t\"" + p_models[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_W " + str(w_models.size()));
	println("string MODEL_W[NUM_MODEL_W] =");
	println("{");
	for (int s = 0; s < w_models.size(); s++)
		println("\t\"" + w_models[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_PROPS " + str(props.size()));
	println("string MODEL_PROPS[NUM_MODEL_PROPS] =");
	println("{");
	for (int s = 0; s < props.size(); s++)
		println("\t\"" + props[s] + "\",");
	println("};\n");

	println("#define NUM_MODEL_MONSTERS " + str(monster.size()));
	println("string MODEL_MONSTERS[NUM_MODEL_MONSTERS] =");
	println("{");
	for (int s = 0; s < monster.size(); s++)
		println("\t\"" + monster[s] + "\",");
	println("};\n");

	println("V_: " + str(v_models.size()));
	println("P_: " + str(p_models.size()));
	println("W_: " + str(w_models.size()));
	println("Prop: " + str(props.size()));
	println("Monster: " + str(monster.size()));

	writeLog();
}

void genSpriteList()
{
	vector<string> allSprites;
	string spritePath = "sprites/";
	vector<string> dirs = getAllSubdirs(spritePath);
	vector<string> animated;
	vector<string> singleframe;

	for (int i = 0; i < dirs.size(); i++)
	{
		vector<string> results = getDirFiles(dirs[i], "spr");
		string cpath = getSubStr(dirs[i],8); // skip sprites/
		for (int k = 0; k < results.size(); k++)
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
	println("string ANIMATED_SPRITES[NUM_ANIMATED_SPRITES] =");
	println("{");
	for (int s = 0; s < animated.size(); s++)
		println("\t\"" + animated[s] + "\",");
	println("};\n");

	println("#define NUM_STATIC_SPRITES " + str(singleframe.size()));
	println("string STATIC_SPRITES[NUM_STATIC_SPRITES] =");
	println("{");
	for (int s = 0; s < singleframe.size(); s++)
		println("\t\"" + singleframe[s] + "\",");
	println("};\n");

	
	println("Animated sprites: " + str(animated.size()));
	println("Static sprites: " + str(singleframe.size()));

	writeLog();
}

vector<Wad> getWads(bool defaultOnly)
{
	vector<Wad> wads;
	for (int i = 0; i < NUM_DEFAULT_WADS; i++)
	{
		string name = default_wads[i] + ".wad";
		if (fileExists(name))
			wads.push_back(Wad(name));
		else if (fileExists(wadPath + name))
			wads.push_back(Wad(wadPath + name));
	}
	if (!defaultOnly)
	{
		vector<string> files = getDirFiles(wadPath, "wad");
		for (int i = 0; i < files.size(); i++)
		{
			string prefix = getSubStr(files[i],0,6);
			if (matchStrCase(prefix,"gsrand"))
				continue;

			bool repeat = false;
			for (int k = 0; k < wads.size(); k++)
			{
				if (matchStr(wadPath + files[i],wads[k].filename))
				{
					repeat = true;
					break;
				}
			}
			if (!repeat)
				wads.push_back(Wad(wadPath + files[i]));
		}
	}
	for (int i = 0; i < wads.size(); i++)
	{
		//println("Caching wad " + wads[i].filename);
		wads[i].readInfo();
		//wads[i].loadCache();
	}
	return wads;
}

WADTEX ** loadRandomTextures(vector<string> wadTextures, vector<Wad> wads)
{
	if (wads.size() <= 0)
	{
		println("No Usable WADs!");
		return NULL;
	}

	if (wadTextures.size() > 0)
	{
		if (texMode == TEX_MASTERWAD)
			println("Loading " + str(wadTextures.size()) + " random textures...");

		int fails = 0;
		WADTEX ** newTex = new WADTEX*[wadTextures.size()];
		for (int i = 0; i < wadTextures.size(); i++)
		{
			int randWad = rand() % wads.size();
			int randTex = rand() % wads[randWad].header.nDir;
			WADDIRENTRY& de = wads[randWad].dirEntries[randTex];
			if (de.nType != 0x43 || de.nSize > MAXTEXELS)
			{
				if (fails++ > 1000)
				{
					println("Failed to find valid textures after 1000 retries");
					return NULL;
				}
				i--;
				continue;
			}
			fails = 0;
			newTex[i] = wads[randWad].readTexture(randTex);
			if (newTex[i] == NULL)
				println("DANGER NULL TEX");
			for (int k = 0; k < MAXTEXTURENAME; k++)
			{
				if (k < wadTextures[i].length())
					newTex[i]->szName[k] = wadTextures[i][k];
				else
					newTex[i]->szName[k] = '\0';
			}
		}
		return newTex;
	}
	else
		println("No wadTextures found");
	return NULL;
}

void writeWad(vector<string> wadTextures, vector<Wad> wads, string mapname)
{
	WADTEX ** newTex = loadRandomTextures(wadTextures, wads);
	if (newTex != NULL)
	{
		Wad newWad = Wad();
		string filename = "";
		if (texMode == TEX_WADS && !usePrefix)
			filename += "gsrand_";
		filename += mapname + ".wad";

		if (texMode == TEX_MASTERWAD)
			println("Writing " + filename + "...");
		newWad.write(filename, newTex, wadTextures.size());

		for (int i = 0; i < wadTextures.size(); i++)
		{
			delete [] newTex[i]->data;
			delete newTex[i];
		}
		delete [] newTex;
	}
}

BSPTEXDATA * genTexLump(vector<string> wadTextures, vector<Wad> wads, BSP * map)
{
	WADTEX ** newTex = loadRandomTextures(wadTextures, wads);
	if (newTex != NULL)
	{
		BSPTEXDATA * texdata = new BSPTEXDATA();
		texdata->numTex = wadTextures.size();
		texdata->offset = new int[texdata->numTex];
		texdata->len = new int[texdata->numTex];
		texdata->offset = new int[texdata->numTex];

		int offset = sizeof(int)*(texdata->numTex+1);
		
		texdata->tex = newTex;
		for (int i = 0; i < texdata->numTex; i++)
		{
			texdata->offset[i] = offset;
			int w = newTex[i]->nWidth;
			int h = newTex[i]->nHeight;
			int sz = w*h;	   // miptex 0
			int sz2 = sz / 4;  // miptex 1
			int sz3 = sz2 / 4; // miptex 2
			int sz4 = sz3 / 4; // miptex 3
			int szAll = sz + sz2 + sz3 + sz4 + 2 + 256*3 + 2;
			texdata->len[i] = szAll;
			offset += texdata->len[i] + sizeof(BSPMIPTEX);
		}
		if (!usePrefix)
			saveLumpBackup(map, LUMP_TEXTURES, ".texbackup");
		map->header.lump[LUMP_TEXTURES].nLength = offset;
		return texdata;
	}
	return NULL;
}

int makeMapWad(BSP * map, vector<Wad>& wads)
{
	map->texdata = NULL;
	byte * data = map->lumps[LUMP_TEXTURES];
	int idx = 0;
	int len = map->header.lump[LUMP_TEXTURES].nLength;

	if (idx + sizeof(int) > len)
	{
		println("Can't read texture header! " + str(idx) + ", " + str(len));
		return 0;
	}
	int numTextures = *(reinterpret_cast<int*>(&data[idx]));
	idx += sizeof(int);

	if (numTextures > MAX_MAP_TEXTURES)
	{
		println("Too many textures!");
		return 0;
	}
	
	vector<string> wadTextures;
	int find = 0;
	if (numTextures > 0)
	{
		int * miptexOffset = new int[numTextures]; // file offset from beginning of the texture lump

		for (int i = 0; i < numTextures; i++)
		{
			if (idx + sizeof(int) > len)
			{
				println("Can't read texture header! " + str(idx) + ", " + str(len));
				return 0;
			}
			miptexOffset[i] = *(reinterpret_cast<int*>(&data[idx]));
			idx += sizeof(int);
		}
		
		// if miptex offsets are > 0 then the texture is stored inside the bsp
		for (int i = 0; i < numTextures; i++)
		{
			idx = miptexOffset[i];
			if (idx + sizeof(BSPMIPTEX) > len)
			{
				println("Can't read texture! " + str(idx) + ", " + str(len));
				return 0;
			}

			BSPMIPTEX miptex = *(reinterpret_cast<BSPMIPTEX*>(&data[idx]));
			//println("tex: " + string(miptex.szName));
			if (miptex.nOffsets[0] <= 0 || texMode == TEX_MAP)
			{
				string tname = string(miptex.szName);
				if (texMode == TEX_MAP || (!matchStr(tname, "clip") && !matchStr(tname, "null") &&
					!matchStr(tname, "sky") && !matchStr(tname, "aaatrigger") &&
					!matchStr(tname, "bevel") && !matchStr(tname, "hint") &&
					!matchStr(tname, "origin") && !matchStr(tname, "skip") &&
					!matchStr(tname, "translucent")))
				{
					if (texMode == TEX_MASTERWAD)
					{
						bool repeat = false;
						for (int k = 0; k < masterWadTex.size(); k++)
						{
							if (matchStr(masterWadTex[k],tname))
								repeat = true;
						}
						if (!repeat)
							masterWadTex.push_back(tname);
					}
					else
						wadTextures.push_back(tname);
					find++;
				}
				
			}
		}
		delete [] miptexOffset;
	}

	//println("bsp has " + str(wadTextures.size()) + " replacable textures");

	if (texMode == TEX_MAP)
	{
		map->texdata = genTexLump(wadTextures,wads,map);
		if (map->texdata == NULL)
			println("UH OH");
	}
	if (texMode == TEX_WADS && wadTextures.size() > 0)
		writeWad(wadTextures,wads,map->name);

	return find;
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
		 << "0: Next Page\n\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1')
			return;
		if (choice == '0')
			break;
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
		<< "0: Next Page\n\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1')
			return;
		if (choice == '0')
			break;
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
		<< "0: Next Page\n\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1')
			return;
		if (choice == '0')
			break;
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
		<< "     since missing animations can stop map progression.\n"
		<< "1: Done\n"
		<< "0: Next Page\n\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1')
			return;
		if (choice == '0')
			break;
	}

	system("cls");
	cout << "\n                      Map prefix\n"
		<< "-------------------------------------------------------\n"
		<< "The map prefix 'gsrand_' cannot be changed. It is set\n"
		<< "automatically if any texture mode is active. You can\n"
		<< "disable it when this happens, but anyone who joins your\n"
		<< "server will get a 'Map Differs' error or not see the\n"
		<< "randomized textures.\n\n"
		<< "1: Done\n"
		<< "0: Next Page\n\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1')
			return;
		if (choice == '0')
			break;
	}
	
}

int _tmain(int argc, _TCHAR* argv[])
{
	srand ( time(NULL) );
	//srand (1337);
	//genSoundList();
	//genModelList();
	//genSpriteList();
	//return 0;
	
	while (true)
	{
		system("cls"); // WINDOWS ONLY
		cout << "Welcome to w00tguy's map randomizer!\n" << endl;

		cout << "Options:\n";
		cout << "1: Texture Mode: ";
		if (texMode == TEX_MAP)
			cout << "Write to map\n";
		else if (texMode == TEX_MASTERWAD)
			cout << "Write to a common WAD\n";
		else if (texMode == TEX_WADS)
			cout << "Write WAD for each map\n";
		else if (texMode == TEX_NONE)
			cout << "Do not change\n";

		cout << "2: Entity Mode : ";
		if (entMode == ENT_NONE)
			cout << "Do not change\n";
		else if (entMode == ENT_SUPER)
			cout << "Super random\n";
		else if (entMode == ENT_APPEAR)
			cout << "Appearances only\n";

		cout << "3: Sound Mode  : ";
		if (sndMode == SND_NONE)
			cout << "Do not change\n";
		else if (sndMode == SND_ALL)
			cout << "Everything\n";
		else if (sndMode == SND_WORLD)
			cout << "World only\n";
		else if (sndMode == SND_MONST)
			cout << "Monsters only\n";

		cout << "4: Model Mode  : ";
		if (mdlMode == MDL_NONE)
			cout << "Do not change\n";
		else if (mdlMode == MDL_CRAZY)
			cout << "Super random\n";
		else if (mdlMode == MDL_TYPED)
			cout << "Use similar types\n";

		cout << "5: Map Prefix  : ";
		if (usePrefix)
			cout << "gsrand_\n";
		else if (!usePrefix)
			cout << "No prefix\n";

		cout << endl;

		cout << "6: Randomize!" << endl;
		cout << "7: Undo all changes\n" << endl;

		cout << "0: Help" << endl;

		char choice = _getch();
		if (choice == '6')
		{
			system("cls"); // WINDOWS ONLY
			initLists();
			if (checkBSPs() == 0)
			{
				println("\nDone.");
				println("\nInstructions for playing with others:");
				if (!(texMode == TEX_NONE) || usePrefix)
					println("-Give them your BSPs");
				else
					println("-Just start up a server, they don't need any files!");
				if (texMode == TEX_MAP && !usePrefix)
				{
					println("\nPlayers joining you will get a \"Map differs\" error without these files");
				}
				else if (texMode == TEX_MASTERWAD)
				{
					println("-Give them your gsrand.wad");
				}
				else if (texMode == TEX_WADS)
				{
					println("-Give them your gsrand_mapname.wad files (there's one for each map)");
				}
				if (!(texMode == TEX_MAP) && !(texMode == TEX_NONE) && !usePrefix)
					println("\nPlayers joining you will see normal textures without these files");

				cout << endl;
				writeLog();
				system("pause");
			}
		}
		else if (choice == '7')
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
			if (++sndMode >= SND_MODES)
				sndMode = 0;			
		}
		else if (choice == '4')
		{
			if (++mdlMode >= MDL_MODES)
				mdlMode = 0;			
		}
		else if (choice == '5')
		{
			usePrefix = !usePrefix;	
		}
		else if (choice == '0')
		{
			printHelp();
		}

		if (choice != '5')
			usePrefix = texMode != TEX_NONE;
	}
	




	
	return 0;
}

