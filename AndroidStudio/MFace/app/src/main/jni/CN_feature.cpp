#include "ScoreHead.h"

using namespace std;


void CN_feature(double *pdCN,int ***block,double **ppdw2c,int block_row,int block_col)
{	
	int index_im;
	matrix_Init1D_double(11,pdCN);

	for (int q=0;q<block_col;q++)
	{
		for (int p=0;p<block_row;p++)
		{
			index_im=floor(block[p][q][0]/8)+32*floor(block[p][q][1]/8)+32*32*floor(block[p][q][2]/8);
			matrix_add_double_1D(11,ppdw2c[index_im],pdCN,pdCN);
		}
	}
	matrix_rdiv_double_1D(11,pdCN,block_row*block_col,pdCN);
}