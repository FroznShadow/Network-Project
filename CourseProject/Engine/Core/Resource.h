#ifndef RESOURCE_H
#define RESOURCE_H

#include "AudioSystem.h"

#include <Core/Components/Material.h>

enum TYPE
{
	Text = 1,
	Audio = 2,
	Image = 3,
	Font = 4,
	OBJ = 5,
};

class Resource
{
public:
	Resource() : audio(new AudioSystem) {};
	~Resource() {};

	std::string filepath;
	TYPE type;
	std::string scope;
	int ID;
	std::vector<int> resourceUsers;

	Engine::Material* _material;
	std::vector < glm::vec3 >  _vertices;
	std::vector < glm::vec2 >  _uvs;
	std::vector < glm::vec3 >  _normals;

	std::vector < glm::uvec3 > _indices;

	void setTextData(std::string readFile) { textData = readFile; }
	void setImageData(std::vector<unsigned char> imageFile, unsigned width, unsigned height) { std::cout << "setting image data" << std::endl; imageData = imageFile; iHeight = height; iWidth = width; }
	void setAudioData(irrklang::ISoundSource* audioFile) { std::cout << "Setting audio data " << std::endl; audioData = audioFile; }
	void setAudioEngine(irrklang::ISoundEngine* engine) { audio->setEngine(engine); }
	void setAudio(std::string filename) { audio->includeAudio(filename); }
	std::string getTextData() { return textData; }
	std::vector<unsigned char> getImageData() { return imageData; }

	int iHeight, iWidth;
	irrklang::ISoundSource* getAudioData() { return audioData; }
	AudioSystem* getAudioSystem() { return audio; }
private:
	std::string textData;
	std::vector<unsigned char> imageData;
	irrklang::ISoundSource* audioData;
	AudioSystem* audio;
};

#endif