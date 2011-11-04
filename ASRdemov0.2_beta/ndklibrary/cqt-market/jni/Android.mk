# A simple test for the minimal standard C++ library
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := localwebviewjni_download
LOCAL_SRC_FILES := com_demos_MainActivity_CqtMarketApiInterface.cpp
LOCAL_SRC_FILES += market_local_data.cpp
LOCAL_SRC_FILES += market_packagemanager.cpp
LOCAL_SRC_FILES += market_platform.cpp
LOCAL_SRC_FILES += market_readams.cpp
LOCAL_SRC_FILES += market_service.cpp
LOCAL_LDLIBS :=-llog
#include $(BUILD_EXECUTABLE)
include $(BUILD_SHARED_LIBRARY)
