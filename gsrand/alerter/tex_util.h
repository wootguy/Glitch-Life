#include <vector>
#include "Wad.h"

struct BSP;
struct BSPTEXDATA;

using namespace std;

vector<Wad> getWads(bool defaultOnly);

void writeWad(vector<string> wadTextures, vector<Wad> wads, string mapname);

BSPTEXDATA * genTexLump(vector<string> wadTextures, vector<Wad> wads, BSP * map);

int makeMapWad(BSP * map, vector<Wad>& wads);