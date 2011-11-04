/*
 * @file   AudioInputInterface.hpp
 * @date   2010/03/10 19:17
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
/*! \brief AudioInputInterface declaration.
 * 
 * @file   AudioInputInterface.hpp
 *
 * This file contains the declaration of the AudioInputInterface.
 */
#ifndef SVOX_ASR_AUDIOINTERFACE_HPP
#define SVOX_ASR_AUDIOINTERFACE_HPP


/*** Includes *********************************************************************************************************/

#include "SVOX_Types.h"

namespace svox {
namespace asr {


/*** Class declaration ************************************************************************************************/

/** \brief Abstract audio interface.
 *
 *  This class defines an abstract audio interface.
 *  It must be implemented by the customer to pass audio data to the recognizer.
 */
class AudioInputInterface
{
public:
    virtual ~AudioInputInterface();
    
    enum ResultCode
    {
        AUDIO_OK,
        AUDIO_NO_DATA_AVAILABLE,
        AUDIO_END_OF_FILE,
        AUDIO_ERROR
    };

    enum SourceType
    {
        AUDIO_FROM_FILE,
        AUDIO_FROM_DEVICE,
        AUDIO_UNKNOWN_SOURCE
    };


    /**
     *  Returns the sample rate of the audio data.
     *
     *  @return sample rate in samples/second
     */
    virtual SVOX_32S getSampleRate() = 0;


    /**
     *  Returns the source type of the audio data.
     *
     *  @return source type
     */
    virtual SourceType getSourceType() = 0;


    /**
     *  This method is called before audio data is requested with the read() method. It can be used by the derived
     *  class to setup the audio device and audio management. An empty default implementation is provided.
     *
     *  @param preferredBlockSize the recommended number of samples which should be passed to the recognizer
     *                            each time the read() method is called
     *
     *  @return result code
     */
    virtual ResultCode startCapture(SVOX_32S preferredBlockSize);


    /**
     *  This method is called if no more audio data is needed.
     *
     *  @return result code
     */
    virtual ResultCode stopCapture();


    /**
     *  Returns a block of audio data.

     *  For speech recognition it must contain PCM data, 1 channel, 11025 or 16000 Hz, 16 bit little
     *  endian.  The method should return \a preferredBlockSize samples, this parameter is passed to the
     *  startCapture() method.  The method must return samples, it is expected to block (e.g. on a
     *  semaphore) until it can deliver at least one sample, thus passing a NULL pointer is also not
     *  allowed. Except the return value is AUDIO_END_OF_FILE, then buffer may be NULL if and only if
     *  numSamples is also 0.
     *    
     *
     *  @param buffer [out]      returns a pointer to a audio data block
     *  @param numSamples [out]  returns the number of samples in the block
     *
     *  @return result code
     */
    virtual ResultCode read(SVOX_16S *&buffer, SVOX_32S &numSamples) = 0;


    /**
     *  After a block of audio data was read with the read() method, it must be released with this method.
     *
     *  @param buffer [in]      the audio data to release
     *  @param numSamples [in]  number of samples in the block; must be same number as got from read()
     *
     *  @return result code
     */
    virtual ResultCode releaseBuffer(SVOX_16S *buffer, SVOX_32S numSamples) = 0;

};


}} // namespaces


#endif // SVOX_ASR_AUDIOINTERFACE_HPP

/**********************************************************************************************************************/
