#ifndef ZZC_MATRIX_H
#define ZZC_MATRIX_H
class ZMatrix{

public:
	int row, col;
	double* mat;

	ZMatrix(int r,int c);
	~ZMatrix();
	static void ZMatMUL(ZMatrix& A, ZMatrix &B, ZMatrix &C);
	static void ZMatADD(ZMatrix& A, ZMatrix &B, ZMatrix &C);
	static void ZMatSUB(ZMatrix& A, ZMatrix &B, ZMatrix &C);
	static void ZMatScaleMul(ZMatrix &A, ZMatrix &B, double s);
	static void Reset(ZMatrix &A);
};
#endif