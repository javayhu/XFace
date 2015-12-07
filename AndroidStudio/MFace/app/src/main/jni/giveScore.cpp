#include "ScoreHead.h"

using namespace std;


double giveScore(double **ppdA,double **ppdB,char flag,int ClassNum)
{
	double score=0;
	double *d=(double *)pvMalloc1D(ClassNum,sizeof(double));
	double *distance=(double *)pvMalloc1D(ClassNum,sizeof(double));
	matrix_Init1D_double(ClassNum,d);
	matrix_Init1D_double(ClassNum,distance);
		
	for (int index=0;index<ClassNum;index++)
	{
		for (int index2=0;index2<ClassNum;index2++)
		{
			d[index2]=dist2(ppdB[index],ppdA[index2]);
		}
		int pos=minPosition(d,ClassNum);
		distance[index]=d[pos];
	}
	if(flag=='0')
	{
		score=-log(mean(distance,ClassNum))*22;	
	}
	else
	{
		score=-log(mean(distance,ClassNum))*25;	
	}
	if(score>100)
		score=100;
	if(score<0)
		score=10;

	Free1D(d);
	Free1D(distance);

	return score;
}