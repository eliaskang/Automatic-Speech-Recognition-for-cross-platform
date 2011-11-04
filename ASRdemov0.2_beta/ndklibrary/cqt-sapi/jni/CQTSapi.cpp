
#include "CQTSapi.h"
#define TEST_SINGLE_THREAD

#ifdef TEST_SINGLE_THREAD
#include "SAPIEngine.h"
static SAPIEngine* g_Engine = 0;
#endif

CQTSapi::CQTSapi(CQtObject* parent)
:CQtObject(parent)
{
#ifdef TEST_SINGLE_THREAD
	if(!g_Engine)
		g_Engine = new SAPIEngine(parent);

	g_Engine->Engine_Create();
	m_handle = g_Engine->GetOneSapiHandle();	
#endif
}

CQTSapi::~CQTSapi()
{
#ifdef TEST_SINGLE_THREAD
	if(g_Engine)
	{
		g_Engine->Engine_Destory();
		delete g_Engine;
		g_Engine = 0;
	}	
#endif
}

SAPIRESULTCODE CQTSapi::Sapi_Register(PSAPIRegister pSapiRegister)
{
#ifdef TEST_SINGLE_THREAD
	SAPIRESULTCODE iRet = g_Engine->Engine_Register(m_handle,pSapiRegister);
	if(iRet != SAPI_OK)
	{
		return iRet;
	}
	g_Engine->Engine_SetActive(m_handle);

	return SAPI_OK;
#endif
}

SAPIRESULTCODE CQTSapi::Sapi_UnRegister()
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_UnRegister(m_handle);
#endif
}

SAPIRESULTCODE CQTSapi::Sapi_Stop()
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_Hi_QUit(m_handle);
#endif
	return SAPI_ERROR;
}

ENSAPIStatus   CQTSapi::Sapi_Status()
{
	return SAPI_Status_Idle;
}

SAPIRESULTCODE CQTSapi::Sapi_Hi_Start()
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_Hi_Start(m_handle);
#endif
	return SAPI_OK;
}
	
SAPIRESULTCODE CQTSapi::Sapi_Lo_ASRRule(char* lpRuleName)
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_Lo_ASRRule(m_handle,lpRuleName);
#endif
	return SAPI_ERROR;
}

SAPIRESULTCODE CQTSapi::Sapi_Lo_TTSRule(char* lpRuleName)
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_Lo_TTSRule(m_handle,lpRuleName);
#endif
	return SAPI_ERROR;
}

SAPIRESULTCODE CQTSapi::Sapi_Lo_VoiceRule(char* lpRuleName)
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_Lo_VoiceRule(m_handle,lpRuleName);
#endif
	return SAPI_ERROR;
}

SAPIRESULTCODE CQTSapi::Sapi_Lo_TTSText(char* lpTTSText,char* lpLanguage)
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_Lo_TTSText(m_handle,lpTTSText,lpLanguage);
#endif
	return SAPI_ERROR;
}

SAPIRESULTCODE CQTSapi::Sapi_Lo_VoiceFile(char* lpPcmFile)
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_Lo_VoiceFile(m_handle,lpPcmFile);
#endif
	return SAPI_ERROR;
}

SAPIRESULTCODE CQTSapi::Sapi_Hi_Pause()
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_Hi_Pause(m_handle);
#endif
	return SAPI_ERROR;
}

SAPIRESULTCODE CQTSapi::Sapi_Hi_Resume()
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_Hi_Resume(m_handle);
#endif
	return SAPI_ERROR;
}

SAPIRESULTCODE CQTSapi::Sapi_Hi_QUit()
{
#ifdef TEST_SINGLE_THREAD
	return g_Engine->Engine_Hi_QUit(m_handle);
#endif
	return SAPI_ERROR;
}