/*** CommandAndControl Demo Application *************************************************************************/
#define TEST_OTHER1
#include <stdio.h>

#ifdef TEST_OTHER
int main(int, char **)
{
	printf("Hello!");
	return 0;
}

#else
#include "Recognizer.hpp"

/*
 *   Main function:
 *   Press RETURN to start the recognition, or 'q' followed by RETURN to terminate the program.
 *   You can speak an (isolated) us-city.
 */
int main(int, char **)
{
    svox::common::Configuration   config;
    svox::asr::Recognizer        *asr = svox::asr::Recognizer::New();
    svox::asr::Grammar           *grammar;
    svox::asr::RecognitionResult *result;
	
    // initialize the speech recognizer for en-US
	printf("initialize the speech recognizer for en-US\r\n");
    config.setStaticResourcePath("../../resources");
    asr->initialize("en-US", &config);
	
    // load language models for the oneshot destination input and activate it
    asr->loadGrammar("cc", svox::asr::Grammar::NORMAL, 0, 0, grammar);
    asr->activateGrammar(grammar);
	
    grammar->deactivateAllRules();
    grammar->activateRule("global");
    grammar->activateRule("top");
	
	asr->startRecognition();
	asr->waitForRecognitionResult();
	
	// print results
	asr->getRecognitionResults(result);
	if (0 != result)
	{		
		const SVOX_Char * spelling = result->getUtterance(0)->getWord(0)->getSpelling();
		//std::cout << "Recognized : " << spelling << std::endl;            
		if ((svox::common::String("communication") == spelling)
			|| (svox::common::String("entertainment") == spelling))
		{
			grammar->deactivateAllRules();
			grammar->activateRule("global");
			grammar->activateRule(spelling);
		}
		else 
		{
			grammar->deactivateAllRules();
			grammar->activateRule("global");
			grammar->activateRule("top");
		}
	}
	
	
    // unload grammar and deinitialize speech recognizer
    asr->unloadGrammar(grammar);
    asr->deinitialize();
    delete asr;

	return 0;
}
#endif
