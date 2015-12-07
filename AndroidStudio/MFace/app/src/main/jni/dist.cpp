#include "ScoreHead.h"

using namespace std;

double dist(double *b,double*a){
	double product=0;
	for (int index=0;index<11;index++)
	{	
		product+=(b[index]-a[index])*(b[index]-a[index]);
	}
	return product;
}