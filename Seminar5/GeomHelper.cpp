#include "MyForm.h"

using namespace Seminar5;


double GeomHelper::degToRad(double deg)
{
	return Math::PI * deg / 180.0;
}
/*	(1    0       0    0)
* (0   cos a   sin a   0)
* (0   -sin a  cos a   0)
* (0     0       0     1)*/
void GeomHelper::rotateX(Matrix^ transforms, double angleDegrees)
{
	double deg = degToRad(angleDegrees);
	double sin = Math::Sin(deg);
	double cos = Math::Cos(deg);
	Matrix^ transform = gcnew Matrix(4, 4,
		1, 0, 0, 0,
		0, cos, sin, 0,
		0, -sin, cos, 0,
		0, 0, 0, 1);
	transforms->set(transforms*transform);

}

void GeomHelper::rotateX(Matrix^ transforms, double m, double n, bool inverseMatrix)
{
	double d = Math::Sqrt(m*m + n*n);
	double a = n / d;
	double b = m / d;
	Matrix^ transform;
	if (!inverseMatrix)
		transform = gcnew Matrix(4, 4,
		1, 0, 0, 0,
		0, a, b, 0,
		0, -b, a, 0,
		0, 0, 0, 1);
	else {
		double sum = a*a + b*b;
		/*transform = gcnew Matrix(4, 4,
			1, 0, 0, 0,
			0, a / sum, -b / sum, 0,
			0, b / sum, a / sum, 0,
			0, 0, 0, 1);*/
		transform = gcnew Matrix(4, 4,
			1, 0, 0, 0,
			0, a, -b, 0,
			0, b, a, 0,
			0, 0, 0, 1);
	}
	transforms->set(transforms*transform);
}
/* (cos a   0  -sin a  0)
*  (0       1    0     0)
*  (sin a   0   cos a  0)
*  (0       0    0     1)*/
void GeomHelper::rotateY(Matrix^ transforms, double angleDegrees)
{
	double deg = degToRad(angleDegrees);
	double sin = Math::Sin(deg);
	double cos = Math::Cos(deg);
	Matrix^ transform = gcnew Matrix(4, 4,
		cos, 0, -sin, 0,
		0, 1, 0, 0,
		sin, 0, cos, 0,
		0, 0, 0, 1);
	transforms->set(transforms*transform);

}

void GeomHelper::rotateY(Matrix^ transforms, double l, double m, double n, bool inverseMatrix)
{
	double d = Math::Sqrt(m*m + n*n);

	Matrix^ transform;
	//if (!inverseMatrix)
	//	transform = gcnew Matrix(4, 4,
	//	l, 0, d, 0,
	//	0, 1, 0, 0,
	//	-d, 0, l, 0,
	//	0, 0, 0, 1);
	//else {
	//	double sum = d*d + l*l;
	//	/*transform = gcnew Matrix(4, 4,
	//		l / sum, 0, -d / sum, 0,
	//		0, 1, 0, 0,
	//		d / sum, 0, l / sum, 0,
	//		0, 0, 0, 1);*/
	//	transform = gcnew Matrix(4, 4,
	//		l, 0, -d, 0,
	//		0, 1, 0, 0,
	//		d, 0, l, 0,
	//		0, 0, 0, 1);
	double new_d = Math::Sqrt(d*d + l*l);
	double a = d / new_d;
	double b = l / new_d;
	if (!inverseMatrix)
		transform = gcnew Matrix(4, 4,
		a, 0, b, 0,
		0, 1, 0, 0,
		-b, 0, a, 0,
		0, 0, 0, 1);
	else {
		transform = gcnew Matrix(4, 4,
			a, 0, -b, 0,
			0, 1, 0, 0,
			b, 0, a, 0,
			0, 0, 0, 1);
	}
	transforms->set(transforms*transform);
}
/* (cos a   sin a  0   0)
*  (-sin a  cos a  0   0)
*  (0        0     1   0)
*  (0        0     0   1)*/
void GeomHelper::rotateZ(Matrix^ transforms, double angleDegrees)
{
	double deg = degToRad(angleDegrees);
	double sin = Math::Sin(deg);
	double cos = Math::Cos(deg);
	Matrix^ transform = gcnew Matrix(4, 4,
		cos, sin, 0, 0,
		-sin, cos, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	transforms->set(transforms*transform);

}
/* (1  0  0  0)
*  (0  1  0  0)
*  (0  0 -1  0)
*  (0  0  0  1)*/
void GeomHelper::reflectX(Matrix^ transforms)
{
	Matrix^ transform = gcnew Matrix(4, 4,
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1);
	transforms->set(transforms*transform);

}
/* (1  0   0  0)
*  (0  -1  0  0)
*  (0  0   1  0)
*  (0  0   0  1)*/
void GeomHelper::reflectY(Matrix^ transforms)
{
	Matrix^ transform = gcnew Matrix(4, 4,
		1, 0, 0, 0,
		0, -1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	transforms->set(transforms*transform);

}
/* (-1  0   0  0)
*  (0   1   0  0)
*  (0   0   1  0)
*  (0   0   0  1)*/
void GeomHelper::reflectZ(Matrix^ transforms)
{
	Matrix^ transform = gcnew Matrix(4, 4,
		-1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1);
	transforms->set(transforms*transform);

}

void GeomHelper::translate(Matrix^ transforms, int dx, int dy, int dz)
{
	Matrix^ transform = gcnew Matrix(4, 4,
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		dx, dy, dz, 1);
	transforms->set((transforms*transform)->normalize());

}

void GeomHelper::scale(Matrix^ transforms, double p, double q, double r, double s)
{
	Matrix^ transform = gcnew Matrix(4, 4,
		p, 0, 0, 0,
		0, q, 0, 0,
		0, 0, r, 0,
		0, 0, 0, s);
	transforms->set((transforms*transform)->normalize());

}
void GeomHelper::project(Matrix^ transforms, Matrix^ projection, Projection projMode, int dist)
{
	Matrix^ project = nullptr;
	switch (projMode)
	{
	case Projection::Central:
		project = gcnew Matrix(4, 4,
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, -1.0 / dist,
			0, 0, 0, 1);
		break;
	case Projection::Parallel:
		project = gcnew Matrix(4, 4,
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 1);
		break;
	case Projection::Front:
		project = gcnew Matrix(4, 4,
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 0, 0,
			0, 0, dist, 1);
		break;
	case Projection::Top:
		project = gcnew Matrix(4, 4,
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 1, 0,
			0, dist, 0, 1);
		break;
	case Projection::Side:
		project = gcnew Matrix(4, 4,
			0, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			dist, 0, 0, 1);
		break;
	}
	projection->set((transforms*project)->normalize());

}