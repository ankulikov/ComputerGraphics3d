#include "MyForm.h"

using namespace Seminar5;

Torus::Torus()
{
	int omegaPoints = 20; //кол-во точек на малом круге
	int nuPoints = 20; //кол-во точек на большом круге
	double omegaStep = 2*  Math::PI / (omegaPoints - 1);
	double nuStep = 2 * Math::PI / (nuPoints - 1);
	array<array<double>^, 2>^ points = gcnew array<array<double>^, 2>(nuPoints, omegaPoints);
	double nu = 0;
	for (int i = 0; i < nuPoints; i++, nu += nuStep)
	{
		double om = 0;
		for (int j = 0; j < omegaPoints; j++, om += omegaStep)
		{
			points[i, j] = gcnew array<double>(3);

			points[i, j][0] = (rBig + rSmall * Math::Cos(om))* Math::Cos(nu);
			points[i, j][1] = (rBig + rSmall * Math::Cos(om))* Math::Sin(nu);
			points[i, j][2] = rSmall * Math::Sin(om);
		}

	}
	List<array<double>^>^ vertices_list = gcnew List<array<double>^>();
	for (int i = 0; i < nuPoints; i++) {
		for (int j = 0; j < omegaPoints; j++) { 
			vertices_list->Add(points[i, j]);
		}
	}

	vertices = gcnew Matrix(vertices_list->ToArray());

	List<array<double>^>^ facets_list = gcnew List<array<double>^>();

	for (int i = 0; i < nuPoints - 1; i++)
	{
		for (int j = 0; j < omegaPoints - 1; j++)
		{
			array<double>^ arr = gcnew array<double>(3);
			facets_list->Add(arr);
			arr[0] = i*(omegaPoints) + j;
			arr[1] = i*(omegaPoints) + j + 1;
			arr[2] = (i + 1)*(omegaPoints) + j;

			arr = gcnew array<double>(3);
			facets_list->Add(arr);
			arr[0] = i*(omegaPoints) + j + 1;
			arr[1] = (i + 1)*(omegaPoints) + j;
			arr[2] = (i + 1)*(omegaPoints) + j + 1;
		}
	}

	facets = gcnew Matrix(facets_list->ToArray());
}

Torus^ Torus::Instance::get() {
	if (!instance)
		instance = gcnew Torus();
	return instance;
}