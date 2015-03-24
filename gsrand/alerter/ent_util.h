#include "Keyvalue.h"
#include "Entity.h"
#include <string>

#define IN_ATTACK    1
#define IN_JUMP      2
#define IN_DUCK      4
#define IN_FORWARD   8
#define IN_BACK      16
#define IN_USE       32
#define IN_CANCEL    64 // ???
#define IN_LEFT      128 // turn left
#define IN_RIGHT     256 // turn right
#define IN_MOVELEFT  512
#define IN_MOVERIGHT 1024
#define IN_ATTACK2   2048 // only works if weapon has secondary fire?
#define IN_RUN       4096 // walk button
#define IN_RELOAD    8192
#define IN_ALT1      16384 // buggy?
#define IN_SCORE     32768 // scoreboard

using namespace std;

struct BSP;

Keyvalue extractKeyvalue(string line);

void ripent(BSP * map, Entity** entData, bool restore);

bool needsRipent(BSP * map, Entity** ents);

int add_gsrand_ents(Entity ** ents);

Entity ** getMapEnts(BSP * map, bool printInfo, int& numEnts);

void randomize_skybox(Entity ** ents);

void update_changelevels(Entity** ents, string mapname);

void do_entity_randomization(Entity** ents, string mapname);

Entity * add_new_entity(Entity** current_ents, string classname);

int get_weapon_id(string weapon_name);