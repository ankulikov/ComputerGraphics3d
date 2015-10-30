#include "MyForm.h"

using namespace Seminar5;

Matrix::Matrix(int m, int n)
{
	this->m = m;
	this->n = n;
	arr = gcnew array<double, 2>(m, n);
}

Matrix::Matrix(Matrix^ other)
{
	set(other);
}

Matrix::Matrix(int m, int n, ...array<double>^ coords) :Matrix(m, n)
{
	for (int i = 0; i < coords->Length; i++)
	{
		arr[i / n, i%n] = coords[i];
	}
}

Matrix::Matrix(array<array<double>^>^ coords) : Matrix(coords->Length, coords[0]->Length)
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			arr[i, j] = coords[i][j];
		}
	}
}

Matrix::Matrix(array<double, 2>^ coords) :
Matrix(coords->GetLength(0), coords->GetLength(1))
{
	for (int i = 0; i < m; i++)
	{
		for (int j = 0; j < n; j++)
		{
			arr[i, j] = coords[i, j];
		}
	}
}

Matrix^ Matrix::operator+(const Matrix^ m1, const Matrix^ m2)
{
	Matrix^ m3 = gcnew Matrix(m1->m, m1->n);
	for (int i = 0; i < m1->m; i++)
	{
		for (int j = 0; j < m1->n; j++)
		{
			m3->arr[i, j] = m1->arr[i, j] + m2->arr[i, j];
		}
	}
	return m3;
}

Matrix^ Matrix::operator-(const Matrix^ m1, const Matrix^ m2)
{
	Matrix^ m3 = gcnew Matrix(m1->m, m1->n);
	for (int i = 0; i < m1->m; i++)
	{
		for (int j = 0; j < m1->n; j++)
		{
			m3[i, j] = m1->arr[i, j] - m2->arr[i, j];
		}
	}
	return m3;
}

Matrix^ Matrix::operator*(Matrix^ m1, Matrix^ m2)
{
	Matrix^ m3 = gcnew Matrix(m1->m, m2->n);
	for (int row1 = 0; row1 < m1->m; row1++)
	{
		for (int col2 = 0; col2 < m2->n; col2++)
		{
			for (int i = 0; i < m1->n; i++)
			{
				m3[row1, col2] += m1->arr[row1, i] * m2->arr[i, col2];
			}
		}
	}
	return m3;
}

Matrix^ Matrix::operator*(const int scale, const Matrix^ m1)
{
	Matrix^ m3 = gcnew Matrix(m1->m, m1->n);
	for (int i = 0; i < m1->m; i++)
	{
		for (int j = 0; j < m1->n; j++)
		{
			m3[i, j] = m1->arr[i, j] * scale;
		}
	}
	return m3;
}

Matrix^ Matrix::normalize()
{
	if (n == 4)
	{
		for (int i = 0; i < m; i++)
		{
			if (arr[i, 3] == 0) arr[i, 3] = 0.1;
			for (int j = 0; j < 3; j++)
				arr[i, j] /= arr[i, 3];
			arr[i, 3] = 1;
		}
	}
	return this;
}

Matrix^ Matrix::set(Matrix^ other)
{
	this->m = other->m;
	this->n = other->n;
	this->arr = gcnew array<double, 2>(m, n);
	Array::Copy(other->arr, this->arr, m*n);
	return this;
}

String^ Matrix::ToString()
{
	StringBuilder^ builder = gcnew StringBuilder();
	builder->Append("[");
	for (int i = 0; i < m; i++)
	{
		builder->Append("[");
		for (int j = 0; j < n; j++)
		{
			builder->Append(arr[i, j] + ", ");
		}
		builder->Remove(builder->Length - 2, 2);
		builder->Append("], ");
	}
	builder->Remove(builder->Length - 2, 2);
	builder->Append("]");
	return builder->ToString();

}




