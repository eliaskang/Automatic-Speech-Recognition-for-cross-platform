/*
 * @file   Grammar.hpp
 * @date   2009/11/27 8:35
 * @author dirk.buehler, joachim.grill
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
/*! @brief Grammar class declaration.
 * 
 * @file   Grammar.hpp
 *
 * This file contains the declaration of the Grammar class.
 *
 */
#ifndef SVOX_ASR_GRAMMAR_HPP
#define SVOX_ASR_GRAMMAR_HPP


/*** Includes *********************************************************************************************************/

#include "SVOX_Types.h"
#include "RecognizerTypes.hpp"


/*** Forward declarations *********************************************************************************************/

namespace svox {
    namespace climax {
        class ComplexListManager;
    }
}


/*** Class declaration ************************************************************************************************/


namespace svox {
namespace asr {

/** @brief A class to hold grammar for the recognizer
 * 
 *  Grammars are used to define "language models", i.e. specifications
 *  of what can be spoken by the user at a given time. This class is
 *  used to load, store and modify grammars to use with the
 *  svox::asr::Recognizer class.
 *  
 */
class Grammar
{
public:

    /** @brief This enumeration is used for distinguishing different types of grammars. **/
    enum GrammarType
    {
        NORMAL,               //!< normal grammar for command and control

		ISOLATED,             //!< recognition on vocabulary (without grammar)

        OSD,                  //!< grammar for one-shot destination input
        OSD_SUBLEVEL,         //!< grammar for one-shot destination input starting at a given level,
                              //!< e.g. street-city recognition for a pre-selected state (en-US)

        OSD_SINGLE_TOPLEVEL,  //!< grammar for single-step recognition using an OSD resource (toplevel tag, e.g. state)
        OSD_SINGLE_SUBLEVEL   //!< grammar for single-step recognition using an OSD resource (sub tag, e.g. street)
    };


    /**
     *  Activate a grammar rule. If this grammar is used for recognition, only the set of active rules is used.
     *  For a newly loaded grammar all rules are de-activated.
     *
     *  @param rulename   the name of the grammar rule to activate
     */
    virtual ResultCode activateRule(const SVOX_Char *rulename) = 0;

    /**
     *  Activate all rules of a grammar.
     */
    virtual ResultCode activateAllRules() = 0;


    /**
     *  De-activate a grammar rule.
     *
     *  @param rulename   the name of the grammar rule to de-activate
     */
    virtual ResultCode deactivateRule(const SVOX_Char *rulename) = 0;

    /**
     *  De-activate all rules of a grammar.
     */
    virtual ResultCode deactivateAllRules() = 0;

    /**
     *  Register a list manager column for the passed grammar class name.
     *
     *  @param className    the name of the grammar class in which dynamic data shall be inserted by the list manager
     *  @param listManager  the list manager instance which provides the dynamic data
     *  @param columnId     the column of the list manager dataset which should be filled in the specified class
     */
    virtual ResultCode registerListManager( const SVOX_Char *className, 
                                            svox::climax::ComplexListManager *listManager, 
                                            SVOX_32S columnId) = 0;

    /**
     *  Returns the name of the grammar.
     *
     *  @return the grammar name
     */
    virtual const SVOX_Char *getName() const = 0;

    /**
     *  Returns a detailed error message about the error of the last grammar operation.
     *
     *  @return error message string
     */
    virtual const SVOX_Char *getLastErrorDetails() = 0;

protected:
    // grammars shall be created / destroyed by the Recognizer class
    Grammar() {}
    ~Grammar() {}
};


} } // namespaces

#endif // SVOX_ASR_GRAMMAR_HPP

/**********************************************************************************************************************/
