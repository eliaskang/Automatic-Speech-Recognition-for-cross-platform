/*** Prompter Demo Application *******************************************************************/
#include "new_svox_engine.h"
#include <linux/soundcard.h>
#include <string.h>
#include <strings.h>
#define stricmp strcasecmp
#include <stdarg.h>
#include <stdio.h>
#include <android/log.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <stdlib.h>

#include "ComplexListManager.hpp"
#include "Grammar.hpp"
#include "Recognizer.hpp"
#include "PrompterManager.hpp"
#include "PrompterInterface.hpp"
#include "PrompterManager.hpp"
#include "PrompterInterface.hpp"
#include "AudioOutputInterface.hpp"

using namespace svox;
using svox::prompter::PrompterInterface;
using svox::prompter::PrompterManager;
#define ERROR_CHECK(func, ok) errorCheck1(func, ok, __LINE__, #func);
void errorCheck1(prompter::ResultCode resultCode, prompter::ResultCode ok, SVOX_32S line, const char *statement)
{
    if (ok != resultCode)
    {       
		__android_log_print(ANDROID_LOG_INFO,"Error in line:","");
		__android_log_print(ANDROID_LOG_INFO,"error code:","");
       // exit(resultCode);
    }
}

#define ASR_ERROR_CHECK(rc) errorCheck<svox::asr::Recognizer>(asr, rc, __LINE__, #rc);
#define GRM_ERROR_CHECK(rc) errorCheck<svox::asr::Grammar>(grammar, rc, __LINE__, #rc);
#define ERROR_CHECK_CLM(func) chkError(0, func, svox::climax::OK, __LINE__, #func)
#define ERROR_CHECK_ASR(func) chkError(asr, func, svox::asr::OK, __LINE__, #func);
template <class T>
void errorCheck(T *object, svox::asr::ResultCode rc, int line, const char *statement)
{
    if (svox::asr::OK != rc)
    {
        //std::cout << "Error in line " << line << " \"" << statement << "\":" << std::endl;
        //std::cout << object->getLastErrorDetails() << " (error code " << rc << ")" << std::endl;
		__android_log_print(ANDROID_LOG_INFO,"Error in line:","");
		__android_log_print(ANDROID_LOG_INFO,"error code:","");
       // exit(rc);
    }
}

template<typename ENUM>
void chkError(svox::asr::Recognizer *asr, ENUM resultCode, ENUM ok, SVOX_32S line, const char *statement)
{	
    if (ok != (resultCode))
    {
        //std::cout << "Error in line " << line << " \"" << statement << "\":" << std::endl;
        //std::cout << (asr ? asr->getLastErrorDetails() : "") << " (error code " << resultCode << ")" << std::endl;
        //exit(resultCode);
    }
}

void MY_SAPI_DB_PRINTF(char* fmt,...)
{
	char szTempInfo[MY_MAX_STRING_COUNT_HUGE*2];
	memset(szTempInfo,0,MY_MAX_STRING_COUNT_HUGE*2);
	
	va_list args;
	int n;
	va_start(args, fmt);
	n = vsprintf(szTempInfo, fmt, args);
	va_end(args);
	strcat(szTempInfo,"\r\n");
	__android_log_print(ANDROID_LOG_INFO,"SAPI_Printf",szTempInfo);
}


//#define USE_ANDROID_RECORD	1
//#define USE_OSS_SOUND			1
#define USE_PCM_FILE			1

#ifdef USE_ANDROID_RECORD
#include "AudioRecord.h"
#include "AudioSystem.h"
#include "mediarecorder.h"
using namespace android;
#endif

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

#ifdef USE_OSS_SOUND	
	int			m_audio_fd;	
#endif

	SVOX_16S*	m_lpBufferSound;
	int			m_nBlockSize;
#ifdef USE_ANDROID_RECORD
	AudioRecord* m_pAudioRecord;
#endif

#ifdef USE_PCM_FILE
	FILE* m_fpPcm;
#endif
};

MyAudioInputInterface::MyAudioInputInterface()
{
#ifdef USE_OSS_SOUND
	m_audio_fd = -1;	
#endif
	m_nBlockSize = 0;
	m_lpBufferSound = 0;

#ifdef USE_ANDROID_RECORD
	m_pAudioRecord = 0;
#endif
}

MyAudioInputInterface::~MyAudioInputInterface()
{
	CloseSnd();
}

bool MyAudioInputInterface::OpenSnd()
{	

#ifdef USE_OSS_SOUND
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
#endif

#ifdef USE_ANDROID_RECORD
	m_pAudioRecord = new AudioRecord(AUDIO_SOURCE_MIC/*AUDIO_SOURCE_MIC*//*AUDIO_SOURCE_DEFAULT*/,
		16000,
		AudioSystem::PCM_16_BIT,
		AudioSystem::CHANNEL_IN_MONO
		);
	
	/*
	m_pAudioRecord->set(AUDIO_SOURCE_MIC,
		16000,
		AudioSystem::PCM_16_BIT,
		AudioSystem::CHANNEL_IN_MONO,
		0,
		0,
		0,
		0,
		0,
		false);*/
	m_pAudioRecord->start();
	MY_SAPI_DB_PRINTF("ASR-----OpenSnd OK");
#endif

#ifdef USE_PCM_FILE
	m_fpPcm = fopen("/mnt/sdcard/pcmfile/1.pcm","rb");
#endif
	return true;
}

bool MyAudioInputInterface::CloseSnd()
{
#ifdef USE_OSS_SOUND
	if(m_audio_fd < 0)
		return false;
	
	close(m_audio_fd);
	m_audio_fd = -1;
	
	printf("CloseSnd OK\r\n");
#endif

#ifdef USE_ANDROID_RECORD
	if(m_pAudioRecord)
	{
		m_pAudioRecord->stop();
		delete m_pAudioRecord;
		m_pAudioRecord = 0;
		MY_SAPI_DB_PRINTF("ASR-----CloseSnd OK");
	}
#endif

#ifdef USE_PCM_FILE
	if(m_fpPcm)
		fclose(m_fpPcm);
	m_fpPcm = 0;
#endif
	return true;
}

SVOX_32S MyAudioInputInterface::getSampleRate()
{
	return 16000;
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
	
	CloseSnd();
	return AUDIO_OK;
}


AudioInputInterface::ResultCode MyAudioInputInterface::startCapture(SVOX_32S preferredBlockSize)
{
	MY_SAPI_DB_PRINTF("ASR-----startCapture preferredBlockSize = %d",preferredBlockSize);
	m_nBlockSize = preferredBlockSize;
	m_lpBufferSound = new SVOX_16S[preferredBlockSize + 1];
	if(!m_lpBufferSound)
	{
		MY_SAPI_DB_PRINTF("ASR-----startCapture ERROR!");
		return AUDIO_ERROR;
	}	
	OpenSnd();
	MY_SAPI_DB_PRINTF("ASR-----startCapture OK!");
	return AUDIO_OK;
}

AudioInputInterface::ResultCode MyAudioInputInterface::read(SVOX_16S *&buffer, SVOX_32S &numSamples)
{
#ifdef USE_OSS_SOUND
	int len;	
	len = ::read(m_audio_fd, (void*)m_lpBufferSound, m_nBlockSize*sizeof(SVOX_16S));	
	if (len < 0) 
	{
		printf("Read Sound Error!\r\n");
		return AUDIO_ERROR;
	}
	
	buffer = m_lpBufferSound;
	numSamples = len/(sizeof(SVOX_16S));
#endif

#ifdef USE_ANDROID_RECORD
	int len;
	int nReadLen = m_nBlockSize*sizeof(SVOX_16S);
	len = m_pAudioRecord->read((void*)m_lpBufferSound,nReadLen);
	//MY_SAPI_DB_PRINTF("ASR-----read =%d",len);
	if (len < 0) 
	{
		MY_SAPI_DB_PRINTF("ASR-----read =%d -----------ERROR!",len);
		return AUDIO_ERROR;
	}

#if 0
	FILE* fp = fopen("/sdcard/my_record.pcm","a+b");
	if(fp)
	{
		fwrite((void*)m_lpBufferSound,1,len,fp);
		fclose(fp);
		MY_SAPI_DB_PRINTF("ASR-----write pcm file =%d",len);
	}
#endif
	
	buffer = m_lpBufferSound;
	numSamples = len/(sizeof(SVOX_16S));
#endif


#ifdef USE_PCM_FILE
	int len;	
	len = fread((void*)m_lpBufferSound,1,m_nBlockSize*sizeof(SVOX_16S),m_fpPcm);	
	if (len < 0) 
	{
		printf("Read Sound Error!\r\n");
		return AUDIO_ERROR;
	}
	
	buffer = m_lpBufferSound;
	numSamples = len/(sizeof(SVOX_16S));
#endif
	return AUDIO_OK;
}

AudioInputInterface::ResultCode MyAudioInputInterface::releaseBuffer(SVOX_16S *buffer, SVOX_32S numSamples)
{
	return AUDIO_OK;
}


//#define USE_CALLBACK_APP	1
//#define INTER_PLAY_DEVICE	1
#define USE_AUDIO_TRACK	1
#ifdef USE_AUDIO_TRACK
#include "AudioTrack.h"
#include "AudioSystem.h"
using namespace android;
#endif

class MyOutSound:public svox::prompter::AudioOutputInterface
{
public:
	MyOutSound(SvoxOutputPcm* pOutput)
	{
#ifdef USE_CALLBACK_APP
		m_pOutput=pOutput;
#endif

#ifdef INTER_PLAY_DEVICE
		m_audio_fd = ::open("/dev/snd/dsp", O_WRONLY, 0);
		if(m_audio_fd < 0) 
		{
			__android_log_print(ANDROID_LOG_INFO,"MyOutSound","Open sound device /dev/snd/dsp/dev/snd/dsp/dev/snd/dsp/dev/snd/dsp Error!\r\n");
			return;
		}
		
		int i=0;
		i=22500;
		ioctl(m_audio_fd,SNDCTL_DSP_SPEED,(char *)&i);
		i=1;
		ioctl(m_audio_fd,SNDCTL_DSP_CHANNELS,(char *)&i);
		i=AFMT_S16_LE;
		ioctl(m_audio_fd,SNDCTL_DSP_SETFMT,(char *)&i);
		
		__android_log_print(ANDROID_LOG_INFO,"MyOutSound","OpenSnd OK\r\n");
#endif

#ifdef USE_AUDIO_TRACK
		m_pAudioTrack = new AudioTrack();
		m_pAudioTrack->set(3,
			22500,   
			AudioSystem::PCM_16_BIT,
			AudioSystem::CHANNEL_OUT_MONO,
			0,
			0,
			0,
			this,
			0,
			0,
			false);
		m_pAudioTrack->start();
#endif
	};

	~MyOutSound()
	{
#ifdef INTER_PLAY_DEVICE
		if(m_audio_fd >= 0)
		{
			::close(m_audio_fd);
			m_audio_fd = -1;
			__android_log_print(ANDROID_LOG_INFO,"MyOutSound","Close snd OK\r\n");
		}
		
#endif

#ifdef USE_AUDIO_TRACK
		m_pAudioTrack->stop();
		delete m_pAudioTrack;
		m_pAudioTrack = 0;
#endif
	};

#ifdef USE_CALLBACK_APP
	SvoxOutputPcm* m_pOutput;
#endif
	
#ifdef INTER_PLAY_DEVICE
	int m_audio_fd;
#endif

#ifdef USE_AUDIO_TRACK
	AudioTrack* m_pAudioTrack;
#endif
	
	ResultCode waitUntilProcessed()
	{
#ifdef USE_CALLBACK_APP
		m_pOutput->waitUntilProcessed();
#endif
		return AUDIO_OK;
	};

	ResultCode writeAudioData(const SVOX_16S *audioData, SVOX_32S sampleCount)
	{
#ifdef USE_AUDIO_TRACK
		char* lp = (char*)audioData;
		int nTotal = sampleCount*2;
		m_pAudioTrack->write(lp, nTotal);
#endif

#ifdef USE_CALLBACK_APP
		__android_log_print(ANDROID_LOG_INFO,"TTS","------------------writeAudioData-1\r\n");
		m_pOutput->writeAudioData(audioData,sampleCount);
#endif

#ifdef INTER_PLAY_DEVICE
		char* lp = (char*)audioData;
		int nTotal = sampleCount*2;
		int nOffsert = 0;
		int nlen = 0;
		
		if(m_audio_fd < 0)
		{
			__android_log_print(ANDROID_LOG_INFO,"FUCK","write Sound Error,not open sound device!\r\n");
			return AUDIO_OK;
		}
		
		for(;;)
		{
			nlen = nTotal-nOffsert;
			nlen = ::write(m_audio_fd, (void*)(lp + nOffsert),nlen);			
			if (nlen < 0) 
			{
				__android_log_print(ANDROID_LOG_INFO,"TTS","write Sound Error!\r\n");
				return AUDIO_OK;
			}
			nOffsert += nlen;
			if(nOffsert >= nTotal)
				break;
		}

#if 0
		FILE* fp = fopen("/data/output.pcm","a+b");
		if(fp)
		{
			__android_log_print(ANDROID_LOG_INFO,"TTS","------------------writeAudioData-2\r\n");
			fwrite((char*)audioData,1,sampleCount*sizeof(SVOX_16S),fp);
			fclose(fp);
		}		
#endif
#endif

		return AUDIO_OK;
	};
};

//////////////////////////////////////////////////////////////////////////
bool MySvoxEngine::TE_SetResourcePath(char* lpPath,char* lpAppName,SvoxOutputPcm* pOutput)
{
	m_pSvoxOutput = pOutput;
	strcpy(m_szPath,lpPath);
	strcat(m_szPath,"/Svox/resources");
	strcpy(m_appName,lpAppName);
	
	strcpy(m_szPathText,lpPath);
	strcat(m_szPathText,"/TTSText/");
	
	strcpy(m_szPathVoice,lpPath);
	strcat(m_szPathVoice,"/Voice/");
	return true;
}

svox::asr::Recognizer *g_asr = 0;
bool MySvoxEngine::TE_ASR(char* lpRuleName, char* lpRuleContext,char* lpLanguage,char** lpResult)
{
	if(g_asr)
		return false;
	
	MY_SAPI_DB_PRINTF("1-----MySvoxEngine::TE_ASR: lpRuleName = %s\r\n",lpRuleName);
	svox::common::Configuration   config;	
	svox::asr::Recognizer        *asr = svox::asr::Recognizer::New();
	MY_SAPI_DB_PRINTF("2-----MySvoxEngine::TE_ASR: lpRuleName = %s\r\n",lpRuleName);
	g_asr = asr;
	svox::asr::Grammar           *grammar;
	svox::asr::RecognitionResult *result;

#ifdef SUPPORT_ASR_VAR_VALUE
	//判断是否有可变量参数
	//<@text:xxx>
	bool bHaveVarMode = false;
	char* lpStart = strstr(lpRuleContext,"<@text:");
	char szVarName[256];
	char szVarName_In[256];
	if(lpStart && m_pSvoxOutput)
	{
		char* lpEnd = strstr(lpStart,">");
		if(lpEnd && (lpEnd - lpStart) < 200)
		{
			int nLen = lpEnd - lpStart;
			strncpy(szVarName,lpStart,nLen + 1);
			strncpy(szVarName_In,lpStart+1,nLen - 1);
			bHaveVarMode = true;
			MY_SAPI_DB_PRINTF("MySvoxEngine::TE_ASR: szVarName = %s\r\n",szVarName);
			MY_SAPI_DB_PRINTF("MySvoxEngine::TE_ASR: szVarName_In = %s\r\n",szVarName_In);
		}
	}

	svox::climax::ComplexListManager *climax = 0;
	if(bHaveVarMode)
	{
		climax = svox::climax::ComplexListManager::New();
	}
#endif
	
	MY_SAPI_DB_PRINTF("MySvoxEngine::TE_ASR: lpRuleName = %s\r\n",lpRuleName);
	MY_SAPI_DB_PRINTF("MySvoxEngine::TE_ASR: lpRuleContext = %s\r\n",lpRuleContext);
	MY_SAPI_DB_PRINTF("MySvoxEngine::TE_ASR: lpLanguage = %s\r\n",lpLanguage);
	MY_SAPI_DB_PRINTF("MySvoxEngine::TE_ASR: m_szPath = %s\r\n",m_szPath);
	MY_SAPI_DB_PRINTF("MySvoxEngine::TE_ASR: m_appName = %s\r\n",m_appName);
	
	// initialize the speech recognizer for en-US
	MY_SAPI_DB_PRINTF("1-1\r\n");
	config.setStaticResourcePath(m_szPath);
#ifdef SUPPORT_ASR_VAR_VALUE
	if(bHaveVarMode)
	{
		config.setNBestHypotheses(5);
	}
#endif
	MY_SAPI_DB_PRINTF("2-2\r\n");
	ASR_ERROR_CHECK(asr->initialize(/*lpLanguage*/"en-US", &config));
	
#ifdef SUPPORT_ASR_VAR_VALUE
	const svox::climax::ColumnID columnId = 0;
	if(bHaveVarMode)
	{
		ERROR_CHECK_CLM(climax->setStoreSpellings(true));
		ERROR_CHECK_CLM(climax->addColumn());
		ERROR_CHECK_CLM(climax->setConfigurationDone());
	}
#endif
	// load language models for the oneshot destination input and activate it
	MY_SAPI_DB_PRINTF("3-3\r\n");
	ASR_ERROR_CHECK(asr->loadGrammar(m_appName, svox::asr::Grammar::NORMAL, 0, 0, grammar));
	MY_SAPI_DB_PRINTF("4-4\r\n");
#ifdef SUPPORT_ASR_VAR_VALUE
	if(bHaveVarMode)
	{
		ERROR_CHECK_ASR(grammar->registerListManager(szVarName_In, climax, 0));
	}	
#endif
	ASR_ERROR_CHECK(asr->activateGrammar(grammar));
	MY_SAPI_DB_PRINTF("5-5\r\n");
	GRM_ERROR_CHECK(grammar->deactivateAllRules());	
	MY_SAPI_DB_PRINTF("6-6\r\n");
	GRM_ERROR_CHECK(grammar->activateRule(lpRuleName));
	MY_SAPI_DB_PRINTF("7-7\r\n");

#ifdef SUPPORT_ASR_VAR_VALUE
	if(bHaveVarMode)
	{
		int idx = 0;
		for(idx = 0;; idx++)
		{
			char* lp = m_pSvoxOutput->ITF_GetASRVarValue(szVarName_In,idx);
			if(!lp)
				break;

			MY_SAPI_DB_PRINTF("MySvoxEngine::TE_ASR: ITF_GetASRVarValue idx=%d, value=%s\r\n",idx,lp);
			ERROR_CHECK_CLM(climax->beginEntry());
			ERROR_CHECK_CLM(climax->setColumnValueBySpelling(columnId, svox::common::String(lp)));
			ERROR_CHECK_CLM(climax->addEntry());
		}

		ERROR_CHECK_CLM(climax->performG2P(svox::common::String("en-US"), &config));
		ERROR_CHECK_ASR(asr->prepareRecognition());
	}
#endif
	
	// Add by Flex.wang
	MyAudioInputInterface* myAI = new MyAudioInputInterface();
	GRM_ERROR_CHECK(asr->setAudioInput(myAI));
	MY_SAPI_DB_PRINTF("8-8\r\n");
	// End add	
	
	// start recognition
	//std::cout << "Please speak!" << std::endl << std::endl;	
	ASR_ERROR_CHECK(asr->startRecognition());
	MY_SAPI_DB_PRINTF("9-9\r\n");
	asr->waitForRecognitionResult();	
	MY_SAPI_DB_PRINTF("10-10\r\n");
	
	// print results
	ASR_ERROR_CHECK(asr->getRecognitionResults(result));
	if (0 != result)
	{
		int m = 0;
		char szTempWord[1024];
		for (int i = 0; i < result->getNumUtterances(); i++)
		{
			const svox::asr::RecognizedUtterance *utt = result->getUtterance(i);
			memset(szTempWord,0,1024);
			for (int j = 0; j < utt->getNumWords(); j++)
			{
				const svox::asr::RecognizedWord *word = utt->getWord(j);
				char* lpR = (char*)word->getSpelling();
				if(lpR)
				{
					int nOrgLen = strlen(szTempWord);
					int nNowLen = strlen(lpR);
					
					if( (nOrgLen + nNowLen) < 1020 )
					{
						if(szTempWord[0])
						{
							strcat(szTempWord," ");
						}
						strcat(szTempWord,lpR);
					}
				}
			}

			if(szTempWord[0])
			{
				MY_SAPI_DB_PRINTF("SVOX-result:%s\r\n",szTempWord);
				lpResult[m] = new char[strlen(szTempWord) + 10];
				memset(lpResult[m],0,strlen(szTempWord) + 10);
				strcpy(lpResult[m],szTempWord);
				m++;
			}
		}
	}//end if
	
	// unload grammar and deinitialize speech recognizer
	asr->unloadGrammar(grammar);
	asr->deinitialize();
	delete myAI;
	g_asr = 0;
	delete asr;	
	return false;
}


PrompterInterface *g_prompter = 0;
bool MySvoxEngine::TE_TTS(char* lpTTSText,char* lpLanguage,bool isFile)
{
    MY_SAPI_DB_PRINTF("TE_TTS-----1");
	if(g_prompter)
		return false;
	
	MY_SAPI_DB_PRINTF("TE_TTS-----2");
	PrompterInterface *prompter = 0;
	PrompterManager *prompterMgr = PrompterManager::getInstance();
	char szTempTTS[MY_MAX_STRING_COUNT_HUGE];
	strcpy(szTempTTS,m_szPath);	
	strcat(szTempTTS,"/TTS/Plus");	
	prompterMgr->initialize(szTempTTS);
	prompterMgr->createPrompterInstance("SDS", prompter);
	
	MY_SAPI_DB_PRINTF("TE_TTS-----222222222222222222222222222222222222222222222222222222222");
	MyOutSound* myout = new MyOutSound(m_pSvoxOutput);
	prompter->setAudioOutput(myout);
	prompter->setVoice(lpLanguage);
	g_prompter = prompter;
	MY_SAPI_DB_PRINTF("TE_TTS-----3");
	MY_SAPI_DB_PRINTF("TE_TTS-----33333333333333333333333333333333333333333333333333333333");
//	m_pSvoxOutput->Start();
	if(isFile)
	{
		if(lpTTSText[0] != '/')
		{
			char szPathTFile[MY_MAX_STRING_COUNT_HUGE];
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
	MY_SAPI_DB_PRINTF("TE_TTS-----4");
	prompter->wait();
//	m_pSvoxOutput->Stop();
	g_prompter = 0;
	MY_SAPI_DB_PRINTF("TE_TTS-----5");
	prompterMgr->destroyPrompterInstance(prompter);	
    PrompterManager::destroyInstance();
	delete myout;
	return true;
}

bool MySvoxEngine::TE_Voice(char* lpVoiceFile)
{
	MY_SAPI_DB_PRINTF("TE_Voice-----1");
	if(g_prompter)
		return false;
	
	MY_SAPI_DB_PRINTF("TE_Voice-----2");
	PrompterInterface *prompter = 0;
	PrompterManager *prompterMgr = PrompterManager::getInstance();        
	char szTempTTS[MY_MAX_STRING_COUNT_HUGE];
	strcpy(szTempTTS,m_szPath);
	strcat(szTempTTS,"/TTS/Plus");	
	MY_SAPI_DB_PRINTF("TE_Voice-----2-1,szTempTTS=%s",szTempTTS);
	prompterMgr->initialize(szTempTTS);
	MY_SAPI_DB_PRINTF("TE_Voice-----2-2,szTempTTS=%s",szTempTTS);
	prompterMgr->createPrompterInstance("SDS", prompter);
	MyOutSound* myout = new MyOutSound(m_pSvoxOutput);
	prompter->setAudioOutput(myout);
	MY_SAPI_DB_PRINTF("TE_Voice-----2-3");
	g_prompter = prompter;
	prompter->setVoice("en-US");
	MY_SAPI_DB_PRINTF("TE_Voice-----3");
	//m_pSvoxOutput->Start();
	if(lpVoiceFile[0] != '/')
	{
		char szPathTFile[MY_MAX_STRING_COUNT_HUGE];
		strcpy(szPathTFile,m_szPathVoice);
		strcat(szPathTFile,lpVoiceFile);
		MY_SAPI_DB_PRINTF("TE_Voice-----szPathTFile = %s",szPathTFile);
		prompter->playAudioFile(szPathTFile);
	}else
	{
		MY_SAPI_DB_PRINTF("TE_Voice-----lpVoiceFile = %s",lpVoiceFile);
		prompter->playAudioFile(lpVoiceFile);
	}
	
	MY_SAPI_DB_PRINTF("TE_Voice-----4");
	prompter->wait();
	MY_SAPI_DB_PRINTF("TE_Voice-----5");
	//m_pSvoxOutput->Stop();
	g_prompter = 0;
	prompterMgr->destroyPrompterInstance(prompter);	
    PrompterManager::destroyInstance();
	delete myout;
	return true;
}

void MySvoxEngine::TE_Stop()
{
	if(g_asr)
		g_asr->abortRecognition();
	
	if(g_prompter)
		g_prompter->abort();
}
