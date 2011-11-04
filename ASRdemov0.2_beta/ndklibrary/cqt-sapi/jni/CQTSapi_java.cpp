
#include "CQTSapi_java.h"
#include <pthread.h>
#include <stdio.h>
#include <string.h>


//////////////////////////////////////////////////////////////////////////
template<class VarItem>
int					__CSS_GetVarCount(VarItem* pFirstItem);
template<class VarItem>
VarItem*			__CSS_GetVarItem(int idx, VarItem* pFirstItem);
template<class VarItem>
bool				__CSS_AddVarItem(VarItem* pItem, VarItem** pFirstItem);

bool				__CSS_ClearValueItem(PVarValueItem* pFirstItem);
bool				__CSS_ClearVarItem(PMemoryVARValue* pFirstItem);
bool				__CSS_ClearVarValueItem(char* lpVar, PMemoryVARValue* pFirstItem);

//////////////////////////////////////////////////////////////////////////
template<class VarItem>
int	__CSS_GetVarCount(VarItem* pFirstItem)//得到个数
{
	VarItem* pCur;
	int nTotal = 0;

	if(!pFirstItem) 
		return nTotal;
	
	pCur=pFirstItem;
	for(;pCur;)
	{
		pCur=pCur->next;
		nTotal++;
	}

	return nTotal;
}

template<class VarItem>
VarItem* __CSS_GetVarItem(int idx, VarItem* pFirstItem)
{
	VarItem* pCur;
	int nTotal = 0;
	if(!pFirstItem)
		return 0;
	
	pCur = pFirstItem;
	for(;pCur;)
	{
		if(idx == nTotal)
		{
			return pCur;
		}
		pCur = pCur->next;
		nTotal++;
	}
	
	return 0;
}

template<class VarItem>
bool __CSS_AddVarItem(VarItem* pItem, VarItem** pFirstItem)
{
	if(!pItem ||!pFirstItem)
		return false;

	if(!(*pFirstItem))
	{
		(*pFirstItem) = new VarItem;
		memcpy((*pFirstItem),pItem,sizeof(VarItem));
		(*pFirstItem)->next = 0;
		return true;
	}

	//找到最后一个
	VarItem* pCur = (*pFirstItem);
	for(;;)
	{
		if(!(pCur->next))
		{
			break;
		}
		pCur = pCur->next;
	}

	pCur->next = new VarItem;
	memcpy(pCur->next,pItem,sizeof(VarItem));
	pCur->next->next = 0;
	return true;
}

bool __CSS_ClearValueItem(PVarValueItem* pFirstItem)
{
	PVarValueItem pCur = *pFirstItem;
	PVarValueItem pTemp;
	for(;pCur;)
	{
		pTemp = pCur;		
		pCur = pCur->next;
		if(pTemp->lpValue)
		{
			delete pTemp->lpValue;
		}
		delete pTemp;
	}
	*pFirstItem = 0;
	return true;
}

bool __CSS_ClearVarItem(PMemoryVARValue* pFirstItem)
{
	PMemoryVARValue pCur = *pFirstItem;
	PMemoryVARValue pTemp;
	for(;pCur;)
	{
		pTemp = pCur;		
		pCur = pCur->next;
		if(pTemp->lpVar)
		{
			delete pTemp->lpVar;
		}
		if(pTemp->pValueItem)
		{
			__CSS_ClearValueItem(&(pTemp->pValueItem));
		}
		delete pTemp;
	}
	*pFirstItem = 0;
	return true;
}

PVarValueItem __CSS_FindVarItem(PMemoryVARValue pFirstItem, char* lpVar)
{
	if(!lpVar)
		return 0;

	int	nCount = __CSS_GetVarCount(pFirstItem);
	PMemoryVARValue pItem;

	for(int i = 0; i < nCount; i++)
	{
		pItem = __CSS_GetVarItem(i,pFirstItem);
		if(pItem && pItem->lpVar && 0 == strcmp(pItem->lpVar,lpVar))
		{
			return pItem->pValueItem;
		}
	}
	
	return 0;
}

PMemoryVARValue __CSS_FindVarItemEx(PMemoryVARValue pFirstItem, char* lpVar)
{
	if(!lpVar)
		return 0;
	
	int	nCount = __CSS_GetVarCount(pFirstItem);
	PMemoryVARValue pItem;
	
	for(int i = 0; i < nCount; i++)
	{
		pItem = __CSS_GetVarItem(i,pFirstItem);
		if(pItem && pItem->lpVar && 0 == strcmp(pItem->lpVar,lpVar))
		{
			return pItem;
		}
	}
	
	return 0;
}

int	__CSS_SetVarValue(char* lpVar, char* lpValue,PMemoryVARValue* pFirstItem)
{	
	PMemoryVARValue pValueItem = __CSS_FindVarItemEx(*pFirstItem,lpVar);
	if(!pValueItem)
	{
		//添加项目
		TMemoryVARValue tVarItem;
		memset(&tVarItem,0,sizeof(TMemoryVARValue));
		tVarItem.lpVar = new char[strlen(lpVar) + 1];
		strcpy(tVarItem.lpVar,lpVar);
		
		SAPI_DB_PRINTF("2__CSS_SetVarValue--lpVar=%s,lpValue=%s",lpVar,lpValue);
		if(!__CSS_AddVarItem(&tVarItem,pFirstItem))
			return 1;

		SAPI_DB_PRINTF("3__CSS_SetVarValue--lpVar=%s,lpValue=%s",lpVar,lpValue);
		pValueItem = __CSS_FindVarItemEx((*pFirstItem),lpVar);
	}

	SAPI_DB_PRINTF("4__CSS_SetVarValue--lpVar=%s,lpValue=%s",lpVar,lpValue);
	if(!pValueItem)
		return 2;

	TVarValueItem tValueItem;
	memset(&tValueItem,0,sizeof(TVarValueItem));
	tValueItem.lpValue = new char[strlen(lpValue) + 1];
	strcpy(tValueItem.lpValue, lpValue);	
	if(!__CSS_AddVarItem(&tValueItem, &(pValueItem->pValueItem)))
		return 3;
	
	return 0;
}

bool __CSS_ClearVarValueItem(char* lpVar, PMemoryVARValue* pFirstItem)
{
	PMemoryVARValue pItem = __CSS_FindVarItemEx((*pFirstItem), lpVar);
	if(pItem)
	{
		__CSS_ClearValueItem(&(pItem->pValueItem));
		pItem->pValueItem = 0;
		return true;
	}

	return false;
}
//////////////////////////////////////////////////////////////////////////

PMemoryVARValue g_pASRMemoryVar = 0;
PMemoryVARValue g_pTTSMemoryVar = 0;
//////////////////////////////////////////////////////////////////////////

PFNcallback_SAPI_DebugOutInfo	pfncallback_SAPI_DebugOutInfo;
PFNcallback_SAPI_ASRResult		pfncallback_SAPI_ASRResult;
pthread_t Debuginfo_thread;
pthread_t ASRResult_thread;
JavaVM*  g_jvmCSS = 0;
void* ThreadDebuginfo(void* param)
{
	int status;
    JNIEnv *env;
    bool isAttached = false;	
	char* lpInfo = (char*)param;

	SAPI_DB_PRINTF("ThreadDebuginfo--lpInfo=%s",lpInfo);
	status =g_jvmCSS->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status< 0) 
	{
        status =g_jvmCSS->AttachCurrentThread(&env, NULL);		
        if(status < 0)
		{
			delete lpInfo;
			return NULL;
        }
        isAttached = true;
    }
	
	pfncallback_SAPI_DebugOutInfo(env,lpInfo);
	
	if(isAttached)
	{		
		g_jvmCSS->DetachCurrentThread();
	}
	
	delete lpInfo;
	return 0;
}

void* ThreadASRResult(void* param)
{
	int status;
    JNIEnv *env;
    bool isAttached = false;	
	char* lpASR = (char*)param;
	
	status =g_jvmCSS->GetEnv((void **) &env, JNI_VERSION_1_4);
	if(status< 0) 
	{
        status =g_jvmCSS->AttachCurrentThread(&env, NULL);		
        if(status < 0)
		{
			delete lpASR;
			return NULL;
        }
        isAttached = true;
    }
	
	pfncallback_SAPI_ASRResult(env,lpASR);
	
	if(isAttached)
	{		
		g_jvmCSS->DetachCurrentThread();
	}	
	delete lpASR;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
MySapiInterface::MySapiInterface()
{
}

MySapiInterface::~MySapiInterface()
{
}

bool MySapiInterface::ITF_DebugInfoOutput(SAPIHANDLE hande, EnDebugInfoType etype,char* lpOutputinfo)
{
	char* lpInfo = new char[strlen(lpOutputinfo) + 1];
	strcpy(lpInfo,lpOutputinfo);
	SAPI_DB_PRINTF("ITF_DebugInfoOutput--lpInfo=%s",lpInfo);
	int rr =pthread_create(&Debuginfo_thread, NULL, ThreadDebuginfo, lpInfo);
	return true;
}

char* MySapiInterface::ITF_GetASRVarValue(char* lpASRWordVar,int idx)
{
	PVarValueItem pValueItem = __CSS_FindVarItem(g_pASRMemoryVar,lpASRWordVar);
	if(!pValueItem)
		return 0;

	int	nCount = __CSS_GetVarCount(pValueItem);
	if(idx >= nCount)
		return 0;

	PVarValueItem pItem = __CSS_GetVarItem(idx,pValueItem);
	if(!pItem)
		return 0;
	return pItem->lpValue;
}

char* MySapiInterface::ITF_GetTTVarValue(char* lpTTSWordVar)
{
	PVarValueItem pValueItem = __CSS_FindVarItem(g_pTTSMemoryVar,lpTTSWordVar);
	if(!pValueItem)
		return 0;
	
	int	nCount = __CSS_GetVarCount(pValueItem);
	if(nCount <= 0)
		return 0;
	
	PVarValueItem pItem = __CSS_GetVarItem(0,pValueItem);
	if(!pItem)
		return 0;

	return pItem->lpValue;
}

bool MySapiInterface::ITF_GetUserPressKey()
{
	return true;
}

bool MySapiInterface::ITF_CallApplication(char* lpASRRuleName,char** pparam)
{
	if(!pparam[0])
		return false;

	char* lpASR = new char[strlen(pparam[0]) + 1];
	strcpy(lpASR,pparam[0]);
	SAPI_DB_PRINTF("ITF_CallApplication--lpASR=%s",lpASR);
	int rr =pthread_create(&ASRResult_thread, NULL, ThreadASRResult, lpASR);

#if 1
	SAPI_DB_PRINTF("__DBUG__ITF_CallApplication--lpASRRuleName=%s",lpASRRuleName);
	char szTempResult[MAX_STRING_COUNT_HUGE];	
	for(int i = 0; i < MAX_ASR_RESULT_COUNT; i++)
	{
		if(pparam && pparam[i])
		{
			sprintf(szTempResult,"[%d]%s",i+1,pparam[i]);
			SAPI_DB_PRINTF("__DBUG__ITF_CallApplication--szTempResult=%s",szTempResult);
		}
	}
#endif

	return true;
}
//////////////////////////////////////////////////////////////////////////
CQTSapi_Service::CQTSapi_Service()
{
	m_pcqtSapi = 0;
	m_pmySapiInterface = 0;
}

CQTSapi_Service::~CQTSapi_Service()
{
	SAPI_DB_PRINTF("~CQTSapi_Service------------1");
	if(m_pcqtSapi)
	{
		m_pcqtSapi->Sapi_UnRegister();
		delete m_pcqtSapi;
	}
	m_pcqtSapi = 0;

	SAPI_DB_PRINTF("~CQTSapi_Service------------2");
	__CSS_ClearVarItem(&g_pASRMemoryVar);
	__CSS_ClearVarItem(&g_pTTSMemoryVar);

	SAPI_DB_PRINTF("~CQTSapi_Service------------3");
	if(m_pmySapiInterface)
		delete m_pmySapiInterface;
	m_pmySapiInterface = 0;
	SAPI_DB_PRINTF("~CQTSapi_Service------------4");
}

void CQTSapi_Service::SetCallBack(PFNcallback_SAPI_DebugOutInfo pfnDebug, PFNcallback_SAPI_ASRResult pfnReslut) 
{
	pfncallback_SAPI_DebugOutInfo = pfnDebug; 
	pfncallback_SAPI_ASRResult = pfnReslut;
}

//初始化
bool CQTSapi_Service::CSS_Init(char* lpResourcePath, char* lpConfigName,CQtObject* obj)
{	
	g_jvmCSS = obj->jvm;
	SAPI_DB_PRINTF("CSS_Init 1 g_jvmCSS = %x!",g_jvmCSS);
	m_pcqtSapi = new CQTSapi(obj);
	m_pmySapiInterface = new MySapiInterface;

	SAPI_DB_PRINTF("CSS_Init 2!");
	TSAPIRegister tRegister;
	tRegister.pCqtSapiInterface = m_pmySapiInterface;
	strcpy(tRegister.szConfigResoucePath,lpResourcePath);
	strcpy(tRegister.szConfigFileName,lpConfigName);
	SAPIRESULTCODE rt = m_pcqtSapi->Sapi_Register(&tRegister);
	if(rt != SAPI_OK)
	{
		SAPI_DB_PRINTF("CSS_Init ERROR!");
		return false;
	}

	SAPI_DB_PRINTF("CSS_Init--OK");
	return true;
}

//ASR变量清除,这里偷个懒，先全部清除
int	CQTSapi_Service::CSS_ASRClear(char* lpVar)
{
	SAPI_DB_PRINTF("CSS_ASRClear--OK");
	__CSS_ClearVarValueItem(lpVar, &g_pASRMemoryVar);	
	return 0;
}

//ASR变量设置
int	CQTSapi_Service::CSS_SetASRRuleValue(char* lpVar, char* lpValue)
{	
	SAPI_DB_PRINTF("CSS_SetASRRuleValue--OK");
	return __CSS_SetVarValue(lpVar, lpValue,&g_pASRMemoryVar);
}

//TTS变量清除,这里偷个懒，先全部清除
int	CQTSapi_Service::CSS_TTSClear(char* lpVar)
{
	SAPI_DB_PRINTF("CSS_TTSClear--OK");
	__CSS_ClearVarValueItem(lpVar, &g_pTTSMemoryVar);
	return 0;
}

//TTS变量设置
int	CQTSapi_Service::CSS_SetTTSRuleValue(char* lpVar, char* lpValue)
{
	SAPI_DB_PRINTF("CSS_SetTTSRuleValue--OK");
	return __CSS_SetVarValue(lpVar, lpValue,&g_pTTSMemoryVar);
}