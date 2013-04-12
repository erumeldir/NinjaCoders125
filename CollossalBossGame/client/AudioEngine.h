/*
 * AudioEngine.h
 * Handles the audio playback
 */
#include "defs.h"

#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

class AudioEngine {
public:
	static void init() { ae = new AudioEngine(); }
	static AudioEngine *get() { return ae; }
	static void clean() { delete ae; }

private:
	//Constructors/destructors are private
	AudioEngine();
	virtual ~AudioEngine();

	static AudioEngine *ae;	//instance
};
typedef AudioEngine AE;

#endif
