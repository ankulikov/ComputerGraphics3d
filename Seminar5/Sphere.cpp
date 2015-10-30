#include "MyForm.h"

using namespace Seminar5;

Sphere::Sphere()
{
	int omegaPoints = 15; //���-�� ����� �� ��������� (���� 180) 
	int nuPoints = 30; //���-�� ����� �� ��������� (360) 
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
	//� points ������ ������� �������� ���� � �� �� ������� �������� �����,
	//� ��������� ������� - ���� � �� �� ������ �������� ����� 
	vertices_list->Add(points[0, 0]); // ��������� ������� �������� �����
	for (int i = 0; i < nuPoints; i++) {
		for (int j = 1; j < omegaPoints - 1; j++) { //�� ����� �������� �����
			vertices_list->Add(points[i, j]);
		}
	}
	// ��������� ������ �������� �����
	vertices_list->Add(points[points->GetLength(0) - 1, points->GetLength(1) - 1]);

	vertices = gcnew Matrix(vertices_list->ToArray());

	List<array<double>^>^ facets_list = gcnew List<array<double>^>();

	//��������� ������� �������� ����� � ������ ���������� 
	for (int i = 0; i < nuPoints - 1; i++)
	{
		array<double>^ arr = gcnew array<double>(3);
		facets_list->Add(arr);
		arr[0] = 0;
		arr[1] = (i + 1)*(omegaPoints - 2) + 1; 
		arr[2] = i*(omegaPoints - 2) + 1;
	}

	//��������������� ��������� ��������� ����� �����
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

	//��������� ������ �������� ����� � ��������� ����������
	int max_index = vertices->Rows - 1; //���� � �����
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