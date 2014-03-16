#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/contrib/detection_based_tracker.hpp>
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

// read csv file to get images and labels(integer)
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
	LOGD("#### native init facerec type=%d", facerec);
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
	LOGD("#### xfacerec=%ld,mataddr=%ld", xfacerec, mataddr);//xfacerec now is useless
	const char* mpath = jenv->GetStringUTFChars(jmodelpath, NULL);
	string modelpath(mpath);
	jint result = -1;
	Mat sample = *((Mat*) mataddr);
	//process image first
	if(sample.rows < sample.cols){//when rows < cols, that is height < width
		flip(sample.t(), sample, 0);//**no need to flip now!**wrong**
	}
	LOGD("sample width=%d,height=%d",sample.cols,sample.rows);//320*240
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

 inline void vector_Rect_to_Mat(vector<Rect>& v_rect, Mat& mat) {
 	mat = Mat(v_rect, true);
 }

 /*
  * Class:     edu_thu_xface_libs_XFaceLibrary
  * Method:    nativeCreateObject
  * Signature: (Ljava/lang/String;I)J
  */
 JNIEXPORT jlong JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeCreateObject(
 		JNIEnv * jenv, jclass, jstring jFileName, jint faceSize) {
 	LOGD(
 			"Java_edu_thu_xface_libs_XFaceLibrary_nativeCreateObject enter");
 	const char* jnamestr = jenv->GetStringUTFChars(jFileName, NULL);
 	string stdFileName(jnamestr);
 	jlong result = 0;

 	try {
 		DetectionBasedTracker::Parameters DetectorParams;
 		if (faceSize > 0)
 			DetectorParams.minObjectSize = faceSize;
 		result = (jlong) new DetectionBasedTracker(stdFileName, DetectorParams);
 	} catch (cv::Exception& e) {
 		LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
 		jclass je = jenv->FindClass("org/opencv/core/CvException");
 		if (!je)
 			je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je, e.what());
 	} catch (...) {
 		LOGD("nativeCreateObject caught unknown exception");
 		jclass je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je,
 				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
 		return 0;
 	}

 	LOGD(
 			"Java_edu_thu_xface_libs_XFaceLibrary_nativeCreateObject exit");
 	return result;
 }

 /*
  * Class:     edu_thu_xface_libs_XFaceLibrary
  * Method:    nativeDestroyObject
  * Signature: (J)V
  */
 JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeDestroyObject(
 		JNIEnv * jenv, jclass, jlong thiz) {
 	LOGD(
 			"Java_edu_thu_xface_libs_XFaceLibrary_nativeDestroyObject enter");
 	try {
 		if (thiz != 0) {
 			((DetectionBasedTracker*) thiz)->stop();
 			delete (DetectionBasedTracker*) thiz;
 		}
 	} catch (cv::Exception& e) {
 		LOGD("nativeestroyObject caught cv::Exception: %s", e.what());
 		jclass je = jenv->FindClass("org/opencv/core/CvException");
 		if (!je)
 			je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je, e.what());
 	} catch (...) {
 		LOGD("nativeDestroyObject caught unknown exception");
 		jclass je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je,
 				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
 	}
 	LOGD(
 			"Java_edu_thu_xface_libs_XFaceLibrary_nativeDestroyObject exit");
 }

 /*
  * Class:     edu_thu_xface_libs_XFaceLibrary
  * Method:    nativeStart
  * Signature: (J)V
  */
 JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeStart(
 		JNIEnv * jenv, jclass, jlong thiz) {
 	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeStart enter");
 	try {
 		((DetectionBasedTracker*) thiz)->run();
 	} catch (cv::Exception& e) {
 		LOGD("nativeStart caught cv::Exception: %s", e.what());
 		jclass je = jenv->FindClass("org/opencv/core/CvException");
 		if (!je)
 			je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je, e.what());
 	} catch (...) {
 		LOGD("nativeStart caught unknown exception");
 		jclass je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je,
 				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
 	}
 	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeStart exit");
 }

 /*
  * Class:     edu_thu_xface_libs_XFaceLibrary
  * Method:    nativeStop
  * Signature: (J)V
  */
 JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeStop(
 		JNIEnv * jenv, jclass, jlong thiz) {
 	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeStop enter");
 	try {
 		((DetectionBasedTracker*) thiz)->stop();
 	} catch (cv::Exception& e) {
 		LOGD("nativeStop caught cv::Exception: %s", e.what());
 		jclass je = jenv->FindClass("org/opencv/core/CvException");
 		if (!je)
 			je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je, e.what());
 	} catch (...) {
 		LOGD("nativeStop caught unknown exception");
 		jclass je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je,
 				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
 	}
 	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeStop exit");
 }

 /*
  * Class:     edu_thu_xface_libs_XFaceLibrary
  * Method:    nativeSetFaceSize
  * Signature: (JI)V
  */
 JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeSetFaceSize(
 		JNIEnv * jenv, jclass, jlong thiz, jint faceSize) {
 	LOGD(
 			"Java_edu_thu_xface_libs_XFaceLibrary_nativeSetFaceSize enter");
 	try {
 		if (faceSize > 0) {
 			DetectionBasedTracker::Parameters DetectorParams =
 					((DetectionBasedTracker*) thiz)->getParameters();
 			DetectorParams.minObjectSize = faceSize;
 			((DetectionBasedTracker*) thiz)->setParameters(DetectorParams);
 		}
 	} catch (cv::Exception& e) {
 		LOGD("nativeStop caught cv::Exception: %s", e.what());
 		jclass je = jenv->FindClass("org/opencv/core/CvException");
 		if (!je)
 			je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je, e.what());
 	} catch (...) {
 		LOGD("nativeSetFaceSize caught unknown exception");
 		jclass je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je,
 				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
 	}
 	LOGD(
 			"Java_edu_thu_xface_libs_XFaceLibrary_nativeSetFaceSize exit");
 }

 /*
  * Class:     edu_thu_xface_libs_XFaceLibrary
  * Method:    nativeDetect
  * Signature: (JJJ)V
  */
 JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeDetect(
 		JNIEnv * jenv, jclass, jlong thiz, jlong imageGray, jlong faces) {
 	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeDetect enter");
 	try {
 		vector<Rect> RectFaces;
 		((DetectionBasedTracker*) thiz)->process(*((Mat*) imageGray));
 		((DetectionBasedTracker*) thiz)->getObjects(RectFaces);
 		vector_Rect_to_Mat(RectFaces, *((Mat*) faces));
 	} catch (cv::Exception& e) {
 		LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
 		jclass je = jenv->FindClass("org/opencv/core/CvException");
 		if (!je)
 			je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je, e.what());
 	} catch (...) {
 		LOGD("nativeDetect caught unknown exception");
 		jclass je = jenv->FindClass("java/lang/Exception");
 		jenv->ThrowNew(je,
 				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
 	}
 	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeDetect exit");
 }

