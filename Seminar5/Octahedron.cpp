#include "MyForm.h"

using namespace Seminar5;

Octahedron^ Octahedron::Instance::get() {
	if (!instance)
		instance = gcnew Octahedron();
	return instance;
}

Octahedron::Octahedron()
{
	vertices = gcnew Matrix(gcnew array<double, 2> {
		{0.0f * size, 0.0f * size, 0.0f * size, 1},
		{ -1.0f * size, 0.0f * size, 1.0f * size, 1 },
		{ 0.0f * size, -1.0f * size, 1.0f * size, 1 },
		{ 1.0f * size, 0.0f * size, 1.0f * size, 1 },
		{ 0.0f * size, 1.0f * size, 1.0f * size, 1 },
		{ 0.0f * size, 0.0f * size, 2.0f * size, 1 }
	});

	facets = gcnew Matrix(8, 3,
		0, 4, 1,
		0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		5, 2, 1,
		5, 3, 2,
		5, 4, 3,
		5, 1, 4);
}