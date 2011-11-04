# A simple test for the minimal standard C++ library
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := cqtsapi_ndk
LOCAL_CPPFLAGS := -DPLATFROM_SAPI_ANDROID_NDK
LOCAL_SRC_FILES := CQTSapi.cpp
LOCAL_SRC_FILES += SAPIConfig.cpp
LOCAL_SRC_FILES += SAPIEngine.cpp
LOCAL_SRC_FILES += SAPIPlatform.cpp
LOCAL_SRC_FILES += SvoxEngine.cpp
LOCAL_SRC_FILES += com_demos_CQTSapi.cpp
LOCAL_SRC_FILES += CQTSapi_java.cpp
LOCAL_LDLIBS :=-L$(LOCAL_PATH)/lib -ltestsvox -lmedia -lutils -llog
#include $(BUILD_EXECUTABLE)
include $(BUILD_SHARED_LIBRARY)
