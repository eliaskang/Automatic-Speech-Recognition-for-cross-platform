/*** Prompter Demo Application *******************************************************************/

//#include <iostream>
#include <stdlib.h>
//#include <string>
#include<android/log.h>
#include "PrompterManager.hpp"
#include "PrompterInterface.hpp"
#include "AudioOutputInterface.hpp"

using namespace svox;
using svox::prompter::PrompterInterface;
using svox::prompter::PrompterManager;


/* poor man's error handling: */
#define ERROR_CHECK(func, ok) errorCheck(func, ok, __LINE__, #func);
void errorCheck(prompter::ResultCode resultCode, prompter::ResultCode ok, SVOX_32S line, const char *statement)
{
    if (ok != resultCode)
    {
       // std::cout << "Error in line " << line << " \"" << statement << "\"" << std::endl;
       // std::cout << "(error code " << resultCode << ")" << std::endl;
		__android_log_print(ANDROID_LOG_INFO,"Error in line:","");
		__android_log_print(ANDROID_LOG_INFO,"error code:","");
        exit(resultCode);
    }
}

#include <stdio.h>
class MyOutSound:public svox::prompter::AudioOutputInterface
{
public:
	MyOutSound(){};
	~MyOutSound(){};

	ResultCode waitUntilProcessed(){return AUDIO_OK;};
	ResultCode writeAudioData(const SVOX_16S *audioData, SVOX_32S sampleCount)
	{
		__android_log_print(ANDROID_LOG_INFO,"TTS","------------------writeAudioData-1\r\n");
		FILE* fp = fopen("/data/output.pcm","a+b");
		if(fp)
		{
			__android_log_print(ANDROID_LOG_INFO,"TTS","------------------writeAudioData-2\r\n");
			fwrite((char*)audioData,1,sampleCount*sizeof(SVOX_16S),fp);
			fclose(fp);
		}
		return AUDIO_OK;
	};


};

#if 0
int main(int, char **)
{
	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------getInstance\r\n");
    PrompterInterface *prompter = 0;
    PrompterManager *prompterMgr = PrompterManager::getInstance();

	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------initialize\r\n");
    /* init the prompter manager with the path to svoxconfig.txt */
    ERROR_CHECK(prompterMgr->initialize("/data/build/Svox/resources/TTS/Plus"), prompter::OK);

    /* create a prompter instance, giving it a unique name of your choice. */
	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------createPrompterInstance\r\n");
    ERROR_CHECK(prompterMgr->createPrompterInstance("SDS", prompter), prompter::OK);

	MyOutSound* myout = new MyOutSound;
	prompter->setAudioOutput(myout);

    /* load a language for the created prompter instance */
	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------setVoice\r\n");
    ERROR_CHECK(prompter->setVoice("en-US"), prompter::OK);

    //std::cout << std::endl << "Welcome!" << std::endl;
	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------Welcome!\r\n");
    prompter->playString("Welcome! "); /* synthesize a text */
    prompter->wait();                  /* wait until audio data is played */

	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------test.wav!\r\n");
    //std::cout << std::endl << "Playing an audio file --> test.wav." << std::endl;
    prompter->playString("Playing an audio file."); /* synthesize a text */
    prompter->playAudioFile("test.wav");            /* add the contend of the handed wav file to the audiostream */
    prompter->wait();                               /* wait until audio data is played */

	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------Reading a text file.!\r\n");
    //std::cout << std::endl << "Reading a text file --> test.txt." << std::endl;
    prompter->playString("Reading a text file."); /* synthesize a text */
    //prompter->setVoice("voictest.wav");
    prompter->playTextFile("test.txt");   /* synthesize the contend of the handed txt file */
    prompter->wait();                             /* wait until audio data is played */

    //std::cout << std::endl << "Interactive mode.";
	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------Interactive mode.!\r\n");
    prompter->playString("Interactive mode."); /* synthesize a text */
	prompter->wait();  
	__android_log_print(ANDROID_LOG_INFO,"TTS","Please enter a text message to synthesize, or 'q' to quit:!\r\n");
    prompter->playString("Please enter a text message to synthesize, or 'q' to quit:"); /* synthesize */
    prompter->wait();                          /* wait until audio data is played */

#if 0
    std::string message;
    for (;;)
    {
        std::cout << std::endl << "Please enter the text message to synthesize, or 'q' to quit: ";
        std::getline(std::cin, message);
        if (message == "q")
        {
            break;
        }
        prompter->playString(message.c_str(),"ISS"); /* synthesize a text using the ISS context */
        prompter->wait();                            /* wait until audio data is played */
    }
#endif
	__android_log_print(ANDROID_LOG_INFO,"TTS","Good bye!\r\n");
    prompter->playString("Good bye!");
    prompter->wait();

    // deinitialize prompter
    prompterMgr->destroyPrompterInstance(prompter);
    PrompterManager::destroyInstance();
	delete myout;

    return 0;
}
#else

#include <string.h>
#include <strings.h>
#define stricmp strcasecmp
#include <stdarg.h>
#include <stdio.h>

//////////////////////////////////////////////////////////////////////////
#define MAX_STRING_COUNT_HUGE 512
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
	__android_log_print(ANDROID_LOG_INFO,"SAPI_Printf",szTempInfo);
}

bool TE_TTS(char* lpTTSText,char* lpLanguage,bool isFile)
{
    __android_log_print(ANDROID_LOG_INFO,"TTS","------------------getInstance\r\n");
    PrompterInterface *prompter = 0;
    PrompterManager *prompterMgr = PrompterManager::getInstance();
	
	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------initialize\r\n");
    /* init the prompter manager with the path to svoxconfig.txt */
    ERROR_CHECK(prompterMgr->initialize("/data/build/Svox/resources/TTS/Plus"), prompter::OK);
	
    /* create a prompter instance, giving it a unique name of your choice. */
	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------createPrompterInstance\r\n");
    ERROR_CHECK(prompterMgr->createPrompterInstance("SDS", prompter), prompter::OK);
	
	MyOutSound* myout = new MyOutSound;
	prompter->setAudioOutput(myout);
	
    /* load a language for the created prompter instance */
	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------setVoice\r\n");
    ERROR_CHECK(prompter->setVoice("en-US"), prompter::OK);
	
    //std::cout << std::endl << "Welcome!" << std::endl;
	__android_log_print(ANDROID_LOG_INFO,"TTS","------------------Welcome!\r\n");
    prompter->playString("Welcome! "); /* synthesize a text */
    prompter->wait();                  /* wait until audio data is played */
	
    // deinitialize prompter
    prompterMgr->destroyPrompterInstance(prompter);
    PrompterManager::destroyInstance();
	delete myout;

	return true;
}

#endif