/*
 * AudioEngine.cpp
 * Handles the audio playback
 */
#include "AudioEngine.h"
#include <iostream>

//static members
AudioEngine *AudioEngine::ae;

/*
 * Engine constructor.Initialize the FMOD engine
 */
AudioEngine::AudioEngine() {
	DC::get()->print("[Audio] Initializing Audio Engine\n");
	//create our fmod system
	result = FMOD::System_Create(&system);
	FMOD_ERRCHECK(result);

	result = system->getVersion(&version);
	FMOD_ERRCHECK(result);

	DC::get()->print("[Audio] Using FMOD %d\n", version);
}

/*
 * Engine destructor. Deletes all audio assets and cleans all buffers.
 */
AudioEngine::~AudioEngine() {
	
}