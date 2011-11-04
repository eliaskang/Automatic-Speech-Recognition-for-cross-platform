# A simple test for the minimal standard C++ library
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := testsvox
LOCAL_SRC_FILES := new_svox_engine.cpp
LOCAL_CPPFLAGS := -DSVOX_BUILD_FOR_Linux -DNDEBUG
LOCAL_CPPFLAGS += -DNDEBUG
LOCAL_C_INCLUDES := $(LOCAL_PATH)/include $(LOCAL_PATH)/include/media $(LOCAL_PATH)/include/Recognizer $(LOCAL_PATH)/include/PAL $(LOCAL_PATH)/include/Common $(LOCAL_PATH)/include/Prompter $(LOCAL_PATH)/include/Climax
#LOCAL_STATIC_LIBRARIES := $(LOCAL_PATH)/Android-ndk-r4b-arm-eabi-Release/libRecognizer.a
#LOCAL_STATIC_LIBRARIES += $(LOCAL_PATH)/Android-ndk-r4b-arm-eabi-Release/libRecognizer.a
#LOCAL_STATIC_LIBRARIES += $(LOCAL_PATH)/Android-ndk-r4b-arm-eabi-Release/libCommon.a
#LOCAL_STATIC_LIBRARIES += $(LOCAL_PATH)/Android-ndk-r4b-arm-eabi-Release/libClimax.a
#LOCAL_STATIC_LIBRARIES += $(LOCAL_PATH)/Android-ndk-r4b-arm-eabi-Release/libvsr.a
#LOCAL_STATIC_LIBRARIES += $(LOCAL_PATH)/Android-ndk-r4b-arm-eabi-Release/libPAL.a
#LOCAL_STATIC_LIBRARIES += $(LOCAL_PATH)/Android-ndk-r4b-arm-eabi-Release/libsvoxsll.a
LOCAL_LDLIBS :=-L$(LOCAL_PATH)/lib -lmedia -lutils -L$(LOCAL_PATH)/Android-ndk-r4b-arm-eabi-Release -lRecognizer -lPrompter -lClimax -lCommon -lvsr -lPAL -llog -lAudio -lsvoxsll
#include $(BUILD_EXECUTABLE)
include $(BUILD_SHARED_LIBRARY)
