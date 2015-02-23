#pragma once
#include "studio.h"
#include "Globals.h"
#include <vector>
#include <string>

typedef unsigned char byte;

#define MAX_VERTEX_WEIGHTS 8

#define MDL_HIT_GENERIC  0
#define MDL_HIT_HEAD     1
#define MDL_HIT_CHEST    2
#define MDL_HIT_STOMACH  3
#define MDL_HIT_LEFTARM  4
#define MDL_HIT_RIGHTARM 5
#define MDL_HIT_LEFTLEG  6
#define MDL_HIT_RIGHTLEG 7
#define MDL_HIT_ARMOR    10
#define MDL_HIT_HELMET   11

#define ACT_IDLE			1
#define ACT_WALK			3
#define ACT_FALL			9
#define ACT_TURN_LEFT		15
#define ACT_TURN_RIGHT		16
#define ACT_SMALL_FLINCH	26
#define ACT_BIG_FLINCH		27
#define ACT_MELEE_ATTACK1	30
#define ACT_DIE_SIMPLE		36
#define ACT_DIE_BACKWARD	37
#define ACT_DIE_FORWARD		38
#define ACT_DIE_HEADSHOT	66
#define ACT_DIE_GUTSHOT		68
#define ACT_FLINCH_LEFTARM  73
#define ACT_FLINCH_RIGHTARM 74
#define ACT_FLINCH_LEFTLEG  75
#define ACT_FLINCH_RIGHTLEG 76
#define ACT_TOTAL			77

class MDL
{
public:
	std::string path;

	MDL(std::string file);
	~MDL(void);
	
	void loadMDL(std::string file);

private:
	void loadAnims(char * buffer, std::string file);
};

