#pragma once
#include <string>
#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>

bool alShowError();

class Sound
{
public:
    static int maxSources; // maximum simultaneously playing sounds.
    static unsigned int sources[64]; // sound channels
	static ALCdevice* device;
	static ALCcontext* context;
    unsigned int bufferID; // ID for the sound data
    int channel; // sound channel to play in (must be < maxSources)
    float listenerPos[3]; // position of the listener
    float listenerVel[3]; // velocity of the listener
    float listenerOri[3]; // orientation of the listener
    float sourcePos[3];   // position of the sound
    float sourceVel[3];	  // velocity of the sound


    /*
    	Create a new sound from a WAV or OGG file
    	soundFile - relative path to the sound file
    */
    Sound(const std::string& soundFile);
    Sound(void);
    ~Sound(void);

	// Create an Audio context and initialize sources
	static bool init();

    // Set default values for all sound channels. Always returns true.
    // TODO: Error checking
    static bool initSources();

    // Return the amount of sounds currently being played.
    static int querySources();

    // Stop all currently playing sounds
    static int stopAllSources();

    // True if this sound is currently playing; false if not
    bool isPlaying();

    // Play this sound on its assigned sound channel
    void play();

	// Play this sound at the specified volume
	void play(float volume);

    // Play this sound repeatably until stopped.
    void loop();

    // Stop this sound if it currently playing
    void stop();

    // Stop this sound if it is currently playing. The sound will start playing
    // from this point the next time play() is called
    void pause();

	// change the pitch of the sound (max seems to be 4.0f)
	void setPitch(float pitch);

private:
    // find a sound channel that has no sounds currently playing on it.
    int getFreeSource();

    // Load WAV data into an OpenAL buffer
    void loadWav(const char* filename);

    // Load OGG data into an OpenAL buffer
    void loadOgg(const char* filename);

    // True if the a sound is playing on the specified sound channel
    static bool isPlaying(int channel);

    // Stop the sound playing on the specified sound channel.
    static void stop(int channel);
};
