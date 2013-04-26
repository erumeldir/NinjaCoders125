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
	//start up fmod
	int initResult = startFMOD();

	//handle init success/failure
	switch(initResult) {
	case -1:
		DC::get()->print("[Audio] ERROR: Critical FMOD error. Exiting\n");
		exit(-1); //for now just quit if critical error
		break;
	case 0:
		DC::get()->print("[Audio] WARNING: Init failed - no audio.\n");
		break;
	case 1:
		DC::get()->print("[Audio] Init successful");
	}	

}

/*
 * Engine destructor. Deletes all audio assets and cleans all buffers.
 */
AudioEngine::~AudioEngine() {
	
}

/*
 * Creates, and initializes the fmod engine
 * Returns: -1 if encounterd a critical error
 *           0 for unsuccssful init and no audio
 *           1 for init successful
 */
int AudioEngine::startFMOD() {

	bool fmodErrThrown = false;
	bool computerHasAudio = true;

	DC::get()->print("[Audio] Initializing Audio Engine\n");
	//create our fmod system
	result = FMOD::System_Create(&system);
	FMOD_ERRCHECK(result);
	if(fmodErrThrown)
		return -1;

	result = system->getVersion(&version);
	FMOD_ERRCHECK(result);
	if(fmodErrThrown)
		return -1;
	

	DC::get()->print("[Audio] Using FMOD %d\n", version);

	//ensure the computer's fmod version is correct
	if(version < FMOD_VERSION)
	{
		DC::get()->print("[Audio] ERROR! You are using an old version of FMOD %d. This program requires %d.\n",version,FMOD_VERSION);
		return -1;
	}

	result = system->getNumDrivers(&numdrivers);
	FMOD_ERRCHECK(result);

	if(numdrivers == 0)
	{
		result = system->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
		DC::get()->print("[Audio] WARNING: No driver found. Sound not enabled\n.");
		computerHasAudio = false;
		FMOD_ERRCHECK(result);
		if(fmodErrThrown)
			return -1;
	}
	else
	{
		result = system->getDriverCaps(0, &caps, 0, &speakermode);
		FMOD_ERRCHECK(result);
		if(fmodErrThrown)
			return -1;
	}

	//set the user selected speaker mode
	result = system->setSpeakerMode(speakermode);
	FMOD_ERRCHECK(result);
	if(fmodErrThrown)
		return -1;

	DC::get()->print("[Audio] Checking for hardware emulation...\n");

	if(caps & FMOD_CAPS_HARDWARE_EMULATED)
	{
		//the user has the hardware acceleration set to off! this is really bad for latency
		result = system->setDSPBufferSize(1024, 10);
		DC::get()->print("[Audio] WARNING: Hardware acceleration off!\n");
		FMOD_ERRCHECK(result);
		if(fmodErrThrown)
			return -1;
	}

	//get the driver info
	result = system->getDriverInfo(0, drivername, 256, 0);
	FMOD_ERRCHECK(result);
	if(fmodErrThrown)
		return -1;

	DC::get()->print("[Audio] Driver: %s\n\n", drivername);

	if(strstr(drivername, "SigmaTel"))
	{
		//SigmaTel devices crackle if format is PCM 16bit
		//set to PCM floating point to fix
		result = system->setSoftwareFormat(48000,FMOD_SOUND_FORMAT_PCMFLOAT, 0, 0, FMOD_DSP_RESAMPLER_LINEAR);
		FMOD_ERRCHECK(result)
		if(fmodErrThrown)
			return -1;
	}

	//System::init(Max channels, init flags, extra driver data) only change max channels/flags
	result = system->init(100, FMOD_INIT_NORMAL | FMOD_INIT_VOL0_BECOMES_VIRTUAL, 0);
	if(result == FMOD_ERR_OUTPUT_CREATEBUFFER)
	{
		//selected speaker mode not supported by the soundcard, switch back to stereo and retry init
		result = system->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
		fmodErrThrown = false;
		result = system->init(100, FMOD_INIT_NORMAL, 0);
	}
	FMOD_ERRCHECK(result);
	if(fmodErrThrown)
		return -1;

	if(computerHasAudio)
		return 1;
	else
		return 0;
}
