#include "MyForm.h"

using namespace Seminar5;

Icosahedron^ Icosahedron::Instance::get() {
	if (!instance)
		instance = gcnew Icosahedron();
	return instance;
}

Icosahedron::Icosahedron()
{
	vertices = gcnew Matrix(gcnew array<double, 2>
	{
		{ 0.809f * size, 0.5f * size, 0.588f * size, 1 },
		{ 0.309f * size, -0.5f * size, 0.951f * size, 1 },
		{ -0.309f * size, 0.5f * size, 0.951f * size, 1 },
		{ -0.809f * size, -0.5f * size, 0.588f * size, 1 },
		{ -1.00f * size, 0.5f * size, 0.0f * size, 1 },
		{ -0.809f * size, -0.5f * size, -0.588f * size, 1 },
		{ -0.309f * size, 0.5f * size, -0.951f * size, 1 },
		{ 0.309f * size, -0.5f * size, -0.951f * size, 1 },
		{ 0.809f * size, 0.5f * size, -0.588f * size, 1 },
		{ 1.0f * size, -0.5f * size, 0.0f * size, 1 },
		{ 0.0f * size, 1.118f * size, 0.0f * size, 1 },
		{ 0.0f * size, -1.118f * size, 0.0f * size, 1 }
	});
	facets = gcnew Matrix(20, 3,
		0, 10, 2,
		2, 10, 4,
		4, 10, 6,
		6, 10, 8,
		8, 10, 0,
		0, 2, 1,
		1, 2, 3,
		2, 4, 3,
		3, 4, 5,
		4, 6, 5,
		5, 6, 7,
		6, 8, 7,
		7, 8, 9,
		8, 0, 9,
		9, 0, 1,
		1, 11, 9,
		3, 11, 1,
		5, 11, 3,
		7, 11, 5,
		9, 11, 7
		);
}