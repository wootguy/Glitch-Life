#include "tex_util.h"
#include "gsrand.h"

void find_all_skies(string skyPath)
{
	user_skies.clear();

	vector<string> dirs = getAllSubdirs(skyPath);

	for (uint i = 0; i < dirs.size(); i++)
	{
		vector<string> results = getDirFiles(dirs[i], "tga");
		string cpath = "";
		int dir_find = dirs[i].find("gfx/env/");
		if (dir_find != string::npos && dirs[i].length() > dir_find + string("gfx/env/").length())
			cpath = getSubStr(dirs[i], dir_find + string("gfx/env/").length()); // skip gfx/env/
		for (uint k = 0; k < results.size(); k++)
		{
			string name = getSubStr(results[k],0,results[k].length()-4);
			if (name.length() < 2)
				continue;
			string sky_name = getSubStr(name, 0, name.length()-2);
			string side = getSubStr(name, sky_name.length());
			if (matchStr(side, "lf") || matchStr(side, "rt") || matchStr(side, "dn") ||
				matchStr(side, "ft") || matchStr(side, "bk"))
				continue; // only check each "up"

			bool hasLF = false;
			bool hasRT = false;
			bool hasDN = false;
			bool hasFT = false;
			bool hasBK = false;
			for (uint i = 0; i < results.size(); ++i)
			{
				if (matchStr(sky_name + "lf.tga", results[i])) hasLF = true;
				if (matchStr(sky_name + "rt.tga", results[i])) hasRT = true;
				if (matchStr(sky_name + "dn.tga", results[i])) hasDN = true;
				if (matchStr(sky_name + "ft.tga", results[i])) hasFT = true;
				if (matchStr(sky_name + "bk.tga", results[i])) hasBK = true;
			}
			if (hasLF && hasRT && hasDN && hasFT && hasBK)
				user_skies.push_back(sky_name);	
		}
	}
}

void get_all_skies()
{
	find_all_skies("../valve/gfx/env/");
	vector<string> temp_skies = user_skies;
	find_all_skies("gfx/env/");

	insert_unique(temp_skies, user_skies);

	filter_default_content(user_skies, SKIES, NUM_SKIES);

	/*
	println("#define NUM_SKIES " + str(user_skies.size()));
	println("static const char * SKIES[NUM_STATIC_SPRITES] =");
	println("{");
	for (uint s = 0; s < user_skies.size(); s++)
		println("\t\"" + user_skies[s] + "\",");
	println("};\n");

	writeLog();
	*/
}

vector<Wad> getWads()
{
	vector<Wad> wads;
	vector<string> files = getDirFiles(wadPath, "wad");
	for (uint i = 0; i < files.size(); i++)
	{
		string prefix = getSubStr(files[i],0,6);
		if (matchStrCase(prefix,"gsrand"))
			continue;
		string wad = wadPath + files[i];
		bool is_default = false;
		for (int i = 0; i < NUM_DEFAULT_WADS; i++)
		{
			string name = string(default_wads[i]) + ".wad";
			if (matchStr(wad, name))
				is_default = true;
		}
		if (is_default && contentMode == CONTENT_CUSTOM || 
			!is_default && contentMode == CONTENT_DEFAULT)
			continue;

		wads.push_back(Wad(wad));
	}
	for (uint i = 0; i < wads.size(); i++)
	{
		//println("Caching wad " + wads[i].filename);
		wads[i].readInfo();
		//wads[i].loadCache();
	}
	return wads;
}

void make_grapple_textures(BSP * map)
{
	
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
		for (uint i = 0; i < wadTextures.size(); i++)
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
			string tex_name = wadTextures[i];
			if (barnacle_grapple_hook && tex_name.find("sky") != 0 && tex_name.find("aaatrigger") != 0)
				tex_name = "xeno_grapple"; // all textures by this name can be grappled
			for (uint k = 0; k < MAXTEXTURENAME; k++)
			{
				if (k < tex_name.length())
					newTex[i]->szName[k] = tex_name[k];
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
		if (texMode == TEX_WADS && prefixMode == PREFIX_NONE)
			filename += MAP_PREFIX;
		filename += mapname + ".wad";

		if (texMode == TEX_MASTERWAD)
			println("Writing " + filename + "...");
		newWad.write(filename, newTex, wadTextures.size());

		for (uint i = 0; i < wadTextures.size(); i++)
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
		if (prefixMode == PREFIX_NONE)
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

	if (idx + (int)sizeof(int) > len)
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
			if (idx + (int)sizeof(int) > len)
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
			if (idx + (int)sizeof(BSPMIPTEX) > len)
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
						for (uint k = 0; k < masterWadTex.size(); k++)
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
