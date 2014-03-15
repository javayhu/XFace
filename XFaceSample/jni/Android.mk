LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#opencv
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk

#C++
LOCAL_SRC_FILES := xface.cpp
#LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog

LOCAL_MODULE    := xface

include $(BUILD_SHARED_LIBRARY)


#include $(CLEAR_VARS)
#LOCAL_SRC_FILES  := DetectionBasedTracker_jni.cpp
#LOCAL_C_INCLUDES += $(LOCAL_PATH)
#LOCAL_LDLIBS     += -llog -ldl
#LOCAL_MODULE     := detection_based_tracker
#include $(BUILD_SHARED_LIBRARY)
