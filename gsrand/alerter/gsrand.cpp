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
	wlists[PIPEWRENCH] = wrench;
	wlists[CROWBAR] = crowbar;
	wlists[MINE] = mine;
	wlists[EGON] = egon;

	wsize[PIPEWRENCH] = NUM_WRENCH;
	wsize[CROWBAR] = NUM_CROWBAR;
	wsize[MINE] = NUM_MINE;
	wsize[EGON] = NUM_EGON;

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
				if (line.length() >= 17 && matchStr(getSubStr(line, 0, 17), "weapon_pipewrench"))
					weaps[PIPEWRENCH] = true;
				if (line.length() >= 14 && matchStr(getSubStr(line, 0, 14), "weapon_crowbar"))
					weaps[CROWBAR] = true;
				if (line.length() >= 15 && matchStr(getSubStr(line, 0, 15), "weapon_tripmine"))
					weaps[MINE] = true;
				if (line.length() >= 11 && matchStr(getSubStr(line, 0, 11), "weapon_egon"))
					weaps[EGON] = true;
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
		fout << "\n## GSRAND BEGIN\n" << 
			"globalsoundlist ../../maps/" << mapname << ".gsrand\n" <<
			"sentence_file maps/" << mapname << "_sentences.gsrand";
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

	for (int i = 0; i < MONSTER_TYPES; i++)
	{
		if (mdlMode == MDL_CRAZY)
		{
			int randType = rand() % 4;
			if (randType == 0)
		}
		monsters[i] = 0;
	}

	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (ents[i] == NULL) 
			break;
		string cname = ents[i]->keyvalues["classname"];

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
				ents[i]->keyvalues["displayname"] = "hurr i'm a durr";
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

		if (matchStr(cname, "weaponbox") || matchStr(cname, "game_player_equip"))
		{
			if (ents[i]->keyvalues["weapon_pipewrench"].length() > 0)
				weaps[PIPEWRENCH] = true;
			if (ents[i]->keyvalues["weapon_crowbar"].length() > 0)
				weaps[CROWBAR] = true;
			if (ents[i]->keyvalues["weapon_tripmine"].length() > 0)
				weaps[MINE] = true;
			if (ents[i]->keyvalues["weapon_egon"].length() > 0)
				weaps[EGON] = true;
		}

		string child = "";

		if (matchStr(cname, "monstermaker"))
		{
			if (entMode == ENT_SUPER)
				ents[i]->keyvalues["classname"] = cname = "squadmaker";
		}

		if (matchStr(cname, "squadmaker"))
		{
			if (entMode == ENT_SUPER)
			{
				ents[i]->keyvalues["change_rendermode"] = "1";
				ents[i]->keyvalues["notsolid"] = "1";
			}
			child = ents[i]->keyvalues["monstertype"];
		}

		if (matchStr(cname,"weapon_pipewrench") || matchStr(child,"weapon_pipewrench"))
			weaps[PIPEWRENCH] = true;
		if (matchStr(cname,"weapon_crowbar") || matchStr(child,"weapon_crowbar"))
			weaps[CROWBAR] = true;
		if (matchStr(cname,"weapon_tripmine") || matchStr(child,"weapon_tripmine"))
			weaps[MINE] = true;
		if (matchStr(cname,"weapon_egon") || matchStr(child,"weapon_egon"))
			weaps[EGON] = true;

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
					int rot1 = rand() % 60 - 30;
					int rot2 = rand() % 60 - 30;
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

				// 1/4 chance of making monster an ally
				if (rand() % 4 == 0 && matchIdx != SCIENTIST && entMode == ENT_SUPER)
				{
					int ally = 0;
					if (matchStr(ents[i]->keyvalues["classname"],"squadmaker") || 
						matchStr(ents[i]->keyvalues["classname"],"monstermaker"))
						ents[i]->keyvalues["respawn_as_playerally"] = "1";
					else
						ents[i]->keyvalues["is_player_ally"] = "1";
					ents[i]->keyvalues["displayname"] = "I have come for to help you";
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

int checkBSPs()
{
	string path = getWorkDir() + "maps/";
	vector<string> files = getDirFiles(getWorkDir() + "maps/","bsp");

	cout << "Found " << files.size() << " maps\n\n";
	cout << "WARNING:\nClosing the application before this finishes may cause file corruption.\n\n";
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

	numOverflow = 0;
	getAllSounds();
	getAllVoices();
	vector<Wad> wads;
	if (texMode == TEX_MAP || texMode == TEX_WADS)
	{
		wads = getWads(false);
		println("loaded " + str(wads.size()) + " wads");
	}


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

		if (sndMode != SND_NONE || entMode != ENT_NONE)
		{
			if (!verbose && entMode != ENT_NONE)
				print(str(numEnts) + " ents. ");
			tallyMonsters(ents, map->name);
		}

		if (sndMode != SND_NONE || usePrefix)
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

		if (texMode != TEX_NONE || sndMode != SND_NONE || entMode != ENT_NONE)
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

	for (int i = 0; i < dirs.size(); i++)
	{
		vector<string> results = getDirFiles(dirs[i], "mdl");
		string cpath = getSubStr(dirs[i],7); // skip models/
		for (int k = 0; k < results.size(); k++)
		{
			string name = getSubStr(results[k],0,results[k].length()-4);
			if (name.length() > 0)
			{
				ifstream fin (dirs[i] + results[k], ios::binary);

				MDLHEADER mdlHead;
				fin.read((char*)&mdlHead, sizeof(MDLHEADER));

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
						v_models.push_back(cpath + results[k]);
						continue;
					}
					else if (matchStr(prefix, "p_"))
					{
						p_models.push_back(cpath + results[k]);
						continue;
					}
					else if (matchStr(prefix, "w_"))
					{
						w_models.push_back(cpath + results[k]);
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
						props.push_back(cpath + results[k]);
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

				monster.push_back(cpath + results[k]);

			}				
		}
	}

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

