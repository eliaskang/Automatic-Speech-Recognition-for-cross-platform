
#include "SvoxEngine.h"

#if defined(PLATFORM_SAPI_LINUX_QT) || defined(PLATFROM_SAPI_ANDROID_QT)
#include <QtCore/QCoreApplication>
#include <iostream>
#include <linux/soundcard.h>
#endif

#ifdef PLATFROM_SAPI_WINCE_EVC
#define SVOX_BUILD
#endif

#ifdef PLATFORM_SAPI_LINUX_QT
#define SVOX_BUILD
#endif


//////////////////////////////////////////////////////////////////////////
#ifdef SVOX_BUILD1
#include <iostream>
#include <stdlib.h>
#include <string>
#include "PrompterManager.hpp"
#include "PrompterInterface.hpp"
#endif

#ifdef SVOX_BUILD
#include "PrompterInterface.hpp"
#include "PrompterManager.hpp"
#include "Grammar.hpp"
#include "Recognizer.hpp"

using namespace svox;
using namespace svox::prompter;
using svox::prompter::PrompterInterface;
using svox::prompter::PrompterManager;

#define ASR_ERROR_CHECK(rc) errorCheck<svox::asr::Recognizer>(asr, rc, __LINE__, #rc);
#define GRM_ERROR_CHECK(rc) errorCheck<svox::asr::Grammar>(grammar, rc, __LINE__, #rc);
template <class T>
void errorCheck(T *object, svox::asr::ResultCode rc, int line, const char *statement)
{
    if (svox::asr::OK != rc)
    {
        //std::cout << "Error in line " << line << " \"" << statement << "\":" << std::endl;
        //std::cout << object->getLastErrorDetails() << " (error code " << rc << ")" << std::endl;
        exit(rc);
    }
}
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef SOUND_USE_QMULTIMEDIA_ALSA
//////////////////////////////////////////////////////////////////////////
#define MY_SOUND_RATE	16000
AudioInfo::AudioInfo(const QAudioFormat &format, QObject *parent)
:   QIODevice(parent)
,   m_format(format)
,   m_maxAmplitude(0)
,   m_level(0.0)

{   
	m_nTotalLen = 0;
	m_nBlockSize = 0;
	m_lpBuffer = 0;
	m_bIsWirting = false;
	m_bisRecording = false;
	m_nWritelen = 0;
}

AudioInfo::~AudioInfo()
{
}

void AudioInfo::start()
{
	m_nWritelen = 0;
	m_bIsWirting = false;
	m_bisRecording = true;
    open(QIODevice::WriteOnly);	
}

void AudioInfo::stop()
{
	if(m_lpBuffer)
		delete m_lpBuffer;
	
    close();
	m_bisRecording = false;
}

bool  AudioInfo::Start(int nBlocksize)
{
	m_nBlockSize = nBlocksize;
	m_nTotalLen = m_nBlockSize*4 + 1024;
	m_lpBuffer = new char[m_nTotalLen];
	start();
	return true;
}

bool AudioInfo::GetData(char* lpData, int nSize)
{
	if(!m_bisRecording)
		return false;
	if(m_nWritelen >= nSize)
	{
		m_quitMutex.lock();
		memcpy(lpData,m_lpBuffer,nSize);
		m_nWritelen = m_nWritelen - nSize;
		memmove(m_lpBuffer,m_lpBuffer + nSize,m_nWritelen);
		m_quitMutex.unlock();
	}	
	
	return true;
}

qint64 AudioInfo::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
		Q_UNUSED(maxlen)
		
		return 0;
}

qint64 AudioInfo::writeData(const char *data, qint64 len)
{
	int nNewlen = m_nWritelen + len;
	if(nNewlen < m_nTotalLen)
	{
		m_quitMutex.lock();
		memcpy(m_lpBuffer + m_nWritelen,data,len);
		m_nWritelen += len;
		m_quitMutex.unlock();
	}else
	{
		return 0;
	}	
    return len;
}
//////////////////////////////////////////////////////////////////////////
#define MAX_OUTPUT_PCM_LEN	(1024*1024)
Generator::Generator(QObject *parent)
:QIODevice(parent)					 
{  
	m_nLen = 0;
	m_buffer = new char[MAX_OUTPUT_PCM_LEN];
}

void Generator::start()
{
    open(QIODevice::ReadOnly);
}

void Generator::stop()
{
    close();
}

void Generator::PlayBuf(char* lpData, int nLen)
{
	SAPI_DB_PRINTF("Generator::PlayBuf()--------------------------------------nLen=%d",nLen);
	for(;m_nLen != 0;)
	{
		usleep(10);
	}
	
	m_quitMutex.lock();
	m_nLen = nLen;
	m_nOffsert = 0;
	if(nLen >= (MAX_OUTPUT_PCM_LEN-2))
	{
		m_nLen = MAX_OUTPUT_PCM_LEN-2;
	}
	memcpy(m_buffer,lpData,m_nLen);
	m_quitMutex.unlock();
}

qint64 Generator::readData(char *data, qint64 len)
{
	SAPI_DB_PRINTF("Generator::readData()--------------------------------------len=%d",len);
	if(m_nLen <= 0)
	{
		memset(data,0,len);
		return len;
	}
	
    m_quitMutex.lock();
	int left = m_nLen - m_nOffsert;
	int nRetLen = 0;
	if(left > len)
	{
		nRetLen = len;
		memcpy(data,m_buffer+m_nOffsert,len);
		m_nOffsert += len;
	}else
	{
		nRetLen = left;
		memcpy(data,m_buffer+m_nOffsert,left);
		m_nOffsert = 0;
		m_nLen = 0;
	}
	m_quitMutex.unlock();
	
    return nRetLen;
}

qint64 Generator::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    Q_UNUSED(len);	
    return 0;
}

qint64 Generator::bytesAvailable() const
{
    return (1024*50) + QIODevice::bytesAvailable();
}

//////////////////////////////////////////////////////////////////////////
MyOutputPcm::MyOutputPcm(CQtObject* obj)
:   m_device(QAudioDeviceInfo::defaultOutputDevice())
,   m_generator(0)
,   m_audioOutput(0)
,   m_output(0)
{
	m_parent = obj;
	initializeAudio();
}

bool MyOutputPcm::waitUntilProcessed()
{
	return true;
}

//ÉùÒôÀ´ÁË
bool MyOutputPcm::writeAudioData(const short* lpPcm,int nSampleCount)
{
	SAPI_DB_PRINTF("MyOutputPcm::writeAudioData()--------------------------------------1");
	char* lp = (char*)lpPcm;
	int nlen = nSampleCount*2;
	SAPI_DB_PRINTF("MyOutputPcm::writeAudioData()--------------------------------------2,nlen=%d",nlen);
	m_generator->PlayBuf(lp,nlen);
	return true;
}

const int DataFrequencyHz = 22500;
void MyOutputPcm::initializeAudio()
{
	SAPI_DB_PRINTF("initializeAudio------------------1");
	m_format.setFrequency(DataFrequencyHz);
    m_format.setChannels(1);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);
	
    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
	SAPI_DB_PRINTF("QAudioDeviceInfo------------devicename = %s",info.deviceName().toLatin1().data());
    if (!info.isFormatSupported(m_format)) 
	{
		SAPI_DB_PRINTF("Default format not supported - trying to use nearest");
        qWarning() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }
	
    m_generator = new Generator(m_parent);	
    createAudioOutput();
}

void MyOutputPcm::createAudioOutput()
{
	if(m_audioOutput)
		delete m_audioOutput;
    m_audioOutput = 0;
    m_audioOutput = new QAudioOutput(m_device, m_format, m_parent);    
}

bool MyOutputPcm::Start()
{
	SAPI_DB_PRINTF("MyOutputPcm::Start()--------------------------------------1");
	m_generator->start();
    m_audioOutput->start(m_generator);
	return true;
}

bool MyOutputPcm::Stop()
{
	SAPI_DB_PRINTF("MyOutputPcm::Start()--------------------------------------2");
	m_generator->stop();
    m_audioOutput->stop();
	return true;
}

#else

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#if defined(PLATFROM_SAPI_ANDROID_QT) || defined(PLATFROM_SAPI_ANDROID_NDK)

#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>

#define MY_SOUND_RATE	16000
MyOutputPcm::MyOutputPcm(CQtObject* obj)
{
	m_pInterface = 0;
	m_parent = obj;
}

char* MyOutputPcm::ITF_GetASRVarValue(char* lpASRWordVar,int idx)
{
	return m_pInterface->ITF_GetASRVarValue(lpASRWordVar,idx);
}

char* MyOutputPcm::ITF_GetTTVarValue(char* lpTTSWordVar)
{
	return m_pInterface->ITF_GetTTVarValue(lpTTSWordVar);
}

#endif
#endif

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifdef PLATFORM_SAPI_LINUX_QT
#include <sys/soundcard.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>
#define MY_SOUND_RATE	16000
using namespace svox;
using namespace asr;
class MyAudioInputInterface : public AudioInputInterface
{
public:
	MyAudioInputInterface();
	~MyAudioInputInterface();

	SVOX_32S getSampleRate();
	SourceType getSourceType();
	ResultCode startCapture(SVOX_32S preferredBlockSize);
	ResultCode stopCapture();
	ResultCode read(SVOX_16S *&buffer, SVOX_32S &numSamples);
	ResultCode releaseBuffer(SVOX_16S *buffer, SVOX_32S numSamples);

private:
	bool	OpenSnd();
	bool	CloseSnd();

	int			m_nBlockSize;
	int			m_audio_fd;
	SVOX_16S*	m_lpBufferSound;
};

MyAudioInputInterface::MyAudioInputInterface()
{
	m_audio_fd = -1;
	m_nBlockSize = 0;
	m_lpBufferSound = 0;
}

MyAudioInputInterface::~MyAudioInputInterface()
{
	CloseSnd();
}

bool MyAudioInputInterface::OpenSnd()
{	
	if(m_audio_fd >= 0)
		return false;

	if((m_audio_fd = open("/dev/dsp", O_RDONLY, 0)) < 0) 
	{
		printf("Open sound dev Error!\r\n");
		return false;
	}

	int i=0;
	i=MY_SOUND_RATE;
	ioctl(m_audio_fd,SNDCTL_DSP_SPEED,(char *)&i);
	i=1;
	ioctl(m_audio_fd,SNDCTL_DSP_CHANNELS,(char *)&i);
	i=AFMT_S16_LE;
	ioctl(m_audio_fd,SNDCTL_DSP_SETFMT,(char *)&i);

	printf("OpenSnd OK\r\n");
	return true;
}

bool MyAudioInputInterface::CloseSnd()
{
	if(m_audio_fd < 0)
		return false;

	close(m_audio_fd);
	m_audio_fd = -1;

	printf("CloseSnd OK\r\n");
	return true;
}

SVOX_32S MyAudioInputInterface::getSampleRate()
{
	return MY_SOUND_RATE;
}

AudioInputInterface::SourceType MyAudioInputInterface::getSourceType()
{
	return AUDIO_FROM_DEVICE;
}

AudioInputInterface::ResultCode MyAudioInputInterface::stopCapture()
{
	if(!m_lpBufferSound)
		return AUDIO_ERROR;	
	
	delete[] m_lpBufferSound;
	m_lpBufferSound = 0;

	printf("stopCapture\r\n");
	CloseSnd();

	return AUDIO_OK;
}


AudioInputInterface::ResultCode MyAudioInputInterface::startCapture(SVOX_32S preferredBlockSize)
{
	m_nBlockSize = preferredBlockSize;
	printf("preferredBlockSize = %d\r\n",preferredBlockSize);
	m_lpBufferSound = new SVOX_16S[preferredBlockSize + 1];
	if(!m_lpBufferSound)
	{
		return AUDIO_ERROR;
	}
	
	OpenSnd();
	return AUDIO_OK;
}

AudioInputInterface::ResultCode MyAudioInputInterface::read(SVOX_16S *&buffer, SVOX_32S &numSamples)
{
	int len;
	//printf("numSamples=%d\r\n",numSamples);

	len = ::read(m_audio_fd, (void*)m_lpBufferSound, m_nBlockSize*sizeof(SVOX_16S));
	//printf("Read Sound Data = %d Bytes!\r\n",len);
	if (len < 0) 
	{
		printf("Read Sound Error!\r\n");
		return AUDIO_ERROR;
	}

	buffer = m_lpBufferSound;
	numSamples = len/(sizeof(SVOX_16S));
	return AUDIO_OK;
}

AudioInputInterface::ResultCode MyAudioInputInterface::releaseBuffer(SVOX_16S *buffer, SVOX_32S numSamples)
{
#if 0
	if(!m_lpBufferSound)
		return AUDIO_ERROR;	

	delete[] m_lpBufferSound;
	m_lpBufferSound = 0;
#endif
	//printf("releaseBuffer OK\r\n");
	return AUDIO_OK;
}
// end add by elias.kang
#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
SVoxEngine::SVoxEngine(CQtObject* parent)
:CThirdEngine(parent)
{
	
}

SVoxEngine::~SVoxEngine()
{
	
}

bool SVoxEngine::TE_SetResourcePath(char* lpPath,char* lpAppName)
{
	strcpy(m_szPath,lpPath);
	strcat(m_szPath,"/Svox/resources");
	strcpy(m_appName,lpAppName);
	//	strcat(m_appName,".grm");
	
	strcpy(m_szPathText,lpPath);
	strcat(m_szPathText,"/TTSText/");
	
	strcpy(m_szPathVoice,lpPath);
	strcat(m_szPathVoice,"/Voice/");
	
#if defined(PLATFROM_SAPI_ANDROID_QT) || defined(PLATFROM_SAPI_ANDROID_NDK)
	m_pOutputPcm = new MyOutputPcm(this);	
	m_mySvox.TE_SetResourcePath(lpPath,lpAppName,m_pOutputPcm);
#endif
	return true;
}

#ifdef SVOX_BUILD
svox::asr::Recognizer        *g_asr = 0;
#endif
bool SVoxEngine::TE_ASR(char* lpRuleName, char* lpRuleContext,char* lpLanguage,CQTSapiInterface* pInterface,char** lpResult)
{
#ifdef SVOX_BUILD
	if(g_asr)
		return false;
	
	SAPI_DB_PRINTF("1-----SVoxEngine::TE_ASR: lpRuleName = %s\r\n",lpRuleName);
	svox::common::Configuration   config;	
	svox::asr::Recognizer        *asr = svox::asr::Recognizer::New();
	SAPI_DB_PRINTF("2-----SVoxEngine::TE_ASR: lpRuleName = %s\r\n",lpRuleName);
	g_asr = asr;
	svox::asr::Grammar           *grammar;
	svox::asr::RecognitionResult *result;
	
	SAPI_DB_PRINTF("SVoxEngine::TE_ASR: lpRuleName = %s\r\n",lpRuleName);
	SAPI_DB_PRINTF("SVoxEngine::TE_ASR: lpRuleContext = %s\r\n",lpRuleContext);
	SAPI_DB_PRINTF("SVoxEngine::TE_ASR: lpLanguage = %s\r\n",lpLanguage);
	SAPI_DB_PRINTF("SVoxEngine::TE_ASR: m_szPath = %s\r\n",m_szPath);
	SAPI_DB_PRINTF("SVoxEngine::TE_ASR: m_appName = %s\r\n",m_appName);
	
	// initialize the speech recognizer for en-US
	SAPI_DB_PRINTF("1-1\r\n");
	config.setStaticResourcePath(m_szPath);
	SAPI_DB_PRINTF("2-2\r\n");
	ASR_ERROR_CHECK(asr->initialize(/*lpLanguage*/"en-US", &config));
	
	// load language models for the oneshot destination input and activate it
	SAPI_DB_PRINTF("3-3\r\n");
	ASR_ERROR_CHECK(asr->loadGrammar(m_appName, svox::asr::Grammar::NORMAL, 0, 0, grammar));
	SAPI_DB_PRINTF("4-4\r\n");
	ASR_ERROR_CHECK(asr->activateGrammar(grammar));
	SAPI_DB_PRINTF("5-5\r\n");
	GRM_ERROR_CHECK(grammar->deactivateAllRules());	
	SAPI_DB_PRINTF("6-6\r\n");
	GRM_ERROR_CHECK(grammar->activateRule(lpRuleName));
	SAPI_DB_PRINTF("7-7\r\n");
	
#ifdef PLATFORM_SAPI_LINUX_QT
	MyAudioInputInterface* myAI = new MyAudioInputInterface();
	GRM_ERROR_CHECK(asr->setAudioInput(myAI));
#endif

	SAPI_DB_PRINTF("8-8\r\n");
	// End add	
	
	// start recognition
	//std::cout << "Please speak!" << std::endl << std::endl;	
	ASR_ERROR_CHECK(asr->startRecognition());
	SAPI_DB_PRINTF("9-9\r\n");
	asr->waitForRecognitionResult();	
	SAPI_DB_PRINTF("10-10\r\n");
	
	// print results
	ASR_ERROR_CHECK(asr->getRecognitionResults(result));
	if (0 != result)
	{
		int m = 0;			
		for (int i = 0; i < result->getNumUtterances(); i++)
		{
			const svox::asr::RecognizedUtterance *utt = result->getUtterance(i);
			for (int j = 0; j < utt->getNumWords(); j++)
			{
				const svox::asr::RecognizedWord *word = utt->getWord(j);
				if( m < MAX_ASR_RESULT_COUNT && word->getSpelling())
				{
					lpResult[m] = new char[256];
					strcpy(lpResult[m],word->getSpelling());
					m++;
				}
			}
		}
	}//end if
	
	
	// unload grammar and deinitialize speech recognizer
	asr->unloadGrammar(grammar);
	asr->deinitialize();
#ifdef PLATFORM_SAPI_LINUX_QT
	delete myAI;
#endif
	g_asr = 0;
	delete asr;	
	return true;
#else
#if defined(PLATFROM_SAPI_ANDROID_QT) || defined(PLATFROM_SAPI_ANDROID_NDK)
	m_pOutputPcm->SetCQTSapiInterface(pInterface);
	return  m_mySvox.TE_ASR(lpRuleName,lpRuleContext,lpLanguage,lpResult);	
#endif
	return false;
#endif
}


void SVoxEngine::TestASR()
{
}


#ifdef SVOX_BUILD
PrompterInterface *g_prompter = 0;
#endif
bool SVoxEngine::TE_TTS(char* lpTTSText,char* lpLanguage,bool isFile)
{
#ifdef SVOX_BUILD
    SAPI_DB_PRINTF("TE_TTS-----1");
	if(g_prompter)
		return false;
	
	SAPI_DB_PRINTF("TE_TTS-----2");
	PrompterInterface *prompter = 0;
	PrompterManager *prompterMgr = PrompterManager::getInstance();
	char szTempTTS[MAX_STRING_COUNT_HUGE];
	strcpy(szTempTTS,m_szPath);	
	strcat(szTempTTS,"/TTS/Plus");	
	prompterMgr->initialize(szTempTTS);
	prompterMgr->createPrompterInstance("SDS", prompter);
	prompter->setVoice(lpLanguage);
	g_prompter = prompter;
	SAPI_DB_PRINTF("TE_TTS-----3");
	if(isFile)
	{
		if(lpTTSText[0] != '/')
		{
			char szPathTFile[MAX_STRING_COUNT_HUGE];
			strcpy(szPathTFile,m_szPathText);
			strcat(szPathTFile,lpTTSText);
			prompter->playTextFile(szPathTFile);
		}else
		{
			prompter->playTextFile(lpTTSText);
		}	
	}
	else
	{
		prompter->playString(lpTTSText);
	}
	SAPI_DB_PRINTF("TE_TTS-----4");
	prompter->wait();
	g_prompter = 0;
	SAPI_DB_PRINTF("TE_TTS-----5");
	prompterMgr->destroyPrompterInstance(prompter);	
    PrompterManager::destroyInstance();
#else
#if defined(PLATFROM_SAPI_ANDROID_QT) || defined(PLATFROM_SAPI_ANDROID_NDK)
	m_mySvox.TE_TTS(lpTTSText,lpLanguage,isFile);
#endif
#endif
	return true;
}

bool SVoxEngine::TE_Voice(char* lpVoiceFile)
{
#ifdef SVOX_BUILD
	SAPI_DB_PRINTF("TE_Voice-----1");
	if(g_prompter)
		return false;
	
	SAPI_DB_PRINTF("TE_Voice-----2");
	PrompterInterface *prompter = 0;
	PrompterManager *prompterMgr = PrompterManager::getInstance();        
	char szTempTTS[MAX_STRING_COUNT_HUGE];
	strcpy(szTempTTS,m_szPath);
	strcat(szTempTTS,"/TTS/Plus");	
	SAPI_DB_PRINTF("TE_Voice-----2-1,szTempTTS=%s",szTempTTS);
	prompterMgr->initialize(szTempTTS);
	SAPI_DB_PRINTF("TE_Voice-----2-2,szTempTTS=%s",szTempTTS);
	prompterMgr->createPrompterInstance("SDS", prompter);
	SAPI_DB_PRINTF("TE_Voice-----2-3");
	g_prompter = prompter;
	prompter->setVoice("en-US");
	SAPI_DB_PRINTF("TE_Voice-----3");
	
	if(lpVoiceFile[0] != '/')
	{
		char szPathTFile[MAX_STRING_COUNT_HUGE];
		strcpy(szPathTFile,m_szPathVoice);
		strcat(szPathTFile,lpVoiceFile);
		SAPI_DB_PRINTF("TE_Voice-----szPathTFile = %s",szPathTFile);
		prompter->playAudioFile(szPathTFile);
	}else
	{
		SAPI_DB_PRINTF("TE_Voice-----lpVoiceFile = %s",lpVoiceFile);
		prompter->playAudioFile(lpVoiceFile);
	}
	
	SAPI_DB_PRINTF("TE_Voice-----4");
	prompter->wait();
	SAPI_DB_PRINTF("TE_Voice-----5");
	g_prompter = 0;
	prompterMgr->destroyPrompterInstance(prompter);	
    PrompterManager::destroyInstance();
#else
#if defined(PLATFROM_SAPI_ANDROID_QT) || defined(PLATFROM_SAPI_ANDROID_NDK)
	m_mySvox.TE_Voice(lpVoiceFile);
#endif
#endif
	return true;
}

bool SVoxEngine::TE_Stop()
{
#ifdef SVOX_BUILD
	if(g_asr)
		g_asr->abortRecognition();
	
	if(g_prompter)
		g_prompter->abort();
#else
#if defined(PLATFROM_SAPI_ANDROID_QT) || defined(PLATFROM_SAPI_ANDROID_NDK)
	m_mySvox.TE_Stop();
#endif
#endif
	return true;
}
