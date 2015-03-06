#include <vector>
#include "Wad.h"

struct BSP;
struct BSPTEXDATA;

using namespace std;

vector<Wad> getWads();

void get_all_skies();

void writeWad(vector<string> wadTextures, vector<Wad> wads, string mapname);

BSPTEXDATA * genTexLump(vector<string> wadTextures, vector<Wad> wads, BSP * map);

int makeMapWad(BSP * map, vector<Wad>& wads);