/*
 * @file   RecognizerTypes.hpp
 * @date   2010/04/07 11:35
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
/*! \brief Definition of some types used by the Recognizer module
 *
 * @file RecognizerTypes.hpp
 *
 * This file defines some types needed by the svox::asr::Recognizer
 * and friends.
 */

#ifndef SVOX_ASR_RECOGNIZER_TYPES_HPP
#define SVOX_ASR_RECOGNIZER_TYPES_HPP

// for the following symbols macros are defined in windows.h; if the customer includes windows.h before including
// the ISS headers we get conflicts, as the preprocessor does not respect namespaces
#undef ERROR_FILE_NOT_FOUND

namespace svox {
namespace asr {


/*** Result type used by all Recognizer classes ***********************************************************************/

enum ResultCode
{
    OK,                            //!< method was successful

    ERROR_OUT_OF_MEMORY,           //!< recognizer run out of memory
    ERROR_FILE_NOT_FOUND,          //!< a file was not found in the filesystem
    ERROR_ID_NOT_FOUND,            //!< a rulename, class name, columnId was not found

    ERROR_STATE_VIOLATION,         //!< initialize() was called on an already initialized Recognizer,
                                   //!< or a method was called on a not yet initialized Recognizer,
                                   //!< or a method call failed because of a active recognition

    ERROR_PARAMETER,               //!< an invalid parameter was passed

    ERROR_AUDIOINTERFACE,          //!< reading data from the audio interface failed
    ERROR_AUDIOFORMAT,             //!< sample rate of the audio device is not supported by the recognizer

    ERROR_GRAMMAR_ALREADY_LOADED,  //!< tried to load an already loaded grammar a second time
    ERROR_GRAMMAR_EMPTY,           //!< recognition was started with no active grammar / grammar rules

    ERROR_VSR,                     //!< an error of the VSR engine occurred,
                                   //!< call getLastErrorDetails() for further details

    ERROR_CLX,                     //!< an error of the list manager occurred

    ERROR_INTERNAL                 //!< internal error occurred
};



}} // namespaces

#endif // SVOX_ASR_RECOGNIZER_TYPES_HPP

/**********************************************************************************************************************/
