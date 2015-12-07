#include "ScoreHead.h"

#include <android/log.h>

#define LOG_TAG "PointFaceLib-cpp"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;

double MainFunction(Mat &img,char flag,const char *cs1,const char*cs2,const char *cs3) {
	int ClassNum=10;

	LOGD("native main cs1: %s", cs1);
	
	//��mat���һ����ά���pppiImg
	if(img.empty())
	{
		cout<<"can't open"<<endl;
		return -1;
	}

	int im_row=img.rows;
	int im_col=img.cols;

	int ***pppiImg=(int ***)pppvMalloc3D(img.rows,img.cols,3,sizeof(int **),sizeof(int *),sizeof(int));
	for (int i=0;i<img.rows;i++){
		for (int j=0;j<img.cols;j++){
			pppiImg[i][j][0]=(unsigned int)(img.at<Vec3b>(i,j)[2]);
			pppiImg[i][j][1]=(unsigned int)(img.at<Vec3b>(i,j)[1]);
			pppiImg[i][j][2]=(unsigned int)(img.at<Vec3b>(i,j)[0]);
		}
	}
	
	FILE *fw2c;
	if((fw2c=fopen(cs1,"rb"))==NULL)
	{
		fprintf(stderr,"error redirecting stdout\n");
		return -2;
	}
	int M=32768;
	int N=11;
	double **ppdw2c=(double **)ppvMalloc2D(M,N,sizeof(double *),sizeof(double));
	for(int i=0;i<M;i++){
		for(int j=0;j<N;j++){
			float temp;
			fscanf(fw2c,"%f",&temp);
			ppdw2c[i][j]=temp;
		}
	}
	fclose(fw2c);

	//ppdA��codebook�е�10��11ά����
	/*int ClassNum=10;*/
	double **ppdA=(double **)ppvMalloc2D(ClassNum,11,sizeof(double *),sizeof(double));
	if(flag='0')//Ů��
	{
		ifstream fFemaleCodebook(cs3);
		for(int i=0;i<ClassNum;i++){
			for(int j=0;j<11;j++){
				fFemaleCodebook>>ppdA[i][j];
			}
		}
		fFemaleCodebook.close();
	}
	else//����
	{
		ifstream fMaleCodebook(cs2);
		for(int i=0;i<ClassNum;i++){
			for(int j=0;j<11;j++){
				fMaleCodebook>>ppdA[i][j];
			}
		}
		fMaleCodebook.close();
	}


	//CN_test��һ����ά����,����֮ǰ
	int block_row=128;
	int block_col=128;
	int step=1;
	int n1=floor(im_row/block_row);
	int n2 = floor(im_col/block_col);

	//int number=floor((img.rows-block_row)/step+1)*floor((img.cols-block_col)/step+1);
	int number=floor((n1-1)/step+1)*floor((n2-1)/step+1);
	double **ppdCN_test=(double **)ppvMalloc2D(number,11,sizeof(double *),sizeof(double));
	CN_feature_extraction(ppdCN_test,pppiImg,ppdw2c,block_row,block_col,n1,n2,step);

	//B�ǲ���ͼƬ��10��11ά����
	double **ppdB=(double **)ppvMalloc2D(ClassNum,11,sizeof(double *),sizeof(double));
	KMeans(ppdB,ppdCN_test,number,ClassNum);


	//score�Ƿ���
	double score=0;
	score=giveScore(ppdA,ppdB,flag,ClassNum);

	//�ͷ��ڴ�
	Free3D((void ***)pppiImg,img.rows,img.cols);
	Free2D((void **)ppdw2c,M);
	Free2D((void **)ppdA,ClassNum);
	Free2D((void **)ppdCN_test,number);
	Free2D((void **)ppdB,ClassNum);
	
	return score;

}