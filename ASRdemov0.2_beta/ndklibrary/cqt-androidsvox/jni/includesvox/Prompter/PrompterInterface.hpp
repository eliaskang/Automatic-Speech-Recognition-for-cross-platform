/*
 * @file   PrompterInterface.hpp
 * @date   2009/10/14 10:48
 * @author andreas.kirbach
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

/** \brief PrompterInterface definition.
 * 
 * @file   PrompterInterface.hpp
 *
 * This file contains the the definition of the prompter interface.
 */
#ifndef SVOX_PROMPTER_INTERFACE_HPP
#define SVOX_PROMPTER_INTERFACE_HPP

/* common */
#include "String.hpp"

/* pal */
#include "SVOX_Types.h"

/* interface types */
#include "PrompterTypes.hpp"

namespace svox 
{
/** \brief A namespace for the %prompter 
 *
 * This namespace collects all classes and symbols that are related to
 * the %prompter interface.
 */

namespace prompter 
{
using namespace svox::common;

class AudioOutputInterface;
class PrompterEventAdapter;

/* version number coded in 32Bit. 0xAABBCCDD */
/* AA: not used */
/* BB: main version: a change in this digit is indicating an incompatible 
                     interface change. For instance an application which 
                     requires 2.0.0 can not be started with a prompter 
                     implementing 1.9.0 or 3.0.0 */
/* CC: sub version: a change in this digit is indicating an interface 
                    extension, or a compatible change. For instance it is 
                    possible to start an application which requires 1.5.0 
                    with a prompter implementing 1.7.0 but it is not possible to
                    start it with a prompter implementing 1.4.0 */
/* DD: revision marker: a change in this digit is indicating a released 
                    prompter revision. This digit has to be incremented if fully 
                    compatible changes were made so it does not matter if an 
                    application which requires 1.0.5 is started with a prompter 
                    implementing 1.0.2 or 1.0.9. 
                    Of course it is always recommended to use the latest 
                    available prompter implementation.*/
#define P_MAIN 0x01
#define P_SUB  0x04
#define P_REV  0x00
#define PROMPTER_VERSION ((((P_MAIN << 8) | P_SUB) << 8) | P_REV)

/** \brief Interface for the SVOX prompting module.
 *
 * This abstract class defines the interface of the SVOX prompting module.
 * The module is capable of playing files and generating audio-data using 
 * the text to speech module from SVOX. The methods are nonblocking with 
 * the only exception of the wait method which will block until a currently 
 * active operation is finished.
 */
class PrompterInterface
{
public:
    /* VERSION */
    /** \brief get version information of the module.
    *
    * This method will return the requested version information
    *
    * @param versionId [in]: enum defining the desired version information
    * @param version [out]: string used to return the desired version information
    * @return ResultCode
    */
    virtual ResultCode getVersion(VersionId versionId, String &version) = 0; 


    /* VOICE, LANGUAGE */
    /** \brief Set the text-to-speech-voice for this instance.
    *
    * This method will set the handed text-to-speech-voice for this instance.
    * There is only one active voice allowed.
    * Setting two, one voice after another will result in replacing 
    * the first loaded voice with the second. To unload the currently 
    * active voice hand an empty string instead of a voice name.
    * This method is synchronous and returns when the new voice is loaded.
    *
    * @param voiceName [in]: name of the voice which has to be activated
    * @return ResultCode
    */
    virtual ResultCode setVoice(const SVOX_Char *voiceName) = 0; 

    /** \brief Get the text-to-speech-voice for this instance.
    *
    * This method will get the currently active text-to-speech-voice name.
    *
    * @param voiceName [out] string there the name will be written to
    * @return ResultCode
    */
    virtual ResultCode getVoice(String &voiceName)= 0; 


    /* SPEED, VOLUME, PITCH */
    /** \brief set the handed text-to-speech-param.
    *
    * This method is synchronous and will set the handed text-to-speech-parameter.
    *
    * @param ttsParamId [in]: enum defining the desired text-to-speech-param
    * @param curValue [in]: value the defined param has to be set to.
    * @see ResultCode TTSParamId
    * @return ResultCode
    */
    virtual ResultCode setParam(TTSParamId ttsParamId, SVOX_32S curValue) = 0; 

    /** \brief get the handed text-to-speech-param including lower and upper bound.
    *
    * This method is synchronous and will return the handed text-to-speech-parameter, 
    * including upper bound, lower bound and default value.
    *
    * @param ttsParamId [in]: enum defining the desired text-to-speech-param
    * @param curValue [out]: param there the desired value will be written to.
    * @param minValue [out]: param there the lower bound of the desired value will be written to.
    * @param maxValue [out]: param there the upper bound of the desired value will be written to.
    * @param defValue [out]: param there the default setting of the desired value will be written to.
    * @see ResultCode TTSParamId
    * @return ResultCode
    */
    virtual ResultCode getParam(TTSParamId ttsParamId, SVOX_32S &curValue, SVOX_32S &minValue, SVOX_32S &maxValue, SVOX_32S &defValue) = 0; 

    /** \brief get the handed text-to-speech-param.
    *
    * This method is synchronous and will return the handed text-to-speech-parameter.
    *
    * @param ttsParamId [in]: enum defining the desired text-to-speech-param
    * @param curValue [out]: param there the desired value will be written to.
    * @see ResultCode TTSParamId
    * @return ResultCode
    */
    virtual ResultCode getParam(TTSParamId ttsParamId, SVOX_32S &curValue) = 0; 

    /* FILE, TEXT, STRING */
    /** \brief play the handed audio-file.
    *
    * This method is asynchronous. 
    * It will just trigger the desired action before it returns.
    * To wait for the triggered operation call wait().
    *
    * When several play* interface calls are done in a row they are queued and processed one by one.
    * A call to the wait method will block until all requests done so far are processed.
    * The internal queue can hold 256 requests at most. 
    * If it is full the error "ERROR_BUSY" will be returned here.
    *
    * Once triggered the prompter module will open the handed file and play its contend using the 
    * connected audio device. Currently only files containing raw data (PCM 16Bit) are supported.
    *
    * @param fileName [in]: UTF8 string containing the name of the file which has to be played
    *
    * @see wait
    *
    * @return ResultCode
    */
    virtual ResultCode playAudioFile(const SVOX_Char *fileName) = 0;

    /** \brief hand the contend of the file to the TTS for playing regarding
    *         the handed context.
    * 
    * This method is asynchronous. 
    * It will just trigger the desired action before it returns.
    * To wait for the triggered operation call wait().
    *
    * When several play* interface calls are done in a row they are queued and processed one by one.
    * A call to the wait method will block until all requests done so far are processed.
    * The internal queue can hold 256 requests at most. 
    * If it is full the error "ERROR_BUSY" will be returned here.
    *
    * Once triggered the prompter module will open the handed file and pass its contend to the 
    * text_to_speech- engine, which will generate the corresponding audio-data considering the 
    * handed context. Then the audio data is played using the connected audio device. 
    *
    * The text files must be encoded in UTF (Unicode Transformation Format), 
    * either as UTF-8 or UTF-16. If the text file begins with a BOM (Byte 
    * Order Mark), the BOM information is used to determine the byte order 
    * and encoding of the file (ie. UTF-16 LE, or UTF-16 BE, or UTF-8). If the
    * text file contains character sequences that are undefined in UTF, the 
    * SVOX system automatically advances to the start of the next possible 
    * character sequence, thereby skipping the bytes causing an invalid sequence.
    * 
    * If the text file does not start with a BOM and the file name has an .utf
    * extension, the file is interpreted as an UTF-8 encoded text file and the
    * file is read and interpreted in the same way as files starting with a BOM.
    * 
    * But if the name of the text file does not have an .utf extension (e.g. 
    * .txt), and again no BOM, the file is first interpreted as an UTF-8 
    * encoded text file. Should there occur any invalid UTF character sequences 
    * in the file the interpretation is switched to a platform-dependent, 
    * single-byte encoding (Latin 1 on Windows and Unix platforms).
    *
    * @param fileName [in]: UTF8 string with the name of the file containing the text which has to be synthesized.
    * @param context [in]: UTF8 string defining the context which has to be considered. 
    *                      This string is referencing a section in the user defined 
    *                      text-preprocessing file (tpp) of the text to speech engine,
    *                      so double check for any typos.
    *
    * @see wait
    *
    * @return ResultCode
    */
    virtual ResultCode playTextFile(const SVOX_Char *fileName, const SVOX_Char *context="DEFAULT") = 0; 

    /** \brief hand the message string to the TTS for playing regarding the 
    *         handed context.
    *
    * This method is asynchronous. 
    * It will just trigger the desired action before it returns.
    * To wait for the triggered operation call wait().
    *
    * When several play* interface calls are done in a row they are queued and processed one by one.
    * A call to the wait method will block until all requests done so far are processed.
    * The internal queue can hold 256 requests at most. 
    * If it is full the error "ERROR_BUSY" will be returned here.
    *
    * Once triggered the prompter module will pass the handed message to the text_to_speech-engine, 
    * which will generate the corresponding audio-data considering the handed context. 
    * Then the audio data is played using the connected audio device. 
    *
    * @param message [in]: UTF8 string containing the message which has to be synthesized.
    * @param context [in]: UTF8 string defining the context which has to be considered. 
    *                      This string is referencing a section in the user defined 
    *                      text-preprocessing file (tpp) of the text to speech engine,
    *                      so double check for any typos.
    *
    * @see wait
    *
    * @return ResultCode
    */
    virtual ResultCode playString(const SVOX_Char *message, const SVOX_Char *context="DEFAULT") = 0;

    /** \brief hand the marker string to the TTS to be returned when the position is reached.
    *
    * This method is asynchronous. 
    * It will just trigger the desired action before it returns.
    * To wait for the triggered operation call wait().
    * 
    * The method is used to insert a position marker identified by a string in the synthesis stream.
    * The insertion of such a marker implies a word boundary at the position of the marker.
    * The function should therefore only be called before or after words in the text submitted.
    *
    * This marker is put into the queue of the play* interface calls done so far.
    * It will be returned once all the preceding requests are serviced.
    * 
    * @param message [in]: UTF8 string containing the marker returned when the desired position is reached.
    *
    * @see play*
    *
    * @return ResultCode
    */
    virtual ResultCode setPositionMarker(const SVOX_Char *marker) = 0;

    /** \brief abort playing.
    *
    * This method is synchronous and will abort a currently active output operation, which 
    * can be an active text to speech session or the playback of an audio-file.
    *
    * If several play* requests were done in a row calling this method will only abort the request 
    * currently serviced. The prompter will then resume with the next request.
    *
    * @return ResultCode
    */
    virtual ResultCode abort() = 0;

    /** \brief pause the currently running operation.
    *
    * This method is currently not implemented and will return ERROR_NOT_IMPLEMENTED if called.
    *
    * This method can be used if the application wants to pause a 
    * currently running operation.
    *
    * @return ResultCode
    */
    virtual ResultCode pause() = 0;

    /** \brief resume the currently paused operation.
    *
    * This method is currently not implemented and will return ERROR_NOT_IMPLEMENTED if called.
    *
    * This method can be used if the application wants to resume a 
    * currently paused operation.
    *
    * @return ResultCode
    */
    virtual ResultCode resume() = 0;

    /** \brief wait for a currently running operation.
    *
    * This method is synchronous and will block until all requests done so far are processed 
    * by the prompter module.
    *
    * @see playAudioFile playTextFile playString
    *
    * @return ResultCode
    */
    virtual ResultCode wait() = 0;

    /** \brief jump backward and forward from the current position
    *
    * This method is currently not implemented and will return ERROR_NOT_IMPLEMENTED if called.
    *
    * This method can be used if the application wants to jump backward 
    * and forward on a currently active but paused stream. 
    * This feature can be used for instance to rewind a paused stream before 
    * resuming it. So it is easier for the user to remember the context of 
    * the paused operation.
    * If the handed size of the jump would exceed the duration of the 
    * announcement the jump will be limited to the boundaries of the 
    * announcement, meaning a backward jump will stop at the beginning and 
    * a forward jump at the end of the announcement.
    * 
    * @param size [in]: size of the jump in ms, positive values will trigger 
    *                   a forward jump and negative values backward jump.
    * @return ResultCode
    */
    virtual ResultCode skip(SVOX_32S size) = 0;

    /** \brief get the current state.
    *
    * This method will return the state which was recently set.
    *
    * @return the last state set
    */
    virtual PState getState() = 0;


    /** \brief set the audio output interface.
     *
     * This method will hand an instance of the audio output interface to which 
     * the generated samples will be handed. 
     *
     * The output interface will be used when the next voice is loaded with setVoice.
     * If no audio interface is set the internal implementation will be used.
     *
     * ATTENTION: currently a change of the registered output adapter will only have the desired effect 
     *            when it is called prior to setVoice. This may change in the future.
     *
     * @param audioInterface [in]: instance of AudioOutputInterface
     * @return ResultCode
     */
    virtual ResultCode setAudioOutput(AudioOutputInterface *audioInterface) = 0;

    /** \brief set the event adapter.
    *
    * This method will hand an instance of the event adapter to the 
    * prompter module, which will activate the event interface.
    * from now on the methods of this module are called.
    *
    * @param eventAdapter [in]: instance of PrompterEventAdapter
    *
    * @return ResultCode
    */
    virtual ResultCode setEventAdapter(PrompterEventAdapter *eventAdapter) = 0;

protected:
    PrompterInterface(){};
    virtual ~PrompterInterface(){};

private: /* prevent copy and assignment of an instance */
    PrompterInterface(const PrompterInterface&);
    const PrompterInterface& operator=(const PrompterInterface&);
};

}} /* namespaces */

#endif /* SVOX_PROMPTER_INTERFACE_HPP */
