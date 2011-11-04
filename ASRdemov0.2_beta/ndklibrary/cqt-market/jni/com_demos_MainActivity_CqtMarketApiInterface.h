/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_demos_MainActivity_CqtMarketApiInterface */

#ifndef _Included_com_demos_MainActivity_CqtMarketApiInterface
#define _Included_com_demos_MainActivity_CqtMarketApiInterface
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_CQT_API_Init
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1CQT_1API_1Init
  (JNIEnv *, jobject);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_CQT_API_Uninit
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1CQT_1API_1Uninit
  (JNIEnv *, jobject);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_Class_GetCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1Class_1GetCount
  (JNIEnv *, jobject);

JNIEXPORT jstring JNICALL Java_com_demos_cqtmarketapi_JS_1Class_1GetID
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_Class_GetName
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1Class_1GetName
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_Class_GetImg
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1Class_1GetImg
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_AP_GetName
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1AP_1GetName
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_AP_GetCompany
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1AP_1GetCompany
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_AP_GetImg
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1AP_1GetImg
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_AP_GetStar
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1AP_1GetStar
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_AP_GetPrice
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1AP_1GetPrice
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_AP_GetDescribe
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1AP_1GetDescribe
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_AP_GetVersion
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1AP_1GetVersion
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_AP_GetDownloadCount
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1AP_1GetDownloadCount
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_GetRecommendApCount
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1GetRecommendApCount
  (JNIEnv *, jobject);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_GetRecommendAppID
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1GetRecommendAppID
  (JNIEnv *, jobject);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_CLASS_AP_GetCount
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1CLASS_1AP_1GetCount
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_MainActivity_CqtMarketApiInterface
 * Method:    JS_CLASS_AP_GetAppID
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_demos_MainActivity_00024CqtMarketApiInterface_JS_1CLASS_1AP_1GetAppID
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_demos_cqtmarketapi
 * Method:    JS_InstallPackage
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_cqtmarketapi_JS_1InstallPackage
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_cqtmarketapi
 * Method:    JS_UnintallPackage
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_cqtmarketapi_JS_1UnintallPackage
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_cqtmarketapi
 * Method:    JS_IsInstallPackage
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_cqtmarketapi_JS_1IsInstallPackage
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_cqtmarketapi
 * Method:    JS_IsUpdatePackage
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_cqtmarketapi_JS_1IsUpdatePackage
  (JNIEnv *, jobject, jstring);

/*
 * Class:     com_demos_cqtmarketapi
 * Method:    JS_UpdatePackage
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_demos_cqtmarketapi_JS_1UpdatePackage
  (JNIEnv *, jobject, jstring);
#ifdef __cplusplus
}
#endif
#endif
