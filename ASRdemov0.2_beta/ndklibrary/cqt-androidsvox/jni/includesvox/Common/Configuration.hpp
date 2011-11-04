/*
 * @file   Configuration.hpp
 * @date   2010/04/06 11:49
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
/*! \brief Configuration  declaration/implementation.
 *
 * @file   Configuration.hpp
 *
 * This file contains the declaration/implementation of the Configuration.
 */
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include "SVOX_Types.h"
#include "String.hpp"
#include "Array.hpp"

// stolen from recogx_raw:
#define DEF_INVALID_VALUE -2  // ugly, we should remove this...


namespace svox
{
namespace common
{

/** \brief Class thats hold the configuration details relevant for the recognizer and speech dialog components.
 *
 * This class collects all configuration parameters for the
 * Recognizer and DialogControl class. It is mainly used to encapsulate the
 * configuration file operations.
 * The setter functions for string parameters return false if the internal memory allocation fails. In this case
 * the old configuration value is not modified.
 */
class Configuration
{
public:
    Configuration();
    // copy ctor is not defined, its standard implementation does the right things
    /**
     * Loads configuration values from a file.  The format of
     * the configuration files is like this:
     *
     * \code
     *  COMMON.STATIC_RESOURCE_PATH = "c:/mypath/myfolder"
     *  ASR.VSS_KEY_N_BEST_HYPOTHESES = 5
     * \endcode
     *
     * It is possible to set and reuse (helper) variables:
     *
     * \code
     *  BASE_PATH = "c:/mypath"
     *  SEPARATOR = "/"
     *  COMMON.STATIC_RESOURCE = BASE_PATH..SEPARATOR.."myfolder"
     *  ASR.VSS_KEY_N_BEST_HYPOTHESES = 5
     * \endcode
     *
     * Possible configuration keys are listed with the getter/setter
     * methods in this class.  Configuration keys that are not present
     * in the file will not be changed by calling this method.
     *
     * @param configFile path name of the file to load
     * @returns \a true if loading succeeded
     * @returns \a false in case of any error
     */

    bool loadFromFile(const SVOX_Char *configFile);

   /**
     *  Creates a path string to an ASR language resource file.
     *  Example: config.buildLanguageResourcePath("en-US", config.getMMBFilename());
     *           returns:
     *           "<STATIC_RESOURCE_PATH>/ASR/Languages/en-US/svoxasr.mmb"
     *
     *  @param locale the language and country code, e.g. en-US
     *  @param filename the file to access
     *
     *  @return path to the passed file (or an invalid string if memory allocation fails)
     */
    String buildLanguageResourcePath(const SVOX_Char *locale, const SVOX_Char *filename) const;

   /**
     *  Creates a path string to a grammar file.
     *  Example: config.buildGrammarPath("en-US", "osd.brr");
     *           returns:
     *           "<STATIC_RESOURCE_PATH>/ASR/Grammars/en-US/osd.brr"
     *
     *  @param locale the language and country code, e.g. en-US
     *  @param filename the file to access
     *  @param suffix optional string which is appended to the filename
     *
     *  @return path to the passed file (or an invalid string if memory allocation fails)
     */
    String buildGrammarPath(const SVOX_Char *locale, const SVOX_Char *filename, const SVOX_Char *suffix = 0) const;

    /**
     *  Creates a path string to a speech dialog application file.
     *  Example: config.buildDialogPath("vde.dapp");
     *           returns:
     *           "<STATIC_RESOURCE_PATH>/Dialog/vde.dapp"
     *
     *  @param filename the file to access
     *
     *  @return path to the passed file (or an invalid string if memory allocation fails)
     */
    String buildDialogPath(const SVOX_Char *filename = 0) const;


    /**
     *  Creates a path string to a TTS file or directory.
     *
     *  @param filename file or subpath to access
     *
     *  @return path to the passed file (or an invalid string if memory allocation fails)
     */
    String buildTTSPath(const SVOX_Char *filename) const;

    /**
     *  With this function the customer can add a file/path mapping to load a SVOX resource file from a
     *  customer specific filesystem location instead of the common SVOX resource directory structure.
     *  Complete file paths as well as parts of a path can be mapped with this function.
     *
     *  Example:
     *  Configuration::addPathMapping("/svox/ASR/Grammars/en-US/osd.brr", "/mnt/cdrom/osd_us.brr");
     *  Configuration::addPathMapping("Grammars/en-US", "grammars_us");
     *
     *  The mappings are applied in the order they are added. If you swap the two calls in the example above,
     *  the second mapping will have no effect because the first mapping already replaces "Grammars/en-US".
     *
     *  Filenames and (sub-)paths within a binary resource file cannot be modified with path mappings. 
     *
     *  @param from the search string
     *  @param to   the replacement
     *  @return     true if successful
     */
    bool addPathMapping(const SVOX_Char *from, const SVOX_Char *to);

    /**
     *  This method returns true, if a previous call of a (string) setter method failed.
     *  This can happen if the setter method runs out of memory
     */
    bool setterFailed() const;



public: // generated code block

    // --- COMMON configuration -----------------------------------------------------------------------

    /**
     *  The path from which all static resources are loaded; this path may point to a read-only file system. 
     * 
     *  Config file key: COMMON.STATIC_RESOURCE_PATH 
     *  Default value:   "." 
     */
    String getStaticResourcePath() const;
    bool setStaticResourcePath(const SVOX_Char *value);

    /**
     *  The path which is used to store resources at runtime; this requires write access to the file system; if this path is not set, staticResourcePath is used instead. 
     * 
     *  Config file key: COMMON.DYNAMIC_RESOURCE_PATH 
     *  Default value:   not set 
     */
    String getDynamicResourcePath() const;
    bool setDynamicResourcePath(const SVOX_Char *value);


    // --- ASR configuration -----------------------------------------------------------------------

    /**
     *  The name of the file containing the binary HMM representation. 
     * 
     *  Config file key: ASR.MMB_FILENAME 
     *  Default value:   "svoxasr.mmb" 
     */
    String getMMBFilename() const;
    bool setMMBFilename(const SVOX_Char *value);

    /**
     *  The name of the file containing the binary preprocessing representation. 
     * 
     *  Config file key: ASR.VVB_FILENAME 
     *  Default value:   "svoxasr.vvb" 
     */
    String getVVBFilename() const;
    bool setVVBFilename(const SVOX_Char *value);

    /**
     *  The name of the file containing the binary THS representation. 
     * 
     *  Config file key: ASR.THS_FILENAME 
     *  Default value:   "svoxasr.ths" 
     */
    String getTHSFilename() const;
    bool setTHSFilename(const SVOX_Char *value);

    /**
     *  The name of the G2P phoneme inventory file. 
     * 
     *  Config file key: ASR.G2PINV_FILENAME 
     *  Default value:   "svoxasr.inv" 
     */
    String getINVFilenameG2P() const;
    bool setINVFilenameG2P(const SVOX_Char *value);

    /**
     *  The name of the file containing the G2P mapping neuronal network. 
     * 
     *  Config file key: ASR.G2PNET_FILENAME 
     *  Default value:   "svoxasr.net" 
     */
    String getNETFilenameG2P() const;
    bool setNETFilenameG2P(const SVOX_Char *value);

    /**
     *  The name of the file containing the exception lookup lexicon for G2P. 
     * 
     *  Config file key: ASR.G2PXLB_FILENAME 
     *  Default value:   "svoxasr.xlb" 
     */
    String getXLBFilenameG2P() const;
    bool setXLBFilenameG2P(const SVOX_Char *value);

    /**
     *  The name of the LID phoneme inventory file. 
     * 
     *  Config file key: ASR.LIDINV_FILENAME 
     *  Default value:   "svoxlid.inv" 
     */
    String getINVFilenameLID() const;
    bool setINVFilenameLID(const SVOX_Char *value);

    /**
     *  The name of the file containing the LID neuronal network. 
     * 
     *  Config file key: ASR.LIDNET_FILENAME 
     *  Default value:   "svoxlid.net" 
     */
    String getNETFilenameLID() const;
    bool setNETFilenameLID(const SVOX_Char *value);

    /**
     *  The name of the file containing the exception lookup lexicon for LID. 
     * 
     *  Config file key: ASR.LIDXLB_FILENAME 
     *  Default value:   "svoxlid.xlb" 
     */
    String getXLBFilenameLID() const;
    bool setXLBFilenameLID(const SVOX_Char *value);

    /**
     *  The name of the VSVM configuration file. 
     * 
     *  Config file key: ASR.VSVM_CONFIG_FILENAME 
     *  Default value:   not set 
     */
    String getVSVMConfigFilename() const;
    bool setVSVMConfigFilename(const SVOX_Char *value);

    /**
     *  The name of the phoneme mapping file. 
     * 
     *  Config file key: ASR.PHONMAP_FILENAME 
     *  Default value:   "phonmap.inv" 
     */
    String getPhonmapFilename() const;
    bool setPhonmapFilename(const SVOX_Char *value);

    /**
     *  The name of the vocabulary file containing OSD commands. 
     * 
     *  Config file key: ASR.OSD_COMMANDVOC_FILENAME 
     *  Default value:   not set 
     */
    String getOSDCommandVocFilename() const;
    bool setOSDCommandVocFilename(const SVOX_Char *value);

    /**
     *  The suffix of binary resource files. 
     * 
     *  Config file key: ASR.BINARY_RESOURCE_SUFFIX 
     *  Default value:   ".brr" 
     */
    String getBinaryResourceSuffix() const;
    bool setBinaryResourceSuffix(const SVOX_Char *value);

    /**
     *   
     * 
     *  Config file key: ASR.VSF_KEY_NOISE_ENVIRONMENT  
     *  Default value:   not set 
     */
    SVOX_32S getNoiseEnvironment() const;
    void setNoiseEnvironment(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSF_KEY_NOISE_REDUCTION  
     *  Default value:   not set 
     */
    SVOX_32S getNoiseReduction() const;
    void setNoiseReduction(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSF_KEY_VAD_TYPE 
     *  Default value:   not set 
     */
    SVOX_32S getVadType() const;
    void setVadType(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSFX_KEY_LISTEN_THRESHOLD 
     *  Default value:   -1 
     */
    SVOX_32S getListenThreshold() const;
    void setListenThreshold(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSFX_KEY_LISTEN_SEEKBACK 
     *  Default value:   17 
     */
    SVOX_32S getListenSeekback() const;
    void setListenSeekback(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSFX_KEY_LISTEN_WINDOW_SIZE 
     *  Default value:   8 
     */
    SVOX_32S getListenWindowSize() const;
    void setListenWindowSize(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSS_KEY_N_BEST_HYPOTHESES 
     *  Default value:   not set 
     */
    SVOX_32S getNBestHypotheses() const;
    void setNBestHypotheses(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSV_KEY_WSP 
     *  Default value:   not set 
     */
    SVOX_32S getWsp() const;
    void setWsp(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSS_KEY_MINSTABLE_TIME 
     *  Default value:   not set 
     */
    SVOX_32S getMinstableTime() const;
    void setMinstableTime(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSS_KEY_MIN_ENDPOINTSTABLE_TIME 
     *  Default value:   not set 
     */
    SVOX_32S getMinEndpointstableTime() const;
    void setMinEndpointstableTime(SVOX_32S value);

    /**
     *  Timeout between start of recognition and first speech signal (in ms) 
     * 
     *  Config file key: ASR.INITIAL_TIMEOUT 
     *  Default value:   SVOX_32S_MAX 
     */
    SVOX_32S getInitialTimeout() const;
    void setInitialTimeout(SVOX_32S value);

    /**
     *  Maximum duration of utterance (in ms) 
     * 
     *  Config file key: ASR.UTTERANCE_TIMEOUT 
     *  Default value:   SVOX_32S_MAX 
     */
    SVOX_32S getUtteranceTimeout() const;
    void setUtteranceTimeout(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSS_KEY_PRUNING_OFFSET 
     *  Default value:   not set 
     */
    SVOX_32S getPruningOffset() const;
    void setPruningOffset(SVOX_32S value);

    /**
     *   
     * 
     *  Config file key: ASR.VSS_KEY_FRAME_STARTUP_PENALTY 
     *  Default value:   not set 
     */
    SVOX_32S getFrameStartupPenalty() const;
    void setFrameStartupPenalty(SVOX_32S value);

    /**
     *  The size of the G2P word cache (in entries). Cache size 0 disables the cache. 
     * 
     *  Config file key: ASR.WORD_CACHE_SIZE 
     *  Default value:   0 
     */
    SVOX_32S getWordCacheSize() const;
    void setWordCacheSize(SVOX_32S value);

    /**
     *  The size of the G2P sentence cache (in entries). Cache size 0 disables the cache. 
     * 
     *  Config file key: ASR.SENTENCE_CACHE_SIZE 
     *  Default value:   0 
     */
    SVOX_32S getSentenceCacheSize() const;
    void setSentenceCacheSize(SVOX_32S value);

    /**
     *  Directory in which raw audio files are written for each recognition. 
     * 
     *  Config file key: ASR.AUDIO_CAPTURE_PATH 
     *  Default value:   not set 
     */
    String getAudioCapturePath() const;
    bool setAudioCapturePath(const SVOX_Char *value);

    /**
     *  For internal use only. 
     * 
     *  Config file key: ASR.INTERNAL_CONFIG 
     *  Default value:   not set 
     */
    String getAsrInternalConfig() const;
    bool setAsrInternalConfig(const SVOX_Char *value);


    // --- PROMPTER configuration -----------------------------------------------------------------------

    /**
     *  The lingware type which shall be used for prompting. 
     * 
     *  Config file key: PROMPTER.LINGWARE_TYPE 
     *  Default value:   "Plus" 
     */
    String getLingwareType() const;
    bool setLingwareType(const SVOX_Char *value);


    // --- DIALOG configuration -----------------------------------------------------------------------

    /**
     *  The function with this name is called (via ApplicationController) if the Diane engine emits a GUI text. 
     * 
     *  Config file key: DIALOG.GUIPROMPT_CALL 
     *  Default value:   "BackgroundSystem.guiprompt" 
     */
    String getGUIPromptCall() const;
    bool setGUIPromptCall(const SVOX_Char *value);

    /**
     *  The audio file which contains the "beep" sound which is typically played before speech recognition is started. 
     * 
     *  Config file key: DIALOG.BEEP_PROMPT 
     *  Default value:   "beep.wav" 
     */
    String getBeepPrompt() const;
    bool setBeepPrompt(const SVOX_Char *value);

    /**
     *  The prompter context which is used by the speech dialog for speech output. 
     * 
     *  Config file key: DIALOG.DIALOG_PROMPTER_CONTEXT 
     *  Default value:   "ISS" 
     */
    String getDialogPrompterContext() const;
    bool setDialogPrompterContext(const SVOX_Char *value);

    /**
     *  This event name is used for PTT barge-in and for PTT activation of the recognizer. 
     * 
     *  Config file key: DIALOG.PTT_EVENT_NAME 
     *  Default value:   "PTT" 
     */
    String getPTTEventName() const;
    bool setPTTEventName(const SVOX_Char *value);


private: // generated code block

    // COMMON configuration values
    String mStaticResourcePath;
    String mDynamicResourcePath;

    // ASR configuration values
    String mMMBFilename;
    String mVVBFilename;
    String mTHSFilename;
    String mINVFilenameG2P;
    String mNETFilenameG2P;
    String mXLBFilenameG2P;
    String mINVFilenameLID;
    String mNETFilenameLID;
    String mXLBFilenameLID;
    String mVSVMConfigFilename;
    String mPhonmapFilename;
    String mOSDCommandVocFilename;
    String mBinaryResourceSuffix;
    SVOX_32S mNoiseEnvironment;
    SVOX_32S mNoiseReduction;
    SVOX_32S mVadType;
    SVOX_32S mListenThreshold;
    SVOX_32S mListenSeekback;
    SVOX_32S mListenWindowSize;
    SVOX_32S mNBestHypotheses;
    SVOX_32S mWsp;
    SVOX_32S mMinstableTime;
    SVOX_32S mMinEndpointstableTime;
    SVOX_32S mInitialTimeout;
    SVOX_32S mUtteranceTimeout;
    SVOX_32S mPruningOffset;
    SVOX_32S mFrameStartupPenalty;
    SVOX_32S mWordCacheSize;
    SVOX_32S mSentenceCacheSize;
    String mAudioCapturePath;
    String mAsrInternalConfig;

    // PROMPTER configuration values
    String mLingwareType;

    // DIALOG configuration values
    String mGUIPromptCall;
    String mBeepPrompt;
    String mDialogPrompterContext;
    String mPTTEventName;

    bool mSetterFailed;

    struct Mapping
    {
        String mFrom;
        String mTo;
    };
    Array<Mapping> mPathMapping;

    void mapFilePath(String &filepath) const;
};


}} // namespaces

#endif // CONFIGURATION_HPP
