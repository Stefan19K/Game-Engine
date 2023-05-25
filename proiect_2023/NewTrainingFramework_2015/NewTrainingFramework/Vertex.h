#pragma once
#include "../Utilities/Math.h"
#include <iostream>

struct Vertex 
{
	Vector3 pos;
	Vector3 col;
	Vector3 norm;
	Vector3 binorm;
	Vector3 tgt;
	Vector2 uv;
	Vector2 uv2;

	Vertex()
	{
		pos = Vector3();
		col = Vector3();
		norm = Vector3();
		binorm = Vector3();
		tgt = Vector3();
		uv = Vector2();
		uv2 = Vector2();
	}

	Vertex(const Vertex& t) : pos(t.pos), col(t.col), norm(t.norm), binorm(t.binorm), tgt(t.tgt), uv(t.uv), uv2(t.uv2)
	{
	}

	void print()
	{
		std::cout << "Vertex data:\n";
		std::cout << "pos : " << pos.x << " " << pos.y << " " << pos.z << "\n";
		std::cout << "col : " << col.x << " " << col.y << " " << col.z << "\n";
		std::cout << "norm : " << norm.x << " " << norm.y << " " << norm.z << "\n";
		std::cout << "binorm : " << binorm.x << " " << binorm.y << " " << binorm.z << "\n";
		std::cout << "tgt : " << tgt.x << " " << tgt.y << " " << tgt.z << "\n";
		std::cout << "uv : " << uv.x << " " << uv.y << "\n";
		std::cout << "uv2 : " << uv2.x << " " << uv2.y << "\n";
	}
};