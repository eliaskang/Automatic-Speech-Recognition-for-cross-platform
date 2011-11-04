/*** Prompter Demo Application *******************************************************************/

#ifndef __MARKET_PLATFORM_H__
#define __MARKET_PLATFORM_H__
#include "market_typedef.h"

#ifndef PLATFORM_ANDROID_JNI
typedef JavaVM void;
typedef JNIEnv void;
#endif
void MY_SAPI_DB_PRINTF(char* fmt,...);

typedef struct __tagGlobalParam
{
	char szName[MARKET_STRING_MIN];
	JavaVM *jvm;
	JNIEnv *env;

	//接口函数
	PFNvoidFunction			pfnRefreshUI;
	PFNGetURLFileFromAMS	pfnGetURLFileFromAMS;
	PFNGetURLStringFromAMS	pfnGetURLStringFromAMS;

	PFNParseClassXML		pfnParseClassXML;
	PFNGetClassCount		pfnGetClassCount;
	PFNGetClassNodeName		pfnGetClassNodeName;

	PFNParseClassXML		pfnParseAppXML;
	PFNGetClassCount		pfnGetAppCount;
	PFNGetClassNodeName		pfnGetAppNodeName;

	PFNJNI_InstallPackage  pfnPFNJNI_InstallPackage;
	PFNJNI_uninstallApp	   pfnPFNJNI_uninstallApp;
	PFNJNI_GetPackageName  pfnPFNJNI_GetPackageName;
	PFNJNI_InitPackmanamer pfnPFNJNI_InitPackmanamer;
	PFNJNI_GetVersionInfo  pfnPFNJNI_GetVersionInfo;
}TGlobalParam,*PGlobalParam;

//基类
class MarketInterface
{
public:
	MarketInterface();
	~MarketInterface();

	//设置全局参数
	void			MAP_SetParam(PGlobalParam param);
	PGlobalParam	MAP_GetParam();	

private:
	TGlobalParam	m_tGlobalParam;
};

//////////////////////////////////////////////////////////////////////////

#endif