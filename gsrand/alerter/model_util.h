#include <string>
#include "modelList.h"

struct BSP;
class Entity;
using namespace std;

void get_all_sprites();

void get_all_models();

string get_random_model(int request_model_type);

string get_random_sprite(int request_model_type);

void init_random_monster_models();

int getModelType(string entity_name);

bool parse_gmr_line(string line, string& a, string& b);
// "models" actually means models+sprites+brush entities
int count_map_models(BSP * map, Entity** ents, string path, int& total_models, int& potential_additions,
					string& gmr_path, string_hashmap& ent_models, string original_map_name);

string random_model_replace(string model);

string get_random_replacement(string model, vector<string>& replaced, vector<string>& replace_models);

vector<string> writeGMR(string new_gmr_path, string old_gmr_path, string_hashmap& ent_models, int replace_level);

string replace_entity_model(Entity * ent, string model_key, int model_type, int& potential_additions);

string replace_entity_sprite(Entity * ent, string model_key, int sprite_type, int& potential_additions);

bool is_safe_model_replacement(string classname, string model, string replacement);

void do_model_replacement(BSP * map, Entity** ents, string path, string original_map_name);

void genModelList();

void genSpriteList();