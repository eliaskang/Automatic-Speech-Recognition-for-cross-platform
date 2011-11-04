/*
 * @file   RecognizerCallback.hpp
 * @date   2010/03/10 19:33
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
/*! \brief Recogizer callback interface declaration.
 * 
 * @file   RecognizerCallback.hpp
 *
 * This file contains the declaration of the abstract RecognizerCallback
 * interface. That must be implemented by the user to receive
 * callbacks from the recoignizer.
 */
#ifndef SVOX_ASR_RECOGNIZERCALLBACK_HPP
#define SVOX_ASR_RECOGNIZERCALLBACK_HPP


/*** Includes *********************************************************************************************************/

#include "SVOX_Types.h"
#include "RecognitionResult.hpp"


namespace svox
{
namespace asr
{


/*** Class definition *************************************************************************************************/

/** \brief Abstract interface to derive customer callback classes from.
 *
 *  This class declares an interface which can be implemented by the customer to receive information
 *  from the recognizer. For all methods an empty default implementation is provided, the customer must
 *  only implement the methods he is interested in.
 *  Remark: most callback methods are called from the Recognizer-internal worker thread; they must not block
 *  and should return as fast as possible.
 */
class RecognizerCallback
{
public:

    virtual ~RecognizerCallback();
    

    enum AudioQuality
    {
        AUDIO_OK,
        AUDIO_TOO_LOUD,
        AUDIO_TOO_SOFT,
        AUDIO_CLIPPED,
        AUDIO_OVERDRIVED
    };

    enum RecogEvent
    {
        RECOGNITION_START,
        RECOGNITION_SPEECH_DETECTED,
        RECOGNITION_STOP,
        RECOGNITION_ABORT,
        RECOGNITION_INITIAL_TIMEOUT,
        RECOGNITION_UTTERANCE_TIMEOUT,
        RECOGNITION_REJECTED
    };


    /**
     *  This method is called as soon as the recognition is completed.   
     *
     *  @param result  the recognition result; in the case of a rejection the result is empty
     */
    virtual void onRecogResult(RecognitionResult *result);


    /**
     *  This method is called if a recognizer event occurs.
     *
     *  @param event the event
     */
    virtual void onRecogEvent(RecogEvent event);


    /**
     *  If audio capture is enabled in the Recognizer class, this method is called for each audio block which is
     *  processed by the recognizer.
     *
     *  @param audioData    the audio data; always 11025 Hz
     *  @param numSamples   size of the audio data block
     */
    virtual void onAudioData(SVOX_16S *audioData, SVOX_32U numSamples);


    /**
     *  This method is called for each processed frame (15ms) with an audio level value.
     *
     *  @param audioLevel  an audio level value, in the current implementation between 1 .. 32535
     */
    virtual void onAudioLevel(SVOX_32U audioLevel);


    /**
     *  If the audio signal quality is bad, this method is called.
     *
     *  @param quality  the audio quality
     */
    virtual void onAudioQuality(AudioQuality quality);

};


}} // namespaces

#endif // SVOX_ASR_RECOGNIZERCALLBACK_HPP

/**********************************************************************************************************************/
