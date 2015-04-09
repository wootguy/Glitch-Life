#include "Entity.h"
#include "Wad.h"

struct BSP;

void corrupt_map_verts(BSP * map, Entity ** ents);

void corrupt_map_lightmap(BSP * map);

void corrupt_map_textures(BSP * map, Entity ** ents);

void randomize_bsp_models(BSP * map, Entity ** ents);

void convert_texture_color(COLOR3& c);

#define NUM_BRUSH_ENTS 25
static const char * brush_ents[NUM_BRUSH_ENTS] =
{
	"func_breakable",
	"func_button",
	"func_conveyor",
	"func_door",
	"func_door_rotating",
	"func_group",
	"func_illusionary",
	"func_pendulum",
	"func_plat",
	"func_platrot",
	"func_pushable",
	"func_rotating",
	"func_rot_button",
	"func_tank",
	"func_tanklaser",
	"func_tankmortar",
	"func_tankrocket",
	"func_tracktrain",
	"func_train",
	"func_wall",
	"func_wall_toggle",
	"momentary_door",
	"momentary_rot_button",
	"func_trackchange",
	"func_trackautochange"
};