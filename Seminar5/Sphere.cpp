#include "MyForm.h"

using namespace Seminar5;

Sphere::Sphere()
{
	int omegaPoints = 15; //кол-во точек на меридиане (дуга 180) 
	int nuPoints = 30; //кол-во точек на параллели (360) 
	double omegaStep = Math::PI / (omegaPoints - 1);
	double nuStep = 2 * Math::PI / (nuPoints - 1);
	array<array<double>^, 2>^ points = gcnew array<array<double>^, 2>(nuPoints, omegaPoints);
	double nu = 0;
	for (int i = 0; i < nuPoints; i++, nu += nuStep)
	{
		double om = 0;
		for (int j = 0; j < omegaPoints; j++, om += omegaStep)
		{
			points[i, j] = gcnew array<double>(3);

			points[i, j][0] = radius*Math::Sin(om)*Math::Cos(nu);
			points[i, j][1] = radius*Math::Sin(om)*Math::Sin(nu);
			points[i, j][2] = radius*Math::Cos(om);
		}

	}
	List<array<double>^>^ vertices_list = gcnew List<array<double>^>();
	//в points первый столбец содержит одну и ту же верхнюю пол€рную точку,
	//а последний столбец - одну и ту же нижнюю пол€рную точку 
	vertices_list->Add(points[0, 0]); // добавл€ем верхнюю пол€рную точку
	for (int i = 0; i < nuPoints; i++) {
		for (int j = 1; j < omegaPoints - 1; j++) { //не берем пол€рные точки
			vertices_list->Add(points[i, j]);
		}
	}
	// добавл€ем нижнюю пол€рную точку
	vertices_list->Add(points[points->GetLength(0) - 1, points->GetLength(1) - 1]);

	vertices = gcnew Matrix(vertices_list->ToArray());

	List<array<double>^>^ facets_list = gcnew List<array<double>^>();

	//соедин€ем верхнюю пол€рную точку с первой параллелью 
	for (int i = 0; i < nuPoints - 1; i++)
	{
		array<double>^ arr = gcnew array<double>(3);
		facets_list->Add(arr);
		arr[0] = 0;
		arr[1] = (i + 1)*(omegaPoints - 2) + 1; 
		arr[2] = i*(omegaPoints - 2) + 1;
	}

	//последовательно соедин€ем параллели между собой
	for (int i = 0; i < nuPoints - 1; i++)
	{
		for (int j = 1; j < omegaPoints - 2; j++)
		{
			array<double>^ arr = gcnew array<double>(3);
			facets_list->Add(arr);
			arr[0] = i*(omegaPoints - 2) + j + 1;
			arr[1] = i*(omegaPoints - 2) + j;
			arr[2] = (i + 1)*(omegaPoints - 2) + j;

			arr = gcnew array<double>(3);
			facets_list->Add(arr);
			arr[0] = (i + 1)*(omegaPoints - 2) + j + 1;
			arr[1] = i*(omegaPoints - 2) + j + 1;
			arr[2] = (i + 1)*(omegaPoints - 2) + j;
			
		}
	}

	//соедин€ем нижнюю пол€рную точку с последней параллелью
	int max_index = vertices->Rows - 1; //идем с конца
	for (int i = 0; i < nuPoints - 1; i++)
	{
		array<double>^ arr = gcnew array<double>(3);
		facets_list->Add(arr);
		arr[0] = max_index;
		arr[1] = max_index - ((i + 1)*(omegaPoints - 2) + 1);
		arr[2] = max_index - (i*(omegaPoints - 2) + 1);
		
	}

	facets = gcnew Matrix(facets_list->ToArray());
}

Sphere^ Sphere::Instance::get() {
	if (!instance)
		instance = gcnew Sphere();
	return instance;
}