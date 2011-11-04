#include "SAPIEngine.h"
#include "SvoxEngine.h"

#include <string.h>
#ifndef PLATFROM_SAPI_WINCE_EVC
#include <strings.h>
#define stricmp strcasecmp
#endif
#include <stdarg.h>
#include <stdio.h>

#ifdef PLATFROM_SAPI_WINCE_EVC
#define stricmp _stricmp
#define usleep Sleep
#endif

#if defined(PLATFROM_SAPI_ANDROID_NDK)
#define usleep(x) (x)
#endif


//////////////////////////////////////////////////////////////////////////
void SAPI_DB_PRINTF(char* fmt,...)
{
	char szTempInfo[MAX_STRING_COUNT_HUGE*2];
	memset(szTempInfo,0,MAX_STRING_COUNT_HUGE*2);
	
	va_list args;
	int n;
	va_start(args, fmt);
	n = vsprintf(szTempInfo, fmt, args);
	va_end(args);
	strcat(szTempInfo,"\r\n");
#if defined(PLATFROM_SAPI_ANDROID_QT) || defined(PLATFROM_SAPI_ANDROID_NDK)
	__android_log_print(ANDROID_LOG_INFO,"SAPI_Printf",szTempInfo);
#else
	printf(szTempInfo);
#endif
}

void SAPI_Engine_Info(PEngineInfo pEngine, EnDebugInfoType eType, char* fmt,...)
{
	char szTempInfo[MAX_STRING_COUNT_HUGE*2];
	memset(szTempInfo,0,MAX_STRING_COUNT_HUGE*2);

	va_list args;
	int n;
	va_start(args, fmt);
	n = vsprintf(szTempInfo, fmt, args);
	va_end(args);
	
	pEngine->m_tSapiRegister.pCqtSapiInterface->ITF_DebugInfoOutput(pEngine->handle,eType,szTempInfo);
}

//////////////////////////////////////////////////////////////////////////
//CQT Thread
EngineThread::EngineThread(PEngineInfo pEngine)
{
	m_pEngine = pEngine;
	m_pEngine->enStatus = SAPI_Status_Idle;
}

EngineThread::~EngineThread()
{
}

void EngineThread::T_run()
{
	//1
	PEngineInfo pEngine  = m_pEngine;
	if(!pEngine)
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"EngineThread not resource,Maybe you not init");
		pEngine->enStatus = SAPI_Status_Idle;
		return;
	}	
	
	//高级规则处理
	int ntype = pEngine->tEngineParamData.nType;
	switch (ntype)
	{
	case en_type_High_level:
		pEngine->enStatus = SAPI_Status_HighLevel;
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"EngineThread Start high-level service.");
		if(pEngine->haveHighlevel)
		{
			PTagFunctionItem pTag = pEngine->pConfig->SP_GetTagItem(0);
			if(pTag)
			{				
				Run_TagFun(pEngine,pTag->szTagName);
			}else
			{
				SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"EngineThread had not find your high-level config rules, please check!");
			}
		}else
		{
			SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"EngineThread had not find your high-level config rules, please check!");
		}
		break;
	case en_type_ASR_Rule:
		{
			pEngine->enStatus = SAPI_Status_ASRING;
			SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"EngineThread Start ASR[%s] service.",pEngine->tEngineParamData.szName);
			PASRRuleItem pItem = pEngine->pConfig->SP_GetASRName(pEngine->tEngineParamData.szName);
			if(pItem)
			{
				char* lpResult[MAX_ASR_RESULT_COUNT];
				memset(lpResult,0,sizeof(char*)*MAX_ASR_RESULT_COUNT);

				pEngine->pThirdEngine->TE_ASR(pItem->szRuleName,
					pItem->lpContext,
					pItem->lpASRLanguage,
					pEngine->m_tSapiRegister.pCqtSapiInterface,
					lpResult);

				pEngine->m_tSapiRegister.pCqtSapiInterface->ITF_CallApplication(pItem->szRuleName,lpResult);

				for(int k = 0; k < MAX_ASR_RESULT_COUNT; k++)
				{
					if(lpResult[k])
						delete lpResult[k];
					lpResult[k] = 0;
				}
			}else
			{
				SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"EngineThread not find ASR[%s] rule!",pEngine->tEngineParamData.szName);
			}
		}
		break;
	case en_type_TTS_Rule:
		{
			pEngine->enStatus = SAPI_Status_TTSING;
			SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"EngineThread Start TTS rule[%s] service.",pEngine->tEngineParamData.szName);
			PTTSTextItem pItem = pEngine->pConfig->SP_GetTTSName(pEngine->tEngineParamData.szName);
			if(pItem)
			{
				pEngine->pThirdEngine->TE_TTS(pItem->lpText,pItem->lpTTSLanguage,pItem->bisFile);
			}else
			{
				SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"EngineThread not find TTS rule[%s]!",pEngine->tEngineParamData.szName);
			}
		}
		break;
	case en_type_Voice_Rule:
		{
			pEngine->enStatus = SAPI_Status_VoiceING;
			SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"EngineThread Start Voice voice rule [%s] service.",pEngine->tEngineParamData.szName);
			PCustomvoiceItem pItem = pEngine->pConfig->SP_GetVoiceName(pEngine->tEngineParamData.szName);
			if(pItem)
			{
				pEngine->pThirdEngine->TE_Voice(pItem->lpCustomFile);
			}else
			{
				SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"EngineThread not find Voice rule[%s]!",pEngine->tEngineParamData.szName);
			}
		}
		break;
	case en_type_TTS_text:
		{
			pEngine->enStatus = SAPI_Status_TTSING;
			SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"EngineThread Start Voice TTS Text [%s] service.",pEngine->tEngineParamData.lpText);			
			pEngine->pThirdEngine->TE_TTS(pEngine->tEngineParamData.lpText,pEngine->tEngineParamData.szLanguage,false);			
			delete pEngine->tEngineParamData.lpText;
			pEngine->tEngineParamData.lpText = 0;
		}
		break;
	case en_type_Voice_File:
		{
			pEngine->enStatus = SAPI_Status_VoiceING;
			SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"EngineThread Start Voice TTS Text [%s] service.",pEngine->tEngineParamData.lpText);
			pEngine->pThirdEngine->TE_Voice(pEngine->tEngineParamData.lpText);	
			delete pEngine->tEngineParamData.lpText;
			pEngine->tEngineParamData.lpText = 0;
		}
	default:
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"EngineThread not find resource, Maybe other error!");
		break;
	}

	pEngine->enStatus = SAPI_Status_Idle;
	SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"EngineThread stop service.");
}

//////////////////////////////////////////////////////////////////////////
bool EngineThread::SetWrokStatus(ENSAPIStatus enStatus)
{
	return true;
}

bool EngineThread::Do_Item(PEngineInfo pEngine,PDoRuleItem pDo,char** ppResult)
{
	memset(ppResult,0,sizeof(char*)*MAX_ASR_RESULT_COUNT);

	//play voice file
	if( 0 == stricmp("play" ,pDo->szFunction))
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[Play voice] voice rule name=%s",pDo->szParam);
		PCustomvoiceItem pVoice = pEngine->pConfig->SP_GetVoiceName(pDo->szParam);
		if(pVoice)
		{
			pEngine->pThirdEngine->TE_Voice(pVoice->lpCustomFile);
		}else
		{
			SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"--[Error][Play voice] Do not find voice rule name=%s",pDo->szParam);
		}
		return true;
	}

	//TTS
	if( 0 == stricmp("say" ,pDo->szFunction))
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[TTS Rule] Play TTS rule name=%s",pDo->szParam);
		PTTSTextItem pItem = pEngine->pConfig->SP_GetTTSName(pDo->szParam);
		if(pItem)
		{
			pEngine->pThirdEngine->TE_TTS(pItem->lpText,pItem->lpTTSLanguage,pItem->bisFile);
		}else
		{
			SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"--[Error][Play TTS] Do not find TTS rule name=%s",pDo->szParam);
		}
		return true;
	}

	//ASR
	if( 0 == stricmp("get" ,pDo->szFunction))
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[ASR Rule] ASR rule name=%s",pDo->szParam);
		strcpy(pEngine->szLastASRRuleName,pDo->szParam);
		PASRRuleItem pItem = pEngine->pConfig->SP_GetASRName(pDo->szParam);
		if(pItem)
		{
			pEngine->pThirdEngine->TE_ASR(pItem->szRuleName,
				pItem->lpContext,
				pItem->lpASRLanguage,
				pEngine->m_tSapiRegister.pCqtSapiInterface,
				ppResult);

			char szTempResult[MAX_STRING_COUNT_HUGE];
			for(int k = 0; k < MAX_ASR_RESULT_COUNT; k++)
			{
				if(ppResult[k])
				{
					sprintf(szTempResult,"ASR Result: [%d] %s",k+1, ppResult[k]);
					SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"%s",szTempResult);
				}
			}
		}else
		{
			SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"--[Error][ASR RULE] Do not find ASR rule name=%s",pDo->szParam);
		}
		return true;
	}

	//Goto
	if( 0 == stricmp("goto" ,pDo->szFunction))
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[Goto Rule] goto tag function name=%s",pDo->szParam);
		Run_TagFun(pEngine,pDo->szParam);
		return true;
	}

	//Goto
	if( 0 == stricmp("sleep" ,pDo->szFunction))
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[Sleep Rule] Now Sleep =%s",pDo->szParam);
		int nTime = atoi(pDo->szParam);
		usleep(nTime);
		return true;
	}

	if( 0 == stricmp("call" ,pDo->szFunction))
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[Call Rule] Now call application");
		pEngine->m_tSapiRegister.pCqtSapiInterface->ITF_CallApplication(pEngine->szLastASRRuleName,ppResult);
		return true;
	}
	return false;
}

bool EngineThread::Run_DoRuleItem(PEngineInfo pEngine,PDoRuleItem pDoRule,char** ppResult)
{	
	int i = 0;
	int nCount = pEngine->pConfig->SP_GetDoItemCount(pDoRule);	
	for(i = 0; i < nCount; i++)
	{
		if(!WaitForContinue(pEngine))
			return false;

		PDoRuleItem pDo = pEngine->pConfig->SP_GetDoItem(i,pDoRule);
		Do_Item(pEngine,pDo,ppResult);
	}
}

bool EngineThread::Run_ResultItem(PEngineInfo pEngine,PResultRuleItem pResultItem,char** ppResult)
{
	//////////////////////////////////////////////////////////////////////////
	//看看结果是否有多项选择
	//////////////////////////////////////////////////////////////////////////
	//现在第一个版本只选一个结果进行处理。
	char* lpFirstResult = ppResult[0];
	if(!lpFirstResult)
		return false;

	//////////////////////////////////////////////////////////////////////////
	int i = 0;
	int nCount = pEngine->pConfig->SP_GetResultItemCount(pResultItem);
	for(i = 0; i < nCount; i++)
	{
		if(!WaitForContinue(pEngine))
			return false;		

		PResultRuleItem pItem = pEngine->pConfig->SP_GetResultItem(i,pResultItem);
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[Result Select] RequestName=%s,param = %s,result=%s",pItem->szRequestName,pItem->lpParam,lpFirstResult);
		if( 0 == stricmp(pItem->szRequestName,"ifget") && pItem->lpParam && 0 == stricmp(pItem->lpParam,lpFirstResult))
		{
			Run_DoRuleItem(pEngine,pItem->pDoRuleItem,ppResult);
			return true;
		}else if(0 == stricmp(pItem->szRequestName,"default"))
		{
			Run_DoRuleItem(pEngine,pItem->pDoRuleItem,ppResult);
			return true;
		}
	}

	return false;
}

bool EngineThread::Run_TagFun(PEngineInfo pEngine,char* lpTagName)
{
	SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"High-level start tag function[%s]",lpTagName);
	int i = 0;
	int nCount = pEngine->pConfig->SP_GetTagItemCount();
	PTagFunctionItem pTag = 0;	
	for(i = 0; i < nCount; i++)
	{
		PTagFunctionItem pItem = pEngine->pConfig->SP_GetTagItem(i);
		if(0 == stricmp(pItem->szTagName,lpTagName))
		{
			pTag = pItem;
			break;
		}
	}

	if(!pTag)
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"[Error]High-level start tag function [%s] error,Please check your config rules!",lpTagName);
		return false;
	}
	
	nCount = pEngine->pConfig->SP_GetProcItemCount(pTag->pProcRulesItem);	
	for(i = 0; i < nCount; i++)
	{
		if(!WaitForContinue(pEngine))
			return false;

		ProcRulesItem pProcItem = pEngine->pConfig->SP_GetProcItem(i,pTag->pProcRulesItem);
		if(pProcItem)
		{
			char* lpResult[MAX_ASR_RESULT_COUNT];			
			if(!Run_DoRuleItem(pEngine,&(pProcItem->tDoRuleItem),lpResult))
				return false;

			Run_ResultItem(pEngine,pProcItem->pResultRuleItem,lpResult);

			//删除结果
			for(int k = 0; k < MAX_ASR_RESULT_COUNT; k++)
			{
				if(lpResult[k])
					delete lpResult[k];
				lpResult[k] = 0;
			}
		}
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
bool EngineThread::StartWork()
{
	SAPI_ASSERT(m_pEngine);
	m_pEngine->enStatus = SAPI_Status_Starting;
	T_start();

	return true;
}

bool EngineThread::PauseWork()
{	
	SAPI_ASSERT(m_pEngine);
	if(m_pEngine->enStatus == SAPI_Status_Paused || 
		m_pEngine->enStatus == SAPI_Status_Idle  ||
		m_pEngine->enStatus == SAPI_Status_Over )
	{
		return false;
	}

	m_pEngine->enStatus = SAPI_Status_Pauseing;
	for(;m_pEngine->enStatus != SAPI_Status_Pauseing;)
	{
		usleep(100);
	}

	return true;
}

bool EngineThread::isWorking()
{
	if(m_pEngine->enStatus == SAPI_Status_Idle)
	{
		return false;
	}

	return true;
}

bool EngineThread::ResumeWork()
{
	SAPI_ASSERT(m_pEngine);
	if(m_pEngine->enStatus != SAPI_Status_Paused)
		return false;

	m_pEngine->enStatus = SAPI_Status_Starting;
	return true;
}

bool EngineThread::StopWork()
{
	if(m_pEngine->enStatus == SAPI_Status_Idle)
		return false;

	SAPI_ASSERT(m_pEngine);
	m_pEngine->enStatus = SAPI_Status_Stoping;
	if(m_pEngine->pThirdEngine)
		m_pEngine->pThirdEngine->TE_Stop();
	for(;m_pEngine->enStatus != SAPI_Status_Idle;)
	{
		usleep(100);
	}
	m_pEngine->enStatus = SAPI_Status_Idle;
	return true;
}

bool EngineThread::WaitForContinue(PEngineInfo pEngine)
{
	if(!pEngine) return false;

	if(pEngine->enStatus == SAPI_Status_Pauseing)
		pEngine->enStatus = SAPI_Status_Paused;

	for(;pEngine->enStatus == SAPI_Status_Paused;)
	{
		usleep(100);		
	}

	if(pEngine->enStatus == SAPI_Status_Stoping)
	{
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
//Engine
SAPIEngine::SAPIEngine(CQtObject* parent)
:CQtObject(parent)
{
	m_parent = parent;
	memset(m_pEngineinfo,0,sizeof(PEngineInfo)*ENGINE_MAX_COUNT);
	m_handecount = 1;
	m_nActiveHandle = -1;
}

SAPIEngine::~SAPIEngine()
{
	Engine_Destory();
}

void SAPIEngine::Engine_Create()
{	
}

void SAPIEngine::Engine_Destory()
{
	for(int i = 0; i < ENGINE_MAX_COUNT;)
	{
		__Engine_ZeroEngine(i);
	}
}

int	SAPIEngine::__Engine_GetIdleEngine()
{
	for(int i = 0; i < ENGINE_MAX_COUNT;)
	{
		if(!m_pEngineinfo[i])
			return i;
	}

	return -1;
}

bool SAPIEngine::__Engine_ZeroEngine(int i)
{
	PEngineInfo pEngine = m_pEngineinfo[i];
	if(pEngine)
	{
		Engine_UnRegister(pEngine->handle);		
	}
	
	return true;
}

int	SAPIEngine::__Engine_GetEngine(SAPIHANDLE handle)
{
	for(int i = 0; i < ENGINE_MAX_COUNT;)
	{
		if(m_pEngineinfo[i] && m_pEngineinfo[i]->handle == handle)
			return i;
	}

	return -1;
}
//////////////////////////////////////////////////////////////////////////
SAPIHANDLE	SAPIEngine::GetOneSapiHandle()
{
	m_handecount++;
	if(m_handecount >= 0xffffff)
		m_handecount = 1;


	return m_handecount;
}

SAPIRESULTCODE SAPIEngine::Engine_Register(SAPIHANDLE handle,PSAPIRegister pSapiRegister)//Register 
{
	SAPIRESULTCODE iRet = SAPI_OK;
	//check and init input value	
	SAPI_ASSERT(pSapiRegister);
	if(!pSapiRegister)
		return SAPI_ERROR;

	int idx = __Engine_GetIdleEngine();
	if(idx < 0)
	{		
		SAPI_DB_PRINTF("--SAPIEngine::Engine_Register  Engine is over,not idle\r\n");
		return SAPI_ENGINE_NO_RESOURCE;
	}

	//Create engine data
	m_pEngineinfo[idx] = new TEngineInfo;
	memset(m_pEngineinfo[idx],0,sizeof(TEngineInfo));
	PEngineInfo pEngine = m_pEngineinfo[idx];
	memcpy(&(pEngine->m_tSapiRegister),pSapiRegister,sizeof(TSAPIRegister));
	pEngine->pConfig = new CSAPIConfig;	
	pEngine->handle = handle;

	//Init and read sapi config file
	char szConfigfile[MAX_STRING_COUNT_HUGE];
	memset(szConfigfile,0,MAX_STRING_COUNT_HUGE);
	strcpy(szConfigfile,pSapiRegister->szConfigResoucePath);
	strcat(szConfigfile,"/");
	strcat(szConfigfile,pSapiRegister->szConfigFileName);
	strcat(szConfigfile,".sapi");
	
	SAPI_DB_PRINTF("[Info]--Engine_Register  szConfigFileName = %s\r\n",szConfigfile);
	SAPI_DB_PRINTF("                         szConfigResoucePath = %s\r\n",pSapiRegister->szConfigResoucePath);
	SAPI_DB_PRINTF("                         szConfigFileName = %s\r\n",pSapiRegister->szConfigFileName);	
	iRet = pEngine->pConfig->SP_Open(szConfigfile);	
	if(iRet != SAPI_OK)
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"[Register engine error]EngineThread do not read config file[%s],please check!",szConfigfile);
		Engine_UnRegister(handle);
		return iRet;
	}

	pEngine->haveHighlevel = false;
	if(pEngine->pConfig->SP_GetTagItemCount() > 0)
		pEngine->haveHighlevel = true;

	//////////////////////////////////////////////////////////////////////////
	//Engine
	PConfigMainInfo pMainInfo = pEngine->pConfig->SP_GetMainInfo();	
	if(pMainInfo->lpString[en_MainInfo_Engine] && 0 == stricmp(pMainInfo->lpString[en_MainInfo_Engine],"svox"))
	{
		pEngine->pThirdEngine = (CThirdEngine*)new SVoxEngine(m_parent);
		szConfigfile[MAX_STRING_COUNT_HUGE];
		memset(szConfigfile,0,MAX_STRING_COUNT_HUGE);
		strcpy(szConfigfile,pSapiRegister->szConfigResoucePath);
		strcat(szConfigfile,"/build");
		pEngine->pThirdEngine->TE_SetResourcePath(szConfigfile,pSapiRegister->szConfigFileName);
	}else
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"[Error]EngineThread do not not support engine [%s],please check!",pMainInfo->lpString[en_MainInfo_Engine]);
		Engine_UnRegister(handle);
		return SAPI_ENGINE_NOT_SUPPORT;
	}
	pEngine->isInitFlag = true;
	pEngine->pEngineThread = new EngineThread(pEngine);
	SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[OK] Register engine have successful!");
	return SAPI_OK;
}

SAPIRESULTCODE SAPIEngine::Engine_UnRegister(SAPIHANDLE handle)
{
	int i = __Engine_GetEngine(handle);	
	if(i < 0)
	{
		return SAPI_ERROR;
	}

	PEngineInfo pEngine = m_pEngineinfo[i];
	if(!pEngine)
	{
		return SAPI_ERROR;
	}

	if(pEngine->pEngineThread->isWorking())
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"[Error] Engine has working,please try after let engine to stoped!");
		return SAPI_ENGINE_BUSY;
	}	
	if(Engine_isActive(handle))
	{
		SAPI_Engine_Info(pEngine,en_ITF_MSG_Error,"[Error] Engine has active,please try after let engine to stoped!");
		return SAPI_ENGINE_NOT_ACTIVE;
	}

	if(pEngine)
	{
		if(pEngine->pConfig)
			pEngine->pConfig->DeleteAll();

		if(pEngine->pEngineThread)
			delete pEngine->pEngineThread;
		if(pEngine->pConfig)
			delete pEngine->pConfig;

		delete m_pEngineinfo[i];
		m_pEngineinfo[i] = 0;
	}
	
	SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[OK] unregister engine successful!");
	return SAPI_OK;
}

bool SAPIEngine::Engine_SetActive(SAPIHANDLE handle)
{
	if(m_nActiveHandle == handle)
		return true;

	if(m_nActiveHandle >= 0)
	{
		PEngineInfo pEngine = Engine_GetFromHandle(handle);
		if(!pEngine)
			return false;

		if(pEngine->pEngineThread->isWorking())
		{
			pEngine->pEngineThread->PauseWork();
		}
	}

	m_nActiveHandle = handle;	
	return true;
}

SAPIRESULTCODE SAPIEngine::Engine_Stop(SAPIHANDLE handle)			//stop sapi
{
	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
	{
		return SAPI_ERROR;
	}

	pEngine->pEngineThread->StopWork();
	SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[OK] Stoped!");
	return SAPI_OK;
}

bool SAPIEngine::Engine_isActive(SAPIHANDLE handle)
{
	if(m_nActiveHandle != handle)
		return false;

	return true;
}

SAPIRESULTCODE SAPIEngine::Engine_Hi_Start(SAPIHANDLE handle)		//start sapi	
{
	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return SAPI_ERROR;

	if(pEngine->pEngineThread->isWorking())
	{
		return SAPI_ENGINE_BUSY;
	}
	
	if(!Engine_isActive(handle))
		return SAPI_ENGINE_NOT_ACTIVE;
	
	//2开始工作
	memset(&(pEngine->tEngineParamData),0,sizeof(TEngineParamData));
	pEngine->tEngineParamData.nType = en_type_High_level;
	pEngine->pEngineThread->StartWork();

	return SAPI_OK;
}

SAPIRESULTCODE SAPIEngine::Engine_Hi_Pause(SAPIHANDLE handle)
{
	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return SAPI_ERROR;
	
	if(!pEngine->pEngineThread)
		return SAPI_ERROR;

	pEngine->pEngineThread->PauseWork();
	SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[OK] Paused!");
	return SAPI_OK;
}

SAPIRESULTCODE SAPIEngine::Engine_Hi_Resume(SAPIHANDLE handle)
{
	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return SAPI_ERROR;
	
	if(!pEngine->pEngineThread)
		return SAPI_ERROR;
	
	pEngine->pEngineThread->ResumeWork();
	SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[OK] Resumed!");
	return SAPI_OK;
}

SAPIRESULTCODE SAPIEngine::Engine_Hi_QUit(SAPIHANDLE handle)
{
	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return SAPI_ERROR;
	
	if(!pEngine->pEngineThread)
		return SAPI_ERROR;
	
	pEngine->pEngineThread->StopWork();
	SAPI_Engine_Info(pEngine,en_ITF_MSG_Info,"[OK] Quited!");
	return SAPI_OK;
}

bool SAPIEngine::__isWorking(SAPIHANDLE handle)
{
	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return true;
	
	return pEngine->pEngineThread->isWorking();
}

PEngineInfo	SAPIEngine::Engine_GetFromHandle(SAPIHANDLE handle)
{
	int i = __Engine_GetEngine(handle);	
	if(i < 0)
		return 0;
	
	return m_pEngineinfo[i];
}

ENSAPIStatus   SAPIEngine::Engine_Status(SAPIHANDLE handle)			//query status
{
	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return SAPI_Status_Unknow;

	return pEngine->enStatus;
}


SAPIRESULTCODE SAPIEngine::Engine_Lo_ASRRule(SAPIHANDLE handle,char* lpRuleName)	//start a asr rule
{
	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return SAPI_ENGINE_NO_RESOURCE;

	if(!lpRuleName || strlen(lpRuleName) >= (MAX_STRING_COUNT_LARGE-1))
	{
		SAPI_Engine_Info(pEngine, en_ITF_MSG_Error, STRING_ENGINE_INPUT_PARAM_ERROR);
		return SAPI_ERROR;
	}	

	if(__isWorking(handle))
	{
		SAPI_Engine_Info(pEngine, en_ITF_MSG_Error, STRING_ENGINE_BUSY);
		return SAPI_ENGINE_BUSY;
	}	
	
	if(!Engine_isActive(handle))
	{
		SAPI_Engine_Info(pEngine, en_ITF_MSG_Error, STRING_ENGINE_NOT_ACTIVE);
		return SAPI_ENGINE_NOT_ACTIVE;
	}

	memset(&(pEngine->tEngineParamData),0,sizeof(TEngineParamData));
	pEngine->tEngineParamData.nType = en_type_ASR_Rule;
	strcpy(pEngine->tEngineParamData.szName,lpRuleName);

#if 0
	((SVoxEngine*)(pEngine->pThirdEngine))->TestASR();
#else
	pEngine->pEngineThread->StartWork();
#endif
	return SAPI_OK;
}

SAPIRESULTCODE SAPIEngine::Engine_Lo_TTSRule(SAPIHANDLE handle,char* lpRuleName)	//start a tts rule
{
	if(!lpRuleName || strlen(lpRuleName) >= (MAX_STRING_COUNT_LARGE-1))
		return SAPI_ERROR;

	if(__isWorking(handle))
	{
		return SAPI_ENGINE_BUSY;
	}

	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return SAPI_ENGINE_NO_RESOURCE;
	
	if(!Engine_isActive(handle))
		return SAPI_ENGINE_NOT_ACTIVE;

	memset(&(pEngine->tEngineParamData),0,sizeof(TEngineParamData));
	pEngine->tEngineParamData.nType = en_type_TTS_Rule;
	strcpy(pEngine->tEngineParamData.szName,lpRuleName);
	
	pEngine->pEngineThread->StartWork();
	return SAPI_OK;
}

SAPIRESULTCODE SAPIEngine::Engine_Lo_VoiceRule(SAPIHANDLE handle,char* lpRuleName)	//start a voic rule
{
	if(!lpRuleName || strlen(lpRuleName) >= (MAX_STRING_COUNT_LARGE-1))
		return SAPI_ERROR;

	if(__isWorking(handle))
	{
		return SAPI_ENGINE_BUSY;
	}

	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return SAPI_ENGINE_NO_RESOURCE;
	
	if(!Engine_isActive(handle))
		return SAPI_ENGINE_NOT_ACTIVE;
	
	memset(&(pEngine->tEngineParamData),0,sizeof(TEngineParamData));
	pEngine->tEngineParamData.nType = en_type_Voice_Rule;
	strcpy(pEngine->tEngineParamData.szName,lpRuleName);
	
	pEngine->pEngineThread->StartWork();
	return SAPI_OK;
}

SAPIRESULTCODE SAPIEngine::Engine_Lo_TTSText(SAPIHANDLE handle,char* lpTTSText,char* lpLanguage)	//start a tts text
{
	if(!lpTTSText || !lpLanguage || strlen(lpLanguage) >= MAX_STRING_COUNT_SMALL)
		return SAPI_ERROR;

	if(__isWorking(handle))
	{
		return SAPI_ENGINE_BUSY;
	}

	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return SAPI_ENGINE_NO_RESOURCE;
	
	if(!Engine_isActive(handle))
		return SAPI_ENGINE_NOT_ACTIVE;

	memset(&(pEngine->tEngineParamData),0,sizeof(TEngineParamData));
	pEngine->tEngineParamData.nType = en_type_TTS_text;
	int nLen = strlen(lpTTSText);
	pEngine->tEngineParamData.lpText = new char[nLen + 10];
	memset(pEngine->tEngineParamData.lpText,0,sizeof(char)*(nLen + 10));
	strcpy(pEngine->tEngineParamData.lpText,lpTTSText);
	strcpy(pEngine->tEngineParamData.szLanguage,lpLanguage);
	
	pEngine->pEngineThread->StartWork();
	return SAPI_OK;	
}

SAPIRESULTCODE SAPIEngine::Engine_Lo_VoiceFile(SAPIHANDLE handle,char* lpPcmFile)	//start a voic file
{
	if(!lpPcmFile)
		return SAPI_ERROR;

	if(__isWorking(handle))
	{
		return SAPI_ENGINE_BUSY;
	}

	PEngineInfo pEngine = Engine_GetFromHandle(handle);
	if(!pEngine)
		return SAPI_ENGINE_NO_RESOURCE;
	
	if(!Engine_isActive(handle))
		return SAPI_ENGINE_NOT_ACTIVE;

	memset(&(pEngine->tEngineParamData),0,sizeof(TEngineParamData));
	pEngine->tEngineParamData.nType = en_type_Voice_File;
	int nLen = strlen(lpPcmFile);
	pEngine->tEngineParamData.lpText = new char[nLen + 10];
	memset(pEngine->tEngineParamData.lpText,0,sizeof(char)*(nLen + 10));
	strcpy(pEngine->tEngineParamData.lpText,lpPcmFile);
	
	pEngine->pEngineThread->StartWork();
	return SAPI_ERROR;
}

