#include "StdAfx.h"
#include "MDL.h"
#include <fstream>
#include "Util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

MDL::MDL(std::string file)
{
	this->path = file;
	loadMDL(file);
}

MDL::~MDL(void)
{
}

void MDL::loadMDL( std::string file )
{
	if (!fileExists(file))
	{
		println("Model does not exist: " + file);
		return;
	}
	char * buffer = loadFile(file);

	studiohdr_t * head = (studiohdr_t *)buffer;
	
	loadAnims(buffer, file);

	delete [] buffer;
}

void MDL::loadAnims( char * buffer, std::string file )
{
	studiohdr_t * head = (studiohdr_t *)buffer;
	mstudioseqdesc_t * seq = (mstudioseqdesc_t *)&buffer[head->seqindex];

	for (int i = 0; i < head->numseq; i++, seq++)
	{
		print("GOT SEQ " + string(seq->label));
	}
}

