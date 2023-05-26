#include "stdafx.h"
#include "Resources.h"

void Model::LoadVertexData()
{
	LoadVerticesIndices();
	LoadWiredIndices();
}

void Model::LoadHitBoxData(Vector3& col)
{
	Vector3 min = hitbox->vMin;
	Vector3 max = hitbox->vMax;

	hitbox->nrVertices = 8;
	hitbox->vertices = vector<Vertex>(hitbox->nrVertices);
	hitbox->vertices[0].pos = Vector3(min.x, min.y, min.z);
	hitbox->vertices[1].pos = Vector3(max.x, min.y, min.z);
	hitbox->vertices[2].pos = Vector3(max.x, min.y, max.z);
	hitbox->vertices[3].pos = Vector3(min.x, min.y, max.z);

	hitbox->vertices[4].pos = Vector3(min.x, max.y, min.z);
	hitbox->vertices[5].pos = Vector3(max.x, max.y, min.z);
	hitbox->vertices[6].pos = Vector3(max.x, max.y, max.z);
	hitbox->vertices[7].pos = Vector3(min.x, max.y, max.z);

	for (int i = 0; i < hitbox->nrVertices; i++)
		hitbox->vertices[i].col = col;

	hitbox->nrIndices = 24;
	hitbox->indices = { 
		0, 1, 1, 2, 2, 3, 3, 0,
		0, 4, 1, 5, 2, 6, 3, 7,
		4, 5, 5, 6, 6, 7, 7, 4
	};
}

void Model::LoadVerticesIndices()
{
	std::string line;
	std::ifstream fin(modelResource->path + modelResource->file);
	getline(fin, line);
	std::stringstream iss(line);
	iss >> line >> nrVertices;
	for (unsigned int i{}; i < nrVertices; i++) {
		Vertex vertex;
		std::string str;
		getline(fin, line);
		line = line.substr(5, line.size() - 5);
		char* p = strtok((char*)line.c_str(), ";");
		while (p) {
			std::string property{ p };
			std::size_t pos = property.find(":");
			std::string attribute = property.substr(1, pos - 1);
			std::string value = property.substr(pos + 2, property.size() - pos - 3);
			//std::cout << attribute << " " << value << "\n";

			pos = value.find(",");
			std::size_t pos2 = value.find(",", pos + 1);
			//std::cout << pos << " " << pos2 << "\n";
			float x = std::stof(value.substr(0, pos));
			float y = std::stof(value.substr(pos + 2, pos2 - pos - 1));
			//std::cout << y << " " << value.substr(pos + 2, pos2 - pos - 2) << "\n";
			//std::cout << x << " " << y << "\n";
			if (attribute == "uv") {
				vertex.uv = Vector2(x, y);
			}
			else {
				float z = std::stof(value.substr(pos2 + 1, value.size() - pos2 - 2));
				//std::cout << x << " " << y << " " << z << "\n";
				if (attribute == "pos") {
					vertex.pos.x = x;
					vertex.pos.y = y;
					vertex.pos.z = z;

					// set the lowest and highest vertices for hitbox
					if (i == 0) {
						hitbox->vMin = Vector3(x, y, z);
						hitbox->vMax = Vector3(x, y, z);
					}
					else {
						hitbox->vMin.x = x < hitbox->vMin.x ? x : hitbox->vMin.x;
						hitbox->vMin.y = y < hitbox->vMin.y ? y : hitbox->vMin.y;
						hitbox->vMin.z = z < hitbox->vMin.z ? z : hitbox->vMin.z;

						hitbox->vMax.x = x > hitbox->vMax.x ? x : hitbox->vMax.x;
						hitbox->vMax.y = y > hitbox->vMax.y ? y : hitbox->vMax.y;
						hitbox->vMax.z = z > hitbox->vMax.z ? z : hitbox->vMax.z;
					}
				}
				else if (attribute == "norm") {
					vertex.norm.x = x;
					vertex.norm.y = y;
					vertex.norm.z = z;
				}
				else if (attribute == "binorm") {
					vertex.binorm.x = x;
					vertex.binorm.y = y;
					vertex.binorm.z = z;
				}
				else if (attribute == "tgt") {
					vertex.tgt.x = x;
					vertex.tgt.y = y;
					vertex.tgt.z = z;
				}
			}
			p = strtok(NULL, ";");
		}

		vertex.col.x = 1.0f;
		vertex.col.y = 0.0f;
		vertex.col.z = 0.0f;
		//vertex.print();
		vertices.push_back(vertex);
		//vertices[i].print();
	}

	// read indices;
	getline(fin, line);
	iss = std::stringstream(line);
	iss >> line >> nrIndices;
	//std::cout << indicesCount << "\n";
	for (int i = 0; i < nrIndices;) {
		//std::cout << i << " " << indicesCount << "\n";
		getline(fin, line);
		line = line.substr(5, line.size() - 5);
		//std::cout << line << "\n";
		char* p = strtok((char*)line.c_str(), ", ");
		while (p) {
			std::string id{ p };
			unsigned int index = std::stoi(id);
			indices.push_back(index);
			i++;
			p = strtok(NULL, ", ");
		}
	}
}

void Model::LoadWiredIndices()
{
	nrWiredIndices = nrIndices * 2;
	for (GLuint i = 0; i < nrIndices; i += 3) {
		wiredIndices.push_back(indices[i]);
		wiredIndices.push_back(indices[i + 1]);
		wiredIndices.push_back(indices[i + 1]);
		wiredIndices.push_back(indices[i + 2]);
		wiredIndices.push_back(indices[i + 2]);
		wiredIndices.push_back(indices[i]);
	}
}

void Model::Load(Vector3& col)
{
	LoadVertexData();
	LoadHitBoxData(col);

	// buffer object
	glGenBuffers(1, &vbold);
	glBindBuffer(GL_ARRAY_BUFFER, vbold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * nrVertices, vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// index object
	glGenBuffers(1, &ibold);
	glBindBuffer(GL_ARRAY_BUFFER, ibold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indices[0]) * nrIndices, indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// wired index object
	glGenBuffers(1, &wiredIbold);
	glBindBuffer(GL_ARRAY_BUFFER, wiredIbold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wiredIndices[0]) * nrWiredIndices, wiredIndices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Hitbox object
	// buffer object
	glGenBuffers(1, &hitbox->vbold);
	glBindBuffer(GL_ARRAY_BUFFER, hitbox->vbold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hitbox->vertices[0]) * hitbox->nrVertices, hitbox->vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// index object
	glGenBuffers(1, &hitbox->ibold);
	glBindBuffer(GL_ARRAY_BUFFER, hitbox->ibold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(hitbox->indices[0]) * hitbox->nrIndices, hitbox->indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLint Model::GenerateTerrain(GLint cellCount, GLfloat dimCell, GLfloat offsetY)
{
	// Vector3 orig = Vector3();
	if (cellCount % 2 != 0)
		exit(-1);

	GLfloat topLeftX = -dimCell * cellCount / 2.0f;
	GLfloat topLeftZ = -dimCell * cellCount / 2.0f;

	nrVertices = (cellCount + 1) * (cellCount + 1);
	for (int i = 0; i < cellCount + 1; i++) {
		for (int j = 0; j < cellCount + 1; j++) {
			Vertex vt = Vertex();
			vt.pos.x = topLeftX + j * dimCell;
			vt.pos.z = topLeftZ + i * dimCell;
			vt.pos.y = offsetY;
			vt.col = Vector3(0.0f, 1.0f, 0.5f);
			vt.uv.x = j;
			vt.uv.y = i;
			vt.uv2.x = j / (GLfloat)cellCount;
			vt.uv2.y = i / (GLfloat)cellCount;

			//TODO
			vt.norm = Vector3(0.0f, 1.0f, 0.0f);

			vertices.push_back(vt);
		}
	}

	nrIndices = cellCount * cellCount * 6;
	cellCount += 1;
	for (int i = 0; i < cellCount - 1; i++) {
		for (int j = 0; j < cellCount - 1; j++) {
			indices.push_back(i * cellCount + j);
			indices.push_back((i + 1) * cellCount + j);
			indices.push_back(i * cellCount + j + 1);

			indices.push_back((i + 1) * cellCount + j);
			indices.push_back((i + 1) * cellCount + j + 1);
			indices.push_back(i * cellCount + j + 1);
		}
	}

	nrWiredIndices = nrIndices * 2;
	for (GLuint i = 0; i < nrIndices; i += 3) {
		wiredIndices.push_back(indices[i]);
		wiredIndices.push_back(indices[i + 1]);

		wiredIndices.push_back(indices[i + 1]);
		wiredIndices.push_back(indices[i + 2]);

		wiredIndices.push_back(indices[i + 2]);
		wiredIndices.push_back(indices[i]);
	}

	// buffer object
	glGenBuffers(1, &vbold);
	glBindBuffer(GL_ARRAY_BUFFER, vbold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * nrVertices, vertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// index object
	glGenBuffers(1, &ibold);
	glBindBuffer(GL_ARRAY_BUFFER, ibold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(indices[0]) * nrIndices, indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// wired index object
	glGenBuffers(1, &wiredIbold);
	glBindBuffer(GL_ARRAY_BUFFER, wiredIbold);
	glBufferData(GL_ARRAY_BUFFER, sizeof(wiredIndices[0]) * nrWiredIndices, wiredIndices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Shader::Load()
{
	string str;

	str = shaderResource->path + shaderResource->vertexShader;
	char* vs = new char[str.length() + 1];
	strncpy(vs, str.c_str(), str.length());
	vs[str.length()] = '\0';

	str = shaderResource->path + shaderResource->fragmentShader;
	char* fs = new char[str.length() + 1];
	strncpy(fs, str.c_str(), str.length());
	fs[str.length()] = '\0';

	shader = new Shaders();
	int r = shader->Init(vs, fs);
	/*cout << "shader initialized with exit status " << r << '\n';*/

	delete[] vs;
	delete[] fs;
}

char* Texture::LoadCubeFace(char* pixels, GLint face, GLint width, GLint height, int cnt)
{
	char* array = new char[width * height * cnt];
	GLint orig_width = width * 4;
	GLint orig_height = height * 3;
	GLint pos = 0;

	switch (face) {
	case 0:
		for (int i = height; i < height * 2; i++) {
			for (int j = width * 2 * cnt; j < width * 3 * cnt; j++) {
				array[pos] = pixels[i * orig_width * cnt + j];
				++pos;
			}
		}

		return array;
	case 1:
		for (int i = height; i < height * 2; i++) {
			for (int j = 0; j < width * cnt; j++) {
				array[pos] = pixels[i * orig_width * cnt + j];
				++pos;
			}
		}

		return array;
	case 2:
		for (int i = 0; i < height; i++) {
			for (int j = width * cnt; j < width * 2 * cnt; j++) {
				array[pos] = pixels[i * orig_width * cnt + j];
				++pos;
			}
		}

		return array;
	case 3:
		for (int i = height * 2; i < orig_height; i++) {
			for (int j = width * cnt; j < width * 2 * cnt; j++) {
				array[pos] = pixels[i * orig_width * cnt + j];
				++pos;
			}
		}

		return array;
	case 4:
		for (int i = height; i < height * 2; i++) {
			for (int j = width * cnt; j < width * 2 * cnt; j++) {
				array[pos] = pixels[i * orig_width * cnt + j];
				++pos;
			}
		}

		return array;
	case 5:
		for (int i = height; i < height * 2; i++) {
			for (int j = width * 3 * cnt; j < orig_width * cnt; j++) {
				array[pos] = pixels[i * orig_width * cnt + j];
				++pos;
			}
		}

		return array;
	default:
		return nullptr;
	}
}

void Texture::Load()
{
	glGenTextures(1, &textureId);
	glBindTexture(textureResource->type, textureId);
	glTexParameteri(textureResource->type, GL_TEXTURE_MIN_FILTER, textureResource->minFilter);
	glTexParameteri(textureResource->type, GL_TEXTURE_MAG_FILTER, textureResource->magFilter);
	glTexParameteri(textureResource->type, GL_TEXTURE_WRAP_S, textureResource->wrapS);
	glTexParameteri(textureResource->type, GL_TEXTURE_WRAP_T, textureResource->wrapT);

	if (this->textureResource->type == GL_TEXTURE_CUBE_MAP) {
		GLsizei width;
		GLsizei height;
		GLsizei bpp;
		char* arrayPixels;

		arrayPixels = LoadTGA((textureResource->path + textureResource->file).c_str(), &width, &height, &bpp);

		GLsizei s_width = width / 4;
		GLsizei s_height = height / 3;

		if (bpp == 24) {
			for (int i = 0; i < 6; i++) {
				char* miniArrPixels = LoadCubeFace(arrayPixels, i, s_width, s_height, 3);
				glTexImage2D(GL_TEXTURE_CUBE_MAP + 2 + i, 0, GL_RGB, s_width, s_height, 0, GL_RGB,
					GL_UNSIGNED_BYTE, miniArrPixels);
				delete[] miniArrPixels;
			}
		} else if (bpp == 32) {

			for (int i = 0; i < 6; i++) {
				char* miniArrPixels = LoadCubeFace(arrayPixels, i, s_width, s_height, 4);
				glTexImage2D(GL_TEXTURE_CUBE_MAP + 2 + i, 0, GL_RGBA, s_width, s_height, 0, GL_RGBA,
					GL_UNSIGNED_BYTE, miniArrPixels);
				delete [] miniArrPixels;
			}
		}
			

		return;
	}
	else if (this->textureResource->type == GL_TEXTURE_2D) {
		GLsizei width;
		GLsizei height;
		GLsizei bpp;
		char* arrayPixels;
		char aux;

		arrayPixels = LoadTGA((textureResource->path + textureResource->file).c_str(), &width, &height, &bpp);
		if (this->textureResource->reversed) {
			for (int i = 0; i < height / 2; i++) {
				for (int j = 0; j < width * (bpp / 8); j++) {
					aux = arrayPixels[i * width * (bpp / 8) + j];
					arrayPixels[i * width * (bpp / 8) + j] = arrayPixels[(height - 1 - i) * width * (bpp / 8) + j];
					arrayPixels[(height - 1 - i) * width * (bpp / 8) + j] = aux;
				}
			}
		}

		if (bpp == 24)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, arrayPixels);
		else if (bpp == 32)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
				GL_UNSIGNED_BYTE, arrayPixels);
	}
}