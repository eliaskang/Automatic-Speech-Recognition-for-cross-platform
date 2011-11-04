/***************************************************************************
* Cqt supply high-level api(use config file), and suppy low-level api(use 
* function api). Developer can select one to work asr ,tts and custom voice.
* High-level use Sapi_Hi_
* Low-level use Sapi_Lo_
* Other use Sapi_
****************************************************************************
*     Date     |   version    |  describe           |   author     
*---------------------------------------------------------------------------
*   2011-01-07 |     v0.1     |  Create Linux/QT    |   elias.kang
*   2011-04-22 |     v0.2     |  Support Android/QT |   elias.kang
*   2011-05-03 |     v0.3     |  Support Wince/EVC  |   elias.kang
***************************************************************************/

#ifndef __CQT_SAPI_JAVA_H_
#define __CQT_SAPI_JAVA_H_
#include <jni.h>
#include "SAPItypes.h"
#include "SAPIPlatform.h"
#include "CQTSapi.h"

//变量保存环境
typedef struct __tagVarValueItem
{
	char* lpValue;
	struct __tagVarValueItem* next;
}TVarValueItem,*PVarValueItem;

typedef struct __tagMemoryVARValue
{
	char* lpVar;
	PVarValueItem pValueItem;
	struct __tagMemoryVARValue* next;
}TMemoryVARValue,*PMemoryVARValue;

class MySapiInterface : public CQTSapiInterface
{
public:
	MySapiInterface();
	~MySapiInterface();
	
public:
	bool	ITF_DebugInfoOutput(SAPIHANDLE hande, EnDebugInfoType etype,char* lpOutputinfo);
	char*	ITF_GetASRVarValue(char* lpASRWordVar,int idx);// if return NULL is over
	char*	ITF_GetTTVarValue(char* lpTTSWordVar);
	bool	ITF_GetUserPressKey();
	bool	ITF_CallApplication(char* lpASRRuleName,char** pparam);	
};

typedef bool (*PFNcallback_SAPI_DebugOutInfo)(JNIEnv *env,char* lpInfo);
typedef bool (*PFNcallback_SAPI_ASRResult)(JNIEnv *env,char* lpASR);
class CQTSapi_Service
{
	Q_OBJECT
public:
	CQTSapi_Service();
	~CQTSapi_Service();	

	//设置回调函数
	void		SetCallBack(PFNcallback_SAPI_DebugOutInfo pfnDebug, PFNcallback_SAPI_ASRResult pfnReslut);

	//初始化
	bool		CSS_Init(char* lpResourcePath, char* lpConfigName,CQtObject* obj);

	//ASR变量清除
	int			CSS_ASRClear(char* lpVar);

	//ASR变量设置
	int			CSS_SetASRRuleValue(char* lpVar, char* lpValue);

	//TTS变量清除
	int			CSS_TTSClear(char* lpVar);

	//TTS变量设置
	int			CSS_SetTTSRuleValue(char* lpVar, char* lpValue);

	//得到sapi对象
	CQTSapi*	CSS_GetSapi() {return m_pcqtSapi;};

private:
	CQTSapi*			m_pcqtSapi;
	MySapiInterface*	m_pmySapiInterface;
};

#endif//__CQT_SAPI_H_