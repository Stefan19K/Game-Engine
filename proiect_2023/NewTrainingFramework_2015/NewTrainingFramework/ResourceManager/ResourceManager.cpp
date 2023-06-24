#include "stdafx.h"
#include "ResourceManager.h"
#include "../Utilities/memDbg.h"

using namespace resourceManager;

ResourceManager* ResourceManager::rmInstance = nullptr;

void ResourceManager::ReadConfigFile()
{
	xml_document<> doc;
	ifstream file(c_pathToXmlFile);
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	string content(buffer.str());
	doc.parse<0>(&content[0]);
	xml_node<>* pRoot = doc.first_node();
	for (xml_node<>* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling())
	{
		char *name = pNode->first_node()->name();
		if (string(pNode->name()) == "models") {
			ModelResource resource;
			GetResourceData(pNode, name, &resource);
		}

		if (string(pNode->name()) == "shaders") {
			ShaderResource resource;
			GetResourceData(pNode, name, &resource);
		}

		if (string(pNode->name()) == "textures") {
			TextureResource resource;
			GetResourceData(pNode, name, &resource);
		}
	}
}

void ResourceManager::GetResourceData(xml_node<>* root, const char* name, ModelResource* resource)
{
	if (!root)
		return;

	for (xml_node<>* pNode = root->first_node(name); pNode; pNode = pNode->next_sibling(name))
	{
		xml_node<>* auxNode = pNode->first_node();
		if (!auxNode)
			return;

		if (string(name) == "folder") {
			xml_attribute<>* pAttr = pNode->first_attribute("path");
			resource->path = pAttr->value();
		}
		else if (string(name) == "model") {
			xml_attribute<>* pAttr = pNode->first_attribute("id");
			resource->id = atoi(pAttr->value());
		}
		else if (string(name) == "file") {
			resource->file = pNode->value();
			ModelResource *res = new ModelResource( resource->id, resource->path, resource->file);
			modelResources.insert(pair<GLuint, ModelResource*>(resource->id, res));
			//resource->print();
		}

		GetResourceData(pNode, auxNode->name(), resource);
	}
}

void ResourceManager::GetResourceData(xml_node<>* root, const char* name, ShaderResource* resource)
{
	if (!root)
		return;

	for (xml_node<>* pNode = root->first_node(name); pNode; pNode = pNode->next_sibling(name))
	{
		xml_node<>* auxNode = pNode->first_node();
		if (!auxNode)
			return;

		if (string(name) == "folder") {
			xml_attribute<>* pAttr = pNode->first_attribute("path");
			resource->path = pAttr->value();
		}
		else if (string(name) == "shader") {
			xml_attribute<>* pAttr = pNode->first_attribute("id");
			resource->id = atoi(pAttr->value());

			GetResourceData(pNode, auxNode->name(), resource);
			GetResourceData(pNode, auxNode->next_sibling()->name(), resource);
			continue;
		}
		else if (string(name) == "vs") {
			resource->vertexShader = pNode->value();
		} else if (string(name) == "fs") {
			resource->fragmentShader = pNode->value();
			ShaderResource* res = new ShaderResource(resource->id, resource->path, resource->vertexShader, resource->fragmentShader);
			shaderResources.insert(pair<GLuint, ShaderResource*>(resource->id, res));
			//resource->print();
		}

		GetResourceData(pNode, auxNode->name(), resource);
	}
}

void ResourceManager::GetResourceData(xml_node<>* root, const char* name, TextureResource* resource)
{
	if (string(name) == "reversed") {
		resource->reversed = true;
	}

	if (!root)
		return;

	for (xml_node<>* pNode = root->first_node(name); pNode; pNode = pNode->next_sibling(name))
	{
		xml_node<>* auxNode = pNode->first_node();
		if (!auxNode)
			return;

		if (string(name) == "folder") {
			xml_attribute<>* pAttr = pNode->first_attribute("path");
			resource->path = pAttr->value();
		}
		else if (string(name) == "texture") {
			resource->reversed = false;

			xml_attribute<>* pAttr = pNode->first_attribute("id");
			resource->id = atoi(pAttr->value());

			pAttr = pNode->first_attribute("type");
			resource->type = GetTextureType(string(pAttr->value()));

			while (auxNode) {
				GetResourceData(pNode, auxNode->name(), resource);
				auxNode = auxNode->next_sibling();
			}

			continue;
		} else if (string(name) == "file") {
			resource->file = pNode->value();
		} else if (string(name) == "min_filter") {
			resource->minFilter = GetFilterMode(string(pNode->value()));
		} else if (string(name) == "mag_filter") {
			resource->magFilter = GetFilterMode(string(pNode->value()));
		} else if (string(name) == "wrap_s") {
			resource->wrapS = GetWrapMode(string(pNode->value()));
		} else if (string(name) == "wrap_t") {
			resource->wrapT = GetWrapMode(string(pNode->value()));
			TextureResource* res = new TextureResource(
				resource->id,
				resource->reversed,
				resource->path,
				resource->type,
				resource->file,
				resource->minFilter,
				resource->magFilter,
				resource->wrapS,
				resource->wrapT
			);
			textureResources.insert(pair<GLuint, TextureResource*>(resource->id, res));
			//resource->print();
		}

		GetResourceData(pNode, auxNode->name(), resource);
	}
}

GLenum ResourceManager::GetTextureType(string& type)
{
	if (type == "2d")
		return GL_TEXTURE_2D;

	if (type == "cube")
		return GL_TEXTURE_CUBE_MAP;
}

GLenum ResourceManager::GetFilterMode(string& type)
{
	if (type == "LINEAR")
		return GL_LINEAR;

	if (type == "NEAREST")
		return GL_NEAREST;
}

GLenum ResourceManager::GetWrapMode(string& type)
{
	if (type == "CLAMP_TO_EDGE")
		return GL_CLAMP_TO_EDGE;

	if (type == "REPEAT")
		return GL_REPEAT;

	if (type == "MIRRORED_REPEAT")
		return GL_MIRRORED_REPEAT;
}

Model* ResourceManager::LoadModel(const GLuint id, Vector3& col)
{
	if (models.find(id) == models.end()) {
		Model *newModel = new Model(modelResources.at(id));
		newModel->Load(col);
		models.insert(pair<GLuint, Model*>(id, newModel));
	}

	return models.at(id);
}

Model* resourceManager::ResourceManager::GenerateModel(GLuint* id, GLint cellCount, GLfloat dimCell, GLfloat offsetY)
{
	Model* newModel = new Model(nullptr);
	newModel->GenerateTerrain(cellCount, dimCell, offsetY);

	*id = 0;
	while (true) {
		if (models.find(*id) == models.end())
			break;
		else
			*id = *id + 1;
	}

	models.insert(pair<GLuint, Model*>(*id, newModel));

	return models.at(*id);
}

Shader* ResourceManager::LoadShader(const GLuint id)
{
	if (shaders.find(id) == shaders.end()) {
		Shader* newShader = new Shader(shaderResources.at(id));
		newShader->Load();
		shaders.insert(pair<GLuint, Shader*>(id, newShader));
	}

	return shaders.at(id);
}

Texture* ResourceManager::LoadTexture(const GLuint id)
{
	if (textures.find(id) == textures.end()) {
		if (textureResources.find(id) == textureResources.end())
			return nullptr;

		Texture* newTexture = new Texture(textureResources.at(id));
		newTexture->Load();
		textures.insert(pair<GLuint, Texture*>(id, newTexture));
	}

	return textures.at(id);
}

ResourceManager* ResourceManager::GetInstance()
{
	if (!rmInstance)
		rmInstance = new ResourceManager();

	return rmInstance;
}

void ResourceManager::Init()
{
	ReadConfigFile();
	/*for (auto const& key : textureResources)
		key.second->print();*/
}

void resourceManager::ResourceManager::LoadData(const GLuint id, string& resource, Vector3& col)
{
	if (resource == "model")
		LoadModel(id, col);

	if (resource == "shader")
		LoadShader(id);

	if (resource == "texture")
		LoadTexture(id);
}

void ResourceManager::FreeResources()
{
	free(rmInstance);
	rmInstance = nullptr;
}

GLint resourceManager::ResourceManager::GetSkyTextureId()
{
	map<GLuint, Texture*>::iterator it;

	for (it = textures.begin(); it != textures.end(); it++)
	{
		if (it->second->GetTextureResource()->file == "envMap.tga")
			return it->first;
	}

	return -1;
}

//void resourceManager::ResourceManager::UpdateModelUV(GLuint id, int param, GLfloat offset)
//{
//	for (int i = 0; i < models.at(id)->GetNrVertices(); i++) {
//		models[id]->SetVertexUv(i, param, offset);
//	}
//}
