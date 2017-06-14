#ifndef AUDIOSYSTEM_H
#define AUDIOSYSTEM_H
#include "../../Dependencies/include/irrKlang/irrKlang.h"
#include <iostream>

class AudioSystem
{
public:
	AudioSystem() {}
	~AudioSystem();

	void audioPlay();
	void audioPause();
	void includeAudio(std::string filepath);
	void setPosition(irrklang::vec3df pos);
	void setVolume();
	void AudioSystem::setEngine(irrklang::ISoundEngine* engine);

private:
	irrklang::ISoundEngine* soundEngine;
	irrklang::ISoundSource* soundSource;
	irrklang::ISound* sound;
	irrklang::vec3df position;
	int volume;
};

#endif // !AUDIOSYSTEM_H