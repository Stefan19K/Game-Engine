#include "SceneObject/SceneObject.h"

class TerrainObject : public SceneObject {
protected:
	GLuint followingCamIdx;
	Vector3 folAxes;
	GLint cellCount;
	GLfloat dimCell;
	GLfloat offsetY;
	Vector3 height;
	Vector2 uvOffset;

public:
	TerrainObject() :
		followingCamIdx(0),
		folAxes(Vector3()),
		cellCount(0),
		dimCell(0.0f),
		offsetY(0.0f),
		height(Vector3()),
		uvOffset(Vector2()),
		SceneObject() {}

	/*TerrainObject(
		GLuint _id,
		GLuint _modelId,
		GLuint _shaderId,
		vector<GLuint> _textureIds,
		vector<GLuint> _lightIds,
		string _type,
		string _name,
		Vector3 _position,
		Vector3 _rotation,
		Vector3 _scale,
		Vector3 _height,
		bool _depthTest,
		GLuint _folCamId,
		Vector3 _folAxes,
		GLint _cellCount,
		GLfloat _dimCell,
		GLfloat _offsetY
	) : 
		followingCamIdx(_folCamId),
		folAxes(_folAxes),
		cellCount(_cellCount),
		dimCell(_dimCell),
		offsetY(_offsetY),
		height(_height),
		uvOffset(Vector2()),
		SceneObject(_id, _modelId, _shaderId, _textureIds, _lightIds, _type, _name, _position, _rotation, _scale, _depthTest) {}

	TerrainObject(
		GLuint _id,
		GLuint _modelId,
		GLuint _shaderId,
		string _type,
		string _name,
		Vector3 _color,
		Vector3 _position,
		Vector3 _rotation,
		Vector3 _scale,
		Vector3 _height,
		bool _wired,
		bool _depthTest,
		GLuint _folCamId,
		Vector3 _folAxes,
		GLint _cellCount,
		GLfloat _dimCell,
		GLfloat _offsetY
	) :
		followingCamIdx(_folCamId),
		folAxes(_folAxes),
		cellCount(_cellCount),
		dimCell(_dimCell),
		offsetY(_offsetY),
		height(_height),
		uvOffset(Vector2()),
		SceneObject(_id, _modelId, _shaderId, _type, _name, _color, _position, _rotation, _scale, _wired, _depthTest) {}*/

	void Draw(Camera* cam);
	void Update(GLfloat deltaTime);

	// Setters
	void SetFolCamID(GLuint id)		{ this->followingCamIdx = id; }
	void SetFolAxes(Vector3 axes)	{ this->folAxes = axes; }
	void SetCellCount(GLint count)	{ this->cellCount = count; }
	void SetDimCell(GLfloat dim)	{ this->dimCell = dim; }
	void SetOffsetY(GLfloat offset) { this->offsetY = offset; }
	void SetHeight(Vector3 height)	{ this->height = height; }

	// Getters
	GLint	GetCellCount()			{ return this->cellCount; }
	GLfloat GetDimCell()			{ return this->dimCell; }
	GLfloat GetOffsetY()			{ return this->offsetY; }
	GLuint	GetFolCamId()			{ return this->followingCamIdx; }
};