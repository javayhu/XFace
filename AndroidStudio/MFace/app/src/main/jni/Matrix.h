// ========================================================================
// (C) Copyright 2004,2005 Sony Corporation, All Rights Reserved
// @(#) $Id: Matrix.h,v 2.0 2005/11/08 00:50:11 kuni Exp $ 
//
// NAME
// Matrix.h
//
// DESCRIPTION
//
// HISTORY
//
// LOG
//   $Log: Matrix.h,v $
//   Revision 2.0  2005/11/08 00:50:11  kuni
//   no message
//
//   Revision 1.1.1.1  2005/11/08 00:48:22  kuni
//   Initial Revision
//
//   Revision 1.2  2005/05/27 08:48:08  takano
//   ヘッダーにヘッダーをつけた。
//
// ========================================================================
///For int
void matrix_Init2D_int(
		int dim1,
		int dim2,
		int **ppA
);
///For double
void matrix_Init2D_double(
		int dim1,
		int dim2,
		double **ppA
);
void matrix_Init1D_double(
		int dim1,		
		double *pA
);

void matrix_Init1D_int(
		int dim1,		
		int *pA
);
///For Complex
void matrix_Init1D_Complex(
		int dim1,
		Complex *pA
);
void matrix_Init2D_Complex(
		int dim1,
		int dim2,
		Complex **ppA
);
void matrix_Init3D_int(
		int dim1,
		int dim2,
		int dim3,
		int ***ppA
);
void matrix_Init3D_float(
		int dim1,
		int dim2,
		int dim3,
		float ***ppA
);
void matrix_Init3D_double(
		int dim1,
		int dim2,
		int dim3,
		double ***ppA
);
void matrix_Init3D_Complex(
		int dim1,
		int dim2,
		int dim3,
		Complex ***ppA
);
void matrix_Init4D_int(
		int dim1,
		int dim2,
		int dim3,
		int dim4,
		int ****ppA
);
void matrix_Init4D_Complex(
		int dim1,
		int dim2,
		int dim3,
		int dim4,
		Complex ****ppA
);

//2D Complex MatrixのReal表示
void matrix_Printf2D_ComplexR(
		int dim1,
		int dim2,
		Complex **ppA
);
//2D Complex MatrixのImag表示
void matrix_Printf2D_ComplexI(
		int dim1,
		int dim2,
		Complex **ppA
);

///

void matrix_Seikika(
	int iRow,
	int iCol,
	Complex **ppA, //Iinput  [Row][Col]
	Complex **ppB // Output(正規化されたもの)
);

double matrix_SoukanKeisuu( // aとbの相関値をリターンする
	int iRow,
	int iCol,
	Complex **a, // a[iRow][iCol]
	Complex **b // b[iRow][iCol]
);

void matrix_printf2(
// printf a[L][M] 
	int L,
	int M,
	Complex **a
);

///ファイル出力関数
void matrix_fprintf2(
	FILE *fp,
// printf a[L][M] 
	int L,
	int M,
	Complex **a
);
//Int型
void matrix_fprintf2D_int(
				char *pcFileName,					// ファイル名
				char *pcWriteMode,					// "w", "a"など書き込みモードを指定
				int  **ppA,
				int  dim1,
				int  dim2
);
//Double型
void matrix_fprintf2D_double(
				char *pcFileName,					// ファイル名
				char *pcWriteMode,					// "w", "a"など書き込みモードを指定
				double  **ppA,
				int  dim1,
				int  dim2
);

void matrix_make_unit( // 単位行列を作る
	int iRow, // iRow==iCol
	Complex **a // a[iRow][iRow]
);
void matrix_mult(
	int L,
	int M,
	int N,
	Complex **a,
	Complex **b,
	Complex **c
); /*** a[L][M]*b[M][N]=c[L][N] ***/

void matrix_small(
	int M,
	int N,
	Complex **a,
	Complex **b
); /*** b[M-1][M-1] = a[M][M]  ***/
  
Complex matrix_det(
	int M,
	int N,
	Complex **a
	//double det1
); /*** det1 = a[1][1] ***/

Complex matrix_det1(
	int M,
	int N,
	Complex **a
	//double det1
);

Complex matrix_det2(
	int M,
	int N,
	Complex **a
	//double det1
); 

Complex matrix_det3(
	int M,
	int N,
	Complex **a
	//double det1
); 


void matrix_mult_double(
// a[L][M]*b[M][N]=c[L][N] 
	int L,
	int M,
	int N,
	double **a,
	double **b,
	double **c
);

void matrix_add(
int L,
int N,
Complex **a,
Complex **b,
Complex **c
); /** a[L][N]+b[L][N]=c[L][N] **/

void matrix_add_double(
int L,
int N,
double **a,
double **b,
double **c
); /** a[L][N]+b[L][N]=c[L][N] **/

void matrix_add_double_1D(
int L,
double *a,
double *b,
double *c
); /** a[L]+b[L]=c[L] **/


void matrix_sub(
int L,
int N,
Complex **a,
Complex **b,
Complex **c
); /** a[L][N]-b[L][N]=c[L][N] **/

void matrix_rmul(
int L,
int N,
Complex **a,
double b,
Complex **c
); /** a[L][N]*b=c[L][N] **/

void matrix_rdiv(
int L,
int N,
Complex **a,
double b,
Complex **c
); /** a[L][N]/b=c[L][N] **/

void matrix_rdiv_double_1D(
int L,
double *a,
double b,
double *c
); /** a[L]/b=c[L] **/


void matrix_tenchi(
int L,
int N,
Complex **a,
Complex **b
); /*** T:tenchi gyouretu ***/

void matrix_tenchi_double(
// T:tenchi gyouretu
	int L,
	int N,
	double **a,
	double **b
);

void matrix_kyouyaku(
int L,
int N,
Complex **a,
Complex **b
); /*** *:kyouyaku gyouretu ***/

void matrix_tenkyou(
int L,
int N,
Complex **a,
Complex **b
); /**** H:tenchi kyouyaku ***/

void matrix_copy(
int L,
int N,
Complex **a,
Complex **b
); /*** copy matrix a to b ****/

void matrix_copy_double_1D(
int L,
double *a,
double *b
); /*** copy matrix a to b ****/

void matrix_copy_int_1D(
int L,
int *a,
int *b
); /*** copy matrix a to b ****/


void matrix_copy_double_2D(
// copy matrix a to b
	int L,
	int N,
	double **a,
	double **b
);

//実対称行列の個有値と固有ベクトル
void matrix_real_symmetric_eigen(
	int n,
	double **a, // a[n][n] is a symmetric matrix
	double *lambda, // lambda[n]
	double **v, // v[n][n] v[][0],v[][1],v[][2]
	int kmax // input learning time usually n*n
);

void matrix_complex_hermite_eigen(          //(A+iB)*(u+iv)=ramuda(u+iv)
	int nn, // size of Matrix of a
	Complex **a, // a[nn][nn] is a symmetric matrix 
	double *lambda, // lambda[nn] eigen value 
	Complex **v, // v[nn][nn] eigen vector retuhoukou vector ga nn ko
	int kmax // input learning time usualy 2*nn*2*nn 
);

void sqrtm(
		   int Row,
		   int Col,
		   double **H,		///Input
		   double **R		///Output
);

void svd(
	int Row,
	int Col,
	Complex **H, // H[Row][Col]=U*D*V_H
	Complex **U, // U[Row][iMinRowCol]
	Complex **D, // D[iMinRowCol][iMinRowCol]
	Complex **V  // V[Col][iMinRowCol)]
	// iMinRowCol= Min(Row,Col)
);

void lu2_1(
int JISUU,
Complex **a,
int *pivot
);

void lu2_2(
int JISUU,
Complex **a,
Complex *b,
int *pivot
);
void matrix_GeneralizedinverseUsingLU(
	int iMMSEMod, // 0:普通の逆行列 1:雑音電力を使用する。MMSE型の逆行列を生成する。
	double dNoisePower, // 雑音電力(Aが正規化されている場合の電力)
	// Row<=Col recommend 0: Row>Col recommend 1:
	int iMod, // 0:MinNorm(最小ノルム法) 1:LeastMean(最小２乗法) 2:自動選択（お任せ）
	int Row, 
	int Col,
	int iMinRowCol,  // min(Row,Col)
	Complex **A,   // A[Row][Col] input
	Complex **A_INV // A_INV[Col][Row] output
);

void matrix_generalizedinverse( // Using SVD
	int Row, 
	int Col,
	int iMinRowCol,  // min(Row,Col)
	Complex **A,   // A[Row][Col] input
	Complex **AINV // AINV[Col][Row] output
);
void LMS_1Output( // 
	// e=r-y=r-WH*X
	// W=W+dStep*X*eの共役
	int iJisuu,
	double dStep, // 
	Complex **r, // r[1][1]
	Complex **W, // W[iJisuu][1]	
	Complex **X // X[iJisuu][1]
);

void LMS_MultiOutput( // 
	// e=r-y=r-WH*X
	// W=W+dStep*X*eの共役
	// W[iCol][iRow]
	// WH[iRow][iCol] // これを基準に考える
	// X[iCol][1]
	// r[iRow][1]
	// y[iRow][1]

	int iRow,
	int iCol,
	double dStep, // 
	Complex **r, 
	Complex **W,
	Complex **X
);


void RLS(
	int iReset, // 1:reset 0:nomal 2:not update W 
	int iJisuu, // 次数
	double dBoukyakuKeisuu, // 0.95とか1.0とか
	Complex **W, // W[iJisuu][1], // suitei suru omomi iJisuu*1 
	Complex **X, // X[iJisuu][1], // nyuuryoku iJisuu*1
	Complex **R, // R[1][1], // kyoushi shinngou 1*1 
	Complex **GOSA, // GOSA[1][1], // gosa shinngou 
	Complex **I, // I[iJisuu][iJisuu] 
	Complex **P // P[iJisuu][iJisuu]
);

void qr( Complex **Q, Complex **R, int cls, int row );
void qr1( Complex **Q, Complex **R, int cls, int row );

//int dsvd(float **a, int m, int n, float *w, float **v);
int dsvd(double **a, int m, int n, double *w, double **v);