#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <edu_thu_xface_libs_XFaceLibraryV1.h>

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
 * Method:    initFacerec
 * Signature: (Ljava/lang/String;)I
 */JNIEXPORT jlong JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeInitFacerec(
		JNIEnv * jenv, jclass jclazz, jstring jpath) {
	LOGD("init facerec");
	const char* cpath = jenv->GetStringUTFChars(jpath, NULL);
	string path(cpath);
	LOGD("path is %s.", cpath);
	//when log using string get this error!
	//error: cannot pass objects of non-trivially-copyable type 'std::string {aka struct std::basic_string<char>}' through '...'
	jlong result = 1;
	// These vectors hold the images and corresponding labels.
	vector<Mat> images;
	vector<int> labels;
	// Read in the data. This can fail if no valid input filename is given.
	try {
		read_csv(path, images, labels);
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
	Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
	model->train(images, labels);
	//error: invalid cast from type 'cv::Ptr<cv::FaceRecognizer>' to type 'jlong {aka long long int}'
	//result = (jlong)(model);
	result = (jlong) (model.obj); //get through
	// Get the height from the first image. We'll need this
	// later in code to reshape the images to their original size:
//	height = images[0].rows;
//	width = images[0].cols;
	return result;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    facerec
 * Signature: (J)I
 */JNIEXPORT jint JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeFacerec(
		JNIEnv * jenv, jclass jclazz, jlong jthiz, jint jnativeint) {
	LOGD("######### addr= %d , thiz=%ld ########", jnativeint, jthiz);
	jint result = -1;
	Mat sample = *((Mat*) jnativeint);
//	LOGD("sample width=%d,height=%d",sample.cols,sample.rows);//320*240
	//process image first
	flip(sample.t(), sample, 0);
	resize(sample, sample, Size(240, 360));
//	imwrite("/mnt/sdcard/xface/sample.jpg", sample);
//	result = ((FaceRecognizer*)jthiz)->predict(sample);
//	const char * name = jenv->GetStringUTFChars(((FaceRecognizer*) jthiz)->name(),NULL);
	FaceRecognizer* model = ((FaceRecognizer*)jthiz);
	if(model){
		result = model->predict(sample);
	}else{
		LOGD("model is null");
	}
	LOGD("result is %d", result);
	return result;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    destoryFacerec
 * Signature: ()I
 */JNIEXPORT jint JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeDestoryFacerec(
		JNIEnv * jenv, jclass jclazz, jlong jthiz) {
	LOGD("destory facerec");
	if (jthiz != 0) {
		delete ((Ptr<FaceRecognizer>*) jthiz);
	}
	return 1;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    testLong
 * Signature: (J)V
 */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_testLong(JNIEnv *,
		jclass, jlong jnum) {
	LOGD("number is %ld ", jnum);
	//number is negative!!!!why?
}

