#include "Wad.h"
#include "Util.h"
#include <iostream>
#include <fstream>
#include "gsrand.h"

Wad::Wad(void)
{
	cache = NULL;
	dirEntries = NULL;
}

Wad::Wad( const string& file )
{
	this->filename = file;
	numTex = -1;
	cache = NULL;
	dirEntries = NULL;
}

Wad::~Wad(void)
{
	//if (dirEntries)
	//	delete [] dirEntries;
}

bool Wad::readInfo()
{
	string path = filename;
	const char * file = (path.c_str());

	if (!fileExists(file))
	{
		err(filename + " does not exist!");
		return false;
	}

	ifstream fin(file, ifstream::in|ios::binary);
	if (!fin.good())
		return false;

	uint begin = fin.tellg();
	fin.seekg (0, ios::end);
	uint end = fin.tellg();
	uint sz = end - begin;
	fin.seekg(0, ios::beg);

	if (sz < sizeof(WADHEADER))
	{
		fin.close();
		return false;
	}

	//
	// WAD HEADER
	//
	fin.read((char*)&header, sizeof(WADHEADER)); 

	if (string(header.szMagic).find("WAD3") != 0)
	{
		fin.close();
		return false;
	}

	if (header.nDirOffset >= sz)
	{
		fin.close();
		return false;
	}

	//
	// WAD DIRECTORY ENTRIES
	//
	fin.seekg(header.nDirOffset);
	numTex = header.nDir;
	dirEntries = new WADDIRENTRY[numTex];

	bool usableTextures = false;
	for (int i = 0; i < numTex; i++)
	{
		if (fin.eof()) { println("Unexpected end of WAD"); return false; }
		fin.read((char*)&dirEntries[i], sizeof(WADDIRENTRY)); 
		if (dirEntries[i].nType == 0x43) usableTextures = true;
	}
	fin.close();

	if (!usableTextures)
	{
		delete [] dirEntries;
		if (printRejects) {
			println(filename + " was ignored because it contains no regular textures");
		}
		return false; // we can't use these types of textures (see fonts.wad as an example)
	}
	

	return true;
}

void Wad::loadCache()
{
	cache = new WADTEX*[numTex];
	for (int i = 0; i < numTex; i++)
	{
		if (dirEntries[i].nType == 0x43)
			cache[i] = readTexture(string(dirEntries[i].szName));
		else
			cache[i] = NULL;
	}
}

bool Wad::hasTexture(string name)
{
	for (int d = 0; d < header.nDir; d++)
		if (matchStr(name, dirEntries[d].szName))
			return true;
	return false;
}

WADTEX * Wad::readTexture( int dirIndex )
{
	if (dirIndex < 0 || dirIndex >= numTex)
	{
		println("invalid wad directory index");
		return NULL;
	}
	//if (cache != NULL)
		//return cache[dirIndex];
	string name = string(dirEntries[dirIndex].szName);
	return readTexture(name);
}

WADTEX * Wad::readTexture( const string& texname )
{
	string path = filename;
	const char * file = (path.c_str());

	ifstream fin(file, ifstream::in|ios::binary);
	if (!fin.good())
		return NULL;

	int idx = -1;
	for (int d = 0; d < header.nDir; d++)
	{
		if (matchStr(texname, dirEntries[d].szName))
		{
			idx = d;
			break;
		}
	}

	if (idx < 0)
	{
		fin.close();
		return NULL;
	}
	if (dirEntries[idx].bCompression)
	{
		err("OMG texture is compressed. I'm too scared to load it :<");
		return NULL;
	}
	fin.seekg(dirEntries[idx].nFilePos);

	BSPMIPTEX mtex;
	fin.read((char*)&mtex, sizeof(BSPMIPTEX));

	int w = mtex.nWidth;
	int h = mtex.nHeight;
	int sz = w*h;	   // miptex 0
	int sz2 = sz / 4;  // miptex 1
	int sz3 = sz2 / 4; // miptex 2
	int sz4 = sz3 / 4; // miptex 3
	int szAll = sz + sz2 + sz3 + sz4 + 2 + 256*3 + 2;

	byte * data = new byte[szAll];
	fin.read((char*)data, szAll);		
	
	fin.close();

	WADTEX * tex = new WADTEX;
	for (int i = 0; i < MAXTEXTURENAME; i++)
		tex->szName[i] = mtex.szName[i];
	for (int i = 0; i < MIPLEVELS; i++)
		tex->nOffsets[i] = mtex.nOffsets[i];
	tex->nWidth = mtex.nWidth;
	tex->nHeight = mtex.nHeight;
	tex->data = data;

	return tex;
}

bool Wad::write( std::string filename, WADTEX ** textures, int numTex )
{
	ofstream myFile(filename, ios::out | ios::binary | ios::trunc);

	header.szMagic[0] = 'W';
	header.szMagic[1] = 'A';
	header.szMagic[2] = 'D';
	header.szMagic[3] = '3';
	header.nDir = numTex;

	int tSize = sizeof(BSPMIPTEX)*numTex;
	for (int i = 0; i < numTex; i++)
	{
		int w = textures[i]->nWidth;
		int h = textures[i]->nHeight;
		int sz = w*h;	   // miptex 0
		int sz2 = sz / 4;  // miptex 1
		int sz3 = sz2 / 4; // miptex 2
		int sz4 = sz3 / 4; // miptex 3
		int szAll = sz + sz2 + sz3 + sz4 + 2 + 256*3 + 2;
		tSize += szAll;
	}

	header.nDirOffset = 12 + tSize;
	myFile.write ((char*)&header, sizeof(WADHEADER));

	for (int i = 0; i < numTex; i++)
	{
		BSPMIPTEX miptex;
		for (int k = 0; k < MAXTEXTURENAME; k++)
			miptex.szName[k] = textures[i]->szName[k];

		int w = textures[i]->nWidth;
		int h = textures[i]->nHeight;
		int sz = w*h;	   // miptex 0
		int sz2 = sz / 4;  // miptex 1
		int sz3 = sz2 / 4; // miptex 2
		int sz4 = sz3 / 4; // miptex 3
		int szAll = sz + sz2 + sz3 + sz4 + 2 + 256*3 + 2;
		miptex.nWidth = w;
		miptex.nHeight = h;
		miptex.nOffsets[0] = sizeof(BSPMIPTEX);
		miptex.nOffsets[1] = sizeof(BSPMIPTEX) + sz;
		miptex.nOffsets[2] = sizeof(BSPMIPTEX) + sz + sz2;
		miptex.nOffsets[3] = sizeof(BSPMIPTEX) + sz + sz2 + sz3;

		myFile.write ((char*)&miptex, sizeof(BSPMIPTEX));
		myFile.write ((char*)textures[i]->data, szAll);
	}

	int offset = 12;
	for (int i = 0; i < numTex; i++)
	{
		WADDIRENTRY entry;
		entry.nFilePos = offset;
		int w = textures[i]->nWidth;
		int h = textures[i]->nHeight;
		int sz = w*h;	   // miptex 0
		int sz2 = sz / 4;  // miptex 1
		int sz3 = sz2 / 4; // miptex 2
		int sz4 = sz3 / 4; // miptex 3
		int szAll = sz + sz2 + sz3 + sz4 + 2 + 256*3 + 2;
		entry.nDiskSize = szAll + sizeof(BSPMIPTEX);
		entry.nSize = szAll + sizeof(BSPMIPTEX);
		entry.nType = 0x43; // Texture
		entry.bCompression = false;
		entry.nDummy = 0;
		for (int k = 0; k < MAXTEXTURENAME; k++)
			entry.szName[k] = textures[i]->szName[k];
		offset += szAll + sizeof(BSPMIPTEX);

		myFile.write ((char*)&entry, sizeof(WADDIRENTRY));
	}
	
	//myFile.write ((char*)textures[0]->data, szAll);
	myFile.close();

	return true;
}

