static char rcsid[]= 
"@(#) $Id: Matrix.cpp,v 2.0 2005/11/08 00:50:11 kuni Exp $";
// ========================================================================
// (C) Copyright 2004,2005 Sony Corporation, All Rights Reserved
//
// NAME
//  Matrix.cpp 
//
// DESCRIPTION
//
// HISTORY
// Ver0.1 2004.2.10�@SVD�̃o�O�C��
// Ver0.2 2004.11.30 SVD��V�̑��v�f�������ɂȂ�悤�Ɉʑ��̐��K�����s��
// Ver0.3 2004.12.08 Malloc���[�e�B���e�B�[�̕ύX
// Ver0.4 2004.1.20 matrix_fprintf��ǉ�
// Ver0.5 2004.1.21 LU�����̋t�s���MMSE�ŎG����0.0�ł�0.000001�Ōv�Z����悤�ɂ����B�G��0�ɂ��Ă��x�����łȂ��Ȃ��ʂ�����B
// Ver0.6 2005.10.6 �ŏ��Q��@�̃m�C�Y�ʂ�ύX
//
// LOG
//   $Log: Matrix.cpp,v $
//   Revision 2.0  2005/11/08 00:50:11  kuni
//   no message
//
//   Revision 1.1.1.1  2005/11/08 00:48:22  kuni
//   Initial Revision
//
//   Revision 1.2.2.2  2005/10/24 02:06:45  takano
//   �ŏ��Q��@�������g���B
//
//   Revision 1.2.2.1  2005/06/21 06:50:35  takano
//   �A���e�i�L�����u���[�V�����Ή��iMatrix.cpp,LinerAndDb.cpp�͌x�����v�����g�A�E�g����̂����߂��B�N���X�^�[���s����ɂȂ邩��B�Z���t��3-1,1->3�ɑΉ��B�u�����`�C���o�����X�̗^���������t���b�g�ɂ����B�d�͂��ۑ�����悤�ɂ����B��M���ŃL�����u���W����������悤�ɂ����B
//
//   Revision 1.2  2005/04/01 08:29:03  takano
//   �t�@�C���̐擪�Ƀw�b�_���������ƂƁA��L��
//
// ========================================================================
//----------------- Complex matrix  ------------- 
#include <math.h>
#include <stdlib.h>
#include "Complex.h"
#include "Malloc2.h"
#include "Matrix.h"
///================================Initialize Function================================//
//For Int
void matrix_Init2D_int(
		int dim1,
		int dim2,
		int **ppA
){
	int i,j;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			ppA[i][j] = 0;
		}
	}
}

void matrix_Init1D_int(
		int dim1,		
		int *pA
){
	int i;
	for(i=0; i<dim1; i++){
			pA[i] = 0.0;
	}
}


//For double
void matrix_Init2D_double(
		int dim1,
		int dim2,
		double **ppA
){
	int i,j;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			ppA[i][j] = 0.0;
		}
	}
}
//For double
void matrix_Init1D_double(
		int dim1,
		double *pA
){
	int i;
	for(i=0; i<dim1; i++){
			pA[i] = 0.0;
	}
}
//For Complex
void matrix_Init1D_Complex(
		int dim1,
		Complex *pA
){
	int i;
	for(i=0; i<dim1; i++){
		pA[i] = cmplx(0.0, 0.0);
	}
}



void matrix_Init2D_Complex(
		int dim1,
		int dim2,
		Complex **ppA
){
	int i,j;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			ppA[i][j] = cmplx(0.0, 0.0);
		}
	}
}
void matrix_Init3D_int(
		int dim1,
		int dim2,
		int dim3,
		int ***ppA
){
	int i,j,k;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			for(k=0; k<dim3; k++){
				ppA[i][j][k] = 0;
			}
		}
	}
}
void matrix_Init3D_double(
		int dim1,
		int dim2,
		int dim3,
		double ***ppA
){
	int i,j,k;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			for(k=0; k<dim3; k++){
				ppA[i][j][k] = 0.0;
			}
		}
	}
}
void matrix_Init3D_float(
		int dim1,
		int dim2,
		int dim3,
		float ***ppA
){
	int i,j,k;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			for(k=0; k<dim3; k++){
				ppA[i][j][k] = 0.0;
			}
		}
	}
}
void matrix_Init3D_Complex(
		int dim1,
		int dim2,
		int dim3,
		Complex ***ppA
){
	int i,j,k;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			for(k=0; k<dim3; k++){
				ppA[i][j][k] = cmplx(0.0, 0.0);
			}
		}
	}
}
void matrix_Init4D_int(
		int dim1,
		int dim2,
		int dim3,
		int dim4,
		int ****ppA
){
	int i,j,k,l;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			for(k=0; k<dim3; k++){
				for(l=0; l<dim4; l++){
					ppA[i][j][k][l] = 0;
				}
			}
		}
	}
}
void matrix_Init4D_Complex(
		int dim1,
		int dim2,
		int dim3,
		int dim4,
		Complex ****ppA
){
	int i,j,k,l;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			for(k=0; k<dim3; k++){
				for(l=0; l<dim4; l++){
					ppA[i][j][k][l] = cmplx(0.0, 0.0);
				}
			}
		}
	}
}

///================================Display Function================================//
//2D Complex Matrix��Real�\��
void matrix_Printf2D_ComplexR(
		int dim1,
		int dim2,
		Complex **ppA
){
	int i,j;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			printf("%f,", ppA[i][j].i);
		}
		printf("\n");
	}
}
//2D Complex Matrix��Imag�\��
void matrix_Printf2D_ComplexI(
		int dim1,
		int dim2,
		Complex **ppA
){
	int i,j;
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			printf("%f,", ppA[i][j].q);
		}
		printf("\n");
	}
}

///================================Seikika Function================================//
void matrix_Seikika(
	int iRow,
	int iCol,
	Complex **ppA, //Iinput  [Row][Col]
	Complex **ppB // Output(���K�����ꂽ����)
)
{
	int iIndex1,iIndex2;
	double dTemp;
	dTemp=0.0;
	for(iIndex1=0;iIndex1<iRow;iIndex1++){
		for(iIndex2=0;iIndex2<iCol;iIndex2++){
			dTemp+=complexpower(ppA[iIndex1][iIndex2]);
		}
	}
	dTemp/=(double)(iRow*iCol);
	//---- Seikika ---------
	for(iIndex1=0;iIndex1<iRow;iIndex1++){
		for(iIndex2=0;iIndex2<iCol;iIndex2++){
			ppB[iIndex1][iIndex2]=rmul(1.0/sqrt(dTemp),ppA[iIndex1][iIndex2]);
		}
	}
}

///================================Soukan Function================================//
double matrix_SoukanKeisuu( // a��b�̑��֒l�����^�[������
	int iRow,
	int iCol,
	Complex **a, // a[iRow][iCol]
	Complex **b // b[iRow][iCol]
)
{
	int iIndex1,iIndex2;
	double dBuff1,dBuff2;
	Complex cBuff3,cBuff4,cBuff5;

	dBuff1=0.0;
	dBuff2=0.0;
	cBuff5=cmplx(0.0,0.0);
	for(iIndex1=0;iIndex1<iRow;iIndex1++){
		for(iIndex2=0;iIndex2<iCol;iIndex2++){
			dBuff1+=pow(complexabs(a[iIndex1][iIndex2]),2.0);
			dBuff2+=pow(complexabs(b[iIndex1][iIndex2]),2.0);
			cBuff3=conjg(a[iIndex1][iIndex2]);
			cBuff4=cmul(cBuff3,b[iIndex1][iIndex2]);
			cBuff5=cadd(cBuff5,cBuff4);
		}
	}
	dBuff1/=(double)(iRow*iCol);
	dBuff2/=(double)(iRow*iCol);
	cBuff5=rmul(1.0/(double)(iRow*iCol),cBuff5); // ���q
	dBuff1=sqrt(dBuff1); // ����1
	dBuff2=sqrt(dBuff2); // ����2
	cBuff5=rmul(1.0/(dBuff1*dBuff2),cBuff5);
	return(complexabs(cBuff5));
}

void matrix_printf2(
// printf a[L][M] 
	int L,
	int M,
	Complex **a
)
{
	int iIndex1,iIndex2;
	for(iIndex1=0;iIndex1<L;iIndex1++){
		for(iIndex2=0;iIndex2<M;iIndex2++){
			printf("[%.10lf %.10lf] ",real(a[iIndex1][iIndex2]),aimag(a[iIndex1][iIndex2]));
		}
		printf("\n");
	}
}

///================================fprintf Function================================//
void matrix_fprintf2(
	FILE *fp,
// printf a[L][M] 
	int L,
	int M,
	Complex **a
)
{
	int iIndex1,iIndex2;
	for(iIndex1=0;iIndex1<L;iIndex1++){
		for(iIndex2=0;iIndex2<M;iIndex2++){
			fprintf(fp,"[%.10lf %.10lf] ",real(a[iIndex1][iIndex2]),aimag(a[iIndex1][iIndex2]));
		}
		fprintf(fp,"\n");
	}
}
///Int�^
void matrix_fprintf2D_int(
				char *pcFileName,					// �t�@�C����
				char *pcWriteMode,					// "w", "a"�ȂǏ������݃��[�h���w��
				int  **ppA,
				int  dim1,
				int  dim2
)
{
	int  i,j;
	FILE *fp;

	fp = fopen(pcFileName, pcWriteMode);
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			fprintf(fp, "%d,", ppA[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	return ;
}
///Double�^
void matrix_fprintf2D_double(
				char *pcFileName,					// �t�@�C����
				char *pcWriteMode,					// "w", "a"�ȂǏ������݃��[�h���w��
				double  **ppA,
				int  dim1,
				int  dim2
)
{
	int  i,j;
	FILE *fp;

	fp = fopen(pcFileName, pcWriteMode);
	for(i=0; i<dim1; i++){
		for(j=0; j<dim2; j++){
			fprintf(fp, "%f,", ppA[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

	return ;
}

///================================Unit Matrix Function================================//
void matrix_make_unit( // �P�ʍs������
	int iRow, // iRow==iCol
	Complex **a // a[iRow][iRow]
)
{
	int iIndex1,iIndex2;
	for(iIndex1=0;iIndex1<iRow;iIndex1++){
		for(iIndex2=0;iIndex2<iRow;iIndex2++){
			if(iIndex1==iIndex2){
				a[iIndex1][iIndex2]=cmplx(1.0,0.0);
			}
			else{
				a[iIndex1][iIndex2]=cmplx(0.0,0.0);	
			}
		}
	}
}

///================================Add/Sub/Mult/Div Function================================//
void matrix_mult(
// a[L][M]*b[M][N]=c[L][N] 
	int L,
	int M,
	int N,
	Complex **a,
	Complex **b,
	Complex **c
)
{
	int i,j,k;
	for(i=0;i<L;i++){
		for(j=0;j<N;j++){
			c[i][j]=cmplx(0.0,0.0);
			for(k=0;k<M;k++){
				c[i][j]=cadd(c[i][j],cmul(a[i][k],b[k][j]));
			}
		}
	}
}

void matrix_mult_double(
// a[L][M]*b[M][N]=c[L][N] 
	int L,
	int M,
	int N,
	double **a,
	double **b,
	double **c
)
{
	int i,j,k;
	for(i=0;i<L;i++){
		for(j=0;j<N;j++){
			c[i][j]=0.0;
			for(k=0;k<M;k++){
				c[i][j] = c[i][j] + a[i][k]*b[k][j];
			}
		}
	}
}

void matrix_add(
// a[L][N]+b[L][N]=c[L][N] 
	int L,
	int N,
	Complex **a,
	Complex **b,
	Complex **c
)
{
	int i,j;
	for(i=0;i<L;i++){
		for(j=0;j<N;j++){
			c[i][j]=cadd(a[i][j],b[i][j]);
		}
	}
}

void matrix_add_double(
int L,
int N,
double **a,
double **b,
double **c
){ /** a[L][N]+b[L][N]=c[L][N] **/
	int i,j;
	for(i=0;i<L;i++){
		for(j=0;j<N;j++){
			c[i][j]=a[i][j]+b[i][j];
		}
	}
}

void matrix_add_double_1D(
int L,
double *a,
double *b,
double *c
){ /** a[L]+b[L]=c[L] **/

	int i;
	for(i=0;i<L;i++){
		
			c[i]=a[i]+b[i];
	
	}
}

void matrix_sub(
// a[L][N]-b[L][N]=c[L][N]
	int L,
	int N,
	Complex **a,
	Complex **b,
	Complex **c
)
{
        int i,j;
        for(i=0;i<L;i++){
                for(j=0;j<N;j++){
                        c[i][j]=csub(a[i][j],b[i][j]);
                } 
        } 
}


void matrix_rmul(
// a[L][N]*b=c[L][N]
	int L,
	int N,
	Complex **a,
	double b,
	Complex **c
)
{
        int i,j;
        for(i=0;i<L;i++){
                for(j=0;j<N;j++){
                        c[i][j]=rmul(b,a[i][j]);
                } 
        } 
}
void matrix_rdiv(
// a[L][N]/b=c[L][N]
	int L,
	int N,
	Complex **a,
	double b,
	Complex **c
)
{ 
        int i,j; 
        for(i=0;i<L;i++){ 
                for(j=0;j<N;j++){ 
                        c[i][j]=rdiv(a[i][j],b); 
                }  
        }  
}

void matrix_rdiv_double_1D(
int L,
double *a,
double b,
double *c
){ /** a[L]/b=c[L] **/
	for(int j=0;j<L;j++){ 
                        c[j]=a[j]/b; 
                }
}




///================================Transform Function================================//
void matrix_tenchi(
// T:tenchi gyouretu
	int L,
	int N,
	Complex **a,
	Complex **b
)
{
	int i,j;
	for(i=0;i<L;i++){
		for(j=0;j<N;j++){
			b[j][i]=a[i][j];
		}
	}
}

void matrix_tenchi_double(
// T:tenchi gyouretu
	int L,
	int N,
	double **a,
	double **b
)
{
	int i,j;
	for(i=0;i<L;i++){
		for(j=0;j<N;j++){
			b[j][i]=a[i][j];
		}
	}
}

void matrix_kyouyaku(
//kyouyaku gyouretu
	int L,
	int N,
	Complex **a,
	Complex **b
)
{
        int i,j; 
        for(i=0;i<L;i++){ 
                for(j=0;j<N;j++){ 
                        b[i][j]=conjg(a[i][j]); 
                } 
        } 
}
void matrix_tenkyou(
// H:tenchi kyouyaku
	int L,
	int N,
	Complex **a,
	Complex **b
)
{ 
        int i,j;  
        for(i=0;i<L;i++){  
                for(j=0;j<N;j++){  
                        b[j][i]=conjg(a[i][j]);  
                }  
        }  
}

void matrix_copy(
// copy matrix a to b
	int L,
	int N,
	Complex **a,
	Complex **b
)
{
        int i,j;  
        for(i=0;i<L;i++){  
                for(j=0;j<N;j++){  
                        b[i][j]=a[i][j];
                } 
        } 
}

void matrix_copy_double_2D(
// copy matrix a to b
	int L,
	int N,
	double **a,
	double **b
)
{
        int i,j;  
        for(i=0;i<L;i++){  
                for(j=0;j<N;j++){  
                        b[i][j]=a[i][j];
                } 
        } 
}

void matrix_copy_double_1D(
int L,
double *a,
double *b
){
	 int i;  
        for(i=0;i<L;i++){  
                  
             b[i]=a[i];
        }
}

void matrix_copy_int_1D(
int L,
int *a,
int *b
){ /*** copy matrix a to b ****/
	 int i;  
        for(i=0;i<L;i++){  
                  
             b[i]=a[i];
        }
}

void matrix_small(
  // a(M x M) change to  b( M-1 x M-1) without N column 
	int M,
	int N,
	Complex **a,
	Complex **b
)
{
	int i;
	int j;
	if( N == 1 ){
		for( i = 0; i< (M-1); i++){
			for( j = 0; j<(M-1); j++){
				b[i][j] = a[ i +1 ][ j +1];
			}
		}
	}
	else{
		for( i = 0; i < N; i++){
			for(j = 0; j< (M-1) ; j++){
				b[i][j] = a[i+1][j];
			}
		}
		for( i = N; i< (M-1); i++){
			for(j = 0; j< (M-1); j++){
				b[i][j] = a[i+1][j+1];
		
			}
		}
	}
}

Complex matrix_det(
 // answer = det( a[M][N] ) 
 int M,
 int N,
 Complex **a
 )
{
	Complex det1;
	det1 = cmplx(0.0, 0.0);


	if(M == N)
	{
		if(M == 1)
		{
			det1.i = sqrt( a[0][0].i * a[0][0].i + a[0][0].q * a[0][0].q );
		}
		else if(M == 2)
		{
			det1 = csub(cmul(a[0][0], a[1][1]) , cmul( a[1][0] , a[0][1]) );
		}
		else if(M == 3)
		{
			det1 = matrix_det1( M, N, a );
		}
		else if(M == 4)
		{
		det1 = matrix_det2( M, N, a );
		}
		else
		{
		printf("Sorry. not supported at Matrix.cpp. function of matrix_det is not supported over 4 Matrix \n");
		exit(1);
		// det1 = matrix_det3( M, N, a );
		}

	}
	else
	{
		printf("matrix_det supposes square matrix only. \n ");
	}
	//printf("check2 = %f \n", det1.i);
	return (det1);
}

Complex matrix_det1(
 // answer = det( a[M][N] ) 
 int M,
 int N,
 Complex **a
 //double det1
 )
{
	//double det1 =0.0;
	Complex det2;
	det2 = cmplx(0.0, 0.0);

	if(M == N)
	{
		// ��`
			Complex b1;
			Complex b2;
			Complex b3;
			Complex d;

			b1 = cmplx(0.0, 0.0);
			b2 = cmplx(0.0, 0.0);
			b3 = cmplx(0.0, 0.0);
			d = cmplx(0.0, 0.0);

			Complex **C1;
			Complex **C2;
			Complex **C3;

			C1 = (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
			C2 = (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
			C3 = (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));

			C1[0][0] =a[1][1];  C1[0][1] =a[1][2]; C1[1][0] =a[2][1]; C1[1][1] =a[2][2];
			C2[0][0] =a[0][1];  C2[0][1] =a[0][2]; C2[1][0] =a[2][1]; C2[1][1] =a[2][2];
			C3[0][0] =a[0][1];  C3[0][1] =a[0][2]; C3[1][0] =a[1][1]; C3[1][1] =a[1][2];

			// Caluculation

			b1 = cmul( a[0][0], matrix_det(2, 2, C1) );
			b2 = cmul( a[1][0], matrix_det(2, 2, C2) );
			b3 = cmul( a[2][0], matrix_det(2, 2, C3) );

			d = cadd( d, b1);
			d = csub( d, b2);
			d = cadd( d, b3);

			det2 = d;

			Free2D((void **) C1, 4);
			Free2D((void **) C2, 4);
			Free2D((void **) C3, 4);
	}

return (det2);
}


Complex matrix_det2(
 // answer = det( a[M][N] ) 
 int M,
 int N,
 Complex **a
 //double det1
 )
{
	Complex det3;
	det3 = cmplx(0.0, 0.0);

	if(M == N)
	{
		// ��`
			Complex b41;
			Complex b42;
			Complex b43;
			Complex b44;
			Complex d4;

			b41 = cmplx(0.0, 0.0);
			b42 = cmplx(0.0, 0.0);
			b43 = cmplx(0.0, 0.0);
			d4 =  cmplx(0.0, 0.0);

			Complex **C41;
			Complex **C42;
			Complex **C43;
			Complex **C44;

			C41 = (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
			C42 = (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
			C43 = (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
			C44 = (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));

			C41[0][0] =a[1][1];  C41[0][1] =a[1][2]; C41[0][2] =a[1][3]; C41[1][0] =a[2][1]; C41[1][1] =a[2][2]; C41[1][2] =a[2][3]; C41[2][0] =a[3][1]; C41[2][1] =a[3][2]; C41[2][2] =a[3][3];
			C42[0][0] =a[0][1];  C42[0][1] =a[0][2]; C42[0][2] =a[0][3]; C42[1][0] =a[2][1]; C42[1][1] =a[2][2]; C42[1][2] =a[2][3]; C42[2][0] =a[3][1]; C42[2][1] =a[3][2]; C42[2][2] =a[3][3];
			C43[0][0] =a[0][1];  C43[0][1] =a[0][2]; C43[0][2] =a[0][3]; C43[1][0] =a[1][1]; C43[1][1] =a[1][2]; C43[1][2] =a[1][3]; C43[2][0] =a[3][1]; C43[2][1] =a[3][2]; C43[2][2] =a[3][3];
			C44[0][0] =a[0][1];  C44[0][1] =a[0][2]; C44[0][2] =a[0][3]; C44[1][0] =a[1][1]; C44[1][1] =a[1][2]; C44[1][2] =a[1][3]; C44[2][0] =a[2][1]; C44[2][1] =a[2][2]; C44[2][2] =a[2][3]; 

			// Caluculation

			b41 = cmul( a[0][0], matrix_det(3, 3, C41) );
			b42 = cmul( a[1][0], matrix_det(3, 3, C42) );
			b43 = cmul( a[2][0], matrix_det(3, 3, C43) );
			b44 = cmul( a[3][0], matrix_det(3, 3, C44) );

			d4 = cadd( d4, b41);
			d4 = csub( d4, b42);
			d4 = cadd( d4, b43);
			d4 = csub( d4, b44);

			det3 = d4;

			Free2D((void **) C41, 4);
			Free2D((void **) C42, 4);
			Free2D((void **) C43, 4);
			Free2D((void **) C44, 4);
	}

return (det3);
}



Complex matrix_det3(
 // answer = det( a[M][N] ) 
 int M,
 int N,
 Complex **a
 )
{
	Complex det3;
	det3 = cmplx(0.0, 0.0);

	if(M == N)
	{
			Complex bb;
			bb = cmplx(0.0, 0.0);
			Complex dd;
			dd = cmplx(0.0, 0.0);

			Complex **CC;
			CC = (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));


			for(int k =0; k < M; k++)
			{
				 matrix_small( M, (k+1), a, CC);
				 bb = matrix_det( (M-1), (M-1), CC);
				 if( (k+1)%2 == 0 )
				 {
					 dd =csub( dd,  cmul( a[k][0] ,  bb) );
				 }
				 else
				 {
					 dd =cadd(dd,  cmul( a[k][0] ,  bb) );
				 }
			}
			det3 = dd;
			Free2D((void **) CC, 4);
	}

return (det3);
}





///================================Eigen Function================================//
void matrix_real_symmetric_eigen(
	int n,
	double **a, // a[n][n] is a symmetric matrix
	double *lambda, // lambda[n]
	double **v, // v[n][n] v[][0],v[][1],v[][2]
	int kmax // input learning time usually n*n
)
{
	int i,j,k,p,q;
	double d,eps,theta,s,c;
	double oldapj,oldaqj,oldaip,oldaiq,oldvip,oldviq;
	/** set v[n][n] to be an identity matrix ***/
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			v[i][j]=(i==j) ? (1.0):(0.0);
		}
	}
	/** find th absMAX of a[n][n] **/
	d=0.0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			d=(fabs(a[i][j])>d) ? (fabs(a[i][j])):(d);
		}
	}
	/** set epsilon **/
	eps=d*1.0e-16;
	/** begin main loop **/
	kmax=(kmax>=n*n) ? (kmax):(n*n);
	for(k=0;k<kmax;k++){
		/** find the absMAX element of a[n][n] **/
		p=0;
		q=1;
		for(i=0;i<n-1;i++){
			for(j=i+1;j<n;j++){
				if( fabs(a[i][j])>fabs(a[p][q]) ){
					p=i;
					q=j;
				}
			}
		}
		if(fabs(a[p][q])<eps) break;
		/** compute theta **/
		if(fabs(a[q][q]-a[p][p])<eps){
			theta=0.7853981634;
		}
		else{
			theta=0.5*atan(2.0*a[p][q]/(a[q][q]-a[p][p]));
		}
		/** compute elements **/
		s=sin(theta);
		c=cos(theta);
		for(j=0;j<n;j++){
			oldapj=a[p][j];
			oldaqj=a[q][j];
			a[p][j]=oldapj*c-oldaqj*s;
			a[q][j]=oldapj*s+oldaqj*c;
		}
		for(i=0;i<n;i++){
			oldaip=a[i][p];
			oldaiq=a[i][q];
			a[i][p]=oldaip*c-oldaiq*s;
			a[i][q]=oldaip*s+oldaiq*c;
			oldvip=v[i][p];
			oldviq=v[i][q];
			v[i][p]=oldvip*c-oldviq*s;
			v[i][q]=oldvip*s+oldviq*c;
		}
	}
	/** store eigenvalues **/
	for(i=0;i<n;i++){
		lambda[i]=a[i][i];
	}
}

///================================Hermite Eigen Function================================//
//�G���~�[�g�s��̌L�l�ƌŗL�x�N�g��
void matrix_complex_hermite_eigen(          //(A+iB)*(u+iv)=ramuda(u+iv)
	int nn, // size of Matrix of a
	Complex **a, // a[nn][nn] is a symmetric matrix 
	double *lambda, // lambda[nn] eigen value 
	Complex **v, // v[nn][nn] v[][0],v[][1]eigen vector retuhoukou vector ga nn ko
	int kmax // input learning time usualy 2*nn*2*nn 
)
{
	int iIndex1,iIndex2;
	int iInitFlag;
	int iIndexMax;
	int *piIndexEigenvalue_Big; // piIndexEigenvalue_Big[nn*2]
	piIndexEigenvalue_Big=(int *)calloc(nn*2,sizeof(int));
	int *piFlagEigen; // piFlagEigen[nn*2]
	piFlagEigen=(int *)calloc(nn*2,sizeof(int));

	iIndexMax = 0;

	int i,j;
	double  **a_big; // a_big[2*nn][2*nn];
	double **eigenvector_big; //eigenvector_big[2*nn][2*nn];
	double *eigenvalue_big; // eigenvalue_big[2*nn];
	//--------calloc --------------------
	a_big=(double **)ppvMalloc2D(
		(2*nn),
		(2*nn),
		sizeof(double *),
		sizeof(double)
	);
	eigenvector_big=(double **)ppvMalloc2D(
		(2*nn),
		(2*nn),
		sizeof(double *),
		sizeof(double)
	);
	eigenvalue_big=(double *)pvMalloc1D(
		(2*nn),
		sizeof(double)
	);
	/*** make a_big ***/
	for(i=0;i<2*nn;i++){
		for(j=0;j<2*nn;j++){
			if((i<nn)&&(j<nn)){ /** A **/
				a_big[i][j]=real(a[i][j]);
			}
			else if((i<nn)&&(j>=nn)){ /** -B **/
				a_big[i][j]=-1.0*aimag(a[i][j-nn]);
			}
			else if((i>=nn)&&(j<nn)){ /** B **/
				a_big[i][j]=aimag(a[i-nn][j]);	
			}
			else{ /** A ***/
				a_big[i][j]=real(a[i-nn][j-nn]);
			}
//			printf("%lf ",a_big[i][j]);
		}
//		printf("\n");	
	}
	//---- �����̌J��Ԃ��񐔂����x�ɉe������H
/***
	int iIndex1,iIndex2;
	for(iIndex1=0;iIndex1<(2*nn);iIndex1++){
		for(iIndex2=0;iIndex2<(2*nn);iIndex2++){
			printf("%lf ",a_big[iIndex1][iIndex2]);
		}
		printf("\n");
	}
***/
	matrix_real_symmetric_eigen((2*nn),a_big,eigenvalue_big,eigenvector_big,kmax);
	// ���S�ɓ����ŗL�l���o�Ă����ꍇ�̕t���Đn�̑΍�

	int iPerfectFlag;
	iPerfectFlag=1;
	for(iIndex1=1;iIndex1<nn*2;iIndex1++){
		if(eigenvalue_big[iIndex1]!=eigenvalue_big[iIndex1-1]){
			iPerfectFlag=0;
		}
	}

	if(iPerfectFlag==1){
		for(iIndex1=0;iIndex1<nn;iIndex1++){
			piIndexEigenvalue_Big[iIndex1*2+0]=iIndex1;
			piIndexEigenvalue_Big[iIndex1*2+1]=iIndex1;
		}
	}
	else{
		//--- �ɂ̑傫���ŕ��ׂ����Ԃ̃C���f�b�N�X -------
		for(iIndex1=0;iIndex1<nn*2;iIndex1++){
			piFlagEigen[iIndex1]=0;
		}
		for(iIndex1=0;iIndex1<nn*2;iIndex1++){
			iInitFlag=0;
			for(iIndex2=0;iIndex2<nn*2;iIndex2++){
				if(piFlagEigen[iIndex2]==0){
					if(iInitFlag==0){
						iIndexMax=iIndex2;
						iInitFlag=1;
					}
					if(eigenvalue_big[iIndexMax]<eigenvalue_big[iIndex2]){
						iIndexMax=iIndex2;
					}
				}
			}
			piFlagEigen[iIndexMax]=1;	
			piIndexEigenvalue_Big[iIndex1]=iIndexMax;	
		}
	}			
	/**** 2kumi kara 1kumi wo erabu ***/
	for(i=0;i<nn;i++){
		lambda[i]=eigenvalue_big[piIndexEigenvalue_Big[i*2]];
	}
	/**** caution!!! eigen vector ha retu vector no atumari ****/
	for(j=0;j<nn;j++){
		for(i=0;i<nn;i++){
			v[i][j]=cmplx(eigenvector_big[i][piIndexEigenvalue_Big[j*2]],eigenvector_big[i+nn][piIndexEigenvalue_Big[j*2]]);
		}
	}
/***	
	printf("V=\n");
	matrix_printf2(2,2,v);

	for(i=0;i<4;i++){
		printf("JUNBAN[%d]=%d\n",i,piIndexEigenvalue_Big[i]);
	}

	for(i=0;i<4;i++){
		printf("lambda_Big[%d]=%lf\n",i,eigenvalue_big[i]);
	}
	printf("Eigenvalue_Big=\n");
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			printf("%lf ",eigenvector_big[i][j]);
		}
		printf("\n");
	}
**/
	//---- free -----------------
	Free2D(
		(void **)a_big,
		(2*nn)
	);
	Free2D(
		(void **)eigenvector_big,
		(2*nn)
	);
	
	Free1D(
		(void *)eigenvalue_big
	);
	free(piIndexEigenvalue_Big);
	free(piFlagEigen);
}

/***
//���ڂ̕��f�́@�G���~�[�g�s��̌L�l�ƌŗL�x�N�g��
void matrix_direct_hermite_eigen(
	int n,
	Complex **a, // a[n][n] is a symmetric matrix
	double *lambda, // lambda[n]
	Complex **v, // v[n][n] v[][0],v[][1],v[][2]
	int kmax // input learning time usually n*n
)
{
	int i,j,k,p,q;
	double d,eps,theta,s,c;
	Complex oldapj,oldaqj,oldaip,oldaiq,oldvip,oldviq;
	//-- set v[n][n] to be an identity matrix 
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(i==j){
				v[i][j]=cmplx(1.0,0.0);
			}
			else{
				v[i][j]=cmplx(0.0,0.0);
			}
			//v[i][j]=(i==j) ? (1.0):(0.0);
		}
	}
	//-- find th absMAX of a[n][n] 
	d=0.0;
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			d=(complexabs(a[i][j])>d) ? (complexabs(a[i][j])):(d);
		}
	}
	//--- set epsilon 
	eps=d*1.0e-16;
	//--- begin main loop 
	kmax=(kmax>=n*n) ? (kmax):(n*n);
	for(k=0;k<kmax;k++){
		// find the absMAX element of a[n][n] 
		p=0;
		q=1;
		for(i=0;i<n-1;i++){
			for(j=i+1;j<n;j++){
				if( complexabs(a[i][j])>complexabs(a[p][q]) ){
					p=i;
					q=j;
				}
			}
		}
		if(complexabs(a[p][q])<eps) break;
		// compute theta 
		if(complexabs(csub(a[q][q],a[p][p]))<eps){
			theta=0.7853981634;
		}
		else{
KOKOMONDAI			theta=0.5*atan(2.0*a[p][q]/(a[q][q]-a[p][p]));
		}
		// compute elements 
		s=sin(theta);
		c=cos(theta);
		for(j=0;j<n;j++){
			oldapj=a[p][j];
			oldaqj=a[q][j];
		//	a[p][j]=oldapj*c-oldaqj*s;
			a[p][j]=csub(rmul(c,oldapj),rmul(s,oldaqj));
		//	a[q][j]=oldapj*s+oldaqj*c;
			a[q][j]=cadd( rmul(s,oldapj),rmul(c,oldaqj));
		}
		for(i=0;i<n;i++){
			oldaip=a[i][p];
			oldaiq=a[i][q];
		//	a[i][p]=oldaip*c-oldaiq*s;
			a[i][p]=csub(rmul(c,oldaip),rmul(s,oldaiq));
		//	a[i][q]=oldaip*s+oldaiq*c;
			a[i][q]=cadd(rmul(s,oldaip),rmul(c,oldaiq));
			oldvip=v[i][p];
			oldviq=v[i][q];
		//	v[i][p]=oldvip*c-oldviq*s;
			v[i][p]=csub(rmul(c,oldvip),rmul(s,oldviq));
		//	v[i][q]=oldvip*s+oldviq*c;
			v[i][q]=cadd(rmul(s,oldvip),rmul(c,oldviq));
		}
	}
	// store eigenvalues 
	for(i=0;i<n;i++){
		lambda[i]=real(a[i][i]);
	}
}
***/

/////================================sqrtm================================//
////�s��̕����Ԃ�
//void sqrtm(
//		   int Row,
//		   int Col,
//		   double **H,		///Input
//		   double **R		///Output
//){
//
//	int i,j;
//
//	Complex **Hin;
//	Complex **U;
//	Complex **D;
//	Complex **V;
//	Complex **D_sqrt;
//	Complex **V_t;
//	Complex **Buf1;
//	Complex **Buf2;
//	double *dD_vec;
//	double **dD;
//	double **dV;
//
//	Hin		= (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
//	U		= (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
//	D		= (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
//	V		= (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
//	D_sqrt	= (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
//	V_t		= (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
//	Buf1	= (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
//	Buf2	= (Complex **)ppvMalloc2D(4, 4, sizeof(Complex *), sizeof(Complex));
//	dD_vec	= (double *)malloc( 4 * sizeof(double) );
//	dD		= (double **)ppvMalloc2D(4, 4, sizeof(double *), sizeof(double));
//	dV		= (double **)ppvMalloc2D(4, 4, sizeof(double *), sizeof(double));
//
//	///double to Complex
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			Hin[i][j].i = H[i][j];
//			Hin[i][j].q = 0.0;
//		}
//	}
//	
//	/*
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			printf("%f,", Hin[i][j].i);	
//		}
//		printf("\n");
//	}
//	*/
//
//	///SVD����
//	/*
//	svd(
//		Row,
//		Col,
//		Hin,
//		U,
//		D,
//		V
//		);
//	*/
//
//	dsvd(
//		H,
//		Row,
//		Col,
//		dD_vec,
//		dV
//		);
//
//	//Vector to Matrix
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			if(i==j){
//				dD[i][j] = dD_vec[i];
//			}else{
//				dD[i][j] = 0.0;
//			}
//		}
//	}	
//
//	///Check
//	/*
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			//printf("%f,", U[i][j].i);
//			//printf("%f,", D[i][j].i);
//			printf("%f,", V[i][j].i);
//			//printf("%f %f ", U[i][j].i, U[i][j].q);
//			//printf("%f %f ", D[i][j].i, D[i][j].q);
//			//printf("%f %f ", V[i][j].i, V[i][j].q);
//		}
//		printf("\n");
//	}
//	//*/
//	/*
//	printf("Loop\n");
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			//printf("%f,", H[i][j]);
//			//printf("%f,", dD[i][j]);
//			printf("%f,", dV[i][j]);
//		}
//		printf("\n");
//	}
//	//*/
//	
//	///Double to Complex
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			D[i][j].i = dD[i][j];
//			D[i][j].q = 0.0;
//			U[i][j].i = H[i][j];
//			U[i][j].q = 0.0;
//			V[i][j].i = dV[i][j];
//			V[i][j].q = 0.0;
//		}
//	}
//
//	///D�ɑ΂���sqrt
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			D_sqrt[i][j].i = sqrt(D[i][j].i);
//			D_sqrt[i][j].q = sqrt(D[i][j].q);
//		}
//	}
//
//	///U*D_sqrt*V'
//	matrix_mult(Row, Col, Row, U, D_sqrt, Buf1);
//	matrix_tenchi(Row, Col, V, V_t);
//	matrix_mult(Row, Col, Row, Buf1, V_t, Buf2);
//
//	///Complex to Double
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			R[i][j] = Buf2[i][j].i;
//		}
//	}	
//
///*
//	///D�ɑ΂���sqrt
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			D_sqrt[i][j].i = sqrt(D[i][j].i);
//			D_sqrt[i][j].q = sqrt(D[i][j].q);
//		}
//	}
//
//	///U*D_sqrt*V'
//	matrix_mult(Row, Col, Row, U, D_sqrt, Buf1);
//	matrix_tenchi(Row, Col, V, V_t);
//	matrix_mult(Row, Col, Row, Buf1, V_t, Buf2);
//
//	///Complex to Double
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			R[i][j] = Buf2[i][j].i;
//		}
//	}	
//*/
//
//	///Check
//	/*
//	for(i=0; i<Row; i++){
//		for(j=0; j<Col; j++){
//			printf("%f %f ", U[i][j].i, U[i][j].q);
//			printf("%f %f ", D[i][j].i, D[i][j].q);
//			printf("%f %f ", V[i][j].i, V[i][j].q);
//			printf("%f %f ", D_sqrt[i][j].i, D_sqrt[i][j].q);
//			printf("%f %f ", Buf2[i][j].i, Buf2[i][j].q);
//			printf("%f ", R[i][j]);
//		}
//		printf("\n");
//	}
//	//*/
//
//	Free2D((void **)Hin,4);
//	Free2D((void **)U, 4);
//	Free2D((void **)D, 4);
//	Free2D((void **)V, 4);
//	Free2D((void **)D_sqrt, 4);
//	Free2D((void **)V_t, 4);
//	Free2D((void **)Buf1, 4);
//	Free2D((void **)Buf2, 4);
//	free(dD_vec);
//	Free2D((void **)dD, 4);
//	Free2D((void **)dV, 4);
//	
//}
//


///================================SVD Function================================//
void svd(
	int Row,
	int Col,
	Complex **H, // H[Row][Col]=U*D*V_H
	Complex **U, // U[Row][iMinRowCol)]
	Complex **D, // D[iMinRowCol][iMinRowCol]
	Complex **V  // V[Col][iMinRowCol]
	// iMinRowCol= Min(Row,Col)
)
{
	int iIndex1,iIndex2,iIndex3;
	int iMinRowCol;
	int iMaxRowCol;
	int flag;
	Complex EigenVectorBuff[1000];
	double lambda_Temp;
	int iRealRank;
	double dBunbo;

	Complex cKaitenBuff,cKaitenBuff2;
	double dBuff1;

	Complex **Temp1; // [Row][iMinRowCol]
	Complex **Temp2; // [Col][Col]
	Complex **H_T; // [Col][Row]
	Complex **H_TH; //[Col][Col]
	Complex **HH_T; //[Row][Row]
	double *lambda; //[iMinRowCol]
	Complex **D_Inv; //[iMinRowCol][iMinRowCol]
	Complex **U_T; //[iMinRowCol][Row]
	Complex **V_T; //[iMinRowCol][Col]
	if(Row>=Col){
		iMinRowCol=Col;
		iMaxRowCol=Row;
	}
	else{
		iMinRowCol=Row;
		iMaxRowCol=Col;
	}

	cKaitenBuff = cmplx(0.0, 0.0);
	cKaitenBuff2 = cmplx(0.0, 0.0);

	//---- Calloc -----------------
	D_Inv=(Complex**)ppvMalloc2D(
		iMinRowCol,
		iMinRowCol,
		sizeof(Complex *),
		sizeof(Complex)
	);	
	if(Row>=Col){
		Temp1=(Complex**)ppvMalloc2D( // V
			Col,
			Col,
			sizeof(Complex *),
			sizeof(Complex)
		);
		Temp2=(Complex**)ppvMalloc2D( // U
			Row,
			iMinRowCol,
			sizeof(Complex *),
			sizeof(Complex)
		);
	}
	else{
		Temp1=(Complex**)ppvMalloc2D( // U
			Row,
			Row,
			sizeof(Complex *),
			sizeof(Complex)
		);
		Temp2=(Complex**)ppvMalloc2D( // V
			iMinRowCol,
			Col,
			sizeof(Complex *),
			sizeof(Complex)
		);
	}
	H_T=(Complex**)ppvMalloc2D(
		Col,
		Row,
		sizeof(Complex *),
		sizeof(Complex)
	);
	H_TH=(Complex**)ppvMalloc2D(
		Col,
		Col,
		sizeof(Complex *),
		sizeof(Complex)
	);
	HH_T=(Complex**)ppvMalloc2D(
		Row,
		Row,
		sizeof(Complex *),
		sizeof(Complex)
	);
	lambda=(double *)pvMalloc1D(
		iMinRowCol,
		sizeof(double)
	);
	U_T=(Complex**)ppvMalloc2D(
		iMinRowCol,
		Row,
		sizeof(Complex *),
		sizeof(Complex)
	);
	V_T=(Complex**)ppvMalloc2D(
		iMinRowCol,
		Col,
		sizeof(Complex *),
		sizeof(Complex)
	);
	//---------------------------
/**
			H[0][0]=cmplx(0.0,1.41); // FOR DEBUG
			H[0][1]=cmplx(0.0,0.0); // FOR DEBUG
			H[1][0]=cmplx(0.0,0.0); // FOR DEBUG
			H[1][1]=cmplx(0.0,1.41); // FOR DEBUG
**/
/**
			H[0][0]=cmplx(1.000000000000001,0.0); // FOR DEBUG
			H[0][1]=cmplx(0.000000000000002,0.0); // FOR DEBUG
			H[1][0]=cmplx(0.000000000000003,0.0); // FOR DEBUG
			H[1][1]=cmplx(1.000000000000004,0.0); // FOR DEBUG
**/
	matrix_tenkyou(Row,Col,H,H_T);
	matrix_mult(Col,Row,Col,H_T,H,H_TH);
	matrix_mult(Row,Col,Row,H,H_T,HH_T);
/**
	printf("H=\n");
	matrix_printf2(2,2,H);
	printf("H_T=\n");
	matrix_printf2(2,2,H_T);
	printf("H_TH=\n");
	matrix_printf2(2,2,H_TH);
	matrix_printf2(2,2,H_TH);
**/
	//--- V ----
	if(Row>=Col){
		matrix_complex_hermite_eigen(Col,H_TH,lambda,Temp1,(2*Col*2*Col));
	}
	else{
		matrix_complex_hermite_eigen(Row,HH_T,lambda,Temp1,(2*Row*2*Row));	
	}

	//-- rearrange ------
	for(iIndex1=0;iIndex1<(iMinRowCol-1);iIndex1++){
		flag=1;
		iIndex2=iIndex1+1;
		while(flag==1){
			//--- ���ׂƂ̔�r�Ɠ��ւ�------		
			if(lambda[iIndex2-1]<lambda[iIndex2]){
				lambda_Temp=lambda[iIndex2-1];
				lambda[iIndex2-1]=lambda[iIndex2];
				lambda[iIndex2]=lambda_Temp;
				for(iIndex3=0;iIndex3<iMinRowCol;iIndex3++){	
					EigenVectorBuff[iIndex3]=Temp1[iIndex3][iIndex2-1];
				}
				for(iIndex3=0;iIndex3<iMinRowCol;iIndex3++){	
					Temp1[iIndex3][iIndex2-1]=Temp1[iIndex3][iIndex2];
				}
				for(iIndex3=0;iIndex3<iMinRowCol;iIndex3++){	
					Temp1[iIndex3][iIndex2]=EigenVectorBuff[iIndex3];
				}
				
				if(iIndex2==1){
					flag=0;
				}
				else{
					iIndex2--;
				}
			}
			else{
				flag=0;
			}
		}
	}
/**
	for(iIndex1=0;iIndex1<iMinRowCol;iIndex1++){
	printf("lambda=%lf\n",lambda[iIndex1]);
	}
**/
	//----- Temp1 no Seikika ------------------------
	//--- Norm no Seikika -----------
	for(iIndex1=0;iIndex1<iMinRowCol;iIndex1++){
		dBunbo=0.0;
		for(iIndex2=0;iIndex2<iMinRowCol;iIndex2++){
			dBunbo+=pow(complexabs(Temp1[iIndex2][iIndex1]),2.0);
		}
		dBunbo=sqrt(dBunbo);
		for(iIndex2=0;iIndex2<iMinRowCol;iIndex2++){
			Temp1[iIndex2][iIndex1]=rmul(1.0/dBunbo,Temp1[iIndex2][iIndex1]);
		}
	}
	//-------- SET U_Temp->U,V_Temp->V  lambda->D ------------------
	iRealRank=0;
	for(iIndex1=0;iIndex1<iMinRowCol;iIndex1++){
		if(lambda[iIndex1]>0.000000000000000000000000001){
			iRealRank=iIndex1+1;
		}
	}
	if(iRealRank>iMinRowCol){
		iRealRank=iMinRowCol;
	}
	for(iIndex1=0;iIndex1<iMinRowCol;iIndex1++){
		for(iIndex2=0;iIndex2<iMinRowCol;iIndex2++){
			D[iIndex1][iIndex2]=cmplx(0.0,0.0);
		}
	}
	for(iIndex1=0;iIndex1<iRealRank;iIndex1++){
		D[iIndex1][iIndex1]=cmplx(sqrt(lambda[iIndex1]),0.0);
	}
	for(iIndex1=0;iIndex1<iMinRowCol;iIndex1++){
		for(iIndex2=0;iIndex2<iMinRowCol;iIndex2++){
			if(iIndex1==iIndex2){
				if(real(D[iIndex1][iIndex1])<0.000000000000000000000000001){
					D[iIndex1][iIndex1]=cmplx(0.000000000000000000000000001,0.0);
				}
				D_Inv[iIndex1][iIndex2]=cmplx(1.0/real(D[iIndex1][iIndex1]),0.0);
			}
			else{
				D_Inv[iIndex1][iIndex2]=cmplx(0.0,0.0);
			}
		}
	}
	if(Row>=Col){
		//---- ���V�����߂Ă���ɂ��AU�����߂�ꍇ --------

		//----- �e�񖈂�V�x�N�g���̑�P�s�ڂɑ�������v�f�������ɂȂ�悤�ɉ�]�@------
		for(iIndex2=0;iIndex2<iMinRowCol;iIndex2++){
			for(iIndex1=0;iIndex1<Col;iIndex1++){
				if(iIndex1==0){
					cKaitenBuff=conjg(Temp1[iIndex1][iIndex2]);	
					dBuff1=complexabs(Temp1[iIndex1][iIndex2]);
					cKaitenBuff2=rdiv(cKaitenBuff,dBuff1);
				}
				if(iIndex2<iRealRank){
					V[iIndex1][iIndex2]=cmul(Temp1[iIndex1][iIndex2],cKaitenBuff2);
				}
				else{
					V[iIndex1][iIndex2]=cmplx(0.0,0.0);
				}
			}
		}
		//-------- Make U U=HV_T/s---------------------------------
		matrix_mult(Row,Col,iMinRowCol,H,V,Temp2);
		matrix_mult(Row,iMinRowCol,iMinRowCol,Temp2,D_Inv,U);
	}
	else{
		//--- ���U�����߂Ă���ɂ��V�����߂�ꍇ -----

		//----- �e�񖈂�U�x�N�g���̑�P�s�ڂɑ�������v�f�������ɂȂ�悤�ɉ�]�@------
		for(iIndex2=0;iIndex2<iMinRowCol;iIndex2++){
			for(iIndex1=0;iIndex1<Row;iIndex1++){
				if(iIndex1==0){
					cKaitenBuff=conjg(Temp1[iIndex1][iIndex2]);	
					dBuff1=complexabs(Temp1[iIndex1][iIndex2]);
					cKaitenBuff2=rdiv(cKaitenBuff,dBuff1);
				}
				if(iIndex2<iRealRank){
					U[iIndex1][iIndex2]=cmul(Temp1[iIndex1][iIndex2],cKaitenBuff2);
				}
				else{
					U[iIndex1][iIndex2]=cmplx(0.0,0.0);
				}
			}
		}
		//-------- Make V V=U_H*H---------------------------------
		matrix_tenkyou(Row,iMinRowCol,U,U_T);
		matrix_mult(iMinRowCol,Row,Col,U_T,H,Temp2);
		matrix_mult(iMinRowCol,iMinRowCol,Col,D_Inv,Temp2,V_T);	
		matrix_tenkyou(iMinRowCol,Col,V_T,V);
	}

	//-------- free ------------------------
	Free2D(
		(void **)D_Inv,
		iMinRowCol	
	);
	if(Row>=Col){
		Free2D(
			(void **)Temp1,
			Col	
		);
		Free2D(
			(void **)Temp2,
			Row	
		);
	}
	else{
		Free2D(
			(void **)Temp1,
			Row	
		);
		Free2D(
			(void **)Temp2,
			iMinRowCol	
		);
	}
	Free2D( 
		(void **)H_T,
		Col	
	);
	Free2D(
		(void **)H_TH,
		Col	
	);
	Free2D(
		(void **)HH_T,
		Row	
	);
	Free1D(
		(void *)lambda
	);
	Free2D(
		(void **)U_T,
		iMinRowCol
	);
	Free2D(
		(void **)V_T,
		iMinRowCol
	);
	
}

// LU���� 
#define LOW_LIMIT 0.0000000001
void lu2_1(
int JISUU,
Complex **a,
int *pivot
)
{
	int i,j,k;
	int ii,jj;
	Complex complex_a,complex_b;
	int imax;
	int iimax;
	double max;



	for( i = 0; i < JISUU; i++ ){


/**
		printf("%d -------------------------------------------\n",i);
		printf("before pivot\n");
			for(j=0;j<JISUU;j++){
				printf("pivot[%d]=%d\n",j,pivot[j]);
			}
		for(k=0;k<JISUU;k++){
			for(j=0;j<JISUU;j++){
				printf("%lf %lf ",real(a[pivot[k]][j]),aimag(a[pivot[k]][j]));
			}
			printf("\n");
		}
**/


		ii=pivot[i];
		// �����s�{�b�g�I��
		max=complexabs(a[ii][i]);
		iimax=ii;
		imax=i;
		for(j=i+1;j<JISUU;j++){
			jj=pivot[j];
			if(complexabs(a[jj][i])>max){
				iimax=jj;
				imax=j;
				max=complexabs(a[jj][i]);
			}
		}
//		printf("iimax=%d ii=%d\n",iimax,ii);
		if(i!=imax){
			// swapping gyou  using pivot[]
			//���z�I�ɍs�ԍ�����ւ��邱�Ƃɂ��X���b�s���O
			pivot[i]=iimax;
			pivot[imax]=ii;
		}
		if(max<LOW_LIMIT){
		//	printf("a[i][i] become %lf\n",max);
		//	printf("lu2.cpp ERROR!!!\n");
		//	exit(1);
		}
	//	printf("a[i][i] become %2.30lf\n",max);
		// �����s�{�b�g�I���I��
		
		ii=pivot[i];
		complex_a=cmplx(1.0,0.0);
		complex_b=cdiv(complex_a,a[ii][i]); //����Z���ŏ��ɂ���Ă���

		for( j = i+1; j < JISUU; j++ ){

/**
int takai,takaj;
printf("A[][]--------------------------------\n");
for(takai=0;takai<JISUU;takai++){
	for(takaj=0;takaj<JISUU;takaj++){
		printf("[%.3lf %.3lf]",real(a[pivot[takai]][takaj]),aimag(a[pivot[takai]][takaj]));
	}
	printf("\n");
}
**/

			// a[j][i] /= a[i][i];
			jj=pivot[j];
			a[jj][i]=cmul(a[jj][i],complex_b);
			for( k = i+1; k < JISUU; k++ ){
				// a[j][k] -= a[i][k] * a[j][i];
				a[jj][k]=csub(a[jj][k], cmul(a[ii][k],a[jj][i]) );
			}
		}
	}
}
// �������߂�
void lu2_2(
int JISUU,
Complex **a,
Complex *b,
int *pivot
)
{
	int i,j;
	int ii,jj;
	Complex b_buff[100]; // �Ō�̉����r�{�b�g���ɓ��ւ��邽�߂̃o�b�t�@
	for( i = 0; i < JISUU; i++ ){
		ii=pivot[i];
		for( j = i+1; j < JISUU; j++ ){
			// b[j] -= b[i] * a[j][i];
			jj=pivot[j];
			b[jj]=csub(b[jj],cmul(b[ii],a[jj][i]));
		}
	}
	for( i = JISUU-1; i >= 0; i-- ){
		ii=pivot[i];
		for( j = i+1; j<JISUU; j++ ){
			jj=pivot[j];
			// b[i] -= a[i][j] * b[j];
			b[ii]=csub(b[ii],cmul(a[ii][j],b[jj]));
		}
		// b[i] /= a[i][i];
		b[ii]=cdiv(b[ii],a[ii][i]);
	}    
	// �s�b�{�b�g���ɕ��בւ�
	for(i=0;i<JISUU;i++){
		b_buff[i]=b[i];
	}
	for(i=0;i<JISUU;i++){
		b[i]=b_buff[pivot[i]];
	}
}


///================================LU Function================================//
void matrix_GeneralizedinverseUsingLU(
	int iMMSEMod, // 0:���ʂ̋t�s�� 1:�G���d�͂��g�p����BMMSE�^�̋t�s��𐶐�����B
	double dNoisePower, // �G���d��(A�����K������Ă���ꍇ�̓d��)
	// Row<=Col recommend 0: Row>Col recommend 1:
	int iMod, // 0:MinNorm(�ŏ��m�����@) 1:LeastMean(�ŏ��Q��@) 2:�����I���i���C���j
	int Row, 
	int Col,
	int iMinRowCol,  // min(Row,Col)
	Complex **A,   // A[Row][Col] input
	Complex **A_INV // A_INV[Col][Row] output
)
{
	double dNoisePowerBuff;
	if(dNoisePower<0.00001){
//		dNoisePowerBuff=0.00001;
		dNoisePowerBuff=dNoisePower;
	}
	else{
		dNoisePowerBuff=dNoisePower;
	}
	int iIndex1,iIndex2;
	int iModInternal;
	Complex TEMPVECTOR[1000];  
	Complex **AH; // AH[Col][Row]
	Complex **AH_A; // AH_A[Col][Col]
	Complex **AH_A_TEMP; // AH_A_TEMP[Col][Col]
	Complex **NOISE_MATRIX_Col; // NOISE_MATRIX_Col[Col][Col]
	Complex **A_AH; // A_AH[Row][Row]
	Complex **A_AH_TEMP; // A_AH_TEMP[Row][Row]
	Complex **NOISE_MATRIX_Row; // NOISE_MATRIX_Row[Row][Row]
	Complex **AH_A_INV; //AH_A_INV[Col][Col]
	Complex **A_AH_INV; //A_AH_INV[Row][Row]
	int *piPivotRow;
	piPivotRow=(int *)pvMalloc1D(Row,sizeof(int));
	int *piPivotCol;
	piPivotCol=(int *)pvMalloc1D(Col,sizeof(int));

	iModInternal = 0;

	//--- calloc ----------------
	AH=(Complex **)ppvMalloc2D(
		Col,
		Row,
		sizeof(Complex *),
		sizeof(Complex)
	);
	AH_A=(Complex **)ppvMalloc2D(
		Col,
		Col,
		sizeof(Complex *),
		sizeof(Complex)
	);
	AH_A_TEMP=(Complex **)ppvMalloc2D(
		Col,
		Col,
		sizeof(Complex *),
		sizeof(Complex)
	);
	NOISE_MATRIX_Col=(Complex **)ppvMalloc2D(
		Col,
		Col,
		sizeof(Complex *),
		sizeof(Complex)
	);
	A_AH=(Complex **)ppvMalloc2D(
		Row,
		Row,
		sizeof(Complex *),
		sizeof(Complex)
	);
	A_AH_TEMP=(Complex **)ppvMalloc2D(
		Row,
		Row,
		sizeof(Complex *),
		sizeof(Complex)
	);
	NOISE_MATRIX_Row=(Complex **)ppvMalloc2D(
		Row,
		Row,
		sizeof(Complex *),
		sizeof(Complex)
	);
	AH_A_INV=(Complex **)ppvMalloc2D(
		Col,
		Col,
		sizeof(Complex *),
		sizeof(Complex)
	);
	A_AH_INV=(Complex **)ppvMalloc2D(
		Row,
		Row,
		sizeof(Complex *),
		sizeof(Complex)
	);
	//---------------------------
	matrix_tenkyou(Row,Col,A,AH); // Make AH
	matrix_mult(Col,Row,Col,AH,A,AH_A); // Make AH_A
	matrix_mult(Row,Col,Row,A,AH,A_AH); // Make A_AH


	//---mod deisision ------------------
	if(iMod==0){ // �ŏ��m����
		iModInternal=0;
	}
	else if(iMod==1){ // �ŏ��Q��
		iModInternal=1;
	}
	else if(iMod==2){ // ���C�����[�h
		if(Row<Col){
			iModInternal=0;
		}
		else{
			iModInternal=1;
		}
	}
	else{
		printf("ParameterError!\n");
	}

	if(iModInternal==0){
	//�s�{�b�g�̏���
	//---- MAKE A_AH_INV; // [Row][Row]
	for(iIndex1=0;iIndex1<Row;iIndex1++){
		piPivotRow[iIndex1]=iIndex1;
	}
	//---- MAKE NOISE_MATRIX_Row -----
	for(iIndex1=0;iIndex1<Row;iIndex1++){
		for(iIndex2=0;iIndex2<Row;iIndex2++){
			if(iIndex1==iIndex2){
				if(iMMSEMod==1){
					NOISE_MATRIX_Row[iIndex1][iIndex2]=cmplx((double)iMinRowCol*dNoisePowerBuff,0.0);
				}
				else{
					NOISE_MATRIX_Row[iIndex1][iIndex2]=cmplx(0.0,0.0);
				}
			}
			else{
				NOISE_MATRIX_Row[iIndex1][iIndex2]=cmplx(0.0,0.0);	
			}
		}
	}
	matrix_add(Row,Row,A_AH,NOISE_MATRIX_Row,A_AH_TEMP);
	lu2_1(Row,A_AH_TEMP,piPivotRow);
	for(iIndex1=0;iIndex1<Row;iIndex1++){
		for(iIndex2=0;iIndex2<Row;iIndex2++){
			if(iIndex1==iIndex2){
				TEMPVECTOR[iIndex2]=cmplx(1.0,0.0);
			}
			else{
				TEMPVECTOR[iIndex2]=cmplx(0.0,0.0);
			}
		}
		lu2_2(Row,A_AH_TEMP,TEMPVECTOR,piPivotRow);
		for(iIndex2=0;iIndex2<Row;iIndex2++){
			A_AH_INV[iIndex2][iIndex1]=TEMPVECTOR[iIndex2];
		}
	}
	matrix_mult(Col,Row,Row,AH,A_AH_INV,A_INV);
	}
	else{


	//---- MAKE AH_A_INV; // [Col][Col]
	for(iIndex1=0;iIndex1<Col;iIndex1++){
		piPivotCol[iIndex1]=iIndex1;
	}
	//----- MAKE NOISE_MATRIX_Col ------
	for(iIndex1=0;iIndex1<Col;iIndex1++){
		for(iIndex2=0;iIndex2<Col;iIndex2++){
			if(iIndex1==iIndex2){
				if(iMMSEMod==1){
					NOISE_MATRIX_Col[iIndex1][iIndex2]=cmplx((double)iMinRowCol*dNoisePowerBuff,0.0);  // Ver0.6 �C��2005.10.06
				}
				else{
					NOISE_MATRIX_Col[iIndex1][iIndex2]=cmplx(0.0,0.0);	
				}
			}
			else{
				NOISE_MATRIX_Col[iIndex1][iIndex2]=cmplx(0.0,0.0);
			}
		}
	}
	matrix_add(Col,Col,AH_A,NOISE_MATRIX_Col,AH_A_TEMP);
	//---- MAKE Noise Matrix ---------
	//--------------------------------
	lu2_1(Col,AH_A_TEMP,piPivotCol);
	for(iIndex1=0;iIndex1<Col;iIndex1++){
		for(iIndex2=0;iIndex2<Col;iIndex2++){
			if(iIndex1==iIndex2){
				TEMPVECTOR[iIndex2]=cmplx(1.0,0.0);
			}
			else{
				TEMPVECTOR[iIndex2]=cmplx(0.0,0.0);
			}
		}
		lu2_2(Col,AH_A_TEMP,TEMPVECTOR,piPivotCol);
		for(iIndex2=0;iIndex2<Col;iIndex2++){
			AH_A_INV[iIndex2][iIndex1]=TEMPVECTOR[iIndex2];
		}
	}
	matrix_mult(Col,Col,Row,AH_A_INV,AH,A_INV);
	}
	//---- Free ---------------------
	free(piPivotRow);
	free(piPivotCol);
	Free2D((void **)AH,Col);
	Free2D((void **)AH_A,Col);
	Free2D((void **)A_AH,Row);
	Free2D((void **)AH_A_TEMP,Col);
	Free2D((void **)NOISE_MATRIX_Col,Col);
	Free2D((void **)A_AH_TEMP,Row);
	Free2D((void **)NOISE_MATRIX_Row,Row);
	Free2D((void **)AH_A_INV,Col);
	Free2D((void **)A_AH_INV,Row);
}

///================================Inverse Function================================//
void matrix_generalizedinverse(
	int Row, 
	int Col,
	int iMinRowCol,  // min(Row,Col)
	Complex **A,   // A[Row][Col] input
	Complex **AINV // AINV[Col][Row] output
)
{
	int iIndex1;
	Complex **U; // U[Row][iMinRowCol)]
	Complex **D; // D[iMinRowCol][iMinRowCol]
	Complex **V;  // V[Col][iMinRowCol]
	Complex **UH;  // UH[iMinRowCol][Row]
	Complex **VD; // VD[Col][iMinRowCol]
	//--- calloc ----------------
	U=(Complex **)ppvMalloc2D(
		Row,
		iMinRowCol,
		sizeof(Complex *),
		sizeof(Complex)
	);
	D=(Complex **)ppvMalloc2D(
		iMinRowCol,
		iMinRowCol,
		sizeof(Complex *),
		sizeof(Complex)
	);
	V=(Complex **)ppvMalloc2D(
		Col,
		iMinRowCol,
		sizeof(Complex *),
		sizeof(Complex)
	);
	UH=(Complex **)ppvMalloc2D(
		iMinRowCol,
		Row,
		sizeof(Complex *),
		sizeof(Complex)
	);
	VD=(Complex **)ppvMalloc2D(
		Col,
		iMinRowCol,
		sizeof(Complex *),
		sizeof(Complex)
	);
	//---------------------------
	svd(Row,Col,A,U,D,V);
	for(iIndex1=0;iIndex1<iMinRowCol;iIndex1++){
		D[iIndex1][iIndex1]=cmplx(1.0/real(D[iIndex1][iIndex1]),0.0);
	}
	matrix_tenkyou(Row,iMinRowCol,U,UH);
	matrix_mult(Col,iMinRowCol,iMinRowCol,V,D,VD);
	matrix_mult(Col,iMinRowCol,Row,VD,UH,AINV);
	

	//---- Free ---------------------
	Free2D((void **)U,Row);
	Free2D((void **)D,iMinRowCol);
	Free2D((void **)V,Col);
	Free2D((void **)UH,iMinRowCol);
	Free2D((void **)VD,Col);
}

void LMS_1Output( // 
	// e=r-y=r-WH*X
	// W=W+dStep*X*e�̋���
	int iJisuu,
	double dStep, // 
	Complex **r, // r[1][1]
	Complex **W, // W[iJisuu][1]	
	Complex **X // X[iJisuu][1]
)
{
	Complex **e; // e[1][1]
	e=(Complex **)ppvMalloc2D(1,1,sizeof(Complex *),sizeof(Complex));
	Complex **WH; // WH[1][iJisuu]
	WH=(Complex **)ppvMalloc2D(1,iJisuu,sizeof(Complex *),sizeof(Complex));
	// e=r-WH*X
	Complex **WHX; // WHX[1][1]=WH*X
	WHX=(Complex **)ppvMalloc2D(1,1,sizeof(Complex *),sizeof(Complex));	
	Complex **eH; // eH[1][1]
	eH=(Complex **)ppvMalloc2D(1,1,sizeof(Complex *),sizeof(Complex));
	Complex **XeH; // XeH[iJisuu][1]
	XeH=(Complex **)ppvMalloc2D(iJisuu,1,sizeof(Complex *),sizeof(Complex));
	Complex **dStepXeH; // dStepXeH[iJisuu][1]
	dStepXeH=(Complex **)ppvMalloc2D(iJisuu,1,sizeof(Complex *),sizeof(Complex));
	Complex **WBuff1; // WBuff1[iJisuu][1]
	WBuff1=(Complex **)ppvMalloc2D(iJisuu,1,sizeof(Complex *),sizeof(Complex));


	//---- MAKE WH ------
	matrix_tenkyou(iJisuu,1,W,WH);
	//---- WHX ----------						
	matrix_mult(1,iJisuu,1,WH,X,WHX);
	//-----MAKE e -------
	matrix_sub(1,1,r,WHX,e);
	//----- MAKE eH --------
	matrix_tenkyou(1,1,e,eH);
	//----- XeH ----------
	matrix_mult(iJisuu,1,1,X,eH,XeH);
	//-----dStepXeH -----------
	matrix_rmul(iJisuu,1,XeH,dStep,dStepXeH);
	//-- Copy -----------
	matrix_copy(iJisuu,1,W,WBuff1);
	//-----MAKE New W -------------
	matrix_add(iJisuu,1,WBuff1,dStepXeH,W);
}
void LMS_MultiOutput( // 
	// e=r-y=r-WH*X
	// W=W+dStep*X*e�̋���
	// WH[iRow][iCol] // �������ɍl����
	// y[iRow][1]
	int iRow,
	int iCol,
	double dStep, // 0.02���x
	Complex **r, // r[iRow][1]
	Complex **W, // W[iCol][iRow]
	Complex **X // X[iCol][1]
)
{
	Complex **e; // [iRow][1]
	e=(Complex **)ppvMalloc2D(iRow,1,sizeof(Complex *),sizeof(Complex));
	Complex **WH; // WH[iRow][iCol]
	WH=(Complex **)ppvMalloc2D(iRow,iCol,sizeof(Complex *),sizeof(Complex));
	// e=r-WH*X
	Complex **WHX; // WHX[iRow][1]=WH*X
	WHX=(Complex **)ppvMalloc2D(iRow,1,sizeof(Complex *),sizeof(Complex));	
	Complex **eH; // eH[1][iRow]
	eH=(Complex **)ppvMalloc2D(1,iRow,sizeof(Complex *),sizeof(Complex));
	Complex **XeH; // XeH[iCol][iRow]
	XeH=(Complex **)ppvMalloc2D(iCol,iRow,sizeof(Complex *),sizeof(Complex));
	Complex **dStepXeH; // dStepXeH[iCol][iRow]
	dStepXeH=(Complex **)ppvMalloc2D(iCol,iRow,sizeof(Complex *),sizeof(Complex));
	Complex **WBuff1; // WBuff1[iCol][iRow]
	WBuff1=(Complex **)ppvMalloc2D(iCol,iRow,sizeof(Complex *),sizeof(Complex));


	//---- MAKE WH ------
	matrix_tenkyou(iCol,iRow,W,WH);
	//---- WHX ----------						
	matrix_mult(iRow,iCol,1,WH,X,WHX);
	//-----MAKE e -------
	matrix_sub(iRow,1,r,WHX,e);
	//----- MAKE eH --------
	matrix_tenkyou(iRow,1,e,eH);
	//----- XeH ----------
	matrix_mult(iCol,1,iRow,X,eH,XeH);
	//-----dStepXeH -----------
	matrix_rmul(iCol,iRow,XeH,dStep,dStepXeH);
	//-- Copy -----------
	matrix_copy(iCol,iRow,W,WBuff1);
	//-----MAKE New W -------------
	matrix_add(iCol,iRow,WBuff1,dStepXeH,W);
	//---- free ------
	Free2D((void **)e,iRow);	
	Free2D((void **)WH,iRow);
	Free2D((void **)WHX,iRow);	
	Free2D((void **)eH,1);
	Free2D((void **)XeH,iCol);
	Free2D((void **)dStepXeH,iCol);
	Free2D((void **)WBuff1,iCol);
}


#define PINIT 1000000.0 // P no shoki chi 
void RLS(
	int iReset, // 1:reset 0:nomal 2:not update W 
	int iJisuu, // ����
	double dBoukyakuKeisuu, // 0.95�Ƃ�1.0�Ƃ�
	Complex **W, // W[iJisuu][1], // suitei suru omomi iJisuu*1 
	Complex **X, // X[iJisuu][1], // nyuuryoku iJisuu*1
	Complex **R, // R[1][1], // kyoushi shinngou 1*1 
	Complex **GOSA, // GOSA[1][1], // gosa shinngou 
	Complex **I, // I[iJisuu][iJisuu] 
	Complex **P // P[iJisuu][iJisuu]
)
{
	Complex **K; // K[iJisuu][1]
	K=(Complex **)ppvMalloc2D(iJisuu,1,sizeof(Complex *),sizeof(Complex));
	Complex **TEMP1; // TEMP1[iJisuu][1]
	TEMP1=(Complex **)ppvMalloc2D(iJisuu,1,sizeof(Complex *),sizeof(Complex));
	Complex **TEMP2; // TEMP2[1][iJisuu]
	TEMP2=(Complex **)ppvMalloc2D(1,iJisuu,sizeof(Complex *),sizeof(Complex));
	Complex **TEMP3; // TEMP3[1][iJisuu]
	TEMP3=(Complex **)ppvMalloc2D(1,iJisuu,sizeof(Complex *),sizeof(Complex));	
	Complex **TEMP4; // TEMP4[1][1]
	TEMP4=(Complex **)ppvMalloc2D(1,1,sizeof(Complex *),sizeof(Complex));
	double TEMP5;
	Complex **TEMP6; // TEMP6[iJisuu][iJisuu];
	TEMP6=(Complex **)ppvMalloc2D(iJisuu,iJisuu,sizeof(Complex *),sizeof(Complex));
	Complex **TEMP7; // TEMP7[iJisuu][iJisuu]; 
	TEMP7=(Complex **)ppvMalloc2D(iJisuu,iJisuu,sizeof(Complex *),sizeof(Complex));
	Complex **TEMP8; // TEMP8[iJisuu][iJisuu]; 
	TEMP8=(Complex **)ppvMalloc2D(iJisuu,iJisuu,sizeof(Complex *),sizeof(Complex));	
	Complex **TEMP9; // [1][1]
	TEMP9=(Complex **)ppvMalloc2D(1,1,sizeof(Complex *),sizeof(Complex));
	Complex **TEMP10; // [1][1]
	TEMP10=(Complex **)ppvMalloc2D(1,1,sizeof(Complex *),sizeof(Complex));
	Complex **TEMP11; // [1][1]
	TEMP11=(Complex **)ppvMalloc2D(1,1,sizeof(Complex *),sizeof(Complex));
	Complex **TEMP12; // [iJisuu][1]
	TEMP12=(Complex **)ppvMalloc2D(iJisuu,1,sizeof(Complex *),sizeof(Complex));
	Complex **TEMP13; // [iJisuu][1]
	TEMP13=(Complex **)ppvMalloc2D(iJisuu,1,sizeof(Complex *),sizeof(Complex));


	int i,j;
	if(iReset==1){
		// SHOKIKA 
		GOSA[0][0]=cmplx(0.0,0.0);
		for(i=0;i<iJisuu;i++){
			// HONTOUHA CHIKAI ATAI Ni SHOKIKA SURU NOGA YOI
			if(i==0){
				W[i][0]=cmplx(1.0,0.0);
			}
			else{
				W[i][0]=cmplx(0.0,0.0);
			}
		}
		for(i=0;i<iJisuu;i++){
			for(j=0;j<iJisuu;j++){
				if(i==j){
					P[i][j]=cmplx(PINIT,0.0);
					I[i][j]=cmplx(1.0,0.0);
				}
				else{
					P[i][j]=cmplx(0.0,0.0);
					I[i][j]=cmplx(0.0,0.0);
				}
			}
		}
	}
	else{
		//1:K(m+1)=P(m)X(m+1)/{dBoukyakuKeisuu+XH(m+1)P(m)X(m+1)}
		matrix_mult(iJisuu,iJisuu,1,P,X,TEMP1); // PX
		matrix_tenkyou(iJisuu,1,X,TEMP2);	// XH
		matrix_mult(1,iJisuu,iJisuu,TEMP2,P,TEMP3); // XH*P
		matrix_mult(1,iJisuu,1,TEMP3,X,TEMP4); // XH*P*X 
		TEMP5=dBoukyakuKeisuu+real(TEMP4[0][0]);
		matrix_rdiv(iJisuu,1,TEMP1,TEMP5,K);

		// 2:P(m+1)={I-K(m+1)XH(m+1)}P(m)
		matrix_mult(iJisuu,1,iJisuu,K,TEMP2,TEMP6);
		matrix_sub(iJisuu,iJisuu,I,TEMP6,TEMP7);
		matrix_mult(iJisuu,iJisuu,iJisuu,TEMP7,P,TEMP8);
		matrix_copy(iJisuu,iJisuu,TEMP8,P);

		// 3:W(m+1)=W(m)+K(m+1){R*(m+1)-XH(m+1)W(m)}
		matrix_mult(1,iJisuu,1,TEMP2,W,TEMP9); // XH*W 
		matrix_kyouyaku(1,1,R,TEMP10); // R* 
		matrix_sub(1,1,TEMP10,TEMP9,TEMP11); // R*-XH*W 
		matrix_copy(1,1,TEMP11,GOSA); 
		matrix_mult(iJisuu,1,1,K,TEMP11,TEMP12); //K(R* -XH*W) 
		matrix_add(iJisuu,1,W,TEMP12,TEMP13); 
		if(iReset!=2){
		matrix_copy(iJisuu,1,TEMP13,W);
		}
	}
	//--- free ---------------
	Free2D((void **)K,iJisuu);
	Free2D((void **)TEMP1,iJisuu);
	Free2D((void **)TEMP2,1);
	Free2D((void **)TEMP3,1);
	Free2D((void **)TEMP4,1);
	Free2D((void **)TEMP6,iJisuu);
	Free2D((void **)TEMP7,iJisuu);
	Free2D((void **)TEMP8,iJisuu);
	Free2D((void **)TEMP9,1);
	Free2D((void **)TEMP10,1);
	Free2D((void **)TEMP11,1);
	Free2D((void **)TEMP12,iJisuu);
	Free2D((void **)TEMP13,iJisuu);
}


///================================QR Function================================//
/**
 *
 * QR����(���f����)
 * 
 *@param Q �v�f�����ׂ�0�̍s��
 *@param R ���f�[�^�s��
 *@param cls ��͍s��̍s��
 *@param row ��͍s��̗�
 *@return Q�F���j�^���s��AR�F��O�p�s��
 **/

void qr( Complex **Q, Complex **R, int cls, int row ) {
	int k,i,j,n,n1;
	double		D;
	double		res;
	double		res1;
	Complex		p;
	Complex		result;
	Complex		q;
	Complex		temp;
	Complex		*w		= NULL;
	Complex		*ver	= NULL;
	Complex		*tw		= NULL;

	///Input Check
	/*
	for(i=0; i<2; i++){
		for(j=0; j<2; j++){
			//printf("Q %f %f", Q[i][j].i, Q[i][j].q);
			printf("R %f %f", R[i][j].i, R[i][j].q);
		}
		printf("\n");
	}
	*/
	//printf("cls %d row %d\n", cls, row);


	//����
	p.i			= 0.0;
	p.q			= 0.0;
	result.i	= 0.0;
	result.q	= 0.0;
	q.i			= 0.0;
	q.q			= 0.0;

	w	= (Complex *)malloc(cls * sizeof(Complex));
	ver	= (Complex *)malloc(cls * sizeof(Complex));
	tw	= (Complex *)malloc(row * sizeof(Complex));

	//����s��łȂ����̂ɑΉ�������.
	if(cls > row) n = row+1;
	else          n = cls;
	n1 = n-1;
	
	for( i = 0 ; i < n ; i++ )
	{
		Q[i][i].i  = 1.0;
		Q[i][i].q = 0.0;
	}

	//QR�����������Ȃ�.
	for(k = 0;k < n1;k++)
	{
		//HouseHolder�ϊ��������Ȃ�.
		//�x�N�g��a���쐬
		res = 0.0;
		for( i = k ; i < cls ; i++ )
		{
			res += ( R[i][k].i*R[i][k].i+R[i][k].q*R[i][k].q );
		}
		q.i = 0.0; q.q = 0.0;

		//�m�����̌v�Z
		D = sqrt(res);
		// �n�E�X�z���_�[�ϊ������邽�߂̃x�N�g���̌v�Z
		for(i = 0;i < k;i++) 
		{
			w[i].i = 0.0;
			w[i].q = 0.0;
		}
		w[k].i = R[k][k].i-D;
		w[k].q = R[k][k].q;

		for(i = k+1;i < cls;i++) 
		{
			w[i].i = R[i][k].i;
			w[i].q = R[i][k].q;
		}
		res1 = 0.0;
		for( i = k ; i < cls ; i++ ) 
			res1 += ( w[i].i*w[i].i+w[i].q*w[i].q );
		res1 = sqrt(res1);
		for(i = 0;i < cls;i++) 
		{
			w[i].i = w[i].i / res1; 
			w[i].q = w[i].q / res1;
		}
		// P = I - (1+w)*v*v^{H}�̌v�Z
		// w = (||r_{k}||-r^{*}_{kk})/(||r_{k}||-r_{kk})
		p.i = ((D-R[k][k].i)*(D-R[k][k].i)-R[k][k].q*R[k][k].q)/
			((D-R[k][k].i)*(D-R[k][k].i)+R[k][k].q*R[k][k].q);
		p.q= (2*(D-R[k][k].i)*R[k][k].q)/
			((D-R[k][k].i)*(D-R[k][k].i)+R[k][k].q*R[k][k].q);

		//�O�p�s��𐶐�����.(�v�Z�����ώG�ɂȂ邽��2�i�K�ɕ����Čv�Z)
		//P^{H}_{n}*R_{n-1} = R_{n}
		//1�i�K��
		result.i = result.q = 0.0;
		for(i = k;i < row;i++)
		{
			for(j = k;j < cls;j++)
			{
				result.i = result.i + w[j].i * R[j][i].i + 
					w[j].q * R[j][i].q;
				result.q = result.q + w[j].i * R[j][i].q -
					w[j].q * R[j][i].i;
			}
			tw[i].i = result.i;
			tw[i].q = result.q;
			result.i = 0.0;
			result.q = 0.0;
		}
		//2�i�K��
		for(j = k;j < cls;j++)
		{
			for(i = k;i < row;i++)
			{
				temp.i = w[j].i*tw[i].i-w[j].q*tw[i].q;
				temp.q = w[j].i*tw[i].q + w[j].q*tw[i].i;
				R[j][i].i = R[j][i].i - 
					((1.0+p.i)*temp.i-p.q*temp.q);
				R[j][i].q = R[j][i].q -
					((1.0+p.i)*temp.q+p.q*temp.i);
			}
		}
		//�G���~�[�g�s��𐶐�����.�v�Z�����ώG�ɂȂ邽��2�i�K�ɕ����Čv�Z
		//Q_{n-1}*P_{n} = Q_{n}
		//1�i�K��
		result.i = result.q = 0.0;
		for(j = 0;j < cls;j++)
		{
			for(i = k;i < cls;i++)
			{
				result.i = result.i+Q[j][i].i*w[i].i -
					Q[j][i].q*w[i].q;
				result.q = result.q+Q[j][i].i*w[i].q +
					Q[j][i].q*w[i].i;
			}
			ver[j].i = result.i;
			ver[j].q = result.q;
			result.i = 0.0;
			result.q = 0.0;
		}
		//2�i�K��
		for(j = 0;j < cls;j++)
		{
			for(i = k;i < cls;i++)
			{
				temp.i = ver[j].i*w[i].i + ver[j].q*w[i].q;
				temp.q = -ver[j].i*w[i].q + ver[j].q*w[i].i;
				Q[j][i].i = Q[j][i].i - 
					((1.0+p.i)*temp.i+p.q*temp.q);
				Q[j][i].q = Q[j][i].q - 
					((1.0+p.i)*temp.q-p.q*temp.i);
			}
		}
	}


	///Output Check
	/*
	for(i=0; i<2; i++){
		for(j=0; j<2; j++){
			//printf("%f,%f,", Q[i][j].i, Q[i][j].q);
			printf("%f,%f,", R[i][j].i, R[i][j].q);
		}
		printf("\n");
	}
	//*/

	free(tw);
	free(w);
	free(ver);
}

///================================QR1 Function================================//
/**
 *
 * QR����(���f����)
 * 
 *@param Q �v�f�����ׂ�0�̍s��
 *@param R ���f�[�^�s��
 *@param cls ��͍s��̍s��
 *@param row ��͍s��̗�
 *@return Q�F���j�^���s��AR�F��O�p�s��
 **/

void qr1( Complex **Q, Complex **R, int cls, int row ) {
	int n1;
	double		D;
	double		res;
	double		res1;
	int			icase;
	Complex		p;
	Complex		result;
	Complex		q;
	Complex		temp;
	Complex		*w		= NULL;
	Complex		*ver	= NULL;
	Complex		*tw		= NULL;

	///Input Check
	//printf("Q %f %f R %f %f\n", Q[0][0].i, Q[0][0].q, R[0][0].i, R[0][0].q);
	//printf("cls %d row %d\n", cls, row);

	/// �}�C�i�X�����t������(Matlab��qr()�Ɛ����������킹�邽��)
	if(R[0][0].i >= 0 && R[0][0].q >= 0){ // + +
		icase = 1;
	}else if(R[0][0].i >= 0 && R[0][0].q < 0){ // + -
		icase = 2;
	}else if(R[0][0].i < 0 && R[0][0].q >= 0){ // - +
		icase = 3;
	}else if(R[0][0].i < 0 && R[0][0].q < 0){	// - -
		icase = 4;
	}else{
		icase = 5;
	}


	//����
	p.i			= 0.0;
	p.q			= 0.0;
	result.i	= 0.0;
	result.q	= 0.0;
	q.i			= 0.0;
	q.q			= 0.0;

	w	= (Complex *)malloc(cls * sizeof(Complex));
	ver	= (Complex *)malloc(cls * sizeof(Complex));
	tw	= (Complex *)malloc(row * sizeof(Complex));

	n1 = 1;

	Q[0][0].i  = 1.0;
	Q[0][0].q = 0.0;

	//QR�����������Ȃ�.

	//HouseHolder�ϊ��������Ȃ�.
	//�x�N�g��a���쐬
	res = 0.0;
	res += ( R[0][0].i*R[0][0].i+R[0][0].q*R[0][0].q );
	q.i = 0.0; q.q = 0.0;

	//�m�����̌v�Z
	D = sqrt(res);
	// �n�E�X�z���_�[�ϊ������邽�߂̃x�N�g���̌v�Z
	w[0].i = 0.0;
	w[0].q = 0.0;
	w[0].i = R[0][0].i-D;
	w[0].q = R[0][0].q;

	w[0].i = R[0][0].i;
	w[0].q = R[0][0].q;
	res1 = 0.0;
	res1 += ( w[0].i*w[0].i+w[0].q*w[0].q );
	res1 = sqrt(res1);
	w[0].i = w[0].i / res1; 
	w[0].q = w[0].q / res1;

	// P = I - (1+w)*v*v^{H}�̌v�Z
	// w = (||r_{k}||-r^{*}_{kk})/(||r_{k}||-r_{kk})
	p.i = ((D-R[0][0].i)*(D-R[0][0].i)-R[0][0].q*R[0][0].q)/
		((D-R[0][0].i)*(D-R[0][0].i)+R[0][0].q*R[0][0].q);
	p.q= (2*(D-R[0][0].i)*R[0][0].q)/
		((D-R[0][0].i)*(D-R[0][0].i)+R[0][0].q*R[0][0].q);

	//�O�p�s��𐶐�����.(�v�Z�����ώG�ɂȂ邽��2�i�K�ɕ����Čv�Z)
	//P^{H}_{n}*R_{n-1} = R_{n}
	//1�i�K��
	result.i = result.q = 0.0;

	result.i = result.i + w[0].i * R[0][0].i + 
		w[0].q * R[0][0].q;
	result.q = result.q + w[0].i * R[0][0].q -
		w[0].q * R[0][0].i;
	tw[0].i = result.i;
	tw[0].q = result.q;
	result.i = 0.0;
	result.q = 0.0;
	//2�i�K��
	temp.i = w[0].i*tw[0].i-w[0].q*tw[0].q;
	temp.q = w[0].i*tw[0].q + w[0].q*tw[0].i;
	R[0][0].i = R[0][0].i - 
		((1.0+p.i)*temp.i-p.q*temp.q);
	R[0][0].q = R[0][0].q -
		((1.0+p.i)*temp.q+p.q*temp.i);
	//�G���~�[�g�s��𐶐�����.�v�Z�����ώG�ɂȂ邽��2�i�K�ɕ����Čv�Z
	//Q_{n-1}*P_{n} = Q_{n}
	//1�i�K��
	result.i = result.q = 0.0;
	result.i = result.i+Q[0][0].i*w[0].i -
		Q[0][0].q*w[0].q;
	result.q = result.q+Q[0][0].i*w[0].q +
		Q[0][0].q*w[0].i;
	ver[0].i = result.i;
	ver[0].q = result.q;
	result.i = 0.0;
	result.q = 0.0;
	//2�i�K��
	temp.i = ver[0].i*w[0].i + ver[0].q*w[0].q;
	temp.q = -ver[0].i*w[0].q + ver[0].q*w[0].i;
	Q[0][0].i = Q[0][0].i - 
		((1.0+p.i)*temp.i+p.q*temp.q);
	Q[0][0].q = Q[0][0].q - 
		((1.0+p.i)*temp.q-p.q*temp.i);

	///1�~1�p�ɏC��
	/*
	Q[0][0].i = -Q[0][0].i;
	Q[0][0].q = -Q[0][0].q;
	R[0][0].i = -R[0][0].i;
	R[0][0].q = -R[0][0].q;
	*/

	if(icase == 1){
		Q[0][0].i = -1 * Q[0][0].i;
		Q[0][0].q = -1 * Q[0][0].q;
		R[0][0].i = -1 * R[0][0].i;
		R[0][0].q = -1 * R[0][0].q;
	}else if(icase == 2){
		Q[0][0].i = -1 * Q[0][0].i;
		Q[0][0].q = -1 * Q[0][0].q;
		R[0][0].i = -1 * R[0][0].i;
		R[0][0].q = -1 * R[0][0].q;	
	}else if(icase == 3){
	
	}else if(icase == 4){

	}else if(icase == 5){

	}else{
		printf("Error at qr1 function at Matrix.cpp\n");
		exit(1);
	}

	///Output Check
	//printf("Q %f %f R %f %f\n", Q[0][0].i, Q[0][0].q, R[0][0].i, R[0][0].q);

	free(tw);
	free(w);
	free(ver);
}


/* 
 * svdcomp - SVD decomposition routine. 
 * Takes an mxn matrix a and decomposes it into udv, where u,v are
 * left and right orthogonal transformation matrices, and d is a 
 * diagonal matrix of singular values.
 *
 * This routine is adapted from svdecomp.c in XLISP-STAT 2.1 which is 
 * code from Numerical Recipes adapted by Luke Tierney and David Betz.
 *
 * Input to dsvd is as follows:
 *   a = mxn matrix to be decomposed, gets overwritten with u
 *   m = row dimension of a
 *   n = column dimension of a
 *   w = returns the vector of singular values of a
 *   v = returns the right orthogonal transformation matrix
*/

//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>
//#include "defs_and_types.h"
// 
//static double PYTHAG(double a, double b)
//{
//    double at = fabs(a), bt = fabs(b), ct, result;
//
//    if (at > bt)       { ct = bt / at; result = at * sqrt(1.0 + ct * ct); }
//    else if (bt > 0.0) { ct = at / bt; result = bt * sqrt(1.0 + ct * ct); }
//    else result = 0.0;
//    return(result);
//}
//
//
//int dsvd(double **a, int m, int n, double *w, double **v)
//{
//    int flag, i, its, j, jj, k, l, nm;
//    double c, f, h, s, x, y, z;
//    double anorm = 0.0, g = 0.0, scale = 0.0;
//    double *rv1;
//  
//	flag = 0;
//	i = 0;
//	its = 0;
//	j = 0;
//	jj = 0;
//	k = 0;
//	l = 0;
//	nm = 0;
//
//    if (m < n) 
//    {
//        fprintf(stderr, "#rows must be > #cols \n");
//        return(0);
//    }
//  
//    rv1 = (double *)malloc((unsigned int) n*sizeof(double));
//
///* Householder reduction to bidiagonal form */
//    for (i = 0; i < n; i++) 
//    {
//        /* left-hand reduction */
//        l = i + 1;
//        rv1[i] = scale * g;
//        g = s = scale = 0.0;
//        if (i < m) 
//        {
//            for (k = i; k < m; k++) 
//                scale += fabs((double)a[k][i]);
//            if (scale) 
//            {
//                for (k = i; k < m; k++) 
//                {
//                    a[k][i] = (float)((double)a[k][i]/scale);
//                    s += ((double)a[k][i] * (double)a[k][i]);
//                }
//                f = (double)a[i][i];
//                g = -SIGN(sqrt(s), f);
//                h = f * g - s;
//                a[i][i] = (float)(f - g);
//                if (i != n - 1) 
//                {
//                    for (j = l; j < n; j++) 
//                    {
//                        for (s = 0.0, k = i; k < m; k++) 
//                            s += ((double)a[k][i] * (double)a[k][j]);
//                        f = s / h;
//                        for (k = i; k < m; k++) 
//                            a[k][j] += (float)(f * (double)a[k][i]);
//                    }
//                }
//                for (k = i; k < m; k++) 
//                    a[k][i] = (float)((double)a[k][i]*scale);
//            }
//        }
//        w[i] = (float)(scale * g);
//    
//        /* right-hand reduction */
//        g = s = scale = 0.0;
//        if (i < m && i != n - 1) 
//        {
//            for (k = l; k < n; k++) 
//                scale += fabs((double)a[i][k]);
//            if (scale) 
//            {
//                for (k = l; k < n; k++) 
//                {
//                    a[i][k] = (float)((double)a[i][k]/scale);
//                    s += ((double)a[i][k] * (double)a[i][k]);
//                }
//                f = (double)a[i][l];
//                g = -SIGN(sqrt(s), f);
//                h = f * g - s;
//                a[i][l] = (float)(f - g);
//                for (k = l; k < n; k++) 
//                    rv1[k] = (double)a[i][k] / h;
//                if (i != m - 1) 
//                {
//                    for (j = l; j < m; j++) 
//                    {
//                        for (s = 0.0, k = l; k < n; k++) 
//                            s += ((double)a[j][k] * (double)a[i][k]);
//                        for (k = l; k < n; k++) 
//                            a[j][k] += (float)(s * rv1[k]);
//                    }
//                }
//                for (k = l; k < n; k++) 
//                    a[i][k] = (float)((double)a[i][k]*scale);
//            }
//        }
//        anorm = MAX(anorm, (fabs((double)w[i]) + fabs(rv1[i])));
//    }
//  
//    /* accumulate the right-hand transformation */
//    for (i = n - 1; i >= 0; i--) 
//    {
//        if (i < n - 1) 
//        {
//            if (g) 
//            {
//                for (j = l; j < n; j++)
//                    v[j][i] = (float)(((double)a[i][j] / (double)a[i][l]) / g);
//                    /* double division to avoid underflow */
//                for (j = l; j < n; j++) 
//                {
//                    for (s = 0.0, k = l; k < n; k++) 
//                        s += ((double)a[i][k] * (double)v[k][j]);
//                    for (k = l; k < n; k++) 
//                        v[k][j] += (float)(s * (double)v[k][i]);
//                }
//            }
//            for (j = l; j < n; j++) 
//                v[i][j] = v[j][i] = 0.0;
//        }
//        v[i][i] = 1.0;
//        g = rv1[i];
//        l = i;
//    }
//  
//    /* accumulate the left-hand transformation */
//    for (i = n - 1; i >= 0; i--) 
//    {
//        l = i + 1;
//        g = (double)w[i];
//        if (i < n - 1) 
//            for (j = l; j < n; j++) 
//                a[i][j] = 0.0;
//        if (g) 
//        {
//            g = 1.0 / g;
//            if (i != n - 1) 
//            {
//                for (j = l; j < n; j++) 
//                {
//                    for (s = 0.0, k = l; k < m; k++) 
//                        s += ((double)a[k][i] * (double)a[k][j]);
//                    f = (s / (double)a[i][i]) * g;
//                    for (k = i; k < m; k++) 
//                        a[k][j] += (float)(f * (double)a[k][i]);
//                }
//            }
//            for (j = i; j < m; j++) 
//                a[j][i] = (float)((double)a[j][i]*g);
//        }
//        else 
//        {
//            for (j = i; j < m; j++) 
//                a[j][i] = 0.0;
//        }
//        ++a[i][i];
//    }
//
//    /* diagonalize the bidiagonal form */
//    for (k = n - 1; k >= 0; k--) 
//    {                             /* loop over singular values */
//        for (its = 0; its < 30; its++) 
//        {                         /* loop over allowed iterations */
//            flag = 1;
//            for (l = k; l >= 0; l--) 
//            {                     /* test for splitting */
//                nm = l - 1;
//                if (fabs(rv1[l]) + anorm == anorm) 
//                {
//                    flag = 0;
//                    break;
//                }
//                if (fabs((double)w[nm]) + anorm == anorm) 
//                    break;
//            }
//            if (flag) 
//            {
//                c = 0.0;
//                s = 1.0;
//                for (i = l; i <= k; i++) 
//                {
//                    f = s * rv1[i];
//                    if (fabs(f) + anorm != anorm) 
//                    {
//                        g = (double)w[i];
//                        h = PYTHAG(f, g);
//                        w[i] = (float)h; 
//                        h = 1.0 / h;
//                        c = g * h;
//                        s = (- f * h);
//                        for (j = 0; j < m; j++) 
//                        {
//                            y = (double)a[j][nm];
//                            z = (double)a[j][i];
//                            a[j][nm] = (float)(y * c + z * s);
//                            a[j][i] = (float)(z * c - y * s);
//                        }
//                    }
//                }
//            }
//            z = (double)w[k];
//            if (l == k) 
//            {                  /* convergence */
//                if (z < 0.0) 
//                {              /* make singular value nonnegative */
//                    w[k] = (float)(-z);
//                    for (j = 0; j < n; j++) 
//                        v[j][k] = (-v[j][k]);
//                }
//                break;
//            }
//            if (its >= 30) {
//                free((void*) rv1);
//                fprintf(stderr, "No convergence after 30,000! iterations \n");
//                return(0);
//            }
//    
//            /* shift from bottom 2 x 2 minor */
//            x = (double)w[l];
//            nm = k - 1;
//            y = (double)w[nm];
//            g = rv1[nm];
//            h = rv1[k];
//            f = ((y - z) * (y + z) + (g - h) * (g + h)) / (2.0 * h * y);
//            g = PYTHAG(f, 1.0);
//            f = ((x - z) * (x + z) + h * ((y / (f + SIGN(g, f))) - h)) / x;
//          
//            /* next QR transformation */
//            c = s = 1.0;
//            for (j = l; j <= nm; j++) 
//            {
//                i = j + 1;
//                g = rv1[i];
//                y = (double)w[i];
//                h = s * g;
//                g = c * g;
//                z = PYTHAG(f, h);
//                rv1[j] = z;
//                c = f / z;
//                s = h / z;
//                f = x * c + g * s;
//                g = g * c - x * s;
//                h = y * s;
//                y = y * c;
//                for (jj = 0; jj < n; jj++) 
//                {
//                    x = (double)v[jj][j];
//                    z = (double)v[jj][i];
//                    v[jj][j] = (float)(x * c + z * s);
//                    v[jj][i] = (float)(z * c - x * s);
//                }
//                z = PYTHAG(f, h);
//                w[j] = (float)z;
//                if (z) 
//                {
//                    z = 1.0 / z;
//                    c = f * z;
//                    s = h * z;
//                }
//                f = (c * g) + (s * y);
//                x = (c * y) - (s * g);
//                for (jj = 0; jj < m; jj++) 
//                {
//                    y = (double)a[jj][j];
//                    z = (double)a[jj][i];
//                    a[jj][j] = (float)(y * c + z * s);
//                    a[jj][i] = (float)(z * c - y * s);
//                }
//            }
//            rv1[l] = 0.0;
//            rv1[k] = f;
//            w[k] = (float)x;
//        }
//    }
//    free((void*) rv1);
//    return(1);
//}
