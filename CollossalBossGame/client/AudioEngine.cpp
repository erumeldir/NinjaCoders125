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
	std::cout << "Initializing Audio" << std::endl;
}

/*
 * Engine destructor. Deletes all audio assets and cleans all buffers.
 */
AudioEngine::~AudioEngine() {

}