/***************************************************************************
* Cqt Platfrom ,support QT,windows or Other
* 
* 
* 
* 
****************************************************************************
*     Date     |   version    |  describe           |   author     
*---------------------------------------------------------------------------
*   2011-01-07 |     v0.1     |  Create             |   elias.kang
*              |              |                     |    
***************************************************************************/


#ifndef __SAPI_PLATFROM_H__
#define __SAPI_PLATFROM_H__
#include "SAPItypes.h"


#if defined(PLATFROM_SAPI_WINCE_EVC)

#include <afxwin.h>
#define Q_OBJECT
#define QObject CWnd

#elif defined(PLATFROM_SAPI_ANDROID_NDK)

#define Q_OBJECT
class QObject
{
public:
	QObject(){};
	~QObject(){};

	JavaVM *jvm;
};

#else
#include <QObject>
#include <QMutex>
#include <QThread>
#endif

//Obj ，process msg and other
class CQtObject: public QObject
{
	Q_OBJECT
public:
	CQtObject(CQtObject* parent);
	virtual ~CQtObject();

public:
};

//////////////////////////////////////////////////////////////////////////
class CThirdEngine: public CQtObject
{
	Q_OBJECT
public:
	CThirdEngine(CQtObject* parent);
	virtual ~CThirdEngine(){};
	
public:
	virtual bool	TE_SetResourcePath(char* lpPath,char* lpAppName) = 0;	//设置资源的路径
	virtual bool	TE_ASR(char* lpRuleName, 
		char* lpRuleContext,
		char* lpLanguage,
		CQTSapiInterface* pInterface,
		char** lpResult) = 0;//

	virtual bool	TE_TTS(char* lpTTSText,char* lpLanguage,bool isFile) = 0;
	virtual bool	TE_Voice(char* lpVoiceFile) = 0;
	virtual bool	TE_Stop() = 0;
};

//////////////////////////////////////////////////////////////////////////
//mutex
#if defined(PLATFROM_SAPI_WINCE_EVC) || defined(PLATFROM_SAPI_ANDROID_NDK)
class CQtMutex
{
public:
	CQtMutex(){};
	~CQtMutex(){};
	void lock() {};
	void unlock() {};
};
#else
class CQtMutex
{
public:
	CQtMutex(){};
	~CQtMutex(){};
	void lock() {m_quitMutex.lock();};
	void unlock() {m_quitMutex.unlock();};

private:
	QMutex m_quitMutex;
};
#endif

//Thread
#if defined(PLATFROM_SAPI_WINCE_EVC) || defined(PLATFROM_SAPI_ANDROID_NDK)
class CQtThread 
{
public:
	CQtThread() {};
	~CQtThread() {};
	virtual void T_run() = 0;
	void T_start();
	void T_Quit();
	void run() {T_run();};
};
#else
class CQtThread: public QThread
{
public:
	CQtThread() {};
	~CQtThread() {};
	virtual void T_run() = 0;
	void T_start(){start();};
	void T_Quit(){};
	void run() {T_run();};
};
#endif

#endif//__CQT_SAPI_H_