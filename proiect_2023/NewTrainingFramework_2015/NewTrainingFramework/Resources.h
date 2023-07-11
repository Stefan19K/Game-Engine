#pragma once
#include <GLES2/gl2.h>
#include "Vertex.h"
#include "Shaders.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

struct HitBox {
	Vector3 vMin;
	Vector3 vMax;
	GLuint vbold;
	GLuint ibold;
	GLuint nrVertices;
	GLuint nrIndices;
	vector<Vertex> vertices;
	vector<GLuint> indices;

	HitBox() : vMin(Vector3()), vMax(Vector3()), vbold(0), ibold(0), nrVertices(0), nrIndices(0) {}
};

struct Normal {
	Vector3 vMin;
	Vector3 vMax;
	GLuint vbold;
	GLuint ibold;
	GLuint nrVertices;
	GLuint nrIndices;
	vector<Vertex> vertices;
	vector<GLuint> indices;

	Normal() : vMin(Vector3()), vMax(Vector3()), vbold(0), ibold(0), nrVertices(0), nrIndices(0) {}
};

struct CoordSys {
	Vector3 oxColor;
	Vector3 oyColor;
	Vector3 ozColor;
	GLuint vbold;
	GLuint ibold;
	GLuint nrVertices;
	GLuint nrIndices;
	vector<Vertex> vertices;
	vector<GLuint> indices;

	CoordSys() : 
		oxColor(Vector3(1.0f, 0.0f, 0.0f)), oyColor(Vector3(0.0f, 1.0f, 0.0f)), ozColor(Vector3(0.0f, 0.0f, 1.0f)),
		vbold(0), ibold(0), nrVertices(0), nrIndices(0) {}

	// void Initialize();
	void Initialize(Vector3 center = Vector3(), GLfloat x = 1000.0f, GLfloat y = 1000.0f, GLfloat z = 1000.0f);
};

struct ModelResource {
	unsigned int id;
	string path;
	string file;

	ModelResource() : id(0), path(""), file("") {}
	ModelResource(unsigned int _id, string& _path, string& _file) : id(_id), path(_path), file(_file) {}

	void print() {
		cout << "id is " << id << "\n";
		cout << "path is " << path << "\n";
		cout << "file is " << file << "\n\n";
	}
};

struct ShaderResource {
	unsigned int id;
	string path;
	string vertexShader;
	string fragmentShader;

	ShaderResource() : id(0), path(""), vertexShader(""), fragmentShader("") {}
	ShaderResource(unsigned int _id, string& _path, string& vs, string& fs) : id(_id), path(_path), vertexShader(vs), fragmentShader(fs) {}

	void print() {
		cout << "id is " << id << "\n";
		cout << "path is " << path << "\n";
		cout << "vertexShader is " << vertexShader << "\n";
		cout << "fragmentShader is " << fragmentShader << "\n\n";
	}
};

struct TextureResource {
	unsigned int id;
	bool reversed;
	string path;
	GLenum type;
	string file;
	GLenum minFilter;
	GLenum magFilter;
	GLenum wrapS;
	GLenum wrapT;

	TextureResource() :
		id(0),
		reversed(false),
		path(""),
		type(0),
		file(""),
		minFilter(0),
		magFilter(0),
		wrapS(0),
		wrapT(0) {}

	TextureResource(
		unsigned int _id,
		bool _reversed,
		string _path,
		GLenum _type,
		string _file,
		GLenum _minFilter,
		GLenum _magFilter,
		GLenum _wrapS,
		GLenum _wrapT
	) :
		id(_id),
		reversed(_reversed),
		path(_path),
		type(_type),
		file(_file),
		minFilter(_minFilter),
		magFilter(_magFilter),
		wrapS(_wrapS),
		wrapT(_wrapT) {}

	void print() {
		cout << "id is " << id << "\n";
		cout << "path is " << path << "\n";
		cout << "type is " << type << "\n";
		cout << "file is " << file << "\n";
		cout << "minFilter is " << minFilter << "\n";
		cout << "magFilter is " << magFilter << "\n";
		cout << "wrapS is " << wrapS << "\n";
		cout << "wrapT is " << wrapT << "\n\n";
	}
};

class Model {
private:
	ModelResource* modelResource;
	GLuint ibold;
	GLuint wiredIbold;
	GLuint vbold;
	GLuint nrVertices;
	GLuint nrIndices;
	GLuint nrWiredIndices;
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<GLuint> wiredIndices;
	HitBox* hitbox;
	Normal* normals;
	CoordSys* coordSys;

	void LoadVertexData();
	void LoadHitBoxData(Vector3& col);
	void LoadNormalsData();
	void LoadVerticesIndices();
	void LoadWiredIndices();
	void LoadCoordSys();

public:
	Model() :
		modelResource(nullptr),
		ibold(0),
		wiredIbold(0),
		vbold(0),
		nrVertices(0),
		nrIndices(0),
		nrWiredIndices(0),
		hitbox(new HitBox()),
		normals(new Normal()),
		coordSys(new CoordSys()) {}

	Model(ModelResource* resource) :
		modelResource(resource),
		ibold(0),
		wiredIbold(0),
		vbold(0),
		nrVertices(0),
		nrIndices(0),
		nrWiredIndices(0),
		hitbox(new HitBox),
		normals(new Normal()),
		coordSys(new CoordSys()) {}

	~Model() {}

	void Load(Vector3& col = Vector3());
	GLint GenerateTerrain(GLint cellCount, GLfloat dimCell, GLfloat offsetY);

	// Getters
	ModelResource*	GetModelResource()	{ return this->modelResource; }
	GLuint			GetIbold()			{ return this->ibold; }
	GLuint			GetWiredIbold()		{ return this->wiredIbold; }
	GLuint			GetVbold()			{ return this->vbold; }
	GLuint			GetNrVertices()		{ return this->nrVertices; }
	GLuint			GetNrIndices()		{ return this->nrIndices; }
	GLuint			GetNrWiredIndices()	{ return this->nrWiredIndices; }
	vector<Vertex>	GetVertices()		{ return this->vertices; }
	vector<GLuint>	GetIndices()		{ return this->indices; }
	vector<GLuint>	GetWiredIndices()	{ return this->wiredIndices; }
	HitBox*			GetHitbox()			{ return this->hitbox; }
	Normal*			GetNormal()			{ return this->normals; }
	CoordSys*		GetCoordSys()		{ return this->coordSys; }

	// Show structure data
	void print()
	{
		modelResource->print();
		cout << "ibold : " << ibold << "\n";
		cout << "wiredIbold : " << wiredIbold << "\n";
		cout << "vbold : " << vbold << "\n";
		cout << "nrVertices : " << nrVertices << "\n";
		cout << "nrIndices : " << nrIndices << "\n";
		cout << "nrWiredOndices : " << nrWiredIndices << "\n\n";
	}

	// Setters
	/*void SetVertexUv(GLuint id, int param, GLfloat offset) {
		param == 0 ? vertices[id].uv2.x += offset : vertices[id].uv2.y += offset;}*/
};

class Shader {
private:
	ShaderResource* shaderResource;
	Shaders* shader;

public:
	Shader() :
		shaderResource(nullptr) {}

	Shader(ShaderResource* resource) :
		shaderResource(resource) {}

	~Shader() {}

	void Load();

	// Getters
	ShaderResource* GetShaderResource() { return this->shaderResource; }
	Shaders* GetShader() { return this->shader; }
};

class Texture {
private:
	TextureResource* textureResource;
	GLuint textureId;

	char* LoadCubeFace(char* pixels, GLint face, GLint width, GLint height, int cnt);

public:
	Texture() :
		textureResource(nullptr),
		textureId(0) {}

	Texture(TextureResource* resource) :
		textureResource(resource),
		textureId(0) {}

	~Texture() {}

	void Load();

	// Getters
	TextureResource* GetTextureResource() { return this->textureResource; }
	GLuint GetTextureId() { return this->textureId; }
};

// Trajectory related classes and structures.
enum TrajectoryType
{
	LINEAR,
	LINE_STRIP,
	LINE_LOOP,
	CIRCLE

	// Add more trajectory types here
};

enum DirectionType
{
	NORMAL,
	ALTERN

	// Add more direction types here
};

struct Trajectory
{
	TrajectoryType trajType;
	DirectionType dirType;
	vector<Vector3> points;
	GLint iterCount;
	GLfloat speed;
	GLint currPointPos;
	GLint nextPointPos;

	// Variables used only for the circle trajectory.
	GLfloat radius;
	Vector3 center;
	Vector3 planeAngles;

	Trajectory() : 
		trajType(LINEAR), 
		dirType(NORMAL), 
		iterCount(0), 
		speed(0.0f), 
		radius(0.0f), 
		center(Vector3()), 
		planeAngles(Vector3()),
		currPointPos(0),
		nextPointPos(1) {}
};
