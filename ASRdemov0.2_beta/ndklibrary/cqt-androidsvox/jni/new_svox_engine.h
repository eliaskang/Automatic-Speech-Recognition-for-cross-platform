/*** Prompter Demo Application *******************************************************************/

#ifndef __SVOXZ_ENGINE_HH__
#define __SVOXZ_ENGINE_HH__

#define SUPPORT_ASR_VAR_VALUE
#define MY_MAX_STRING_COUNT_HUGE 512
#define MY_MAX_STRING_COUNT_LARGE 256

class SvoxOutputPcm
{
public: 
	SvoxOutputPcm(){};
	~SvoxOutputPcm(){};
	virtual char* ITF_GetASRVarValue(char* lpASRWordVar,int idx) = 0;// if return NULL is over
	virtual char* ITF_GetTTVarValue(char* lpTTSWordVar) = 0;
};

class MySvoxEngine
{
public:
	MySvoxEngine(){ m_pSvoxOutput = 0;};
	~MySvoxEngine(){};
	
public:
	bool	TE_SetResourcePath(char* lpPath,char* lpAppName,SvoxOutputPcm* pOutput);	//设置资源的路径
	bool	TE_ASR(char* lpRuleName, char* lpRuleContext,char* lpLanguage,char** lpResult);	
	bool	TE_TTS(char* lpTTSText,char* lpLanguage,bool isFile);
	bool	TE_Voice(char* lpVoiceFile);
	void	TE_Stop();
	
	
private:
	SvoxOutputPcm*	m_pSvoxOutput;
	char m_szPath[MY_MAX_STRING_COUNT_HUGE];
	char m_szPathText[MY_MAX_STRING_COUNT_HUGE];
	char m_szPathVoice[MY_MAX_STRING_COUNT_HUGE];
	char m_appName[MY_MAX_STRING_COUNT_LARGE];
};

#endif