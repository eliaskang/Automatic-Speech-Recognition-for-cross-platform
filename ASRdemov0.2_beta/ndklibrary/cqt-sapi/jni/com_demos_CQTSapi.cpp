/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
#include "com_demos_CQTSapi.h"
#include "CQTSapi.h"
#include "CQTSapi_java.h"
#include "SAPItypes.h"

jclass     g_mClass;					// Reference to JTX07class
jobject    g_mObject;
JavaVM*	   gJavaVM;

jmethodID  g_id_callback_SAPI_DebugOutInfo;
jmethodID  g_id_callback_SAPI_ASRResult;
static char* jstringTostring(JNIEnv *env, jstring jstr)//需要释放内存
{
	char* rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr= (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0)
	{
		rtn = (char*)new char[alen + 1];
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}


//////////////////////////////////////////////////////////////////////////
//回调调试信息
bool pfncallback_SAPI_DebugOutInfo(JNIEnv *env,char* lpInfo)
{
	SAPI_DB_PRINTF("pfncallback_SAPI_DebugOutInfo--1,lpInfo=%s",lpInfo);
	jstring jInfo = env->NewStringUTF(lpInfo);
	SAPI_DB_PRINTF("pfncallback_SAPI_DebugOutInfo--2");

	env->CallVoidMethod(g_mObject, g_id_callback_SAPI_DebugOutInfo, jInfo);
	SAPI_DB_PRINTF("pfncallback_SAPI_DebugOutInfo--over");
	return true;
}

bool pfncallback_SAPI_ASRResult(JNIEnv *env,char* lpASR)
{
	SAPI_DB_PRINTF("pfncallback_SAPI_DebugOutInfo--1,lpInfo=%s",lpASR);
	jstring jASR = env->NewStringUTF(lpASR);
	SAPI_DB_PRINTF("pfncallback_SAPI_DebugOutInfo--2");
	
	env->CallVoidMethod(g_mObject, g_id_callback_SAPI_ASRResult, jASR);
	SAPI_DB_PRINTF("pfncallback_SAPI_DebugOutInfo--over");
	return true;
}

/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_Init
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
static CQTSapi_Service* g_pSAPIService = 0;
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1JS_SAPI_Init
  (JNIEnv *env, jobject obj, jstring jstrResourcePath, jstring jstrConfigName)
{	
	//外部有反复初始化的情况，这里就不要再初始化了。
	if(g_pSAPIService)
		return 0;

	//全局变量初始化
	jclass clazz =env->GetObjectClass(obj);	
	g_mClass =(jclass)env->NewGlobalRef(clazz);
	g_mObject = env->NewGlobalRef(obj);
	g_id_callback_SAPI_DebugOutInfo = env->GetMethodID(g_mClass, "callback_SAPI_DebugOutInfo", "(Ljava/lang/String;)V");
	g_id_callback_SAPI_ASRResult = env->GetMethodID(g_mClass, "callback_SAPI_ASRResult", "(Ljava/lang/String;)V");

	g_pSAPIService = new CQTSapi_Service;
	if(!g_pSAPIService)
	{
		return 1;
	}
	g_pSAPIService->SetCallBack(pfncallback_SAPI_DebugOutInfo,pfncallback_SAPI_ASRResult);

	//需要释放内存
	char* lpResourcePath = jstringTostring(env, jstrResourcePath);
	if(!lpResourcePath)
	{		
		return 2;
	}
	char* lpConfigName = jstringTostring(env, jstrConfigName);
	if(!lpConfigName)
	{
		delete[] lpResourcePath;
		return 3;
	}

	//////////////////////////////////////////////////////////////////////////
	CQtObject* thisobj = new CQtObject(0);
	thisobj->jvm = gJavaVM;	
	bool bRet = g_pSAPIService->CSS_Init(lpResourcePath, lpConfigName, thisobj);
	delete[] lpResourcePath;
	delete[] lpConfigName;

	if(bRet)
	{
		return 0;
	}	
	return 4;
}


/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_ASRClear
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1ASRClear
  (JNIEnv *env, jobject obj, jstring jVar)
{
	 char* lpVar = jstringTostring(env, jVar);
	 if(!lpVar)
	 {
		 return 1;
	 }

	 g_pSAPIService->CSS_ASRClear(lpVar);

	 delete lpVar;
	 return 0;
}


/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_SetASRRuleValue
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1SetASRRuleValue
  (JNIEnv *env, jobject obj, jstring jVar, jstring jValue)
{
	char* lpVar = jstringTostring(env, jVar);
	if(!lpVar)
	{
		return 1;
	}
	char* lpValue = jstringTostring(env, jValue);
	if(!lpValue)
	{
		delete lpVar;
		return 2;
	}
	
	g_pSAPIService->CSS_SetASRRuleValue(lpVar,lpValue);
	
	delete lpVar;
	delete lpValue;
	return 0;
}

/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_TTSClear
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1TTSClear
  (JNIEnv *env, jobject obj, jstring jVar)
{
	char* lpVar = jstringTostring(env, jVar);
	if(!lpVar)
	{
		return 1;
	}
	
	g_pSAPIService->CSS_ASRClear(lpVar);
	
	delete lpVar;
	return 0;
}

/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_SetTTSRuleValue
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1SetTTSRuleValue
  (JNIEnv *env, jobject obj, jstring jVar, jstring jValue)
{
	char* lpVar = jstringTostring(env, jVar);
	if(!lpVar)
	{
		return 1;
	}
	char* lpValue = jstringTostring(env, jValue);
	if(!lpValue)
	{
		delete lpVar;
		return 2;
	}
	
	g_pSAPIService->CSS_SetTTSRuleValue(lpVar,lpValue);
	
	delete lpVar;
	delete lpValue;
	return 0;
}

/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_Stop
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1Stop
  (JNIEnv *env, jobject obj)
{
	g_pSAPIService->CSS_GetSapi()->Sapi_Stop();
	return 0;
}

/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_Hi_Start
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1Hi_1Start
  (JNIEnv *env, jobject obj)
{
	g_pSAPIService->CSS_GetSapi()->Sapi_Hi_Start();
	return 0;
}

/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_Lo_ASRRule
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1Lo_1ASRRule
  (JNIEnv *env, jobject obj, jstring jRule)
{
	char* lpRule = jstringTostring(env, jRule);
	if(!lpRule)
	{
		return 1;
	}
	
	g_pSAPIService->CSS_GetSapi()->Sapi_Lo_ASRRule(lpRule);
	
	delete lpRule;
	return 0;
}

/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_Lo_TTSRule
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1Lo_1TTSRule
  (JNIEnv *env, jobject obj, jstring jRule)
{
	char* lpRule = jstringTostring(env, jRule);
	if(!lpRule)
	{
		return 1;
	}
	
	g_pSAPIService->CSS_GetSapi()->Sapi_Lo_TTSRule(lpRule);
	
	delete lpRule;
	return 0;
}

/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_Lo_VoiceRule
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1Lo_1VoiceRule
  (JNIEnv *env, jobject obj, jstring jRule)
{
	char* lpRule = jstringTostring(env, jRule);
	if(!lpRule)
	{
		return 1;
	}
	
	g_pSAPIService->CSS_GetSapi()->Sapi_Lo_VoiceRule(lpRule);
	
	delete lpRule;
	return 0;
}

/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_Lo_TTSText
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1Lo_1TTSText
  (JNIEnv *env, jobject obj, jstring jText)
{
	char* lpText = jstringTostring(env, jText);
	if(!lpText)
	{
		return 1;
	}
	
	g_pSAPIService->CSS_GetSapi()->Sapi_Lo_TTSText(lpText,"en_US");
	
	delete lpText;
	return 0;
}

/*
 * Class:     com_demos_CQTSapi
 * Method:    JS_SAPI_Lo_VoiceFile
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_CQTSapi_JS_1SAPI_1Lo_1VoiceFile
  (JNIEnv *env, jobject obj, jstring jFile)
{
	char* lpFile = jstringTostring(env, jFile);
	if(!lpFile)
	{
		return 1;
	}
	
	g_pSAPIService->CSS_GetSapi()->Sapi_Lo_VoiceFile(lpFile);
	
	delete lpFile;
	return 0;
}


static JNINativeMethod methods[] = {
	{"JS_SAPI_Init","(Ljava/lang/String;Ljava/lang/String;)I",(void*)Java_com_demos_CQTSapi_JS_1SAPI_1JS_SAPI_Init},
	{"JS_SAPI_ASRClear","(Ljava/lang/String;)I",(void*)Java_com_demos_CQTSapi_JS_1SAPI_1ASRClear},
	{"JS_SAPI_SetASRRuleValue","(Ljava/lang/String;Ljava/lang/String;)I",(void *)Java_com_demos_CQTSapi_JS_1SAPI_1SetASRRuleValue},
	{"JS_SAPI_TTSClear","(Ljava/lang/String;)I",(void *)Java_com_demos_CQTSapi_JS_1SAPI_1TTSClear},
	{"JS_SAPI_SetTTSRuleValue","(Ljava/lang/String;Ljava/lang/String;)I",(void *)Java_com_demos_CQTSapi_JS_1SAPI_1SetTTSRuleValue},
	{"JS_SAPI_Stop","()I",(void *)Java_com_demos_CQTSapi_JS_1SAPI_1Stop},
	{"JS_SAPI_Hi_Start","()I",(void *)Java_com_demos_CQTSapi_JS_1SAPI_1Hi_1Start},
	{"JS_SAPI_Lo_ASRRule","(Ljava/lang/String;)I",(void *)Java_com_demos_CQTSapi_JS_1SAPI_1Lo_1ASRRule},
	{"JS_SAPI_Lo_TTSRule","(Ljava/lang/String;)I",(void *)Java_com_demos_CQTSapi_JS_1SAPI_1Lo_1TTSRule},
	{"JS_SAPI_Lo_VoiceRule","(Ljava/lang/String;)I",(void *)Java_com_demos_CQTSapi_JS_1SAPI_1Lo_1VoiceRule},
	{"JS_SAPI_Lo_TTSText","(Ljava/lang/String;)I",(void *)Java_com_demos_CQTSapi_JS_1SAPI_1Lo_1TTSText},
	{"JS_SAPI_Lo_VoiceFile","(Ljava/lang/String;)I",(void *)Java_com_demos_CQTSapi_JS_1SAPI_1Lo_1VoiceFile}
};


/*

  * Registerseveral native methods for one class.
  
*/
static int registerNativeMethods(JNIEnv* env, JNINativeMethod* gMethods, int numMethods)								 
{
    jclass clazz;	
    clazz =env->FindClass("com/demos/CQTSapi");
    if (clazz ==NULL) 
	{		
	//	SAPI_DB_PRINTF("Native registration unable to find class '%s'",className);		
        return JNI_FALSE;		
    }
	
    if(env->RegisterNatives(clazz, gMethods, numMethods) < 0) 
	{		
	//	SAPI_DB_PRINTF("RegisterNatives failed for '%s'", className);		
        return JNI_FALSE;		
    }	
	
    return JNI_TRUE;	
}

static int registerNatives(JNIEnv* env)
{	
	if(!registerNativeMethods(env, methods, sizeof(methods) / sizeof(methods[0]))) 
	{		
		return JNI_FALSE;		
	}	
	
	return JNI_TRUE;	
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{	
	JNIEnv *env;	
	gJavaVM = vm;	
	int result;
	
	SAPI_DB_PRINTF("JNI_OnLoad called");	
	if (vm->GetEnv((void**) &env, JNI_VERSION_1_4)!= JNI_OK) 
	{		
		SAPI_DB_PRINTF("Failed to get the environment using GetEnv()");		
		return -1;
	}
	
	
	if (registerNatives(env) != JNI_TRUE)
	{		
		SAPI_DB_PRINTF("ERROR: registerNatives failed");
        goto bail;		
    }	
    result =JNI_VERSION_1_4;	
	
bail:	
    return result;
	
}

#if 1
void JNI_OnUnload(JavaVM* vm, void* reserved)
{
	SAPI_DB_PRINTF("JNI_OnUnload---------------------------------------------------1");
	SAPI_DB_PRINTF("JNI_OnUnload---------------------------------------------------1");
	SAPI_DB_PRINTF("JNI_OnUnload---------------------------------------------------1");
	SAPI_DB_PRINTF("JNI_OnUnload---------------------------------------------------over");
	if(g_pSAPIService)
		delete g_pSAPIService;
	g_pSAPIService = 0;
}
#endif