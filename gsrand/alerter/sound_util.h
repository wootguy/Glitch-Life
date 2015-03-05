#include "soundlists.h"
#include "sentences.h"
#include "masterList.h"
#include <vector>
using namespace std;

class Entity;

const int num_exts = 16;
static const char * exts[num_exts] = {"AIFF", "ASF", "DLS", "FLAC", "IT", "M3U", "MID", "MOD", 
						 "MP2",	 "MP3", "OGG", "S3M", "VAG", "WAV", "WMA", "XM"};

string get_random_sound();

void getAllSounds();

void getAllVoices();

void genSoundList();

vector<sound> getReplacableSounds(bool printInfo);

void writeMonsterSoundLists(string mapname);

void writeGSR(string filename, vector<sound> writeList);

string constructSentence();

void writeSentences(string mapName);

void formatSentences();

void do_ent_sounds(Entity** ents, string mapname);