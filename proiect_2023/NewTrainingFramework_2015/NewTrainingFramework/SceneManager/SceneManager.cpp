#include "stdafx.h"
#include "SceneManager.h"
#include "ResourceManager/ResourceManager.h"
#include "SceneObject/TerrainObject/TerrainObject.h"
#include "SceneObject/SkyBoxObject/SkyBoxObject.h"
#include "SceneObject/FireObject/FireObject.h"
#include "../Utilities/memDbg.h"

using namespace sceneManager;
using namespace resourceManager;

SceneManager* SceneManager::smInstance = nullptr;

void SceneManager::ReadConfigFile()
{
	xml_document<> doc;
	ifstream file(c_pathToXmlFile);
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	string content(buffer.str());
	doc.parse<0>(&content[0]);
	xml_node<>* pRoot = doc.first_node();
	xml_node<>* node;

	node = pRoot->first_node("gameName");
	if (node) {
		LoadProjectName(node);
	}

	node = pRoot->first_node("defaultScreenSize");
	if (node) {
		LoadScreenSize(node);
	}

	node = pRoot->first_node("backgroundColor");
	if (node) {
		LoadBackgroundColor(node);
	}

	node = pRoot->first_node("fog");
	if (node) {
		LoadFog(node);
	}

	node = pRoot->first_node("controls");
	if (node) {
		LoadControls(node);
	}

	node = pRoot->first_node("cameras");
	if (node) {
		LoadCameras(node);
	}

	node = pRoot->first_node("activeCamera");
	if (node) {
		LoadActiveCamera(node);
	}

	node = pRoot->first_node("objects");
	if (node) {
		LoadObjects(node);
	}

	node = pRoot->first_node("ambientalLight");
	LoadAmbientalLight(node);

	node = pRoot->first_node("lights");
	LoadLights(node);

	node = pRoot->first_node("debugSettings");
	LoadDebugSettings(node);
}

void SceneManager::LoadProjectName(xml_node<>* root)
{
	this->prjName = root->value();
}

void SceneManager::LoadScreenSize(xml_node<>* root)
{
	xml_node<>* node = nullptr;

	node = root->first_node("width");
	this->width = atoi(node->value());

	node = root->first_node("height");
	this->height = atoi(node->value());

	node = root->first_node("fullscreen");
	if (node && string(node->value()) == "true") {

		this->fullScreen = true;
		this->width = 1920.0f;
		this->height = 1080.0f;
	}
	else {
		this->fullScreen = false;
	}
}

void SceneManager::LoadBackgroundColor(xml_node<>* root)
{
	xml_node<>* node = nullptr;

	node = root->first_node("r");
	this->bckgrColor.x = atof(node->value());

	node = root->first_node("g");
	this->bckgrColor.y = atof(node->value());

	node = root->first_node("b");
	this->bckgrColor.z = atof(node->value());
}

void SceneManager::LoadFog(xml_node<>* root)
{
	for (xml_node<>* node = root->first_node(); node; node = node->next_sibling())
	{
		if (string(node->name()) == "r") {
			this->minFogField = atof(node->value());
		} else if (string(node->name()) == "R") {
			this->maxFogField = atof(node->value());
		} else if (string(node->name()) == "color") {
			xml_node<>* pNode;
			
			pNode = node->first_node("r");
			this->fogColor.x = atof(pNode->value());

			pNode = node->first_node("g");
			this->fogColor.y = atof(pNode->value());

			pNode = node->first_node("b");
			this->fogColor.z = atof(pNode->value());
		}
	}
}

void SceneManager::LoadControls(xml_node<>* root)
{
	for (xml_node<>* pNode = root->first_node("control"); pNode; pNode = pNode->next_sibling("control"))
	{
		xml_node<>* node = nullptr;
		char* key;
		string action;

		node = pNode->first_node("key");
		key = node->value();

		node = pNode->first_node("action");
		action = node->value();
		this->controls.insert(pair<unsigned char, Action>(*key, GetActionType(action)));
	}
}

void SceneManager::LoadCameras(xml_node<>* root)
{
	for (xml_node<>* pNode = root->first_node("camera"); pNode; pNode = pNode->next_sibling("camera"))
	{
		xml_node<>* node = nullptr;
		xml_node<>* nnode = nullptr;
		xml_attribute<>* pAttr = pNode->first_attribute("id");
		GLuint id = atoi(pAttr->value());
		Camera* cam = new Camera();

		node = pNode->first_node("type");
		// TODO: actually use type.

		node = pNode->first_node("position");
		if (node) {
			cam->SetPosition(
				Vector3(
					atof((node->first_node("x"))->value()),
					atof((node->first_node("y"))->value()),
					atof((node->first_node("z"))->value())
				)
			);
		}

		node = pNode->first_node("target");
		if (node) {
			cam->SetTarget(
				Vector3(
					atof((node->first_node("x"))->value()),
					atof((node->first_node("y"))->value()),
					atof((node->first_node("z"))->value())
				)
			);
		}
		
		node = pNode->first_node("up");
		if (node) {
			cam->SetUp(
				Vector3(
					atof((node->first_node("x"))->value()),
					atof((node->first_node("y"))->value()),
					atof((node->first_node("z"))->value())
				)
			);
		}
		

		node = pNode->first_node("translationSpeed");
		if (node) {
			cam->SetMoveSpeed(atof(node->value()));
		}
		

		node = pNode->first_node("rotationSpeed");
		if (node) {
			cam->SetRotateSpeed(atof(node->value()));
		}

		node = pNode->first_node("fov");
		if (node) {
			cam->SetFov(atof(node->value()));
		}
		

		node = pNode->first_node("near");
		if (node) {
			cam->SetZNear(atof(node->value()));
		}
		
		node = pNode->first_node("far");
		if (node) {
			cam->SetZFar(atof(node->value()));
		}
		
		cam->UpdateProjMatrix();
		cam->UpdateWorldView();

		this->cameras.insert(pair<GLuint, Camera*>(id, cam));
		this->cameraIds.push_back(id);
	}
}

void SceneManager::LoadActiveCamera(xml_node<>* root)
{
	GLuint id = atoi(root->value());
	for (int i = 0; i < this->cameraIds.size(); ++i)
		if (this->cameraIds[i] == id) {
			this->activeCameraId = i;
			return;
		}

	exit(-1);
}

void SceneManager::LoadObjects(xml_node<>* root)
{
	for (xml_node<>* pNode = root->first_node("object"); pNode; pNode = pNode->next_sibling("object"))
	{
		xml_node<>* node = nullptr;
		xml_attribute<>* pAttr = pNode->first_attribute("id");
		GLuint id = atoi(pAttr->value());

		node = pNode->first_node("type");
		if (string(node->value()) == "normal") {
			LoadNormalObject(pNode, id);
		}
		else if (string(node->value()) == "terrain") {
			LoadTerrainObject(pNode, id);
		}
		else if (string(node->value()) == "skybox") {
			LoadSkyBoxObject(pNode, id);
		}
		else if (string(node->value()) == "fire") {
			LoadFireObject(pNode, id);
		}
	}
}

void SceneManager::LoadNormalObject(xml_node<>* root, GLuint id)
{
	xml_node<>* node = nullptr;
	SceneObject* obj = new SceneObject();

	obj->SetType(string("normal"));

	node = root->first_node("bboxColor");
	if (node) {
		obj->SetHitBoxColor(
			Vector3(
				atof((node->first_node("r"))->value()),
				atof((node->first_node("g"))->value()),
				atof((node->first_node("b"))->value())
			)
		);
	}

	node = root->first_node("model");
	if (node) {
		GLuint modelId = atoi(node->value());
		ResourceManager::GetInstance()->LoadData(modelId, string("model"), obj->GetHitBoxColor());
		obj->SetModelId(modelId);
	}

	node = root->first_node("shader");
	if (node) {
		GLuint shaderId = atoi(node->value());
		ResourceManager::GetInstance()->LoadData(shaderId, string("shader"));
		obj->SetShaderId(shaderId);
	}

	node = root->first_node("name");
	if (node) {
		obj->SetName(string(node->value()));
	}

	node = root->first_node("position");
	if (node) {
		obj->SetPos(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("rotation");
	if (node) {
		obj->SetRotation(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("scale");
	if (node) {
		obj->SetScale(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("color");
	if (node) {
		obj->SetColor(
			Vector3(
				atof((node->first_node("r"))->value()),
				atof((node->first_node("g"))->value()),
				atof((node->first_node("b"))->value())
			)
		);
	}
	
	node = root->first_node("depthTest");
	node ? obj->SetDepthTest(true) : obj->SetDepthTest(false);

	node = root->first_node("wired");
	node ? obj->SetWired(true) : obj->SetWired(false);

	node = root->first_node("lights");
	if (node) {
		for (xml_node<>* pNode = node->first_node(); pNode; pNode = pNode->next_sibling())
		{
			obj->AddLightId(atoi(pNode->value()));
		}
	}

	node = root->first_node("textures");
	if (node) {
		for (xml_node<>* pNode = node->first_node(); pNode; pNode = pNode->next_sibling())
		{
			xml_attribute<>* pAttr = pNode->first_attribute("id");
			GLuint textureId = atoi(pAttr->value());
			ResourceManager::GetInstance()->LoadData(textureId, string("texture"));
			obj->AddTextureId(textureId);
		}
	}

	node = root->first_node("trajectory");
	if (node) {
		Trajectory* traj = new Trajectory();
		xml_attribute<>* attr1;
		xml_node<>* node2;

		attr1 = node->first_attribute("type");
		traj->trajType = attr1 ? GetTrajectoryType(attr1->value()) : LINEAR;

		attr1 = node->first_attribute("iteration");
		traj->iterCount = attr1 ? (string(attr1->value()) == "infinite" ? -1 : atoi(attr1->value())) : 0;
		
		attr1 = node->first_attribute("direction");
		traj->dirType = attr1 ? GetDirectionType(attr1->value()) : NORMAL;

		// This part is only for the linear trajectory.
		node2 = node->first_node("endPoint");
		if (node2) {
			traj->points.push_back(obj->GetPosition());
			traj->points.push_back(
				Vector3(
					atof((node2->first_node("x"))->value()),
					atof((node2->first_node("y"))->value()),
					atof((node2->first_node("z"))->value())
				)
			);
		}

		node2 = node->first_node("points");
		if (node2) {
			traj->points.push_back(obj->GetPosition());
			for (xml_node<>* pNode = node2->first_node("point"); pNode; pNode = pNode->next_sibling("point")) {
				traj->points.push_back(
					Vector3(
						atof((pNode->first_node("x"))->value()),
						atof((pNode->first_node("y"))->value()),
						atof((pNode->first_node("z"))->value())
					)
				);
			}
		}

		node2 = node->first_node("speed");
		if (node2)
			traj->speed = atof(node2->value());

		// Next fields are completed only for the circle trajectory!
		node2 = node->first_node("center");
		if (node2) {
			traj->center = Vector3(
				atof((node2->first_node("x"))->value()),
				atof((node2->first_node("y"))->value()),
				atof((node2->first_node("z"))->value())
			);
		}

		node2 = node->first_node("radius");
		if (node2)
			traj->radius = atof(node2->value());

		node2 = node->first_node("rotationPlane");
		if (node2) {
			traj->planeAngles = Vector3(
				atof((node2->first_node("x"))->value()),
				atof((node2->first_node("y"))->value()),
				atof((node2->first_node("z"))->value())
			);
		}

		obj->SetTrajectory(traj);
	}

	obj->UpdateTranformationMatrix();

	this->objects.insert(pair<GLuint, SceneObject*>(id, obj));
}

void SceneManager::LoadTerrainObject(xml_node<>* root, GLuint id)
{
	xml_node<>* node = nullptr;
	TerrainObject* obj = new TerrainObject();

	obj->SetType(string("terrain"));

	node = root->first_node("bboxColor");
	if (node) {
		obj->SetHitBoxColor(
			Vector3(
				atof((node->first_node("r"))->value()),
				atof((node->first_node("g"))->value()),
				atof((node->first_node("b"))->value())
			)
		);
	}

	node = root->first_node("cellCount");
	if (node) {
		obj->SetCellCount(atoi(node->value()));
	}

	node = root->first_node("dimCell");
	if (node) {
		obj->SetDimCell(atof(node->value()));
	}

	node = root->first_node("offsetY");
	if (node) {
		obj->SetOffsetY(atof(node->value()));
	}

	node = root->first_node("model");
	if (node) {
		GLuint modelId;
		if (string(node->value()) == "generated") {
			ResourceManager::GetInstance()->GenerateModel(
				&modelId, 
				obj->GetCellCount(), 
				obj->GetDimCell(), 
				obj->GetOffsetY()
			);
			obj->SetModelId(modelId);
		}
		else {
			node = root->first_node("model");
			if (node) {
				modelId = atoi(node->value());
				ResourceManager::GetInstance()->LoadData(modelId, string("model"), obj->GetHitBoxColor());
				obj->SetModelId(modelId);
			}
		}
	}

	node = root->first_node("shader");
	if (node) {
		GLuint shaderId = atoi(node->value());
		ResourceManager::GetInstance()->LoadData(shaderId, string("shader"));
		obj->SetShaderId(shaderId);
	}

	node = root->first_node("name");
	if (node) {
		obj->SetName(string(node->value()));
	}

	node = root->first_node("position");
	if (node) {
		obj->SetPos(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("rotation");
	if (node) {
		obj->SetRotation(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("scale");
	if (node) {
		obj->SetScale(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("depthTest");
	node ? obj->SetDepthTest(true) : obj->SetDepthTest(false);

	node = root->first_node("wired");
	node ? obj->SetWired(true) : obj->SetWired(false);

	node = root->first_node("lights");
	if (node) {
		for (xml_node<>* pNode = node->first_node(); pNode; pNode = pNode->next_sibling())
		{
			obj->AddLightId(atoi(pNode->value()));
		}
	}

	node = root->first_node("textures");
	if (node) {
		for (xml_node<>* pNode = node->first_node(); pNode; pNode = pNode->next_sibling())
		{
			xml_attribute<>* pAttr = pNode->first_attribute("id");
			GLuint textureId = atoi(pAttr->value());
			ResourceManager::GetInstance()->LoadData(textureId, string("texture"));
			obj->AddTextureId(textureId);
		}

		obj->SortTextures();
	}

	node = root->first_node("height");
	if (node) {
		obj->SetHeight(
			Vector3(
				atof((node->first_node("r"))->value()),
				atof((node->first_node("g"))->value()),
				atof((node->first_node("b"))->value())
			)
		);
	}

	node = root->first_node("followingCamera");
	if (node) {
		xml_node<>* auxNode = node->first_node("cameras");
		Vector3 folAxes = Vector3();

		obj->SetFolCamID(atoi(auxNode->first_node("camera")->value()));
		folAxes.x = node->first_node("ox") ? 1.0f : 0.0f;
		folAxes.y = node->first_node("oy") ? 1.0f : 0.0f;
		folAxes.z = node->first_node("oz") ? 1.0f : 0.0f;

		obj->SetFolAxes(folAxes);
		obj->SetPos(cameras.at(obj->GetFolCamId())->GetPosition()
			+ Vector3(0.0f, obj->GetOffsetY(), 0.0f));
	}

	obj->UpdateTranformationMatrix();

	this->objects.insert(pair<GLuint, SceneObject*>(id, obj));
}

void SceneManager::LoadSkyBoxObject(xml_node<>* root, GLuint id)
{
	xml_node<>* node = nullptr;
	SkyBoxObject* obj = new SkyBoxObject();

	obj->SetType(string("skybox"));

	node = root->first_node("bboxColor");
	if (node) {
		obj->SetHitBoxColor(
			Vector3(
				atof((node->first_node("r"))->value()),
				atof((node->first_node("g"))->value()),
				atof((node->first_node("b"))->value())
			)
		);
	}

	node = root->first_node("offsetY");
	if (node) {
		obj->SetOffsetY(atof(node->value()));
	}

	node = root->first_node("model");
	if (node) {
		GLuint modelId;

		modelId = atoi(node->value());
		ResourceManager::GetInstance()->LoadData(modelId, string("model"), obj->GetHitBoxColor());
		obj->SetModelId(modelId);
	}

	node = root->first_node("shader");
	if (node) {
		GLuint shaderId = atoi(node->value());
		ResourceManager::GetInstance()->LoadData(shaderId, string("shader"));
		obj->SetShaderId(shaderId);
	}

	node = root->first_node("name");
	if (node) {
		obj->SetName(string(node->value()));
	}

	node = root->first_node("position");
	if (node) {
		obj->SetPos(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("rotation");
	if (node) {
		obj->SetRotation(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("scale");
	if (node) {
		obj->SetScale(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("depthTest");
	node ? obj->SetDepthTest(true) : obj->SetDepthTest(false);

	node = root->first_node("wired");
	node ? obj->SetWired(true) : obj->SetWired(false);

	node = root->first_node("lights");
	if (node) {
		for (xml_node<>* pNode = node->first_node(); pNode; pNode = pNode->next_sibling())
		{
			obj->AddLightId(atoi(pNode->value()));
		}
	}

	node = root->first_node("textures");
	if (node) {
		for (xml_node<>* pNode = node->first_node(); pNode; pNode = pNode->next_sibling())
		{
			xml_attribute<>* pAttr = pNode->first_attribute("id");
			GLuint textureId = atoi(pAttr->value());
			ResourceManager::GetInstance()->LoadData(textureId, string("texture"));
			obj->AddTextureId(textureId);
		}

		obj->SortTextures();
	}

	node = root->first_node("followingCamera");
	if (node) {
		xml_node<>* auxNode = node->first_node("cameras");
		Vector3 folAxes = Vector3();

		obj->SetFolCamID(atoi(auxNode->first_node("camera")->value()));
		folAxes.x = node->first_node("ox") ? 1.0f : 0.0f;
		folAxes.y = node->first_node("oy") ? 1.0f : 0.0f;
		folAxes.z = node->first_node("oz") ? 1.0f : 0.0f;

		obj->SetFolAxes(folAxes);
		obj->SetPos(cameras.at(obj->GetFolCamId())->GetPosition()
			+ Vector3(0.0f, obj->GetOffsetY(), 0.0f));
	}

	obj->UpdateTranformationMatrix();

	this->objects.insert(pair<GLuint, SceneObject*>(id, obj));
}

void SceneManager::LoadFireObject(xml_node<>* root, GLuint id)
{
	xml_node<>* node = nullptr;
	FireObject* obj = new FireObject();

	obj->SetType(string("fire"));

	node = root->first_node("bboxColor");
	if (node) {
		obj->SetHitBoxColor(
			Vector3(
				atof((node->first_node("r"))->value()),
				atof((node->first_node("g"))->value()),
				atof((node->first_node("b"))->value())
			)
		);
	}

	node = root->first_node("dispMax");
	if (node) {
		obj->SetDispMax(atof(node->value()));
	}

	node = root->first_node("model");
	if (node) {
		GLuint modelId;

		modelId = atoi(node->value());
		ResourceManager::GetInstance()->LoadData(modelId, string("model"), obj->GetHitBoxColor());
		obj->SetModelId(modelId);
	}

	node = root->first_node("shader");
	if (node) {
		GLuint shaderId = atoi(node->value());
		ResourceManager::GetInstance()->LoadData(shaderId, string("shader"));
		obj->SetShaderId(shaderId);
	}

	node = root->first_node("name");
	if (node) {
		obj->SetName(string(node->value()));
	}

	node = root->first_node("position");
	if (node) {
		obj->SetPos(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("rotation");
	if (node) {
		obj->SetRotation(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("scale");
	if (node) {
		obj->SetScale(
			Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value())
			)
		);
	}

	node = root->first_node("depthTest");
	node ? obj->SetDepthTest(true) : obj->SetDepthTest(false);

	node = root->first_node("wired");
	node ? obj->SetWired(true) : obj->SetWired(false);

	node = root->first_node("lights");
	if (node) {
		for (xml_node<>* pNode = node->first_node(); pNode; pNode = pNode->next_sibling())
		{
			obj->AddLightId(atoi(pNode->value()));
		}
	}

	node = root->first_node("textures");
	if (node) {
		for (xml_node<>* pNode = node->first_node(); pNode; pNode = pNode->next_sibling())
		{
			xml_attribute<>* pAttr = pNode->first_attribute("id");
			GLuint textureId = atoi(pAttr->value());
			ResourceManager::GetInstance()->LoadData(textureId, string("texture"));
			obj->AddTextureId(textureId);
		}

		obj->SortTextures();
	}

	obj->UpdateTranformationMatrix();

	this->objects.insert(pair<GLuint, SceneObject*>(id, obj));
}

void SceneManager::LoadAmbientalLight(xml_node<>* root)
{
	xml_node<>* node;
	this->ambLight = new AmbientLight();

	if (!root)
		return;
	
	node = root->first_node("color");
	this->ambLight->color = Vector3(
		atof((node->first_node("r"))->value()),
		atof((node->first_node("g"))->value()),
		atof((node->first_node("b"))->value()));

	node = root->first_node("ratio");
	this->ambLight->ka = atof(node->value());
}

void SceneManager::LoadLights(xml_node<>* root)
{
	if (!root)
		return;

	for (xml_node<>* pNode = root->first_node("light"); pNode; pNode = pNode->next_sibling("light"))
	{
		xml_node<>* node = nullptr;
		xml_attribute<>* pAttr = nullptr;
		Light* light = new Light();
		GLuint id;
		GLuint objId;

		pAttr = pNode->first_attribute("id");
		id = atoi(pAttr->value());
		
		node = pNode->first_node("associatedObject");
		objId = atoi(node->value());
		
		if (objects.find(objId) == objects.end())
			return;

		light->pos = objects.at(objId)->GetPosition(); // Here we consider that a valid object exists.

		node = pNode->first_node("diffuseColor");
		light->diffColor = Vector3(
			atof((node->first_node("r"))->value()),
			atof((node->first_node("g"))->value()),
			atof((node->first_node("b"))->value()));
		light->kd = atof((node->first_node("ratio"))->value());

		node = pNode->first_node("specularColor");
		light->specColor = Vector3(
			atof((node->first_node("r"))->value()),
			atof((node->first_node("g"))->value()),
			atof((node->first_node("b"))->value()));
		light->ks = atof((node->first_node("ratio"))->value());

		node = pNode->first_node("type");
		light->SetType(node->value());

		node = pNode->first_node("direction");
		if (node) {
			light->direction = Vector3(
				atof((node->first_node("x"))->value()),
				atof((node->first_node("y"))->value()),
				atof((node->first_node("z"))->value()));
		}

		node = pNode->first_node("spotAngle");
		if (node)
			light->spotAngle = atof(node->value());

		this->lights.insert(pair<GLuint, Light*>(id, light));
	}
}

void SceneManager::LoadDebugSettings(xml_node<>* root)
{
	// TODO
	xml_node<>* node;
	coordSys = new CoordSys();

	if (!root)
		return;

	node = root->first_node("globalAxes");
	if (node) {
		xml_node<>* nNode = node->first_node("OXColor");
		if (nNode) {
			coordSys->oxColor = Vector3(
				atof((nNode->first_node("r"))->value()),
				atof((nNode->first_node("g"))->value()),
				atof((nNode->first_node("b"))->value()));
		}

		 nNode = node->first_node("OYColor");
		if (nNode) {
			coordSys->oyColor = Vector3(
				atof((nNode->first_node("r"))->value()),
				atof((nNode->first_node("g"))->value()),
				atof((nNode->first_node("b"))->value()));
		}

		nNode = node->first_node("OZColor");
		if (nNode) {
			coordSys->ozColor = Vector3(
				atof((nNode->first_node("r"))->value()),
				atof((nNode->first_node("g"))->value()),
				atof((nNode->first_node("b"))->value()));
		}
	}

	coordSys->Initialize();
}

void SceneManager::DrawCoordSystem()
{
	Shader* shader = ResourceManager::GetInstance()->LoadShader(0);
	Shaders* shdr = shader->GetShader();

	glBindBuffer(GL_ARRAY_BUFFER, coordSys->vbold);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, coordSys->ibold);

	if (shdr->positionAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->positionAttribute);
		glVertexAttribPointer(shdr->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (shdr->colorAttribute != -1)
	{
		glEnableVertexAttribArray(shdr->colorAttribute);
		glVertexAttribPointer(shdr->colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}

	if (shdr->modelUniform != -1)
	{
		glUniformMatrix4fv(shdr->modelUniform, 1, 0, (GLfloat*)Matrix().SetIdentity().m);
	}

	if (shdr->viewUniform != -1)
	{
		glUniformMatrix4fv(shdr->viewUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetViewMatrix().m);
	}

	if (shdr->projectionUniform != -1)
	{
		glUniformMatrix4fv(shdr->projectionUniform, 1, 0,
			(GLfloat*)SceneManager::GetInstance()->GetActiveCamera()->GetProjectionMatrix().m);
	}

	glDrawElements(GL_LINES, coordSys->nrIndices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Action SceneManager::GetActionType(const string& str)
{
	// Move camera constants
	if (str == "MOVE_CAMERA_POSITIVE_Z")
		return MOVE_CAMERA_POSITIVE_Z;

	if (str == "MOVE_CAMERA_NEGATIVE_Z")
		return MOVE_CAMERA_NEGATIVE_Z;

	if (str == "MOVE_CAMERA_POSITIVE_X")
		return MOVE_CAMERA_POSITIVE_X;

	if (str == "MOVE_CAMERA_NEGATIVE_X")
		return MOVE_CAMERA_NEGATIVE_X;

	if (str == "MOVE_CAMERA_POSITIVE_Y")
		return MOVE_CAMERA_POSITIVE_Y;

	if (str == "MOVE_CAMERA_NEGATIVE_Y")
		return MOVE_CAMERA_NEGATIVE_Y;

	// Rotate camera constants
	if (str == "ROTATE_CAMERA_POSITIVE_Z")
		return ROTATE_CAMERA_POSITIVE_Z;

	if (str == "ROTATE_CAMERA_NEGATIVE_Z")
		return ROTATE_CAMERA_NEGATIVE_Z;

	if (str == "ROTATE_CAMERA_POSITIVE_X")
		return ROTATE_CAMERA_POSITIVE_X;

	if (str == "ROTATE_CAMERA_NEGATIVE_X")
		return ROTATE_CAMERA_NEGATIVE_X;

	if (str == "ROTATE_CAMERA_POSITIVE_Y")
		return ROTATE_CAMERA_POSITIVE_Y;

	if (str == "ROTATE_CAMERA_NEGATIVE_Y")
		return ROTATE_CAMERA_NEGATIVE_Y;

	// debug mode
	if (str == "SWITCH_DEBUG_MODE")
		return SWITCH_DEBUG_MODE;

	// Go through cameras
	if (str == "NEXT_CAMERA")
		return NEXT_CAMERA;

	if (str == "PREV_CAMERA")
		return PREV_CAMERA;
}

TrajectoryType sceneManager::SceneManager::GetTrajectoryType(const string& str)
{
	if (str == "linear")
		return LINEAR;

	if (str == "line_strip")
		return LINE_STRIP;

	if (str == "line_loop")
		return LINE_LOOP;

	if (str == "circle")
		return CIRCLE;

	// Add conditions if u decide to add more trajectories.
}

DirectionType sceneManager::SceneManager::GetDirectionType(const string& str)
{
	if (str == "normal")
		return NORMAL;

	if (str == "alternate")
		return ALTERN;

	// Add more conditions if u decide to add more directions.
}

SceneManager* SceneManager::GetInstance()
{
	if (!smInstance)
		smInstance = new SceneManager();

	return smInstance;
}

void SceneManager::Init()
{
	debugMode = false;

	ReadConfigFile();
	glClearColor(bckgrColor.x, bckgrColor.y, bckgrColor.z, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneManager::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glViewport(0, 0, width, height);
	if (debugMode == false) {
		for (const auto& object : objects) {
			if (object.second->GetType() == "normal")
				object.second->Draw(cameras.at(cameraIds[activeCameraId]));
			if (object.second->GetType() == "terrain")
				((TerrainObject*)object.second)->Draw(cameras.at(cameraIds[activeCameraId]));
			if (object.second->GetType() == "skybox")
				((SkyBoxObject*)object.second)->Draw(cameras.at(cameraIds[activeCameraId]));
			if (object.second->GetType() == "fire")
				((FireObject*)object.second)->Draw(cameras.at(cameraIds[activeCameraId]));
		}
	}
	else {
		DrawCoordSystem();

		for (const auto& object : objects) {
			if (object.second->GetType() == "normal")
				object.second->DrawDebugMode();
			if (object.second->GetType() == "terrain")
				((TerrainObject*)object.second)->DrawDebugMode();
			if (object.second->GetType() == "skybox")
				((SkyBoxObject*)object.second)->DrawDebugMode();
			if (object.second->GetType() == "fire")
				((FireObject*)object.second)->DrawDebugMode();
		}
	}

	/*glViewport(0, 500, 400, 200);
	if (debugMode == false) {
		for (const auto& object : objects) {
			if (object.second->GetType() == "normal")
				object.second->Draw(cameras.at(activeCameraId));
			if (object.second->GetType() == "terrain")
				((TerrainObject*)object.second)->Draw(cameras.at(activeCameraId));
			if (object.second->GetType() == "skybox")
				((SkyBoxObject*)object.second)->Draw(cameras.at(activeCameraId));
			if (object.second->GetType() == "fire")
				((FireObject*)object.second)->Draw(cameras.at(activeCameraId));
		}
	}
	else {
		for (const auto& object : objects) {
			if (object.second->GetType() == "normal")
				object.second->DrawDebugMode();
			if (object.second->GetType() == "terrain")
				((TerrainObject*)object.second)->DrawDebugMode();
			if (object.second->GetType() == "skybox")
				((SkyBoxObject*)object.second)->DrawDebugMode();
			if (object.second->GetType() == "fire")
				((FireObject*)object.second)->DrawDebugMode();
		}
	}*/
	
}

void SceneManager::Update(GLfloat deltaTimeSeconds)
{
	cameras.at(cameraIds[activeCameraId])->SetDeltaTime(deltaTimeSeconds);
	for (const auto& object : objects) {
		if (object.second->GetType() == "normal")
			object.second->Update(deltaTimeSeconds);
		if (object.second->GetType() == "terrain")
			((TerrainObject*)object.second)->Update(deltaTimeSeconds);
		if (object.second->GetType() == "skybox")
			((SkyBoxObject*)object.second)->Update(deltaTimeSeconds);
		if (object.second->GetType() == "fire")
			((FireObject*)object.second)->Update(deltaTimeSeconds);
	}
}

void SceneManager::Key(unsigned char key, bool bIsPressed)
{

	if (bIsPressed) {
		Action action;
		
		if (this->controls.find(key) == controls.end())
			return;
		else
			action = this->controls.at(key);

		switch (action)
		{
		case sceneManager::MOVE_CAMERA_POSITIVE_Z:
			this->cameras.at(this->cameraIds[activeCameraId])->moveOz(1.0f);
			break;
		case sceneManager::MOVE_CAMERA_NEGATIVE_Z:
			this->cameras.at(this->cameraIds[activeCameraId])->moveOz(-1.0f);
			break;
		case sceneManager::MOVE_CAMERA_POSITIVE_X:
			this->cameras.at(this->cameraIds[activeCameraId])->moveOx(1.0f);
			break;
		case sceneManager::MOVE_CAMERA_NEGATIVE_X:
			this->cameras.at(this->cameraIds[activeCameraId])->moveOx(-1.0f);
			break;
		case sceneManager::MOVE_CAMERA_POSITIVE_Y:
			this->cameras.at(this->cameraIds[activeCameraId])->moveOy(1.0f);
			break;
		case sceneManager::MOVE_CAMERA_NEGATIVE_Y:
			this->cameras.at(this->cameraIds[activeCameraId])->moveOy(-1.0f);
			break;
		case sceneManager::ROTATE_CAMERA_POSITIVE_Z:
			this->cameras.at(this->cameraIds[activeCameraId])->rotateOZ(1.0f);
			break;
		case sceneManager::ROTATE_CAMERA_NEGATIVE_Z:
			this->cameras.at(this->cameraIds[activeCameraId])->rotateOZ(-1.0f);
			break;
		case sceneManager::ROTATE_CAMERA_POSITIVE_X:
			this->cameras.at(this->cameraIds[activeCameraId])->rotateOX(1.0f);
			break;
		case sceneManager::ROTATE_CAMERA_NEGATIVE_X:
			this->cameras.at(this->cameraIds[activeCameraId])->rotateOX(-1.0f);
			break;
		case sceneManager::ROTATE_CAMERA_POSITIVE_Y:
			this->cameras.at(this->cameraIds[activeCameraId])->rotateOY(1.0f);
			break;
		case sceneManager::ROTATE_CAMERA_NEGATIVE_Y:
			this->cameras.at(this->cameraIds[activeCameraId])->rotateOY(-1.0f);
			break;
		case sceneManager::SWITCH_DEBUG_MODE:
			this->debugMode ^= 1;
			break;
		case sceneManager::NEXT_CAMERA:
			this->activeCameraId = (this->activeCameraId + 1) % this->cameraIds.size();
			break;
		case sceneManager::PREV_CAMERA:
			if (this->activeCameraId == 0)
				this->activeCameraId = this->cameraIds.size();
			this->activeCameraId--;
			break;
		case sceneManager::ACTION_COUNT:
			break;
		default:
			break;
		}
	}
}
