#include "StdAfx.h"
#include "Wad.h"
#include "Util.h"
#include <iostream>
#include <fstream>

Wad::Wad(void)
{
	cache = NULL;
}

Wad::Wad( const string& file )
{
	this->filename = file;
	numTex = -1;
	cache = NULL;
}

Wad::~Wad(void)
{
}

void Wad::readInfo()
{
	string path = getWorkDir() + filename;
	const char * file = (path.c_str());

	if (!fileExists(file))
	{
		err(filename + " does not exist!");
		return;
	}

	ifstream fin(file, ifstream::in|ios::binary);


	//
	// WAD HEADER
	//
	fin.read((char*)&header, sizeof(WADHEADER));

	//
	// WAD DIRECTORY ENTRIES
	//
	fin.seekg(header.nDirOffset);
	numTex = header.nDir;
	dirEntries = new WADDIRENTRY[numTex];

	for (int i = 0; i < numTex; i++)
	{
		if (fin.eof()) { err("Unexpected end of file"); return; }
		fin.read((char*)&dirEntries[i], sizeof(WADDIRENTRY)); 
	}

	//println("first dir entry at " + str(header.nDirOffset) + " points to " + str(dirEntries[0].nFilePos));
	//println("Read " + str(numTex) + " Wad Directory Entries");
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
	string path = getWorkDir() + filename;
	const char * file = (path.c_str());

	if (!fileExists(file))
		return NULL;

	ifstream fin(file, ifstream::in|ios::binary);

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
		return NULL;
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
	for (int i = 0; i < szAll; i++)
		fin.read((char*)&data[i], sizeof(byte));
	
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

		for (int k = 0; k < szAll; k++)
			myFile.write ((char*)&textures[i]->data[k], sizeof(byte));
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

