#include "StdAfx.h"
#include <vector>
#include "Sound.h"
#include <al/al.h>
#include <al/alc.h>
#include <al/alut.h>
#include "Util.h"
//#include <vorbis/vorbisfile.h>
//#include <vorbis/codec.h>

unsigned int Sound::sources[] = {0};
int Sound::maxSources = 64;
ALCdevice* Sound::device = NULL;
ALCcontext* Sound::context = NULL;

Sound::Sound(void)
{
}

Sound::Sound(const string& soundFile)
{
    string path = getWorkDir() + soundFile;
    const char* filename = path.c_str();

    string suffix = getSubStr(soundFile, soundFile.length() - 3);
    if (matchStr(suffix, "wav"))
        loadWav(filename);
    else if (matchStr(suffix, "ogg"))
        loadOgg(filename);
    else
        err("Unknown sound file ." + suffix);

    channel = -1;

    //println("Loaded " + soundFile);
}

void Sound::loadWav(const char* filename)
{
    if (!fileExists(filename))
    {
        err("File does not exist: " + string(filename));
        return;
    }
    ALenum format;
    ALsizei size;
    ALvoid* data;
    ALsizei freq;
    ALboolean loop;

    alGenBuffers(1, &bufferID);
    alShowError();

    alutLoadWAVFile((char*)filename, &format, &data, &size, &freq, &loop);
    alBufferData(bufferID, format, data, size, freq);
    alutUnloadWAV(format, data, size, freq);
    alShowError();
}

void Sound::loadOgg(const char* filename)
{
	/*
    if (!fileExists(filename))
    {
        err("File does not exist: " + string(filename));
        return;
    }

    ALenum format;
    ALsizei freq;
    vector<char> bufferData;

    alGenBuffers(1, &bufferID);
    alShowError();

    const int BUFFER_SIZE = 32768;
    int endian = 0;             // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    char array[BUFFER_SIZE];    // Local fixed size array (32 kb buffers)
    FILE *f;
    // Open for binary reading
    fopen_s(&f, filename, "rb");

    vorbis_info *pInfo;
    OggVorbis_File ogg;
    ov_open(f, &ogg, NULL, 0);

    // Get some information about the OGG file
    pInfo = ov_info(&ogg, -1);

    // Check the number of channels... always use 16-bit samples
    if (pInfo->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;
    // end if

    // The frequency of the sampling rate
    freq = pInfo->rate;

    do
    {
        // Read up to a buffer's worth of decoded sound data
        bytes = ov_read(&ogg, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
        // Append to end of buffer
        bufferData.insert(bufferData.end(), array, array + bytes);
    }
    while (bytes > 0);

    ov_clear(&ogg);

    alBufferData(bufferID, format, &bufferData[0], static_cast <ALsizei> (bufferData.size()), freq);
    alShowError();

    channel = -1;
	*/
}

bool Sound::init()
{
	device = alcOpenDevice(NULL); // open default device
	if (device != NULL)
	{
		context = alcCreateContext(device,NULL); // create context
		if (context != NULL)
			alcMakeContextCurrent(context); // set active context
		else
		{
			err("Audio context failure");
			return false;
		}
	}
	else
	{
		err("Audio device faliure");
		return false;
	}

	return initSources();
}

bool Sound::initSources()
{
    for (int i = 0; i < maxSources - 1; i++)
    {
        sources[i] = 0;
        alGenSources(1, &sources[i]);
        // Position of the source sound.
        float sourcePos[] = { 0.0f, 0.0f, 0.0f };

        // Velocity of the source sound.
        float sourceVel[] = { 0.0f, 0.0f, 0.0f };

        // assign default properties to each source, but do not assign a buffer yet.
        alSourcef(sources[i], AL_PITCH,    1.0f      );
        alSourcef(sources[i], AL_GAIN,     1.0f      );
        alSourcefv(sources[i], AL_POSITION, sourcePos );
        alSourcefv(sources[i], AL_VELOCITY, sourceVel );

        int error = alGetError();
        if (error != AL_NO_ERROR)
        {
            println("Sound channels: " + str(i+1));
            maxSources = i+1;
            return true;
        }
    }
    println("Sound channels: " + str(maxSources));
    return true;
}


int Sound::querySources()
{
    int active = 0;
    for (int i = 0; i < maxSources; i++)
    {
        if (Sound::isPlaying(i))
            active++;
    }
    return active;
}

int Sound::stopAllSources()
{
    int stopped = 0;
    for (int i = 0; i < maxSources; i++)
    {
        if (isPlaying(i))
        {
            stop(i);
            stopped++;
        }
    }
    return stopped;
}

void Sound::play()
{
    channel = getFreeSource();
    if (channel != -1)
    {
        alSourcei(sources[channel], AL_BUFFER, bufferID);
        alSourcei(sources[channel], AL_LOOPING, AL_FALSE);
		alSourcef(sources[channel], AL_GAIN, 1.0f);
        alSourceRewind(sources[channel]);
        alSourcePlay(sources[channel]);
    }
}

void Sound::play( float volume )
{
	channel = getFreeSource();
	if (channel != -1)
	{
		alSourcei(sources[channel], AL_BUFFER, bufferID);
		alSourcei(sources[channel], AL_LOOPING, AL_FALSE);
		alSourcef(sources[channel], AL_GAIN, volume);
		alSourceRewind(sources[channel]);
		alSourcePlay(sources[channel]);
	}
}


void Sound::loop()
{
    channel = getFreeSource();
    if (channel != -1)
    {
        alSourcei(sources[channel], AL_BUFFER, bufferID);
        alSourcei(sources[channel], AL_LOOPING, AL_TRUE);
		alSourcef(sources[channel], AL_GAIN, 1.0f);
        alSourceRewind(sources[channel]);
        alSourcePlay(sources[channel]);
    }
}

void Sound::stop()
{
    alSourceStop(sources[channel]);
}

void Sound::stop(int channel)
{
    alSourceStop(sources[channel]);
}

void Sound::pause()
{
    alSourcePause(sources[channel]);
}

void Sound::setPitch(float pitch)
{
	alSourcef(sources[channel], AL_PITCH, pitch);
}

int Sound::getFreeSource()
{
    for (int i = 0; i < maxSources; i++)
    {
        if (!Sound::isPlaying(i))
            return i;
    }
    println("Warning: There are no free audio channels to play sounds!");
    return -1;
}

bool Sound::isPlaying()
{
    int state;
    alGetSourcei(sources[channel], AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}

bool Sound::isPlaying(int channel)
{
    ALint state;
    alGetSourcei(sources[channel], AL_SOURCE_STATE, &state);
    //alShowError();
    return state == AL_PLAYING;
}

Sound::~Sound(void)
{
    alDeleteBuffers(1, &bufferID);
}

bool alShowError()
{
	int error = alGetError();
	if (error == AL_INVALID_NAME)
		err("OpenAL: Invalid Name Parameter!");
	if (error == AL_INVALID_ENUM)
		err("OpenAL: Invalid Enum Parameter!");
	if (error == AL_INVALID_VALUE)
		err("OpenAL: Invalid Enum Parameter Value!");
	if (error == AL_INVALID_OPERATION)
		err("OpenAL: Invalid Operation!");
	if (error == AL_OUT_OF_MEMORY)
		err("OpenAL: Out of Memory!");
	if (error == AL_NO_ERROR)
		return false;
	return true;
}
