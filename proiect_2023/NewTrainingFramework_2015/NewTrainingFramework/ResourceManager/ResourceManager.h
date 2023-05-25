#pragma once
#include "../rapidxml.hpp"
#include "../Resources.h"
#include "../Vertex.h"
#include "../Shaders.h"
#include <GLES2/gl2.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <vector>

using namespace std;
using namespace rapidxml;

namespace resourceManager {
	const string c_pathToResourcesPacket{ "../../NewResourcesPacket/" };
	const string c_pathToXmlFile{ c_pathToResourcesPacket + "XmlFiles/resourceManager.xml" };
	const string c_pathToModelsFolder{ c_pathToResourcesPacket };
	const string c_pathToShadersFolder{ c_pathToResourcesPacket };
	const string c_pathToTexturesFolder{ c_pathToResourcesPacket };

	class ResourceManager {
	private:
		static ResourceManager* rmInstance;
		map<GLuint, ModelResource*> modelResources;
		map<GLuint, ShaderResource*> shaderResources;
		map<GLuint, TextureResource*> textureResources;
		map<GLuint, Model*> models;
		map<GLuint, Shader*> shaders;
		map<GLuint, Texture*> textures;

		ResourceManager() {};

		/// <summary>
		/// These functions are used to load all the resources needed from the
		/// ResourceManager XML file.
		/// </summary>
		void ReadConfigFile();
		void GetResourceData(xml_node<>* root, const char* name, ModelResource* resource);
		void GetResourceData(xml_node<>* root, const char* name, ShaderResource* resource);
		void GetResourceData(xml_node<>* root, const char* name, TextureResource* resource);
		GLenum GetTextureType(string& type);
		GLenum GetFilterMode(string& type);
		GLenum GetWrapMode(string& type);

	public:
		static ResourceManager* GetInstance();
		void Init(); //initializari - pot fi si in constructor

		/// <summary>
		/// These functions are used to load only resorces asked by the SceneManager
		/// class. If the resource was not loaded before it creates it and loads it
		/// in the computer.
		/// </summary>
		/// <param name="id">Id of the resource needed</param>
		/// <returns>A pointer to the resource</returns>
		Model* LoadModel(const GLuint id);
		Shader* LoadShader(const GLuint id);
		Texture* LoadTexture(const GLuint id);

		Model* GenerateModel(GLuint* id, GLint cellCount, GLfloat dimCell, GLfloat offsetY);
		void LoadData(const GLuint id, string& resource);
		void FreeResources();//eliberarea zonelor de memorie alocate
		// void UpdateModelUV(GLuint id, int param, GLfloat offset);

		// Getters
		map<GLuint, Model*> GetModels() { return this->models; }
		GLint GetSkyTextureId();
	};
}

