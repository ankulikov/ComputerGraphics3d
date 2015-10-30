#include "MyForm.h"

using namespace Seminar5;

Tetrahedron^ Tetrahedron::Instance::get() {
	if (!instance)
		instance = gcnew Tetrahedron();
	return instance;
}

Tetrahedron::Tetrahedron()
{
	vertices = gcnew Matrix(4, 4,
		0, 0, size*Math::Sqrt(6) / 4, 1,
		size / Math::Sqrt(3), 0, 0, 1,
		-size*Math::Sqrt(3) / 6, size / 2, 0, 1,
		-size*Math::Sqrt(3) / 6, -size / 2, 0, 1);

	facets = gcnew Matrix(4, 3,
		0, 1, 2,
		0, 2, 3,
		0, 1, 3,
		1, 2, 3);
}