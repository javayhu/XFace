#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/contrib/detection_based_tracker.hpp>

#include <helper.hpp>
#include <eigenfaces.hpp>
#include <xface.h>
#include <preprocessFace.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <android/log.h>

#define LOG_TAG "xface native lib"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

//char* sdpath = "";
//string sdpath = "";

//const char *faceCascadeFilename = "haarcascade_frontalface_alt_tree.xml";  // Haar face detector.
//const char *eyeCascadeFilename1 = "haarcascade_lefteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//const char *eyeCascadeFilename2 = "haarcascade_righteye_2splits.xml";   // Best eye detector for open-or-closed eyes.
//const char *eyeCascadeFilename1 = "haarcascade_mcs_lefteye.xml";       // Good eye detector for open-or-closed eyes.
//const char *eyeCascadeFilename2 = "haarcascade_mcs_righteye.xml";       // Good eye detector for open-or-closed eyes.

//const char *faceCascadeFilename = "lbpcascade_frontalface.yml"; // LBP face detector.
//const char *eyeCascadeFilename1 = "haarcascade_eye.xml"; // Basic eye detector for open eyes only.
//const char *eyeCascadeFilename2 = "haarcascade_eye_tree_eyeglasses.xml"; // Basic eye detector for open eyes if they might wear glasses.

using namespace std;
using namespace cv;

//------------------------------------------------------------------------------
// face recognition part
//------------------------------------------------------------------------------

// read csv file to get images and labels(integer)
static void read_csv(const string& filename, vector<Mat>& images, vector<int>& labels,
		char separator = ';') {
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
 */JNIEXPORT jlong JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeTrainModel(JNIEnv * jenv,
		jclass jclazz, jlong xfacerec, jstring jdatapath, jstring jorlpath, jstring jmodelpath,
		jint component, jdouble threshold) {
	LOGD("#### native train model xfacerec=%lld", xfacerec);
	//0
	const char* dpath = jenv->GetStringUTFChars(jdatapath, NULL);
	string datapath(dpath);
	const char* mpath = jenv->GetStringUTFChars(jmodelpath, NULL);
	string modelpath(mpath);
//	const char* opath = jenv->GetStringUTFChars(jorlpath, NULL);// delete orl path!!!
//	string orlpath(opath);
	jlong result = 1;
	vector<Mat> images;
	vector<int> labels;
	try {
		read_csv(datapath, images, labels);
	} catch (cv::Exception& e) {
		LOGD("error opening face data file,reason:%s", e.what());
		result = -1;
		return result;
	}

	// not orl faces! but my train faces this time!
//	vector<Mat> orlImages;
//	vector<int> orlLabels;
//	try {
//		read_csv(orlpath, orlImages, orlLabels);
//	} catch (cv::Exception& e) {
//		LOGD("error opening orl face data file,reason:%s", e.what());
//		result = -1;
//		return result;
//	}

//	for (int i = 0; i < images.size(); i++) {
//		orlImages.push_back(images.at(i)); //put images to orlImages!-> first orl images, then user images
////		labels.push_back(orlLabels.at(i));//do not add labels!
//	}

// if old xfacerec exists! destroy it!
	if (xfacerec != 0) {
		delete (Eigenfaces*) xfacerec;
	}
//	LOGD("#### native train model mid, address is %lld ", result);
	// eigenfaces --> attention: orlImages's size is not as much as labels
	Eigenfaces* eigenfaces = new Eigenfaces(images, labels, component, threshold); //error!!!
	(*eigenfaces).save(modelpath);
	result = (jlong) (eigenfaces);
	LOGD("#### native train model end, address is %lld ", result);
	// end

	return result;
}

const int faceWidth = 70;
const int faceHeight = faceWidth;

bool processFace(Mat& image, jlong facecas, jlong eyecas, jlong eyeglasscas) {
	LOGD("#### native process face");
	const bool preprocessLeftAndRightSeparately = true; // Preprocess left & right sides of the face separately, in case there is stronger light on one side.
	CascadeClassifier* faceCascade = ((CascadeClassifier*) (facecas));
	CascadeClassifier* eyeCascade = ((CascadeClassifier*) (eyecas));
	CascadeClassifier* eyeGlassCascade = ((CascadeClassifier*) (eyeglasscas));

	// Find a face and preprocess it to have a standard size and contrast & brightness.
	Rect faceRect; // Position of detected face.
	Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
	Point leftEye, rightEye; // Position of the detected eyes.
	// put data back!!!
	image = getPreprocessedFace(image, faceWidth, (*faceCascade), (*eyeCascade), (*eyeGlassCascade),
			preprocessLeftAndRightSeparately, &faceRect, &leftEye, &rightEye, &searchedLeftEye,
			&searchedRightEye);

	bool gotFaceAndEyes = false;
	if (image.data) {
		gotFaceAndEyes = true;
	}
	LOGD("#### native process face end %d", gotFaceAndEyes?1:0);
	return gotFaceAndEyes;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeAddImage
 * Signature: (Ljava/lang/String;Ljava/lang/String;IDI)J
 */int JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeAddImage(JNIEnv * jenv, jclass jclazz,
		jlong xfacerec, jlong facecas, jlong eyecas, jlong eyeglasscas, jstring jmodelpath,
		jlong mataddr, jint label, jstring jfilepath, jstring jdemopath) {
	LOGD("#### native add image xfacerec=%lld, label=%d, mataddr=%lld", xfacerec, label, mataddr);
	const char* mpath = jenv->GetStringUTFChars(jmodelpath, NULL);
	string modelpath(mpath);
	const char* fpath = jenv->GetStringUTFChars(jfilepath, NULL);
	string filepath(fpath);
	const char* dpath = jenv->GetStringUTFChars(jdemopath, NULL);
    string demopath(dpath);
	LOGD("#### image file save path=%s, camerapath", fpath, dpath);
//	int result = 0;

	Mat sample = *((Mat*) mataddr);
//	LOGD("#### add image size1 is %d * %d", sample.rows, sample.cols);//rows=height 480 cols=width 640
	if (sample.rows < sample.cols) { //when rows < cols, that is height < width
		flip(sample.t(), sample, 0); //counter-clock wise 90
	}
//	LOGD("#### add image size2 is %d * %d", sample.rows, sample.cols);//rows=height 640 cols=width 480
	//imwrite("/mnt/sdcard/xface/sample_before.jpg", sample);
	imwrite(demopath, sample);
	bool gotFaceAndEyes = processFace(sample, facecas, eyecas, eyeglasscas);
	if (!gotFaceAndEyes) {
		return 0; //fail
	}
	//imwrite("/mnt/sdcard/xface/sample_after.jpg", sample);
	imwrite(filepath, sample); //save image file

	// similar but wrong code! reason: the orignal eigenfaces do not change!
//	Eigenfaces eigenfaces = *((Eigenfaces*) (xfacerec));
//	eigenfaces.addImage(modelpath, sample, label);

	// eigenfaces
	Eigenfaces* eigenfaces = ((Eigenfaces*) (xfacerec));
	eigenfaces->addImage(modelpath, sample, label);
//	result = (jlong) (&eigenfaces); //address changed! [hujiawei] I don't know why! --> different thread?!
	LOGD("#### native add image ok, address is %lld", (jlong) (&eigenfaces));
	//though address changed, the heap object also changes
	// end
	return 1;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeInitFacerec
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */JNIEXPORT jlong JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeInitFacerec(JNIEnv * jenv,
		jclass jclazz, jstring jmodelpath, jint component, jdouble threshold) {
	LOGD("#### native init facerec component=%d, threshold=%f", component, threshold);
	const char* mpath = jenv->GetStringUTFChars(jmodelpath, NULL);
	string modelpath(mpath);
	jlong result = 0;

	// eigenfaces
	Eigenfaces* eigenfaces = new Eigenfaces(component, threshold);
	(*eigenfaces).load(modelpath); //empty!!! can not be empty!!!
	result = (jlong) (eigenfaces);
	LOGD("#### native init facerec end, model address is %lld ", result);
	// end

	return result;
}

///

JNIEXPORT jlong JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeInitCascade(JNIEnv * jenv,
		jclass jclazz, jstring path) {
	LOGD("#### native init cascade");
	const char* mpath = jenv->GetStringUTFChars(path, NULL);
	LOGD("#### cascade file path is %s", mpath);
	string caspath(mpath);
	jlong result = 0;

	CascadeClassifier* cascade = new CascadeClassifier();
	(*cascade).load(caspath);
	result = (jlong) (cascade);
	LOGD("#### native init cascade end, address is %lld ", result);
	return result;
}

bool processimage(Mat& rgba, Mat& gray, jlong facecas, jlong eyecas, jlong eyeglasscas) {
	const bool preprocessLeftAndRightSeparately = true; // Preprocess left & right sides of the face separately, in case there is stronger light on one side.
	CascadeClassifier* faceCascade = ((CascadeClassifier*) (facecas));
	CascadeClassifier* eyeCascade = ((CascadeClassifier*) (eyecas));
	CascadeClassifier* eyeGlassCascade = ((CascadeClassifier*) (eyeglasscas));

	// Find a face and preprocess it to have a standard size and contrast & brightness.
	Rect faceRect; // Position of detected face.
	Rect searchedLeftEye, searchedRightEye; // top-left and top-right regions of the face, where eyes were searched.
	Point leftEye, rightEye; // Position of the detected eyes.
	// put data back!!!
	Mat preprocessedFace = getPreprocessedFace(gray, faceWidth, (*faceCascade), (*eyeCascade),
			(*eyeGlassCascade), preprocessLeftAndRightSeparately, &faceRect, &leftEye, &rightEye,
			&searchedLeftEye, &searchedRightEye);

	bool gotFaceAndEyes = false;
	if (preprocessedFace.data) {
		gotFaceAndEyes = true;
	}

	if (gotFaceAndEyes) { //if detect face, then draw rectangle on RGBA image!!!!
		// Draw an anti-aliased rectangle around the detected face.
		Scalar eyeColor = CV_RGB(255, 255, 0); //yellow
		Scalar faceColor = CV_RGB(0, 255, 255); //blue
		if (faceRect.width > 0) {
			rectangle(rgba, faceRect, faceColor, 2, CV_AA);

			// Draw light-blue anti-aliased circles for the 2 eyes.
			if (leftEye.x >= 0) { // Check if the eye was detected
				circle(rgba, Point(faceRect.x + leftEye.x, faceRect.y + leftEye.y), 6, eyeColor, 1,
						CV_AA);
			}
			if (rightEye.x >= 0) { // Check if the eye was detected
				circle(rgba, Point(faceRect.x + rightEye.x, faceRect.y + rightEye.y), 6, eyeColor,
						1, CV_AA);
			}
		}
	}
	return gotFaceAndEyes;
}

JNIEXPORT jint JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeFacedet(JNIEnv * jenv,
		jclass jclazz, jlong xfacerec, jlong facecas, jlong eyecas, jlong eyeglasscas, jlong mRgba,
		jlong mGray) {
	LOGD("#### native facerec xfacedet=%lld, mRgba=%lld, mGray=%lld", xfacerec, mRgba, mGray);
	jint result = -1;
	Mat rgba = *((Mat*) mRgba); //480*720
	if (rgba.rows < rgba.cols) { //when rows < cols, that is height < width
		flip(rgba.t(), rgba, 0); //counter-clock wise 90
	}
	Mat gray = *((Mat*) mGray); //480*720
	if (gray.rows < gray.cols) { //when rows < cols, that is height < width
		flip(gray.t(), gray, 0); //counter-clock wise 90
	}
// 	LOGD("#### rgba image size is %d * %d", rgba.rows, rgba.cols);//rows=height 720 cols=width 480
	//	resize(sample, sample, Size(width, height)); //480*720->92*112
	bool getresult = processimage(rgba, gray, facecas, eyecas, eyeglasscas);
	if (!getresult) {
		return result;
	}
	result = 1;
	LOGD("#### native facedet end, result is %d", result);//though detect face, no rectangle result showed on screen!!!
	return result;
}
///

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeFacerec
 * Signature: (Ljava/lang/String;)I
 */JNIEXPORT jint JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeFacerec(JNIEnv * jenv,
		jclass jclazz, jlong xfacerec, jlong facecas, jlong eyecas, jlong eyeglasscas,
		jstring jmodelpath, jlong mataddr/*, jint width, jint height*/) {
	LOGD("#### native facerec xfacerec=%lld, mataddr=%lld", xfacerec, mataddr);
	const char* mpath = jenv->GetStringUTFChars(jmodelpath, NULL);
	string modelpath(mpath);
	jint result = -1;
	Mat sample = *((Mat*) mataddr); //480*720
	if (sample.rows < sample.cols) { //when rows < cols, that is height < width
		flip(sample.t(), sample, 0); //counter-clock wise 90
	}
	LOGD("#### gray image size is %d * %d", sample.rows, sample.cols);
	imwrite("/mnt/sdcard/xface/sample_0gray.jpg", sample);
	//rows=height 720 cols=width 480
//	resize(sample, sample, Size(width, height)); //480*720->92*112
	bool getresult = processFace(sample, facecas, eyecas, eyeglasscas);
	if (!getresult) {
		return result;
	}

	LOGD("#### processed image size is %d * %d", sample.rows, sample.cols);
	//rows=height 720 cols=width 480
	//imwrite("/mnt/sdcard/xface/sample_face.jpg", sample);

	// eigenfaces
	Eigenfaces eigenfaces = *((Eigenfaces*) (xfacerec));
	result = eigenfaces.predict(sample);
	// similar but wrong code! not change the orignal object
//	Eigenfaces* eigenfaces = ((Eigenfaces*) (xfacerec));
//	eigenfaces->predict(sample);
	//end

	LOGD("#### native facerec end result is %d, address is %lld", result, (jlong)(&eigenfaces));
	return result;
}

/*
 * Class:     edu_thu_xface_libs_XFaceLibrary
 * Method:    nativeDestoryFacerec
 * Signature: ()I
 */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeDestoryFacerec(JNIEnv * jenv,
		jclass jclazz, jlong xfacerec, jlong facecas, jlong eyecas, jlong eyeglasscas) {
	LOGD("#### native destory facerec xfacerec=%lld", xfacerec);
	if (xfacerec != 0) {
		delete (Eigenfaces*) xfacerec;
	}
	if (facecas != 0) {
		delete (CascadeClassifier*) facecas;
	}
	if (eyecas != 0) {
		delete (CascadeClassifier*) eyecas;
	}
	if (eyeglasscas != 0) {
		delete (CascadeClassifier*) eyeglasscas;
	}
	LOGD("#### native destory facerec end");
}

//------------------------------------------------------------------------------
// face detection part
//------------------------------------------------------------------------------
//inline void vector_Rect_to_Mat(vector<Rect>& v_rect, Mat& mat) {
//	mat = Mat(v_rect, true);
//}
//
///*
// * Class:     edu_thu_xface_libs_XFaceLibrary
// * Method:    nativeCreateObject
// * Signature: (Ljava/lang/String;I)J
// */JNIEXPORT jlong JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeInitFacedetect(JNIEnv * jenv,
//		jclass, jstring jFileName, jint faceSize) {
//	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeCreateObject enter");
//	const char* jnamestr = jenv->GetStringUTFChars(jFileName, NULL);
//	string stdFileName(jnamestr);
//	jlong result = 0;
//
//	try {
//		DetectionBasedTracker::Parameters DetectorParams;
//		if (faceSize > 0)
//			DetectorParams.minObjectSize = faceSize;
//		result = (jlong) new DetectionBasedTracker(stdFileName, DetectorParams);
//	} catch (cv::Exception& e) {
//		LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
//		jclass je = jenv->FindClass("org/opencv/core/CvException");
//		if (!je)
//			je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je, e.what());
//	} catch (...) {
//		LOGD("nativeCreateObject caught unknown exception");
//		jclass je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je,
//				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
//		return 0;
//	}
//
//	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeCreateObject exit");
//	return result;
//}
//
///*
// * Class:     edu_thu_xface_libs_XFaceLibrary
// * Method:    nativeDestroyObject
// * Signature: (J)V
// */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeDestroyFacedetect(JNIEnv * jenv,
//		jclass, jlong thiz) {
//	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeDestroyObject enter");
//	try {
//		if (thiz != 0) {
//			((DetectionBasedTracker*) thiz)->stop();
//			delete (DetectionBasedTracker*) thiz;
//		}
//	} catch (cv::Exception& e) {
//		LOGD("nativeestroyObject caught cv::Exception: %s", e.what());
//		jclass je = jenv->FindClass("org/opencv/core/CvException");
//		if (!je)
//			je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je, e.what());
//	} catch (...) {
//		LOGD("nativeDestroyObject caught unknown exception");
//		jclass je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je,
//				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
//	}
//	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeDestroyObject exit");
//}
//
///*
// * Class:     edu_thu_xface_libs_XFaceLibrary
// * Method:    nativeStart
// * Signature: (J)V
// */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeStartFacedetect(JNIEnv * jenv,
//		jclass, jlong thiz) {
//	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeStart enter");
//	try {
//		((DetectionBasedTracker*) thiz)->run();
//	} catch (cv::Exception& e) {
//		LOGD("nativeStart caught cv::Exception: %s", e.what());
//		jclass je = jenv->FindClass("org/opencv/core/CvException");
//		if (!je)
//			je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je, e.what());
//	} catch (...) {
//		LOGD("nativeStart caught unknown exception");
//		jclass je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je,
//				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
//	}
//	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeStart exit");
//}
//
///*
// * Class:     edu_thu_xface_libs_XFaceLibrary
// * Method:    nativeStop
// * Signature: (J)V
// */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeStopFacedetect(JNIEnv * jenv,
//		jclass, jlong thiz) {
//	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeStop enter");
//	try {
//		((DetectionBasedTracker*) thiz)->stop();
//	} catch (cv::Exception& e) {
//		LOGD("nativeStop caught cv::Exception: %s", e.what());
//		jclass je = jenv->FindClass("org/opencv/core/CvException");
//		if (!je)
//			je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je, e.what());
//	} catch (...) {
//		LOGD("nativeStop caught unknown exception");
//		jclass je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je,
//				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
//	}
//	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeStop exit");
//}
//
///*
// * Class:     edu_thu_xface_libs_XFaceLibrary
// * Method:    nativeSetFaceSize
// * Signature: (JI)V
// */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeSetFaceSize(JNIEnv * jenv,
//		jclass, jlong thiz, jint faceSize) {
//	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeSetFaceSize enter");
//	try {
//		if (faceSize > 0) {
//			DetectionBasedTracker::Parameters DetectorParams =
//					((DetectionBasedTracker*) thiz)->getParameters();
//			DetectorParams.minObjectSize = faceSize;
//			((DetectionBasedTracker*) thiz)->setParameters(DetectorParams);
//		}
//	} catch (cv::Exception& e) {
//		LOGD("nativeStop caught cv::Exception: %s", e.what());
//		jclass je = jenv->FindClass("org/opencv/core/CvException");
//		if (!je)
//			je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je, e.what());
//	} catch (...) {
//		LOGD("nativeSetFaceSize caught unknown exception");
//		jclass je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je,
//				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
//	}
//	LOGD( "Java_edu_thu_xface_libs_XFaceLibrary_nativeSetFaceSize exit");
//}
//
///*
// * Class:     edu_thu_xface_libs_XFaceLibrary
// * Method:    nativeDetect
// * Signature: (JJJ)V
// */JNIEXPORT void JNICALL Java_edu_thu_xface_libs_XFaceLibrary_nativeFacedetect(JNIEnv * jenv, jclass,
//		jlong thiz, jlong imageGray, jlong faces) {
//	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeDetect enter");
//	try {
////		Mat sample = *((Mat*) imageGray);
////		flip(sample.t(), sample, 0);//counter-clock wise 90
//		vector<Rect> RectFaces;
//		((DetectionBasedTracker*) thiz)->process(*((Mat*) imageGray));
//		((DetectionBasedTracker*) thiz)->getObjects(RectFaces);
//		vector_Rect_to_Mat(RectFaces, *((Mat*) faces));
////		flip(sample.t(), sample, 1);
//	} catch (cv::Exception& e) {
//		LOGD("nativeCreateObject caught cv::Exception: %s", e.what());
//		jclass je = jenv->FindClass("org/opencv/core/CvException");
//		if (!je)
//			je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je, e.what());
//	} catch (...) {
//		LOGD("nativeDetect caught unknown exception");
//		jclass je = jenv->FindClass("java/lang/Exception");
//		jenv->ThrowNew(je,
//				"Unknown exception in JNI code {highgui::VideoCapture_n_1VideoCapture__()}");
//	}
//	LOGD("Java_edu_thu_xface_libs_XFaceLibrary_nativeDetect exit");
//}

