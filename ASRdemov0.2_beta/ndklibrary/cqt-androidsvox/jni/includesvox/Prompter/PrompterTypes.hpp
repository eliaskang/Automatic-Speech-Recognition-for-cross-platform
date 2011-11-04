/*
* @file   PrompterTypes.hpp
* @date   2010/05/12 11:35
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
/*! \brief Definition of some types used by the Prompter module
* 
* @file PrompterTypes.hpp
*
* This file defines some types needed by the svox::Prompter
* and friends.
*/

#ifndef SVOX_PROMPTER_TYPES_HPP
#define SVOX_PROMPTER_TYPES_HPP

namespace svox 
{
    /** \brief A namespace for the %prompter 
    *
    * This namespace collects all classes and symbols that are related to
    * the %prompter interface.
    */

    namespace prompter 
    {
        /*** Result type used by all Prompter classes ***********************************************************************/

        enum ResultCode
        {
            OK = 0,                   /**< operation finished with no errors */
            ERROR_NOT_INITIALIZED,    /**< the instance is not initialized */
            ERROR_ALREADY_INITIALIZED, /**< the instance is already initialized */
            ERROR_ALREADY_ACTIVE,     /**< the requested instance is already active */
            ERROR_NOT_ACTIVE,         /**< the requested instance is not active */
            ERROR_INV_PARAM,          /**< an invalid param was handed */
            ERROR_INVALID_LANGUAGE,   /**< the handed language is not available, check the spelling */
            ERROR_NO_LANGUAGE_LOADED, /**< load a language via the manager or wait until loading is finished. */
            ERROR_BUSY,               /**< the module is busy and can not service the request. */
            ERROR_NOT_IMPLEMENTED,    /**< the called method is not available yet */
            ERROR_INTERNAL,           /**< unspecific internal error, please reinitialize the module. */
            ERROR_FATAL               /**< fatal error, e.g. out-of-memory */
        };

        enum PState
        {
            INVALID = 0, /**< used internally in the constructor and in the destructor to inform the user that the instance is destroyed */
            INIT,       /**< instance was started correctly*/
            IDLE,       /**< the voice was loaded and the module is now ready */
            PROCESSING, /**< the module is currently processing a tanscripe request and will switch to IDLE when finished.*/
            PLAYING,    /**< the module is currently processing a play* request and will switch to IDLE when finished */
            ABORTING    /**< the module is aborting an active request and will switch to IDLE when finished */
        };

        enum TTSParamId
        {
            SPEED = 0,  /**< param id to reference the speed parameter */
            VOLUME,     /**< param id to reference the volume parameter */
            PITCH       /**< param id to reference the pitch parameter */
        };

        enum VersionId
        {
            PROMPTER,  /**< use to request the version of the prompter module */
            TTS_CORE   /**< use to request the version of the tts core of the prompter module */
        };

    } /* namespace prompter */
} /* namespace svox */

#endif // SVOX_PROMPTER_TYPES_HPP

/**********************************************************************************************************************/
