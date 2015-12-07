LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

#opencv
OPENCV_CAMERA_MODULES:=on
OPENCV_INSTALL_MODULES:=on
OPENCV_LIB_TYPE:=SHARED
include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk

#C++
LOCAL_SRC_FILES := helper.cpp \
eigenfaces.cpp \
xface.cpp

#LOCAL_C_INCLUDES += $(LOCAL_PATH)
LOCAL_LDLIBS     += -llog

LOCAL_MODULE    := xface

include $(BUILD_SHARED_LIBRARY)

#include $(CLEAR_VARS)
##opencv !!!! pay attention here! the second module uses the opencv libs too! build again!
##otherwise, the source file will not find the opencv header files
#OPENCV_CAMERA_MODULES:=on
#OPENCV_INSTALL_MODULES:=on
#OPENCV_LIB_TYPE:=SHARED
#include ${OPENCVROOT}/sdk/native/jni/OpenCV.mk
#
#LOCAL_SRC_FILES  := facetracker.cpp
#LOCAL_C_INCLUDES += $(LOCAL_PATH)
#LOCAL_LDLIBS     += -llog -ldl
#LOCAL_MODULE     := facetracker
#include $(BUILD_SHARED_LIBRARY)
