#include "sound_util.h"
#include "ent_util.h"
#include "gsrand.h"

string get_random_sound()
{
	if (user_sounds.size() == 0)
		return "ambience/_comma.wav";
	return user_sounds[rand() % user_sounds.size()];
	
}

void getAllSounds()
{
	initMasterList();
	
	string soundPath = "sound/";
	vector<string> dirs = getAllSubdirs(soundPath);

	user_sounds.clear();
	user_sound_dirs.clear();
	for (uint i = 0; i < dirs.size(); i++)
	{
		string prefix = "";
		string vdir = "";
		if (dirs[i].length() > 6)
		{
			prefix = getSubStr(dirs[i], 6); // skip sound/
			vdir = getSubStr(prefix,0,prefix.length()-1);
		}
	
		for (int e = 0; e < num_exts; e++)
		{
			vector<string> results = getDirFiles(dirs[i], exts[e]);
			for (uint k = 0; k < results.size(); k++)
				if (results[k].find("null") != 0)
					user_sounds.push_back(prefix + results[k]);
		}
	}

	vector<string> default_sounds;
	for (uint d = 0; d < NUM_MASTER_DIRS; ++d)
		for (uint s = 0; s < masterSize[d]; ++s)
			default_sounds.push_back(masterDirs[d] + string("/") + masterList[d][s]);

	vector<string> filtered_sounds;
	filtered_sounds.reserve(user_sounds.size());
	if (contentMode != CONTENT_EVERYTHING)
	{
		for (uint i = 0, sz = user_sounds.size(); i < sz; ++i)
		{
			bool match = false;
			if (user_sounds[i].find("thunder.wav") == 0) // the only default sound not in a folder
				match = true;
			for (uint d = 0; d < default_sounds.size() && !match; ++d)
			{
				if (matchStr(user_sounds[i], default_sounds[d]))
				{
					match = true;
					if (contentMode == CONTENT_DEFAULT)
						filtered_sounds.push_back(user_sounds[i]);
				}
			}
			if (!match && contentMode == CONTENT_CUSTOM) // must not have found a match
				filtered_sounds.push_back(user_sounds[i]);
		}
		user_sounds = filtered_sounds;
	}

	set<string> sound_dirs;
	for (uint i = 0; i < user_sounds.size(); ++i)
	{
		string sound;
		string sound_dir;
		int dirfind = user_sounds[i].find_last_of("/\\");
		if (dirfind != string::npos)
		{
			sound = getSubStr(user_sounds[i], dirfind+1, user_sounds[i].length()-4); // strip dirs and .wav/.ogg/etc.
			sound_dir = getSubStr(user_sounds[i], 0, dirfind);
		}
		else
		{
			sound = user_sounds[i];
			sound_dir = "/";
		}
		sound_dirs.insert(sound_dir);
		user_voices[sound_dir].push_back(sound);
	}
	for (set<string>::iterator it = sound_dirs.begin(); it != sound_dirs.end(); ++it)
		user_sound_dirs.push_back(*it);
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
					voice[i][s] = getSubStr(masterList[k][s], 0, strlen(masterList[k][s])-4);
				vsize[i] = masterSize[k];
				found = true;
				break;
			}
		}
		if (!found)
			println("couldn't find " + string(voice_dirs[i]));
	}
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

	for (uint i = 0; i < dirs.size(); i++)
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
				
				for (uint k = 0; k < results.size(); k++)
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
			for (uint c = 0; c < formatted.length(); c++)
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
			println("static const char * " + formatted + "[" + num_cnt + "] =\n{");
			for (uint s = 0; s < sounds.size(); s++)
			{
				println("\t\"" + sounds[s] + "\",");
			}
			println("};\n");
		}	
	}

	println("#define NUM_MASTER_DIRS " + str(validDirs.size()));
	println("static const char ** masterList[NUM_MASTER_DIRS];");
	println("static int           masterSize[NUM_MASTER_DIRS];");
	println("static const char *  masterDirs[NUM_MASTER_DIRS];");
	println("void initMasterList()\n{");
	for (uint i = 0; i < validDirs.size(); i++)
	{
		println("\tmasterList[" + str(i) + "] = " + codeDirs[i] + ';');
	}
	println("");
	for (uint i = 0; i < validDirs.size(); i++)
	{
		println("\tmasterSize[" + str(i) + "] = " + str(dirSizes[i]) + ';');
	}
	println("");
	for (uint i = 0; i < validDirs.size(); i++)
	{
		println("\tmasterDirs[" + str(i) + "] = \"" + validDirs[i] + "\";");
	}
	println("}\n");

	println("found " + str(all.size()) + " files in " + str(dirs.size()) + " dirs");
	println("Valid dirs: " + str(validDirs.size()));
	writeLog();
}

vector<sound> getReplacableSounds(bool printInfo)
{
	vector<sound> writable;
	int group = rand() % NUM_SND_DIRS;
	writable.reserve(256);
	
	for (int i = 0; i < NUM_SQUEEK; i++) 
		writable.push_back(sound("squeek/" + string(squeek[i]) + ".wav"));

	for (int i = 0; i < NUM_HORNET; i++) 
		writable.push_back(sound("hornet/" + string(hornet[i]) + ".wav"));

	for (int i = 0; i < NUM_COMMON; i++)
		writable.push_back(sound("common/" + string(common[i]) + ".wav"));

	for (int i = 0; i < NUM_PLAYER; i++)
		writable.push_back(sound("player/" + string(player[i]) + ".wav"));

	for (int i = 0; i < NUM_WEAPONS; i++)
		writable.push_back(sound("weapons/" + string(weapons[i]) + ".wav"));

	for (int i = 0; i < NUM_ITEMS; i++)
		writable.push_back(sound("items/" + string(items[i]) + ".wav"));

	for (int i = 0; i < NUM_SPECIAL; i++)
		writable.push_back(sound(special[i]));

	for (int i = 0; i < NUM_MASTER_buttons; i++)
		writable.push_back(sound("buttons/" + string(MASTER_buttons[i])));

	//for (int i = 0; i < NUM_MASTER_plats; i++)
		//writable.push_back(sound("plats/" + MASTER_plats[i] + ".wav"));

	//for (int i = 0; i < NUM_MASTER_doors; i++)
		//writable.push_back(sound("doors/" + MASTER_doors[i] + ".wav"));

	for (int i = 0; i < NUM_MASTER_debris; i++)
		writable.push_back(sound("debris/" + string(MASTER_debris[i])));

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
			writable.push_back(sound("plats/" + string(trainmoves[i]) + ".wav"));
	}
	for (int i = 0; i < NUM_TRAIN_STOPS; i++)
	{
		//if (tstop[i])
			writable.push_back(sound("plats/" + string(trainstops[i]) + ".wav"));
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
			writable.push_back(sound("doors/" + string(doormoves[i]) + ".wav"));
	}
	for (int i = 0; i < NUM_DOOR_STOPS; i++)
	{
		if (dstop[i])
			writable.push_back(sound("doors/" + string(doorstops[i]) + ".wav"));
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
	
	if (printInfo) println("Wrote ambient sounds: " + str(writable.size()));

	if (writable.size() > MAX_REPLACEMENTS)
		numOverflow++;

	return writable;
}

void writeMonsterSoundLists(string mapname)
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
				string randSnd = get_random_sound();
				if (matchStr(randSnd, mlists[i][k]))
				{
					i--;
					continue;
				}
				res_list.insert("sound/" + randSnd);
				myfile << '\"';
				if (i != ASLAVE)
					myfile << mdirs[i] << '/';
				myfile << mlists[i][k] << ".wav\" \"" << randSnd << '\"' << '\n';
			}
			myfile.close();

			//println("Wrote " + mdirs[i] + ": " + str(msize[i]));
			monsters[i] = 0; // so we don't pick it again
			monsterWrites++;

			res_list.insert(filename);
		}
	}
}

void writeGSR(string filename, vector<sound> writeList)
{
	ofstream myfile;
	myfile.open (filename);

	for (uint i = 0; i < writeList.size(); i++)
	{
		string randSnd = get_random_sound();
		if (matchStr(randSnd, writeList[i].filename))
		{
			i--;
			continue;
		}
		res_list.insert("sound/" + randSnd);
		myfile << '\"' << writeList[i].filename << "\" \"" << randSnd << '\"' << '\n';
	}
	myfile.close();
	//println("wrote " + str(writeList.size()) + " sound replacements");
}

string constructSentence()
{
	if (user_sound_dirs.empty())
		return "";
	string sentence;

	if (contentMode == CONTENT_CUSTOM || rand() % 4 == 0)
	{
		int randVoice = rand() % user_sound_dirs.size();
		sentence = string(user_sound_dirs[randVoice] ) + "/";
		if (user_voices.find(user_sound_dirs[randVoice]) == user_voices.end())
			println("VOICE DIR IS EMPTY: " + user_sound_dirs[randVoice]);
		else
		{
			for (int i = 0, words = (rand() % 5) + 1; i < words; i++)
			{
				int r = rand() % user_voices[user_sound_dirs[randVoice]].size();
				sentence += user_voices[user_sound_dirs[randVoice]][r] + ' ';
			}
			return sentence;
		}
	}

	int randVoice = rand() % NUM_VOICE_DIRS;
	sentence = string(voice_dirs[randVoice]) + '/';
		
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
		fout << sentences[i] << " " << constructSentence() << "\n";

	fout.close();
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
				for (uint i = 0; i < line.length(); i++)
				{
					if (line[i] == ' ' || line[i] == '\t')
					{
						end = i;
						break;
					}
				}
				line = getSubStr(line, 0, end);

				bool repeat = false;
				for (uint i = 0; i < text.size(); i++)
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
	for (uint i = 0; i < text.size(); i++)
	{
		fout << "\"" <<  text[i] << "\"," << endl;
	}

	fout.close();
}

void do_ent_sounds(Entity** ents, string mapname)
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
	
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (ents[i] == NULL) 
			break;
		string cname = ents[i]->keyvalues["classname"];

		if (matchStr(cname, "ambient_generic"))
		{
			string snd = ents[i]->keyvalues["message"];
			ents[i]->keyvalues["message"] = get_random_sound();
			// TODO: chance to use random sentence
			// if (snd.find(".") == string::npos)
			res_list.insert("sound/" + ents[i]->keyvalues["message"]);
			continue;
		}

		if (matchStr(cname, "env_sentence"))
		{
			int words = rand() % 10;
			ents[i]->keyvalues["_text"] = "";
			for (uint i = 0; i < words; i++)
			{
				string w = MASTER_vox[rand() % NUM_MASTER_vox];
				ents[i]->keyvalues["_text"] += getSubStr(w, 0, w.length()-4);
				if (i != words-1)
				{
					if (rand() % 4) ents[i]->keyvalues["_text"] += " ";
					else ents[i]->keyvalues["_text"] += ".";
				}
			}
		}

		if (matchStr(cname, "env_sound"))
		{
			ents[i]->keyvalues["roomtype"] = str((rand() % 27) + 1);
		}

		if (matchStr(cname,"func_breakable"))
		{
			string matstr = ents[i]->keyvalues["material"];
			int material = atoi(matstr.c_str());
			if (material >= 0 && material < BREAKABLE_TYPES)
				btypes[material] = true;
			continue;
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

		if (matchStr(cname,"func_door") || matchStr(cname,"func_door_rotating") || matchStr(cname,"momentary_door"))
		{
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
			ents[i]->keyvalues["classname"] = cname = "squadmaker"; // just make a squadmaker so we have more options

		if (matchStr(cname, "squadmaker"))
			child = ents[i]->keyvalues["monstertype"];	

		int wep_id = get_weapon_id(cname);
		if (wep_id == -1)
			wep_id = get_weapon_id(child);
		if (wep_id != -1)
			weaps[wep_id] = wep_id != 0;

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
		}
	}
}
