#include "AudioSystem.h"

AudioSystem::~AudioSystem()
{
	soundEngine->drop();
}

void AudioSystem::audioPlay()
{
	soundEngine->play3D(soundSource, position);
}

void AudioSystem::audioPause()
{
	sound->setIsPaused();
}
void AudioSystem::setEngine(irrklang::ISoundEngine* engine)
{
	soundEngine = engine;
}
void AudioSystem::includeAudio(std::string filePath)
{
	soundSource = soundEngine->addSoundSourceFromFile(filePath.c_str());
}

void AudioSystem::setPosition(irrklang::vec3df pos)
{
	position = pos;
}
void AudioSystem::setVolume()
{
	sound->setVolume(volume);
}

