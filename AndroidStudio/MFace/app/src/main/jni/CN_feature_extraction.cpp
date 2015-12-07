#include "ScoreHead.h"

using namespace std;

void CN_feature_extraction(double **ppdCN_test,int ***pppiImg,double **ppdw2c,int block_row,int block_col,int n1,int n2,int step)
{

	int ***block=(int ***)pppvMalloc3D(block_row,block_col,3,sizeof(int **),sizeof(int *),sizeof(int));

	int count=0;
	for (int p=0;p<n1;p=p+step)
	{
		for (int q=0;q<n2;q=q+step)
		{
			for (int x=0;x<block_row;x++)
			{
				for (int y=0;y<block_col;y++)
				{
					matrix_copy_int_1D(3,pppiImg[p*block_row+x][q*block_col+y],block[x][y]);
				}
			}
			CN_feature(ppdCN_test[count],block,ppdw2c,block_row,block_col);
			count++;
		}	
	}

	Free3D((void ***)block,block_row,block_col);
}