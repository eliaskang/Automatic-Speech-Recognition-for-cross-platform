/*** Prompter Demo Application *******************************************************************/

#ifndef __MARKET_TYPEDEF_H_
#define __MARKET_TYPEDEF_H_

//平台定义
//#define PLATFORM_WINCE_EVC
#define PLATFORM_ANDROID_JNI

#ifdef PLATFORM_ANDROID_JNI
#include <string.h>
#include <jni.h>
#include <android/log.h>
//#include <utils/IPCThreadState.h>
//#include <utils/ProcessState.h>
//#include "android_runtime/AndroidRuntime.h"
//using namespace android;
#endif

#define MARKET_STRING_MIN		64
#define MARKET_STRING_NORMAL	128
#define MARKET_STRING_LARGER	256
#define MARKET_STRING_HUGE		512

//////////////////////////////////////////////////////////////////////////
//
typedef void (*PFNvoidFunction)(JNIEnv *env);

//HTTP AMS下载到文件的接口
typedef bool (*PFNGetURLFileFromAMS)(JNIEnv *env,const char* lpURL, const char* lpFile);
typedef char* (*PFNGetURLStringFromAMS)(JNIEnv *env,const char* lpURL);//返回结果需要销毁内存,

//解析XML的接口
typedef bool (*PFNParseClassXML)(JNIEnv *env,const char* lpFile);
typedef int (*PFNGetClassCount)(JNIEnv *env);
typedef char* (*PFNGetClassNodeName)(JNIEnv *env,int idx,const char* lpNodeName);

//安装管理接口
typedef bool (*PFNJNI_InstallPackage)(JNIEnv *env, char* lpPackageFile);
typedef void (*PFNJNI_uninstallApp)(JNIEnv *env, char* lpPackage);
typedef char* (*PFNJNI_GetPackageName)(JNIEnv *env, char* lpPackageFile);
typedef bool (*PFNJNI_InitPackmanamer)(JNIEnv *env);
typedef char* (*PFNJNI_GetVersionInfo)(JNIEnv *env,char* lpPackage);


#endif 