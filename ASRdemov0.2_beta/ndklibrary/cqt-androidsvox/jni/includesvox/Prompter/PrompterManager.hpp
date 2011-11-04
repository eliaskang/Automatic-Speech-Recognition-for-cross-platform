/*
* @file   PrompterManager.hpp
* @date   2009/10/14 10:21
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
/*! \brief prompter manager interface definition.
* 
* @file   PrompterManager.hpp
*
* This file contains the definition of the prompter manager module
*/
#ifndef SVOX_PROMPTER_MANAGER_HPP
#define SVOX_PROMPTER_MANAGER_HPP

/* pal */
#include "SVOX_Types.h"

/* common */
#include "String.hpp"
#include "StringList.hpp"

/* interface types */
#include "PrompterTypes.hpp"

using namespace svox::common;

namespace svox 
{
    namespace prompter 
    {
        class PrompterInterface;

        /*! \brief interface of the prompter-module manager.
        *
        * This module is responsible for the several instances of the SVOX 
        * prompter module.
        */
        class PrompterManager
        {
        public:

            /*! \brief create a named prompter instance.
            *
            * This method will create a named prompter instance. The handed name 
            * has to be unique in the system since it is also used to identify and register the 
            * audio-source of the prompter instance at the audio-manager.
            * the registration itself is done internally when the instance is created.
            * Calling this method twice using the same name will result in an an error
            * since the name is already assigned to an active prompter instance.
            * If an instance was created it has to be destroyed with a call of 
            * destroyPrompterInstance.
            *
            * @param instName [in] unique name for the prompter instance
            * @param promIfHandle [out] handle of the instance which was created 
            *                             for the handed name
            * @see destroyPrompterInstance
            * @return ResultCode
            */
            virtual ResultCode createPrompterInstance(const SVOX_Char *instName, PrompterInterface *&promIfHandle ) = 0;

            /*! \brief destroy a named prompter instance.
            *
            * This method will destroy the handed prompter instance after deregistering it at the audio manager.
            * After calling this method the handle is invalid. The handle has to be created before 
            * with a call of createPrompterInstance.
            *
            * @param promIfHandle [in] handle of the instance which has to be destroyed
            * @see createPrompterInstance
            * @return ResultCode
            */
            virtual ResultCode destroyPrompterInstance(PrompterInterface *&promIfHandle ) = 0;


            /*! \brief get the language-names which can be set.
            *
            * This method is currently not implemented and will return ERROR_NOT_IMPLEMENTED if called.

            * This method will return all language-names for the current 
            * configuration which can be activated.
            *
            * @param languageNames [out]
            * @return ResultCode
            */
            virtual ResultCode getLanguages(StringList &languageNames) = 0;

            /*! \brief get the voices which can be set.
            *
            * This method is currently not implemented and will return ERROR_NOT_IMPLEMENTED if called.
            *
            * This method will return all voices which can be set.
            *
            * @param voiceNames [out]
            * @return ResultCode
            */
            virtual ResultCode getVoices(StringList &voiceNames) = 0;

            /*! \brief get the voices which can be set for a language.
            *
            * This method is currently not implemented and will return ERROR_NOT_IMPLEMENTED if called.
            *
            * This method will return all voices for the handed language which can 
            * be set.
            *
            * @param languageName [in] name of the language for which the voices 
            *                          are requested
            * @param voiceNames [out]
            * @return ResultCode
            */
            virtual ResultCode getVoices(const SVOX_Char *languageName, StringList &voiceNames) = 0;

            /*! \brief get detailed voice info.
            *
            * This method is currently not implemented and will return ERROR_NOT_IMPLEMENTED if called.
            *
            * This method will return detailed information to the handed voice.
            *
            * @param voiceName [in]: string containing the name of the voice 
            *                        information is requested for
            * @param language [out]: language of the handed voice
            * @param gender [out]: gender of the handed voice
            * @param age [out]: age of the handed voice
            * @param samplerate [out]: samplerate of the handed voice
            * @see getVoices
            * @return ResultCode
            */
            virtual ResultCode getVoiceInfo(const SVOX_Char *voiceName, String &language, String &gender, String &age, SVOX_32S &samplerate) = 0;



            /*! \brief init the prompter manager instance.
            *
            * Init the prompter manager. This method has to be called before 
            * invoking any interface method except getInstance();
            * Calling this method twice in a row will not change the path handed 
            * with the first call. The second call will return ERROR_ALREADY_INITIALIZED.
            *
            * @param configFilePath [in]: path to the configuration file
            * @see getInstance
            * @return ResultCode
            */
            virtual ResultCode initialize(const SVOX_Char *configFilePath) = 0;


            /*! \brief get the current instance.
            *
            * This method will return the instance of the prompter manager. If the 
            * instance does not exist it will be created.
            * To use the prompter manager it has to be initializes by calling 
            * initialize, otherwise calling an interface method will return an error
            * @see initialize
            * @return PrompterManager*
            */
            static PrompterManager *getInstance();

            /*! \brief destroy the current prompter manager instance.
            *
            * This method will destroy the prompter manager instance.
            * To use the manager again call getInstance and initialize.
            * @see getInstance initialize
            */
            static void destroyInstance();

        protected: 
            PrompterManager(){};
            virtual ~PrompterManager(){};

        private: /* prevent copy and assignment of an instance */
            PrompterManager(const PrompterManager&);
            const PrompterManager& operator=(const PrompterManager&);
        };

    } /* namespace prompter */
} /* namespace svox */

#endif /* SVOX_PROMPTER_MANAGER_HPP */
