// alerter.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "gsrand.h"
#include <sys/stat.h>
#include <conio.h>
#include "sound_util.h"
#include "model_util.h"
#include "tex_util.h"
#include "ent_util.h"

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

	fout.open (path + mapname + ".cfg", ios::out | ios::trunc);
	if (text.size() > 0)
	{
		bool has_grapple;
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
				fout << text[i];
			}
			if (i != text.size()-1)
				fout << endl;
		}

		if (!has_grapple && (mdlMode != MDL_NONE || entMode == ENT_SUPER))
			fout << "weapon_grapple\n";
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

bool restoreCFG(string path, string mapname)
{
	vector<string> text;
	string line;

	ifstream myfile (path + mapname + ".cfg.bak");
	if (myfile.is_open())
	{
		while ( !myfile.eof() )
		{
			getline (myfile,line);
			text.push_back(line);
		}
	}
	else
		return false;
	myfile.close();

	ofstream fout;

	if (prefixMode != PREFIX_NONE)
		mapname = MAP_PREFIX + mapname;

	fout.open (path + mapname + ".cfg", ios::out | ios::trunc);
	if (text.size() > 0)
	{
		for (uint i = 0; i < text.size()-1; i++)
			fout << text[i] << endl;
		fout << text[text.size()-1];
	}
	fout.close();
	remove( string(path + mapname + ".cfg.bak").c_str() );

	return true;
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
		print("Found " + str(user_sounds.size()) + " sounds");
		int voices = 0;
		for (int i = 0; i < NUM_MASTER_DIRS; ++i)
			sounds += masterSize[i];
		for (int i = 0; i < NUM_VOICE_DIRS; ++i)
			voices += vsize[i];
		println("Using default SC 4.8 sounds (" + str(sounds) + " sounds)");
	}

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

	cout << "\nWARNING:\nClosing the program before this finishes may cause file corruption.\n\n";
	cout << "1: Abort\n";
	cout << "0: Continue\n";

	while (true)
	{
		char choice = _getch();
		if (choice == '1') return 1;
		if (choice == '0') break;
	}

	Entity *** entLists = new Entity**[files.size()];
	int idx = 0;

	system("cls");

	for (uint f = 0; f < files.size(); f++)
	{
		string mapName = getSubStr(files[f], 0, files[f].length()-4);
		if (verbose)
		{
			println("\n        " + mapName);
			println("---------------------------");
		}

		string fpath = path + files[f];

		if (!verbose)
		{
			string prefix = prefixMode == PREFIX_NONE ? "" : MAP_PREFIX;
			print(prefix + mapName + " --- ");
		}

		BSP * map = loadBSP(mapName, true);
		if (prefixMode != PREFIX_NONE)
			map->name = MAP_PREFIX + map->name;

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

		if (sndMode != SND_NONE)
			do_ent_sounds(ents, map->name);

		if (entMode != ENT_NONE)
		{
			if (!verbose && entMode != ENT_NONE)
				print(str(numEnts) + " ents. ");
			do_entity_randomization(ents, map->name);
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

		if (texMode != TEX_NONE || sndMode != SND_NONE || entMode != ENT_NONE || mdlMode != MDL_NONE || prefixMode != PREFIX_NONE)
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
		if (prefixMode == PREFIX_NONE)
		{
			if (restoreCFG(path, files[f]))
				numUpdated++;
		}
	}
	if (numUpdated > 0)
		println("Restored " + str(numUpdated) + " CFGs.");

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

int _tmain(int argc, _TCHAR* argv[])
{
 	srand ( (uint)time(NULL) );
	//srand (1337);
	//genSoundList();
	//genModelList();
	//genSpriteList();
	//return 0;
	
	//getAllSounds();
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

		cout << "3: Model Mode  : ";
		if (mdlMode == MDL_NONE)
			cout << "Do not change\n";
		else if (mdlMode == MDL_CRAZY)
			cout << "Super random\n";
		else if (mdlMode == MDL_TYPED)
			cout << "Use similar types\n";

		cout << "4: Sound Mode  : ";
		if (sndMode == SND_NONE)
			cout << "Do not change\n";
		else if (sndMode == SND_ALL)
			cout << "Everything\n";
		else if (sndMode == SND_WORLD)
			cout << "World only\n";
		else if (sndMode == SND_MONST)
			cout << "Monsters only\n";

		cout << "5: Content Mode: ";
		if (contentMode == CONTENT_EVERYTHING)
			cout << "Everything\n";
		else if (contentMode == CONTENT_DEFAULT)
			cout << "Default only\n";
		else if (contentMode == CONTENT_CUSTOM)
			cout << "Non-default only\n";

		cout << "6: Map Prefix  : ";
		if (prefixMode == PREFIX_GSRAND)
			cout << "gsrand_\n";
		else if (prefixMode == PREFIX_TIME)
			cout << MAP_PREFIX << " (current time encoded in base36)\n";
		else if (prefixMode == PREFIX_CUSTOM)
			cout << "Custom\n";
		else if (prefixMode == PREFIX_NONE)
			cout << "No prefix\n";

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
				println("\nInstructions for playing with others:");
				if (!(texMode == TEX_NONE) || prefixMode != PREFIX_NONE)
					println("-Give them your BSPs");
				else
					println("-Just start up a server, they don't need any files!");
				if (texMode == TEX_MAP && prefixMode == PREFIX_NONE)
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
				if (!(texMode == TEX_MAP) && !(texMode == TEX_NONE) && prefixMode == PREFIX_NONE)
					println("\nPlayers joining you will see normal textures without these files");

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

