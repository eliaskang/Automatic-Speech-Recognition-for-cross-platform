/*
 * @file   Recognizer.hpp
 * @date   2010/03/24 10:56
 * @author joachim.grill
 *
 * Copyright by SVOX Deutschland GmbH
 *
 * Transmittal, reproduction, dissemination and/or editing of this
 * source code as well as utilization of its contents and communication
 * thereof to others without express authorization are prohibited.
 * Offenders will be held liable for payment of damages.  All rights
 * created by patent grant or registration of a utility model or design
 * patent are reserved.
 */
/*! \brief Declaration of the SVOX speech recognizer class.
 * 
 * @file   Recognizer.hpp
 *
 * This file contains the declaration of the Recognizer class, which
 * is used to control the SVOX speech recognizer.
 */
#ifndef SVOX_ASR_RECOGNIZER_HPP
#define SVOX_ASR_RECOGNIZER_HPP


/*** Includes *********************************************************************************************************/

#include "AudioInputInterface.hpp"
#include "RecognizerCallback.hpp"
#include "RecognitionResult.hpp"
#include "Grammar.hpp"
#include "Configuration.hpp"
#include "RecognizerTypes.hpp"

namespace svox {
/** \brief A namespace for the application interface of the speech recognizer 
 *
 * this namespace collects all the classes, functions and definitions
 * that are relevant for the speech recognizer.
 */
namespace asr {


/*** Class definition *************************************************************************************************/

/** \brief This class defines the interface of the SVOX Speech Recognizer engine.
 *  
 * The class Recognizer is the main interface for operating the SVOX
 * Speech recognizer engine. It can be parametrized with
 * svox::asr::Grammar instances to supply a language model. It must be
 * configured with an appropriate AudioInputInterface instance. 
 */
class Recognizer
{
public:

    enum ParameterType
    {
        PARAMETER_N_BEST,                  //!< size of n-best hypothesis list
        PARAMETER_MIN_ENDPOINTSTABLE_TIME, //!< minimum endpoint stable time (in ms)

        PARAMETER_INITIAL_TIMEOUT,         //!< timeout between startRecognition() and first speech signal (in ms)
        PARAMETER_UTTERANCE_TIMEOUT,       //!< maximum duration of utterance (in ms)

        PARAMETER_AUDIOCAPTURE_CALLBACK,   //!< set this parameter to 1 to received audio data via audioDataCallback();
                                           //!< sample rate of the audio data is 11kHz, as it is passed to the VSR core.
        PARAMETER_AUDIOCAPTURE_FILE        //!< set this parameter to 1 to write a raw audio file for each
                                           //!< recognition run; the files are stored in the dynamicResourcePath, each
                                           //!< file gets an unique timestamp as file name
    };


    /**
     *  Creates a new recognizer instance.
     *
     *  @return Recognizer object
     */
    static Recognizer *New();


    /**
     *  Releases the Recognizer object. If the Recognizer is initialized, it is de-initialized automatically.
     *  @see deinitialize()
     */
    virtual ~Recognizer();


    /**
     *  Initializes the Recognizer instance. This method must be called before all other methods (except New()).
     *  It resets all parameters of the Recognizer instance.
     *  With an (optional) configuration object file paths and ASR parameters can be set.
     *
     *  Recognizer::initialize() loads the following files: <br>
     *  <br>
     *      \<STATIC_RESOURCE_PATH>/Languages/\<locale>/\<MMB_FILENAME> \n
     *      \<STATIC_RESOURCE_PATH>/Languages/\<locale>/\<VVB_FILENAME> \n
     *      \<STATIC_RESOURCE_PATH>/Grammars/\<locale>/\<OSD_COMMANDVOC_FILENAME> \n
     *  \n
     *  @param locale              locale to be used by the ASR: \<language>-\<COUNTRY> or \<language>\<COUNTRY>,
     *                             e.g. en-US, de-DE, frCA; for details see ISO 639-1 and ISO 3166
     *
     *  @param configuration       ASR configuration class
     *
     *  @return result code
     */
    virtual ResultCode initialize(const SVOX_Char *locale,
                                  const svox::common::Configuration *configuration = 0) = 0;

    /**
     *  De-initializes the Recognizer object. All loaded grammars are unloaded and released automatically.
     *  The grammar objects get invalid and may no longer be used!
     *
     *  @return result code
     */
    virtual ResultCode deinitialize() = 0;


    /**
     * Update values from Configuration. this method is used to update
     * configuration values within an already initialized instance of
     * the Recognizer. Not all configuration values can be changed,
     * because the semantic of this action would be unclear. 
     *
     * The following configuration items may not be changed :    
     * \li StaticResourcePath, 
     * \li DynamicResourcePath, 
     * \li MMBFilename, 
     * \li VVBFilename, 
     * \li THSFilename, 
     * \li INVFilenameG2P, 
     * \li NETFilenameG2P, 
     * \li XLBFilenameG2P, 
     * \li INVFilenameLID, 
     * \li NETFilenameLID, 
     * \li XLBFilenameLID, 
     * \li VSVMConfigFilename, 
     * \li PhonmapFilename, 
     * \li OSDCommandVocFilename, 
     * \li BinaryResourceSuffix, 
     *
     * @param configuration reference to Configuration object. Is used
     * to copy new configuration parameters from. Is not stored
     * internally, therefore may be destroyed once this method
     * returns. 
     *
     *  @return result code
     */ 
    virtual ResultCode updateConfiguration(const svox::common::Configuration &configuration) = 0;
    

    /**
     *  Install a callback class which will receive notifications for all recognition results, events, log messages.
     *
     *  @param callbacks an object with the callback implementations
     *
     *  @return result code
     */
    virtual ResultCode setCallback(RecognizerCallback *callbacks) = 0;


    /**
     *  Sets a class which provides audio data. This can be used to integrate a project specific audio device or to
     *  read audio data from file. If no audio interface is set with this method, the SVOX default audio interface
     *  implementation is used (if available).
     *
     *  @param audioInterface the interface which shall be used by the recognizer to request audio samples.
     *
     *  @return result code
     */
    virtual ResultCode setAudioInput(AudioInputInterface *audioInterface) = 0;


    /**
     *  Loads a (non-OSD) grammar. 
     *
     *  @param grammarName   the name of the grammar; the Recognizer loads the following files: <br>
     *                       \<staticResourcePath>/Grammars/\<language>\<COUNTRY>.\<grammarName>.voc <br>
     *                       \<staticResourcePath>/Grammars/\<language>\<COUNTRY>.\<grammarName>.grm <br>
     *
     *  @param grammar [out] returns a pointer to the grammar object      
     *
     *  @return result code
     */
    virtual ResultCode loadGrammar(const SVOX_Char *grammarName,
                                   Grammar         *&grammar) = 0;

    /**
     *  Loads a grammar. 
     *
     *  @param grammarName   the name of the grammar; the Recognizer loads the following files: <br>
     *                       grammarType == NORMAL: <br>
     *                           \<staticResourcePath>/Grammars/\<language>-\<COUNTRY>/\<grammarName>.voc <br>
     *                           \<staticResourcePath>/Grammars/\<language>-\<COUNTRY>/\<grammarName>.grm <br>
     *                       grammarType == OSD...: <br>
     *                           \<staticResourcePath>/Grammars/\<language>-\<COUNTRY>/\<grammarName>.brr <br>
     *
     *  @param grammarType   the grammar type (NORMAL, OSD, OSD_SUBLEVEL, ...)
     *
     *  @param filterType    for grammarType OSD_SUBLEVEL or OSD_SINGLE_SUBLEVEL this string defines the category
     *                       (state, city, ...) for which a filter shall be applied; not yet supported, at the moment
     *                       all filterValues must be unique
     *
     *  @param filterValue   for grammarType OSD_SUBLEVEL or OSD_SINGLE_SUBLEVEL this string must contain a hex-encoded
     *                       id pointing to a sublevel vocabulary which is used for recognition;
     *                       for all other grammarTypes this parameter must be 0
     *
     *  @param grammar [out] returns a pointer to the grammar object      
     *
     *  @return result code
     */
    virtual ResultCode loadGrammar(const SVOX_Char      *grammarName,
                                   Grammar::GrammarType grammarType,
                                   const SVOX_Char      *filterType,
                                   const SVOX_Char      *filterValue,
                                   Grammar              *&grammar) = 0;


    /**
     *  Loads a grammar. In addition to the loadGrammar() method with 5 parameters, this method accepts a
     *  grammar directory. It can be used to implement different OSD input orders (city street, street city) using
     *  the same OSD binary resources (brr), but different template grammar files.
     *
     *  For non-OSD grammars the grammarDirectory is ignored, and this method should not be used.
     *
     *  @param grammarDirectory this directory (as subdir within ASR/Grammars/language-COUNTRY) is used to load the
     *                          template grammars for OSD recognition. The directory is NOT used to load the BRR file,
     *                          it must be located in the ASR/Grammars/language-COUNTRY directory.
     *
     *  for other parameters see documentation of method above
     */
    virtual ResultCode loadGrammar(const SVOX_Char      *grammarName,
                                   Grammar::GrammarType grammarType,
                                   const SVOX_Char      *filterType,
                                   const SVOX_Char      *filterValue,
                                   const SVOX_Char      *grammarDirectory,
                                   Grammar              *&grammar) = 0;


    /**
     *  Releases the passed grammar.
     *
     *  @param grammar [in] grammar to release
     *
     *  @return result code
     */
    virtual ResultCode unloadGrammar(Grammar *grammar) = 0;


    /**
     *  Activates the passed grammar. Only activated grammars are used for speech recognition.
     *  A newly loaded grammar is not activated.
     *
     *  @param grammar [in] grammar to activate
     *
     *  @return result code
     */
    virtual ResultCode activateGrammar(Grammar *grammar) = 0;


    /**
     *  De-activates the passed grammar. Only activated grammars are used for speech recognition.
     *
     *  @param grammar [in] grammar to de-activate
     *
     *  @return result code
     */
    virtual ResultCode deactivateGrammar(Grammar *grammar) = 0;


    /**
     *  In some more complex use-cases time consuming grammar preparation is necessary before a recognition can be
     *  started. This can be triggered by calling this method. The method is also automatically called by
     *  startRecognition(), i.e. it is not mandatory to call this method before starting a recognition.
     *
     *  @return result code
     */
    virtual ResultCode prepareRecognition() = 0;


    /**
     *  Starts the speech recognition. All activated rules of all activated grammars are used by the recognizer.
     *  This method is asynchronous, it activates an internal worker thread which performs the recognition.
     *
     *  @return result code
     */
    virtual ResultCode startRecognition() = 0;


    /**
     *  With this method a running recognition can be aborted. This
     *  method does not block.  If this method succeeds, the
     *  recongitzer module will generate an ABORT event as soon as the
     *  recgnition has been canceled
     *
     *  @return result code
     */
    virtual ResultCode abortRecognition() = 0;


    /**
     *  This method blocks until the recognition is completed. If no recognition is running, it returns immediately.
     *
     *  @return result code
     */
    virtual ResultCode waitForRecognitionResult() = 0;


    /**
     *  Returns the recognition result of the last recognition.
     *
     *  @param results recognition results
     *
     *  @return result code
     */
    virtual ResultCode getRecognitionResults(RecognitionResult *&results) = 0;


    /**
     *  Sets a recognizer parameter. @see ParameterType for detailed description.
     *
     *  @param id    the parameter to change
     *  @param value this value is assigned to the parameter
     *
     *  @return result code
     */
    virtual ResultCode setParameter(ParameterType id, SVOX_32S value) = 0;


    /**
     *  Returns a detailed error message about the error of the last Recognizer method call.
     *
     *  @return result code
     */
    virtual const SVOX_Char *getLastErrorDetails() = 0;

};


}} // namespaces


#endif // SVOX_ASR_RECOGNIZER


/**********************************************************************************************************************/
