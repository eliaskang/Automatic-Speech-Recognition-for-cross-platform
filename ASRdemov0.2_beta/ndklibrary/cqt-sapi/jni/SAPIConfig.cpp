// SAPIConfig.cpp: implementation of the CSAPIConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "SAPIConfig.h"
#include <stdio.h>
#include <string.h>

#ifdef PLATFROM_SAPI_WINCE_EVC
#define stricmp _stricmp
#else
#include <strings.h>
#define stricmp strcasecmp
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
template<class SapiItem>
int					__SP_GetSapiItemCount(SapiItem* pFirstItem);
template<class SapiItem>
SapiItem*			__SP_GetSapiItem(int idx, SapiItem* pFirstItem);
template<class SapiItem>
bool				__SP_SetSapiItem(int idx, SapiItem* pItem, SapiItem* pFirstItem);
template<class SapiItem>
bool				__SP_InsertSapiItem(int idx,SapiItem* pItem, SapiItem** pFirstItem);
template<class SapiItem>
bool				__SP_DeleteSapiItem(int idx,SapiItem** pFirstItem);

//////////////////////////////////////////////////////////////////////////

static char* g_lpMainInfoTag[] = {
	"SAPIVersion","ApplicationName","Version","author","Company","Email","Engine","Beep"
};
static char* g_lpdwflagString[] = 
{"Break","noBreak","QueryMultResult","FistMultResult","DoOneResult","QueryOneResult"};
static int g_flagArray[] = 
{
	SAPI_DO_RULE_FLAG_BREAK,SAPI_DO_RULE_FLAG_NOBREAK,SAPI_DO_RULE_FLAG_DONOTHING,SAPI_DO_RULE_FLAG_QueryMultResult,
	SAPI_DO_RULE_FLAG_FistMultResult,SAPI_DO_RULE_FLAG_DoOneResult,SAPI_DO_RULE_FLAG_QueryOneResult
};
//////////////////////////////////////////////////////////////////////////
int GetFlags(char* lpString)
{
	int nFlag = 0;
	int nTotal = SAPI_ARRAY_SIZE(g_lpdwflagString);
	for( int i = 0; i < nTotal; i++)
	{
		if(strstr(lpString,g_lpdwflagString[i]))
		{
			nFlag |= (0x0001 << (i+1));
		}
	}

	return nFlag;
}

//////////////////////////////////////////////////////////////////////////
template<class SapiItem>
int		__SP_GetSapiItemCount(SapiItem* pFirstItem)
{
	int nTotal = 0;
	if(!pFirstItem) 
		return nTotal;
	
	SapiItem* pCur = pFirstItem;
	for(;pCur; pCur = pCur->next)
	{
		nTotal++;
	}
	
	return nTotal;
}

template<class SapiItem>
SapiItem*	__SP_GetSapiItem(int idx, SapiItem* pFirstItem)
{
	int nTotal = 0;
	if(!pFirstItem) 
		return 0;
	
	SapiItem* pCur = pFirstItem;
	for(;pCur; pCur = pCur->next)
	{
		if(idx == nTotal)
		{
			return pCur;
		}
		nTotal++;
	}
	
	return 0;
}

template<class SapiItem>
bool	__SP_SetSapiItem(int idx, SapiItem* pItem, SapiItem* pFirstItem)
{
	if(!pItem ||!pFirstItem)
		return false;

	SapiItem* pOrgItem = __SP_GetSapiItem(idx, pFirstItem);
	if(!pOrgItem)
		return false;

	memcpy(pOrgItem,pItem,sizeof(SapiItem));
	return true;
}

template<class SapiItem>
bool	__SP_InsertSapiItem(int idx,SapiItem* pItem, SapiItem** pFirstItem)
{
	if(!pItem)
		return false;

	if(idx == -1 && !(*pFirstItem))
	{
		(*pFirstItem) = new SapiItem;
		memcpy((*pFirstItem),pItem,sizeof(SapiItem));
		(*pFirstItem)->next = 0;
		(*pFirstItem)->prev = 0;
		return true;
	}

	if(idx == 0 && (*pFirstItem))
	{
		SapiItem* pThis = new SapiItem;
		memcpy(pThis,pItem,sizeof(SapiItem));
		pThis->next = (*pFirstItem)->next;
		pThis->prev = 0;
		(*pFirstItem)->next->prev = pThis;
		(*pFirstItem) = pThis;
		return true;
	}

	int nTotal = 0;
	SapiItem* pCur = (*pFirstItem);
	SapiItem* pLast = 0;
	for(;pCur; pCur = pCur->next)
	{
		pLast = pCur;
		if(idx == nTotal)
		{
			break;
		}
		nTotal++;
	}

	SAPI_ASSERT(pLast);
	if(pLast)
	{
		SapiItem* pThis = new SapiItem;
		memcpy(pThis,pItem,sizeof(SapiItem));
		pThis->next = pThis->prev = 0;

		if(idx >= 0)
		{
			SAPI_ASSERT(pLast->prev);
			if(pLast->prev)
			{
				pLast->prev->next = pThis;
				pThis->prev = pLast->prev;
				pThis->next = pLast;
				pLast->prev = pThis;
				return true;
			}else
			{
				return false;
			}
		}
		else
		{
			SAPI_ASSERT(!(pLast->next));
			pThis->prev = pLast;
			pThis->next = 0;
			pLast->next = pThis;
			return true;
		}
	}

	return false;
}

template<class SapiItem>
bool	__SP_DeleteSapiItem(int idx,SapiItem** pFirstItem)
{
	int nTotal = 0;
	if(!(*pFirstItem)) 
		return false;
	
	SapiItem* pCur = (*pFirstItem);
	for(;pCur; pCur = pCur->next)
	{
		if(idx == nTotal)
		{
			if(!(pCur->prev))
			{
				(*pFirstItem) = pCur->next;
				if(*pFirstItem)
				{
					(*pFirstItem)->prev = 0;
				}				
				delete pCur;
			}else
			{
				if(pCur->prev)
					pCur->prev->next = pCur->next;
				if(pCur->next)
					pCur->next->prev = pCur->prev;
				delete pCur;
			}

			return true;
		}
		nTotal++;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////

CSAPIConfig::CSAPIConfig()
{	
	m_pSapiConfig = 0;
	memset(m_szConfigFile,0,sizeof(char)*MAX_STRING_COUNT_LARGE);
}

CSAPIConfig::~CSAPIConfig()
{
	DeleteAll();
}

SAPIRESULTCODE CSAPIConfig::SP_Init()
{
	SAPI_ASSERT(!m_pSapiConfig);
	m_pSapiConfig = new TSAPIConfig;
	SAPI_ASSERT(m_pSapiConfig);
	memset(m_pSapiConfig,0,sizeof(TSAPIConfig));
	return SAPI_OK;
}

SAPIRESULTCODE	CSAPIConfig::SP_New(char* lpConfigName)
{
	SAPIRESULTCODE iRet;
	DeleteAll();
	iRet = SP_Init();
	if(SAPI_OK != iRet)
	{
		return iRet;
	}
	SAPI_ASSERT(strlen(lpConfigName) < MAX_STRING_COUNT_HUGE);
	strncpy(m_szConfigFile,lpConfigName,MAX_STRING_COUNT_HUGE-1);

	return iRet;
}

SAPIRESULTCODE	CSAPIConfig::SP_Open(char* lpConfigName)
{
	SAPIRESULTCODE iRet;
	DeleteAll();

	iRet = SP_Init();
	if(SAPI_OK != iRet)
	{
		return iRet;
	}

	SAPI_ASSERT(strlen(lpConfigName) < MAX_STRING_COUNT_HUGE);
	strncpy(m_szConfigFile,lpConfigName,MAX_STRING_COUNT_HUGE-1);
	
	SAPI_DB_PRINTF("SP_Open---m_szConfigFile = %s\r\n",m_szConfigFile);
	iRet = __SP_Read();
	if(SAPI_OK != iRet)
	{
		return iRet;
	}

	return iRet;
}

SAPIRESULTCODE	CSAPIConfig::SP_Save()
{
	if(!m_szConfigFile[0])
		return SAPI_NOEXIST_CONFIG_FILE;

	//open file
	FILE* fp  = fopen(m_szConfigFile,"wt");
	if(!fp)
		return SAPI_NOEXIST_CONFIG_FILE;

	//main
	__SP_SaveMainInfo(fp);

	//ASR
	__SP_SaveASRRules(fp);

	//TTSText
	__SP_SaveTTSText(fp);

	//Custom voice
	__SP_SaveCustomVoice(fp);

	//Tag Control Codes
	__SP_SaveTagControlCodes(fp);

	fclose(fp);
	return SAPI_OK;
}

bool CSAPIConfig::__SP_SaveMainInfo(FILE* fp)
{
	fprintf(fp,"[MAIN]\r\n");
	PConfigMainInfo pMainInfo = &(m_pSapiConfig->tConfigMainInfo);
	
	for(int i = 0; i < en_MainInfo_Count; i++)
	{
		if(pMainInfo->lpString[i])
		{
			fprintf(fp,"%s=%s\r\n",g_lpMainInfoTag[i],pMainInfo->lpString[i]);
		}		
	}

	if(pMainInfo->bAsrBeep)
	{
		fprintf(fp,"Beep=yes\r\n");
	}else
	{
		fprintf(fp,"Beep=no\r\n");
	}

	fprintf(fp,"\r\n\r\n\r\n");
	return true;
}

bool CSAPIConfig::__SP_SaveASRRules(FILE* fp)
{
	fprintf(fp,"[ASR RULE]\r\n");	
	int nTotalCount = SP_GetRuleItemCount();
	for( int i = 0; i < nTotalCount; i++)
	{
		PASRRuleItem pASRItem = SP_GetRuleItem(i);		
		fprintf(fp,"VarName=%s",pASRItem->szRuleName);
		if(pASRItem->lpASRLanguage)
		{
			fprintf(fp,",%s",pASRItem->lpASRLanguage);
		}
		fprintf(fp,"\r\n");		
		fprintf(fp,"%s=%s",pASRItem->szRuleName,pASRItem->lpContext);		
		fprintf(fp,"\r\n");
	}	
	
	fprintf(fp,"\r\n\r\n\r\n");
	return true;
}

bool CSAPIConfig::__SP_SaveTTSText(FILE* fp)
{
	fprintf(fp,"[TTS TEXT]\r\n");
	int nTotalCount = SP_GetTTSTextItemCount();
	for( int i = 0; i < nTotalCount; i++)
	{
		PTTSTextItem pItem = SP_GetTTSTextItem(i);
		fprintf(fp,"VarName=%s",pItem->szTTSName);
		if(pItem->lpTTSLanguage)
		{
			fprintf(fp,",%s",pItem->lpTTSLanguage);
		}
		if(pItem->bisFile)
		{
			fprintf(fp,",File");
		}
		fprintf(fp,"\r\n");
		fprintf(fp,"%s=%s",pItem->szTTSName,pItem->lpText);		
		fprintf(fp,"\r\n");
	}
	
	fprintf(fp,"\r\n\r\n\r\n");
	return true;
}

bool CSAPIConfig::__SP_SaveCustomVoice(FILE* fp)
{
	fprintf(fp,"[CUSTOM VOICE]\r\n");
	int nTotalCount = SP_GetCustomItemCount();
	for( int i = 0; i < nTotalCount; i++)
	{
		PCustomvoiceItem pItem = SP_GetCustomItem(i);
		fprintf(fp,"VarName=%s\r\n",pItem->szCustomName);
		fprintf(fp,"%s=%s\r\n",pItem->szCustomName,pItem->lpCustomFile);
	}
	
	fprintf(fp,"\r\n\r\n\r\n");
	return true;
}

bool CSAPIConfig::__SP_SaveTagControlCodes(FILE* fp,char* lpBuffer)
{
	char* lpW = lpBuffer;
	if(fp)
		fprintf(fp,"[COMMAND RULE]\r\n");


	int nTotalCount = SP_GetTagItemCount();
	for( int i = 0; i < nTotalCount; i++)
	{
		PTagFunctionItem pTagItem = SP_GetTagItem(i);
		if(fp)
		{
			fprintf(fp,"Tag:%s\r\n",pTagItem->szTagName);
		}
		if(lpW)
		{
			sprintf(lpW,"Tag:%s\r\n",pTagItem->szTagName);
			lpW += strlen(lpW);
		}		

		int nProcCount = SP_GetProcItemCount(pTagItem->pProcRulesItem);
		for(int k = 0; k < nProcCount; k++)
		{
			ProcRulesItem pProcItem = SP_GetProcItem(k,pTagItem->pProcRulesItem);
			if(lpW)
			{
				__SP_SaveProcItem(fp,pProcItem,&lpW);
			}else
			{
				__SP_SaveProcItem(fp,pProcItem,0);
			}
			
		}
		if(fp)
		{
			fprintf(fp,"EndTag\r\n\r\n");
		}
		if(lpW)
		{
			sprintf(lpW,"EndTag\r\n\r\n");
			lpW += strlen(lpW);
		}
		
	}

	if(fp)
	{
		fprintf(fp,"\r\n\r\n\r\n");
	}
	if(lpW)
	{
		sprintf(lpW,"\r\n\r\n\r\n");
		lpW += strlen(lpW);
	}
	
	return true;
}

bool CSAPIConfig::__SP_SaveProcItem(FILE* fp,ProcRulesItem pProcItem,char** lpBuffer)
{	
	__SP_SaveDoItem(fp,&(pProcItem->tDoRuleItem),false,lpBuffer);
	__SP_SaveReusltItem(fp,pProcItem->pResultRuleItem,lpBuffer);
	
	return true;
}

bool CSAPIConfig::__SP_SaveReusltItem(FILE* fp,PResultRuleItem pResultItem,char** lpBuffer)
{
	char* lpW = 0;

	if(lpBuffer)
	{
		lpW = *lpBuffer;
	}	

	int	nTotal = SP_GetResultItemCount(pResultItem);
	if(nTotal <= 0)
		return false;

	if(fp)
	{
		fprintf(fp,"{\r\n");
	}

	if(lpW)
	{
		sprintf(lpW,"{\r\n");
		lpW += strlen(lpW);
	}
	
	for(int i = 0; i < nTotal; i++)
	{
		PResultRuleItem	pItem = SP_GetResultItem(i,pResultItem);
		if(pItem->lpParam)
		{
			if(fp)
			{
				fprintf(fp," %s:%s\r\n",pItem->szRequestName,pItem->lpParam);
			}
			if(lpW)
			{
				sprintf(lpW," %s:%s\r\n",pItem->szRequestName,pItem->lpParam);
				lpW += strlen(lpW);
			}
		}
		else
		{
			if(fp)
			{
				fprintf(fp," %s:\r\n",pItem->szRequestName);
			}
			if(lpW)
			{
				sprintf(lpW," %s:\r\n",pItem->szRequestName);
				lpW += strlen(lpW);
			}
		}

		if(fp)
		{
			fprintf(fp," {\r\n");
		}

		if(lpW)
		{
			sprintf(lpW," {\r\n");
			lpW += strlen(lpW);
		}		
		__SP_SaveDoItem(fp,pItem->pDoRuleItem,true,&lpW);

		if(fp)
		{
			fprintf(fp," }\r\n");
		}
		if(lpW)
		{
			sprintf(lpW," }\r\n");
			lpW += strlen(lpW);
		}		
	}

	if(fp)
	{
		fprintf(fp,"}\r\n");
	}
	if(lpW)
	{
		sprintf(lpW,"}\r\n");
		lpW += strlen(lpW);
	}

	if(lpBuffer && *lpBuffer)
	{
		*lpBuffer = lpW;
	}

	return true;
}

bool CSAPIConfig::__SP_SaveDoItem(FILE* fp,PDoRuleItem pDoItem,bool bhead,char** lpBuffer)
{
	char* lpW = 0;
	if(lpBuffer)
	{
		lpW = *lpBuffer;
	}

	int nTotal = SP_GetDoItemCount(pDoItem);
	for(int i = 0; i < nTotal; i++)
	{
		PDoRuleItem	pDo = SP_GetDoItem(i,pDoItem);
		if(pDo)
		{
			if(bhead)
			{
				if(fp)
				{
					fprintf(fp,"   %s:%s",pDo->szFunction,pDo->szParam);
				}

				if(lpW)
				{
					sprintf(lpW,"   %s:%s",pDo->szFunction,pDo->szParam);
					lpW += strlen(lpW);
				}
			}
			else
			{
				if(fp)
				{
					fprintf(fp,"%s:%s",pDo->szFunction,pDo->szParam);
				}

				if(lpW)
				{
					sprintf(lpW,"%s:%s",pDo->szFunction,pDo->szParam);
					lpW += strlen(lpW);
				}
			}
			
			bool bFirst = false;
			if(pDo->nFlag)
			{
				int k = 0;
				if(fp)
				{
					fprintf(fp,",");
				}
				if(lpW)
				{
					sprintf(lpW,",");
					lpW += strlen(lpW);
				}
				
				for(k=0; k < SAPI_ARRAY_SIZE(g_flagArray); k++)
				{
					if(pDo->nFlag&g_flagArray[k])
					{
						if(!bFirst)
						{
							if(fp)
							{
								fprintf(fp,"%s",g_lpdwflagString[k]);
							}
							if(lpW)
							{
								sprintf(lpW,"%s",g_lpdwflagString[k]);
								lpW += strlen(lpW);
							}
						}else
						{
							if(fp)
							{
								fprintf(fp,"|%s",g_lpdwflagString[k]);
							}
							
							if(lpW)
							{
								sprintf(lpW,"|%s",g_lpdwflagString[k]);
								lpW += strlen(lpW);
							}
						}						
						bFirst = true;						
					}
				}
			}

			if(fp)
			{
				fprintf(fp,"\r\n");
			}
			if(lpW)
			{
				sprintf(lpW,"\r\n");
				lpW += strlen(lpW);
			}			
		}
	}
	
	if(lpBuffer && *lpBuffer)
	{
		*lpBuffer = lpW;
	}
	return true;
}

bool CSAPIConfig::DeleteAll()
{
	if(!m_pSapiConfig)
		return false;

	//////////////////////////////////////////////////////////////////////////
	//delete main info
	int i = 0;
	PConfigMainInfo pMainInfo = &(m_pSapiConfig->tConfigMainInfo);
	for(i = 0; i < en_MainInfo_Count; i++)
	{
		if(pMainInfo->lpString[i])
		{
			delete pMainInfo->lpString[i];
			pMainInfo->lpString[i] = 0;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//delete ASR rules
	for(;SP_GetRuleItemCount() > 0;)
	{
		SP_DeleteRuleItem(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//delete TTS Text Item
	for(;SP_GetTTSTextItemCount() > 0;)
	{
		SP_DeleteTTSTextItem(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//delete Custom voice Item
	for(;SP_GetCustomItemCount() > 0;)
	{
		SP_DeleteCustomItem(0);
	}

	//////////////////////////////////////////////////////////////////////////
	//delete TagItem	
	for(;SP_GetTagItemCount() > 0;)
	{
		SP_DeleteTagItem(0);
	}

	if(m_pSapiConfig)
		delete m_pSapiConfig;
	m_pSapiConfig = 0;
	return true;
}

SAPIRESULTCODE CSAPIConfig::SP_Uninit()
{
	DeleteAll();
	return SAPI_OK;
}

#define START_MAIN_INFO		"main"
#define START_ASR_RULES		"ASR RULE"
#define START_TTS_TEXT		"TTS TEXT"
#define START_CUSTOM_VOICE	"CUSTOM VOICE"
#define START_COMMAND_RULES	"COMMAND RULE"
SAPIRESULTCODE CSAPIConfig::__SP_Read()
{
	SAPI_ASSERT(m_pSapiConfig);
	FILE* fp = fopen(m_szConfigFile,"rt");
	if(!fp)
	{
		SAPI_DB_PRINTF("CSAPIConfig::__SP_Read open file error\r\n");
		return SAPI_NOEXIST_CONFIG_FILE;
	}
	
	fseek(fp,0,SEEK_END);
	int nFileLen = ftell(fp);	
	fseek(fp,0,SEEK_SET);
	char* lpBuffer = new char[nFileLen + 100];
	if(!lpBuffer)
	{
		fclose(fp);
		return SAPI_MALLOC_MEMORY_ERROR;
	}
	memset(lpBuffer,0,nFileLen+100);
	fread(lpBuffer,1,nFileLen + 100, fp);
	fclose(fp);
	
	SAPIRESULTCODE iRet = 0;
	char* lpASRRule;
	char* lpTTSRule;
	char* lpCUstomRule;
	char* lpControlCodeRule;
	char* lpMainInfo = __SP_FindString(START_MAIN_INFO,lpBuffer);
	if(!lpMainInfo)
	{
		iRet = SAPI_READ_MAININFO_ERROR;
		goto READ_ERROR;
	}

	lpASRRule =  __SP_FindString(START_ASR_RULES,lpMainInfo);
	if(!lpASRRule)
	{
		iRet = SAPI_READ_ASRRULES_ERROR;
		goto READ_ERROR;
	}
	*(lpASRRule-1) = 0;

	lpTTSRule =  __SP_FindString(START_TTS_TEXT,lpASRRule);
	if(!lpTTSRule)
	{
		iRet = SAPI_READ_TTSText_ERROR;
		goto READ_ERROR;
	}
	*(lpTTSRule-1) = 0;

	lpCUstomRule =  __SP_FindString(START_CUSTOM_VOICE,lpTTSRule);
	if(!lpCUstomRule)
	{
		iRet = SAPI_READ_CUSTOMVOICE_ERROR;
		goto READ_ERROR;
	}
	*(lpCUstomRule-1) = 0;

	lpControlCodeRule =  __SP_FindString(START_COMMAND_RULES,lpCUstomRule);
	if(!lpControlCodeRule)
	{
		iRet = SAPI_READ_CONTROLCODES_ERROR;
		goto READ_ERROR;
	}
	*(lpControlCodeRule-1) = 0;

	//read main info	
	iRet = __SP_ReadMainInfo(lpMainInfo);
	if(SAPI_OK != iRet)
		goto READ_ERROR;

	//read asr rules info
	iRet = __SP_ReadASRRules(lpASRRule);
	if(SAPI_OK != iRet)
		goto READ_ERROR;

	//read tts text info
	iRet = __SP_ReadTTSRules(lpTTSRule);
	if(SAPI_OK != iRet)
		goto READ_ERROR;

	//read custom voice info
	iRet = __SP_ReadCUSTOMRules(lpCUstomRule);
	if(SAPI_OK != iRet)
		goto READ_ERROR;

	//read control tag info
	iRet = __SP_ReadTAGRules(lpControlCodeRule);
	if(SAPI_OK != iRet)
		goto READ_ERROR;

	delete lpBuffer;
	lpBuffer = 0;
	return SAPI_OK;

READ_ERROR:
	delete lpBuffer;
	lpBuffer = 0;
	return iRet;
}

char* CSAPIConfig::__SP_IngoreLine(char* lpSrc)
{
	char* lpCur = lpSrc;	
	for(;lpCur && lpCur[0]; lpCur++)
	{
		char c = lpCur[0];
		if(c == '\r' || c == '\n')
		{
			return __SP_SkipWhiteSpace(lpCur);			
		}
	}
	
	return 0;
}

char*	CSAPIConfig::__SP_GetOverLine(char* lpBuffer)
{
	char* lpCur = lpBuffer;
	for(;lpCur && lpCur[0]; lpCur++)
	{
		char c1 = lpCur[0];
		char c2 = lpCur[1];
		if(c1 == '/' && c2 == '/')
		{
			return lpCur;
		}

		if(c1 == 0 || c1 == '\r' || c1 == '\n')
		{
			return lpCur;
		}
	}

	return 0;
}

char* CSAPIConfig::__SP_GetStartTag(char* lpBuffer,char* lpTagName,int nNamelen)
{
	char* lpCur = lpBuffer;
	memset(lpTagName,0,nNamelen);

	for(;lpCur && lpCur[0];)
	{
		lpCur = __SP_SkipWhiteSpace(lpCur);
		if(!lpCur) return 0;
		lpCur = __SP_SkipLine(lpCur);
		if(!lpCur) return 0;

		//Tag:xxxx
		char szTagName[5];
		strncpy(szTagName,lpCur,4);
		szTagName[4] = 0;
		if(0 != stricmp(szTagName,"tag:"))
		{
			lpCur = __SP_IngoreLine(lpCur);
			continue;
		}
		lpCur += 4;

		lpCur = __SP_SkipWhiteSpace(lpCur);
		char* lpS = lpCur;
		lpCur = __SP_GetOverLine(lpCur);
		if(!lpCur)
		{
			return 0;
		}
		int n = lpCur - lpS + 1;
		if(n > (nNamelen -1))
			n = nNamelen - 1;
		
		strncpy(lpTagName,lpS,n);
		__SP_BreakWhiteSpace(lpTagName);
		return __SP_SkipLine(lpCur);
	}
	
	return 0;
}

char* CSAPIConfig::__SP_GetDoRule(char* lpBuffer,PDoRuleItem pDoRuleItem,bool* isEndtag)
{
	char* lpCur = lpBuffer;
	*isEndtag = false;
	memset(pDoRuleItem,0,sizeof(TDoRuleItem));

	for(;lpCur && lpCur[0];)
	{
		lpCur = __SP_SkipWhiteSpace(lpCur);
		if(!lpCur) return 0;
		lpCur = __SP_SkipLine(lpCur);
		if(!lpCur) return 0;
		
		//NAME
		//xxx:
		int i = 0;
		char* lpS = lpCur;
		char* lpE = 0;
		for( ;lpCur && lpCur[0]; lpCur++)
		{
			char c = lpCur[0];
			if(c == ':' || c == ' ' || c == '\t' || c == '\n' || c == '\r')
			{
				lpCur++;
				break;
			}
			lpE = lpCur;
		}
		if(!lpE)
		{
			return 0;
		}
		int n = lpE - lpS + 1;
		if(n > (MAX_STRING_COUNT_SMALL-1))
			n = MAX_STRING_COUNT_SMALL-1;
		strncpy(pDoRuleItem->szFunction,lpS,n);
		__SP_BreakWhiteSpace(pDoRuleItem->szFunction);
		if(0 == stricmp(pDoRuleItem->szFunction,"EndTag"))
		{
			*isEndtag = true;
			return lpCur;
		}
		if(0 == stricmp(pDoRuleItem->szFunction,"}"))
		{
			*isEndtag = true;
			return lpCur;
		}


		//param1
		bool bhaveFlag = false;
		lpCur = __SP_SkipWhiteSpace(lpCur);
		if(!lpCur) return 0;
		lpS = lpCur;
		lpE = 0;
		for( ;lpCur && lpCur[0]; lpCur++)
		{
			char c = lpCur[0];
			char c1 = lpCur[0];
			char c2 = lpCur[1];
			if(c == ',' || c == '\r' || c == '\n')
			{
				if(c == ',') bhaveFlag = true;
				lpCur++;
				break;
			}

			if(c1 == '/' && c2 == '/')
			{
				bhaveFlag = false;
				lpCur = __SP_SkipLine(lpCur);
				break;
			}
			lpE = lpCur;
		}
		if(!lpE)
		{
			return 0;
		}
		n = lpE - lpS + 1;
		if(n > (MAX_STRING_COUNT_SMALL-1))
			n = MAX_STRING_COUNT_SMALL-1;
		strncpy(pDoRuleItem->szParam,lpS,n);
		__SP_BreakWhiteSpace(pDoRuleItem->szParam);

		//dwflag
		bool bBreak = true;
		for(;bhaveFlag && bBreak && lpCur && lpCur[0];)
		{
			char szTemp[MAX_STRING_COUNT_LARGE];
			memset(szTemp,0,MAX_STRING_COUNT_SMALL);
			lpCur = __SP_SkipWhiteSpace(lpCur);
			if(!lpCur) return 0;
			lpS = lpCur;
			lpE = 0;
			for( ;lpCur && lpCur[0]; lpCur++)
			{
				char c = lpCur[0];
				if(c == '|' || c == '\r' || c == '\n')
				{
					if(c != '|')
					{
						bBreak = false;
					}
					lpCur++;
					break;
				}
				lpE = lpCur;
			}
			if(!lpE)
			{
				return 0;
			}
			n = lpE - lpS + 1;
			if(n > (MAX_STRING_COUNT_SMALL-1))
				n = MAX_STRING_COUNT_SMALL-1;
			strncpy(szTemp,lpS,n);
			__SP_BreakWhiteSpace(szTemp);
			int nIdxString = -1;
			for(int m = 0; m < SAPI_ARRAY_SIZE(g_lpdwflagString);m++)
			{
				if(0 == stricmp(g_lpdwflagString[m],szTemp))
				{
					nIdxString = m;
					break;
				}
			}

			if(nIdxString >= 0)
			{
				pDoRuleItem->nFlag |= (0x0001<<(nIdxString+1));
			}
		}//end for	

		lpCur = __SP_SkipWhiteSpace(lpCur);
		return __SP_SkipLine(lpCur);
	}

	return 0;
}

char* CSAPIConfig::__SP_GetResultRule(char* lpBuffer,PResultRuleItem* ppResultItem)
{
	*ppResultItem = 0;
	char* lpCur = lpBuffer;
	lpCur = __SP_SkipWhiteSpace(lpCur);
	if(!lpCur) return 0;
	lpCur = __SP_SkipLine(lpCur);
	if(!lpCur) return 0;

	//·¢ÏÖ£û£ý
	char c = lpCur[0];
	if(c != '{')
		return lpCur;
	lpCur++;
	
	for(;;)
	{		
		lpCur = __SP_SkipWhiteSpace(lpCur);
		if(!lpCur) return 0;
		lpCur = __SP_SkipLine(lpCur);
		if(!lpCur) return 0;
		
		TResultRuleItem tResultItem;
		memset(&tResultItem,0,sizeof(TResultRuleItem));

		//xxx:NAME
		int i = 0;
		char* lpS = lpCur;
		char* lpE = 0;
		for( ;lpCur && lpCur[0]; lpCur++)
		{
			c = lpCur[0];
			if(c == ':' || c == ' ' || c == '\t' || c == '\n' || c == '\r')
			{				
				lpCur++;
				break;
			}
			lpE = lpCur;
		}
		if(!lpE)
		{
			return 0;
		}
		int n = lpE - lpS + 1;
		if(n > (MAX_STRING_COUNT_SMALL-1))
			n = MAX_STRING_COUNT_SMALL-1;
		strncpy(tResultItem.szRequestName,lpS,n);
		__SP_BreakWhiteSpace(tResultItem.szRequestName);
		if(0 == stricmp(tResultItem.szRequestName,"}"))
		{
			break;
		}
		
		//param
		if(0 != stricmp(tResultItem.szRequestName,"DEFAULT"))
		{
			lpCur = __SP_SkipWhiteSpace(lpCur);
			if(!lpCur) return 0;
			lpS = lpCur;
			lpE = 0;
			for( ;lpCur && lpCur[0]; lpCur++)
			{
				char c = lpCur[0];
				if(c == '\r' || c == '\n')
				{					
					break;
				}
				lpE = lpCur;
			}
			if(!lpE)
			{
				return 0;
			}
			n = lpE - lpS + 1;
			tResultItem.lpParam = new char[n+10];
			memset(tResultItem.lpParam,0,n+10);
			strncpy(tResultItem.lpParam,lpS,n);
			__SP_BreakWhiteSpace(tResultItem.lpParam);
		}

		lpCur = __SP_SkipWhiteSpace(lpCur);
		if(!lpCur) return 0;
		lpCur = __SP_SkipLine(lpCur);
		if(!lpCur) return 0;
		c = lpCur[0];
		SAPI_ASSERT( c == '{' );
			lpCur ++;
		for(;;)
		{
			lpCur = __SP_SkipWhiteSpace(lpCur);
			if(!lpCur) return 0;
			lpCur = __SP_SkipLine(lpCur);
			if(!lpCur) return 0;
			
			TDoRuleItem tDoItem;
			bool bisEndTag = false;
			lpCur = __SP_GetDoRule(lpCur,&tDoItem,&bisEndTag);
			if(bisEndTag)
			{
				break;
			}
			__SP_InsertSapiItem(-1,&tDoItem,&(tResultItem.pDoRuleItem));
			
		}//end for
		__SP_InsertSapiItem(-1,&tResultItem,ppResultItem);
	}
	
	return __SP_SkipLine(lpCur);
}

SAPIRESULTCODE	CSAPIConfig::__SP_ReadTAGRules(char* lpBuffer)
{
	char* lpCur = lpBuffer;
	for(;lpCur && lpCur[0];)
	{
		TagFunctionItem tTag;
		memset(&tTag,0,sizeof(TagFunctionItem));

		//start
		lpCur = __SP_GetStartTag(lpCur,tTag.szTagName,MAX_STRING_COUNT_SMALL);
		if(!lpCur)
			break;
		
		for(;;)
		{
			TProcRulesItem tProcItem;
			memset(&tProcItem,0,sizeof(TProcRulesItem));

			//get do rule
			bool bisEndTag = false;
			lpCur = __SP_GetDoRule(lpCur,&(tProcItem.tDoRuleItem),&bisEndTag);
			if(bisEndTag)
			{
				break;
			}

			if(!lpCur)
			{
				SAPI_ASSERT(false);
				return SAPI_READ_CONTROLCODES_ERROR;
			}
			
			//get result		
			lpCur = __SP_GetResultRule(lpCur,&(tProcItem.pResultRuleItem));
			__SP_InsertSapiItem(-1,&tProcItem, &(tTag.pProcRulesItem));
		}		

		SP_InsertTagItem(-1, &tTag);
	}
	
	return SAPI_OK;
}

SAPIRESULTCODE	CSAPIConfig::__SP_ReadCUSTOMRules(char* lpBuffer)
{
	char* lpCur = lpBuffer;
	char  szName[MAX_STRING_COUNT_SMALL];
	char* lpParam1 = 0;
	char* lpParam2 = 0;	
	
	for( ; ; )
	{
		TCustomvoiceItem tvoice;
		memset(&tvoice,0,sizeof(TCustomvoiceItem));
		
		lpParam1 = 0;
		lpParam2 = 0;
		lpCur = __SP_ParseRule(lpCur,szName,MAX_STRING_COUNT_SMALL,&lpParam1,0);
		if(!lpCur)
		{
			if(lpParam1)
				delete lpParam1;
			if(lpParam2)
				delete lpParam2;
			break;
		}
		
		if(0 != stricmp(szName,"VarName"))
		{
			SAPI_ASSERT(FALSE);
			return SAPI_READ_ASRRULES_ERROR;
		}
		SAPI_ASSERT(lpParam1 && lpParam1[0]);
		if(!lpParam1 || !lpParam1[0])
		{
			if(lpParam1) delete lpParam1;
			return SAPI_READ_ASRRULES_ERROR;
		}		
		strcpy(tvoice.szCustomName,lpParam1);	
		delete lpParam1;
		lpParam1 = 0;
		
		//////////////////////////////////////////////////////////////////////////
		lpCur = __SP_ParseRule(lpCur,szName,MAX_STRING_COUNT_SMALL,&lpParam1,0);
		if(!lpCur)
		{
			SAPI_ASSERT(FALSE);
			return SAPI_READ_ASRRULES_ERROR;
		}
		if(0 != stricmp(tvoice.szCustomName,szName))
		{
			if(lpParam1) delete lpParam1;			
			SAPI_ASSERT(FALSE);
			return SAPI_READ_ASRRULES_ERROR;
		}
		SAPI_ASSERT(lpParam1 && lpParam1[0]);
		if(!lpParam1 || !lpParam1[0])
		{
			if(lpParam1) delete lpParam1;
			return SAPI_READ_ASRRULES_ERROR;
		}
		tvoice.lpCustomFile = lpParam1;
		
		//////////////////////////////////////////////////////////////////////////
		if(!SP_InsertCustomItem(-1,&tvoice))
		{
			delete tvoice.lpCustomFile;
			return SAPI_READ_ASRRULES_ERROR;
		}
	}
	
	return SAPI_OK;
}

SAPIRESULTCODE	CSAPIConfig::__SP_ReadTTSRules(char* lpBuffer)
{
	char* lpCur = lpBuffer;
	char  szName[MAX_STRING_COUNT_SMALL];
	char* lpParam1 = 0;
	char* lpParam2 = 0;	
	
	for( ; ; )
	{
		TTSTextItem tTtsItem;
		memset(&tTtsItem,0,sizeof(TTSTextItem));
		
		lpParam1 = 0;
		lpParam2 = 0;
		lpCur = __SP_ParseRule(lpCur,szName,MAX_STRING_COUNT_SMALL,&lpParam1,&lpParam2);
		if(!lpCur)
		{
			if(lpParam1)
				delete lpParam1;
			if(lpParam2)
				delete lpParam2;
			break;
		}
		
		if(0 != stricmp(szName,"VarName"))
		{
			SAPI_ASSERT(FALSE);
			return SAPI_READ_ASRRULES_ERROR;
		}
		SAPI_ASSERT(lpParam1 && lpParam1[0]);
		if(!lpParam1 || !lpParam1[0])
		{
			if(lpParam1) delete lpParam1;
			return SAPI_READ_ASRRULES_ERROR;
		}		
		strcpy(tTtsItem.szTTSName,lpParam1);
		if(lpParam2)
		{
			char* lp = strstr(lpParam2,",");
			if(lp) 
			{				
				*lp = 0;
				__SP_BreakWhiteSpace(lpParam2);
				lp++;
				__SP_BreakWhiteSpace(lp);
				if(0 == stricmp(lp,"File"))
				{
					tTtsItem.bisFile = true;
				}else
				{
					tTtsItem.bisFile = false;
				}
			}
		}
		tTtsItem.lpTTSLanguage = lpParam2;
		delete lpParam1;
		lpParam1 = 0;
		
		//////////////////////////////////////////////////////////////////////////
		lpCur = __SP_ParseRule(lpCur,szName,MAX_STRING_COUNT_SMALL,&lpParam1,0);
		if(!lpCur)
		{
			SAPI_ASSERT(FALSE);
			return SAPI_READ_ASRRULES_ERROR;
		}
		if(0 != stricmp(tTtsItem.szTTSName,szName))
		{
			if(lpParam1) delete lpParam1;			
			SAPI_ASSERT(FALSE);
			return SAPI_READ_ASRRULES_ERROR;
		}
		SAPI_ASSERT(lpParam1 && lpParam1[0]);
		if(!lpParam1 || !lpParam1[0])
		{
			if(lpParam1) delete lpParam1;
			return SAPI_READ_ASRRULES_ERROR;
		}
		tTtsItem.lpText = lpParam1;
		
		//////////////////////////////////////////////////////////////////////////
		if(!SP_InsertTTSTextItem(-1,&tTtsItem))
		{
			delete tTtsItem.lpTTSLanguage;
			delete tTtsItem.lpText;
			return SAPI_READ_ASRRULES_ERROR;
		}
	}
	
	return SAPI_OK;
}

SAPIRESULTCODE	CSAPIConfig::__SP_ReadASRRules(char* lpBuffer)
{
	char* lpCur = lpBuffer;
	char  szName[MAX_STRING_COUNT_SMALL];
	char* lpParam1 = 0;
	char* lpParam2 = 0;	
	
	for( ; ; )
	{
		TASRRuleItem tAsrRuleItem;
		memset(&tAsrRuleItem,0,sizeof(TASRRuleItem));

		lpParam1 = 0;
		lpParam2 = 0;
		lpCur = __SP_ParseRule(lpCur,szName,MAX_STRING_COUNT_SMALL,&lpParam1,&lpParam2);
		if(!lpCur)
		{
			if(lpParam1)
				delete lpParam1;
			if(lpParam2)
				delete lpParam2;
			break;
		}

		if(0 != stricmp(szName,"VarName"))
		{
			SAPI_ASSERT(FALSE);
			return SAPI_READ_ASRRULES_ERROR;
		}
		SAPI_ASSERT(lpParam1 && lpParam1[0]);
		if(!lpParam1 || !lpParam1[0])
		{
			if(lpParam1) delete lpParam1;
			return SAPI_READ_ASRRULES_ERROR;
		}		
		strcpy(tAsrRuleItem.szRuleName,lpParam1);
		tAsrRuleItem.lpASRLanguage = lpParam2;
		delete lpParam1;
		lpParam1 = 0;

		//////////////////////////////////////////////////////////////////////////
		lpCur = __SP_ParseRule(lpCur,szName,MAX_STRING_COUNT_SMALL,&lpParam1,0);
		if(!lpCur)
		{
			SAPI_ASSERT(FALSE);
			return SAPI_READ_ASRRULES_ERROR;
		}
		if(0 != stricmp(tAsrRuleItem.szRuleName,szName))
		{
			if(lpParam1) delete lpParam1;			
			SAPI_ASSERT(FALSE);
			return SAPI_READ_ASRRULES_ERROR;
		}
		SAPI_ASSERT(lpParam1 && lpParam1[0]);
		if(!lpParam1 || !lpParam1[0])
		{
			if(lpParam1) delete lpParam1;
			return SAPI_READ_ASRRULES_ERROR;
		}
		tAsrRuleItem.lpContext = lpParam1;

		//////////////////////////////////////////////////////////////////////////
		if(!SP_InsertRuleItem(-1,&tAsrRuleItem))
		{
			delete tAsrRuleItem.lpASRLanguage;
			delete tAsrRuleItem.lpContext;
			return SAPI_READ_ASRRULES_ERROR;
		}
	}

	return SAPI_OK;
}

char* CSAPIConfig::__SP_BreakHeadRetun(char* lpSrc)
{
	char* lpCur = lpSrc;
	for(;lpCur && lpCur[0];lpCur++)
	{
		char c = lpCur[0];
		if( c != '\r' && c != '\n' )
		{
			return lpCur;
		}
	}

	return 0;
}

void CSAPIConfig::__SP_BreakWhiteSpace(char* lpBuf)
{
	char* lpCur = lpBuf;
	for(;lpCur && lpCur[0]; lpCur++)
	{
		char c = lpCur[0];
		if(c != ' ' && c != '\t' && c != '\r' && c != '\n')
		{
			break;
		}
	}

	if(lpCur)
	{
		strcpy(lpBuf,lpCur);
		if(strlen(lpBuf) <= 0)
		{
			return;
		}
	}
	else
	{
		lpBuf[0] = 0;
		return;
	}

	lpCur = lpBuf + strlen(lpBuf) - 1;
	for(;lpCur != lpBuf && lpCur && lpCur[0];lpCur--)
	{
		char c = lpCur[0];
		if(c == ' ' || c == '\t' || c == '\r' || c == '\n')
		{
			lpCur[0] = 0;			
		}else
		{
			break;
		}
	}
}


char* CSAPIConfig::__SP_GetString(char* lpSrc,char* lpStartTag, char* lpEndTag,char* lpBuf,int nBufLen)
{
	char* lpS = strstr(lpSrc,lpStartTag);
	if(!lpS) return 0;
	lpS += strlen(lpStartTag);

	char* lpE = strstr(lpS,lpEndTag);
	if(!lpE) return 0;
	int nReadlen = lpE - lpS;
	lpE += strlen(lpEndTag);
	if(nReadlen >= (nBufLen-1))
		nReadlen = nBufLen-1;

	memset(lpBuf,0,nBufLen);
	strncpy(lpBuf,lpS,nReadlen);
	__SP_BreakWhiteSpace(lpBuf);
	return lpE;
}

char* CSAPIConfig::__SP_FindString(char* lpString,char* lpBuf)
{
	char* lp = lpBuf;
	char szTemp[MAX_STRING_COUNT_SMALL];
	for(;;)
	{
		lp = __SP_GetString(lp,"[", "]",szTemp,MAX_STRING_COUNT_SMALL);
		if(!lp)
			return 0;

		if(0 == stricmp(lpString,szTemp))
		{
			return lp;
		}
	}

	SAPI_ASSERT(false);
	return 0;
}

char*	CSAPIConfig::__SP_SkipWhiteSpace(char* lpSrc)
{
	char* lpCur = lpSrc;
	for(;lpCur && lpCur[0]; lpCur++)
	{
		char c = lpCur[0];
		if(c != ' ' && c != '\t' && c != '\r' && c != '\n')
		{
			return lpCur;
		}
	}

	//SAPI_ASSERT(false);
	return 0;
}

char*	CSAPIConfig::__SP_SkipLine(char* lpSrc)
{
	char* lpCur = lpSrc;
	if(lpCur[0] == '/' && lpCur[1] == '/')
	{
		for(;lpCur && lpCur[0];lpCur++)
		{
			char c = lpCur[0];
			if(c == '\n' || c == '\r')
			{
				lpCur = __SP_SkipWhiteSpace(lpCur);
				break;
			}
		}
	}
	return lpCur;
}

char*	CSAPIConfig::__SP_ParseRule(char* lpSrc,char* lpName,int nNameLen,char** lparam1,char** lparam2)
{
	*lparam1 = 0;
	if(lparam2)
		*lparam2 = 0;
	char* lpCur = __SP_SkipWhiteSpace(lpSrc);
	if(!lpCur) return 0;
	lpCur = __SP_SkipLine(lpCur);
	if(!lpCur) return 0;
	memset(lpName,0,nNameLen);

	//Get name var
	int i = 0;
	for(;lpCur && lpCur[0]; lpCur++)
	{
		char c = lpCur[0];		
		if( c == '=')
		{
			lpCur++;
			break;
		}

		lpName[i++] = lpCur[0];
	}
	__SP_BreakWhiteSpace(lpName);
	SAPI_ASSERT(lpCur);
	if(!lpCur || !lpCur[0]) 
		return 0;
	
	//Get Param 1
	if(0 != stricmp(lpName,"VarName") || 0 == lparam2)
	{
		char* lpStart = lpCur;
		char* lpEnd = 0;
		for( ;lpCur && lpCur[0]; lpCur++ )
		{
			char c1 = lpCur[0];
			char c2 = lpCur[1];

			if( c1 == '/' && c2 == '/' )
			{
				lpCur = __SP_SkipLine(lpCur);
				break;
			}

			if(c1 == '\r' || c1 == '\n')
				break;
			lpEnd = lpCur;
		}

		int nTotal = lpEnd - lpStart + 1;
		*lparam1 = new char[nTotal + 10];
		memset(*lparam1,0,nTotal + 10);
		strncpy(*lparam1,lpStart,nTotal);
		__SP_BreakWhiteSpace(*lparam1);
		return lpCur;
	}else
	{
		char* lpStart = lpCur;
		char* lpEnd = 0;
		for(;lpCur&&lpCur[0];lpCur++)
		{
			char c = lpCur[0];
			if(c == '\r'||c == '\n'|| c== ',')
			{
				if(c == ',')
					lpCur++;
				break;
			}
			lpEnd = lpCur;
		}
		if(!lpEnd)
			return 0;

		int nTotal = lpEnd - lpStart + 1;
		*lparam1 = new char [nTotal + 10];
		memset(*lparam1,0,nTotal + 10);
		strncpy(*lparam1,lpStart,nTotal);
		__SP_BreakWhiteSpace(*lparam1);
		
		
		lpStart = lpCur;
		lpEnd = 0;
		for( ;lpCur && lpCur[0]; lpCur++ )
		{
			char c1 = lpCur[0];
			char c2 = lpCur[1];
			
			if( c1 == '/' && c2 == '/' )
			{
				lpCur = __SP_SkipLine(lpCur);
				break;
			}
			
			if(c1 == '\r' || c1 == '\n')
				break;
			lpEnd = lpCur;
		}
		
		nTotal = lpEnd - lpStart + 1;
		*lparam2 = new char[nTotal + 10];
		memset(*lparam2,0,nTotal + 10);
		strncpy(*lparam2,lpStart,nTotal);
		__SP_BreakWhiteSpace(*lparam2);
		return lpCur;
	}

	return 0;
}

SAPIRESULTCODE	CSAPIConfig::__SP_ReadMainInfo(char* lpBuffer)
{
	char* lpCur = lpBuffer;
	PConfigMainInfo pMainInfo = &(m_pSapiConfig->tConfigMainInfo);
	char szName[MAX_STRING_COUNT_SMALL];
	char* lpParam1 = 0;

	int nTotalItem = SAPI_ARRAY_SIZE(g_lpMainInfoTag);
	for( ; ; )
	{
		lpParam1 = 0;
		lpCur = __SP_ParseRule(lpCur,szName,MAX_STRING_COUNT_SMALL,&lpParam1,0);
		if(!lpCur)
		{
			break;
		}

		SAPI_ASSERT(lpParam1 && lpParam1[0]);
		if(0 == stricmp(szName,"Beep"))
		{
			if(0 == stricmp(lpParam1,"yes"))
			{
				pMainInfo->bAsrBeep = true;
			}else
			{
				pMainInfo->bAsrBeep = false;
			}			
		}else
		{
			for(int k = 0; k < nTotalItem - 1; k++)
			{
				if(0 == stricmp(g_lpMainInfoTag[k],szName))
				{
					pMainInfo->lpString[k] = lpParam1;
					break;
				}
			}
		}
	}

	return SAPI_OK;
}


//Main Info
PConfigMainInfo CSAPIConfig::SP_GetMainInfo()
{
	SAPI_ASSERT(m_pSapiConfig);
	return &(m_pSapiConfig->tConfigMainInfo);
	return 0;
}

bool CSAPIConfig::SP_SetMainInfo(PConfigMainInfo pInfo)
{
	SAPI_ASSERT(m_pSapiConfig);
	return false;
}

//ASR Rules
int	CSAPIConfig::SP_GetRuleItemCount()
{
	return __SP_GetSapiItemCount(m_pSapiConfig->pASRRuleItem);
}

PASRRuleItem CSAPIConfig::SP_GetRuleItem(int idx)
{
	return  __SP_GetSapiItem(idx, m_pSapiConfig->pASRRuleItem);	
}

bool CSAPIConfig::SP_SetRuleItem(int idx,PASRRuleItem pAsrItem)
{
	return __SP_SetSapiItem(idx, pAsrItem, m_pSapiConfig->pASRRuleItem);
}

bool CSAPIConfig::SP_InsertRuleItem(int idx,PASRRuleItem pAsrItem)// idx=-1, is add item data
{
	return __SP_InsertSapiItem(idx, pAsrItem, &(m_pSapiConfig->pASRRuleItem));
}

bool CSAPIConfig::SP_DeleteRuleItem(int idx)
{
	return __SP_DeleteSapiItem(idx, &(m_pSapiConfig->pASRRuleItem));
}

PASRRuleItem	CSAPIConfig::SP_GetASRName(char* lpName)
{
	int i = 0;
	int nCount = SP_GetRuleItemCount();
	for(i = 0; i < nCount; i++)
	{
		PASRRuleItem pItem = SP_GetRuleItem(i);
		if( 0 == stricmp(pItem->szRuleName,lpName) )
		{
			return pItem;
		}
	}
	
	return 0;
}

bool CSAPIConfig::SP_CreateSvoxGram(char* lpGramFile,char* lpLanguage)
{
	PConfigMainInfo pMaininfo = SP_GetMainInfo();
	if(!pMaininfo)
		return false;

	if(!pMaininfo->lpString[en_MainInfo_AppName])
		return false;

	if(SP_GetRuleItemCount() <= 0)
		return false;

	FILE* fp = fopen(lpGramFile,"wt");
	if(!fp)
		return false;
	
	fprintf(fp,"#JSGF V1.0 UTF-8 %s;\r\n",lpLanguage);
	fprintf(fp,"grammar %s;\r\n",pMaininfo->lpString[en_MainInfo_AppName]);

	for(int i = 0; i < SP_GetRuleItemCount(); i++)
	{
		PASRRuleItem pItem = SP_GetRuleItem(i);

		if(0 == stricmp(lpLanguage,pItem->lpASRLanguage))
		{
			fprintf(fp,"public <%s> = %s;\r\n",pItem->szRuleName,pItem->lpContext);
		}
	}

	fclose(fp);
	return true;
}

//TTS Texts
int	CSAPIConfig::SP_GetTTSTextItemCount()
{
	return __SP_GetSapiItemCount(m_pSapiConfig->pTTsTextItem);
}
PTTSTextItem CSAPIConfig::SP_GetTTSTextItem(int idx)
{
	return  __SP_GetSapiItem(idx, m_pSapiConfig->pTTsTextItem);	
}
bool CSAPIConfig::SP_SetTTSTextItem(int idx,PTTSTextItem pItem)
{
	return __SP_SetSapiItem(idx, pItem, m_pSapiConfig->pTTsTextItem);
}
bool CSAPIConfig::SP_InsertTTSTextItem(int idx,PTTSTextItem pItem)
{
	return __SP_InsertSapiItem(idx, pItem, &(m_pSapiConfig->pTTsTextItem));
}
bool CSAPIConfig::SP_DeleteTTSTextItem(int idx)
{
	return __SP_DeleteSapiItem(idx, &(m_pSapiConfig->pTTsTextItem));
}

PTTSTextItem CSAPIConfig::SP_GetTTSName(char* lpName)
{
	int i = 0;
	int nCount = SP_GetTTSTextItemCount();
	for(i = 0; i < nCount; i++)
	{
		PTTSTextItem pItem = SP_GetTTSTextItem(i);
		if( 0 == stricmp(pItem->szTTSName,lpName) )
		{
			return pItem;
		}
	}

	return 0;
}

//Custom voices
int	CSAPIConfig::SP_GetCustomItemCount()
{
	return __SP_GetSapiItemCount(m_pSapiConfig->pCustomVoiceItem);
}

PCustomvoiceItem CSAPIConfig::SP_GetCustomItem(int idx)
{
	return  __SP_GetSapiItem(idx, m_pSapiConfig->pCustomVoiceItem);	
}

bool CSAPIConfig::SP_SetCustomItem(int idx,PCustomvoiceItem pItem)
{
	return __SP_SetSapiItem(idx, pItem, m_pSapiConfig->pCustomVoiceItem);
}

bool CSAPIConfig::SP_InsertCustomItem(int idx,PCustomvoiceItem pItem)
{
	return __SP_InsertSapiItem(idx, pItem, &(m_pSapiConfig->pCustomVoiceItem));
}

bool CSAPIConfig::SP_DeleteCustomItem(int idx)
{
	return __SP_DeleteSapiItem(idx, &(m_pSapiConfig->pCustomVoiceItem));
}
PCustomvoiceItem	CSAPIConfig::SP_GetVoiceName(char* lpName)
{
	int i = 0;
	int nCount = SP_GetCustomItemCount();
	for(i = 0; i < nCount; i++)
	{
		PCustomvoiceItem pItem = SP_GetCustomItem(i);
		if( 0 == stricmp(pItem->szCustomName,lpName) )
		{
			return pItem;
		}
	}
	
	return 0;
}

//Tag Functions
int	CSAPIConfig::SP_GetTagItemCount()
{
	return __SP_GetSapiItemCount(m_pSapiConfig->pTagFunctionItem);
}

PTagFunctionItem CSAPIConfig::SP_GetTagItem(int idx)
{
	return  __SP_GetSapiItem(idx, m_pSapiConfig->pTagFunctionItem);	
}

bool CSAPIConfig::SP_SetTagItem(int idx, PTagFunctionItem pTag)
{
	return __SP_SetSapiItem(idx, pTag, m_pSapiConfig->pTagFunctionItem);
}

bool CSAPIConfig::SP_InsertTagItem(int idx, PTagFunctionItem pTag)
{
	return __SP_InsertSapiItem(idx, pTag, &(m_pSapiConfig->pTagFunctionItem));
}

bool CSAPIConfig::SP_DeleteTagItem(int idx)
{
	PTagFunctionItem pTagItem = SP_GetTagItem(idx);	
	for(; SP_GetProcItemCount(pTagItem->pProcRulesItem) > 0;)
	{
		SP_DeleteProcItem(0,&(pTagItem->pProcRulesItem));
	}//end for

	return __SP_DeleteSapiItem(idx, &(m_pSapiConfig->pTagFunctionItem));
}


//
int	 CSAPIConfig::SP_GetProcItemCount(ProcRulesItem pFirstProcItem)
{
return __SP_GetSapiItemCount(pFirstProcItem);
}

ProcRulesItem	CSAPIConfig::SP_GetProcItem(int idx,ProcRulesItem pFirstProcItem)
{
return  __SP_GetSapiItem(idx, pFirstProcItem);	
}
bool	CSAPIConfig::SP_SetProcItem(int idx,ProcRulesItem pProcItem,ProcRulesItem pFirstProcItem)
{
return __SP_SetSapiItem(idx, pProcItem, pFirstProcItem);
}
bool	CSAPIConfig::SP_InsertProcItem(int idx,ProcRulesItem pProcItem,ProcRulesItem* pFirstProcItem)
{
	return __SP_InsertSapiItem(idx, pProcItem, (pFirstProcItem));
}
bool	CSAPIConfig::SP_DeleteProcItem(int idx,ProcRulesItem* pFirstProcItem)
{
	ProcRulesItem pProcItem = SP_GetProcItem(idx,*pFirstProcItem);
	for(;SP_GetResultItemCount(pProcItem->pResultRuleItem) > 0;)
	{
		SP_DeleteResultItem(0,&(pProcItem->pResultRuleItem));
	}	

	return __SP_DeleteSapiItem(idx, (pFirstProcItem));
}


//
int	CSAPIConfig::SP_GetResultItemCount(PResultRuleItem pFirstProcItem)
{
return __SP_GetSapiItemCount(pFirstProcItem);
}
PResultRuleItem		CSAPIConfig::SP_GetResultItem(int idx,PResultRuleItem pFirstProcItem)
{
return  __SP_GetSapiItem(idx, pFirstProcItem);	
}
bool	CSAPIConfig::SP_SetResultItem(int idx,PResultRuleItem pProcItem,PResultRuleItem pFirstProcItem)
{
return __SP_SetSapiItem(idx, pProcItem, pFirstProcItem);
}
bool	CSAPIConfig::SP_InsertResultItem(int idx,PResultRuleItem pProcItem,PResultRuleItem* pFirstProcItem)
{
return __SP_InsertSapiItem(idx, pProcItem, (pFirstProcItem));
}
bool	CSAPIConfig::SP_DeleteResultItem(int idx,PResultRuleItem* pFirstProcItem)
{
	PResultRuleItem	pResultItem = SP_GetResultItem(idx,*pFirstProcItem);
	for(;SP_GetDoItemCount(pResultItem->pDoRuleItem);)
	{
		SP_DeleteDoItem(0,&(pResultItem->pDoRuleItem));
	}

	return __SP_DeleteSapiItem(idx, (pFirstProcItem));
}


//////////////////////////////////////////////////////////////////////////
int					CSAPIConfig::SP_GetDoItemCount(PDoRuleItem pFirstProcItem)
{
return __SP_GetSapiItemCount(pFirstProcItem);
}
PDoRuleItem		CSAPIConfig::SP_GetDoItem(int idx,PDoRuleItem pFirstProcItem)
{
return  __SP_GetSapiItem(idx, pFirstProcItem);	
}
bool				CSAPIConfig::SP_SetDoItem(int idx,PDoRuleItem pProcItem,PDoRuleItem pFirstProcItem)
{
return __SP_SetSapiItem(idx, pProcItem, pFirstProcItem);
}
bool				CSAPIConfig::SP_InsertDoItem(int idx,PDoRuleItem pProcItem,PDoRuleItem* pFirstProcItem)
{
return __SP_InsertSapiItem(idx, pProcItem, (pFirstProcItem));
}
bool				CSAPIConfig::SP_DeleteDoItem(int idx,PDoRuleItem* pFirstProcItem)
{
return __SP_DeleteSapiItem(idx, (pFirstProcItem));
}
