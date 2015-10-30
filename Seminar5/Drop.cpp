#include "MyForm.h"

using namespace Seminar5;

Drop::Drop()
{
	int bPoints = 15; //кол-во точек на меридиане (дуга 180) //b == omega
	int aPoints = 20; //кол-во точек на параллели (360) //a = nu
	double bStep = Math::PI / (bPoints - 1);
	double aStep = 2 * Math::PI / (aPoints - 1);
	array<array<double>^, 2>^ points = gcnew array<array<double>^, 2>(aPoints, bPoints);
	double a = 0;
	for (int i = 0; i < aPoints; i++, a += aStep)
	{
		double b = -Math::PI / 2;
		for (int j = 0; j < bPoints; j++, b += bStep)
		{
			points[i, j] = gcnew array<double>(3);

			points[i, j][0] = radius*Math::Cos(b)*Math::Sin(a);
			points[i, j][1] = radius*Math::Cos(b)*Math::Cos(a);
			points[i, j][2] = radius*Math::Sin(b) +
				(b>0 ? radius*Math::Pow(b * 2 / Math::PI, 4) : 0);
			/*Debug::WriteLine(String::Format("i={0}, j={1}, (x,y,z)=({2}, {3}, {4})",
				i, j, points[i, j][0], points[i, j][1], points[i, j][2]));*/
		}

	}
	List<array<double>^>^ vertices_list = gcnew List<array<double>^>();
	//в points первый столбец содержит одну и ту же верхнюю пол€рную точку,
	//а последн€€ столбец - одну и ту же нижнюю пол€рную точку 
	vertices_list->Add(points[0, 0]); // добавл€ем верхнюю пол€рную точку
	for (int i = 0; i < aPoints; i++) {
		for (int j = 1; j < bPoints - 1; j++) { //не берем пол€рные точки
			vertices_list->Add(points[i, j]);
		}
	}
	// добавл€ем нижнюю пол€рную точку
	vertices_list->Add(points[points->GetLength(0) - 1, points->GetLength(1) - 1]);

	vertices = gcnew Matrix(vertices_list->ToArray());

	List<array<double>^>^ facets_list = gcnew List<array<double>^>();


	//соедин€ем верхнюю пол€рную точку с первой параллелью 
	for (int i = 0; i < aPoints - 1; i++)
	{
		array<double>^ arr = gcnew array<double>(3);
		facets_list->Add(arr);
		arr[0] = 0;
		arr[1] = (i + 1)*(bPoints - 2) + 1;
		arr[2] = i*(bPoints - 2) + 1;
	}

	for (int i = 0; i < aPoints - 1; i++)
	{
		for (int j = 1; j < bPoints - 2; j++)
		{
			array<double>^ arr = gcnew array<double>(3);
			facets_list->Add(arr);
			arr[0] = i*(bPoints - 2) + j + 1;
			arr[1] = i*(bPoints - 2) + j;
			arr[2] = (i + 1)*(bPoints - 2) + j;

			arr = gcnew array<double>(3);
			facets_list->Add(arr);
			arr[0] = (i + 1)*(bPoints - 2) + j + 1;
			arr[1] = i*(bPoints - 2) + j + 1;
			arr[2] = (i + 1)*(bPoints - 2) + j;
		}
	}

	//соедин€ем нижнюю пол€рную точку с последней параллелью
	int max_index = vertices->Rows - 1; //идем с конца
	for (int i = 0; i < aPoints - 1; i++)
	{
		array<double>^ arr = gcnew array<double>(3);
		facets_list->Add(arr);
		arr[0] = max_index;
		arr[1] = max_index - ((i + 1)*(bPoints - 2) + 1);
		arr[2] = max_index - (i*(bPoints - 2) + 1);
	}

	facets = gcnew Matrix(facets_list->ToArray());
}

Drop^ Drop::Instance::get() {
	if (!instance)
		instance = gcnew Drop();
	return instance;
}