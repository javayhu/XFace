#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <cv.h>
#include "highgui.h"
#include "Complex.h"
#include "Malloc2.h"
#include "Matrix.h"
#include <fstream>
#include <math.h>
#include <stdio.h>

using namespace cv;

double MainFunction(Mat &img,char flag,const char *cs1,const char *cs2,const char *cs3);
void CN_feature_extraction(double **ppdCN_test,int ***pppiImg,double **ppdw2c,int block_row,int block_col,int n1,int n2,int step);
void CN_feature(double *pdCN,int ***block,double **ppdw2c,int block_row,int block_col);

void KMeans(double **ppdB,double **ppdCN_test,int number,int ClassNum);
double giveScore(double **ppdA,double **ppdB,char flag,int ClassNum);
double dist2(double *b,double*a);
int minPosition(double *a,int len);
double mean(double *a,int len);
double dist(double *b,double*a);

