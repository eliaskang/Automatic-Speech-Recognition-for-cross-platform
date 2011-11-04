/***************************************************************************
* Cqt supply high-level api(use config file), and suppy low-level api(use 
* function api). Developer can select one to work asr ,tts and custom voice.
* High-level use Sapi_Hi_
* Low-level use Sapi_Lo_
* Other use Sapi_
****************************************************************************
*     Date     |   version    |  describe           |   author     
*---------------------------------------------------------------------------
*   2011-01-07 |     v0.1     |  Create             |   elias.kang
*              |              |                     |    
***************************************************************************/

#ifndef __CQT_SAPI_H_
#define __CQT_SAPI_H_

#include "SAPItypes.h"
#include "SAPIPlatform.h"
#include <stdio.h>
#include <stdlib.h>

//#define SOUND_USE_QMULTIMEDIA_ALSA
#if defined(PLATFORM_SAPI_LINUX_QT) || defined(PLATFROM_SAPI_ANDROID_QT)
#include <QtGui/QMainWindow>
#include <QByteArray>
#include <QObject>
#include <QThread>
#include <QDebug>
#endif

#if defined(PLATFROM_SAPI_ANDROID_NDK) || defined(PLATFROM_SAPI_ANDROID_QT)
#include "new_svox_engine.h"
#endif

#ifdef SOUND_USE_QMULTIMEDIA_ALSA
#include <QtMultimedia/qaudioinput.h>
#include <QtMultimedia/QAudioOutput>
#include <QIODevice>
#include <QMutex>
class AudioInfo : public QIODevice
{
    Q_OBJECT
public:
    AudioInfo(const QAudioFormat &format, QObject *parent);
    ~AudioInfo();
	
    void start();
    void stop();
	
    qreal level() const { return m_level; }	
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);

	bool   Start(int nBlocksize);
	bool   GetData(char* lpData, int nSize);	

	char* m_lpBuffer;
	int	  m_nBlockSize;
	int	  m_nTotalLen;
	bool  m_bIsWirting;
	bool  m_bisRecording;
	int	  m_nWritelen;
	QMutex m_quitMutex;
	
private:
    const QAudioFormat m_format;
    quint16 m_maxAmplitude;
    qreal m_level; // 0.0 <= m_level <= 1.0
	
signals:
    void update();
};


class Generator : public QIODevice
{
    Q_OBJECT
public:
    Generator(QObject *parent);
    ~Generator(){};
	
    void start();
    void stop();
	void PlayBuf(char* lpData, int nLen);
	
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    qint64 bytesAvailable() const;	
	
private:
	int	   m_nLen;
	int	   m_nOffsert;
    char*  m_buffer;
	QMutex m_quitMutex;
};

class MyOutputPcm: public SvoxOutputPcm
{
public:
	MyOutputPcm(CQtObject* obj);
	~MyOutputPcm(){};

	bool waitUntilProcessed();
	bool writeAudioData(const short* lpPcm,int nSampleCount);

	bool Start();
	bool Stop();
	void initializeAudio();
    void createAudioOutput();
private:
	CQtObject*		m_parent;

	QAudioDeviceInfo m_device;
    Generator*       m_generator;
    QAudioOutput*    m_audioOutput;
    QIODevice*       m_output; // not owned
    QAudioFormat     m_format;
};
#else

#if defined(PLATFROM_SAPI_ANDROID_QT) || defined(PLATFROM_SAPI_ANDROID_NDK)
class MyOutputPcm: public SvoxOutputPcm
{
public:
	MyOutputPcm(CQtObject* obj);
	~MyOutputPcm(){};

	char* ITF_GetASRVarValue(char* lpASRWordVar,int idx);
	char* ITF_GetTTVarValue(char* lpTTSWordVar);
	void  SetCQTSapiInterface(CQTSapiInterface* pInterface) {m_pInterface=pInterface;};
public:
	CQtObject*			m_parent;
	CQTSapiInterface*	m_pInterface;
};
#endif

#endif


//////////////////////////////////////////////////////////////////////////
class SVoxEngine : public CThirdEngine
{
	Q_OBJECT
public:
	SVoxEngine(CQtObject* parent);
	virtual ~SVoxEngine();

public:
	bool	TE_SetResourcePath(char* lpPath,char* lpAppName);	//设置资源的路径
	bool	TE_ASR(char* lpRuleName, char* lpRuleContext,char* lpLanguage,CQTSapiInterface* pInterface,char** lpResult);	
	bool	TE_TTS(char* lpTTSText,char* lpLanguage,bool isFile);
	bool	TE_Voice(char* lpVoiceFile);
	bool	TE_Stop();

public:
	void    TestASR();

#if defined(PLATFROM_SAPI_ANDROID_QT) || defined(PLATFROM_SAPI_ANDROID_NDK)
	MyOutputPcm* m_pOutputPcm;
	MySvoxEngine m_mySvox;
#endif
private:
	char m_szPath[MAX_STRING_COUNT_HUGE];
	char m_szPathText[MAX_STRING_COUNT_HUGE];
	char m_szPathVoice[MAX_STRING_COUNT_HUGE];
	char m_appName[MAX_STRING_COUNT_LARGE];	
};

#endif//__CQT_SAPI_H_