#include "MyForm.h"

using namespace Seminar5;

const int MAX_DEPTH = 10000;

static GraphicsStorage::GraphicsStorage()
{
	projection = gcnew Matrix(0, 0); //будет заполнена при операциях проекции
	setFigure(Icosahedron::Instance);

	linePen = gcnew Pen(Color::Red);
}







void GraphicsStorage::redraw(Graphics^ graphics, int centerX, int centerY, GeomHelper::Projection projType, int dist)
{
	graphics->Clear(Color::Yellow);
	GeomHelper::project(vertices, projection, projType, dist);
	//в зависимости от вида ортографической проекции берем нужные координаты 
	int i1, i2; //x=0, y=1, z=2 
	switch (projType)
	{
	case GeomHelper::Projection::Top: //на экране плоскость XZ
		GeomHelper::translate(projection, centerX, 0, centerY);
		i1 = 0; i2 = 2;
		break;
	case GeomHelper::Projection::Side: //на экране плоскость YZ
		GeomHelper::translate(projection, 0, centerX, centerY);
		i1 = 1; i2 = 2;
		break; 
	default: //на экране плоскость XY
		GeomHelper::translate(projection, centerX, centerY, 0);
		i1 = 0; i2 = 1;
		break;
	}
	Matrix^ facets = figure->Facets;

	array<int>^ drawPoints;

	for (int i = 0; i < facets->Rows; i++)
	{
		drawPoints = gcnew array<int>(6) {
			//для грани (3 точки) получаем координаты точек после проектирования
			projection[facets[i, 0], i1], projection[facets[i, 0], i2],
				projection[facets[i, 1], i1], projection[facets[i, 1], i2],
				projection[facets[i, 2], i1], projection[facets[i, 2], i2]
		};
		//Ограничиваем глубину прорисовки
		for (int j = 0; j < drawPoints->Length; j++)
			drawPoints[j] = Math::Min(Math::Max(drawPoints[j], -MAX_DEPTH), MAX_DEPTH);
		//Выводим треугольнки: 0->1, 1->2, 2->1 
		graphics->DrawLine(linePen, 
			drawPoints[0], drawPoints[1], drawPoints[2], drawPoints[3]);
		graphics->DrawLine(linePen,
			drawPoints[2], drawPoints[3], drawPoints[4], drawPoints[5]);
		graphics->DrawLine(linePen,
			drawPoints[4], drawPoints[5], drawPoints[0], drawPoints[1]);
	}
}

void GraphicsStorage::setFigure(Figure^ figure)
{
	GraphicsStorage::figure = figure;
	resetVertices();
}

void GraphicsStorage::resetVertices()
{
	vertices = gcnew Matrix(figure->Vertices);
}