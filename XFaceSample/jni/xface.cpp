#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/contrib/detection_based_tracker.hpp>

#include <helper.hpp>
#include <eigenfaces.hpp>
#include <edu_thu_xface_libs_XFaceLibrary.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <android/log.h>

#define LOG_TAG "xface native lib --- xface.cpp "
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;

//------------------------------------------------------------------------------
// face recognition part
//------------------------------------------------------------------------------

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
 * Signature: (Ljava/lang/String;Ljava/lang/String;IDI)J
 */JNIEXPORT jlong JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeTrainModel(
		JNIEnv * jenv, jclass jclazz, jstring jdatapath, jstring jorlpath,
		jstring jmodelpath, jint component, jdouble threshold) {
	LOGD("#### native train model");//about 44 seconds
	//facerec algorithm
	const char* dpath = jenv->GetStringUTFChars(jdatapath, NULL);
	string datapath(dpath);
	const char* mpath = jenv->GetStringUTFChars(jmodelpath, NULL);
	string modelpath(mpath);
	const char* opath = jenv->GetStringUTFChars(jorlpath, NULL);
	string orlpath(opath);
	//	LOGD("data path is %s.", dpath);
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
		LOGD("error opening face data file,reason:%s", e.what());
		result = -1;
		return result;
	}
	// Quit if there are not enough images for this demo.
//	if (images.size() <= 1) {
//		LOGD("This demo needs at least 2 images to work.");
//		result = -2;
//		return result;
//	}

	vector<Mat> orlImages;
	vector<int> orlLabels;
	try {
		read_csv(orlpath, orlImages, orlLabels);
	} catch (cv::Exception& e) {
		LOGD("error opening orl face data file,reason:%s", e.what());
		result = -1;
		return result;
	}

	for (int i = 0; i < images.size(); i++) {
		orlImages.push_back(images.at(i));//put images to orlImages!-> first orl images, then user images
//		labels.push_back(orlLabels.at(i));//do not add labels!
	}

	// eigenfaces --> attention: orlImages's size is not as much as labels
	Eigenfaces eigenfaces(orlImages, labels, component, threshold); //
	eigenfaces.save(modelpath);
	result = (long) (&eigenfaces);
	LOGD("native model saved, address is %lld ", result);
	// end

	return result;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeInitFacerec
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */JNIEXPORT jlong JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeInitFacerec(
		JNIEnv * jenv, jclass jclazz, jstring jdatapath, jstring jmodelpath,
		jint component, jdouble threshold) {
	LOGD("#### native init facerec");
	//facerec algorithm
//	const char* dpath = jenv->GetStringUTFChars(jdatapath, NULL);
//	string datapath(dpath);
//	const char* mpath = jenv->GetStringUTFChars(jmodelpath, NULL);
//	string modelpath(mpath);
//	LOGD("data path is %s.", dpath);
	//when log using string get this error!
	//error: cannot pass objects of non-trivially-copyable type 'std::string {aka struct std::basic_string<char>}' through '...'
	jlong result = 1;
	// These vectors hold the images and corresponding labels.
//	vector<Mat> images;
//	vector<int> labels;
	// Read in the data. This can fail if no valid input filename is given.
//	try {
//		read_csv(datapath, images, labels);
//	} catch (cv::Exception& e) {
//		LOGD("error opening file,reason:%s", e.what());
//		result = -1;
//		return result;
//	}
	// Quit if there are not enough images for this demo.
//	if (images.size() <= 1) {
//		LOGD("This demo needs at least 2 images to work.");
//		result = -2;
//		return result;
//	}

//	vector<Mat> orlImages;
//	vector<int> orlLabels;
//	try {
//		read_csv("/mnt/sdcard/xface/orlfaces.txt", orlImages, orlLabels);
//	} catch (cv::Exception& e) {
//		LOGD("error opening file,reason:%s", e.what());
//		result = -1;
//		return result;
//	}
//
//	for (int i = 0; i < orlImages.size(); i++) {
//		images.push_back(orlImages.at(i));
//		labels.push_back(orlLabels.at(i));
//	}

	// eigenfaces
//	Eigenfaces eigenfaces(images, labels, component, threshold); //
//	eigenfaces.save(modelpath);
//	result = (long) (&eigenfaces);
//	LOGD("native model saved, address is %lld ", result);
	// end

	// opencv
//	Ptr<FaceRecognizer> model;
//	if (facerec == 1) {
//		model = createEigenFaceRecognizer(); //component, threshold
//	} else if (facerec == 2) {
//		model = createFisherFaceRecognizer(); //component, threshold
//	} else {
//		model = createEigenFaceRecognizer(); //component, threshold
//	}
//	model->train(images, labels);
//	model->save(modelpath);
//	FaceRecognizer* recobj = model.obj;
//	result = (jlong) (recobj); // wrong: (jlong) model
//	LOGD("native model saved, result is %lld ", result);
	// end

	return result;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeFacerec
 * Signature: (Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeFacerec(
		JNIEnv * jenv, jclass jclazz, jstring jmodelpath, jlong xfacerec,
		jlong mataddr, jint width, jint height) {
	LOGD("#### facerec xfacerec=%lld,mataddr=%lld", xfacerec, mataddr);
	//xfacerec now is useless
	const char* mpath = jenv->GetStringUTFChars(jmodelpath, NULL);
	string modelpath(mpath);
	jint result = -1;
	Mat sample = *((Mat*) mataddr);
	//process image first
	if (sample.rows < sample.cols) { //when rows < cols, that is height < width
		flip(sample.t(), sample, 0); //**no need to flip now!**wrong**
	}
	resize(sample, sample, Size(width, height)); //320*240->92*112
//	imwrite("/mnt/sdcard/xface/sample.jpg", sample);

	// eigenfaces
	Eigenfaces eigenfaces;
	eigenfaces.load(modelpath);
	result = eigenfaces.predict(sample);

//    Eigenfaces* model2 = (Eigenfaces*)(xfacerec);
	//Eigenfaces obtained is not the last one! that is to say all values in the eigenfaces are missing!
//	Eigenfaces model2 = *((Eigenfaces*)(xfacerec));//&eigenfaces
//	LOGD("native model obtained, address is %lld ", &model2);
//    result = model2.predict(sample);

//	long facelong = (long) (&eigenfaces);
//	Eigenfaces eigenfaces2(facelong);
//	result = eigenfaces2.predict(sample);
	// end

	// opencv
//	FaceRecognizer* model = ((FaceRecognizer*) xfacerec);//error!
//	result = model->predict(sample);
//	result = (*((FaceRecognizer*) xfacerec)).predict(sample); //->predict(sample);
//	Ptr<FaceRecognizer> model = createEigenFaceRecognizer();
//	model->load(modelpath);
//	result = model->predict(sample);
	//end

	LOGD("predict result is %d", result);
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

//------------------------------------------------------------------------------
// face detection part
//------------------------------------------------------------------------------
inline void vector_Rect_to_Mat(vector<Rect>& v_rect, Mat& mat) {
	mat = Mat(v_rect, true);
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeCreateObject
 * Signature: (Ljava/lang/String;I)J
 */JNIEXPORT jlong JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeInitFacedetect(
		JNIEnv * jenv, jclass, jstring jFileName, jint faceSize) {
	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeCreateObject enter");
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

	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeCreateObject exit");
	return result;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeDestroyObject
 * Signature: (J)V
 */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeDestroyFacedetect(
		JNIEnv * jenv, jclass, jlong thiz) {
	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeDestroyObject enter");
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
	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeDestroyObject exit");
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeStart
 * Signature: (J)V
 */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeStartFacedetect(
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
 */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeStopFacedetect(
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
 */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeSetFaceSize(
		JNIEnv * jenv, jclass, jlong thiz, jint faceSize) {
	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeSetFaceSize enter");
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
	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeSetFaceSize exit");
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeDetect
 * Signature: (JJJ)V
 */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeFacedetect(
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

