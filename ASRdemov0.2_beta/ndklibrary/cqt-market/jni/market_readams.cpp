
#include "market_readams.h"
#include <stdlib.h>
#include <stdio.h>

CQTMarket_ReadAMS::CQTMarket_ReadAMS(PGlobalParam param)
{
	MAP_SetParam(param);
	m_pCqtLocalData = 0;
}

CQTMarket_ReadAMS::~CQTMarket_ReadAMS()
{
}

void CQTMarket_ReadAMS::AMS_SetLocalHandle(CQTLocalAppData* pLocalData)
{
	m_pCqtLocalData = pLocalData;
}

#include <pthread.h>
pthread_t AMSthread;
JavaVM*  g_jvm = 0;
void* ReadAMSRun(void* param)
{
	int status;
    JNIEnv *env;
    bool isAttached = false;

	MY_SAPI_DB_PRINTF("0000000000000000000000000000000000000000000000000ReadAMSRun--1");
	CQTMarket_ReadAMS* pThis = (CQTMarket_ReadAMS*)param;	
	status =g_jvm->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status< 0) 
	{		
		MY_SAPI_DB_PRINTF("callback_handler: failed to get JNI environment assuming native thread");		
        status =g_jvm->AttachCurrentThread(&env, NULL);
		
        if(status< 0)
		{
			MY_SAPI_DB_PRINTF("callback_handler: failed to attach current thread");
			return NULL;
        }
        isAttached = true;
    }
	
	pThis->DoRunService(env);

	if(isAttached)
	{
		MY_SAPI_DB_PRINTF("0000000000000000000000000000000000000000000000000ReadAMSRun--2");
		g_jvm->DetachCurrentThread();
	}

	return 0;
}

#define AMS_URL_CLASS_DOWNLOAD			"http://172.30.92.249:8081/ivas/market/getCategories.do?interface=cqt-api0001"
#define AMS_URL_CLASS_XML_FILE			"/mnt/sdcard/appclass.xml"
#define AMS_CLASS_NODENAME_ID			"id"
#define AMS_CLASS_NODENAME_NAME			"name"
#define AMS_CLASS_NODENAME_APPCOUNT		"app_count"
#define AMS_CLASS_NODENAME_ICON			"icon"
void CQTMarket_ReadAMS::GetAppClass(JNIEnv *env)
{
	PGlobalParam thisparam = MAP_GetParam();

	//从URL读取内容
	MY_SAPI_DB_PRINTF("pfnGetURLFileFromAMSRead Start");
	bool bResult = thisparam->pfnGetURLFileFromAMS(env,AMS_URL_CLASS_DOWNLOAD,AMS_URL_CLASS_XML_FILE);
	if(!bResult)
	{
		MY_SAPI_DB_PRINTF("GetAppClass Read AMS Data ERROR!");
		return ;
	}
	MY_SAPI_DB_PRINTF("pfnGetURLFileFromAMS Read end");

	//从内容文件
	bResult = thisparam->pfnParseClassXML(env,AMS_URL_CLASS_XML_FILE);
	if(!bResult)
	{
		MY_SAPI_DB_PRINTF("pfnParseClassXML Parse Class XML Data ERROR!");
		return ;
	}

	//添加类
	int nCount = thisparam->pfnGetClassCount(env);
	MY_SAPI_DB_PRINTF("pfnGetClassCount Read AMS class count=%d!",nCount);
	for(int i = 0; i < nCount; i++)
	{
		TAppClassData tClass;
		memset(&tClass,0,sizeof(TAppClassData));
		char* lpID = thisparam->pfnGetClassNodeName(env, i, AMS_CLASS_NODENAME_ID);
		if(lpID)
		{
			strncpy(tClass.szClassID,lpID,MARKET_STRING_LARGER-1);
		}else
		{
			MY_SAPI_DB_PRINTF("pfnGetClassNodeName Get Class Node Name ID ERROR!");
			continue;
		}
		MY_SAPI_DB_PRINTF("pfnGetClassNodeName lpID=%s",lpID);

		char* lpName = thisparam->pfnGetClassNodeName(env, i, AMS_CLASS_NODENAME_NAME);
		if(lpName)
		{
			strncpy(tClass.szClassName,lpName,MARKET_STRING_LARGER-1);
		}else
		{
			MY_SAPI_DB_PRINTF("pfnGetClassNodeName Get Class Node Name NAME ERROR!");
			continue;
		}
		MY_SAPI_DB_PRINTF("pfnGetClassNodeName lpName=%s",lpName);

		char* lpIcon = thisparam->pfnGetClassNodeName(env, i, AMS_CLASS_NODENAME_ICON);
		if(lpIcon)
		{
			strncpy(tClass.szClassImgPath,lpIcon,MARKET_STRING_LARGER-1);
		}else
		{
			MY_SAPI_DB_PRINTF("pfnGetClassNodeName Get Class Node Name icon ERROR!");
			continue;
		}
		MY_SAPI_DB_PRINTF("pfnGetClassNodeName lpIcon=%s",lpIcon);

		int idxadd = m_pCqtLocalData->CA_AddClassItem(&tClass);
		if(idxadd < 0)
		{
			MY_SAPI_DB_PRINTF("CA_AddClassItem Add Class Item Data ERROR!");
		}		
	}
}

void CQTMarket_ReadAMS::GetRecommend(JNIEnv *env)
{
	PGlobalParam thisparam = MAP_GetParam();
}

#define APP_DOWNLOAD_LIST_URL	"http://172.30.92.249:8081/ivas/market/getAppsByCategoryID.do?interface=cqt-api0002&categoryID="
#define AMS_URL_APP_XML_FILE	"/mnt/sdcard/applist.xml"
void CQTMarket_ReadAMS::GetAppData(JNIEnv *env)
{
	char szDownloadURL[MARKET_STRING_HUGE];
	PGlobalParam thisparam = MAP_GetParam();

	int nClassCount = m_pCqtLocalData->CA_GetClassCount();
	int i = 0;
	for( i = 0; i < nClassCount; i++)
	{
		PAppClassData pClass = m_pCqtLocalData->CA_GetClassItem(i);
		sprintf(szDownloadURL,"%s%s",APP_DOWNLOAD_LIST_URL,pClass->szClassID);
		MY_SAPI_DB_PRINTF("GetAppData szDownloadURL=%s!",szDownloadURL);
		bool bResult = thisparam->pfnGetURLFileFromAMS(env,szDownloadURL,AMS_URL_APP_XML_FILE);
		if(!bResult)
		{
			MY_SAPI_DB_PRINTF("pfnGetURLFileFromAMS Read AMS Data ERROR!");
			return ;
		}
		
		//从内容文件
		bResult = thisparam->pfnParseAppXML(env,AMS_URL_APP_XML_FILE);
		if(!bResult)
		{
			MY_SAPI_DB_PRINTF("pfnParseAppXML Parse Class XML Data ERROR!");
			return ;
		}
		
		//添加类
		int nCount = thisparam->pfnGetAppCount(env);
		MY_SAPI_DB_PRINTF("pfnGetAppCount Read AMS class count=%d!",nCount);
		int k = 0;
		for(k = 0; k < nCount; k++)
		{
			TCqtApp tApp;
			memset(&tApp,0,sizeof(TCqtApp));
			strcpy(tApp.tAppBaseInfo.szClassID,pClass->szClassID);

			/////////
			char* lpID = thisparam->pfnGetAppNodeName(env, k, "id");
			if(lpID)
			{
				strncpy(tApp.tAppBaseInfo.szAppID,lpID, MARKET_STRING_LARGER-1);
			}else
			{
				MY_SAPI_DB_PRINTF("pfnGetAppNodeName Get Class Node Name ID ERROR!");
				continue;
			}
			MY_SAPI_DB_PRINTF("pfnGetAppNodeName lpID=%s",lpID);
			
			/////////
			char* lpName = thisparam->pfnGetAppNodeName(env, k, "name");
			if(lpName)
			{
				strncpy(tApp.tAppBaseInfo.szName, lpName, MARKET_STRING_LARGER-1);
			}else
			{
				MY_SAPI_DB_PRINTF("pfnGetAppNodeName Get Class Node Name NAME ERROR!");
				continue;
			}
			MY_SAPI_DB_PRINTF("pfnGetAppNodeName lpName=%s",lpName);
			
			////////
			char* lp = thisparam->pfnGetAppNodeName(env, k, "star");
			if(lp)
			{
				tApp.tAppBaseInfo.nStar = atoi(lp);				
			}else
			{
				MY_SAPI_DB_PRINTF("pfnGetAppNodeName Get Class Node Name icon ERROR!");
				continue;
			}
			MY_SAPI_DB_PRINTF("pfnGetAppNodeName lp=%s",lp);

			////////
			lp = thisparam->pfnGetAppNodeName(env, k, "price");
			if(lp)
			{
				strncpy(tApp.tAppBaseInfo.szPrice, lp, MARKET_STRING_LARGER-1);			
			}else
			{
				MY_SAPI_DB_PRINTF("pfnGetAppNodeName Get Class Node Name icon ERROR!");
				continue;
			}
			MY_SAPI_DB_PRINTF("pfnGetAppNodeName lp=%s",lp);

			////////
			lp = thisparam->pfnGetAppNodeName(env, k, "logo_url");
			if(lp)
			{
				strncpy(tApp.tAppBaseInfo.szIcon, lp, MARKET_STRING_LARGER-1);
			}else
			{
				MY_SAPI_DB_PRINTF("pfnGetAppNodeName Get Class Node Name icon ERROR!");
				continue;
			}
			MY_SAPI_DB_PRINTF("pfnGetAppNodeName lp=%s",lp);

			////////
			lp = thisparam->pfnGetAppNodeName(env, k, "description");
			if(lp)
			{
				strncpy(tApp.tAppBaseInfo.szDescribe, lp, MARKET_STRING_LARGER-1);
			}else
			{
				MY_SAPI_DB_PRINTF("pfnGetAppNodeName Get Class Node Name icon ERROR!");
				continue;
			}
			MY_SAPI_DB_PRINTF("pfnGetAppNodeName lp=%s",lp);

			/////////
			if(!m_pCqtLocalData->CA_AddAppItem(&tApp))
			{
				MY_SAPI_DB_PRINTF("CA_AddAppItem Add Class Item Data ERROR!");
			}		
		}		
	}
}

void CQTMarket_ReadAMS::DoRunService(JNIEnv *env)
{
	PGlobalParam thisparam = MAP_GetParam();

	GetAppClass(env);
	GetRecommend(env);
	GetAppData(env);

	thisparam->pfnRefreshUI(env);
}

void CQTMarket_ReadAMS::AMS_StartService()
{
	PGlobalParam thisparam = MAP_GetParam();
	g_jvm = thisparam->jvm;
	MY_SAPI_DB_PRINTF("0000000000000000000000000000000000000000000000000AMS_StartService--1");
	int rr =pthread_create(&AMSthread, NULL, ReadAMSRun, this);
	//DoRunService(thisparam->env);
	MY_SAPI_DB_PRINTF("0000000000000000000000000000000000000000000000000AMS_StartService--2");
}

