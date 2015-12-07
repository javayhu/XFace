#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <edu_thu_xface_libs_XFaceLibrary.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <android/log.h>

#define LOG_TAG "xface lib"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;

//static FaceRecognizer* xfacerecognizer;
//jlong xfaceaddr;

static void read_csv(const string& filename, vector<Mat>& images,
		vector<int>& labels, char separator = ';') {
	std::ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		LOGD("No valid input file was given, please check the given filename.");
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeInitFacerec
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */JNIEXPORT jlong JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeInitFacerec(
		JNIEnv * jenv, jclass jclazz, jstring jdatapath, jstring jmodelpath,
		jint component, jdouble threshold, jint facerec) {
	LOGD("native init facerec type=%d", facerec);
	const char* dpath = jenv->GetStringUTFChars(jdatapath, NULL);
	string datapath(dpath);
	const char* mpath = jenv->GetStringUTFChars(jmodelpath, NULL);
	string modelpath(mpath);
	LOGD("path is %s.", dpath);
	//when log using string get this error!
	//error: cannot pass objects of non-trivially-copyable type 'std::string {aka struct std::basic_string<char>}' through '...'
	jlong result = 1;
	// These vectors hold the images and corresponding labels.
	vector<Mat> images;
	vector<int> labels;
	// Read in the data. This can fail if no valid input filename is given.
	try {
		read_csv(datapath, images, labels);
	} catch (cv::Exception& e) {
		LOGD("error opening file,reason:%s", e.what());
		result = -1;
		return result;
	}
	// Quit if there are not enough images for this demo.
	if (images.size() <= 1) {
		LOGD("This demo needs at least 2 images to work.");
		result = -2;
		return result;
	}
	Ptr<FaceRecognizer> model;
	if (facerec == 1) {
		model = createEigenFaceRecognizer(); //component, threshold
	} else if (facerec == 2) {
		model = createFisherFaceRecognizer(); //component, threshold
	} else {
		model = createEigenFaceRecognizer(); //component, threshold
	}
	model->train(images, labels);
//	xfacerecognizer = model.obj;
//	result = (jlong) xfacerecognizer;
//	LOGD("model saved,xfacerecognizer addr = %ld", result);
//	xfaceaddr = result;
	//-2142842732!!! but,in return,get 1954432
	// 1-1111111 10111001 00101111 01101100
	// 3747692 -> 2054768
	model->save(modelpath);
	LOGD("native model saved");
	return result;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeFacerec
 * Signature: (Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeFacerec(
		JNIEnv * jenv, jclass jclazz, jlong xfacerec, jstring jmodelpath,
		jlong mataddr, jint width, jint height) {
	LOGD("xfacerec=%ld,mataddr=%ld", xfacerec, mataddr);
	const char* mpath = jenv->GetStringUTFChars(jmodelpath, NULL);
	string modelpath(mpath);
	jint result = -1;
	Mat sample = *((Mat*) mataddr);
	//process image first
	if(sample.rows < sample.cols){//when rows < cols, that is height < width
		flip(sample.t(), sample, 0);//**no need to flip now!**wrong**
	}
	LOGD("########## sample width=%d,height=%d",sample.cols,sample.rows);//320*240
	resize(sample, sample, Size(width, height));
	//	imwrite("/mnt/sdcard/xface/sample.jpg", sample);
	//	result = ((FaceRecognizer*)jthiz)->predict(sample);
	Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
	model->load(modelpath);
//	LOGD("sample processed ok");
//	xfacerecognizer = ((FaceRecognizer*) (jlong)modeladdr);
//	if (xfacerecognizer) {
//		LOGD("xfacerecognizer is not null");
//		LOGD("xfacerecognizer addr = %ld", (jlong)xfacerecognizer);
//		//276743367
//		int pre = xfacerecognizer->predict(sample);
//		LOGD("xfacerecognizer predict is %d", pre);
//	}
	result = model->predict(sample);
	LOGD("result is %d", result);
	return result;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeDestoryFacerec
 * Signature: ()I
 */JNIEXPORT jint JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeDestoryFacerec(
		JNIEnv * jenv, jclass jclazz, jlong xfacerec) {
	LOGD("native destory facerec");
	return 1;
}

