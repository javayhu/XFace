#include "ScoreHead.h"

using namespace std;

double dist2(double *b,double*a){
	double product=0;
	double normb=0;
	double norma=0;
	for (int index=0;index<11;index++)
	{
		product+=b[index]*a[index];
		normb+=b[index]*b[index];
		norma+=a[index]*a[index];
	}
	normb=sqrt(normb);
	norma=sqrt(norma);
	return 1-product/(normb*norma);
}