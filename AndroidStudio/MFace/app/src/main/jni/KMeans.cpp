#include "ScoreHead.h"

using namespace std;


void KMeans(double **ppdB,double **ppdCN_test,int number,int ClassNum)
{
	double threshold=0.0001;
	int *classindex=(int *)pvMalloc1D(number,sizeof(int));
	int *num=(int *)pvMalloc1D(ClassNum,sizeof(int));
	double **center=(double **)ppvMalloc2D(ClassNum,11,sizeof(double *),sizeof(double));
	for (int index=0;index<ClassNum;index++)
	{
		matrix_copy_double_1D(11,ppdCN_test[index],ppdB[index]);
	}
	double *d=(double *)pvMalloc1D(ClassNum,sizeof(double));
	int times=20;
	int count=1;
	while (true){
		matrix_copy_double_2D(ClassNum,11,ppdB,center);
		for (int index=0;index<number;index++)
		{
			for (int index2=0;index2<ClassNum;index2++)
			{
				d[index2]=dist(ppdCN_test[index],ppdB[index2]);
			}
			int pos=minPosition(d,ClassNum);
			classindex[index]=pos;
		}
		//更新聚类中心
		for (int index=0;index<ClassNum;index++)
		{
			num[index]=1;
		}
		for (int index=0;index<number;index++)
		{
			matrix_add_double_1D(11,ppdB[classindex[index]],ppdCN_test[index],ppdB[classindex[index]]);
			num[classindex[index]]+=1;
		}
		for (int index=0;index<ClassNum;index++)
		{
			matrix_rdiv_double_1D(11,ppdB[index],num[index],ppdB[index]);
		}
		double errormax=0;
		double error=0;
		for (int index=0;index<ClassNum;index++)
		{
			error=dist(center[index],ppdB[index]);
			if(error>errormax)
				errormax=error;
		} 
		count++;
		if(count>times || errormax<threshold)
			break;
	}
	Free1D((void *)classindex);
	Free1D((void *)d);
	Free1D((void *)num);
	Free2D((void **)center,ClassNum);
}