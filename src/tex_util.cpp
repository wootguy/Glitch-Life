#include <string.h>
#include "tex_util.h"
#include "gsrand.h"
#include "bsp_util.h"

void find_all_skies(string skyPath)
{
	string last_print = str((int)user_skies.size());

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
			{
				continue; // only check each "up"
			}

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
			{
				user_skies.push_back(sky_name);	
				backspace(last_print.size());
				last_print = str((int)user_skies.size());
				print(last_print);
			}
			else if (printRejects)
			{
				backspace(last_print.size());
				last_print = "";
				println("Sky is incomplete: " + cpath + sky_name);
			}
		}
	}
}

void get_all_skies()
{
	user_skies.clear();
	print("0");
	find_all_skies("../valve/gfx/env/");
	vector<string> temp_skies = user_skies;
	find_all_skies("gfx/env/");
	insert_unique(temp_skies, user_skies);
	if (!skipUppercase)
	{
		find_all_skies("../svencoop_downloads/gfx/env/");
		insert_unique(temp_skies, user_skies);
	}

	vector<string> search_paths;

	int old_sz = user_skies.size();
	int total_count = old_sz;
	int exclude_count = 0;
	string last_print = str(old_sz);
	filter_default_content(user_skies, SKIES, NUM_SKIES, search_paths, ".tga", total_count, exclude_count, last_print);
	if (user_skies.size() != old_sz)
	{
		backspace(last_print.size());
		int nfiltered = old_sz - user_skies.size();
		print(str((int)user_skies.size()) + " (" + str(nfiltered) + " excluded)");
	}
	println("");

	/*
	println("#define NUM_SKIES " + str(user_skies.size()));
	println("static const char * SKIES[NUM_SKIES] =");
	println("{");
	for (uint s = 0; s < user_skies.size(); s++)
		println("\t\"" + user_skies[s] + "\",");
	println("};\n");

	writeLog();
	*/
}

void create_tex_embed_wad(vector<Wad>& wads)
{
	set<string> names; // existing textures in existing wads (so we don't add duplicates to our new wad)
	for (uint i = 0; i < wads.size(); ++i)
		for (int k = 0; k < wads[i].header.nDir; ++k)
			names.insert(toLowerCase(string(wads[i].dirEntries[k].szName)));

	set<string> unique;
	int num_repeat = 0;

	vector<string> bsp_paths;
	bsp_paths.push_back("maps/");
	bsp_paths.push_back("../valve/maps/");
	bsp_paths.push_back("../svencoop_downloads/maps/");

	string last_print;

	string wad_output_path = "gsrand_embedded_textures.wad";

	ofstream fout(wad_output_path, ios::out | ios::binary | ios::trunc);
	Wad output(wad_output_path);
	output.header.szMagic[0] = 'W';
	output.header.szMagic[1] = 'A';
	output.header.szMagic[2] = 'D';
	output.header.szMagic[3] = '3';
	output.header.nDir = 0;       // we'll update this once we know how many textures there are
	output.header.nDirOffset = 0; // same with this
	fout.write ((char*)&output.header, sizeof(WADHEADER));

	int total_textures = 0;
	vector<int> tex_sizes;
	vector<string> tex_names;

	uint64 last_print_time = getSystemTime();

	for (uint k = 0; k < bsp_paths.size(); k++)
	{
		vector<string> bsp_files = getDirFiles(bsp_paths[k],"bsp");

		int embed_maps = 0;
		for (uint f = 0; f < bsp_files.size(); f++)
		{
			if (bsp_files[f].find("gsrand_") == 0)
				continue;

			if (getSystemTime() - last_print_time > 1000*50)
			{
				backspace(last_print.size());
				last_print = str(total_textures) + "   " + bsp_files[f];
				print(last_print);
				last_print_time = getSystemTime();
			}

			int length;
			byte * tex_lump = loadTextureChunk(bsp_paths[k] + bsp_files[f], length);
			if (length <= 0)
				continue;
			if (!tex_lump)
			{
				println("Failed to get tex lump for: " + bsp_files[f]);
				continue;
			}
			int numTex = ((int*)tex_lump)[0];
			bool had_embed = false;
			for (int i = 0; i < numTex; i++)
			{
				if (numTex > MAX_MAP_TEXTURES)
					break;

				int offset = ((int*)tex_lump)[i+1];
				BSPMIPTEX * mip = (BSPMIPTEX*)(&tex_lump[offset]);
				if (offset + sizeof(BSPMIPTEX) > length)
					continue;
				string tex_name = toLowerCase(string(mip->szName));

				int sz = mip->nHeight*mip->nWidth;	   // miptex 0
				int sz2 = sz / 4;  // miptex 1
				int sz3 = sz2 / 4; // miptex 2
				int sz4 = sz3 / 4; // miptex 3
				int szAll = sz + sz2 + sz3 + sz4 + 2 + 256*3 + 2;

				if (mip->nOffsets[0] == 0 || mip->nOffsets[0] + offset + szAll > length)
					continue; // texture stored in external wad
				if (!tex_name.length() || tex_name.length() > MAXTEXTURENAME)
					continue;
				if (tex_name.find("xeno_grapple") == 0)
					continue; // just in case we accidently read a randomized map
				if (names.find(tex_name) != names.end() || unique.find(tex_name) != unique.end())
				{
					num_repeat++;
					continue;
				}

				had_embed = true;
				unique.insert(tex_name);
				total_textures++;
				tex_sizes.push_back(szAll);
				tex_names.push_back(mip->szName);

				fout.write((char*)mip, sizeof(BSPMIPTEX));
				fout.write((char*)&tex_lump[offset + mip->nOffsets[0]], szAll);
			}

			if (had_embed)
				embed_maps++;

			delete [] tex_lump;
		} 
	}

	backspace(last_print.size());
	println(str(total_textures));

	if (total_textures)
	{
		int offset = 12;
		for (int i = 0; i < total_textures; i++)
		{
			WADDIRENTRY entry;
			entry.nFilePos = offset;
			entry.nDiskSize = tex_sizes[i] + sizeof(BSPMIPTEX);
			entry.nSize = tex_sizes[i] + sizeof(BSPMIPTEX);
			entry.nType = 0x43; // Texture
			entry.bCompression = false;
			entry.nDummy = 0;
			memset(entry.szName, 0, MAXTEXTURENAME);
			if (!tex_names[i].size())
				tex_names[i] = "NONAME";
			for (int k = 0; k < MAXTEXTURENAME && k < tex_names[i].size(); k++)
				entry.szName[k] = tex_names[i][k];
			offset += tex_sizes[i] + sizeof(BSPMIPTEX);

			fout.write ((char*)&entry, sizeof(WADDIRENTRY));
		}

		// update the header
		fout.seekp(0);
		output.header.nDir = total_textures;
		output.header.nDirOffset = offset;
		fout.write ((char*)&output.header, sizeof(WADHEADER)); 
	}
	fout.close();

	if (!total_textures)
	{
		remove(output.filename.c_str());
		return;
	}

	if (!output.readInfo())
		println("Embedded textures wad got corrupted somehow");
	else
		wads.push_back(output);
}

vector<Wad> getWads()
{
	vector<Wad> wads;

	set<string> user_unique_wads;
	vector<string> wad_paths;
	wad_paths.push_back("");
	wad_paths.push_back("../valve/");
	wad_paths.push_back("../svencoop_downloads/");
	string last_print;

	for (uint k = 0; k < wad_paths.size(); k++)
	{
		vector<string> files = getDirFiles(wad_paths[k], "wad");
		for (uint i = 0; i < files.size(); i++)
		{
			if (files[i].find("gsrand") != string::npos)
				continue;
			string wad = files[i];
			bool is_default = false;
			for (int d = 0; d < NUM_DEFAULT_WADS; d++)
			{
				string name = string(default_wads[d]) + ".wad";
				if (matchStr(wad, name))
					is_default = true;
			}
			if ((is_default && contentMode == CONTENT_CUSTOM) || 
				(!is_default && contentMode == CONTENT_DEFAULT))
				continue;

			if (user_unique_wads.find(wad) == user_unique_wads.end())
			{
				user_unique_wads.insert(wad);
				Wad w = Wad(wad_paths[k] + wad);
				if (w.readInfo())
				{
					wads.push_back(w);
					backspace(last_print.size());
					last_print = str((int)wads.size());
					print(last_print);
				}
				else if (printRejects)
				{
					backspace(last_print.size());
					last_print = "";
					println("Invalid WAD: " + w.filename);
				}
			}
		} 
	}

	backspace(last_print.size());
	last_print = str((int)wads.size());
	println(last_print);

	return wads;
}

void make_grapple_textures(BSP * map)
{
	
}

WADTEX * load_random_texture(vector<Wad>& wads)
{
	int total_textures = 0;
	for (uint i = 0; i < wads.size(); ++i)
		total_textures += wads[i].header.nDir;

	int r = rand() % total_textures;
	int fails = 0;
	for (uint i = 0; i < wads.size(); ++i)
	{
		if (r >= wads[i].header.nDir)
		{
			if (i == wads.size()-1)
			{
				// this should never happen again. I fixed the bug. Oh god please don't happen again.
				println("The number of textures was overestimated " + str(wads.size()));
			}
			r -= wads[i].header.nDir;
		}
		else
		{
			WADDIRENTRY& de = wads[i].dirEntries[r];
			if (de.nType != 0x43 || de.nSize > MAXTEXELS)
			{
				if (fails++ > 1000)
				{
					println("Failed to find valid textures after 1000 retries");
					return NULL;
				}
				// Try a different random texture index
				i = -1;
				r = rand() % total_textures;
			}
			else
			{
				WADTEX * tex = wads[i].readTexture(r);

				int maxDim = max(256 >> textureCompression, 16);
				if (textureCompression && tex->nWidth * tex->nWidth > maxDim*maxDim)
				{
					WADTEX * ctex = new WADTEX;
					float scale = (float)maxDim / (float)max(tex->nWidth, tex->nHeight);
					ctex->nWidth = (float)tex->nWidth * scale;
					ctex->nHeight = (float)tex->nHeight * scale;
					// round to multiple of 16
					ctex->nWidth = max( ((ctex->nWidth >> 4) << 4), 16);
					ctex->nHeight = max( ((ctex->nHeight >> 4) << 4), 16);

					float xScale = (float)tex->nWidth / (float)ctex->nWidth;
					float yScale = (float)tex->nHeight / (float)ctex->nHeight;
					int w = ctex->nWidth;
					int h = ctex->nHeight;
					int sz = w*h;	   // miptex 0
					int sz2 = sz / 4;  // miptex 1
					int sz3 = sz2 / 4; // miptex 2
					int sz4 = sz3 / 4; // miptex 3
					int szAll = sz + sz2 + sz3 + sz4 + 2 + 256*3 + 2;

					int wOld = tex->nWidth;
					int hOld = tex->nHeight;
					int szOld = wOld*hOld;	 // miptex 0
					int sz2Old = szOld / 4;  // miptex 1
					int sz3Old = sz2Old / 4; // miptex 2
					int sz4Old = sz3Old / 4; // miptex 3
					int szAllOld = szOld + sz2Old + sz3Old + sz4Old + 2 + 256*3 + 2;
						
					ctex->nOffsets[0] = sizeof(BSPMIPTEX);
					ctex->nOffsets[1] = sizeof(BSPMIPTEX) + sz;
					ctex->nOffsets[2] = sizeof(BSPMIPTEX) + sz + sz2;
					ctex->nOffsets[3] = sizeof(BSPMIPTEX) + sz + sz2 + sz3;

					//println("Compressing " + string(tex->szName) + " " + str(tex->nWidth) + "x" + str(tex->nHeight) + " -> " + str(ctex->nWidth) + "x" + str(ctex->nHeight) + " " + str(xScale) + " " + str(yScale));

					// compress
					ctex->data = new byte[szAll];
					memset(ctex->data, 0, szAll);
					for (int y = 0; y < h; y++)
					{
						for (int x = 0; x < w; x++)
						{
							int dst = y*w + x;
							int src = (int)(y*yScale)*wOld + x*xScale;
							ctex->data[dst] = tex->data[src];
						}
					}

					// generate mipmaps
					int maxMipLevel = 3;
					int dWidth = w;
					int dHeight = h;
					w = w/2;
					h = h/2;
					int skip = 2;
					int mipLevel = 1;

					byte * buffer = ctex->data + sz;

					while (mipLevel <= maxMipLevel)
					{
						unsigned int idx = 0;
						for (int y = 0; y < dHeight; y += skip)
						{
							for (int x = 0; x < dWidth; x += skip)
							{
								int src = (y*dWidth + x);
								buffer[idx++] = ctex->data[src];		
							}
						}
						buffer += w*h;
						mipLevel++;
						w /= 2;
						h /= 2;
						skip *= 2;
					}
					// copy pallete and texture name
					memcpy(ctex->data + szAll - (256*3 + 2), tex->data + szAllOld - (256*3 + 2), 256*3);
					memcpy(ctex->szName, tex->szName, 16);

					delete [] tex->data;
					delete tex;
					tex = ctex;
				}
				return tex;
			}
		}
	}
	return NULL;
}

WADTEX ** loadRandomTextures(vector<string> wadTextures, vector<Wad>& wads)
{
	if (wads.size() <= 0)
	{
		println("No usable WADs!");
		return NULL;
	}

	if (wadTextures.size() > 0)
	{
		if (texMode == TEX_MASTERWAD)
			println("Loading " + str((int)wadTextures.size()) + " random textures...");

		int grapple_id = 0;
		WADTEX ** newTex = new WADTEX*[wadTextures.size()];
		for (uint i = 0; i < wadTextures.size(); i++)
		{
			newTex[i] = load_random_texture(wads);
			if (newTex[i] == NULL)
			{
				println("Failed to load random texture");
				break;
			}
			string tex_name = wadTextures[i];
			bool should_hook = grapple_mode == GRAPPLE_HOOK_ALWAYS || ((entMode == ENT_SUPER || corruptMode != CORRUPT_NONE) && grapple_mode == GRAPPLE_HOOK);
			if (texMode != TEX_MASTERWAD && should_hook && tex_name.find("sky") != 0 && tex_name.find("aaatrigger") != 0)
				tex_name = "xeno_grapple" + str(grapple_id++); // all textures by this name can be grappled
			if (grapple_id > 999)
			{
				println("Too many textures!");
				grapple_id = 0;
			}
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

void writeWad(vector<string>& wadTextures, vector<Wad>& wads, string mapname)
{
	if (wads.size() <= 0)
	{
		println("No usable WADs!");
		return;
	}

	if (wadTextures.size() > 0)
	{
		string filename = mapname + ".wad";
		res_list.insert(filename);
		super_res_list.insert(filename);

		println("\nCreating " + filename);
		print("Writing textures: ");

		ofstream fout(filename, ios::out | ios::binary | ios::trunc);
		Wad output(filename);
		output.header.szMagic[0] = 'W';
		output.header.szMagic[1] = 'A';
		output.header.szMagic[2] = 'D';
		output.header.szMagic[3] = '3';
		output.header.nDir = wadTextures.size();
		output.header.nDirOffset = 0; // update this later
		fout.write ((char*)&output.header, sizeof(WADHEADER));

		vector<int> tex_sizes;
		vector<string> tex_names;
		uint64 last_print_time = getSystemTime();
		string last_print;

		for (uint i = 0; i < wadTextures.size(); i++)
		{
			WADTEX * new_tex = load_random_texture(wads);
			if (new_tex == NULL)
			{
				println("Failed to load random textures");
				break;
			}
			string tex_name = wadTextures[i];
			memcpy(new_tex->szName, tex_name.c_str(), MAXTEXTURENAME);

			if (corruptMode != CORRUPT_NONE)
			{
				if (masterWadCorruptions.find(wadTextures[i]) != masterWadCorruptions.end())
				{
					int w = new_tex->nWidth;
					int h = new_tex->nHeight;
					int sz = w*h;	   // miptex 0
					int sz2 = sz / 4;  // miptex 1
					int sz3 = sz2 / 4; // miptex 2
					int sz4 = sz3 / 4; // miptex 3

					ctexMode = masterWadCorruptions[wadTextures[i]];

					byte * pal_data = &new_tex->data[sz + sz2 + sz3 + sz4 + 2];
					for (uint k = 0; k < 256*3; k += 3)
						convert_texture_color(*(COLOR3*)&pal_data[k]);
				}
			}

			int sz = new_tex->nHeight*new_tex->nWidth;	   // miptex 0
			int sz2 = sz / 4;  // miptex 1
			int sz3 = sz2 / 4; // miptex 2
			int sz4 = sz3 / 4; // miptex 3
			int szAll = sz + sz2 + sz3 + sz4 + 2 + 256*3 + 2;

			tex_sizes.push_back(szAll);
			tex_names.push_back(new_tex->szName);

			fout.write((char*)new_tex, sizeof(BSPMIPTEX));
			fout.write((char*)new_tex->data, szAll);

			delete [] new_tex->data;
			delete new_tex;

			if (getSystemTime() - last_print_time > 1000*50)
			{
				backspace(last_print.size());
				last_print = str(i) + " / " + str((int)wadTextures.size());
				print(last_print);
				last_print_time = getSystemTime();
			}
		}

		backspace(last_print.size());
		last_print = str((int)wadTextures.size()) + " / " + str((int)wadTextures.size());
		print(last_print);

		int offset = 12;
		for (int i = 0; i < wadTextures.size(); i++)
		{
			WADDIRENTRY entry;
			entry.nFilePos = offset;
			entry.nDiskSize = tex_sizes[i] + sizeof(BSPMIPTEX);
			entry.nSize = tex_sizes[i] + sizeof(BSPMIPTEX);
			entry.nType = 0x43; // Texture
			entry.bCompression = false;
			entry.nDummy = 0;
			for (int k = 0; k < MAXTEXTURENAME; k++)
				entry.szName[k] = tex_names[i][k];
			offset += tex_sizes[i] + sizeof(BSPMIPTEX);

			fout.write ((char*)&entry, sizeof(WADDIRENTRY));
		}

		// update the header
		fout.seekp(0);
		output.header.nDirOffset = offset;
		fout.write ((char*)&output.header, sizeof(WADHEADER)); 
	}
	else
		println("No wadTextures found");
}

BSPTEXDATA * genTexLump(vector<string> wadTextures, vector<Wad>& wads, BSP * map)
{
	WADTEX ** newTex = loadRandomTextures(wadTextures, wads);
	if (newTex != NULL)
	{
		BSPTEXDATA * texdata = new BSPTEXDATA();
		texdata->numTex = wadTextures.size();
		texdata->offset = new int[texdata->numTex];
		texdata->len = new int[texdata->numTex];

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

int makeMapWad(BSP * map, string map_name, vector<Wad>& wads)
{
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
			miptexOffset[i] = *((int*)&data[idx]);
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

			BSPMIPTEX miptex = *((BSPMIPTEX*)&data[idx]);
			//println("tex: " + string(miptex.szName));
			if (miptex.nOffsets[0] == 0 || texMode == TEX_MAP)
			{
				string tname = string(miptex.szName);
				if (texMode == TEX_MAP || (!matchStr(tname, "clip") && !matchStr(tname, "null") &&
					!matchStr(tname, "sky") && !matchStr(tname, "aaatrigger") &&
					!matchStr(tname, "bevel") && !matchStr(tname, "hint") &&
					!matchStr(tname, "origin") && !matchStr(tname, "skip") &&
					!matchStr(tname, "translucent")))
				{
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
		delete [] map->lumps[LUMP_TEXTURES];
		BSPTEXDATA * tex_lump = genTexLump(wadTextures,wads,map);

		int numTex = tex_lump->numTex;
		int new_len = (numTex+1)*sizeof(int);
		for (int i = 0; i < numTex; i++)
			new_len += tex_lump->len[i] + sizeof(BSPMIPTEX);

		map->lumps[LUMP_TEXTURES] = new byte[new_len];
		int * ilump = (int*)map->lumps[LUMP_TEXTURES];
		byte * lump = map->lumps[LUMP_TEXTURES];
		ilump[0] = numTex;
		int writeOffset = (numTex+1)*sizeof(int);
		for (int i = 0; i < numTex; i++)
		{
			ilump[i+1] = tex_lump->offset[i];
			memcpy(lump + writeOffset, tex_lump->tex[i], sizeof(BSPMIPTEX));
			writeOffset += sizeof(BSPMIPTEX);
			memcpy(lump + writeOffset, tex_lump->tex[i]->data, tex_lump->len[i]);
			writeOffset += tex_lump->len[i];
			delete [] tex_lump->tex[i]->data;
			delete tex_lump->tex[i];
		}

		delete [] tex_lump->tex;
		delete [] tex_lump->len;
		delete [] tex_lump->offset;
		delete tex_lump;

		map->header.lump[LUMP_TEXTURES].nLength = new_len;
	}

	return find;
}


// remove all map texture data so that all textures are loaded from WADs
vector<string> unEmbedAllTextures(BSP * map, int& grapple_id, int& global_id)
{
	byte * textures = map->lumps[LUMP_TEXTURES];
	int num_textures = ((int*)textures)[0];
	int lump_len = map->header.lump[LUMP_TEXTURES].nLength;

	vector<string> tex_names;
	string_hashmap animated_names; // we have to make sure animated textures use the same suffix

	if (lump_len == 0)
	{
		println("Texture lump is empty!");
		return tex_names;
	}
	if (num_textures > MAX_MAP_TEXTURES)
	{
		println("Map has too many textures!");
		return tex_names;
	}

	int new_lump_sz =  sizeof(BSPMIPTEX)*num_textures + (1 + num_textures)*sizeof(int);

	byte * new_lump = new byte[new_lump_sz];
	int * inew_lump = (int*)new_lump;
	inew_lump[0] = num_textures;

	int writeOffset = (1 + num_textures)*sizeof(int);

	for (int i = 0; i < num_textures; i++)
	{
		int offset = ((int*)textures)[i + 1]; // offset into old data
		inew_lump[i+1] = writeOffset;
		if (offset + sizeof(BSPMIPTEX) > lump_len || offset < 0)
			continue;
		BSPMIPTEX * t = (BSPMIPTEX*)&textures[offset];

		if (writeOffset + sizeof(BSPMIPTEX) > new_lump_sz)
		{
			println("TEXTURE LUMP OVERFLOW");
			break;
		}

		string name = t->szName;
		bool should_hook = grapple_mode == GRAPPLE_HOOK_ALWAYS || ((entMode == ENT_SUPER || corruptMode != CORRUPT_NONE) && grapple_mode == GRAPPLE_HOOK);
		if (should_hook && !matchStr(t->szName, "sky") && !matchStr(t->szName, "aaatrigger") && t->szName[0] != '!' &&
			t->szName[0] != '{')
		{
			string gname;
			if (masterWadRenames.find(toLowerCase(t->szName)) != masterWadRenames.end())
				gname = masterWadRenames[toLowerCase(t->szName)];
			else
			{
				gname = "xeno_grapple" + base36(grapple_id++);
				if (base36(grapple_id).length() > 3)
				{
					println("Too many textures to grapple! " + str(grapple_id));
					grapple_id = 0;
				}

				masterWadRenames[toLowerCase(t->szName)] = gname;
			}

			memcpy(t->szName, gname.c_str(), gname.length());
			t->szName[gname.length()] = '\0';
		}
		/*
		else if (!matchStr(t->szName, "sky"))
		{
			// rename it so that if the next map uses the same name it will be a different texture
			string gname;
			if (t->szName[0] == '+' || t->szName[0] == '-') // animated texture
			{
				string suffix = string(t->szName+2);
				if (animated_names.find(suffix) != animated_names.end())
					suffix = animated_names[suffix];
				else
					suffix = animated_names[suffix] = base36(global_id++);
				gname = t->szName[0];
				gname += t->szName[1] + suffix;
			}
			else if (t->szName[0] == '!' || t->szName[0] == '{' || t->szName[0] == '~')
				gname = t->szName[0] + base36(global_id++);
			else
				gname = base36(global_id++);

			memcpy(t->szName, gname.c_str(), gname.length());
			t->szName[gname.length()] = '\0';
		}
		*/
		memset(t->nOffsets, 0, sizeof(int)*4);

		memcpy(new_lump + writeOffset, t, sizeof(BSPMIPTEX));
		writeOffset += sizeof(BSPMIPTEX);
		tex_names.push_back(t->szName);
	}

	delete [] map->lumps[LUMP_TEXTURES];
	map->lumps[LUMP_TEXTURES] = new_lump;
	map->header.lump[LUMP_TEXTURES].nLength = new_lump_sz;

	return tex_names;
}

// embed textures that the map references from WAD files
void embedAllTextures(BSP * map, Entity ** ents)
{
	byte * textures = map->lumps[LUMP_TEXTURES];
	int num_textures = ((int*)textures)[0];
	int lump_len = map->header.lump[LUMP_TEXTURES].nLength;

	if (lump_len == 0)
		return println("Texture lump is empty!");
	if (num_textures > MAX_MAP_TEXTURES)
	{
		println("Map has too many textures!");
		return;
	}

	vector<Wad*> map_wads;
	for (int i = 0; i < MAX_MAP_ENTITIES; i++)
	{
		if (ents[i] == NULL)
			break;
		string cname = toLowerCase(ents[i]->keyvalues["classname"]);

		if (matchStr(cname, "worldspawn"))
		{
			if (ents[i]->hasKey("wad"))
			{
				vector<string> wads = splitString(ents[i]->keyvalues["wad"], ";");
				for (uint k = 0; k < wads.size(); ++k)
				{
					int dirfind = wads[k].find_last_of("\\/");
					if (dirfind) wads[k] = getSubStr(wads[k], dirfind+1);
					if (fileExists(wads[k]))
					{
						Wad * w = new Wad(wads[k]);
						w->readInfo();
						map_wads.push_back(w);
					}
					else if (fileExists("../valve/" + wads[k]))
					{
						Wad * w = new Wad("../valve/" + wads[k]);
						w->readInfo();
						map_wads.push_back(w);
					}
				}
			}
			if (ctexMode == CTEX_BW || ctexMode == CTEX_GREY || lightMode == LIGHT_DARK)
				ents[i]->keyvalues["skyname"] = "black";
			break;
		}
	}

	int new_lump_sz = (1 + num_textures)*sizeof(int); // num tex and tex offsets

	for (int i = 0; i < num_textures; i++)
	{
		int offset = ((int*)textures)[i + 1];
		if (offset == -1)
			continue; // apparently this is normal since a lot of maps do this
		if (offset + sizeof(BSPMIPTEX) > lump_len)
		{
			println("Bad tex offset: " + str(offset));
			continue;
		}
		BSPMIPTEX * t = (BSPMIPTEX*)&textures[offset];

		if (t->nOffsets[0] == 0 || t->nOffsets[0] + offset > lump_len || t->nWidth == 0 || t->nHeight == 0)
		{
			WADTEX * wad_tex = NULL;
			for (uint k = 0; k < map_wads.size(); k++)
			{
				Wad * w = map_wads[k];
				wad_tex = w->readTexture(t->szName);
				if (wad_tex != NULL)
					break;
			}
			if (wad_tex)
			{
				t->nWidth = wad_tex->nWidth;
				t->nHeight = wad_tex->nHeight;
				//println("loaded1: " + string(t->szName) + " " + str(t->nWidth) + "x" + str(t->nHeight));
				delete [] wad_tex->data;
				delete wad_tex;
			}
			else
			{
				//println("loaded1 failed: " + string(t->szName));
				t->nWidth = t->nHeight = 16;
				t->nOffsets[0] = 0;
			}
		}

		int sz = t->nHeight*t->nWidth;	   // miptex 0
		int sz2 = sz / 4;  // miptex 1
		int sz3 = sz2 / 4; // miptex 2
		int sz4 = sz3 / 4; // miptex 3
		int szAll = sz + sz2 + sz3 + sz4 + 2 + 256*3 + 2;

		new_lump_sz += sizeof(BSPMIPTEX) + szAll;
	}

	byte * new_lump = new byte[new_lump_sz];
	int * inew_lump = (int*)new_lump;
	inew_lump[0] = num_textures;

	int writeOffset = (1 + num_textures)*sizeof(int);

	int grapple_id = 0;
	int missing = 0;

	for (int i = 0; i < num_textures; i++)
	{
		int offset = ((int*)textures)[i + 1]; // offset into old data
		inew_lump[i+1] = writeOffset;
		if (offset + sizeof(BSPMIPTEX) > lump_len || offset < 0)
			continue;
		BSPMIPTEX * t = (BSPMIPTEX*)&textures[offset];

		int sz = t->nHeight*t->nWidth;	   // miptex 0
		int sz2 = sz / 4;  // miptex 1
		int sz3 = sz2 / 4; // miptex 2
		int sz4 = sz3 / 4; // miptex 3
		int szAll = sz + sz2 + sz3 + sz4 + 2 + 256*3 + 2;

		if (writeOffset + szAll + sizeof(BSPMIPTEX) > new_lump_sz)
		{
			println("TEXTURE LUMP OVERFLOW");
			break;
		}

		string name = t->szName;
		bool should_hook = grapple_mode == GRAPPLE_HOOK_ALWAYS || ((entMode == ENT_SUPER || corruptMode != CORRUPT_NONE) && grapple_mode == GRAPPLE_HOOK);
		if (should_hook && !matchStr(t->szName, "sky") && !matchStr(t->szName, "aaatrigger") && t->szName[0] != '!' &&
			t->szName[0] != '{')
		{
			string gname = "xeno_grapple" + str(grapple_id++);
			if (grapple_id > 999)
			{
				println("Too many textures! " + str(grapple_id));
				grapple_id -= 1000;
			}
			memcpy(t->szName, gname.c_str(), gname.length());
			t->szName[gname.length()] = '\0';
		}
		
		if (t->nOffsets[0] == 0 || t->nOffsets[0] + offset + szAll > lump_len) // texture stored in external wad
		{
			WADTEX * wad_tex = NULL;
			for (uint k = 0; k < map_wads.size(); k++)
			{
				Wad * w = map_wads[k];
				wad_tex = w->readTexture(name);
				if (wad_tex != NULL)
					break;
			}
			
			t->nOffsets[0] = sizeof(BSPMIPTEX);
			t->nOffsets[1] = sizeof(BSPMIPTEX) + sz;
			t->nOffsets[2] = sizeof(BSPMIPTEX) + sz + sz2;
			t->nOffsets[3] = sizeof(BSPMIPTEX) + sz + sz2 + sz3;
			memcpy(new_lump + writeOffset, t, sizeof(BSPMIPTEX));
			writeOffset += sizeof(BSPMIPTEX);

			if (wad_tex == NULL)
			{
				println("Couldn't load WAD texture: " + string(name));
				missing++;
			}
			else
			{
				//println("loaded2: " + string(t->szName) + " " + str(t->nWidth) + "x" + str(t->nHeight));
				if (writeOffset + szAll > new_lump_sz)
					println("MAYDAY MAYDAY NINER NINER");
				if (t->nWidth != wad_tex->nWidth || t->nHeight != wad_tex->nHeight)
					println("SUPER BAWAN " + str(t->nWidth) + "x" + str(t->nHeight) + " " + str(wad_tex->nWidth) + "x" + str(wad_tex->nHeight));
				memcpy(new_lump + writeOffset, wad_tex->data, szAll);
				delete [] wad_tex->data;
				delete wad_tex;
			}
			writeOffset += szAll;	
			//println("DID THAT");
		}
		else // texture in old data
		{
			if (t->nOffsets[0] != 40)
				println("WAT: " + str(t->nOffsets[0]));
			memcpy(new_lump + writeOffset, t, sizeof(BSPMIPTEX));
			memcpy(new_lump + writeOffset + sizeof(BSPMIPTEX), &textures[offset + sizeof(BSPMIPTEX)], szAll);
			writeOffset += sizeof(BSPMIPTEX) + szAll;
		}
	}

	if (missing)
		println("Failed to find " + str(missing) + " textures");

	delete [] map->lumps[LUMP_TEXTURES];
	map->lumps[LUMP_TEXTURES] = new_lump;
	map->header.lump[LUMP_TEXTURES].nLength = new_lump_sz;

	for (uint i = 0; i < map_wads.size(); i++)
	{
		if (map_wads[i]->dirEntries)
			delete [] map_wads[i]->dirEntries;
		delete map_wads[i];
	}
}
