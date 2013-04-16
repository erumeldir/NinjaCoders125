/*
 * AudioEngine.h
 * Handles the audio playback
 */
#include "defs.h"
#include "DebugConsole.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

//error check macro for FMOD init
#define FMOD_ERRCHECK(val_check) \
	if(val_check != FMOD_OK) { \
		DC::get()->print("[Audio] FMOD ERROR! (%d) %s\n", result, FMOD_ErrorString(val_check)); \
		exit(-1); \
	}

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

	//FMOD objects
	FMOD::System	 *system;
	FMOD_RESULT		 result;
	uint			 version;
	int				 numdrivers;
	FMOD_SPEAKERMODE speakermode; //number of channels
	FMOD_CAPS	     caps;
	char			 name[256];


};
typedef AudioEngine AE;
#endif
