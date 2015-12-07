#include "ScoreHead.h"

using namespace std;

double mean(double *a,int len)
{
	double mean=0;
	for (int index=0;index<len;index++)
	{
		mean+=a[index];
	}
	mean=mean/len;
	return mean;
}