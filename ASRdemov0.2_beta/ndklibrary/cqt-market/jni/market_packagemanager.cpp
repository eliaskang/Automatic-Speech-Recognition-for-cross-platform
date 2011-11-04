
#include "market_packagemanager.h"
#include <stdio.h>
#include <unistd.h>

CQTPackManager::CQTPackManager(PGlobalParam param)
{
	m_pcqtLocalAppData = 0;
	MAP_SetParam(param);
}

CQTPackManager::~CQTPackManager()
{
}


#include <pthread.h>
pthread_t MAP_thread;
JavaVM*  g_jvmMAP = 0;
void* ScranMAPRun(void* param)
{
	int status;
    JNIEnv *env;
    bool isAttached = false;
	
	MY_SAPI_DB_PRINTF("0000000000000000000000000000000000000000000000000ScranMAPRun--Start");
	CQTPackManager* pThis = (CQTPackManager*)param;	
	status =g_jvmMAP->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status< 0) 
	{		
		MY_SAPI_DB_PRINTF("callback_handler: failed to get JNI environment assuming native thread");		
        status =g_jvmMAP->AttachCurrentThread(&env, NULL);
		
        if(status < 0)
		{
			MY_SAPI_DB_PRINTF("callback_handler: failed to attach current thread");
			return NULL;
        }
        isAttached = true;
    }
	
	pThis->CPM_ScanLocalPackInfo(env);
	
	if(isAttached)
	{
		MY_SAPI_DB_PRINTF("0000000000000000000000000000000000000000000000000ScranMAPRun--End");
		g_jvmMAP->DetachCurrentThread();
	}
	
	return 0;
}

pthread_mutex_t   CPM_Scanlock = PTHREAD_MUTEX_INITIALIZER;
bool CQTPackManager::CPM_ScanLocalPackInfo(JNIEnv *env)
{
	PGlobalParam thisparam = MAP_GetParam();

	//初始化程序管理接口	
	m_pcqtLocalAppData->CA_ReadLocal();
	MY_SAPI_DB_PRINTF("CPM_ScanLocalPackInfo-----------3");

	for(;;)
	{		
		bool bChanged = false;
		
		MY_SAPI_DB_PRINTF("CPM_ScanLocalPackInfo-----------1");
		bool bRet = thisparam->pfnPFNJNI_InitPackmanamer(env);
		if(!bRet)
		{
			MY_SAPI_DB_PRINTF("CPM_ScanLocalPackInfo--pfnPFNJNI_InitPackmanamer Error");
			return false;
		}


		pthread_mutex_lock(&CPM_Scanlock);
		//看看本地记录了多少安装接口
		int nLocalCount = m_pcqtLocalAppData->CA_GetLocalCount();
		int i = 0;
		MY_SAPI_DB_PRINTF("CPM_ScanLocalPackInfo-----------4 nLocalCount=%d",nLocalCount);
		for(i = 0; i < m_pcqtLocalAppData->CA_GetLocalCount();)
		{
			PAppInstall p = m_pcqtLocalAppData->CA_GetLocalItem(i);
			char* lpVersion = thisparam->pfnPFNJNI_GetVersionInfo(env,p->szPackageName);		
			if(!lpVersion || strlen(lpVersion) <= 0 || 0 == strcmp(lpVersion,"noversion"))//去除
			{
				if(0 != strcmp(p->szPackageVersion,"firstinstall"))
				{
					bChanged = true;
					m_pcqtLocalAppData->CA_DeleteLocalItem(i);
					m_pcqtLocalAppData->CA_SaveLocal();
					MY_SAPI_DB_PRINTF("CPM_ScanLocalPackInfo--lpVersion=%s Delete packagename = %s",lpVersion,p->szPackageName);
					continue;
				}
				
				i++;
				continue;
			}
			
			if(0 == strcmp(p->szPackageVersion,"firstinstall"))
			{
				MY_SAPI_DB_PRINTF("CPM_ScanLocalPackInfo-- firstinstall OK");
				strcpy(p->szPackageVersion,lpVersion);
				bChanged = true;
				m_pcqtLocalAppData->CA_SaveLocal();
				MY_SAPI_DB_PRINTF("CPM_ScanLocalPackInfo-- CA_SaveLocal OK");
			}
			
			MY_SAPI_DB_PRINTF("CPM_ScanLocalPackInfo--lpVersion=%s",lpVersion);		
			if(0 != strcmp(p->szPackageVersion,lpVersion))
			{
				strcpy(p->szPackageVersion,lpVersion);
				m_pcqtLocalAppData->CA_SaveLocal();	
			}
			i++;
		}		
		if(bChanged)
		{
			//m_pcqtLocalAppData->CA_SaveLocal();
			thisparam->pfnRefreshUI(env);
			MY_SAPI_DB_PRINTF("CPM_ScanLocalPackInfo--pfnRefreshUI----------s");		
		}
		pthread_mutex_unlock(&CPM_Scanlock);

		sleep(5);
	}
	return true;
}

void CQTPackManager::CPM_ScanService()
{
	PGlobalParam thisparam = MAP_GetParam();
	g_jvmMAP = thisparam->jvm;
	int rr =pthread_create(&MAP_thread, NULL, ScranMAPRun, this);	
}

pthread_t MAP_install_thread;
JavaVM*  g_jvmMAP_install = 0;
void* ScranMAPRun_Install(void* param)
{
	int status;
    JNIEnv *env;
    bool isAttached = false;

	CQTPackManager* pThis = (CQTPackManager*)param;	
	status =g_jvmMAP_install->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status< 0) 
	{		
		MY_SAPI_DB_PRINTF("callback_handler: failed to get JNI environment assuming native thread");		
        status =g_jvmMAP_install->AttachCurrentThread(&env, NULL);
		
        if(status < 0)
		{
			MY_SAPI_DB_PRINTF("callback_handler: failed to attach current thread");
			return NULL;
        }
        isAttached = true;
    }
	
	pThis->CPM_InstallPackageThread(env,pThis->m_szAppID);

	if(isAttached)
	{
		MY_SAPI_DB_PRINTF("0000000000000000000000000000000000000000000000000ScranMAPRun--End");
		g_jvmMAP_install->DetachCurrentThread();
	}
}

int	CQTPackManager::CPM_InstallPackage(char* lpAppID)
{
	PGlobalParam thisparam = MAP_GetParam();

	strcpy(m_szAppID,lpAppID);
	g_jvmMAP_install = thisparam->jvm;
	int rr = pthread_create(&MAP_install_thread, NULL, ScranMAPRun_Install, this);	
	return 0;
}

#define URL_DOWNLOAD_PACKAGE	"http://172.30.92.249:8081/ivas/market/downloadAppByID.do?applicationID="
int	CQTPackManager::CPM_InstallPackageThread(JNIEnv *env,char* lpAppID)
{
	PGlobalParam thisparam = MAP_GetParam();
	TAppInstall tApp;
	memset(&tApp,0,sizeof(TAppInstall));
	strcpy(tApp.szAppID,lpAppID);

	//下载
	char szURL[1024];
	sprintf(szURL,"%s%s",URL_DOWNLOAD_PACKAGE,lpAppID);
	char szLocal[256];
	sprintf(szLocal,"/mnt/sdcard/%s.apk",lpAppID);


	//下载包
	bool bRet = thisparam->pfnGetURLFileFromAMS(env,szURL,szLocal);
	if(!bRet)
	{
		MY_SAPI_DB_PRINTF("CPM_InstallPackage--pfnGetURLFileFromAMS ERROR");
		return 2;
	}

	//////////////////////////////////////////////////////////////////////////
	MY_SAPI_DB_PRINTF("CPM_InstallPackage--szURL=%s,LocalURL = %s",szURL,szLocal);
	char* lpInfo = thisparam->pfnPFNJNI_GetPackageName(env,szLocal);
	MY_SAPI_DB_PRINTF("CPM_InstallPackage--lpInfo=%s",lpInfo);
	if(!lpInfo || !strstr(lpInfo,"&&^^"))
	{
		MY_SAPI_DB_PRINTF("CPM_InstallPackage--szURL=%s,LocalURL = %s--ERROR",szURL,szLocal);
		return 1;
	}

	char* lpPackageName = lpInfo;
	char* lp = strstr(lpInfo,"&&^^");
	strncpy(tApp.szPackageName,lpPackageName,lp-lpPackageName);
	MY_SAPI_DB_PRINTF("CPM_InstallPackage--szPackageName=%s",tApp.szPackageName);
	lp += 4;
	//strcpy(tApp.szPackageVersion,lp);
	strcpy(tApp.szPackageVersion,"firstinstall");
	MY_SAPI_DB_PRINTF("CPM_InstallPackage--szPackageVersion=%s",tApp.szPackageVersion);

	MY_SAPI_DB_PRINTF("CPM_InstallPackage--------------------------------------------------CA_AddLocalItem Start!");
	pthread_mutex_lock(&CPM_Scanlock);
	int i = 0;	
	for(i = 0; i < m_pcqtLocalAppData->CA_GetLocalCount();)
	{
		PAppInstall p = m_pcqtLocalAppData->CA_GetLocalItem(i);
		if(0 == strcmp(p->szPackageVersion,"firstinstall"))
		{
			m_pcqtLocalAppData->CA_DeleteLocalItem(i);
			m_pcqtLocalAppData->CA_SaveLocal();
			continue;
		}
		i++;
	}
	m_pcqtLocalAppData->CA_AddLocalItem(&tApp);
	pthread_mutex_unlock(&CPM_Scanlock);
	MY_SAPI_DB_PRINTF("CPM_InstallPackage--------------------------------------------------CA_AddLocalItem End!");
	//开始安装
	thisparam->pfnPFNJNI_InstallPackage(env,szLocal);
	MY_SAPI_DB_PRINTF("CPM_InstallPackage--------------------------------------------------pfnPFNJNI_InstallPackage End!");

	//需要判断安装成功后，才能加入数据库	
	//CPM_ScanLocalPackInfo(env);
	//MY_SAPI_DB_PRINTF("CPM_InstallPackage--------------------------------------------------CPM_ScanLocalPackInfo End!");
	thisparam->pfnRefreshUI(env);
	//MY_SAPI_DB_PRINTF("CPM_InstallPackage--------------------------------------------------pfnRefreshUI End!");
	return 0;
}


#if 1
pthread_t MAP_Uninstall_thread;
JavaVM*  g_jvmMAP_Uninstall = 0;
void* ScranMAPRun_Uninstall(void* param)
{
	int status;
    JNIEnv *env;
    bool isAttached = false;
	
	CQTPackManager* pThis = (CQTPackManager*)param;	
	status =g_jvmMAP_Uninstall->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status< 0) 
	{		
		MY_SAPI_DB_PRINTF("callback_handler: failed to get JNI environment assuming native thread");		
        status =g_jvmMAP_Uninstall->AttachCurrentThread(&env, NULL);
		
        if(status < 0)
		{
			MY_SAPI_DB_PRINTF("callback_handler: failed to attach current thread");
			return NULL;
        }
        isAttached = true;
    }
	
	pThis->CPM_UnintallPackageThread(env,pThis->m_szAppID);
	
	if(isAttached)
	{
		MY_SAPI_DB_PRINTF("0000000000000000000000000000000000000000000000000ScranMAPRun--End");
		g_jvmMAP_Uninstall->DetachCurrentThread();
	}
}
#endif

int	CQTPackManager::CPM_UnintallPackage(char* lpAppID)
{
	PGlobalParam thisparam = MAP_GetParam();
	
	strcpy(m_szAppID,lpAppID);
	g_jvmMAP_Uninstall = thisparam->jvm;
	int rr = pthread_create(&MAP_Uninstall_thread, NULL, ScranMAPRun_Uninstall, this);	
}

int	CQTPackManager::CPM_UnintallPackageThread(JNIEnv *env,char* lpAppID)
{
	PGlobalParam thisparam = MAP_GetParam();
	PAppInstall pApp = m_pcqtLocalAppData->CA_GetLocalItemByAppID(lpAppID);
	if(!pApp)
	{
		MY_SAPI_DB_PRINTF("CPM_UnintallPackage-- CA_GetLocalItemByAppID ERROR");
		return 1;
	}

	MY_SAPI_DB_PRINTF("CPM_UnintallPackage-- szPackageName 0= %s",pApp->szPackageName);
	thisparam->pfnPFNJNI_uninstallApp(env,pApp->szPackageName);	
	MY_SAPI_DB_PRINTF("CPM_UnintallPackage-- szPackageName 1= %s",pApp->szPackageName);
	//CPM_ScanLocalPackInfo(env);
	//MY_SAPI_DB_PRINTF("CPM_UnintallPackage-- szPackageName 2= %s",pApp->szPackageName);
	//thisparam->pfnRefreshUI(env);
	return 0;
}

int	CQTPackManager::CPM_UpdatePackage(char* lpAppID)
{
	return CPM_InstallPackage(lpAppID);
}

int	CQTPackManager::CPM_IsInstallPackage(char* lpAppID)
{
	//m_pcqtLocalAppData->CA_ReadLocal();
	PAppInstall pApp = m_pcqtLocalAppData->CA_GetLocalItemByAppID(lpAppID);
	if(pApp)
	{
		if(0 == strcmp(pApp->szPackageVersion,"firstinstall"))
		{
			return 0;
		}
		return 1;
	}

	return 0;
}

int	CQTPackManager::CPM_IsUpdatePackage(char* lpAppID)
{
	//m_pcqtLocalAppData->CA_ReadLocal();
	PAppInstall pApp = m_pcqtLocalAppData->CA_GetLocalItemByAppID(lpAppID);
	if(!pApp)
	{
		return 0;
	}

	PCqtApp p = m_pcqtLocalAppData->CA_GetAppItemFromID(lpAppID);
	if(!p)
	{
		return 0;
	}

	if(0 != strcmp(p->tAppBaseInfo.szVersion,pApp->szPackageVersion))
	{
		return 1;
	}

	return 0;
}