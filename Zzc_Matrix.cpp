#include "Zzc_Matrix.h"
#include <cstdio>
#include <cstring>
#include <windows.h>
ZMatrix::ZMatrix(int r, int c)
{

	row = r;
	col = c;
	mat = new double[row*col];
	memset(mat, 0, row*col*sizeof(double));
}
ZMatrix::~ZMatrix()
{
	delete[] mat;
}
void ZMatrix::Reset(ZMatrix &A)
{
	memset(A.mat, 0, A.row*A.col*sizeof(double));
}
void ZMatrix::ZMatMUL(ZMatrix& A, ZMatrix &B, ZMatrix &C)
{
	for (int i = 0; i < C.row; i++)
	{
		for (int j = 0; j < C.col; j++)
		{
			*(C.mat + i*C.col + j) = 0;
			for (int k = 0; k < A.col; k++)
			{
				*(C.mat+i*C.col+j) += (*(A.mat+i*A.col+k) * (*(B.mat+k*B.col+j)));
			}
		}
	}
}
void ZMatrix::ZMatADD(ZMatrix& A, ZMatrix &B, ZMatrix &C)
{
	for (int i = 0; i < C.row; i++)
	{
		for (int j = 0; j < C.col; j++)
		{
			*(C.mat + i*C.col + j) = *(A.mat + i*A.col + j) + (*(B.mat + i*B.col + j));
		}
	}
}
void ZMatrix::ZMatSUB(ZMatrix& A, ZMatrix &B, ZMatrix &C)
{
	for (int i = 0; i < C.row; i++)
	{
		for (int j = 0; j < C.col; j++)
		{
			*(C.mat + i*C.col + j) = *(A.mat + i*A.col + j) - (*(B.mat + i*B.col + j));
		}
	}
}
void ZMatrix::ZMatScaleMul(ZMatrix &A, ZMatrix &C, double s)
{
	for (int i = 0; i < C.row; i++)
	{
		for (int j = 0; j < C.col; j++)
		{
			*(C.mat + i*C.col + j) = s*(*(A.mat + i*A.col + j));
		}
	}
}