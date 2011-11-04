// SAPIConfig.h: interface for the CSAPIConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(__SAPI_CONFIG_H)
#define __SAPI_CONFIG_H
#include "SAPItypes.h"

#include <stdio.h>
#define SAPI_DO_RULE_FLAG_BREAK				(0x0001<<1)
#define SAPI_DO_RULE_FLAG_NOBREAK			(0x0001<<2)
#define SAPI_DO_RULE_FLAG_DONOTHING			(0x0001<<3)
#define SAPI_DO_RULE_FLAG_QueryMultResult	(0x0001<<4)
#define SAPI_DO_RULE_FLAG_FistMultResult	(0x0001<<5)
#define SAPI_DO_RULE_FLAG_DoOneResult		(0x0001<<6)
#define SAPI_DO_RULE_FLAG_QueryOneResult	(0x0001<<7)

//define Config main info
typedef enum __tagENMainInfoString
{
	en_MainInfo_SAPIVersion = 0,
	en_MainInfo_AppName,
	en_MainInfo_Version,
	en_MainInfo_Author,
	en_MainInfo_Company,
	en_MainInfo_Email,
	en_MainInfo_Engine,
	en_MainInfo_Count
}ENMainInfoString;
typedef struct __tagTConfigMainInfo
{
	char* lpString[en_MainInfo_Count];//Ãû×Ö£¬used for grammar title.	
	bool bAsrBeep;//When Asr start, do play voice of beep?
}TConfigMainInfo,*PConfigMainInfo;

//define data struct of ASR rule item 
typedef struct __tagTASRRuleItem
{
	char	szRuleName[MAX_STRING_COUNT_SMALL];// ASR grammar's rule name
	char*	lpASRLanguage;// ASR language
	char*	lpContext;
	struct __tagTASRRuleItem* prev;
	struct __tagTASRRuleItem* next;
}TASRRuleItem,*PASRRuleItem;

//define tts data structure
typedef struct __tagTTSTextItem
{
	char szTTSName[MAX_STRING_COUNT_SMALL];//TTS text name
	char* lpTTSLanguage;	//synthetize language
	char* lpText;
	bool  bisFile;//is text: false ,text file:true
	struct __tagTTSTextItem* prev;
	struct __tagTTSTextItem* next;
}TTSTextItem,*PTTSTextItem;

//define Custom voice data structure
typedef struct __tagTCustomvoiceItem
{
	char szCustomName[MAX_STRING_COUNT_SMALL];//voice name	
	char* lpCustomFile;
	struct __tagTCustomvoiceItem* prev;
	struct __tagTCustomvoiceItem* next;
}TCustomvoiceItem,*PCustomvoiceItem;

//define Do Rule data structure
typedef struct __tagTDoRuleItem
{
	char szFunction[MAX_STRING_COUNT_SMALL];// function name
	char szParam[MAX_STRING_COUNT_SMALL];
	int	 nFlag;
	struct __tagTDoRuleItem* prev;
	struct __tagTDoRuleItem* next;
}TDoRuleItem,*PDoRuleItem;

//define result rule data structure
typedef struct __tagTResultRuleItem
{
	char		szRequestName[MAX_STRING_COUNT_SMALL];
	char*		lpParam;
	PDoRuleItem pDoRuleItem;
	struct __tagTResultRuleItem* prev;
	struct __tagTResultRuleItem* next;
}TResultRuleItem,*PResultRuleItem;

typedef struct __tagProcRulesItem
{
	TDoRuleItem			tDoRuleItem;
	PResultRuleItem		pResultRuleItem;

	struct __tagProcRulesItem* prev;
	struct __tagProcRulesItem* next;
}TProcRulesItem,*ProcRulesItem;

typedef struct __tagTTagFunctionItem
{
	char				szTagName[MAX_STRING_COUNT_SMALL];
	ProcRulesItem		pProcRulesItem;	
	struct __tagTTagFunctionItem* prev;
	struct __tagTTagFunctionItem* next;
}TagFunctionItem,*PTagFunctionItem;

typedef struct __tagSAPIConfig
{
	//main info and setting
	TConfigMainInfo tConfigMainInfo;

	//ASR Rules
	PASRRuleItem	pASRRuleItem;

	//TTS Texts
	PTTSTextItem	pTTsTextItem;

	//Custom voices
	PCustomvoiceItem pCustomVoiceItem;

	//Tag Functions
	PTagFunctionItem pTagFunctionItem;
}TSAPIConfig,*PSAPIConfig;

class CSAPIConfig  
{
public:
	CSAPIConfig();
	virtual ~CSAPIConfig();	

	//////////////////////////////////////////////////////////////////////////
	SAPIRESULTCODE	SP_New(char* lpConfigName);
	SAPIRESULTCODE	SP_Open(char* lpConfigName);
	SAPIRESULTCODE	SP_Save();

	//Main Info
	PConfigMainInfo SP_GetMainInfo();
	bool			SP_SetMainInfo(PConfigMainInfo pInfo);

	//ASR Rules
	int				SP_GetRuleItemCount();
	PASRRuleItem	SP_GetRuleItem(int idx);
	bool			SP_SetRuleItem(int idx,PASRRuleItem pAsrItem);
	bool			SP_InsertRuleItem(int idx,PASRRuleItem pAsrItem);// idx=-1, is add item data
	bool			SP_DeleteRuleItem(int idx);
	bool			SP_CreateSvoxGram(char* lpGramFile,char* lpLanguage);
	PASRRuleItem	SP_GetASRName(char* lpName);

	//TTS Texts
	int				SP_GetTTSTextItemCount();
	PTTSTextItem	SP_GetTTSTextItem(int idx);
	bool			SP_SetTTSTextItem(int idx,PTTSTextItem pItem);
	bool			SP_InsertTTSTextItem(int idx,PTTSTextItem pItem);
	bool			SP_DeleteTTSTextItem(int idx);
	PTTSTextItem	SP_GetTTSName(char* lpName);


	//Custom voices
	int					SP_GetCustomItemCount();
	PCustomvoiceItem	SP_GetCustomItem(int idx);
	PCustomvoiceItem	SP_GetVoiceName(char* lpName);
	bool				SP_SetCustomItem(int idx,PCustomvoiceItem pItem);
	bool				SP_InsertCustomItem(int idx,PCustomvoiceItem pItem);
	bool				SP_DeleteCustomItem(int idx);


	//Tag Functions
	int					SP_GetTagItemCount();
	PTagFunctionItem	SP_GetTagItem(int idx);
	bool				SP_SetTagItem(int idx, PTagFunctionItem pTag);
	bool				SP_InsertTagItem(int idx, PTagFunctionItem pTag);
	bool				SP_DeleteTagItem(int idx);

	//ProcRulesItem
	int					SP_GetProcItemCount(ProcRulesItem pFirstProcItem);
	ProcRulesItem		SP_GetProcItem(int idx,ProcRulesItem pFirstProcItem);
	bool				SP_SetProcItem(int idx,ProcRulesItem pProcItem,ProcRulesItem pFirstProcItem);
	bool				SP_InsertProcItem(int idx,ProcRulesItem pProcItem,ProcRulesItem* pFirstProcItem);
	bool				SP_DeleteProcItem(int idx,ProcRulesItem* pFirstProcItem);
	bool				__SP_SaveProcItem(FILE* fp,ProcRulesItem pProcItem,char** lpBuffer = 0);


	//result
	int					SP_GetResultItemCount(PResultRuleItem pFirstProcItem);
	PResultRuleItem		SP_GetResultItem(int idx,PResultRuleItem pFirstProcItem);
	bool				SP_SetResultItem(int idx,PResultRuleItem pProcItem,PResultRuleItem pFirstProcItem);
	bool				SP_InsertResultItem(int idx,PResultRuleItem pProcItem,PResultRuleItem* pFirstProcItem);
	bool				SP_DeleteResultItem(int idx,PResultRuleItem* pFirstProcItem);
	bool				__SP_SaveReusltItem(FILE* fp,PResultRuleItem pResultItem,char** lpBuffer = 0);
	

	//do rule
	int					SP_GetDoItemCount(PDoRuleItem pFirstProcItem);
	PDoRuleItem			SP_GetDoItem(int idx,PDoRuleItem pFirstProcItem);
	bool				SP_SetDoItem(int idx,PDoRuleItem pProcItem,PDoRuleItem pFirstProcItem);
	bool				SP_InsertDoItem(int idx,PDoRuleItem pProcItem,PDoRuleItem* pFirstProcItem);
	bool				SP_DeleteDoItem(int idx,PDoRuleItem* pFirstProcItem);
	bool				__SP_SaveDoItem(FILE* fp,PDoRuleItem pDoItem,bool bhead = false,char** lpBuffer = 0);


private:
	SAPIRESULTCODE			 __SP_Read();	
	SAPIRESULTCODE			 __SP_ReadMainInfo(char* lpBuffer);
	SAPIRESULTCODE			 __SP_ReadASRRules(char* lpBuffer);
	SAPIRESULTCODE			 __SP_ReadTTSRules(char* lpBuffer);	
	SAPIRESULTCODE			 __SP_ReadCUSTOMRules(char* lpBuffer);

public:
	SAPIRESULTCODE			 __SP_ReadTAGRules(char* lpBuffer);	

private:
	bool					__SP_SaveMainInfo(FILE* fp);
	bool					__SP_SaveASRRules(FILE* fp);
	bool					__SP_SaveTTSText(FILE* fp);
	bool					__SP_SaveCustomVoice(FILE* fp);

public:
	bool					__SP_SaveTagControlCodes(FILE* fp,char* lpBuffer = 0);

public:
	SAPIRESULTCODE			SP_Init();
	SAPIRESULTCODE			SP_Uninit();
	bool					DeleteAll();

private:
	char*					__SP_FindString(char* lpString,char* lpBuf);
	char*					__SP_BreakHeadRetun(char* lpSrc);
	void					__SP_BreakWhiteSpace(char* lpBuf);	
	char*					__SP_SkipWhiteSpace(char* lpSrc);
	char*					__SP_SkipLine(char* lpSrc);
	char*					__SP_IngoreLine(char* lpSrc);
	char*					__SP_ParseRule(char* lpSrc,char* lpName,int nNameLen,char** lparam1,char** lparam2);
	char*					__SP_GetString(char* lpSrc,char* lpStartTag, char* lpEndTag,char* lpBuf,int nBufLen);
	
private:
	char*					__SP_GetOverLine(char* lpBuffer);
	char*					__SP_GetStartTag(char* lpBuffer,char* lpTagName,int nNamelen);
	char*					__SP_GetDoRule(char* lpBuffer,PDoRuleItem pDoRuleItem,bool* isEndtag);	
	char*					__SP_GetResultRule(char* lpBuffer,PResultRuleItem* ppResultItem);
	

private:	
	PSAPIConfig		m_pSapiConfig;
	char			m_szConfigFile[MAX_STRING_COUNT_HUGE];

};

#endif // __SAPI_CONFIG_H
