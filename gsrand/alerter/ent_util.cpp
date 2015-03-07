#include "ent_util.h"
#include "gsrand.h"

Keyvalue extractKeyvalue(string line)
{
	int begin = -1;
	int end = -1;

	string key, value;
	key = "";
	value = "";
	int comment = 0;

	for (uint i = 0; i < line.length(); i++)
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

void ripent(BSP * map, Entity** entData, bool restore)
{
	if (restore)
	{
		loadLumpBackup(map, LUMP_ENTITIES, ".entbackup");
		loadLumpBackup(map, LUMP_TEXTURES, ".texbackup");
	}
	else
	{
		if (!prefixMode != PREFIX_NONE)
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
	else if (prefixMode != PREFIX_NONE)
		newWadString = MAP_PREFIX + map->name;
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
	if (texMode == TEX_MAP)
	{
		ents[0]->keyvalues["wad"] = "";
		return true;
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

void randomize_skybox(Entity ** ents)
{
	if (!user_skies.size())
		return;
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (ents[i] == NULL) 
			break;
		string cname = ents[i]->keyvalues["classname"];

		if (matchStr(cname,"worldspawn"))
			ents[i]->keyvalues["skyname"] = user_skies[rand() % user_skies.size()];
	}
}

void update_changelevels(Entity** ents, string mapname)
{
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (ents[i] == NULL) 
			break;
		string cname = ents[i]->keyvalues["classname"];

		if (matchStr(cname,"trigger_changelevel"))
		{
			if (prefixMode != PREFIX_NONE)
				ents[i]->keyvalues["map"] = MAP_PREFIX + ents[i]->keyvalues["map"];
			if (entMode == ENT_SUPER) 
			{
				if (matchStr(mapname,ents[i]->keyvalues["map"])) // no restarts!
					ents[i]->keyvalues["map"] = "Sanic Hodgepodge";
			}
		}
	}
}

void do_entity_randomization(Entity** ents, string mapname)
{	
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (ents[i] == NULL) 
			break;
		string cname = ents[i]->keyvalues["classname"];

		// map-specific changes
		if (entMode != ENT_NONE && ents[i]->keyvalues.find("master") != ents[i]->keyvalues.end())
			if (matchStr(cname, "trigger_once") && matchStr(ents[i]->keyvalues["master"], "alldead_ms"))
				ents[i]->keyvalues.erase("master"); // bypass "kill all monsters" bullshit

		if (ents[i]->keyvalues.find("targetname") != ents[i]->keyvalues.end())
		{
			if (matchStr(cname,"trigger_hurt") && matchStr(ents[i]->keyvalues["targetname"],"electro_hurt")) 
				ents[i]->keyvalues["spawnflags"] = str( atoi(ents[i]->keyvalues["spawnflags"].c_str()) | 32 ); // c2a1 crash
			if (matchStr(cname, "func_door_rotating") && matchStr(ents[i]->keyvalues["targetname"],"c3a2d_petals"))
				ents[i]->keyvalues["classname"] = cname = "func_breakable"; // anything these doors do crashes (except tiny moves) 
		}

		if (matchStr(cname,"func_breakable"))
		{
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
				int randSpeed = (int)pow(2.0,(rand()%5+7));
				int randSpeed2 = (int)pow(2.0,rand()%14);
				ents[i]->keyvalues["speed"] = to_string((_Longlong)randSpeed);
				//ents[i]->keyvalues["yaw_speed"] = to_string((_Longlong)randSpeed);
			}
		}

		if (matchStr(cname,"func_train") || matchStr(cname,"func_tracktrain"))
		{
			if (entMode == ENT_SUPER)
			{
				int randSpeed = (int)pow(2.0,(rand()%5+7));
				int randRot1 = (int)pow(2.0,rand()%10);
				int randRot2 = (int)pow(2.0,rand()%10);
				int randRot3 = (int)pow(2.0,rand()%10);
				ents[i]->keyvalues["speed"] = to_string((_Longlong)randSpeed);
			}
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
				int rotation = (int)pow(2.0,rand()%7+7);
				ents[i]->keyvalues["rotation"] = to_string((_Longlong)rotation);
			}
		}

		if (matchStr(cname,"func_conveyor") || matchStr(cname,"func_plat") || matchStr(cname,"func_platrot"))
		{
			if (entMode == ENT_SUPER)
			{
				int randSpeed = (int)pow(2.0,rand()%5+7);
				ents[i]->keyvalues["speed"] = to_string((_Longlong)randSpeed);
			}
		}

		if (matchStr(cname,"func_water"))
		{
			if (entMode == ENT_SUPER)
			{
				int randHeight = (int)pow(2.0,rand()%6);
				ents[i]->keyvalues["WaveHeight"] = to_string((_Longlong)randHeight);
			}
		}

		if (matchStr(cname,"func_rotating") || matchStr(cname,"func_rot_button"))
		{
			if (entMode == ENT_SUPER)
			{
				int randSpeed = (int)pow(2.0,rand()%8+4);
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
				int randSpeed = (int)pow(2.0,rand()%6+4);
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
				int randLip = (int)-pow(2.0,rand()%5+6);
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
			if (entMode == ENT_SUPER)
			{
				int randSpeed = (int)pow(2.0,(rand()%8+4));
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

		if (matchStr(cname,"func_button"))
		{
			if (entMode == ENT_SUPER)
			{
				if (atoi(ents[i]->keyvalues["wait"].c_str()) >= 0)
					ents[i]->keyvalues["wait"] = "1";
			}

			continue;
		}

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

		if (child.length() > 0)
			cname = child;

		if (cname.length() < 9) continue; // too short for MONSTER_X

		string prefix = getSubStr(cname,0,8);

		if (matchStr(prefix,"monster_"))
		{
			string mtype = getSubStr(cname,8);

			if (mdlMode != MDL_NONE && rand() % 3) // less effects when using model replacement
				continue;
			if (entMode == ENT_SUPER || entMode == ENT_APPEAR)
			{
				// 50% chance of weird effect
				if (rand() % 2)
				{
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
				}


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
				if (rand() % 5 == 0 && cname.find("monster_scientist") != 0 && entMode == ENT_SUPER)
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

Entity * add_new_entity(Entity** current_ents, string classname)
{
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (current_ents[i] == NULL)
		{
			current_ents[i] = new Entity();
			current_ents[i]->addKeyvalue("classname", classname);
			return current_ents[i];
		}
	}
	println("MAYDAY MAYDAY NINER NINER! ENTITY LIMIT REACHED D:::");
	return NULL;
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
