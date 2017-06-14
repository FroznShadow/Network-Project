#include "ResourceManager.h"
#include <string>
#include <fstream>
#include <Windows.h>
#include "..\Lodepng\lodepng.h"
#include <Core/Components/Material.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <..\Dependencies\include\tiny_obj_loader\tiny_obj_loader.h>

ResourceManager* ResourceManager::Instance = nullptr;

ResourceManager* ResourceManager::GetInstance()
{
	if (Instance == nullptr)
	{
		std::cout << "Creating a new resource manager. " << std::endl;
		Instance = new ResourceManager;

	}
	return Instance;
}
Resource* ResourceManager::LoadResource(std::string filepath)
{
	for (auto i : _resources)
	{
		if (i->filepath == filepath)
		{
			std::cout << "resource already loaded" << std::endl;
			i->resourceUsers.push_back(1);
			return i;
		}
	}

	//TextFile
	if (filepath.substr(filepath.size() - 4) == ".txt" || filepath.substr(filepath.size() - 3) == ".vs" || filepath.substr(filepath.size() - 3) == ".fs")
	{
		if (filepath.substr(filepath.size() - 3) == ".vs" || filepath.substr(filepath.size() - 3) == ".fs")
		{
			std::cout << "Loading shader file" << std::endl;
			// load shader file
		}
		std::cout << "A text file" << std::endl;
		Resource* res = new Resource();
		res = ResourceManager::LoadTextResource(filepath);
		return res;
	}
	//ImageFile
	else if (filepath.substr(filepath.size() - 4) == ".png")
	{
		std::cout << "An image file" << std::endl;

		Resource * res = new Resource();
		res = ResourceManager::LoadImageResource(filepath);
		return res;
	}
	//AudioFile
	else if (filepath.substr(filepath.size() - 4) == ".wav")
	{
		std::cout << "An audio file" << std::endl;

		Resource *res = new Resource();
		res = ResourceManager::LoadAudioResource(filepath);

		return res;
	}
	//FontFile
	else if (filepath.substr(filepath.size() - 4) == ".ttf")
	{
		std::cout << "a font file" << std::endl;


		unsigned size = strlen(filepath.c_str()) + 1; // +1 to include NULL
		wchar_t* wc = new wchar_t[size];
		unsigned outSize;
		mbstowcs_s(&outSize, wc, size, filepath.c_str(), size - 1);

		AddFontResourceEx(wc, FR_PRIVATE, 0);

		Resource *res = new Resource();
		res = ResourceManager::LoadFontResource(filepath);
		return res;
	}
	else if (filepath.substr(filepath.size() - 4) == ".obj")
	{
		std::cout << "An object file" << std::endl;

		Resource *res = new Resource();
		res = ResourceManager::LoadObjectResource(filepath);

		return res;
	}
	//UnknownFile
	else
	{
		std::cout << "filetype not regognized" << std::endl;
		return 0;
	}
}
void ResourceManager::UnLoadResource(int ID)
{

	_it = _resources.begin();
	while (_it != _resources.end())
	{
		if ((*_it)->ID == ID)
		{
			std::cout << "Deleting resource" << std::endl;
			(*_it)->resourceUsers.pop_back();
			if ((*_it)->resourceUsers.size() <= 0)
			{
				std::cout << "unloading resource" << std::endl;
				_it = _resources.erase(_it);
			}
			else
				_it++;
		}
		else
			_it++;
	}
}
int ResourceManager::GetResourceUsers(int ID)
{
	_it = _resources.begin();
	while (_it != _resources.end())
	{
		if ((*_it)->ID == ID)
		{
			return (*_it)->resourceUsers.size();
		}
		_it++;
	}
	std::cout << "Resource not found" << std::endl;
	return 0;
}

Resource* ResourceManager::LoadTextResource(std::string filepath)
{
	std::cout << "Loading Resource" << std::endl;
	Resource * res = new Resource;
	res->filepath = filepath;
	res->type = Text;
	res->ID = ++ID_generator;
	res->resourceUsers.push_back(1);

	std::ifstream file(filepath, std::ifstream::binary);
	if (file.is_open())
	{
		// Get size of file///////////
		file.seekg(0, file.end);	//
		int size = file.tellg();	//
		file.seekg(0, file.beg);	//

									// Allocate buffer for text/////
		char* buffer = new char[size];//

									  // Read file//////////////////
		file.read(buffer, size);	// 
		buffer[size] = NULL;		//
		file.close();				//

									//Change text into string/////
		std::string readFile;		//
		readFile = buffer;			//

									//Set text to resource////////
		res->setTextData(readFile);//

	}
	_resources.push_back(res);
	return res;
}
Resource* ResourceManager::LoadImageResource(std::string filepath)
{

	std::cout << "Loading resource" << std::endl;
	unsigned width, height;

	Resource * res = new Resource();
	res->filepath = filepath;
	res->type = Image;
	res->ID = ++ID_generator;
	res->resourceUsers.push_back(1);

	std::vector<unsigned char> image;
	unsigned error = lodepng::decode(image, width, height, filepath);

	res->setImageData(image, width, height);

	_resources.push_back(res);
	return res;
}
Resource* ResourceManager::LoadAudioResource(std::string filepath)
{
	std::cout << "Loading Resource" << std::endl;

	Resource *res = new Resource();
	res->filepath = filepath;
	res->type = Audio;
	res->ID = ++ID_generator;
	res->resourceUsers.push_back(1);


	res->setAudioEngine(soundEngine);

	res->setAudio(filepath);
	_resources.push_back(res);
	return res;
}
Resource* ResourceManager::LoadFontResource(std::string filepath)
{
	std::cout << "Loading Resource" << std::endl;

	Resource *res = new Resource();
	res->filepath = filepath;
	res->type = Font;
	res->ID = ++ID_generator;
	res->resourceUsers.push_back(1);

	_resources.push_back(res);
	return res;
}
Resource* ResourceManager::LoadObjectResource(std::string filepath)
{

	std::cout << "Loading Resource" << std::endl;

	Resource *res = new Resource();
	res->filepath = filepath;
	res->type = OBJ;
	res->ID = ++ID_generator;
	res->resourceUsers.push_back(1);

	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector< glm::vec3 > temp_vertices;
	std::vector< glm::vec2 > temp_uvs;
	std::vector< glm::vec3 > temp_normals;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string err;
	std::string filedir = filepath.substr(0, filepath.find_last_of('/') + 1);
	tinyobj::LoadObj(shapes, materials, err, filepath.c_str(), filedir.c_str());

	if (!err.empty()) {
		std::cout << err << std::endl;
	}


	for (size_t i = 0; i < shapes.size(); i++) {
		size_t offset = res->_vertices.size();
		for (size_t j = 0; j < shapes[i].mesh.positions.size() / 3; j++)
		{
			res->_vertices.push_back(glm::vec3(shapes[i].mesh.positions[3 * j + 0], shapes[i].mesh.positions[3 * j + 1], shapes[i].mesh.positions[3 * j + 2]));
			res->_vertices.push_back(glm::vec3(shapes[i].mesh.texcoords[2 * j + 0], shapes[i].mesh.texcoords[2 * j + 1], 0.0f));
			res->_vertices.push_back(glm::vec3(shapes[i].mesh.normals[3 * j + 0], shapes[i].mesh.normals[3 * j + 1], shapes[i].mesh.normals[3 * j + 2]));
		}
		for (size_t j = 0; j < shapes[i].mesh.indices.size() / 3; j++) {
			res->_indices.push_back(glm::uvec3(shapes[i].mesh.indices[3 * j + 0] + offset, shapes[i].mesh.indices[3 * j + 1] + offset, shapes[i].mesh.indices[3 * j + 2] + offset));
		}
	}


	for (size_t i = 0; i < materials.size(); i++) {
		res->_material = new Engine::Material(glm::vec3(materials[i].emission[0], materials[i].emission[1], materials[i].emission[2]),
			glm::vec3(materials[i].ambient[0], materials[i].ambient[1], materials[i].ambient[2]),
			glm::vec3(materials[i].diffuse[0], materials[i].diffuse[1], materials[i].diffuse[2]),
			glm::vec3(materials[i].specular[0], materials[i].specular[1], materials[i].specular[2]),
			glm::vec3(materials[i].transmittance[0], materials[i].transmittance[1], materials[i].transmittance[2]),
			materials[i].ior, materials[i].shininess, materials[i].dissolve, materials[i].illum, materials[i].dummy);
	}












	//FILE * file = fopen(filepath.c_str(), "r");
	//if (file == NULL){
	//	printf("Impossible to open the file !\n");
	//	return false;
	//}

	//while (1){

	//	char lineHeader[128];
	//	// read the first word of the line
	//	int resource = fscanf(file, "%s", lineHeader);
	//	if (resource == EOF)
	//		break; // EOF = End Of File. Quit the loop.

	//	if (strcmp(lineHeader, "v") == 0){
	//		glm::vec3 vertex;
	//		fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
	//		temp_vertices.push_back(vertex);

	//	}
	//	else if (strcmp(lineHeader, "vt") == 0){
	//		glm::vec2 uv;
	//		fscanf(file, "%f %f\n", &uv.x, &uv.y);
	//		temp_uvs.push_back(uv);

	//	}
	//	else if (strcmp(lineHeader, "vn") == 0){
	//		glm::vec3 normal;
	//		fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
	//		temp_normals.push_back(normal);

	//	}
	//	else if (strcmp(lineHeader, "f") == 0)
	//	{
	//		std::string vertex1, vertex2, vertex3;
	//		unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
	//		int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
	//		//int matches = fscanf(file, "%d//%d %d//%d %d//%d\n", &vertexIndex[0], &normalIndex[0], &vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);

	//		if (matches != 9){
	//			printf("File can't be read by our simple parser : ( Try exporting with other options\n");
	//			return false;
	//		}
	//		vertexIndices.push_back(vertexIndex[0]);
	//		vertexIndices.push_back(vertexIndex[1]);
	//		vertexIndices.push_back(vertexIndex[2]);
	//		uvIndices.push_back(uvIndex[0]);
	//		uvIndices.push_back(uvIndex[1]);
	//		uvIndices.push_back(uvIndex[2]);
	//		normalIndices.push_back(normalIndex[0]);
	//		normalIndices.push_back(normalIndex[1]);
	//		normalIndices.push_back(normalIndex[2]);
	//	}
	//	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	//	{
	//		unsigned int vertexIndex = vertexIndices[i];
	//		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
	//		res->_vertices.push_back(vertex);
	//	}
	//	for (unsigned int i = 0; i < uvIndices.size(); i++)
	//	{
	//		unsigned int uvIndex = uvIndices[i];
	//		glm::vec2 uv = temp_uvs[uvIndex - 1];
	//		res->_uvs.push_back(uv);
	//	}
	//	for (unsigned int i = 0; i < normalIndices.size(); i++)
	//	{
	//		unsigned int normalIndex = normalIndices[i];
	//		glm::vec3 normal = temp_normals[normalIndex - 1];
	//		res->_normals.push_back(normal);
	//	}
	//}


	_resources.push_back(res);
	return res;
}
