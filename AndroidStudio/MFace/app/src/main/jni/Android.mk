LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#opencv
OPENCVROOT:= /Users/hujiawei/Android/opencv_sdk
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk

#hujiawei add your source files to compile, no header files
LOCAL_SRC_FILES  := PointFaceLib.cpp \
Malloc2.cpp \
Matrix.cpp \
dist.cpp \
dist2.cpp \
mean.cpp \
min.cpp \
CN_feature.cpp \
CN_feature_extraction.cpp \
giveScore.cpp \
KMeans.cpp \
MainFunction.cpp

LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog -ldl

LOCAL_MODULE     := pointface

include $(BUILD_SHARED_LIBRARY)
