#pragma once
#include "rapidxml.hpp"
#include "SceneObject/SceneObject.h"
#include "Camera.h"
#include <string>
#include <map>
#include <algorithm>    // std::sort

using namespace std;
using namespace rapidxml;

namespace sceneManager {
	const string c_pathToResourcesPacket{ "../../NewResourcesPacket/" };
	const string c_pathToXmlFile{ c_pathToResourcesPacket + "XmlFiles/sceneManager.xml" };

	struct AmbientLight {
		Vector3 color;
		GLfloat ka;

		AmbientLight() : color(Vector3()), ka(1.0f) {}
	};

	struct Light {
		GLuint type;
		Vector3 pos;
		Vector3 direction;
		Vector3 diffColor;
		Vector3 specColor;
		GLfloat kd;
		GLfloat ks;
		GLfloat shininess;
		GLfloat spotAngle;

		Light() :
			type(0),
			pos(Vector3()),
			direction(Vector3()),
			diffColor(Vector3()),
			specColor(Vector3()),
			kd(0.0f),
			ks(0.0f),
			shininess(0.0f),
			spotAngle(0.0f)
		{}

		void SetType(string _type)
		{
			if (_type == "point")
				type = 0;
			else if (_type == "directional")
				type = 1;
			else if (_type == "spotlight")
				type = 2;
		}
	};

	enum Action {
		MOVE_CAMERA_POSITIVE_Z,
		MOVE_CAMERA_NEGATIVE_Z,
		MOVE_CAMERA_POSITIVE_X,
		MOVE_CAMERA_NEGATIVE_X,
		MOVE_CAMERA_POSITIVE_Y,
		MOVE_CAMERA_NEGATIVE_Y,
		ROTATE_CAMERA_POSITIVE_Z,
		ROTATE_CAMERA_NEGATIVE_Z,
		ROTATE_CAMERA_POSITIVE_X,
		ROTATE_CAMERA_NEGATIVE_X,
		ROTATE_CAMERA_POSITIVE_Y,
		ROTATE_CAMERA_NEGATIVE_Y,
		ACTION_COUNT
	};

	class SceneManager {
	private:
		static SceneManager* smInstance;
		string prjName;
		GLint width;
		GLint height;
		bool fullScreen;
		Vector3 bckgrColor;
		GLfloat minFogField;
		GLfloat maxFogField;
		Vector3 fogColor;
		map<unsigned char, Action> controls;
		map<GLuint, Camera*> cameras;
		GLuint activeCameraId;
		map<GLuint, SceneObject*> objects;

		AmbientLight* ambLight;
		map<GLuint, Light*> lights;

		void ReadConfigFile();
		void LoadProjectName(xml_node<>* root);
		void LoadScreenSize(xml_node<>* root);
		void LoadBackgroundColor(xml_node<>* root);
		void LoadFog(xml_node<>* root);
		void LoadControls(xml_node<>* root);
		void LoadCameras(xml_node<>* root);
		void LoadActiveCamera(xml_node<>* root);
		void LoadObjects(xml_node<>* root);
		void LoadNormalObject(xml_node<>* root, GLuint id);
		void LoadTerrainObject(xml_node<>* root, GLuint id);
		void LoadSkyBoxObject(xml_node<>* root, GLuint id);
		void LoadFireObject(xml_node<>* root, GLuint id);
		void LoadAmbientalLight(xml_node<>* root);
		void LoadLights(xml_node<>* root);
		void LoadDebugSettings(xml_node<>* root);

		Action GetActionType(const string& str);

	public:
		static SceneManager* GetInstance();
		void Init(); //initializari - pot fi si in constructor
		void Draw();
		void Update(GLfloat deltaTimeSeconds);
		void Key(unsigned char key, bool bIsPressed);

		// Getters
		string	GetName()					{ return prjName; }
		GLint	GetWidth()					{ return width; }
		GLint	GetHeight()					{ return height; }
		Camera* GetActiveCamera()			{ return cameras.at(activeCameraId); }
		GLfloat GetMinFogView()				{ return minFogField; }
		GLfloat GetMaxFogView()				{ return maxFogField; }
		Vector3 GetFogColor()				{ return fogColor; }
		Vector3 GetAmbientColor()			{ return ambLight->color; }
		GLfloat GetKa()						{ return ambLight->ka; }
		Camera* GetCamera(const GLuint id)	{ return cameras.at(id); }
		Light*	GetLight(const GLuint id)	{ return lights.at(id); }
	};
}
