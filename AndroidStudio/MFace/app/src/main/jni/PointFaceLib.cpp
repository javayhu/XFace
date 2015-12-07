#include <jni.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "PointFaceLib.h"
#include "ScoreHead.h"

#include <android/log.h>

#define LOG_TAG "PointFaceLib-cpp"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

using namespace std;
using namespace cv;

JNIEXPORT jdouble JNICALL Java_mface_thu_edu_cn_mface_PointFaceLib_nativePoint
  (JNIEnv * jenv, jobject obj, jlong address, jint flag, jstring js1, jstring js2, jstring js3){

      Mat sample = *((Mat*) address);
      if(sample.empty()){
          LOGD("sample empty");
          return -3;
      }

      char cflag = '1';
      if(flag==0)   cflag='0';

      const char* cs1 = jenv->GetStringUTFChars(js1, NULL);
      //string s1(cs1);
      LOGD("native cs1: %s", cs1);

      const char* cs2 = jenv->GetStringUTFChars(js2, NULL);
      //string s2(cs2);

      const char* cs3 = jenv->GetStringUTFChars(js3, NULL);
      //string s3(cs3);

      //call your function here!!!!!! ->double MainFunction(Mat &img,char flag)
      return MainFunction(sample,cflag,cs1,cs2,cs3);

      //return 88.2;
  }
