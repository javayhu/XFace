#include "ScoreHead.h"

using namespace std;

int minPosition(double *a,int len)
{
	int pos=0;
	double minimum=a[pos];
	for (int index=0;index<len;index++)
	{
		if(a[index]<minimum)
		{
			pos=index;
			minimum=a[index];
		}
	}
	return pos;
}