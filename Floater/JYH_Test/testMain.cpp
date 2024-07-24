#include "../External/include/fmod/fmod.hpp"
#include "../External/include/fmod/fmod_errors.h"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>

#pragma comment(lib, "../External/lib/x64/release/fmod_vc.lib")
#pragma comment(lib, "../External/lib/x64/release/fmodL_vc.lib")

class FMODInstance {
private:
	FMOD::System* system;
	int driverIndex;

public:
	FMODInstance(int driverIdx) : system(nullptr), driverIndex(driverIdx) {
		FMOD_RESULT result = FMOD::System_Create(&system);
		if (result != FMOD_OK) {
			std::cout << "FMOD system creation failed. Error: " << FMOD_ErrorString(result) << std::endl;
			return;
		}

		result = system->setDriver(driverIndex);
		if (result != FMOD_OK) {
			std::cout << "Failed to set driver. Error: " << FMOD_ErrorString(result) << std::endl;
			return;
		}

		result = system->init(32, FMOD_INIT_NORMAL, nullptr);
		if (result != FMOD_OK) {
			std::cout << "FMOD system initialization failed. Error: " << FMOD_ErrorString(result) << std::endl;
			return;
		}

		// Set master volume
		FMOD::ChannelGroup* masterGroup;
		system->getMasterChannelGroup(&masterGroup);
		masterGroup->setVolume(1.0f);
	}

	~FMODInstance() {
		if (system) {
			system->release();
		}
	}

	FMOD::System* getSystem() { return system; }
	int getDriverIndex() const { return driverIndex; }

	FMOD_RESULT playSound(const char* filename) {
		FMOD::Sound* sound = nullptr;
		FMOD::Channel* channel = nullptr;
		FMOD_RESULT result;

		result = system->createSound(filename, FMOD_DEFAULT, nullptr, &sound);
		if (result != FMOD_OK) {
			std::cout << "Failed to create sound. Error: " << FMOD_ErrorString(result) << std::endl;
			return result;
		}

		result = system->playSound(sound, nullptr, false, &channel);
		if (result != FMOD_OK) {
			std::cout << "Failed to play sound. Error: " << FMOD_ErrorString(result) << std::endl;
			sound->release();
			return result;
		}

		// Check if the sound is actually playing
		bool isPlaying;
		channel->isPlaying(&isPlaying);
		std::cout << "Sound is playing on driver " << driverIndex << ": " << (isPlaying ? "Yes" : "No") << std::endl;

		return FMOD_OK;
	}

	void update() {
		if (system) {
			system->update();
		}
	}
};

class SoundEngine {
private:
	std::vector<std::unique_ptr<FMODInstance>> instances;

public:
	void addInstance(int driverIndex) {
		instances.push_back(std::make_unique<FMODInstance>(driverIndex));
	}

	void playAllSounds(const char* filename) {
		for (auto& instance : instances) {
			instance->playSound(filename);
		}
	}

	void updateAll() {
		for (auto& instance : instances) {
			instance->update();
		}
	}
};

int main() {
	SoundEngine engine;

	// Add instances for different drivers
	engine.addInstance(0);  // Default driver
	engine.addInstance(4);  // Second driver (if available)

	// Play sound on all instances
	engine.playAllSounds("../Resources/Sound/ingame/prism.mp3");

	// Main loop
	for (int i = 0; i < 10000; i++) {  // Run for a while to hear the sound
		engine.updateAll();
		// Add a small delay if needed
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	return 0;
}
